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
 * @package PanoramaJs
 * @author shivaya <shivaya>at<users.sourceforge.net>
 * @author javascript viewer : Henri ASTRE
 * @version $Revision: 16034 $
 */
class PanoramaJsModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('panoramajs');
		$this->setName($gallery->i18n('Panorama (Js)'));
		$this->setDescription(
			$gallery->i18n(
				'View wide jpeg/gif images in a javascript panorama viewer'
			)
		);
		$this->setVersion('2.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;
		$this->setGroup('display', $gallery->i18n('Display'));
		$this->setCallbacks('getSiteAdminViews|getItemLinks');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new PanoramaJsModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion) {
		if (!isset($currentVersion)) {
			// Initial install
			foreach (array(
				'itemType' => 1,
				'itemLink' => 1,
				'width'    => 800,
				'rotate'   => 1,
				'speed'    => 1,
			) as $key => $value) {
				$ret = $this->setParameter($key, $value);

				if ($ret) {
					return $ret;
				}
			}
		} else {
			if (version_compare($currentVersion, '1.0.6', '<')) {
				GalleryCoreApi::requireOnce(
					'modules/panoramajs/classes/PanoramaJsUpgradeHelper.class'
				);

				$ret = PanoramaJsUpgradeHelper::revertEntities();

				if ($ret) {
					return $ret;
				}
			}
		}

		return null;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryRenderer',
			'PanoramaJsRenderer',
			'PanoramaJsRenderer',
			'modules/panoramajs/classes/PanoramaJsRenderer.class',
			'panoramajs',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemEditOption',
			'PanoramaJsOption',
			'PanoramaJsOption',
			'modules/panoramajs/PanoramaJsOption.inc',
			'panoramajs',
			array('ItemEditPhoto')
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Panorama (Js)'),
					'view' => 'panoramajs.PanoramaJsSiteAdmin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getItemLinks
	 */
	public function getItemLinks($items, $wantsDetailedLinks, $permissions) {
		$links = array();

		list($ret, $param) = GalleryCoreApi::fetchAllPluginParameters('module', 'panoramajs');

		if ($ret) {
			return array($ret, null);
		}

		if ($param['itemLink']) {
			GalleryCoreApi::requireOnce('modules/panoramajs/classes/PanoramaJsHelper.class');

			/*
			 * This won't obey the acting user id, but that's ok because it's only used to
			 * determine if a panorama is even possible.  We'll still also gate on the acting
			 * user's permissions that we receive.
			 */
			list($ret, $images) = PanoramaJsHelper::fetchViewableImages($items);

			if ($ret) {
				return array($ret, null);
			}

			foreach ($items as $item) {
				$itemId = $item->getId();

				if (!isset($images[$itemId])) {
					continue;
				}

				$image    = $images[$itemId];
				$mimeType = $item->getMimeType();
				$width    = $image->getWidth();

				// Applet supports jpeg and gif images
				if (GalleryUtilities::isA($item, 'GalleryPhotoItem')
					&& $item->getRenderer() != 'PanoramaJsRenderer'
					&& preg_match(':^image/(gif|p?jpeg)$:', $mimeType)
					&& $width > $param['width']
					&& ($image->getHeight() / $width < 0.6)
				) {
					$links[$itemId][] = array(
						'text'   => $this->translate('View Panorama (Js)'),
						'params' => array(
							'view'   => 'panoramajs.PanoramaJs',
							'itemId' => $itemId,
						),
					);
				}
			}
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::getRewriteRules
	 */
	public function getRewriteRules() {
		$rules           = array();
		$rule            = array();
		$rule['comment'] = $this->translate('View Panorama (Js)');
		$rule['match']   = array(
			'view' => 'panoramajs.PanoramaJs',
		);

		$rule['onLoad']   = array('rewrite', 'RewriteSimpleHelper', 'loadItemIdFromPath');
		$rule['pattern']  = 'v/%path%/panoramaJs.html';
		$rule['keywords'] = array(
			'path' => array(
				'pattern'  => '([^?]+)',
				'help'     => $this->translate('Short URL for viewer panorama items'),
				'function' => array('rewrite', 'RewriteSimpleHelper', 'parsePath'),
				'help'     => $this->translate(
					'Path to an item (eg, /album/image.jpg.html)'
				),
			),
		);

		$rules[] = $rule;

		return $rules;
	}

	/**
	 * @see GalleryPlugin::uninstall
	 */
	public function uninstall() {
		$ret = GalleryCoreApi::deleteRenderer('PanoramaJsRenderer');

		if ($ret) {
			return $ret;
		}

		return parent::uninstall();
	}
}
