<?php
/*
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
 * @version $Revision: 1916 $
 * @author Daniel Leaberry <leaberry@gmail.com>
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * Required classes
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutProductMapHelper.class');

/**
 *
 * @package Checkout
 */
class CheckoutModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('checkout');
		$this->setName($gallery->i18n('Checkout'));
		$this->setDescription(
			$gallery->i18n(
				'Shopping cart module supporting different products and payment'
			)
		);

		$this->setVersion('1.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('commerce', $gallery->i18n('Commerce'));
		$this->setCallbacks('registerEventListeners|getSiteAdminViews|getItemLinks|getSystemLinks');
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new CheckoutModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		// Register the GalleryCheckoutTransaction class
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEntity',
			'GalleryCheckoutTransaction',
			'GalleryCheckoutTransaction',
			'modules/checkout/classes/GalleryCheckoutTransaction.class',
			'checkout',
			null
		);

		if ($ret) {
			return $ret;
		}

		// Register the GalleryCheckoutItem class
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEntity',
			'GalleryCheckoutItem',
			'GalleryCheckoutItem',
			'modules/checkout/classes/GalleryCheckoutItem.class',
			'checkout',
			null
		);

		if ($ret) {
			return $ret;
		}

		// Register our item edit plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'ItemEditPlugin',
			'CheckoutItemEdit',
			'CheckoutItemEdit',
			'modules/checkout/CheckoutItemEdit.inc',
			'checkout',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::registerEventListeners()
	 */
	public function registerEventListeners() {
		$myListener = new CheckoutModule();
		GalleryCoreApi::registerEventListener('GalleryCheckout::emptyCart', $myListener);
		GalleryCoreApi::registerEventListener('GalleryCheckout::purchaseComplete', $myListener);
		GalleryCoreApi::registerEventListener('GalleryCheckout::transactionChange', $myListener);
		GalleryCoreApi::registerEventListener('GalleryCheckout::transactionUpdate', $myListener);
		GalleryCoreApi::registerEventListener('GalleryCheckout::transactionSetPaymentPlugin', $myListener);
		GalleryCoreApi::registerEventListener('GalleryCheckout::adminNotify', $myListener);
		GalleryCoreApi::registerEventListener('GalleryEntity::delete', $myListener);
	}

	/**
	 * @see GalleryModule::upgrade()
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		if (!empty($currentVersion)) {
			// remove template cache to use new admin template
			//$templateCacheDir = $gallery->getConfig('data.smarty.templates_c');
			//if ($platform->file_exists($templateCacheDir)) {
			// Ignore errors
			//    $platform->recursiveRmdir($templateCacheDir);
			// }
			$storage =& $gallery->getStorage();

			switch ($currentVersion) {
				case '0.1.1':
				case '0.1.2':
				case '0.1.3':
				case '0.1.4':
				case '0.1.5':
					// First standalone version.  Register our permissions
					$permissions[] = array('purchase', $gallery->i18n('[checkout] Purchase item'), 0, array());

					foreach ($permissions as $p) {
						$ret = GalleryCoreApi::registerPermission(
							$this->getId(),
							'checkout.' . $p[0],
							$p[1],
							$p[2],
							$p[3]
						);

						if ($ret) {
							return $ret;
						}
					}

					// Do the schema upgrade
					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.0'));

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.1.6':
					// Do the schema upgrade
					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.1'));

					if ($ret) {
						return $ret;
					}

					// Convert the old integer amounts to strings by dividing by 100
					$query = '
	        UPDATE
	          [GalleryCheckoutTransaction]
	        SET
	          [::amount] = [::amount]/100
	        ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.1.7':
					// Do the schema upgrade
					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.2'));

					if ($ret) {
						return $ret;
					}

					// For transactions with an email address, set flagPaid
					$query = '
	            UPDATE
	              [GalleryCheckoutTransaction]
	            SET
	              [::flagPaid] = 1
	            WHERE
	              [::email] IS NOT NULL
	            ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}

					/*
					 * For transactions with CheckoutPaypal children, set
					 * paymentPlugin=checkoutpaypal
					 * Only run this step if CheckoutPaypal is activated
					 *
					 */

					list($ret, $moduleStatus) = GalleryCoreApi::fetchPluginStatus('module', true);

					if ($ret) {
						return array($ret, null);
					}

					if (!empty($moduleStatus['checkoutpaypal']['active'])) {
						$query = '
	                UPDATE
	                  [GalleryCheckoutTransaction]
	                INNER JOIN
	                  [GalleryChildEntity]
	                ON
	                  [GalleryChildEntity::parentId] = [GalleryCheckoutTransaction::id]
	                SET
	                  [GalleryCheckoutTransaction::paymentPlugin] = \'checkoutpaypal\'
	                ';

						$ret = $storage->execute($query);

						if ($ret) {
							return $ret;
						}
					}

					// For all other transactions, set paymentPlugin=checkoutemail
					$query = '
	            UPDATE
	              [GalleryCheckoutTransaction]
	            SET
	              [::paymentPlugin] = \'checkoutemail\'
	            WHERE
	              [::paymentPlugin] IS NULL
	            ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.1.8':
					// Do the schema upgrade
					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.3'));

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.1.9':
				case '0.1.10':
				case '0.1.11':
				case '0.1.12':
					/* DON'T do the schema upgrade - there is a problem changing the INTEGER size in G2.1
					 * $ret = $storage->configureStore($this->getId(), array('GalleryCheckoutItem:1.0'));
					 * if ($ret) {
					 *     return $ret;
					 * }
					 */
				case '0.1.13':
				case '0.1.14':
				case '0.1.15':
					// Do the schema upgrade
					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.4'));

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.1.16':
					// Do the schema upgrade
					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.5'));

					if ($ret) {
						return $ret;
					}

					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutItem:1.1'));

					if ($ret) {
						return $ret;
					}

					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.6'));

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.1.17':
				case '0.1.17.1':
				case '0.1.18':
					// update products
					// How many products do we have?
					list($ret, $numProducts) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'numProducts');

					if ($ret) {
						return $ret;
					}

					if (empty($numProducts)) {
						$numProducts = 0;
					}

					//note: admin page values are all owned by the root album
					list($ret, $rootAlbumId) = GalleryCoreApi::getDefaultAlbumId();

					if ($ret) {
						return $ret;
					}

					//rewrite the old product details
					for ($i = 0; $i < $numProducts; $i++) {
						list($ret, $name) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'product' . $i);

						if ($ret) {
							return $ret;
						}

						list($ret, $price) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'price' . $i);

						if ($ret) {
							return $ret;
						}

						list($ret, $restrict) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'restrict' . $i);

						if ($ret) {
							return $ret;
						}

						list($ret, $downloadable) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'downloadable' . $i);

						if ($ret) {
							return $ret;
						}

						list($ret, $productId) = $storage->getUniqueId();

						if ($ret) {
							return $ret;
						}

						foreach (array(
							'name'          => $name,
							'price'         => $price,
							'position'      => $i,
							'visible'       => $restrict == 'on' ? 'false' : 'true',
							'download'      => $downloadable == 'on' ? 'true' : 'false',
							'chargePostage' => $downloadable == 'on' ? 'false' : 'true',
							'showPapers'    => 'false',
						)
						as $paramName => $paramValue) {
							$ret = CheckoutProductMapHelper::setParam($productId, $paramName, $paramValue, $rootAlbumId);

							if ($ret) {
								return $ret;
							}
						}

						foreach (array('product', 'price', 'restrict', 'downloadable', 'prodref') as $oldParamName) {
							$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', $oldParamName . $i);

							if ($ret) {
								return $ret;
							}
						}

						$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'numProducts');

						if ($ret) {
							return $ret;
						}
					}


					// Fall Through
				case '0.2.0':
					/* version number cockup in galleryCheckoutTransaction.class in 0.1.18 means we
					need to manually advance the schema from 1.6 to 1.7 where it should have been */

					$query = '
	            UPDATE
	              [GallerySchema]
	            SET
	              [::minor] = 7
	            WHERE
	              [::name] = \'CheckoutTransaction\'
	            AND
	              [::minor] = 6
	            ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}

					// Do the real schema upgrade
					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.7'));

					if ($ret) {
						return $ret;
					}

					// update postage
					list($ret, $postage) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'postage');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginparameter('module', 'checkout', 'postPrice0', $postage);

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginparameter('module', 'checkout', 'postType0', $gallery->i18n('Standard'));

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginparameter('module', 'checkout', 'numPostages', 1);

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'postage');

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.2.1':

					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.8'));

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.3.0':

					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.9'));

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'selecttype');

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.3.1':
					//new installs of 0.3.1 had the wrong schema...
					$query = '
	            UPDATE
	              [GallerySchema]
	            SET
	              [::minor] = 10
	            WHERE
	              [::name] = \'CheckoutTransaction\'
	            AND
	              [::minor] = 9
	            ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}

					$ret = $storage->configureStore(
						$this->getId(),
						array(
							'CheckoutDiscountMap:1.0',
							'GalleryCheckoutTransaction:1.10',
							'GalleryCheckoutItem:1.2',
						)
					);

					if ($ret) {
						return $ret;
					}

					//remove parameters now associated with checkoutdownload module
					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'allowDownloadsPrePurchase');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'allowDownloadOriginal');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'displayDLLink');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'limitDaysDLAvailable');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'limitNumDLsAllowed');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'useFoldersInDL');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'daysDLAvailable');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'numDLsAllowed');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'zipfilename');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'useUniqueDLFilename');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'emailDLFromAddress');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'emailDLSubject');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'emailDownloadLink');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'allowDownloadsPostPurchase');

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.3.2':

					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.11'));

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'cformat', '%.2f');

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.3.3':
					// change addAlbum to treatAlbumAsCollection, also move defaultqty to first product
					list($ret, $canAddAlbum) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'canAddAlbum');

					if ($ret) {
						return $ret;
					}

					list($ret, $defaultqty) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'defaultqty');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'canAddAlbum');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'defaultqty');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginparameter('module', 'checkout', 'treatAlbumAsCollection', $canAddAlbum);

					if ($ret) {
						return $ret;
					}

					// set all existing products: appliesToItem = true
					list($ret, $rootAlbumId) = GalleryCoreApi::getDefaultAlbumId();

					if ($ret) {
						return array($ret, null);
					}

					list($ret, $products) = CheckoutProductMapHelper::fetchProducts($rootAlbumId, true, false);
					// do use local settings, don't pass products to plugins to modify
					if ($ret) {
						return $ret;
					}

					foreach ($products as $product) {
						$ret = CheckoutProductMapHelper::setParam($product['productId'], 'appliesToItem', 'true', $rootAlbumId);

						if ($ret) {
							return $ret;
						}

						// once only, may set initialQuantity on a product
						if ($defaultqty) {
							$ret = CheckoutProductMapHelper::setParam($product['productId'], 'initialQuantity', '1', $rootAlbumId);

							if ($ret) {
								return $ret;
							}

							$defaultqty = false;
						}
					}

					// register new purchase album permission
					$permissions[] = array('purchaseAlbum', $gallery->i18n('[checkout] Purchase album'), 0, array());

					foreach ($permissions as $p) {
						$ret = GalleryCoreApi::registerPermission($this->getId(), 'checkout.' . $p[0], $p[1], $p[2], $p[3]);

						if ($ret) {
							return $ret;
						}
					}


					// Fall Through
				case '0.3.4':

					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.12'));

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.3.5':
				case '0.4.0':
				case '0.4.1':
					// New installs with wrong schema
					$query = '
	            UPDATE
	              [GallerySchema]
	            SET
	              [::minor] = 13
	            WHERE
	              [::name] = \'CheckoutTransaction\'
	            AND
	              [::minor] = 12
	            ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}

					// add new columns
					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.13'));

					if ($ret) {
						return $ret;
					}

					// copy old to new
					foreach (array('Confirmed', 'Paid', 'Cleared', 'Fulfil', 'Fulfilled', 'Completed', 'Hold', 'Problem', 'Downloaded') as $title) {
						$query = "UPDATE [GalleryCheckoutTransaction] SET [::time$title] = 1 WHERE [::flag$title] = 1;";
						$gallery->debug("Monster query: $query");

						$ret = $storage->execute($query);

						if ($ret) {
							return $ret;
						}
					}

					// Remove old columns
					$ret = $storage->configureStore($this->getId(), array('GalleryCheckoutTransaction:1.14'));

					if ($ret) {
						return $ret;
					}

					// Put email, postage and paper settings into new format
					list($ret, $mp) = GalleryCoreApi::fetchAllPluginParameters('module', 'checkout');

					if ($ret) {
						return array($ret, null);
					}

					// Postage settings
					$posts = array();

					for ($i = 0; $i < $mp['numPostages']; $i++) {
						$posts[$i]['type']  = $mp['postType' . $i];
						$posts[$i]['price'] = $mp['postPrice' . $i];

						$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'postType' . $i);

						if ($ret) {
							return $ret;
						}

						$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'postPrice' . $i);

						if ($ret) {
							return $ret;
						}
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'numPostages');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'postages', serialize($posts));

					if ($ret) {
						return $ret;
					}

					// Papers
					$papers = array();

					for ($i = 0; $i < $mp['numPapers']; $i++) {
						$papers[$i] = $mp['paper' . $i];

						$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'paper' . $i);

						if ($ret) {
							return $ret;
						}
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'numPapers');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'papers', serialize($papers));

					if ($ret) {
						return $ret;
					}

					// Emails
					$emails = array();

					for ($i = 0; $i < $mp['numAddresses']; $i++) {
						foreach (array('toAddress', 'emailLanguage') as $paramName) {
							$emails[$i][$paramName] = $mp[$paramName . $i];

							$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', $paramName . $i);

							if ($ret) {
								return $ret;
							}
						}

						foreach (array('showCustomer', 'showLinks', 'showContents', 'showThumbs', 'isAdmin') as $paramName) {
							$emails[$i][$paramName] = $mp[$paramName . $i] ? 1 : 0;

							$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', $paramName . $i);

							if ($ret) {
								return $ret;
							}
						}
					}

					$ret = GalleryCoreApi::removePluginParameter('module', 'checkout', 'numAddresses');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'emails', serialize($emails));

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case '0.4.2':
					// papers and postage groups
					list($ret, $mp) = GalleryCoreApi::fetchAllPluginParameters('module', 'checkout');

					if ($ret) {
						return array($ret, null);
					}

					$papers   = unserialize($mp['papers']);
					$postages = unserialize($mp['postages']);

					foreach ($papers as $key => $type) {
						$papers[$key] = array(
							1 => $type,
						);
					}

					foreach ($postages as $key => $details) {
						$postages[$key]['price'] = array(
							1 => $details['price'],
							2 => '',
						);
					}

					$query = '
	            UPDATE
	              [CheckoutProductMap]
	            SET
	              [::paramName] = \'postageBand\', [::paramValue] = 0
	            WHERE
	              [::paramName] = \'chargePostage\'
	            AND
	              [::paramValue] = \'false\'
	            ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}

					$query = '
	            UPDATE
	              [CheckoutProductMap]
	            SET
	              [::paramName] = \'postageBand\', [::paramValue] = 1
	            WHERE
	              [::paramName] = \'chargePostage\'
	            AND
	              [::paramValue] = \'true\'
	            ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}

					$query = '
	            UPDATE
	              [CheckoutProductMap]
	            SET
	              [::paramName] = \'paperSet\', [::paramValue] = 0
	            WHERE
	              [::paramName] = \'showPapers\'
	            AND
	              [::paramValue] = \'false\'
	            ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}

					$query = '
	            UPDATE
	              [CheckoutProductMap]
	            SET
	              [::paramName] = \'paperSet\', [::paramValue] = 1
	            WHERE
	              [::paramName] = \'showPapers\'
	            AND
	              [::paramValue] = \'true\'
	            ';

					$ret = $storage->execute($query);

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'numPostageBands', '2');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'numPaperSets', '2');

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'postages', serialize($postages));

					if ($ret) {
						return $ret;
					}

					$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'papers', serialize($papers));

					if ($ret) {
						return $ret;
					}


					// Fall Through
				case 'end of upgrade path':
					break;

				default:
					return GalleryCoreApi::error(ERROR_BAD_PLUGIN, sprintf('Unknown module version %s', $currentVersion));
			}
		}

		if (!isset($currentVersion)) {
			// Initial install.  Register our permissions
			$permissions[] = array('purchase', $gallery->i18n('[checkout] Purchase item'), 0, array());
			$permissions[] = array('purchaseAlbum', $gallery->i18n('[checkout] Purchase album'), 0, array());

			foreach ($permissions as $p) {
				$ret = GalleryCoreApi::registerPermission($this->getId(), 'checkout.' . $p[0], $p[1], $p[2], $p[3]);

				if ($ret) {
					return $ret;
				}
			}

			// Set current user's email address to receive order notifications
			// And also to send email to purchaser
			list($ret, $user) = GalleryCoreApi::loadEntitiesById($gallery->getActiveUserId(), 'GalleryUser');

			if ($ret) {
				return array($ret, null);
			}

			$emails[0] = array(
				'toAddress'     => '1',
				'showCustomer'  => '1',
				'showContents'  => '1',
				'showThumbs'    => '1',
				'showLinks'     => '1',
				'isAdmin'       => '0',
				'emailLanguage' => 'transaction_language',
			);

			$emails[1] = array(
				'toAddress'     => $user->getEmail(),
				'showCustomer'  => '1',
				'showContents'  => '1',
				'showThumbs'    => '1',
				'showLinks'     => '1',
				'isAdmin'       => '1',
				'emailLanguage' => 'transaction_language',
			);

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'emails', serialize($emails));

			if ($ret) {
				return $ret;
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'postages', serialize(array()));

			if ($ret) {
				return $ret;
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'papers', serialize(array()));

			if ($ret) {
				return $ret;
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'numPostageBands', '2');

			if ($ret) {
				return $ret;
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'numPaperSets', '2');

			if ($ret) {
				return $ret;
			}

			$ret = GalleryCoreApi::setPluginParameter('module', 'checkout', 'csymbol', '');

			if ($ret) {
				return $ret;
			}

			// give purchase permission to everyone on root album
			list($ret, $coreParams) = GalleryCoreApi::fetchAllPluginParameters('module', 'core');

			if ($ret) {
				return $ret;
			}

			$gallery->guaranteeTimeLimit(300);

			$ret = GalleryCoreApi::addGroupPermission($coreParams['id.rootAlbum'], $coreParams['id.everybodyGroup'], 'checkout.purchase', true);

			if ($ret) {
				return $ret;
			}
		}

		return null;
	}

	/**
	 * @see GalleryModule::needsConfiguration
	 */
	public function needsConfiguration() {
		list($ret, $value) = $this->getParameter('configured');

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
			return array($ret, false);
		}

		return array(null, !$needsConfiguration);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		global $gallery;

		return array(
			null,
			array(
				array(
					'name' => $gallery->i18n('Checkout Configuration'),
					'view' => 'checkout.Admin',
				),
				array(
					'name' => $gallery->i18n('Checkout Orders'),
					'view' => 'checkout.AdminTransactions',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'checkout.Admin';
	}

	/**
	 * @see GalleryModule::getItemLinks()
	 */
	public function getItemLinks($items, $wantsDetailedLinks, $permissions) {
		list($ret, $alwaysShow) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'alwaysShowAddToCart');

		if ($ret) {
			return $ret;
		}

		$links = array();

		foreach ($items as $item) {
			if (isset($wantsDetailedLinks[$item->getId()]) || $alwaysShow) {
				if (($item->getCanContainChildren() && isset($permissions[$item->getId()]['checkout.purchaseAlbum']))
					|| (!$item->getCanContainChildren() && isset($permissions[$item->getId()]['checkout.purchase']))
				) {
					$itemTypeNames           = $item->itemTypeName();
					$links[$item->getId()][] = array(
						'text' => $this->translate(
							array(
								'text' => 'Add %s To Cart',
								'arg1' => $itemTypeNames[0],
							)
						),
						'params' => array(
							'controller' => 'checkout.AddToCart',
							'itemId'     => $item->getId(),
							'return'     => 1,
						),
					);
				}
			}
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::getSystemLinks
	 */
	public function getSystemLinks() {
		$links              = array();
		$links['view_cart'] = array(
			'text'   => $this->translate('View Cart'),
			'params' => array(
				'view'   => 'checkout.Select',
				'return' => 1,
			),
		);

		list($ret, $inGroup) = GalleryCoreApi::isUserInSiteAdminGroup();

		if ($ret) {
			return array($ret, null);
		}

		if ($inGroup) {
			$links['admin_orders'] = array(
				'text'   => $this->translate('Admin Orders'),
				'params' => array(
					'view'    => 'core.SiteAdmin',
					'subView' => 'checkout.AdminTransactions',
					'return'  => 1,
				),
			);
		}

		return array(null, $links);
	}

	/**
	 * Event handler for GalleryCheckout::* events
	 *
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		$result = null;

		if ($event->getEventName() == 'GalleryEntity::delete') {
			$entity = $event->getEntity();

			if (GalleryUtilities::isA($entity, 'GalleryItem')) {
				$ret = CheckoutProductMapHelper::deleteItemParameters($entity->getId());

				if ($ret) {
					return array($ret, null);
				}
			}

			return array(null, null);
		}

		return array(null, $result);
	}
}
