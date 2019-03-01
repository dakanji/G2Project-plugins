<?php
/*
 * $RCSfile: KeywordAnalysis.inc,v $
 * $Revision: 1845 $
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
 * @package MRAnalysis
 * @author Mark Ridgwell <g2development@markridgwell.com>
 */


// Required Files\Classes
GalleryCoreApi::requireOnce('modules/mranalysis/classes/KeywordAnalysis.class');

class KeywordAnalysisController extends GalleryController {
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
			$itemId = $form['itemId'];
			$action = $form['changetype'];


			if (0 == strcasecmp($action, 'AddKeyword')) {
				$keyword = $form['keyword'];

				$ret = KeywordAnalysis::addkeyword($itemId, $keyword);

				if ($ret) {
					return array($ret, null);
				}
			} elseif (0 == strcasecmp($action, 'CorrectTypo')) {
				$orignal_keyword = $form['originalkeyword'];
				$replace_keyword = $form['replacekeyword'];

				$ret = KeywordAnalysis::correctkeyword($itemId, $orignal_keyword, $replace_keyword);

				if ($ret) {
					return array($ret, null);
				}
			} else {
				die('Unknown Action : ' . $action);
			}

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'mranalysis');

			if ($ret) {
				return array($ret, null);
			}
			list($ret, $isActive) = $module->isActive();

			if ($ret) {
				return array($ret, null);
			}


			$redirect['view'] = 'core.SiteAdmin';

			if ($isActive) {
				$redirect['subView'] = 'mranalysis.KeywordAnalysis';
				$status['saved']     = 1;
			}
		} elseif (isset($form['action']['cancel'])) {
			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'mranalysis.KeywordAnalysis';
		}

		if (!empty($redirect)) {
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'mranalysis.KeywordAnalysis';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}



/**
 * @package MRAnalysis
 * @subpackage KeywordAnalysisView
 */
class KeywordAnalysisView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$form['formName'] = 'mranalysis.KeywordAnalysis';

		$items = KeywordAnalysis::get_item_list();

		$template->setVariable('controller', 'mranalysis.KeywordAnalysis');
		$template->setVariable('baditems', $items);

		return array(
			null,
			array(
				'body' => 'modules/mranalysis/templates/KeywordAnalysis.tpl',
			),
		);
	}
}
