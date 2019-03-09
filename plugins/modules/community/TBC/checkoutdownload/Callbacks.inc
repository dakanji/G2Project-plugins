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
  * @version $Revision: 1690 $
  * @package CheckoutDownload
  * @author Alec Myers <alec@alecmyers.com>
  */
GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutTransactionHelper.class');
GalleryCoreApi::requireOnce('modules/checkoutdownload/classes/CheckoutDownloadHelper.class');

class CheckoutdownloadCallbacks {
	public function callback($params, &$smarty, $callback, $userId = null) {
		global $gallery;

		switch ($callback) {
			case 'downloadInfo':
				 // transaction status flags
				 list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($params['transactionId']);

				if ($ret) {
					return $ret;
				}

				if (!isset($transaction)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				$block                                =& $smarty->_tpl_vars['block'];
				$block['checkoutDownload']['verify1'] = $transaction->getDate();
				$block['checkoutDownload']['verify2'] = $transaction->getSecurity();

				list(
					$ret,
					$block['checkoutDownload']['downloadTest']
				) = CheckoutDownloadHelper::checkDownloadOffer($params['transactionId']);
				if ($ret) {
					return $ret;
				}

				$block['checkoutDownload']['downloaded']       = $transaction->getTimeDownloaded();
				$block['checkoutDownload']['downloadAttempts'] = $transaction->getDownloadAttempts();
				return null;
			break;
		}

		return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}
}
