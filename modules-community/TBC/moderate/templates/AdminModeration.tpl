{*
 * $Revision: 1861 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Moderation Page"} </h2>
</div>
    
<div class="gbBlock">
  <p class="giDescription">
   <span style="font-size:small">
    {g->text text="Below you have items not yet approved. Choose you action for each one and then Commit."}
   </span>
  </p>
</div>

<div class="gbBlock">
  <table width="99%" align="center" cellspacing="6">
    {if (isset($images))}
	{assign var="imagesCont" value=1}
	{foreach from=$images item=timg}

		{if $imagesCont == 1}
			<tr>
		{/if}

		<td align="center" style="border: 1px solid #dedede">
			<p><span style="font-size:small">Item Id: {$timg}</span><p>
			{g->block type="imageblock.ImageBlock" blocks="specificItem" showDate=1 showOwner=1 showViews=0 showTitle=1 showHeading=0 useDefaults=0  itemId=$timg linkTarget="_blank"} 
			<p>
				<span style="font-size:small"><input type="checkbox" name="{g->formVar var="form[approved][]}" value="{$timg}"/>{g->text text="Approve"}<br/>
				<input type="checkbox" name="{g->formVar var="form[deleted][]}" value="{$timg}"/><i>{g->text text="Delete"}</i></span>
		</td>

		{assign var="imagesCont" value="`$imagesCont+1`"}
		{if $imagesCont == 4}
			{assign var="imagesCont" value=1}
			</tr>
		{/if}
	{/foreach}
	<tr>
		<td colspan="3" align="center">
			<p><br/>
			<input type="submit" name="{g->formVar var="form[action][commit]"}" class="inputTypeSubmit" value="{g->text text="Commit Actions"}"/>
		</td>
	</tr>
    {/if}
  </table>
</div>
