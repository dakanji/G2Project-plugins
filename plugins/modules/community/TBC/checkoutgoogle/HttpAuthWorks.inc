<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2008 Bharat Mediratta
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
GalleryCoreApi::requireOnce('modules/checkoutgoogle/classes/CheckoutGoogleResponseHelper.class');

/**
 * Reflect the HTTP authtype, username, and password so we can check that Gallery can access them.
 * @package CheckoutGoogle
 * @subpackage UserInterface
 * @author Jack Bates <ms419@freezone.co.uk>
 * @version $Revision: 1683 $
 */
class HttpAuthWorksView extends GalleryView {
	/**
	 * @see GalleryView::isImmediate
	 */
	public function isImmediate() {
		return true;
	}

	/**
	 * @see GalleryView::renderImmediate
	 */
	public function renderImmediate($status, $error) {
		echo implode("\n", CheckoutGoogleResponseHelper::getHttpAuth());
	}
}
