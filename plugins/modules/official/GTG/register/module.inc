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
 * The self-registration module
 *
 * @package Registration
 * @author Sebastian Eichner <mailsp@sebastian-eichner.de>
 * @version $Revision: 18172 $
 */
class RegisterModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('register');
		$this->setName($gallery->i18n('Registration'));
		$this->setDescription(
			$gallery->i18n(
				'New User Registration'
			)
		);

		// Update upgrade() as required
		$this->setVersion('2.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('gallery', $gallery->i18n('Gallery'));
		$this->setCallbacks('getSystemLinks|getUserAdminViews|getSiteAdminViews');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new RegisterModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion, $x1 = null) {
		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'register');

		if ($ret) {
			return $ret;
		}

		foreach (array(
			'confirmation'     => 'admin',
			'emailadmins'      => 1,
			'emailusers'       => 1,
			'subject'          => $this->translate('Account activation'),
			'adminsubject'     => $this->translate('New user registration'),
			'usersubject'      => $this->translate('Welcome to Gallery'),
			'from'             => 'webmaster@' . GalleryUtilities::getServerVar('SERVER_NAME'),
			'validation.level' => 'HIGH',
		) as $key => $value) {
			if (!isset($params[$key])) {
				$ret = $this->setParameter($key, $value);

				if ($ret) {
					return $ret;
				}
			}
		}

		return null;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEntity',
			'GalleryPendingUser',
			'GalleryPendingUser',
			'modules/register/classes/GalleryPendingUser.class',
			'register',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CaptchaAdminOption',
			'RegisterCaptchaAdminOption',
			'RegisterCaptchaAdminOption',
			'modules/register/classes/RegisterCaptchaAdminOption.class',
			'register',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::getModuleEntityTypes
	 */
	public function getModuleEntityTypes() {
		return array('GalleryPendingUser');
	}

	/**
	 * @see GalleryModule::getSystemLinks
	 */
	public function getSystemLinks() {
		$links = array();

		list($ret, $isAnonymous) = GalleryCoreApi::isAnonymousUser();

		if ($ret) {
			return array($ret, null);
		}

		if ($isAnonymous) {
			$links['Register'] = array(
				'text'   => $this->translate('Register'),
				'params' => array(
					'view'    => 'core.UserAdmin',
					'subView' => 'register.UserSelfRegistration',
					'return'  => 1,
				),
			);
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::getUserAdminViews
	 */
	public function getUserAdminViews($user) {
		$views = array();

		list($ret, $isAnonymous) = GalleryCoreApi::isAnonymousUser($user->getId());

		if ($ret) {
			return array($ret, null);
		}

		if ($isAnonymous) {
			$views[] = array(
				'name' => $this->translate('Register'),
				'view' => 'register.UserSelfRegistration',
			);
		}

		return array(null, $views);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Registration'),
					'view' => 'register.AdminSelfRegistration',
				),
			),
		);
	}
}
