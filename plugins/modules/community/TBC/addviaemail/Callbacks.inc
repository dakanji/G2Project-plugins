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
 * Callbacks for AddviaEmail
 * @package AddviaEmaail
 * @author Brian Janice <cobalt027@users.sourceforge.net>
 * @version $Revision: 896 $
 */
class AddviaEmailCallbacks {
	public function callback($params, &$smarty, $callback, $userId) {
		switch ($callback) {
			case 'AddviaEmailBlock':
				list($ret, $moduleParams) = GalleryCoreApi::fetchAllPluginParameters('module', 'addviaemail');

				if ($ret) {
					return $ret;
				}

				$block =& $smarty->_tpl_vars['block'];

				GalleryCoreApi::requireOnce('modules/addviaemail/classes/AddviaemailHelper.class');

				$mintutesBetweenChecks = $moduleParams['pollFloor'];
				$secondsBetweenChecks  = $mintutesBetweenChecks * 60;
				$timeOfLastCheck       = $moduleParams['timeOfLastCheck'];

				if ((time() - $timeOfLastCheck) > $secondsBetweenChecks) {
					list($ret, $params) = AddviaemailHelper::processEmails();

					if ($ret) {
						$block['addviaemail']['failure'] = 1;
					}

					$block['addviaemail']['checkperformed'] = '1';
				} else {
					$block['addviaemail']['checkperformed'] = '0';
				}

				list($ret, $moduleParams) = GalleryCoreApi::fetchAllPluginParameters('module', 'addviaemail');

				if ($ret) {
					return $ret;
				}

				$block['addviaemail']['timeoflastcheck'] = $moduleParams['timeOfLastCheck'];

				if (!empty($params['newItems'])) {
					$block['addviaemail']['emailsprocessed'] = count($params['newItems']);
				} else {
					$block['addviaemail']['emailsprocessed'] = 0;
				}
		}

		return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}
}
