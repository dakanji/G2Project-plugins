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
 * Generate Css required for the tag Cloud depending on selected options
 * @package Tags
 * @subpackage UserInterface
 * @author Eric Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1064 $
 */
class TagCloudCssView extends GalleryView {
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
	 * @see GalleryView::renderImmediate
	 */
	public function renderImmediate($status, $error) {
		global $gallery;

		$platform =& $gallery->getPlatform();

		// Get the file timestamp
		$slash                = $platform->getDirectorySeparator();
		$cssDir               = $gallery->getConfig('data.gallery.plugins_data') . 'modules' . $slash . 'tags';
		$cssFile              = $cssDir . $slash . 'tags.css';
		$cssFileStat          = $platform->stat($cssFile);
		$fileLastModifiedDate = $cssFileStat[9];
		if (function_exists('getallheaders')) {
			$headers = getallheaders();
			if (isset($headers['If-modified-since'])) {
				$headers['If-Modified-Since'] = $headers['If-modified-since'];
			}

			if (isset($headers['If-Modified-Since'])
				&& ($when = strtotime($headers['If-Modified-Since'])) > 0
				&& $fileLastModifiedDate < $when
			) {
				// File not changed, don't resend
				header('HTTP/1.0 304 Not Modified');
				return null;
			}
		}

		// Send the CSS
		$cssFileRelative = 'plugins_data' . $slash . 'modules' . $slash . 'tags' . $slash . 'tags.css';
		$success         = $gallery->fastDownload(
			$cssFileRelative,
			'tags.css',
			$fileLastModifiedDate,
			'text/css',
			$platform->filesize($cssFile)
		);

		if (!$success) {
			return GalleryCoreApi::error(
				ERROR_UNKNOWN,
				__FILE__,
				__LINE__,
				'The CSS file could not be sent to the browser'
			);
		}

		return null;
	}
}
