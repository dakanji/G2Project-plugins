{*
 * $Revision: 1070 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div id="gsContent" class="gcBorder1">
<div class="gbBlock gcBackground1">
   <h2>{g->text text="Edit Tags"}</h2>
</div>
<div class="gbTabBar">
  {if ($tagMode == 'Existing')}
    <span class="giSelected o"><span>
      <img src="{g->url href="modules/tags/images/ThisTags.png"}" style="position:relative; top:5px;"/>{g->text text="Current Tags"}
    </span></span>
  {else}
    <span class="o"><span>
      <a href="{g->url arg1="view=tags.TagsItemEdit" arg2="itemId=$itemId"}">
        <img src="{g->url href="modules/tags/images/ThisTags.png"}" style="position:relative; top:5px;"/>
        {g->text text="Current Tags"}
      </a>
    </span></span>
  {/if}
  {if ($tagMode == 'Popular')}
    <span class="giSelected o"><span>
      <img src="{g->url href="modules/tags/images/PopularTags.png"}" style="position:relative; top:5px;"/>{g->text text="Popular Tags"}
    </span></span>
  {else}
    <span class="o"><span>
      <a href="{g->url arg1="view=tags.TagsItemEdit" arg2="itemId=$itemId" arg3="tagMode=Popular"}">
        <img src="{g->url href="modules/tags/images/PopularTags.png"}" style="position:relative; top:5px;"/>
        {g->text text="Popular Tags"}
      </a>
    </span></span>
  {/if}
  {if !empty($tagExifInfo)}
  {if ($tagMode == 'Exif')}
    <span class="giSelected o"><span>
      <img src="{g->url href="modules/tags/images/TagManage.png"}" style="position:relative; top:5px;"/>{g->text text="Tags from Exif"}
    </span></span>
  {else}
    <span class="o"><span>
        <img src="{g->url href="modules/tags/images/TagManage.png"}" style="position:relative; top:5px;"/>
		<a href="{g->url arg1="view=tags.TagsItemEdit" arg2="itemId=$itemId" arg3="tagMode=Exif"}">{g->text text="Tags from Exif"}</a>
    </span></span>
  {/if}
  {/if}
</div>

{if isset($form.error.noextension)}
  <span class="giError">{g->text text="The selection could not be extended, no children for this item"}</span>
{/if}


<div class="gbBlock">
{if $tagMode == 'Existing'}
  {if !empty($tagList)}
  <table class="gbDataTable" style="width:500px;border: 1px solid black">
  {foreach from=$tagList key=tagName item=tagId}
    <tr class="{cycle values="gbEven,gbOdd"}">
      <td>{$tagName}</td>
      <td align="right"><a href="javascript:unassignTag('{g->url arg1="view=tags.TagsCallback" arg2="itemId=`$itemId`" arg3="command=delTag" arg4="tagName=__TAG__" useAuthToken=true}', '{$tagName}');" style='background:url({g->url href="modules/tags/images/TagDelete.png"}) no-repeat left center;padding:5px;padding-left:20px'>Remove</a></td>
    </tr>
  {/foreach}
  </table>
  {else}
    {g->text text="There are no tags defined for this item!"}<br />
  {/if}
    <br />
    <input id="NewTag" type="text" 
	        onkeypress="return handleKey(event, '{g->url 
			 arg1="view=tags.TagsCallback"
		         arg2="itemId=`$itemId`" arg3="command=newTag" 
		         arg4="tagName=__TAG__" htmlEntities=false useAuthToken=true}')" 
	       size="30" style="margin-right:10px;">
    {g->autoComplete element="NewTag"}
      {g->url arg1="view=tags.TagsCallback" arg2="command=getTagList" arg3="prefix=__VALUE__" htmlEntities=false}
    {/g->autoComplete}
	    <a href="javascript:addNewTag('{g->url arg1="view=tags.TagsCallback"
		         arg2="itemId=`$itemId`" arg3="command=newTag" 
		         arg4="tagName=__TAG__" htmlEntities=false useAuthToken=true}')" 
		   style='background:url({g->url href="modules/tags/images/TagAdd.png"}) no-repeat left center;padding:5px;padding-left:20px'>Add Tag(s)</a> 
		<span class="description">{g->text text="Note: You can add multiple tags separated by , or ;"}</span>
  </div>

  {if $tagItem eq 'Album' && !empty($tagList)}
  <div class="gbBlock">
  <p class="description">
    {g->text text="Extend this tag selection"}
  </p>
  <form action="{g->url}" method="post" enctype="application/x-www-form-urlencoded"
	 id="TagsItemEditForm">
	    {g->hiddenFormVars}
	    <input type="hidden" name="{g->formVar var="controller"}" value="tags.TagsItemEdit"/>
	    <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
	    <input type="hidden" name="{g->formVar var="itemId"}" value="{$itemId}"/>
  <input name="{g->formVar var="form[AlbumItems]"}" type="checkbox" value="AlbumItems" />{g->text text="Items in Album"}<br />
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input name="{g->formVar var="form[AllAlbumItems]"}" type="checkbox" value="AllAlbumItems" />{g->text text="All Items"}<br />
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input name="{g->formVar var="form[AllAlbumAlbums]"}" type="checkbox" value="AllAlbumAlbums" />{g->text text="All Albums"}<br /><br />
  <input name="{g->formVar var="form[SubAlbumItems]"}" type="checkbox" value="SubAlbumItems" />{g->text text="Items in Sub Albums"}<br />
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input name="{g->formVar var="form[AllSubAlbumItems]"}" type="checkbox" value="AllSubAlbumItems" />{g->text text="All Items"}<br />
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input name="{g->formVar var="form[AllSubAlbumAlbums]"}" type="checkbox" value="AllSubAlbumAlbums" />{g->text text="All Albums"}<br /><br />
  </div>
  <div class="gbBlock gcBackground2">
     <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][apply]"}" value="{g->text text="Apply"}" />
     <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}" />
     </form>
  </div>
  {/if}
{/if}
{if $tagMode == 'Popular'}
  {if !empty($popularTags)}
  <table class="gbDataTable" style="width:500px;border: 1px solid black">
  {counter start=$popLimit direction="down" print=false assign=num}
  {foreach from=$popularTags key=tagName item=count}
    {if $num > 0}
    <tr class="{cycle values="gbEven,gbOdd"}">
      <td>{$tagName}</td>
      <td align="right">
         {if isset($tagList[$tagName])}
           {g->text text="Already Tagged"}
         {else}
           <a href="Javascript:assignTag('{g->url 
		           arg1="view=tags.TagsCallback" 
				   arg2="itemId=`$itemId`" arg3="command=addTag" 
				   arg4="tagName=__TAG__" useAuthToken=true}', '{$tagName}');" 
			  style='background:url({g->url href="modules/tags/images/TagAdd.png"}) no-repeat left center;padding:5px;padding-left:20px'>Add this Tag</a>
         {/if}
      </td>
    </tr>
    {counter}
    {/if}
  {/foreach}
  </table>
  {else}
    {g->text text="There are no tags defined Anywhere!"}<br />
  {/if}
  </div>
{/if}
{if $tagMode == 'Exif'}
    <h3>{g->text text="Current Tags"}</h3>
    <p class="description">
      {if empty($tagList)}
	      {g->text text="none"}
	  {else}
	      {foreach name=tagForeach from=$tagList key=tagName item=tagId}
	         {$tagName}{if !$smarty.foreach.tagForeach.last}, {/if}
	      {/foreach}
	  {/if}
	</p>
    </div>
    <div class="gbBlock">    
    <h3>{g->text text="Exif Info"}</h3>
    <p class="description">{g->text text="Click a Value to add it as a tag"}</p>
    <table class="gbDataTable">
      {section name=outer loop=$tagExifInfo step=2}
        <tr>
        {section name=inner loop=$tagExifInfo start=$smarty.section.outer.index max=2}
      <td class="gbEven">
		{g->text text=$tagExifInfo[inner].title}
      </td>
      <td class="gbOdd">
        <input type="hidden" id="NewTag{$smarty.section.inner.index}" value="{$tagExifInfo[inner].value}"/>
		<a href="Javascript:addNewTag('{g->url arg1="view=tags.TagsCallback"
		         arg2="itemId=`$itemId`" arg3="command=newTag" 
		         arg4="tagName=__TAG__" htmlEntities=false useAuthToken=true}','{$smarty.section.inner.index}')">
			{$tagExifInfo[inner].value}
		</a>
      </td>
      {/section}
    </tr>
    {/section}
  </table>
  </div> 
{/if}
</div>