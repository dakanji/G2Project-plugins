<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2009 Bharat Mediratta
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
 * @package TagTree
 * @author Chad Parry <spam@chad.parry.org>
 * @author Bharat Mediratta <bharat@menalto.com>
 * @version $Revision: 1720 $
 */
class TagTreePreloads {
	public function preload(&$template, $preload, $paramSet) {
		static $loaded;

		if (!empty($loaded[$preload])) {
			return null;
		}

		$loaded[$preload] = 1;
		switch ($preload) {
			case 'TagTree':
				$template->style('modules/tagtree/lib/xloadtree/xtree.css');
				$template->style('modules/tagtree/xtree.css');
				$template->javascript('modules/tagtree/lib/xloadtree/xtree.js');
				$template->javascript('modules/tagtree/lib/xloadtree/xmlextras.js');
				$template->javascript('modules/tagtree/lib/xloadtree/xloadtree.js');

				return null;
		}

		return GalleryCoreApi::error(ERROR_BAD_PARAMETER);
	}
}
