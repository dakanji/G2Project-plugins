<?php
/*
 * $RCSfile: module.inc $
 * $Revision: 1845 $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
 * AlbumTimestamp
 *
 * This module provides an option to automatically set an albums timestamp
 * based on its child items times.
 *
 * @package AlbumTimestamp
 * @author Leo Bergolth <leo@strike.wu.ac.at>
 * @version
 */
 // also extends GalleryEventListener
class AlbumTimestampModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('albumtimestamp');
		$this->setName($gallery->i18n('Album Timestamp'));
		$this->setDescription(
			$gallery->i18n(
				'Automatically set album timestamps ' .
						 'based on child item times.'
			)
		);
		$this->setVersion('0.2.0');
		$this->_templateVersion = 1;
		$this->setGroup('gallery', $this->translate('Gallery'));
		$this->setRequiredCoreApi(array(7, 54));
		$this->setRequiredModuleApi(array(3, 9));
	}

	/**
	 * Load as Singleton
	 */
	public static function doStatic() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new AlbumTimestampModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEventListener',
			'AlbumTimestampModule',
			'AlbumTimestampModule',
			'modules/albumtimestamp/module.inc',
			'albumtimestamp',
			array('GalleryEntity::save', 'GalleryEntity::delete')
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemEditOption',
			'AlbumTimestampItemOption',
			'AlbumTimestampItemOption',
			'modules/albumtimestamp/AlbumTimestampItemOption.inc',
			'albumtimestamp',
			array('ItemEditItem')
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * Handle the timestamp of the parent album when an item is saved/deleted
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		global $gallery;
		GalleryCoreApi::requireOnce('modules/albumtimestamp/classes/AlbumTimestampHelper.class');

		if (!AlbumTimestampHelper::eventHandlerEnabled()) {
			// temporarily disabled
			AlbumTimestampHelper::log('LEO: handleEvent: handler disabled');

			return array(null, null);
		}
		$eventName = $event->getEventName();
		$item      = $event->getEntity();

		if (!GalleryUtilities::isA($item, 'GalleryChildEntity')) {
			// this handler is only for items and albums, not e.g. for users or groups
			return array(null, null);
		}

		$itemId  = $item->getId();
		$albumId = $item->getParentId();
		$isNew   = $item->testPersistentFlag(STORAGE_FLAG_NEWLY_CREATED);
		AlbumTimestampHelper::log("LEO: handleEvent: $eventName itemId=$itemId");

		if (GalleryUtilities::isA($item, 'GalleryAlbumItem') && $eventName == 'GalleryEntity::save'
			&& $isNew
		) {
			/* it's a new album
			set the albumTimestampMode according to newAlbumTimestampMode
			   no inheritance here - only set the mode if the parent album has an explicit setting */
			list($ret, $newAlbumMode) = AlbumTimestampHelper::getItemParam('newAlbumTimestampMode', $albumId, 0);

			if ($ret) {
				return array($ret, null);
			}
			AlbumTimestampHelper::log("LEO: handleEvent: new album $itemId, mode: $newAlbumMode");

			if (isset($newAlbumMode)) {
				list($ret, $oldValue) = AlbumTimestampHelper::setItemParam(
					'albumTimestampMode',
					$newAlbumMode,
					$itemId
				);

				if ($ret) {
					return array($ret, null);
				}
			}
		}

		if (!(GalleryUtilities::isA($item, 'GalleryItem')
			&& $item->isModified('originationTimestamp')
			&& isset($albumId))
		) {
			return array(null, null);
		}

		list($ret, $albumTimestampMode) = AlbumTimestampHelper::getItemParam('albumTimestampMode', $albumId, 1);

		if ($ret) {
			return array($ret, null);
		}

		if (is_null($albumTimestampMode) || $albumTimestampMode == 'off') {
			return array(null, null);
		}

		$prevTimestamp     = $item->getOriginalValue('originationTimestamp');
		$currentTimestamp  = $item->getOriginationTimestamp();
		list($ret, $album) = $item->fetchParent();

		if ($ret) {
			return array($ret, null);
		}
		$albumTimestamp = $album->getOriginationTimestamp();

		switch ($eventName) {
			case 'GalleryEntity::save':
				if (isset($prevTimestamp)) {
					// some shortcuts for special cases
					if (($albumTimestampMode == 'newest') || ($albumTimestampMode == 'oldest')) {
						if ((($albumTimestampMode == 'newest')
							&& ($currentTimestamp > $albumTimestamp))
							|| (($albumTimestampMode == 'oldest')
							&& ($currentTimestamp < $albumTimestamp))
						) {
							$ret = AlbumTimestampHelper::setOriginationTimestamp(
								$album,
								$currentTimestamp
							);

							if ($ret) {
								return array($ret, null);
							}

							break;
						}

						if ($albumTimestamp != $prevTimestamp) {
							// nothing to do
							break;
						}
					}
				}
				// in all other cases we have to determine the new album timestamp
				// TODO: $album is locked - need to set it directly here
				$ret = AlbumTimestampHelper::autoSetAlbumTimestamps(
					$album,
					$albumTimestampMode,
					0,
					$item
				);

				if ($ret) {
					return array($ret, null);
				}

				break;

			case 'GalleryEntity::delete':
				if ($albumTimestamp == $prevTimestamp) {
					// this item did define the album timestamp
					$ret = AlbumTimestampHelper::autoSetAlbumTimestamps(
						$album,
						$albumTimestampMode,
						0,
						$item
					);

					if ($ret) {
						return array($ret, null);
					}
				}

				break;
		}

		return array(null, null);
	}
}
