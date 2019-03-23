<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
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
 * OpenID Module.  Login and register with an actually distributed identity system.
 * @package OpenId
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 1578 $
 */
class OpenIdModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('openid');
		$this->setName($gallery->i18n('OpenID'));
		$this->setDescription(
			$gallery->i18n(
				'Login with a distributed identity system (unfinished module)'
			)
		);

		// Update upgrade() as required
		$this->setVersion('1.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('authentication', $gallery->i18n('Authentication'));
		$this->setCallbacks('getSystemLinks|getSiteAdminViews|registerEventListeners');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new OpenIdModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryOption',
			'OpenIdOption',
			'OpenIdOption',
			'modules/openid/OpenIdOption.inc',
			'openid',
			array('UserPreferences', 'UserSelfRegistration')
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
		$ret = GalleryCoreApi::registerEventListener(
			'GalleryEntity::delete',
			new OpenIdModule()
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerEventListener(
			'GalleryPendingUser::createGalleryUser',
			new OpenIdModule()
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion) {
		switch ($currentVersion) {
			case null:
				// Initial install
				break;

			case '0.0.1':
				// Add OpenIdMap
			case '0.0.2':
				// Add GalleryPendingUser::createGalleryUser event listener
			case '0.0.3':
				// Add GalleryEntity::delete event listener
			case '0.0.4':
				// Add site admin view
			case '0.0.5':
				// Add OpenID option to UserSelfRegistration
			case '0.0.6':
				// Add getSystemLinks for Login with OpenID link
			case '0.0.7':
			case '1.0.0':
				// Updated GalleryCoreApi requirement to 8.0
				// Upgraded GalleryModule requirement to 4.0
			case 'end of upgrade path':
				/*
				 * Leave this bogus case at the end of the legitimate case statements
				 * so that we always properly terminate our upgrade path with a break.
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
	}

	/**
	 * @see GalleryModule::getSystemLinks
	 */
	public function getSystemLinks() {
		global $gallery;

		$links = array();

		if ($gallery->getConfig('login')) {
			list($ret, $isAnonymous) = GalleryCoreApi::isAnonymousUser();

			if ($ret) {
				return array($ret, null);
			}

			if ($isAnonymous) {
				// Do not add separate Login link if default login is configured for OpenID
				$loginRedirect = $gallery->getConfig('loginRedirect');

				if (empty($loginRedirect['subView'])
					|| $loginRedirect['subView'] !== 'openid.UserLogin'
				) {
					$links['OpenID'] = array(
						'text'   => $this->translate('Login with OpenID'),
						'params' => array(
							'view'    => 'core.UserAdmin',
							'subView' => 'openid.UserLogin',
							'return'  => 1,
						),
					);
				}
			}
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate($this->getName()),
					'view' => 'openid.OpenIdSiteAdmin',
				),
			),
		);
	}

	/**
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		switch ($event->getEventName()) {
			case 'GalleryEntity::delete':
				$entity = $event->getEntity();

				if (GalleryUtilities::isA($entity, 'GalleryUser')
					|| GalleryUtilities::isA($entity, 'GalleryPendingUser')
				) {
					GalleryCoreApi::requireOnce('modules/openid/classes/OpenIdHelper.class');

					// Fetch unused Gallery ids
					list($ret, $galleryIds) = OpenIdHelper::fetchUnusedGalleryIds();

					if ($ret) {
						return array($ret, null);
					}

					// Remove corresponding OpenIDs
					$galleryIds[] = $entity->getId();

					$ret = GalleryCoreApi::removeMapEntry(
						'OpenIdMap',
						array(
							'galleryId' => $galleryIds,
						)
					);

					if ($ret) {
						return array($ret, null);
					}
				}

				break;

			case 'GalleryPendingUser::createGalleryUser':
				$pendingUser = $event->getEntity();

				// Fetch real user
				list($ret, $user) = GalleryCoreApi::fetchUserByUserName($pendingUser->getUserName());

				if ($ret) {
					return array($ret, null);
				}

				// Change OpenID mapping from pending user to real user
				$ret = GalleryCoreApi::updateMapEntry(
					'OpenIdMap',
					array(
						'galleryId' => $pendingUser->getId(),
					),
					array(
						'galleryId' => $user->getId(),
					)
				);

				if ($ret) {
					return array($ret, null);
				}

				break;
		}

		return array(null, null);
	}
}
