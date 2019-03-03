<?php
/*
 * $RCSfile: Callbacks.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/**
 * @package Favourites
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1371 $
 */
class FavouritesCallbacks {
	public function callback($params, &$smarty, $callback, $userId = null) {
		global $gallery;

		switch ($callback) {
			case 'getTop10':
				// return 10 favourite items
				GalleryCoreApi::requireOnce('modules/favourites/classes/FavouritesMapHelper.class');
				$block =& $smarty->_tpl_vars['block'];

				list($ret, $block['favourites']['Top10']) = FavouritesMapHelper::fetchFavourites($gallery->getActiveUserId());

				if ($ret) {
					return $ret;
				}

				list($ret, $extra) = FavouritesMapHelper::fetchFavourites(6);

				if ($ret) {
					return $ret;
				}

				foreach ($extra as $additional) {
					$block['favourites']['Top10'][] = $additional;
				}

				$block['favourites']['Top10'] = array_slice($block['favourites']['Top10'], 0, 14);

				foreach (array_slice($block['favourites']['Top10'], 0, 5) as $additional) {
					$block['favourites']['Top10'][] = $additional;
				}

				return null;
		}

		return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}
}
