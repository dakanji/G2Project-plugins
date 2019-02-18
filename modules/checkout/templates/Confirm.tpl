{*
 * $Revision: 1689 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div id="gsContent" class = "gcBorder1">
<div class="gbBlock gcBackground1">
<h2 class="gbTitle">
{g->text text="Checkout"} {g->text text="Step 2 - Confirm your selection"}
</h2>
<p>
{g->text text="Take a moment to make sure the contents of your order are correct, then please proceed to the payment stage."}
</p>
</div>

{* include cart contents *}
{include file='gallery:modules/checkout/templates/TransactionContents.tpl'}
<div class="gbBlock">
  <form action="{g->url}" method="post"> 
     <table class="checkoutButtonTable checkoutPaymentOptions" width="100%">
     {if $tpl.paymentPlugin == ''}
     <tr>
        <td>&nbsp;</td>  
        <td align="right" class="checkoutPaymentButtons"> 
	{g->hiddenFormVars}
	<input type="hidden" name="{g->formVar var="controller"}" value="{$tpl.controller}"/>
	<input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
        <input type="submit" class="inputTypeSubmit checkoutButton" name="{g->formVar var="form[action][modify]"}" value="{g->text text="Modify Order"}"/>
        </td>
      </tr>
      {/if}
      <tr>
        <td>&nbsp;</td>
	<td align="right">
	<input type="submit" class="inputTypeSubmit checkoutButton checkoutShoppingButton1" 
	name="{g->formVar var="form[action][continueshopping]"}" 
	value="{g->text text="Continue Shopping"}"/>
      	</td>
      </tr>
  </table> 
        </form>
</div>

<div class="gbBlock">
{if $tpl.paymentPlugin == ''}
<h3 class="gbTitle">
    {g->text text="Payment Options"}
</h3>
    <table class="checkoutButtonTable checkoutPaymentOptions" width="100%">
      
      {if isset($tpl.requireTandCs) and $tpl.requireTandCs}
      <tr>
        <td align="left" class="checkoutPaymentOptionText">
        <span class="checkoutTandCtext">
        {g->text text="By continuing you confirm that you have read and agree to the"}
        {if isset($tpl.TandCLink)}<a href="{$tpl.TandCLink}" target="_blank"><span class="checkoutLink">{/if}
        {g->text text="Terms and Conditions"}
        {if isset($tpl.TandCLink)}</span></a>{/if}
        </span>
        </td>
        <td align="right" class="checkoutPaymentButtons"></td>
      </tr>
      {/if}
      
    {foreach from=$tpl.payments item=payment}
          {include file="gallery:`$payment.paymentTemplate`"}
    <tr><td>&nbsp</td><td>&nbsp</td></tr>
    {/foreach}
    
    </table>
{else}
<h3 class="gbTitle">
    {g->text text="Order already submitted."}
</h3>
<p>
{g->text text="This order has already been recorded. If you want to place another order, please use the 'Continue Shopping' button."}
</p>
{/if}
</div>


    
</div>