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
 * The implementation of the TagTree module
 *
 * @package TagTree
 * @author Chad Parry <spam@chad.parry.org>
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 1733 $
 */
class TagTreeModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('tagtree');
		$this->setName($gallery->i18n('Tag Tree'));
		$this->setDescription(
			$gallery->i18n('Jump directly to any album using a hierarchical tag view')
		);
		$this->setVersion('1.1.0');
		$this->setGroup('blocks', $gallery->i18n('Blocks'));
		$this->setCallbacks('getSiteAdminViews|registerEventListeners');
		$this->setRequiredCoreApi(array(7, 5));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * @see GalleryModule::registerEventListeners
	 */
	public function registerEventListeners() {
		$listener = new TagTreeModule();
		GalleryCoreApi::registerEventListener('Gallery::ViewableTreeChange', $listener, true);
		GalleryCoreApi::registerEventListener('Gallery::ItemOrder', $listener, true);
		GalleryCoreApi::registerEventListener('GalleryEntity::save', $listener, true);
		GalleryCoreApi::registerEventListener('GalleryTagMap::Change', $listener, true);
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion) {
		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'tagtree');

		if ($ret) {
			return $ret;
		}

		foreach (array(
			'showItems'          => 0,
			'usePersistence'     => 0,
			'treeExpandCollapse' => 0,
		) as $key => $value) {
			if (!isset($params[$key])) {
				$ret = $this->setParameter($key, $value);

				if ($ret) {
					return $ret;
				}
			}
		}

		GalleryDataCache::removeFromDisk(
			array(
				'type'   => 'module-data',
				'module' => 'tagtree',
			)
		);

		return null;
	}

	/**
	 * @see GalleryModule::uninstall
	 */
	public function uninstall() {
		GalleryDataCache::removeFromDisk(
			array(
				'type'   => 'module-data',
				'module' => 'tagtree',
			)
		);

		return parent::uninstall();
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 */
	public function needsConfiguration() {
		GalleryCoreApi::requireOnce('modules/tagtree/classes/TagTreeCompatibilityHelper.class');
		list($ret, $isCompatible) = TagTreeCompatibilityHelper::isCompatibleTagsModuleActive();

		if ($ret) {
			return array($ret, null);
		}

		return array(null, !$isCompatible);
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		list($ret, $needsConfiguration) = $this->needsConfiguration();

		if ($ret) {
			return array($ret, null);
		}
		// The Tags module needs to be updated, and the user should request that explicitly.
		return array(null, !$needsConfiguration);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Tag Tree'),
					'view' => 'tagtree.TagTreeSiteAdmin',
				),
			),
		);
	}

	/**
	 * Event handler for Gallery::ViewableTreeChange
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		$invalidateCache = false;

		if ($event->getEventName() == 'Gallery::ViewableTreeChange') {
			$invalidateCache = true;
		} elseif ($event->getEventName() == 'GalleryEntity::save') {
			$entity = $event->getEntity();

			if ($entity->getEntityType() == 'GalleryAlbumItem'
				and $entity->isModified('title')
			) {
				$invalidateCache = true;
			}
		} elseif ($event->getEventName() == 'GalleryTagMap::Change') {
			$invalidateCache = true;
		}

		if ($invalidateCache) {
			GalleryDataCache::removeFromDisk(
				array(
					'type'   => 'module-data',
					'module' => 'tagtree',
				)
			);
		}

		return array(null, null);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'tagtree.TagTreeConfigAdmin';
	}
}
