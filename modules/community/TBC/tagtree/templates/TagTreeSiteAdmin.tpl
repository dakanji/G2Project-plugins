{*
 * $Revision: 1720 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Tag Tree Settings"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Settings saved successfully"}
</h2></div>
{/if}

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="Albums can be displayed in a dynamic tree, organized by tags and directory structure."}
  </p>

  <h4> {g->text text="Tree Settings"} </h4>

  <input type="checkbox" id="cbShowItems"{if $form.showItems} checked="checked"{/if}
   name="{g->formVar var="form[showItems]"}"/>
  <label for="cbShowItems">
    {g->text text="Show items in addition to tags and albums"}
  </label>
  <br/>

  <input type="checkbox" id="cbPersistence"{if $form.usePersistence} checked="checked"{/if}
   name="{g->formVar var="form[usePersistence]"}"/>
  <label for="cbPersistence">
    {g->text text="Use cookies to remember which branches are open"}
  </label>
  <br/>

  <input type="checkbox" id="cbExpandCollapse"{if $form.treeExpandCollapse} checked="checked"{/if}
   name="{g->formVar var="form[treeExpandCollapse]"}"/>
  <label for="cbExpandCollapse">
    {g->text text="Show expand-all and collapse-all options"}
  </label>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
</div>

