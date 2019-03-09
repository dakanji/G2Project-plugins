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
GalleryCoreApi::requireOnce('modules/bulkmanage/classes/BulkManageHelper.class');/**
 * Handle all form requests
 *
 * Load the plugin and let it process the form data, then run the progress bar
 *
 * @version $Revision: 1036 $
 * @package BulkManage
 * @subpackage UserInterface
 * @author Aidan Lister <aidan@php.net>
 */
class BulkManageController extends GalleryController {
	public function handleRequest($form) {
		global $gallery;

		list($itemId, $bulkPlugin) = GalleryUtilities::getRequestVariables('itemId', 'bulkPlugin');

		// Load the item
		list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

		if ($ret) {
			return array($ret, null);
		}

		if (!$item->getCanContainChildren()) {
			// Invalid itemId given, return MISSING_OBJECT to get standard error page
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}

		// Check permissions
		$ret = GalleryCoreApi::assertHasItemPermission($item->getId(), 'core.edit');

		if ($ret) {
			return array($ret, null);
		}

		// Load the correct bulkmanage plugin
		list($ret, $plugin) = GalleryCoreApi::newFactoryInstanceById('BulkManagePlugin', $bulkPlugin);

		if ($ret) {
			return array($ret, null);
		}

		if (!isset($plugin)) {
			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
		}

		// Load the helper class
		$helper = new BulkManageHelper();

		// Check form
		if (isset($form['action']['bulkmanage'])) {
			// The "Go" button was hit so tell the plugin
			list($ret, $error, $status, $params) = $plugin->handleRequest($form);

			if ($ret) {
				return array($ret, null);
			}

			// Register progress bar
			$templateAdapter =& $gallery->getTemplateAdapter();
			$templateAdapter->registerTrailerCallback(
				array($helper, 'execute'),
				// Parameters passed to helper
					array($itemId, $plugin, $params)
			);

			$results['delegate']['view'] = 'core.ProgressBar';
		}

		// If we have an error then pass back to plugin to handle it
		$results['error']  = $error;
		$results['status'] = $status;
		if (!empty($results['error'])) {
			$results['delegate']['view']       = 'core.ItemAdmin';
			$results['delegate']['subView']    = 'bulkmanage.BulkManage';
			$results['delegate']['bulkPlugin'] = $bulkPlugin;
		}

		return array(null, $results);
	}
}

/**
 * This view will show the selected plugin for bulkmanaging items
 */
class BulkManageView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		list($itemId, $bulkPlugin) = GalleryUtilities::getRequestVariables('itemId', 'bulkPlugin');

		// Check permissions
		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.edit');

		if ($ret) {
			return array($ret, null);
		}

		// Load item
		list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

		if ($ret) {
			return array($ret, null);
		}

		// Get all the plugins
		list($ret, $pluginInstances) = BulkManageHelper::getAllPlugins();

		if ($ret) {
			return array($ret, null);
		}

		// Check atleast one plugin is available
		if (count($pluginInstances) === 0) {
			return array(GalleryCoreApi::error(ERROR_UNSUPPORTED_OPERATION), null);
		}

		// Get the preferred plugin from the session
		$session              =& $gallery->getSession();
		$bulkPluginSessionKey = 'bulkmanage.view.plugin.' . get_class($item);
		if (empty($bulkPlugin) || !isset($pluginInstances[$bulkPlugin])) {
			$bulkPlugin = $session->get($bulkPluginSessionKey);
			if (empty($bulkPlugin) || !isset($pluginInstances[$bulkPlugin])) {
				$ids        = array_keys($pluginInstances);
				$bulkPlugin = $ids[0];
			}
		}

		$session->put($bulkPluginSessionKey, $bulkPlugin);

		// Get display data for all plugins
		$plugins = array();
		foreach ($pluginInstances as $pluginId => $plugin) {
			list($ret, $title) = $plugin->getTitle();

			if ($ret) {
				return array($ret, null);
			}

			$plugins[] = array(
				'title'      => $title,
				'id'         => $pluginId,
				'isSelected' => ($pluginId == $bulkPlugin),
			);
		}

		$BulkManage               = array();
		$BulkManage['bulkPlugin'] = $bulkPlugin;
		$BulkManage['plugins']    = $plugins;
		$BulkManage['itemId']     = $item->getId();

		// Let the plugin load its template data
		list($ret, $BulkManage['pluginFile'], $BulkManage['pluginL10Domain']) = $pluginInstances[$bulkPlugin]->loadTemplate($template, $form, $item);

		if ($ret) {
			return array($ret, null);
		}

		$template->setVariable('BulkManage', $BulkManage);
		$template->setVariable('controller', 'bulkmanage.BulkManage');
		return array(
			null,
			array(
				'body' => 'modules/bulkmanage/templates/BulkManage.tpl',
			),
		);
	}
}

/**
 * Interface for plugins to extend
 * @abstract
 */
class BulkManagePlugin {
	/**
	 * Check the form was filled correctly and return a validated parameter array
	 *
	 * @see GalleryController::handleRequest
	 * @return array object GalleryStatus, array Errors, array Status, array Paramters
	 */
	public function handleRequest($form) {
		return array(GalleryCoreApi::error(ERROR_UNIMPLEMENTED), null, null, null);
	}

	/**
	 * Load a template
	 *
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		return array(GalleryCoreApi::error(ERROR_UNIMPLEMENTED), null, null);
	}

	/**
	 * Get plugin title
	 *
	 * @return array object GalleryStatus a status code
	 *               string Plugin's title
	 */
	public function getTitle() {
		return array(GalleryCoreApi::error(ERROR_UNIMPLEMENTED), null);
	}

	/**
	 * Apply changes to each individual item
	 * Note: Method is called by BulkManageHelper
	 *
	 * @param GalleryDataItem $item
	 * @param array Associative array of parameters
	 * @return array object GalleryStatus a status code
	 *               bool Success
	 */
	public function processItem($item, $params = null) {
		return array(GalleryCoreApi::error(ERROR_UNIMPLEMENTED), null);
	}

	/**
	 * Get required api version
	 *
	 * @return array int Major version
	 *               int Minor version
	 */
	public function getRequiredApiVersion() {
		return array(GalleryCoreApi::error(ERROR_UNIMPLEMENTED), null);
	}

	/**
	 * Get action title for progress bar
	 *
	 * @return array object GalleryStatus a status code
	 *               string The action title for the progress bar
	 */
	public function getActionTitle() {
		return array(GalleryCoreApi::error(ERROR_UNIMPLEMENTED), null);
	}

	/**
	 * Get the name of this plugin
	 *
	 * @return array object GalleryStatus a status code
	 *               string The name of this plugin
	 */
	public function getId() {
		return array(GalleryCoreApi::error(ERROR_UNIMPLEMENTED), null);
	}
}
