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
 *
 * @package CheckoutEmail
 * @version $Revision: 1583 $
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * @package CheckoutEmail
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
			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutemail',
				'paymentText',
				$form['paymentText']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutemail',
				'paymentButtonText',
				$form['paymentButtonText']
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'checkoutemail',
				'active',
				$form['active']
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkoutemail');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $isActive) = $module->isActive();

			if ($ret) {
				return array($ret, null);
			}

			$redirect['view'] = 'core.SiteAdmin';
			if ($isActive) {
				$redirect['subView'] = 'checkoutemail.Admin';
				$status['saved']     = 1;
			} else {
				$redirect['subView']  = 'core.AdminModules';
				$status['configured'] = 'Checkout by Email';
			}
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'checkoutemail.Admin';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'checkoutemail.Admin';
		}

		$results['status'] = $status;
		$results['error']  = $error;
		return array(null, $results);
	}
}

/**
 * @package CheckoutEmail
 * @subpackage UserInterface
 */
class AdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'AdminCheckoutEmail') {
			$form['formName'] = 'AdminCheckoutEmail';

			list($ret, $form['paymentText']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutemail',
				'paymentText'
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['paymentButtonText']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutemail',
				'paymentButtonText'
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['active']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutemail',
				'active'
			);

			if ($ret) {
				return array($ret, null);
			}
		}

		$template->setVariable('controller', 'checkoutemail.Admin');

		return array(
			null,
			array(
				'body' => 'modules/checkoutemail/templates/Admin.tpl',
			),
		);
	}
}
