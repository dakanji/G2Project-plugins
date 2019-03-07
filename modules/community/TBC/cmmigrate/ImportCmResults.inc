<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
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
 * @package CmMigrate
 * @subpackage UserInterface
 * @author David Angier <david@angier.co.uk>
 * @version $Revision: 847 $
 */
class ImportCmResultsView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		global $gallery;

		if ($form['formName'] != 'ImportCmResults') {
			$form['formName'] = 'ImportCmResults';
		}

		$status = $template->getVariable('status');
		$counts = array();

		foreach (array('albumImportFailure', 'albumImportSuccess') as $key) {
			if (isset($status[$key])) {
				$counts[$key] = count($status[$key]);
			} else {
				$counts[$key] = 0;
			}
		}

		$ImportResults           = array();
		$ImportResults['counts'] = $counts;
		$ImportResults['status'] = $status;

		$template->setVariable('ImportResults', $ImportResults);
		$template->setVariable('controller', 'cmmigrate.ImportCmResults');

		return array(
			null,
			array(
				'body' => 'modules/cmmigrate/templates/ImportCmResults.tpl',
			),
		);
	}
}
