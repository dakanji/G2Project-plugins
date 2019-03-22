<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
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
 * This view produces xml output to feed to Simpleviewer
 * @package SimpleviewerSource
 * @version $Revision: 1729 $
 * @author Alec Myers <alec@alecmyers.com>
 */
class XMLOutView extends GalleryView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	/**
	 * @see GalleryView::renderImmediate
	 */
	public function renderImmediate($status, $error) {
		global $gallery;

		// Figure out which item we're talking about
		$itemId = (int)GalleryUtilities::getRequestVariables('itemId');

		if (empty($itemId)) {
			return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
		}

		// Make sure we have permission to view this item
		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.view');

		if ($ret) {
			return $ret;
		}

		// Load the item
		list($ret, $albumItem) = GalleryCoreApi::loadEntitiesById($itemId);

		if ($ret) {
			return array($ret, null);
		}

		// get the title
		$albumTitle = $albumItem->getTitle();

		// fetch the childrenIds that we can see
		list($ret, $showItemIds) = GalleryCoreApi::fetchChildItemIdsWithPermission($itemId, 'core.view');

		if ($ret) {
			return array($ret, null);
		}

		// compile parameter list
		GalleryCoreApi::requireOnce('modules/simpleviewersource/classes/SimpleviewerParameterMapHelper.class');

		list($ret, $fnParams) = SimpleviewerParameterMapHelper::fetchParams($itemId, 'fixed');

		if ($ret) {
			return $ret;
		}

		list($ret, $moduleParams) = SimpleviewerParameterMapHelper::fetchParams($itemId, 'private');

		if ($ret) {
			return $ret;
		}

		list($ret, $customParams) = SimpleviewerParameterMapHelper::fetchParams($itemId, '');

		if ($ret) {
			return $ret;
		}

		// get url generator
		$urlgen                =& $gallery->getUrlGenerator();
		$fnParams['thumbPath'] = $urlgen->generateUrl(
			array(
				'view'   => 'simpleviewersource.DownloadThumb',
				'itemId' => '',
			),
			array(
				'forceFullUrl'   => true,
				'forceSessionId' => false,
				'useAuthToken'   => false,
				'htmlEntities'   => true,
			)
		);

		$fnParams['imagePath'] = $urlgen->generateUrl(
			array(
				'view'           => 'simpleviewersource.DownloadMax',
				'maxImageHeight' => $fnParams['maxImageHeight'],
				'maxImageWidth'  => $fnParams['maxImageWidth'],
				'itemId'         => '',
			),
			array(
				'forceFullUrl'   => true,
				'forceSessionId' => false,
				'useAuthToken'   => false,
				'htmlEntities'   => true,
			)
		);

		// check if enabled for this album
		if (!$moduleParams['enabled']) {
			return null;
		}

		// output simpleviewer options
		header('Content-type: text/xml');
		echo '<?xml version="1.0" encoding="UTF-8"?>';
		echo "\n<simpleviewerGallery ";

		// parameters
		foreach (array_merge($fnParams, $customParams) as $name => $value) {
			echo "$name='$value'\n";
		}

		echo '>';
		echo "\n";

		// output items
		foreach ($showItemIds as $myItemId) {
			list($ret, $myItem) = GalleryCoreApi::loadEntitiesByID($myItemId);

			if ($ret) {
				return $ret;
			}

			if (GalleryUtilities::isA($myItem, 'GalleryPhotoItem')) {
				$myFilename = $myItem->getTitle();
				$myCaption  = $myItem->getSummary();
				echo "<image><filename>$myItemId</filename><caption>$myFilename</caption></image>\n";

				if (isset($moduleParams['maxItems']) && !(--$moduleParams['maxItems'])) {
					break;
				}
			}
		}

		// done
		echo '</simpleviewerGallery>';

		return null;
	}
}
