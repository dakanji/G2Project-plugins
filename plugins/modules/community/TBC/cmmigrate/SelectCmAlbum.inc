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
GalleryCoreApi::requireOnce('modules/cmmigrate/classes/CmMigrateHelper.class');

/**
 * @package CmMigrate
 * @subpackage UserInterface
 * @author David Angier <david@angier.co.uk>
 * @version $Revision: 847 $
 */
class SelectCmAlbumController extends GalleryController {
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

		$error = $status = array();
		if (isset($form['action']['select'])) {
			if (empty($form['copperminePath'])) {
				$error[] = 'form[error][copperminePath][missing]';
			} else {
				$platform =& $gallery->getPlatform();
				$slash    = $platform->getDirectorySeparator();
				$tmp      = trim($form['copperminePath']);
				if ($tmp[strlen($tmp) - 1] != $slash) {
					$tmp .= $slash;
				}

				$form['copperminePath'] = trim($tmp);
				if (!CmMigrateHelper::isValidCopperminePath($form['copperminePath'], $slash)) {
					$error[] = 'form[error][copperminePath][invalid]';
				}
			}

			if (empty($error)) {
				// Save our albums path in the session
				$session     =& $gallery->getSession();
				$recentPaths = $session->get('cmmigrate.view.SelectGallery.recentPaths');
				if (empty($recentPaths)) {
					$recentPaths = array();
				}

				if (isset($recentPaths[$form['copperminePath']])) {
					$recentPaths[$form['copperminePath']]++;
				} else {
					$recentPaths[$form['copperminePath']] = 1;
				}

				$session->put('cmmigrate.view.SelectCmAlbum.recentPaths', $recentPaths);

				$redirect['view']           = 'core.SiteAdmin';
				$redirect['subView']        = 'cmmigrate.ChooseCmObjects';
				$redirect['copperminePath'] = $form['copperminePath'];
			}
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'cmmigrate.SelectCmAlbum';
		}

		$results['status'] = $status;
		$results['error']  = $error;
		return array(null, $results);
	}
}

/**
 * Select album view
 */
class SelectCmAlbumView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'SelectCmAlbum') {
			$form['formName']       = 'SelectCmAlbum';
			$form['copperminePath'] = '';
		}

		// Load our recent paths from the session
		$session     =& $gallery->getSession();
		$recentPaths = $session->get('cmmigrate.view.SelectCmAlbum.recentPaths');
		if (empty($recentPaths)) {
			$recentPaths = array();
		}

		list($ret, $operations) = GalleryCoreApi::getToolkitOperations('image/jpeg');

		if ($ret) {
			return array($ret, null);
		}

		$hasToolkit = false;
		for ($i = 0; $i < count($operations); $i++) {
			if ($operations[$i]['name'] == 'thumbnail') {
				$hasToolkit = true;
				break;
			}
		}

		$SelectCmAlbum                = array();
		$SelectCmAlbum['recentPaths'] = $recentPaths;
		$SelectCmAlbum['hasToolkit']  = $hasToolkit;

		$template->setVariable('controller', 'cmmigrate.SelectCmAlbum');
		$template->setVariable('SelectCmAlbum', $SelectCmAlbum);

		return array(
			null,
			array(
				'body' => 'modules/cmmigrate/templates/SelectCmAlbum.tpl',
			),
		);
	}
}
