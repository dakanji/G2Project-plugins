{*
 * $Revision: 1253 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}

{if isset($map.LegendPos) and isset($map.LegendFeature) and $map.LegendFeature neq '0' and ((isset($map.AlbumLegend) and $map.AlbumLegend) or (isset($map.PhotoLegend) and $map.PhotoLegend) or (isset($map.regroupItems) and $map.regroupItems))}
{if $map.LegendPos eq '0' or $map.LegendPos eq '1'}
<div class="{$class}">
{/if}
  <div id="legend" class="gcBackground1" style="{if $map.LegendPos eq '0'}right:5px;{if $map.ThumbBarPos eq '1'}position:relative;top:-{$map.ThumbHeight}px;{/if}{/if}{if $map.LegendPos eq 0 or $map.LegendPos eq 1}width:150px;{else}width:mywidth;{/if}border: 2px solid black;padding:.5em;">
    <center><h2>{g->text text="Legend(s)"}</h2></center><br/>
    {if $map.AlbumLegend eq "1"}
    {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}
      <br/>
      <table style="border:1px solid; border-color: black !important;border-collapse:collapse;"><tr class="gcBackground2" onclick="togglealbumlegend();" style="cursor:pointer;background-color:#dfdfdf;border-bottom:1px solid black;"><td width=100%>{g->text text="Album Legend"}</td><td><img alt="none" name="albumarrow" src="{g->url href="modules/map/images/down.png"}" height="16" width="15"/></td></tr>
      <tr><td colspan=3>
    {/if}
    <table {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}id="albumlegend" {if $map.ExpandAlbumLegend}style="display:block;"{else}style="display:none;"{/if}{/if}>
    {if $map.LegendPos eq '2' or $map.LegendPos eq '3'}<tr><td>{g->text text="Album Legend"}</td>{/if}
    {foreach from=$map.allmarkers key=num item=imagelist}
    {foreach from=$imagelist key=name item=images}
       {if $name eq $map.useAlbumMarkerSet}
       {foreach from=$images item=image key=num}
       {if $map.AlbumLegends.$num neq ""}
         {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}<tr>{/if}
         {assign var="colonpos" value=$image|strpos:":"}
         {assign var="newimage" value=$image|substr:$colonpos+2}
         <td><input onclick="togglemarkers('A{$num}')" value="1" type="checkbox" name="CA{$num}" checked="checked" /></td><td id="A{$num}">{$newimage}</td><td>{$map.AlbumLegends.$num}</td>
         {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}</tr>{/if}
       {/if}
       {/foreach}
       {/if}
    {/foreach}
    {/foreach}
    </table>
    {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}</td></tr></table>{/if}
    {/if}
    {if $map.PhotoLegend eq "1"}
    {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}
    <br/>
    <table style="border:1px solid; border-color: black !important;border-collapse:collapse;"><tr class="gcBackground2" onclick="togglephotolegend();" style="cursor:pointer;background-color:#dfdfdf;border-bottom:1px solid black;"><td width=100%>{g->text text="Photo Legend"}</td><td><img alt="none" name="photoarrow" src="{g->url href="modules/map/images/down.png"}" height="16" width="15"/></td></tr>
    <tr><td colspan=3>
    {/if}
    <table {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}id="photolegend" {if $map.ExpandAlbumLegend}style="display:block;"{else}style="display:none;"{/if}{/if}>
    {if $map.LegendPos eq '2' or $map.LegendPos eq '3'}<tr><td>{g->text text="Photo Legend"}</td>{/if}
    {foreach from=$map.allmarkers key=num item=imagelist}
    {foreach from=$imagelist key=name item=images}
       {if $name eq $map.useMarkerSet}
       {foreach from=$images item=image key=num}
       {if $map.PhotoLegends.$num neq ""}
         {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}<tr>{/if}
         {assign var="colonpos" value=$image|strpos:":"}
         {assign var="newimage" value=$image|substr:$colonpos+2}
         <td><input onclick="togglemarkers('P{$num}')" value="1" type="checkbox" name="CP{$num}" checked="checked" /></td><td id="P{$num}">{$newimage}</td><td>{$map.PhotoLegends.$num}</td>
         {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}</tr>{/if}
       {/if}
       {/foreach}
       {/if}
    {/foreach}
    {/foreach}
    </table>
    {if $map.LegendPos eq '0' or $map.LegendPos eq '1'}</td></tr></table>{/if}
    {/if}
    {if isset($map.regroupItems) and $map.regroupItems eq "1"}
    <br/><table style="border:1px solid;border-collapse:collapse;"><tr class="gcBackground2" style="cursor:pointer;background-color:#dfdfdf;border-bottom:1px solid;"><td colspan=2 width=100%>{g->text text="Grouping"}</td></tr>
    <tr><td>
    {if !isset($map.isIE) or (isset($map.isIE) and $map.isIE eq false)}
    <img alt="none" src="{g->url href="modules/map/images/multi/{$map.regroupIcon}.png"}"/>
    {else}
    <img alt="none" src="{g->url href="modules/map/images/blank.gif"}" style="filter: progid:DXImageTransform.Microsoft.AlphaImageLoader(src='{g->url href="modules/map/images/multi/"}{$map.regroupIcon}.png');"/>
    {/if}
    </td><td>{g->text text="When you click on this icon, it automatically zooms in to show the icons that are Regrouped together"}</td></tr>
    </table>
    {/if}
  </div>
</div>
{/if}