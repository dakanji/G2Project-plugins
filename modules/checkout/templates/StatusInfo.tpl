{*
 * $Revision: 1689 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="checkout.transactionFlags" transactionId=$tpl.id}
<div class = "gbBlock"> 
 <h3>{g->text text="The status of this order is as follows:"}</h3>
 <ul>
 {if $block.checkout.problem}
 <li>{g->text text="There is a problem with this order. Please contact us for details."}</li>
 {elseif $block.checkout.hold}
  <li>{g->text text="There order has been placed on hold. Please contact us for details."}</li>
   
  {else}
  {* If > 1 contains a time - if = 1 then just a flag value *}
  {if $block.checkout.completed}
  <li>{if $block.checkout.completed > 1}{g->date timestamp=$block.checkout.completed style = "datetime"} : {/if}{g->text text="Completed"} </li>
  {elseif $block.checkout.fulfilled}
  <li>{if $block.checkout.fulfilled > 1}{g->date timestamp=$block.checkout.fulfilled style = "datetime"} : {/if}{g->text text="Completed"} </li>
  {/if}

  {if $block.checkout.cleared}
  <li>{if $block.checkout.cleared > 1}{g->date timestamp=$block.checkout.cleared style = "datetime"} : {/if}{g->text text="Payment cleared"}</li>
  {elseif $block.checkout.paid}
  <li>{if $block.checkout.paid > 1}{g->date timestamp=$block.checkout.paid style = "datetime"} : {/if}{g->text text="Payment received"} </li>
  {/if}
  
  {if $block.checkout.confirmed}
  <li>{if $block.checkout.confirmed > 1}{g->date timestamp=$block.checkout.confirmed style = "datetime"} : {/if}{g->text text="Order confirmed"}</li>
  {/if}
   
  {/if}
  <li>{g->date timestamp=$block.checkout.date style = "datetime"} : {g->text text="Order recorded"}</li>
  </ul>
 </div>