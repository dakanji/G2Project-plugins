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
 * The Bulk Manage module
 *
 * @version $Revision: 1213 $
 * @package BulkManage
 * @author Aidan Lister <aidan@php.net>
 */
class BulkManageModule extends GalleryModule {
	/**
	 * Hold the API version this module provides to plugins
	 *
	 * @var array
	 */
	public $_bulkManageApiVersion;

	/**
	 * Constructor
	 */
	public function __construct() {
		global $gallery;
		$this->setId('bulkmanage');
		$this->setName($gallery->i18n('Bulk Manage'));
		$this->setDescription($gallery->i18n('Facilitates the bulk editing of items'));
		$this->setVersion('7.0.0');
		$this->setCallbacks('getItemLinks|getItemAdminViews');
		$this->setGroup('gallery', $this->translate('Gallery'));
		$this->setRequiredCoreApi(array(7, 2));
		$this->setRequiredModuleApi(array(3, 0));
		$this->_templateVersion      = 1;
		$this->_bulkManageApiVersion = array(1, 0);
	}

	/**
	 * Load as Singleton
	 */
	public static function doStatic() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new BulkManageModule();
		}

		return $singleton;
	}

	/**
	 * Get the API version this module provides to plugins
	 *
	 */
	public function getBulkManageApiVersion() {
		return $this->_bulkManageApiVersion;
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		// We don't require any special configuration
		return array(null, true);
	}

	/**
	 * @see GalleryModule::getItemAdminViews
	 */
	public function getItemAdminViews($item) {
		$views = array();

		if (!$item->getCanContainChildren()) {
			return array(null, $views);
		}

		list($ret, $permissions) = GalleryCoreApi::getPermissions($item->getId());

		if ($ret) {
			return array($ret, null);
		}
		list($ret, $childCounts) = GalleryCoreApi::fetchChildCounts(array($item->getId()));

		if ($ret) {
			return array($ret, null);
		}
		$childCount = empty($childCounts[$item->getId()]) ? 0 : $childCounts[$item->getId()];

		if (isset($permissions['core.edit']) && $childCount > 1) {
			$views[] = array(
				'name' => $this->translate('Bulk Manage'),
				'view' => 'bulkmanage.BulkManage',
			);
		}

		return array(null, $views);
	}

	/**
	 * @see GalleryModule::getItemLinks
	 */
	public function getItemLinks($items, $wantsDetailedLinks, $permissions) {
		$links   = array();
		$itemIds = array();

		foreach ($items as $item) {
			if ($item->getCanContainChildren()) {
				$itemIds[] = $item->getId();
			}
		}

		if (!empty($itemIds)) {
			foreach ($itemIds as $itemId) {
				if (isset($permissions[$itemId]['core.edit'], $wantsDetailedLinks[$itemId])
				) {
					$links[$itemId][] = array(
						'text'   => $this->translate('Bulk Manage'),
						'params' => array(
							'view'    => 'core.ItemAdmin',
							'subView' => 'bulkmanage.BulkManage',
							'itemId'  => $itemId,
							'return'  => 1,
						),
					);
				}
			}
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'BulkManagePlugin',
			'BulkManageRescan',
			'BulkManageRescan',
			'modules/bulkmanage/BulkManageRescan.inc',
			'bulkmanage',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'BulkManagePlugin',
			'BulkManageProperties',
			'BulkManageProperties',
			'modules/bulkmanage/BulkManageProperties.inc',
			'bulkmanage',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}
}
