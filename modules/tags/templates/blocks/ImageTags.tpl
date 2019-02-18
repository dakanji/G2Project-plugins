{*
 * $Revision: 1077 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}

{* @todo: Rename this block to ItemTags when Gallery 2.3 is out *}

{g->callback type="tags.TagsLink" itemId=$item.id|default:$theme.item.id}

<div class="{$class}">
{if !empty($block.tags.TagsLink.Tags)}
      {g->text text="Tags: "} 
      {counter start=0 print=false assign=num}
      {foreach from=$block.tags.TagsLink.Tags key=rawTagName item=tagName}
         <a href="{g->url arg1="view=tags.VirtualAlbum" arg2="tagName=`$rawTagName`"}">
           {$tagName}</a>{if $num < $block.tags.TagsLink.TagsNumber-1}, {/if}
         {counter}
      {/foreach}
{else}
   {g->text text="No Tags"}
{/if}
</div>
