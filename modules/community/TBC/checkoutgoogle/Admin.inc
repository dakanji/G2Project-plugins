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
  * @package CheckoutGoogle
  * @subpackage UserInterface
  * @author Alec Myers <alec@alecmyers.com>
  * @version $Revision: 1687 $
  */

 /**
  * Required classes
  */
GalleryCoreApi::requireOnce('modules/checkoutgoogle/classes/CheckoutGoogleResponseHelper.class');

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
			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'active', $form['active']);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'gcMerchantId', trim($form['gcMerchantId']));

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'gcMerchantKey', trim($form['gcMerchantKey']));

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'gcTestMerchantId', trim($form['gcTestMerchantId']));

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'gcTestMerchantKey', trim($form['gcTestMerchantKey']));

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'gcMode', $form['gcMode']);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'gcCurrency', $form['gcCurrency']);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'gcLocation', $form['gcLocation']);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'gcCallbackURL', trim($form['gcCallbackURL']));

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutgoogle', 'overrideURL', isset($form['overrideURL']) ? 1 : 0);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkoutgoogle');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $isActive) = $module->isActive();

			if ($ret) {
				return array($ret, null);
			}

			$redirect['view'] = 'core.SiteAdmin';

			if ($isActive) {
				$redirect['subView'] = 'checkoutgoogle.Admin';
				$status['saved']     = 1;
			} else {
				$redirect['subView']  = 'core.AdminModules';
				$status['configured'] = 'Checkout by Google';
			}
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'checkoutgoogle.Admin';
		} elseif (isset($form['action']['test'])) {
			// Send Test Notification
			$testXML = '<?xml version="1.0" encoding="UTF-8"?><gallery-test><google-order-number>0</google-order-number></gallery-test>';
			$testURL = $form['gcCallbackURL'] . '?g2_view=checkoutgoogle.Response';

			list($success, $body) = CheckoutGoogleResponseHelper::SendData($testXML, $testURL);

			if ($success && ($body == 'OK')) {
				$status['testSuccessful'] = 1;
				$status['testResult']     = $testURL;
			} else {
				$status['testFailed'] = 1;
				$status['testResult'] = $body;
			}

			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'checkoutgoogle.Admin';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'checkoutgoogle.Admin';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

class AdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'AdminCheckoutGoogle') {
			$form['formName'] = 'AdminCheckoutGoogle';

			list($ret, $form['active']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'active');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['gcMerchantId']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'gcMerchantId');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['gcMerchantKey']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'gcMerchantKey');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['gcTestMerchantId']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'gcTestMerchantId');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['gcTestMerchantKey']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'gcTestMerchantKey');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['gcMode']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'gcMode');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['gcCurrency']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'gcCurrency');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['gcLocation']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'gcLocation');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['gcCallbackURL']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'gcCallbackURL');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['overrideURL']) = GalleryCoreApi::getPluginParameter('module', 'checkoutgoogle', 'overrideURL');

			if ($ret) {
				return array($ret, null);
			}
		}

		$template->setVariable('controller', 'checkoutgoogle.Admin');

		return array(
			null,
			array(
				'body' => 'modules/checkoutgoogle/templates/Admin.tpl',
			),
		);
	}
}
