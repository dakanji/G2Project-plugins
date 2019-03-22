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
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @package Puzzle
 * @version $Revision: 1660 $ $Date: 2006/07/03 08:04:11 $
 * @author J. Mike Rollins <rollins@CamoTruck.Net>
 * @contrib azn_oohwee
 */

/**
 * CamoTruck.Net Puzzle Module
 *
 * This module provides support for adding puzzles to items
 *
 * @package Puzzle
 */
class puzzleModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('puzzle');
		$this->setName('Puzzle Module');
		$this->setDescription(
			$gallery->i18n(
				'CamoTruck.Net: Puzzle Module'
			)
		);

		// Update upgrade() as required
		$this->setVersion('3.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('display', $this->translate('Display'));
		$this->setCallbacks('getItemLinks');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new puzzleModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		// We don't require any special configuration
		return array(null, true);
	}

	/**
	 * @see GalleryModule::getItemLinks()
	 */
	public function getItemLinks($items, $wantsDetailedLinks, $permissions) {
		global $gallery;

		GalleryCoreApi::requireOnce('modules/puzzle/classes/PuzzleHelper.class');

		$links = array();

		// use item to find the album
		// list ($ret, $enabled) = GalleryCoreApi::getPluginParameter('module','puzzle','enabled',$item->getId());
		foreach ($items as $item) {
			$itemId = $item->getId();

			if (isset($wantsDetailedLinks[$itemId])) {
				if (GalleryUtilities::isA($item, 'GalleryPhotoItem')) {
					// find the parent album of this photo item.
					list($ret, $parentAlbum) = PuzzleHelper::findParentAlbum($item);

					if ($ret) {
						// cannot find parent album.
						// don't know how this can happen ... it is probably a bad, bad thing
						return array($ret, null);
					}

					$gallery->debug('<br>(modules:puzzle) getItemLinks: The parent album for this item is ' . $item->getId());

					// check the album for the 'enabled' parameter
					list($ret, $enabled) = PuzzleHelper::isEnabledForAlbum($parentAlbum);

					if ($ret) {
						// don't know what can cause this error
						return array($ret, null);
					}

					$gallery->debug(
						'<br>(modules:puzzle) getItemLinks: isEnabledForAlbum returned '
						. $enabled
					);

					if ($enabled == 'yes') {
						$gallery->debug(
							'<br>(modules:puzzle) getItemLinks: Creating puzzle link.'
						);

						$params['view']          = 'puzzle.MakePuzzle';
						$params['itemId']        = $itemId;
						$params['return']        = 1;
						$links[$item->getId()][] = array(
							'text'   => $this->translate('Puzzle'),
							'params' => $params,
						);
					}
				} // is photo item
			}
		}

		return array(null, $links);
	}

	// if there is a Gallery function which does this, please send me an email about it
	public function findParentAlbum($item) {
		global $gallery;

		$found = false;

		// just being paranoid
		$count = 0;

		// The parent of any photo should be the album, but we traverse the tree to make sure
		while (true) {
			// Avoid an infinite loop.
			if (++$count > 10000) {
				return array(GalleryCoreApi::error(ERROR_UNKNOWN, __FILE__, __LINE__), null);
			}

			list($ret, $parent) = $item->fetchParent();

			if ($ret) {
				return array($ret, null);
			}

			if (GalleryUtilities::isA($parent, 'GalleryAlbumItem')) {
				return array(null, $parent);
			}

			$item = $parent;
		}
	}

	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemEditOption',
			'PuzzleOptions',
			'PuzzleOptions',
			'modules/puzzle/PuzzleOptions.inc',
			'puzzle',
			array('ItemEditItem')
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}
}
