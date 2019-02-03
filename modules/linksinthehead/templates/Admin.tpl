{*
 * $Revision: 1359 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}


<div class="gbBlock gcBackground1">
  <h2 class="gbTitle"> {g->text text="Links In The Head - Settings"} </h2>
</div>

  {if !empty($status)}
  <div id="gsStatus">
    {if isset($status.saved)}
    <div class="giStatus">
<br>
      {g->text text="&nbsp;Settings saved successfully"}
<br>
    </div>
    {/if}
  </div>
  {/if}

  <div class="gbAdmin">
    <div class="giDescription">
<br>
      {g->text text="&nbsp;Enter the links you would like displayed in the header. Any URL left empty will not be used."}
    </div>
<br>
    <div class="gbDataEntry">


      <h3 class="giTitle">
	{g->text text="&nbsp;Links"}
      </h3>
      	<table cellspacing="5">
      		{section name=x loop=$form.url}
      			<tr>
	      			<td>{g->text text="Link Name:"}</td>
	      			<td>

					<input type="text" name="{g->formVar var="form[linkname][`$smarty.section.x.index`]"}" size="12" value="{if isset($form.linkname[x])}{$form.linkname[x]}{/if}"/>

					</td>

	      			<td>{g->text text="URL:"}</td>
	      			<td><input type="text" name="{g->formVar var="form[url][`$smarty.section.x.index`]"}" size="40" value="{$form.url[x]}"/></td>
      			</tr>

      			<tr>
	      			<td><br><br><br></td>
      			</tr>
      		{/section}
      	</table>
    </div>
  </div>

<div class="gbBlock gcBackground1">
    <input type="submit" name="{g->formVar var="form[action][save]"}"
     value="{g->text text="Save"}"/>
  </div>

