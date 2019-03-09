<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
 * This controller fields requests from Uploadify and performs any required changes
 * to the data model.
 * @package Uploadify
 * @author Wayne Patterson <suprsidr@flashyourweb.com>
 */
class UploadifyController extends GalleryController {
	/**
	 * ItemAddOption instances to use when handling this request.  Only used by test code.
	 * @var array (optionId => ItemAddOption) $_optionInstances
	 * @access private
	 */
	public $_optionInstances;

	/**
	 * Tests can use this method to hardwire a specific set of option instances to use.
	 * This avoids situations where some of the option instances will do unpredictable
	 * things and derail the tests.
	 *
	 * @param array $optionInstances (optionId => ItemAddOption, ...)
	 */
	public function setOptionInstances($optionInstances) {
		$this->_optionInstances = $optionInstances;
	}

	/**
	 * @see GalleryController::isAllowedInEmbedOnly
	 */
	public function isAllowedInEmbedOnly() {
		return false;
	}

	/**
	 * @see GalleryController::omitAuthTokenCheck
	 */
	public function omitAuthTokenCheck() {
		return false;
	}

	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$session =& $gallery->getSession();
		$ret     = GalleryController::assertIsGenuineRequest();

		if ($ret) {
			$gallery->debug('Uploadify User Agent: ' . GalleryUtilities::getServerVar('HTTP_USER_AGENT'));
			$user = $gallery->getActiveUser();

			if (isset($user)) {
				$gallery->debug('Uploadify User: ', $user->getuserName());
			} else {
				$gallery->debug('Uploadify User: ', 'error getting user');
			}

			return array($ret, null);
		}

		GalleryCoreApi::requireOnce('modules/core/ItemAdd.inc');

		$controller = new ItemAddController();

		list($ret, $item) = $controller->getItem();

		if ($ret) {
			return array($ret, null);
		}

		$itemId = $item->getId();

		// Make sure we have permission to add to this item
		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.addDataItem');

		if ($ret) {
			return array($ret, null);
		}

		if (!$item->getCanContainChildren()) {
			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
		}

		$file   = GalleryUtilities::getFile('userfile');
		$status = $error = $results = array();

		switch ($file['error']) {
			case 0:
				   $status['controllerResponse'] = 'Success'; // No Error
				break;

			case 1:
				   $results['error'] = 'The file is bigger than this PHP installation allows';

				break;

			case 2:
				  $results['error'] = 'The file is bigger than this form allows';

				break;

			case 3:
				 $results['error'] = 'Only part of the file was uploaded';

				break;

			case 4:
				 $results['error'] = 'No file was uploaded';

				break;

			case 6:
				 $results['error'] = 'Missing a temporary folder';

				break;

			case 7:
				 $results['error'] = 'Failed to write file to disk';

				break;

			case 8:
				 $results['error'] = 'File upload stopped by extension';

				break;

			default:
				 $results['error'] = 'Unknown error ' . $file['error'];

				break;
		}

		if (isset($results['error'])) {
			return array(null, $results);
		}

		$ret = $this->addDataItem($form, $file, $itemId);

		if ($ret) {
			$status['controllerError'] = $ret->getAsText();
		}

		$results['delegate']['view'] = 'uploadify.Uploadify';
		$results['status']           = $status;
		$results['error']            = $error;

		return array(null, $results);
	}

	/**
	 * Add a data item to Gallery
	 *
	 * @param array $form key/value pairs from Gallery Remote
	 * @return GalleryStatus a status code
	 */
	public function addDataItem($form, $file, $itemId) {
		global $gallery;

		list($ret, $lockIds[]) = GalleryCoreApi::acquireReadLock($itemId);

		if ($ret) {
			return $ret;
		}

		if (empty($file['name'])) {
			echo '0';
		}

		// Get the mime type.
		list($ret, $mimeType) = GalleryCoreApi::getMimeType($file['name'], $file['type']);

		if ($ret) {
			return GalleryCoreApi::error(ERROR_MISSING_FILENAME);
		}

		$filename = basename($file['name']);
		$base     = GalleryUtilities::getFileBase($filename);

		//$this->_log(print_r($_REQUEST,true));
		$title       = (isset($form['set']['title']) && $form['set']['title'] == 1) ? $base : '';
		$summary     = (isset($form['set']['summary']) && $form['set']['summary'] == 1) ? $base : '';
		$description = (isset($form['set']['description']) && $form['set']['description'] == 1) ? $base : '';

		/*
		 * Don't use uploaded files, because the framework cannot safely copy them.
		 * Move it to our temporary directory first.
		 */
		$platform =& $gallery->getPlatform();

		if ($platform->is_uploaded_file($file['tmp_name'])) {
			$tmpFile = $platform->move_uploaded_file($file['tmp_name']);

			if (!$tmpFile) {
				return GalleryCoreApi::error(ERROR_PLATFORM_FAILURE);
			}

			$needToDeleteTmpFile = true;
		} else {
			$tmpFile             = $file['tmp_name'];
			$needToDeleteTmpFile = false;
		}

		list($ret, $newItem) = GalleryCoreApi::addItemToAlbum(
			$tmpFile,
			basename($file['name']),
			$title,
			$summary,
			$description,
			$mimeType,
			$itemId
		);

		// Get rid of the tmp file if necessary
		if ($needToDeleteTmpFile) {
			@$platform->unlink($tmpFile);
		}

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::releaseLocks($lockIds);

		if ($ret) {
			return $ret;
		}

		if (isset($this->_optionInstances)) {
			$optionInstances = $this->_optionInstances;
		} else {
			GalleryCoreApi::requireOnce('modules/core/ItemAdd.inc');

			list($ret, $optionInstances) = ItemAddOption::getAllAddOptions();

			if ($ret) {
				return $ret;
			}
		}

		// Allow ItemAddOptions to process added item(s)
		foreach ($optionInstances as $option) {
			list($ret, $optionErrors, $optionWarnings) = $option->handleRequestAfterAdd(
				$form,
				array($newItem)
			);

			if ($ret) {
				return $ret;
			}

			/*
			 * Swallow option warnings and errors for now.
			 *
			 * TODO: Report warnings and errors back to Gallery Remote.  If the upload is denied
			 * because of quota limitations, then we'll get an error that we should report back.
			 */
		}

		// Check if buildResizes is checked and execute.
		if (isset($form['buildResizes']) && $form['buildResizes'] == 1) {
			list($ret) = $this->_runBuildResizes($newItem);

			if ($ret) {
				return $ret;
			}
		}

		return null;
	}

	public function _log($logentry) {
		global $gallery;

		if (is_array($logentry)) {
			$logentry = implode("\n", $logentry);
		}

		$logentry  = str_replace(array('<?', '?>'), array('(@', '@)'), $logentry);
		$timestamp = strftime('%c');
		fwrite(
			fopen($gallery->getConfig('data.gallery.base') . 'tmp/uploadify.log', 'a'),
			$timestamp . ': ' . $logentry . "\n"
		);

		chmod($gallery->getConfig('data.gallery.base') . 'tmp/uploadify.log', 0777);
	}

	public function _runBuildResizes($item) {
		global $gallery;

		if (!GalleryUtilities::isA($item, 'GalleryPhotoItem')) {
			return null;
		}

		$storage     =& $gallery->getStorage();
		$resizePrefs = array();
		$itemId      = $item->getId();

		// Load existing resizes -  there should not be any but...
		list($ret, $resizesSet) = GalleryCoreApi::fetchResizesByItemIds(array($itemId));

		if ($ret) {
			return $ret;
		}

		$resizesTable = array();

		foreach ($resizesSet as $itemId => $resizes) {
			foreach ($resizes as $resize) {
				$resizesTable[$itemId][$resize->getDerivativeOperations()] = $resize;
			}
		}

		$albumId = $item->getParentId();

		if (!isset($resizePrefs[$albumId])) {
			// Keep resizes for albums in memory
			list($ret, $preferences) = GalleryCoreApi::fetchDerivativePreferencesForItem($albumId);

			if ($ret) {
				return $ret;
			}

			$resizePrefs[$albumId] = array();

			foreach ($preferences as $preference) {
				if ($preference['derivativeType'] == DERIVATIVE_TYPE_IMAGE_RESIZE
					&& preg_match(
						'/(?:resize|scale)\|(\d+)(?:,(\d+))?/',
						$preference['derivativeOperations'],
						$matches
					)
				) {
					$width                   = $matches[1];
					$height                  = empty($matches[2]) ? $width : $matches[2];
					$resizePrefs[$albumId][] = array(
						'operations' => $preference['derivativeOperations'],
						'width'      => $width,
						'height'     => $height,
					);
				}
			}
		}

		list($ret, $source) = GalleryCoreApi::fetchPreferredSource($item);

		if ($ret) {
			return $ret;
		}

		$mimeType = $source->getMimeType();

		/*
		 * Determine operations and check against existing resizes again
		 * there should not be any.
		 */
		$newResizes = array();

		for ($i = 0; $i < count($resizePrefs[$albumId]); $i++) {
			if (!isset($resizePrefs[$albumId][$i][$mimeType])) {
				list(
					$ret,
					$resizePrefs[$albumId][$i][$mimeType]['operations'],
					$resizePrefs[$albumId][$i][$mimeType]['outputMimeType']
				) = GalleryCoreApi::makeSupportedViewableOperationSequence(
					$mimeType,
					$resizePrefs[$albumId][$i]['operations'],
					false
				);

				if ($ret) {
					return $ret;
				}
			}

			$resize = $resizePrefs[$albumId][$i];

			// Validate toolkit support before adding back the resizes
			if (empty($resize[$mimeType]['operations'])) {
				continue;
			}

			$operations = $resize[$mimeType]['operations'];

			// Special case to make sure that we don't upsample photos
			if (GalleryUtilities::isA($item, 'GalleryPhotoItem')
				&& $item->getWidth() <= $resize['width']
				&& $item->getHeight() <= $resize['height']
			) {
				continue;
			}

			if (isset($resizesTable[$itemId][$operations])) {
				// Keep existing resize, build it if requested
				list($ret) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent(
					$resizesTable[$itemId][$operations]->getId()
				);

				if ($ret) {
					return $ret;
				}

				unset($resizesTable[$itemId][$operations]);
			} else {
				// Create resize with these settings on next pass
				$newResizes[] = $resize;
			}
		}

		// Add new resizes, using existing derivative entities until we run out
		foreach ($newResizes as $resize) {
			if (!empty($resizesTable[$itemId])) {
				$derivative = array_shift($resizesTable[$itemId]);

				list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($derivative->getId());

				if ($ret) {
					return $ret;
				}
			} else {
				list($ret, $derivative) = GalleryCoreApi::newFactoryInstanceByHint(
					'GalleryDerivative',
					$source->getEntityType()
				);

				if ($ret) {
					return $ret;
				}

				if (!isset($derivative)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				$ret = $derivative->create($itemId, DERIVATIVE_TYPE_IMAGE_RESIZE);

				if ($ret) {
					return $ret;
				}
			}

			$derivative->setDerivativeSourceId($source->getId());
			$derivative->setDerivativeOperations($resize[$mimeType]['operations']);
			$derivative->setMimeType($resize[$mimeType]['outputMimeType']);
			$ret = GalleryCoreApi::estimateDerivativeDimensions($derivative, $source);

			if ($ret) {
				return $ret;
			}

			$ret = $derivative->save();

			if ($ret) {
				return $ret;
			}

			if (isset($lockId)) {
				$ret = GalleryCoreApi::releaseLocks($lockId);

				if ($ret) {
					return $ret;
				}

				$lockId = null;
			}

			// Build if requested
			list($ret) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent(
				$derivative->getId()
			);

			if ($ret) {
				return $ret;
			}
		}

		// Remove any leftover resizes
		if (isset($resizesTable[$itemId])) {
			foreach ($resizesTable[$itemId] as $resize) {
				$ret = GalleryCoreApi::deleteEntityById(
					$resize->getId(),
					'GalleryDerivative'
				);

				if ($ret) {
					return $ret;
				}
			}
		}

		$ret = $storage->checkPoint();

		if ($ret) {
			return $ret;
		}

		return null;
	}
}

/**
 * This is an immediate view that emits well formed Gallery Remote protocol 2 output
 */
class UploadifyView extends GalleryView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	/**
	 * @see GalleryView::isAllowedInEmbedOnly
	 */
	public function isAllowedInEmbedOnly() {
		return false;
	}

	/**
	 * @see GalleryView::renderImmediate
	 */
	public function renderImmediate($status, $error) {
		if (!headers_sent()) {
			header('Content-type: text/plain; charset=UTF-8');
		}

		if (isset($status['controllerError'])) {
			echo 'Error: ' . $status['controllerError'];
		}

		if (isset($status['controllerResponse'])) {
			echo $status['controllerResponse'];
		}

		if (isset($error)) {
			echo $error;
		} else {
			return null;
		}
	}
}
