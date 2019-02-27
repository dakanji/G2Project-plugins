{*
 * $Revision: 1514 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<script type="text/javascript">
// <![CDATA[
{literal} 
function display(id) {
    container = document.getElementById('previewImage').src = 'main.php?g2_view=core.DownloadItem&g2_itemId='+id+'&g2_rnd='+Math.random();
}
// ]]>
{/literal}</script>

 {if !empty($status)}
  <div id="gsStatus">
    {if isset($status.saved)}
    <div class="giStatus">
      {g->text text="Settings saved successfully"}
    </div>
    {/if}
  </div>
  {/if}
  
<div class="gbBlock">
 <h3> {g->text text="Original"} </h3>
    <input type="button" class="inputTypeSubmit" value="{g->text text="Display Original"}" onclick="display({$id});"/>
</div class="gbBlock">

<div class="gbBlock">
  <h3> {g->text text="Derivatives"} </h3>
      <p class="giDescription">
      {g->text text="Raw data for the derivatives of item"} {$id}
      </p>
        <table class="gbDataTable">
      	<tr class="{cycle values="gbOdd,gbEven" advance=false}">
      	<th></th>
      	<th>{g->text text="Delete"}</th>
      	<th align="center">{g->text text="id"}</th>
      	<th align="center">{g->text text="Type"}</th>
      	<th align="center">{g->text text="Size"}</th>
        <th align="center">{g->text text="w"}</th>
        <th align="center">{g->text text="h"}</th>
        <th align="center">{g->text text="mime"}</th>
        <th align="center">{g->text text="sourceId"}</th>
        <th align="center">{g->text text="derivativeOperations"}</th>
        <th align="center">{g->text text="postFilterOperations"}</th>
        <th align="center">{g->text text="Broken?"}</th>
        <th align="center">{g->text text="Save and Rebuild"}</th>
        </tr>
        	
  {foreach from=$form.derivatives item=d }
  	{assign var=did value=$d.id}
	<tr class="{cycle}" >
	<td>{if $d.id > 0}<input type="button" class="inputTypeSubmit" value="{g->text text="Display"}" onclick="display({$d.id});"/>{/if}</td>
	<td align="center">{if $d.id > 0}<input type="checkbox" name="{g->formVar var="form[derivatives][`$d.id`][delete]"}" value = "true">{/if}</td>
	<td align="center">{$d.id}</td>
	<td><select name="{g->formVar var="form[derivatives][`$d.id`][derivativeType]"}">
      <option value = "1" {if $d.derivativeType == 1}selected{/if}>{g->text text="Thumbnail"}</option>
      <option value = "2" {if $d.derivativeType == 2}selected{/if}>{g->text text="Resize"}</option>
      <option value = "3" {if $d.derivativeType == 3}selected{/if}>{g->text text="Preferred"}</option>
      </select></td>
	<td align="center">{$d.derivativeSize}</td>
	<td align="center">{$d.width}</td>
	<td align="center">{$d.height}</td>
	<td align="center">{$d.mimeType}</td>
	<td align="center"><input type="text" name="{g->formVar var="form[derivatives][`$d.id`][derivativeSourceId]"}" value="{$d.derivativeSourceId}"  size = "4" style="text-align:center;"></td>
	<td align="center"><input type="text" name="{g->formVar var="form[derivatives][`$d.id`][derivativeOperations]"}" value="{$d.derivativeOperations}"  size = "50"></td>
	<td align="center"><input type="text" name="{g->formVar var="form[derivatives][`$d.id`][postFilterOperations]"}" value="{$d.postFilterOperations}"  size = "30"></td>
	<td align="center">{if $d.isBroken}{g->text text="Broken"}{/if}</td>
	<td><input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][save][`$d.id`]"}" value="{g->text text="Go"}"/></td>
	</tr>
	
  {/foreach}	
   	</table>
</div>
<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][save][all]"}"
     value="{g->text text="Save And Rebuild All"}"/>
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][cancel]"}"
     value="{g->text text="Cancel"}"/>
</div>
<div class="gbBlock">
<div id="preview"><img id="previewImage"></div>
</div>

