<?php

/*
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/**
 * Required classes
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/GalleryCheckoutApi.class');
GalleryCoreApi::requireOnce('modules/checkoutgoogle/classes/CheckoutGoogleResponseHelper.class');

/**
 * @package CheckoutGoogle
 * @subpackage UserInterface
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1642 $
 */
class ResponseView extends GalleryView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	/**
	 * @see GalleryView::renderImmedate
	 */
	public function renderImmediate($status, $error) {
		global $gallery;

		$urlGenerator =& $gallery->getUrlGenerator();

		// Verify Authentification
		if (!CheckoutGoogleResponseHelper::CheckAuthentication()) {
			return GalleryCoreApi::error(ERROR_PERMISSION_DENIED);
		}

		// Retrieve the XML sent in the HTTP POST request to the ResponseHandler
		$xmlResponse       = isset($HTTP_RAW_POST_DATA) ? $HTTP_RAW_POST_DATA : file_get_contents('php://input');
		list($root, $data) = CheckoutGoogleResponseHelper::GetParsedXML($xmlResponse);

		// Record notification
		$googleOrderNumber = $data[$root]['google-order-number']['VALUE'];

		$ret = GalleryCoreApi::addMapEntry(
			'CheckoutGoogleResponseMap',
			array(
				'googleOrderNumber' => $googleOrderNumber,
				'response'          => $xmlResponse,
			)
		);

		if ($ret) {
			return $ret;
		}

		// Take appropriate action
		switch ($root) {
			case 'gallery-test':
				// Local reachability test
				if (!headers_sent()) {
					header('Content-type: text/plain; charset=UTF-8');
				}

				echo 'OK';

				break;

			case 'new-order-notification':
				// Figure out which URL the order came from and reset the URL generator
				$mainURL = $data[$root]['shopping-cart']['merchant-private-data']['mainURL']['VALUE'];

				$urlGenerator->init($baseUri = $mainURL);

				// Figure out which order
				$transactionId = $data[$root]['shopping-cart']['merchant-private-data']['transactionId']['VALUE'];

				list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($transactionId, 'GalleryCheckoutTransaction');

				if ($ret) {
					return $ret;
				}

				if (!isset($transaction)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				// Record Google order Number + URL
				$ret = GalleryCoreApi::addMapEntry(
					'CheckoutGoogleOrderMap',
					array(
						'transactionId'     => $transactionId,
						'googleOrderNumber' => $googleOrderNumber,
						'mainURL'           => $mainURL,
					)
				);

				if ($ret) {
					return $ret;
				}

				// Set Payment Plugin for this transaction
				$ret = GalleryCheckoutApi::setPaymentPlugin($transaction, 'checkoutgoogle');

				if ($ret) {
					return $ret;
				}

				// Store Customer Details
				$custDetails['custFirstName']     = $data[$root]['buyer-billing-address']['structured-name']['first-name']['VALUE'];
				$custDetails['custLastName']      = $data[$root]['buyer-billing-address']['structured-name']['last-name']['VALUE'];
				$custDetails['custRecipientName'] = $data[$root]['buyer-shipping-address']['contact-name']['VALUE'];
				$custDetails['telephone']         = $data[$root]['buyer-billing-address']['phone']['VALUE'];
				$custDetails['custAddress5']      = $data[$root]['buyer-billing-address']['region']['VALUE'];
				$custDetails['custAddress4']      = $data[$root]['buyer-billing-address']['city']['VALUE'];
				$custDetails['custAddress2']      = $data[$root]['buyer-billing-address']['address2']['VALUE'];
				$custDetails['custAddress1']      = $data[$root]['buyer-billing-address']['address1']['VALUE'];
				$custDetails['custZip']           = $data[$root]['buyer-billing-address']['postal-code']['VALUE'];
				$custDetails['custCountry']       = $data[$root]['buyer-billing-address']['country-code']['VALUE'];
				$custDetails['custEmail']         = $data[$root]['buyer-billing-address']['email']['VALUE'];
				$shipToAddress                    = 'Delivery Address:' . "\n" .
				$data[$root]['buyer-shipping-address']['address1']['VALUE'] . ', ' .
				$data[$root]['buyer-shipping-address']['address2']['VALUE'] . ', ' .
				$data[$root]['buyer-shipping-address']['city']['VALUE'] . ', ' .
				$data[$root]['buyer-shipping-address']['region']['VALUE'] . ', ' .
				$data[$root]['buyer-shipping-address']['postal-code']['VALUE'] . ', ' .
				$data[$root]['buyer-shipping-address']['country-code']['VALUE'];
				$custDetails['custNotes']         = 'Google Order Number:' . $googleOrderNumber . "  \n";
				$custDetails['custNotes']        .= $shipToAddress;

				$ret = GalleryCheckoutApi::transactionUpdate($transaction, $custDetails);

				if ($ret) {
					return $ret;
				}

				list($ret, $transaction) = $transaction->refresh();

				if ($ret) {
					return $ret;
				}

				// Tell Google our order number?
				list($ret, $result) = CheckoutGoogleResponseHelper::SendTransactionId($transactionId);

				if ($ret) {
					return array($ret, null);
				}

				// Tell admin
				$statusMessage   = array();
				$statusMessage[] = array(
					'domain' => 'modules_checkoutgoogle',
					'data'   => $gallery->i18n('Google order notification received.'),
				);

				break;

			case 'charge-amount-notification':
				// Find transactionId and load transaction
				$googleOrderNumber = $data[$root]['google-order-number']['VALUE'];

				list($ret, $transactionId, $mainURL) = CheckoutGoogleResponseHelper::fetchTransactionId($googleOrderNumber);

				if ($ret) {
					return $ret;
				}

				$urlGenerator->init($baseUri = $mainURL);

				list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($transactionId, 'GalleryCheckoutTransaction');

				if ($ret) {
					return $ret;
				}

				if (!isset($transaction)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				// Check to see if the whole amount was received
				if ($data[$root]['total-charge-amount']['VALUE'] == $transaction->getAmount()) {
					$ret = GalleryCheckoutApi::changeTransactionStatus($transaction, 'clear');

					if ($ret) {
						return $ret;
					}

					$statusMessage   = array();
					$statusMessage[] = array(
						'domain' => 'modules_checkoutgoogle',
						'data'   => $gallery->i18n('Google Checkout payment is completed. The Google Checkout payment has been accepted.'),
					);
				} else {
					$statusMessage   = array();
					$statusMessage[] = array(
						'domain' => 'modules_checkoutgoogle',
						'data'   => $gallery->i18n('Google Checkout payment state has changed.'),
					);
					$statusMessage[] = "\n";
					$statusMessage[] = array(
						'domain' => 'modules_checkoutgoogle',
						'data'   => array(
							'text' => $gallery->i18n('Order total is %d'),
							'arg1' => $transaction->getAmount(),
						),
					);
					$statusMessage[] = "\n";
					$statusMessage[] = array(
						'domain' => 'modules_checkoutgoogle',
						'data'   => array(
							'text' => $gallery->i18n('Google payment so far is %d'),
							'arg1' => $data[$root]['total-charge-amount']['VALUE'],
						),
					);
				}

				break;

			// Not very interesting
			// All logged, but no need to notify
			case 'risk-information-notification':
			case 'order-state-change-notification':
				break;

			// More interesting: Alert admin
			case 'request-received':
			case 'error':
			case 'diagnosis':
			case 'checkout-redirect':
			case 'chargeback-amount-notification':
			case 'refund-amount-notification':
			default:
				// Invalid or unhandled message
				 // Find transactionId and load transaction
				$googleOrderNumber = $data[$root]['google-order-number']['VALUE'];

				list($ret, $transactionId, $mainURL) = CheckoutGoogleResponseHelper::fetchTransactionId($googleOrderNumber);

				if ($ret) {
					return $ret;
				}

				$urlGenerator->init($baseUri = $mainURL);

				list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($transactionId, 'GalleryCheckoutTransaction');

				if ($ret) {
					return $ret;
				}

				if (!isset($transaction)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				$statusMessage   = array();
				$statusMessage[] = array(
					'domain' => 'modules_checkoutgoogle',
					'data'   => array(
						'text' => $gallery->i18n('Google Notification Recieved: %s'),
						'arg1' => $root,
					),
				);
				$statusMessage[] = "\n\n";
				$statusMessage[] = array(
					'domain' => 'modules_checkoutgoogle',
					'data'   => $gallery->i18n('The full data received is:'),
				);

				$statusMessage[]     = "\n";
					$statusMessage[] = $xmlResponse;
		}

		// Post an administrative notification event so the admin can take up events not handled automatically
		if (isset($statusMessage)) {
			$ret = GalleryCheckoutApi::SendAdminNotification($transaction, $statusMessage);

			if ($ret) {
				return $ret;
			}
		}

		return null;
	}
}
