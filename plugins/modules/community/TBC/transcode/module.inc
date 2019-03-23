<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2012 Bharat Mediratta
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
 * Transcode video files.
 *
 * @package Transcode
 * @author Don Willingham <donwillingham@users.sf.net>
 * @author Wayne Patterson <suprsidr@flashyourweb.com>
 * @version $Revision:$
 */
class TranscodeModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('transcode');
		$this->setName($gallery->i18n('Transcode'));
		$this->setDescription(
			$gallery->i18n(
				'Transcode video files'
			)
		);
		$this->setVersion('1.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('display', $gallery->i18n('Display'));
		$this->setCallbacks('getSiteAdminViews|registerEventListeners');
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemEditPlugin',
			'ItemEditTranscode',
			'ItemEditTranscode',
			'modules/transcode/ItemEditTranscode.inc',
			'transcode',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemAddOption',
			'TranscodeOnUploadOption',
			'TranscodeOnUploadOption',
			'modules/transcode/TranscodeOnUploadOption.inc',
			'transcode',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryRenderer',
			'TranscodeVideoRenderer',
			'TranscodeVideoRenderer',
			'modules/transcode/classes/TranscodeVideoRenderer.class',
			'transcode',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::registerEventListeners
	 */
	public function registerEventListeners() {
		$TranscodeModule = new TranscodeModule();
		GalleryCoreApi::registerEventListener(
			'Gallery::DeactivatePlugin',
			$TranscodeModule
		);
	}

	/**
	 * @see GalleryModule::activate
	 */
	public function activate($postActivationEvent = true) {
		list($ret, $movie) = $this->_getSupportedMovieTypes();

		if ($ret) {
			return array($ret, null);
		}

		if (empty($movie)) {
			// Can't activate unless there's already toolkit support
			return array(
				null,
				array(
					'view'    => 'core.SiteAdmin',
					'subView' => 'transcode.CantTranscode',
				),
			);
		}

		$ret = $this->setParameter('movieTypes', implode('|', $movie));

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $redirect) = parent::activate($postActivationEvent);

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $redirect);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'transcode.CantTranscode';
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Transcode'),
					'view' => 'transcode.AdminTranscode',
				),
			),
		);
	}

	/**
	 * Find out what mime types currently have support for:
	 *   convert-to-video/x-flv
	 *
	 * @return array object GalleryStatus a status code
	 *               array mime types
	 */
	public function _getSupportedMovieTypes() {
		list($ret, $mimeTypes) = GalleryCoreApi::getToolkitOperationMimeTypes(
			'convert-to-video/x-flv'
		);

		if ($ret) {
			return array($ret, null);
		}

		return array(null, array_keys($mimeTypes));
	}

	/**
	 * Handler for Gallery::DeactivatePlugin event.
	 *
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		/*
		 * We are going to deactivate this module which will generate another
		 * Gallery::DeactivatePlugin event, so make sure that we do not handle
		 * that event and avoid an infinite loop!
		 */
		$data = $event->getData();

		if ($event->getEventName() == 'Gallery::DeactivatePlugin'
			&& $data['pluginType'] == 'module'
			&& $data['pluginId'] != 'transcode'
		) {
			list($ret, $isActive) = $this->isActive();

			if ($ret) {
				return array($ret, null);
			}

			if ($isActive) {
				// Reactivate may fail if toolkit support no longer found.
				list($ret, $redirect) = $this->reactivate();

				if ($ret) {
					return array($ret, null);
				}
			}
		}

		return array(null, null);
	}

	/**
	 * @see GalleryPlugin::uninstall
	 */
	public function uninstall() {
		$ret = GalleryCoreApi::deleteRenderer('TranscodeVideoRenderer');

		if ($ret) {
			return $ret;
		}

		return parent::uninstall();
	}
}
