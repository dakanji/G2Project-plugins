{*
 * $Revision: 847 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Select Albums"} </h2>
</div>

{if !empty($form.error)}
<div class="gbBlock"><h2 class="giError">
  {if isset($form.error.nothingSelected)}
    {g->text text="You must choose something to import!"}
  {/if}
  {if isset($form.error.emptyCustomField)}
    {g->text text="You must specify a custom field name"}
  {/if}
</h2></div>
{/if}

<div class="gbBlock">
  <h3> {g->text text="Import Albums"} </h3>

  <p class="giDescription">
    {g->text text="Select the albums to migrate"}
  </p>

  <div>
    <h4> {g->text text="Source:"} </h4>

    <select multiple="multiple" size="10" name="{g->formVar var="form[sourceAlbums][]"}">
      {foreach from=$ChooseCmObjects.cmAlbumTree item=album}
	<option value="{$album.data->albumId}">
	  {"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"|repeat:$album.depth}--
	  {g->text text="%s" arg1=$album.data->name|htmlentities}
	</option>
      {/foreach}
    </select>
  </div>

  <div>
    <h4> {g->text text="Destination:"} </h4>

    <select name="{g->formVar var="form[destinationAlbumID]"}">
      {foreach from=$ChooseCmObjects.g2AlbumTree item=album}
	<option value="{$album.data.id}"{if $form.destinationAlbumID==$album.data.id}
	 selected="selected"{/if}>
	  {"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"|repeat:$album.depth}--
	  {$album.data.title|default:$album.data.pathComponent}
	</option>
      {/foreach}
    </select>
  </div>

  <div>
    <h4> {g->text text="Thumbnail generation"} </h4>

    <input type="checkbox" id="cbThumb" {if !empty($form.generateThumbnails)} checked="checked"{/if}
     name="{g->formVar var="form[generateThumbnails]"}"/>
    <label for="cbThumb">
      {g->text text="Normally Gallery will generate thumbnails the first time you look at them, then save them for subsequent views.  If you check this box, we will create the thumbnails at migration time.  Migration will take longer but the first time you view an album will go much faster."}
    </label>
  </div>
</div>

<div class="gbBlock gcBackground1">
  <input type="hidden"
   name="{g->formVar var="copperminePath"}" value="{$ChooseCmObjects.copperminePath}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][import]"}" value="{g->text text="Prepare Import"}"/>
</div>
