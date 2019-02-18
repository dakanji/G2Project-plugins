<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2009 Bharat Mediratta
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
 * Dynamic album view using a query on TagTree
 * @package TagTree
 * @subpackage UserInterface
 * @author Chad Parry <spam@chad.parry.org>
 * @author Eric H Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1720 $
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
		global $gallery;
		GalleryCoreApi::requireOnce('modules/tagtree/classes/TagTreeHelper.class');

		list($ret, $item) = GalleryCoreApi::newFactoryInstance('GalleryDynamicAlbum');

		if ($ret) {
			return array($ret, null, null, null);
		}

		if (!isset($item)) {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null, null, null);
		}
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'tagtree');

		if ($ret) {
			return array($ret, null, null, null);
		}
		list($tags, $albumId, $itemId) = GalleryUtilities::getRequestVariables('tags', 'albumId', 'itemId');

		list($ret, $albumTitle) = TagTreeHelper::getItemTitle($albumId);

		if ($ret) {
			return array($ret, null);
		}

		$generator =& $gallery->getUrlGenerator();
		$albumUrl  = $generator->generateUrl(
			array(
				'view'   => 'core.ShowItem',
				'itemId' => $albumId,
			)
		);
		$albumLink = sprintf('<a href="%s">%s</a>', $albumUrl, $albumTitle);

		if (empty($tags)) {
			$title = $module->translate(
				array(
					'text' => 'All within %s',
					'arg1' => $albumLink,
				)
			);
		} else {
			$tagLinks = array();

			foreach ($tags as $tagId) {
				list($ret, $tagName) = TagTreeHelper::getTagName($tagId);

				if ($ret) {
					return array($ret, null);
				}
				$tagUrl  = $generator->generateUrl(
					array(
						'view'    => 'tags.VirtualAlbum',
						'tagName' => $tagName,
					)
				);
				$tagLink = sprintf('<a href="%s">%s</a>', $tagUrl, $tagName);
				array_push($tagLinks, $tagLink);
			}
			$tagSummary = implode(',', $tagLinks);

			$title = $module->translate(
				array(
					'text' => 'Tagged %s within %s',
					'arg1' => $tagSummary,
					'arg2' => $albumLink,
				)
			);
		}
		$item->create(
			$title,
			array(
				array('Tag Tree', 'Tag Tree'),
				array($module->translate('Tag Tree'), $module->translate('Tag Tree')),
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
				'view'        => 'tagtree.VirtualAlbum',
				'tags'        => $tags,
				'albumId'     => $albumId,
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

		$tags    = GalleryUtilities::getRequestVariables('tags');
		$albumId = GalleryUtilities::getRequestVariables('albumId');

		$theme['pageUrl'] = array(
			'view'    => 'tagtree.VirtualAlbum',
			'tags'    => $tags,
			'albumId' => $albumId,
		);
		// Perform query for this dynamic album
		list($ret, $theme['allChildIds']) = $this->getChildIds($theme['actingUserId'], $tags, $albumId);

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
	public function getChildIds($userId, $tags = null, $albumId = null) {
		global $gallery;

		if (!isset($tags)) {
			$tags = GalleryUtilities::getRequestVariables('tags');
		}

		if (empty($tags)) {
			$tags = array();
		}

		if (!isset($albumId)) {
			$albumId = GalleryUtilities::getRequestVariables('albumId');
		}

		if (empty($albumId)) {
			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
		}

		list($ret, $itemIds) = TagTreeHelper::fetchMatchingItems($tags, $albumId, $userId);

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $itemIds);
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	public function getViewDescription() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'tagtree');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Tag Tree'));
	}
}
