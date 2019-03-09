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
 * This plugin will handle assigning values to custom fields
 * @package SimpleviewerSource
 * @version $Revision: 1725 $
 * @author Alec Myers <alec@alecmyers.com>
 */
class SimpleviewerSourceItemEdit extends ItemEditPlugin {
	/**
	 * @see ItemEditPlugin::handleRequest
	 */
	public function handleRequest($form, &$item, &$preferred) {
		$status  = 0;
		$error   = array();
		$albumId = $item->getId();

		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'simpleviewersource');

		if ($ret) {
			return array($ret, null, null, null);
		}

		if (isset($form['action']['save'])) {
			$params = array_merge($form['fixed'], $form['custom'], $form['private']);
			GalleryCoreApi::requireOnce(
				'modules/simpleviewersource/classes/SimpleviewerParameterMapHelper.class'
			);

			foreach ($params as $myTuple) {
				trim($myTuple['paramName']);

				if (!isset($myTuple['paramValue'])) {
					$myTuple['paramValue'] = '';
				}

				// parameters must have a non-blank name
				if (strlen($myTuple['paramName'])
					&& strlen($myTuple['paramValue'])
					&& !($myTuple['id'])
				) {
					// create the parameter
					$ret = SimpleviewerParameterMapHelper::createParam(
						$albumId,
						$myTuple['paramName'],
						$myTuple['category'],
						$myTuple['paramValue']
					);

					if ($ret) {
						return array($ret, null);
					}
				} elseif (($myTuple['id']) && strlen($myTuple['paramName'])) {
					// update parameters
					$ret = SimpleviewerParameterMapHelper::updateParam(
						$myTuple['id'],
						$myTuple['paramName'],
						$myTuple['paramValue']
					);

					if ($ret) {
						return array($ret, null);
					}
				}

				// dead parameters
				if (isset($myTuple['delete']) && $myTuple['delete']) {
					$ret = SimpleviewerParameterMapHelper::deleteParam($myTuple['id']);

					if ($ret) {
						return array($ret, null);
					}
				}
			}

			$status = $module->translate('Changes saved successfully');
		}

		return array(null, $error, $status, null);
	}

	/**
	 * @see ItemEditPlugin::loadTemplate
	 */
	public function loadTemplate(&$template, &$form, $item, $thumbnail) {
		global $gallery;

		if ($form['formName'] != 'SimpleviewerSourceItemEdit') {
			$form['formName'] = 'SimpleviewerSourceItemEdit';

			// work out the url for the XML generator, as it's helpful to display it
			$myUrlGenerator =& $gallery->getUrlGenerator();
			$XMLurl         = $myUrlGenerator->generateUrl(
				array(
					'view'   => 'simpleviewersource.XMLOut',
					'itemId' => $item->getId(),
				),
				array(
					'forceFullUrl'   => true,
					'forceSessionId' => false,
					'htmlEntities'   => false,
				)
			);

			// and escape the & to %26
			$XMLurl = ereg_replace('&', '%26', $XMLurl);
			GalleryCoreApi::requireOnce(
				'modules/simpleviewersource/classes/SimpleviewerParameterMapHelper.class'
			);

			// private parameters - used by this module but not passed to Simpleviewer
			foreach (array(
				'enabled',
				'maxItems',
				'offerSlideshow',
				'height',
				'width',
				'backgroundColor',
				'preloaderColor',
			) as $myParamName) { // easy to add more
				list(
					$ret,
					$form['private'][$myParamName]
				) = SimpleviewerParameterMapHelper::fetchParam(
					$item->getId(),
					$myParamName,
					'private'
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			// fixed-name parameters - passed to Simpleviewer
			foreach (array(
				'maxImageHeight',
				'maxImageWidth',
				'textColor',
				'frameColor',
				'frameWidth',
				'stagePadding',
				'navPosition',
				'thumbnailColumns',
				'thumbnailRows',
				'backgroundImagePath',
			) as $myParamName) {
				list(
					$ret,
					$form['fixed'][$myParamName]
				) = SimpleviewerParameterMapHelper::fetchParam(
					$item->getId(),
					$myParamName,
					'fixed'
				);

				if ($ret) {
					return array($ret, null);
				}
			}

			list(
				$ret,
				$allCustomParamNames
			) = SimpleviewerParameterMapHelper::fetchParamNames(
				$item->getId(),
				''
			);

			foreach ($allCustomParamNames as $myParamName) {
				list(
					$ret,
					$aParam
				) = SimpleviewerParameterMapHelper::fetchParam(
					$item->getId(),
					$myParamName,
					''
				);

				if ($ret) {
					return array($ret, null);
				}

				$form['custom'][] = array(
					'id'         => $aParam['id'],
					'paramName'  => $myParamName,
					'paramValue' => $aParam['paramValue'],
				);
			}

			$form['custom'][] = array(
				'id'         => '',
				'paramName'  => null,
				'paramValue' => null,
			);

			$form['custom'][] = array(
				'id'         => '',
				'paramName'  => null,
				'paramValue' => null,
			);
		}

		$template->setVariable('XMLurl', $XMLurl);
		$template->setVariable('controller', 'simpleviewersource.SimpleviewerSourceItemEdit');

		return array(
			null,
			'modules/simpleviewersource/templates/SimpleviewerSourceItemEdit.tpl',
			'modules_simpleviewersource',
		);
	}

	/**
	 * @see ItemEditPlugin::isSupported
	 */
	public function isSupported($item, $thumbnail) {
		if (GalleryUtilities::isA($item, 'GalleryAlbumItem')) {
			return true;
		}
	}

	/**
	 * @see ItemEditPlugin::getTitle
	 */
	public function getTitle() {
		list($ret, $module) = GalleryCoreApi::loadPlugin('module', 'simpleviewersource');

		if ($ret) {
			return array($ret, null);
		}

		return array(null, $module->translate('Simpleviewer'));
	}
}
