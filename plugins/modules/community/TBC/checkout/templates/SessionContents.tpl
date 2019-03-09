{*
 * $Revision: 1711 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}

     <table class="gbDataTable gcBorder1 checkoutSelectTable" width="80%">
     <tr><th colspan="4"><h3>{g->text text="Order Contents"}</h3></th></tr>
      <tr>
        <th>{g->text text="Remove"}</th>
        <th>{g->text text="Item"}</th>
        <th>{g->text text="Info"}</th>
        <th>{g->text text="Product - Price - Quantity"}</th>
      </tr>

    {if isset($form.error.noQuantity)}
    <tr>
    <td></td>
    <td></td>
    <td></td>
       <td style="text-align:right;"><span class="checkoutErrorText" style="color:Red;font-weight:bold;">
		{g->text text="You must set a quantity for at least one item before continuing"}<br><br></span>
    </td></tr>
    {/if}
    
        {if isset($form.error.noPaper)}
    <tr>
    <td></td>
    <td></td>
    <td></td>
       <td style="text-align:right;"><span class="checkoutErrorText" style="color:Red;font-weight:bold;">
		{g->text text="Please confirm your option selections."}<br><br></span>
    </td></tr>
    {/if}
              {if isset($form.error.minValue)}
    <tr>
    <td></td>
    <td></td>
    <td></td>
    <td style="text-align:right;"><span class="checkoutErrorText" style="color:Red;font-weight:bold;">
		{g->text text="Your order doesn't reach the minimum value required."}<br><br></span>
    </td></tr>
    {/if}
      	  {* ----- Loop through each item ------- *}
          {foreach from=$tpl.items item=item}
                   
            <tr class="{cycle values="gbEven,gbOdd"}">    
              <td style="text-align:center;">
                <input type="checkbox" name="{g->formVar var="form[photos][`$item.id`][remove]"}"/>
              </td>
              <td class="checkoutThumbnails">
                <a name="{$item.id}" href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$item.id`"}">
                {if isset($tpl.thumbnails[$item.id])}
                  {g->image item=$tpl.items[$item.id] image=$tpl.thumbnails[$item.id] maxSize=150 class="checkoutThumbnail"}
                {else}
                  {g->text text="No thumbnail"}
                {/if}
                </a>
              </td>			    
              <td class="checkoutItems">
                <strong>{g->text text="Title"}:</strong> 
                {$item.fullItemDetails.title|default:''|markup}<br/>
                <strong>{g->text text="Album"}:</strong> 
                {$item.parentTitle|default:''|markup}<br/>
                <strong>{g->text text="Summary"}:</strong> 
                {$item.fullItemDetails.summary|default:''|markup}			               
              </td>
              <td>
                <table cellspacing="0" cellpadding="0" width="100%" class="checkoutProducts">
                  
                  {* ---- loop through all products for this item ----- *}
                  {if $tpl.itemProducts[$item.id]|@count == 0} 
                  <tr><td class="checkoutProduct">{g->text text="There are no products available for this item."}</td></tr>
                  {else}
                  {foreach from=$tpl.itemProducts[$item.id] item=thisProduct}
                      {if isset($item[$thisProduct.productId].quant) or $thisProduct.dropdown != 'true' or $item.comboCount < 2 }
                  <tr>
                  	<td class="checkoutProduct">{$thisProduct.name}{$thisProduct.description|default:''|markup}</td>
                  	<td>{*papers column*}
                          {if $thisProduct.paperSet}
                          {foreach  from=$tpl.paper item=paperType name=i}
                          {if isset($paperType[$thisProduct.paperSet])}
                            <input type="radio" class="checkoutPaperType" name="{g->formVar var="form[photos][`$item.id`][paper][`$thisProduct.productId`]"}" value="{$paperType[$thisProduct.paperSet]}"
                            {if (isset($item[$thisProduct.productId].paper) && $item[$thisProduct.productId].paper == $paperType[$thisProduct.paperSet])
                             || (!isset($item[$thisProduct.productId].paper) && $smarty.foreach.i.index == 0)} checked="checked"{/if}/>{$paperType[$thisProduct.paperSet]}<br>
                          {/if}
                          {/foreach}
                          {else}&nbsp;{*for IE's benefit in certain table configs*}
                          {/if}
                        </td>
                    <td class="checkoutProductPriceQuantity" align="right">
                          {if isset($thisProduct.price)}{$tpl.csymbol}{$thisProduct.price|string_format:$tpl.cformat}{/if}&nbsp;&nbsp;
		    </td>
		    <td class="checkoutProductPriceQuantity" align="right">
                      <span class="checkoutProductQuantity">
                      <input type="text" name="{g->formVar var="form[photos][`$item.id`][quant][`$thisProduct.productId`]"}" 
				value="{$item[$thisProduct.productId].quant|default:''}" 
				size="1" style="text-align:right;" />
                      </span>
                    </td>
                  </tr>
                      {/if}
                  {/foreach}
                  
                  {if $item.comboCount > 1}
                  <tr>
                    <td class="checkoutProduct" colspan="3">
                      <select name="{g->formVar var="form[photos][`$item.id`][newproduct]"}">
                    {assign var="comboQty" value=""} 
                    <option value="" selected="selected">{g->text text="(Select A Product)"}</option>
                    {* ---- loop through all products again for drop-down box ------ *}
                     {foreach from=$tpl.itemProducts[$item.id] item=thisProduct}
                      {if !isset($item[$thisProduct.productId].quant) && $thisProduct.dropdown == 'true'}
                        <option value="{$thisProduct.productId}">
                        {$thisProduct.name}{if isset($thisProduct.price)}  - {$tpl.csymbol} {$thisProduct.price|string_format:$tpl.cformat} &nbsp;&nbsp;&nbsp;{/if}</option>
                      {/if}
                    {/foreach}		
                      </select>
                    </td>
                    <td class="checkoutProductQuantity" align="right">
                      <input type="text" name="{g->formVar var="form[photos][`$item.id`][newqty]"}" size="1" style="text-align:right;" value=""/>
                    </td>
                  </tr>
                  
                  {/if}
                  {/if}
                </table>
              </td>
            </tr>
          {/foreach}   {* -------- item ------ *}
    
          
             {if isset($form.error.noQuantity)}
    <tr>
    <td></td>
    <td></td>
    <td></td>
    <td style="text-align:right;"><span class="checkoutErrorText" style="color:Red;font-weight:bold;">
		{g->text text="You must set a quantity for at least one item before continuing"}<br><br></span>
    </td></tr>
    {/if}
              {if isset($form.error.noPaper)}
    <tr>
    <td></td>
    <td></td>
    <td></td>
    <td style="text-align:right;"><span class="checkoutErrorText" style="color:Red;font-weight:bold;">
		{g->text text="Please confirm your option selections."}<br><br></span>
    </td></tr>
    {/if}
              {if isset($form.error.minValue)}
    <tr>
    <td></td>
    <td></td>
    <td></td>
    <td style="text-align:right;"><span class="checkoutErrorText" style="color:Red;font-weight:bold;">
		{g->text text="Your order doesn't reach the minimum value required."}<br><br></span>
    </td></tr>
    {/if}
     
    {* ------ Accessories ----------------- *}
    {if $tpl.accessories|@count > 0}
    <tr><th colspan="4"><h3>{g->text text="Accessory Items"}</h3></th></tr>
    {foreach from=$tpl.accessories item=item key=index}
                   
            <tr class="{cycle values="gbEven,gbOdd"}">    
              <td style="text-align:center;">
              </td>
              
              <td class="checkoutThumbnails">
                {if isset($tpl.thumbnails[$item.id])}
                  {g->image item=$tpl.accessories[$index] image=$tpl.thumbnails[$item.id] maxSize=100 class="checkoutThumbnail"}
                {else}
                  {g->text text="No picture available"}
                {/if}
              </td>			    
              <td class="checkoutItems">
                <strong>{$item.fullItemDetails.title|markup}</strong><br/>
                {$item.fullItemDetails.description|markup}	         
              </td>
              
              <td>
                <table cellspacing="0" cellpadding="0" width="100%" class="checkoutProducts">
                  
                  {* ---- loop through all products for this item ----- *}
                  {if $item.options|@count == 0} 
                  <tr><td class="checkoutProduct">{g->text text="There are no options available for this item."}</td></tr>
                  {else}
                  {foreach from=$item.options item=thisOption}
                  <tr>
                    <td class="checkoutProduct">{$thisOption.name}{$thisOption.description|default:''|markup}</td>
                    <td class="checkoutProductPriceQuantity" align="right">
                          {if isset($thisOption.price)}{$tpl.csymbol}{$thisOption.price|string_format:$tpl.cformat}{/if}&nbsp;&nbsp;
		    </td>
		    <td class="checkoutProductPriceQuantity" align="right">
                      <span class="checkoutProductQuantity">
                      <input type="text" name="{g->formVar var="form[accessories][`$thisOption.id`][quant]"}" 
                      	value="{$tpl.chosenAccessories[$thisOption.id].quant|default:''}" 
                      	size="1" style="text-align:right;" />
                      </span>
                    </td>
                  </tr>
                  {/foreach} {* -------- option ------ *}
                  {/if}
                </table>
              </td>
            </tr>
          {/foreach}   {* -------- accessory ------ *}
    {/if}
    
          

    
    <tr><th colspan="4"><h3>{g->text text="Other Information"}</h3></th></tr>
    
    {* -------- Discounts ------------------ *}
    {if $tpl.askDiscountCode}
    <tr><td></td>
    <td></td>
    <td><strong>{g->text text="If you have a discount code, enter it here: "}</strong><br></td>
	<td align="right"><input name = "{g->formVar var="form[discountCode]"}" 
	     type="text" size = "10" value="{$tpl.discountCode|default:''}"></td>
    </tr>
    {else}
      {if isset($tpl.discountCode)}
      <input name = "{g->formVar var="form[discountCode]"}" type="hidden" value="{$tpl.discountCode}">
      {/if}
    {/if}
    
    {* -------- Postage Section ------------ *}
   
    
    {if isset($form.error.noPostage)}
    <tr>
    <td></td>
    <td></td>
    <td></td>
    <td style="text-align:right;"><span class="checkoutErrorText" style="color:limegreen;font-weight:bold;">
		{g->text text="Your order requires postage."}<br>
		{g->text text="Please confirm your selection."}<br></span>
    </td></tr>
    {/if}

    {if ($tpl.postageOptions|@count) }
    <tr><td></td>
        <td></td>

    <td><strong>{g->text text="Choose a Postage, Packing &amp; Delivery option:"}</strong><br></td>
   
    <td align="right">
    
    {if isset ($tpl.postageOptions[0])} {* any postage options at all*}
 
    {if isset($tpl.postageOptions[1])} {* more than 1 postage option *}
    	
    <select name = "{g->formVar var="form[postage]"}">
    <option value = "" {if !isset($tpl.items.postage)}selected="selected"{/if}>{g->text text="(Choose a Postage and Delivery Option)"}</option>
    {section name=x loop=$tpl.postageOptions}
    <option value = "{$smarty.section.x.index}"  {if $smarty.section.x.index == $tpl.postageSet}selected{/if}>{$tpl.postageOptions[x].type} - {g->text text="from"} {$tpl.csymbol}{$tpl.postageOptions[x].price[1]|string_format:$tpl.cformat}</option>
    {/section}
    </select>
    	</td>
    {else} {* only 1 postage option *}
    <input name = "{g->formVar var="form[postage]"}" type="hidden" value = "0">{$tpl.postageOptions[0].type} - {g->text text="from"} {$tpl.csymbol}{$tpl.postageOptions[0].price[1]|string_format:$tpl.cformat}
     {/if} {* end only 1 postage option *}
     {/if} {* end exist any postage options at all *}
    </td></tr> 
    {/if}  {* end should show postage options *}
    
    
    {* --------- special instructions ----------- *}
    
    <tr><td></td>
    <td></td>
    <td><strong>{g->text text="If you have special instructions enter them here:"}</strong><br></td>
	<td align="right"><textarea name="{g->formVar var="form[instructions]"}" rows="4" cols="40">
	{$tpl.instructions|default:''}</textarea></td>
    </tr>
   	
    </table>
