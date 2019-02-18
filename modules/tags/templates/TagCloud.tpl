{*
 * $Revision: 1054 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
 <div class="gbBlock gcBackground1">
  <h2> {g->text text="Tag Cloud"} </h2>
</div>
<div class="gbBlock" align="center">
{g->text text="Search Tag: "}<input type="text" id="TagNameSearch" value="" />
{g->autoComplete element="TagNameSearch"}
    {g->url arg1="view=tags.TagsCallback" arg2="command=getTagList" arg3="prefix=__VALUE__" htmlEntities=false}
{/g->autoComplete}

<a href="javascript:viewVirtualAlbum('{g->url arg1="view=tags.VirtualAlbum" arg2="tagName=__TAG__" htmlEntities=false}')">{g->text text="Click to view Album"}</a>
</div>  
<div id="folksonomy">
  {if !empty($tagCloud)}
  <ul>
    {foreach from=$tagCloud item=tag key=rawTagName}
      <li class="tag{$tag.count}">
         <a href="{g->url arg1="view=tags.VirtualAlbum" arg2="tagName=`$rawTagName`"}">{$tag.name}</a>
      </li>
    {/foreach}
  </ul>
  {else}
     {g->text text="You have not defined any tags yet, you may do so "}
     <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=tags.AdminTags" arg3="return=1"}">
	   {g->text text="by clicking here."}
     </a>
  {/if}
</div>
