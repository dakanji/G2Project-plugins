<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

/**
 * @package Force Permissions
 * @version $Revision: 15513 $
 * @author
 */

/**
 * Force Permissions
 *
 * Force all items moved into an album to adopt the same permissions as
 * the containing album.
 *
 * @package Forcepermissions
 */
class ForcepermissionsModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('forcepermissions');
		$this->setName($gallery->i18n('Force Permissions'));
		$this->setDescription(
			$gallery->i18n(
				'Force parent album permissions on child items'
			)
		);

		// Update upgrade() as required
		$this->setVersion('2.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('data', $gallery->i18n('Extra Data'));
		$this->setCallbacks('registerEventListeners');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new ForcepermissionsModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::registerEventListeners
	 */
	public function registerEventListeners() {
		$ForcepermissionsModule = new ForcepermissionsModule();
		GalleryCoreApi::registerEventListener(
			'GalleryEntity::save',
			$ForcepermissionsModule
		);
	}

	/**
	 * Event handler for GalleryEntity::save event.
	 * Apply permissions of parent album to items moved into it.
	 *
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		$entity = $event->getEntity();

		if (GalleryUtilities::isA($entity, 'GalleryItem')
			&& !$entity->testPersistentFlag(STORAGE_FLAG_NEWLY_CREATED)
			&& $entity->isModified('parentId')
		) {
			// Copy the parent album's permissions to the item
			if (GalleryUtilities::isA($entity, 'GalleryAlbumItem')) {
				$ret = GalleryCoreApi::copyPermissions($entity->id, $entity->parentId);

				if ($ret) {
					return array($ret, null);
				}

				list($ret, $childIds) = GalleryCoreApi::fetchDescendentItemIds($entity);

				if ($ret) {
					return array($ret, null);
				}

				foreach ($childIds as $childId) {
					$ret = GalleryCoreApi::copyPermissions($childId, $entity->parentId);

					if ($ret) {
						return array($ret, null);
					}
				}
			} else {
				$ret = GalleryCoreApi::copyPermissions($entity->id, $entity->parentId);

				if ($ret) {
					return array($ret, null);
				}
			}
		}

		return array(null, null);
	}
}
