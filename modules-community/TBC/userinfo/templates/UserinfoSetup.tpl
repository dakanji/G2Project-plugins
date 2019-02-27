<div class="gbBlock gcBackground1">
  <h2> {g->text text="Setup Page"} </h2>
</div>

<div class="gbTabBar">
  <span class="o"><span>
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=userinfo.Userinfo"}">
      {g->text text="User Info"}
    </a>
  </span></span>

  <span class="giSelected o"><span>
    {g->text text="Setup"}
  </span></span>
</div>


{if !empty($status)}
<div class="gbBlock"><h2 class="giSuccess">
  {if isset($status.saved)}
    {g->text text="Settings saved successfully"} <br/>
  {/if}
  {if isset($status.save1)}
    {g->text text="User Info Tab settings saved"} <br/>
  {/if}
</h2></div>
{/if}
{if !empty($form.error)}
<div class="gbBlock"><h2 class="giError">
  {g->text text="There was a problem processing your request, see below for details."}
</h2></div>
{/if}

{if isset($status.emailTestError) or isset($form.emailTestError)}
<div class="gbBlock"><h2 class="giError">
  {g->text text="Error sending test email, see below for details."}
</h2></div>
{/if}

<br/>

<div class="gbBlock">
<table class="gbDataTable" border="0">
<tr>
	<td align="center" colspan="2"><b><font size="4">{g->text text="User Info Tab"}</font></b></td>
</tr>
<tr><td>&nbsp;</td></tr>
<tr>
	<td>{g->text text="# records Per Page:"}</td>
	<td>
		<select name="{g->formVar var="form[uiPerPage]"}">
		{html_options values=$UserinfoSetup.uiPerPageList
		selected=$form.uiPerPage output=$UserinfoSetup.uiPerPageList}
	    </select>
	</td>
</tr>
<tr>
	<td>{g->text text="Default Sort Column:"}</td>
	<td>
		<select name="{g->formVar var="form[uiDefaultSortColumn]"}">
		{html_options values=$UserinfoSetup.uiDefaultSortColumnList
		selected=$form.uiDefaultSortColumn output=$UserinfoSetup.uiDefaultSortColumnList2}
	    </select>
	</td>
</tr>
<tr>
	<td>{g->text text="Default Sort Order:"}</td>
	<td>
		<select name="{g->formVar var="form[uiDefaultSortOrder]"}">
		{html_options values=$UserinfoSetup.uiDefaultSortOrderList
		selected=$form.uiDefaultSortOrder output=$UserinfoSetup.uiDefaultSortOrderList2}
	    </select>
	</td>
</tr>
<tr>
	<td>
		<a href="http://php.net/strftime" target="_blank">{g->text text="Date/Time Format"}</a>
	</td>
	<td>
		<input type="text" size="12" name="{g->formVar var="form[uiDateFormat]"}" value="{$UserinfoSetup.data.result.uiDateFormat}"/>
	</td>
	<td>
		{g->date format=$form.uiDateFormat}
	</td>
</tr>

</table>
<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save1]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
</div>
</div>


<br/><br/>

<!--
<div class="gbBlock">
<table class="gbDataTable" border="0">
<tr>
	<td align="center" colspan="2"><b><font size="4">{g->text text="User Info Tab"}</font></b></td>
</tr>
<tr><td>&nbsp;</td></tr>
<tr>
	<td>{g->text text="# records Per Page:"}</td>
	<td>
		<select name="{g->formVar var="form[uiPerPage]"}">
		{html_options values=$UserinfoSetup.uiPerPageList
		selected=$form.uiPerPage output=$UserinfoSetup.uiPerPageList}
	    </select>
	</td>
</tr>
<tr>
	<td>{g->text text="Default Sort Column:"}</td>
	<td>
		<select name="{g->formVar var="form[uiDefaultSortColumn]"}">
		{html_options values=$UserinfoSetup.uiDefaultSortColumnList
		selected=$form.uiDefaultSortColumn output=$UserinfoSetup.uiDefaultSortColumnList2}
	    </select>
	</td>
</tr>
<tr>
	<td>{g->text text="Default Sort Order:"}</td>
	<td>
		<select name="{g->formVar var="form[uiDefaultSortOrder]"}">
		{html_options values=$UserinfoSetup.uiDefaultSortOrderList
		selected=$form.uiDefaultSortOrder output=$UserinfoSetup.uiDefaultSortOrderList2}
	    </select>
	</td>
</tr>
</table>
<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save2]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
</div>
</div>
-->