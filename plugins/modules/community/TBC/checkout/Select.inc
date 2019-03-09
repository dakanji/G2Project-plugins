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
 * @version $Revision: 1711 $
 * @author Daniel Leaberry <leaberry@gmail.com>
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * Required classes
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutInterfaceHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutProductMapHelper.class');

/**
 * This view is for letting the user select the sizes and quantities for ordering.
 * It is the entry point to the user's experience of the module, acting as the
 * 'view my cart' page
 *
 * @package Checkout
 * @subpackage UserInterface
 */
class SelectView extends GalleryView {
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

		//$this->setCacheControl($template, 'no-cache'); RESTORE for 2.3
		// entry point for checkout
		CheckoutHelper::StoreReturnURL();
		$form['formName'] = 'Select';

		// General options
		list($ret, $tpl) = GalleryCoreApi::fetchAllPluginParameters('module', 'checkout');

		if ($ret) {
			return array($ret, null);
		}

		// full details of all items in cart including product/quantity and paper info, and postage
		list($ret, $items) = CheckoutHelper::fetchCheckoutItems();

		if ($ret) {
			return array($ret, null);
		}

		// plugins adjust list of items before fetching products
		$ret = CheckoutInterfaceHelper::doItemListPlugins($items);

		if ($ret) {
			return array($ret, null);
		}

		// Fetch all accessories
		list($ret, $tpl['accessories']) = CheckoutProductMapHelper::fetchAccessories();

		if ($ret) {
			return array($ret, null);
		}

		if (array_key_exists('accessories', $items)) {
			$tpl['chosenAccessories'] = $items['accessories'];
			unset($items['accessories']);
		}

		if (array_key_exists('discountCode', $items)) {
			$tpl['discountCode'] = $items['discountCode'];
			unset($items['discountCode']);
		}

		if (array_key_exists('instructions', $items)) {
			$tpl['instructions'] = $items['instructions'];
			unset($items['instructions']);
		}

		if (array_key_exists('postage', $items)) {
			$tpl['postageSet'] = $items['postage'];
			unset($items['postage']);
		} else {
			$tpl['postageSet'] = -1;
		}

		// Build table of available products for each item in the cart
		$itemProducts = array();
		foreach ($items as $item) {
			list($ret, $thisItemsProducts) = CheckoutProductMapHelper::fetchVisibleProducts($item['id'], true);

			if ($ret) {
				return array($ret, null);
			}

			$itemProducts[$item['id']] = $thisItemsProducts;

			/* count how many products with unset quantities there are
			(if two or more then use combo if requested) */
			$comboCount = 0;
			foreach ($itemProducts[$item['id']] as $thisProduct) {
				if (!isset($item[$thisProduct['productId']]['quant']) && $thisProduct['dropdown'] == 'true') {
					$comboCount++;
				}

				$items[$item['id']]['comboCount'] = $comboCount;
			}
		}

		// decide whether to include postage options in the template
		$offerPostage          = false;
		$tpl['postageOptions'] = array();
		foreach ($items as $item) {
			foreach ($itemProducts[$item['id']] as $product) {
				if (isset($item[$product['productId']]['quant'])
					&& $item[$product['productId']]['quant'] > 0
					&& $product['postageBand'] >= 0
				) {
					$offerPostage = true;
				}
			}
		}

		if ($offerPostage) {
			// Get the postage options
			list($ret, $tpl['postageOptions']) = CheckoutHelper::fetchPostages();

			if ($ret) {
				return array($ret, null);
			}
		}

		// Get the thumbnails
		$thumbnailIdList = array_keys($items);
		foreach ($tpl['accessories'] as $accessory) {
			$thumbnailIdList[] = $accessory['id'];
		}

		list($ret, $thumbnailList) = GalleryCoreApi::fetchThumbnailsByItemIds($thumbnailIdList);

		if ($ret) {
			return array($ret, null);
		}

		$thumbnails = array();
		foreach ($thumbnailList as $thumbnail) {
			$thumbnails[$thumbnail->getParentId()] = (array)$thumbnail;
		}

		// Get the paper options
		list($ret, $tpl['paper']) = CheckoutHelper::fetchPapers();

		if ($ret) {
			return array($ret, null);
		}

		// Hack to allow the euro symbol to be used when using the Gallery2 bridge component for Joomla (to overcome UTF encoding problem)
		if ($tpl['csymbol'] == 'e') {
			$tpl['csymbol'] = '&euro;';
		}

		$tpl['items']        = $items;
		$tpl['thumbnails']   = $thumbnails;
		$tpl['itemProducts'] = $itemProducts;
		$tpl['controller']   = 'checkout.Confirm';

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkout');

		if ($ret) {
			return array($ret, null);
		}

		$template->title($tpl['busname'] . ' ' . $module->translate('Checkout'));
		$template->setVariable('tpl', $tpl);
		// $template->setVariable('status', $status);

		return array(
			null,
			array(
				'body' => 'modules/checkout/templates/Select.tpl',
			),
		);
	}
}
