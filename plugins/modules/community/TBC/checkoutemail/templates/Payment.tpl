{*
 * $Revision: 1583 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{if $payment.paymentVariables.active}
<tr>
<td align="left" class="checkoutPaymentOptionText">
{$payment.paymentVariables.paymentText}
</td>
<td align="right" class="checkoutPaymentButtons">
 <form action="{g->url}" method="post">
  {g->hiddenFormVars}
  <input type="hidden" name="{g->formVar var="controller"}" value="checkoutemail.Details"/>
  <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
  <input type="submit"  class="inputTypeSubmit checkoutButton" name="{g->formVar var="form[action][email]"}" value="{$payment.paymentVariables.paymentButtonText}"/>
</form>
</td>
</tr>
{/if}
