{*
 * $Revision: 1070 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}

<form>
{g->text text="New Tag(s): "}

<input id="NewTag{$itemId}" type="text" onkeypress="return handleKey(event, '{g->url arg1="view=tags.TagsCallback" arg2="itemId=`$itemId`" arg3="command=newTag" arg4="tagName=__TAG__" htmlEntities=false useAuthToken=true}', {$itemId})" size="10" style="margin-right:10px;">

<a href="javascript:addNewTag('{g->url arg1="view=tags.TagsCallback" arg2="itemId=`$itemId`" arg3="command=newTag" arg4="tagName=__TAG__" htmlEntities=false useAuthToken=true}', {$itemId})" style="background:url({g->url href="modules/tags/images/TagAdd.png"}) no-repeat left center;padding:5px;padding-left:20px"></a>

</form>		    