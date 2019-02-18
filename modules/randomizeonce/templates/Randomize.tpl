{*
 * $Revision: 1529 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
 
 
<div id="gsContent" class="gcBorder1">
<div class="gbBlock gcBackground1">
   <h2>{g->text text="Randomize Display Order"}</h2>
</div>

{if isset($status.randomized)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Custom display order successfully randomized."}
</h2></div>
{/if}

<div class="gbBlock">
<p>
{g->text text="Use the button below to assign a random value to the custom sort weight for each item in this album."}
</p>
</div>

<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][randomize]"}"
     value="{g->text text="Randomize Custom Display Order"}"/>
</div>
