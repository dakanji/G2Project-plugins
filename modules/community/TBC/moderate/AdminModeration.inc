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
 * This controller will send the user to the chosen subView in the AdminModeration
 *
 * @package Moderation
 * @subpackage UserInterface
 * @author Emanuele Pedrona <e.pedrona@edronsoft.com>
 * @version $Revision: 1861 $ $Date: 2006/04/21 12:48:00 $
 */
class AdminModerationController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$results = $status = $error = array();

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $core) = GalleryCoreApi::fetchAllPluginParameters('module', 'core');

		if ($ret) {
			return array($ret, null);
		}

		$userid = $gallery->getActiveUserId();

		if (isset($form)) {
			if (isset($form['approved'])) {
				//Approve Item
				foreach ($form['approved'] as $itemtoapprove) {
					$itemtoapprove_int = intval($itemtoapprove);
					//Remove Item Permissions
					$ret = GalleryCoreApi::removeItemPermissions($itemtoapprove_int);

					if ($ret) {
						return array($ret, null);
					}

					//Add my permissions
					$ret = GalleryCoreApi::addGroupPermission($itemtoapprove_int, $core['id.adminGroup'], 'core.all');

					if ($ret) {
						return array($ret, null);
					}

					$viewPermissions = array('core.viewAll');

					//          switch ($module['view']) {
					//              case 'everybody':
					$groupId = $core['id.everybodyGroup'];
					//                  break;
					/*              case 'allusers':
					$groupId = $core['id.allUserGroup'];
					break;
					} */
					if (isset($groupId)) {
						foreach ($viewPermissions as $permission) {
							$ret = GalleryCoreApi::addGroupPermission($itemtoapprove_int, $groupId, $permission);

							if ($ret) {
								return array($ret, null);
							}
						}
					}

					//check if parent item is root album for user
					list($ret, $thisEntity) = GalleryCoreApi::loadEntitiesById($itemtoapprove_int);

					if ($ret) {
						return array($ret, null);
					}

					list($ret, $parentEntity) = GalleryCoreApi::loadEntitiesById($thisEntity->getParentId());

					if ($ret) {
						return array($ret, null);
					}

					/*          list ($ret, $rootEntity) = GalleryCoreApi::loadEntitiesById($parentEntity->getParentId());
					if ($ret) {
					return array($ret, null);
					}

					list ($ret, $rootId) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.rootAlbum');
					if ($ret) {
					return array($ret, null);
					}

					if ($rootEntity->getId() == $rootId) { */

					$ret = GalleryCoreApi::addGroupPermission($parentEntity->getId(), $groupId, $permission);

					if ($ret) {
						return array($ret, null);
					}

					//          }
					/* Check if the comments module is active */        //j0yst1ck mod
					list($ret, $moduleStatus) = GalleryCoreApi::fetchPluginStatus('module');

					if ($ret) {
						return array($ret, null);
					}

					if (isset($moduleStatus['comment']) && !empty($moduleStatus['comment']['active'])
						&& $moduleStatus['comment']['active']
					) {
						$ret = GalleryCoreApi::addGroupPermission($itemtoapprove_int, $core['id.everybodyGroup'], array('comment.view'));

						if ($ret) {
							return array($ret, null);
						}

						$ret = GalleryCoreApi::addGroupPermission($itemtoapprove_int, $core['id.allUserGroup'], array('comment.add'));

						if ($ret) {
							return array($ret, null);
						}
					}

					//get OwnerId
					$query = 'SELECT [::ownerId] FROM [ModerateMap] WHERE [::itemId] = ?';

					list($ret, $searchResults) = $gallery->search($query, array($itemtoapprove));

					if ($ret) {
						return array($ret, null);
					}

					if ($result = $searchResults->nextResult()) {
						$myuserid = $result[0];
					} else {
						$myuserid = $userid;
					}

					//add owner permissions
					$ret = GalleryCoreApi::addUserPermission($itemtoapprove_int, $myuserid, 'core.all');

					if ($ret) {
						return array($ret, null);
					}

					//remove the ability to edit permissions
					$ret = GalleryCoreApi::removeUserPermission($itemtoapprove_int, $myuserid, 'core.changePermissions');

					if ($ret) {
						return array($ret, null);
					}

					//remove from ToApprove table in database
					$ret = GalleryCoreApi::removeMapEntry(
						'ModerateMap',
						array(
							'itemId' => $itemtoapprove_int,
						)
					);

					if ($ret) {
						return array($ret, null);
					}
				}
			}

			if (isset($form['deleted'])) {
				foreach ($form['deleted'] as $itemtodelete) {
					$itemtodelete = intval($itemtodelete);

					$ret = GalleryCoreApi::deleteEntityById($itemtodelete);
					/*		if ($ret) {
					return array($ret, null);
					}

					 */
					//remove from ToApprove table in database
					$ret = GalleryCoreApi::removeMapEntry(
						'ModerateMap',
						array(
							'itemId' => $itemtodelete,
						)
					);

					if ($ret) {
						return array($ret, null);
					}
				}
			}

			$redirect['view']    = 'core.SiteAdmin';
			$redirect['subView'] = 'moderate.AdminModeration';
			$results['redirect'] = $redirect;
		} else {
			$results['delegate']['view']    = 'core.SiteAdmin';
			$results['delegate']['subView'] = 'moderate.AdminModeration';
		}

		$results['status'] = $status;
		$results['error']  = $error;

		return array(null, $results);
	}
}

/**
 * This view will show items not yet approved
 */
class AdminModerationView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		//$form['formName'] = 'Moderate';
		//add images IDs
		$images = array();

		$query = 'SELECT [::itemId] FROM [ModerateMap]';

		list($ret, $searchResults) = $gallery->search($query);

		if ($ret) {
			return array($ret, null);
		}

		while ($t1row = $searchResults->nextResult()) {
			$images[] = $t1row[0];
		}

		$imagesCont = 0;

		$template->setVariable('images', $images);
		$template->setVariable('imagesCont', $imagesCont);
		$template->setVariable('controller', 'moderate.AdminModeration');

		return array(
			null,
			array(
				'body' => 'modules/moderate/templates/AdminModeration.tpl',
			),
		);
	}
}
