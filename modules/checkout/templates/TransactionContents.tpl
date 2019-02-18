{*
 * $Revision: 1711 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{* load data for this template *}
{g->callback type="checkout.transactionItems" transactionId=$tpl.id}
<div class="gbBlock">
<h3>
{g->text text="Order Contents"}
</h3>

<table class="gbDataTable gcBorder1 checkoutSelectTable" width="80%">
    <tr>
      <th>{g->text text="Item"}</th>
      <th>{g->text text="Info"}</th>
      <th>{g->text text="Product - Price - Quantity"}</th>
      <th align="right">{g->text text="Subtotal"}</th>    	
    </tr>
        {section name=x loop=$block.checkout.items}
        
        {* first image, or change of image from previous *}
        {if $smarty.section.x.first or ($block.checkout.items[x.index_prev].itemId != $block.checkout.items[x].itemId) } 
        {assign var="totalThisImage" value=0}
          <tr class="{cycle values="gbEven,gbOdd"}">    
            <td class="checkoutThumbnails">
              {if isset($block.checkout.items[x].fullItem) && isset($block.checkout.items[x].thumbnail)}
                {g->image item=$block.checkout.items[x].fullItem image=$block.checkout.items[x].thumbnail maxSize=150 class="checkoutThumbnail"}
              {else}
                {g->text text="No image available"}
              {/if}
            </td>    
            <td class="checkoutItems">
            <strong>{$block.checkout.items[x].itemName|markup}</strong><br/>
              {$block.checkout.items[x].itemText|markup}<br/>
            </td> 
            <td>
              <table cellspacing="0" cellpadding="0" width="100%">
        {/if} 
         {*repeat just this bit if this product is of the same image as the previous one*}
            <tr>
              <td class="checkoutProduct">{$block.checkout.items[x].productName}  {$block.checkout.items[x].notes}</td>
              <td class="checkoutProductPriceQuantity" align="right">
              	{$block.checkout.csymbol}&nbsp;{$block.checkout.items[x].priceEach|string_format:$block.checkout.cformat}&nbsp;&nbsp;
              	<span class="checkoutProductQuantity"><b>{$block.checkout.items[x].qty}</b></span>
              	{assign var=totalThisImage value=`$totalThisImage+$block.checkout.items[x].priceTotal`}
              </td>
            </tr>
            
        {* next image different, or this is the last *}
        {if $smarty.section.x.last or ($block.checkout.items[x.index_next].itemId != $block.checkout.items[x].itemId) } 
              </table>
            </td>
            <td align="right" class="checkoutProductPriceSubtotal">{$block.checkout.csymbol}&nbsp;{$totalThisImage|string_format:$block.checkout.cformat}</td>
          </tr>
        {/if}
        
        {/section} 
        {* discount *}
        {if $block.checkout.discount != ''}
        <tr class="{cycle values="gbEven,gbOdd"}">
            <td></td>
            <td></td>
            <td class="checkoutProductPricePostage"><font color="red"><strong>{g->text text="Discount"}:</strong><br>
            {$block.checkout.discountDescription}</font></td>
          <td align="right" class="checkoutProductPricePostage"><font color="red">{$block.checkout.csymbol}&nbsp;-{$block.checkout.discount|string_format:$block.checkout.cformat}</font></td>
        </tr>
        {/if}
        {* postage/delivery *}
        {if $block.checkout.postage != ''}
        <tr class="{cycle values="gbEven,gbOdd"}">
            <td></td>
            <td></td>
            <td class="checkoutProductPricePostage"><strong>{g->text text="Postage and Packing"}:</strong><br>
            {$block.checkout.postageType}</td>
          <td align="right" class="checkoutProductPricePostage">{$block.checkout.csymbol}&nbsp;{$block.checkout.postage|string_format:$block.checkout.cformat}</td>
        </tr>
        {/if}
        <tr class="{cycle values="gbEven,gbOdd"}">
          
          <td colspan="3" align="right">{g->text text="Total"}:</td>
          <td align="right" class="checkoutProductPriceTotal"><strong>{$block.checkout.csymbol}&nbsp;{$block.checkout.total|string_format:$block.checkout.cformat}</strong></td>
        </tr>
</table>
</div>
        
{if $block.checkout.comments != ''}
<div class="gbBlock">
<h3>
{g->text text="Special Instructions"}
</h3>
{$block.checkout.comments}
</div>
{/if}
