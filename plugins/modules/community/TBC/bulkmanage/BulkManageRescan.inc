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

/**
 * @version $Revision: 1034 $
 * @package BulkManage
 * @subpackage UserInterface
 * @author Aidan Lister <aidan@php.net>
 */
class BulkManageRescan extends BulkManagePlugin {
	/**
	 * @see BulkManagePlugin::handleRequest
	 */
	public function handleRequest($form) {
		$error  = array();
		$status = array();
		$params = array();

		// Accumulate parameters
		$recursive           = (isset($form['recursive']) && $form['recursive'] === 'on') ? true : false;
		$params['recursive'] = $recursive;
		return array(null, $error, $status, $params);
	}

	/**
	 * @see BulkManagePlugin::loadTemplate
	 */
	public function loadTemplate(&$template) {
		$template->setVariable('controller', 'bulkmanage.BulkManage');

		return array(null, 'modules/bulkmanage/templates/BulkManageRescan.tpl', 'modules_bulkmange');
	}

	/**
	 * @see BulkManagePlugin::getTitle
	 */
	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'bulkmanage');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Rescan'));
	}

	/**
	 * @see BulkManagePlugin::getAction
	 */
	public function getActionTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'bulkmanage');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Rescanning Items'));
	}

	/**
	 * @see BulkManagePlugin::processItem
	 */
	public function processItem($item) {
		// Deal with GalleryPhotoItem's and GalleryMovieItem's only
		if (!GalleryUtilities::isA($item, 'GalleryPhotoItem')
			&& !GalleryUtilities::isA($item, 'GalleryMovieItem')
		) {
			return array(null, false);
		}

		// Delete all the derivatives
		$ret = GalleryCoreApi::invalidateDerivativeDimensionsBySourceIds(array($item->id));

		if ($ret) {
			return array($ret, null);
		}

		// Rescan
		$ret = $item->rescan();

		if ($ret) {
			return array($ret, null);
		}

		// Save
		$ret = $item->save();

		if ($ret) {
			return array($ret, null);
		}

		return array(null, true);
	}

	/**
	 * @see BulkManagePlugin::getRequiredApiVersion
	 */
	public function getRequiredApiVersion() {
		return array(1, 0);
	}

	/**
	 * @see BulkManagePlugin::getId
	 */
	public function getId() {
		return array(null, 'BulkManageRescan');
	}
}
