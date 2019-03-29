{*
 * $Revision: 847 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{*<div class="gbBlock gcBackground1">
  <h2> {g->text text="Add Items"} </h2>
</div>

{if (!$ItemAdd.hasToolkit)}
<div class="gbBlock giWarning">
  {g->text text="You do not have any Graphics Toolkit activated that can handle JPEG images.  If you add images, you will probably not have any thumbnails."}
  {capture name="url"}
    {g->url arg1="view=core.SiteAdmin" arg2="subView=core.AdminModules"}
  {/capture}
  {if $ItemAdd.isAdmin}
    {g->text text="Visit the <a href=\"%s\">Modules</a> page to activate a Graphics Toolkit." arg1=$smarty.capture.url}
  {/if}
</div>
{/if}

<div class="gbTabBar">
  {foreach from=$ItemAdd.plugins item=plugin}
    {if $plugin.isSelected}
      <span class="giSelected o"><span>
	{$plugin.title}
      </span></span>
    {else}
      <span class="o"><span>
	<a href="{g->url arg1="view=core.ItemAdmin" arg2="subView=core.ItemAdd"
	 arg3="itemId=`$ItemAdmin.item.id`" arg4="addPlugin=`$plugin.id`"}">{$plugin.title}</a>
      </span></span>
    {/if}
  {/foreach}
</div>

<input type="hidden" name="{g->formVar var="addPlugin"}" value="{$ItemAdd.addPlugin}"/>

{include file="gallery:`$ItemAdd.pluginFile`" l10Domain=$ItemAdd.pluginL10Domain}
*}

<div id="calendar">
<table>
  <tr>
    {section name=date loop=$CalendarMonth.endDate start=$CalendarMonth.startDate step=86400 max=7}
    <th class="calendar-header"> {g->date timestamp=$smarty.section.date.index format="%A"} </th>
    {/section}
  </tr>
</table>
{section name=weekDate loop=$CalendarMonth.endDate start=$CalendarMonth.startDate step=604800}
<table class="week-table">
  <tr>
    {section name=date loop=$CalendarMonth.endDate start=$smarty.section.weekDate.index step=86400 max=7}
    <th class="day-header">
      <a class="day-number" href="{g->date timestamp=$smarty.section.date.index format="%Y%m%d"}"> {g->date timestamp=$smarty.section.date.index format="%e"} </a>
      <a class="new-event" onclick="addNewEvent('{g->date timestamp=$smarty.section.date.index format="%Y%m%d"}', '{g->date timestamp=$smarty.section.date.index format="%Y%m%d"}'); return false"><span> {g->text text="New Event"} </span></a>
    </th>
    {/section}
  </tr>
  {section name="eventsPerDay" loop=$CalendarMonth.eventsPerDay}
  <tr>
    {section name=date loop=$CalendarMonth.endDate start=$smarty.section.weekDate.index step=86400 max=7}
    {if $smarty.section.eventsPerDay.first}
    <td id="{g->date timestamp=$smarty.section.date.index format="%Y%m%d"}-{g->date timestamp=$smarty.section.date.index format="%Y%m%d"}" rowspan="{$CalendarMonth.eventsPerDay}" ondblclick="addNewEvent('{g->date timestamp=$smarty.section.date.index format="%Y%m%d"}', '{g->date timestamp=$smarty.section.date.index format="%Y%m%d"}'); return false;"> &nbsp; </td>
    {/if}
    {/section}
  </tr>
  {/section}
</table>
{/section}
</div>
