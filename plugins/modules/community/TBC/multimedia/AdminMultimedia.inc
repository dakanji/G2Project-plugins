<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2012 Bharat Mediratta
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
 * Settings for Multimedia
 * @package Multimedia
 * @subpackage UserInterface
 * @author Wayne Patterson <suprsidr@flashyourweb.com>
 * @version $Revision: 17580 $
 */
class AdminMultimediaController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$error = $status = array();

		if (isset($form['action']['save'])) {
			foreach (array(
				'color1',
				'color2',
				'color4',
				'allowDownload',
				'share',
				'allowFullscreen',
				'autoStart',
			) as $key) {
				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'multimedia',
					$key,
					$form[$key]
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			$status['saved'] = 1;
		}

		return array(
			null,
			array(
				'redirect' => array(
					'view'    => 'core.SiteAdmin',
					'subView' => 'multimedia.AdminMultimedia',
				),
				'status'   => $status,
				'error'    => $error,
			),
		);
	}
}

/**
 * Settings for Multimedia
 */
class AdminMultimediaView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		// Load our default values if we did not just come from this form.
		if ($form['formName'] != 'AdminMultimedia') {
			$form['formName'] = 'AdminMultimedia';
		}

		foreach (array(
			'color1',
			'color2',
			'color4',
			'allowDownload',
			'share',
			'allowFullscreen',
			'autoStart',
		) as $key) {
			list($ret, $form[$key]) = GalleryCoreApi::getPluginParameter(
				'module',
				'multimedia',
				$key
			);

			if ($ret) {
				return array($ret, null);
			}
		}

		$template->setVariable('controller', 'multimedia.AdminMultimedia');

		return array(
			null,
			array(
				'body' => 'modules/multimedia/templates/AdminMultimedia.tpl',
			),
		);
	}
}
