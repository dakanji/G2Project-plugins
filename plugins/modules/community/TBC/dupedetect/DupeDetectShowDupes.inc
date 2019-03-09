<?php

/*
 * $RCSfile: DupeDetectShowDupes.inc,v $
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
 * @version $Revision: 0.9 $ $Date: 2005/08/09 05:46:20 $
 * @package dupedetect
 * @author Aviad Tsherniak <tshernia@tau.ac.il>
 */

/**
 * @package DupeDetect
 * @subpackage UserInterface
 */
class DupeDetectShowDupesController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		// Make sure we have adequate permissions
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret->isError()) {
			return array($ret, null);
		}

		if (isset($form['pairs'])) {
			$pairIds = array_keys($form['pairs']);
			foreach ($form['pairs'] as $pairId => $itemIds) {
				$gallery->debug("pair # $pairId is $itemIds \n");
			}

			foreach ($form['actions'] as $pairId => $action) {
				$gallery->debug("pair # $pairId marked for $action \n");
			}
		}

		$status = array();
		if (isset($form['action']['submit'])) {
			$status['ignored']['count'] = 0;
			$status['linked']['count']  = 0;
			$status['deleted']['count'] = 0;
			$pairsToLink                = array();
			$pairsToMarkAsIgnored       = array();
			$itemsToDelete              = array();

			// now let's see what we have to do
			GalleryCoreApi::requireOnce('modules/dupedetect/classes/DupeDetectHelper.class');
			if (isset($form['actions'])) {
				foreach ($form['actions'] as $pairId => $action) {
					list($srcId, $dupId) = explode(',', $form['pairs'][$pairId]);

					switch ($action) {
						case 'ignore':
							$pairsToMarkAsIgnored[] = array(
								'src' => $srcId,
								'dup' => $dupId,
							);

							$status['ignored'][] = $pairId;
							break;
						case 'leaveAsIs':
							// just do nothing
							break;
						case 'linkToSrc':
							$pairsToLink[] = array(
								'master' => $srcId,
								'slave'  => $dupId,
							);

							$status['linked'][] = $pairId;
							break;
						case 'linkToDup':
							$pairsToLink[] = array(
								'master' => $dupId,
								'slave'  => $srcId,
							);

							$status['linked'][] = $pairId;
							break;
						case 'deleteSrc':
							$itemsToDelete[]     = $srcId;
							$status['deleted'][] = $srcId;
							break;
						case 'deleteDup':
							$itemsToDelete[]     = $dupId;
							$status['deleted'][] = $dupId;
							break;
					}
				}
			}

			// handle all of those who need to be linked
			if (isset($pairsToLink)) {
				foreach ($pairsToLink as $pair) {
					list($ret, $newLinkedItem) = DupeDetectHelper::linkItems(
						$pair['master'],
						$pair['slave']
					);

					if ($ret->isError()) {
						return array($ret, null);
					}

					$redirect = true;
					$status['linked']['count']++;
				}
			}

			// handle all pairs who need to be ignored
			if (isset($pairsToMarkAsIgnored)) {
				foreach ($pairsToMarkAsIgnored as $pair) {
					$ret = DupeDetectHelper::markAsIgnoredDuplicate(
						$pair['src'],
						$pair['dup']
					);

					if ($ret->isError()) {
						return array($ret, null);
					}

					$redirect = true;
					$status['ignored']['count']++;
				}
			}

			// handle all items marked for deletion
			if (isset($itemsToDelete)) {
				foreach ($itemsToDelete as $id) {
					// Make sure we have permission to delete this item
					list($ret, $permissions) = GalleryCoreApi::getPermissions($id);

					if ($ret->isError()) {
						return array($ret, null);
					}

					if (!isset($permissions['core.delete'])) {
						return array(
							GalleryStatus::error(
								ERROR_PERMISSION_DENIED,
								__FILE__,
								__LINE__,
								"Don't have permission to delete this item"
							),
							null,
						);
					}

					// so we can delete now
					$ret = GalleryCoreApi::deleteEntityById($id);

					if ($ret->isError()) {
						return array($ret, null);
					}

					$redirect = true;
					$status['deleted']['count']++;
				}
			}
		} // end of form[action][save]
		elseif (isset($form['action']['reset'])) {
			$redirect = true;
		}

		if (isset($redirect)) {
			$results['redirect'] = array(
				'view'    => 'core.SiteAdmin',
				'subView' => 'dupedetect.DupeDetectShowDupes',
			);
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'dupedetect.DupeDetectShowDupes';
		}

		$results['status'] = $status;
		$results['error']  = array();
		return array(GalleryStatus::success(), $results);
	}
}

/**
 * @package DupeDetect
 * @subpackage UserInterface
 */
class DupeDetectShowDupesView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		GalleryCoreApi::requireOnce('modules/dupedetect/classes/DupeDetectHelper.class');
		if ($form['formName'] != 'DupeDetectShowDupes') {
			$form['showIgnored'] = false;
			$form['formName']    = 'DupeDetectShowDupes';
		}

		if (!isset($form['showIgnored'])) {
			$form['showIgnored'] = false;
		}

		$typeToShow = null;
		if ($form['showIgnored'] == true) {
			$statusToShow = null;
		} else {
			$statusToShow = DUPEDETECT_UNHANDLED_DUPLICATE;
		}

		list($ret, $dupList) = DupeDetectHelper::fetchDupes(
			null,
			null,
			$typeToShow,
			$statusToShow
		);

		if ($ret->isError()) {
			return array($ret, null);
		}

		/* remove pairs that appear more than once in the list, due to multiple DiD algorithm;
		 * e.g. an exact duplication will be discovered both by CRC32 and histogram
		 */
		$pairsIds = array();
		foreach ($dupList as $key => $dupRecord) {
			$pairId = $dupRecord['srcItemId'] . '|' . $dupRecord['dupItemId'];
			if (array_key_exists($pairId, $pairsIds)) {
				$gallery->debug("dupList size is $dupList\n");

				// take the exact one
				if ($dupRecord['dupType'] == DUPEDETECT_HISTOGRAM_ALGORITHM) {
					$gallery->debug("***splicing out pair $key ($pairId)- new is histogram***\n");
					unset($dupList[$key]);
				} else { // $dupRecord is CRC32 duplication, so delete the other one
					$gallery->debug("***splicing out pair $pairId  - new is CRC32***\n");
					unset($dupList[$pairsIds[$pairId]]);
					$pairsIds[$pairId] = $key;
				}
			} else {
				$pairsIds[$pairId] = $key;
			}
		}

		// create a list of ids for all the items - source and dups - for thumb extraction
		$allIds = array();
		foreach ($dupList as $dupRecord) {
			$allIds[] = $dupRecord['srcItemId'];
			$allIds[] = $dupRecord['dupItemId'];
		}

		// get thumbnails
		list($ret, $thumbnails) = GalleryCoreApi::fetchThumbnailsByItemIds($allIds);

		if ($ret->isError()) {
			return array($ret, null);
		}

		$pairsList = array();
		foreach ($dupList as $dupRecord) {
			$dupRecord['srcItemData'] = $dupRecord['srcItem']->getMemberData();

			// attach thumbnail
			if (isset($thumbnails[$dupRecord['srcItem']->getId()])) {
				$thumbnail = $thumbnails[$dupRecord['srcItem']->getId()];

				list($ret, $thumbnail) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent(
					$thumbnail->getId()
				);

				if ($ret->isError()) {
					// Ignore thumbnail errors
				} else {
					$dupRecord['srcItemData']['thumbnail'] = $thumbnail->getMemberData();
				}
			}

			$dupRecord['dupItemData'] = $dupRecord['dupItem']->getMemberData();

			// attach thumbnail
			if (isset($thumbnails[$dupRecord['dupItem']->getId()])) {
				$thumbnail = $thumbnails[$dupRecord['dupItem']->getId()];

				list($ret, $thumbnail) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent(
					$thumbnail->getId()
				);

				if ($ret->isError()) {
					// Ignore thumbnail errors
				} else {
					$dupRecord['dupItemData']['thumbnail'] = $thumbnail->getMemberData();
				}
			}

			$dupRecord['pairId'] = count($pairsList);
			$pairsList[]         = $dupRecord;
		}

		$form['list'] = $pairsList;
		$dupTypeList  = array(
			DUPEDETECT_CRC32_ALGORITHM     => 'Exact <BR> (CRC32)',
			DUPEDETECT_HISTOGRAM_ALGORITHM => 'Similar <BR>(Histogram)',
		);

		$form['dupTypeList'] = $dupTypeList;
		$dupStatus           = array(
			DUPEDETECT_UNHANDLED_DUPLICATE => 'New duplicate',
			DUPEDETECT_IGNORED_DUPLICATE   => 'Marked as ignored',
		);

		$form['dupStatusList'] = $dupStatus;
		$template->setVariable('controller', 'dupedetect.DupeDetectShowDupes');
		return array(
			GalleryStatus::success(),
			array(
				'body' => 'modules/dupedetect/templates/DupeDetectShowDupes.tpl',
			),
		);
	}
}
