{*
 * $Revision: 885 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Add via Email Page"} </h2>
</div>

<div class="gbBlock">
  This is the Add via Email dev/debug/results page<br/>
</div>

<div class="gbBlock">
{if $user.isAdmin}
  The parameters are:<br/>
  serverType:{$Addviaemail.serverType}<br/>
  serverAddress:{$Addviaemail.serverAddress}<br/>
  subjectToMatch:{$Addviaemail.subjectToMatch}<br/>
  tmpLocation:{$Addviaemail.tmpLocation}<br/>
  owner:{$Addviaemail.owner}<br/>
  destinationAlbum:{$Addviaemail.destinationAlbum}<br/>
  deleteProcessed:{$Addviaemail.deleteProcessed}<br/>
{/if}
  <br/>
  {g->text one="Added [1] new item" many="Added [%d] new items" count=$newItems arg1=$newItems}<br/>
  <br/>
  {if !empty($failure)}
  	{g->text text="ERROR!"}</br>
  	{$failureMsg}
  {/if}
<br/>
<br/>
{if $user.isAdmin}
Check the <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=addviaemail.Configure" arg3="return=true"}">configuration</a><br/>
{/if}
</div>





<!--
<div class="gbBlock">
  The item you chose for this action was: <b>{$Addviaemail.item.title|default:$Addviaemail.item.pathComponent}</b>
</div>
<div class="gbBlock">
  {if empty($Addviaemail.value)}
  There is no value yet for this item.
  {else}
  The value in the database for this item is: <b>{$Addviaemail.value}</b>
  {/if}
</div>


<form action="{g->url}" method="post">
  <div>
    {g->hiddenFormVars}
    <input type="hidden" name="{g->formVar var="controller"}" value="addviaemail.Addviaemail"/>
    <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
    <input type="hidden" name="{g->formVar var="itemId"}" value="{$Addviaemail.item.id}"/>
  </div>

  <div class="gbBlock">
    {g->text text="Enter a value for this item:"}
    <input type="text" name="{g->formVar var="form[value]"}"/>
  </div>

  <div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit"
      name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  </div>
</form>
-->
