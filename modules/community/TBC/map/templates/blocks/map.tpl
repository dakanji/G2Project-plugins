{*
 * $Revision: 1253 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}

{g->callback type="map.LoadFilters"}
{if !empty($block.map.LoadFilters)}
<div class="{$class}" {if $map.ShowFilters eq 2}style="right:10px;{if $map.ThumbBarPos eq '1'}position:relative;top:-{$map.ThumbHeight}px;{/if}"{/if}>
  {if isset($map.ShowFilters) and ($map.ShowFilters eq 3 or $map.ShowFilters eq 4)}
  <table border=0 width=400px><tr><td width=200px>
  {/if}
  <h3>{g->text text="Area to show on the Map"}</h3>
  {if isset($map.ShowFilters) and ($map.ShowFilters eq 3 or $map.ShowFilters eq 4)}</td><td width=200px>{/if}
    <select style="width:75%;" onchange="{literal}if (this.value) { newLocation = this.value; this.options[0].selected = true; location.href= newLocation; }{/literal}">
    {foreach item=option from=$block.map.LoadFilters.filters}
       <option label="{$option.name}" {if $option.params neq ''}value="{g->url params=$option.params}"{/if} {if $option.name eq $block.map.LoadFilters.filterOn}selected{/if}>{$option.name}</option>
    {/foreach}
  </select>
  {if isset($map.ShowFilters) and ($map.ShowFilters eq 3 or $map.ShowFilters eq 4)}</td></tr></table>{/if}
</div>
{/if}