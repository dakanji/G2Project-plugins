{*
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="checkout.transactionCustomerDetails" transactionId=$form.transactionId}

<div class="gbBlock  gcBackground1">
  <h2 class="gbTitle">
    {g->text text="Checkout"}::{g->text text="Order Administration"}
  </h2>
</div>


  
<div class = "gbBlock"> 
<table class="gbDataTable">
   <tr class = "gbOdd">
   <td align=right><strong>{g->text text="Order Number"}</strong></td><td>{$form.transactionId}</td>
   </tr>
   <tr class = "gbEven">
   <td align=right><strong>{g->text text="Date and Time"}</strong></td><td>{g->date timestamp=$block.checkout.date style = "datetime"}</td>
   </tr>
</table>
</div>

<div class = "gbBlock gcBackground1" > 
<h3>{g->text text="Order Processing"}</h3>
 {if !empty($status)}
  <div id="gsStatus">
  <h2 class="giSuccess">
    {if isset($status.cleared)}
      {g->text text="Order Moved to Cleared Status"}
    {/if}
    {if isset($status.dispatched)}
      {g->text text="Order Moved to Dispatched Status"}
    {/if}
    {if isset($status.reconfirmed)}
      {g->text text="A duplicate order confirmation email has been sent."}
    {/if}
    {if isset($status.notified)}
      {g->text text="An administrative notification has been sent."}
    {/if}
   </h2>
  </div>
  <br/>
  {/if}
<input type="hidden" name="{g->formVar var="form[transactionId]"}" value ="{$form.transactionId}">
<input type="hidden" name="{g->formVar var="form[formName]"}" value ="AdminOrder" >
<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][reconfirm]"}" value="{g->text text="Re-confirm Order"}"/>
<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][clear]"}" value="{g->text text="Payment Cleared"}"/>
<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][complete]"}" value="{g->text text="Order Completed"}"/>
<input type="submit" class="inputTypeSubmit"name="{g->formVar var="form[action][admin]"}" value="{g->text text="Send Admin Notification"}"/>
<p>{g->text text="Any text entered here will be included with the notification emails."}</p>
<textarea name="{g->formVar var="form[messageText]"}" cols=70 rows=5>{$form.messageText}</textarea>
</div>

<div class = "gbBlock"> 
<h3>{g->text text="Customer Details"}</h3>

{if isset($status.edit)}
<table class="gbDataTable">
   <tr class = "gbOdd">
     <td align=right><strong>{g->text text="First Name"}</strong></td>
     <td><input type="text" name="{g->formVar var="form[custDetails][custFirstName]"}" size="50" value="{$block.checkout.firstName}"</input></td>
   </tr>
   <tr class = "gbOdd">
     <td align=right><strong>{g->text text="Last Name"}</strong></td>
     <td><input type="text" name="{g->formVar var="form[custDetails][custLastName]"}" size="50" value="{$block.checkout.lastName}"</input></td>
   </tr>
   <tr class = "gbEven">
     <td align=right><strong>{g->text text="Recipient Name"}</strong></td>
     <td><input type="text" name="{g->formVar var="form[custDetails][custRecipientName]"}" size="50" value="{$block.checkout.recipientName}"</input></td>
   </tr>
   <tr class = "gbOdd">
     <td align=right><strong>{g->text text="Telephone"}</strong></td>
     <td><input type="text" name="{g->formVar var="form[custDetails][telephone]"}" size="50" value="{$block.checkout.telephone}"</input></td>
   </tr>
   <tr class = "gbEven">
   <td align=right><strong>{g->text text="Address"}</strong></td>
     <td><textarea cols=50 rows=5 name="{g->formVar var="form[custDetails][address]"}">
   {$block.checkout.address1}
   {$block.checkout.address2}
   {$block.checkout.address3}
   {$block.checkout.address4}
   {$block.checkout.address5}
   </textarea>
   </td>
   </tr> 
   <tr class = "gbOdd"> 
     <td align=right><strong>{g->text text="Postal Code"}</strong></td>
     <td><input type="text" name="{g->formVar var="form[custDetails][custZip]"}" size="50" value="{$block.checkout.addressZip}"</input></td>
   </tr>
   <tr class = "gbEven">
     <td align=right><strong>{g->text text="Country"}</strong></td>
     <td><input type="text" name="{g->formVar var="form[custDetails][custCountry]"}" size="50" value="{$block.checkout.country}"</input></td>
   </tr>
   <tr class = "gbOdd">
     <td align=right><strong>{g->text text="Email"}</strong></td>
     <td><input type="text" name="{g->formVar var="form[custDetails][custEmail]"}" size="50" value="{$block.checkout.email}"</input></td>
   </tr>
   <tr class = "gbEven">
     <td align=right><strong>{g->text text="Gallery Buyer Id"}</strong></td>
     <td>{$block.checkout.buyerId}</a></td>
   </tr>
   </tr>
   <tr class = "gbOdd">
     <td align=right><strong>{g->text text="Payment Plugin"}</strong></td>
     <td>{$block.checkout.paymentPlugin}</a></td>
   </tr>
</table>
<input type="submit" class="inputTypeSubmit"name="{g->formVar var="form[action][saveAddress]"}" value="{g->text text="Save Changes"}"/>
<input type="submit" class="inputTypeSubmit"name="{g->formVar var="form[action][discardAddress]"}" value="{g->text text="Discard Changes"}"/>
&nbsp;&nbsp;&nbsp;&nbsp;
<input type="submit" class="inputTypeSubmit"name="{g->formVar var="form[action][deleteOrder]"}" value="{g->text text="Delete Order"}"/>
{else}
{if !empty($status)}
  <div id="gsStatus">
  <h2 class="giSuccess">
    {if isset($status.addressSaved)}
      {g->text text="Changes Saved."}
    {/if}
   </h2>
  </div>
  <br/>
{/if}
<table class="gbDataTable">
   <tr class = "gbOdd">
   <td align=right><strong>{g->text text="Name"}</strong></td><td>{$block.checkout.name}</td>
   </tr>
   <tr class = "gbEven">
   <td align=right><strong>{g->text text="Recipient Name"}</strong></td><td>{$block.checkout.recipientName}</td>
   </tr>
   <tr class = "gbOdd">
   <td align=right><strong>{g->text text="Telephone"}</strong></td><td>{$block.checkout.telephone}</td>
   </tr>
   <tr class = "gbEven">
   <td align=right><strong>{g->text text="Address"}</strong></td>
   <td>{$block.checkout.address1}<br>
   {$block.checkout.address2}<br>
   {$block.checkout.address3}<br>
   {$block.checkout.address4}<br>
   {$block.checkout.address5}<br>
   </td>
   </tr> 
   <tr class = "gbOdd"> 
   <td align=right><strong>{g->text text="Postal Code"}</strong></td><td>   {$block.checkout.addressZip}</td>
   </tr>
   <tr class = "gbEven">
   <td align=right><strong>{g->text text="Country"}</strong></td><td>{$block.checkout.country}</td>
   </tr>
   <tr class = "gbOdd">
   <td align=right><strong>{g->text text="Email"}</strong></td><td>
   <a href="mailto:{$block.checkout.email}?subject={$block.checkout.orderStatusSubject}{g->text text="Order No."} {$form.transactionId}">
   {$block.checkout.email}</a></td>
   </tr>
   <tr class = "gbEven">
   <td align=right><strong>{g->text text="Gallery Buyer Id"}</strong></td><td>{$block.checkout.buyerId}</a></td>
   </tr>
   </tr>
   <tr class = "gbOdd">
   <td align=right><strong>{g->text text="Payment Plugin"}</strong></td><td>{$block.checkout.paymentPlugin}</a></td>
   </tr>
</table>
<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][edit]"}" value="{g->text text="Edit"}"/>
{/if}
</div>

{*include status information*}
{include file='gallery:modules/checkout/templates/StatusInfo.tpl'}

{*plugins*}
{foreach from=$statusPlugins item=plugin}
  {include file="gallery:`$plugin.statusPageTemplate`" l10Domain=$plugin.l10Domain}
{/foreach}
 {if isset($status.editNotes)}
<div class = "gbBlock"> 
 <h3>{g->text text="Customer Notes"}</h3>
 <textarea name="{g->formVar var="form[notes][custNotes]"}" cols=70 rows=5>{$block.checkout.custNotes}</textarea><br/>
  <h3>{g->text text="Customer Comments"}</h3>
 <textarea name="{g->formVar var="form[notes][custComments]"}" cols=70 rows=5>{$block.checkout.custComments}</textarea><br/>
 <input type="submit" class="inputTypeSubmit"name="{g->formVar var="form[action][saveNotes]"}" value="{g->text text="Save Changes"}"/>
 <input type="submit" class="inputTypeSubmit"name="{g->formVar var="form[action][discardNotes]"}" value="{g->text text="Discard Changes"}"/>
 </div>
 {else}
 <div class = "gbBlock"> 
 <h3>{g->text text="Customer Comments and Notes"}</h3>
 {if !empty($status)}
  <div id="gsStatus">
  <h2 class="giSuccess">
    {if isset($status.notesSaved)}
      {g->text text="Changes Saved."}
    {/if}
   </h2>
  </div>
  <br/>
{/if}
{$block.checkout.custNotes}<br>
{$block.checkout.custComments}<br>
<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][editNotes]"}" value="{g->text text="Edit"}"/>
</div>
{/if}


{* include cart contents *}
{include file='gallery:modules/checkout/templates/TransactionContents.tpl'}

</div>