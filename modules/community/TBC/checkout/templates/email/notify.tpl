{*
 * $Revision: 1649 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
--{$boundary}
Content-Type: text/plain; charset=utf-8;
Content-Transfer-Encoding: 8bit

{g->text text="Administrative Notification:"}

{$additionalText}

{*plugins*}
{foreach from=$emailPlugins item=plugin}
  {include file="gallery:`$plugin.emailTemplate`"  l10Domain=$plugin.l10Domain}
{/foreach}

{g->callback type="checkout.transactionVerifyDetails" transactionId=$transactionId}
{g->text text="You can check the status of this order any time at this location:"}
{g->url arg1="view=checkout.OrderStatus" arg2="transactionId=`$transactionId`" arg3="verify1=`$block.checkout.verify1`" arg4="verify2=`$block.checkout.verify2`" forceFullUrl=true htmlEntities=false forceSessionId=false}
--{$boundary}--
