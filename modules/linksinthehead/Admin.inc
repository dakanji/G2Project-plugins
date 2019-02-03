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
 * @package linksinthehead
 * @author andrabr <n0tablog.wordpress.com>
 */

/**
 * @package linksinthehead
 * @subpackage UserInterface
 */
class AdminController extends GalleryController
{

	//
	//     @see GalleryController::handleRequest
	//
	function handleRequest($form)
	{
		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
		if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		}

		$error  = array();
		$status = array();

		if (isset($form['action']['save'])) {

			//	Explicitly erase whatever was previously set

			list ($ret, $links_in_the_head) = GalleryCoreApi::fetchAllPluginParameters('module', 'linksinthehead');
			if ($ret) {
				return array($ret->wrap(__FILE__, __LINE__), null);
			}
			foreach ($links_in_the_head as $linkname => $redirectURL) {
				if (!(strpos($linkname, '_') === 0)) {
					$ret = GalleryCoreApi::removePluginParameter('module', 'linksinthehead', $linkname);
					if ($ret) {
						return array($ret->wrap(__FILE__, __LINE__), null);
					}
				}
			}


			for ($i = 0; $i < count($form['url']); $i++) {

				// Only write real links to the module parameters
				if ((!empty($form['linkname'][$i])) && (!empty($form['url'][$i]))) {

					// Strip malicious content if html markup allowed
					list ($ret, $markup) = GalleryCoreApi::getPluginParameter('module', 'core', 'misc.markup');
					if ($ret) {
						return array($ret->wrap(__FILE__, __LINE__), null);
					}
					if ($markup == 'html') {
						$form['linkname'][$i] = GalleryUtilities::htmlSafe($form['linkname'][$i], true);
					}

					$ret = GalleryCoreApi::setPluginParameter(
						'module', 'linksinthehead', $form['linkname'][$i], $form['url'][$i]);
					if ($ret) {
						return array($ret->wrap(__FILE__, __LINE__), null);
					}


				}

			}


			list ($ret, $module) = GalleryCoreApi::loadPlugin('module', 'linksinthehead');
			if ($ret) {
				return array($ret->wrap(__FILE__, __LINE__), null);
			}

			list ($ret, $isActive) = $module->isActive();
			if ($ret) {
				return array($ret->wrap(__FILE__, __LINE__), null);
			}

			$redirect['view'] = 'core.SiteAdmin';
			if ($isActive) {
				$redirect['subView'] = 'linksinthehead.Admin';
				$status['saved']     = 1;
			} else {
				$redirect['subView']  = 'core.AdminModules';
				$status['configured'] = 'linksinthehead';
			}

		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'linksinthehead.Admin';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'linksinthehead.Admin';
		}
		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * @package linksinthehead
 * @subpackage UserInterface
 */
class AdminView extends GalleryView
{

	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form)
	{
		global $gallery;

		if ($form['formName'] != 'Adminlinksinthehead') {
			/* Get module options */


			$form['formName'] = 'Adminlinksinthehead';


			list ($ret, $links_in_the_head) = GalleryCoreApi::fetchAllPluginParameters('module', 'linksinthehead');


			if ($ret) {
				return array($ret->wrap(__FILE__, __LINE__), null);
			}

			/* Create the relevant number of link entry areas.
	   * Minimum will be 1 items, or at least 1 more than
	   * we currently have configured
	   */

			$i = 0;
			foreach ($links_in_the_head as $linkname => $redirectURL) {
				if (!(strpos($linkname, '_') === 0)) {
					$form['linkname'][$i] = $linkname;
					$form['url'][$i]      = $redirectURL;
					$i++;
				}
			}
			$form['linkname'][$i] = "";
			$form['url'][$i]      = "";

		}


		$template->setVariable('controller', 'linksinthehead.Admin');

		return array(
			null,
			array('body' => 'modules/linksinthehead/templates/Admin.tpl'),
		);
	}
}
