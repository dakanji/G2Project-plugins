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

// TODO: Make library includes absolute
ini_set('include_path', dirname(__FILE__) . '/lib:' . ini_get('include_path'));
GalleryCoreApi::requireOnce('modules/openid/lib/Auth/OpenID/CryptUtil.php');
GalleryCoreApi::requireOnce('modules/openid/lib/Auth/Yadis/Yadis.php');

/**
 * Show configuration warnings and settings to site admins.
 * @package OpenId
 * @subpackage UserInterface
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 1572 $
 */
class OpenIdSiteAdminController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$error = $status = array();

		if (isset($form['action']['addMapping'])) {
			list($ret, $user) = GalleryCoreApi::fetchUserByUserName($form['username']);

			if ($ret) {
				return array($ret, null);
			}
			$ret = GalleryCoreApi::addMapEntry(
				'OpenIdMap',
				array(
					'openId'    => $form['openId'],
					'galleryId' => $user->getId(),
				)
			);

			if ($ret) {
				return array($ret, null);
			}
		} elseif (isset($form['action']['save'])) {
			$status['saved'] = true;
		}

		return array(
			null,
			array(
				'redirect' => array(
					'view'    => 'core.SiteAdmin',
					'subView' => 'openid.OpenIdSiteAdmin',
				),
				'error'    => $error,
				'status'   => $status,
			),
		);
	}
}

/**
 * Show configuration warnings and settings to site admins.
 */
class OpenIdSiteAdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;
		$platform =& $gallery->getPlatform();

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'openid');

		if ($ret) {
			return array($ret, null);
		}

		$OpenIdSiteAdmin = array();

		$OpenIdSiteAdmin['randSource'] = Auth_OpenID_RAND_SOURCE;
		$OpenIdSiteAdmin['randHandle'] = @$platform->fopen($OpenIdSiteAdmin['randSource'], 'r');

		if ($OpenIdSiteAdmin['randHandle'] !== false) {
			$OpenIdSiteAdmin['expectedSize'] = 6;
			$OpenIdSiteAdmin['randData']     = $platform->fread($OpenIdSiteAdmin['randHandle'], $OpenIdSiteAdmin['expectedSize']);
			$stat                            = fstat($OpenIdSiteAdmin['randHandle']);
			$OpenIdSiteAdmin['randSize']     = $stat['size'];
			$platform->fclose($OpenIdSiteAdmin['randHandle']);
		}
		$OpenIdSiteAdmin['uname'] = php_uname();

		$OpenIdSiteAdmin['xmlExtension'] = Auth_Yadis_getXMLParser();

		global $__Services_Yadis_xml_extensions;
		$OpenIdSiteAdmin['xmlExtensions'] = $__Services_Yadis_xml_extensions;

		$OpenIdSiteAdmin['curlPresent'] = Auth_Yadis_Yadis::curlPresent();

		$fetcher                        = Auth_Yadis_Yadis::getHTTPFetcher();
		$OpenIdSiteAdmin['fetchUrl']    = 'http://www.openidenabled.com/resources/php-fetch-test';
		$OpenIdSiteAdmin['fetchResult'] = $fetcher->get($OpenIdSiteAdmin['fetchUrl']);
		$OpenIdSiteAdmin['expectedUrl'] = $OpenIdSiteAdmin['fetchUrl'] . '.txt';

		// Load our default values if we didn't just come from this form
		if ($form['formName'] != 'OpenIdSiteAdmin') {
			$form['formName'] = 'OpenIdSiteAdmin';
		}

		// Render HTML body
		$template->setVariable('OpenIdSiteAdmin', $OpenIdSiteAdmin);
		$template->setVariable('controller', 'openid.OpenIdSiteAdmin');

		return array(
			null,
			array(
				'body' => 'modules/openid/templates/OpenIdSiteAdmin.tpl',
			),
		);
	}
}
