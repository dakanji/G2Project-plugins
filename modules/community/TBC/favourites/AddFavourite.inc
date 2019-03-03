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
 * Add an items to the user's favourites
 * @package Favourites
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1371 $
 */
class AddFavouriteController extends GalleryController {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function handleRequest($form) {
		global $gallery;

		$results = array();
		$errors  = array();

		list($itemId, $command) = GalleryUtilities::getRequestVariables('itemId', 'command');

		global $gallery;

		$userId = $gallery->getActiveUserId();

		list($ret, $isAnonymousUser) = GalleryCoreApi::isAnonymousUser($userId);

		if ($ret) {
			return array($ret, null);
		}

		if (!$isAnonymousUser) {
			GalleryCoreApi::requireOnce('modules/favourites/classes/FavouritesMapHelper.class');

			if ($command == 'remove') {
				$ret = FavouritesMapHelper::removeFavourite($userId, $itemId);

				if ($ret) {
					return array($ret, null);
				}

				$results['status']['removed'] = 1;
			} elseif ($command == 'clear') {
				$ret = FavouritesMapHelper::clearFavourites($userId);

				if ($ret) {
					return array($ret, null);
				}

				$results['status']['cleared'] = 1;

				list($ret, $itemId) = GalleryCoreApi::getDefaultAlbumId();

				if ($ret) {
					return array($ret, null);
				}
			} else {
				$ret = FavouritesMapHelper::setFavourite($userId, $itemId);

				if ($ret) {
					return array($ret, null);
				}

				$results['status']['added'] = 1;
			}
		} else {
			$errors[] = 'form[error][isGuest]';
		}

		if (!empty($errors)) {
			$results['delegate'] = array(
				'view'   => 'favourites.AddFavourite',
				'itemId' => $itemId,
			);

			$results['status'] = array();
			$results['error']  = $errors;
		} else {
			$results['redirect'] = array(
				'view'   => 'favourites.AddFavourite',
				'itemId' => $itemId,
			);

			$results['error'] = array();
		}

		return array(null, $results);
	}
}

class AddFavouriteView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		// from the URL ...
		$itemId = GalleryUtilities::getRequestVariables('itemId');

		if (!empty($itemId)) {
			list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

			if ($ret) {
				return array($ret, null);
			}

			$template->setVariable(
				'itemDetails',
				array(
					'itemId'       => $itemId,
					'itemTypeName' => $item->itemTypeName(),
				)
			);
		}

		return array(
			null,
			array(
				'body' => 'modules/favourites/templates/FavouritesNotify.tpl',
			),
		);
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	public function getViewDescription() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'favourites');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Add to Favorites'));
	}
}
