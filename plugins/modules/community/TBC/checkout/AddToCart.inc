<?php

/*
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
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
 * @version $Revision: 1649 $
 * @package Checkout
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * Required classes
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutHelper.class');

/**
 * This controller will handle the addition of an item to the cart
 *
 * @package Checkout
 * @subpackage UserInterface
 */
class AddToCartController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest()
	 */
	public function handleRequest($form) {
		global $gallery;

		$items   = array();
		$IPQ     = array();
		$session =& $gallery->getSession();

		list($ret, $treatAlbumAsCollection) = GalleryCoreApi::getPluginParameter(
			'module',
			'checkout',
			'treatAlbumAsCollection'
		);

		if ($ret) {
			return $ret;
		}

		$itemsDetails = explode(',', GalleryUtilities::getRequestVariables('itemId'));

		foreach ($itemsDetails as $itemDetails) {
			if (empty($itemDetails)) {
				continue;
			}

			// break out product and quantity if set
			$itemProductQuantity = explode('|', $itemDetails);

			if (empty($itemProductQuantity[0])) {
				continue;
			}

			$itemId = $itemProductQuantity[0];

			// check permission to purchase
			list($ret, $hasPermission) = CheckoutHelper::hasPurchasePermission($itemId);

			if ($ret) {
				return array($ret, null);
			}

			if (!$hasPermission) {
				return array(GalleryCoreApi::error(ERROR_PERMISSION_DENIED), null);
			}

			list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

			if ($ret) {
				return array($ret, null);
			}

			// Figure out what items we care about
			if ($item->getCanContainChildren() && $treatAlbumAsCollection) {
				list($ret, $ids) = GalleryCoreApi::fetchChildDataItemIds($item);

				if ($ret) {
					return array($ret, null);
				}

				list($ret, $newItems) = GalleryCoreApi::loadEntitiesById($ids, 'GalleryEntity');

				if ($ret) {
					return array($ret, null);
				}
			} else {
				$ids      = array($itemId);
				$newItems = array($item);
			}

			// deal with each item to add
			foreach ($newItems as $newItem) {
				if (!isset($firstItemId)) {
					$firstItemId = $newItem->getId();
				}

				$items[] = $newItem;

				if (isset($itemProductQuantity[1], $itemProductQuantity[2])) {
					$IPQ[$newItem->getId()][$itemProductQuantity[1]] = $itemProductQuantity[2];
				}
			}
		}

		// Get the current cart contents
		list($ret, $checkoutItems) = CheckoutHelper::loadCheckoutItemsFromSession();

		if ($ret) {
			return $ret;
		}

		// Add the new items
		$ret = CheckoutHelper::addItemsToItemList($checkoutItems, $items, $IPQ);

		if ($ret) {
			return array($ret, null);
		}

		// Save the cart
		$ret = CheckoutHelper::storeItemListInSession($checkoutItems);

		if ($ret) {
			return $ret;
		}

		// Prepare our results
		list($ret, $cartRedirect) = GalleryCoreApi::getPluginParameter(
			'module',
			'checkout',
			'cartredirect'
		);

		if ($ret) {
			return array($ret, null);
		}

		if ($cartRedirect == 1) {
			CheckoutHelper::StoreReturnURL();

			$results['redirect']['view'] = 'checkout.Select';

			if (isset($firstItemId)) {
				$results['redirect']['fragment'] = $firstItemId;
			}

			$results['status'] = array();
		} else {
			$returnUrl                   = GalleryUtilities::getRequestVariables('return');
			$results['redirect']['href'] = $returnUrl;
			$results['status']           = array(
				'checkoutSuccess' => true,
			);
		}

		$results['error'] = array();

		return array(null, $results);
	}
}
