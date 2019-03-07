<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2009 Bharat Mediratta
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
 * Settings for Tag Tree
 * @package TagTree
 * @subpackage UserInterface
 * @author Chad Parry <spam@chad.parry.org>
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 1720 $
 */
class TagTreeSiteAdminController extends GalleryController {
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
			foreach (array(
				'showItems',
				'usePersistence',
				'treeExpandCollapse',
			) as $key) {
				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'tagtree',
					$key,
					((isset($form[$key]) && $form[$key]) ? 1 : 0)
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			$status['saved'] = 1;

			// Clear our cache
			GalleryDataCache::removeFromDisk(
				array(
					'type'   => 'module-data',
					'module' => 'tagtree',
				)
			);
		}

		$results = array(
			'redirect' => array(
				'view'    => 'core.SiteAdmin',
				'subView' => 'tagtree.TagTreeSiteAdmin',
			),
			'status'   => $status,
			'error'    => $error,
		);

		return array(null, $results);
	}
}

/**
 * Settings for Tag Tree
 */
class TagTreeSiteAdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		foreach (array(
			'showItems',
			'usePersistence',
			'treeExpandCollapse',
		) as $key) {
			if (!isset($form[$key])) {
				$form[$key] = false;
			}
		}

		if ($form['formName'] != 'TagTreeSiteAdmin') {
			$form['formName'] = 'TagTreeSiteAdmin';

			list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'tagtree');

			if ($ret) {
				return array($ret, null);
			}

			foreach ($params as $key => $value) {
				$form[$key] = $value;
			}
		}

		$template->setVariable('controller', 'tagtree.TagTreeSiteAdmin');

		return array(
			null,
			array(
				'body' => 'modules/tagtree/templates/TagTreeSiteAdmin.tpl',
			),
		);
	}
}
