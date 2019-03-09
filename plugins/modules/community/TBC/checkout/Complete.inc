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
 * @version $Revision: 1588 $
 * @package Checkout
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * This view is for letting the user know the transaction was completed, and
 * emptying the shopping cart
 *
 * @package Checkout
 * @subpackage UserInterface
 */
class CompleteController extends GalleryController {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function handleRequest($form) {
		global $gallery;

		/* note: errors returned here should NOT affect order record
		   it just means the customer baulked at payment - not that there was a processing error
		   besides it's easily spoofed */
		list($error, $transactionId) = GalleryUtilities::getRequestVariables('error', 'transactionId');

		if (empty($transactionId)) {
			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
		}

		list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($transactionId);

		if ($ret) {
			return array($ret, null);
		}

		if (!isset($transaction)) {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}

		if (!GalleryUtilities::isA($transaction, 'GalleryCheckoutTransaction')) {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}

		// Security check
		$verify1 = GalleryUtilities::getRequestVariables('verify1');
		$verify2 = GalleryUtilities::getRequestVariables('verify2');
		if ($verify1 != $transaction->getDate() || $verify2 != $transaction->getSecurity()) {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}

		$results['error'] = $results['status'] = array();
		if ($error == 'cancel') {
			$results['delegate']['view']          = 'checkout.OrderStatus';
			$results['delegate']['transactionId'] = $transactionId;
			$results['delegate']['verify1']       = $verify1;
			$results['delegate']['verify2']       = $verify2;
			$results['error'][]                   = 'form[error][cancel]';
		} elseif ($error == 'alreadyPlaced') {
			// Empty out the shopping cart
			GalleryCheckoutApi::emptyCart();
			$results['redirect']['view']          = 'checkout.OrderStatus';
			$results['redirect']['transactionId'] = $transactionId;
			$results['redirect']['verify1']       = $verify1;
			$results['redirect']['verify2']       = $verify2;
			$results['status']['alreadyPlaced']   = 1;
		} else {
			// First, empty out the shopping cart
			GalleryCheckoutApi::emptyCart();

			// and trigger a confirmation email
			if ($transaction->getAmount() > 0) {
				GalleryCheckoutApi::changeTransactionStatus($transaction, 'confirm');
			} else {
				GalleryCheckoutApi::changeTransactionStatus($transaction, 'clear');
			}

			if ($ret) {
				return array($ret, null);
			}

			$results['redirect']['view']          = 'checkout.OrderStatus';
			$results['redirect']['transactionId'] = $transactionId;
			$results['redirect']['verify1']       = $verify1;
			$results['redirect']['verify2']       = $verify2;
			$results['status']['newlyPlaced']     = 1;
		}

		return array(null, $results);
	}
}
