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

/**
 * @package CustomDerivatives
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1514 $
 */
class DerivativesItemEdit extends ItemEditPlugin {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function handleRequest($form, &$item, &$preferred) {
		global $gallery;

		$error = array();

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null, null, null);
		}

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'watermark');

		if ($ret) {
			return array($ret, null, null, null);
		}

		$itemId = $item->getId();
		if (isset($form['action']['save'])) {
			// Save data
			foreach ($form['derivatives'] as $derivativeId => $derivativeInfo) {
				if (isset($form['action']['save']['all']) or isset($form['action']['save'][$derivativeId])) {
					if (isset($derivativeInfo['delete'])) {
						// Delete
						$ret = GalleryCoreApi::deleteEntityById($derivativeId);

						if ($ret) {
							return array($ret, null, null, null);
						}

						unset($form['derivatives'][$derivativeId]);
					} elseif ($derivativeInfo['derivativeSourceId']) {
						if ($derivativeId > 0) {
							// Existing, get lock
							list($ret, $derivative) = GalleryCoreApi::loadEntitiesById($derivativeId);

							if ($ret) {
								return array($ret, null, null, null);
							}

							list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($derivative->getId());

							if ($ret) {
								return array($ret, null, null, null);
							}
						} else {
							// New
							list($ret, $source) = GalleryCoreApi::loadEntitiesById($derivativeInfo['derivativeSourceId']);

							if ($ret) {
								return array($ret, null, null, null);
							}

							list($ret, $derivative) = GalleryCoreApi::newFactoryInstanceByHint('GalleryDerivative', $source->getEntityType());

							if ($ret) {
								return array($ret, null, null, null);
							}

							if (!isset($derivative)) {
								return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null, null, null);
							}

							$ret = $derivative->create($itemId, $derivativeInfo['derivativeType']);

							if ($ret) {
								return array($ret, null, null, null);
							}

							$derivative->setMimeType('???');
						}

						// Save
						$derivative->setDerivativeSourceId($derivativeInfo['derivativeSourceId']);
						$derivative->setDerivativeType($derivativeInfo['derivativeType']);
						$derivative->setDerivativeOperations($derivativeInfo['derivativeOperations']);
						if (isset($derivativeInfo['postFilterOperations'])) {
							$derivative->setPostFilterOperations($derivativeInfo['postFilterOperations']);
						}

						$ret = $derivative->save();

						if ($ret) {
							return array($ret, null, null, null);
						}

						if (isset($lockId)) {
							$ret = GalleryCoreApi::releaseLocks($lockId);

							if ($ret) {
								return array($ret, null, null, null);
							}

							$lockId = null;
						}

						list($ret, $derivative) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($derivative->getId(), true);

						if ($ret) {
							return array($ret, null, null, null);
						}
					}
				}
			}

			$status = $module->translate('Changes saved successfully');
		}

		return array(null, $error, $status, false);
	}

	/**
	 * @see ItemEditPlugin::loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $item, $thumbnail) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'customderivatives');

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'DerivativesItemEdit') {
			$form['formName'] = 'DerivativesItemEdit';
		}

		$itemId = $item->getId();
		if (!empty($itemId)) {
			list($ret, $derivs) = GalleryCoreApi::fetchDerivativesByItemIds(array($itemId));

			if ($ret) {
				return array($ret, null);
			}

			if (isset($derivs[$itemId])) {
				foreach ($derivs[$itemId] as $deriv) {
					$form['derivatives'][] = (array)$deriv;
				}
			}

			$form['derivatives'][] = array(
				'id'                   => $module->translate('New'),
				'derivativeType'       => 2,
				'derivativeSize'       => '',
				'width'                => '',
				'height'               => '',
				'mimeType'             => '',
				'derivativeSourceId'   => '',
				'derivativeOperations' => '',
				'postFilterOperations' => '',
				'isBroken'             => '',
			);
		}

		$template->setVariable('id', $itemId);

		return array(null, 'modules/customderivatives/templates/DerivativesItemEdit.tpl', 'modules_customderivatives');
	}

	/**
	 * @see ItemEditPlugin::isSupported
	 */
	public function isSupported($item, $thumbnail) {
		//check if this should return a result code or not
		if (GalleryUtilities::isA($item, 'GalleryPhotoItem')) {
			return true;
		}
	}

	/**
	 * @see ItemEditPlugin::getTitle
	 */
	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'customderivatives');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Derivatives'));
	}
}
