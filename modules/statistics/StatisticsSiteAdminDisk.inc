<?php
/*
 * $RCSfile: StatisticsAdminCursory.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Cursory Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Cursory Public License for more details.
 *
 * You should have received a copy of the GNU Cursory Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

GalleryCoreApi::requireOnce('modules/statistics/StatisticsSiteAdmin.inc');/**
 * @version $Revision: 1036 $
 * @package Statistics
 * @author Aidan Lister <aidan@php.net>
 */
class StatisticsSiteAdminDisk extends StatisticsSiteAdminPlugin {
	public function loadTemplate(&$template, &$form) {
		$cacheParams = array(
			'type'   => 'entity',
			'itemId' => 'Statistics_SiteAdmin_Disk_Module',
		);
		$cache       = GalleryDataCache::getFromDisk($cacheParams);
		$statistics  = array();

		// Safety check for when blank cache
		if (is_array($cache)) {
			$statistics['gallery'] = $this->_sizeReadable($cache['base']);
			$statistics['albums']  = $this->_sizeReadable($cache['albums']);
			$statistics['galbums'] = $this->_sizeReadable($cache['galbums']);

			$statistics['derivatives']  = $this->_sizeReadable($cache['derivative']);
			$statistics['gderivatives'] = $this->_sizeReadable($cache['gderivative']);

			$statistics['database'] = $this->_sizeReadable($cache['database']);
			$statistics['total']    = $this->_sizeReadable($cache['base'] + $cache['database']);

			$statistics['used']     = $this->_sizeReadable($cache['used']);
			$statistics['capacity'] = $this->_sizeReadable($cache['capacity']);
			$statistics['free']     = $this->_sizeReadable($cache['capacity'] - $cache['used']);
			$statistics['percent']  = round((($cache['capacity'] - $cache['used']) / $cache['capacity']) * 100);
		}

		$template->setVariable(
			'StatisticsSiteAdmin',
			array(
				'Disk' => $statistics,
			)
		);

		// Controller
		$template->setVariable('controller', 'statistics.StatisticsSiteAdmin');

		return array(null, 'modules/statistics/templates/StatisticsSiteAdminDisk.tpl', 'modules_statistics');
	}

	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'statistics');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Disk Usage'));
	}

	public function isAppropriate() {
		return array(null, true);
	}

	public function recalculateTask($module, $templateAdapter) {
		global $gallery;
		$taskname = $module->translate('Calculating Disk Usage');

		// Reset progress bar
		$templateAdapter->updateProgressBar(
			$taskname,
			$module->translate('Calculating size of database'),
			0
		);

		// Size of physical database
		// Get major mysql version
		$query               = 'SELECT SUBSTRING(VERSION(),1,1)';
		list($ret, $results) = $gallery->search($query);

		if ($ret) {
			return $ret;
		}

		$dbversion = $results->nextResult();
		$dbversion = (int)$dbversion[0];

		// Query the database
		$query               = 'SHOW TABLE STATUS';
		list($ret, $results) = $gallery->search($query);

		if ($ret) {
			return $ret;
		}

		$total = 0;

		while ($result = $results->nextResult()) {
			// Data_length + Index_length
			switch ($dbversion) {
				case 4:
					$total += $result[5] + $result[7];

					break;

				case 3:
				case 5:
					$total += $result[6] + $result[8];

					break;
			}
		}
		$statistics['database'] = $total;

		// Size of items according to database
		$query           = 'SELECT SUM([GalleryDataItem::size]) FROM [GalleryDataItem]';
		list($ret, $res) = $gallery->search($query);

		if ($ret) {
			return $ret;
		}

		$row                   = $res->nextResult();
		$statistics['galbums'] = $row[0];

		// Size of derivatives according to database
		$query           = 'SELECT SUM([GalleryDerivative::derivativeSize]) FROM [GalleryDerivative]';
		list($ret, $res) = $gallery->search($query);

		if ($ret) {
			return $ret;
		}

		$row                       = $res->nextResult();
		$statistics['gderivative'] = $row[0];


		// Paths
		$progress = 0.1;
		$minitask = $module->translate('Scanning root directory');
		$templateAdapter->updateProgressBar(
			$taskname,
			$minitask,
			$progress
		);
		$base       = $gallery->getConfig('data.gallery.base');
		$derivative = $gallery->getConfig('data.gallery.cache') . 'derivative/';
		$albums     = $gallery->getConfig('data.gallery.albums');

		$statistics['capacity'] = disk_total_space($base);
		$statistics['used']     = disk_total_space($base) - disk_free_space($base);

		$statistics['base'] = $this->_dirSize($base, $taskname, $minitask, $progress, $module, $templateAdapter);

		$progress = 0.5;
		$minitask = $module->translate('Scanning derivative folder');
		$templateAdapter->updateProgressBar(
			$taskname,
			$minitask,
			$progress
		);
		$statistics['derivative'] = $this->_dirSize($derivative, $minitask, $taskname, $progress, $module, $templateAdapter);

		$progress = 0.8;
		$minitask = $module->translate('Scanning album folder');
		$templateAdapter->updateProgressBar(
			$taskname,
			$minitask,
			$progress
		);
		$statistics['albums'] = $this->_dirSize($albums, $taskname, $minitask, $progress, $module, $templateAdapter);

		// Put to disk
		$cacheParams        = array(
			'type'   => 'entity',
			'itemId' => 'Statistics_SiteAdmin_Disk_Module',
		);
		list($ret, $status) = GalleryDataCache::putToDisk($cacheParams, $statistics);

		if ($ret) {
			return $ret;
		}

		return array(null, true);
	}

	/**
	 * Return human readable sizes
	 */
	public function _sizeReadable($size) {
		// Units
		$sizes = array('MB', 'GB', 'TB');
		$mod   = 1000;
		$ii    = count($sizes) - 1;

		// Loop
		$i    = 0;
		$size = $size / $mod / $mod;

		while ($size >= $mod && $i < $ii) {
			$size /= $mod;
			$i++;
		}

		return sprintf('%01.2f %s', $size, $sizes[$i]);
	}

	/**
	 * Calculate the size of a directory
	 *
	 * @param string $path
	 * @param object $templateAdapter
	 */
	public function _dirSize($path, $taskname, $minitask, $progress, $module, $templateAdapter) {
		// Init
		$size = 0;

		// Trailing slash
		if (substr($path, -1, 1) !== DIRECTORY_SEPARATOR) {
			$path .= DIRECTORY_SEPARATOR;
		}

		// Sanity check
		if (is_file($path)) {
			return filesize($path);
		}

		if (!is_dir($path)) {
			return false;
		}

		// Iterate queue
		$queue       = array($path);
		$queue_count = 1;
		$i           = 0;

		while ($i < $queue_count) {
			$current = $queue[$i++];
			// Open directory
			if ($dh = opendir($current)) {
				while (false !== ($entry = readdir($dh))) {
					// Skip pointers
					if ($entry == '.' || $entry == '..') {
						continue;
					}

					// Get list of directories or filesizes
					$path = $current . $entry;

					if (is_file($path)) {
						$size += filesize($path);
					} elseif (is_dir($path)) {
						$queue[$queue_count++] = $path . DIRECTORY_SEPARATOR;
					}
				}

				closedir($dh);
			}

			$templateAdapter->updateProgressBar(
				$taskname,
				$minitask . ' (' . $size . ')',
				$progress += 0.0001
			);
		}

		return $size;
	}
}
