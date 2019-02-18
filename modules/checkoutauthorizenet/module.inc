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

/**
 * Checkout plugin to process payments through Authorize.Net
 * @author Michael Rodriguez-Torrent <mike@themikecam.com>
 * @package CheckoutAuthorizeNet
 */
class CheckoutAuthorizeNetModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('checkoutauthorizenet');
		$this->setName('Checkout by Authorize.Net');
		$this->setDescription($gallery->i18n('Complete checkout by Authorize.Net'));
		$this->setVersion('7.0.0');
		$this->setGroup('commerce', $gallery->i18n('Commerce'));
		$this->setCallbacks('getSiteAdminViews');
		$this->setRequiredCoreApi(array(7, 2));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * Load as Singleton
	 */
	public static function doStatic() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new CheckoutAuthorizeNetModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		if (empty($currentVersion)) {
			$currentVersion = '0';
		}

		switch ($currentVersion) {
			case '0':
			case '0.1.0':
				// Fix AuthorizeNetAim.class:109 direct call of cURL, now uses $this->_curlPath

			case 'end of upgrade path':
				break;

			default:
				return GalleryCoreApi::error(
					ERROR_BAD_PLUGIN,
					__FILE__,
					__LINE__,
					sprintf('Unknown module version %s', $currentVersion)
				);
		}

		return null;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		// Register our plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutPluginInterface_1_0',
			'CheckoutAuthorizeNetPlugin',
			'checkoutauthorizenet',
			'modules/checkoutauthorizenet/classes/CheckoutAuthorizeNetPlugin.class',
			'checkoutauthorizenet',
			null
		);

		if ($ret) {
			return $ret->wrap(__FILE__, __LINE__);
		}

		return null;
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 */
	public function needsConfiguration() {
		list($ret, $value) = $this->getParameter('authorizeNetId');

		if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		}

		return array(null, empty($value));
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Checkout by Authorize.Net'),
					'view' => 'checkoutauthorizenet.Admin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'checkoutauthorizenet.Admin';
	}
}
