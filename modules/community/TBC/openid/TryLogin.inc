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
 * This controller will handle a user logging in to Gallery.  Begin the OpenID authentication
 * process.
 * @package OpenId
 * @subpackage UserInterface
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 1574 $
 */
class TryLoginController extends GalleryController {
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

		$results = array();
		$error   = array();

		if (isset($form['action']['login'])) {
			if (empty($form['openId'])) {
				$error[] = 'form[error][openId][missing]';
			} else {
				$consumer =& OpenIdHelper::getConsumer();

				// Begin the OpenID authentication process
				$request = $consumer->begin($form['openId']);

				if (!isset($request)) {
					$error[] = 'form[error][openId][invalid]';
				}
			}

			if (empty($error)) {
				/*
				 * TODO: Probably need to have this check only in FinishLogin.. the given
				 * openId may not be the actual identity URL for this user.. for example,
				 * for Yahoo! openid you can enter "yahoo.com" and we don't get the particular
				 * user's identity URL until returning from the provider.
				 */

				list($ret, $userId) = OpenIdHelper::fetchGalleryIdByOpenId($form['openId']);

				if ($ret) {
					if (!($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
						return array($ret, null);
					}

					/*
					 * Check for interface provided by Register module, so we can add new users
					 * with approval policy selected in that module's configuration.
					 * Note: RegisterApi not yet implemented in register module as of Gallery 2.3!
					 */

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
						/*
						 * TODO: Where do Gallery installations document their identity information
						 * policy?
						 */
						$sregRequest = Auth_OpenID_SRegRequest::build(
							OPENID_SREG_REQUIRED,
							OPENID_SREG_OPTIONAL
						);

						if ($sregRequest) {
							$request->addExtension($sregRequest);
						}
					}
				}

				if (empty($error)) {
					// Ensure we have a persistent session before URL generation (need authToken)
					$session = $gallery->getSession();

					$ret = $session->start();

					if ($ret) {
						return array($ret, null);
					}

					/*
					 * Redirect the user to the OpenID server for authentication.  Store the token
					 * for this authentication so we can verify the response.
					 * Sample code says: For OpenID 1, send a redirect.  For OpenID 2, use a
					 * Javascript form to send a POST request to the server.
					 * Here, always redirect (even if $request->shouldSendRedirect() is false)
					 */
					$trustRoot = $urlGenerator->generateUrl(
						array(),
						array(
							'forceFullUrl'   => true,
							'htmlEntities'   => false,
							'forceSessionId' => false,
						)
					);

					$returnTo = $urlGenerator->generateUrl(
						array(
							'controller' => 'openid.FinishLogin',
							'form'       => $form,
						),
						array(
							'forceFullUrl' => true,
							'htmlEntities' => false,
						)
					);

					$redirect = $request->redirectURL($trustRoot, $returnTo);

					if (Auth_OpenID::isFailure($redirect)) {
						return array(
							GalleryStatus::error(
								ERROR_UNKNOWN,
								'Could not redirect: ' . $redirect->message
							),
							null,
						);
					}

					$results['redirect'] = array(
						'href' => $redirect,
					);

					// Set cookie to remember OpenID for future logins
					if (!empty($form['setCookie'])) {
						setcookie('OpenIDForGallery', $form['openId'], time() + 90 * 24 * 3600);
					}
				}
			}
		} else /* isset($form['action']['cancel']) */ {
			$results['return'] = true;
		}

		if (!empty($error)) {
			$results['delegate']['view']    = 'core.UserAdmin';
			$results['delegate']['subView'] = 'openid.UserLogin';
		}

		$results['status'] = array();
		$results['error']  = $error;

		return array(null, $results);
	}
}
