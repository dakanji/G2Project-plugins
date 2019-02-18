{*
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Custom Product Settings"} </h2>
</div>


    {if ($isRootAlbum)}
   <p class="giDescription">
     {g->text text="This is the root album, settings here can apply to everything in your Gallery."}
   </p>
     {else}
     
     <p class="giDescription">
    {g->text text="Values set here take precedence over global settings and settings in parent albums, and will apply to child albums and photos unless overriden there."}
     </p>
     <p class="giDescription">
    {g->text text="If a product already has any local settings you can modify them, or erase them all by ticking the Erase box."}
     </p>
     {/if}

  <table class="gbDataTable">
    <tr>
      <th>{g->text text="Root Name"}</th>
      <th>{g->text text="Local Name"}</th>
      <th>{g->text text="Description"}</th>
      <th>{g->text text="Price"}</th>
      <th>{g->text text="Visible"}</th>
      <th>{g->text text="Initial Quantity"}</th>
      <th>{g->text text="Quantity Limit"}</th>
      {if (!$isRootAlbum)}<th>{g->text text="Erase All Local Settings"}</th>{/if}
    </tr>
  {foreach from=$form.products item=product }
    <tr>
      <td>{$product.rootName}</td>
      <td><input type="text" name="{g->formVar var="form[products][`$product.id`][name]"}" value="{$product.name|default:''}" size = "20"></td>
      <td><input type="text" name="{g->formVar var="form[products][`$product.id`][description]"}" value="{$product.description|default:''}" size = "50"></td>
      <td>{$csymbol}<input type="text" name="{g->formVar var="form[products][`$product.id`][price]"}" value="{$product.price|default:''}" size = "6" style="text-align:right;"></td>
      <td><input type="radio" name="{g->formVar var="form[products][`$product.id`][visible]"}" value="true" {if isset($product.visible) && $product.visible == "true"}checked{/if}> Yes 
          <input type="radio" name="{g->formVar var="form[products][`$product.id`][visible]"}" value="false" {if isset($product.visible) && $product.visible == "false"}checked{/if}> No</td>
      <td><input type="text" name="{g->formVar var="form[products][`$product.id`][initialQuantity]"}" value="{$product.initialQuantity|default:''}" size = "3" style="text-align:center;"></td>
      <td><input type="text" name="{g->formVar var="form[products][`$product.id`][quantityLimit]"}" value="{$product.quantityLimit|default:''}" size = "3" style="text-align:center;"></td>
      {if (!$isRootAlbum)}
      <td>{if $product.offerRemove == "true"}<input type="checkbox" name="{g->formVar var="form[products][`$product.id`][remove]"}" value = "true">{/if}</td>
      {/if}
    </tr>
  {/foreach}
  </table>
</div>

{if isset ($form.accessories)}
<div class="gbBlock">
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Offer this Item as an Accessory"} </h2>
</div>
<p class="giDescription"> 
{g->text text="Setting Accessory Options means this item will appear every time the cart contents are shown, with the customer given the chance to enter a quantity against each option you list here."}
{g->text text="For example, if this item was a 30cm by 50cm picture frame, the options you list here might be the colours in which you stock it."}
</p>
 <table class="gbDataTable">
    <tr>
    <th>{g->text text="Option"}</th>
    <th>{g->text text="Display Order"}</th>
    <th>{g->text text="Price"}</th>
    <th>{g->text text="Postage Band"}</th>
    </tr>
  {foreach from=$form.accessories item=accessory name=x }
    <tr>
      <td><input type="hidden" name="{g->formVar var="form[accessories][`$accessory.id`][isAccessory]"}" value="1">
      <input type="text" name="{g->formVar var="form[accessories][`$accessory.id`][name]"}" value="{$accessory.name|default:''}" size = "60"></td>
      <td><input type="text" name="{g->formVar var="form[accessories][`$accessory.id`][position]"}" value="{$smarty.foreach.x.iteration}"  size = "3"></td>
      <td>{$csymbol}<input type="text" name="{g->formVar var="form[accessories][`$accessory.id`][price]"}" value="{$accessory.price|default:''}"size = "6" style="text-align:right;"></td>
      <td align="center"><select name = "{g->formVar var="form[accessories][`$accessory.id`][postageBand]"}">
	   <option label="None" value="0" {if $accessory.postageBand == '0'}selected{/if}>{g->text text="None"}</option>
	   {section name=j loop=$numPostageBands}
	   <option value="{$smarty.section.j.iteration}" {if $accessory.postageBand == $smarty.section.j.iteration}selected{/if}>{$smarty.section.j.iteration}</option>
	   {/section}
	 </select></td>
    </tr>
  {/foreach}
  </table>
<p class="giDescription"> 
{g->text text="The display priority determines in which order the possibly many accessory items in your gallery are offered on the checkout page. Smaller values appear higher up the page."}
</p>
{g->text text="Display priority for this item:"} <input type="text" name="{g->formVar var="form[position]"}" value="{$form.position|default:''}" size = "3">
</div>
{/if}


{foreach from=$customPlugins item=plugin}
{include file="gallery:`$plugin.customPageTemplate`" l10Domain=$ItemEdit.pluginL10Domain}
{/foreach}
    
    
<div class="gbBlock gcBackground1">
    <input type="submit" name="{g->formVar var="form[action][save]"}"
     value="{g->text text="Save Settings"}"/>
    <input type="submit" name="{g->formVar var="form[action][cancel]"}"
     value="{g->text text="Cancel"}"/>
</div>


