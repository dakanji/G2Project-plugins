<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
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
 * FIXME No controller This controller will handle the addition of an item as a children to
 * another item.
 *
 * @package Calendar
 * @subpackage UserInterface
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 847 $
 */
class CalendarMonthController extends GalleryController {
	/**
	 * ItemAddOption instances to use when handling this request.  Only used by test code.
	 *
	 * @var array (optionId => object ItemAddOption) $_optionInstances
	 * @access private
	 */
	public $_optionInstances;

	/**
	 * Tests can use this method to hardwire a specific set of option instances to use.
	 * This avoids situations where some of the option instances will do unpredictable
	 * things and derail the tests.
	 *
	 * @param array (optionId => ItemAddOption, ...)
	 */
	public function setOptionInstances($optionInstances) {
		$this->_optionInstances = $optionInstances;
	}

	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		list($itemId, $addPlugin) = GalleryUtilities::getRequestVariables('itemId', 'addPlugin');

		// Make sure we have permission to add to this item
		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.addDataItem');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $lockId) = GalleryCoreApi::acquireReadLock($item->getId());

		if ($ret) {
			return array($ret, null);
		}

		// Load the correct add plugin
		list($ret, $plugin) = GalleryCoreApi::newFactoryInstanceById('ItemAddPlugin', $addPlugin);

		if ($ret) {
			GalleryCoreApi::releaseLocks($lockId);

			return array($ret, null);
		}

		if (!isset($plugin)) {
			GalleryCoreApi::releaseLocks($lockId);

			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__), null);
		}

		list($ret, $error, $status) = $plugin->handleRequest($form, $item);

		if ($ret) {
			GalleryCoreApi::releaseLocks($lockId);

			return array($ret, null);
		}

		if (empty($error) && isset($status['addedFiles'])) {
			if (isset($this->_optionInstances)) {
				$optionInstances = $this->_optionInstances;
			} else {
				list($ret, $optionInstances) = ItemAddOption::getAllAddOptions();

				if ($ret) {
					GalleryCoreApi::releaseLocks($lockId);

					return array($ret, null);
				}
			}

			$addedItems = array();

			for ($i = 0; $i < count($status['addedFiles']); $i++) {
				$file =& $status['addedFiles'][$i];

				list($ret, $addedItem) = GalleryCoreApi::loadEntitiesById($file['id']);

				if ($ret) {
					GalleryCoreApi::releaseLocks($lockId);

					return array($ret, null);
				}

				// Check if we should extract individual files out of an archive
				$toolkit = null;

				if (GalleryUtilities::isA($addedItem, 'GalleryDataItem')) {
					list($ret, $toolkit) = GalleryCoreApi::getToolkitByOperation($addedItem->getMimeType(), 'extract');

					if ($ret) {
						GalleryCoreApi::releaseLocks($lockId);

						return array($ret, null);
					}
				}

				if (isset($toolkit)) {
					list($ret, $addedFiles) = $this->_extractAndAddFiles($addedItem, $toolkit);

					if ($ret) {
						GalleryCoreApi::releaseLocks($lockId);

						return array($ret, null);
					}

					$ret = GalleryCoreApi::deleteEntityById($addedItem->getId());

					if ($ret) {
						GalleryCoreApi::releaseLocks($lockId);

						return array($ret, null);
					}

					unset($status['addedFiles'][$i--]);
					$status['addedFiles'] = array_merge($status['addedFiles'], $addedFiles);
				} else {
					// This is not an archive, add it to our array of item objects
					$addedItems[] = $addedItem;
				}
			}

			// Allow ItemAddOptions to process added item(s)
			foreach ($optionInstances as $option) {
				list($ret, $optionErrors, $optionWarnings) = $option->handleRequestAfterAdd($form, $addedItems);

				if ($ret) {
					GalleryCoreApi::releaseLocks($lockId);

					return array($ret, null);
				}

				$error = array_merge($error, $optionErrors);

				// for each item, put the items warnings into our status array
				for ($j = 0; $j < count($optionWarnings); $j++) {
					$status['addedFiles'][$j]['warnings'] = array_merge($status['addedFiles'][$j]['warnings'], $optionWarnings[$j]);
				}
			}

			$results['redirect'] = array(
				'view'    => 'core.ItemAdmin',
				'subView' => 'core.ItemAddConfirmation',
				'itemId'  => $item->getId(),
			);
		} else {
			$results['delegate']['view']      = 'core.ItemAdmin';
			$results['delegate']['subView']   = 'core.ItemAdd';
			$results['delegate']['addPlugin'] = $addPlugin;
		}

		$ret = GalleryCoreApi::releaseLocks($lockId);

		if ($ret) {
			return array($ret, null);
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * FIXME Month view This view will prompt for files to add as children to an album.
 *
 * @package Calendar
 * @subpackage UserInterface
 */
class CalendarMonthView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		/*global $gallery;
		list ($itemId, $addPlugin) = GalleryUtilities::getRequestVariables('itemId', 'addPlugin');

		/* Make sure we have permission to add to this item

		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.addDataItem');

		if ($ret) {
		return array($ret, null);
		}

		list ($ret, $isAdmin) = GalleryCoreApi::isUserInSiteAdminGroup();

		if ($ret) {
		return array($ret, null);
		}

		list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

		if ($ret) {
		return array($ret, null);
		}

		/* Get all the add plugins
		list ($ret, $allPluginIds) =

		GalleryCoreApi::getAllFactoryImplementationIds('ItemAddPlugin');

		if ($ret) {
		return array($ret, null);
		}

		$pluginInstances = array();

		foreach (array_keys($allPluginIds) as $pluginId) {
		list ($ret, $plugin) =

		GalleryCoreApi::newFactoryInstanceById('ItemAddPlugin', $pluginId);

		if ($ret) {
		return array($ret, null);
		}

		list ($ret, $isAppropriate) = $plugin->isAppropriate();

		if ($ret) {
		return array($ret, null);
		}

		if ($isAppropriate) {
		$pluginInstances[$pluginId] = $plugin;
		}
		}

		/* Get all the add options
		list ($ret, $optionInstances) = ItemAddOption::getAllAddOptions();

		if ($ret) {
		return array($ret, null);
		}

		/*
		* If the plugin is empty get it from the session.  If it's empty there,
		* default to the first plugin we find.  Either way, save the user's
		* preference in the session.
		$session =& $gallery->getSession();
		$addPluginSessionKey = 'core.view.ItemAdd.addPlugin.' . get_class($item);

		if (empty($addPlugin) || !isset($pluginInstances[$addPlugin])) {
		$addPlugin = $session->get($addPluginSessionKey);

		if (empty($addPlugin) || !isset($pluginInstances[$addPlugin])) {
		$ids = array_keys($pluginInstances);
		$addPlugin = $ids[0];
		}
		}

		$session->put($addPluginSessionKey, $addPlugin);

		/* Get display data for all plugins
		$plugins = array();

		foreach ($pluginInstances as $pluginId => $plugin) {
		list ($ret, $title) =  $plugin->getTitle();

		if ($ret) {
		return array($ret, null);
		}

		$plugins[] = array('title' => $title,
				   'id' => $pluginId,
				   'isSelected' => ($pluginId == $addPlugin));
		}

		$ItemAdd = array();
		$ItemAdd['addPlugin'] = $addPlugin;
		$ItemAdd['plugins'] = $plugins;
		$ItemAdd['isAdmin'] = $isAdmin;

		/* Let the plugin load its template data
		list ($ret, $ItemAdd['pluginFile'], $ItemAdd['pluginL10Domain']) =

		$pluginInstances[$addPlugin]->loadTemplate($template, $form, $item);

		if ($ret) {
		return array($ret, null);
		}

		/* Now let all options load their template data
		$ItemAdd['options'] = array();

		foreach ($optionInstances as $option) {
		list ($ret, $entry['file'], $entry['l10Domain']) =

		$option->loadTemplate($template, $form, $item);

		if ($ret) {
		return array($ret, null);
		}

		if (!empty($entry['file'])) {
		$ItemAdd['options'][] = $entry;
		}
		}

		/* Make sure that we've got some toolkits
		list ($ret, $operations) = GalleryCoreApi::getToolkitOperations('image/jpeg');

		if ($ret) {
		return array($ret, null);
		}

		$ItemAdd['hasToolkit'] = false;

		for ($i = 0; $i < sizeof($operations); $i++) {
		if ($operations[$i]['name'] == 'thumbnail') {
		$ItemAdd['hasToolkit'] = true;

		break;
		}
		}

		$template->setVariable('ItemAdd', $ItemAdd);
		$template->setVariable('controller', 'core.ItemAdd');

		return array(null,
			 array('body' => 'modules/core/templates/ItemAdd.tpl'));*/
		$weekStartDate               = time() - (3 * 86400);
		$CalendarMonth               = array();
		$CalendarMonth['monthDates'] = array();
		$today                       = getdate();
		$startDate                   = time() - ($today['mday'] * 86400) - ((time() - $weekStartDate) % 604800);
		$endDate                     = mktime(0, 0, 0, $today['mon'] + 1, 0, $today['year']);
		$date                        = $startDate;

		while ($date < mktime(0, 0, 0, $today['mon'] + 1, 0, $today['year'])) {
			$weekDates = array();

			for ($i = 0; $i < 7; $i++) {
				$weekDates[] = $date;
				$date       += 86400;
			}

			$CalendarMonth['monthDates'][] = $weekDates;
		}

		$CalendarMonth['startDate']    = $startDate;
		$CalendarMonth['endDate']      = $endDate;
		$CalendarMonth['eventsPerDay'] = 4;

		$template->setVariable('CalendarMonth', $CalendarMonth);
		$template->javascript('modules/calendar/lib/javascript-logging/js/logging.js');
		$template->javascript('modules/calendar/lib/javascript-logging/js/class.js');
		$template->style('modules/calendar/lib/javascript-logging/css/logging.css');
		$template->javascript('modules/calendar/lib/DataRequestor.js');
		$template->javascript('modules/calendar/lib/domdrag/dom-drag.js');
		$template->javascript('modules/calendar/javascript/calendar.js');
		$template->javascript('modules/calendar/javascript/util.js');
		$template->style('modules/calendar/css/calendar.css');

		return array(
			null,
			array(
				'body' => 'modules/calendar/templates/CalendarMonth.tpl',
			),
		);
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	public function getViewDescription() {
		list($ret, $core) = GalleryCoreApi::loadPlugin('module', 'core');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $core->translate('add items'));
	}
}
