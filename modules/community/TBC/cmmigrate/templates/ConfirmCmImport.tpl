{*
 * $Revision: 847 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Confirm Migration"} </h2>
</div>

<div class="gbBlock">
  <h3> {g->text text="Albums to import:"} </h3>

  <table class="gbDataTable"><tr>
    <th> {g->text text="Album Name"} </th>
    <th> {g->text text="Album Description"} </th>
    <th> {g->text text="Album Notes"} </th>
  </tr>
  {foreach from=$form.sourceAlbums item=albumId}
  <tr class="{cycle values="gbEven,gbOdd"}">
    <td>
      {$ConfirmCmImport.names.$albumId}
      <input type="hidden" name="{g->formVar var="form[sourceAlbums][$albumId]"}" value="1"/>
    </td><td>
      {$ConfirmCmImport.titles.$albumId}
    </td><td>
      {if ($ConfirmCmImport.existingAlbums.$albumId)> 0}
	{g->text text="An album already exists with this name.  This album will be renamed."}
	<br/>
      {/if}
    </td>
  </tr>
  {/foreach}
  </table>
</div>

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="Top level albums and albums whose parents are not selected will be imported into this album:"}
  </p>
  <div class="giInfo">
    <span class="summary">
      {g->text text="Title: %s"
       arg1=$ConfirmCmImport.targetAlbum.title|default:$ConfirmCmImport.targetAlbum.pathComponent}
    </span>
    <span class="summary">
      {g->text text="Description: %s" arg1=$ConfirmCmImport.targetAlbum.description|default:""}
    </span>
  </div>
  <input type="hidden"
   name="{g->formVar var="form[destinationAlbumID]"}" value="{$ConfirmCmImport.destinationAlbumID}"/>
</div>

{if isset($form.generateThumbnails)}
<div class="gbBlock">
  <p class="giDescription">
    {g->text text="Thumbnails will be generated during migration"}
  </p>
  <input type="hidden"
   name="{g->formVar var="form[generateThumbnails]"}" value="{$form.generateThumbnails}"/>
</div>
{/if}

<div class="gbBlock gcBackground1">
  <input type="hidden"
   name="{g->formVar var="copperminePath"}" value="{$ConfirmCmImport.copperminePath}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][import]"}" value="{g->text text="Import"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
</div>
