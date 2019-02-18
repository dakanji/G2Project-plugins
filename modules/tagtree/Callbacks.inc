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
 *//**
 * @package TagTree
 * @author Chad Parry <spam@chad.parry.org>
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 1720 $
 */
class TagTreeCallbacks {
	public function callback($callbackParams, &$smarty, $callback, $userId) {
		GalleryCoreApi::requireOnce('modules/tagtree/classes/TagTreeNode.class');
		GalleryCoreApi::requireOnce('modules/tagtree/classes/TagTreeHelper.class');

		global $gallery;
		static $idCount = '';

		switch ($callback) {
			case 'LoadAlbumData':
				list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'tagtree');

				if ($ret) {
					return $ret;
				}

				list($ret, $rootAlbumId) = GalleryCoreApi::getDefaultAlbumId();

				if ($ret) {
					return $ret;
				}
				list($ret, $rootAlbum) = GalleryCoreApi::loadEntitiesById($rootAlbumId);

				if ($ret) {
					return $ret;
				}
				$rootAlbumTitle = $rootAlbum->getTitle() ? $rootAlbum->getTitle() : $rootAlbum->getPathComponent();

				$translator    =& $gallery->getTranslator();
				$params['rtl'] = $translator->isRightToLeft() ? '_rtl' : '';

				$blockId                           = 'tagtree' . $idCount++;
				$block                             =& $smarty->_tpl_vars['block'];
				$block['tagtree']['LoadAlbumData'] = array(
					'blockId'        => $blockId,
					'rootAlbumId'    => $rootAlbumId,
					'rootAlbumTitle' => smarty_modifier_escape($rootAlbum->title, 'javascript'),
					'params'         => $params,
				);

				return null;
		}

		return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}
}
