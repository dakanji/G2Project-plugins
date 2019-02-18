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
 * @package AboutPage
 * @author Darren Guiden <g2@pengs.co.uk>
 */

/**
 * @package AboutPage
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
			return array($ret->wrap(__FILE__, __LINE__), null);
		}

		$error  = array();
		$status = array();

		if (isset($form['action']['save'])) {
			list($ret, $markup) = GalleryCoreApi::getPluginParameter('module', 'core', 'misc.markup');

			if ($ret) {
				return array($ret->wrap(__FILE__, __LINE__), null);
			}

			if ($markup == 'html') {
				// Strip malicious content if html markup allowed
				$form['message'] = GalleryUtilities::htmlSafe($form['message'], true);
			}

			$ret = GalleryCoreApi::setPluginParameter(
				'module',
				'aboutpage',
				'message',
				$form['message']
			);

			if ($ret) {
				return array($ret->wrap(__FILE__, __LINE__), null);
			}

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'aboutpage');

			if ($ret) {
				return array($ret->wrap(__FILE__, __LINE__), null);
			}
			list($ret, $isActive) = $module->isActive();

			if ($ret) {
				return array($ret->wrap(__FILE__, __LINE__), null);
			}

			$redirect['view'] = 'core.SiteAdmin';

			if ($isActive) {
				$redirect['subView'] = 'aboutpage.Admin';
				$status['saved']     = 1;
			} else {
				$redirect['subView']  = 'core.AdminModules';
				$status['configured'] = 'aboutpage';
			}
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'aboutpage.Admin';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'aboutpage.Admin';
		}
		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * @package AboutPage
 * @subpackage UserInterface
 */
class AdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'AdminAboutPage') {
			// Get module options
			list($ret, $form['message']) = GalleryCoreApi::getPluginParameter(
				'module',
				'aboutpage',
				'message'
			);

			if ($ret) {
				return array($ret->wrap(__FILE__, __LINE__), null);
			}

			$form['formName'] = 'AdminAboutPage';

			$template->setVariable('controller', 'aboutpage.Admin');

			return array(
				null,
				array(
					'body' => 'modules/aboutpage/templates/Admin.tpl',
				),
			);
		}
	}
}
