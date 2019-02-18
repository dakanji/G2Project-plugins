{*
 * $Revision: 1895 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{if $payment.paymentVariables.active}
{if $payment.paymentVariables.orderTotal > 0 && ($payment.paymentVariables.mode != 'test' || $user.isAdmin ) }
<td align="left" class="checkoutPaymentOptionText">
{if isset($form.error.checkoutgoogle.redirectFailed)}
    <div class="giError">
   {g->text l10Domain="modules_checkoutgoogle" text="Apologies. There was a problem preventing you from transferring to the Google Checkout service. Please try again, or choose another method of payment."}
   </div>
{else}
   {g->text l10Domain="modules_checkoutgoogle" text="Click the Google Checkout button to transfer to Google to enter your customer and payment details for this order."}
{/if}
</td>
<td align="right" class="checkoutPaymentButtons">
 <form action="{g->url}" method="post">
  {g->hiddenFormVars}
  <input type="hidden" name="{g->formVar var="controller"}" value="checkoutgoogle.SendData"/>
  <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
<input type="image"  name="{g->formVar var="form[action][send]"}" alt="{g->text text="Secure Payments by Google Checkout"}"
  	src="https://checkout.google.com/buttons/checkout.gif?merchant_id={$payment.paymentVariables.merchantId}&w=180&h=46&style=trans&variant=text&loc={$payment.paymentVariables.loc}" width="180" height="46"/>
</form> 
</td>
{/if}
{/if}