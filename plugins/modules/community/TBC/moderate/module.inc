<?php

/*
 * $RCSfile: module.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
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
 * The implementation of the moderation-module
 * @package Moderation
 * @author Emanuele Pedrona <e.pedrona@edronsoft.com>
 * @version $Revision: 1861 $ $Date: 2006/04/21 20:28:14 $
 */
class ModerateModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('moderate');
		$this->setName($gallery->i18n('Moderate'));
		$this->setDescription(
			$gallery->i18n(
				'User/Moderation'
			)
		);

		// Update upgrade() as required
		$this->setVersion('2.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('gallery', $gallery->i18n('Gallery'));
		$this->setCallbacks('registerEventListeners|getSiteAdminViews');
	}

	/**
	 * @see GalleryModule::registerEventListeners
	 */
	public function registerEventListeners() {
		GalleryCoreApi::registerEventListener('GalleryEntity::delete', new ModerateModule());
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => 'Moderate',
					'view' => 'moderate.AdminModeration',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemAddOption',
			'ModerateOption',
			'ModerateOption',
			'modules/moderate/ModerateOption.inc',
			'moderate',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * Event handler for GalleryEntity::delete events
	 * Hide user root album if it becomes empty
	 *
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		$result = null;

		if ($event->getEventName() == 'GalleryEntity::delete') {
			$thisEntity = $event->getEntity();

			if ($thisEntity->getEntityType() != 'GalleryAlbumItem'
				&& $thisEntity->getEntityType() != 'GalleryPhotoItem'
			) {
				return array(null, $result);
			}

			//check if parent item is root album for user
			list($ret, $core) = GalleryCoreApi::fetchAllPluginParameters('module', 'core');

			if ($ret) {
				return array($ret, null);
			}

			$rootId = $core['id.rootAlbum'];

			list($ret, $parentEntity) = GalleryCoreApi::loadEntitiesById(
				$thisEntity->getParentId()
			);

			if ($ret) {
				return array($ret, null);
			}

			if ($parentEntity->getId() != $rootId) {
				list($ret, $rootEntity) = GalleryCoreApi::loadEntitiesById(
					$parentEntity->getParentId()
				);

				if ($ret) {
					return array($ret, null);
				}

				if ($rootEntity->getId() == $rootId) {
					list($ret, $child_count) = GalleryCoreApi::fetchChildCounts(
						array($parentEntity->getId())
					);

					if ($ret) {
						return array($ret, null);
					}

					if ($child_count[$parentEntity->getId()] <= 1) {
						/*	$ret = GalleryCoreApi::deleteEntityById($parentEntity->getId());

						if ($ret) {
						return array($ret, null);
						}

						*/
						$ret = GalleryCoreApi::removeGroupPermission(
							$parentEntity->getId(),
							$core['id.everybodyGroup'],
							'core.viewAll',
							false
						);

						if ($ret) {
							return array($ret, null);
						}
					}
				}
			}
		}

		return array(null, $result);
	}
}
