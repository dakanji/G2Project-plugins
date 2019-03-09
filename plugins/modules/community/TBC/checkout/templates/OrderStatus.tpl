{*
 * $Revision: 1588 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<form action="{g->url}" method="post">
{g->hiddenFormVars}
<input type="hidden" name="{g->formVar var="controller"}" value="{$tpl.controller}"/>
<input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
{if $tpl.isAdmin}
<div class="gbBlock gcBackground1">
  <h2 class="gbTitle">
   {g->text text="Site Admins Only"}
  </h2>
  </div>
<p>
  <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminOrder" arg3="transactionId=`$tpl.id`"}">
  <input type="button" class="inputTypeSubmit"  value="{g->text text="Order Administration Page"}"/>
  </a>
</p>
{/if}
 <div  class = "gcBorder1">
{if !empty($form.error.cancel) }
  <h2 class="giError">
	{g->text text="Order Cancelled."}
  </h2>
  <p>
  {g->text text="The order process was halted, but your cart has not been emptied."}
  </p>
{else}


{if isset($status.newlyPlaced)}
<div class="gbBlock gcBackground1">
  <h2 class="gbTitle">
   {g->text text="Your order is confirmed."}
  </h2>
</div>
 <p>
  {g->text text="Thank you very much for placing this order. You can bookmark this page and return to it to track progress of the order, and we'll also send you an email when the order status changes."}
</p>
{elseif isset($status.alreadyPlaced)}
<div class="gbBlock gcBackground1">
  <h2 class="gbTitle">
    {g->text text="Order Already Placed"}
  </h2>
<p>
  {g->text text="This order has already been placed. You can see its status on this page."}
</p>
</div>
{else}
<div class="gbBlock gcBackground1">
  <h2 class="gbTitle">
    {g->text text="Order Status"}
  </h2>
</div>
{/if}

<div class = "gbBlock"> 
<table class="gbDataTable">
   <tr class = "gbOdd">
   <td align=right><strong>{g->text text="Order Number"}</strong></td><td>{$tpl.id}</td>
</table>
</div>

{*plugins*}
{foreach from=$statusPlugins item=plugin}
  {include file="gallery:`$plugin.statusPageTemplate`" l10Domain=$plugin.l10Domain}
{/foreach}
    
{* status information *}
{include file='modules/checkout/templates/StatusInfo.tpl'}

{/if}
<div class="gbBlock gcBackground1">

  <input type="submit" class="inputTypeSubmit checkoutButton" name="{g->formVar var="form[action][continueShopping]"}" value="{g->text text="Continue shopping"}"/>
</form> 
</div>
</div>
