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
 * /**
 * @package CheckoutDownload
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1690 $
 */

/**
 * Required classes
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutProductMapHelper.class');

/**
 * @package CheckoutDownload
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
			$storage =& $gallery->getStorage();

			// write products here
			// note: Admin page values are all owned by the root album
			list($ret, $rootAlbumId) = GalleryCoreApi::getDefaultAlbumId();

			if ($ret) {
				return array($ret, null);
			}

			foreach ($form['products'] as $productId => $productParameters) {
				// Make sure that tick-box fields are either "true" or "false"
				foreach (array('download') as $booleanParam) {
					if (!isset($productParameters[$booleanParam])) {
						$productParameters[$booleanParam] = 'false';
					}
				}

				foreach (array('downloadSize') as $textParam) {
					// Remove leading trailing white space, and if nothing left, unset it
					$productParameters[$textParam] = trim($productParameters[$textParam]);

					if ($productParameters[$textParam] == '') {
						unset($productParameters[$textParam]);

						$ret = CheckoutProductMapHelper::removeParam(
							$productId,
							$textParam,
							$rootAlbumId
						);

						if ($ret) {
							return array($ret, null);
						}
					}
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

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutdownload',
				'daysDLAvailable',
				$form['daysDLAvailable']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutdownload',
				'numDLsAllowed',
				$form['numDLsAllowed']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutdownload',
				'zipfilename',
				$form['zipfilename']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['allowDownloadsPrePurchase'])) {
				$form['allowDownloadsPrePurchase'] = 0;
			} else {
				$form['allowDownloadsPrePurchase'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutdownload',
				'allowDownloadsPrePurchase',
				$form['allowDownloadsPrePurchase']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['limitDaysDLAvailable'])) {
				$form['limitDaysDLAvailable'] = 0;
			} else {
				$form['limitDaysDLAvailable'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutdownload',
				'limitDaysDLAvailable',
				$form['limitDaysDLAvailable']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['limitNumDLsAllowed'])) {
				$form['limitNumDLsAllowed'] = 0;
			} else {
				$form['limitNumDLsAllowed'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutdownload',
				'limitNumDLsAllowed',
				$form['limitNumDLsAllowed']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['useFoldersInDL'])) {
				$form['useFoldersInDL'] = 0;
			} else {
				$form['useFoldersInDL'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutdownload',
				'useFoldersInDL',
				$form['useFoldersInDL']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['useUniqueDLFilename'])) {
				$form['useUniqueDLFilename'] = 0;
			} else {
				$form['useUniqueDLFilename'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutdownload',
				'useUniqueDLFilename',
				$form['useUniqueDLFilename']
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['canAddAlbum'])) {
				$form['canAddAlbum'] = 0;
			} else {
				$form['canAddAlbum'] = 1;
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutdownload',
				'canAddAlbum',
				$form['canAddAlbum']
			);

			if ($ret) {
				return array($ret, null);
			}

			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'checkoutdownload.Admin';
			$status['saved']     = 1;
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'checkoutdownload.Admin';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'checkoutdownload.Admin';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * @package CheckoutDownload
 * @subpackage UserInterface
 */
class AdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'AdminCheckoutDownload') {
			// Get module options
			list($ret, $form) = GalleryCoreApi::fetchAllPluginParameters(
				'module',
				'checkoutdownload'
			);

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['allowDownloadsPrePurchase'])) {
				$form['allowDownloadsPrePurchase'] = 0;
			}

			if ($form['allowDownloadsPrePurchase'] == 1) {
				$form['allowDownloadsPrePurchase'] = true;
			} else {
				$form['allowDownloadsPrePurchase'] = false;
			}

			if (!isset($form['allowDownloadsPostPurchase'])) {
				$form['allowDownloadsPostPurchase'] = 0;
			}

			if ($form['allowDownloadsPostPurchase'] == 1) {
				$form['allowDownloadsPostPurchase'] = true;
			} else {
				$form['allowDownloadsPostPurchase'] = false;
			}

			if (!isset($form['allowDownloadOriginal'])) {
				$form['allowDownloadOriginal'] = 0;
			}

			if (!isset($form['limitDaysDLAvailable'])) {
				$form['limitDaysDLAvailable'] = 0;
			}

			if ($form['limitDaysDLAvailable'] == 1) {
				$form['limitDaysDLAvailable'] = true;
			} else {
				$form['limitDaysDLAvailable'] = false;
			}

			if (!isset($form['limitNumDLsAllowed'])) {
				$form['limitNumDLsAllowed'] = 0;
			}

			if ($form['limitNumDLsAllowed'] == 1) {
				$form['limitNumDLsAllowed'] = true;
			} else {
				$form['limitNumDLsAllowed'] = false;
			}

			if (!isset($form['useFoldersInDL'])) {
				$form['useFoldersInDL'] = 0;
			}

			if ($form['useFoldersInDL'] == 1) {
				$form['useFoldersInDL'] = true;
			} else {
				$form['useFoldersInDL'] = false;
			}

			if (!isset($form['useUniqueDLFilename'])) {
				$form['useUniqueDLFilename'] = 0;
			}

			if ($form['useUniqueDLFilename'] == 1) {
				$form['useUniqueDLFilename'] = true;
			} else {
				$form['useUniqueDLFilename'] = false;
			}

			if (!isset($form['canAddAlbum'])) {
				$form['canAddAlbum'] = 0;
			}

			if ($form['canAddAlbum'] == 1) {
				$form['canAddAlbum'] = true;
			} else {
				$form['canAddAlbum'] = false;
			}

			$form['formName'] = 'AdminCheckoutDownload';

			// products section
			list($ret, $rootAlbumId) = GalleryCoreApi::getDefaultAlbumId();

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $products) = CheckoutProductMapHelper::fetchProducts(
				$rootAlbumId,
				true,
				false
			);

			//do use local settings, don't pass products to plugins to modify
			if ($ret) {
				return array($ret, null);
			}

			foreach ($products as $product) {
				$form['products'][] = array(
					'id'                => $product['productId'],
					'name'              => $product['name'],
					'download'          => key_exists('download', $product) ? $product['download'] : '',
					'downloadSize'      => key_exists('downloadSize', $product) ? $product['downloadSize'] : '',
					'downloadOperation' => key_exists('downloadOperation', $product) ? $product['downloadOperation'] : '',
				);
			}
		}

		$template->setVariable('controller', 'checkoutdownload.Admin');

		return array(
			null,
			array(
				'body' => 'modules/checkoutdownload/templates/Admin.tpl',
			),
		);
	}
}
