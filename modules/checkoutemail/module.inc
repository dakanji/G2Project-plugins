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
 * @package CheckoutEmail
 * @version $Revision: 1583 $
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 *
 *//**
 * Checkout plugin to process payments by email
 *
 * @package CheckoutEmail
 */
class CheckoutEmailModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('checkoutemail');
		$this->setName($gallery->i18n('Checkout by Email'));
		$this->setDescription($gallery->i18n('Complete checkout by email'));
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
			'CheckoutEmailPaymentPlugin',
			'CheckoutEmailPayment',
			'modules/checkoutemail/classes/CheckoutEmailPaymentPlugin.class',
			'checkoutemail',
			null
		);

		if ($ret) {
			return $ret;
		}
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutEmailInterface_1_0',
			'CheckoutEmailEmailPlugin',
			'checkoutEmailEmail',
			'modules/checkoutemail/classes/CheckoutEmailEmailPlugin.class',
			'checkoutemail',
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
		list($ret, $value) = $this->getParameter('paymentText');

		if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
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
					'name' => $this->translate('Checkout by Email'),
					'view' => 'checkoutemail.Admin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'checkoutemail.Admin';
	}

	/**
	 * @see GalleryModule::upgrade()
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		if (!empty($currentVersion)) {
			$storage =& $gallery->getStorage();

			switch ($currentVersion) {
				case '0.0.1':
				case '0.0.2':
				case '0.0.3':
				case '0.0.4':
				case '0.0.5':
				case '0.0.6':
				case '0.0.7':
				case '0.0.8':
					$ret = GalleryCoreApi::unregisterFactoryImplementationsByModuleId('checkoutemail');

					if ($ret) {
						return $ret->wrap(__FILE__, __LINE__);
					}
					$this->performFactoryRegistrations();
					// Fall Through
				case '0.0.9':
				case '0.0.9.1':
				case '0.0.10':
				case '0.2.0':
				case '0.2.1':
				case '0.3.0':
				case '0.3.1':
					// remove old parameters
					list($ret, $numAddresses) = GalleryCoreApi::getPluginParameter('module', 'checkoutemail', 'numAddresses');

					if ($ret) {
						return array($ret, null);
					}

					for ($i = 0; $i < $numAddresses; $i++) {
						$ret = GalleryCoreApi::removePluginParameter('module', 'checkoutemail', 'toAddress' . $i);

						if ($ret) {
							return $ret;
						}
						$ret = GalleryCoreApi::removePluginParameter('module', 'checkoutemail', 'showCustomer' . $i);

						if ($ret) {
							return $ret;
						}
						$ret = GalleryCoreApi::removePluginParameter('module', 'checkoutemail', 'showThumbs' . $i);

						if ($ret) {
							return $ret;
						}
						$ret = GalleryCoreApi::removePluginParameter('module', 'checkoutemail', 'showLinks' . $i);

						if ($ret) {
							return $ret;
						}
					}
					$ret = GalleryCoreApi::removePluginParameter('module', 'checkoutemail', 'numAddresses');

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
