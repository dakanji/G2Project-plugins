<?php

/*
 * $RCSfile: AddComment.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
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
 * @package Puzzle
 * @version $Revision: 1660 $ $Date: 2006/07/03 08:04:11 $
 * @author J. Mike Rollins <rollins@CamoTruck.Net>
 * @contrib azn_oohwee
 */

/**
 * CamoTruck.Net Puzzle Module
 *
 * This module provides support for adding puzzles to items
 *
 * @package Puzzle
 */
class MakePuzzleView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */

	// use the first image that is greater than $LIMIT in size
	public $LIMIT = 480;

	// the max width and height for the applet to display
	// var $WIDTH  = 500;
	// var $HEIGHT = 400;
	public $WIDTH  = 600;
	public $HEIGHT = 400;

	public function findAppletItem($item) {
		global $gallery;

		$gallery->debug('<hr>(modules:puzzle) findAppletItemId: Beginning');

		// Record the itemId number
		$itemId = $item->getId();

		// Obtain a list of the permissions for this user
		list($ret, $permissions) = GalleryCoreApi::getPermissions($itemId);

		if ($ret) {
			return $ret;
		}

		// DEBUG Information
		$gallery->debug('<hr>');

		if (!empty($permissions)) {
			foreach (array_keys($permissions) as $key) {
				$gallery->debug("<br>(modules:puzzle) findAppletItemId: permissions[$key] = " . $permissions[$key]);
			}
		}

		// The images array will contain keys which indicate the relative
		// display size of the image
		$images = array();

		// if permissions allow us to view the source
		// then index this source in the images array
		if (isset($permissions['core.viewSource'])) {
			list($ret, $source) = GalleryCoreApi::fetchPreferredSource($item);

			if ($ret) {
				return array($ret, null, null);
			}

			$size = ($source->getWidth()) * ($source->getHeight());

			if ($size > 0) {
				$images[$size] = $source;
			}

			$gallery->debug(
				'<hr>(modules:puzzle) findAppletItemId: indexing item ' . $source->getId() .
							' (' . $source->getWidth() . 'x' . $source->getHeight() . ').'
			);
		}

		// if permissions allow us to viewResizes of the image
		// then index the resizes in the images array
		if (isset($permissions['core.viewResizes'])) {
			list($ret, $resizes) = GalleryCoreApi::fetchResizesByItemIds(array($itemId));

			if ($ret) {
				return $ret;
			}

			if (!empty($resizes[$itemId])) {
				foreach (array_keys($resizes[$itemId]) as $key) {
					$resizedImage = $resizes[$itemId][$key];
					$size         = ($resizedImage->getWidth()) * ($resizedImage->getHeight());

					if ($size > 0) {
						$images[$size] = $resizedImage;
					}

					$gallery->debug(
						'<br>(modules:puzzle) findAppletItemId: indexing item ' . $resizedImage->getId() .
									' (' . $resizedImage->getWidth() . 'x' . $resizedImage->getHeight() . ').'
					);
				}
			} // if(!empty)
		}

		// check to see if we found any images
		if (empty($images)) {
			return array(false, 0);
		}

		// we have an images[size] array populated with one or more images which we are allowed to see
		// sort the array by sizes from largest to smallest
		krsort($images);

		// default to the first element
		$targetImage = array_shift($images);

		// return if the targetImage was our only image
		if (empty($images)) {
			return array(true, $targetImage);
		}

		// cycle through the other images
		foreach (array_keys($images) as $size) {
			if (($this->LIMIT > $images[$size]->getWidth()) || ($this->LIMIT > $images[$size]->getHeight())) {
				// this image is too small, we will use the last one we remember
				break;
			}

			// this image good, remember it
			$targetImage = $images[$size];
		}

		return array(true, $targetImage);
	}

	//findAppletItemId
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		GalleryCoreApi::requireOnce('modules/puzzle/classes/PuzzleHelper.class');

		// Load our item
		list($ret, $item) = $this->_getItem();

		if ($ret) {
			return array($ret, null);
		}

		// find the parent album of this photo item.
		list($ret, $parentAlbum) = PuzzleHelper::findParentAlbum($item);

		if ($ret) {
			// cannot find parent album.
			// do not know how this can happen ... it is probably a bad, bad thing
			return array($ret, null);
		}

		$gallery->debug('<br>(modules:puzzle) loadTemplate: found album for item, album id=' . $parentAlbum->getId());

		// check the album for the 'enabled' parameter
		list($ret, $enabled) = PuzzleHelper::isEnabledForAlbum($parentAlbum);

		if ($ret) {
			// do not know what can cause this error
			return array($ret, null);
		}

		$gallery->debug('<br>(modules:puzzle) loadTemplate: isEnabledForAlbum = ' . $enabled);

		// Access Denied
		if ($enabled == 'no') {
			// The user followed a link, but the link is invalid.
			$gallery->debug('<br>(modules:puzzle) loadTemplate: puzzles are disabled for this album.');

			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'puzzle');

			if ($ret) {
				return array($ret, null);
			}

			$template->title($module->translate('Make a puzzle'));

			// Let's return a nice error page
			return array(
				null,
				array(
					'body' => 'modules/puzzle/templates/AlbumDisabled.tpl',
				),
			);
		}

		$gallery->debug('<br>(modules:puzzle) loadTemplate: puzzles are allowed for this album.');

		// Access Allowed
		// find an image to use.
		list($success, $appletItem) = $this->findAppletItem($item);

		if (!$success) {
			$gallery->debug('<br>(modules:puzzle) loadTemplate: findAppletItemId did not return any suitable images');

			// Let's return a nice error page
			list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'puzzle');

			if ($ret) {
				return array($ret, null);
			}

			$template->title($module->translate('Make a puzzle'));

			return array(
				null,
				array(
					'body' => 'modules/puzzle/templates/MakePuzzleError.tpl',
				),
			);
		}

		$gallery->debug(
			'<br>(modules:puzzle) loadTemplate: findAppletItemId returned item ' . $appletItem->getId() .
						' (' . $appletItem->getWidth() . 'x' . $appletItem->getHeight() . ').'
		);

		// Adjust for Aspect Ratio
		// find the width and height we should use for the applet
		$widthRatio = $this->WIDTH / $appletItem->getWidth();

		// $puzzleWidth  = $widthRatio * $appletItem->getWidth();
		$puzzleWidth  = $this->WIDTH;
		$puzzleHeight = $widthRatio * $appletItem->getHeight();

		if ($puzzleHeight > $this->HEIGHT) {
			$heightRatio = $this->HEIGHT / $puzzleHeight;

			// $puzzleHeight = $heightRatio * $puzzleHeight;
			$puzzleHeight = $this->HEIGHT;
			$puzzleWidth  = $heightRatio * $puzzleWidth;
		}

		$puzzleHeight = intval(round($puzzleHeight));
		$puzzleWidth  = intval(round($puzzleWidth));

		$gallery->debug(
			'<br>(modules:puzzle) loadTemplate: item ' . $appletItem->getId() . ' rescaled from ' .
						'(' . $appletItem->getWidth() . 'x' . $appletItem->getHeight() . ')' . ' to ' .
						'(' . $puzzleWidth . 'x' . $puzzleHeight . ')'
		);

		// set the variables for the template
		$MakePuzzle                 = array();
		$MakePuzzle['itemId']       = $item->getId();
		$MakePuzzle['serialNumber'] = $item->getSerialNumber();

		// get the session information for the Java applet
		$session                          =& $gallery->getSession();
		$MakePuzzle['sessionId']          = urlencode($session->getId());
		$MakePuzzle['puzzleItemId']       = $appletItem->getId();
		$MakePuzzle['puzzleSerialNumber'] = $appletItem->getSerialNumber();
		$MakePuzzle['puzzleWidth']        = $puzzleWidth;
		$MakePuzzle['puzzleHeight']       = $puzzleHeight;
		$MakePuzzle['puzzleType']         = 1;

		if (!empty($form['puzzleType'])) {
			if ($form['puzzleType'] == 'slide') {
				$MakePuzzle['puzzleType'] = 1;
			} elseif ($form['puzzleType'] == 'swap') {
				$MakePuzzle['puzzleType'] = 0;
			}
		}

		$MakePuzzle['puzzleNum'] = 2;

		if (!empty($form['puzzleLevel'])) {
			if ($form['puzzleLevel'] == 'easy') {
				$MakePuzzle['puzzleNum'] = 2;
			}

			if ($form['puzzleLevel'] == 'moderate') {
				$MakePuzzle['puzzleNum'] = 3;
			}

			if ($form['puzzleLevel'] == 'hard') {
				$MakePuzzle['puzzleNum'] = 4;
			}

			if ($form['puzzleLevel'] == 'veryhard') {
				$MakePuzzle['puzzleNum'] = 5;
			}
		}

		$template->setVariable('MakePuzzle', $MakePuzzle);

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'puzzle');

		if ($ret) {
			return array($ret, null);
		}

		$template->title($module->translate('Make a puzzle'));

		return array(
			null,
			array(
				'body' => 'modules/puzzle/templates/MakePuzzle.tpl',
			),
		);
	}

	/**
	 * @see GalleryView::getViewDescription()
	 */
	public function getViewDescription() {
		list($ret, $core) = GalleryCoreApi::loadPlugin('module', 'core');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $core->translate('Make a puzzle of an image.'));
	}
}
