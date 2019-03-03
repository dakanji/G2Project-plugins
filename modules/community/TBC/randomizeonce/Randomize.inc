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
 * @package CustomDerivatives
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1529 $
 */
class RandomizeController extends GalleryController {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function handleRequest($form) {
		global $gallery;

		$error    = array();
		$status   = array();
		$redirect = array();

		// collect album id
		$itemId = GalleryUtilities::getRequestVariables('itemId');

		// Check permissions
		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.edit');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId, 'GalleryItem');

		if ($ret) {
			return array($ret, null, null, null);
		}

		list($ret, $childIds) = GalleryCoreApi::fetchChildItemIds($item);

		if ($ret) {
			return array($ret, null);
		}

		foreach ($childIds as $childId) {
			$ret = GalleryCoreApi::setItemOrderWeight($childId, mt_rand(1, 99999));
		}

		// Delegate back to the same page so that we can do more randomizing!
		$redirect['view']     = 'core.ItemAdmin';
		$redirect['subView']  = 'randomizeonce.Randomize';
		$redirect['itemId']   = $itemId;
		$status['randomized'] = 1;

		$results['redirect'] = $redirect;
		$results['status']   = $status;
		$results['error']    = $error;

		return array(null, $results);
	}
}

class RandomizeView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		// collect album id
		$itemId = GalleryUtilities::getRequestVariables('itemId');
		// Check permissions
		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.edit');

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'Randomize') {
			$form['formName'] = 'Randomize';
		}

		$template->setVariable('controller', 'randomizeonce.Randomize');

		return array(
			null,
			array(
				'body' => 'modules/randomizeonce/templates/Randomize.tpl',
			),
		);
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	public function getViewDescription() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'randomizeonce');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('randomize display order'));
	}
}
