{*
 * $Revision: 1389 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="checkout.transactionFlags" transactionId=$transactionId}
{if $rendering == 'text'}
{g->text text="Order Status:"}
 {if $block.checkout.problem}
{g->text text="There is a problem with this order. Please contact us for details."}
 {elseif $block.checkout.hold}
{g->text text="There order has been placed on hold. Please contact us for details."}
   {elseif $block.checkout.completed}
{g->text text="This order has been completed."}
   {elseif $block.checkout.fulfilled}
{g->text text="This order has been completed."}
   {elseif $block.checkout.cleared}
{g->text text="Payment for this order has cleared, and it is being processed."}
   {elseif $block.checkout.paid}
{g->text text="Payment has been received, and we are waiting for it to clear."}
   {else}
{g->text text="We are waiting for payment for this order."}
  {/if}
{/if}

{if $rendering == 'html'}
<strong>{g->text text="Order Status:"}</strong>
  {if $block.checkout.problem}
{g->text text="There is a problem with this order. Please contact us for details."}
 {elseif $block.checkout.hold}
{g->text text="There order has been placed on hold. Please contact us for details."}
   {elseif $block.checkout.completed}
{g->text text="This order has been completed."}
   {elseif $block.checkout.fulfilled}
{g->text text="This order has been completed."}
   {elseif $block.checkout.cleared}
{g->text text="Payment for this order has cleared, and it is being processed."}
   {elseif $block.checkout.paid}
{g->text text="Payment has been received, and we are waiting for it to clear."}
   {else}
{g->text text="We are waiting for payment for this order."}
  {/if}
<br/>
<br/>
{/if}