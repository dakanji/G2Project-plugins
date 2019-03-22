<?php

/*
 * $RCSfile: Contact.inc,v $
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
 * @version
 * @package ContactOwner
 * @subpackage UserInterface
 * @author Darren Guiden <g2@pengs.co.uk>
 * @author Tim Almdal <public@timalmdal.com>
 */

/**
 * This controller will send the email to the gallery owner
 */
class ContactController extends GalleryController {
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
			$emailsent           = false;
			$error               = $status               = $results               = array();

			if (strlen($form['custName']) < 1) {
				$error[] = 'form[error][contactOwner][name]';
			}

			if (empty($form['custEmail'])) {
				$error[] = 'form[error][contactOwner][email][required]';
			} elseif (!GalleryUtilities::isValidEmailString($form['custEmail'])) {
				$error[] = 'form[error][contactOwner][email][invalid]';
			}

			if (strlen($form['custSubject']) < 1) {
				$error[] = 'form[error][contactOwner][subject]';
			}

			if (strlen($form['custMessage']) < 1) {
				$error[] = 'form[error][contactOwner][message]';
			}

			// validate your form data, set $error values if applicable
			if (empty($error) && $captchaLevel != 'OFF') {
				list($ret, $pluginInstances) = GalleryCoreApi::getAllFactoryImplementationIds('GalleryValidationPlugin');

				if ($ret) {
					return array($ret, null);
				}

				foreach (array_keys($pluginInstances) as $pluginId) {
					list($ret, $pluginInstances[$pluginId]) = GalleryCoreApi::newFactoryInstanceById('GalleryValidationPlugin', $pluginId);

					if ($ret) {
						return array($ret, null);
					}
				}

				// Let each plugin do its verification
				foreach ($pluginInstances as $plugin) {
					list($ret, $pluginErrors, $continue) = $plugin->performValidation($form);

					if ($ret) {
						return array($ret, null);
					}

					$error = array_merge($error, $pluginErrors);

					if (!$continue) {
						break;
					}
				}
			}

			if (empty($error)) {
				$nextView = 'contactowner.Sent';

				// Get our plugin parameters
				list($ret, $parameters) = GalleryCoreApi::fetchAllPluginParameters('module', 'contactowner');

				if ($ret) {
					return array($ret, null);
				}

				$toEmailAddress      = $parameters['toEmailAddress'];
				$toSubject           = $parameters['toSubject'];
				$maxEmailsPerSession = $parameters['maxEmailsPerSession'];
				$emailCount          = 1;

				if ($session->exists('contactowner.emailcount')) {
					$emailCount = $session->get('contactowner.emailcount');

					if (1 <= $emailCount && $emailCount <= 999) {
						$emailCount += 1;
					}
				}

				if ($emailCount <= $maxEmailsPerSession) {
					// Begin constructing the data to pass to the email templates
					$boundary = uniqid('contactowner');
					$email    = array(
						'custName'    => $form['custName'],
						'custEmail'   => $form['custEmail'],
						'custSubject' => $form['custSubject'],
						'custMessage' => $form['custMessage'],
						'boundary'    => $boundary,
					);

					$headers = implode(
						"\r\n",
						array(
							'MIME-Version: 1.0',
							'Content-Type: multipart/alternative;',
							'	boundary = "' . $boundary . '"',
						)
					);

					// This is where we actually send the email
					$ret = GalleryCoreApi::sendTemplatedEmail(
						'modules/contactowner/templates/Email.tpl',
						$email,
						$form['custEmail'],
						// $toEmailAddress, $toSubject, $headers);
						$toEmailAddress,
						$toSubject
					);

					if ($ret) {
						return array($ret, null);
					}

					$emailsent = true;

					$session->remove('contactowner.custsubject');
					$session->remove('contactowner.custmessage');
					$session->put('contactowner.emailcount', $emailCount);
				}
			} else {
				$nextView = 'contactowner.Contact';
			}
		}

		$session->put('contactowner.emailsent', $emailsent);

		$returnType                      = empty($error) ? 'redirect' : 'delegate';
		$results[$returnType]['view']    = 'core.UserAdmin';
		$results[$returnType]['subView'] = $nextView;
		$results['status']               = $status;
		$results['error']                = $error;

		return array(null, $results);
	}
}

/**
 * This view is for collecting the user's details
 *
 * @package ContactOwner
 * @subpackage UserInterface
 */
class ContactView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$session =& $gallery->getSession();

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'contactowner');

		if ($ret) {
			return array($ret, null);
		}

		$plugins = array();

		list($ret, $parameters) = $module->fetchParameters();

		if ($ret) {
			return array($ret, null);
		}

		$captchaLevel = $parameters['validation.level'];

		if ($captchaLevel != 'OFF') {
			list($ret, $allPluginIds) = GalleryCoreApi::getAllFactoryImplementationIds('GalleryValidationPlugin');

			if ($ret) {
				return array($ret, null);
			}

			// Let each plugin load its template data
			foreach (array_keys($allPluginIds) as $pluginId) {
				list($ret, $plugin) = GalleryCoreApi::newFactoryInstanceById('GalleryValidationPlugin', $pluginId);

				if ($ret) {
					return array($ret, null);
				}

				list($ret, $data['file'], $data['l10Domain']) = $plugin->loadTemplate($form);

				if ($ret) {
					return array($ret, null);
				}

				if (isset($data['file'])) {
					$plugins[] = $data;
				}
			}
		}

		$template->setVariable('validationPlugins', $plugins);

		$maxEmailsPerSession = $parameters['maxEmailsPerSession'];

		if ($session->exists('contactowner.emailcount')) {
			$emailCount = $session->get('contactowner.emailcount');

			if ($emailCount < 1 || $emailCount > 999) {
				$emailCount = 1;
			} else {
				$emailCount += 1;
			}
		} else {
			$emailCount = 1;
		}

		$contactOwner = array();

		if ($emailCount <= $maxEmailsPerSession) {
			if (empty($form['formName'])) {
				$form['formName'] = 'ContactView';

				list($ret, $isAnonymousUser) = GalleryCoreApi::isAnonymousUser();

				if ($ret) {
					return array($ret, null);
				}

				// Populate the form if a user is logged in
				if (!$isAnonymousUser) {
					// Get the current user information
					$user              = $gallery->getActiveUser();
					$form['custName']  = $user->getFullName();
					$form['custEmail'] = $user->getEmail();
				}
			}

			$contactOwner['title'] = $module->translate('Contact::Enter Your Details');

			$template->setVariable('controller', 'contactowner.Contact');

			$body = 'modules/contactowner/templates/Contact.tpl';
		} else {
			$contactOwner['emailsent'] = false;
			$contactOwner['title']     = $gallery->translate('Contact::Email Failed');
			$body                      = 'modules/contactowner/templates/Sent.tpl';
		}

		$template->setVariable('ContactOwner', $contactOwner);

		return array(
			null,
			array(
				'body' => $body,
			),
		);
	}
}
