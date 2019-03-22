<?php

/*
 * $RCSfile: BrokenImages.inc,v $
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
GalleryCoreApi::requireOnce('modules/mranalysis/classes/SettingsAccess.class');
GalleryCoreApi::requireOnce('modules/mranalysis/classes/SettingsConstants.class');

/**
 * @package MRAnalysis
 * @subpackage BrokenImagesView
 */
class BrokenImagesView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$form['formName'] = 'AdminBrokenImages';

		$gallery->guaranteeTimeLimit(120);

		$query = '
            SELECT DISTINCT
                [GalleryPhotoItem::id]
            FROM
                [GalleryDerivativeImage]
            INNER JOIN
				[GalleryDerivative]
			ON [
				GalleryDerivativeImage::id] = [GalleryDerivative::id]
            INNER JOIN
				[GalleryChildEntity]
			ON
				[GalleryDerivativeImage::id] = [GalleryChildEntity::id]
            INNER JOIN
				[GalleryEntity]
			ON
				[GalleryDerivativeImage::id] = [GalleryEntity::id]
            INNER JOIN
				[GalleryPhotoItem]
			ON
				[GalleryChildEntity::parentId] = [GalleryPhotoItem::id]
            WHERE
				[GalleryDerivative::isBroken] = 1
		';

		if (SettingsAccess::GetValueWithDefault(
			'TreatZeroSizedDerivativesAsBroken',
			SettingsConstants::DefaultTreatZeroSizedDerivativesAsBroken()
		)
		) {
			$query .= ' OR ([GalleryDerivativeImage::width] = 0 and [GalleryDerivativeImage::height] = 0)';
		}

		$query .= ' ORDER BY [GalleryPhotoItem::id]';

		$items = ItemList::readItemsFromQuery($query);

		$template->setVariable('controller', 'mranalysis.BrokenImages');
		$template->setVariable('baditems', $items);

		return array(
			null,
			array(
				'body' => 'modules/mranalysis/templates/BrokenImages.tpl',
			),
		);
	}
}
