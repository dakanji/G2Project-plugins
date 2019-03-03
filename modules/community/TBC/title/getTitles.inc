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
 * @package Titles
 * @author AlecMyers <alec@alecmyers.com>
 * @version $Revision: 1420 $
 */
class GetTitlesView extends GalleryView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	/**
	 * @see GalleryView::isControllerLike
	 */
	public function isControllerLike() {
		return true;
	}

	/**
	 * @see GalleryView::renderImmediate
	 */
	public function renderImmediate($status, $error) {
		global $gallery;

		list($command, $chars) = GalleryUtilities::getRequestVariables('command', 'chars');

		if (!headers_sent()) {
			header('Content-type: text/plain; charset=UTF-8');
		}

		switch ($command) {
			case 'begin':
				$chars .= '%';

				break;

			case 'contain':
				$chars = '%' . $chars . '%';

				break;

			default:
				return null;
		}

		$results = array();

		$query        = 'SELECT 
		[GalleryItem::title]
		FROM [GalleryItem]
		WHERE
		[GalleryItem::title]
		LIKE ?
		ORDER BY [GalleryItem::title]
		';
		$queryOptions = array($chars);

		list($ret, $searchResults) = $gallery->search($query, $queryOptions);

		if ($ret) {
			return $ret;
		}

		while ($result = $searchResults->nextResult()) {
			$results[] = $result[0];
		}

		// safety limit of first 100 results
		$results = array_slice($results, 0, 100);
		echo implode("\n", $results);

		return null;
	}
}
