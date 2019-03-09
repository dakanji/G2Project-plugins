{*
 * $Revision: 1649 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Cart Contents"} </h2>
</div>

{if !isset($sessions)}
<div class="gbBlock">
{g->text text="There are no carts stored."}
</div>

{else}
<div class="gbBlock">
    <table class="gbDataTable">
    <tr>
    <th>{g->text text="Session Id"}</th>
    <th>{g->text text="user"}</th>
    <th>{g->text text="Session Last Modified"}</th>
    <th>{g->text text="IP address"}</th>
    <th>{g->text text="Last Transaction Id"}</th>
    <th>{g->text text="Items"}</th>
    </tr>  	
   
    {foreach from=$sessions item=session}
       {assign var="id" value=$session.userId}
       <tr class="{cycle values="gbEven,gbOdd"}">
       <td><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminSession" arg3="sessId=`$session.id`"}">{$session.id|truncate:13:"&hellip;":true}</td>
       <td>{$userNames.$id}</td>
       <td>{g->date timestamp=$session.sessionModTime style="datetime"}</td>
       <td>{$session.host}</a></td>
       <td><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminOrder" arg3="transactionId=`$session.transactionId`"}">{$session.transactionId}</a></td>
       <td>{$session.items|@count}</td>
       </tr>
    {/foreach}
    </table> 
</div>
{/if}