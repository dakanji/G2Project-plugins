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
  * CustomDerivative module
  * @version $Revision: 1507 $
  * @package CustomDerivatives
  * @author Alec Myers <alec@alecmyers.com>
  */class CustomDerivativesModule extends GalleryModule {
 	public function __construct() {
 		global $gallery;

 		$this->setId('customderivatives');
 		$this->setName($gallery->i18n('Customize Derivatives'));
 		$this->setDescription($gallery->i18n("Alter Derivatives' Operations"));
 		$this->setVersion('0.1.0');
 		$this->setGroup('display', $gallery->i18n('Display'));
 		$this->setCallbacks('');
 		$this->setRequiredCoreApi(array(7, 10));
 		$this->setRequiredModuleApi(array(3, 2));
 	}

 	/**
 	 * @see GalleryModule::performFactoryRegistrations()
 	 */
 	public function performFactoryRegistrations() {

		// Register our item edit plugin
 		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemEditPlugin',
			'DerivativesItemEdit',
			'DerivativesItemEdit',
			'modules/customderivatives/DerivativesItemEdit.inc',
			'customderivatives',
			null
		);

 		if ($ret) {
 			return $ret;
 		}

 		return null;
 	}
 }
