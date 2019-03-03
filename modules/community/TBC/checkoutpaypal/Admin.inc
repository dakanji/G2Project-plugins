<?php
/*
 * $RCSfile: Admin.inc,v $
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
 * @package CheckoutPaypal
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 */

/**
 * @package CheckoutPaypal
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
			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutpaypal', 'active', $form['active']);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutpaypal', 'paypalname', $form['paypalname']);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutpaypal', 'ccode', $form['ccode']);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutpaypal', 'imageUrl', $form['imageUrl']);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutpaypal', 'ppurl', $form['ppurl']);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkoutpaypal', 'pprtnbuttontext', $form['pprtnbuttontext']);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkoutpaypal');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $isActive) = $module->isActive();

			if ($ret) {
				return array($ret, null);
			}

			$redirect['view'] = 'core.SiteAdmin';

			if ($isActive) {
				$redirect['subView'] = 'checkoutpaypal.Admin';
				$status['saved']     = 1;
			} else {
				$redirect['subView']  = 'core.AdminModules';
				$status['configured'] = 'Checkout by Paypal';
			}
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'checkoutpaypal.Admin';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'checkoutpaypal.Admin';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * @package checkoutpaypal
 * @subpackage UserInterface
 */
class AdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'AdminCheckoutPaypal') {
			$form['formName'] = 'AdminCheckoutPaypal';

			list($ret, $form['active']) = GalleryCoreApi::getPluginParameter('module', 'checkoutpaypal', 'active');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['paypalname']) = GalleryCoreApi::getPluginParameter('module', 'checkoutpaypal', 'paypalname');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['ccode']) = GalleryCoreApi::getPluginParameter('module', 'checkoutpaypal', 'ccode');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['imageUrl']) = GalleryCoreApi::getPluginParameter('module', 'checkoutpaypal', 'imageUrl');

			if ($ret) {
				return array($ret, null);
			}

			if (!isset($form['ccode'])) {
				$form['ccode'] = 'USD';
			}

			list($ret, $form['ppurl']) = GalleryCoreApi::getPluginParameter('module', 'checkoutpaypal', 'ppurl');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['pprtnbuttontext']) = GalleryCoreApi::getPluginParameter('module', 'checkoutpaypal', 'pprtnbuttontext');

			if ($ret) {
				return array($ret, null);
			}
		}

		$template->setVariable('controller', 'checkoutpaypal.Admin');

		return array(
			null,
			array(
				'body' => 'modules/checkoutpaypal/templates/Admin.tpl',
			),
		);
	}
}
