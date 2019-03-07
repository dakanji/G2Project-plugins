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
 *
 * @package Checkout
 * @version $Revision: 1714 $
 * @author Daniel Leaberry <leaberry@gmail.com>
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * Required classes
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutProductMapHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutDiscountMapHelper.class');

/**
 * @package Checkout
 * @subpackage UserInterface
 */
class AdminController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$error  = array();
		$status = array();

		if (isset($form['action']['save'])) {
			// Store email, paper, postage details
			foreach ($form['emails'] as $index => $email) {
				if (!isset($email['toAddress']) || trim($email['toAddress']) == '') {
					unset($form['emails'][$index]);
				} else {
					// Insert missing checkbox fields
					foreach (array('showCustomer', 'showContents', 'showThumbs', 'showLinks', 'isAdmin') as $checkBox) {
						if (!isset($email[$checkBox])) {
							$form['emails'][$index][$checkBox] = '0';
						}
					}
				}
			}

			// Papers
			foreach ($form['papers'] as $i => $type) {
				foreach ($type as $j => $paper) {
					if (trim($paper) == '') {
						unset($form['papers'][$i][$j]);
					}
				}

				if (empty($form['papers'][$i])) {
					unset($form['papers'][$i]);
				}
			}

			// Postage Rates
			foreach ($form['postages'] as $index => $postage) {
				if (!isset($postage['type']) || trim($postage['type']) == '') {
					unset($form['postages'][$index]);
				}
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'emails',
				serialize($form['emails'])
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'postages',
				serialize($form['postages'])
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'papers',
				serialize($form['papers'])
			);

			if ($ret) {
				return array($ret, null);
			}

			$storage =& $gallery->getStorage();

			// write products here
			//note: admin page values are all owned by the root album
			list($ret, $rootAlbumId) = GalleryCoreApi::getDefaultAlbumId();

			if ($ret) {
				return array($ret, null);
			}

			foreach ($form['products'] as $productId => $productParameters) {
				//if delete is ticked...
				if (key_exists('delete', $productParameters)
					&& $productParameters['delete'] == 'true'
				) {
					$ret = CheckoutProductMapHelper::deleteProduct($productId);

					if ($ret) {
						return array($ret, null);
					}

					continue;
				}

				// If the name is blank for a product, make no changes to it
				if ($productParameters['name'] == '') {
					continue;
				}

				// If the productId is negative, it's new, so assign a unique Id
				if ($productId < 0) {
					list($ret, $productId) = $storage->getUniqueId();

					if ($ret) {
						return array($ret, null);
					}
				}

				// Make sure that tick-box fields are either "true" or "false"
				foreach (array(
					'visible',
					'chargePostage',
					'dropdown',
					'appliesToAlbum',
					'appliesToItem',
				) as $booleanParam) {
					if (!isset($productParameters[$booleanParam])) {
						$productParameters[$booleanParam] = 'false';
					}
				}

				// Make sure it has a postage band set
				if (!isset($productParameters['postageBand'])
					|| $productParameters['postageBand'] == ''
				) {
					$productParameters['postageBand'] = '0';
				}

				// Make sure it has a papers set set
				if (!isset($productParameters['paperSet'])
					|| $productParameters['paperSet'] == ''
				) {
					$productParameters['paperSet'] = '0';
				}

				// Then go ahead and store each parameter for that product
				foreach ($productParameters as $paramName => $paramValue) {
					$ret = CheckoutProductMapHelper::setParam(
						$productId,
						$paramName,
						$paramValue,
						$rootAlbumId
					);

					if ($ret) {
						return array($ret, null);
					}
				}
			}

			// discounts stored here
			foreach ($form['discounts'] as $discountId => $discount) {
				//if delete is ticked...
				if (key_exists('delete', $discount) && $discount['delete'] == 'true') {
					$ret = CheckoutDiscountMapHelper::deleteDiscount($discountId);

					if ($ret) {
						return array($ret, null);
					}

					continue;
				}

				//make sure that at least one required field is non-zero:
				if ($discount['discountPercent'] == ''
					&& $discount['discountFixed'] == ''
					&& !isset($discount['freePostage'])
				) {
					continue;
				}

				//if the discountId is negative, it's new, so assign a unique Id
				if ($discountId < 0) {
					list($ret, $discountId) = $storage->getUniqueId();

					if ($ret) {
						return array($ret, null);
					}
				}

				//make sure that tick-box fields exist
				foreach (array('applyDefault', 'freePostage') as $booleanParam) {
					if (!isset($discount[$booleanParam])) {
						$discount[$booleanParam] = '';
					}
				}

				//then go ahead and store it
				$discount['discountId'] = $discountId;

				$ret = CheckoutDiscountMapHelper::setDiscount($discount);

				if ($ret) {
					return array($ret, null);
				}
			}

			// transaction purge time limits
			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'deleteOrphanDays',
				$form['deleteOrphanDays']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'deleteAllDays',
				$form['deleteAllDays']
			);

			if ($ret) {
				return array($ret, null);
			}

			// other stuff
			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'carttext',
				$form['carttext']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'TandCLink',
				$form['TandCLink']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'busname',
				$form['busname']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'csymbol',
				$form['csymbol']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'cformat',
				$form['cformat']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'minOrderValue',
				$form['minOrderValue']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['cartredirect'])) {
				$form['cartredirect'] = 0;
			} else {
				$form['cartredirect'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'cartredirect',
				$form['cartredirect']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['askDiscountCode'])) {
				$form['askDiscountCode'] = 0;
			} else {
				$form['askDiscountCode'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'askDiscountCode',
				$form['askDiscountCode']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['customPrices'])) {
				$form['customPrices'] = 0;
			} else {
				$form['customPrices'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'customPrices',
				$form['customPrices']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['requireTandCs'])) {
				$form['requireTandCs'] = 0;
			} else {
				$form['requireTandCs'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'requireTandCs',
				$form['requireTandCs']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['treatAlbumAsCollection'])) {
				$form['treatAlbumAsCollection'] = 0;
			} else {
				$form['treatAlbumAsCollection'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'treatAlbumAsCollection',
				$form['treatAlbumAsCollection']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['alwaysShowAddToCart'])) {
				$form['alwaysShowAddToCart'] = 0;
			} else {
				$form['alwaysShowAddToCart'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'alwaysShowAddToCart',
				$form['alwaysShowAddToCart']
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkout');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $isActive) = $module->isActive();

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'orderStatusFromAddress',
				$form['orderStatusFromAddress']
			);

			if ($ret) {
				return array($ret, null);
			}

			$form['numPostageBands'] = min(20, $form['numPostageBands']);
			$form['numPostageBands'] = max($form['numPostageBands'], 2);

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'numPostageBands',
				$form['numPostageBands']
			);

			if ($ret) {
				return array($ret, null);
			}

			$form['numPaperSets'] = min(20, $form['numPaperSets']);
			$form['numPaperSets'] = max($form['numPaperSets'], 1);

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'numPaperSets',
				$form['numPaperSets']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'orderStatusSubject',
				$form['orderStatusSubject']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['showCartPlugins'])) {
				$form['showCartPlugins'] = 0;
			} else {
				$form['showCartPlugins'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'showCartPlugins',
				$form['showCartPlugins']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkout',
				'configured',
				'true'
			);

			if ($ret) {
				return array($ret, null);
			}

			$redirect['view'] = 'core.SiteAdmin';

			if ($isActive) {
				$redirect['subView'] = 'checkout.Admin';
				$status['saved']     = 1;
			} else {
				$redirect['subView']  = 'core.AdminModules';
				$status['configured'] = 'checkout';
			}
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'checkout.Admin';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'checkout.Admin';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * @package Checkout
 * @subpackage UserInterface
 */
class AdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'AdminCheckout') {
			$form['formName'] = 'AdminCheckout';

			// Get module options
			list($ret, $mp) = GalleryCoreApi::fetchAllPluginParameters('module', 'checkout');

			if ($ret) {
				return array($ret, null);
			}

			// Put Single-value parameters into $form
			$form = $mp;

			// Email addresses and parameters
			$form['emails'] = unserialize($mp['emails']);

			/*
			 * Include check-box fields to stop smarty notices since
			 * there's no 'default' option, and {if isset...}{/if} very clumsy
			 */
			$form['emails'][] = array(
				'showCustomer'  => '',
				'showContents'  => '',
				'showThumbs'    => '',
				'showLinks'     => '',
				'isAdmin'       => '',
				'emailLanguage' => 'transaction_language',
			);

			$form['emails'][] = array(
				'showCustomer'  => '',
				'showContents'  => '',
				'showThumbs'    => '',
				'showLinks'     => '',
				'isAdmin'       => '',
				'emailLanguage' => 'transaction_language',
			);

			// Papers
			$form['papers']   = unserialize($mp['papers']);
			$form['papers'][] = array();
			$form['papers'][] = array();

			// Postage Rates
			$form['postages']   = unserialize($mp['postages']);
			$form['postages'][] = array(
				'price' => array(),
			);

			$form['postages'][] = array(
				'price' => array(),
			);

			if (!isset($form['cartredirect'])) {
				$form['cartredirect'] = 1;
			}

			if ($form['cartredirect'] == 1) {
				$form['cartredirect'] = true;
			} else {
				$form['cartredirect'] = false;
			}

			if (!isset($form['customPrices'])) {
				$form['customPrices'] = 1;
			}

			if ($form['customPrices'] == 1) {
				$form['customPrices'] = true;
			} else {
				$form['customPrices'] = false;
			}

			if (!isset($form['requireTandCs'])) {
				$form['requireTandCs'] = 0;
			}

			if ($form['requireTandCs'] == 1) {
				$form['requireTandCs'] = true;
			} else {
				$form['requireTandCs'] = false;
			}

			if (!isset($form['treatAlbumAsCollection'])) {
				$form['treatAlbumAsCollection'] = 0;
			}

			if ($form['treatAlbumAsCollection'] == 1) {
				$form['treatAlbumAsCollection'] = true;
			} else {
				$form['treatAlbumAsCollection'] = false;
			}

			if (!isset($form['alwaysShowAddToCart'])) {
				$form['alwaysShowAddToCart'] = 0;
			}

			if ($form['alwaysShowAddToCart'] == 1) {
				$form['alwaysShowAddToCart'] = true;
			} else {
				$form['alwaysShowAddToCart'] = false;
			}

			if (!isset($form['showCartPlugins'])) {
				$form['showCartPlugins'] = 0;
			}

			if ($form['showCartPlugins'] == 1) {
				$form['showCartPlugins'] = true;
			} else {
				$form['showCartPlugins'] = false;
			}

			if (!isset($form['selecttype']) || empty($form['selecttype'])) {
				$form['selecttype'] = 'combo';
			}

			// products section
			list($ret, $rootAlbumId) = GalleryCoreApi::getDefaultAlbumId();

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $products) = CheckoutProductMapHelper::fetchProducts($rootAlbumId, true, false);

			// Do use local settings, don't pass products to plugins to modify
			if ($ret) {
				return array($ret, null);
			}

			$displayPos = 1;

			foreach ($products as $product) {
				$form['products'][] = array(
					'id'              => $product['productId'],
					'name'            => $product['name'],
					'price'           => $product['price'],
					'description'     => $product['description'],
					'position'        => $displayPos++,
					'visible'         => key_exists('visible', $product) ? $product['visible'] : '',
					'appliesToItem'   => key_exists('appliesToItem', $product) ? $product['appliesToItem'] : '',
					'appliesToAlbum'  => key_exists('appliesToAlbum', $product) ? $product['appliesToAlbum'] : '',
					'chargePostage'   => key_exists('chargePostage', $product) ? $product['chargePostage'] : '',
					'postageBand'     => key_exists('postageBand', $product) ? $product['postageBand'] : '0',
					'paperSet'        => key_exists('paperSet', $product) ? $product['paperSet'] : '0',
					'initialQuantity' => key_exists('initialQuantity', $product) ? $product['initialQuantity'] : '',
					'quantityLimit'   => key_exists('quantityLimit', $product) ? $product['quantityLimit'] : '',
					'dropdown'        => key_exists('dropdown', $product) ? $product['dropdown'] : '',
				);
			}

			// Add four blank lines
			for ($i = -4; $i < 0; $i++) {
				$form['products'][] = array(
					'id'              => "$i",
					'name'            => '',
					'price'           => '',
					'description'     => '',
					'position'        => $displayPos++,
					'visible'         => 'true',
					'appliesToItem'   => 'true',
					'appliesToAlbum'  => 'false',
					'chargePostage'   => 'true',
					'postageBand'     => '',
					'paperSet'        => '',
					'initialQuantity' => '',
					'quantityLimit'   => '',
					'dropdown'        => 'false',
				);
			}

			// discounts
			list($ret, $discounts) = CheckoutDiscountMapHelper::fetchDiscounts();

			if ($ret) {
				return array($ret, null);
			}

			$form['discounts'] = $discounts;
			$displayPos        = count($discounts);

			// add two blank lines
			for ($i = -2; $i < 0; $i++) {
				$form['discounts'][] = array(
					'discountId'      => "$i",
					'priority'        => $displayPos++,
					'description'     => '',
					'entryCode'       => '',
					'applyDefault'    => 'false',
					'minValue'        => '',
					'minItems'        => '',
					'minProducts'     => '',
					'groupId'         => '',
					'discountPercent' => '',
					'discountFixed'   => '',
					'freePostage'     => 'false',
				);
			}

			if (!isset($form['askDiscountCode'])) {
				$form['askDiscountCode'] = 0;
			}

			if ($form['askDiscountCode'] == 1) {
				$form['askDiscountCode'] = true;
			} else {
				$form['askDiscountCode'] = false;
			}
		}

		// group list for discounts
		list($ret, $groupList) = GalleryCoreApi::fetchGroupNames();

		if ($ret) {
			return array($ret, null);
		}

		// language settings for emails
		$platform     =& $gallery->getPlatform();
		$translator   =& $gallery->getTranslator();
		$languageList = array();
		$canTranslate = false;

		if ($translator->canTranslate()) {
			$canTranslate = true;

			// Set up our language selection list
			$AdminCore['languageList'] = array();

			$supportedLanguages = $translator->getSupportedLanguages();

			foreach ($supportedLanguages as $language => $countryList) {
				foreach ($countryList as $country => $languageData) {
					$languageList[$language . '_' . $country] = $languageData['description'];
				}
			}
		}

		$template->setVariable('groupList', $groupList);
		$template->setVariable('languageList', $languageList);
		$template->setVariable('canTranslate', $canTranslate);
		$template->setVariable('controller', 'checkout.Admin');

		return array(
			null,
			array(
				'body' => 'modules/checkout/templates/Admin.tpl',
			),
		);
	}
}
