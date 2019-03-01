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
 * CheckoutGroups module
 *
 * @version $Revision: 1644 $
 * @package CheckoutGroups
 * @author Alec Myers <alec@alecmyers.com>
 */
class CheckoutGroupsModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('checkoutgroups');
		$this->setName($gallery->i18n('Checkout Groups'));
		$this->setDescription(
			$gallery->i18n(
				'Adds purchasers to configured groups'
			)
		);

		$this->setVersion('1.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('commerce', $gallery->i18n('Commerce'));
		$this->setCallbacks('registerEventListeners');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new CheckoutGroupsModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::upgrade()
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		if (!empty($currentVersion)) {
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
		}

		return null;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		// Register our plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutCustomPageInterface_1_0',
			'CheckoutGroupsPlugin',
			'CheckoutGroups',
			'modules/checkoutgroups/classes/CheckoutGroupsPlugin.class',
			'checkoutgroups',
			null
		);

		if ($ret) {
			return $ret;
		}
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutStatusPageInterface_1_0',
			'CheckoutGroupsStatusPlugin',
			'CheckoutGroupsStatus',
			'modules/checkoutgroups/classes/CheckoutGroupsStatusPlugin.class',
			'checkoutgroups',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutProductListInterface_1_0',
			'CheckoutGroupsProductPlugin',
			'CheckoutGroupsProduct',
			'modules/checkoutgroups/classes/CheckoutGroupsProductPlugin.class',
			'checkoutGroups',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	public function registerEventListeners() {
		$myListener = new CheckoutGroupsModule();
		GalleryCoreApi::registerEventListener('GalleryCheckout::transactionChange', $myListener);
		GalleryCoreApi::registerEventListener('GalleryEntity::delete', $myListener);
	}

	public function handleEvent($event) {
		$result = null;

		if ($event->getEventName() == 'GalleryEntity::delete') {
			$entity = $event->getEntity();

			if (GalleryUtilities::isA($entity, 'GalleryGroup')) {
				$ret = GalleryCoreApi::removeMapEntry(
					'CheckoutProductMap',
					array(
						'paramName'  => 'groupId',
						'paramValue' => $entity->getId(),
					)
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			return array(null, null);
		}

		if ($event->getEventName() == 'GalleryCheckout::transactionChange') {
			GalleryCoreApi::requireOnce('modules/checkoutgroups/classes/CheckoutGroupsEventHelper.class');
			$ret = CheckoutGroupsEventHelper::handleTransactionChange($event);

			if ($ret) {
				return array($ret, null);
			}

			return array(null, $result);
		}
	}
}
