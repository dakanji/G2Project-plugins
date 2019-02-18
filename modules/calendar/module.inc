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
 * @package Calendar
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 847 $
 */
class CalendarModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('calendar');
		$this->setName($gallery->i18n('Calendar'));
		$this->setVersion('0.0.2');
		$this->setDescription($gallery->i18n('FIXME An Ajax Calendar'));
		$this->setGroup('misc', $gallery->i18n('Miscellaneous'));
		$this->setCallbacks('getSiteAdminViews|getUserAdminViews');
		$this->setRequiredCoreApi(array(7, 2));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * Load as Singleton
	 */
	public static function doStatic() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new CalendarModule();
		}

		return $singleton;
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
					'view' => 'calendar.CalendarSiteAdmin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getUserAdminViews
	 */
	public function getUserAdminViews($user) {
		if (GalleryCapabilities::can('login')) {
			list($ret, $anonymousUserId) = GalleryCoreApi::getAnonymousUserId();

			if ($ret) {
				return array($ret, null);
			}

			if ($user->getId() != $anonymousUserId) {
				return array(
					null,
					array(
						array(
							'name' => $this->translate('Calendar Settings'),
							'view' => 'calendar.CalendarUserAdmin',
						),
					),
				);
			}
		}
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEntity',
			'CalendarEvent',
			'CalendarEvent',
			'modules/calendar/classes/CalendarEvent.class',
			$this->getId(),
			null
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
				// FIXME Added CalendarEvent

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
