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
 * This controller will handle assigning tags to items
 *
 * @package Tags
 * @subpackage UserInterface
 * @author Eric H Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1727 $
 */
class TagsItemEditController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		list($itemId, $tagMode) = GalleryUtilities::getRequestVariables('itemId', 'tagMode');

		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'tags.edit');

		if ($ret) {
			return array($ret, null);
		}

		$status = $error = array();

		list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

		if ($ret) {
			return array($ret, null);
		}

		if (isset($form['action']['apply'])) {
			if ($item->getCanContainChildren()) {
				// Figure out what to extend it to
				/** todo move this to the callback and add permission check on the first 2 functions */
				$items = array();

				if (!empty($form['AlbumItems']) && !empty($form['AllAlbumItems'])) {
					list($ret, $tmpItems) = GalleryCoreApi::fetchChildDataItemIds($item);

					if ($ret) {
						return array($ret, null);
					}

					$items = array_merge($items, $tmpItems);
				}

				if (!empty($form['AlbumItems']) && !empty($form['AllAlbumAlbums'])) {
					list($ret, $tmpItems) = GalleryCoreApi::fetchChildAlbumItemIds($item);

					if ($ret) {
						return array($ret, null);
					}

					$items = array_merge($items, $tmpItems);
				}

				if (!empty($form['SubAlbumItems']) && !empty($form['AllSubAlbumItems'])) {
					list($ret, $tmpItems) = GalleryCoreApi::fetchDescendentItemIds(
						$item,
						null,
						null,
						'tags.edit'
					);

					if ($ret) {
						return array($ret, null);
					}

					$items = array_merge($items, $tmpItems);
				}

				if (!empty($form['SubAlbumItems']) && !empty($form['AllSubAlbumAlbums'])) {
					list($ret, $tmpItems) = GalleryCoreApi::fetchDescendentAlbumItemIds(
						$item,
						null,
						null,
						'tags.edit'
					);

					if ($ret) {
						return array($ret, null);
					}

					$items = array_merge($items, $tmpItems);
				}

				$templateAdapter =& $gallery->getTemplateAdapter();
				$templateAdapter->registerTrailerCallback(
					array($this, 'runApplyTags'),
					array($items, $itemId)
				);
				$results['delegate']['view'] = 'core.ProgressBar';
			} else {
				$results['delegate']['view']   = 'tags.TagsItemEdit';
				$results['delegate']['itemId'] = $itemId;
				$error[]                       = '$form[error][noextension]';
			}
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}

	public function runApplyTags($items, $itemId) {
		global $gallery;

		$storage         =& $gallery->getStorage();
		$templateAdapter =& $gallery->getTemplateAdapter();

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'tags');

		if ($ret) {
			return $ret;
		}

		list($ret, $tags) = TagsHelper::getTagsByItemId($itemId);

		if ($ret) {
			return $ret;
		}

		$heading = $module->translate('Applying Album Tags to SubItems');
		$templateAdapter->updateProgressBar($heading, $module->translate('Preparing...'), 0);
		$total = count($items);

		foreach ($items as $key => $id) {
			foreach ($tags as $rawTagName => $tagName) {
				$message = $module->translate(
					array(
						'text' => 'Processing item %d of %d',
						'arg1' => $key + 1,
						'arg2' => $total,
					)
				);
				$templateAdapter->updateProgressBar($heading, $message, ($key + 1) / $total);
				$ret = TagsHelper::assignTag($id, $tagName);

				if ($ret) {
					return $ret;
				}
			}
		}

		$redirect = array(
			'view'   => 'tags.TagsItemEdit',
			'itemId' => $itemId,
		);

		$urlGenerator =& $gallery->getUrlGenerator();
		$templateAdapter->completeProgressBar($urlGenerator->generateUrl($redirect));

		return null;
	}
}

class TagsItemEditView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		list($itemId, $tagMode) = GalleryUtilities::getRequestVariables(
			'itemId',
			'tagMode'
		);

		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'tags.edit');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

		if ($ret) {
			return array($ret, null);
		}

		/** @todo Interact with the EXIF factory instead */
		$exifModuleActive = false;

		list($ret, $exifModule) = GalleryCoreApi::loadPlugin('module', 'exif');

		if ($ret) {
			return array($ret, null);
		}

		if (!empty($exifModule)) {
			list($ret, $exifModuleActive) = $exifModule->isActive();

			if ($ret) {
				return array($ret, null);
			}
		}

		$tagExifInfo = array();

		if ($exifModuleActive) {
			// Grab the list of fields in 'details'
			list($ret, $path) = $item->fetchPath();

			GalleryCoreApi::requireOnce('modules/exif/classes/ExifHelper.class');

			list($ret, $exifData) = ExifHelper::getExifData($path, EXIF_DETAILED);

			if ($ret) {
				return array($ret, null);
			}

			$tagExifInfo = $exifData;
		}

		if (!$exifModuleActive || empty($tagExifInfo)) {
			// No EXIF ensure we don't go to that tab by 'mistake'
			$tagMode = ($tagMode == 'Exif') ? 'Existing' : $tagMode;
		}

		$tagItem = GalleryUtilities::isA($item, 'GalleryAlbumItem') ? 'Album' : 'Other';
		$tagMode = ($tagMode == '') ? 'Existing' : $tagMode;

		if ($form['formName'] != 'TagsItemEdit') {
			$form['formName'] = 'TagsItemEdit';
			$form['itemId']   = $itemId;
		}

		list($ret, $tagList) = TagsHelper::getTagListWithTagCount();

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $thisItemTagList) = TagsHelper::getTagsByItemId($itemId);

		if ($ret) {
			return array($ret, null);
		}

		$tempArray = array();

		foreach ($thisItemTagList as $tagId => $tagName) {
			$tempArray[$tagName] = $tagId;
		}

		$thisItemTagList = $tempArray;

		list($ret, $limitPop) = GalleryCoreApi::getPluginParameter(
			'module',
			'tags',
			'LimitPopularTag'
		);

		if ($ret) {
			return array($ret, null);
		}

		if ($limitPop) {
			list($ret, $popLimit) = GalleryCoreApi::getPluginParameter(
				'module',
				'tags',
				'PopularTaglimit'
			);

			if ($ret) {
				return array($ret, null);
			}
		} else {
			$popLimit = count($tagList);
		}

		$template->javascript('lib/yui/yahoo-min.js');
		$template->javascript('lib/yui/event-min.js');
		$template->javascript('lib/yui/connection-min.js');
		$template->javascript('modules/tags/js/Tags.js');
		$template->setVariable('controller', 'tags.TagsItemEdit');
		$template->setVariable('tagList', $thisItemTagList);
		$template->setVariable('popularTags', $tagList);
		$template->setVariable('tagItem', $tagItem);
		$template->setVariable('tagMode', $tagMode);
		$template->setVariable('tagExifInfo', $tagExifInfo);
		$template->setVariable('itemId', $itemId);
		$template->setVariable('popLimit', $popLimit);

		return array(
			null,
			array(
				'body' => 'modules/tags/templates/TagsItemEdit.tpl',
			),
		);
	}
}
