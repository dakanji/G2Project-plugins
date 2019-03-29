{*
 * $Revision: 1433 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}


<script type="text/javascript" src="{g->url href="lib/xml_slideshow/swfobject.js"}"></script>

<div id="flashcontent" style="height:700px;">
  <h1>Oops!</h1>
  <p>It looks like you do not have flash player 6 installed. <a href="http://www.macromedia.com/go/getflashplayer" >Click here</a> to go to Macromedia download page.
  </p>
</div>
		
		<script type="text/javascript">		
			var fo = new SWFObject("{g->url href="lib/xml_slideshow/viewer.swf"}", "viewer", "{$height}", "{$width}", "7", "{$backgroundColor}");			
			fo.addVariable("preloaderColor", "{$preloaderColor}");		
			fo.addVariable("xmlDataPath", "{$XMLurl}");			
			fo.write("flashcontent");		
</script>
