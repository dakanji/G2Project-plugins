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
 *//**
 * FIXME The implementation of the core-module
 *
 * Strictly, this module is separate from the CalDAV module.  However it is designed with CalDAV in
 * mind.  ICalendarResources will be written & read using CalDAV, and it is far easier to composit
 * ICalendarResources than to break them up for CalDAV
 *
 * @package ICalendar
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 1.1.1.1 $ $Date: 2006/04/05 18:40:13 $
 */
class ICalendarModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('icalendar');
		$this->setName($gallery->i18n('iCalendar'));
		$this->setDescription(
			$gallery->i18n(
				'FIXME iCalendar format'
			)
		);

		$this->setVersion('1.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('misc', $gallery->i18n('Miscellaneous'));
		$this->setCallbacks('getSiteAdminViews');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new ICalendarModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::activate
	 */
	public function activate($postActivationEvent = true) {
		$ret = GalleryCoreApi::addMimeType('ics', 'text/calendar', false);

		if ($ret && !($ret->getErrorCode() & ERROR_COLLISION)) {
			return array($ret, null);
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
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->getName(),
					'view' => 'icalendar.ICalendarSiteAdmin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEntity',
			'ICalendarResourceItem',
			'ICalendarResourceItem',
			'modules/icalendar/classes/ICalendarResourceItem.class',
			$this->getId(),
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryItem',
			'ICalendarResourceItem',
			'ICalendarResourceItem',
			'modules/icalendar/classes/ICalendarResourceItem.class',
			$this->getId(),
			array('text/calendar')
		);

		if ($ret) {
			return $ret;
		}
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion) {
		switch ($currentVersion) {
			case null:
				// Initial install

			case '0.0.1':
				// FIXME Added ICalendarResourceItem & ICalendarResourceMap

			case 'end of upgrade path':
				break;

			default:
				return GalleryCoreApi::error(
					ERROR_BAD_PLUGIN,
					__FILE__,
					__LINE__,
					sprintf('Unknown module version %s', $currentVersion)
				);
		}
	}
}
