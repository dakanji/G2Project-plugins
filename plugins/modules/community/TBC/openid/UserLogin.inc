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
 * This view prompts for login information.
 * @package OpenId
 * @subpackage UserInterface
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 1574 $
 */
class UserLoginView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		if ($form['formName'] != 'UserLogin') {
			$form['formName']  = 'UserLogin';
			$form['openId']    = '';
			$form['setCookie'] = false;

			/*
			 * When logging in we don't have a session yet, thus no navigation history / a place
			 * to store the returnUrl.  Thus store the returnUrl in the login form.
			 */
			$returnUrl         = GalleryUtilities::getRequestVariables('return');
			$form['returnUrl'] = !empty($returnUrl) ? $returnUrl : '';

			/*
			 * If just entering login process (not here via delegate from TryLogin/FinishLogin,
			 * then check for cookie with user's OpenID.. if found, redirect to TryLogin.
			 */
			$openIdCookie = GalleryUtilities::getCookieVar('OpenIDForGallery');
			if (!empty($openIdCookie)) {
				$form['openId']          = $openIdCookie;
				$form['setCookie']       = 1;
				$form['action']['login'] = 1;
				return array(
					null,
					array(
						'redirect' => array(
							'controller' => 'openid.TryLogin',
							'form'       => $form,
						),
					),
				);
			}
		} else {
			if (!isset($form['setCookie'])) {
				$form['setCookie'] = false;
			}

			if (isset($form['error']['openId']['error'])) {
				foreach (array('responseStatus', 'responseMessage') as $key) {
					// Check for values set in FinishLogin controller
					$template->setVariable($key, GalleryUtilities::getRequestVariables($key));
				}
			}
		}

		// Render the HTML body
		$template->setVariable('controller', 'openid.TryLogin');
		$template->style('modules/openid/data/openid.css');
		return array(
			null,
			array(
				'body' => 'modules/openid/templates/UserLogin.tpl',
			),
		);
	}
}
