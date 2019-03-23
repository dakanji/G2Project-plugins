<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
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
 * @version $Revision: 643 $ $Date: 2006-06-28 22:23:21 -0700 (Wed, 28 Jun 2006) $
 * @package Download
 * @subpackage UserInterface
 * @author Jack Bates <ms419@freezone.co.uk>
 */
GalleryCoreApi::requireOnce('modules/core/DownloadItem.inc');
GalleryCoreApi::requireOnce('modules/download/lib/HTTP/Download.php');/**

 * Support HTTP compression, partial downloads & resuming using PEAR HTTP_Download package
 *
 * This enables MP3 players like Winamp to skip back & forward without buffering an entire file -
 * http://www.oreillynet.com/pub/a/network/2005/01/07/primetime.html
 *
 * @package Download
 * @subpackage UserInterface
 */
class DownloadView extends DownloadItemView {
	public function _sendFile($data) {
		global $gallery;

		$session =& $gallery->getSession();

		// Print services bypass security so they can get access to full size version of prints
		if (!$session->get('core.isPrintService')) {
			// Make sure we have permission
			$permission = 'core.viewSource';

			switch ($data['derivativeType']) {
				case DERIVATIVE_TYPE_IMAGE_THUMBNAIL:
					$permission = 'core.view';

					break;

				case DERIVATIVE_TYPE_IMAGE_RESIZE:
					$permission = 'core.viewResizes';

					break;

				// DERIVATIVE_TYPE_IMAGE_PREFERRED uses core.viewSource
			}

			$ret = GalleryCoreApi::assertHasItemPermission($data['parentId'], $permission);

			if ($ret) {
				return $ret;
			}
		}

		HTTP_Download::staticSend(
			array(
				'file'               => $data['derivativePath'],
				'contenttype'        => $data['mimeType'],
				'contentdisposition' => "inline; filename='$data[pseudoFileName]'",
				'expires'            => GalleryUtilities::getHttpDate(time() + 31536000),
			)
		);

		return null;
	}
}
