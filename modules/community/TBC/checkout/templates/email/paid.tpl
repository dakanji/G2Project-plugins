{*
 * $Revision: 1649 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
--{$boundary}
{assign var=rendering value='text'}
Content-Type: text/plain; charset=utf-8;
Content-Transfer-Encoding: 8bit

{g->text text="Order Number:"} {$transactionId}

{$additionalText}

{g->text text="Payment for this order has been received and it is being processed."}
{g->text text="You can check the status of this order any time, at this location: "}
{g->callback type="checkout.transactionVerifyDetails" transactionId=$transactionId}
{g->url arg1="view=checkout.OrderStatus" arg2="transactionId=`$transactionId`" arg3="verify1=`$block.checkout.verify1`" arg4="verify2=`$block.checkout.verify2`" forceFullUrl=true htmlEntities=false forceSessionId=false}

{*plugins*}
{foreach from=$emailPlugins item=plugin}
  {include file="gallery:`$plugin.emailTemplate`"  l10Domain=$plugin.l10Domain}
{/foreach}
{if $showCustomer}
  {include file="gallery:modules/checkout/templates/email/CustomerDetails.tpl"  l10Domain="modules_checkout"}
{/if}
{if $showContents}
  {include file="gallery:modules/checkout/templates/email/TransactionContents.tpl"  l10Domain="modules_checkout"}
{/if}


--{$boundary}
Content-Type: text/html; charset=utf-8
Content-Transfer-Encoding: 8bit
{assign var=rendering value='html'}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>{g->text text="Order information"}</title>
</head>
<body>

{g->text text="Order Number:"} {$transactionId}
<br>
<br>
<pre>{$additionalText}
</pre>
<br>
{g->text text="Payment for this order has been received and it is being processed."}<br>
{g->text text="You can check the status of this order any time here:"}

<form action="{g->url arg1="view=checkout.OrderStatus" arg2="transactionId=`$transactionId`" arg3="verify1=`$block.checkout.verify1`" arg4="verify2=`$block.checkout.verify2`" forceFullUrl=true forceSessionId=false}" method="post">
    <button type="submit">{g->text text="Order Status"}</button>
</form>
<br>
{*plugins*}
{foreach from=$emailPlugins item=plugin}
  {include file="gallery:`$plugin.emailTemplate`"  l10Domain=$plugin.l10Domain}
{/foreach}

<br>
{if $showCustomer}
  {include file="gallery:modules/checkout/templates/email/CustomerDetails.tpl"  l10Domain="modules_checkout"}
{/if}
{if $showContents}
  {include file="gallery:modules/checkout/templates/email/TransactionContents.tpl"  l10Domain="modules_checkout"}
{/if}



</body>
</html>

--{$boundary}--
