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
 * This ItemEditOption will handle adding/removing the PanoramaJs renderer
 * @package PanoramaJs
 * @subpackage UserInterface
 * @author shivaya <shivaya>at<users.sourceforge.net>
 * @version $Revision: 15513 $
 */
class PanoramaJsOption extends ItemEditOption {
	/**
	 * @see ItemEditOption::isAppropriate
	 */
	public function isAppropriate($item, $thumbnail) {
		if (!GalleryUtilities::isA($item, 'GalleryPhotoItem')) {
			return array(null, false);
		}

		list($ret, $param) = GalleryCoreApi::fetchAllPluginParameters('module', 'panoramajs');

		if ($ret || !$param['itemType']) {
			return array(null, false);
		}

		list($ret, $preferred) = GalleryCoreApi::fetchPreferredsByItemIds(array($item->getId()));

		if ($ret) {
			return array(null, false);
		}

		if (empty($preferred)) {
			$width    = $item->getWidth();
			$mimeType = $item->getMimeType();
		} else {
			$width    = $preferred[$item->getId()]->getWidth();
			$mimeType = $preferred[$item->getId()]->getMimeType();
		}

		return array(
			null,
			$item->getRenderer() == 'PanoramaJsRenderer' ||
			($width > $param['width'] &&
			 ($mimeType == 'image/jpeg' || $mimeType == 'image/gif')),
		);
	}

	/**
	 * @see ItemEditOption::loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $item, $thumbnail) {
		list($ret, $preferred) = GalleryCoreApi::fetchPreferredsByItemIds(array($item->getId()));

		if ($ret) {
			return array($ret, null, null);
		}

		$form['PanoramaJsOption']['isPanoramaJs'] = $item->getRenderer() == 'PanoramaJsRenderer';

		return array(null, 'modules/panoramajs/templates/PanoramaJsOption.tpl', 'modules_panoramajs');
	}

	/**
	 * @see ItemEditOption::handleRequestAfterEdit
	 */
	public function handleRequestAfterEdit($form, &$item, &$preferred) {
		$isUsingPanoramaJsRenderer = $item->getRenderer() == 'PanoramaJsRenderer';

		if ($isUsingPanoramaJsRenderer && !isset($form['PanoramaJsOption']['isPanoramaJs'])) {
			$item->setRenderer(null);
		} elseif (!$isUsingPanoramaJsRenderer && isset($form['PanoramaJsOption']['isPanoramaJs'])) {
			$item->setRenderer('PanoramaJsRenderer');
		}

		if ($item->isModified()) {
			list($ret, $lockId) = GalleryCoreApi::acquireWriteLock(array($item->getId()));

			if ($ret) {
				return array($ret, null, null);
			}

			$ret = $item->save();

			if ($ret) {
				GalleryCoreApi::releaseLocks($lockId);

				return array($ret, null, null);
			}

			$ret = GalleryCoreApi::releaseLocks($lockId);

			if ($ret) {
				return array($ret, null, null);
			}
		}

		return array(null, array(), array());
	}
}
