
<div class="gbBlock">
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Price Bands"} </h2>
</div>

 <p class="giDescription">
    {g->text text="Unit price for quantities from one e.g. '8,6,4' means 8 for the first, 6 each for two, or 4 each for three or more. The description field will appear instead of the single price. For complicated pricing structures you can put 'table A' in the description and include the detail by putting it in the relevant templates."}
 </p>
 <p class="giDescription">
    {g->text text="Hint: You may wish to turn on html or bbcode embedded markup and then begin your price description either with a space or with e.g. &lt;br&gt; for display purposes."}
 </p>
 
 <table class="gbDataTable">
    <tr>
      <th>{g->text text="Product Name"}</th>
      <th>{g->text text="Price each for 1, 2, 3 etc. items"}</th>
      <th>{g->text text="Description of Pricing"}</th>
    </tr>
          	{foreach from=$form.products item=product }
    <tr>
      <td>{$product.rootName}</td>
      <td>{$csymbol}<input type="text" name="{g->formVar var="form[products][`$product.id`][priceBandData]"}" size="20" value="{if isset($product.priceBandData)}{$product.priceBandData}{/if}"></td>
      <td><input type="text" name="{g->formVar var="form[products][`$product.id`][priceBandDescription]"}" size="60" value="{if isset($product.priceBandDescription)}{$product.priceBandDescription}{/if}"></td>
    </tr>

    		{/foreach}
  </table>
</div>


