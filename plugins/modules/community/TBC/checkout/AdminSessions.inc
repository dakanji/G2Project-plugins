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

/**
 * View Checkout Transactions
 */
class AdminSessionsView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'Sessions') {
			$form['formName'] = 'Sessions';
		}

		GalleryCoreApi::requireOnce('modules/core/classes/GallerySession.class');
		$mySession = new GallerySession();

		list($ret, $results) = GalleryCoreApi::getMapEntry(
			'GallerySessionMap',
			array(
				'id',
				'userId',
				'remoteIdentifier',
				'creationTimestamp',
				'modificationTimestamp',
				'data',
			),
			array(),
			array(
				'orderBy' => array(
					'modificationTimestamp' => ORDER_DESCENDING,
				),
				'limit'   => 500,
			)
		);

		if ($ret) {
			return array($ret, null);
		}

		$badHosts  = array('search.msn.com', 'crawl.yahoo.net', '^65.55');
		$userNames = array();
		while ($pSession = $results->nextResult()) {
			$mySession->_userId            = (int)$pSession[1];
			$mySession->_remoteIdentifier  = unserialize($pSession[2]);
			$mySession->_creationTime      = (int)$pSession[3];
			$mySession->_modificationTime  = (int)$pSession[4];
			$mySession->_sessionData       = unserialize($pSession[5]);
			$mySession->_loadedSessionData = $mySession->_getSerializedSession();

			//$hostName = gethostbyaddr($mySession->_remoteIdentifier[0]);
			// Remove obvious searchbots
			$host = $mySession->_remoteIdentifier[0];
			foreach ($badHosts as $badHost) {
				if (ereg($badHost, $host)) {
					continue 2;
				}
			}

			$items = $mySession->get('checkout.items');

			/* Remove empty carts
			if (empty($items)) {
			continue;
			}

			*/
			$transactionId = $mySession->get('checkout.transactionId');
			if (ereg(';', $transactionId)) {
				$transactionId = unserialize($transactionId);
			}

			$userId         = $mySession->getuserId();
			$users[$userId] = $userId;
			$sessions[]     = array(
				'id'             => $pSession[0],
				'userId'         => $userId,
				'sessionModTime' => $mySession->getModificationTime(),
				'cartModTime'    => $mySession->get('checkout.cartLastModified'),
				'host'           => $host,
				'transactionId'  => $transactionId,
				'items'          => $items,
			);
		}

		// Get user names
		list($ret, $users) = GalleryCoreApi::loadEntitiesById($users, 'GalleryUser');

		if ($ret) {
			return array($ret, null);
		}

		$userNames = array();
		foreach ($users as $user) {
			$userNames[$user->getId()] = $user->getFullName();
		}

		$template->setVariable('sessions', $sessions);
		$template->setVariable('userNames', $userNames);
		$template->setVariable('controller', 'checkout.AdminTransactions');
		return array(
			null,
			array(
				'body' => 'modules/checkout/templates/AdminSessions.tpl',
			),
		);
	}
}
