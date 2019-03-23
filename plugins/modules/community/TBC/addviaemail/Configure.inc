<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
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
 * Configure AddviaEmail
 * @package AddviaEmail
 * @subpackage UserInterface
 * @author Brian Janice <cobalt027@users.sourceforge.net>
 * @version $Revision: 896 $
 */
class ConfigureController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		GalleryCoreApi::requireOnce('modules/addviaemail/classes/ImageUploadHelper.class');

		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$error = $status = array();

		if (isset($form['action']['save'])) {
			if (empty($form['serverAddress'])) {
				$error[] = 'form[error][serverAddress][missing]';
			}

			if (empty($form['port'])) {
				$error[] = 'form[error][port][missing]';
			}

			if (empty($form['userName'])) {
				$error[] = 'form[error][userName][missing]';
			}

			if (empty($form['passWord'])) {
				$error[] = 'form[error][passWord][missing]';
			}

			if (empty($form['subjectToMatch'])) {
				$error[] = 'form[error][subjectToMatch][missing]';
			}

			if (empty($form['tmpLocation'])) {
				$error[] = 'form[error][tmpLocation][missing]';
			}

			if (empty($form['owner'])) {
				$error[] = 'form[error][owner][missing]';
			} else {
				list($ret, $user) = GalleryCoreApi::fetchUserByUserName($form['owner']);

				if ($ret) {
					$error[] = 'form[error][owner][bad]';
				}
			}

			if (empty($error)) {
				if (empty($form['album'])) {
					// They dont want to create one, they chose from the list
					$albumId = $form['destinationAlbum'];
				} else {
					// They want to create a new one, and they've chose the parent
					list($ret, $albumId) = ImageUploadHelper::createUploadAlbum(
						$form['album'],
						$form['destinationAlbum'],
						$user->getId()
					);

					if ($ret) {
						return array($ret, null);
					}
				}
			}
		}

		if (isset($form['action']['save']) && empty($error)) {
			// If no error to this point, it is safe to save preferences
			if ($form['serverType'] == '1') {
				$form['deleteProcessed'] = '1';
			}

			foreach (array(
				'serverType',
				'serverAddress',
				'ssl',
				'port',
				'userName',
				'passWord',
				'subjectToMatch',
				'bodyAsCaption',
				'tmpLocation',
				'deleteProcessed',
				'pollFloor',
			) as $key) {
				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'addviaemail',
					$key,
					$form[$key]
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'addviaemail',
				'owner',
				$user->getId()
			);

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'addviaemail',
				'destinationAlbum',
				$albumId
			);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'addviaemail');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $isActive) = $module->isActive();

			if ($ret) {
				return array($ret, null);
			}

			if ($isActive) {
				$redirectSubview = 'addviaemail.Configure';
				$status['saved'] = 1;
			} else {
				$redirectSubview      = 'core.AdminPlugins';
				$status['configured'] = 'addviaemail';
			}
		} elseif (isset($form['action']['cancel'])) {
			$redirectSubview = 'core.AdminPlugins';
		} elseif (isset($form['action']['reset'])) {
			$redirectSubview = 'addviaemail.Configure';
		}

		if (!empty($redirectSubview)) {
			$results['redirect'] = array(
				'view'    => 'core.SiteAdmin',
				'subView' => $redirectSubview,
			);
		} else {
			$results['delegate'] = array(
				'view'    => 'core.SiteAdmin',
				'subView' => 'addviaemail.Configure',
			);
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * Configure AddviaEmail
 */
class ConfigureView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		// Load our default values if we didn't just come from this form.
		if ($form['formName'] != 'AddviaEmailConfigure') {
			$form['formName'] = 'AddviaEmailConfigure';
			$form['album']    = '';

			// Set defaults from the existing Parameters, when we're editing
			list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'addviaemail');

			if ($ret) {
				return array($ret, null);
			}

			foreach (array(
				'serverAddress',
				'serverType',
				'ssl',
				'port',
				'userName',
				'subjectToMatch',
				'bodyAsCaption',
				'tmpLocation',
				'owner',
				'deleteProcessed',
				'destinationAlbum',
				'pollFloor',
			) as $key) {
				if (!empty($params[$key])) {
					$form[$key] = $params[$key];

					if ($key == 'owner') {
						// Convert the UserID back into a Username for display
						list($ret, $user) = GalleryCoreApi::loadEntitiesById($params['owner']);

						if ($ret) {
							return array($ret, null);
						}

						$form['owner'] = $user->getUserName();
					}
				} else {
					$form[$key] = '';

					if ($key == 'tmpLocation') {
						// handle first time config - to preload the tmpDir variable
						$form['tmpLocation'] = $gallery->getConfig('data.gallery.tmp');
					}

					if ($key == 'pollFloor') {
						// handle first time config - to preload the tmpDir variable
						$form['pollFloor'] = 5;
					}
				}
			}
		}

		$form['passWord'] = '';
		$configure        = array();

		// Check for required email functions
		GalleryCoreApi::requireOnce('modules/addviaemail/classes/AddviaemailAdminHelper.class');

		$funcsReport = AddviaemailAdminHelper::testRequiredFunctions();

		if (count($funcsReport['fail']) > 0) {
			$configure['funcsReport'] = $funcsReport;
		}

		// Gets the album information from the gallery
		list($ret, $albumIds) = GalleryCoreApi::fetchAllItemIds('GalleryAlbumItem', 'core.addAlbumItem');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $albums) = GalleryCoreApi::loadEntitiesById($albumIds);

		if ($ret) {
			return array($ret, null);
		}

		$configure['albumTree'] = GalleryUtilities::createAlbumTree($albums);

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'addviaemail');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $isActive) = $module->isActive();

		if ($ret) {
			return array($ret, null);
		}

		$configure['isConfigure'] = !$isActive;

		$template->setVariable('AddviaEmailConfigure', $configure);
		$template->setVariable('controller', 'addviaemail.Configure');

		return array(
			null,
			array(
				'body' => 'modules/addviaemail/templates/Configure.tpl',
			),
		);
	}
}
