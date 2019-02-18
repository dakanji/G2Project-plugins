{* $Revision: 1371 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="favourites.getTop10"}


<div style="width: 100%; height: 20px;"></div>

<div id="slide_outer" style="width:100%; height:231px;  position:relative; overflow: hidden;">
<div  id="slide_inner" style="width:4500px; height:100%; position: absolute; " >

{section name=x loop=$block.favourites.Top10}
<div class="gallery-thumb"> <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$block.favourites.Top10[x]`"}">
  <img src="{g->url arg1="view=simpleviewersource.DownloadThumb" arg2="itemId=`$block.favourites.Top10[x]`"}"></a></div>
{/section}


</div>
</div>
<script type="text/javascript">
moveRight();
function moveRight() {ldelim}
	new Effect.Move ('slide_inner', {ldelim} x: -3360, y: 0, mode: 'relative', transition:Effect.Transitions.linear, duration:60, afterFinish: moveLeft {rdelim});
{rdelim}
function moveLeft() {ldelim}
	new Effect.Move ('slide_inner', {ldelim} x: 3360, y: 0, mode: 'relative', transition:Effect.Transitions.linear, duration:00, afterFinish: moveRight{rdelim});
{rdelim}
</script>
<div style="width: 100%; height: 20px;"></div>


