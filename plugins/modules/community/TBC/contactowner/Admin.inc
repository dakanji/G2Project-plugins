<?php

/*
 * $RCSfile: Admin.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
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
 * @author Darren Guiden <g2@pengs.co.uk>
 */

/**
 * @package ContactOwner
 * @subpackage UserInterface
 */
class AdminController extends GalleryController {
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
		if (isset($form['action']['save'])) {
			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'contactowner');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $smtpFrom) = GalleryCoreApi::getPluginParameter('module', 'core', 'smtp.from');

			if ($ret) {
				return array($ret, null);
			}

			$parameters = array(
				'toEmailAddress'      => $smtpFrom,
				'toSubject'           => $module->translate('Contact Received From Gallery Website'),
				'maxEmailsPerSession' => 3,
			);

			foreach (array_keys($parameters) as $key) {
				if (!empty($form[$key])) {
					if ($key == 'toEmailAddress') {
						if (!empty($form[$key])
							&& !GalleryUtilities::isValidEmailString($form[$key])
						) {
							$error[] = 'form[error][contactOwner][email]';
						} else {
							$parameters[$key] = $form[$key];
						}
					} elseif ($key == 'maxEmailsPerSession') {
						if ((string)$form[$key] !== (string)(int)$form[$key]) {
							$error[] = 'form[error][contactOwner][maxEmails][notnumeric]';
						} else {
							if (1 <= $form[$key] && $form[$key] <= 999) {
								$parameters[$key] = $form[$key];
							} else {
								$error[] = 'form[error][contactOwner][maxEmails][value]';
							}
						}
					} else {
						$parameters[$key] = $form[$key];
					}
				}
			}

			list($ret, $isActive) = $module->isActive();

			if ($ret) {
				return array($ret, null);
			}

			if (empty($error)) {
				foreach ($parameters as $key => $value) {
					$ret = $module->setParameter($key, $value);

					if ($ret) {
						return array($ret, null);
					}
				}

				$redirect['view'] = 'core.SiteAdmin';
				if ($isActive) {
					$redirect['subView'] = 'contactowner.Admin';
					$status['saved']     = 1;
				} else {
					$redirect['subView']  = 'core.AdminModules';
					$status['configured'] = 'Contact Owner';
				}
			}
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'contactowner.Admin';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'contactowner.Admin';
		}

		$results['status'] = $status;
		$results['error']  = $error;
		return array(null, $results);
	}
}

/**
 * @package ContactOwner
 * @subpackage UserInterface
 */
class AdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		if ($form['formName'] != 'AdminContactOwner') {
			$form['formName'] = 'AdminContactOwner';

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'contactowner');

			if ($ret) {
				return array($ret, null);
			}

			list($ret, $parameters) = GalleryCoreApi::fetchAllPluginParameters('module', 'contactowner');

			if ($ret) {
				return array($ret, null);
			}

			foreach (array('toEmailAddress', 'toSubject', 'maxEmailsPerSession') as $key) {
				$form[$key] = $parameters[$key];
			}

			if (empty($form['toEmailAddress'])) {
				list($ret, $form['toEmailAddress']) = GalleryCoreApi::getPluginParameter('module', 'core', 'smtp.from');

				if ($ret) {
					return array($ret, null);
				}
			}
		}

		$template->setVariable('controller', 'contactowner.Admin');

		return array(
			null,
			array(
				'body' => 'modules/contactowner/templates/Admin.tpl',
			),
		);
	}
}
