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
 * @version $Revision: 1200 $
 * @package CheckoutAuthorizeNet
 * @author Michael Rodr�guez-Torrent <mike@themikecam.com>
 */

/**
 * This view is for collecting the user's billing and shipping details
 *
 * @package CheckoutAuthorizeNet
 * @subpackage UserInterface
 */
class ConfirmationView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$session =& $gallery->getSession();

		// First, empty out the shopping cart
		$event = GalleryCoreApi::newEvent('GalleryCheckout::emptyCart');

		list($ret) = GalleryCoreApi::postEvent($event);

		if ($ret) {
			return $ret;
		}

		// Get parameters from the checkout plugin
		list($ret, $postage)
					= GalleryCoreApi::getPluginParameter('module', 'checkout', 'postage');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $csymbol)
					= GalleryCoreApi::getPluginParameter('module', 'checkout', 'csymbol');

		if ($ret) {
			return array($ret, null);
		}

		// Get our plugin parameters
		list($ret, $fromAddress) = GalleryCoreApi::getPluginParameter(
			'module',
			'checkoutauthorizenet',
			'fromEmail'
		);

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $subject) = GalleryCoreApi::getPluginParameter(
			'module',
			'checkoutauthorizenet',
			'emailSubject'
		);

		if ($ret) {
			return array($ret, null);
		}

		// Begin constructing the data to pass to the email templates
		$email = array(
			'transactionId'
																				   => unserialize($session->get('checkoutauthorizenet.transactionId')),
			'items'             => unserialize($session->get('checkoutauthorizenet.items')),
			'product'           => unserialize($session->get('checkoutauthorizenet.product')),
			'price'             => unserialize($session->get('checkoutauthorizenet.price')),
			'paper'             => unserialize($session->get('checkoutauthorizenet.paper')),
			'postage'           => $postage,
			'csymbol'           => $csymbol,
			'total'             => unserialize($session->get('checkoutauthorizenet.total')),
			'custFirstName'     => $form['custFirstName'],
			'custLastName'      => $form['custLastName'],
			'custEmail'         => $form['custEmail'],
			'custPhone'         => $form['custPhone'],
			'custAddress'       => $form['custAddress'],
			'custCity'          => $form['custCity'],
			'custState'         => $form['custState'],
			'custCcZip'         => $form['custCcZip'],
			'custCcCountry'     => $form['custCcCountry'],
			'custRecipientName' => $form['custRecipientName'],
			'custAddress1'      => $form['custAddress1'],
			'custAddress2'      => $form['custAddress2'],
			'custAddress3'      => $form['custAddress3'],
			'custAddress4'      => $form['custAddress4'],
			'custAddress5'      => $form['custAddress5'],
			'custZip'           => $form['custZip'],
			'custCountry'       => $form['custCountry'],
			'custComments'      => $form['custComments'],
						/*
						'custAbaCode' => $form['custAbaCode'],
						'custAccountNumber' => $form['custAccountNumber']
						*/
		);

		// Load the thumbnails
		list($ret, $thumbnailList)
					= GalleryCoreApi::fetchThumbnailsByItemIds(array_keys($email['items']));

		if ($ret) {
			return array($ret, null);
		}

		foreach ($thumbnailList as $thumbnail) {
			$email['thumbnails'][$thumbnail->getParentId()] = (array)$thumbnail;
		}

		$toAddresses = array($email['custEmail']);

		list($ret, $toAddresses[]) = GalleryCoreApi::getPluginParameter(
			'module',
			'checkoutauthorizenet',
			'storeEmail'
		);

		if ($ret) {
			return array($ret, null);
		}

		foreach ($toAddresses as $toAddress) {
			$email['boundary'] = uniqid('checkoutauthorizenet');
			$headers           = "MIME-Version: 1.0\n";
			$headers          .= "Content-Type: multipart/alternative;\n";
			$headers          .= '   boundary = "' . $email['boundary'] . '"';

			// This is where we actually send the email
			$ret = GalleryCoreApi::sendTemplatedEmail(
				'modules/checkoutauthorizenet/templates/ConfirmationEmail.tpl',
				$email,
				$fromAddress,
				$email['custEmail'],
				$subject,
				$headers
			);

			if ($ret) {
				return array($ret, null);
			}
		}

		$template->title(
			$gallery->i18n('Checkout') . '::' .
						 $gallery->i18n('Order Complete')
		);

		return array(
			null,
			array(
				'body' => 'modules/checkoutauthorizenet/templates/Confirmation.tpl',
			),
		);
	}
}
