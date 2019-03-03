<?php
$blocks['TagsLink'] = array(
	'description' => $gallery->i18n('Remove Me'),
);

$blocks['ImageTags'] = array(
	'description' => $gallery->i18n('Item Tags'),
);

$blocks['TagLinks'] = array(
	'description' => $gallery->i18n('Tag cloud'),
	'vars'        => array(
		'sizeLimit' => array(
			'description' => $gallery->i18n('Maximum number of keywords (empty for no limit)'),
			'type'        => 'text',
			'default'     => '',
		),
	),
);
