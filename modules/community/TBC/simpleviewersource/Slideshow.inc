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
 * Display the slideshow
 * @package SimpleviewerSource
 * @version $Revision: 1433 $
 * @author Alec Myers <alec@alecmyers.com>
 */
GalleryCoreApi::requireOnce(
	'modules/simpleviewersource/classes/SimpleviewerParameterMapHelper.class'
);

class SlideshowView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$itemId = GalleryUtilities::getRequestVariables('itemId');

		$myUrlGenerator =& $gallery->getUrlGenerator();
		$XMLurl         = $myUrlGenerator->generateUrl(
			array(
				'view'   => 'simpleviewersource.XMLOut',
				'itemId' => $itemId,
			),
			array(
				'forceFullUrl'   => true,
				'forceSessionId' => false,
				'htmlEntities'   => false,
			)
		);

		// and escape the & to %26
		$XMLurl = ereg_replace('&', '%26', $XMLurl);

		// get height and width
		list($ret, $parameters) = SimpleviewerParameterMapHelper::fetchParams(
			$itemId,
			'private'
		);

		if ($ret) {
			return array($ret, null);
		}

		$template->setVariable('height', $parameters['width']);
		$template->setVariable('width', $parameters['height']);
		$template->setVariable('backgroundColor', $parameters['backgroundColor']);
		$template->setVariable('preloaderColor', $parameters['preloaderColor']);
		$template->setVariable('XMLurl', $XMLurl);

		return array(
			null,
			array(
				'body'          => 'modules/simpleviewersource/templates/Slideshow.tpl',
				'useFullScreen' => false,
			),
		);
	}
}
