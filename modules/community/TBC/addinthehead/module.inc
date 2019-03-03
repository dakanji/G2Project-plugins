<?php
/*
 * $RCSfile: module.inc,v $
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
 * @package addinthehead
 * @author gallery@markridgwell.com
 */
class AddInTheHeadModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('addinthehead');
		$this->setName('Add Items in the Heading');
		$this->setDescription(
			$gallery->i18n(
				'Add in the Head'
			)
		);

		$this->setVersion('1.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('display', $gallery->i18n('Display'));
		$this->setCallbacks('getSystemLinks');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new AddInTheHeadModule();
		}

		return $instance;
	}

	public function addExternalLink($path, &$links, $containstext, $title, $url) {
		if (stripos($path, $containstext)) {
			$links['ExternalLink' . $containstext] = array(
				'text'   => $title,
				'params' => array(
					'href' => $url,
				),
			);
		}
	}

	/**
	 * @see GalleryModule::getSystemLinks
	 */
	public function getSystemLinks() {
		global $gallery;

		$links = array();

		list($path, $itemId) = GalleryUtilities::getRequestVariables('path', 'itemId');

		if (!empty($path)) {
			list($ret, $itemId) = GalleryCoreApi::fetchItemIdByPath($path);
		}

		list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

		if ($ret) {
			return array(null, $links);
		}

		if (GalleryUtilities::isA($item, 'GalleryAlbumItem')) {
			$urlGenerator =& $gallery->getUrlGenerator();

			list($ret, $hasAddAlbumPermission) = GalleryCoreApi::hasItemPermission($itemId, 'core.addAlbumItem');

			if (!$ret && $hasAddAlbumPermission) {
				$url = $urlGenerator->generateUrl(
					array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'core.ItemAddAlbum',
						'itemId'  => $itemId,
					)
				);

				$links['AddImage'] = array(
					'text'   => 'Add Album',
					'params' => array(
						'href' => $url,
					),
				);
			}

			list($ret, $hasAddAlbumPermission) = GalleryCoreApi::hasItemPermission($itemId, 'core.addDataItem');

			if (!$ret && $hasAddAlbumPermission) {
				$url = $urlGenerator->generateUrl(
					array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'core.ItemAdd',
						'itemId'  => $itemId,
					)
				);

				$links['AddAlbum'] = array(
					'text'   => 'Add Photo',
					'params' => array(
						'href' => $url,
					),
				);
			}

			/*
			 * if isset($theme.permissions.core_addDataItem) {
			 * 	<a href="{g->url arg1="view=core.ItemAdmin" arg2="subView=core.ItemAdd" arg3="itemId=`$theme.item.id`"}"> {g->text text="Add a photo!"} </a>
			 * }
			 */
		}

		/*
		 * AddInTheHeadModule::addExternalLink(
		 * 	$path,
		 * 	$links,
		 * 	'Maldon',
		 * 	'Mad Maldon Mud Race',
		 * 	'http://www.maldonmudrace.com/'
		 * );
		 */

		// Return
		return array(null, $links);
	}
}
