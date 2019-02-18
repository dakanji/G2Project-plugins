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
 * @version $Revision: 1200 $
 * @package CheckoutAuthorizeNet
 * @author Michael Rodríguez-Torrent <mike@themikecam.com>
 */

/**
 * This controller will send the payment details to Authorize.Net for processing
 *
 * @package CheckoutAuthorizeNet
 * @subpackage UserInterface
 */
class DetailsController extends GalleryController {
	/**
	 * A stored GalleryAuthorizeNet object.
	 *
	 * @var object $_authorizeNet
	 */
	public $_authorizeNet;

	/**
	 * Set a pre-existing GalleryAuthorizeNet to use in this instance.
	 *
	 * @param object authorizeNet an object of class GalleryAuthorizeNet
	 */
	public function setAuthorizeNet(&$authorizeNet) {
		$this->_authorizeNet =& $authorizeNet;
	}

	/**
	 * @see GalleryController::handleRequest()
	 */
	public function handleRequest(&$form) {
		global $gallery;
		$session =& $gallery->getSession();
		$error   = $status   = array();

		if (isset($form['action']['send'])) {
			// Basic input checks
			if (empty($form['custFirstName']) || empty($form['custLastName'])) {
				$error[] = 'form[error][custName][bad]';
			}

			if (empty($form['custEmail'])
				|| !GalleryUtilities::isValidEmailString($form['custEmail'])
			) {
				$error[] = 'form[error][custEmail][bad]';
			}

			if (empty($form['custPhone'])) {
				$error[] = 'form[error][custPhone][bad]';
			}

			if (!is_numeric($form['custCcNumber']) || strlen($form['custCcNumber']) != 16) {
				$error[] = 'form[error][custCcNumber][bad]';
			}

			if (empty($form['custCcMonth']) || $form['custCcMonth'] < 1
				|| $form['custCcMonth'] > 12
			) {
				$error[] = 'form[error][custCcMonth][bad]';
			}

			if (empty($form['custCcYear']) || date('Y') > $form['custCcYear']) {
				$error[] = 'form[error][custCcYear][bad]';
			}

			if (empty($form['custAddress'])) {
				$error[] = 'form[error][custAddress][bad]';
			}

			if (empty($form['custState'])) {
				$error[] = 'form[error][custState][bad]';
			}

			if (empty($form['custCity'])) {
				$error[] = 'form[error][custCity][bad]';
			}

			if (empty($form['custCcZip'])) {
				$error[] = 'form[error][custZip][bad]';
			}

			if (empty($form['custCcCountry'])) {
				$error[] = 'form[error][custCountry][bad]';
			}

			// TODO: Move to AuthorizeNetAim?
			// Perform checksum validation on routing number
			/* Check processing can be added relatively easily, given the time.
			$abaCode = $form['custAbaCode'];
			$checksum = (substr($abaCode, 0, 1) * 3 + substr($abaCode, 1, 1) * 7
							+ substr($abaCode, 2, 1) + substr($abaCode, 3, 1) * 3
							+ substr($abaCode, 4, 1) * 7 + substr($abaCode, 5, 1)
							+ substr($abaCode, 6, 1) * 3 + substr($abaCode, 7, 1) * 7
							+ substr($abaCode, 8, 1))
						% 10;
			*/
			// Note that this is only valid for US checks
			/*
			if (empty($abaCode) || !is_numeric($abaCode) || strlen($abaCode) != 9
					|| $checksum != 0) {
				$error[] = 'form[error][custAbaCode][bad]';
			}
			if (empty($form['custAccountNumber']) || !is_numeric($form['custAccountNumber'])) {
				$error[] = 'form[error][custAccountNumber][bad]';
			}
			*/

			// The input seems okay, let's move on
			if (empty($error)) {
				// Perform Authorize.Net processing
				if (empty($this->_authorizeNet)) {
					GalleryCoreApi::requireOnce(
						'modules/checkoutauthorizenet/classes/GalleryAuthorizeNet.class'
					);
					$auth = new GalleryAuthorizeNet();
				} else {
					$auth =& $this->_authorizeNet;
				}
				$auth->init();

				$auth->setFirstName($form['custFirstName']);
				$auth->setLastName($form['custLastName']);
				$auth->setEmail($form['custEmail']);
				$auth->setEmailCustomer('FALSE');
				$auth->setPhone($form['custPhone']);
				$auth->setCardNum($form['custCcNumber']);
				$auth->setExpDate($form['custCcMonth'] . '/' . $form['custCcYear']);
				$auth->setAddress($form['custAddress']);
				$auth->setCity($form['custCity']);
				$auth->setState($form['custState']);
				$auth->setZip($form['custCcZip']);
				$auth->setCountry($form['custCcCountry']);
				$auth->setAmount(unserialize($session->get('checkoutauthorizenet.total')));
				list($ret, $errorMsg) = $auth->process();

				if ($ret) {
					return array($ret->wrap(__FILE__, __LINE__), null);
				}
				// If Authorize.Net returned an error, save for output
				if (!empty($errorMsg)) {
					$error[] = 'form[error][authorizeNet][fail]';
					GalleryUtilities::putRequestVariable('authorizeNetError', $errorMsg);
				} else {
					// Write the customer information back into the original transaction
					$transactionId
																					         = unserialize($session->get('checkoutauthorizenet.transactionId'));
					list($ret, $transaction) = GalleryCoreApi::loadEntitiesById($transactionId);

					if ($ret) {
						return array($ret->wrap(__FILE__, __LINE__), null);
					}

					if (!isset($transaction)) {
						return array(
							GalleryCoreApi::error(ERROR_MISSING_OBJECT, __FILE__, __LINE__),
							null,
						);
					}
					// Get a write lock for the transaction and refresh from database
					list($ret, $lockId)
							= GalleryCoreApi::acquireWriteLock(array($transaction->getId()));

					if ($ret) {
						return array($ret->wrap(__FILE__, __LINE__), null);
					}
					list($ret, $transaction) = $transaction->refresh();

					if ($ret) {
						return array($ret->wrap(__FILE__, __LINE__), null);
					}
					// Set the customer information into the original transaction
					$transaction->setFirstName($form['custFirstName']);
					$transaction->setLastName($form['custLastName']);
					$transaction->setEmail($form['custEmail']);
					$transaction->setRecipientName($form['custRecipientName']);
					$transaction->setAddress1($form['custAddress']);
					$transaction->setAddress4($form['custState']);
					$transaction->setAddress5($form['custCity']);
					$transaction->setAddressZip($form['custCcZip']);
					$transaction->setAddressCountry($form['custCcCountry']);
					$transaction->setCustNotes($form['custComments']);
					$transaction->setPaymentPlugin('checkoutauthorizenet');
					$transaction->setFlagPaid(true);
					// Save the modified transaction
					$ret = $transaction->save();

					if ($ret) {
						return array($ret->wrap(__FILE__, __LINE__), null);
					}
					// Release our lock
					$ret = GalleryCoreApi::releaseLocks($lockId);

					if ($ret) {
						return array($ret->wrap(__FILE__, __LINE__), null);
					}

					$delegate['view'] = 'checkoutauthorizenet.Confirmation';
				}
			}
		}

		if (!empty($redirect) && empty($error)) {
			$results['redirect'] = $redirect;
		} elseif (!empty($delegate) && empty($error)) {
			$results['delegate'] = $delegate;
		} else {
			$results['delegate']['view'] = 'checkoutauthorizenet.Details';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * This view is for collecting the user's billing and shipping details
 *
 * @package CheckoutAuthorizeNet
 * @subpackage UserInterface
 */
class DetailsView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		// Load our default values if we didn't just come from this form.
		if ($form['formName'] != 'Details') {
			$form['formName'] = 'Details';

			// Get the current user information
			$user = $gallery->getActiveUser();
			// Populate the form if a user is logged in
			if ($user->getUserName() != 'guest') {
				list($form['custFirstName'], $form['custLastName']) = explode(' ', $user->getFullName(), 2);
				$form['custEmail']                                  = $user->getEmail();
			}
		}

		if (isset($form['error']['authorizeNet']['fail'])) {
			$form['error']['authorizeNet']['msg']
					= GalleryUtilities::getRequestVariables('authorizeNetError');
		}

		$year = date('Y');
		// Do credit cards really last this long?
		for ($x = 0; $x < 20; $x++) {
			$Details['ccYears'][] = $year + $x;
		}
		$template->setVariable('Details', $Details);
		$form['controller'] = 'checkoutauthorizenet.Details';
		$template->title(
			$gallery->i18n('Checkout') . '::' .
						 $gallery->i18n('Customer Details')
		);

		return array(
			null,
			array(
				'body' => 'modules/checkoutauthorizenet/templates/Details.tpl',
			),
		);
	}
}
