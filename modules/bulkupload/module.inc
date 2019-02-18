<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
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
 * @package BulkUpload
 * @version $Revision: 13541 $ $Date: 2006-04-27 09:58:35 -0700 (Thu, 27 Apr 2006) $
 * @author Bharat Mediratta <bharat@menalto.com>
 * This module provides toolkit support to extract items from
 * bulk files such as a zip file
 */
class BulkUploadModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('bulkupload');
		$this->setName($gallery->i18n('Bulk Upload'));
		$this->setDescription($gallery->i18n('Upload files from structured data files'));
		$this->setVersion('1.0.1');
		$this->setGroup('import', $gallery->i18n('Import'));
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
			$singleton = new BulkUploadModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemAddPlugin',
			'ItemAddBulk',
			'ItemAddBulk',
			'modules/bulkupload/ItemAddBulk.inc',
			'bulkupload',
			null
		);

		if ($ret) {
			return $ret;
		}
	}
}
