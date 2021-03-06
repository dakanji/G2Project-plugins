<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
 * This module assists in setting up an alternate guest view of a Gallery
 * that is displayed via a different URL.
 *
 * @package Multiroot
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 18172 $
 */
class MultirootModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('multiroot');
		$this->setName($gallery->i18n('Multiroot'));
		$this->setDescription(
			$gallery->i18n(
				'Define an alternate guest view of the Gallery'
			)
		);

		// Update upgrade() as required
		$this->setVersion('2.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('display', $this->translate('Display'));
		$this->setCallbacks('');
	}

	/**
	 * Class Loader
	 */
	public static function getme() {
		static $instance;

		if (!isset($instance)) {
			$instance = new MultirootModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'multiroot.ConfigureMultiroot';
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 */
	public function needsConfiguration() {
		return array(null, true);
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		return array(null, false);
	}
}
