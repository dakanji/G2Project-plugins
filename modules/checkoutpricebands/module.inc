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
 * CheckoutPriceBands module
 *
 * @version $Revision: 1505 $
 * @package CheckoutGroups
 * @author Alec Myers <alec@alecmyers.com>
 */
class CheckoutPriceBandsModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('checkoutpricebands');
		$this->setName($gallery->i18n('Checkout Price Bands'));
		$this->setDescription($gallery->i18n('Offer different prices according to quantity'));
		$this->setVersion('0.1.0');
		$this->setGroup('commerce', $gallery->i18n('Commerce'));
		$this->setCallbacks('');
		$this->setRequiredCoreApi(array(7, 10));
		$this->setRequiredModuleApi(array(3, 2));
	}

	/**
	 * Load as Singleton
	 */
	public static function doStatic() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new CheckoutPriceBandsModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		// Register our plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutCustomPageInterface_1_0',
			'CheckoutPriceBandsCustomPlugin',
			'checkoutpricebandscustom',
			'modules/checkoutpricebands/classes/CheckoutPriceBandsCustomPlugin.class',
			'checkoutpricebands',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutProductListInterface_1_0',
			'CheckoutPriceBandsProductPlugin',
			'checkoutpricebandsproduct',
			'modules/checkoutpricebands/classes/CheckoutPriceBandsProductPlugin.class',
			'checkoutpricebands',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutTransactionDetailsInterface_1_1',
			'CheckoutPriceBandsTransactionPlugin',
			'checkoutpricebandstransaction',
			'modules/checkoutpricebands/classes/CheckoutPriceBandsTransactionPlugin.class',
			'checkoutpricebands',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}
}
