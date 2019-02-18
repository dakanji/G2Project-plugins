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
 * @package ContactOwner
 * @author Darren Guiden <g2@pengs.co.uk>
 *//**
 * Contact the owner by filling in a PHP form
 *
 * @package ContactOwner
 */
class ContactOwnerModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('contactowner');
		$this->setName('Contact Owner');
		$this->setDescription($gallery->i18n('Contact gallery owner by email'));
		$this->setVersion('0.2.4');
		$this->setGroup('display', $gallery->i18n('Display'));
		$this->setCallbacks('getSystemLinks|getSiteAdminViews');
		$this->setRequiredCoreApi(array(7, 2));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 */
	public function needsConfiguration() {
		list($ret, $value) = $this->getParameter('toEmailAddress');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, empty($value));
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Contact Owner'),
					'view' => 'contactowner.Admin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getSystemLinks
	 */
	public function getSystemLinks() {
		return array(
			null,
			array(
				'Contact' => array(
					'text'   => $this->translate('Contact'),
					'params' => array(
						'view' => 'contactowner.Contact',
					),
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'contactowner.Admin';
	}

	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CaptchaAdminOption',
			'contactownerCaptchaAdminOption',
			'contactownerCaptchaAdminOption',
			'modules/contactowner/classes/contactownerCaptchaAdminOption.class',
			'contactowner',
			null
		);

		if ($ret) {
			return $ret;
		}
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		// We need configuration so return false
		return array(null, false);
	}

	/**
	 * @see GalleryModule::getRewriteRules
	 */
	public function getRewriteRules() {
		return array(
			array(
				'comment' => $this->translate('Contact'),
				'match'   => array(
					'view' => 'contactowner.Contact',
				),
				'pattern' => 'contact',
				'help'    => $this->translate('Contact'),
			),
		);
	}
}
