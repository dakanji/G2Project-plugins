<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2010 Bharat Mediratta
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
 * ItemAdd plugin to show Uploadify
 * @package Uploadify
 * @author Wayne Patterson <suprsidr@flashyourweb.com>
 */
class ItemAddUploadify extends ItemAddPlugin {
	/**
	 * @see ItemAdd:loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $item) {
		global $gallery;

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'uploadify');

		if ($ret) {
			return array($ret, null, null);
		}

		$urlGenerator = &$gallery->getUrlGenerator();

		$session = &$gallery->getSession();

		$session->save();

		if ($form['formName'] != 'ItemAddUploadify') {
			$form['formName'] = 'ItemAddUploadify';
		}

		if (!isset($form['set'])) {
			$form['set'] = array(
				'title'       => 1,
				'summary'     => 0,
				'description' => 0,
			);
		}

		$ItemAddUploadify['submitUrl'] = $urlGenerator->generateUrl(
			array(
				'controller' => 'uploadify.Uploadify',
				'itemId'     => GalleryUtilities::getRequestVariables('itemId'),
				'addPlugin'  => 'ItemAddUploadify',
				'authToken'  => $session->getAuthToken(),
			),
			array(
				'forceFullUrl'   => true,
				'forceSessionId' => true,
				'forceDirect'    => true,
				'htmlEntities'   => false,
			)
		);

		// this is needed for flash
		$ItemAddUploadify['submitUrl'] = urlencode($ItemAddUploadify['submitUrl']);

		$template->style('modules/uploadify/lib/uploadify.css');
		$template->style('modules/uploadify/lib/uploadify.jGrowl.css');
		$template->javascript('modules/uploadify/lib/jquery140.min.js');
		$template->javascript('modules/uploadify/lib/swfobject.js');
		$template->javascript('modules/uploadify/lib/jquery.uploadify.v2.1.0.min.js');
		$template->javascript('modules/uploadify/lib/jquery.jgrowl_minimized.js');
		$template->setVariable('ItemAddUploadify', $ItemAddUploadify);

		return array(
			null,
			'modules/uploadify/templates/ItemAddUploadify.tpl',
			'modules_uploadify',
		);
	}

	/**
	 * @see ItemAddPlugin::getTitle
	 */
	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'uploadify');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Uploadify'));
	}
}
