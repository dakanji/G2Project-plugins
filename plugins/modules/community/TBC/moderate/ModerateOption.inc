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
 * This ItemAddOption enforces moderation
 * Add the newly added item to "ToApprove" table in db.
 *
 * @package Moderation
 * @subpackage UserInterface
 * @author Emanuele Pedrona <e.pedrona@edronsoft.com>
 * @version $Revision: 1861 $ $Date: 2006/04/21 12:53:00 $
 */
class ModerateOption extends ItemAddOption {
	/**
	 * @see ItemAddOption::isAppropriate
	 */
	public function isAppropriate() {
		return array(null, true);
	}

	/**
	 * @see ItemAddOption::handleRequestAfterAdd
	 */
	public function handleRequestAfterAdd($form, $items) {
		global $gallery;

		$warnings = $errors = array();

		/**
		 * If the current user is admin do not start the moderation process, just return so the pic
		 * is added to the gallery immediately
		 */
		list($ret, $isAdmin) = GalleryCoreApi::isUserInSiteAdminGroup();

		if ($isAdmin) {
			return array(null, $errors, $warnings);
		}

		list($ret, $core) = GalleryCoreApi::fetchAllPluginParameters('module', 'core');

		if ($ret) {
			return array($ret, null, null);
		}

		for ($j = 0; $j < count($items); $j++) {
			$ret = GalleryCoreApi::removeItemPermissions($items[$j]->getId());

			if ($ret) {
				return array($ret, null, null);
			}

			$ret = GalleryCoreApi::addGroupPermission($items[$j]->getId(), $core['id.adminGroup'], 'core.all');

			if ($ret) {
				return array($ret, null, null);
			}

			list($ret, $childCounts) = GalleryCoreApi::fetchChildCounts(array($items[$j]->getParentId()));

			if ($ret) {
				return array($ret, null, null);
			}

			/*	if (!$childCounts) {
			$ret = GalleryCoreApi::removeItemPermissions($items[$j]->getParentId());

			if ($ret) {
			return array($ret, null, null);
			}

			$ret = GalleryCoreApi::addGroupPermission($items[$j]->getParentId(), $core['id.adminGroup'], 'core.all');

			if ($ret) {
			return array($ret, null, null);
			}

			$ret = GalleryCoreApi::addUserPermission($items[$j]->getParentId(), $items[$j]->getOwnerId(), 'core.all');

			if ($ret) {
			return array($ret, null, null);
			}
			}

			 */
			$itemid  = $items[$j]->getId();
			$ownerid = $items[$j]->getOwnerId();

			$ret = GalleryCoreApi::addMapEntry(
				'ModerateMap',
				array(
					'itemId'  => $itemid,
					'ownerId' => $ownerid,
				)
			);

			if ($ret) {
				return array($ret, null, null);
			}
		}

		return array(null, $errors, $warnings);
	}
}
