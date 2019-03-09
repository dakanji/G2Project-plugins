{*
 * $Revision: 1660 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<br>
<div class="gbBlock gcBackground1">
<table>
  <tr>
    <td>
      <a href="{g->url arg1="view=puzzle.MakePuzzle" 
                       arg2="form[puzzleType]=slide" 
                       arg3="form[puzzleLevel]=moderate" 
                       arg4="itemId=`$MakePuzzle.itemId`"}" >Slide Puzzle</a>
    </td>
    <td>
      [
      <a href="{g->url arg1="view=puzzle.MakePuzzle" 
                       arg2="form[puzzleType]=slide" 
                       arg3="form[puzzleLevel]=easy" 
                       arg4="itemId=`$MakePuzzle.itemId`"}" >easy</a>, 
      <a href="{g->url arg1="view=puzzle.MakePuzzle" 
                       arg2="form[puzzleType]=slide" 
                       arg3="form[puzzleLevel]=moderate" 
                       arg4="itemId=`$MakePuzzle.itemId`"}" >moderate</a>, 
      <a href="{g->url arg1="view=puzzle.MakePuzzle" 
                       arg2="form[puzzleType]=slide" 
                       arg3="form[puzzleLevel]=hard" 
                       arg4="itemId=`$MakePuzzle.itemId`"}" >hard</a>, 
      <a href="{g->url arg1="view=puzzle.MakePuzzle" 
                       arg2="form[puzzleType]=slide" 
                       arg3="form[puzzleLevel]=veryhard" 
                       arg4="itemId=`$MakePuzzle.itemId`"}" >very hard</a> 
      ]
    </td>
  </tr>
  <tr>
    <td>
      <a href="{g->url arg1="view=puzzle.MakePuzzle"
                       arg2="form[puzzleType]=swap"
                       arg3="form[puzzleLevel]=moderate"
                       arg4="itemId=`$MakePuzzle.itemId`"}" >Swap Puzzle</a>
    </td>
    <td>
        [
        <a href="{g->url arg1="view=puzzle.MakePuzzle"
                         arg2="form[puzzleType]=swap"
                         arg3="form[puzzleLevel]=easy"
                         arg4="itemId=`$MakePuzzle.itemId`"}" >easy</a>,
        <a href="{g->url arg1="view=puzzle.MakePuzzle"
                         arg2="form[puzzleType]=swap" 
                         arg3="form[puzzleLevel]=moderate"
                         arg4="itemId=`$MakePuzzle.itemId`"}" >moderate</a>, 
        <a href="{g->url arg1="view=puzzle.MakePuzzle"
                         arg2="form[puzzleType]=swap"
                         arg3="form[puzzleLevel]=hard" 
                         arg4="itemId=`$MakePuzzle.itemId`"}" >hard</a>,
        <a href="{g->url arg1="view=puzzle.MakePuzzle"
                               arg2="form[puzzleType]=swap"
                         arg3="form[puzzleLevel]=veryhard"
                         arg4="itemId=`$MakePuzzle.itemId`"}" >very hard</a> 
        ]
    </td>
  </tr>
</table>
</div>

<div class="gbBlock gcBackground1">
<br>
{if $MakePuzzle.puzzleType eq 1}
  {g->text text="Slide: Click on a square to move it into the empty box.  Click the yellow Reveal button to show you which blocks are in their correct location."}
{/if}
{if $MakePuzzle.puzzleType eq 0}
  {g->text text="Swap: To swap two squares, click on the first square you want to move.  It will show a red box around the square.  Then click on the square you want to swap.  The squares will then swap places.  Click the yellow Reveal button to show you which blocks are in their correct location."}
{/if}
<br>
<br>
</div>

<div class="gbBlock gcBackground1">
  <center>
  <OBJECT 
    classid = "clsid:8AD9C840-044E-11D1-B3E9-00805F499D93"
    codebase = "http://java.sun.com/update/1.4.2/jinstall-1_4-windows-i586.cab#Version=1,4,0,0"
    WIDTH = "{$MakePuzzle.puzzleWidth}" HEIGHT = "{$MakePuzzle.puzzleHeight}" >
    <PARAM NAME = CODE VALUE = "puzzle.class" >
    <PARAM NAME = CODEBASE VALUE = "{g->url href="modules/puzzle/applets}" >
    <PARAM NAME = "type" VALUE = "application/x-java-applet;version=1.4">
    <PARAM NAME = "scriptable" VALUE = "false">
    <PARAM NAME = "PUZ_WEBSITE" VALUE="J. Mike Rollins <rollins@CamoTruck.Net>">
    <PARAM NAME = "PUZ_XSTYLE" VALUE="0">
    <PARAM NAME = "PUZ_NUMX" VALUE="{$MakePuzzle.puzzleNum}">
    <PARAM NAME = "PUZ_NUMY" VALUE="{$MakePuzzle.puzzleNum}">
    <PARAM NAME = "PUZ_TYPE" VALUE="{$MakePuzzle.puzzleType}">
    <PARAM NAME = "PUZ_SRC"  VALUE="{g->url arg1="view=core.DownloadItem" arg2="itemId=`$MakePuzzle.puzzleItemId`" arg3="serialNumber=`$MakePuzzle.puzzleSerialNumber`" arg4="GALLERYSID=`$MakePuzzle.sessionId`" htmlEntities=false forceFullUrl=true}">
    <PARAM NAME = "PUZ_RESIZE" VALUE="2">
    <COMMENT>
        <EMBED 
            type = "application/x-java-applet;version=1.4" \
            CODE = "puzzle.class" \
            JAVA_CODEBASE = "{g->url href="modules/puzzle/applets}" \
            WIDTH = "{$MakePuzzle.puzzleWidth}" \
            HEIGHT = "{$MakePuzzle.puzzleHeight}" \
            PUZ_WEBSITE ="J. Mike Rollins <rollins@CamoTruck.Net>" \
            PUZ_XSTYLE ="0" \
            PUZ_NUMX ="{$MakePuzzle.puzzleNum}" \
            PUZ_NUMY ="{$MakePuzzle.puzzleNum}" \
            PUZ_TYPE ="{$MakePuzzle.puzzleType}" \
            PUZ_SRC ="{g->url arg1="view=core.DownloadItem" arg2="itemId=`$MakePuzzle.puzzleItemId`" arg3="serialNumber=`$MakePuzzle.puzzleSerialNumber`" arg4="GALLERYSID=`$MakePuzzle.sessionId`" htmlEntities=false forceFullUrl=true}" \
            PUZ_RESIZE ="2" \
            scriptable = false \
            pluginspage = "http://java.sun.com/products/plugin/index.html#download">

            <NOEMBED alt="{g->text text="Your browser doesn't support applets; you should use one of the other upload methods."}">
              {g->text text="Your browser doesn't support applets; you should use one of the other upload methods."}
            </NOEMBED>

        </EMBED>
    </COMMENT>
  </OBJECT>
  </center>
</div>

