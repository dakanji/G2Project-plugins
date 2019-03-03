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
 * Settings for Title Album
 * @package Title
 * @author Alec Myers <alec@alecmyers.com>
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 1420 $
 */
class TitleSiteAdminController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$status = $error = array();

		if (isset($form['action']['save']) && empty($error)) {
			list($ret, $themeSettingsId) = GalleryCoreApi::getPluginParameter('module', 'title', 'themeSettingsId');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $error, $status) = GalleryCoreApi::handleThemeSettingsRequest(
				$form['currentThemeId'],
				$themeSettingsId,
				$form
			);

			if ($ret) {
				return array($ret, null);
			}

			$status = empty($status) ? array() : array(
				'statusMessage' => $status,
			);
		}

		if (isset($form['action']['save']) && empty($error)) {
			// textual data
			foreach (array('themeId') as $key) {
				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'title',
					$key,
					$form[$key]
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			// yes/no data
			$status['saved'] = 1;
		}

		// windup
		$method  = empty($error) ? 'redirect' : 'delegate';
		$results = array(
			$method  => array(
				'view'    => 'core.SiteAdmin',
				'subView' => 'title.TitleSiteAdmin',
			),
			'status' => $status,
			'error'  => $error,
		);

		return array(null, $results);
	}
}

/**
 * Settings for Title Albums
 */
class TitleSiteAdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'TitleSiteAdmin') {
			foreach (array('themeId') as $key) {
				list($ret, $form[$key]) = GalleryCoreApi::getPluginParameter('module', 'title', $key);

				if ($ret) {
					return array($ret, null);
				}
			}
		} else {
			// Reset theme selection on validation error for theme settings
			$form['themeId'] = $form['currentThemeId'];
		}

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'title');

		if ($ret) {
			return array($ret, null);
		}

		// Set up theme selection list
		$themeList = array(
			'' => $module->translate('&laquo; default theme &raquo;'),
		);

		list($ret, $themeStatus) = GalleryCoreApi::fetchPluginStatus('theme');

		if ($ret) {
			return array($ret, null);
		}

		foreach ($themeStatus as $id => $status) {
			if (!empty($status['active'])) {
				list($ret, $theme) = GalleryCoreApi::loadPlugin('theme', $id);

				if ($ret) {
					return array($ret, null);
				}

				$themeList[$id] = $theme->translate($theme->getName());
			}
		}

		// Set up theme settings
		list($ret, $themeSettingsId) = GalleryCoreApi::getPluginParameter('module', 'title', 'themeSettingsId');

		if ($ret) {
			return array($ret, null);
		}

		$ret = GalleryCoreApi::loadThemeSettingsForm(
			$form['themeId'],
			$themeSettingsId,
			$template,
			$form
		);

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'TitleSiteAdmin') {
			$form['formName'] = 'TitleWithSiteAdmin';
		}

		$template->setVariable(
			'TitleSiteAdmin',
			array(
				'themeList' => $themeList,
			)
		);

		$template->setVariable('controller', 'title.TitleSiteAdmin');

		return array(
			null,
			array(
				'body' => 'modules/title/templates/TitleSiteAdmin.tpl',
			),
		);
	}
}
