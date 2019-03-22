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
 * DownloadLink module
 *
 * Provides downloading of binary items as files
 * @version $Revision: 1654 $
 * @package DownloadLink
 * @author Alec Myers <alec@alecmyers.com>
 */
class DownloadLinkModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('downloadlink');
		$this->setName($gallery->i18n('Download Link'));
		$this->setDescription(
			$gallery->i18n(
				'Provides a link to download a picture'
			)
		);

		// Update upgrade() as required
		$this->setVersion('1.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('export', $gallery->i18n('Export'));
		$this->setCallbacks('getItemLinks');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new DownloadLinkModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::getItemLinks()
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		if (!empty($currentVersion)) {
			$storage =& $gallery->getStorage();

			switch ($currentVersion) {
				case '0.1.0':
					break;

				default:
					return GalleryCoreApi::error(
						ERROR_BAD_PLUGIN,
						__FILE__,
						__LINE__,
						sprintf('Unknown module version %s', $currentVersion)
					);
			}
		} else {
			// Initial install....
			list($ret, $coreParams) = GalleryCoreApi::fetchAllPluginParameters('module', 'core');

			if ($ret) {
				return $ret;
			}

			// register permissions
			$permissions[] = array('original', $gallery->i18n('[downloadlink] Show Download Original Link'), 0, array());
			$permissions[] = array('resizes', $gallery->i18n('[downloadlink] Show Download Resize(s) Links'), 0, array());
			$permissions[] = array('thumbnail', $gallery->i18n('[downloadlink] Show Download Thumbnail Link'), 0, array());

			foreach ($permissions as $p) {
				$ret = GalleryCoreApi::registerPermission($this->getId(), 'downloadlink.' . $p[0], $p[1], $p[2], $p[3]);

				if ($ret) {
					return $ret;
				}
			}

			// Give everybody download permissions by default
			$ret = GalleryCoreApi::addGroupPermission($coreParams['id.rootAlbum'], $coreParams['id.everybodyGroup'], 'downloadlink.original', true);

			if ($ret) {
				return $ret;
			}

			$ret = GalleryCoreApi::addGroupPermission($coreParams['id.rootAlbum'], $coreParams['id.everybodyGroup'], 'downloadlink.resizes', true);

			if ($ret) {
				return $ret;
			}
		}

		return null;
	}

	public function getItemLinks($items, $wantsDetailedLinks, $permissions) {
		$links = array();

		foreach ($items as $item) {
			if (!GalleryUtilities::isA($item, 'GalleryPhotoItem')) {
				continue;
			}

			$masterId        = $item->getId();
			$potentialImages = array();

			// try thumbnail
			$ret = GalleryCoreApi::assertHasItemPermission($masterId, 'downloadlink.thumbnail');

			if (!$ret) {
				list($ret, $thumbs) = GalleryCoreApi::fetchThumbnailsByItemIds(array($masterId));

				if ($ret) {
					return array($ret, null);
				}

				if (!empty($thumbs[$masterId])) {
					list($ret, $result) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($thumbs[$masterId]->getId());

					if ($ret) {
						return array($ret, null);
					}

					$potentialImages[] = $result;
				}
			}

			// resizes
			$ret = GalleryCoreApi::assertHasItemPermission($masterId, 'downloadlink.resizes');

			if (!$ret) {
				list($ret, $resizes) = GalleryCoreApi::fetchResizesByItemIds(array($masterId));

				if ($ret) {
					return array($ret, null);
				}

				if (!empty($resizes[$masterId])) {
					foreach ($resizes[$masterId] as $resize) {
						list($ret, $result) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($resize->getId());

						if ($ret) {
							return array($ret, null);
						}

						$potentialImages[] = $result;
					}
				}
			}

			// How about the original?
			$includeOriginal = false;

			$ret = GalleryCoreApi::assertHasItemPermission($masterId, 'downloadlink.original');

			if (!$ret) {
				// fetch preferred
				list($ret, $preferreds) = GalleryCoreApi::fetchPreferredsByItemIds(array($masterId));

				if ($ret) {
					return array($ret, null);
				}

				if (isset($preferreds[$masterId])) {
					list($ret, $result) = GalleryCoreApi::rebuildDerivativeCacheIfNotCurrent($preferreds[$masterId]->getId());

					if ($ret) {
						return array($ret, null);
					}

					$potentialImages[] = $result;
				} else {
					$potentialImages[] = $item;
				}
			}

			$itemTypeNames = $item->itemTypeName();

			foreach ($potentialImages as $potentialImage) {
				$links[$masterId][] = array(
					'text' => $this->translate(
						array(
							'text' => 'Download %s (%s x %s)',
							'arg1' => $itemTypeNames[0],
							'arg2' => $potentialImage->getWidth(),
							'arg3' => $potentialImage->getHeight(),
						)
					),
					'params' => array(
						'view'   => 'downloadlink.OfferDownload',
						'itemId' => $potentialImage->getId(),
					),
				);
			}
		}

		return array(null, $links);
	}
}
