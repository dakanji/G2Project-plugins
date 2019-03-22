<?php

/*
 * $RCSfile: MissingTitles.inc,v $
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
 * @subpackage MissingTitlesView
 */
class MissingTitlesView extends GalleryView {
	/**
	 * @Checks to see if the title is a default camera generated filename.
	 */
	public static function IsBlankOrDefaultFileName($item) {
		$title = $item->title;

		if (strlen($title) == 0) {
			return true;
		}

		return Classification::IsDefaultFileName($title);
	}

	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$form['formName'] = 'AdminMissingTitles';

		$gallery->guaranteeTimeLimit(120);

		$prefixes = Classification::DefaultFileNamePrefixes();

		$query = "
	    	SELECT
				[GalleryPhotoItem::id]
			FROM
				[GalleryPhotoItem]
			INNER JOIN
				[GalleryItem]
			ON
				[GalleryPhotoItem::id] = [GalleryItem::id]
			WHERE
				[GalleryItem::title] IS NULL
			OR
				[GalleryItem::title] = ''
		";

		foreach ($prefixes as $prefix) {
			$query .= " OR [GalleryItem::title] LIKE '$prefix%'";
		}

		$query .= ' ORDER BY [GalleryPhotoItem::id]';

		$items = ItemList::readItemsFromQuery(
			$query,
			array('MissingTitlesView', 'IsBlankOrDefaultFileName')
		);

		$template->setVariable('controller', 'mranalysis.MissingTitles');
		$template->setVariable('baditems', $items);

		return array(
			null,
			array(
				'body' => 'modules/mranalysis/templates/MissingTitles.tpl',
			),
		);
	}
}
