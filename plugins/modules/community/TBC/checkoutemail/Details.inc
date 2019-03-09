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
 GalleryCoreApi::requireOnce('modules/checkout/classes/GalleryCheckoutApi.class');

/**
 * @version $Revision: 1915 $
 * @package CheckoutEmail
 * @author Matthew Turnbull <matthew.turnbull@messagescan.net>
 * @author Alec Myers <alec@alecmyers.com>
 */
class DetailsController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest()
	 */
	public function handleRequest($form) {
		global $gallery;

		$error   = array();
		$status  = array();
		$results = array();

		list($ret, $transaction) = GalleryCheckoutApi::getCurrentTransactionForPayment();

		if ($ret) {
			return array($ret, null);
		}

		// see if this transaction has a payment plugin already
		if ($transaction->getPaymentPlugin() != '') {
			$results['redirect']['error'] = 'alreadyPlaced';
		} elseif (isset($form['action']['email'])) {
			// Entry from checkout page here
			$results['delegate']['view'] = 'checkoutemail.Details';
		} elseif (isset($form['action']['send'])) {
			if (!empty($form['custEmail'])
				&& GalleryUtilities::isValidEmailString($form['custEmail'])
			) {
				$form['custRecipientName'] = trim($form['custRecipientName']);
				if (empty($form['custRecipientName'])) {
					$form['custRecipientName'] = $form['custFirstName']
						. ' '
						. $form['custLastName'];
				}

				$ret = GalleryCheckoutApi::setPaymentPlugin($transaction, 'checkoutemail');

				if ($ret) {
					return array($ret, null);
				}

				$ret = GalleryCheckoutApi::transactionUpdate($transaction, $form);

				if ($ret) {
					return array($ret, null);
				}
			} else {
				$error[]                     = 'form[error][badEmail]'; // badly formed email address
				$results['delegate']['view'] = 'checkoutemail.Details';
			}
		}

		if (!isset($results['delegate'])) {
			$results['redirect']['controller']    = 'checkout.Complete';
			$results['redirect']['transactionId'] = $transaction->getId();
			$results['redirect']['verify1']       = $transaction->getDate();
			$results['redirect']['verify2']       = $transaction->getSecurity();
		}

		$results['status'] = $status;
		$results['error']  = $error;
		return array(null, $results);
	}
}

/**
 * This view is for collecting the user's details
 */
class DetailsView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'Details') {
			// First time to this page
			$form['formName'] = 'Details';

			// Populate the form if a user is logged in
			list($ret, $anonymous) = GalleryCoreApi::isAnonymousUser();

			if ($ret) {
				return array($ret, null);
			}

			if (!$anonymous) {
				$user = $gallery->getActiveUser();

				list(
					$form['custFirstName'],
					$form['custLastName']
				)                  = explode(' ', $user->getFullName(), 2);
				$form['custEmail'] = $user->getEmail();
			}
		}

		$status             = $template->getVariable('status');
		$form['controller'] = 'checkoutemail.Details';

		list($ret, $busname) = GalleryCoreApi::getPluginParameter(
			'module',
			'checkout',
			'busname'
		);

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkoutemail');

		if ($ret) {
			return array($ret, null);
		}

		$template->title($busname . ' ' . $module->translate('Customer Details'));
		$template->setVariable('form', $form);
		return array(
			null,
			array(
				'body' => 'modules/checkoutemail/templates/Details.tpl',
			),
		);
	}
}
