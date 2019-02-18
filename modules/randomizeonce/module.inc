<?php
/*
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
 *
 * @package RandomizeOnce
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1529 $
 *//**
 * Add links to the header menu.
 *
 */
class RandomizeOnceModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('randomizeonce');
		$this->setName('Randomize Once');
		$this->setDescription($gallery->i18n('Randomize Sort Order'));
		$this->setVersion('7.0.0');
		$this->setGroup('display', $gallery->i18n('Display'));
		$this->setCallbacks('getItemLinks|getItemAdminViews');
		$this->setRequiredCoreApi(array(7, 10));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * Load as Singleton
	 */
	public static function doStatic() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new RandomizeOnceModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::getItemLinks
	 */
	public function getItemLinks($items, $wantsDetailedLinks, $permissions) {
		global $gallery;
		$links = array();

		foreach ($items as $item) {
			$itemId = $item->getId();

			if ($item->getCanContainChildren() && isset($permissions[$itemId]['core.edit'], $wantsDetailedLinks[$itemId])) {
				$links[$itemId][] = array(
					'text'   => $this->translate('Randomize Display Order'),
					'params' => array(
						'view'    => 'core.ItemAdmin',
						'subView' => 'randomizeonce.Randomize',
						'itemId'  => $itemId,
						'return'  => 1,
					),
				);
			}
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::getItemAdminViews
	 */
	public function getItemAdminViews($item) {
		global $gallery;

		$views                   = array();
		list($ret, $permissions) = GalleryCoreApi::getPermissions($item->getId());

		if ($ret) {
			return array($ret, null);
		}

		if ($item->getCanContainChildren() && isset($permissions['core.edit'])) {
			$views[] = array(
				'name' => $this->_translate('Randomize Display Order'),
				'view' => 'randomizeonce.Randomize',
			);
		}

		return array(null, $views);
	}
}
