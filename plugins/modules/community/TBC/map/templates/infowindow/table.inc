<?php
$map['Labels'][0]      = 'none';
$map['extramap']       = '';
$map['infowindows'][0] = "'<table border=0 cellpadding=0 style=\"border-collapse: collapse;\"><tr>'
      +'<td  colspan=\"2\" style=\"border-bottom:3px solid orange\"><a href=\"'+itemLink+'\">'+title+'</a></td></tr>'
      +'<tr><td style=\"padding-top:2px;vertical-align:top;\">&nbsp;<a href=\"'+itemLink+'\"><img src=\"'+thumbLink+'\" width=\"'+thw+'\" height=\"'+thh+'\" alt=\"'+title+'\ style=\"max-width:200px;max-height:200px;\"/></a></td>'
      +'<td style=\"padding-top:2px;padding-left:2px;\"><br/><font color=black><b>" . $module->translate('Date:') . "</b> '+created+'</font>'";

if ($map['showItemSummaries']) {
	$map['infowindows'][0] = $map['infowindows'][0] . "
        +(summary!=\"\"?'<br/><br/><div style=\"width:200px\"><font color=black>'+summary+'</font></div>':'')";
}

if ($map['showItemDescriptions']) {
	$map['infowindows'][0] = $map['infowindows'][0] . "
        +(description!=\"\"?'<br/><br/><div style=\"width:200px\"><font color=black>'+description+'</font></div>':'')";
}

if ($map['showZoomLinks']) {
	$map['infowindows'][0] = $map['infowindows'][0] . "
        +'<br/><br/><a href=\"#zoominlink\" onclick=\"javascript:map.setCenter(new GLatLng('+lat+','+lon+'), '+zoomlevel+'); return false;\">" . $module->translate('Zoom In') . "</a>'";
}

$map['infowindows'][0] = $map['infowindows'][0] . "
        +'</td></tr></table>'";
