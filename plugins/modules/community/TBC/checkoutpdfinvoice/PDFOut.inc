<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
 * This view produces pdf invoices for Checkout transactions
 *
 * @package checkoutPDFInvoice
 * @author Alec Myers <alec@alecmyers.com>
 * @version $Revision: 1700 $
 */
class PDFOutView extends GalleryView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	/**
	 * @see GalleryView::renderImmediate
	 */
	public function renderImmediate($status, $error) {
		GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutHelper.class');
		GalleryCoreApi::requireOnce('modules/checkout/classes/CheckoutTransactionHelper.class');
		GalleryCoreApi::requireOnce('modules/core/classes/helpers/GalleryDerivativeHelper_simple.class');
		GalleryCoreApi::requireOnce('modules/checkoutpdfinvoice/lib/fpdf/fpdf.php');
		GalleryCoreApi::requireOnce('modules/checkoutpdfinvoice/lib/fpdf/fpdi.php');

		global $gallery;

		// Make sure user is adminstrator
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return $ret;
		}

		list($ret, $csymbol) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'csymbol');

		if ($ret) {
			return $ret;
		}

		$csymbol = PDFOutView::fString($csymbol);

		list($ret, $cformat) = GalleryCoreApi::getPluginParameter('module', 'checkout', 'cformat');

		if ($ret) {
			return $ret;
		}

		// Figure out which item we are talking about
		$transactionId = (int)GalleryUtilities::getRequestVariables('transactionId');

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

		// Perform security check
		$verify1 = GalleryUtilities::getRequestVariables('verify1');
		$verify2 = GalleryUtilities::getRequestVariables('verify2');

		if ($verify1 != $transaction->getDate() || $verify2 != $transaction->getSecurity()) {
			return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
		}

		// get the transaction items
		list($ret, $itemIds) = CheckoutTransactionHelper::fetchTransactionItemIds($transactionId);

		if ($ret) {
			return $ret;
		}

		$lineHeight   = 3.5;
		$leftLine     = 10;
		$rightLine    = 200;
		$leftMargin   = 10;
		$rightMargin  = 10;
		$firstItemY   = 105;
		$itemYinc     = 25;
		$thumbWidth   = 30;
		$thumbHeight  = 23;
		$itemsPerPage = 7;
		$col1X        = 15;
		$col2X        = 50;
		$col2Width    = 35;
		$col3X        = 90;
		$col3Width    = 35;
		$col4X        = 130;
		$col4Width    = 100;
		$col5XRight   = 160;
		$col5Width    = 100;
		$col6XRight   = 190;
		$col6Width    = 100;
		$commentY     = 267;

		// initiate FPDI
		$pdf = new FPDI();

		// add a page
		$pdf->AliasNbPages();

		// set the sourcefile
		$pdf->setSourceFile('modules/checkoutpdfinvoice/templates/pdf/invoice_master.pdf');

		// import page 1
		$tplIdx = $pdf->importPage(1);

		$pdf->SetFont('Arial');
		$pdf->SetTextColor(0, 0, 0);
		$pdf->SetFontSize(9.5);

		// split the items into groups to display on each page
		$itemGroups = array_chunk($itemIds, $itemsPerPage);

		foreach ($itemGroups as $myGroup) {
			// start a new page
			$pdf->AddPage();
			$pdf->useTemplate($tplIdx, 0, 0);

			PDFOutView::writeHeader($transaction, $pdf);

			$pdf->SetY($firstItemY);

			// items
			foreach ($myGroup as $myItemId) {
				$curY = $pdf->GetY();

				list($ret, $transactionItem) = GalleryCoreApi::loadEntitiesById($myItemId);

				if ($ret) {
					return $ret;
				}

				if (!isset($transactionItem)) {
					return GalleryCoreApi::error(ERROR_MISSING_OBJECT);
				}

				$masterId = $transactionItem->getItem();

				list($ret, $thumbs) = GalleryDerivativeHelper_simple::fetchThumbnailsByItemIds(array($masterId));

				if ($ret) {
					return $ret;
				}

				$thumb = $thumbs[$masterId];

				if (method_exists($thumb, 'fetchPath')) {
					list($ret, $path) = $thumb->fetchPath();

					if (!$ret and $path) {
						$printWidth = $thumbHeight * $thumb->getWidth() / $thumb->getHeight();

						$pdf->Image($path, $col1X + ($thumbWidth - $printWidth) / 2, $curY - $thumbHeight / 2, 0, $thumbHeight, 'JPEG');
					}
				}

				$pdf->SetXY($col2X, $curY);

				$itemName = PDFOutView::fString($transactionItem->getItemName());

				$pdf->MultiCell($col2Width, $lineHeight, $itemName, 0, 'L');
				$pdf->SetXY($col3X, $curY);

				$productName = PDFOutView::fString($transactionItem->getProductName());
				$notes       = PDFOutView::fString($transactionItem->getNotes());

				$pdf->MultiCell($col3Width, $lineHeight, $productName . "\n" . $notes, 0, 'L');
				$pdf->SetXY($col4X, $curY);
				$pdf->MultiCell($col4Width, $lineHeight, 'Qty: ' . $transactionItem->getQty(), 0, 'L');
				$pdf->SetXY($col5XRight - $col5Width, $curY);
				$pdf->MultiCell($col5Width, $lineHeight, '@ ' . sprintf($cformat, $transactionItem->getPriceEach()), 0, 'R');
				$pdf->SetXY($col6XRight - $col6Width, $curY);
				$pdf->MultiCell($col6Width, $lineHeight, $csymbol . sprintf($cformat, $transactionItem->getPriceTotal()), 0, 'R');
				$pdf->SetY($curY + $itemYinc);
			}
		}

		// if the last page was full or nearly full, start a new page
		$itemGroups = array_reverse($itemGroups);

		if (count($itemGroups[0]) + 1 >= $itemsPerPage) {
			$pdf->AddPage();
			$pdf->useTemplate($tplIdx, 0, 0);

			PDFOutView::writeHeader($transaction, $pdf);

			$pdf->SetY($firstItemY);
		}

		// discount
		$curY     = $pdf->GetY();
		$discount = $transaction->getDiscount();

		if ($discount) {
			$curY = $pdf->GetY();

			$pdf->SetXY($col3X, $curY);

			$discountDescription = PDFOutView::fString($gallery->i18n('Discount:') . ' ' . $transaction->getDiscountDescription());

			$pdf->MultiCell($col3Width + $col4Width, $lineHeight, $discountDescription, 0, 'L');
			$pdf->SetXY($col6XRight - $col6Width, $curY);
			$pdf->MultiCell($col6Width, $lineHeight, '-' . $csymbol . sprintf($cformat, $discount), 0, 'R');
			$pdf->SetY($curY + $itemYinc / 4);
		}

		// postage
		$curY = $pdf->GetY();

		$pdf->SetXY($col3X, $curY);

		$postageType = PDFOutView::fString($gallery->i18n('Postage and Packing:') . ' ' . $transaction->getPostageType());

		$pdf->MultiCell($col3Width + $col4Width, $lineHeight, $postageType, 0, 'L');
		$pdf->SetXY($col6XRight - $col6Width, $curY);
		$pdf->MultiCell($col6Width, $lineHeight, $csymbol . sprintf($cformat, $transaction->getPostage()), 0, 'R');

		// draw line
		$pdf->SetY($curY + $itemYinc / 2);

		$curY = $pdf->GetY();

		$pdf->Line($leftLine, $curY, $rightLine, $curY);

		// Order Total
		$pdf->SetFont('Arial', 'B');
		$pdf->SetY($curY + $itemYinc / 2);

		$curY = $pdf->GetY();

		$pdf->SetXY($col3X, $curY);

		$OTA = PDFOutView::fString($gallery->i18n('Order Total Amount'));

		$pdf->MultiCell($col3Width, $lineHeight, $OTA, 0, 'L');
		$pdf->SetXY($col6XRight - $col6Width, $curY);
		$pdf->MultiCell($col6Width, $lineHeight, $csymbol . sprintf($cformat, $transaction->getAmount()), 0, 'R');

		// Paid thanks
		if ($transaction->getFlagCleared()) {
			$pdf->SetY($curY + $lineHeight * 2);
			$pdf->SetFont('Arial', 'B');

			$curY           = $pdf->GetY();
			$paymentCleared = PDFOutView::fString($gallery->i18n('Payment has cleared - thank you.'));

			$pdf->Text($col3X, $curY, $paymentCleared);
		}

		// Comments
		$pdf->SetLeftMargin($leftMargin);
		$pdf->SetRightMargin($rightMargin);

		// back up the page
		$pdf->SetY($commentY);
		$pdf->SetFont('Arial');
		$pdf->SetFontSize(8.5);

		$curY = $pdf->GetY();

		$pdf->SetX($leftMargin);

		$commentText = preg_replace('/\s\s+/', ' ', $transaction->getCustNotes() . $transaction->getCustComments());
		$commentText = PDFOutView::fString($commentText);

		$pdf->Write($lineHeight, $commentText);

		// send the output
		$pdf->SetDisplayMode('fullpage');
		$pdf->Output("invoice_$transactionId.pdf", 'I');

		return null;
	}

	public function writeHeader($transaction, &$pdf) {
		global $gallery;

		$lineHeight = 3.5;

		// put the date
		$pdf->Text(156, 49, strftime('%d %b %Y', $transaction->getDate()));

		// order reference
		$pdf->Text(156, 73, $transaction->getId());

		// order placed by
		$placedBy = PDFOutView::fString($transaction->getFirstName() . ' ' . $transaction->getLastName());

		$pdf->Text(156, 80, $placedBy);

		// page
		$pdf->Text(156, 87, $pdf->PageNo() . ' / {nb}');

		// addressee
		$line1 = PDFOutView::fString($transaction->getRecipientName());

		if (empty($line1)) {
			$line1 = $placedBy;
		}

		if ($pdf->PageNo() == 1) {
			$addressblock = $line1 . "\n" .
			trim($transaction->getAddress1()) . "\n" .
			trim($transaction->getAddress2()) . "\n" .
			trim($transaction->getAddress3()) . "\n" .
			trim($transaction->getAddress4()) . "\n" .
			trim($transaction->getAddress5()) . "\n" .
			trim($transaction->getAddressZip()) . "\n" .
			trim($transaction->getAddressCountry());
		} else {
			$addressblock = $gallery->i18n('(continued)');
		}

		$addressblock = PDFOutView::fString($addressblock);

		$pdf->SetLeftMargin(40);
		$pdf->SetXY(40, 55);
		$pdf->Write($lineHeight, $addressblock);
	}

	public function fString($string) {
		// fpdf does not support utf-8 :-(
		$string = trim($string);

		if (function_exists('iconv')) {
			$string = iconv('UTF-8', 'windows-1252', $string);
		} else {
			$string = utf8_decode($string);
		}

		$string = html_entity_decode($string);

		return $string;
	}
}
