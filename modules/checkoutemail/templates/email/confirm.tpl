{*
 * $Revision: 1646 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
 {g->callback type="checkout.transactionTotal" transactionId=$transactionId}
{if $rendering == 'text'}
{g->text text="The total payable for this order is "}{$block.checkout.csymbol}{$block.checkout.total}. 

{g->text text="Your payment options are:"}

IMPORTANT: REPLACE THIS TEXT WITH YOUR PAYMENT DETAILS

{g->text text="As soon as your payment is received your order will be processed - please let us know how you'd like to proceed by replying to this email."}
{g->text text="Looking forward to hearing from you soon!"}



{/if}
{if $rendering == 'html'}
<br>
<br>
{g->text text="The total payable for this order is "}{$block.checkout.csymbol}{$block.checkout.total|string_format:$block.checkout.cformat}.
<br>
<br>
{g->text text="Your payment options are:"}
<br>
<br>
IMPORTANT: REPLACE THIS TEXT WITH YOUR PAYMENT DETAILS
<br>
<br>
{g->text text="As soon as your payment is received your order will be processed - please let us know how you'd like to proceed by replying to this email."}
<br>
<br>
{g->text text="Looking forward to hearing from you soon!"}
<br>
<br>
<br>
{/if}