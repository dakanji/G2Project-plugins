{*
 * $Revision: 1420 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}



<div class="gbBlock gcBackground1">
  <h2> {g->text text="Theme-based Settings for Title (Contain/Begin) Albums"} </h2>
</div>

{if !empty($status) || !empty($form.error)}
<div class="gbBlock">
{if isset($status.saved)}
<h2 class="giSuccess">
  {g->text text="Settings saved successfully"}
</h2>
{/if}

{if !empty($form.error)}
<h2 class="giError">
  {g->text text="There was a problem processing your request."}
</h2>
{/if}
 
</div>
{/if}
{capture assign="message"}
  {g->text text="Theme"} &nbsp; &nbsp; &nbsp;
  <select name="{g->formVar var="form[themeId]"}">
    {html_options options=$TitleSiteAdmin.themeList selected=$form.themeId}
  </select>
  <br/><br/>
  {g->text text="Settings for %s theme in Title Albums" arg1=$ThemeSettingsForm.theme.name}
{/capture}

{g->block type="core.ThemeSettingsForm" class="gbBlock" message=$message formId="siteAdminForm"}

<div class="gbBlock gcBackground1">
  <input type="hidden" name="{g->formVar var="form[currentThemeId]"}" value="{$form.themeId}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
</div>
 