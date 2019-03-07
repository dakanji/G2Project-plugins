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
 * Handle input from our sample page
 * @package Addviaemail
 * @subpackage UserInterface
 * @author Brian Janice <cobalt027@users.sourceforge.net>
 * @version $Revision: 898 $
 */
class AddviaemailpageController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$itemId   = GalleryUtilities::getRequestVariables('itemId');
		$redirect = $status = $error = array();

		list($ret, $moduleParams) = GalleryCoreApi::fetchAllPluginParameters(
			'module',
			'addviaemail'
		);

		if ($ret) {
			return $ret;
		}

		$block               =& $smarty->_tpl_vars['block'];
		$redirect['view']    = 'addviaemail.Addviaemailpage';
		$redirect['itemId']  = (int)$itemId;
		$status['added']     = 1;
		$results['status']   = $status;
		$results['error']    = $error;
		$results['redirect'] = $redirect;

		return array(null, $results);
	}
}

/**
 * View for Addviaemailpage
 */
class AddviaemailpageView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		// Load our item
		list($ret, $item) = $this->_getItem();

		if ($ret) {
			return array($ret, null);
		}

		$Addviaemail         = array();
		$Addviaemail['item'] = (array)$item;

		GalleryCoreApi::requireOnce('modules/addviaemail/classes/AddviaemailHelper.class');

		// list ($ret, $Addviaemail['value']) = AddviaemailHelper::getItemValue($item->getId());
		list($ret, $Addviaemail) = AddviaemailHelper::processEmails();

		if ($ret) {
			$template->setVariable('failure', '1');
			$template->setVariable('failureMsg', $ret->getErrorMessage());

			//return array($ret, null);
		}

		$template->setVariable('Addviaemail', $Addviaemail);
		$template->setVariable('newItems', count($Addviaemail['newItems']));

		return array(
			null,
			array(
				'body' => 'modules/addviaemail/templates/Addviaemail.tpl',
			),
		);
	}
}
