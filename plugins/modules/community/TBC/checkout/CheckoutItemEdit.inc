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
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutProductMapHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutInterfaceHelper.class');

/**
 * This handles per-album and per-photo custom prices
 * @package Checkout
 * @version $Revision: 1711 $
 * @author Alec Myers <alec@alecmyers.com>
 */
class CheckoutItemEdit extends ItemEditPlugin {
	/**
	 * @see ItemEditPlugin::handleRequest
	 */
	public function handleRequest($form, &$item, &$preferred) {
		global $gallery;

		$status  = 0;
		$error   = array();
		$itemId  = $item->getId();
		$storage =& $gallery->getStorage();

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkout');

		if ($ret) {
			return array($ret, null, null, null);
		}

		if (isset($form['action']['save'])) {
			foreach ($form['products'] as $productId => $productParameters) {
				// If remove is ticked...
				if (isset($productParameters['remove']) && $productParameters['remove'] == 'true') {
					$ret = CheckoutProductMapHelper::removeLocal($productId, $itemId);

					if ($ret) {
						return array($ret, null, null, null);
					}

					continue;
				}

				// otherwise ...
				foreach ($productParameters as $paramName => $paramValue) {
					$paramValue = trim($paramValue);
					if ($paramValue != '') {
						$ret = CheckoutProductMapHelper::setParam($productId, $paramName, $paramValue, $itemId);
					} else {
						$ret = CheckoutProductMapHelper::removeParam($productId, $paramName, $itemId);
					}

					if ($ret) {
						return array($ret, null, null, null);
					}
				}
			}

			if (isset($form['accessories'])) {
				foreach ($form['accessories'] as $accessoryId => $accessoryParameters) {
					// If the the name is not empty and the id is negative, get a new id
					$name  = trim($accessoryParameters['name']);
					$price = trim($accessoryParameters['price']);
					if ($accessoryId < 0 && $name != '') {
						list($ret, $accessoryId) = $storage->getUniqueId();

						if ($ret) {
							return array($ret, null, null, null);
						}
					}

					if ($accessoryId > 0 && $name == '') {
						$ret = CheckoutProductMapHelper::deleteProduct($accessoryId);

						if ($ret) {
							return array($ret, null, null, null);
						}
					}

					if ($accessoryId > 0 && $name != '') {
						// Set each parameter for the accessory
						foreach ($accessoryParameters as $paramName => $paramValue) {
							$ret = CheckoutProductMapHelper::setParam($accessoryId, $paramName, $paramValue, $itemId);

							if ($ret) {
								return array($ret, null, null, null);
							}
						}
					}
				}

				$displayPriority = trim($form['position']);
				if ($displayPriority != '') {
					$ret = CheckoutProductMapHelper::setParam(0, 'position', $displayPriority, $itemId);
				} else {
					$ret = CheckoutProductMapHelper::removeParam(0, 'position', $itemId);
				}

				if ($ret) {
					return array($ret, null, null, null);
				}
			}

			$status = $module->translate('Changes saved successfully');
		}

		// plugins
		list($ret, $pluginError, $pluginStatus) = CheckoutInterfaceHelper::saveCustomPagePlugins($form, $item, $preferred);

		if ($ret) {
			return array($ret, null, null, null);
		}

		return array(null, $error, $status, null);
	}

	/**
	 * @see ItemEditPlugin::loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $item, $thumbnail) {
		global $gallery;

		if ($form['formName'] != 'CheckoutItemEdit') {
			$form['formName'] = 'CheckoutItemEdit';
			$thisId           = $item->getId();

			list($ret, $products) = CheckoutProductMapHelper::fetchProducts($thisId, true, false);

			// must be using custom parameters, but don't pass via external plugins
			if ($ret) {
				return array($ret, null);
			}

			list($ret, $globalProducts) = CheckoutProductMapHelper::fetchProducts($thisId, false, false);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $localProdParams) = CheckoutProductMapHelper::fetchLocalParamsByProduct($thisId);

			if ($ret) {
				return array($ret, null);
			}

			$form['products'] = array();

			// display *global* Name and productId, but *local* price and visible fields if they exist
			foreach ($products as $productIndex => $product) {
				$productId = $product['productId'];
				if (key_exists($productId, $localProdParams)) {
					// start with all local params
					$form['products'][$productIndex]                = $localProdParams[$productId];
					$form['products'][$productIndex]['offerRemove'] = 'true';
				} else {
					$form['products'][$productIndex]['offerRemove'] = 'false';
				}

				$form['products'][$productIndex]['id']       = $productId;
				$form['products'][$productIndex]['rootName'] = $globalProducts[$productIndex]['name'];
			}

			unset($form['accessories']);
			if (!$item->getCanContainChildren()) {
				list($ret, $accessories, $form['position']) = CheckoutProductMapHelper::fetchAccessoryData($thisId);

				if ($ret) {
					return array($ret, null);
				}

				foreach ($accessories as $accessory) {
					$form['accessories'][] = array(
						'id'          => $accessory['accessoryId'],
						'name'        => $accessory['name'],
						'price'       => $accessory['price'],
						'postageBand' => $accessory['postageBand'],
					);
				}

				// add two blank lines
				$form['accessories'][] = array(
					'id'          => -1,
					'postageBand' => '0',
				);

				$form['accessories'][] = array(
					'id'          => -2,
					'postageBand' => '0',
				);

				list($ret, $accessories) = CheckoutProductMapHelper::fetchAccessories();
			}
		}

		list($ret, $rootAlbumId) = GalleryCoreApi::getDefaultAlbumId();

		if ($ret) {
			return array($ret, null);
		}

		$isRootAlbum = ($rootAlbumId == $thisId);

		list($ret, $csymbol) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'csymbol');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $numPostageBands) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'numPostageBands');

		if ($ret) {
			return array($ret, null);
		}

		// plugins
		list($ret, $plugins) = CheckoutInterfaceHelper::getCustomPagePlugins($template, $form, $item, $thumbnail);

		if ($ret) {
			return array($ret, null);
		}

		$template->setVariable('csymbol', $csymbol);
		$template->setVariable('numPostageBands', $numPostageBands);
		$template->setVariable('isRootAlbum', $isRootAlbum);
		$template->setVariable('customPlugins', $plugins);
		$template->setVariable('controller', 'checkout.CheckoutItemEdit');
		return array(null, 'modules/checkout/templates/CheckoutItemEdit.tpl', 'modules_checkout');
	}

	/**
	 * @see ItemEditPlugin::isSupported
	 */
	public function isSupported($item, $thumbnail) {
		//check if this should return a result code or not
		list($ret, $customPrices) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'customPrices');

		if ($ret) {
			return array($ret, null);
		}

		if (!$customPrices) {
			return false;
		}

		if (GalleryUtilities::isA($item, 'GalleryAlbumItem')) {
			return true;
		}

		if (GalleryUtilities::isA($item, 'GalleryPhotoItem')) {
			return true;
		}
	}

	/**
	 * @see ItemEditPlugin::getTitle
	 */
	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkout');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Checkout'));
	}
}
