{*
 * $Revision: 1786 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if empty($item)} {assign var=item value=$theme.item} {/if}
{g->callback type="checkout.itemPrices" itemId=$item.id}
{if $block.checkout.showProducts }
<div class="{$class}">
  <h3>{g->text text="Prices"}</h3>
  <table class="gbDataTable">
   {foreach from=$block.checkout.products item=thisProduct}
     <tr class="{cycle values="gbEven,gbOdd"}">   
     <td class="checkoutProduct"> <a class="{g->linkId view="checkout.AddToCart"}" href="{g->url arg1="controller=checkout.AddToCart" arg2="itemId=`$item.id`|`$thisProduct.productId`|1" arg3="return=1"}" rel="nofollow">{$thisProduct.name}</a></td>
     <td class="checkoutProductPriceQuantity" align="right">{if isset($thisProduct.price)}{$block.checkout.csymbol}{$thisProduct.price}{else}&nbsp;{/if}</td>
     </tr>
   {/foreach}
  </table>
</div>
{/if}
