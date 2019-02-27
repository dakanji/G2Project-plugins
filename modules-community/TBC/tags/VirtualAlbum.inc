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

GalleryCoreApi::requireOnce('modules/tags/classes/TagsHelper.class');

/**
 * Dynamic album view using a query on Tags
 * @package Tags
 * @subpackage UserInterface
 * @author Eric H Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1060 $
 */
class VirtualAlbumView extends GalleryView {
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
		list($ret, $item) = GalleryCoreApi::newFactoryInstance('GalleryDynamicAlbum');

		if ($ret) {
			return array($ret, null, null, null);
		}

		if (!isset($item)) {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null, null, null);
		}
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'tags');

		if ($ret) {
			return array($ret, null, null, null);
		}
		list($tagName, $itemId) = GalleryUtilities::getRequestVariables('tagName', 'itemId');

		$item->create(
			$module->translate(
				array(
					'text' => 'Items Tagged: %s',
					'arg1' => $tagName,
				)
			),
			array(
				array('Tagged Items', 'Tagged Items'),
				array(
					$module->translate('Tagged Items'),
					$module->translate('Tagged Items'),
				),
			)
		);

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
			$rawTagName = $tagName;
			GalleryUtilities::unsanitizeInputValues($rawTagName, false);
			$item->urlParams           = array(
				'view'        => 'tags.VirtualAlbum',
				'tagName'     => $rawTagName,
				'highlightId' => $itemId,
			);
			$item->getChildrenFunction = array('VirtualAlbumView', 'getChildIds');
			$viewItem->parent          = $item;
			$item                      = $viewItem;
		}

		list($ret, $theme) = $this->loadThemeForItem();

		if ($ret || !isset($theme)) {
			// Ignore errors here so fallback theme can be used
			return array(null, null, array(), $item);
		}

		list($ret, $params) = $theme->fetchParameters();

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

		$rawTagName = $tagName = GalleryUtilities::getRequestVariables('tagName');

		GalleryUtilities::unsanitizeInputValues($rawTagName, false);
		$theme['pageUrl'] = array(
			'view'    => 'tags.VirtualAlbum',
			'tagName' => $rawTagName,
		);

		// Perform query for this dynamic album
		list($ret, $theme['allChildIds']) = $this->getChildIds($theme['actingUserId'], $tagName);

		if ($ret) {
			return array($ret, null);
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
	public function getChildIds($userId, $tagName = null) {
		global $gallery;
		$storage =& $gallery->getStorage();

		if (!isset($tagName)) {
			$tagName = GalleryUtilities::getRequestVariables('tagName');
		}

		if (empty($tagName)) {
			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
		}

		// Force case-sensitive look-up to make the query use an column index
		list($ret, $tagId) = TagsHelper::getTagIdFromName($tagName, true);

		if ($ret) {
			return array($ret, null);
		}

		if (empty($tagId)) {
			return array(null, array());
		}

		list($ret, $query, $data) = GalleryCoreApi::buildItemQuery(
			'TagItemMap',
			'itemId',
			'[TagItemMap::tagId] = ?',
			null,
			null,
			null,
			'core.view',
			false,
			$userId
		);

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $searchResults) = $gallery->search($query, array_merge(array($tagId), $data));

		if ($ret) {
			return array($ret, null);
		}
		$itemIds = array();

		while ($result = $searchResults->nextResult()) {
			$itemIds[] = $result[0];
		}

		return array(null, array_unique($itemIds));
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	public function getViewDescription() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'tags');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Tagged item'));
	}
}
