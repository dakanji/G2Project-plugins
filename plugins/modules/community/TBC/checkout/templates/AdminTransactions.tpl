{*
 * $Revision: 1649 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<div class="gbBlock gcBackground1">
  <h2> {g->text text="Orders"} </h2>
</div>

{if !empty($status)}
<div class="gbBlock">
  <h2 class="giSuccess">
    {if isset($status.transactionDeleted)}
      {g->text text="Order(s) successfully deleted."}
    {/if}
  </h2>
</div>
{/if}


{if !$txns|@count}
<div class="gbBlock">
{g->text text="There are no orders."}
</div>

{else}
<div class="gbBlock">
{if ($pageDetails.maxPages > 1)}
  <div style="margin-bottom: 10px"><span class="gcBackground1" style="padding: 5px">
    {if ($pageDetails.page > 1)}
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="page=1" 
    arg4="sortBy=`$pageDetails.sortBy`" arg5="showDelete=`$pageDetails.showDelete`"}">{g->text text="&laquo; first"}</a>
    &nbsp;
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="page=`$pageDetails.backPage`" 
    arg4="sortBy=`$pageDetails.sortBy`" arg5="showDelete=`$pageDetails.showDelete`"}">{g->text text="&laquo; back"}</a>
    {/if}

    &nbsp;
    {g->text text="Viewing page %d of %d" arg1=$pageDetails.page arg2=$pageDetails.maxPages}
    &nbsp;

    {if ($pageDetails.page < $pageDetails.maxPages)}
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="page=`$pageDetails.nextPage`" 
    arg4="sortBy=`$pageDetails.sortBy`" arg5="showDelete=`$pageDetails.showDelete`"}">{g->text text="next &raquo;"}</a>
    &nbsp;
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="page=`$pageDetails.maxPages`" 
    arg4="sortBy=`$pageDetails.sortBy`" arg5="showDelete=`$pageDetails.showDelete`"}">{g->text text="last &raquo;"}</a>
    {/if}
  </span></div>
{/if}

    <table class="gbDataTable">
    <tr>

    <th><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="sortBy=date"}">
    	{if $pageDetails.sortBy == "date"}<i>{/if}{g->text text="date"}{if $pageDetails.sortBy == "date"}</i></a>{/if}</th>
    	
    <th><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="sortBy=id"}">
    	{if $pageDetails.sortBy == "id"}<i>{/if}{g->text text="id"}{if $pageDetails.sortBy == "id"}</i>{/if}</a></th>
    	
    <th><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="sortBy=fullName"}">
    	{if $pageDetails.sortBy == "fullName"}<i>{/if}{g->text text="buyer"}{if $pageDetails.sortBy == "fullName"}</i>{/if}</a></th>
    	
   {* <th><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="sortBy=userId"}">
     	{if $pageDetails.sortBy == "userId"}<i>{/if}{g->text text="userId"}{if $pageDetails.sortBy == "userId"}</i>{/if}</a></th>  *}
     	
    <th><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="sortBy=recipientName"}">
     	{if $pageDetails.sortBy == "recipientName"}<i>{/if}{g->text text="recipient"}{if $pageDetails.sortBy == "recipientName"}</i>{/if}</a></th>
     	
   {*  <th><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="sortBy=host"}">
      	{if $pageDetails.sortBy == "host"}<i>{/if}{g->text text="host"}{if $pageDetails.sortBy == "host"}</i>{/if}</a></th>	 *}
      	
    <th><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="sortBy=paymentPlugin"}">
       	{if $pageDetails.sortBy == "paymentPlugin"}<i>{/if}{g->text text="payment method"}{if $pageDetails.sortBy == "paymentPlugin"}</i>{/if}</a></th>
       	
    <th align="right"><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="sortBy=amount"}">
        {if $pageDetails.sortBy == "amount"}<i>{/if}{g->text text="total"}{if $pageDetails.sortBy == "amount"}</i>{/if}</a></th>
    <th align="right">{g->text text="status"}</th> 
    {if $pageDetails.showDelete == 1}
    <th>{g->text text="Delete"}</th>
    {/if}
    </tr>
    {section name=x loop = $txns start=$pageDetails.start max=$pageDetails.pageSize}
       <tr class="{cycle values="gbEven,gbOdd"}">
       <td>{g->date timestamp=$txns[x][0]}</td>
       <td><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminOrder" arg3="transactionId=`$txns[x][1]`" }">{$txns[x][1]}</a></td>
       <td>{$txns[x][2]}</td>
    {* <td>{$txns[x][3]}</td>  *}
       <td>{$txns[x][4]}</td>  
    {* <td>{$txns[x][5]}</td>  *}
       <td>{$txns[x][6]}</td>  
       <td align="right">{$txns[x][7]|string_format:"%.2f"}</td>
       <td align="right">{if $txns[x][14]}<font color="red">{g->text text="problem"}</font>
           {elseif $txns[x][13]}<font color="red">{g->text text="hold"}</font>
           {elseif $txns[x][12]}<font color="grey">{g->text text="completed"}</font>
           {elseif $txns[x][11]}{g->text text="fulfilled"}
           {elseif $txns[x][9]}<font color="green">{g->text text="payment cleared"}</font>
           {elseif $txns[x][8]}{g->text text="paid"}
           {else}<font color="orange">{g->text text="unpaid"}</font>{/if}
	{if $pageDetails.showDelete == 1}
	<td><input type="checkbox" name="{g->formVar var="form[delete][`$txns[x][1]`]"}"/></td>
    	{/if}
       </tr>
    {/section}
    </table> 
    
{if ($pageDetails.maxPages > 1)}
  <div style="margin-bottom: 10px"><span class="gcBackground1" style="padding: 5px">
    {if ($pageDetails.page > 1)}
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="page=1" 
    arg4="sortBy=`$pageDetails.sortBy`" arg5="showDelete=`$pageDetails.showDelete`"}">{g->text text="&laquo; first"}</a>
    &nbsp;
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="page=`$pageDetails.backPage`" 
    arg4="sortBy=`$pageDetails.sortBy`" arg5="showDelete=`$pageDetails.showDelete`"}">{g->text text="&laquo; back"}</a>
    {/if}

    &nbsp;
    {g->text text="Viewing page %d of %d" arg1=$pageDetails.page arg2=$pageDetails.maxPages}
    &nbsp;

    {if ($pageDetails.page < $pageDetails.maxPages)}
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="page=`$pageDetails.nextPage`" 
    arg4="sortBy=`$pageDetails.sortBy`" arg5="showDelete=`$pageDetails.showDelete`"}">{g->text text="next &raquo;"}</a>
    &nbsp;
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminTransactions" arg3="page=`$pageDetails.maxPages`" 
    arg4="sortBy=`$pageDetails.sortBy`" arg5="showDelete=`$pageDetails.showDelete`"}">{g->text text="last &raquo;"}</a>
    {/if}
  </span></div>
{/if}
</div>

{/if}

<input type="hidden" name="{g->formVar var="form[page]"}" value="{$pageDetails.page}">
<div class="gbBlock gcBackground1">
{if $txns|@count}
{if $pageDetails.showDelete == 1}
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][delete]"}"  value="{g->text text="Delete Selected Orders"}"/>
{else}
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][showDelete]"}"  value="{g->text text="Delete Some Orders..."}"/>
{/if}

{/if}
<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][showCarts]"}"  value="{g->text text="Show Current Sessions"}"/>
</div>
