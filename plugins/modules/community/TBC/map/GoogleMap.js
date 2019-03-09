function DisableInputs(inputs,value)
{
	for (var i = 0; i < inputs.length; i++) {
		var elementx      = document.getElementById(inputs[i]);
		elementx.disabled = value;
	}
}

function ToggleInputs(inputs)
{
	for (var j = 0; j < inputs.length; j++) {
		var subinputs = document.getElementsByName(inputs[j]);
		for (var i = 0; i < subinputs.length; i++) {
			subinputs[i].disabled = !subinputs[i].disabled;
		}
	}
}
function verify(existingmarkers)
{
	var box = document.getElementById('SetName');
	value   = box.value;
  
	for (i = 0; i < existingmarkers.length; i++) {
		if (existingmarkers[i].toLowerCase() == value.toLowerCase()) {
			results = confirm("MarkerSet already exist, are you sure ?");
			if (results == false) {
				return false;
			}
		}
	}
	return true;
}

function getmapwidth(Percent,Remove)
{
	if (typeof( window.innerWidth ) == 'number' ) {
		myWidth = window.innerWidth;
	} else if (document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
		myWidth = document.documentElement.clientWidth;
	} else if (document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
		myWidth = document.body.clientWidth;
	}

	  myWidth = (myWidth - Remove) * Percent / 100;

	  return myWidth;
}

function getmapheight(Percent,Remove){
	if (typeof( window.innerWidth ) == 'number' ) {
		myHeight = window.innerHeight;
	} else if (document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
		myHeight = document.documentElement.clientHeight;
	} else if (document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
		myHeight = document.body.clientHeight;
	}

	  myHeight = (myHeight - Remove) * Percent / 100;

	  return myHeight;
}

/* correctly handle PNG transparency in Win IE 5.5 & above. */

function correctPNG(){
	   var arVersion = navigator.appVersion.split("MSIE");
	   var version   = parseFloat(arVersion[1]);
	if ((version >= 5.5) && (document.body.filters)) {
		for (var i = 0; i < document.images.length; i++) {
			 var img     = document.images[i];
			 var imgName = img.src.toUpperCase();
			if (imgName.substring(imgName.length - 3, imgName.length) == "PNG") {
				var imgID    = (img.id) ? "id='" + img.id + "' " : "";
				var imgClass = (img.className) ? "class='" + img.className + "' " : "";
				var imgTitle = (img.title) ? "title='" + img.title + "' " : "title='" + img.alt + "' ";
				var imgStyle = "display:inline-block;" + img.style.cssText;
				if (img.align == "left") {
					imgStyle = "float:left;" + imgStyle;
				}
				if (img.align == "right") {
					imgStyle = "float:right;" + imgStyle;
				}
				if (img.parentElement.href) {
					imgStyle = "cursor:hand;" + imgStyle;
				}
				var strNewHTML = "<span " + imgID + imgClass + imgTitle; + " style=\"" + "width:" + img.width + "px; height:" + img.height + "px;" + imgStyle + ";" + "filter:progid:DXImageTransform.Microsoft.AlphaImageLoader" + "(src=\'" + img.src + "\', sizingMethod='scale');\"><\/span>";
				img.outerHTML = strNewHTML;
				i             = i - 1;
			}
		}
	}
}