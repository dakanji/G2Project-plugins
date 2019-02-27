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
 * Simpleviewer Source module
 *
 * A module to interface Gallery2 with the Simpleviewer flash viewer
 * @package SimpleviewerSource
 * @version $Revision: 1433 $
 * @author Alec Myers <alec@alecmyers.com>
 */
class SimpleviewerSourceModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('simpleviewersource');
		$this->setName($gallery->i18n('Simpleviewer Source'));
		$this->setDescription(
			$gallery->i18n(
				'Outputs xml for Simpleviewer'
			)
		);

		$this->setVersion('1.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('export', $gallery->i18n('Export'));
		$this->setCallbacks('registerEventListeners|getItemLinks');
	}

	/**
	 * Load as Singleton
	 */
	public static function getMe() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new SimpleviewerSourceModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 */
	public function needsConfiguration() {
		list($ret, $value) = $this->getParameter('configured');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, empty($value));
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		list($ret, $needsConfiguration) = $this->needsConfiguration();

		if ($ret) {
			return array($ret, null);
		}

		return array(null, !$needsConfiguration);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'simpleviewersource.Configure';
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {

		// Register our item edit plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemEditPlugin',
			'SimpleviewerSourceItemEdit',
			'SimpleviewerSourceItemEdit',
			'modules/simpleviewersource/SimpleviewerSourceItemEdit.inc',
			'simpleviewersource',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::getItemLinks()
	 */
	public function getItemLinks($items, $wantsDetailedLinks, $permissions) {
		global $gallery;
		GalleryCoreApi::requireOnce('modules/simpleviewersource/classes/SimpleviewerParameterMapHelper.class');

		$links = array();

		foreach ($items as $item) {
			$itemId = $item->getId();

			if (GalleryUtilities::isA($item, 'GalleryAlbumItem')) {
				list($ret, $parameters) = SimpleviewerParameterMapHelper::fetchParams($itemId, 'private');

				if ($ret) {
					return array($ret, null);
				}

				if ($parameters['offerSlideshow'] == '1') {
					list($ret, $showItemIds) = GalleryCoreApi::fetchChildItemIdsWithPermission($itemId, 'core.view');

					if ($ret) {
						return array($ret, null);
					}
					// look for photo items before offering a slideshow
					foreach ($showItemIds as $showItemId) {
						list($ret, $showItem) = GalleryCoreApi::loadEntitiesByID($showItemId);

						if ($ret) {
							return array($ret, null);
						}

						if (GalleryUtilities::isA($showItem, 'GalleryPhotoItem')) {
							$links[$itemId][] = array(
								'text' => $this->translate(
									array(
										'text' => 'Slideshow',
									)
								),
								'params' => array(
									'view'   => 'simpleviewersource.Slideshow',
									'itemId' => $itemId,
								),
							);

							break; //one is enough
						}
					}
				}
			}
		}

		return array(null, $links);
	}

	public function getRewriteRules() {
		return array(
			array(
				'comment'  => $this->translate('Simpleviewer Slideshow'),
				'match'    => array(
					'view' => 'simpleviewersource.Slideshow',
				),
				'pattern'  => 'show/%itemId%',
				'keywords' => array(
					'itemId' => array(
						'pattern' => '([^?]+)',
						'ignored' => 0,
					),
				),
				'help'     => $this->translate('Short URL for Simpleviewer Slideshow'),
			),
		);
	}

	/**
	 * @see GalleryModule::registerEventListeners
	 */
	public function registerEventListeners() {
		GalleryCoreApi::registerEventListener('GalleryEntity::delete', new SimpleviewerSourceModule());
	}

	/**
	 * Event Handler: get rid of any parameters for albums that are deleted
	 *
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		$data = $event->getData();

		if ($event->getEventName() == 'GalleryEntity::delete') {
			$entity = $event->getEntity();

			if (GalleryUtilities::isA($entity, 'GalleryAlbumItem')) {
				GalleryCoreApi::requireOnce('modules/simpleviewersource/classes/SimpleviewerParameterMapHelper.class');
				list($ret, $paramIds) = SimpleviewerParameterMapHelper::fetchParamIdsForAlbum($entity->getId());

				if ($ret) {
					return array($ret, null);
				}

				foreach ($paramIds as $paramId) {
					$ret = SimpleviewerParameterMapHelper::deleteParam($paramId);

					if ($ret) {
						return array($ret, null);
					}
				}
			}
		}

		return array(null, null);
	}
}
