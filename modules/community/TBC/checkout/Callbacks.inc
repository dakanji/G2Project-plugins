<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
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
 * @package Checkout
 * @version $Revision: 1689 $
 * @author Alec Myers <alec@alecmyers.com>
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutTransactionHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutHelper.class');

class CheckoutCallbacks {
	public function callback($params, &$smarty, $callback, $userId = null) {
		global $gallery;

		switch ($callback) {
			case 'countCheckoutItems':
				$itemCount  = 0;
				$thumbnails = array();

				GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutHelper.class');

				// full details of all items in cart including product/quantity and paper info
				list($ret, $items) = CheckoutHelper::fetchCheckoutItems();

				if ($ret) {
					return array($ret, null);
				}

				foreach ($items as $itemId => $item) {
					// only items with numeric keys therefore ignore, postage, discount, etc
					if (!is_numeric($itemId)) {
						continue;
					}

					// get thumbnail
					list($ret, $thumbnail) = GalleryCoreApi::fetchThumbnailsByItemIds(array($itemId));

					if ($ret) {
						return $ret;
					}

					if (isset($thumbnail[$itemId])) {
						$thumbnails[$itemId] = (array)$thumbnail[$itemId];
					}

					$itemCount++;
				}

				$block                           =& $smarty->_tpl_vars['block'];
				$block['checkout']['items']      = $items;
				$block['checkout']['thumbnails'] = $thumbnails;
				$block['checkout']['itemCount']  = $itemCount;

				return null;

			case 'itemPrices':
				// prices for an individual item
				$block =& $smarty->_tpl_vars['block'];

				// permission to purchase?
				list($ret, $canPurchase) = CheckoutHelper::hasPurchasePermission($params['itemId']);

				if ($ret) {
					return $ret;
				}

				if (!$canPurchase) {
					$block['checkout']['showProducts'] = false;

					return null;
				}

					GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutProductMapHelper.class');

					list($ret, $products) = CheckoutProductMapHelper::fetchVisibleProducts(
						$params['itemId'],
						true
					);

				if ($ret) {
					return $ret;
				}

					list($ret, $csymbol) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'csymbol');

				if ($ret) {
					return $ret;
				}

					list($ret, $cformat) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'cformat');

				if ($ret) {
					return $ret;
				}

					$block['checkout']['showProducts'] = true;
					$block['checkout']['products']     = $products;
					$block['checkout']['csymbol']      = $csymbol;
					$block['checkout']['cformat']      = $cformat;

				return null;

			case 'transactionTotal':
				list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($params['transactionId']);

				if ($ret) {
					return $ret;
				}

				if (!isset($transaction)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				list($ret, $csymbol) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'csymbol');

				if ($ret) {
					return $ret;
				}

				list($ret, $cformat) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'cformat');

				if ($ret) {
					return $ret;
				}

				$block                        =& $smarty->_tpl_vars['block'];
				$block['checkout']['csymbol'] = $csymbol;
				$block['checkout']['cformat'] = $cformat;

				$block['checkout']['total'] = $transaction->getAmount();

				return null;

			case 'transactionCustomerDetails':
				// transaction customer details
				list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($params['transactionId']);

				if ($ret) {
					return $ret;
				}

				if (!isset($transaction)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				$block                              =& $smarty->_tpl_vars['block'];
				$block['checkout']['date']          = $transaction->getDate();
				$block['checkout']['buyerId']       = $transaction->getBuyerId();
				$block['checkout']['firstName']     = $transaction->getFirstName();
				$block['checkout']['lastName']      = $transaction->getLastName();
				$block['checkout']['name']          = $transaction->getFirstName() . ' ' . $transaction->getLastName();
				$block['checkout']['email']         = $transaction->getEmail();
				$block['checkout']['recipientName'] = $transaction->getRecipientName();
				$block['checkout']['telephone']     = $transaction->getTelephone();
				$block['checkout']['address1']      = $transaction->getAddress1();
				$block['checkout']['address2']      = $transaction->getAddress2();
				$block['checkout']['address3']      = $transaction->getAddress3();
				$block['checkout']['address4']      = $transaction->getAddress4();
				$block['checkout']['address5']      = $transaction->getAddress5();
				$block['checkout']['addressZip']    = $transaction->getAddressZip();
				$block['checkout']['country']       = $transaction->getAddressCountry();
				$block['checkout']['custNotes']     = $transaction->getCustNotes();
				$block['checkout']['custComments']  = $transaction->getCustComments();
				$block['checkout']['paymentPlugin'] = $transaction->getPaymentPlugin();

				//this one so we can setup a mailto link with sensible subject
				list($ret, $orderStatusSubject) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'orderStatusSubject');

				if ($ret) {
					return $ret;
				}

				$block['checkout']['orderStatusSubject'] = $orderStatusSubject;

				return null;

			case 'transactionFlags':
				 // transaction status flags
				 list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($params['transactionId']);

				if ($ret) {
					return $ret;
				}

				if (!isset($transaction)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				$block                                 =& $smarty->_tpl_vars['block'];
				$block['checkout']['date']             = $transaction->getDate();
				$block['checkout']['confirmed']        = $transaction->getTimeConfirmed();
				$block['checkout']['paid']             = $transaction->getTimePaid();
				$block['checkout']['cleared']          = $transaction->getTimeCleared();
				$block['checkout']['fulfil']           = $transaction->getTimeFulfil();
				$block['checkout']['fulfilled']        = $transaction->getTimeFulfilled();
				$block['checkout']['completed']        = $transaction->getTimeCompleted();
				$block['checkout']['hold']             = $transaction->getTimeHold();
				$block['checkout']['problem']          = $transaction->getTimeProblem();
				$block['checkout']['downloaded']       = $transaction->getTimeDownloaded();
				$block['checkout']['downloadAttempts'] = $transaction->getDownloadAttempts();

				return null;

			case 'transactionItems':
				// get the transaction items for display in a page or email
				$transactionId = $params['transactionId'];

				list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($transactionId);

				if ($ret) {
					return $ret;
				}

				if (!isset($transaction)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				$block                                    =& $smarty->_tpl_vars['block'];
				$block['checkout']['total']               = $transaction->getAmount();
				$block['checkout']['postage']             = $transaction->getPostage();
				$block['checkout']['postageType']         = $transaction->getPostageType();
				$block['checkout']['discount']            = $transaction->getDiscount();
				$block['checkout']['comments']            = $transaction->getCustComments();
				$block['checkout']['discountDescription'] = $transaction->getDiscountDescription();

				list($ret, $csymbol) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'csymbol');

				if ($ret) {
					return $ret;
				}

				$block['checkout']['csymbol'] = $csymbol;

				list($ret, $cformat) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'cformat');

				if ($ret) {
					return $ret;
				}

				$block['checkout']['cformat'] = $cformat;

				list($ret, $items) = CheckoutTransactionHelper::fetchFullTransactionItems($transactionId);

				if ($ret) {
					return $ret;
				}

				//add full items and thumbnails - just to display thumbs!
				foreach ($items as $lineNo => $item) {
					list($ret, $fullItem) = GalleryCoreApi::loadEntitiesById($item['itemId']);

					if ($ret) {
						if ($ret->getErrorCode() & ERROR_MISSING_OBJECT) {
							// Item deleted from the gallery - it remains in the order though
							continue;
						}

						return $ret;
					}

					$items[$lineNo]['fullItem'] = (array)$fullItem;

					list($ret, $thumbnail) = GalleryCoreApi::fetchThumbnailsByItemIds(array($item['itemId']));

					if ($ret) {
						if ($ret->getErrorCode() & ERROR_MISSING_OBJECT) {
							// Item deleted from the gallery - it remains in the order though
							continue;
						}

						return $ret;
					}

					$items[$lineNo]['thumbnail'] = (array)$thumbnail[$item['itemId']];
				}

				$block['checkout']['items'] = $items;

				return null;

			case 'transactionVerifyDetails':
				//two pieces of information checked before releasing the status of an order
				//or permitting a download - for security reasons
				list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($params['transactionId']);

				if ($ret) {
					return $ret;
				}

				if (!isset($transaction)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				$block                        =& $smarty->_tpl_vars['block'];
				$block['checkout']['verify1'] = $transaction->getDate();
				$block['checkout']['verify2'] = $transaction->getSecurity();

				return null;

			break;
		}

		return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}
}
