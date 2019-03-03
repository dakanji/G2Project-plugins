<?php
/*
 * $RCSfile: AlbumTimestampItemOption.inc,v $
 * $Revision: 1845 $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
 * ItemEditOption for automatically setting an albums timestamp based on its children
 * @package AlbumTimestamp
 * @subpackage UserInterface
 * @author Leo Bergolth <leo@strike.wu.ac.at>
 * @version $Revision: 0 $
 */
class AlbumTimestampItemOption extends ItemEditOption {
	/**
	 * @see ItemEditOption::isAppropriate
	 */
	public function isAppropriate($item, $thumbnail) {
		$isAlbum = $item->getCanContainChildren();

		list($ret, $hasPermission) = GalleryCoreApi::hasItemPermission($item->getId(), 'core.changePermissions');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $isAlbum && $hasPermission);
	}

	/**
	 * @see ItemEditOption::loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $item, $thumbnail) {
		GalleryCoreApi::requireOnce('modules/albumtimestamp/classes/AlbumTimestampHelper.class');

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'keyalbum');

		if ($ret) {
			return array($ret, null, null);
		}

		list($ret, $mode, $inheritedMode) = AlbumTimestampHelper::getItemInheritedParam('albumTimestampMode', $item->getId());

		list($ret, $newAlbumMode, $newInheritedMode) = AlbumTimestampHelper::getItemInheritedParam('newAlbumTimestampMode', $item->getId());

		$form['AlbumTimestampItemOption']['mode']         = isset($mode) ? $mode : 'inherited';
		$form['AlbumTimestampItemOption']['newAlbumMode'] = isset($newAlbumMode) ? $newAlbumMode : 'off';

		// AlbumTimestampHelper::log("LEO: loadTemplate: itemId=$itemId mode=$mode newAlbumMode=$newAlbumMode");
		$newText = $module->translate('Use the timestamp of the newest item in this album');
		$oldText = $module->translate('Use the timestamp of the oldest item in this album');

		$settingsList = array(
			'inherited' => '',
			'off'       => $module->translate('Do not set this albums timestamp automatically'),
			'newest'    => $newText,
			'oldest'    => $oldText,
		);

		$settingsList['inherited'] = '* '
		. $module->translate('Inherited Setting') . ': '
			. $settingsList[isset($inheritedMode) ? $inheritedMode : 'off']
		. ' *';

		$newSettingsList = array(
			'off'    => $module->translate('Do not set an explicit mode for new subalbums.'),
			'newest' => $newText,
			'oldest' => $oldText,
		);

		$template->setVariable(
			'AlbumTimestampItemOption',
			array(
				'settingsList'    => $settingsList,
				'newSettingsList' => $newSettingsList,
			)
		);

		return array(
			null,
			'modules/albumtimestamp/templates/AlbumTimestampItemOption.tpl',
			'modules_albumtimestamp',
		);
	}

	/**
	 * @see ItemEditOption::handleRequestAfterEdit
	 */
	public function handleRequestAfterEdit($form, &$item, &$preferred) {
		global $gallery;

		GalleryCoreApi::requireOnce('modules/albumtimestamp/classes/AlbumTimestampHelper.class');

		$itemId = $item->getId();

		$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.changePermissions');

		if ($ret) {
			return array($ret, null, null);
		}

		$error           = $warning           = array();
		$mode            = $form['AlbumTimestampItemOption']['mode'];
		$allowedSettings = array(
			'inherited' => 1,
			'off'       => 1,
			'newest'    => 1,
			'oldest'    => 1,
		);

		if (isset($mode, $allowedSettings[$mode])) {
			if ($mode == 'inherited') {
				$mode = null;
			}

			$gallery->debug('LEO: setAlbumTimestampMode: ' . $itemId . ', ' . $mode);

			$ret = AlbumTimestampHelper::setAlbumTimestampMode($item, $mode);

			if ($ret) {
				return array($ret, null, null);
			}
		}

		$newAlbumMode = $form['AlbumTimestampItemOption']['newAlbumMode'];
		AlbumTimestampHelper::log('LEO: set newAlbumTimestampMode: ' . $itemId . ', ' . $newAlbumMode);
		$allowedNewAlbumSettings = array(
			'off'    => 1,
			'newest' => 1,
			'oldest' => 1,
		);

		if (isset($newAlbumMode, $allowedNewAlbumSettings[$newAlbumMode])) {
			if ($newAlbumMode == 'off') {
				$newAlbumMode = null;
			}

			$gallery->debug('LEO: set newAlbumTimestampMode: ' . $itemId . ', ' . $newAlbumMode);

			list($ret, $oldValue) = AlbumTimestampHelper::setItemParam(
				'newAlbumTimestampMode',
				$newAlbumMode,
				$itemId
			);

			if ($ret) {
				return array($ret, null, null);
			}
		}

		if (isset($form['AlbumTimestampItemOption']['forceUpdate'])) {
			$ret = AlbumTimestampHelper::autoSetAlbumTimestamps($item, null, 1, null, 1);

			if ($ret) {
				return array($ret, null, null);
			}
		}

		return array(null, $error, $warning);
	}

	/**
	 * @see ItemEditOption::requiresProgressBar
	 */
	public function requiresProgressBar($form) {
		/* use progress bar since changing an albums timestamp mode also requires recursive
		updates of the subalbums timestamps */
		/* TODO: is this necessary?
		return isset($form['AlbumTimestampItemOption']['mode']);
		*/
		return isset($form['AlbumTimestampItemOption']['forceUpdate'])
		|| (($form['AlbumTimestampItemOption']['oldMode'] != $form['AlbumTimestampItemOption']['mode'])
		&& $form['AlbumTimestampItemOption']['mode'] != 'off');
	}
}
