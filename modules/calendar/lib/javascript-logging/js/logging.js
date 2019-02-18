/*
  JavaScript Logging v1.1.01
  -----------------------

  Usage

	In Your HTML

	  <script language="JavaScript" type="text/javascript" src="js/logging.js"></script>

	In Your JavaScript

	  try {
		// ...
		log('value of foo: ' + foo);
		//...
	  } catch(e) {
		logError(e);
	  }

  Options

	loggingDisplay = false;                   // The log will no longer be created or displayed

	loggingStartExpanded = true;              // The log will start expanded

	LOGGING_BASE = '/javascript-logging/';    // The location where javascript logging was unzipped to
											  // so that the CSS + additional javascript can be loaded

*/

var logging;
var loggingList;
var loggingDisplay;
var loggingStartExpanded;
var LOGGING_BASE = '';

var showLogging;
var hideLogging;

function initLogging(display) {
	if (loggingDisplay == null) {
		loggingDisplay = display;
	}

	if (loggingDisplay && logging == null) {
		logging           = document.createElement('div');
		logging.innerHTML = '<div id="logging-title" class="title"><span class="show-hide"><a id="show-logging" href="">[+]</a><a id="hide-logging" href="">[-]</a></span> JavaScript Logging</div>';
		loggingList       = document.createElement('ul');
		logging.appendChild(loggingList);
		log('logging initialisation');
	}
}

function onloadLoggingInit() {
	if (loggingDisplay) {
		document.body.appendChild(logging);
		addClass(logging, 'logging');

		showLogging = document.getElementById('show-logging');
		hideLogging = document.getElementById('hide-logging');

		hideLog();
		if (loggingStartExpanded) {
			showLog();
		}

		log('logging initialisation completed');
	}
}

function showLog() {
	removeClass(loggingList, 'hidden');
	removeClass(hideLogging, 'hidden');
	addClass(showLogging, 'hidden');
	var title     = document.getElementById('logging-title');
	title.onclick = hideLog;

	return false;
}

function hideLog() {
	addClass(loggingList, 'hidden');
	addClass(hideLogging, 'hidden');
	removeClass(showLogging, 'hidden');
	var title     = document.getElementById('logging-title');
	title.onclick = showLog;

	return false;
}

function log(text) {
	if (loggingDisplay) {
		var loggingNode = document.createElement('li');

		var internal = document.createElement('pre');
		internal.appendChild(document.createTextNode(text));

		loggingNode.appendChild(internal);
		loggingList.appendChild(loggingNode);
	}
}

function logError(e) {
	if (loggingDisplay) {
		var loggingNode = document.createElement('li');
		addClass(loggingNode, 'error');

		var date = new Date();
		loggingNode.appendChild(document.createTextNode(e));
		loggingList.appendChild(loggingNode);
	}

	showLog();
}

function logArray(ary) {
	if (ary == null) {
		log('empty array');
		return;
	}

	for (var ct1 = 0; ct1 < ary.length; ct1++) {
		log(ary[ct1]);
	}
}

if (loggingDisplay == null) {
	document.write('<script language="JavaScript" type="text/javascript" src="' + LOGGING_BASE + 'js/class.js"></script>');
	document.write('<style type="text/css">@import "' + LOGGING_BASE + 'css/logging.css";</style>');
	initLogging(true);

	var loggingOldHandlingFunction = (window.onload != null) ? window.onload : function () {};
	window.onload                  = function () {
		onloadLoggingInit(); loggingOldHandlingFunction(); };
}
