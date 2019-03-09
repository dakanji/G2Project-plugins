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
 * @package Tags
 * @author Eric H Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1087 $
 */
class TagsCallbacks {
	public function callback($params, &$smarty, $callback, $userId = null) {
		global $gallery;

		GalleryCoreApi::requireOnce('modules/tags/classes/TagsHelper.class');

		switch ($callback) {
			case 'TagsLink':
				$tags = array();

				list($ret, $tags) = TagsHelper::getTagsByItemId($params['itemId']);

				if ($ret) {
					return $ret;
				}

				$block                                   =& $smarty->_tpl_vars['block'];
				$block['tags']['TagsLink']['Tags']       = $tags;
				$block['tags']['TagsLink']['TagsNumber'] = count($tags);

				return null;

			case 'GetTagCloud':
				list($ret, $tags) = TagsHelper::getTagListWithTagCount();

				if ($ret) {
					return array($ret, null);
				}

				$tagCloud = TagsHelper::createTagCloudFromList($tags);
				$block    =& $smarty->_tpl_vars['block'];

				if (isset($params['totalTagCount']) and $params['totalTagCount']) {
					$totalTagCount = 0;

					foreach ($tagCloud as $tag) {
						$totalTagCount += $tag['rawTagCount'];
					}

					$block['tags']['totalTagCount'] = $tagCloud;
				}

				$block['tags']['taglist'] = $tagCloud;

				return null;
		}

		return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}
}
