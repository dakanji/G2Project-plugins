<?php

/*
 * $RCSfile: StatisticsAdminCursory.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Cursory Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Cursory Public License for more details.
 *
 * You should have received a copy of the GNU Cursory Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */
GalleryCoreApi::requireOnce('modules/statistics/StatisticsSiteAdmin.inc');/**

 * @version $Revision: 1036 $
 * @package BulkManage
 * @author Aidan Lister <aidan@php.net>
 */
class StatisticsSiteAdminCursory extends StatisticsSiteAdminPlugin {
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		// Total views
		$query = '
            SELECT SUM([GalleryItemAttributesMap::viewCount])
            FROM [GalleryItemAttributesMap]';

		list($ret, $results) = $gallery->search($query);

		if ($ret) {
			return $ret;
		}

		$count = 0;

		if ($result = $results->nextResult()) {
			$count = (int)$result[0];
		}

		$statistics['views'] = $count;

		// Total items
		$query = '
            SELECT COUNT([GalleryItem::id])
            FROM [GalleryItem]

            WHERE [GalleryItem::canContainChildren] = 0';

		list($ret, $results) = $gallery->search($query);

		if ($ret) {
			return $ret;
		}

		$count = 0;

		if ($result = $results->nextResult()) {
			$count = (int)$result[0];
		}

		$statistics['items'] = $count;

		// Total albums
		$query = '
            SELECT COUNT([GalleryItem::id])
            FROM [GalleryItem]

            WHERE [GalleryItem::canContainChildren] = 1';

		list($ret, $results) = $gallery->search($query);

		if ($ret) {
			return $ret;
		}

		$count = 0;

		if ($result = $results->nextResult()) {
			$count = (int)$result[0];
		}

		$statistics['albums'] = $count;

		// Total users
		list($ret, $groupid) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.allUserGroup');

		if ($ret) {
			return $ret;
		}

		list($ret, $count) = GalleryCoreApi::fetchUserCount(null, $groupid);

		if ($ret) {
			return $ret;
		}

		$statistics['users'] = $count + 1;

		$template->setVariable(
			'StatisticsSiteAdmin',
			array(
				'Cursory' => $statistics,
			)
		);

		// Controller
		$template->setVariable('controller', 'statistics.StatisticsSiteAdmin');

		return array(null, 'modules/statistics/templates/StatisticsSiteAdminCursory.tpl', 'modules_statistics');
	}

	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'statistics');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Cursory'));
	}

	public function isAppropriate() {
		return array(null, true);
	}
}
