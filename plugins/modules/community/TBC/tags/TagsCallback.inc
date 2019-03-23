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
GalleryCoreApi::requireOnce('modules/tags/classes/TagsHelper.class');

/**
 * This controller will handle assigning tags to items
 *
 * @package Tags
 * @subpackage UserInterface
 * @author Eric H Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1087 $
 */
class TagsCallbackView extends GalleryView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	/**
	 * @see GalleryView::isControllerLike
	 */
	public function isControllerLike() {
		return true;
	}

	/**
	 * @see GalleryView::renderImmediate
	 */
	public function renderImmediate($status, $error) {
		list($itemId, $command, $tagName) = GalleryUtilities::getRequestVariables(
			'itemId',
			'command',
			'tagName'
		);

		$tagName = trim($tagName);

		if (!headers_sent()) {
			header('Content-type: text/plain; charset=UTF-8');
		}

		switch ($command) {
			case 'addTag':
				if (!empty($tagName)) {
					$ret = TagsHelper::assignTag($itemId, $tagName);

					if ($ret) {
						return $ret;
					}
				}

				break;

			case 'delTag':
				if (!empty($tagName)) {
					$ret = TagsHelper::unassignTag($itemId, $tagName);

					if ($ret) {
						return $ret;
					}
				}

				break;

			case 'newTag':
				if (!empty($tagName)) {
					list($ret, $tagList) = TagsHelper::splitTags($tagName);

					if ($ret) {
						return $ret;
					}

					foreach ($tagList as $tag) {
						$ret = TagsHelper::addNewTag($tag);

						if ($ret) {
							return $ret;
						}

						$ret = TagsHelper::assignTag($itemId, $tag);

						if ($ret) {
							return $ret;
						}
					}
				}

				break;

			case 'getTagList':
				$prefix = GalleryUtilities::getRequestVariables('prefix');

				list($ret, $tagList) = TagsHelper::getTaglist($prefix);

				if (!$ret) {
					/*
					 * AutoComplete's handling of HTML entities has been fixed in r16352. Calling
					 * unsanitize() doesn't do the same but is a workaround for older G2 versions.
					 */
					if (!GalleryUtilities::isCompatibleWithApi(
						array(7, 29),
						GalleryCoreApi::getApiVersion()
					)
					) {
						/** @todo Remove this check / call on the next major API change. */
						GalleryUtilities::unsanitizeInputValues($tagList, false);
					}

					echo implode("\n", $tagList);
				}

				// @todo Log Error when debug is on
				break;

			default:
				// @note Not sure if this should a debug log instead
				return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
		}

		return null;
	}
}
