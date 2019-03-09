{*
 * $Revision: 1420 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="{$class}">
{g->text text="Display items containing:"}
<form action="{g->url method="post"}">
  <input type="hidden" name="{g->formVar var="view"}" value="title.Contain"/>
  <input type="text" id="giFormTitle" size="20" autocomplete="off" name="{g->formVar var="chars"}" />
  {g->autoComplete element="giFormTitle"}
   {g->url arg1="view=title.getTitles" arg2="command=contain" arg3="chars=__VALUE__" htmlEntities=false}
   {/g->autoComplete}
    <button type="submit">{g->text text="View"}</button>
</form>
</div>

