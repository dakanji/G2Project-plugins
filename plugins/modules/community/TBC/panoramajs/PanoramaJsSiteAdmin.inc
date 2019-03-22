<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
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
 * Settings for PanoramaJs
 * @package PanoramaJs
 * @subpackage UserInterface
 * @author shivaya <shivaya>at<users.sourceforge.net>
 * @version $Revision: 15513 $
 */
class PanoramaJsSiteAdminController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$status = $error = array();

		if (isset($form['action']['save'])) {
			if (!isset($form['width']) || ((int)$form['width']) < 100) {
				$error[] = 'form[error][width]';
			} else {
				foreach (array('itemType', 'itemLink', 'rotate') as $key) {
					$ret = GalleryCoreApi::setPluginParameter(
						'module',
						'panoramajs',
						$key,
						(isset($form[$key]) && $form[$key]) ? 1 : 0
					);

					if ($ret) {
						return array($ret, null);
					}
				}

				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'panoramajs',
					'width',
					(int)$form['width']
				);

				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'panoramajs',
					'speed',
					(int)$form['speed']
				);

				if ($ret) {
					return array($ret, null);
				}

				$status['saved'] = 1;
			}
		}

		// Figure out where to redirect upon success
		$method                      = empty($error) ? 'redirect' : 'delegate';
		$results[$method]['view']    = 'core.SiteAdmin';
		$results[$method]['subView'] = 'panoramajs.PanoramaJsSiteAdmin';
		$results['status']           = $status;
		$results['error']            = $error;

		return array(null, $results);
	}
}

/**
 * Settings for PanoramaJs
 */
class PanoramaJsSiteAdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		GalleryCoreApi::requireOnce('modules/panoramajs/classes/PanoramaJsHelper.class');

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		foreach (array('itemType', 'itemLink') as $key) {
			if (!isset($form[$key])) {
				$form[$key] = false;
			}
		}

		if ($form['formName'] != 'PanoramaJsSiteAdmin') {
			$form['formName'] = 'PanoramaJsSiteAdmin';

			foreach (array('itemType', 'itemLink', 'width', 'rotate', 'speed') as $key) {
				list($ret, $form[$key]) = GalleryCoreApi::getPluginParameter('module', 'panoramajs', $key);

				if ($ret) {
					return array($ret, null);
				}
			}
		}

		$template->setVariable('controller', 'panoramajs.PanoramaJsSiteAdmin');

		return array(
			null,
			array(
				'body' => 'modules/panoramajs/templates/PanoramaJsSiteAdmin.tpl',
			),
		);
	}
}
