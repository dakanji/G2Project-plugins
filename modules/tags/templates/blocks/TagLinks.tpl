{*
 * $Revision: 1087 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}

{g->callback type="tags.GetTagCloud" totalTagCount=true}
{if !empty($block.tags.taglist)}
<div id="folksonomy" style="width:auto; border: none;background:none">
  <ul>
      {foreach from=$block.tags.taglist item=tag key=rawTagName}
           <li class="tag{$tag.count}">
               <a href="{g->url arg1="view=tags.VirtualAlbum" arg2="tagName=`$rawTagName`"}">{$tag.name}</a>
           </li>
      {/foreach}
  </ul>
</div>
{/if}