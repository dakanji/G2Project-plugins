<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2012 Bharat Mediratta
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
 * Provides downloading of binary items
 * @package Multimedia
 * @subpackage UserInterface
 * @author Wayne Patterson <suprsidr@flashyourweb.com>
 */
class DownloadItemView extends GalleryView {
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
		return true;
	}

	/**
	 * @see GalleryView::shouldSaveSession
	 */
	public function shouldSaveSession() {
		return false;
	}

	/**
	 * @see GalleryView::autoCacheControl
	 */
	public function autoCacheControl() {
		return false;
	}

	/**
	 * @see GalleryView::renderImmediate
	 */
	public function renderImmediate($status, $error) {
		// Figure out which item we're talking about
		$itemId = (int)GalleryUtilities::getRequestVariables('itemId');

		if (empty($itemId)) {
			return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
		}

		// Load the item
		list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId, 'GalleryEntity');

		if ($ret) {
			return $ret;
		}

		if (!method_exists($item, 'fetchPath') || !method_exists($item, 'getMimeType')) {
			// Avoid information disclosure for bogus entities by acting as if it didn't exist
			return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
		}

		$derivativeType = null;

		if (GalleryUtilities::isA($item, 'GalleryDerivative')) {
			$derivativeType = $item->getDerivativeType();
		}

		$ret = $this->_assertPermissions($item, $derivativeType);

		if ($ret) {
			return $ret;
		}

		// Figure out the filename
		list($ret, $pseudoFileName) = GalleryUtilities::getPseudoFileName($item);

		if ($ret) {
			return $ret;
		}

		// Don't allow malicious URLs
		$fileName = GalleryUtilities::getRequestVariables('fileName');

		if (!empty($fileName) && $fileName != $pseudoFileName) {
			return GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__, 'malicious url');
		}

		// Get the path to the file
		list($ret, $path) = $item->fetchPath();

		if ($ret) {
			return $ret;
		}

		// Rebuild derivative cache, if necessary
		if (!empty($derivativeType)) {
			list($ret, $item) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($itemId);

			if ($ret) {
				return $ret;
			}
		}

		if (isset($_SERVER['HTTP_RANGE'])) {
			$ret = $this->_sendByteRange(
				array(
					'derivativePath' => $path,
					'mimeType'       => $item->getMimeType(),
					'pseudoFileName' => $pseudoFileName,
				)
			);

			if ($ret) {
				return $ret;
			}
		} else {
			$ret = $this->_sendFile(
				array(
					'derivativePath' => $path,
					'mimeType'       => $item->getMimeType(),
					'pseudoFileName' => $pseudoFileName,
				)
			);

			if ($ret) {
				return $ret;
			}
		}

		$ret = GalleryCoreApi::createFastDownloadFile($item);

		// Ignore failures since the file has already been sent
		return null;
	}

	public function _sendFile($data) {
		global $gallery;

		$platform =& $gallery->getPlatform();
		$phpVm    = $gallery->getPhpVm();

		/**
		 * Try to prevent Apache's mod_deflate from gzipping the output since it's likely already
		 * a binary file and broken versions of mod_deflate sometimes get the byte count wrong.
		 */
		if (function_exists('apache_setenv') && !@$gallery->getConfig('apacheSetenvBroken')) {
			@apache_setenv('no-gzip', '1');
		}

		$requestMethod = strtolower(GalleryUtilities::getServerVar('REQUEST_METHOD'));

		$phpVm->header('Content-type: ' . $data['mimeType']);
		$phpVm->header('Content-Disposition: inline; filename="' . $data['pseudoFileName'] . '"');

		$stats = $platform->stat($data['derivativePath']);
		$phpVm->header('Last-Modified: ' . GalleryUtilities::getHttpDate($stats[9]));
		$phpVm->header('Expires: ' . GalleryUtilities::getHttpDate(2147483647));

		/**
		 * If the file is publicly visible we could set this cache control to public (as we
		 * do for fast download) but that would require us to do some extra calculations here that
		 * would duplicate what we have in GalleryEntityHelper::createFastDownloadfile, so for now
		 * just set the cache control to private.
		 *
		 * @todo: refactor the GalleryEntityHelper code so that we can do the permission check
		 * here too and set Cache-Control to public when appropriate.
		 */
		$phpVm->header('Cache-Control: private');

		// If the request method is HEAD, don't send back the body
		if ($requestMethod == 'head') {
			$phpVm->header('Content-length: 0');
		} else {
			if ($stats[7] > 0) {
				$phpVm->header('Content-length: ' . $stats[7]);
			}

			/*
			 * Don't use readfile() because it buffers the entire file in memory.  Profiling shows
			 * that this approach is as efficient as fpassthru() but we get to call
			 * guaranteeTimeLimit which prevents it from failing on very large files
			 */
			if ($fd = $platform->fopen($data['derivativePath'], 'rb')) {
				while (true) {
					$bits = $platform->fread($fd, 65535);

					if (strlen($bits) == 0) {
						break;
					}

					echo $bits;

					$gallery->guaranteeTimeLimit(30);
				}

				$platform->fclose($fd);
			}
		}

		return null;
	}

	public function _sendByteRange($data) {
		global $gallery;

		$platform =& $gallery->getPlatform();
		$phpVm    = $gallery->getPhpVm();

		/**
		 * Try to prevent Apache's mod_deflate from gzipping the output since it's likely already
		 * a binary file and broken versions of mod_deflate sometimes get the byte count wrong.
		 */
		if (function_exists('apache_setenv') && !@$gallery->getConfig('apacheSetenvBroken')) {
			@apache_setenv('no-gzip', '1');
		}

		$phpVm->header('Content-type: ' . $data['mimeType']);
		$fp     = $platform->fopen($data['derivativePath'], 'rb');
		$size   = $platform->filesize($data['derivativePath']); // File size
		$length = $size;           // Content length
		$start  = 0;               // Start byte
		$end    = $size - 1;       // End byte
		// Now that we've gotten so far without errors we send the accept range header
		/* At the moment we only support single ranges.
		 * Multiple ranges requires some more work to ensure it works correctly
		 * and comply with the spesifications: http://www.w3.org/Protocols/rfc2616/rfc2616-sec19.html#sec19.2
		 *
		 * Multirange support annouces itself with:
		 * $phpVm->header('Accept-Ranges: bytes');
		 *
		 * Multirange content must be sent with multipart/byteranges mediatype,
		 * (mediatype = mimetype)
		 * as well as a boundry header to indicate the various chunks of data.
		 */
		$phpVm->header("Accept-Ranges: 0-$length");

		// $phpVm->header('Accept-Ranges: bytes');
		// multipart/byteranges
		// http://www.w3.org/Protocols/rfc2616/rfc2616-sec19.html#sec19.2
		if (isset($_SERVER['HTTP_RANGE'])) {
			$c_start = $start;
			$c_end   = $end;

			// Extract the range string
			list(, $range) = explode('=', GalleryUtilities::getServerVar('HTTP_RANGE'), 2);

			// Make sure the client hasn't sent us a multibyte range
			if (strpos($range, ',') !== false) {
				// (?) Shoud this be issued here, or should the first
				// range be used? Or should the header be ignored and
				// we output the whole content?
				$phpVm->header('HTTP/1.1 416 Requested Range Not Satisfiable');
				$phpVm->header("Content-Range: bytes $start-$end/$size");

				// (?) Echo some info to the client?
				return GalleryCoreApi::error(ERROR_BAD_REQUEST);
			}

			// If the range starts with an '-' we start from the beginning
			// If not, we forward the file pointer
			// And make sure to get the end byte if spesified
			if ($range0 == '-') {
				// The n-number of the last bytes is requested
				$c_start = $size - substr($range, 1);
			} else {
				$range   = explode('-', $range);
				$c_start = $range[0];
				$c_end   = (isset($range[1]) && is_numeric($range[1])) ? $range[1] : $size;
			}

			/* Check the range and make sure it's treated according to the specs.
			 * http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
			 */

			// End bytes can not be larger than $end.
			$c_end = ($c_end > $end) ? $end : $c_end;

			// Validate the requested range and return an error if it's not correct.
			if ($c_start > $c_end || $c_start > $size - 1 || $c_end >= $size) {
				$phpVm->header('HTTP/1.1 416 Requested Range Not Satisfiable');
				$phpVm->header("Content-Range: bytes $start-$end/$size");

				// (?) Echo some info to the client?
				return GalleryCoreApi::error(ERROR_BAD_REQUEST);
			}

			$start  = $c_start;
			$end    = $c_end;
			$length = $end - $start + 1; // Calculate new content length
			$platform->fseek($fp, $start);
			$phpVm->header('HTTP/1.1 206 Partial Content');
		}

		// Notify the client the byte range we'll be outputting
		$phpVm->header("Content-Range: bytes $start-$end/$size");
		$phpVm->header("Content-Length: $length");

		// Start buffered download
		$buffer = 1024 * 8;

		while (!$platform->feof($fp) && ($p = ftell($fp)) <= $end) {
			if ($p + $buffer > $end) {
				// In case we're only outputtin a chunk, make sure we don't
				// read past the length
				$buffer = $end - $p + 1;
			}

			$gallery->guaranteeTimeLimit(30); // Reset time limit for big files
			echo $platform->fread($fp, $buffer);
			flush(); // Free up memory. Otherwise large files will trigger PHP's memory limit.
		}

		$platform->fclose($fp);

		return null;
	}

	/**
	 * Assert the required permissions for the given item.
	 * @param GalleryChildEntity $item GalleryDataItem or GalleryChildEntity with a data item as
	 *        parent. Throws ERROR_MISSING_OBJECT if a non-item has no item as parent.
	 * @return GalleryStatus
	 */
	public function _assertPermissions($item, $derivativeType) {
		global $gallery;

		$session             =& $gallery->getSession();
		$itemIdForPermission = $item->getId();

		if (!empty($derivativeType)) {
			$itemIdForPermission = $item->getParentId();
		}

		// Make sure we have permission
		if (($ids = $session->get('core.isPrintService')) && in_array($item->getId(), $ids)) {
			// Print services only need core.view to get access to full size version of photos
			$permission = 'core.view';
		} else {
			$permission = 'core.viewSource';

			switch ($derivativeType) {
				case DERIVATIVE_TYPE_IMAGE_THUMBNAIL:
					$permission = 'core.view';

					break;

				case DERIVATIVE_TYPE_IMAGE_RESIZE:
					$permission = 'core.viewResizes';

					break;

				// DERIVATIVE_TYPE_IMAGE_PREFERRED uses core.viewSource
			}
		}

		$ret = GalleryCoreApi::assertHasItemPermission($itemIdForPermission, $permission);

		if ($ret) {
			// Avoid information disclosure
			if ($ret->getErrorCode() & ERROR_PERMISSION_DENIED) {
				if ($permission != 'core.view') {
					list($ret2, $hasPermission) = GalleryCoreApi::hasItemPermission($item->getId(), 'core.view');

					if ($ret2) {
						return $ret2;
					}
				}

				if ($permission == 'core.view' || empty($hasPermission)) {
					$ret->addErrorCode(ERROR_MISSING_OBJECT);

					return $ret;
				}
			}

			return $ret;
		}

		return null;
	}
}
