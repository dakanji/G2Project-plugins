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
 * Provide Bulk Property Editing
 *
 * @version $Revision: 1036 $
 * @package BulkManage
 * @subpackage UserInterface
 * @author Aidan Lister <aidan@php.net>
 */
class BulkManageProperties extends BulkManagePlugin {
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

		// Get the active parameters
		foreach ($form['property'] as $key => $property) {
			// Check if the "active" checkbox was tagged
			if (isset($property['active']) && $property['active'] === 'on') {
				// Build an array of parameters to pass to the stacker
				unset($property['active']);
				$params[$key] = $property;
			}
		}

		// Errors
		if (empty($params)) {
			$error[] = 'form[error][noFilters]';
		}

		return array(null, $error, $status, $params);
	}

	/**
	 * @see BulkManagePlugin::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		// Controller
		$template->setVariable('controller', 'bulkmanage.BulkManage');
		$template->style('modules/bulkmanage/bulkmanage.css');

		if ($form['formName'] != 'BulkManageProperties') {
			$form['formName']                             = 'BulkManageProperties';
			$form['property']['enumfilename']['pattern']  = '%04d.%s';
			$form['property']['enumfilename']['startnum'] = '1';
		}

		return array(null, 'modules/bulkmanage/templates/BulkManageProperties.tpl', 'modules_bulkmange');
	}

	/**
	 * @see BulkManagePlugin::getTitle
	 */
	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'bulkmanage');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Properties'));
	}

	/**
	 * @see BulkManagePlugin::getAction
	 */
	public function getActionTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'bulkmanage');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Editing Items'));
	}

	/**
	 * @see BulkManagePlugin::processItem
	 */
	public function processItem($item, $params) {
		// Deal with GalleryDataItem's only
		if (!GalleryUtilities::isA($item, 'GalleryDataItem')) {
			return array(null, false);
		}

		// Enumerate filename
		if (isset($params['enumfilename'])) {
			$startnum = (int)$params['enumfilename']['startnum'];
			static $i = 0;

			// Get filename
			$filename = $item->getPathComponent();
			$pathinfo = pathinfo($filename);

			// Get extension
			if (isset($pathinfo['extension'])) {
				$ext = (isset($params['enumfilename']['uppercase'])
						&& $params['enumfilename']['uppercase'] === 'on') ? strtoupper($pathinfo['extension']) : $pathinfo['extension'];
			} else {
				$ext = '';
			}

			// Rename file
			$newfilename = sprintf($params['enumfilename']['pattern'], $startnum + $i++, $ext);

			$ret = $item->rename($newfilename);

			if ($ret) {
				return array($ret, null);
			}
		}

		// Blank the title
		if (isset($params['blanktitle'])) {
			if ($params['blanktitle']['blank'] === 'on') {
				$item->setTitle('');
			}
		}

		// Blank the summary
		if (isset($params['blanksummary'])) {
			if ($params['blanksummary']['blank'] === 'on') {
				$item->setSummary('');
			}
		}

		// Blank the keywords
		if (isset($params['blankkeywords'])) {
			if ($params['blankkeywords']['blank'] === 'on') {
				$item->setKeywords('');
			}
		}

		// Blank the description
		if (isset($params['blankdescription'])) {
			if ($params['blankdescription']['blank'] === 'on') {
				$item->setDescription('');
			}
		}

		// Reset the timestamp to our exif data
		if (isset($params['resettimestamp'])) {
			if ($params['resettimestamp']['reset'] === 'on') {
				list($ret, $originationTimestamp) = GalleryCoreApi::fetchOriginationTimestamp($item);

				if ($ret) {
					return array($ret, null);
				}

				$item->setOriginationTimestamp($originationTimestamp);
			}
		}

		// Shift the timestamp an arbitrary amount
		if (isset($params['shifttimestamp'])) {
			if (!empty($params['shifttimestamp']['shift'])) {
				$oldtimestamp = $item->getOriginationTimestamp();
				$item->setOriginationTimestamp($oldtimestamp + (int)$params['shifttimestamp']['shift']);
			}
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
		return array(null, 'BulkManageProperties');
	}
}
