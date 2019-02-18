{*
 * $Revision: 1054 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
 <div class="gbBlock gcBackground1">
  <h2> {g->text text="Tags Management"} </h2>
</div>

{if !empty($form.error)}
<div class="gbBlock">
   <h2 class="giError">{g->text text="There was a problem processing your request."}</h2>
</div>
{/if}

{if isset($status.deleted)}
<div class="gbBlock">
   <h2 class="giSuccess">{g->text text="The Tag and it's mappings have been successfully deleted."}</h2>
</div>
{/if}

{if !empty($status.added)}
<div class="gbBlock">
   <h2 class="giSuccess">{g->text text="The New Tag was created successfully."}</h2>
</div>
{/if}

{if !empty($status.renamed)}
<div class="gbBlock">
   <h2 class="giSuccess">{g->text text="The Tag was renamed successfully."}</h2>
</div>
{/if}

<div class="gbBlock">
  {g->text text="This allows to add, remove and edit tags from the database"}
</div>

<form action="{g->url}" method="post">
  <div>
    {g->hiddenFormVars}
    <input type="hidden" name="{g->formVar var="controller"}" value="tags.AdminTags"/>
    <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
  </div>

  <div class="gbBlock">
    {g->text text="Enter a Tag Name and Click Add:"}
    <input type="text" name="{g->formVar var="form[TagNameAdd]"}"/>
    <input type="submit" class="inputTypeSubmit"
      name="{g->formVar var="form[action][add]"}" value="{g->text text="Add"}"/>
      {if isset($form.error.notagname)}
        <span class="giError">{g->text text="You need to enter a name for your tag"}</span>
      {/if}
  </div>

  {if !empty($Tags)}
  <div class="gbBlock">
    {g->text text="Select a Tag and Click Delete or Edit"}
    <select name="{g->formVar var="form[TagNameDel]"}">
      {foreach from=$Tags  key=TagName item=count}
	<option value="{$TagName}"{if isset($TagNameEdit)
	     && $TagNameEdit eq $TagName} selected="selected"{/if}>{$TagName}</option>
      {/foreach}
    </select>
    <input type="submit" class="inputTypeSubmit"
      name="{g->formVar var="form[action][delete]"}" value="{g->text text="Delete"}"/>&nbsp;&nbsp;&nbsp;
    <input type="submit" class="inputTypeSubmit" value="{g->text text="Edit"}" onclick="ShowEdit();return false;"/>
    <span id="EditBox"{if !isset($form.error.nonewtagname)} style="visibility:hidden"{/if}>
       {g->text text="New Name:"}
       <input type="text" name="{g->formVar var="form[TagNameNew]"}"/>
       <input type="submit" class="inputTypeSubmit"
          name="{g->formVar var="form[action][edit]"}" value="{g->text text="Save"}"/>
    </span>
    {if isset($form.error.nonewtagname)}
      <span class="giError">{g->text text="Please Enter the new tag name"}</span>
    {/if}
  </div>

  <div class="gbBlock">
  <p class="giDescription">
    {g->text text="Currently existing tags and usage:"}
  </p>
  <table class="gbDataTable">
    <tr>
      <th>Tag Name</th><th>Number of Instances</th>
    </tr>
      {foreach from=$Tags  key=TagName item=Count}
        <tr>
          <td>
            {$TagName}
          </td>
          <td align="center">
            {$Count}
          </td>
        </tr>
      {/foreach}
  </table>
  </div>
  {/if}
</form>


