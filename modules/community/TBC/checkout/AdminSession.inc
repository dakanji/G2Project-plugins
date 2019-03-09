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
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutHelper.class');

/**
 * @package Checkout
 * @version $Revision: 1718 $
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * View Checkout Transactions
 */
class AdminSessionController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest()
	 */
	public function handleRequest($form) {
		global $gallery;

		$error   = array();
		$status  = array();
		$results = array();

		if (isset($form['action']['savechanges']) || isset($form['action']['emptycart'])
			|| isset($form['action']['addmine'])
		) {
			GalleryCoreApi::requireOnce('modules/core/classes/GallerySession.class');

			$mySession = new GallerySession();

			list($ret, $searchResults) = GalleryCoreApi::getMapEntry(
				'GallerySessionMap',
				array(
					'userId',
					'remoteIdentifier',
					'creationTimestamp',
					'modificationTimestamp',
					'data',
				),
				array(
					'id' => $form['sessId'],
				)
			);

			if ($ret) {
				return array($ret, null);
			}

			$pSession                     = $searchResults->nextResult();
			$mySession->_sessionId        = $form['sessId'];
			$mySession->_userId           = (int)$pSession[0];
			$mySession->_remoteIdentifier = unserialize($pSession[1]);
			$mySession->_creationTime     = (int)$pSession[2];
			$mySession->_modificationTime = (int)$pSession[3];
			$mySession->_sessionData      = unserialize($pSession[4]);
			$mySession->_isPersistent     = true;

			// Save or Clear
			if (isset($form['action']['savechanges']) || isset($form['action']['addmine'])) {
				$items = CheckoutHelper::parseForm($form);

				if ($ret) {
					return array($ret, null);
				}

				if (isset($form['action']['addmine'])) {
					list($ret, $myItems) = CheckoutHelper::loadCheckoutItemsFromSession();

					// Why no sensible array merge for arrays with numeric indices?
					foreach ($myItems as $myKey => $myItem) {
						$items[$myKey] = $myItem;
					}

					if ($ret) {
						return array($ret, null);
					}
				}

				$mySession->put('checkout.items', $items);
				$status['saved'] = 1;
			}

			if (isset($form['action']['emptycart'])) {
				$mySession->remove('checkout.items');
				$status['cleared'] = 1;
			}

			$mySession->save(true);
			$results['redirect']['view']    = 'core.SiteAdmin';
			$results['redirect']['subView'] = 'checkout.AdminSession';
			$results['redirect']['sessId']  = $form['sessId'];
		} elseif (isset($form['action']['discardchanges'])) {
			$results['redirect']['view']    = 'core.SiteAdmin';
			$results['redirect']['subView'] = 'checkout.AdminSession';
			$results['redirect']['sessId']  = $form['sessId'];
			$status['discarded']            = 1;
		} elseif (isset($form['action']['return'])) {
			$results['redirect']['view']    = 'core.SiteAdmin';
			$results['redirect']['subView'] = 'checkout.AdminSessions';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

class AdminSessionView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'Session') {
			$form['formName'] = 'Session';
			$form['sessId']   = GalleryUtilities::getRequestVariables('sessId');

			if (empty($form['sessId'])) {
				return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
			}
		}

		GalleryCoreApi::requireOnce('modules/core/classes/GallerySession.class');

		$mySession = new GallerySession();

		list($ret, $results) = GalleryCoreApi::getMapEntry(
			'GallerySessionMap',
			array(
				'id',
				'userId',
				'remoteIdentifier',
				'creationTimestamp',
				'modificationTimestamp',
				'data',
			),
			array(
				'id' => $form['sessId'],
			)
		);

		if ($ret) {
			return $ret;
		}

		$userNames                     = array();
		$pSession                      = array();
		$pSession                      = $results->nextResult();
		$mySession->_userId            = (int)$pSession[1];
		$mySession->_remoteIdentifier  = unserialize($pSession[2]);
		$mySession->_creationTime      = (int)$pSession[3];
		$mySession->_modificationTime  = (int)$pSession[4];
		$mySession->_sessionData       = unserialize($pSession[5]);
		$mySession->_loadedSessionData = $mySession->_getSerializedSession();
		$hostName                      = gethostbyaddr($mySession->_remoteIdentifier[0]);
		$host                          = $hostName ? $hostName : $mySession->_remoteIdentifier[0];
		$transactionId                 = $mySession->get('checkout.transactionId');

		if (ereg(';', $transactionId)) {
			$transactionId = unserialize($transactionId);
		}

		$userId  = $mySession->getuserId();
		$session = array(
			'id'                => $pSession[0],
			'userId'            => $userId,
			'sessionModTime'    => $mySession->getModificationTime(),
			'sessionCreateTime' => $mySession->getCreationTime(),
			'cartModTime'       => $mySession->get('checkout.cartLastModified'),
			'host'              => $host,
			'transactionId'     => $transactionId,
			'returnURL'         => $mySession->get('checkout.returnURL'),
		);

		// Get user name
		list($ret, $user) = GalleryCoreApi::loadEntitiesById($userId, 'GalleryUser');

		if ($ret) {
			return array($ret, null);
		}

		$session['userName'] = $user->getFullName();

		list($ret, $tpl) = GalleryCoreApi::fetchAllPluginParameters('module', 'checkout');

		if ($ret) {
			return array($ret, null);
		}

		$tpl['askDiscountCode'] = true;

		// get the items from the required session
		$rawItems = $mySession->get('checkout.items');

		if (is_null($rawItems)) {
			$rawItems = array();
		}

		list($ret, $items) = CheckoutHelper::fetchCheckoutItems($rawItems);

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

		// build table of available products for each item in the cart
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
				if (!isset($item[$thisProduct['productId']]['quant'])
					&& $thisProduct['dropdown'] == 'true'
				) {
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
					&& $product['chargePostage'] == 'true'
				) {
					$offerPostage = true;
				}
			}
		}

		if ($offerPostage) {
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

		$tpl['items']        = $items;
		$tpl['thumbnails']   = $thumbnails;
		$tpl['itemProducts'] = $itemProducts;
		$tpl['controller']   = 'checkout.Confirm';

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkout');

		if ($ret) {
			return array($ret, null);
		}

		// Hack to allow the euro symbol to be used when using the Gallery2 bridge component for Joomla (to overcome UTF encoding problem)
		if ($tpl['csymbol'] == 'e') {
			$tpl['csymbol'] = '&euro;';
		}

		$tpl['itemCount'] = count($items);

		$template->setVariable('tpl', $tpl);
		$template->setVariable('session', $session);
		$template->setVariable('controller', 'checkout.AdminSession');

		return array(
			null,
			array(
				'body' => 'modules/checkout/templates/AdminSession.tpl',
			),
		);
	}
}
