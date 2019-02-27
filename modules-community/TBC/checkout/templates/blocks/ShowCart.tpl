{*
 * $Revision: 1649 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="checkout.countCheckoutItems"}
<div class="{$class}">
{if isset($block.checkout.items.discountCode)}
<div id="cartSplashOuter"><div id="CartSplashInner">Discount Code:{$block.checkout.items.discountCode} </div></div>
{/if}
  {if isset($status.checkoutSuccess)}
  <br>
  <h4 class="giSuccess">{g->text text="Item(s) added OK"}</h4>
  {/if}
{if $block.checkout.itemCount}
  <h3>{g->text one="You have the following item in your shopping cart:" many="You have the following %d items in your cart:"
             count=$block.checkout.itemCount arg1=$block.checkout.itemCount}</h3>
   <p>
  {foreach from=$block.checkout.items key=id item=item}
 {if is_numeric($id)}
    <a href="{g->url arg1="view=checkout.Select" arg2="return=`$theme.item.id`"}#{$id}">
     {if isset($block.checkout.thumbnails[$id])}{g->image item=$item image=$block.checkout.thumbnails[$id] maxSize=100 heightVar=100 class="checkoutThumbnail"}
     {else}{g->text text="No thumbnail"}
     {/if}</a>
  {/if}
  {/foreach}
  </p>
  <p>
   <a class="{g->linkId view="checkout.SelectProducts"}" href="{g->url arg1="view=checkout.Select" arg2="return=`$theme.item.id`"}">{g->text text="View and adjust quantities and complete your order here."}</a>
  </p>
  <br>
{else}
  <h3>{g->text text="Your shopping cart is empty."}</h3>
{/if}
</div>

