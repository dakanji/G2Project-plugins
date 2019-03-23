<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
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
GalleryCoreApi::requireOnce('modules/tags/classes/TagsHelper.class');

/**
 * This controller will save many item tags at once.
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 * @author Changpeng Zhao
 * @author Eric Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1235 $
 */
class BulkTagsEditController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		list($itemId, $page) = GalleryUtilities::getRequestVariables('itemId', 'page');

		$status = $error = array();

		if (isset($form['action']['save'])) {
			$ids = array_keys($form['items']);

			list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($ids);

			if ($ret) {
				return array($ret, null);
			}

			// We'll check permissions one at a time below, but precache them now
			$ret = GalleryCoreApi::studyPermissions($ids);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $items) = GalleryCoreApi::loadEntitiesById($ids);

			if ($ret) {
				return array($ret, null);
			}

			$status['successCount'] = 0;
			$status['errorCount']   = 0;

			foreach ($items as $item) {
				// Make sure we have permission to edit this item
				list($ret, $permissions) = GalleryCoreApi::getPermissions($item->getId());

				if ($ret) {
					return array($ret, null);
				}

				$id = $item->getId();

				if (isset($permissions['tags.edit'])) {
					if ($item->getSerialNumber() == $form['items'][$id]['serialNumber']) {
						// Save the tags
						list($ret, $tags) = TagsHelper::splitTags($form['items'][$id]['tags']);

						if ($ret) {
							return array($ret, null);
						}

						foreach ($tags as $tagName) {
							$ret = TagsHelper::addNewTag($tagName);

							if ($ret) {
								return array($ret, null);
							}

							$ret = TagsHelper::assignTag($id, $tagName);

							if ($ret) {
								return array($ret, null);
							}
						}

						$status[$id]['saved'] = 1;
						$status['successCount']++;
					} else {
						$status[$id]['obsolete'] = 1;
						$status['errorCount']++;
					}
				} else {
					$status[$id]['permissionDenied'] = 1;
					$status['errorCount']++;
				}
			}

			$ret = GalleryCoreApi::releaseLocks($lockId);

			if ($ret) {
				return array($ret, null);
			}

			/*
			 * Figure out where to redirect.  We always redirect even if we have
			 * an error since we may have saved some items, but not others.
			 */
			$redirect['view']   = 'tags.BulkTagsEdit';
			$redirect['itemId'] = (int)$itemId;

			if (!$status['errorCount'] && isset($form['action']['save']['next'])) {
				$redirect['page'] = $page + 1;
			} elseif (!$status['errorCount'] && isset($form['action']['save']['previous'])) {
				$redirect['page'] = max($page - 1, 0);
			} elseif (!$status['errorCount'] && isset($form['action']['save']['done'])) {
				$results['return'] = 1;
				$redirect['page']  = (int)$page;
			} else {
				$redirect['page'] = (int)$page;
			}
		} elseif (isset($form['action']['cancel'])) {
			$results['return'] = 1;
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']   = 'tags.BulkTagsEdit';
			$results['delegate']['itemId'] = (int)$itemId;
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * This view will allow the user to edit many item tags at once.
 */
class BulkTagsEditView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		list($itemId, $page, $selectedId, $albumPage) = GalleryUtilities::getRequestVariables('itemId', 'page', 'selectedId', 'albumPage');

		if ($form['formName'] != 'BulkTagsEdit') {
			$form['formName']   = 'BulkTagsEdit';
			$form['numPerPage'] = 15;

			list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $childIds) = GalleryCoreApi::fetchChildItemIdsWithPermission(
				$itemId,
				'tags.edit'
			);

			if ($ret) {
				return array($ret, null);
			}

			$form['childItems'] = array();
			$numPages           = 1;
			$numPages           = ceil(sizeof($childIds) / $form['numPerPage']);

			if (empty($page) && !empty($selectedId)) {
				// No page given.  Determine which page we're on from the selectedId
				for ($i = 0; $i < count($childIds); $i++) {
					if ($childIds[$i] == $selectedId) {
						$page = ceil(($i + 1) / $form['numPerPage']);
					}
				}
			}

			if (empty($page) && !empty($albumPage)) {
				// Still no page.  Determine which page we're on from albumPage
				list($ret, $theme) = $this->loadThemeForItem($item);

				if ($ret) {
					return array($ret, null);
				}

				list($ret, $params) = $theme->fetchParameters($itemId);

				if ($ret) {
					return array($ret, null);
				}

				$albumPageSize = $theme->getPageSize($params);

				if (!empty($albumPageSize)) {
					$page = ceil((($albumPage - 1) * $albumPageSize + 1) / $form['numPerPage']);
				}
			}

			if (empty($page)) {
				$page = 1;
			}

			$start    = $form['numPerPage'] * ($page - 1);
			$childIds = array_slice($childIds, $start, $form['numPerPage']);

			list($ret, $childItems) = GalleryCoreApi::loadEntitiesById($childIds);

			if ($ret) {
				return array($ret, null);
			}

			// Get child thumbnails and resizes
			list($ret, $derivatives) = GalleryCoreApi::fetchDerivativesByItemIds($childIds);

			if ($ret) {
				return array($ret, null);
			}

			// Build peers table
			foreach ($childItems as $child) {
				$childId                 = $child->getId();
				$form['items'][$childId] = (array)$child;

				// While we're at it, attach thumbnails and resizes
				if (isset($derivatives[$childId])) {
					foreach ($derivatives[$childId] as $derivative) {
						$type = $derivative->getDerivativeType();

						if (empty($form['items'][$childId]['resize'])
							&& $type == DERIVATIVE_TYPE_IMAGE_RESIZE
						) {
							$form['items'][$childId]['resize'] = (array)$derivative;
						} elseif ($type == DERIVATIVE_TYPE_IMAGE_THUMBNAIL) {
							$form['items'][$childId]['thumbnail'] = (array)$derivative;
						}
					}
				}

				// Add the tags
				list($ret, $tags) = TagsHelper::getTagsByItemId($childId);

				if ($ret) {
					return array($ret, null);
				}

				$form['items'][$childId]['tags'] = implode(', ', $tags);
			}
		}

		$urlGenerator              =& $gallery->getUrlGenerator();
		$BulkTagsEdit              = array();
		$BulkTagsEdit['canCancel'] = $urlGenerator->isNavigationBackPossible();
		$BulkTagsEdit['page']      = $page;
		$BulkTagsEdit['itemId']    = $itemId;
		$BulkTagsEdit['numPages']  = $numPages;

		$template->setVariable('BulkTagsEdit', $BulkTagsEdit);
		$template->setVariable('controller', 'tags.BulkTagsEdit');
		$template->javascript('lib/yui/yahoo-min.js');
		$template->javascript('lib/yui/dom-min.js');
		$template->javascript('lib/yui/event-min.js');
		$template->javascript('lib/yui/container-min.js');

		return array(
			null,
			array(
				'body' => 'modules/tags/templates/BulkTagsEdit.tpl',
			),
		);
	}

	/**
	 * @see GalleryView::getViewDescription
	 */
	public function getViewDescription() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'tags');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('bulk tags edit'));
	}
}
