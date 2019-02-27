{*
 * $Revision: 1253 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{* Set defaults *}
{if !isset($item)}{assign var=item value=$theme.item}{/if}
{if !isset($coordStyle)}{assign var=coordStyle value=1}{/if}
{g->callback type="map.ItemCoords" itemId=$item.id coordStyle=$coordStyle}
{if !empty($block.map.ItemCoords) and !empty($block.map.ItemCoords.lat) and !empty($block.map.ItemCoords.lng)}
<div class="{$class}">
{g->text text="%s Coordinates:" arg1=$block.map.ItemCoords.ItemType}
{if $coordStyle eq 1}
{$block.map.ItemCoords.lat.deg}&deg;, {$block.map.ItemCoords.lng.deg}&deg;
{elseif $coordStyle eq 2}
{* Use &#39; for apostrophe because &apos; doesn't work in IE *}
{$block.map.ItemCoords.lat.deg}&deg;{$block.map.ItemCoords.lat.min}&#39;&nbsp;{$block.map.ItemCoords.lat.dir},
{$block.map.ItemCoords.lng.deg}&deg;{$block.map.ItemCoords.lng.min}&#39;&nbsp;{$block.map.ItemCoords.lng.dir}
{elseif $coordStyle eq 3}
{$block.map.ItemCoords.lat.deg}&deg;{$block.map.ItemCoords.lat.min}&#39;{$block.map.ItemCoords.lat.sec}&quot;&nbsp;{$block.map.ItemCoords.lat.dir},
{$block.map.ItemCoords.lng.deg}&deg;{$block.map.ItemCoords.lng.min}&#39;{$block.map.ItemCoords.lng.sec}&quot;&nbsp;{$block.map.ItemCoords.lng.dir}
{/if}
</div>
{/if}