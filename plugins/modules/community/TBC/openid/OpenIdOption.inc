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
 *
 * @package OpenId
 * @subpackage UserInterface
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 904 $
 */
class OpenIdOption extends GalleryOption {
	/**
	 * @see GalleryOption::handleRequest
	 */
	public function handleRequest($form, $name, $data) {
		$ret = GalleryCoreApi::removeMapEntry(
			'OpenIdMap',
			array(
				'galleryId' => $data['user']->getId(),
			)
		);

		if ($ret) {
			return array($ret, null);
		}

		foreach ($form['openIds'] as $openId) {
			if (!empty($openId)) {
				$ret = GalleryCoreApi::addMapEntry(
					'OpenIdMap',
					array(
						'openId'    => $openId,
						'galleryId' => $data['user']->getId(),
					)
				);

				if ($ret) {
					return array($ret, null);
				}
			}
		}

		return array(null, array());
	}

	/**
	 * @see GalleryOption::loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $name, $data) {
		global $gallery;

		if ($form['formName'] != $name) {
			if (!empty($data['openId'])) {
				$form['openIds']   = array();
				$form['openIds'][] = $data['openId'];
			}

			if (!empty($data['user'])) {
				list($ret, $form['openIds']) = OpenIdHelper::fetchOpenIdsByGalleryId($data['user']->getId());

				if ($ret) {
					return array($ret, null, null);
				}
			}
		}

		// Render the HTML body
		$template->style('modules/openid/data/openid.css');
		return array(null, 'modules/openid/templates/OpenIdOption.tpl', 'modules_openid');
	}

	/**
	 * @see GalleryOption::isAppropriate
	 */
	public function isAppropriate($name, $data) {
		switch ($name) {
			case 'UserPreferences':
				// Always true
				return array(null, true);
			case 'UserSelfRegistration':
				// Only true if already authenticated using OpenID
				if (empty($data['openId'])) {
					return array(null, false);
				}

				return array(null, true);
		}

		return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
	}
}
