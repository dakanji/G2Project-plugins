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
 *//**
 * @version $Revision: 1036 $
 * @package BulkManage
 * @author Aidan Lister <aidan@php.net>
 */
class StatisticsSiteAdminController extends GalleryController {
	public function handleRequest($form) {
		$statisticsPlugin = GalleryUtilities::getRequestVariables('statisticsPlugin');

		// Load the correct plugin
		list($ret, $plugin) = GalleryCoreApi::newFactoryInstanceById('StatisticsSiteAdminPlugin', $statisticsPlugin);

		if ($ret) {
			return array($ret, null);
		}

		if (!isset($plugin)) {
			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
		}

		// Hand over to the plugin
		list($ret, $results, $requireProgressBar) = $plugin->handleRequest($form, $itemId);

		if ($ret) {
			return array($ret, null);
		}

		// If we have an error then pass back to plugin to handle it
		if (!empty($results['error'])) {
			$results['delegate']['view']             = 'core.SiteAdmin';
			$results['delegate']['subView']          = 'statistics.StatisticsSiteAdmin';
			$results['delegate']['statisticsPlugin'] = $statisticsPlugin;
		}

		if ($requireProgressBar) {
			$results['delegate']['view'] = 'core.ProgressBar';
		}

		return array(null, $results);
	}
}

class StatisticsSiteAdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$statisticsPlugin = GalleryUtilities::getRequestVariables('statisticsPlugin');

		// Get all the plugins
		list($ret, $allPluginIds) = GalleryCoreApi::getAllFactoryImplementationIds('StatisticsSiteAdminPlugin');

		if ($ret) {
			return array($ret, null);
		}

		$pluginInstances = array();

		foreach (array_keys($allPluginIds) as $pluginId) {
			list($ret, $plugin) = GalleryCoreApi::newFactoryInstanceById('StatisticsSiteAdminPlugin', $pluginId);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $isAppropriate) = $plugin->isAppropriate();

			if ($ret) {
				return array($ret, null);
			}

			if ($isAppropriate) {
				$pluginInstances[$pluginId] = $plugin;
			}
		}

		// Get the preferred plugin from the session
		$session                    =& $gallery->getSession();
		$statisticsPluginSessionKey = 'statistics.view.plugin';

		if (empty($statisticsPlugin) || !isset($pluginInstances[$statisticsPlugin])) {
			$statisticsPlugin = $session->get($statisticsPluginSessionKey);

			if (empty($statisticsPlugin) || !isset($pluginInstances[$statisticsPlugin])) {
				$ids              = array_keys($pluginInstances);
				$statisticsPlugin = $ids[0];
			}
		}

		$session->put($statisticsPluginSessionKey, $statisticsPlugin);

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
				'isSelected' => ($pluginId == $statisticsPlugin),
			);
		}

		$Statistics                     = array();
		$Statistics['statisticsPlugin'] = $statisticsPlugin;
		$Statistics['plugins']          = $plugins;

		// Let the plugin load its template data
		list($ret, $Statistics['pluginFile'], $Statistics['pluginL10Domain']) = $pluginInstances[$statisticsPlugin]->loadTemplate($template, $form);

		if ($ret) {
			return array($ret, null);
		}

		$template->setVariable('Statistics', $Statistics);
		$template->setVariable('controller', 'statistics.StatisticsSiteAdmin');

		return array(
			null,
			array(
				'body' => 'modules/statistics/templates/StatisticsSiteAdmin.tpl',
			),
		);
	}
}

class StatisticsSiteAdminPlugin {
	public function isSupported($item) {
		return false;
	}

	public function loadTemplate(&$template, &$form) {
		return array(GalleryCoreApi::error(ERROR_UNIMPLEMENTED), null, null);
	}

	public function getTitle() {
		return array(GalleryCoreApi::error(ERROR_UNIMPLEMENTED), null);
	}

	public function hasRecalculateTask() {
		return array(null, method_exists($this, 'recalculateTask'));
	}
}
