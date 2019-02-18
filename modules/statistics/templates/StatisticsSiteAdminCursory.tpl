{*
 * $Revision: 1099 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock">
  <h3>{g->text text="Cursory"}</h3>
  <table class="gbDataTable" width="50%">
    <tr><th width="50%">{g->text text="Statistic"}</th><th width="50%">{g->text text="Value"}</th></tr>
    {assign var="s" value=$StatisticsSiteAdmin.Cursory}
    <tr class="{cycle values="gbEven,gbOdd"}"><td>{g->text text="Number of Items"}</td><td>{$s.items}</td></tr>
    <tr class="{cycle values="gbEven,gbOdd"}"><td>{g->text text="Number of Albums"}</td><td>{$s.albums}</td></tr>
    <tr class="{cycle values="gbEven,gbOdd"}"><td>{g->text text="Total Views"}</td><td>{$s.views}</td></tr>
    <tr class="{cycle values="gbEven,gbOdd"}"><td>{g->text text="Total Users"}</td><td>{$s.users}</td></tr>
  </table>
</div>