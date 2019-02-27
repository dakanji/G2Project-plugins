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

GalleryCoreApi::requireOnce('modules/title/Begin.inc');/**
 * Dynamic album view of items whose titles contain given chars
 * @package Title
 * @author Alec Myers <alec@alecmyers.com>
 * @author Alan Harder <alan.harder@sun.com>
 * @version $Revision: 1457 $
 */
class ContainView extends BeginView {
	public function __construct() {
		global $gallery;
		$this->_view            = 'title.Contain';
		$this->_title           = $gallery->i18n('Items whose titles contain');
		$this->_itemType        = array($gallery->i18n('Contain'), $gallery->i18n('contain'));
		$this->_viewDescription = $gallery->i18n('Items whose titles contain a given string');
		$this->_param           = 'contain';
	}

	/**
	 * Dynamic query for items
	 * @param int $userId
	 * @return array object GalleryStatus a status code
	 *               array of item ids
	 */
	public function getChildIds($userId, $chars = null) {
		global $gallery;

		if (!isset($chars)) {
			$chars = GalleryUtilities::getRequestVariables('chars');
		}

		$results = array();
		$chars   = '%' . $chars . '%';

		$query = '
			SELECT
				[GalleryItem::id]
			FROM
				[GalleryItem]
			WHERE
				[GalleryItem::title]
			LIKE ?
			ORDER BY
				[GalleryItem::title]
		';

		$queryOptions              = array($chars);
		list($ret, $searchResults) = $gallery->search($query, $queryOptions);

		if ($ret) {
			return array($ret, null);
		}

		while ($result = $searchResults->nextResult()) {
			$results[] = $result[0];
		}

		// safety limit of first 100 results
		$results = array_slice($results, 0, 100);

		return array(null, $results);
	}
}
