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
 * @version $Revision: 1119 $
 * @package Statistics
 * @author Aidan Lister <aidan@php.net>
 *//**
 * Statistics
 *
 * Provides statistics about Gallery
 *
 * @package Statistics
 */
class StatisticsModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('statistics');
		$this->setName($gallery->i18n('Statistics'));
		$this->setDescription($gallery->i18n('Provides statistics about this Gallery installation'));
		$this->setVersion('7.0.0');
		$this->setCallbacks('getSiteAdminViews');
		$this->setGroup('data', $gallery->i18n('Extra Data'));
		$this->setRequiredCoreApi(array(7, 0));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * Load as Singleton
	 */
	public static function doStatic() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new StatisticsModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		// We don't require any special configuration
		return array(null, true);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		$views = array();

		// These views are always enabled
		$views[] = array(
			'name' => $this->translate('Statistics'),
			'view' => 'statistics.StatisticsSiteAdmin',
		);

		return array(null, $views);
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {

		// Cursory information plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'StatisticsSiteAdminPlugin',
			'StatisticsSiteAdminCursory',
			'StatisticsSiteAdminCursory',
			'modules/statistics/StatisticsSiteAdminCursory.inc',
			'statistics',
			null
		);

		if ($ret) {
			return $ret;
		}

		// Disk Usage plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'StatisticsSiteAdminPlugin',
			'StatisticsSiteAdminDisk',
			'StatisticsSiteAdminDisk',
			'modules/statistics/StatisticsSiteAdminDisk.inc',
			'statistics',
			null
		);

		if ($ret) {
			return $ret;
		}

		// Register our Maintenance task
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'MaintenanceTask',
			'RecalculateStatisticsTask',
			'RecalculateStatisticsTask',
			'modules/statistics/classes/RecalculateStatistics.class',
			'statistics',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}
}
