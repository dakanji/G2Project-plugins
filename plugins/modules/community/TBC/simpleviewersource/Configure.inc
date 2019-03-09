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
 * @package SimpleviewerSource
 * @version $Revision: 1433 $
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * This view sets simpleviewersource parameters on the root album at config time
 */

 /**
  * Add a comment to an item
  * @package simpleviewersource
  * @version $Revision: 1433 $
  * @author Alec Myers <alec@alecmyers.com>
  */
class ConfigureView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$status = array();

		// find the root album
		list($ret, $rootId) = GalleryCoreApi::getDefaultAlbumId();

		if ($ret) {
			return array($ret, null);
		}

		GalleryCoreApi::requireOnce(
			'modules/simpleviewersource/classes/SimpleviewerParameterMapHelper.class'
		);

		$baseSettings = array(
			'enabled'          => array('1', 'private'),
			'offerSlideshow'   => array('1', 'private'),
			'maxItems'         => array('0', 'private'),
			'height'           => array('100%', 'private'),
			'width'            => array('100%', 'private'),
			'backgroundColor'  => array('#000000', 'private'),
			'preloaderColor'   => array('0xffffff', 'private'),
			'navPosition'      => array('left', 'fixed'),
			'thumbnailColumns' => array('3', 'fixed'),
			'thumbnailRows'    => array('3', 'fixed'),
		);

		foreach ($baseSettings as $paramName => $paramDetails) {
			$ret = SimpleviewerParameterMapHelper::createParam($rootId, $paramName, $paramDetails[1], $paramDetails[0]);

			if ($ret) {
				return array($ret, null);
			}
		}

		$ret = GalleryCoreApi::setPluginParameter(
			'module',
			'simpleviewersource',
			'configured',
			true
		);

		if ($ret) {
			return array($ret, null);
		}

		$status['configured'] = 1;
		$template->setVariable('status', $status);
		return array(
			null,
			array(
				'body' => 'modules/simpleviewersource/templates/Configure.tpl',
			),
		);
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	public function getViewDescription() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'simpleviewersource');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Basic configuration'));
	}
}
