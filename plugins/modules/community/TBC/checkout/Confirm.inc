<?php

/*
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
 * @version $Revision: 1916 $
 * @author Daniel Leaberry <leaberry@gmail.com>
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * Required classes
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutTransactionHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutInterfaceHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutProductMapHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutDiscountMapHelper.class');

GalleryCoreApi::requireOnce('modules/checkout/classes/GalleryCheckoutApi.class');

/**
 * This is the controller for both checkout pages
 *
 * @package Checkout
 * @subpackage UserInterface
 *
 */
class ConfirmController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest()
	 */
	public function handleRequest($form) {
		global $gallery;

		$error   = array();
		$status  = array();
		$results = array();

		// CONTINUE TO CHECKOUT
		if (isset($form['action']['continue'])) {
			$session =& $gallery->getSession();

			// Update the item records based on the form
			$ret = CheckoutHelper::updateCart($form);

			if ($ret) {
				return array($ret, null);
			}

			// Load the required info from the cart
			list($ret, $items) = CheckoutHelper::fetchCheckoutItems();

			if ($ret) {
				return array($ret, null);
			}

			// build table of available products for each item in the cart
			$itemProducts = array();
			foreach ($items as $itemId => $item) {
				if (!is_numeric($itemId)) {
					continue;
				}

				list($ret, $thisItemsProducts) = CheckoutProductMapHelper::fetchVisibleProducts($itemId, true);

				if ($ret) {
					return array($ret, null);
				}

				$itemProducts[$itemId] = $thisItemsProducts;
			}

			list($ret, $paper) = CheckoutHelper::fetchPapers();

			if ($ret) {
				return array($ret, null);
			}

			// Get the postage options
			list($ret, $posts) = CheckoutHelper::fetchPostages();

			if ($ret) {
				return array($ret, null);
			}

			// fetch discounts
			list($ret, $discounts) = CheckoutDiscountMapHelper::fetchDiscounts();

			if ($ret) {
				return array($ret, null);
			}

			/* allow plugins to alter $itemProducts, $paper, $posts, $discounts
			   note: building $itemProducts already calls fetchVisibleProducts which in turn passes through
			   the checkoutProductList interface - so no need to repeat modifications to products list done there */
			$ret = checkoutInterfaceHelper::doTransactionDetailsPlugins($items, $itemProducts, $paper, $posts, $discounts);

			if ($ret) {
				return array($ret, null);
			}

			// create a new transaction item - no lock needed as it's new
			list($ret, $transaction) = GalleryCoreApi::newFactoryInstance('GalleryEntity', 'GalleryCheckoutTransaction');

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($transaction)) {
				return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
			}

			$ret = $transaction->create();

			if ($ret) {
				return array($ret, null);
			}

			$total        = 0;
			$itemCount    = 0;
			$productCount = 0;

			// will be set to true if at least one product has non-zero qty
			$atLeastOneQtySet = false;

			// does the order require a postage charge?
			$postageBand = 0;

			// Any required paper choices missing?
			$missingPaper = false;
			foreach ($items as $itemId => $item) {
				// break out for postage, discount etc
				if (!is_numeric($itemId)) {
					continue;
				}

				$subtotal                 = 0;
				$atLeastOneQtySetThisItem = false;
				foreach ($itemProducts[$itemId] as $product) {
					// ignore this entry if no quantity set for it
					if (!isset($item[$product['productId']]['quant']) || $item[$product['productId']]['quant'] == 0) {
						continue;
					}

					// limit to quantityLimit if set
					if (isset($product['quantityLimit']) && $product['quantityLimit'] > 0) {
						$item[$product['productId']]['quant'] = min($item[$product['productId']]['quant'], $product['quantityLimit']);
					}

					$atLeastOneQtySet         = true;
					$atLeastOneQtySetThisItem = true;
					$postageBand              = max($postageBand, $product['postageBand']);

					// Create the item entry
					list($ret, $txnItem) = GalleryCoreApi::newFactoryInstance('GalleryEntity', 'GalleryCheckoutItem');

					if ($ret) {
						return array($ret, null);
					}

					if (!isset($txnItem)) {
						return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
					}

					$ret = $txnItem->create($transaction->getId());

					if ($ret) {
						return array($ret, null);
					}

					// store the txnItem Id in the cart for other modules to use
					$items[$itemId][$product['productId']]['txnItemId'] = $txnItem->getId();

					$txnItem->setItem($itemId);

					$txnItem->setTransaction($transaction->getId());

					$txnItem->setItemName($item['fullItemDetails']['pathComponent']);
					$txnItem->setItemText($item['parentTitle']);
					$txnItem->setProduct($product['productId']);
					$txnItem->setProductName($product['name']);
					$txnItem->setQty($item[$product['productId']]['quant']);

					$productCount += $item[$product['productId']]['quant'];

					$txnItem->setPriceEach($product['price']);
					$txnItem->setPriceTotal(

						number_format($item[$product['productId']]['quant'] * $product['price'], 2, '.', '')
					);

					$subtotal += ($item[$product['productId']]['quant'] * $product['price']);
					if ($product['paperSet']) {
						if (isset($item[$product['productId']]['paper']) && $item[$product['productId']]['paper'] != '') {
							$txnItem->setNotes($item[$product['productId']]['paper']);
						} else {
							$missingPaper = true;
						}
					}

					$ret = $txnItem->save();

					if ($ret) {
						return array($ret, null);
					}
				}

				if ($atLeastOneQtySetThisItem) {
					$itemCount++;
				}

				$total                     += $subtotal;
				$items[$itemId]['subtotal'] = number_format($subtotal, 2, '.', '');
			}

			// Accessories
			list($ret, $accessoriesDetails) = CheckoutProductMapHelper::FetchAccessories();

			if ($ret) {
				return array($ret, null);
			}

			foreach ($accessoriesDetails as $accessoryDetails) {
				$subtotal = 0;
				foreach ($accessoryDetails['options'] as $option) {
					$optionId = $option['id'];
					if (isset($items['accessories'][$optionId]) && $items['accessories'][$optionId]['quant'] > 0) {
						$postageBand = max($postageBand, $option['postageBand']);

						// Create the item entry
						list($ret, $txnItem) = GalleryCoreApi::newFactoryInstance('GalleryEntity', 'GalleryCheckoutItem');

						if ($ret) {
							return array($ret, null);
						}

						if (!isset($txnItem)) {
							return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
						}

						$ret = $txnItem->create($transaction->getId());

						if ($ret) {
							return array($ret, null);
						}

						// store the txnItem Id in the cart for other modules to use
						$items['accessories'][$optionId]['txnItemId'] = $txnItem->getId();

						$txnItem->setItem($accessoryDetails['id']);

						$txnItem->setTransaction($transaction->getId());

						$txnItem->setItemName($accessoryDetails['fullItemDetails']['title']);
						$txnItem->setItemText($accessoryDetails['fullItemDetails']['summary']);
						$txnItem->setProduct($optionId);
						$txnItem->setProductName($option['name']);
						$txnItem->setQty($items['accessories'][$optionId]['quant']);

						$productCount += $items['accessories'][$optionId]['quant'];

						$txnItem->setPriceEach($option['price']);
						$txnItem->setPriceTotal(number_format($items['accessories'][$optionId]['quant'] * $option['price'], 2, '.', ''));

						$subtotal                                   += ($items['accessories'][$optionId]['quant'] * $option['price']);
						$items['accessories'][$optionId]['subtotal'] = number_format($subtotal, 2, '.', '');

						$ret = $txnItem->save();

						if ($ret) {
							return array($ret, null);
						}
					}
				}

				$total += $subtotal;
			}

			// Check minimum order value
			list($ret, $minOrderValue) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'minOrderValue');

			if ($ret) {
				return $ret;
			}

			$belowMinimumValue = ($total < $minOrderValue);

			// Calculate appropriate discount
			unset($myDiscount);
			foreach ($discounts as $discount) {
				list($ret, $inGroup) = GalleryCoreApi::isUserInGroup($gallery->getActiveUserId(), $discount['groupId']);

				if ($ret) {
					return array($ret, null);
				}

				if ($discount['applyDefault'] == 'true'
					|| (isset($items['discountCode']) && !strcasecmp($items['discountCode'], $discount['entryCode']))
				) {
					if ($discount['minValue'] > $total
						|| $discount['minItems'] > $itemCount
						|| $discount['minProducts'] > $productCount || !$inGroup
					) {
						continue;
					}

					$myDiscount = $discount;
					break;
				}
			}

			// Actuate it
			if (isset($myDiscount)) {
				$discountAmount  = $total * $myDiscount['discountPercent'] / 100;
				$discountAmount += $myDiscount['discountFixed'];
				$discountAmount  = number_format($discountAmount, 2, '.', '');

				$transaction->setDiscount($discountAmount);
				$transaction->setDiscountDescription($myDiscount['description']);

				$total -= $discountAmount;
			}

			// Postage
			$postage = 0;

			// If postage needed and only one post option exists, assume it
			if ($postageBand && count($posts) == 1) {
				$items['postage'] = 0;
			}

			if ($postageBand && isset($items['postage'])) {
				$postage = $posts[$items['postage']]['price'][$postageBand];
				if (isset($myDiscount['freePostage']) && $myDiscount['freePostage'] == 'true') {
					$postage = 0;
				}

				$transaction->setPostageType($posts[$items['postage']]['type']);
				$transaction->setPostage($postage);

				$total += $postage;
			}

			// Special instructions etc
			if (isset($items['instructions'])) {
				$transaction->setCustComments($items['instructions']);
			}

			// tax interface here?
			// avoid negative balances
			if ($total < 0) {
				$total = 0;
			}

			// Set and save the data
			$transaction->sethost(GalleryUtilities::getRemoteHostAddress());

			$translator =& $gallery->getTranslator();

			list($ret, $languageCode) = $translator->getDefaultLanguageCode();

			if ($ret) {
				return $ret;
			}

			$transaction->setLanguageCode($languageCode);
			$transaction->setbuyerId($gallery->getActiveUserId());

			// Because the G2 storage API only handles INTEGER numeric storage, store as string
			$total = number_format($total, 2, '.', '');

			$transaction->setamount($total);

			$ret = $transaction->save();

			if ($ret) {
				return array($ret, null);
			}

			// call an event for other modules to know a new transaction has been stored
			$event = GalleryCoreApi::newEvent('GalleryCheckout::newTransaction');

			$event->setEntity($transaction);
			$event->setData($items);

			list($ret) = GalleryCoreApi::postEvent($event);

			if ($ret) {
				return array($ret, null);
			}

			// cart is now updated with subtotals and txnItemIds, so store it
			$ret = CheckoutHelper::storeItemListInSession($items);

			if ($ret) {
				return array($ret, null);
			}

			$session->put('checkout.transactionId', $transaction->getId());

			if (!$atLeastOneQtySet) {
				$error[]                     = 'form[error][noQuantity]';
				$results['delegate']['view'] = 'checkout.Select';
			}

			if ($missingPaper) {
				$error[]                     = 'form[error][noPaper]';
				$results['delegate']['view'] = 'checkout.Select';
			}

			if ($belowMinimumValue) {
				$error[]                     = 'form[error][minValue]';
				$results['delegate']['view'] = 'checkout.Select';
			}

			if ($postageBand && !isset($items['postage'])) {
				$error[]                     = 'form[error][noPostage]';
				$results['delegate']['view'] = 'checkout.Select';
			}

			if (empty($error)) {
				// show confirm page with payment plugins
				$results['redirect']['view'] = 'checkout.Confirm';
			}
		}

		// OR MODIFY CART CONTENTS
		elseif (isset($form['action']['modify'])) {
			$results['redirect']['view'] = 'checkout.Select';
		}

		// OR EMPTY CART
		elseif (isset($form['action']['emptycart'])) {
			// Call the helper to update the item records based on the form
			$ret = CheckoutHelper::emptycart($form);

			if ($ret) {
				return array($ret, null);
			}

			$results['redirect']['view'] = 'checkout.Select';
		}

		// OR CONTINUE SHOPPING
		elseif (isset($form['action']['continueshopping'])) {
			/* If arrived here from Select.tpl ...
			   Call the helper to update the item records based on the form
			   otherwise arrived here from Confirm.tpl - no need to update
			*/
			if (isset($form['formName']) && $form['formName'] == 'Select') {
				$ret = CheckoutHelper::updateCart($form);

				if ($ret) {
					return array($ret, null);
				}
			}

			$returnURL = CheckoutHelper::fetchReturnURL();
			if (!isset($returnURL)) {
				$results['redirect']['view'] = 'core.ShowItem';
			} else {
				$results['redirect']['href'] = $returnURL;
			}
		}

		// OR SAVE CHANGES
		elseif (isset($form['action']['savechanges'])) {
			// Call the helper to update the item records based on the form
			$ret = CheckoutHelper::updateCart($form);

			if ($ret) {
				return array($ret, null);
			}

			$results['redirect']['view'] = 'checkout.Select';
		}

		$results['status'] = $status;
		$results['error']  = $error;
		return array(null, $results);
	}
}

/**
 * This view is for letting the user confirm their order
 *
 * @package Checkout
 * @subpackage UserInterface
 */
class ConfirmView extends GalleryView {
	/**
	 * @see GalleryView::autoCacheControl
	 */
	public function autoCacheControl() {
		return false;
	}

	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		//$this->setCacheControl($template, 'no-cache'); restore for 2.3
		list($ret, $transaction) = GalleryCheckoutApi::getCurrentTransactionForPayment();

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $tpl) = GalleryCoreApi::fetchAllPluginParameters('module', 'checkout');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $plugins) = CheckoutInterfaceHelper::getPaymentPlugins();

		if ($ret) {
			return array($ret, null);
		}

		$tpl['id']            = $transaction->getId();
		$tpl['controller']    = 'checkout.Confirm';
		$tpl['payments']      = $plugins;
		$tpl['paymentPlugin'] = $transaction->getPaymentPlugin();

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkout');

		if ($ret) {
			return array($ret, null);
		}

		$template->title($tpl['busname'] . ' ' . $module->translate('Checkout'));
		$template->setVariable('tpl', $tpl);

		return array(
			null,
			array(
				'body' => 'modules/checkout/templates/Confirm.tpl',
			),
		);
	}
}
