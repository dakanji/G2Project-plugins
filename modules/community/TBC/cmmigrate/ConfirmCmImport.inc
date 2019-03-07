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
GalleryCoreApi::requireOnce('modules/cmmigrate/classes/CmDatabaseTools.class');

/**
 * @package CmMigrate
 * @subpackage UserInterface
 * @author David Angier <david@angier.co.uk>
 * @version $Revision: 847 $
 */
class ConfirmCmImportController extends GalleryController {
	/**
	 * the CmDatabase instance handling the interface to the Coppermine database
	 * @var object CmDatabase
	 */
	public $_cmdb;

	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		// Verify that active user is an admin
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$copperminePath = GalleryUtilities::getRequestVariables('copperminePath');

		$this->_openDatabase($copperminePath);

		if ($ret) {
			return array($ret, null);
		}

		// build framework for error and status arrays
		$error  = array();
		$status = array();

		if (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'cmmigrate.SelectCmGallery';
		} elseif (isset($form['action']['import'])) {
			$templateAdapter =& $gallery->getTemplateAdapter();
			$templateAdapter->registerTrailerCallback(
				array($this, 'performCmImport'),
				array($form, $copperminePath)
			);
			$results['delegate']['view'] = 'core.ProgressBar';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} elseif (empty($results['delegate'])) {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'cmmigrate.ConfirmCmImport';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}

	/**
	 * Actually perform the import
	 *
	 * @param array the form variables
	 * @param string the albums path
	 * @return object GalleryStatus a status code
	 */
	public function performCmImport($form, $copperminePath) {
		global $gallery;

		$storage         = $gallery->getStorage();
		$templateAdapter =& $gallery->getTemplateAdapter();
		$platform        =& $gallery->getPlatform();
		$slash           = $platform->getDirectorySeparator();
		$finishedAlbums  = $albumPosition  = $hiddenAlbums  = array();
		$status          = array(
			'abumImportFailure'  => array(),
			'albumImportSuccess' => array(),
		);

		/*
		 * owner => core.all (which also gives comment.all)
		 *
		 * canRead => core.view,core.viewResizes
		 * canViewFullImages => core.viewSource
		 * canViewComments =>
		 * comment.view
		 * canAddComments => comment.add
		 * canChangeText => core.edit
		 * canAddTo => core.addDataItem
		 * canWrite => core.edit
		 * canDeleteFrom => core.delete
		 * canCreateSubAlbum => core.addAlbumItem
		 *
		 */
		$permissionTranslation = array(
			'canRead'           => array(
				'core.view',
				'core.viewResizes',
			),
			'canViewFullImages' => array('core.viewSource'),
			'canChangeText'     => array('core.edit'),
			'canAddTo'          => array('core.addDataItem'),
			'canWrite'          => array('core.edit'),
			'canDeleteFrom'     => array('core.delete'),
			'canCreateSubAlbum' => array('core.addAlbumItem'),
			'canAddComment'     => array(),
			'canViewComments'   => array(),
		);

		// Get GroupIds for "everybody" and "all users"
		list($ret, $coreParams) = GalleryCoreApi::fetchAllPluginParameters('module', 'core');

		if ($ret) {
			return $ret;
		}

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'cmmigrate');

		if ($ret) {
			return $ret;
		}

		$importPhotosMessage = $module->translate('Importing Photos');

		list($ret, $markupType) = GalleryCoreApi::getPluginParameter('module', 'core', 'misc.markup');

		if ($ret) {
			return $ret;
		}

		$itemsCreated = array();

		/*
		 * Import Any Selected Albums
		 *
		 * First initialize the $form['sourceAlbums'] array to avoid php
		 * warnings
		 */
		if (!isset($form['sourceAlbums'])) {
			$form['sourceAlbums'] = array();
		}

		/*
		 * Check to see if the user selected a destination.  If not,
		 * default to the root album of the new gallery install.
		 */
		if (!isset($form['destinationAlbumID'])) {
			list($ret, $rootId) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.rootAlbum');

			if ($ret) {
				return $ret;
			}

			$form['destinationAlbumID'] = $rootId;
		}

		// Convert the form variable into a local temporary variable
		$destinationAlbumID = $form['destinationAlbumID'];

		// Go through the list of selected coppermine albums to import
		$totalItemsToImport = 0;
		$albumFields        = $albumHighlight        = array();

		list($ret, $albums) = $this->_getAlbums();

		if ($ret) {
			return $ret;
		}

		foreach (array_keys($form['sourceAlbums']) as $sourceAlbumId) {
			/*
			 * attempt to load the metadata from the selected album into a
			 * local cache for later manipulation
			 */
			if (!in_array($sourceAlbumId, array_keys($albums))) {
				return GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__);
			}

			$albumFields[$sourceAlbumId] = $albums[$sourceAlbumId];

			list($ret, $photos) = $this->_cmdb->getPhotos($albums[$sourceAlbumId]);

			if ($ret) {
				return $ret;
			}

			$picturesToImport[$sourceAlbumId] = $photos;
			$totalItemsToImport              += 1 + count($photos);
		}

		/*
		 * Go through the array of albums' metadata one album at a time.  We limit the number of
		 * passes through this list to n^2 times to avoid an infinite loop.  This allows us to make
		 * sure that we add parent albums first by taking albums out of the list as they are
		 * handled.
		 */
		$i = $numberOfItemsImported = 0;

		while ($albumFields && $i < count($form['sourceAlbums']) ** 2 && !connection_aborted()) {
			$gallery->guaranteeTimeLimit(30);
			$album = array_shift($albumFields);

			/*
			 * if the album's parent is not in the list of sourcealbums,
			 * load it into the targeted G2 top album as set in $destinationAlbumID
			 */
			$targetAlbumID = $newAlbumInstanceId = false;

			if (!in_array($album->getParent(), array_keys($form['sourceAlbums']))) {
				$targetAlbumID = $destinationAlbumID;
			} elseif (in_array($album->getParent(), array_keys($finishedAlbums))) {
				// if the album's parent has already been imported, import it into the parent
				$targetAlbumID = $finishedAlbums[$album->getParent()];
			} else {
				$albumFields[$album->albumId] = $album;
				$album                        = null;
			}

			if ($targetAlbumID && $album) {
				// We definitely need notices before each album
				$ret = $templateAdapter->updateProgressBar(
					$importPhotosMessage,
					$album->name,
					$numberOfItemsImported / $totalItemsToImport
				);

				if ($ret) {
					return $ret;
				}

				$desiredname = $platform->legalizePathComponent($album->name);
				$k           = 0;
				$invalidName = true;

				while ($invalidName) {
					list($ret, $existingAlbumId) = GalleryCoreApi::fetchChildIdByPathComponent($targetAlbumID, $desiredname);

					if ($ret) {
						if (!$ret->getErrorCode() & ERROR_MISSING_OBJECT) {
							return $ret;
						}

						$invalidName = false;
					} else {
						$desiredname = $album->name . '_' . $k++;
					}
				}

				if ($targetAlbumID) {
					// Make sure we have permission to edit the target item
					$ret = GalleryCoreApi::assertHasItemPermission(
						$targetAlbumID,
						'core.addAlbumItem'
					);

					if ($ret) {
						return $ret;
					}

					// Try to load targeted parent
					list($ret, $targetAlbumObject) = GalleryCoreApi::loadEntitiesById($targetAlbumID);

					if ($ret) {
						return $ret;
					}

					// Get a lock on said parent
					list($ret, $importLockIds[]) = GalleryCoreApi::acquireReadLock($targetAlbumObject->getId());

					if ($ret) {
						return $ret;
					}

					// If everything is good so far, we create a new instance to be our new album
					if (!empty($desiredname) && $platform->isLegalPathComponent($desiredname)) {
						list($ret, $newAlbumInstance) = GalleryCoreApi::newFactoryInstance('GalleryEntity', 'GalleryAlbumItem');

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						if (!isset($newAlbumInstance)) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return GalleryCoreApi::error(ERROR_MISSING_OBJECT, __FILE__, __LINE__);
						}

						// this is where the album is actually created
						$ret = $newAlbumInstance->create($targetAlbumID, $desiredname);

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						$newAlbumInstanceId = $newAlbumInstance->getId();
						$sourceData         = array(
							'title'   => null,
							'summary' => null,
						);

						// load up the album with metadata from the old album
						if (isset($album->name)) {
							$sourceData['title'] = $this->convertHtml($markupType, $album->name, $sourceEncoding);
							$newAlbumInstance->setTitle($sourceData['title']);
						}

						$newAlbumInstance->setKeywords('');

						if (isset($album->description)) {
							$sourceData['summary'] = $this->convertHtml(
								$markupType,
								$album->description,
								$sourceEncoding
							);
							$newAlbumInstance->setSummary($sourceData['summary']);
						}

						// Save the new album
						$ret = $newAlbumInstance->save();

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						$numberOfItemsImported++;

						$ret = $this->checkTruncation($newAlbumInstance, $sourceData);

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						$itemsCreated[] = $newAlbumInstance->getId();

						// album permissions
						$ret = GalleryCoreApi::removeItemPermissions($newAlbumInstance->getId());

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						if ($coreParams['id.adminGroup']) {
							$ret = GalleryCoreApi::addGroupPermission(
								$newAlbumInstance->getId(),
								$coreParams['id.adminGroup'],
								'core.all',
								false
							);

							if ($ret) {
								return $ret;
							}
						}

						list($ret, $themeId) = GalleryCoreApi::fetchThemeId($newAlbumInstance);

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						list($ret, $theme) = GalleryCoreApi::loadPlugin('theme', $themeId);

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						list($ret, $settings) = $theme->getSettings($newAlbumInstanceId);

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						if (isset($newAlbumInstanceId)) {
							$finishedAlbums[$album->albumId]            = $newAlbumInstanceId;
							$status['albumImportSuccess'][$album->name] = $newAlbumInstanceId;
						} else {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}
					}

					/*
					 * we can let the parent album (and anything else that we might
					 * have tied up) be edited by others now
					 */
					$ret = GalleryCoreApi::releaseLocks($importLockIds);

					if ($ret) {
						return $ret;
					}

					$importLockIds = array();

					$photos = $picturesToImport[$album->albumId];

					if (!isset($photos)) {
						$photos = array();
					}

					$albumHighlight[$newAlbumInstanceId] = false;

					list($ret, $item) = GalleryCoreApi::loadEntitiesById($newAlbumInstanceId);

					if ($ret) {
						return $ret;
					}

					list($ret, $importLockIds[]) = GalleryCoreApi::acquireReadLock($item->getId());

					if ($ret) {
						return $ret;
					}

					list($ret, $thumbnailTable) = GalleryCoreApi::fetchThumbnailsByItemIds(array($item->getId()));

					if ($ret) {
						GalleryCoreApi::releaseLocks($importLockIds);

						return $ret;
					}

					$albumNeedsThumbnail = empty($thumbnailTable) ? true : false;

					foreach ($photos as $j => $importAlbumItem) {
						$gallery->guaranteeTimeLimit(30);
						$dir      = $this->_cmdb->getAlbumPath() . $importAlbumItem->filepath;
						$filename = $importAlbumItem->filename;

						$filepath = $dir . $filename;

						$ret = $templateAdapter->
						updateProgressBar(
							$importPhotosMessage,
							$album->name . ' ' . $filename,
							$numberOfItemsImported / $totalItemsToImport
						);

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						list($base, $extension) = GalleryUtilities::getFileNameComponents($filename);

						list($ret, $mimeType) = GalleryCoreApi::convertExtensionToMime($extension);

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						$data = array(
							'title' => $this->convertHtml(
								$markupType,
								$importAlbumItem->title,
								$sourceEncoding
							),
							'description' => $this->convertHtml(
								$markupType,
								$importAlbumItem->description,
								$sourceEncoding
							),
						);

						list($ret, $newItem) = GalleryCoreApi::addItemToAlbum(
							$filepath,
							$filename,
							$data['title'],
							'',
							$data['description'],
							$mimeType,
							$item->getId()
						);

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						$itemsCreated[] = $newItem->getId();
						$numberOfItemsImported++;

						$ret = $this->checkTruncation($newItem, $data);

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						$itemLockIds = array();

						list($ret, $itemLockIds[]) = GalleryCoreApi::acquireWriteLock($newItem->getId());

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						$newItem->
						setKeywords(
							GalleryCoreApi::convertToUtf8(
								$importAlbumItem->keywords,
								$sourceEncoding
							)
						);

						if (isset($importAlbumItem->views) && $importAlbumItem->views > 0) {
							$ret = GalleryCoreApi::setItemViewCount(
								$newItem->getId(),
								$importAlbumItem->views
							);

							if ($ret) {
								GalleryCoreApi::releaseLocks($importLockIds);

								return $ret;
							}
						}

						$ret = $newItem->save();

						if ($ret) {
							GalleryCoreApi::releaseLocks($importLockIds);

							return $ret;
						}

						$ret = GalleryCoreApi::releaseLocks($itemLockIds);

						if ($ret) {
							return $ret;
						}

						$itemLockIds                    = array();
						$status['addFromLocalServer'][] = array(
							'fileName' => $filename,
							'id'       => $newItem->getId(),
						);
					}
				}

				$ret = $templateAdapter->updateProgressBar(
					$importPhotosMessage,
					$album->name,
					$numberOfItemsImported / $totalItemsToImport
				);

				if ($ret) {
					return $ret;
				}

				$album = null;

				// $i keeps track of how many passes we make over the data
				$i++;
			}
		}

		// Checkpoint to ensure items are saved to the DB in case of a later failure
		$ret = $storage->checkPoint();

		if ($ret) {
			return $ret;
		}

		foreach ($albumHighlight as $highlightRecipientId => $nameOfAlbumToHighlight) {
			if (isset($finishedAlbums[$nameOfAlbumToHighlight])
				&& $finishedAlbums[$nameOfAlbumToHighlight]
			) {
				list($ret, $item) = GalleryCoreApi::loadEntitiesById($highlightRecipientId);

				if ($ret) {
					return $ret;
				}

				list($ret, $importLockIds[]) = GalleryCoreApi::acquireReadLock($item->getId());

				if ($ret) {
					return $ret;
				}

				list($ret, $success) = GalleryCoreApi::setThumbnailFromItem(
					$item->getId(),
					$finishedAlbums[$nameOfAlbumToHighlight]
				);

				if ($ret) {
					GalleryCoreApi::releaseLocks($importLockIds);

					return $ret;
				}
			}
		}

		if (isset($importLockIds)) {
			$ret = GalleryCoreApi::releaseLocks($importLockIds);

			if ($ret) {
				return $ret;
			}

			$importLockIds = array();
		}

		// Checkpoint to ensure highlights are saved to the DB in case of a later failure
		$ret = $storage->checkPoint();

		if ($ret) {
			return $ret;
		}

		$templateAdapter->resetProgressBarStats();

		if (!empty($form['generateThumbnails']) && !connection_aborted()) {
			/*
			 * Get the thumbnails for all of the items we created, in 20 item
			 * increments so that we don't consume too much memory.
			 *
			 * Note: this destroys the $itemsCreated array!
			 */
			$createThumbsMessage = $module->translate('Creating thumbnails');
			$itemsCreatedCount   = count($itemsCreated);
			$thumbsCreated       = 0;

			while (!empty($itemsCreated)) {
				$chunk = array_splice($itemsCreated, 0, 20);

				list($ret, $thumbTable) = GalleryCoreApi::fetchThumbnailsByItemIds($chunk);

				if ($ret) {
					return $ret;
				}

				foreach ($thumbTable as $thumbnail) {
					list($ret, $newThumbnail, $wasRebuilt) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($thumbnail->getId());

					if ($ret) {
						return $ret;
					}

					$thumbsCreated++;

					$ret = $templateAdapter->updateProgressBar(
						$createThumbsMessage,
						$module->translate(
							array(
								'text' => 'Thumbnail %d of %d',
								'arg1' => $thumbsCreated,
								'arg2' => $itemsCreatedCount,
							)
						),
						$thumbsCreated / $itemsCreatedCount
					);

					if ($ret) {
						return $ret;
					}
				}

				GalleryDataCache::reset();
			}
		}

		// Checkpoint to ensure thumbnails are saved to the DB in case of a later failure
		$ret = $storage->checkPoint();

		if ($ret) {
			return $ret;
		}

		$session             =& $gallery->getSession();
		$redirect['view']    = 'core.SiteAdmin';
		$redirect['subView'] = 'cmmigrate.ImportCmResults';

		$redirect['statusId'] = $session->putStatus($status);
		$urlGenerator         =& $gallery->getUrlGenerator();
		$templateAdapter->completeProgressBar($urlGenerator->generateUrl($redirect));

		return null;
	}

	/**
	 * Convert html according to given markup type
	 * @param string markup type (none, bbcode, html)
	 * @param string input string
	 * @return string
	 */
	public function convertHtml($markupType, $item, $sourceEncoding = 'UTF-8') {
		$item = GalleryCoreApi::convertToUtf8($item, $sourceEncoding);

		switch ($markupType) {
			case 'bbcode':
				$item = ConfirmCmImportController::convertHtmlToBbcode($item);

				break;

			case 'html':
				break;

			case 'none':
			default:
				$item = strip_tags($item);
		}

		GalleryUtilities::sanitizeInputValues($item, false);

		return $item;
	}

	/**
	 * Converts HTML to BBCode
	 *
	 * @param string $item item to convert
	 * @return string
	 */
	public function convertHtmlToBbcode($item) {
		static $patterns, $match, $replace;

		if (!isset($patterns)) {
			$patterns = array(
				'#<br\s*(/?)>#i'      => "\n",
				'#<(/?)(b|strong)>#i' => '[${1}b]',
				'#<(/?)(i|em)>#i'     => '[${1}i]',
				'#<(/?)ul>#i'         => '[${1}list]',
				'#<li>#i'             => '[*]',
				'#</li>#i'            => '',
				'#<img[^>]*\s+src\s*=\s*["\']{0,1}([^\s"\'>]+)["\']{0,1}[^>]*>#is' => '[img]$1[/img]',
				'#<a[^>]*\s+href\s*=\s*["\']{0,1}([^\s"\'>]+)["\']{0,1}[^>]*>\s*(.*?)\s*</a>#is' => '[url=$1]$2[/url]',
			);

			$match   = array_keys($patterns);
			$replace = array_values($patterns);
		}

		$item = preg_replace($match, $replace, $item);

		return strip_tags($item);
	}

	/**
	 * Print message if any fields were truncated on imported item.
	 */
	public function checkTruncation(&$item, $sourceData) {
		global $gallery;

		static $titleMessage, $summaryMessage, $setMessage, $truncatedMessage;

		if (!isset($titleMessage)) {
			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'migrate');

			if ($ret) {
				return $ret;
			}

			$titleMessage     = $module->translate('%sTitle%s was truncated for %sitem %d%s.');
			$summaryMessage   = $module->translate('%sSummary%s was truncated for %sitem %d%s.');
			$setMessage       = $module->translate('Set:');
			$truncatedMessage = $module->translate('Truncated:');
		}

		$data = array(
			$item->getTitle(),
			$sourceData['title'],
			$titleMessage,
			$item->getSummary(),
			$sourceData['summary'],
			$summaryMessage,
		);

		while (!empty($data)) {
			list($value, $source, $message) = array_splice($data, 0, 3);

			if ($value != $source) {
				if (!isset($urlGenerator)) {
					$urlGenerator =& $gallery->getUrlGenerator();
				}

				printf(
					"<div class=\"gbBlock\">$message<br/>" .
					"<b>$setMessage</b> %s<br/><b>$truncatedMessage</b> %s</div>",
					'<b>',
					'</b>',
					'<a href="' . $urlGenerator->generateUrl(
						array(
							'view'       => 'core.ItemAdmin',
							'subView'    => 'core.ItemEdit',
							'itemId'     => $item->getId(),
							'editPlugin' => 'ItemEditItem',
						)
					) . '">',
					$item->getId(),
					'</a>',
					$value,
					substr($source, strlen($value))
				);
			}
		}

		return null;
	}

	/**
	 * Opens and initialises the Coppermine database
	 * @return object GalleryStatus a status code
	 */
	public function _openDatabase($copperminePath) {
		$this->_cmdb = new CmDatabaseTools($copperminePath);
		$ret         = $this->_cmdb->openDatabase();

		return $ret;
	}

	/**
	 * Get all the objects that could be considered to be albums from Coppermine
	 * @return array object GalleryStatus a status code
	 *               array of CmEntity keyed by integer albumId
	 */
	public function _getAlbums() {
		list($ret, $albums) = $this->_cmdb->getAlbums();

		if ($ret) {
			return array($ret, null);
		}

		foreach ($albums as $a) {
			$retval[$a->getAlbumId()] = $a;
		}

		return array(null, $retval);
	}
}

/**
 * Confirm import view
 */
class ConfirmCmImportView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$platform       =& $gallery->getPlatform();
		$existingAlbums = $titles = array();

		if (!isset($form['sourceAlbums'])) {
			$form['sourceAlbums'] = array();
		}

		$copperminePath     = GalleryUtilities::getRequestVariables('copperminePath');
		$destinationAlbumID = $form['destinationAlbumID'];

		list($ret, $albums) = $this->_getAlbums($copperminePath);

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $targetAlbum) = GalleryCoreApi::loadEntitiesById($destinationAlbumID);

		if ($ret) {
			return array($ret, null);
		}

		foreach ($form['sourceAlbums'] as $sourceAlbumId) {
			if (!in_array($sourceAlbumId, array_keys($albums))) {
				return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__), null);
			}

			$albumFields[$sourceAlbumId] = $albums[$sourceAlbumId];
		}

		$i = 0;

		if (!isset($albumFields)) {
			$albumFields = array();
		}

		while ($albumFields) {
			$existingAlbumId = false;
			$album           = array_shift($albumFields);

			$albumId   = $album->GetAlbumId();
			$albumName = $album->name;

			if (!in_array($album->getParent(), $form['sourceAlbums'])) {
				list($ret, $existingAlbumId) = GalleryCoreApi::fetchChildIdByPathComponent($destinationAlbumID, $albumName);
			}

			if ($ret) {
				if (!$ret->getErrorCode() & ERROR_MISSING_OBJECT) {
					return array($ret, null);
				}

				$existingAlbums[$albumId] = false;
			} else {
				$existingAlbums[$albumId] = $existingAlbumId;
			}

			$titles[$albumId] = $album->description;
			$names[$albumId]  = $albumName;
		}

		$ConfirmCmImport                       = array();
		$ConfirmCmImport['copperminePath']     = $copperminePath;
		$ConfirmCmImport['existingAlbums']     = $existingAlbums;
		$ConfirmCmImport['destinationAlbumID'] = $destinationAlbumID;
		$ConfirmCmImport['targetAlbum']        = (array)$targetAlbum;
		$ConfirmCmImport['names']              = $names;
		$ConfirmCmImport['titles']             = $titles;

		$template->setVariable('controller', 'cmmigrate.ConfirmCmImport');
		$template->setVariable('ConfirmCmImport', $ConfirmCmImport);

		return array(
			null,
			array(
				'body' => 'modules/cmmigrate/templates/ConfirmCmImport.tpl',
			),
		);
	}

	public function _getAlbums($copperminePath) {
		$cmdb = new CmDatabaseTools($copperminePath);

		$cmdb->openDatabase();

		list($ret, $albums) = $cmdb->getAlbums();

		if ($ret) {
			return array($ret, null);
		}

		foreach ($albums as $a) {
			$retval[$a->getAlbumId()] = $a;
		}

		return array(null, $retval);
	}
}
