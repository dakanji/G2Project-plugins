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
 * This immediate view provides the backend for callbacks made by the AJAX
 * NodeExpansionView.
 *
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Chad Parry <spam@chad.parry.org>
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 1720 $
 */
class NodeExpansionCallbackView extends GalleryView {
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

		GalleryCoreApi::requireOnce('modules/tagtree/classes/TagTreeHelper.class');
		GalleryCoreApi::requireOnce('modules/tagtree/classes/TagTreeNode.class');

		$userId = $gallery->getActiveUserId();

		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'tagtree');

		if ($ret) {
			return $ret;
		}

		list($tags, $albumId) = GalleryUtilities::getRequestVariables('tags', 'albumId');

		if (empty($tags)) {
			$tags = array();
		}

		$tags    = array_map(create_function('$tagId', 'return intval($tagId);'), $tags);
		$albumId = intval($albumId);

		list($ret, $tree) = NodeExpansionCallbackView::fetchCachedTagTree($tags, $albumId, $userId);

		if ($ret) {
			return $ret;
		}

		if (!$tree) {
			$onlyAlbums = !$params['showItems'];

			list($ret, $tree) = NodeExpansionCallbackView::generateTagTree($tags, $albumId, $onlyAlbums, $userId);

			if ($ret) {
				if ($ret->getErrorCode() & ERROR_PERMISSION_DENIED) {
					return null;
				}

				return $ret;
			}
		}

		header('Content-Type: text/xml');
		echo "<?xml version=\"1.0\"?>\n<tree>\n";

		$ret = NodeExpansionCallbackView::printTree($tree, $userId, $params);

		if ($ret) {
			return $ret;
		}

		echo "</tree>\n";

		return null;
	}

	public function printTree($tree, $userId, $params) {
		global $gallery;

		$urlGenerator =& $gallery->getUrlGenerator();
		$children     = $tree->children;
		usort(
			$children,
			create_function(
				'$a,$b',
				'return strcasecmp($a->title, $b->title);'
			)
		);

		foreach ($children as $node) {
			switch ($node->type) {
				case 'album':
				case 'item':
					$nodeAction = $urlGenerator->generateUrl(
						array(
							'view'   => 'core.ShowItem',
							'itemId' => $node->itemId,
						),
						array(
							'forceServerRelativeUrl' => true,
						)
					);

					break;

				case 'tag':
					$nodeAction = $urlGenerator->generateUrl(
						array(
							'view'    => 'tagtree.VirtualAlbum',
							'tags'    => $node->tags,
							'albumId' => $node->itemId,
						),
						array(
							'forceServerRelativeUrl' => true,
						)
					);

					break;
			}

			switch ($node->type) {
				case 'album':
					$closedIcon = $urlGenerator->generateUrl(
						array(
							'href' => 'modules/tagtree/lib/xloadtree/images/foldericon.png',
						)
					);

					$openIcon = $urlGenerator->generateUrl(
						array(
							'href' => 'modules/tagtree/lib/xloadtree/images/openfoldericon.png',
						)
					);

					break;

				case 'tag':
					$closedIcon = $openIcon = $urlGenerator->generateUrl(
						array(
							'href' => 'modules/tagtree/images/tag.png',
						)
					);

					break;

				case 'item':
					$closedIcon = $urlGenerator->generateUrl(
						array(
							'href' => 'modules/tagtree/images/item.png',
						)
					);

					$openIcon = null;

					break;
			}

			$icons = sprintf(' icon="%s"', $closedIcon);

			if (!is_null($openIcon)) {
				$icons = $icons . sprintf(' openIcon="%s"', $openIcon);
			}

			if ($node->type == 'item') {
				$nodeSrc = '';
			} else {
				if ($params['usePersistence']) {
					list($ret, $subTree) = NodeExpansionCallbackView::fetchCachedTagTree($node->tags, $node->itemId, $userId);

					if ($ret) {
						return $ret;
					}
				} else {
					$subTree = null;
				}

				if ($subTree) {
					$nodeSrc = '';
				} else {
					$nodeChildren = $urlGenerator->generateUrl(
						array(
							'view'    => 'tagtree.NodeExpansionCallback',
							'tags'    => $node->tags,
							'albumId' => $node->itemId,
						)
					);

					$nodeSrc = sprintf('src="%s" ', $nodeChildren);
				}
			}

			printf(
				'<tree text="%s" %saction="%s"%s',
				htmlspecialchars($node->title),
				$nodeSrc,
				$nodeAction,
				$icons
			);

			if (isset($subTree) && !empty($subTree->children)) {
				echo ">\n";

				$ret = NodeExpansionCallbackView::printTree($subTree, $userId, $params);

				if ($ret) {
					return $ret;
				}

				echo "</tree>\n";
			} else {
				echo " />\n";
			}
		}

		return null;
	}

	public function fetchCachedTagTree($tags, $albumId, $userId) {
		if (TAGTREE_CACHE) {
			list($ret, $cachePathInfo) = NodeExpansionCallbackView::getCachePathInfo($tags, $albumId, $userId);

			if ($ret) {
				return array($ret, null);
			}

			$tree = GalleryDataCache::getFromDisk($cachePathInfo);
		}

		if (!isset($tree)) {
			$tree = null;
		}

		return array(null, $tree);
	}

	public function generateTagTree($tags, $albumId, $onlyAlbums, $userId) {
		list($ret, $tree) = TagTreeHelper::fetchTagTree($tags, $albumId, $onlyAlbums, $userId);

		if ($ret) {
			return array($ret, null);
		}

		if (TAGTREE_CACHE) {
			list($ret, $cachePathInfo) = NodeExpansionCallbackView::getCachePathInfo($tags, $albumId, $userId);

			if ($ret) {
				return array($ret, null);
			}

			GalleryDataCache::putToDisk($cachePathInfo, $tree);
		}

		return array(null, $tree);
	}

	public function getCachePathInfo($tags, $albumId, $userId) {
		global $gallery;

		list($ret, $aclIdsWithSessionPermissions) = GalleryCoreApi::fetchAccessListIds('core.view', $userId, true);

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $aclIds) = GalleryCoreApi::fetchAccessListIds('core.view', $userId, false);

		if ($ret) {
			return array($ret, null);
		}

		$canonicalTags = $tags;
		sort($canonicalTags);
		$sessionPermissions = implode('-', array_diff($aclIdsWithSessionPermissions, $aclIds));
		$cachePathInfo      = array(
			'type'   => 'module-data',
			'module' => 'tagtree',
			'itemId' => $albumId . '_' . serialize($canonicalTags) . '_'
			. $userId . '_' . $sessionPermissions,
		);

		return array(null, $cachePathInfo);
	}
}
