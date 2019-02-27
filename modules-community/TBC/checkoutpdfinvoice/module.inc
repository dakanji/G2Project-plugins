<?php
/*
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
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * CheckoutPDFInvoice module
 *
 *
 * @package CheckoutPDFInvoice
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1700 $
 */
class CheckoutPDFInvoiceModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('checkoutpdfinvoice');
		$this->setName($gallery->i18n('Checkout PDF Invoice'));
		$this->setDescription(
			$gallery->i18n(
				'Download invoices for Checkout transactions in PDF format'
			)
		);

		$this->setVersion('1.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('commerce', $gallery->i18n('Commerce'));
		$this->setCallbacks('');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new CheckoutPDFInvoiceModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		// Register our plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutStatusPageInterface_1_0',
			'CheckoutPDFInvoiceStatusPlugin',
			'CheckoutPDFInvoiceStatus',
			'modules/checkoutpdfinvoice/classes/CheckoutPDFInvoiceStatusPlugin.class',
			'checkoutPDFInvoice',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}
}
