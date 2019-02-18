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
 * @package User Info
 * @author Charles Knowlton charles2007@users.sourceforge.net
 * @version $Revision$
 *//**
 * User Info
 *
 * Logs Login, Logout, and Failed Login Attempts
 * Logs the User ID(if registered user), User Name(if registered user), IP Address, Date and time, and Action(Login, Logout, Failed Login)
 *
 * @package Userinfo
 */
class UserinfoModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('userinfo');
		$this->setName($gallery->i18n('User Info'));
		$this->setDescription($gallery->i18n('Displays User Information'));
		$this->setVersion('7.0.0');
		$this->setCallbacks('getSiteAdminViews');
		$this->setGroup('data', $gallery->i18n('Extra Data'));
		$this->setRequiredCoreApi(array(7, 0));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		if (!isset($currentVersion)) {
			$currentVersion = '0';
		}

		switch ($currentVersion) {
			case '0':
				// Initial install.
				/*
				 * Added new table in version 1.0.3 and need some default data put into it
				 * Adding default data into the newly created table(UserInfoSetup)
				 */
				$rowId               = '1';
				$uiPerPage           = '25';
				$uiDefaultSortColumn = 'timeStamp';
				$uiDefaultSortOrder  = 'DES';
				$uiDateFormat        = '%c';

				$ret = GalleryCoreApi::addMapEntry(
					'UserinfoSetupMap',
					array(
						'rowId'               => $rowId,
						'uiPerPage'           => $uiPerPage,
						'uiDefaultSortColumn' => $uiDefaultSortColumn,
						'uiDefaultSortOrder'  => $uiDefaultSortOrder,
						'uiDateFormat'        => $uiDateFormat,
					)
				);

				if ($ret) {
					return array($ret, null);
				}

				break;

			case '1.0.0':
			case '1.0.1':
			case '1.0.2':
				/*
				 * Added new table in version 1.0.3 and need some default data put into it
				 * Adding default data into the newly created table(UserInfoSetup)
				 */
				$rowId               = '1';
				$uiPerPage           = '25';
				$uiDefaultSortColumn = 'timeStamp';
				$uiDefaultSortOrder  = 'DES';
				$uiDateFormat        = '%c';

				$ret = GalleryCoreApi::addMapEntry(
					'UserinfoSetupMap',
					array(
						'rowId'               => $rowId,
						'uiPerPage'           => $uiPerPage,
						'uiDefaultSortColumn' => $uiDefaultSortColumn,
						'uiDefaultSortOrder'  => $uiDefaultSortOrder,
						'uiDateFormat'        => $uiDateFormat,
					)
				);

				if ($ret) {
					return array($ret, null);
				}

				// Fall Through
			case '1.0.3':
			case 'end of upgrade path':
				/*
				 * Leave this bogus case at the end of the legitimate case statements so that we
				 * always properly terminate our upgrade path with a break.
				 */
				break;

			default:
				return GalleryCoreApi::error(
					ERROR_BAD_PLUGIN,
					__FILE__,
					__LINE__,
					sprintf('Unknown module version %s', $currentVersion)
				);
		}

		return null;
	}

	/**
	 * @see GalleryModule::registerEventListeners
	 * Only works on 2.2 and below
	 */
	/*
	 *   function registerEventListeners() {
	 *   GalleryCoreApi::requireOnce('modules/userinfo/classes/MyEventHandlerClass.class');
	 *   $listener = new MyEventHandlerClass();
	 *   GalleryCoreApi::registerEventListener('Gallery::Login', $listener);
	 *   GalleryCoreApi::registerEventListener('Gallery::FailedLogin', $listener);
	 *   GalleryCoreApi::registerEventListener('Gallery::Logout', $listener);
	 *   }
	 */

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 * Only works on 2.3(svn) and above
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEventListener',
			'MyEventHandlerClass',
			'MyEventHandlerClass',
			'modules/userinfo/classes/MyEventHandlerClass.class',
			'userinfo',
			array('Gallery::Login', 'Gallery::Logout', 'Gallery::FailedLogin'),
			4
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	// Added this to show the module in the admin menu

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('User Info'),
					'view' => 'userinfo.Userinfo',
				),
			),
		);
	}
}
