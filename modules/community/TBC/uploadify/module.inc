<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2010 Bharat Mediratta
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
 * @package Uploadify
 * @author Wayne Patterson <suprsidr@flashyourweb.com>
 */
class UploadifyModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('uploadify');
		$this->setName($gallery->i18n('Uploadify'));
		$this->setDescription(
			$gallery->i18n(
				'Easy to use jQuery uploader for media'
			)
		);

		$this->setVersion('2.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('import', $gallery->i18n('Import'));
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new UploadifyModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryPlugin::upgrade
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters(
			'module',
			'uploadify'
		);

		if ($ret) {
			return $ret;
		}

		// Set default plugin parameters here

		return null;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemAddPlugin',
			'ItemAddUploadify',
			'
			ItemAddUploadify',
			'modules/uploadify/ItemAddUploadify.inc',
			'uploadify',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}
}
