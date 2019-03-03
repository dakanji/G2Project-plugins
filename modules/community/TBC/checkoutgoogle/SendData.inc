<?php
/*
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
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
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
 * @version $Revision: 1912 $
 */
class SendDataController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$error        = array();
		$status       = array();
		$urlGenerator =& $gallery->getUrlGenerator();

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkoutgoogle');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $transaction) = GalleryCheckoutApi::getCurrentTransactionForPayment();

		if ($ret) {
			return array($ret, null);
		}

		$transactionId = $transaction->getId();
		$verify1       = $transaction->getDate();
		$verify2       = $transaction->getSecurity();

		// see if this transaction has a payment plugin already
		if ($transaction->getPaymentPlugin() != '') {
			// order already has a payment plugin
			$results['redirect'] = array(
				'controller'    => 'checkout.Complete',
				'transactionId' => $transactionId,
				'verify1'       => $verify1,
				'verify2'       => $verify2,
				'error'         => 'alreadyPlaced',
			);

			$results['error']  = $error;
			$results['status'] = $status;

			return array(null, $results);
		}

		list($ret, $currency) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'gcCurrency');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $items) = GalleryCheckoutApi::fetchFullTransactionItems($transactionId);

		if ($ret) {
			return array($ret, null);
		}

		$discount = $transaction->getDiscount();
		$postage  = $transaction->getPostage();

		// Build URLs for altering order, finishing
		$editURL = $urlGenerator->generateUrl(
			array(
				'view' => 'checkout.Confirm',
			),
			array(
				'forceFullUrl' => true,
			)
		);

		$continueURL = $urlGenerator->generateUrl(
			array(
				'controller'    => 'checkout.Complete',
				'transactionId' => $transactionId,
				'verify1'       => $verify1,
				'verify2'       => $verify2,
			),
			array(
				'forceFullUrl' => true,
			)
		);

		$mainURL = $urlGenerator->generateUrl(
			array(),
			array(
				'forceFullUrl' => true,
			)
		);

		// Start
		$googleData  = '	<?xml version="1.0" encoding="UTF-8"?>
			<checkout-shopping-cart xmlns="http://checkout.google.com/schema/2">
			<shopping-cart>';
		$googleData .= '<merchant-private-data>
		 	<transactionId>' . $transactionId . '</transactionId>
		 	<mainURL>' . $mainURL . '</mainURL>
		 	</merchant-private-data>';
		$googleData .= '<items>';

		foreach ($items as $item) {
			$name        = htmlspecialchars($item['itemName'], ENT_NOQUOTES);
			$description = htmlspecialchars($item['productName'] . $item['notes'], ENT_NOQUOTES);
			$quantity    = $item['qty'];
			$amount      = $item['priceEach'];
			$googleData .= '<item>
        		    <item-name>' . $name . '</item-name>
        		    <item-description>' . $description . '</item-description>
        		    <unit-price currency="' . $currency . '">' . $amount . '</unit-price>
        		    <quantity>' . $quantity . '</quantity>
			    </item>';
		}

		// Discount
		if ($discount > 0) {
			$discountName        = htmlspecialchars($module->translate('Discount'), ENT_NOQUOTES);
			$discountDescription = htmlspecialchars($transaction->getDiscountDescription(), ENT_NOQUOTES);
			$googleData         .= '<item>
        		    <item-name>' . $discountName . '</item-name>
        		    <item-description>' . $discountDescription . '</item-description>
        		    <unit-price currency="' . $currency . '">' . - $discount . '</unit-price>
        		    <quantity>1</quantity>
			    </item>';
		}

		$googleData .= '</items>
      		     	</shopping-cart>';
		$googleData .= '<checkout-flow-support>
    			<merchant-checkout-flow-support>';
		// Postage
		if ($postage > 0) {
			$postageType = htmlspecialchars($transaction->getPostageType(), ENT_NOQUOTES);
			$googleData .= '<shipping-methods>
        		    <flat-rate-shipping name="' . $postageType . '">
          		    <price currency="' . $currency . '">' . $postage . '</price>
          		    <shipping-restrictions>
          		    <allowed-areas><world-area/></allowed-areas>
          		    </shipping-restrictions>
        		    </flat-rate-shipping>
      			    </shipping-methods>';
		}

		$googleData .= '<edit-cart-url>' . $editURL . '</edit-cart-url>
			<continue-shopping-url>' . $continueURL . '</continue-shopping-url>
    			</merchant-checkout-flow-support>
    			</checkout-flow-support>';
		$googleData .= '</checkout-shopping-cart>';

		list($success, $body, $url, $returnStatus) = CheckoutGoogleResponseHelper::SendData($googleData);

		if ($success) {
			$matches          = array();
			$success          = preg_match('/<redirect-url>(.*)<\/redirect-url>/', $body, $matches);
			$redirect['href'] = $matches[1];
		}

		if (!$success) {
			// Send admin email about the problem
			$statusMessage   = array();
			$statusMessage[] = array(
				'domain' => 'modules_checkoutgoogle',
				'data'   => $gallery->i18n('GoogleCheckout: there was a problem preparing an order when a user tried to use Google Checkout.'),
			);

			$statusMessage[] = "\n\n";
			$statusMessage[] = array(
				'domain' => 'modules_checkoutgoogle',
				'data'   => $gallery->i18n('We contacted Google at this url:'),
			);

			$statusMessage[] = "\n" . $url . "\n\n";
			$statusMessage[] = array(
				'domain' => 'modules_checkoutgoogle',
				'data'   => $gallery->i18n('The content of our message was:'),
			);

			$statusMessage[] = "\n\n" . $googleData . "\n\n";
			$statusMessage[] = array(
				'domain' => 'modules_checkoutgoogle',
				'data'   => $gallery->i18n('The response code was:'),
			);

			$statusMessage[] = "\n" . $url . $returnStatus . "\n\n";
			$statusMessage[] = array(
				'domain' => 'modules_checkoutgoogle',
				'data'   => $gallery->i18n('The Google Website returned the following response in the body:'),
			);

			$statusMessage[] = "\n\n";
			$statusMessage[] = $body;

			$ret = GalleryCheckoutApi::SendAdminNotification($transaction, $statusMessage);

			if ($ret) {
				return array($ret, null);
			}

			$error[] = 'form[error][checkoutgoogle][redirectFailed]';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view'] = 'checkout.Confirm';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}
