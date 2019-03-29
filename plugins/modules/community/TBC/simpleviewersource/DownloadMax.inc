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
GalleryCoreApi::requireOnce('modules/core/classes/helpers/GalleryDerivativeHelper_simple.class');
GalleryCoreApi::requireOnce('modules/core/DownloadItem.inc');

/**
 * @package SimpleviewerSource
 * @version $Revision: 1725 $
 * @author Alec Myers <alec@alecmyers.com>
 */
class DownloadMaxView extends DownloadItemView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	public function renderImmediate($status, $error) {
		global $gallery;

		// Figure out which item we are talking about
		$masterId       = (int)GalleryUtilities::getRequestVariables('itemId');
		$maxImageHeight = (int)GalleryUtilities::getRequestVariables('maxImageHeight');
		$maxImageWidth  = (int)GalleryUtilities::getRequestVariables('maxImageWidth');

		if (empty($masterId)) {
			return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
		}

		// Make sure we have permission to view this item
		$ret = GalleryCoreApi::assertHasItemPermission($masterId, 'core.view');

		if ($ret) {
			return $ret;
		}

		list($ret, $itemId) = DownloadMaxView::getBestImageId($masterId, $maxImageHeight, $maxImageWidth);

		if ($ret) {
			return $ret;
		}

		/*
		$urlgen =& $gallery->getUrlGenerator();
		$mainUrl = $urlgen->generateUrl( array ( 'view' => 'core.DownloadItem',
						 'itemId' => $itemId ),
					 array ( 'forceSessionId' => false,
						  'forceServerRelativeUrl' => true,
						  'htmlEntities' => false,
						  'urlEncode' => false ));
		header("Location:$mainUrl", TRUE, 307 );
		*/
		GalleryUtilities::putRequestVariable('itemId', $itemId);

		list($ret, $view) = GalleryView::loadView('core.DownloadItem');

		if ($ret) {
			return $ret;
		}

		return $view->renderImmediate(null, null);

		return null;
	}

	public function getBestImageId($masterId, $maxImageHeight, $maxImageWidth) {
		$potentialImages = array();

		//how about the original?
		$ret = GalleryCoreApi::assertHasItemPermission($masterId, 'core.viewSource');

		if (!$ret) {
			//is there a preferred derivative of the original?
			list($ret, $preferred) = GalleryCoreApi::fetchPreferredsByItemIds(array($masterId));

			if ($ret) {
				return array($ret, null);
			}

			if (!empty($preferred[$masterId])) {
				$potentialImages[] = $preferred[$masterId];
			} else {
				//if no preferred, use the original	original
				list($ret, $item) = GalleryCoreApi::loadEntitiesById($masterId);

				if ($ret) {
					return array($ret, null);
				}

				$potentialImages[] = $item;
			}
		}

		// If the user can see resized versions consider those too
		$ret = GalleryCoreApi::assertHasItemPermission($masterId, 'core.viewResizes');

		if (!$ret) {
			list($ret, $resizes) = GalleryCoreApi::fetchResizesByItemIds(array($masterId));

			if ($ret) {
				return array($ret, null);
			}

			if (array_key_exists($masterId, $resizes)) {
				foreach ($resizes[$masterId] as $resize) {
					list($ret, $uptodateResize) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($resize->getId());

					if ($ret) {
						return $ret;
					}

					$potentialImages[] = $uptodateResize;
				}
			}
		}

		//can always use the thumbnail
		list($ret, $thumbs) = GalleryCoreApi::fetchThumbnailsByItemIds(array($masterId));

		if ($ret) {
			return array($ret, null);
		}

		$potentialImages[] = $thumbs[$masterId];

		/*
		 * true if maxDimensions are taller/narrower than image,
		 * in which case width is the constraint:
		 */
		$widthbound = (!$maxImageHeight || $potentialImages[0]->height * $maxImageWidth < $potentialImages[0]->width * $maxImageHeight) ? 1 : 0;
		usort($potentialImages, array('DownloadMaxView', 'byWidth'));

		if ($maxImageWidth && $widthbound) {
			foreach ($potentialImages as $potentialImage) {
				if ($potentialImage->width >= $maxImageWidth) {
					//return the first one wider than $maxImageWidth
					return array(null, $potentialImage->getId());
				}
			}
		} elseif ($maxImageHeight) {
			foreach ($potentialImages as $potentialImage) {
				if ($potentialImage->height >= $maxImageHeight) {
					//return the first one taller than $maxImageHeight
					return array(null, $potentialImage->getId());
				}
			}
		}

		$bestImage = array_pop($potentialImages);

		//none of them big enough - use the largest
		return array(null, $bestImage->getId());
	}

	public function byWidth($a, $b) {
		if ($a->width == $b->width) {
			return 0;
		}

		return ($a->width < $b->width) ? -1 : 1;
	}
}
