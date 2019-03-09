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
 * Settings for Transcode
 * @package Transcode
 * @subpackage UserInterface
 * @author Don Willingham <donwillingham@users.sf.net>
 * @version $Revision:$
 */
class AdminTranscodeController extends GalleryController {
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
			if (isset($form['outputMime'])) {
				list($ret, $form['outputExtension']) = GalleryCoreApi::convertMimeToExtensions($form['outputMime']);

				$form['outputExtension'] = $form['outputExtension'][0];
			}

			foreach (array(
				'outputMime',
				'outputExtension',
				'maxWidth',
				'maxHeight',
				'defaultBitrate',
				'defaultAudioChannels',
				'defaultAudioSampleRate',
			) as $key) {
				$ret = GalleryCoreApi::setPluginParameter('module', 'transcode', $key, $form[$key]);

				if ($ret) {
					return array($ret, null);
				}
			}

			foreach (array('onUpload') as $key) {
				$ret = GalleryCoreApi::setPluginParameter(
					'module',
					'transcode',
					$key,
					isset($form[$key]) ? 1 : 0
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			list($ret, $mimeTypes) = GalleryCoreApi::getPluginParameter('module', 'transcode', 'movieTypes');

			$mimeTypes = explode('|', $mimeTypes);

			foreach ($mimeTypes as $key) {
				if (isset($form[selectedMimeTypes][$key])) {
					$selectedMimeTypes .= $key . '|';
				}
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'transcode', 'selectedMimeTypes', $selectedMimeTypes);

			if ($ret) {
				return array($ret, null);
			}

			$status['saved']     = 1;
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'transcode.AdminTranscode';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'transcode.AdminTranscode';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * Settings for transcode
 */
class AdminTranscodeView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'AdminTranscode') {
			$form['formName'] = 'AdminTranscode';

			foreach (array(
				'onUpload',
				'outputMime',
				'outputExtension',
				'maxWidth',
				'maxHeight',
				'defaultBitrate',
				'defaultAudioChannels',
				'defaultAudioSampleRate',
			) as $key) {
				list($ret, $form[$key]) = GalleryCoreApi::getPluginParameter('module', 'transcode', $key);

				if ($ret) {
					return array($ret, null);
				}
			}
		}

		list($ret, $path) = GalleryCoreApi::getPluginParameter('module', 'ffmpeg', 'path');
		GalleryCoreApi::requireOnce('modules/ffmpeg/classes/FfmpegToolkitHelper.class');

		list($ret, $tests, $mimeTypes, $supportsOffset,
			  $mimeTypesEncoder, $encoderCodecs, $version) = FfmpegToolkitHelper::testBinary($path);

		/** Its time to trim the fat and lose the non-web friendly mime types */
		$acceptableEncoders = array('video/x-flv', 'video/mp4', 'video/webm', 'video/ogg');
		$outputOptions      = array();

		foreach ($mimeTypesEncoder as $option) {
			if (in_array($option, $acceptableEncoders)) {
				$outputOptions[] = $option;
			}
		}

		$template->setVariable('outputOptions', $outputOptions);

		list($ret, $mimeTypes) = GalleryCoreApi::getPluginParameter('module', 'transcode', 'movieTypes');

		if (!$ret && empty($form['transcodableMime'])) {
			$template->setVariable('transcodableMime', explode('|', $mimeTypes));
		}

		list($ret, $selectedMimeTypes) = GalleryCoreApi::getPluginParameter('module', 'transcode', 'selectedMimeTypes');

		if (count($selectedMimeTypes) > 0) {
			$selectedMimeTypes = explode('|', $selectedMimeTypes);
		} else {
			$selectedMimeTypes = explode('|', $mimeTypes);
		}

		$template->setVariable('selectedMimeTypes', $selectedMimeTypes);

		//$template->setVariable('selectedMimeTypes', explode('|', $mimeTypes));
		//$template->setVariable('AdminTranscode', array());
		$template->setVariable('controller', 'transcode:AdminTranscode');

		return array(
			null,
			array(
				'body' => 'modules/transcode/templates/AdminTranscode.tpl',
			),
		);
	}
}
