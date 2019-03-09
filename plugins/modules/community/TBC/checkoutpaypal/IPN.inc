<?php

/*
 * $RCSfile: IPN.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2004 Bharat Mediratta
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
 *
 * @version
 * @package CheckoutPaypal
 * @subpackage UserInterface
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * Required classes
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/GalleryCheckoutApi.class');

/**
 * This controller will handle the postback for PayPal IPN
 *
 * @package Checkout
 * @subpackage UserInterface
 *
 */
class IPNView extends GalleryView {
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

		GalleryCoreApi::requireOnce('modules/checkoutpaypal/classes/GalleryCheckoutPaypalIPNHelper.class');

		list($ret, $ppurl) = GalleryCoreApi::getPluginParameter('module', 'checkoutpaypal', 'ppurl');

		if ($ret) {
			return $ret;
		}

		$ppurl             = 'http://' . $ppurl . '/cgi-bin/webscr';
		$IPNVerified       = false;
		$IPNAmountVerified = false;
		$IPNReason         = array();
		$notes             = '';

		// Post $_POST back to PayPal system to validate
		$_POST['cmd'] = '_notify-validate';

		list($responseBody, $responseStatus, $responseHeaders) = galleryCoreApi::postToWebPage($ppurl, $_POST);

		if (preg_match('/^HTTP\/\d+\.\d+\s2\d{2}/', $responseStatus)) {
			if (strcmp($responseBody, 'VERIFIED') == 0) {
				$IPNVerified = true;
			} else {
				echo 'Invalid';
				return null;
			}
		} else {
			echo 'Unable to Connect';
			return null;
		}

		if ($IPNVerified) {
			$IPNpost = GalleryUtilities::getRequestVariablesNoPrefix(
				'invoice',
				'txn_id',
				'payment_status',
				'payment_date',
				'verify_sign',
				'first_name',
				'last_name',
				'payer_email',
				'mc_gross',
				'address_name',
				'address_street',
				'address_city',
				'address_state',
				'address_zip',
				'address_country',
				'memo',
				'business',
				'contact_phone'
			);

			// Retrieve the CheckoutTransaction for this IPN
			list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($IPNpost[0]);

			if ($ret) {
				return $ret;
			}

			if (!isset($transaction)) {
				return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
			}

			// TO DO: check that txn_id has not been previously processed
			// check that receiver_email is your Primary PayPal email
			list($ret, $paypalname) = GalleryCoreApi::getPluginParameter('module', 'checkoutpaypal', 'paypalname');

			if ($ret) {
				return $ret;
			}

			if ($IPNpost[16] != $paypalname) {
				$IPNVerified = false;
				$IPNReason[] = array(
					'domain' => 'checkoutpaypal',
					'data'   => array(
						'text' => $gallery->i18n('WARNING: Transaction business name "%s" does not match site business name "%s".'),
						'arg1' => $IPNpost[16],
						'arg2' => $paypalname,
					),
				);

				$IPNReason[] = "\n";
				$notes      .= 'Wrong Business Name: ' . $IPNpost[16] . ' and ours is ' . $paypalname;

				// check that payment_amount/payment_currency (TO DO) are correct
				if (abs((float)str_replace(',', '', $transaction->getamount())) != abs((float)$IPNpost[8])) {
					$IPNAmountVerified = false;
					$IPNReason[]       = array(
						'domain' => 'checkoutpaypal',
						'data'   => array(
							'text' => $gallery->i18n('WARNING: Paypal amount %d does not match our records %d.'),
							'arg1' => $IPNpost[8],
							'arg2' => $transaction->getamount(),
						),
					);

					$IPNReason[] = "\n";
					$notes      .= 'Wrong Amount: ' . $IPNpost[8] . ' and our value is ' . $transaction->getamount();
				} else {
					$IPNAmountVerified = true;
				}
			}

			if ($IPNVerified) {
				// Count the existing Paypal child transactions
				list($ret, $ppTransactions) = GalleryCheckoutPaypalIPNHelper::fetchPpTxnCounts($transaction->getId());

				if ($ret) {
					return $ret;
				}

				// Has this transaction already been posted back by PayPal? If not, create a record
				if (!isset($ppTransactions[$transaction->getId()])) {
					/* at the moment that condition always succeeds because the GalleryCheckoutPaypalIPN entity
					has the wrong parent type.
					*/

					// Set Payment Plugin for this transaction
					$ret = GalleryCheckoutApi::setPaymentPlugin($transaction, 'checkoutpaypal');

					if ($ret) {
						return $ret;
					}

					// Set the IPN information into the original transaction
					$data['custFirstName']     = $IPNpost[5];
					$data['custLastName']      = $IPNpost[6];
					$data['custEmail']         = $IPNpost[7];
					$data['custRecipientName'] = $IPNpost[9];
					$data['custAddress1']      = $IPNpost[10];
					$data['custAddress2']      = $IPNpost[11];
					$data['custAddress3']      = $IPNpost[12];
					$data['custAddress4']      = '';
					$data['custAddress5']      = '';
					$data['custZip']           = $IPNpost[13];
					$data['custCountry']       = $IPNpost[14];
					$data['custComments']      = 'PayPal notes: ' . $IPNpost[15];
					$data['telephone']         = $IPNpost[17];

					$ret = GalleryCheckoutApi::transactionUpdate($transaction, $data);

					if ($ret) {
						return $ret;
					}

					list($ret, $transaction) = $transaction->refresh();

					if ($ret) {
						return $ret;
					}
				}

				// Check the payment_status
				switch ($IPNpost[2]) {
					case 'Completed':
						$ret = GalleryCheckoutApi::changeTransactionStatus($transaction, 'clear');

						if ($ret) {
							return $ret;
						}

						$IPNReason[] = array(
							'domain' => 'checkoutpaypal',
							'data'   => $gallery->i18n('PayPal payment is Completed. The PayPal payment has been accepted.'),
						);

						$IPNReason[] = "\n";
						$notes      .= ' Accepted. ';
						break;
					case 'Pending':
					case 'Refunded':
					case 'Denied':
						/*
						case 'Canceled-Reversal':
						case 'Expired':
						case 'Failed':
						case 'In-Progress':
						case 'Partially-Refunded':
						case 'Processed':
						*/
					default:
						$IPNReason[] = array(
							'domain' => 'checkoutpaypal',
							'data'   => $gallery->i18n('Unhandled Paypal State:'),
						);

						$IPNReason[] = "\n";
						$IPNReason[] = $IPNpost[2];
						$IPNReason[] = "\n";
						$notes      .= 'Unhandled: ' . $IPNpost[2];
				}
			}

			/* Post an administrative notification
			so the admin can take up events not handled automatically
			*/
			$ret = GalleryCheckoutApi::SendAdminNotification($transaction, $IPNReason);

			if ($ret) {
				return $ret;
			}

			// Add a new Paypal transaction for this IPN
			list($ret, $ppTransaction) = GalleryCoreApi::newFactoryInstance('GalleryEntity', 'GalleryCheckoutPaypalIPN');

			if ($ret) {
				return $ret;
			}

			if (!isset($ppTransaction)) {
				return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
			}

			$ret = $ppTransaction->create($transaction->getId());

			if ($ret) {
				return $ret;
			}

			$ppTransaction->setppId($IPNpost[1]);
			$ppTransaction->setppStatus($IPNpost[2]);
			$ppTransaction->setppDate($IPNpost[3]);
			$ppTransaction->setppSign($IPNpost[4]);

			$notes .= time() . ' PayPal IPN postback received';
			$notes .= ' - PayPal notes: ' . $IPNpost[15];

			$ppTransaction->setnotes($notes);
			$ppTransaction->save();

			return null;
		}
	}
}
