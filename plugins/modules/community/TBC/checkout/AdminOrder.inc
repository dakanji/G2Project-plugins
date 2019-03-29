<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

/**
 * @package Checkout
 * @version $Revision: 1689 $
 * @author Alec Myers <alec@alecmyers.com>
 */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutTransactionHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutProductMapHelper.class');
GalleryCoreApi::requireOnce('modules/checkout/classes/GalleryCheckoutApi.class');

/**
 * View Checkout Transactions
 */
class AdminOrderController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		//check plugins first...
		list($ret, $results) = CheckoutInterfaceHelper::handleStatusPagePlugins($form);

		if ($ret) {
			return array($ret, null);
		}

		if ($results) {
			return array(null, $results);
		}

		//otherwise must be us
		$messageText = '';

		if (isset($form['messageText'])) {
			$messageText = wordwrap($form['messageText'], 70);
		}

		$error  = array();
		$status = array();

		if (isset($form['action']['deleteOrder'])) {
			$ret = GalleryCoreApi::deleteEntityById($form['transactionId']);

			if ($ret) {
				return array($ret, null);
			}

			$status['transactionDeleted']   = 1;
			$results['redirect']['view']    = 'core.SiteAdmin';
			$results['redirect']['subView'] = 'checkout.AdminTransactions';
			$results['status']              = $status;
			$results['error']               = $error;

			return array(null, $results);
		}

		list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($form['transactionId']);

		if ($ret) {
			return array($ret, null);
		}

		if (isset($form['action']['saveAddress'])) {
			list($form['custDetails']['custAddress1'],
					$form['custDetails']['custAddress2'],
					$form['custDetails']['custAddress3'],
					$form['custDetails']['custAddress4'],
					$form['custDetails']['custAddress5']) = explode("\n", $form['custDetails']['address'], 5);

			$ret = GalleryCheckoutApi::transactionUpdate($transaction, $form['custDetails']);

			if ($ret) {
				return array($ret, null);
			}

			$status['addressSaved'] = 1;
		}

		if (isset($form['action']['reconfirm'])) {
			$ret = GalleryCheckoutApi::changeTransactionStatus(
				$transaction,
				'confirm',
				$messageText
			);

			if ($ret) {
				return array($ret, null);
			}

			$status['reconfirmed'] = 1;
		}

		if (isset($form['action']['clear'])) {
			$ret = GalleryCheckoutApi::changeTransactionStatus(
				$transaction,
				'clear',
				$messageText
			);

			if ($ret) {
				return array($ret, null);
			}

			$status['cleared'] = 1;
		}

		if (isset($form['action']['complete'])) {
			$ret = GalleryCheckoutApi::changeTransactionStatus(
				$transaction,
				'complete',
				$messageText
			);

			if ($ret) {
				return array($ret, null);
			}

			$status['dispatched'] = 1;
		}

		if (isset($form['action']['admin'])) {
			$ret = GalleryCheckoutApi::sendAdminNotification($transaction, $messageText);

			if ($ret) {
				return array($ret, null);
			}

			$status['notified'] = 1;
		}

		if (isset($form['action']['edit'])) {
			$status['edit'] = 1;
		}

		if (isset($form['action']['editNotes'])) {
			$status['editNotes'] = 1;
		}

		if (isset($form['action']['saveNotes'])) {
			$ret = GalleryCheckoutApi::transactionUpdate($transaction, $form['notes']);

			if ($ret) {
				return array($ret, null);
			}

			$status['notesSaved'] = 1;
		}

		$results['redirect']['view']          = 'core.SiteAdmin';
		$results['redirect']['subView']       = 'checkout.AdminOrder';
		$results['redirect']['transactionId'] = $form['transactionId'];
		$results['status']                    = $status;
		$results['error']                     = $error;

		return array(null, $results);
	}
}

class AdminOrderView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'AdminOrder') {
			$form['formName']    = 'AdminOrder';
			$form['messageText'] = '';

			// Figure out which item we are talking about
			$form['transactionId'] = (int)GalleryUtilities::getRequestVariables('transactionId');

			if (empty($form['transactionId'])) {
				return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER), null);
			}
		}

		list($ret, $tpl['busname']) = GalleryCoreApi::getPluginParameter(
			'module',
			'checkout',
			'busname'
		);

		if ($ret) {
			return array($ret, null);
		}

		// plugins
		list($ret, $plugins) = CheckoutInterfaceHelper::getStatusPagePlugins($template, $form);

		if ($ret) {
			return array($ret, null);
		}

		$tpl['id'] = $form['transactionId'];

		$template->setVariable('statusPlugins', $plugins);
		$template->title($tpl['busname'] . '::' . $gallery->i18n('Checkout'));
		$template->setVariable('tpl', $tpl);
		$template->setVariable('controller', 'checkout.AdminOrder');

		return array(
			null,
			array(
				'body' => 'modules/checkout/templates/AdminOrder.tpl',
			),
		);
	}
}
