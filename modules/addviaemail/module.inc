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
 *//**
 * Add via Email
 *
 * @package Addviaemail
 * @author Brian Janice <cobalt027@users.sourceforge.net>
 * @version $Revision: 897 $
 */
class AddviaemailModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('addviaemail');
		$this->setName($gallery->i18n('Add via Email'));
		$this->setDescription($gallery->i18n('Add via Email module'));
		$this->setVersion('0.7.3');
		// $this->setCallbacks('getSiteAdminViews|getItemLinks');
		$this->setCallbacks('getSiteAdminViews');
		$this->setGroup('import', $gallery->i18n('Import'));
		$this->setRequiredCoreApi(array(7, 0));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * @see GalleryModule::getItemLinks
	 */
	public function getItemLinks($items, $wantsDetailedLinks, $permissions) {
		$links = array();

		foreach ($items as $item) {
			$params['view']          = 'addviaemail.Addviaemail';
			$params['itemId']        = $item->getId();
			$links[$item->getId()][] = array(
				'text'   => $this->translate('Add via Email'),
				'params' => $params,
			);
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 */
	public function needsConfiguration() {
		GalleryCoreApi::requireOnce('modules/addviaemail/classes/AddviaemailAdminHelper.class');
		$funcsReport = AddviaemailAdminHelper::testRequiredFunctions();

		if (count($funcsReport['fail']) > 0) {
			return array(null, true);
		}

		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'addviaemail');

		if ($ret) {
			return array($ret, null);
		}

		$myParameters = array(
			'serverAddress',
			'serverType',
			'userName',
			'passWord',
			'subjectToMatch',
			'tmpLocation',
			'owner',
			'deleteProcessed',
			'destinationAlbum',
		);

		foreach ($myParameters as $key) {
			if (empty($params[$key])) {
				return array(null, true);
			}
		}

		return array(null, false);
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		// We need to perform a test before activating
		list($ret, $needsConfiguration) = $this->needsConfiguration();

		if ($ret) {
			return array($ret, null);
		}

		return array(null, !$needsConfiguration);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'addviaemail.Configure';
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Add via Email'),
					'view' => 'addviaemail.Configure',
				),
			),
		);
	}
}
