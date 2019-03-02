<?php
/*
 * $RCSfile: DuplicateTitles.inc,v $
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
GalleryCoreApi::requireOnce('modules/mranalysis/classes/DuplicateItemChecker.class');

/**
 * @package MRAnalysis
 * @subpackage DuplicateTitlesView
 */
class DuplicateTitlesView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$form['formName'] = 'AdminDuplicateTitles';

		$gallery->guaranteeTimeLimit(120);

		$tester = new DuplicateItemChecker();

		$query = '
            SELECT
                [GalleryItem::id]
            FROM
                [GalleryPhotoItem]
            INNER JOIN
				[GalleryItem]
			ON
				[GalleryPhotoItem::id] = [GalleryItem::id]
            INNER JOIN
				[GalleryChildEntity]
			ON
				[GalleryItem::id] = [GalleryChildEntity::id]
        ';

		if ($tester->checktitlesacrossalbums) {
			$query .= ' ORDER BY [GalleryItem::title], [GalleryItem::id]';
		} else {
			$query .= ' ORDER BY [GalleryChildEntity::parentid], [GalleryItem::title], [GalleryItem::id]';
		}

		$items = ItemList::readItemsFromQuery($query, array($tester, 'IsDuplicateTitle'));

		// Determine the sort order
		$sortfunction = 'sort_by_item_path';

		if ($tester->checktitlesacrossalbums) {
			$sortfunction = 'sort_by_item_title_then_path';
		}

		if (count($tester->duplicateitems) > 0) {
			$items = array_merge($items, ItemList::LoadSpecificItems($tester->duplicateitems));

			usort($items, array('ItemList', $sortfunction));
		}

		$template->setVariable('controller', 'mranalysis.DuplicateTitles');
		$template->setVariable('baditems', $items);

		return array(
			null,
			array(
				'body' => 'modules/mranalysis/templates/DuplicateTitles.tpl',
			),
		);
	}
}
