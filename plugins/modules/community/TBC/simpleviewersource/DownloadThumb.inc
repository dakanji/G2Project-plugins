<?php

/*
 * $RCSfile: Admin.inc,v $
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
GalleryCoreApi::requireOnce(
	'modules/core/classes/helpers/GalleryDerivativeHelper_simple.class'
);
GalleryCoreApi::requireOnce(

	'modules/core/DownloadItem.inc'
);

/**
 * @package SimpleviewerSource
 * @version $Revision: 1725 $
 * @author Alec Myers <alec@alecmyers.com>
 */
class DownloadThumbView extends DownloadItemView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	public function renderImmediate($status, $error) {
		global $gallery;

		// Figure out which item we're talking about
		$masterId = (int)GalleryUtilities::getRequestVariables('itemId');
		if (empty($masterId)) {
			return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
		}

		// Make sure we have permission to view this item
		$ret = GalleryCoreApi::assertHasItemPermission($masterId, 'core.view');

		if ($ret) {
			return $ret;
		}

		list($ret, $thumbs) = GalleryDerivativeHelper_simple::fetchThumbnailsByItemIds(array($masterId));

		if ($ret) {
			return $ret;
		}

		$item   = $thumbs[$masterId];
		$itemId = $item->getId();

		/*
		 * //try it with a redirect ... shorter code, longer time to service
		 * $urlgen =& $gallery->getUrlGenerator();
		 * $mainUrl = $urlgen->generateUrl(array (
		 * 	'view' => 'core.DownloadItem',
		 * 	'itemId' => $itemId,
		 * ),
		 * array (
		 *	'forceSessionId' => false,
		 *	'forceServerRelativeUrl' => true,
		 *	'htmlEntities' => false,
		 *	'urlEncode' => false
		 * ));
		 *
		 * header("Location:$mainUrl", TRUE, 307);
		 * return null;
		 */
		GalleryUtilities::putRequestVariable('itemId', $itemId);

		list($ret, $view) = GalleryView::loadView('core.DownloadItem');

		if ($ret) {
			return $ret;
		}

		return $view->renderImmediate(null, null);
	}
}
