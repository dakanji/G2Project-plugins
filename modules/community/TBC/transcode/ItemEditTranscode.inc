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
GalleryCoreApi::requireOnce('modules/transcode/classes/TranscodeHelper.class');

/**
 * This plugin handles selecting transcoding options for video
 * @package Transcode
 * @subpackage UserInterface
 * @author Don Willingham <donwillingham@users.sf.net>
 * @version $Revision:$
 */
class ItemEditTranscode extends ItemEditPlugin {
	/**
	 * @see ItemEditPlugin::handleRequest
	 */
	public function handleRequest($form, &$item, &$preferred) {
		$status = null;
		$error  = array();

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'transcode');

		if ($ret) {
			return array($ret, null, null, null);
		}

		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'transcode');

		if ($ret) {
			return array($ret, null, null, null);
		}

		if (count($params['selectedMimeTypes']) > 0) {
			$transcodeTypes = explode('|', $params['selectedMimeTypes']);
		} else {
			$transcodeTypes = explode('|', $params['movieTypes']);
		}

		if (isset($form['action']['save'])) {
			if (isset($form['enabled']) && $form['enabled']) {
				if (!in_array($item->getMimeType(), $transcodeTypes)) {
					$error[] = 'form[error][mime][invalid]';
				}

				if (preg_match('/audio/', $item->getMimeType())) {
					$setVideoFramerate = $setVideoDimensions = $setVideoBitrate = $setVideosameq = $setAudioBitrate = $setAudioChans = $setAudioRate = '';

					// Get actual values
					list($ret, $nativeWidth, $nativeHeight, $nativeFps, $nativeVbr, $nativeAsr, $nativeAChans, $nativeADataRate) = TranscodeHelper::_getProperties($item);

					if ($ret) {
						return array($ret, null, null);
					}

					$mute = 0;

					// Set audio property operation strings
					$setAudioChans = '';
					$setAudioRate  = '';

					// Ensure a valid number of channels
					if (($nativeAChans > 0) && isset($form['encode']['achans'])) {
						if (($form['encode']['achans'] < 0) || ($form['encode']['achans'] > 2)) {
							$error[] = 'form[error][achans][invalid]';
						} elseif ($form['encode']['achans'] != $nativeAChans) {
							$setAudioChans = 'set-audio-channels|' . $form['encode']['achans'] . ';';
							$mute          = ($form['encode']['achans'] == 0);
						}
					}

					// Ensure a valid audio sample rate
					if (!$mute && ($nativeAsr > 0) && isset($form['encode']['asr'])) {
						switch ($form['encode']['asr']) {
							case '11025':
							case '22050':
							case '44100':
								// Only set if different than native
								if ((strlen($setAudioChans) > 0) || ($form['encode']['asr'] != $nativeAsr)) {
									$setAudioRate = 'set-audio-samplerate|' . $form['encode']['asr'] . ';';
								}

								break;

							default:
								$error[] = 'form[error][asr][invalid]';
						}
					}

					$setAudioBitrate = '';

					if (!$mute && isset($form['encode']['audiobitrate']) && ($form['encode']['audiobitrate'] != '')) {
						$setAudioBitrate = 'set-audio-bitrate|' . $form['encode']['audiobitrate'] . ';';
					}

					$params['outputMime'] = isset($form['outputMime']) ? $form['outputMime'] : 'audio/mpeg';
				} else {
					// Get actual values
					list($ret, $nativeWidth, $nativeHeight, $nativeFps, $nativeVbr, $nativeAsr, $nativeAChans, $nativeADataRate) = TranscodeHelper::_getProperties($item);

					if ($ret) {
						return array($ret, null, null);
					}

					// Ensure width, height, and frame/second parameters are good
					if (($nativeWidth > 0) && ($nativeHeight > 0) && ($nativeFps)) {
						if (($form['encode']['width'] < 1) || ($form['encode']['width'] % 2)) {
							$error[] = 'form[error][width][invalid]';
						}

						if (($form['encode']['height'] < 1) || ($form['encode']['height'] % 2)) {
							$error[] = 'form[error][height][invalid]';
						}

						if ($form['encode']['fps'] <= 0.0) {
							$error[] = 'form[error][fps][invalid]';
						}
					}

					// Set resizing operation if different from native
					$setVideoDimensions = '';

					if (($form['encode']['width'] != $nativeWidth) || ($form['encode']['height'] != $nativeHeight)) {
						$setVideoDimensions = 'set-video-dimensions|' . $form['encode']['width'] . ',' . $form['encode']['height'] . ';';
					}

					$mute = 0;

					// Set audio property operation strings
					$setAudioChans = '';
					$setAudioRate  = '';

					// Ensure a valid number of channels
					if (($nativeAChans > 0) && isset($form['encode']['achans'])) {
						if (($form['encode']['achans'] < 0) || ($form['encode']['achans'] > 2)) {
							$error[] = 'form[error][achans][invalid]';
						} elseif ($form['encode']['achans'] != $nativeAChans) {
							$setAudioChans = 'set-audio-channels|' . $form['encode']['achans'] . ';';
							$mute          = ($form['encode']['achans'] == 0);
						}
					}

					// Ensure a valid audio sample rate
					if (!$mute && ($nativeAsr > 0) && isset($form['encode']['asr'])) {
						switch ($form['encode']['asr']) {
							case '11025':
							case '22050':
							case '44100':
								// Only set if different than native
								if ((strlen($setAudioChans) > 0) || ($form['encode']['asr'] != $nativeAsr)) {
									$setAudioRate = 'set-audio-samplerate|' . $form['encode']['asr'] . ';';
								}

								break;

							default:
								$error[] = 'form[error][asr][invalid]';
						}
					}

					$setAudioBitrate = '';

					if (!$mute && isset($form['encode']['audiobitrate']) && ($form['encode']['audiobitrate'] != '')) {
						$setAudioBitrate = 'set-audio-bitrate|' . $form['encode']['audiobitrate'] . ';';
					}

					$setVideoBitrate = '';

					if (isset($form['encode']['bitrate']) && ($form['encode']['bitrate'] != '')) {
						$setVideoBitrate = 'set-video-bitrate|' . $form['encode']['bitrate'] . ';';
					}

					$setVideoFramerate = '';

					if (isset($form['encode']['fps']) && ($form['encode']['fps'] != '')) {
						$setVideoFramerate = 'set-video-framerate|' . (float)$form['encode']['fps'] . ';';
					}

					$setVideosameq = '';

					if (isset($form['sameq']) && ($form['sameq'] != '')) {
						$setVideosameq = 'set-video-sameq;';
					}
				}

				// Only continue without error
				// Test to clear derivative
				if ($form['videoDerivativeId'] > 0) {
					$ret = GalleryCoreApi::deleteEntityById($form['videoDerivativeId'], 'GalleryDerivative');

					if ($ret) {
						return array($ret, null, null, null);
					}

					$reused = false;
				}

				// End Test to clear derivative
				if (empty($error)) {
					// Adjust existing preferred, if it already exists
					$itemId = $item->getId();

					list($ret, $preferreds) = GalleryCoreApi::fetchPreferredsByItemIds(array($itemId));

					if ($ret) {
						return array($ret, null, null, null);
					}

					$reused = false;

					if (!empty($preferreds[$itemId])) {
						foreach ($preferreds as $preferred) {
							$op = strstr($preferred->getDerivativeOperations(), '/convert-to-/');

							if (strlen($op) > 0) {
								list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($preferred->getId());

								if ($ret) {
									return array($ret, null, null, null);
								}

								$preferred->setDerivativeOperations($setVideoFramerate . $setVideoDimensions . $setVideoBitrate . $setVideosameq . $setAudioBitrate . $setAudioChans . $setAudioRate . 'convert-to-' . $params['outputMime']);
								$ret = $preferred->save();

								if ($ret) {
									return array($ret, null, null, null);
								}

								$ret = GalleryCoreApi::releaseLocks($lockId);

								if ($ret) {
									return array($ret, null, null, null);
								}

								$reused = true;

								// Id to build if not on demand
								$buildId = $preferred->getId();
							}
						}
					}

					if (!$reused) {
						// Create a new preferred since one doesn't exist
						list($ret, $source) = GalleryCoreApi::fetchPreferredSource($item);

						if ($ret) {
							return array($ret, null, null, null);
						}

						// Make sure that we have a toolkit
						list($ret, $toolkit, $outputMimeType) = GalleryCoreApi::getToolkitByOperation($source->getMimeType(), 'convert-to-' . $params['outputMime']);

						if ($ret) {
							return array($ret, null, null, null);
						}

						if (isset($toolkit)) {
							list($ret, $derivative) = GalleryCoreApi::newFactoryInstanceByHint('GalleryDerivative', $source->getEntityType());

							if ($ret) {
								return array($ret, null, null, null);
							}

							if (!isset($derivative)) {
								return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null, null, null);
							}

							$ret = $derivative->create($item->getId(), DERIVATIVE_TYPE_IMAGE_PREFERRED);

							if ($ret) {
								return array($ret, null, null, null);
							}

							if (!isset($params['outputMime'])) {
								$params['outputMime'] = 'video/x-flv';
							}

							$derivative->setMimeType($params['outputMime']);

							//if($derivative->getWidth() != $form['encode']['width'])$derivative->setWidth($form['encode']['width']);
							//if($derivative->getHeight() != $form['encode']['height'])$derivative->setHeight($form['encode']['height']);
							$derivative->setDerivativeOperations($setVideoFramerate . $setVideoDimensions . $setVideoBitrate . $setVideosameq . $setAudioBitrate . $setAudioChans . $setAudioRate . 'convert-to-' . $params['outputMime']);
							$derivative->setDerivativeSourceId($source->getId());
							$ret = $derivative->save();

							if ($ret) {
								return array($ret, null, null, null);
							}

							// Id to build if not on demand
							$buildId = $derivative->getId();

							// Set FlashVideoRenderer, is there a way of knowing whether it's available?
							$renderer = '';

							if ($params['outputMime'] == 'video/x-flv' || $params['outputMime'] == 'video/mp4' || $params['outputMime'] == 'video/webm' || $params['outputMime'] == 'video/ogg') {
								$renderer = 'FlashVideoRenderer';
							} elseif (preg_match('/audio/', $params['outputMime'])) {
								$renderer = 'MP3AudioRenderer';
							} else {
								$renderer = 'TranscodeVideoRenderer';
							}

							if (!$item->getRenderer()) {
								list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($item->getId());

								if ($ret) {
									return array($ret, null);
								}

								list($ret, $item) = $item->refresh();

								if ($ret) {
									GalleryCoreApi::releaseLocks($lockId);

									return array($ret, null);
								}

								$item->setRenderer($renderer);
								$ret = $item->save();

								if ($ret) {
									GalleryCoreApi::releaseLocks($lockId);

									return array($ret, null);
								}

								$ret = GalleryCoreApi::releaseLocks($lockId);

								if ($ret) {
									return array($ret, null);
								}
							}
						}
					} // End if not reused
					// Transcode now?
					if (!isset($form['onDemand'])) {
						list($ret) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($buildId, true);

						if ($ret) {
							return array($ret, null, null);
						}
					}
				} // End if no errors
			} else {
				// Disabled, remove preferred
				$itemId = $item->getId();

				list($ret, $preferreds) = GalleryCoreApi::fetchPreferredsByItemIds(array($itemId));

				if ($ret) {
					return array($ret, null, null, null);
				}

				$deletedDeriv = false;

				if (isset($preferreds[$itemId])) {
					foreach ($preferreds as $preferred) {
						$convert = strstr($preferred->getDerivativeOperations(), 'convert-to-');

						if (strlen($convert)) {
							$ret = GalleryCoreApi::deleteEntityById($preferred->getId(), 'GalleryDerivative');

							if ($ret) {
								return array($ret, null, null, null);
							}

							$deletedDeriv = true;
						}
					}
				}

				if ($deletedDeriv) {
					// Clear FlashVideoRenderer
					if ($item->getRenderer()) {
						list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($item->getId());

						if ($ret) {
							return array($ret, null);
						}

						list($ret, $item) = $item->refresh();

						if ($ret) {
							GalleryCoreApi::releaseLocks($lockId);

							return array($ret, null);
						}

						$item->setRenderer(null);
						$ret = $item->save();

						if ($ret) {
							GalleryCoreApi::releaseLocks($lockId);

							return array($ret, null);
						}

						$ret = GalleryCoreApi::releaseLocks($lockId);

						if ($ret) {
							return array($ret, null);
						}
					}
				}
			}

			$status = $module->translate('Changes saved successfully');
		}

		return array(null, $error, $status, false);
	}

	/**
	 * @see ItemEditPlugin::loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $item, $thumbnail) {
		if (GalleryUtilities::isA($item, 'GalleryUnknownItem') && preg_match('/audio/', $item->getMimeType())) {
			// Get actual values
			list($ret, $nativeWidth, $nativeHeight, $nativeFps, $nativeVbr, $nativeAsr, $nativeAChans, $nativeADataRate) = TranscodeHelper::_getProperties($item);

			if ($ret) {
				return array($ret, null, null);
			}

			if ($nativeAChans > 0) {
				$form['native']['achans'] = $form['encode']['achans'] = $nativeAChans;
				$form['native']['asr']    = $form['encode']['asr']    = $nativeAsr;
				$form['native']['abr']    = $form['encode']['abr']    = $nativeADataRate;

				// Warn about downmixing
				if ($nativeAChans > 2) {
					$form['warn']['downmix'] = true;
				}

				// Check for set-audio operations
				list($ret, $toolkit, $outputMimeType) = GalleryCoreApi::getToolkitByOperation($item->getMimeType(), 'set-audio-channels');

				if ($ret || !isset($toolkit)) {
					$form['warn']['achans'] = true;
				}

				list($ret, $toolkit, $outputMimeType) = GalleryCoreApi::getToolkitByOperation($item->getMimeType(), 'set-audio-samplerate');

				if ($ret || !isset($toolkit)) {
					$form['warn']['asr'] = true;
				}
			}

			$derivativeSize            = 0;
			$form['videoDerivativeId'] = '';

			$itemId = $item->getId();

			list($ret, $preferreds) = GalleryCoreApi::fetchPreferredsByItemIds(array($itemId));

			if ($ret) {
				return array($ret, null, null, null);
			}

			if (isset($preferreds[$itemId])) {
				foreach ($preferreds as $preferred) {
					$operations = split(';', $preferred->getDerivativeOperations());

					foreach ($operations as $operation) {
						if (preg_match('/set\-audio\-chans\|(\d+);?/', $operation, $regs)) {
							$form['encode']['achans'] = $regs[1];
						}

						if (preg_match('/set\-audio\-samplerate\|(\d+);?/', $operation, $regs)) {
							$form['encode']['asr'] = $regs[1];
						}

						if (preg_match('/set\-audio\-bitrate\|(\d+);?/', $operation, $regs)) {
							$form['encode']['audiobitrate'] = $regs[1];
						}

						if (preg_match('/convert\-to\-audio/', $operation, $regs)) {
							/** @todo: Support different formats */
							$format          = $regs[1];
							$form['enabled'] = true;

							$form['videoDerivativeId'] = $preferred->getId();
						}

						$derivativeSize          = $preferred->getDerivativeSize();
						$form['encoded']['mime'] = $preferred->getMimeType();
					}
				}
			}

			// Get original and encoded filesizes
			$form['native']['mime'] = $item->getMimeType();
			$form['native']['size'] = $item->getSize();

			$form['encoded']['size'] = $derivativeSize;

			// Get output options
			list($ret, $path) = GalleryCoreApi::getPluginParameter('module', 'ffmpeg', 'path');
			GalleryCoreApi::requireOnce('modules/ffmpeg/classes/FfmpegToolkitHelper.class');

			list($ret, $tests, $mimeTypes, $supportsOffset,
					  $mimeTypesEncoder, $encoderCodecs, $version) = FfmpegToolkitHelper::testBinary($path);

			/** Its time to trim the fat and lose the non-web friendly mime types */
			$acceptableEncoders = array('audio/mpeg', 'audio/ogg');
			$outputOptions      = array();

			foreach ($mimeTypesEncoder as $option) {
				if (in_array($option, $acceptableEncoders)) {
					$outputOptions[] = $option;
				}
			}

			$template->setVariable('outputOptions', $outputOptions);
			$template->setVariable('controller', 'transcode.ItemEditTranscode');

			return array(null, 'modules/transcode/templates/AudioTranscode.tpl', 'modules_transcode');
		}

		// Get actual values
		list($ret, $nativeWidth, $nativeHeight, $nativeFps, $nativeVbr, $nativeAsr, $nativeAChans, $nativeADataRate) = TranscodeHelper::_getProperties($item);

		if ($ret) {
			return array($ret, null, null);
		}

		$aspect = $nativeWidth / $nativeHeight;

		if (($nativeWidth % 2) != 0) {
			$nativeWidth = $nativeWidth - 1;
		}

		if (($nativeHeight % 2) != 0) {
			$nativeHeight = $nativeHeight - 1;
		}

		$template->setVariable('aspect', $aspect);

		$derivativeSize = 0;

		if (($nativeWidth > 0) && ($nativeHeight > 0) && ($nativeFps > 0.0) && ($nativeVbr > 0)) {
			$form['hasVideo']          = true;
			$form['native']['width']   = $form['encode']['width']   = $nativeWidth;
			$form['native']['height']  = $form['encode']['height']  = $nativeHeight;
			$form['native']['fps']     = $form['encode']['fps']     = sprintf('%.2f', $nativeFps);
			$form['native']['bitrate'] = $form['encode']['bitrate'] = $nativeVbr;
		}

		if ($nativeAChans > 0) {
			$form['hasAudio']         = true;
			$form['native']['achans'] = $form['encode']['achans'] = $nativeAChans;
			$form['native']['asr']    = $form['encode']['asr']    = $nativeAsr;

			// Warn about downmixing
			if ($nativeAChans > 2) {
				$form['warn']['downmix'] = true;
			}

			// Check for set-audio operations
			list($ret, $toolkit, $outputMimeType) = GalleryCoreApi::getToolkitByOperation($item->getMimeType(), 'set-audio-channels');

			if ($ret || !isset($toolkit)) {
				$form['warn']['achans'] = true;
			}

			list($ret, $toolkit, $outputMimeType) = GalleryCoreApi::getToolkitByOperation($item->getMimeType(), 'set-audio-samplerate');

			if ($ret || !isset($toolkit)) {
				$form['warn']['asr'] = true;
			}
		}

		if ($form['formName'] != 'ItemEditTranscode') {
			$form['formName'] = 'ItemEditTranscode';

			// Assume transcoding is disabled
			$form['enabled']           = false;
			$form['videoDerivativeId'] = '';

			$itemId = $item->getId();

			list($ret, $preferreds) = GalleryCoreApi::fetchPreferredsByItemIds(array($itemId));

			if ($ret) {
				return array($ret, null, null, null);
			}

			if (isset($preferreds[$itemId])) {
				foreach ($preferreds as $preferred) {
					$operations = split(';', $preferred->getDerivativeOperations());

					foreach ($operations as $operation) {
						if (preg_match('/set\-audio\-chans\|(\d+);?/', $operation, $regs)) {
							$form['encode']['achans'] = $regs[1];
						}

						if (preg_match('/set\-video\-dimensions\|(\d+),(\d+);?/', $operation, $regs)) {
							$form['encode']['width']  = $regs[1];
							$form['encode']['height'] = $regs[2];
						}

						if (preg_match('/set\-video\-framerate\|(\d+);?/', $operation, $regs)) {
							$form['encode']['fps'] = sprintf('%.2f', $regs[1]);
						}

						if (preg_match('/set\-audio\-samplerate\|(\d+);?/', $operation, $regs)) {
							$form['encode']['asr'] = $regs[1];
						}

						if (preg_match('/set\-audio\-bitrate\|(\d+);?/', $operation, $regs)) {
							$form['encode']['audiobitrate'] = $regs[1];
						}

						if (preg_match('/set\-video\-bitrate\|(\d+);?/', $operation, $regs)) {
							$form['encode']['bitrate'] = $regs[1];
						}

						if (preg_match('/convert\-to\-video/', $operation, $regs)) {
							/** @todo: Support different formats */
							$format          = $regs[1];
							$form['enabled'] = true;

							$form['videoDerivativeId'] = $preferred->getId();
						}

						$derivativeSize = $preferred->getDerivativeSize();
					}
				}
			}
		}

		if (isset($form['encode']['achans'])) {
			// Set audio channels to closest valid value
			if ($form['encode']['achans'] < 0) {
				$form['encode']['achans'] = 0;
			} elseif ($form['encode']['achans'] > 2) {
				$form['encode']['achans'] = 2;

				// Also warn about downmixing
				$form['warn']['downmix'] = true;
			}
		}

		if (isset($form['encode']['asr'])) {
			// Set sample rate to closest valid value
			if ($form['encode']['asr'] > 33075) {
				$form['encode']['asr'] = 44100;
			} elseif ($form['encode']['asr'] > 16036.5) {
				$form['encode']['asr'] = 22050;
			} else {
				$form['encode']['asr'] = 11025;
			}
		}

		// Get original and encoded filesizes
		$form['native']['size']  = $item->getSize();
		$form['encoded']['size'] = $derivativeSize;

		// Only default onDemand when enabled, and derivative not already created
		if ($form['enabled'] && ($derivativeSize == 0)) {
			$form['onDemand'] = true;
		}

		// Report defaults
		list($ret, $defaults) = GalleryCoreApi::fetchAllPluginParameters('module', 'transcode');

		if ($ret) {
			return array($ret, null, null, null);
		}

		$template->setVariable('maxWidth', $defaults['maxWidth']);
		$template->setVariable('widthWarningDisplay', 'none');

		if ($form['encode']['width'] > $defaults['maxWidth']) {
			$form['encode']['width']  = $defaults['maxWidth'];
			$form['encode']['height'] = round($form['encode']['width'] / $aspect);

			$template->setVariable('widthWarningDisplay', 'block');
		}

		$template->setVariable('maxHeight', $defaults['maxHeight']);
		$template->setVariable('heightWarningDisplay', 'none');

		if ($form['encode']['height'] > $defaults['maxHeight']) {
			$form['encode']['height'] = $defaults['maxHeight'];
			$form['encode']['width']  = round($form['encode']['height'] * $aspect);

			$template->setVariable('heightWarningDisplay', 'block');
		}

		$template->setVariable('outputMime', $defaults['outputMime']);
		$template->setVariable('defaultBitrate', $defaults['defaultBitrate']);
		$template->setVariable('defaultAudioChannels', $defaults['defaultAudioChannels']);
		$template->setVariable('defaultAudioSampleRate', $defaults['defaultAudioSampleRate']);
		$template->setVariable('controller', 'transcode.ItemEditTranscode');

		return array(null, 'modules/transcode/templates/Transcode.tpl', 'modules_transcode');
	}

	/**
	 * @see ItemEditPlugin::isSupported
	 */
	public function isSupported($item, $thumbnail) {
		if (GalleryUtilities::isA($item, 'GalleryMovieItem') || (GalleryUtilities::isA($item, 'GalleryUnknownItem') && preg_match('/audio/', $item->getMimeType()))) {
			list($ret, $mimeTypes) = GalleryCoreApi::getPluginParameter('module', 'transcode', 'movieTypes');

			//$this->transcodeLog('types: '.print_r(explode('|', $mimeTypes), true));
			if (!$ret) {
				$list = explode('|', $mimeTypes);

				return in_array($item->getMimeType(), $list);
			}
		}

		return false;
	}

	/**
	 * @see ItemEditPlugin::getTitle
	 */
	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'transcode');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Transcode'));
	}

	public function transcodeLog($logentry) {
		global $gallery;

		if (is_array($logentry)) {
			$logentry = implode("\n", $logentry);
		}

		$logentry  = str_replace(array('<?', '?>'), array('(@', '@)'), $logentry);
		$timestamp = strftime('%c');
		fwrite(fopen($gallery->getConfig('data.gallery.base') . 'plugins_data/modules/ffmpeg/ffmpeg.log', 'a'), $timestamp . ': ' . $logentry . "\n");
		chmod($gallery->getConfig('data.gallery.base') . 'plugins_data/modules/ffmpeg/ffmpeg.log', 0777);
	}
}
