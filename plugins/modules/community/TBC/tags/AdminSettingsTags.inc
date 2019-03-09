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
GalleryCoreApi::requireOnce('modules/tags/classes/TagsHelper.class');

/**
 * Settings for the Tags module.
 *
 * @package Tags
 * @subpackage UserInterface
 * @author Eric Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1060 $
 */
class AdminSettingsTagsController extends GalleryController {
	/**
	 * @see GalleryController::handleRequest
	 */
	public function handleRequest($form) {
		global $gallery;

		$redirect = $status = $error = array();
		if (isset($form['action']['save'])) {
			// Error checking
			if ($form['tagAdminMode'] == 'General') {
				if (isset($form['PopularTaglimit']) && $form['PopularTaglimit'] != ''
					&& (!is_numeric($form['PopularTaglimit'])
					|| intval($form['PopularTaglimit']) != $form['PopularTaglimit']
					|| $form['PopularTaglimit'] < 0)
				) {
					$error[] = 'form[error][badlimit]';
				} else {
					foreach (array(
						'TagCloudLink',
						'LimitPopularTag',
						'SummaryShow',
						'SummaryAddTag',
					) as $setting) {
						$ret = GalleryCoreApi::setPluginParameter(
							'module',
							'tags',
							$setting,
							empty($form[$setting]) ? 0 : 1
						);

						if ($ret) {
							return array($ret, null);
						}
					}

					$ret = GalleryCoreApi::setPluginParameter(
						'module',
						'tags',
						'PopularTaglimit',
						$form['PopularTaglimit']
					);

					if ($ret) {
						return array($ret, null);
					}
				}
			}

			if ($form['tagAdminMode'] == 'TagCloud') {
				if ($form['tagCloudTemplate'] == 'DefaultTextTemplate') {
					/** @todo Add validation on the number of classes > 1. remove classNumber */
					if (!isset($form['classNumber']) || $form['classNumber'] != ''
						&& (!is_numeric($form['classNumber'])
						|| intval($form['classNumber']) != $form['classNumber']
						|| $form['classNumber'] < 0)
					) {
						$error[] = 'form[error][classNumber]';
					} else {
						$tagCssClass = '';
						for ($i = 0; $i < $form['classNumber']; $i++) {
							$settings = $form['tagCssClass'][$i];
							if ($settings['font'] != '') {
								if ($settings['color'] == '') {
									$settings['color'] = '#000000'; // default to black
								}

								$tagCssClass .= $settings['font'] . '|' . $settings['color'] . '&&';
							} else {
								// Only store if the font / color are all set
								$form['classNumber'] = $i;
								break;
							}
						}

						$form['tagCssClass'] = substr($tagCssClass, 0, -2);
						foreach (array('classNumber', 'tagCssClass', 'tagCloudTemplate')
						as $setting) {
							$ret = GalleryCoreApi::setPluginParameter(
								'module',
								'tags',
								$setting,
								$form[$setting]
							);

							if ($ret) {
								return array($ret, null);
							}
						}

						// Create the css file to use for the Tag Cloud
						$ret = TagsHelper::createCssFile(true);

						if ($ret) {
							return array($ret, null);
						}
					}
				}
			}

			$method                           = empty($error) ? 'redirect' : 'delegate';
			$status['saved']                  = empty($error);
			$results[$method]['view']         = 'core.SiteAdmin';
			$results[$method]['subView']      = 'tags.AdminSettingsTags';
			$results[$method]['tagAdminMode'] = $form['tagAdminMode'];
		}

		if (isset($form['action']['cancel'])) {
			$results['redirect']['view']         = 'core.SiteAdmin';
			$results['redirect']['subView']      = 'tags.AdminSettingsTags';
			$results['redirect']['tagAdminMode'] = $form['tagAdminMode'];
		}

		$results['status'] = $status;
		$results['error']  = $error;
		return array(null, $results);
	}
}

/**
 * Settings for the Tags module.
 */
class AdminSettingsTagsView extends GalleryView {
	/**
	 * @see GalleryView::loadTemplate
	 */
	public function loadTemplate(&$template, &$form) {
		list($ret, $Settings) = GalleryCoreApi::fetchAllPluginParameters('module', 'tags');

		if ($ret) {
			return array($ret, null);
		}

		if (!isset($form['formName'])) {
			$form['formName'] = 'AdminSettingsTags';
		}

		// Treat the classes
		foreach (explode('&&', $Settings['tagCssClass']) as $class) {
			$classSetting  = explode('|', $class);
			$tagCssClass[] = array($classSetting[0], $classSetting[1]);
		}

		$Settings['tagCssClass'] = $tagCssClass;
		$tagAdminMode            = GalleryUtilities::getRequestVariables('tagAdminMode');
		$tagAdminMode            = empty($tagAdminMode) ? 'General' : $tagAdminMode;

		$template->javascript('lib/yui/yahoo-min.js');

		$template->javascript('lib/yui/event-min.js');

		$template->javascript('lib/yui/dom-min.js');

		$template->javascript('lib/yui/dragdrop-min.js');

		$template->javascript('lib/yui/connection-min.js');

		$template->javascript('lib/yui/color.js');

		$template->javascript('lib/yui/slider-min.js');

		$template->javascript('lib/javascript/ColorChooser.js');
		$template->javascript('modules/tags/js/AdminTags.js');
		$template->setVariable('controller', 'tags.AdminSettingsTags');
		$template->setVariable('tagAdminMode', $tagAdminMode);
		$template->setVariable('Settings', $Settings);

		return array(
			null,
			array(
				'body' => 'modules/tags/templates/AdminSettingsTags.tpl',
			),
		);
	}
}
