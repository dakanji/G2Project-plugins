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
 * Module that create a login cookie for users.
 * @package LoginCookie
 * @author Dayo Akanji <http://dakanji.com>
 * @version $Revision: 0 $
 */
class LoginCookieModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('logincookie');
		$this->setName($gallery->i18n('Login Cookie'));
		$this->setDescription(
			$gallery->i18n(
				'Create User Login Cookie'
			)
		);

		// Update upgrade() as required
		$this->setVersion('1.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('gallery', $gallery->i18n('Gallery'));
		$this->setCallbacks('');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new LoginCookieModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEventListener',
			'LoginCookieModule',
			'LoginCookieModule',
			'modules/logincookie/module.inc',
			'logincookie',
			array('Gallery::Login', 'Gallery::Logout')
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * Event handler for the Gallery::Login and Gallery::Logout events.
	 *
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		switch ($event->getEventName()) {
			case 'Gallery::Login':
				$ret = $this->_LoginCookie('set');

				if ($ret) {
					return array($ret, null);
				}

				break;

			case 'Gallery::Logout':
				$ret = $this->_LoginCookie('unset');

				if ($ret) {
					return array($ret, null);
				}

				break;

			default:
				break;
		}

		return array(null, null);
	}

	/**
	 * Function to set and unset login cookies
	 * @access private
	 */
	public function _LoginCookie($action = 'set') {
		global $gallery;

		if ($action == 'set') {
			$session =& $gallery->getSession();
			$cookie  = 'Set-Cookie: GALLERYUID = ' . $session->getUserId();
		} else {
			$cookie = 'Set-Cookie: GALLERYUID =';
		}

		list($ret, $cookieDomain) = GalleryCoreApi::getPluginParameter('module', 'core', 'cookie.domain');

		if ($ret) {
			return array($ret, null);
		}

		if (!isset($cookieDomain)) {
			$cookieDomain = '';
		}

		$urlGenerator =& $gallery->getUrlGenerator();

		list($ret, $cookiePath) = $urlGenerator->getCookiePath();

		if ($ret) {
			return $ret;
		}

		list($ret, $sessionLifetime) = GalleryCoreApi::getPluginParameter('module', 'core', 'session.lifetime');

		if ($ret) {
			if ($ret->getErrorCode() & ERROR_STORAGE_FAILURE) {
				// During installation it is possible the database is not around yet.  Keep going.
				$sessionLifetime = 0;
			} else {
				return $ret;
			}
		}

		$phpVm = $gallery->getPhpVm();

		if ($action == 'unset') {
			// Expires in the past instructs the browser to delete the cookie
			$expirationDate = GalleryUtilities::getHttpDate($phpVm->time() - (365 * 24 * 3600));
			$cookie        .= '; expires=' . $expirationDate;
		} elseif ($sessionLifetime > 0) {
			$expirationDate = GalleryUtilities::getHttpDate($phpVm->time() + $sessionLifetime);
			$cookie        .= '; expires=' . $expirationDate;
		}

		// cookie path must always be set explicitly
		$cookie .= '; path=' . $cookiePath;

		if (!empty($cookieDomain)) {
			$cookie .= '; domain=' . $cookieDomain;
		}

		// Tag on the secure flag if HTTPS is in use
		if ($this->_isHttps()) {
			$cookie .= '; secure';
		}

		$cookie .= '; HttpOnly=1';

		if (!$phpVm->headers_sent()) {
			GalleryUtilities::setResponseHeader($cookie);
		}

		return null;
	}

	/**
	 * Determine if the request is using the HTTPS protocol.
	 * @return boolen true if the request is using HTTPS
	 * @access private
	 */
	public function _isHttps() {
		$httpType = GalleryUtilities::getServerVar('HTTPS');

		return isset($httpType)
			&& (GalleryUtilities::strToLower($httpType) == 'on'
			|| $httpType == 1
			|| $httpType === true);
	}
}
