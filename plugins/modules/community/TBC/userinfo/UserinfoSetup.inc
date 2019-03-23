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
 * Handle input from our Setup Page
 *
 * @package Userinfo
 * @subpackage UserInterface
 *
 */
class UserinfoSetupController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest()
	 */
	public function handleRequest($form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$status  = $error  = array();
		$canSave = !empty($form['action']) &&
		array_intersect(array_keys($form['action']), array('save1', 'emailTest'));

		if ($canSave) {
			if ($form['action'] = 'save1') {
				$tableName3 = 'UserinfoSetupMap';
				$select3    = array(
					'uiPerPage',
					'uiDefaultSortColumn',
					'uiDefaultSortOrder',
					'uiDateFormat',
				);

				list($ret, $searchResults3) = GalleryCoreApi::getMapEntry($tableName3, $select3);

				while ($result3 = $searchResults3->nextResult()) {
					$data['result'] = array(
						'uiPerPage'           => $result3[0],
						'uiDefaultSortColumn' => $result3[1],
						'uiDefaultSortOrder'  => $result3[2],
						'uiDateFormat'        => $result3[3],
					);
				}

				for ($i = 1; $i <= count($data['result']); $i++) {
					$uiPerPage           = $data['result']['uiPerPage'];
					$uiDefaultSortColumn = $data['result']['uiDefaultSortColumn'];
					$uiDefaultSortOrder  = $data['result']['uiDefaultSortOrder'];
					$uiDateFormat        = $data['result']['uiDateFormat'];
				}

				if (!empty($form['uiPerPage'])) {
					if ($form['uiPerPage'] == $uiPerPage) {
						$input1 = $uiPerPage;
					} else {
						$input1 = $form['uiPerPage'];
					}
				}

				if (!empty($form['uiDefaultSortColumn'])) {
					if ($form['uiDefaultSortColumn'] == $uiDefaultSortColumn) {
						$input2 = $uiDefaultSortColumn;
					} else {
						$input2 = $form['uiDefaultSortColumn'];
					}
				}

				if (!empty($form['uiDefaultSortOrder'])) {
					if ($form['uiDefaultSortOrder'] == $uiDefaultSortOrder) {
						$input3 = $uiDefaultSortOrder;
					} else {
						$input3 = $form['uiDefaultSortOrder'];
					}
				}

				if (!empty($form['uiDateFormat'])) {
					if ($form['uiDateFormat'] == $uiDateFormat) {
						$input4 = $uiDateFormat;
					} else {
						$input4 = $form['uiDateFormat'];
					}
				}
			}
		}

		if ($canSave && empty($error)) {
			if ($form['action'] = 'save1') {
				$match['rowId'] = '1';

				$ret = GalleryCoreApi::updateMapEntry(
					$tableName3,
					$match,
					array(
						'uiPerPage'           => $input1,
						'uiDefaultSortColumn' => $input2,
						'uiDefaultSortOrder'  => $input3,
						'uiDateFormat'        => $input4,
					)
				);

				if ($ret) {
					return array($ret, null);
				}

				$status['save1'] = 1;
			}

			$status['saved']     = 1;
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'userinfo.UserinfoSetup';
		} elseif (isset($form['action']['reset'])) {
			// Redirect back to the same view to reset the form
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'userinfo.UserinfoSetup';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'userinfo.UserinfoSetup';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * This view will show setup options for this module
 * @package Userinfo
 * @subpackage UserInterface
 * @author Charles Knowlton charles2007@users.sourceforge.net
 * @version $Revision$
 */
class UserinfoSetupView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$UserinfoSetup = array();

		if ($form['formName'] != 'UserInfoTab') {
			// Load up our form data
			$tableName = 'UserinfoSetupMap';
			$select    = array(
				'uiPerPage',
				'uiDefaultSortColumn',
				'uiDefaultSortOrder',
				'uiDateFormat',
			);

			$UserinfoSetup['data'] = array();

			list($ret, $searchResults) = GalleryCoreApi::getMapEntry($tableName, $select);

			while ($result = $searchResults->nextResult()) {
				$UserinfoSetup['data']['result'] = array(
					'uiPerPage'           => $result[0],
					'uiDefaultSortColumn' => $result[1],
					'uiDefaultSortOrder'  => $result[2],
					'uiDateFormat'        => $result[3],
				);
			}

			for ($i = 1; $i <= count($UserinfoSetup['data']['result']); $i++) {
				$form['uiPerPage']           = $UserinfoSetup['data']['result']['uiPerPage'];
				$form['uiDefaultSortColumn'] = $UserinfoSetup['data']['result']['uiDefaultSortColumn'];
				$form['uiDefaultSortOrder']  = $UserinfoSetup['data']['result']['uiDefaultSortOrder'];
				$form['uiDateFormat']        = $UserinfoSetup['data']['result']['uiDateFormat'];
			}

			$mapName = 'UserinfoMap';

			list($ret, $mapInfo) = GalleryCoreApi::describeMap($mapName);

			if ($ret) {
				return $ret;
			}

			$data['mapInfo'] = (array_keys($mapInfo));

			for ($i = 1; $i <= count($data['mapInfo']); $i++) {
				if ($data['mapInfo'][0] == 'id') {
					$column1 = 'ID';
				} else {
					$column1 = 'BLANK';
				}

				if ($data['mapInfo'][1] == 'userId') {
					$column2 = 'User ID';
				} else {
					$column2 = 'BLANK';
				}

				if ($data['mapInfo'][2] == 'userName') {
					$column3 = 'User Name';
				} else {
					$column3 = 'BLANK';
				}

				if ($data['mapInfo'][3] == 'ipAddress') {
					$column4 = 'IP Address';
				} else {
					$column4 = 'BLANK';
				}

				if ($data['mapInfo'][4] == 'timeStamp') {
					$column5 = 'Time Stamp';
				} else {
					$column5 = 'BLANK';
				}

				if ($data['mapInfo'][5] == 'action') {
					$column6 = 'Action';
				} else {
					$column6 = 'BLANK';
				}
			}

			$UserinfoSetup['uiPerPageList']            = array('10', '25', '50', '100', '150', '200', '250');
			$UserinfoSetup['uiDefaultSortColumnList']  = (array_keys($mapInfo));
			$UserinfoSetup['uiDefaultSortColumnList2'] = array(
				$column1,
				$column2,
				$column3,
				$column4,
				$column5,
				$column6,
			);

			$UserinfoSetup['uiDefaultSortOrderList']  = array('ASC', 'DES');
			$UserinfoSetup['uiDefaultSortOrderList2'] = array('Ascending', 'Descending');
			$form['formName']                         = 'UserInfoTab';
		}

		$template->setVariable('controller', 'userinfo.UserinfoSetup');
		$template->setVariable('UserinfoSetup', $UserinfoSetup);

		return array(
			null,
			array(
				'body' => 'modules/userinfo/templates/UserinfoSetup.tpl',
			),
		);
	}
}
