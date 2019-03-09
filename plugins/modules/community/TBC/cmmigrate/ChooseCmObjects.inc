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
GalleryCoreApi::requireOnce('modules/cmmigrate/classes/CmDatabaseTools.class');

/**
 * @package CmMigrate
 * @subpackage UserInterface
 * @author David Angier <david@angier.co.uk>
 * @version $Revision: 847 $
 */
class ChooseCmObjectsController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		// Verify that active user is an admin
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$status = $error = array();

		// Make sure the user selected *something* to migrate
		if (isset($form['action']['import'])) {
			if (empty($form['sourceAlbums'])) {
				$error[] = 'form[error][nothingSelected]';
			}

			if (!empty($error)) {
				$results['delegate']['view']    = 'core.SiteAdmin';
				$results['delegate']['subView'] = 'cmmigrate.ChooseCmObjects';
			} else {
				$results['delegate']['view']    = 'core.SiteAdmin';
				$results['delegate']['subView'] = 'cmmigrate.ConfirmCmImport';
			}
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'cmmigrate.ConfirmCmImport';
		}

		$results['status'] = $status;
		$results['error']  = $error;
		return array(null, $results);
	}
}

/**
 * Choose objects view
 */
class ChooseCmObjectsView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$platform       =& $gallery->getPlatform();
		$slash          = $platform->getDirectorySeparator();
		$copperminePath = GalleryUtilities::getRequestVariables('copperminePath');
		if ($copperminePath == null) {
			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__), null);
		}

		if ($form['formName'] != 'ChooseCmObjects') {
			$form['formName']           = 'ChooseCmObjects';
			$form['destinationAlbumID'] = '';
			$form['generateThumbnails'] = 1;
		}

		list($ret, $cmAlbumTree) = $this->_createCmAlbumTree($copperminePath);

		if ($ret) {
			return array($ret, null);
		}

		// Get all possible destinations (where user has write permission)
		list($ret, $rootId) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.rootAlbum');

		if ($ret) {
			return array($ret, null);
		}

		// Get ids of all all albums where we can add new album items
		list($ret, $albumIds) = GalleryCoreApi::fetchAllItemIds('GalleryAlbumItem', 'core.addAlbumItem');

		if ($ret) {
			return array($ret, null);
		}

		// Load all the album entities
		list($ret, $albums) = GalleryCoreApi::loadEntitiesById($albumIds);

		if ($ret) {
			return array($ret, null);
		}

		$g2AlbumTree     = GalleryUtilities::createAlbumTree($albums);
		$ChooseCmObjects = array(
			'copperminePath' => $copperminePath,
			'cmAlbumTree'    => $cmAlbumTree,
			'g2AlbumTree'    => $g2AlbumTree,
		);

		$template->setVariable('ChooseCmObjects', $ChooseCmObjects);
		$template->setVariable('controller', 'cmmigrate.ChooseCmObjects');

		return array(
			null,
			array(
				'body' => 'modules/cmmigrate/templates/ChooseCmObjects.tpl',
			),
		);
	}

	/**
	 * Extract all Coppermine albums and turn it into a depth tree suitable for display
	 * in a hierarchical format.
	 */
	public function _createCmAlbumTree($copperminePath) {
		$cmdb = new CmDatabaseTools($copperminePath);

		$cmdb->openDatabase();
		return $cmdb->getAlbumsSorted();
	}
}
