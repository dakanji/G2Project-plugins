<?php
/*
 * $RCSfile: module.inc,v $
 * $Revision: 1845 $
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/**
 * @package MRAnalysis
 * @author Mark Ridgwell <g2development@markridgwell.com>
 */
class MRAnalysisModule extends GalleryModule {
	// Constructor
	public function __construct() {
		global $gallery;
		$this->setId('mranalysis');
		$this->setName('Analysis');
		$this->setDescription($gallery->i18n('Analysis and diagnostics'));
		$this->setVersion('0.0.12');
		$this->setGroup('analysis', $gallery->i18n('Analysis'));
		$this->setCallbacks('getSiteAdminViews');

		$this->setRequiredCoreApi(array(7, 4));
		$this->setRequiredModuleApi(array(3, 0));
	}

	/**
	 * checks to see if the named module is installed and active - so can use the tables etc.
	 */
	public static function isModuleInstalledAndActive($modulename) {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', $modulename);

		if (!$ret) {
			list($ret, $active) = $module->isActive();

			if (!$ret && $active) {
				return true;
			}
		}

		return false;
	}

	/**
	 * @see GalleryModule::autoConfigure
	 */
	public function autoConfigure() {
		// We don't require any special configuration
		return array(null, true);
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		$views = array();

		$views[] = array(
			'name' => $this->translate('Settings'),
			'view' => 'mranalysis.Settings',
		);

		$views[] = array(
			'name' => $this->translate('Broken Images'),
			'view' => 'mranalysis.BrokenImages',
		);

		$views[] = array(
			'name' => $this->translate('Missing Keywords'),
			'view' => 'mranalysis.MissingKeywords',
		);

		$views[] = array(
			'name' => $this->translate('Default Filenames'),
			'view' => 'mranalysis.DefaultFileNames',
		);

		$views[] = array(
			'name' => $this->translate('Missing Titles'),
			'view' => 'mranalysis.MissingTitles',
		);

		$views[] = array(
			'name' => $this->translate('Duplicate Titles'),
			'view' => 'mranalysis.DuplicateTitles',
		);

		$views[] = array(
			'name' => $this->translate('File Extensions In Titles'),
			'view' => 'mranalysis.ExtensionsInTitles',
		);

		$views[] = array(
			'name' => $this->translate('No File Extension'),
			'view' => 'mranalysis.NoExtensionsInFileNames',
		);


		if (MRAnalysisModule::isModuleInstalledAndActive('tags')) {
			$views[] = array(
				'name' => $this->translate('Missing Tags'),
				'view' => 'mranalysis.MissingTags',
			);
		}

		if (MRAnalysisModule::isModuleInstalledAndActive('map')) {
			$views[] = array(
				'name' => $this->translate('Album Missing Map Location'),
				'view' => 'mranalysis.AlbumMissingMapLocation',
			);
		}

		if (MRAnalysisModule::isModuleInstalledAndActive('mrextraheadings')) {
			// All the 'views' in this section are considered to be under development
			// and will only appear on my development system which has the
			// module above installed.

			if (MRAnalysisModule::isModuleInstalledAndActive('keyalbum')) {
				$views[] = array(
					'name' => $this->translate('Keyword Analysis'),
					'view' => 'mranalysis.KeywordAnalysis',
				);
			}
		}

		return array(null, $views);
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations()
	 */
	public function performFactoryRegistrations() {
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'MaintenanceTask',
			'AddMissingKeywordsTask',
			'AddMissingKeywordsTask',
			'modules/mranalysis/classes/AddMissingKeywords.class',
			'mranalysis',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'MaintenanceTask',
			'CorrectKeywordTyposTask',
			'CorrectKeywordTyposTask',
			'modules/mranalysis/classes/CorrectKeywordTypos.class',
			'mranalysis',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}
}
