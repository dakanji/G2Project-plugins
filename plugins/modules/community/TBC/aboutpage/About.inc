<?php

/*
 * $RCSfile: About.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2004 Bharat Mediratta
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/**
 * @version
 * @package AboutPage
 * @author Darren Guiden <g2@pengs.co.uk>
 */

/**
 * This view is for showing the custom page
 *
 * @package AboutPage
 * @subpackage UserInterface
 */
class AboutView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		// Get module options
		list($ret, $form['message']) = GalleryCoreApi::getPluginParameter(
			'module',
			'aboutpage',
			'message'
		);

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $markup) = GalleryCoreApi::getPluginParameter(
			'module',
			'core',
			'misc.markup'
		);

		if ($ret) {
			return array($ret, null);
		}

		if ($markup == 'html') {
			// Strip malicious content if html markup allowed
			$form['message'] = GalleryUtilities::htmlSafe($form['message'], true);
		}

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'aboutpage');

		if ($ret) {
			return array($ret, null);
		}

		$template->setVariable('form', $form);
		$template->title(
			$gallery->i18n('Contact') . '::' . $gallery->i18n('About')
		);

		return array(
			null,
			array(
				'body' => 'modules/aboutpage/templates/About.tpl',
			),
		);
	}
}
