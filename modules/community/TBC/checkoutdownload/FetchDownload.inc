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
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @version $Revision: 1690 $
 * @package CheckoutDownload
 * @author Alan Harder <alan.harder@sun.com>
 * @author Jay Hennessy <gallery2-development@elucidos.com>
 * @author Alec Myers <alec@alecmyers.com>
 */
GalleryCoreApi::requireOnce('modules/checkoutdownload/classes/CheckoutDownloadHelper.class');

/**
 * This view sends the zip file and deletes the temp file.
 *
 * @package Checkout
 */
class FetchDownloadView extends GalleryView {
	/**
	 * @see GalleryView::isImmediate()
	 */
	public function isImmediate() {
		return true;
	}

	/**
	 * @see GalleryView::renderImmediate()
	 */
	public function renderImmediate($status, $error) {
		global $gallery;

		$platform      =& $gallery->getPlatform();
		$phpVm         = $gallery->getPhpVm();
		$transactionId = GalleryUtilities::getRequestVariables('transactionId');

		if (empty($transactionId)) {
			return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
		}

		// get the transaction
		list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($transactionId);

		if ($ret) {
			return $ret;
		}

		if (!isset($transaction)) {
			return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
		}

		if ($transaction->getClassName() != 'GalleryCheckoutTransaction') {
			return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
		}

		// security check
		$verify1 = GalleryUtilities::getRequestVariables('verify1');
		$verify2 = GalleryUtilities::getRequestVariables('verify2');

		if ($verify1 != $transaction->getDate() || $verify2 != $transaction->getSecurity()) {
			return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
		}

		$file = '';

		list($ret, $permissionResult) = CheckoutDownloadHelper::checkDownloadOffer($transactionId);

		if ($ret) {
			return $ret;
		}

		if ($permissionResult == 'permit') {
			$file = $gallery->getConfig('data.gallery.tmp') . $transactionId . '.zip';

			if (!$platform->is_readable($file)) {
				$view         = 'checkout.OrderStatus';
				$urlGenerator =& $gallery->getUrlGenerator();
				$phpVm->header(
					'Location: ' . $urlGenerator->generateUrl(
						array(
							'view'          => $view,
							'transactionId' => $transactionId,
							'verify1'       => $verify1,
							'verify2'       => $verify2,
						),
						array(
							'htmlEntities' => false,
						)
					)
				);

				return null;
			}

			// send event about download attempt
			$event = GalleryCoreApi::newEvent('GalleryCheckoutDownload::downloadNotify');

			$event->setEntity($transaction);
			$event->setData('permitted');

			list($ret) = GalleryCoreApi::postEvent($event);

			if ($ret) {
				return $ret;
			}

			// Get a write lock for the transaction and refresh from database
			list($ret, $lockId) = GalleryCoreApi::acquireWriteLock(array($transaction->getId()));

			if ($ret) {
				return $ret;
			}

			list($ret, $transaction) = $transaction->refresh();

			if ($ret) {
				return $ret;
			}

			$transaction->setTimeDownloaded(time());
			$transaction->setDownloadAttempts($transaction->getDownloadAttempts() + 1);

			// Save the modified transaction
			$ret = $transaction->save();

			if ($ret) {
				return $ret;
			}

			// Release our lock
			$ret = GalleryCoreApi::releaseLocks($lockId);

			if ($ret) {
				return $ret;
			}

			// On reload return to Cart or Download Confirmation
			list($ret, $zipfilename) = GalleryCoreApi::getPluginParameter('module', 'checkoutDownload', 'zipfilename');

			if ($ret) {
				return $ret;
			}

			list($ret, $uniquefilename) = GalleryCoreApi::getPluginParameter('module', 'checkoutDownload', 'useUniqueDLFilename');

			if ($ret) {
				return $ret;
			}

			if (isset($zipfilename) && !empty($zipfilename)) {
				$filenamebase = $zipfilename;
			} else {
				$filenamebase = 'G2cart';
			}

			$extension = 'zip';
			$uniqueId  = '';

			if ($uniquefilename) {
				$uniqueId = '-' . date('YmdHis');
			}

			$zipName = $filenamebase . $uniqueId . '.' . $extension;

			$size = $platform->filesize($file);

			// Disable output compression (if necessary) since IE seems to have problems with it
			@$phpVm->ini_set('zlib.output_compression', 0);
			$phpVm->header('Content-Type: application/octet-stream');
			$phpVm->header('Content-Disposition: attachment; filename="' . $zipName . '"');

			if ($size > 0) {
				$phpVm->header("Content-Length: $size");
			}

			if ($fd = $platform->fopen($file, 'rb')) {
				while (true) {
					$data = $platform->fread($fd, 65535);

					if (strlen($data) == 0) {
						break;
					}

					echo $data;

					$gallery->guaranteeTimeLimit(30);
				}

				$platform->fclose($fd);
			}

			@$platform->unlink($file);
		}

		return null;
	}
}
