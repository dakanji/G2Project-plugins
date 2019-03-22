<?php

/*
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2004 Bharat Mediratta
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
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 */

/**
 * This view is for letting the user know the transaction was completed, and
 * emptying the shopping cart
 *
 * @package Checkout
 * @subpackage UserInterface
 */

/**
 * Required classes
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutHelper.class');

class OrderStatusController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		//check plugins first...
		list($ret, $results) = CheckoutInterfaceHelper::handleStatusPagePlugins($form);

		if ($ret) {
			return array($ret, null);
		}

		if ($results) {
			return array(null, $results);
		}

		//otherwise must be us
		if (isset($form['action']['continueShopping'])) {
			$results['redirect']['view'] = '';
			$results['status']           = '';
			$results['error']            = '';

			return array(null, $results);
		}
	}
}

class OrderStatusView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		// Figure out which item we're talking about
		$transactionId = GalleryUtilities::getRequestVariables('transactionId');

		if (empty($transactionId)) {
			return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
		}

		// get the transaction
		list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($transactionId);

		if ($ret) {
			return array($ret, null);
		}

		if (!isset($transaction)) {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}

		if ($transaction->getClassName() != 'GalleryCheckoutTransaction') {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}

		//security check
		$verify1 = GalleryUtilities::getRequestVariables('verify1');
		$verify2 = GalleryUtilities::getRequestVariables('verify2');

		if ($verify1 != $transaction->getDate() || $verify2 != $transaction->getSecurity()) {
			return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
		}

		if ($form['formName'] != 'OrderStatus') {
			$form['formName'] = 'OrderStatus';

			// Figure out which item we're talking about
			$form['transactionId'] = $transactionId;
		}

		// order status plugins
		list($ret, $plugins) = CheckoutInterfaceHelper::getStatusPagePlugins($template, $form);

		if ($ret) {
			return array($ret, null);
		}

		// if the user is site admin we can show button for ordersadmin page
		list($ret, $isAdmin) = GalleryCoreApi::isUserInSiteAdminGroup();

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $tpl['busname']) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'busname');

		if ($ret) {
			return array($ret, null);
		}

		$tpl['isAdmin']    = $isAdmin;
		$tpl['id']         = $transactionId;
		$tpl['controller'] = 'checkout.OrderStatus';

		$template->setVariable('statusPlugins', $plugins);
		$template->setVariable('tpl', $tpl);

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkout');

		if ($ret) {
			return array($ret, null);
		}

		$template->title($tpl['busname'] . ' ' . $module->translate('Order Status'));

		return array(
			null,
			array(
				'body' => 'modules/checkout/templates/OrderStatus.tpl',
			),
		);
	}
}
