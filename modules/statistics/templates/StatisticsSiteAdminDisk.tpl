{*
 * $Revision: 1099 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock">
  <h3>{g->text text="Disk Usage"}</h3>
  {assign var="d" value=$StatisticsSiteAdmin.Disk}
  {if empty($d)}
    <div class="giError">
        <h2>{g->text text="No cache file found"}</h2>
    </div>
  {else}
  <table class="gbDataTable" width="50%">
    <tr><th width="50%">{g->text text="Item"}</th><th width="50%">{g->text text="Size (Reported)"}</th></tr>
    <tr class="gbEven"><td>{g->text text="Gallery"}</td><td>{$d.gallery}</td></tr>
    <tr class="gbOdd"><td>&nbsp;&raquo; {g->text text="Albums"}</td><td>{$d.albums} ({$d.galbums})</td></tr>
    <tr class="gbEven"><td>&nbsp;&raquo; {g->text text="Derivatives"}</td><td>{$d.derivatives} ({$d.gderivatives})</td></tr>
    <tr class="gbOdd"><td>{g->text text="Database"}</td><td>{$d.database}</td></tr>
    <tr class="gbEven"><td>{g->text text="Total"}</td><td>{$d.total}</td></tr>
    <tr class="gbOdd"><td>{g->text text="Available space"}</td><td>{g->text text="%s out of %s remains" arg1=$d.free arg2=$d.capacity} ({$d.percent}%)</td></tr>
  </table>
  {/if}
</div>

<div class="gbBlock">
  {capture name="url"}{g->url arg1="controller=core.AdminMaintenance"
                   arg2="form[action][runTask]=1"
                   arg3="taskId=RecalculateStatisticsTask" authToken=1}{/capture}
  <button onclick="window.location='{$smarty.capture.url}'; return false;" />{g->text text="Recalculate"}</button>
</div>