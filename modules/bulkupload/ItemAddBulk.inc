<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
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
 * @version $Revision: 13704 $ $Date: 2006-05-21 10:08:02 -0700 (Sun, 21 May 2006) $
 * @package ItemAdd
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 */

/**
 * This controller will handle the addition of an item as a children to another item.
 *
 * @package ItemAdd
 * @subpackage UserInterface
 */
class ItemAddBulk extends ItemAddPlugin {
	/**
	 * @see ItemAddPlugin::handleRequest
	 */
	public function handleRequest($form, &$item) {
		global $gallery;

		$status = $error = array();

		if (isset($form['action']['add'])) {
			if (empty($form['path'])) {
				$error[] = 'form[error][path]';
			}

			if (empty($error)) {
				$platform =& $gallery->getPlatform();

				if (!$platform->is_readable($form['path'])) {
					$error[] = 'form[error][path][invalid]';
				}
			}

			if (empty($error)) {
				$data = $platform->file($form['path']);

				list($ret, $lockId) = GalleryCoreApi::acquireReadLock($item->getId());

				if ($ret) {
					return array($ret, null, null);
				}

				$fields = split("\t", trim(array_shift($data)));

				foreach ($data as $line) {
					$line = split("\t", trim($line));

					for ($i = 0; $i < count($fields); $i++) {
						$line[$fields[$i]] = $line[$i];
					}

					list($base, $extension) = GalleryUtilities::getFileNameComponents(basename($line['filename']));
					list($ret, $mimeType)   = GalleryCoreApi::convertExtensionToMime($extension);

					if ($ret) {
						GalleryCoreApi::releaseLocks($lockId);

						return array($ret, null, null);
					}

					$fileName = $line['filename'];

					if (!($fileName[0] == '/' || $fileName[0] == '\\')) {
						$fileName = dirname($form['path']) . '/' . $fileName;
					}

					if ($platform->is_readable($fileName)) {
						list($ret, $newItem) = GalleryCoreApi::addItemToAlbum(
							$fileName,
							isset($line['title']) ? $line['title'] : '',
							isset($line['title']) ? $line['title'] : '',
							isset($line['summary']) ? $line['summary'] : '',
							isset($line['description']) ? $line['description'] : '',
							$mimeType,
							$item->getId(),
							false
						);

						if ($ret) {
							GalleryCoreApi::releaseLocks($lockId);

							return array($ret, null, null);
						}

						// Set the keywords by hand, since we don't offer that in the API
						if (!empty($line['keywords'])) {
							list($ret, $itemLockId) = GalleryCoreApi::acquireWriteLock($newItem->getId());

							if ($ret) {
								return array($ret, null, null);
							}

							list($ret, $newItem) = $newItem->refresh();

							if ($ret) {
								return array($ret, null, null);
							}

							$newItem->setKeywords($line['keywords']);
							$ret = $newItem->save();

							if ($ret) {
								return array($ret, null, null);
							}

							$ret = GalleryCoreApi::releaseLocks($itemLockId);

							if ($ret) {
								return array($ret, null, null);
							}
						}
						$newItemId = $newItem->getId();
					} else {
						$newItemId = null;
					}

					$status['addedFiles'][] = array(
						'fileName' => GalleryCoreApi::convertToUtf8($fileName),
						'id'       => $newItemId,
						'warnings' => array(),
					);
				}

				$ret = GalleryCoreApi::releaseLocks($lockId);

				if ($ret) {
					return array($ret, null, null);
				}
			}
		}

		return array(null, $error, $status);
	}

	/**
	 * @see ItemAdd:loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $item) {
		if ($form['formName'] != 'ItemAddBulk') {
			$form['formName'] = 'ItemAddBulk';
		}

		return array(
			null,
			'modules/bulkupload/templates/ItemAddBulk.tpl',
			'modules_bulkupload',
		);
	}

	/**
	 * @see ItemAddPlugin::getTitle
	 */
	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'core');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Bulk Upload'));
	}
}
