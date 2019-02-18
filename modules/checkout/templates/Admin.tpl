{*
 * $Revision: 1718 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
 <div class="gbBlock gcBackground1">
  <h2> {g->text text="Checkout eCommerce Settings"} </h2>
</div>

{if !empty($status)}
<div class="gbBlock"><h2 class="giSuccess">
{if isset($status.saved)}
{g->text text="Settings saved successfully"}
{/if}
</h2></div>
{/if}
  
<div class = "gbBlock"> 
 <h3>{g->text text="Business Information"}</h3>
       <p class="giDescription">
      {g->text text="Some basic details about your business, including its name, and the symbol of the currency in which you wish to price items."}
      </p>
      <p class="giDescription">
      {g->text text="You must also configure a currency formatting string (in sprintf format) such as %.2f for two decimal places or %d for integer. When entering prices use a decimal point - . - not a comma - , . The locale settings applicable when the page is viewed will determine which the user sees."}
      

   <table class="gbDataTable">
    <tr>
      <td> {g->text text="Business Name"} </td>
      <td><input type="text" name="{g->formVar var="form[busname]"}" value="{$form.busname|default:''}"/></td>
    </tr>
    <tr>
      <td>{g->text text="Currency symbol to display in cart"} </td>
      <td><input type="text" name="{g->formVar var="form[csymbol]"}" value="{$form.csymbol|default:''}"/></td>
    </tr>
     <tr>
      <td>{g->text text="Currency formatting string"} </td>
      <td><input type="text" name="{g->formVar var="form[cformat]"}" value="{$form.cformat|default:'%.2f'}"/></td>
    </tr>
   </table>
</div>

<div class="gbBlock">
  <h3> {g->text text="Products and Prices"} </h3>
      <p class="giDescription">
      {g->text text="Enter details of the products you wish to sell. For instance '4x6 print - borderless', '8 x 10', 'printed cushion cover (20 inch)'."}
      {g->text text="Customers will be offered these products for the different images in your gallery."}
      </p>
        <table class="gbDataTable">
      	<tr class="{cycle values="gbOdd,gbEven" advance=false}">
      	<th align="center">{g->text text="Delete"}</th>
        <th align="center">{g->text text="Product Name"}</th>
        <th align="center">{g->text text="Price"}</th>
        <th align="center">{g->text text="Postage Band"}</th>
        <th align="center">{g->text text="Display Position"}</th>
        <th align="center">{g->text text="Visible"}</th>
        <th align="center">{g->text text="Applies to Album Items"}</th>
        <th align="center">{g->text text="Applies to Other Items"}</th>
        <th align="center">{g->text text="Option Set"}</th>
        <th align="center">{g->text text="Initial Quantity"}</th>
        <th align="center">{g->text text="Quantity Limit"}</th>
        <th align="center">{g->text text="Show in Dropdown"}</th>
        </tr>
        <tr class="{cycle}">
        <th>&nbsp</th>
        <th colspan="11">{g->text text="Description"}</th>
        </tr>
	  
          	{foreach from=$form.products item=product }
	<tr class="{cycle advance=false}" >
	<td align="center">{if $product.id > 0}<input type="checkbox" name="{g->formVar var="form[products][`$product.id`][delete]"}" value = "true">{/if}</td>
	<td align="center"><input type="text" name="{g->formVar var="form[products][`$product.id`][name]"}" value="{$product.name}"></td>
	<td align="center">{$form.csymbol}<input type="text" name="{g->formVar var="form[products][`$product.id`][price]"}" value="{$product.price}" size = "6" style="text-align:right;"></td>
	<td align="center"><select name = "{g->formVar var="form[products][`$product.id`][postageBand]"}">
	   <option label="None" value="-1" {if $product.postageBand == '-1'}selected{/if}>{g->text text="None"}</option>
	   {section name=j loop=$form.numPostageBands}
	   <option value="{$smarty.section.j.iteration}" {if $product.postageBand == $smarty.section.j.iteration}selected{/if}>{$smarty.section.j.iteration}</option>
	   {/section}
	 </select></td>
	<td align="center"><input type="text" name="{g->formVar var="form[products][`$product.id`][position]"}" value="{$product.position}"  size = "1" style="text-align:center;"></td>
	<td align="center"><input type="checkbox" name = "{g->formVar var="form[products][`$product.id`][visible]"}" value = "true" {if $product.visible == "true" }checked{/if}></td>
	<td align="center"><input type="checkbox" name = "{g->formVar var="form[products][`$product.id`][appliesToAlbum]"}" value = "true" {if $product.appliesToAlbum == "true" }checked{/if}></td>	
	<td align="center"><input type="checkbox" name = "{g->formVar var="form[products][`$product.id`][appliesToItem]"}" value = "true" {if $product.appliesToItem == "true" }checked{/if}></td>
	<td align="center"><select name = "{g->formVar var="form[products][`$product.id`][paperSet]"}">
	   <option label="None" value="0" {if $product.paperSet == '0'}selected{/if}>{g->text text="None"}</option>
	   {section name=j loop=$form.numPaperSets}
	   <option value="{$smarty.section.j.iteration}" {if $product.paperSet == $smarty.section.j.iteration}selected{/if}>{$smarty.section.j.iteration}</option>
	   {/section}
	 </select></td>
	<td align="center"><input type="text" name="{g->formVar var="form[products][`$product.id`][initialQuantity]"}" value="{$product.initialQuantity}"  size = "1" style="text-align:center;"></td>
	<td align="center"><input type="text" name="{g->formVar var="form[products][`$product.id`][quantityLimit]"}" value="{$product.quantityLimit}"  size = "1" style="text-align:center;"></td>
	<td align="center"><input type="checkbox" name = "{g->formVar var="form[products][`$product.id`][dropdown]"}" value = "true" {if $product.dropdown == "true"}checked{/if}></td>
	</tr>
	<tr class="{cycle}">
	<td>&nbsp;</td>
	<td colspan="11"><input type="text" name="{g->formVar var="form[products][`$product.id`][description]"}" value="{if isset($product.description)}{$product.description}{/if}"  size = "100"></td>
	</tr>
  		{/foreach}	
   	</table>
</div>

<div class="gbBlock">
       <h3>{g->text text="Per-album / per-photo ('custom') settings"}</h3>
       <p class="giDescription">
        {g->text text="Prices and visibility for the products above can be overridden at a local level for each album or photograph (via the Checkout tab on its 'edit' page) if you enable it here."}
       </p>
       <table class="gbDataTable">
       <tr>
         <td width="50%">{g->text text="Activate Custom Settings?"}</td>
         <td><input type="checkbox" name="{g->formVar var="form[customPrices]"}"{if $form.customPrices} checked="checked"{/if}/></td>
      </tr>
      </table>
</div>

<div class="gbBlock">
 <h3>{g->text text="Option Sets"}</h3>
       <p class="giDescription">
        {g->text text="For products with a range of options (eg. matt paper or gloss paper) create multiple sets of options here - then assign one set to each product in the table above."}
        </p>
      <table class="gbDataTable">
      <tr>
         <td width="50%">{g->text text="Number of Option Sets?"}</td>
         <td><input type="text" name="{g->formVar var="form[numPaperSets]"}" value="{$form.numPaperSets}" size="4"></td>
      </tr>
      </table>
      <table class="gbDataTable">
	<tr><th></th>
       	{section name=j loop=$form.numPaperSets}
       	   <th>Set {$smarty.section.j.iteration}</th>
       	{/section}
       	</tr>
	{foreach name=i from=$form.papers item=paper key=i}
	<tr>
	<td>{if $smarty.foreach.i.iteration==1}{g->text text="Default"}{else}{g->text text="Option %d" arg1=$smarty.foreach.i.iteration}{/if}</td>
	{section name=j loop=$form.numPaperSets}
	<td><input type="text" name="{g->formVar var="form[papers][`$i`][`$smarty.section.j.iteration`]"}" value="{$paper[$smarty.section.j.iteration]|default:''}"/></td>
	{/section}
  	</tr>
      	{/foreach}
  	</table>
</div>

<div class="gbBlock">
      <h3>{g->text text="Postage options"}</h3>
 	<p class="giDescription">
        {g->text text="Delivery prices can be set for different bands of products."}
        {g->text text="The band used for any order is that of the highest-banded product in the order."}
        </p>
      <table class="gbDataTable">
      <tr>
         <td width="50%">{g->text text="Number of Postage Bands?"}</td>
         <td><input type="text" name="{g->formVar var="form[numPostageBands]"}" value="{$form.numPostageBands}" size="4"></td>
      </tr>
      </table>
       <p class="giDescription">
        {g->text text="Postage rates by service and band. Different services might be for example 'letter post' and 'special delivery'."}
        </p>
       <table class="gbDataTable">
       		<tr><th>Description</th>
       		{section name=j loop=$form.numPostageBands}
       		    <th>Band {$smarty.section.j.iteration}</th>
       		{/section}
       		</tr>
  		{foreach from=$form.postages item=postage key=i}
		<tr>
  		<td><input type="text" name="{g->formVar var="form[postages][`$i`][type]"}" value="{$postage.type|default:''}" size = "30"  /></td>
  		{section name=j loop=$form.numPostageBands}
       		    <td>{$form.csymbol}<input type="text" name="{g->formVar var="form[postages][`$i`][price][`$smarty.section.j.iteration`]"}" value="{$postage.price[$smarty.section.j.iteration]|default:''}" style="text-align:right;"  size = "3" /></td>
  		{/section}
  		</tr>
      		{/foreach}
  	</table>
</div>

<div class="gbBlock">
  <h3> {g->text text="Discounts"} </h3>
      <p class="giDescription">
      {g->text text="Enter details of discounts to be offered to customers."}
      {g->text text="Each discount marked as default (and optionally, a discount for which the customer enters a matching code) are checked against the order for the criteria listed - in priority order. The first such discount that matches is applied against the order."}
      </p>
        <table class="gbDataTable">
         <tr>
        <th colspan="2">&nbsp;</th>
      	<th align="center" colspan="2">{g->text text="Applies"}</th>
      	<th align="center" colspan="5">{g->text text="Criteria"}</th>
      	<th align="center" colspan="2">{g->text text="Discount"}</th>
        
        </tr>
      	<tr>
      	<th align="center">{g->text text="Delete"}</th>
      	{*<th align="center">{g->text text="id"}</th>*}
      	 <th align="center">{g->text text="Description"}</th>
      	<th align="center">{g->text text="Priority"}</th>
      	<th align="center">{g->text text="Default"}</th>
        <th align="center">{g->text text="Entry Code"}</th>
        <th align="center">{g->text text="Minimum Value"}</th>
        <th align="center">{g->text text="Min. Images"}</th>
        <th align="center">{g->text text="Min. Products"}</th>
        <th align="center">{g->text text="Group"}</th>
        <th align="center">{g->text text="Value"}</th>
        <th align="center">{g->text text="Comp. Postage"}</th>
        
        </tr>
        	
          	{foreach from=$form.discounts item=discount }
	<tr class="{cycle values="gbOdd,gbEven"}">
	<td align="center">{if $discount.discountId > 0}<input type="checkbox" name="{g->formVar var="form[discounts][`$discount.discountId`][delete]"}" value = "true">{/if}</td>
	{*<td align="center">{$discount.discountId}</td>*}
	<td align="center"><input type="text" name="{g->formVar var="form[discounts][`$discount.discountId`][description]"}" value="{$discount.description}" size = "40"></td>
	<td align="center"><input type="text" name="{g->formVar var="form[discounts][`$discount.discountId`][priority]"}" value="{$discount.priority}"  size = "1" style="text-align:center;"></td>
	<td align="center"><input type="checkbox" name = "{g->formVar var="form[discounts][`$discount.discountId`][applyDefault]"}" value = "true" {if $discount.applyDefault == "true" }checked{/if}></td>
	<td align="center"><input type="text" name="{g->formVar var="form[discounts][`$discount.discountId`][entryCode]"}" value="{$discount.entryCode}" size = "6"></td>
	<td align="center">{$form.csymbol}<input type="text" name="{g->formVar var="form[discounts][`$discount.discountId`][minValue]"}" value="{$discount.minValue}" size = "4" style="text-align:right;"></td>
	<td align="center"><input type="text" name="{g->formVar var="form[discounts][`$discount.discountId`][minItems]"}" value="{$discount.minItems}" size = "2" style="text-align:right;"></td>
	<td align="center"><input type="text" name="{g->formVar var="form[discounts][`$discount.discountId`][minProducts]"}" value="{$discount.minProducts}" size = "2" style="text-align:right;"></td>
	<td align="center"><select name="{g->formVar var="form[discounts][`$discount.discountId`][groupId]"}">{html_options options=$groupList selected=$discount.groupId}</select></td>
	<td align="center"><input type="text" name="{g->formVar var="form[discounts][`$discount.discountId`][discountPercent]"}" value="{$discount.discountPercent}" size = "3" style="text-align:right;">% + <br>
	 {$form.csymbol}<input type="text" name="{g->formVar var="form[discounts][`$discount.discountId`][discountFixed]"}" value="{$discount.discountFixed}" size = "6" style="text-align:right;"></td>
	<td align="center"><input type="checkbox" name = "{g->formVar var="form[discounts][`$discount.discountId`][freePostage]"}" value = "true" {if $discount.freePostage == "true" }checked{/if}></td>
	</tr>
  		{/foreach}
   	  </table>
</div>

<div class="gbBlock">
       <h3>{g->text text="Discount (coupon) codes"}</h3>
       <p class="giDescription">
        {g->text text="The customer can be given the opportunity to enter a special discount code, if you wish. If it matches a Code in the discounts table above, that discount will be assessed against the order and may be applied."}
       </p>
       <table class="gbDataTable">
       <tr>
         <td width="50%">{g->text text="Ask for discount code?"}</td>
         <td><input type="checkbox" name="{g->formVar var="form[askDiscountCode]"}"{if $form.askDiscountCode}checked="checked"{/if}/></td>
      </tr>
      </table>
</div>
<div class="gbBlock">
      <h3>{g->text text="Minimum Order Value"}</h3>
      <p class="giDescription">
        {g->text text="You have the option to reject any orders unless they meet a specified minimum order value (excluding postage costs)."}
        {g->text text="If you're happy to accept orders of any value leave this setting blank."}
      </p>
       <table class="gbDataTable">
      <tr>
         <td width="50%">{g->text text="Minimum Order Value?"}</td>
         <td>{$form.csymbol}<input type="text" name="{g->formVar var="form[minOrderValue]"}" value="{$form.minOrderValue|default:''}" size="4" style="text-align:right;"></td>
      </tr>
      </table>
</div>
<div class="gbBlock">
      <h3>{g->text text="Cart Display Options"}</h3>
      <p class="giDescription">
        {g->text text="When a user adds an item to their cart you can have them redirected to show the cart contents, or have them stay at the same page."}
      </p>
       <table class="gbDataTable">
      <tr>
      <td width="50%">{g->text text="Send user to cart when adding items?"}</td>
      <td>
      <input type="checkbox" name="{g->formVar var="form[cartredirect]"}"{if $form.cartredirect} checked="checked"{/if}/>
      </td>
      <tr>
       </table>
       <p class="giDescription">
        {g->text text="You may require purchasers to view and accept a page of terms and conditions before they may complete their order."}
      </p>
       <table class="gbDataTable">
      <tr>
      <td width="50%">{g->text text="Insist user accepts Terms and Conditions?"}</td>
      <td><input type="checkbox" name="{g->formVar var="form[requireTandCs]"}"{if $form.requireTandCs} checked="checked"{/if}/></td>
      </tr>
      <tr>
      <td width="50%">{g->text text="T&amp;C's page link"}</td>
      <td><input type="text" name="{g->formVar var="form[TandCLink]"}" value="{$form.TandCLink|default:''}" size="30"/></td>
      </tr>
      </table>
       <p class="giDescription">
        {g->text text="You can display a paragraph of text with information about your services, products or other details on the cart page by including the text here."}
      </p>
       <table class="gbDataTable">
      <tr>
      <td>  <textarea name="{g->formVar var="form[carttext]"}" cols="70" rows="5">{$form.carttext|default:''}</textarea></td>
      </tr>
      </table>
</div>
<div class="gbBlock">
 	<h3>{g->text text="Other Options"}</h3>
      <p class="giDescription">
        {g->text text="Checkout handles albums in either of two ways. Either as an individual items in their own right or alternatively as a collection of photos. If you treat albums as collections of images then adding an album to the cart adds each contained item instead."}
        {g->text text="Only products marked above as 'apply to Albums' will be offered for album items, and only products marked as 'Apply to Items' will be offered for non-album items such as photos."}
      </p>
       <table class="gbDataTable">
   	<tr>
   	<td width="50%">{g->text text="Treat Albums as collections of contained images?"}</td>
   	<td><input type="checkbox" name="{g->formVar var="form[treatAlbumAsCollection]"}"{if $form.treatAlbumAsCollection} checked="checked"{/if}/></td>
      </tr>
      </table>
      <p class="giDescription">
        {g->text text="You can restrict the display of the Add to Cart option to the main item for the page (album/photo) or have it shown everywhere, for instance under thumbnails too."}
        </p>
       <table class="gbDataTable">
   	<tr>
   	<td width="50%">{g->text text="Show 'Add to Cart' always?"}</td>
   	<td><input type="checkbox" name="{g->formVar var="form[alwaysShowAddToCart]"}"{if $form.alwaysShowAddToCart} checked="checked"{/if}/></td>
      </tr>
      </table>
</div>
<div class="gbBlock">
	<h3>{g->text text="Database Cleaning Options"}</h3>       
 	<p class="giDescription">
        {g->text text="Checkout creates a new transaction record every time the 'Continue to Checkout' button is used, even if the transaction is never finalised. These 'orphan' transaction records will be deleted after a period you can select here."}
        {g->text text="You can also set the time period after which other (completed and uncompleted) orders are purged from the database."}
       </p>
       	<p class="giDescription">
        {g->text text="For either case, to keep the records indefinitely leave the box blank."}
       </p>
        <table class="gbDataTable">
       <tr>
         <td width="50%">{g->text text="Remove orphan transactions older than (days):"}</td>
         <td><input type="text" name="{g->formVar var="form[deleteOrphanDays]"}" size="4" value="{$form.deleteOrphanDays|default:''}"/></td>
      </tr>
      <tr>
         <td width="50%">{g->text text="Remove all other transactions older than (days):"}</td>
         <td><input type="text" name="{g->formVar var="form[deleteAllDays]"}" size="4" value="{$form.deleteAllDays|default:''}"/></td>
      </tr>
      </table>
</div>
<div class="gbBlock">
	<h3>{g->text text="Email Notification Options"}</h3>    
      <p class="giDescription">
        {g->text text="List the email addresses to which you want order status notifications to be sent."}
        {g->text text="Also specify the content for each recipient. Admin messages are notices from payment plugins intended for the site administrator, and may not be suitable for receipt by customers."}
       </p>
       <p class="giDescription">
        {g->text text="If your server setup can translate to different languages, then you can select the language in which emails are sent. The 'customer's language' setting uses the language supplied to their browser at the time the order was confirmed."}
       </p>
	  <table class="gbDataTable">
	    <tr>
	  <th>{g->text text="Email Address"}</th>
	  <th align="center">{g->text text="Customer information"}</th>
	  <th align="center">{g->text text="Order Contents..."}</th>
	  <th align="center">{g->text text="... with thumbnails..."}</th>
	  <th align="center">{g->text text="... and hotlinks"}</th>
	  <th align="center">{g->text text="Admin messages"}</th>
	 {if $canTranslate} <th align="center">{g->text text="Language"}</th> {/if}
	</tr>
        {foreach from=$form.emails item=email key=i}
	  <tr>
	    <td>{if $i == 0}
	    	<input type="checkbox" name="{g->formVar var="form[emails][0][toAddress]"}" value="1" {if $email.toAddress}checked{/if}> {g->text text=" Use the Customer's Provided Email Address"}
	    	{else}
	    	<input type="text" name="{g->formVar var="form[emails][`$i`][toAddress]"}" value="{$email.toAddress|default:''}" size="50">
	    	{/if}</td>
	    <td align="center"><input type="checkbox" name="{g->formVar var="form[emails][`$i`][showCustomer]"}"{if $email.showCustomer} checked{/if} value="1"></td>
	    <td align="center"><input type="checkbox" name="{g->formVar var="form[emails][`$i`][showContents]"}"{if $email.showContents} checked{/if} value="1"></td>
	    <td align="center"><input type="checkbox" name="{g->formVar var="form[emails][`$i`][showThumbs]"}"{if $email.showThumbs} checked{/if} value="1"></td>
	    <td align="center"><input type="checkbox" name="{g->formVar var="form[emails][`$i`][showLinks]"}"{if $email.showLinks} checked{/if} value="1"></td>
	    <td align="center"><input type="checkbox" name="{g->formVar var="form[emails][`$i`][isAdmin]"}"{if $email.isAdmin} checked{/if} value="1"></td>
	    {if $canTranslate}<td>
	        <select name="{g->formVar var="form[emails][`$i`][emailLanguage]"}">
	        <option label="Customer's Language" value="transaction_language" {if $email.emailLanguage == "transaction_language"}selected{/if}>{g->text text="Customer's Language"}</option>
	        {html_options options=$languageList selected=$email.emailLanguage}</select>
	    	</td>{/if}
	    <td> </td>
	  </tr>
	{/foreach}
      </table>
  
      <p class="giDescription">
         {g->text text="Email address to send order status notifications from"}
      </p>
      <table class=gbDataTable">
      <tr>
      <td><input type="text" name="{g->formVar var="form[orderStatusFromAddress]"}" value="{$form.orderStatusFromAddress|default:''}" size="50"></td>
      </tr>
      </table>
      <p class="giDescription">
        {g->text text="Subject line to prefix to notification emails"}
        </p>
      <table class=gbDataTable">
      <tr>
      <td><input type="text" name="{g->formVar var="form[orderStatusSubject]"}" value="{$form.orderStatusSubject|default:''}" size="50"></td>
      </tr>
      </table>
</div>
      
<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][save]"}"
     value="{g->text text="Save Settings"}"/>
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][cancel]"}"
     value="{g->text text="Cancel"}"/>
  </div>
</div>
<br/>
<div class="checkoutCredits">
<b>Credits</b> : Original code by Matthew Turnbull &amp; Daniel Leaberry. 'Checkout+' (version 0.0.15 and up) coded by Jay Hennessy. Version 0.2.0 and up by Alec Myers.<br>
</div>
