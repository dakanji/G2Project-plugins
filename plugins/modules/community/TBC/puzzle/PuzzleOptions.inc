<?php

/*
 * $RCSfile: module.inc,v $
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
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @package Puzzle
 * @version $Revision: 1481 $ $Date: 2006/07/03 08:04:11 $
 * @author J. Mike Rollins <rollins@CamoTruck.Net>
 * @contrib azn_oohwee
 */

/**
 * CamoTruck.Net Puzzle Module
 *
 * This module provides support for adding puzzles to items
 *
 * @package Puzzle
 */class PuzzleOptions extends ItemEditOption {
	public function isAppropriate($item, $thumbnail) {
		return array(null, GalleryUtilities::isA($item, 'GalleryAlbumItem'));
	}

	public function handleRequestAfterEdit($form, $item, $preferred) {
		// examine the form checkbox value
		if (empty($form['PuzzleOptions']['enabled']) || ($form['PuzzleOptions']['enabled'] == 'off')) {
			$status = 'no';
		} elseif (isset($form['PuzzleOptions']['enabled']) && ($form['PuzzleOptions']['enabled'] == 'on')) {
			$status = 'yes';
		} else {        // Hmm... this should not happen
			$status = 'yes';
		}

		$ret = GalleryCoreApi::setPluginParameter('module', 'puzzle', 'enabled', $status, $item->getId());

		if ($ret) {
			return array($ret, null);
		}

		return array(null, null);
	}

	public function loadTemplate(&$template, $form, $item, $thumbnail) {
		list($ret, $enabled) = GalleryCoreApi::getPluginParameter('module', 'puzzle', 'enabled', $item->getId());

		if ($ret) {
			return array($ret, null);
		}

		if (empty($enabled)) {
			$PuzzleOptions['enabled'] = 'yes'; // default to yes
		} elseif ($enabled == 'yes') {
			$PuzzleOptions['enabled'] = 'yes';
		} elseif ($enabled == 'no') {
			$PuzzleOptions['enabled'] = 'no';
		} else {
			$PuzzleOptions['enabled'] = 'no'; // Hmm... this should not happen
		}

		$template->setVariable('PuzzleOptions', $PuzzleOptions);
		return array(
			null,
			'modules/puzzle/templates/PuzzleOptionsTemplate.tpl',
			'modules_puzzle',
		);
	}
}
