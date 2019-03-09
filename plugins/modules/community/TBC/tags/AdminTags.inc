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
GalleryCoreApi::requireOnce('modules/tags/classes/TagsHelper.class');

/**
 * Administering the Tags for the Tags module.
 *
 * @package Tags
 * @subpackage UserInterface
 * @author Eric Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1060 $
 */
class AdminTagsController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$status = $error = $results = array();
		$target = array(
			'view'    => 'core.SiteAdmin',
			'subView' => 'tags.AdminTags',
		);

		if (isset($form['action']['add'])) {
			if (isset($form['TagNameAdd']) && $form['TagNameAdd'] != '') {
				$tagName = $form['TagNameAdd'];

				list($ret, $tagList) = TagsHelper::splitTags($tagName);

				if ($ret) {
					return array($ret, null, null);
				}

				foreach ($tagList as $tag) {
					$ret = TagsHelper::addNewTag(trim($tag));

					if ($ret) {
						return $ret;
					}
				}

				$status['added'] = 1;
			} else {
				$error[] = 'form[error][notagname]';
			}
		}

		if (isset($form['action']['delete'])) {
			$ret = TagsHelper::removeTag($form['TagNameDel']);

			if ($ret) {
				return array($ret, null);
			}

			$status['deleted'] = 1;
		}

		if (isset($form['action']['edit'])) {
			if (isset($form['TagNameNew']) and $form['TagNameNew'] != '') {
				$ret = TagsHelper::renameTag($form['TagNameDel'], trim($form['TagNameNew']));

				if ($ret) {
					return array($ret, null);
				}

				$status['renamed'] = 1;
			} else {
				$error[]           = 'form[error][nonewtagname]';
				$target['TagEdit'] = $form['TagNameDel'];
			}
		}

		$results['status'] = $status;
		$results['error']  = $error;
		if (empty($error)) {
			$route = 'redirect';
		} else {
			$route = 'delegate';
		}

		$results[$route] = $target;
		return array(null, $results);
	}
}

/**
 * Settings for the Tags module.
 */
class AdminTagsView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		list($ret, $tags) = TagsHelper::getTagListWithTagCount(true);

		if ($ret) {
			return array($ret, null);
		}

		$TagNameEdit = GalleryUtilities::getRequestVariables('TagEdit');

		$template->setVariable('Tags', $tags);
		$template->setVariable('TagNameEdit', $TagNameEdit);
		$template->javascript('modules/tags/js/AdminTags.js');

		return array(
			null,
			array(
				'body' => 'modules/tags/templates/AdminTags.tpl',
			),
		);
	}
}
