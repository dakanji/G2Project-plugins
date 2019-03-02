{*
 * $Revision: 1389 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="checkout.transactionItems" transactionId=$transactionId}

{if $rendering == 'text'}
{section name=x loop=$block.checkout.items}
{* first image, or change of image from previous *}
{if $smarty.section.x.first or ($block.checkout.items[x.index_prev].itemId != $block.checkout.items[x].itemId)} 
{assign var="totalThisImage" value=0}
=========================
{if $showLinks}
<{g->url arg1="view=core.ShowItem" arg2="itemId=`$block.checkout.items[x].itemId`" forceFullUrl=true}>
{/if}
   {g->text text="Title"}: {$block.checkout.items[x].itemName|markup}
   {$block.checkout.items[x].itemText|markup}
{/if} 
{*repeat just this bit if this product is of the same image as the previous one*}
{assign var=totalThisImage value=`$totalThisImage+$block.checkout.items[x].priceTotal`}
     {g->text text="Qty"}: {$block.checkout.items[x].qty}{g->text text="of"} {$block.checkout.items[x].productName} {$block.checkout.csymbol}{$block.checkout.items[x].priceEach|string_format:$block.checkout.cformat} {g->text text="each"}
   {g->text text="Notes"}: {$block.checkout.items[x].notes}
{* next image different, or this is the last *}
{if $smarty.section.x.last or ($block.checkout.items[x.index_next].itemId != $block.checkout.items[x].itemId) } 
{g->text text="Subtotal"}: {$block.checkout.csymbol}{$totalThisImage|string_format:$block.checkout.cformat}
{/if}
{/section}
        
=========================
{if $block.checkout.discount != ''}
{g->text text="Discount"}: -{$block.checkout.csymbol}{$block.checkout.discount}
{$block.checkout.discountDescription}
{/if}
{if $block.checkout.postage != ''}
{g->text text="Postage and Packing"}: {$block.checkout.csymbol}{$block.checkout.postage|string_format:$block.checkout.cformat}
{$block.checkout.postageType}
{/if}
              {g->text text="Total"}: {$block.checkout.csymbol}{$block.checkout.total|string_format:$block.checkout.cformat}
{/if}

{if $rendering == 'html'}
<table cellspacing="5">
  <tr>
    <th>{g->text text="Photo"}</th>
    <th>{g->text text="Info"}</th>
    <th>{g->text text="Product"}/{g->text text="Price"}/{g->text text="Quantity"}</th>
    <th>{g->text text="Subtotal"}</th>
  </tr>
  <tr>

   {section name=x loop=$block.checkout.items}
        
{* first image, or change of image from previous *}
{if $smarty.section.x.first or ($block.checkout.items[x.index_prev].itemId != $block.checkout.items[x].itemId) } 
{assign var="totalThisImage" value=0}        
        
      <tr>
        {if $showThumbs}
          <td>
  	    {if $showLinks}
	      <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$block.checkout.items[x].itemId`" forceFullUrl=true}">
	    {/if}
            {if isset($block.checkout.items[x].thumbnail) && isset ($block.checkout.items[x].fullItem)}
              {g->image item=$block.checkout.items[x].fullItem image=$block.checkout.items[x].thumbnail maxSize=150 forceFullUrl=true}
         
            {else}
              {g->text text="No thumbnail"}
            {/if}
  	    {if $showLinks}
	      </a>
	    {/if}
          </td>
	{else}
	  <td>
	    {if $showLinks}
	      <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$block.checkout.items[x].itemId`" forceFullUrl=true}">{$block.checkout.items[x].itemName}</a>
	    {else}
	      {$block.checkout.items[x].itemName}
	    {/if}
	  </td>
	{/if}
	<td align="top">
	  <strong>{g->text text="Title"}:</strong> 
	  {$block.checkout.items[x].itemName|markup}<br>
	  {$block.checkout.items[x].itemText|markup}<br>
	</td> 
	<td>
	  <table cellspacing="0" cellpadding="10" border="1" bordercolor="#999999">
{/if} 
    
	  
{*repeat just this bit if this product is of the same image as the previous one*}
{assign var=totalThisImage value=`$totalThisImage+$block.checkout.items[x].priceTotal`}
	        <tr>
	          <td><center>{$block.checkout.items[x].productName} {$block.checkout.items[x].notes}</td>
	          <td><center>{$block.checkout.csymbol}{$block.checkout.items[x].priceEach|string_format:$block.checkout.cformat}</td>
	          <td><center><b>{$block.checkout.items[x].qty}</td>
	        </tr>

{* next image different, or this is the last *}
{if $smarty.section.x.last or ($block.checkout.items[x.index_next].itemId != $block.checkout.items[x].itemId) } 

	     	    		
	  </table>
	</td>
	<td align="right"><strong>{$block.checkout.csymbol}{$totalThisImage|string_format:$block.checkout.cformat}</strong></td>
      </tr>
      
{/if}
{/section}
{if $block.checkout.discount != ''}
    <tr>
       <td colspan="3" align="right">{g->text text="Discount"}:<br>{$block.checkout.discountDescription}</td>
       <td align="right"><b>-{$block.checkout.csymbol}{$block.checkout.discount|string_format:$block.checkout.cformat}</b></td>
    </tr>
{/if}
{if $block.checkout.postage != ''}
    <tr>
       <td colspan="3" align="right">{g->text text="Postage and Packing"}:<br>{$block.checkout.postageType}</td>
       <td align="right"><b>{$block.checkout.csymbol}{$block.checkout.postage|string_format:$block.checkout.cformat}</b></td>
    </tr>
{/if}
    <tr>
      <td colspan="3" align="right"><strong>{g->text text="Total"}:</strong></td>
      <td align="right"><strong>{$block.checkout.csymbol}{$block.checkout.total|string_format:$block.checkout.cformat}</strong></td>
    </tr>
  </tr>

{/if}