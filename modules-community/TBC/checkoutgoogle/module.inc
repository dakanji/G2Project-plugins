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
 * This view creates a form to send data to digibug.com
 * @package CheckoutGoogle
 * @subpackage UserInterface
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1687 $
 */
class CheckoutGoogleModule extends GalleryModule {
	public function __construct() {
		global $gallery;
		$this->setId('checkoutgoogle');
		$this->setName('Checkout by Google');
		$this->setDescription(
			$gallery->i18n(
				'Complete checkout by Google Checkout'
			)
		);

		$this->setVersion('1.0.0'); // Update upgrade() as required
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));
		$this->_templateVersion = 1;

		$this->setGroup('commerce', $gallery->i18n('Commerce'));
		$this->setCallbacks('getSiteAdminViews|registerEventListeners');
	}

	/**
	 * Load as Singleton
	 */
	public static function doStatic() {
		static $singleton;

		if (!isset($singleton)) {
			$singleton = new CheckoutGoogleModule();
		}

		return $singleton;
	}

	/**
	 * @see GalleryModule::upgrade()
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		if (!empty($currentVersion)) {
			$storage =& $gallery->getStorage();

			switch ($currentVersion) {
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

	/**
	 * @see GalleryModule::registerEventListeners()
	 */
	public function registerEventListeners() {
		GalleryCoreApi::requireOnce('modules/checkoutgoogle/classes/CheckoutGoogleEventHelper.class');
		$myListener = new CheckoutGoogleEventHelper();
		// 2.2.x event registrations
		/*  GalleryCoreApi::registerEventListener('GalleryCheckout::transactionChange', $myListener);
		GalleryCoreApi::registerEventListener('GalleryEntity::delete', $myListener); */
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		// Register our plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutPaymentInterface_1_3',
			'CheckoutGooglePaymentPlugin',
			'CheckoutGooglePayment',
			'modules/checkoutgoogle/classes/CheckoutGooglePaymentPlugin.class',
			'checkoutgoogle',
			null
		);

		if ($ret) {
			return $ret;
		}

		// Register to add to emails
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'CheckoutEmailInterface_1_0',
			'CheckoutGoogleEmailPlugin',
			'CheckoutGoogleEmail',
			'modules/checkoutgoogle/classes/CheckoutGoogleEmailPlugin.class',
			'checkoutgoogle',
			null
		);

		if ($ret) {
			return $ret;
		}

		// Register Events in 2.3
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEventListener',
			'CheckoutGoogleEventHelper',
			'CheckoutGoogleEventHelper',
			'modules/checkoutgoogle/classes/CheckoutGoogleEventHelper.class',
			'checkoutgoogle',
			array('GalleryEntity::delete', 'GalleryCheckout::transactionChange'),
			null
		);

		if ($ret) {
			return $ret;
		}

		// Activation event for rewrite rule
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GalleryEventListener',
			'CheckoutGoogleModule',
			'CheckoutGoogleModule',
			'modules/checkoutgoogle/module.inc',
			'checkoutgoogle',
			array('Gallery::ActivatePlugin'),
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
		list($ret, $value) = $this->getParameter('gcMerchantId');

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
					'name' => $this->translate('Checkout by Google'),
					'view' => 'checkoutgoogle.Admin',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getConfigurationView
	 */
	public function getConfigurationView() {
		return 'checkoutgoogle.Admin';
	}

	/**
	 * @see GalleryEventListener::handleEvent
	 */
	public function handleEvent($event) {
		if ($event->getEventName() == 'Gallery::ActivatePlugin') {
			$data = $event->getData();

			if ($data['pluginType'] != 'module' || $data['pluginId'] != 'checkoutgoogle') {
				return array(null, null);
			}
			// this module being activate
			$ret = $this->_activateRewriteRules();

			if ($ret && !($ret->getErrorCode() & ERROR_CONFIGURATION_REQUIRED)) {
				return array($ret, null);
			}

			return array(null, null);
		}
	}

	/**
	 * Activate essential rewrite rules.
	 * @return GalleryStatus a status code
	 */
	public function _activateRewriteRules() {
		list($ret, $rewriteApi) = GalleryCoreApi::newFactoryInstance('RewriteApi');

		if ($ret) {
			return $ret;
		}

		if (!isset($rewriteApi)) {
			return GalleryCoreApi::error(ERROR_CONFIGURATION_REQUIRED);
		}
		list($ret, $isCompatible) = $rewriteApi->isCompatibleWithApi(array(1, 1));

		if ($ret) {
			return $ret;
		}

		if (!$isCompatible) {
			return GalleryCoreApi::error(ERROR_CONFIGURATION_REQUIRED);
		}

		list($ret, $success) = $rewriteApi->activateRewriteRulesForModule($this->getId());

		if ($ret) {
			return $ret;
		}

		if (!$success) {
			return GalleryCoreApi::error(ERROR_CONFIGURATION_REQUIRED);
		}

		return null;
	}

	/**
	 * @see GalleryModule::getRewriteRules
	 */
	public function getRewriteRules() {
		global $gallery;

		$rules = array();

		/*
		 * Only define the rule to pass the Authorization header to Gallery in a request variable if
		 * it is already active, or if Gallery can't access HTTP usernames and passwords
		 */
		list($ret, $rewriteApi) = GalleryCoreApi::newFactoryInstance('RewriteApi');

		if ($ret) {
			if ($gallery->getDebug()) {
				$gallery->debug('Error in CheckoutGoogleModule::getRewriteRules: ' . $ret->getAsText());
			}

			return $rules;
		}

		if (!isset($rewriteApi)) {
			return $rules;
		}

		list($ret, $isCompatible) = $rewriteApi->isCompatibleWithApi(array(1, 1));

		if ($ret) {
			if ($gallery->getDebug()) {
				$gallery->debug('Error in CheckoutGoogleModule::getRewriteRules: ' . $ret->getAsText());
			}

			return $rules;
		}

		if (!$isCompatible) {
			return $rules;
		}

		list($ret, $activeRules) = $rewriteApi->fetchActiveRulesForModule($this->getId());

		if ($ret) {
			if ($gallery->getDebug()) {
				$gallery->debug('Error in CheckoutGoogleModule::getRewriteRules: ' . $ret->getAsText());
			}

			return $rules;
		}

		if (!in_array('checkoutgoogle', $activeRules)) {
			// Check that Gallery can access HTTP usernames and passwords
			GalleryCoreApi::requireOnce('modules/checkoutgoogle/classes/CheckoutGoogleResponseHelper.class');
			list($ret, $success) = CheckoutGoogleResponseHelper::checkHttpAuth();

			if ($ret) {
				if ($gallery->getDebug()) {
					$gallery->debug(
						'Error in checkoutGoogleModule::getRewriteRules: ' . $ret->getAsText()
					);
				}

				return $rules;
			}

			if ($success) {
				return $rules;
			}
		}

		// Pass the Authorization header to Gallery in a request variable
		$rules['checkoutgoogle'] = array(
			'comment'    => $this->translate('Authorization Header'),
			'help'       => $this->translate('Pass the Authorization header to Gallery in a request variable.'),
			'conditions' => array(
				array(
					'test'    => 'QUERY_STRING',
					'pattern' => GalleryUtilities::prefixFormVariable('view') . '=checkoutgoogle.Response',
				),
				array(
					'test'    => 'QUERY_STRING',
					'pattern' => '!' . GalleryUtilities::prefixFormVariable('authorization') . '=',
				),
				array(
					'test'    => 'HTTP:Authorization',
					'pattern' => '%authorization%',
				),
			),
			'flags'      => array('QSA,L'),
			'keywords'   => array(
				'authorization' => array(
					'pattern' => '(.+)',
					'help'    => $this->translate('Authorization header.'),
				),
			),
		);

		list($ret, $override) = $this->getParameter('overrideURL');

		if ($ret) {
			return array($ret, null);
		}
		list($ret, $gcCallbackURL) = $this->getParameter('gcCallbackURL');

		if ($ret) {
			return array($ret, null);
		}

		if ($override) {
			$rules['checkoutgoogle']['options'] = array(
				'baseUrl' => $gcCallbackURL,
			);
		}

		return $rules;
	}
}
