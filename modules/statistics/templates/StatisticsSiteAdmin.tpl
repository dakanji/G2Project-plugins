{*
 * $Revision: 1099 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock gcBackground1">
  <h2>{g->text text="Statistics"}</h2>
</div>

<div class="gbTabBar">
  {foreach from=$Statistics.plugins item=plugin}
    {if $plugin.isSelected}
      <span class="giSelected o"><span>
	{$plugin.title}
      </span></span>
    {else}
      <span class="o"><span>
	<a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=statistics.StatisticsSiteAdmin"
	  arg3="statisticsPlugin=`$plugin.id`"}">{$plugin.title}</a>
      </span></span>
    {/if}
  {/foreach}
</div>

<input type="hidden" name="{g->formVar var="statisticsPlugin"}" value="{$Statistics.statisticsPlugin}"/>
<input type="hidden" name="{g->formVar var="itemId"}" value="{$Statistics.itemId}"/>

{include file="gallery:`$Statistics.pluginFile`" l10Domain=$Statistics.pluginL10Domain}