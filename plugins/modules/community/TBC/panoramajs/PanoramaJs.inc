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
 * Show image in javascript panorama viewer
 * @package PanoramaJs
 * @subpackage UserInterface
 * @author shivaya <shivaya>at<users.sourceforge.net>
 * @version $Revision: 15513 $
 */
class PanoramaJsView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		GalleryCoreApi::requireOnce('modules/panoramajs/classes/PanoramaJsHelper.class');

		list($ret, $item) = $this->getItem();

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $image) = PanoramaJsHelper::fetchViewableImages(array($item));

		if ($ret) {
			return array($ret, null);
		}

		if (empty($image)) {
			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
		}

		$image = $image[$item->getId()];

		list($ret, $renderer) = GalleryCoreApi::newFactoryInstance(
			'GalleryRenderer',
			'PanoramaJsRenderer'
		);

		if ($ret) {
			return array($ret, null);
		}

		if (!isset($renderer)) {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}

		$template->setVariable(
			'PanoramaJs',
			array(
				'item'       => (array)$item,
				'appletHtml' => $renderer->render('HTML', $image, $item, array()),
			)
		);

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'panoramajs');

		if ($ret) {
			return array($ret, null);
		}

		/*  to do, but i search...

		if (!empty($PanoramaJs.item.title)) {
		$template->title($module->translate(array('text' => 'PanoramaJs for %s', $PanoramaJs.item.title)));
		} else {
		$template->title($module->translate('PanoramaJs'));
		}

		*/
		$template->title($module->translate('PanoramaJs'));
		$template->javascript('modules/panoramajs/lib/env.js');
		$template->javascript('modules/panoramajs/lib/yajevLE.js');
		$template->style('modules/panoramajs/panoramaJs.css');

		return array(
			null,
			array(
				'body' => 'modules/panoramajs/templates/PanoramaJs.tpl',
			),
		);
	}
}
