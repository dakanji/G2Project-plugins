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
 * This module sanitises and caches (if required) Gallery2 html output
 *
 * @package refine
 * @author Dayo Akanji <http://dakanji.com>
 */
class RefineModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('refine');
		$this->setName($gallery->i18n('Refine HTML'));
		$this->setDescription(
			$gallery->i18n(
				'Refine HTML Output'
			)
		);

		// Update upgrade() as required
		$this->setVersion('2.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('gallery', $gallery->i18n('Gallery'));
		$this->setCallbacks('getSiteAdminViews');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new RefineModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 *
	 * Refine path needs to be configured first
	 */
	public function needsConfiguration() {
		list($ret, $value) = $this->getParameter('refineMode');

		if ($ret) {
			return array('<h1>Refine Module needsConfiguration Error: 01</h1>' . $ret, null);
		}

		return array(null, empty($value));
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		list($ret, $needsConfiguration) = $this->needsConfiguration();

		if ($ret) {
			return array('<h1>Refine Module autoConfigure Error: 01</h1>' . $ret, null);
		}

		return array(null, !$needsConfiguration);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'refine.AdminRefine';
	}

	/**
	 * @see GalleryModule::activate
	 */
	public function activate() {
		global $gallery;

		list($ret, $moduleStatus) = GalleryCoreApi::fetchPluginStatus('module');

		if ($ret) {
			return array('<h1>Refine Module Activation Error: 01</h1><br>' . $ret, null);
		}

		if (isset($moduleStatus['joomg2']) && !empty($moduleStatus['joomg2']['active'])) {
			return array(
				'<h1>Refine Module Activation Error: 02</h1><br> '
				. 'This module cannot be activated while the JoomG2 module is active',
				null,
			);
		}

		if (isset($moduleStatus['g2drupal']) && !empty($moduleStatus['g2drupal']['active'])) {
			return array(
				'<h1>Refine Module Activation Error: 03</h1><br> '
				. 'This module cannot be activated while the G2Drupal module is active',
				null,
			);
		}

		list($ret, $redirect) = parent::activate();

		if ($ret) {
			return array('<h1>Refine Module Activation Error: 04</h1><br>' . $ret, null);
		}

		return array(null, $redirect);
	}

	/**
	 * @see GalleryModule::deactivate
	 */
	public function deactivate() {
		list($ret, $redirect) = parent::deactivate();

		if ($ret) {
			return array('<h1>Refine Module Deactivation Error: 01</h1><br>' . $ret, null);
		}

		return array(null, $redirect);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Refine HTML'),
					'view' => 'refine.AdminRefine',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEventListener',
			'RefineModule',
			'RefineModule',
			'modules/refine/module.inc',
			'refine',
			array('Gallery::BeforeOutput')
		);

		if ($ret) {
			return '<h1>Refine Module performFactoryRegistrations Error: 01</h1>' . $ret;
		}

		return null;
	}

	/**
	 * Event handler for the Gallery::BeforeOutput event.
	 * Modifies G2 HTML before output.
	 *
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		global $gallery;

		$templateData = $event->getData();

		list($ret, $refineMode) = GalleryCoreApi::getPluginParameter('module', 'refine', 'refineMode');

		if ($ret) {
			return array('<h1>Refine Module handleEvent Error: 01</h1>' . $ret, null);
		}

		GalleryCoreApi::requireOnce('modules/refine/classes/RefineHTML.class');

		$Refine   = new RefineHTML();
		$htmlData = $Refine->refine_output($templateData['html'], $refineMode);

		return array(null, $htmlData);
	}

	/**
	 * @see GalleryModule::upgrade()
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		list($ret, $moduleStatus) = GalleryCoreApi::fetchPluginStatus('module');

		if ($ret) {
			return '<h1>Refine Module Upgrade Error: 01</h1><br>' . $ret;
		}

		if (isset($moduleStatus['joomg2']) && !empty($moduleStatus['joomg2']['active'])) {
			return '<h1>Refine Module Upgrade Error: 02</h1><br>'
			. 'This module cannot be activated while the JoomG2 module is active';
		}

		if (isset($moduleStatus['g2drupal']) && !empty($moduleStatus['g2drupal']['active'])) {
			return '<h1>Refine Module Upgrade Error: 03</h1><br> '
			. 'This module cannot be activated while the G2Drupal module is active';
		}

		return null;
	}
}
