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
GalleryCoreApi::requireOnce('modules/checkout/classes/GalleryCheckoutApi.class');

/**
 *
 * @package CheckoutEmail
 * @subpackage UserInterface
 * @version $Revision: 1584 $
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 *
 */

/**
 * This controller will handle the email checkout process
 *
 */
class EmailController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest()
	 */
	public function handleRequest($form) {
		global $gallery;

		$error   = array();
		$status  = array();
		$results = array();
		if (isset($form['action']['send'])) {
			if (!empty($form['custEmail'])
				&& GalleryUtilities::isValidEmailString($form['custEmail'])
			) {
				list($ret, $transaction) = GalleryCheckoutApi::getCurrentTransactionForPayment();

				if ($ret) {
					return array($ret, null);
				}

				$transactionId             = $transaction->getId();
				$form['custRecipientName'] = trim($form['custRecipientName']);
				if (empty($form['custRecipientName'])) {
					$form['custRecipientName'] = $form['custFirstName']
						. ' '
						. $form['custLastName'];
				}

				$ret = GalleryCheckoutApi::setPaymentPlugin($transaction, 'checkoutemail');

				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCheckoutApi::transactionUpdate($transaction, $form);

				if ($ret) {
					return array($ret, null);
				}

				$results['redirect']['controller']    = 'checkout.OrderComplete';
				$results['redirect']['transactionId'] = $transactionId;
				$results['redirect']['verify1']       = $transaction->getDate();
				$results['redirect']['verify2']       = $transaction->getSecurity();
			} else {
				$error[]                     = 'form[error][badEmail]'; // badly formed email address
				$results['delegate']['view'] = 'checkoutemail.Details';
			}
		} elseif (isset($form['action']['email'])) {
			// Entry from checkout page here
			$results['delegate']['view'] = 'checkoutemail.Details';
		}

		$results['status'] = $status;
		$results['error']  = $error;
		return array(null, $results);
	}
}
