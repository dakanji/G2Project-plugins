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
GalleryCoreApi::requireOnce('modules/transcode/classes/TranscodeHelper.class');

/**
 * This ItemAddOption transcodes videos to flash video format
 * @package Transcode
 * @subpackage UserInterface
 * @author Don Willingham <donwillingham@users.sf.net>
 * @version $Revision:$
 */
class TranscodeOnUploadOption extends ItemAddOption {
	/**
	 * @see ItemAddOption::isAppropriate
	 */
	public function isAppropriate() {
		list($ret, $onUpload) = GalleryCoreApi::getPluginParameter('module', 'transcode', 'onUpload');

		if ($ret) {
			return array($ret, false);
		}

		return array(null, $onUpload > 0);
	}

	/**
	 * @see ItemAddOption::handleRequestAfterAdd
	 */
	public function handleRequestAfterAdd($form, $items) {
		$errors = $warnings = array();

		if (!empty($form['TranscodeOnUploadOption']['transcodeNever'])) {
			return array(null, $errors, $warnings);
		}

		list($ret, $onUpload) = GalleryCoreApi::getPluginParameter('module', 'transcode', 'onUpload');

		if ($ret) {
			return array($ret, null, null);
		}

		if ((!empty($form['TranscodeOnUploadOption']['transcodeOnUpload']) || !empty($form['TranscodeOnUploadOption']['transcodeLater'])) && $onUpload) {
			list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'transcode');

			if ($ret) {
				return array($ret, null, null, null);
			}

			if (count($params['selectedMimeTypes']) > 0) {
				$transcodeTypes = explode('|', $params['selectedMimeTypes']);
			} else {
				$transcodeTypes = explode('|', $params['movieTypes']);
			}

			// Copy the array because we will change it with do / while / array_splice
			$itemsInBatches = $items;

			/*
			 * Batch size should be <= ulimit max open files, as long as we don't query this value,
			 * assume a value of 100 which is fairly low
			 */
			$batchSize = 100;

			do {
				$currentItems = array_splice($itemsInBatches, 0, $batchSize);
				$itemIds      = array();

				foreach ($currentItems as $item) {
					$itemIds[] = $item->getId();
				}

				list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($itemIds);

				if ($ret) {
					return array($ret, null, null);
				}

				for ($i = 0; $i < count($currentItems); $i++) {
					if (in_array($currentItems[$i]->getMimeType(), $transcodeTypes)) {
						list($ret, $currentItems[$i]) = $currentItems[$i]->refresh();

						if ($ret) {
							return array($ret, null, null);
						}

						GalleryCoreApi::releaseLocks($lockId);

						$itemId = $currentItems[$i]->getId();

						// Create a new preferred since one doesn't exist
						list($ret, $source) = GalleryCoreApi::fetchPreferredSource($currentItems[$i]);

						if ($ret) {
							return array($ret, null, null, null);
						}

						// Make sure that we have a toolkit
						list($ret, $toolkit, $outputMimeType) = GalleryCoreApi::getToolkitByOperation($source->getMimeType(), 'convert-to-' . $params['outputMime']);

						if ($ret) {
							return array($ret, null, null, null);
						}

						if (isset($toolkit)) {
							list($ret, $derivative) = GalleryCoreApi::newFactoryInstanceByHint(
								'GalleryDerivative',
								$source->getEntityType()
							);

							if ($ret) {
								return array($ret, null, null, null);
							}

							if (!isset($derivative)) {
								return array(
									GalleryCoreApi::error(ERROR_MISSING_OBJECT),
									null,
									null,
									null,
								);
							}

							$ret = $derivative->create($currentItems[$i]->getId(), DERIVATIVE_TYPE_IMAGE_PREFERRED);

							if ($ret) {
								return array($ret, null, null, null);
							}

							if ($params['outputMime'] == 'video/x-flv') {
								$renderer = 'FlashVideoRenderer';
							} else {
								$renderer = 'TranscodeVideoRenderer';
							}

							// Set extra derivative operations
							list($ret, $nativeWidth, $nativeHeight, $nativeFps, $nativeVideoBitrate,
							$nativeAudioSampleRate, $nativeAudioChannels) = TranscodeHelper::_getProperties($item);

							if ($ret) {
								return array($ret, null, null);
							}

							$setVideoDimensions = '';

							if ($nativeWidth > $params['maxWidth']) {
								$aspect    = $nativeHeight / $nativeWidth;
								$maxWidth  = $params['maxWidth'];
								$maxHeight = round($maxWidth * $aspect);

								if (($maxWidth % 2) != 0) {
									$maxWidth = $maxWidth - 1;
								}

								if (($maxHeight % 2) != 0) {
									$maxHeight = $maxHeight - 1;
								}

								$setVideoDimensions = 'set-video-dimensions|' . $maxWidth . ',' .
								$maxHeight . ';';
							}

							$setVideoBitrate = '';

							if (isset($params['defaultBitrate']) && ($params['defaultBitrate'] != '')) {
								$setVideoBitrate = 'set-video-bitrate|' . $params['defaultBitrate'] . ';';
							}

							$setAudioChannels = '';

							if (isset($params['defaultAudioChannels']) && ($params['defaultAudioChannels'] != '')) {
								$setAudioChannels = 'set-audio-channels|' . $params['defaultAudioChannels'] . ';';
							}

							$setAudioSampleRate = '';

							if (isset($params['defaultAudioSampleRate']) && ($params['defaultAudioSampleRate'] != '')) {
								$setAudioSampleRate = 'set-audio-samplerate|' . $params['defaultAudioSampleRate'] . ';';
							}

							// End extra derivative operations
							$derivative->setMimeType($outputMime);
							$derivative->setDerivativeOperations(
								$setAudioSampleRate . $setAudioChannels .
								$setVideoBitrate . $setVideoDimensions . 'convert-to-' . $params['outputMime']
							);
							$derivative->setDerivativeSourceId($source->getId());

							$ret = $derivative->save();

							if ($ret) {
								return array($ret, null, null, null);
							}

							// Set FlashVideoRenderer, is there a way of knowing whether it's available?
							if (!$currentItems[$i]->getRenderer()) {
								list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($currentItems[$i]->getId());

								if ($ret) {
									return array($ret, null);
								}

								list($ret, $currentItems[$i]) = $currentItems[$i]->refresh();

								if ($ret) {
									GalleryCoreApi::releaseLocks($lockId);

									return array($ret, null);
								}

								$currentItems[$i]->setRenderer($renderer);

								$ret = $currentItems[$i]->save();

								if ($ret) {
									GalleryCoreApi::releaseLocks($lockId);

									return array($ret, null);
								}

								$ret = GalleryCoreApi::releaseLocks($lockId);

								if ($ret) {
									return array($ret, null);
								}
							}

							if (!empty($form['TranscodeOnUploadOption']['transcodeOnUpload'])) {
								list($ret) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($derivative->getId());

								if ($ret) {
									return array($ret, null, null);
								}
							}
						}
					}
				}

				$ret = GalleryCoreApi::releaseLocks($lockId);

				if ($ret) {
					return array($ret, null, null);
				}
			} while (!empty($itemsInBatches));
		} // End if(!empty($form['TranscodeOnUploadOption']['transcodeOnUpload']) && $params['onUpload'])

		return array(null, $errors, $warnings);
	}

	/**
	 * @see ItemAddOption::loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $item) {
		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'transcode');

		if ($ret) {
			return array($ret, null, null, null);
		}

		if (count($params['selectedMimeTypes']) > 0) {
			$selectedMimeTypes = explode('|', $params['selectedMimeTypes']);
		} else {
			$selectedMimeTypes = explode('|', $params['movieTypes']);
		}

		$template->setVariable('outputMime', $params['outputMime']);
		$template->setVariable('selectedMimeTypes', $selectedMimeTypes);
		$template->setVariable('onUpload', $params['onUpload']);

		return array(null, 'modules/transcode/templates/TranscodeOnUploadOption.tpl', 'modules_transcode');
	}
}
