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
GalleryCoreApi::requireOnce('modules/checkoutdownload/classes/CheckoutDownloadHelper.class');/**

 * This view sends the zip file and deletes the temp file.
 *
 * @package Checkout
 */
class BuildDownloadController extends GalleryController {
	public function omitAuthTokenCheck() {
		return true;
	}

	public function handleRequest($form) {
		global $gallery;

		$platform        =& $gallery->getPlatform();
		$templateAdapter =& $gallery->getTemplateAdapter();
		$error           = array();
		$status          = array();
		$results         = array();
		$transactionId   = GalleryUtilities::getRequestVariables('transactionId');

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
			$templateAdapter->registerTrailerCallback(array($this, 'BuildDownload'), array($transactionId, $verify1, $verify2));

			// Delegate to the progress bar view which will then call our callback function.
			$results['delegate']['view'] = 'core.ProgressBar';
		} else {
			$results['redirect']['view']          = 'checkout.Confirm';
			$results['redirect']['transactionId'] = $transactionId;
			$results['redirect']['verify1']       = $verify1;
			$results['redirect']['verify2']       = $verify2;
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}

	public function BuildDownload($transactionId, $verify1, $verify2) {
		global $gallery;

		$templateAdapter =& $gallery->getTemplateAdapter();
		$platform        =& $gallery->getPlatform();
		$slash           = $platform->getDirectorySeparator();

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'checkoutdownload');

		if ($ret) {
			return $ret;
		}

		$heading = $module->translate('Preparing your download...');
		$templateAdapter->updateProgressBar($heading, $module->translate('Preparing...'), 0);

		list($ret, $itemList) = CheckoutTransactionHelper::fetchTransactionItems($transactionId);

		if ($ret) {
			return $ret;
		}

		list($ret, $itemIdsAndOperations) = CheckoutDownloadHelper::getDownloadableItemIds($itemList);

		if ($ret) {
			return $ret;
		}

		$count = count($itemIdsAndOperations);

		if (empty($itemIdsAndOperations) || !isset($transactionId)) {
			return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
		}

		list($ret, $moduleParams) = GalleryCoreApi::fetchAllPluginParameters('module', 'checkoutdownload');

		if ($ret) {
			return $ret;
		}

		$zipBase = $platform->tempnam($gallery->getConfig('data.gallery.tmp'), 'zip_');
		$zipDir  = $zipBase . '.dir';
		$zipFile = $zipBase . '.zip';

		list($success) = GalleryUtilities::guaranteeDirExists($zipDir);

		if (!$success) {
			return GalleryCoreApi::error(ERROR_PLATFORM_FAILURE);
		}

		$albumDir  = $gallery->getConfig('data.gallery.albums');
		$stripPath = $zipDir;

		// see http://www.phpconcept.net/pclzip/index.en.php#faq05 regarding windows drive letters
		if (substr($stripPath, 1, 1) == ':') {
			$stripPath = substr($stripPath, 2);
		}

		define('PCLZIP_TEMPORARY_DIR', $gallery->getConfig('data.gallery.tmp'));

		GalleryCoreApi::requireOnce('modules/checkoutdownload/lib/pclzip.lib.php');

		$archive = new PclZip($zipFile);
		$i       = 0;

		foreach ($itemIdsAndOperations as $itemRecord) {
			$gallery->guaranteeTimeLimit(60);
			$i++;

			// Update Progress Bar
			$message = $module->translate(
				array(
					'text' => 'Preparing item %d of %d',
					'arg1' => $i,
					'arg2' => $count,
				)
			);
			$templateAdapter->updateProgressBar($heading, $message, $i / $count);
			$itemId = $itemRecord['id'];

			list($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);

			if ($ret) {
				return $ret;
			}

			// Set the scaleToFit to products' data
			$scaleToFit = $itemRecord['downloadSize'];

			// OperationString not used at present
			$OperationString = $itemRecord['downloadOperation'];

			// Sort out paths
			list($ret, $originalPath) = $item->fetchPath();

			if ($ret) {
				return $ret;
			}

			if (!$moduleParams['useFoldersInDL']) {
				$relativePath = basename($originalPath);
			} else {
				$relativePath = (strpos($originalPath, $albumDir) === 0) ? substr($originalPath, strlen($albumDir)) : basename($originalPath);
			}

			$destination = $zipDir . $slash . $relativePath;

			// Insert file-size into name
			if ($scaleToFit != '') {
				$pathParts   = pathinfo($destination);
				$destination = $pathParts['dirname'] . $slash . $scaleToFit . '_' . $pathParts['basename'];
			}

			list($success, $created) = GalleryUtilities::guaranteeDirExists(dirname($destination));

			if (!$success) {
				return GalleryCoreApi::error(ERROR_PLATFORM_FAILURE);
			}

			// Copy or scale?
			if ($scaleToFit == '' && $OperationString == '') {
				if (!$platform->copy($originalPath, $destination)) {
					return GalleryCoreApi::error(ERROR_PLATFORM_FAILURE);
				}
			} else {
				// assemble operations to scale to scaleToFit
				$operation = 'scale|' . $scaleToFit . ',' . $scaleToFit;

				// split the operation up
				list($operationName, $operationArgs) = explode('|', $operation);

				$context           = array();
				$context['width']  = $scaleToFit;
				$context['height'] = $scaleToFit;

				if ($context['width'] == 0 && $context['height'] == 0) {
					// Don't put unknown size into context
					$context = array();
				}

				// Get the appropriate toolkit
				$mimeType = $item->getMimeType();

				list($ret, $toolkit, $nextMimeType) = GalleryCoreApi::getToolkitByOperation($mimeType, $operationName);

				if ($ret) {
					return $ret;
				}

				if (!isset($toolkit)) {
					return GalleryCoreApi::error(ERROR_UNSUPPORTED_OPERATION, __FILE__, __LINE__, "$operationName $mimeType");
				}

				// Perform the operation
				list($ret, $outputMimeType, $context) = $toolkit->performOperation(
					$mimeType,
					$operationName,
					$originalPath,
					$destination,
					explode(',', $operationArgs),
					$context
				);

				if ($ret) {
					return $ret;
				}
			}

			// save new file names for addition to zip
			if ($archive->add($destination, PCLZIP_OPT_REMOVE_PATH, $stripPath) == 0) {
				@$platform->unlink($zipFile);
				@$platform->unlink($zipBase);
				@$platform->rmdir($zipDir);

				return GalleryCoreApi::error(
					ERROR_PLATFORM_FAILURE,
					__FILE__,
					__LINE__,
					$archive->errorInfo(true)
				);
			}

			@$platform->unlink($destination);
			@$platform->rmdir(dirname($destination));
		}

		// Update progress bar
		$message = $module->translate(
			array(
				'text' => 'Finishing off...',
			)
		);
		$templateAdapter->updateProgressBar($heading, $message, 1);
		$finalFile = $gallery->getConfig('data.gallery.tmp') . $transactionId . '.zip';
		@$platform->unlink($finalFile);
		@$platform->rename($zipFile, $finalFile);
		@$platform->unlink($zipFile);
		@$platform->unlink($zipBase);
		@$platform->rmdir($zipDir);

		// Update download count etc
		list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($transactionId);

		if ($ret) {
			return $ret;
		}

		// Yay, we're done! Ensure the progress bar is at 100% and show a continue link.
		$heading = $module->translate(
			array(
				'text' => 'Hit Continue to download your file.',
			)
		);
		$message = $module->translate(
			array(
				'text' => 'Done.',
			)
		);
		$templateAdapter->updateProgressBar($heading, $message, 1);
		$redirect = array(
			'view'          => 'checkoutdownload.FetchDownload',
			'transactionId' => $transactionId,
			'verify1'       => $verify1,
			'verify2'       => $verify2,
		);

		$urlGenerator =& $gallery->getUrlGenerator();
		$templateAdapter->completeProgressBar($urlGenerator->generateUrl($redirect));

		return null;
	}
}
