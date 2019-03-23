<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

/**
 * @version $Revision: 1200 $
 * @package CheckoutAuthorizeNet
 * @author Michael Rodr�guez-Torrent <mike@themikecam.com>
 */

/**
 * @package CheckoutAuthorizeNet
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

		$error = $status = array();

		if (isset($form['action']['save'])) {
			if (empty($form['authorizeNetId'])) {
				$error[] = 'form[error][authorizeNetId][bad]';
			}

			if (empty($form['tranKey'])) {
				$error[] = 'form[error][tranKey][bad]';
			}

			$platform = $gallery->getPlatform();

			// Check that the specified cURL binary exists
			if (!$platform->file_exists($form['curlPath'])
				|| !$platform->is_executable($form['curlPath'])
			) {
				$error[] = 'form[error][curlPath][bad]';
			}

			if (empty($form['fromEmail'])
				|| !GalleryUtilities::isValidEmailString($form['fromEmail'])
			) {
				$error[] = 'form[error][fromEmail][bad]';
			}

			if (empty($form['storeEmail'])
				|| !GalleryUtilities::isValidEmailString($form['storeEmail'])
			) {
				$error[] = 'form[error][storeEmail][bad]';
			}

			if (empty($error)) {
				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'checkoutauthorizenet',
					'authorizeNetId',
					$form['authorizeNetId']
				);

				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'checkoutauthorizenet',
					'tranKey',
					$form['tranKey']
				);

				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'checkoutauthorizenet',
					'mode',
					$form['mode']
				);

				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'checkoutauthorizenet',
					'curlPath',
					$form['curlPath']
				);

				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'checkoutauthorizenet',
					'fromEmail',
					$form['fromEmail']
				);

				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'checkoutauthorizenet',
					'emailSubject',
					$form['emailSubject']
				);

				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'checkoutauthorizenet',
					'storeEmail',
					$form['storeEmail']
				);

				if ($ret) {
					return array($ret, null);
				}

				list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkoutauthorizenet');

				if ($ret) {
					return array($ret, null);
				}

				list($ret, $isActive) = $module->isActive();

				if ($ret) {
					return array($ret, null);
				}

				$redirect['view'] = 'core.SiteAdmin';

				if ($isActive) {
					$redirect['subView'] = 'checkoutauthorizenet.Admin';
					$status['saved']     = 1;
				} else {
					$redirect['subView']  = 'core.AdminModules';
					$status['configured'] = 'checkoutauthorizenet';
				}
			}
		} elseif (isset($form['action']['reset'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'checkoutauthorizenet.Admin';
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'core.AdminModules';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'checkoutauthorizenet.Admin';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * @package CheckoutAuthorizeNet
 * @subpackage UserInterface
 */
class AdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'AdminCheckoutAuthorizeNet') {
			$form['formName'] = 'AdminCheckoutAuthorizeNet';

			list($ret, $form['authorizeNetId']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutauthorizenet',
				'authorizeNetId'
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['tranKey']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutauthorizenet',
				'tranKey'
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['mode']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutauthorizenet',
				'mode'
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['curlPath']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutauthorizenet',
				'curlPath'
			);

			if ($ret) {
				return array($ret, null);
			}

			// A default to get the user started.
			if (empty($form['curlPath'])) {
				$form['curlPath'] = '/usr/bin/curl';
			}

			list($ret, $form['fromEmail']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutauthorizenet',
				'fromEmail'
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['emailSubject']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutauthorizenet',
				'emailSubject'
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['storeEmail']) = GalleryCoreApi::getPluginParameter(
				'module',
				'checkoutauthorizenet',
				'storeEmail'
			);

			if ($ret) {
				return array($ret, null);
			}
		}

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkoutauthorizenet');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $isActive) = $module->isActive();

		if ($ret) {
			return array($ret, null);
		}

		if (!$isActive) {
			$AdminCheckout['isConfigure'] = true;
		}

		$AdminCheckout['modes'] = array(
			'TEST' => 'Test',
			'LIVE' => 'Production',
		);

		$template->setVariable('AdminCheckout', $AdminCheckout);
		$template->setVariable('controller', 'checkoutauthorizenet.Admin');

		return array(
			null,
			array(
				'body' => 'modules/checkoutauthorizenet/templates/Admin.tpl',
			),
		);
	}
}
