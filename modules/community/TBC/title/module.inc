<?php

/*
 * $RCSfile: module.inc,v $
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
 *
 * @package title
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1420 $
 */
class TitleModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('title');
		$this->setName($gallery->i18n('Title Search'));
		$this->setDescription(
			$gallery->i18n(
				'Dynamic albums of items whose titles match given search strings'
			)
		);
		$this->setVersion('1.0.0'); // Update upgrade() as required
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
			$instance = new TitleModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Title Search'),
					'view' => 'title.TitleSiteAdmin',
				),
			),
		);
	}

	public function upgrade($currentVersion) {
		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'title');

		if ($ret) {
			return $ret;
		}

		if (!isset($params['themeSettingsId'])) {
			list($ret, $entity) = GalleryCoreApi::newFactoryInstance(
				'GalleryEntity',
				'GalleryEntity'
			);

			if ($ret) {
				return $ret;
			}

			if (!isset($entity)) {
				return GalleryCoreApi::error(
					ERROR_MISSING_OBJECT,
					__FILE__,
					__LINE__,
					'Unable to get a GalleryEntity instance'
				);
			}

			$ret = $entity->create();

			if ($ret) {
				return $ret;
			}

			$ret = $entity->save(false);

			if ($ret) {
				return $ret;
			}

			$ret = $this->setParameter('themeSettingsId', $entity->getId());

			if ($ret) {
				return $ret;
			}
		}

		return null;
	}

	public function getRewriteRules() {
		return array(
			array(
				'comment'  => $this->translate('Titles Begin'),
				'match'    => array(
					'view' => 'title.Begin',
				),
				'pattern'  => 'begin/%chars%',
				'keywords' => array(
					'chars' => array(
						'pattern' => '([^/?]+)',
						'ignored' => 0,
					),
				),
				'help'     => $this->translate(
					'Short URL for dynamic album of items whose names '
						. 'begin with a sequence of characters'
				),
			),
			array(
				'comment'  => $this->translate('Titles Contain'),
				'match'    => array(
					'view' => 'title.Contain',
				),
				'pattern'  => 'contain/%chars%',
				'keywords' => array(
					'chars' => array(
						'pattern' => '([^/?]+)',
						'ignored' => 0,
					),
				),
				'help'     => $this->translate(
					'Short URL for dynamic album of items whose names '
						. 'contain a sequence of characters'
				),
			),
		);
	}
}
