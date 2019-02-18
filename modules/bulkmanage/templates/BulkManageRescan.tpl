{*
 * $Revision: 1099 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock">
  <p class="giDescription">{g->text text="Bulk rescans items, relearning the attributes of items that have been externally modified."}</p>
  <p>{g->text text="Run recursively, rescanning items in subalbums:"} <input type="checkbox" name="{g->formVar var="form[recursive]"}" /></p>
</div>

{if isset($status.done) && $status.done == 1}
<div class="gbBlock">
  <h2 class="giSuccess">
  {g->text text="Rescan run successfully, %d items rescanned" arg1=$status.count}
  </h2>
</div>
{/if}