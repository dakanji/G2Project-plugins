<?php

/*
 * $RCSfile: Send.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2004 Bharat Mediratta
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
 *
 * @package ContactOwner
 * @subpackage UserInterface
 * @author Darren Guiden <g2@pengs.co.uk>
 */

/**
 * This controller will send the email to the gallery owner
 *
 * @package ContactOwner
 * @subpackage UserInterface
 *
 */
class SendController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest()
	 */
	public function handleRequest($form) {
		global $gallery;

		$session =& $gallery->getSession();

		if (isset($form['action']['send'])) {
			$form['custName']    = GalleryUtilities::htmlSafe($form['custName'], true);
			$form['custEmail']   = GalleryUtilities::htmlSafe($form['custEmail'], true);
			$form['custSubject'] = GalleryUtilities::htmlSafe($form['custSubject'], true);
			$form['custMessage'] = GalleryUtilities::htmlSafe($form['custMessage'], true);
			$resubmit            = false;
			$badname             = false;
			$bademail            = false;
			$badsubject          = false;
			$badmessage          = false;
			$emailsent           = false;

			if (strlen($form['custName']) < 1) {
				$resubmit = true;
				$badname  = true;
			}

			$session->put('contactowner.badname', serialize($badname));

			if (!preg_match('(^[-\w\.]+@([-a-z0-9]+\.)+[a-z]{2,4}$)i', $form['custEmail'])) {
				$resubmit = true;
				$bademail = true;
			}

			$session->put('contactowner.bademail', serialize($bademail));

			if (strlen($form['custSubject']) < 1) {
				$resubmit   = true;
				$badsubject = true;
			}

			$session->put('contactowner.badsubject', serialize($badsubject));

			if (strlen($form['custMessage']) < 1) {
				$resubmit   = true;
				$badmessage = true;
			}

			$session->put('contactowner.badmessage', serialize($badmessage));

			if ($resubmit == false) {
				$nextView = 'contactowner.Sent';

				// Get our plugin parameters
				list($ret, $toEmailAddress) = GalleryCoreApi::getPluginParameter('module', 'contactowner', 'toEmailAddress');

				if ($ret) {
					return array($ret, null);
				}

				list($ret, $toSubject) = GalleryCoreApi::getPluginParameter('module', 'contactowner', 'toSubject');

				if ($ret) {
					return array($ret, null);
				}

				list($ret, $maxEmailsPerSession) = GalleryCoreApi::getPluginParameter('module', 'contactowner', 'maxEmailsPerSession');

				if ($ret) {
					return array($ret, null);
				}

				$temp = $session->get('contactowner.emailcount');

				if (isset($temp)) {
					$emailCount = unserialize($temp);

					if ($emailCount < 1 || $emailCount > 999) {
						$emailCount = 1;
					} else {
						$emailCount += 1;
					}
				} else {
					$emailCount = 1;
				}

				if ($emailCount <= $maxEmailsPerSession) {
					// Begin constructing the data to pass to the email templates
					$email = array(
						'custName'    => $form['custName'],
						'custEmail'   => $form['custEmail'],
						'custSubject' => $form['custSubject'],
						'custMessage' => $form['custMessage'],
					);

					$email['boundary'] = uniqid('contactowner');
					$headers           = "MIME-Version: 1.0\n";
					$headers          .= "Content-Type: multipart/alternative;\n";
					$headers          .= '	boundary = "' . $email['boundary'] . '"';

					// This is where we actually send the email
					$ret = GalleryCoreApi::sendTemplatedEmail(
						'modules/contactowner/templates/Email.tpl',
						$email,
						$form['custEmail'],
						$toEmailAddress,
						$toSubject,
						$headers
					);

					if ($ret) {
						return array($ret, null);
					}

					$emailsent           = true;
					$form['custSubject'] = '';
					$form['custMessage'] = '';

					$session->put('contactowner.custsubject', serialize($form['custSubject']));
					$session->put('contactowner.custmessage', serialize($form['custMessage']));
					$session->put('contactowner.emailcount', serialize($emailCount));
				}
			} else {
				$nextView = 'contactowner.Contact';

				if (isset($form['custName'])) {
					$session->put('contactowner.custname', serialize($form['custName']));
				}

				if (isset($form['custEmail'])) {
					$session->put('contactowner.custemail', serialize($form['custEmail']));
				}

				if (isset($form['custSubject'])) {
					$session->put('contactowner.custsubject', serialize($form['custSubject']));
				}

				if (isset($form['custMessage'])) {
					$session->put('contactowner.custmessage', serialize($form['custMessage']));
				}
			}
		} elseif (isset($form['action']['email'])) {
			$nextView = 'contactowner.Contact';
		}

		$session->put('contactowner.emailsent', serialize($emailsent));

		// not really sure how to deal with these variables... but they're needed
		$error                          = array();
		$isError                        = false;
		$status                         = array();
		$results                        = array();
		$results['redirect']['subView'] = $nextView;
		$results['redirect']['view']    = 'core.UserAdmin';
		$results['status']              = $status;
		$results['error']               = $error;
		$results['isError']             = $isError;
		$errorView                      = '';

		return array(null, $results);
	}
}
