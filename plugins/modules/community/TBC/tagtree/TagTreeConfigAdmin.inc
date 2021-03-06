<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2009 Bharat Mediratta
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
 * A view to display when this module cannot be activated
 * @package TagTree
 * @subpackage UserInterface
 * @author Chad Parry <spam@chad.parry.org>
 * @version $Revision: 1720 $
 */
class TagTreeConfigAdminView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		GalleryCoreApi::requireOnce('modules/tagtree/classes/TagTreeCompatibilityHelper.class');

		$ret = GalleryCoreApi::assertUserIsSiteAdministrator();

		if ($ret) {
			return array($ret, null);
		}

		$form['requiredTagsMajorVersion'] = TAGTREE_REQUIRED_TAGS_MAJOR_VERSION;
		$form['requiredTagsMinorVersion'] = TAGTREE_REQUIRED_TAGS_MINOR_VERSION;

		return array(
			null,
			array(
				'body' => 'modules/tagtree/templates/TagTreeConfigAdmin.tpl',
			),
		);
	}
}
