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
 * Dynamic album view of favourite items!
 * @package Favourites
 * @author Alec Myers <alec@alecmyers.com>
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 1456 $
 */class FavouritesAlbumView extends GalleryView {
	public function __construct() {
		global $gallery;

		$this->_view            = 'favourites.FavouritesAlbum';
		$this->_title           = $gallery->i18n('Favorite Items');
		$this->_itemType        = array($gallery->i18n('Favorite'), $gallery->i18n('favorite'));
		$this->_viewDescription = $gallery->i18n('favorites');
		$this->_param           = 'favourite';
	}

	/**
	 * @see GalleryView::getViewType
	 */
	public function getViewType() {
		return VIEW_TYPE_SHOW_ITEM;
	}

	/**
	 * @see GalleryView::loadThemeAndParameters
	 */
	public function loadThemeAndParameters() {
		global $gallery;

		list($ret, $item) = GalleryCoreApi::newFactoryInstance('GalleryDynamicAlbum');

		if ($ret) {
			return array($ret, null, null, null);
		}

		if (!isset($item)) {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null, null, null);
		}

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'favourites');

		if ($ret) {
			return array($ret, null, null, null);
		}

		list($targetUserId, $itemId) = GalleryUtilities::getRequestVariables('targetUserId', 'itemId');

		// only admins can ask for others' favourites
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if (empty($targetUserId) || $ret) {
			$targetUserId = $gallery->getActiveUserId();

			GalleryUtilities::putRequestVariable('targetUserId', $targetUserId);
		}

		// get user name
		list($ret, $targetUser) = GalleryCoreApi::loadEntitiesById($targetUserId, 'GalleryUser');

		if ($ret) {
			return array($ret, null, null, null);
		}

		// thought that loadEntitiesById with specified entity type would protect against loading non-users but apparently not always
		if (!GalleryUtilities::isA($targetUser, 'GalleryUser')) {
			// Invalid targetUserId given, return MISSING_OBJECT to get standard error page
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null, null, null);
		}

		$targetUserName = $targetUser->GetFullName();

		$item->create(
			$module->translate(
				array(
					'text' => 'Favorites: %s',
					'arg1' => $targetUserName,
				)
			),
			array(
				array('Favorites Album', 'favorites album'),
				array($module->translate('Favorites Album'), $module->translate('favorites album')),
			)
		);

		list($ret, $moduleParams) = GalleryCoreApi::fetchAllPluginParameters('module', 'favourites');

		if ($ret) {
			return array($ret, null, null, null);
		}

		if (isset($moduleParams['description'])) {
			$item->setDescription($moduleParams['description']);
		}

		if (!empty($itemId)) {
			// Viewing an item in this dynamic album
			list($ret, $viewItem) = GalleryCoreApi::loadEntitiesById($itemId);

			if ($ret) {
				return array($ret, null, null, null);
			}

			if (!GalleryUtilities::isA($viewItem, 'GalleryItem')) {
				// Invalid itemId given, return MISSING_OBJECT to get standard error page
				return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null, null, null);
			}

			// Provide parent, parent URL and get-children function to Theme API
			$item->urlParams = array(
				'view'         => 'favourites.FavouritesAlbum',
				'targetUserId' => $targetUserId,
				'highlightId'  => $itemId,
			);

			$item->getChildrenFunction = array('FavouritesAlbumView', 'getChildIds');
			$viewItem->parent          = $item;
			$item                      = $viewItem;
		}

		if (empty($moduleParams['themeId'])) {
			list($ret, $theme) = $this->loadThemeForItem();

			if ($ret || !isset($theme)) {
				// Ignore errors here so fallback theme can be used
				return array(null, null, array(), $item);
			}
		} else {
			list($ret, $theme) = GalleryView::_loadTheme($moduleParams['themeId']);

			if ($ret || !isset($theme)) {
				// Ignore errors here so fallback theme can be used
				return array(null, null, array(), $item);
			}
		}

		list($ret, $params) = $theme->fetchParameters($moduleParams['themeSettingsId']);

		if ($ret) {
			return array($ret, null, null, null);
		}

		return array(null, $theme, $params, $item);
	}

	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		$theme =& $template->getVariableByReference('theme');

		list($itemId, $targetUserId) = GalleryUtilities::getRequestVariables('itemId', 'targetUserId');

		$theme['pageUrl'] = array(
			'view'         => 'favourites.FavouritesAlbum',
			'targetUserId' => $targetUserId,
		);

		if (empty($itemId)) {
			// targetUserId known not to be blank because we set it in LoadThemeandParameters above
			list($ret, $theme['allChildIds']) = $this->getChildIds(null, $targetUserId);

			if ($ret) {
				return array($ret, null);
			}
		} else {
			// Item in dynamic album; use core.ShowItem to check permission, increment view count
			list($ret, $showItem) = GalleryView::loadView('core.ShowItem');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $result) = $showItem->loadTemplate($template, $form);

			if ($ret) {
				return array($ret, null);
			}

			if (isset($result['redirect'])) {
				return array(null, $result);
			}
		}

		return array(null, array());
	}

	/**
	 * Dynamic query for items
	 * @param int $userId
	 * @return array object GalleryStatus a status code
	 *               array of item ids
	 * @static
	 */
	public function getChildIds($userId, $targetUserId = null) {
		global $gallery;

		if (!isset($targetUserId)) {
			$targetUserId = GalleryUtilities::getRequestVariables('targetUserId');
		}

		GalleryCoreApi::requireOnce('modules/favourites/classes/FavouritesMapHelper.class');

		return FavouritesMapHelper::fetchFavourites($targetUserId);
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	public function getViewDescription() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'favouritesalbum');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate($this->_viewDescription));
	}
}
