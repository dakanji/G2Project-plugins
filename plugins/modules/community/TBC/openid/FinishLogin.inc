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
GalleryCoreApi::requireOnce('modules/openid/classes/OpenIdHelper.class');

/**
 * This controller will handle a user logging in to Gallery.  Complete the authentication process
 * using the server's response.
 * @package OpenId
 * @subpackage UserInterface
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 1578 $
 */
class FinishLoginController extends GalleryController {
	/**
	 * ValidationPlugin instances to use when handling this request.  Only used by test code.
	 * @var array $_plugins (array of GalleryValidationPlugin)
	 * @access private
	 */
	public $_plugins;

	/**
	 * Tests can use this method to hardwire a specific set of plugin instances to use.  This avoids
	 * situations where some of the option instances will do unpredictable things and derail the
	 * tests.
	 * @param array $plugins of GalleryValidationPlugin
	 */
	public function setPlugins($plugins) {
		$this->_plugins = $plugins;
	}

	/**
	 * @see GalleryController::isAllowedInMaintenance
	 */
	public function isAllowedInMaintenance() {
		return true;
	}

	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$urlGenerator =& $gallery->getUrlGenerator();
		$returnTo     = $urlGenerator->generateUrl(
			array(
				'controller' => 'openid.FinishLogin',
				'form'       => $form,
			),
			array(
				'forceFullUrl' => true,
				'htmlEntities' => false,
			)
		);
		$results      = $status = $error = array();

		// Complete the authentication using the server's response
		$consumer       =& OpenIdHelper::getConsumer();
		$response       = $consumer->complete($returnTo);
		$responseStatus = empty($response->status) || empty($response->identity_url) ? '' : $response->status;

		if ($responseStatus != Auth_OpenID_SUCCESS) {
			$error[] = 'form[error][openId][error]';

			if (!empty($response->status)) {
				$results['delegate']['responseStatus'] = $response->status;
			}

			if (!empty($response->message)) {
				$results['delegate']['responseMessage'] = $response->message;
			}
		}

		if ($responseStatus != Auth_OpenID_CANCEL) {
			// Prepare for validation
			$options = array(
				'pass' => $responseStatus == Auth_OpenID_SUCCESS,
			);

			list($ret, $options['level']) = GalleryCoreApi::getPluginParameter('module', 'core', 'validation.level');

			if ($ret) {
				return array($ret, null);
			}

			if ($options['level'] == 'MEDIUM') {
				$options['key'] = 'openid.UserLogin.' . $response->identity_url;
			}

			if ($options['level'] == 'OFF') {
				$plugins = array();
			} elseif (isset($this->_plugins)) {
				$plugins = $this->_plugins;
			} else {
				list($ret, $plugins) = GalleryCoreApi::getAllFactoryImplementationIds('GalleryValidationPlugin');

				if ($ret) {
					return array($ret, null);
				}

				foreach (array_keys($plugins) as $pluginId) {
					list($ret, $plugins[$pluginId]) = GalleryCoreApi::newFactoryInstanceById(
						'GalleryValidationPlugin',
						$pluginId
					);

					if ($ret) {
						return array($ret, null);
					}
				}
			}

			// Let each plugin do its verification
			foreach ($plugins as $plugin) {
				list($ret, $pluginErrors, $continue) = $plugin->performValidation($form, $options);

				if ($ret) {
					return array($ret, null);
				}

				$error = array_merge($error, $pluginErrors);

				if (!$continue) {
					break;
				}
			}
		}

		if (empty($error)) {
			list($ret, $userId) = OpenIdHelper::fetchGalleryIdByOpenId($response->identity_url);

			if ($ret) {
				if (!($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
					return array($ret, null);
				}

				list($ret, $registerApi) = GalleryCoreApi::newFactoryInstance('RegisterApi');

				if ($ret) {
					return array($ret, null);
				}

				if (!isset($registerApi)) {
					$error[] = 'form[error][openId][no_register]';
				}

				if (empty($error)) {
					list($ret, $isCompatible) = $registerApi->isCompatibleWithApi(array(1, 0));

					if ($ret) {
						return array($ret, null);
					}

					if (!$isCompatible) {
						$error[] = 'form[error][openId][no_register]';
					}
				}

				if (empty($error)) {
					$sregResponse = Auth_OpenID_SRegResponse::fromSuccessResponse($response);
					$sreg         = $sregResponse->contents();
					$data         = array();

					foreach (array(
						'userName' => 'nickname',
						'fullName' => 'fullname',
						'email'    => 'email',
					) as $key => $sregKey) {
						if (empty($sreg[$sregKey])) {
							$error[] = 'form[error][' . $key . '][missing]';
						} else {
							$data[$key] = $sreg[$sregKey];
						}
					}

					if (!empty($sreg['language'])) {
						$data['language'] = $sreg['language'];
					}
				}

				if (empty($error)) {
					list($ret, $user, $results) = $registerApi->registerNewUser($data);

					if ($ret) {
						if (!($ret->getErrorCode() & ERROR_COLLISION)) {
							return array($ret, null);
						}

						// Set our error status and fall back to the view
						/*
						 * TODO: What should the user do in this case?  Change their OpenID profile
						 * nickname?
						 */
						$error[]            = 'form[error][userName][exists]';
						$status['userName'] = $data['userName'];
					}
				}

				if (empty($error)) {
					$ret = GalleryCoreApi::addMapEntry(
						'OpenIdMap',
						array(
							'openId'    => $response->identity_url,
							'galleryId' => $user->getId(),
						)
					);

					if ($ret) {
						return array($ret, null);
					}
				}
			} else {
				list($ret, $user) = GalleryCoreApi::loadEntitiesById($userId);

				if ($ret) {
					return array($ret, null);
				}

				list($ret, $isDisabled) = GalleryCoreApi::isDisabledUsername($user->getUsername());

				if ($ret) {
					return array($ret, null);
				}

				if ($isDisabled) {
					$error[] = 'form[error][userName][disabled]';
				}
			}
		}

		/*
		 * 4 end states from above code:
		 * (1) $error is non-empty, (2) $user set to a GalleryUser,
		 * (3) registerApi returned a GalleryPendingUser and non-empty $results
		 * (4) mapping existed but loaded entity is a GalleryPendingUser
		 *     (ie, user logged in with OpenID before, but account not approved yet)
		 */
		if (empty($error) && GalleryUtilities::isA($user, 'GalleryUser')) {
			$gallery->setActiveUser($user);
			$event = GalleryCoreApi::newEvent('Gallery::Login');

			$event->setEntity($user);

			list($ret, $redirect) = GalleryCoreApi::postEvent($event);

			if ($ret) {
				return array($ret, null);
			}

			$session =& $gallery->getSession();
			$ret     = $session->regenerate();

			if ($ret) {
				return array($ret, null);
			}

			// Redirect if requested by event listener, otherwise return
			if (!empty($redirect)) {
				$results['redirect'] = array_shift($redirect);
			} elseif (!empty($form['returnUrl'])) {
				$results['redirect'] = array(
					'href' => $urlGenerator->makeAbsoluteUrl($form['returnUrl']),
				);
			} else {
				$results['return'] = true;
			}
		} elseif (empty($error) && !empty($results)) {
			return array(null, $results);
		} else {
			$results['delegate']['view']    = 'core.UserAdmin';
			$results['delegate']['subView'] = 'openid.UserLogin';

			if (empty($error)) {
				$error[] = 'form[error][userName][pending]';
			}
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}
