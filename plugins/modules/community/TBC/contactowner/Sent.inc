<?php

/*
 * $RCSfile: Sent.inc,v $
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
 * @package ContactOwner
 * @author Darren Guiden <g2@pengs.co.uk>
 */

/**
 * This view is for letting the user know the email has been sent (or not...)
 *
 * @package ContactOwner
 * @subpackage UserInterface
 */
class SentView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		$session =& $gallery->getSession();

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'contactowner');

		if ($ret) {
			return array($ret, null);
		}

		if ($session->exists('contactowner.emailsent')) {
			$emailsent = $session->get('contactowner.emailsent');
		} else {
			$emailsent = true;
		}

		$template->setVariable('emailsent', $emailsent);
		$template->title(
			$gallery->i18n('Contact') . '::' .

						 $temp
		);

		return array(
			null,
			array(
				'body' => 'modules/contactowner/templates/Sent.tpl',
			),
		);
	}
}
