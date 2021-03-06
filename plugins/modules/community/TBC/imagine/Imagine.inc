<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
 * @version $Revision: 1635 $
 * @package Imagine
 * @subpackage UserInterface
 * @author Bharat Mediratta
 */
GalleryCoreApi::requireOnce('modules/core/classes/GalleryCoreSearch.class');

/**
 * This is a sample page generated by the Gallery 2 module creator.
 *
 * @package Imagine
 * @subpackage UserInterface
 *
 */
class ImagineView extends GalleryView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	/**
	 * @see GalleryView::renderImmediate
	 */
	public function renderImmediate($status, $error) {
		global $gallery;

		$storage =& $gallery->getStorage();

		list($a, $q) = GalleryUtilities::getRequestVariables('a', 'q');

		if ($a == 'keywords') {
			list($ret, $results) = $gallery->search(
				'
              SELECT
                DISTINCT([GalleryItem::title])
              FROM
                [GalleryItem]
              WHERE
               [GalleryItem::title] NOT LIKE "IMG%"
            '
			);

			$stopWords = array_flip(
				array(
					'and',
					'on',
					'the',
					'amp',
					'img',
					'dsc',
					'dscn',
					'mvcs',
					'mgjpg',
					'with',
					'mvcd',
					'cimg',
					'from',
					'for',
					'day',
					'this',
					'thumb',
				)
			);

			while ($result = $results->nextResult()) {
				$title = strtolower($result[0]);

				foreach (explode(' ', preg_replace('/[^A-Za-z ]/', '', $title)) as $word) {
					if ($word && strlen($word) > 2 && @!array_key_exists($word, $stopWords)) {
						@$seen[$word]++;
					}
				}
			}

			arsort($seen);
			$i = 0;

			foreach ($seen as $key => $value) {
				echo "$key\n";

				if ($i++ == 100) {
					break;
				}
			}
		} elseif ($a == 'search') {
			list($ret, $results) = GalleryCoreSearch::search(
				array(
					'descriptions' => 1,
					'keywords'     => 1,
					'summaries'    => 1,
					'titles'       => 1,
				),
				$q,
				0,
				15
			);

			if ($ret) {
				return $ret;
			}

			$itemIds = array();

			foreach ($results['results'] as $result) {
				$itemIds[] = $result['itemId'];
			}

			list($ret, $thumbs) = GalleryCoreApi::fetchThumbnailsByItemIds($itemIds);

			if ($ret) {
				return $ret;
			}

			$urlGenerator =& $gallery->getUrlGenerator();

			foreach ($thumbs as $thumb) {
				$url = $urlGenerator->generateUrl(
					array(
						'view'   => 'core.DownloadItem',
						'itemId' => $thumb->getId(),
					),
					array(
						'forceDirect'    => true,
						'forceFullUrl'   => true,
						'forceSessionId' => false,
						'htmlEntities'   => false,
					)
				);

				echo $url;
				echo ' ';
				$bigger = $thumb;

				while (GalleryUtilities::isA($bigger, 'GalleryDerivative')) {
					list($ret, $bigger) = GalleryCoreApi::loadEntitiesById($bigger->getDerivativeSourceId());

					if ($ret) {
						return $ret;
					}
				}

				$biggerId = $bigger->getId();

				list($ret, $resizes) = GalleryCoreApi::fetchResizesByItemIds(array($biggerId));

				if ($ret) {
					return $ret;
				}

				if (isset($resizes[$biggerId]) && $resizes[$biggerId]) {
					$bigger = $resizes[$biggerId][0];
				}

				$url = $urlGenerator->generateUrl(
					array(
						'view'   => 'core.DownloadItem',
						'itemId' => $bigger->getId(),
					),
					array(
						'forceDirect'    => true,
						'forceFullUrl'   => true,
						'forceSessionId' => false,
						'htmlEntities'   => false,
					)
				);

				echo $url;
				echo "\n";
			}
		}

		return null;
	}
}
