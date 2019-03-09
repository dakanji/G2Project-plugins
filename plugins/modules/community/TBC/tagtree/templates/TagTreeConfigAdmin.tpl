{*
 * $Revision: 1720 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Tag Tree Settings"} </h2>
</div>

<div class="gbBlock"><h2 class="giError">
  {g->text text="You must install and activate the Tags module to use Tag Tree.
      This version of Tag Tree is compatible with Tags version %d.%d."
      arg1=$form.requiredTagsMajorVersion arg2=$form.requiredTagsMinorVersion}
</h2></div>

