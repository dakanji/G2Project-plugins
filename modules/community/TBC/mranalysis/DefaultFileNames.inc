<?php

/*
 * $RCSfile: DefaultFileNames.inc,v $
 * $Revision: 1848 $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/**
 *
 * @package MRAnalysis
 * @author Mark Ridgwell <g2development@markridgwell.com>
 */

// Required Files\Classes
GalleryCoreApi::requireOnce('modules/mranalysis/classes/ItemList.class');
GalleryCoreApi::requireOnce('modules/mranalysis/classes/Classification.class');

/**
 * @package MRAnalysis
 * @subpackage DefaultFileNamesView
 */
class DefaultFileNamesView extends GalleryView {
	/**
	 * @Checks to see if the filename is a default camera generated one.
	 */
	public static function IsDefaultFileName($item) {
		return Classification::IsDefaultFileName($item->pathComponent);
	}

	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$form['formName'] = 'AdminDefaultFileNames';

		$gallery->guaranteeTimeLimit(120);
		$prefixes    = Classification::DefaultFileNamePrefixes();
		$firstprefix = true;

		$query = '
            SELECT
                [GalleryPhotoItem::id]
            FROM
                [GalleryPhotoItem]
            INNER JOIN
				[GalleryItem]
			ON
				[GalleryPhotoItem::id] = [GalleryItem::id]
            INNER JOIN
				[GalleryFileSystemEntity]
			ON
				[GalleryItem::id] [GalleryFileSystemEntity::id]
            WHERE
        ';

		foreach ($prefixes as $prefix) {
			if ($firstprefix) {
				$firstprefix = false;
			} else {
				$query = $query . ' OR ';
			}

			$query .= " [FileSystemEntity::pathComponent] LIKE '" . $prefix . "%'";
		}

		$query .= ' ORDER BY [GalleryPhotoItem::id]';
		$items  = ItemList::readItemsFromQuery(
			$query,
			array('DefaultFileNamesView', 'IsDefaultFileName')
		);

		$template->setVariable('controller', 'mranalysis.DefaultFileNames');
		$template->setVariable('baditems', $items);

		return array(
			null,
			array(
				'body' => 'modules/mranalysis/templates/DefaultFileNames.tpl',
			),
		);
	}
}
