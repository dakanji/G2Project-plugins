/*
 * $Revision: 1054 $
 * Some functions for ajax-based navigation
 *
 * @Module version 1.3.0
 * @package Tags
 * @author Eric Daniel <ehdaniel@gmail.com>
 */

function handleKey(e, url, item){
	var keycode;
	if (window.event) {
		keycode = window.event.keyCode;
	} else if (e) {
		keycode = e.which;
	} else {
		return true;
	}
	if (keycode == 13) {
		addNewTag(url, item);
		return false;
	} else {
		return true;
	}
}

function addNewTag(url, item) {
	if (item == null) {
		var NewTag = document.getElementById('NewTag').value;
	} else {
		var NewTag = document.getElementById('NewTag' + item).value;
	}
	url = url.replace("__TAG__", encodeURIComponent(NewTag));
	YAHOO.util.Connect.asyncRequest(
		'GET',
		url,
		{success: refreshPage, failure: null, scope: null},
		null
	);
}

function assignTag(url, tag) {
	url = url.replace("__TAG__", encodeURIComponent(tag));
	YAHOO.util.Connect.asyncRequest(
		'GET',
		url,
		{success: refreshPage, failure: null, scope: null},
		null
	);
}

function unassignTag(url, tag) {
	url = url.replace("__TAG__", encodeURIComponent(tag));
	YAHOO.util.Connect.asyncRequest(
		'GET',
		url,
		{success: refreshPage, failure: null, scope: null},
		null
	);
}

function refreshPage(http) {
	window.location.reload(true);
}

function viewVirtualAlbum(url){
	var searchTag = document.getElementById('TagNameSearch').value;
	if (searchTag != "") {
		url             = url.replace("__TAG__", encodeURIComponent(searchTag));
		window.location = url;
	}
	return false;
}