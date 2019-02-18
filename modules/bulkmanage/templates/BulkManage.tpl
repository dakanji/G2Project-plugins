{*
 * $Revision: 1099 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock gcBackground1">
  <h2>{g->text text="Bulk Manage"}</h2>
</div>

<div class="gbTabBar">
  {foreach from=$BulkManage.plugins item=plugin}
    {if $plugin.isSelected}
      <span class="giSelected o"><span>
	{$plugin.title}
      </span></span>
    {else}
      <span class="o"><span>
	<a href="{g->url arg1="view=core.ItemAdmin" arg2="subView=bulkmanage.BulkManage"
	 arg3="itemId=`$BulkManage.itemId`" arg4="bulkPlugin=`$plugin.id`"}">{$plugin.title}</a>
      </span></span>
    {/if}
  {/foreach}
</div>

<input type="hidden" name="{g->formVar var="bulkPlugin"}" value="{$BulkManage.bulkPlugin}"/>
<input type="hidden" name="{g->formVar var="itemId"}" value="{$BulkManage.itemId}"/>

{include file="gallery:`$BulkManage.pluginFile`" l10Domain=$BulkManage.pluginL10Domain}

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][bulkmanage]"}" value="{g->text text="Go"}"/>
</div>