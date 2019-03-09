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
 * @package favourites
 * @author AlecMyers <alec@alecmyers.com>
 * @version $Revision: 1371 $
 */
class GetNamesView extends GalleryView {
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
		list($command, $like) = GalleryUtilities::getRequestVariables('command', 'like');

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if (!headers_sent()) {
			header('Content-type: text/plain; charset=UTF-8');
		}

		switch ($command) {
			case 'lookupUsername':
				GalleryCoreApi::requireOnce('modules/favourites/classes/FavouritesMapHelper.class');

				list($ret, $usernames) = FavouritesMapHelper::fetchUsernamesWithFavouritesLike($like);

				if (!$ret) {
					echo implode("\n", $usernames);
				}

				break;
		}

		return null;
	}
}
