<?php

/*
 * $RCSfile: module.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
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
 * @version $Revision: 0.9 $ $Date: 2005/07/23 22:24:00 $
 * @package DupeDetect
 * @author Aviad Tsherniak <Aviad.Tsherniak@gmail.com>
 */

/**
 * The implementation of the DupeDetect module
 *
 * @package DupeDetect
 */
class DupeDetectModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('dupedetect');
		$this->setName($gallery->i18n('DupeDetect'));
		$this->setDescription(
			$gallery->i18n(
				'DupeDetect'
			)
		);
		$this->setVersion('1.0.0');

		// Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;
		$this->setGroup('gallery', $this->translate('Gallery'));
		$this->setCallbacks('registerEventListeners|getSiteAdminViews');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new DupeDetectModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::registerEventListeners()
	 */
	public function registerEventListeners() {
		GalleryCoreApi::requireOnce('modules/dupedetect/classes/DupeDetectHelper.class');

		$listener = new DupeDetectHelper();

		GalleryCoreApi::registerEventListener('GalleryEntity::save', $listener, true);
		GalleryCoreApi::registerEventListener('GalleryEntity::delete', $listener, true);
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'MaintenanceTask',
			'DupeDetectRecreateDupInfoTask',
			'DupeDetectRecreateDupInfoTask',
			'modules/dupedetect/classes/DupeDetectRecreateDupInfoTask.class',
			'dupedetect',
			null,
			2
		);

		if ($ret->isError()) {
			return $ret;
		}

		return GalleryStatus::success();
	}

	/**
	 * @see GalleryModule::isRecommendedDuringInstall()
	 */
	public function isRecommendedDuringInstall() {
		return false;
	}

	/**
	 * @see GalleryModule::autoConfigure()
	 */
	public function autoConfigure() {
		return array(GalleryStatus::success(), true);
	}

	/**
	 * @see GalleryModule::needsConfiguration()
	 */
	public function needsConfiguration() {
		return array(GalleryStatus::success(), false);
	}

	/**
	 * @see GalleryModule::activate()
	 */
	public function activate() {
		list($ret, $redirect) = parent::activate();

		if ($ret->isError()) {
			return array($ret, null);
		}

		return array(GalleryStatus::success(), $redirect);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews()
	 */
	public function getSiteAdminViews() {
		return array(
			GalleryStatus::success(),
			array(
				array(
					'name' => $this->translate('DupeDetect'),
					'view' => 'dupedetect.DupeDetectShowDupes',
				),
			),
		);
	}
}
