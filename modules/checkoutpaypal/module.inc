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
 * @package CheckoutPaypal
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 *//**
 * Checkout plugin to process payments by email
 *
 * @package CheckoutPaypal
 */
class CheckoutPaypalModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('checkoutpaypal');
		$this->setName('Checkout by Paypal');
		$this->setDescription($gallery->i18n('Complete checkout by paypal'));
		$this->setVersion('0.4.0');
		$this->setGroup('commerce', $gallery->i18n('Commerce'));
		$this->setCallbacks('getSiteAdminViews');
		$this->setRequiredCoreApi(array(7, 4));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		// Register our plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutPaymentInterface_1_3',
			'CheckoutPaypalPaymentPlugin',
			'CheckoutPaypalPayment',
			'modules/checkoutpaypal/classes/CheckoutPaypalPaymentPlugin.class',
			'checkoutpaypal',
			null
		);

		if ($ret) {
			return $ret;
		}

		// Register the GalleryCheckoutPaypalIPN class
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEntity',
			'GalleryCheckoutPaypalIPN',
			'GalleryCheckoutPaypalIPN',
			'modules/checkoutpaypal/classes/GalleryCheckoutPaypalIPN.class',
			'checkoutpaypal',
			null
		);

		if ($ret) {
			return $ret;
		}

		// Register to add to emails
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutEmailInterface_1_0',
			'CheckoutPaypalEmailPlugin',
			'CheckoutPaypalEmail',
			'modules/checkoutpaypal/classes/CheckoutPaypalEmailPlugin.class',
			'checkoutpaypal',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 */
	public function needsConfiguration() {
		list($ret, $value) = $this->getParameter('paypalname');

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
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Checkout by Paypal'),
					'view' => 'checkoutpaypal.Admin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'checkoutpaypal.Admin';
	}

	/**
	 * @see GalleryModule::upgrade()
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		if (!empty($currentVersion)) {
			$storage =& $gallery->getStorage();

			switch ($currentVersion) {
				case '0.1.1':
				case '0.1.2':
				case '0.1.3':
				case '0.1.4':
				case '0.1.5':
				case '0.1.6':
				case '0.1.7':
				case '0.1.8':
				case '0.1.9':
				case '0.1.10':
				case '0.1.11':
					$ret = GalleryCoreApi::unregisterFactoryImplementationsByModuleId('checkoutpaypal');

					if ($ret) {
						return $ret;
					}
					$this->performFactoryRegistrations();
					// Fall Through
				case '0.1.11.1':
				case '0.1.12':
				case '0.2.0':
				case '0.2.1':
				case '0.3.0':
				case '0.3.1':
					// unused parameter
					$ret = GalleryCoreApi::removePluginParameter('module', 'checkoutemail', 'countrycode');

					if ($ret) {
						return $ret;
					}
					// Fall Through
				case '0.3.2':
				case '0.3.3':
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

		return null;
	}
}
