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
GalleryCoreApi::requireOnce('modules/core/DownloadItem.inc');/**

 * Provides downloading of binary items as files
 * @version $Revision: 1654 $
 * @package DownloadLink
 * @author Bharat Mediratta <bharat@menalto.com>
 * @author Alec Myers <alec@alecmyers.com>
 */
class OfferDownloadView extends DownloadItemView {
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
		$phpVm->header('Content-Disposition: attachment; filename="' . $data['pseudoFileName'] . '"');

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
		$permission = 'downloadlink.original';

		switch ($derivativeType) {
			case DERIVATIVE_TYPE_IMAGE_THUMBNAIL:
				$permission = 'downloadlink.thumbnail';

				break;

			case DERIVATIVE_TYPE_IMAGE_RESIZE:
				$permission = 'downloadlink.resizes';

				break;

			// DERIVATIVE_TYPE_IMAGE_PREFERRED uses core.viewSource
		}

		$ret = GalleryCoreApi::assertHasItemPermission($itemIdForPermission, $permission);

		if ($ret) {
			// Avoid information disclosure
			if ($ret->getErrorCode() & ERROR_PERMISSION_DENIED) {
				if ($permission != 'downloadlink.original') {
					list($ret2, $hasPermission) = GalleryCoreApi::hasItemPermission($item->getId(), 'downloadlink.original');

					if ($ret2) {
						return $ret2;
					}
				}

				if ($permission == 'downloadlink.original' || empty($hasPermission)) {
					$ret->addErrorCode(ERROR_MISSING_OBJECT);

					return $ret;
				}
			}

			return $ret;
		}

		return null;
	}
}
