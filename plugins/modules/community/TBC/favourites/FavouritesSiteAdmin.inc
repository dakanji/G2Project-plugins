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
 * Settings for Favourites Album
 * @package Favourites
 * @author Alec Myers <alec@alecmyers.com>
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 1371 $
 */
class FavouritesSiteAdminController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$status = $error = array();

		if (isset($form['action']['displayFromText'])) {
			// look for userName first, then fullName if not found
			$user   = null;
			$userId = null;

			list($ret, $user) = GalleryCoreApi::fetchUserByUserName($form['text']['userName']);

			if ($ret && !($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
				return array($ret, null);
			}

			if ($user != null) {
				$userId = $user->getId();
			} else {
				GalleryCoreApi::requireOnce('modules/favourites/classes/FavouritesMapHelper.class');

				list($ret, $userId) = FavouritesMapHelper::fetchUserIdByFullName($form['text']['userName']);

				if ($ret && !($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
					return array($ret, null);
				}
			}

			if ($userId != null) {
				$results = array(
					'redirect' => array(
						'view'         => 'favourites.FavouritesAlbum',
						'targetUserId' => $userId,
					),
					'status'   => array(),
					'error'    => array(),
				);
			} else {
				$results = array(
					'delegate' => array(
						'view'    => 'core.SiteAdmin',
						'subView' => 'favourites.FavouritesSiteAdmin',
					),
					'error'    => array('form[error][notFound]'),
					'status'   => array(),
				);
			}

			return array(null, $results);
		}

		if (isset($form['action']['save']) && empty($error)) {
			list($ret, $themeSettingsId) = GalleryCoreApi::getPluginParameter('module', 'favourites', 'themeSettingsId');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $error, $status) = GalleryCoreApi::handleThemeSettingsRequest(
				$form['currentThemeId'],
				$themeSettingsId,
				$form
			);

			if ($ret) {
				return array($ret, null);
			}

			$status = empty($status) ? array() : array(
				'statusMessage' => $status,
			);
		}

		if (isset($form['action']['save']) && empty($error)) {
			// textual data
			foreach (array('themeId')

			as $key) {
				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'favourites',
					$key,
					$form[$key]
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			// yes/no data
			foreach (array('offerToGuests') as $key) {
				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'favourites',
					$key,
					empty($form[$key]) ? 0 : 1
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			$status['saved'] = 1;
		}

		// windup
		$method  = empty($error) ? 'redirect' : 'delegate';
		$results = array(
			$method  => array(
				'view'    => 'core.SiteAdmin',
				'subView' => 'favourites.FavouritesSiteAdmin',
			),
			'status' => $status,
			'error'  => $error,
		);

		return array(null, $results);
	}
}

/**
 * Settings for Favourites Albums
 */
class FavouritesSiteAdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'FavouritesSiteAdmin') {
			foreach (array('themeId', 'offerToGuests')

 as $key) {
				list($ret, $form[$key]) = GalleryCoreApi::getPluginParameter('module', 'favourites', $key);

				if ($ret) {
					return array($ret, null);
				}
			}
		} else {
			// Reset theme selection on validation error for theme settings
			$form['themeId'] = $form['currentThemeId'];
		}

		// fetch all users with favourites
		GalleryCoreApi::requireOnce('modules/favourites/classes/FavouritesMapHelper.class');

		list($ret, $results) = FavouritesMapHelper::fetchUsersWithFavourites();

		if ($ret) {
			return array($ret, null);
		}

		$form['list']['users'] = $results;

		if (!isset($form['list']['page']) || $form['list']['page'] < 1) {
			$form['list']['page'] = 1;
		}

		$perPage                  = 20;
		$totalUserCount           = count($form['list']['users']);
		$form['list']['count']    = $totalUserCount;
		$form['list']['pageSize'] = $totalUserCount > $perPage ? $perPage : $totalUserCount;

		// Figure out our max pages, make sure our current page fits in it
		$form['list']['maxPages'] = ceil($totalUserCount / $perPage);
		$form['list']['page']     = min($form['list']['maxPages'], $form['list']['page']);

		// Calculate the next/back pages
		$form['list']['nextPage'] = min($form['list']['page'] + 1, $form['list']['maxPages']);
		$form['list']['backPage'] = max(1, $form['list']['page'] - 1);

		// Calculate which names to show
		$form['list']['start'] = ($form['list']['page'] - 1) * $perPage;

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'favourites');

		if ($ret) {
			return array($ret, null);
		}

		// Set up theme selection list
		$themeList = array(
			'' => $module->translate('&laquo; default theme &raquo;'),
		);

		list($ret, $themeStatus) = GalleryCoreApi::fetchPluginStatus('theme');

		if ($ret) {
			return array($ret, null);
		}

		foreach ($themeStatus as $id => $status) {
			if (!empty($status['active'])) {
				list($ret, $theme) = GalleryCoreApi::loadPlugin('theme', $id);

				if ($ret) {
					return array($ret, null);
				}

				$themeList[$id] = $theme->translate($theme->getName());
			}
		}

		// Set up theme settings
		list($ret, $themeSettingsId) = GalleryCoreApi::getPluginParameter('module', 'favourites', 'themeSettingsId');

		if ($ret) {
			return array($ret, null);
		}

		$ret = GalleryCoreApi::loadThemeSettingsForm(
			$form['themeId'],
			$themeSettingsId,
			$template,
			$form
		);

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'FavouritesSiteAdmin') {
			$form['formName']         = 'FavouritesSiteAdmin';
			$form['text']['userName'] = '';
		}

		$template->setVariable(
			'FavouritesSiteAdmin',
			array(
				'themeList'   => $themeList,
				'defaultList' => $defaultList,
			)
		);

		$template->setVariable('controller', 'favourites.FavouritesSiteAdmin');

		return array(
			null,
			array(
				'body' => 'modules/favourites/templates/FavouritesSiteAdmin.tpl',
			),
		);
	}
}
