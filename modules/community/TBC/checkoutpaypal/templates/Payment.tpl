 {*
 * $Revision: 1798 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
 {if $payment.paymentVariables.active}
 {if $payment.paymentVariables.orderTotal > 0}
 <tr>
    <td align="left" class="checkoutPaymentOptionText">
    {g->text l10Domain="modules_checkoutpaypal" text="Clicking the PayPal button will redirect you to PayPal where you can pay by credit card. Your information is securely encrypted and cannot be intercepted. You do not need a PayPal account to complete the transaction via PayPal."}
    </td>
    
    <td align="right" class="checkoutPaymentButtons">
    <form action="https://{$payment.paymentVariables.ppurl}/cgi-bin/webscr" method="post">
  <input type="hidden" name="cmd" value="_cart" />
  <input type="hidden" name="rm" value="2" />
  <input type="hidden" name="return" value="{g->url arg1="controller=checkout.Complete" arg2="transactionId=`$payment.paymentVariables.transactionId`" arg3="verify1=`$payment.paymentVariables.verify[1]`" arg4="verify2=`$payment.paymentVariables.verify[2]`" forceFullUrl=true}" />  
  <input type="hidden" name="cancel_return" value="{g->url arg1="controller=checkout.Complete" arg2="error=cancel" arg3="transactionId=`$payment.paymentVariables.transactionId`" arg4="verify1=`$payment.paymentVariables.verify[1]`" arg5="verify2=`$payment.paymentVariables.verify[2]`" forceFullUrl=true}" />
  <input type="hidden" name="notify_url" value="{g->url arg1="view=checkoutpaypal.IPN" forceFullUrl=true}" /> 
  <input type="hidden" name="image_url" value="{$payment.paymentVariables.imageUrl}" /> 
  <input type="hidden" name="upload" value="1" />
  <input type="hidden" name="no_shipping" value="2" />
  <input type="hidden" name="charset" value="utf-8">
  <input type="hidden" name="business" value="{$payment.paymentVariables.paypalname}" />
  <input type="hidden" name="currency_code" value="{$payment.paymentVariables.ccode}" />
  <input type="hidden" name="invoice" value="{$payment.paymentVariables.transactionId}" />
  <input type="hidden" name="lc" value="{$payment.paymentVariables.langCountry}" />
  {if !empty($payment.paymentVariables.pprtnbuttontext)}
  <input type="hidden" name="cbt" value="{$payment.paymentVariables.pprtnbuttontext}"/>
  {/if}
  {foreach from=$payment.paymentVariables.ppItems item=item}
    <input type="hidden" name="{$item.ppname}" value="{$item.name}" />
    <input type="hidden" name="{$item.ppnumber}" value="{$item.number}" />
    <input type="hidden" name="{$item.ppamount}" value="{$item.amount}" />
    <input type="hidden" name="{$item.ppshipping}" value="{$item.shipping}" />
    <input type="hidden" name="{$item.ppquantity}" value="{$item.quantity}" />
      {foreach from=$item.options key=k item=i}
        <input type="hidden" name="{$k}" value="{$i}" />
      {/foreach}	
  {/foreach}	
  <div style="width:200px">
  <input type="image" src="http://www.paypal.com/en_US/i/btn/x-click-but01.gif" 
	name="submit" alt="{g->text text="Pay with PayPal"}" />
  <input type="image" name="submit" alt="{g->text text="Pay with PayPal"}" 
	src="{g->url href="modules/checkoutpaypal/images/"}logo_ccVisa.gif" />
  <input type="image" name="submit" alt="{g->text text="Pay with PayPal"}" 
	src="{g->url href="modules/checkoutpaypal/images/"}logo_ccMC.gif" />
  </div>
</form>
    </td>
  </tr>
{/if}
{/if}