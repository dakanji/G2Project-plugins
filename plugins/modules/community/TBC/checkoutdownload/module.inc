<?php

/*
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
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @version $Revision: 1664 $
 * @package CheckoutDownload
 * @author Alec Myers <alec@alecmyers.com>
 */
class CheckoutDownloadModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('checkoutdownload');
		$this->setName($gallery->i18n('Checkout Downloads'));
		$this->setDescription(
			$gallery->i18n(
				'Allows customers to download files from Checkout transactions'
			)
		);

		$this->setVersion('1.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;
		$this->setGroup('commerce', $gallery->i18n('Commerce'));
		$this->setCallbacks('getSiteAdminViews');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new CheckoutDownloadModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		// Register our plugins
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutStatusPageInterface_1_0',
			'CheckoutDownloadStatusPlugin',
			'checkoutDownloadStatus',
			'modules/checkoutdownload/classes/CheckoutDownloadStatusPlugin.class',
			'checkoutdownload',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutEmailInterface_1_0',
			'CheckoutDownloadEmailPlugin',
			'CheckoutDownloadEmail',
			'modules/checkoutdownload/classes/CheckoutDownloadEmailPlugin.class',
			'checkoutdownload',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::upgrade()
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		if (!empty($currentVersion)) {
			switch ($currentVersion) {
				case '0.1.0':
				case '0.1.1':
				case '0.1.2':
				case 'end of upgrade path':
					break;
				default:
					return GalleryCoreApi::error(ERROR_BAD_PLUGIN, sprintf('Unknown module version %s', $currentVersion));
			}
		}

		return null;
		if (!isset($currentVersion)) {
			// Initial install.
		}

		return null;
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		global $gallery;

		return array(
			null,
			array(
				array(
					'name' => $gallery->i18n('Checkout Downloads'),
					'view' => 'checkoutdownload.Admin',
				),
			),
		);
	}
}
