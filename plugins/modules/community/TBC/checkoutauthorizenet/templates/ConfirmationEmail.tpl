--{$boundary}
Content-Type: text/plain; charset=utf-8
Content-Transfer-Encoding: 8bit

{g->text text="Contact Information"}
 {g->text text="Customer"}: {$custFirstName} {$custLastName}
    {g->text text="Email"}: <{$custEmail}>
    {g->text text="Phone"}: {$custPhone}
{g->text text="Billing Information"}
  {g->text text="Address"}: {$custAddress}
     {g->text text="City"}: {$custCity}
    {g->text text="State"}: {$custState}
      {g->text text="Zip"}: {$custCcZip}
  {g->text text="Country"}: {$custCcCountry}

{g->text text="Shipping Information (if different)"}
{g->text text="Recipient"}: {$custRecipientName}
  {g->text text="Address"}: {$custAddress1}
           {$custAddress2}
           {$custAddress3}
           {$custAddress4}
           {$custAddress5}
      {g->text text="Zip"}: {$custZip}
  {g->text text="Country"}: {$custCountry}
{*
{g->text text="Routing Number (ABA code)"}: {$custAbaCode}
{g->text text="Account Number"}: {$custAccountNumber}
*}
 {g->text text="Comments"}: {$custComments}

{foreach from=$items item=item}
=========================
<{g->url arg1="view=core.ShowItem" arg2="itemId=`$item.id`" forceFullUrl=true}>
   {g->text text="Title"}: {$item.title|markup}
     {g->text text="Ref"}: {$item.pathComponent}
 {g->text text="Summary"}: {$item.summary|markup}
{section name=x loop=$product}
{if isset($item.quant[x]) }
     {g->text text="Qty"}: {$item.quant[x]} {g->text text="of"} {$product[x]} [{$csymbol}{if isset($item.itemPrices[x])}{$item.itemPrices[x]}{else}{$price[x]}{/if} {g->text text="each"}]
{/if}
{/section}
{if $item.paper != ""}
   {g->text text="Paper"}: {$item.paper}
{/if}
{g->text text="Subtotal"}: {$csymbol}{$item.subtotal}
{/foreach}
=========================

{g->text text="Postage and Packing"}: {$csymbol}{$postage}
              {g->text text="Total"}: {$csymbol}{$total}

--{$boundary}
Content-Type: text/html; charset=utf-8
Content-Transfer-Encoding: 8bit

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
	<title>Order information</title>
  </head>
  
<body>
<p>The following order has been submitted:</p>

<table cellspacing="5">
  <tr>
    <th colspan="2">
      {g->text text="Contact Information"}
    </th>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="First Name"}:</strong></td>
    <td>{$custFirstName}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Last Name"}:</strong></td>
    <td>{$custLastName}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Email Address"}:</strong></td>
    <td>{$custEmail}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Phone"}:</strong></td>
    <td>{$custPhone}</td>
  </tr>
  <tr>
    <th colspan="2">
      {g->text text="Billing Information"}
    </th>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Address"}:</strong></td>
    <td>{$custAddress}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="City"}:</strong></td>
    <td>{$custCity}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="State"}:</strong></td>
    <td>{$custState}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="ZIP Code"}:</strong></td>
    <td>{$custCcZip}</td>
  </tr>
  <tr>
    <th colspan="2">
      {g->text text="Shipping Information"}
    </th>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Recipient's Name (if different)"}:</strong></td>
    <td>{$custRecipientName}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Postal Address"}:</strong></td>
    <td>{$custAddress1}</td>
  </tr>
  <tr>
    <td align="right">&nbsp;</td>
    <td>{$custAddress2}</td>
  </tr>
  <tr>
    <td align="right">&nbsp;</td>
    <td>{$custAddress3}</td>
  </tr>
  <tr>
    <td align="right">&nbsp;</td>
    <td>{$custAddress4}</td>
  </tr>
  <tr>
    <td align="right">&nbsp;</td>
    <td>{$custAddress5}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Postal/Zip Code"}:</strong></td>
    <td>{$custZip}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Country"}:</strong></td>
    <td>{$custCountry}</td>
  </tr>
  {*
  <tr>
    <td align="right"><strong>{g->text text="Routing Number (ABA code)"}:</strong></td>
    <td>{$custAbaCode}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Account Number"}:</strong></td>
    <td>{$custAccountNumber}</td>
  </tr>
  *}
  <tr>
    <td align="right"><strong>{g->text text="Comments"}:</strong></td>
    <td>{$custComments}</td>
  </tr>
</table>

<table cellspacing="5">
  <tr>
    <th>{g->text text="Photo"}</th>
    <th>{g->text text="Info"}</th>
    <th>{g->text text="Product"}/{g->text text="Price"}/{g->text text="Quantity"}</th>
    {if $paper[0] != ""}
      <th>{g->text text="Paper"}</th>
    {/if}
    <th>{g->text text="Subtotal"}</th>
  </tr>
  <tr>
    {foreach from=$items item=item}
      <tr>
          <td>
	      <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$item.id`"} forceFullUrl=true">
            {if isset($thumbnails[$item.id])}
              {g->image item=$items[$item.id] image=$thumbnails[$item.id] maxSize=150 forceFullUrl=true}
            {else}
              {g->text text="No thumbnail"}
            {/if}
	      </a>
          </td>
	<td align="top">
	  <strong>{g->text text="Title"}:</strong> 
	  {$item.title|markup}<br>
	  <strong>{g->text text="Summary"}:</strong> 
	  {$item.summary|markup}
	</td> 
	<td>
	  <table cellspacing="0" cellpadding="10" border="1" bordercolor="#999999">
	    {section name=x loop=$product}
	      {if isset($item.quant[x]) }
	        <tr>
	          <td><center>{$product[x]}</td>
	          <td><center>{$csymbol}{if isset($item.itemPrices[x])}{$item.itemPrices[x]}{else}{$price[x]}{/if}</td>
	          <td><center><b>{$item.quant[x]}</td>
	        </tr>
	      {/if}
	    {/section}      	    		
	  </table>
	</td>
	{if $item.paper != ""}
	  <td>    	  		
	    {$item.paper}
	  </td>
	{/if}
	<td align="right"><strong>{$csymbol}{$item.subtotal}</strong></td>
      </tr>
    {/foreach} 
    <tr>
      {if $paper[0] !=""}
	<td colspan="4" align="right">{g->text text="Postage and Packing"}:</td>
      {else}
	<td colspan="3" align="right">{g->text text="Postage and Packing"}:</td>
      {/if}
      <td align="right"><b>{$csymbol}{$postage}</b></td>
    </tr>
    <tr>
      {if $paper[0] !=""}
        <td colspan="4" align="right"><strong>{g->text text="Total"}:</strong></td>
      {else}
        <td colspan="3" align="right"><strong>{g->text text="Total"}:</strong></td>
      {/if}
      <td align="right"><strong>{$csymbol}{$total}</strong></td>
    </tr>
  </tr>
</table>
</body>
</html>

--{$boundary}--
