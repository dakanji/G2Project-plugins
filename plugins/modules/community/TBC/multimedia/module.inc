<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2012 Bharat Mediratta
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
 * The implementation of the Multimedia module
 * @package Multimedia
 * @author Wayne Patterson <suprsidr@flashyourweb.com>
 * @version $Revision: 17580 $
 */
class MultimediaModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('multimedia');
		$this->setName($gallery->i18n('Multimedia'));
		$this->setDescription(
			$gallery->i18n(
				'Enable inline display of audio/video files'
			)
		);
		$this->setVersion('2.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('display', $gallery->i18n('Display'));
		$this->setCallbacks('getSiteAdminViews');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new MultimediaModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryRenderer',
			'MultimediaRenderer',
			'MultimediaRenderer',
			'modules/multimedia/classes/MultimediaRenderer.class',
			'multimedia',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemAddOption',
			'MultimediaOption',
			'MultimediaOption',
			'modules/multimedia/MultimediaOption.inc',
			'multimedia',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::activate
	 */
	public function activate($postActivationEvent = true) {
		global $gallery;

		$storage =& $gallery->getStorage();

		// Add renderer our renderer to the mime types that need it
		$query = "
			SELECT
				[GalleryDataItem::id]
			FROM
				[GalleryDataItem]
			WHERE
				[GalleryDataItem::mimeType]
			LIKE
				'video/%'
			OR
				[GalleryDataItem::mimeType]
			LIKE
				'audio/%'
		";

		list($ret, $searchResults) = $gallery->search($query);

		if ($ret) {
			return array($ret, null);
		}

		$ids = array();

		while ($result = $searchResults->nextResult()) {
			$ids[] = (int)$result[0];
		}

		while (!empty($ids)) {
			$itemIds = array_splice($ids, 0, 100);

			list($ret, $items) = GalleryCoreApi::loadEntitiesById($itemIds, 'GalleryDataItem');

			if ($ret) {
				return array($ret, null);
			}

			foreach ($items as $item) {
				if (preg_match('/video/', $item->getMimeType())
					|| preg_match('/audio/', $item->getMimeType())
				) {
					list($ret, $lockId) = GalleryCoreApi::acquireWriteLock($item->getId());

					if ($ret) {
						return array($ret, null);
					}

					list($ret, $item) = $item->refresh();

					if ($ret) {
						GalleryCoreApi::releaseLocks($lockId);

						return array($ret, null);
					}

					$item->setRenderer('MultimediaRenderer');

					$ret = $item->save();

					if ($ret) {
						GalleryCoreApi::releaseLocks($lockId);

						return array($ret, null);
					}

					$ret = GalleryCoreApi::releaseLocks($lockId);

					if ($ret) {
						return array($ret, null);
					}
				}
			}

			$ret = $storage->checkPoint();

			if ($ret) {
				return array($ret, null);
			}
		}

		list($ret, $redirect) = parent::activate($postActivationEvent);

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $redirect);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */

	/*
	 *public function getSiteAdminViews() {
	 *    return array(
	 *		null,
	 *		array(
	 *			array(
	 *				'name'=>$this->translate('Multimedia'),
	 *				'view'=>'multimedia.AdminMultimedia'
	 *			)
	 *		)
	 *	);
	 *}
	 */

	/**
	 * @see GalleryModule::getRewriteRules
	 */
	public function getRewriteRules() {
		return array(
			array(
				'comment'  => $this->translate(
					'Multimedia'
				),
				'match'    => array(
					'view' => 'multimedia.DownloadItem',
				),
				'pattern'  => 'f/%itemId%-%serialNumber%/%fileName%',
				'keywords' => array(
					'serialNumber' => array(
						'pattern'  => '([0-9]+)',
						'help'     => $this->translate(
							'Ensures browsers do not use cached version when image has changed'
						),
						'function' => array(
							'rewrite',
							'RewriteSimpleHelper',
							'parseSerialNumber',
						),
					),
					'fileName' => array(
						'pattern'  => '([^/?]+)',
						'help'     => $this->translate('The item file name.'),
						'function' => array(
							'rewrite',
							'RewriteSimpleHelper',
							'parseFileName',
						),
					),
				),
				'help'     => $this->translate('The general URL for downloading items.'),
			),
		);
	}

	/**
	 * @see GalleryPlugin::uninstall
	 */
	public function uninstall() {
		$ret = GalleryCoreApi::deleteRenderer('MultimediaRenderer');

		if ($ret) {
			return $ret;
		}

		return parent::uninstall();
	}
}
