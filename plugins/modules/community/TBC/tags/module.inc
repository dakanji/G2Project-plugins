<?php

/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2007 Bharat Mediratta
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
 * Tags Module
 *
 * This adds a Tag Management area as well as features to add all the tags to the pictures and
 * albums.  Virtual albums can be created from 1 or more tags via the tag cloud block.
 *
 * @package Tags
 * @author Eric Daniel <ehdaniel@gmail.com>
 * @version $Revision: 1727 $
 */
class TagsModule extends GalleryModule {
	public function __construct() {
		global $gallery;

		$this->setId('tags');
		$this->setName($gallery->i18n('Tags'));
		$this->setDescription(
			$gallery->i18n(
				'Tag management system'
			)
		);

		// Update upgrade() as required
		$this->setVersion('2.0.0');
		$this->setRequiredCoreApi(array(8, 0));
		$this->setRequiredModuleApi(array(4, 0));

		$this->_templateVersion = 1;

		$this->setGroup('data', $gallery->i18n('Extra Data'));
		$this->setCallbacks(
			'getSiteAdminViews|registerEventListeners|getItemLinks|getItemSummaries'
		);
	}

	/**
	 * Class Loader
	 */
	public static function getMe() {
		static $instance;

		if (!isset($instance)) {
			$instance = new TagsModule();
		}

		return $instance;
	}

	/**
	 * @see GalleryModule::registerEventListeners
	 */
	public function registerEventListeners() {
		GalleryCoreApi::requireOnce('modules/tags/classes/TagsHelper.class');
		GalleryCoreApi::registerEventListener('GalleryEntity::delete', new TagsHelper());
	}

	/**
	 * @see GalleryModule::performFactoryRegistrations
	 */
	public function performFactoryRegistrations() {
		// Register our Maintenance Tasks
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'MaintenanceTask',
			'KeywordsToTags',
			'KeywordsToTags',
			'modules/tags/classes/KeywordsToTags.class',
			'tags',
			null
		);

		if ($ret) {
			return $ret;
		}

		$ret = GalleryCoreApi::registerFactoryImplementation(
			'MaintenanceTask',
			'TimestampToTags',
			'TimestampToTags',
			'modules/tags/classes/TimestampToTags.class',
			'tags',
			null
		);

		if ($ret) {
			return $ret;
		}

		// Register the search plugin
		$ret = GalleryCoreApi::registerFactoryImplementation(
			'GallerySearchInterface_1_0',
			'TagsSearch',
			'tags',
			'modules/tags/classes/TagsSearch.class',
			'tags',
			null
		);

		if ($ret) {
			return $ret;
		}

		return null;
	}

	/**
	 * @see GalleryModule::getSiteAdminViews
	 */
	public function getSiteAdminViews() {
		return array(
			null,
			array(
				array(
					'name' => $this->translate('Tags Management'),
					'view' => 'tags.AdminTags',
				),
				array(
					'name' => $this->translate('Tags Settings'),
					'view' => 'tags.AdminSettingsTags',
				),
			),
		);
	}

	/**
	 * @see GalleryModule::getItemLinks
	 */
	public function getItemLinks($items, $wantsDetailedLinks, $permissions) {
		global $gallery;

		$links = array();

		list($ret, $canViewCloud) = GalleryCoreApi::getPluginParameter(
			'module',
			'tags',
			'TagCloudLink'
		);

		if ($ret) {
			return array($ret, null);
		}

		foreach ($items as $item) {
			$itemId = $item->getId();

			if (isset($permissions[$itemId]['tags.edit'])) {
				$links[$itemId][] = array(
					'text'   => $this->translate('Edit Tags'),
					'params' => array(
						'view'   => 'tags.TagsItemEdit',
						'itemId' => $itemId,
					),
				);
			}

			if (isset($permissions[$itemId]['tags.edit'], $wantsDetailedLinks[$itemId])
				&& $item->getCanContainChildren()
			) {
				$links[$itemId][] = array(
					'text'   => $this->translate('Bulk Edit Tags'),
					'params' => array(
						'view'   => 'tags.BulkTagsEdit',
						'itemId' => $itemId,
						'return' => true,
					),
				);
			}

			if ($canViewCloud && $item->getCanContainChildren() && isset($wantsDetailedLinks[$itemId])) {
				$links[$itemId][] = array(
					'text'   => $this->translate('Tag Cloud'),
					'params' => array(
						'view' => 'tags.TagCloud',
					),
				);
			}
		}

		return array(null, $links);
	}

	/**
	 * @see GalleryModule::getItemSummaries
	 */
	public function getItemSummaries($items, $permissions, &$template) {
		global $gallery;

		$urlGenerator =& $gallery->getUrlGenerator();

		list($ret, $canViewSummaries) = GalleryCoreApi::getPluginParameter('module', 'tags', 'SummaryShow');

		if ($ret) {
			return array($ret, null);
		}

		list($ret, $canAddTagsSummaries) = GalleryCoreApi::getPluginParameter('module', 'tags', 'SummaryAddTag');

		if ($ret) {
			return array($ret, null);
		}

		$interfaceAdded = false;
		$summaries      = array();

		GalleryCoreApi::requireOnce('modules/tags/classes/TagsHelper.class');

		foreach ($items as $item) {
			$itemId = $item->getId();

			if ($canViewSummaries) {
				list($ret, $tags) = TagsHelper::getTagsByItemId($itemId);

				if ($ret) {
					return array($ret, null);
				}

				if (!empty($tags)) {
					$tagsLink = '';

					foreach ($tags as $rawTagName => $tagName) {
						$tagUrl = $urlGenerator->generateUrl(
							array(
								'view'    => 'tags.VirtualAlbum',
								'tagName' => $rawTagName,
							)
						);

						if (!empty($tagsLink)) {
							$tagsLink .= ', ';
						}

						$tagsLink .= '<a href="' . $tagUrl . '">' . $tagName . '</a>';
					}

					$summaries[$itemId] = $this->translate('Tags:') . ' ' . $tagsLink;
				}
			}

			// Add the Edit Interface
			if (isset($permissions[$itemId]['tags.edit']) && $canAddTagsSummaries) {
				if (!$interfaceAdded) {
					$template->javascript('lib/yui/yahoo-min.js');
					$template->javascript('lib/yui/event-min.js');
					$template->javascript('lib/yui/connection-min.js');
					$template->javascript('modules/tags/js/Tags.js');

					// Load the G2 templating engine
					GalleryCoreApi::requireOnce('modules/core/classes/GalleryTemplate.class');

					/** @todo Improve performance by doing the data acquiration in a batch */
					$urlsTemplate = new GalleryTemplate(dirname(__FILE__) . '/../..');

					$urlsTemplate->setVariable('l10Domain', 'modules_' . $this->getId());

					$tpl            = 'gallery:modules/tags/templates/TagsSummaryEdit.tpl';
					$interfaceAdded = true;
				}

				if (!isset($summaries[$itemId])) {
					$summaries[$itemId] = '';
				}

				$urlsTemplate->setVariable('itemId', $itemId);

				list($ret, $summary) = $urlsTemplate->fetch($tpl);

				if ($ret) {
					return array($ret, null);
				}

				$summaries[$itemId] .= $summary;
			}
		}

		return array(null, $summaries);
	}

	/**
	 * @see GalleryModule::getRewriteRules
	 * @todo Change TagName keyword / query param to tagWord (naming scheme) (and in all
	 *       corresponding views / controllers too).
	 */
	public function getRewriteRules() {
		return array(
			array(
				'comment' => $this->translate('Tag Cloud'),
				'match'   => array(
					'view' => 'tags.TagCloud',
				),
				'pattern' => 'tagcloud',
				'help'    => $this->translate('Short URL for the Tag Cloud'),
			),
			array(
				'comment'  => $this->translate('Tag Virtual Album'),
				'match'    => array(
					'view' => 'tags.VirtualAlbum',
				),
				'pattern'  => 'tag/%tagName%',
				'help'     => $this-> translate('Short URL for a virtual album based on a Tag'),
				'keywords' => array(
					'tagName' => array(
						'pattern' => '([^?/]+)',
						'help'    => $this->translate('Tag to use to create the Virtual Album'),
					),
				),
			),
		);
	}

	/**
	 * @see GalleryModule::upgrade
	 */
	public function upgrade($currentVersion) {
		global $gallery;

		$storage =& $gallery->getStorage();

		GalleryCoreApi::requireOnce('modules/tags/classes/TagsHelper.class');

		// Set the default parameters
		list($ret, $params) = GalleryCoreApi::fetchAllPluginParameters('module', 'tags');

		if ($ret) {
			return $ret;
		}

		/** @todo: remove classNumber from the database (not needed) */
		foreach (array(
			'TagCloudLink'     => '0',
			'LimitPopularTag'  => '0',
			'SummaryShow'      => '0',
			'SummaryAddTag'    => '0',
			'PopularTaglimit'  => '',
			'classNumber'      => '6',
			'tagCssClass'      => '0.8|#828282&&1.2|#121212&&1.5|#00659C&&2|#008200&&2.5|#FFCB21&&3|#FF0000',
			'tagCloudTemplate' => 'DefaultTextTemplate',
		) as $key => $value) {
			if (!isset($params[$key])) {
				$ret = $this->setParameter($key, $value);

				if ($ret) {
					return $ret;
				}
			}
		}

		// Create the css file to use for the Tag Cloud if does not exist
		$ret = TagsHelper::createCssFile();

		if ($ret) {
			return $ret;
		}

		if (!isset($currentVersion)) {
			$currentVersion = '0';
		}

		switch ($currentVersion) {
			case '0':
				// Initial installation
				// Adding the Edit Tag Permission into the system
				$ret = GalleryCoreApi::registerPermission(
					'tags',
					'tags.edit',
					$gallery->i18n('[Tags] Edit Tags'),
					0,
					array()
				);

				if ($ret) {
					return $ret;
				}

				break;

			case '1.0.0':
			case '1.0.1':
			case '1.0.2':
				// Initially had the duplicate tag removal code here. Merged into case 1.1.1.
			case '1.1.0':
			case '1.1.1':
				/*
				 * Rebuild DB tables with proper naming scheme and proper keys
				 *
				 * 1) Create new DB tables (done by GalleryModule::installOrUpgrade)
				 * 2) Copy data from old tables to new tables
				 *    - Read old tag data into memory in chunks
				 *    - Generate a new unique id for each tag
				 *    - Remove duplicate tag name / id pairs
				 *    - Insert tag data into new tables
				 *    - Remove old tag data from old tables in chunks (idempotent code)
				 *    - Checkpoint
				 * 3) Drop old tables (done by GalleryModule::installOrUpgrade)
				 */
				$oldTagsQuery = '
					SELECT
						[Tags::TagId], [Tags::TagValue]
					FROM
						[Tags]
				';

				$oldTagItemMapQuery = '
					SELECT
						[TagsMap::itemId]
					FROM
						[TagsMap]
					WHERE
						[TagsMap::TagId] = ?
				';

				$deleteOldTagItemMapQuery = '
					DELETE FROM
						[TagsMap]
					WHERE
						[::TagId] = ?
				';

				$deleteOldTagQuery = '
					DELETE FROM
						[Tags]
					WHERE
						[::TagId] = ?
				';

				do {
					$gallery->guaranteeTimeLimit(30);

					list($ret, $oldTagsSearchResults) = $gallery->search(
						$oldTagsQuery,
						array(),
						array(
							'limit' => 100,
						)
					);

					if ($ret) {
						return $ret;
					}

					while (($oldTag = $oldTagsSearchResults->nextResult()) !== false) {
						$gallery->guaranteeTimeLimit(30);

						$oldTagId = (int)$oldTag[0];
						$tagName  = $oldTag[1];

						// Create a new tagId if necessary
						$ret = TagsHelper::addNewTag($tagName);

						if ($ret) {
							return $ret;
						}

						// Import tag / item mapping
						list($ret, $oldTagItemMapSearchResults) = $gallery->search(
							$oldTagItemMapQuery,
							array($oldTagId)
						);

						if ($ret) {
							return $ret;
						}

						if ($oldTagItemMapSearchResults->resultCount() != 0) {
							list($ret, $newTagId) = TagsHelper::getTagIdFromName($tagName);

							if ($ret) {
								return $ret;
							}

							while (($row = $oldTagItemMapSearchResults->nextResult()) !== false) {
								$gallery->guaranteeTimeLimit(30);

								$itemId = (int)$row[0];

								$ret = TagsHelper::assignTagById($itemId, $newTagId);

								if ($ret) {
									return $ret;
								}
							}

							$ret = $storage->execute($deleteOldTagItemMapQuery, array($oldTagId));

							if ($ret) {
								return $ret;
							}
						}

						$ret = $storage->execute($deleteOldTagQuery, array($oldTagId));

						if ($ret) {
							return $ret;
						}
					}

					$ret = $storage->checkPoint();

					if ($ret) {
						return $ret;
					}
				} while ($oldTagsSearchResults->resultCount() != 0);

				// Fall Through
			case '1.2.0':
			case '1.2.1':
			case '1.2.2':
			case '1.2.3':
				// Look if an empty tag is in the database and delete it
				$ret = TagsHelper::removeTag('');

				// We do not care about errors
				// Fall Through
			case '1.2.4':
				// Adding the Edit Tag Permission into the system
				$ret = GalleryCoreApi::registerPermission(
					'tags',
					'tags.edit',
					$gallery->i18n('[Tags] Edit Tags'),
					0,
					array()
				);

				if ($ret) {
					return $ret;
				}

				// Fall Through
			case '1.3.0':
			case '1.3.1':
			case '1.3.2':
			case '1.3.3':
			case '1.3.4':
			case '1.3.5':
			case '1.3.6':
				// Added the GalleryTagMap::Change event
			case '1.4':
			case '1.4.0':
			case '2.0.0':
				// Updated GalleryCoreApi requirement to 8.0
				// Upgraded GalleryModule requirement to 4.0
			case 'end of upgrade path':
				/*
				 * Leave this bogus case at the end of the legitimate case statements
				 * so that we always properly terminate our upgrade path with a break.
				 */
				break;

			default:
				return GalleryCoreApi::error(
					ERROR_BAD_PLUGIN,
					__FILE__,
					__LINE__,
					sprintf('Unknown module version %s', $currentVersion)
				);
		}

		return null;
	}
}
