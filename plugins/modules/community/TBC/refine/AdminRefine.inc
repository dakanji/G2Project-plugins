<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
 * @package Refine
 * @subpackage UserInterface
 *
 * @author Dayo Akanji <http://dakanji.com>
 */
class AdminRefineController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$status = array();

		if (isset($form['action']['save'])) {
			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'refine');

			if ($ret) {
				return array($ret, null);
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'refine', 'refineMode', $form['refineMode']);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $isActive) = $module->isActive();

			if ($ret) {
				return array($ret, null);
			}

			$redirect['view'] = 'core.SiteAdmin';

			if ($isActive) {
				$redirect['subView'] = 'refine.AdminRefine';
				$status['saved']     = 1;
			} else {
				$redirect['subView']  = 'core.AdminModules';
				$status['configured'] = 'Refine HTML';
			}
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'core.AdminPlugins';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'refine.AdminRefine';
		}

		$results['status'] = $status;
		$results['error']  = array();

		return array(null, $results);
	}
}

/**
 * @package Refine
 * @subpackage UserInterface
 */
class AdminRefineView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'AdminRefine') {
			$form['formName'] = 'AdminRefine';

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'refine');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $form['refineMode']) = GalleryCoreApi::getPluginParameter('module', 'refine', 'refineMode');

			if ($ret) {
				return array($ret, null);
			}
		}

		$template->setVariable('controller', 'refine.AdminRefine');

		return array(
			null,
			array(
				'body' => 'modules/refine/templates/AdminRefine.tpl',
			),
		);
	}
}
