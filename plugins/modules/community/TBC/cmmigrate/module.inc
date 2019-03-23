<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
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
 * CmMigrate Module
 *
 * This module provides support for migrating from Coppermine
 *
 * @package CmMigrate
 * @author David Angier <david@angier.co.uk>
 * @version $Revision: 847 $
 */
class CmMigrateModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('cmmigrate');
		$this->setName($gallery->i18n('Coppermine Migration'));
		$this->setDescription(
			$gallery->i18n(
				'Migrate your Coppermine Gallery to Gallery 2'
			)
		);

		// Update upgrade() as required
		$this->setVersion('1.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('import', $gallery->i18n('Import'));
		$this->setCallbacks('getSiteAdminViews');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new CmMigrateModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Coppermine'),
					'view' => 'cmmigrate.SelectCmAlbum',
				),
			),
		);
	}
}
