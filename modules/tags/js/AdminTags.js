/*
 * $Revision: 1054 $
 * Very basic function to enhance the admin page
 *
 * @Module version 1.3.0
 * @package Tags
 * @author Eric Daniel <ehdaniel@gmail.com>
 */

function ShowEdit(){
	var editSpan              = document.getElementById('EditBox');
	editSpan.style.visibility = 'visible';
}

function updateTagClassesNumber(url) {
	var classNumber = document.getElementById('classNumber').value;
	var tagClasses  = "";
	for (i = 0; i < classNumber; i++) {
		var tagFontItemId = 'tagFont' + i;
		var tagFontItem   = document.getElementById(tagFontItemId);
		if (tagFontItem != null) {
			tagFont            = tagFontItem.value;
			var tagColorItemId = 'colorPicker' + i + 'field';
			var tagColorItem   = document.getElementById(tagColorItemId);
			if (tagColorItem != null) {
				tagColor = tagColorItem.value;
			}
				tagClasses += tagFont + "|" + tagColor + "&&";
		}
	}
	tagClasses = tagClasses.substr(0, tagClasses.length - 2);
	url        = url.replace('__CLASSES__', encodeURIComponent(tagClasses));
	url        = url.replace('__CLASSNUM__', encodeURIComponent(classNumber));
	/* Get the new content with ajax */
	YAHOO.util.Connect.asyncRequest(
		'GET',
		url,
		{success: updateTagClassTable, failure: null, scope: null},
		null
	);
}

function updateTagClassTable(http) {
	/* Apply to the table */
	var tagClassesTable       = document.getElementById('tagClasses');
	tagClassesTable.innerHTML = http.responseText;
}


/* Color Picking stuff */
var isInitTagColorChooser;

function ColorChooserShow(elementId) {
	var colorChooser             = document.getElementById('Markup_colorChooser');
	var element                  = document.getElementById(elementId);
	colorChooser.updateElementId = elementId;
	if (!isInitTagColorChooser) {
		init();
		isInitTagColorChooser = true;
	}
	if (colorChooser.style.display == 'block') {
		colorChooser.style.display = 'none';
	} else {
		var pos                    = YAHOO.util.Dom.getXY(element);
		colorChooser.style.display = 'block';
		YAHOO.util.Dom.setXY(colorChooser, [pos[0] - 390, pos[1] - 125]);
	}
}

function ColorChooserUpdate() {
	var colorChooser                    = document.getElementById("Markup_colorChooser");
	var color                           = document.getElementById("Markup_hexval").value;
	var updateElementId                 = colorChooser.updateElementId + "field";
	var updateElement                   = document.getElementById(updateElementId);
	var selectElement                   = document.getElementById(colorChooser.updateElementId);
	updateElement.value                 = "#" + color;
	selectElement.style.backgroundColor = "#" + color;
	selectElement.style.color           = "#" + color;
	colorChooser.style.display          = 'none';
}

function relateColor(id) {
	var link  = document.getElementById(id);
	var color = document.getElementById(id + 'field').value;
	if (color == '') {
			link.style.background = nocolor;
			link.style.color      = nocolor;
			color                 = nocolor;
	} else {
			link.style.background = color;
			link.style.color      = color;
	}
	eval(document.getElementById(id + 'field').title);
}

/* End color picking stuff */