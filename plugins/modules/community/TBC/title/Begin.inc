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
 * Dynamic album view of items starting with given chars
 * @package Title
 * @author Alec Myers <alec@alecmyers.com>
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 1840 $
 */class BeginView extends GalleryView {
	public function __construct() {
		global $gallery;

		$this->_view            = 'title.Begin'; //got this one
		$this->_title           = $gallery->i18n('Items that begin with');
		$this->_itemType        = array($gallery->i18n('Begin'), $gallery->i18n('begin'));
		$this->_viewDescription = $gallery->i18n('Items whose titles begin a particular way'); //got this one
		$this->_param           = 'begin';
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

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'title');

		if ($ret) {
			return array($ret, null, null, null);
		}

		list($chars, $itemId) = GalleryUtilities::getRequestVariables('chars', 'itemId');

		$item->create(
			$module->translate($this->_title) . ': ' . $chars,
			array($this->_itemType, array_map(array($module, 'translate'), $this->_itemType))
		);

		list($ret, $moduleParams) = GalleryCoreApi::fetchAllPluginParameters('module', 'title');

		if ($ret) {
			return array($ret, null, null, null);
		}

		if (isset($moduleParams['description.' . $this->_param])) {
			$item->setDescription($moduleParams['description.' . $this->_param]);
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
				'view'        => $this->_view,
				'chars'       => $chars,
				'highlightId' => $itemId,
			);

			$item->getChildrenFunction = array(get_class($this), 'getChildIds');
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

		list($chars, $itemId) = GalleryUtilities::getRequestVariables('chars', 'itemId');

		$theme['pageUrl'] = array(
			'view'  => $this->_view,
			'chars' => $chars,
		);

		list($ret, $childIds) = $this->getChildIds(null, $chars);

		if ($ret) {
			return array($ret, null);
		}

		// if only one item, go straight to an item view
		if (empty($itemId) && count($childIds) == 1) {
			$itemId   = $childIds[0];
			$redirect = array(
				'view'   => 'core.ShowItem',
				'itemId' => $itemId,
			);

			return array(
				null,
				array(
					'redirect' => $redirect,
				),
			);

			//GalleryUtilities::putRequestVariable('itemId',$itemId);
		}

		if (empty($itemId)) {
			$theme['allChildIds'] = $childIds;
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
	public function getChildIds($userId, $chars = null) {
		if (!isset($chars)) {
			$chars = GalleryUtilities::getRequestVariables('chars');
		}

		global $gallery;

		$results = array();
		$chars  .= '%';

		$query = 'SELECT 
		[GalleryItem::id]
		FROM [GalleryItem]
		WHERE
		[GalleryItem::title]
		LIKE ?
		ORDER BY [GalleryItem::title]
		';

		$queryOptions = array($chars);

		list($ret, $searchResults) = $gallery->search($query, $queryOptions);

		if ($ret) {
			return array($ret, null);
		}

		while ($result = $searchResults->nextResult()) {
			$results[] = $result[0];
		}

		// safety limit of first 100 results
		$results = array_slice($results, 0, 100);
		return array(null, $results);
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	public function getViewDescription() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'title');

		if ($ret) {
			return array($ret, null);
		}

		list($chars) = GalleryUtilities::getRequestVariables('chars');

		return array(null, $module->translate($this->_viewDescription));
	}
}
