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
 * @package Checkout
 * @version $Revision: 1689 $
 * @author Alec Myers <alec@alecmyers.com>
 */
class AdminTransactionsController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$error  = array();
		$status = array();

		if (isset($form['action']['showDelete'])) {
			$results['delegate']['showDelete'] = 1;
		}

		if (isset($form['action']['delete'], $form['delete'])) {
			foreach ($form['delete'] as $id => $decision) {
				$ret = GalleryCoreApi::deleteEntityById($id);

				if ($ret) {
					return array($ret, null);
				}
			}

			$status['transactionDeleted']   = 1;
			$results['redirect']['view']    = 'core.SiteAdmin';
			$results['redirect']['subView'] = 'checkout.AdminTransactions';
			$results['redirect']['page']    = $form['page'];
			$results['status']              = $status;
			$results['error']               = $error;

			return array(null, $results);
		}

		if (isset($form['action']['showCarts'])) {
			$results['redirect']['view']    = 'core.SiteAdmin';
			$results['redirect']['subView'] = 'checkout.AdminSessions';
			$results['status']              = $status;
			$results['error']               = $error;

			return array(null, $results);
		}

		$results['delegate']['view']    = 'core.SiteAdmin';
		$results['delegate']['subView'] = 'checkout.AdminTransactions';
		$results['delegate']['page']    = $form['page'];
		$results['status']              = $status;
		$results['error']               = $error;

		return array(null, $results);
	}
}

/**
 * View Checkout Transactions
 */
class AdminTransactionsView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'Transactions') {
			$form['formName'] = 'Transactions';
		}

		//tidy up transaction table
		$ret = CheckoutTransactionHelper::purgeDeadTransactions();

		if ($ret) {
			return array($ret, null);
		}

		list($page, $sortBy, $order, $showDelete) = GalleryUtilities::getRequestVariables(
			'page',
			'sortBy',
			'order',
			'showDelete'
		);

		if (!isset($page) || $page < 1) {
			$page = 1;
		}

		switch ($sortBy) {
			case 'date':
			case 'id':
			case 'recipientName':
			case 'host':
			case 'paymentPlugin':
			case 'amount':
				$SQLsortBy = "[GalleryCheckoutTransaction::$sortBy]";

				break;

			case 'fullName':
			case 'userName':
				$SQLsortBy = "[GalleryUser::$sortBy]";

				break;

			case 'userId':
				$SQLsortBy = '[GalleryUser::id]';

				break;

			default:
				$sortBy    = 'date';
				$SQLsortBy = '[GalleryCheckoutTransaction::date]';
		}

		if ($order != 'ASC') {
			$order = 'DESC';
		}

		$results = array();

		$query = "SELECT
		[GalleryCheckoutTransaction::date],
		[GalleryCheckoutTransaction::id],
		[GalleryUser::fullName],
		[GalleryUser::userName],
		[GalleryCheckoutTransaction::recipientName],
		[GalleryCheckoutTransaction::host],
		[GalleryCheckoutTransaction::paymentPlugin],
		[GalleryCheckoutTransaction::amount],
		[GalleryCheckoutTransaction::timePaid],
		[GalleryCheckoutTransaction::timeCleared],
		[GalleryCheckoutTransaction::timeFulfil],
		[GalleryCheckoutTransaction::timeFulfilled],
		[GalleryCheckoutTransaction::timeCompleted],
		[GalleryCheckoutTransaction::timeHold],
		[GalleryCheckoutTransaction::timeProblem]
		FROM [GalleryCheckoutTransaction]
		INNER JOIN [GalleryUser]
		ON [GalleryUser::id] = [GalleryCheckoutTransaction::buyerId]
		WHERE ([GalleryCheckoutTransaction::paymentPlugin] IS NOT NULL)
		  OR ([GalleryCheckoutTransaction::timeConfirmed] != 0)
		ORDER BY $SQLsortBy $order
		";

		list($ret, $searchResults) = $gallery->search($query);

		if ($ret) {
			return array($ret, null);
		}

		$txns = array();

		while ($result = $searchResults->nextResult()) {
			$txns[] = $result;
		}

		$pageDetails['sortBy']     = $sortBy;
		$pageDetails['order']      = $order;
		$pageDetails['showDelete'] = $showDelete;
		$perPage                   = 50;
		$countOrders               = 0;

		if (isset($txns)) {
			$countOrders = count($txns);
		}

		$pageDetails['count']    = $countOrders;
		$pageDetails['pageSize'] = $countOrders > $perPage ? $perPage : $countOrders;

		// Figure out our max pages, make sure our current page fits in it
		$pageDetails['maxPages'] = ceil($countOrders / $perPage);
		$pageDetails['page']     = min($pageDetails['maxPages'], $page);

		// Calculate the next/back pages
		$pageDetails['nextPage'] = min($pageDetails['page'] + 1, $pageDetails['maxPages']);
		$pageDetails['backPage'] = max(1, $pageDetails['page'] - 1);

		// Calculate which names to show
		$pageDetails['start'] = ($pageDetails['page'] - 1) * $perPage;

		$template->setVariable('pageDetails', $pageDetails);
		$template->setVariable('txns', $txns);
		$template->setVariable('controller', 'checkout.AdminTransactions');

		return array(
			null,
			array(
				'body' => 'modules/checkout/templates/AdminTransactions.tpl',
			),
		);
	}
}
