{*
 * $Revision: 1188 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}


<div class="gbBlock gcBackground1">
  <h2 class="gbTitle">
    {g->text text="Checkout"}::{g->text text="Step 3 - Customer Details"}
  </h2>
</div>
<div class="gbBlock">
<form action="{g->url}" method="post">
  {g->hiddenFormVars}
  <input type="hidden" name="{g->formVar var="controller"}" value="{$form.controller}" />
  <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}" />

  {if !empty($form.error)}
  <div class="giError">
    {g->text text="The following errors have occurred:"}
    {if isset($form.error.authorizeNet.fail)}
      <br />
      {g->text text="* Authorize.Net returned the following error:"}<br />
      {$form.error.authorizeNet.msg}
    {/if}
    {if isset($form.error.custName.bad)}
      <br />
      {g->text text="* Please enter your full first and last names"}
    {/if}
    {if isset($form.error.custEmail.bad)}
      <br />
      {g->text text="* Please enter a valid e-mail address"}
    {/if}
    {if isset($form.error.custPhone.bad)}
      <br />
      {g->text text="* Please enter your phone number"}
    {/if}
    {if isset($form.error.custCcNumber.bad)}
      <br />
      {g->text text="* Please enter a valid 16-digit credit card number with no spaces or dashes"}
    {/if}
    {if isset($form.error.custCcMonth.bad)}
      <br />
      {g->text text="* Please enter a valid credit card expiry month"}
    {/if}
    {if isset($form.error.custCcYear.bad)}
      <br />
      {g->text text="* Please enter a valid credit card expiry year"}
    {/if}
    {if isset($form.error.custAddress.bad)}
      <br />
      {g->text text="* Please enter a valid billing address"}
    {/if}
    {if isset($form.error.custCity.bad)}
      <br />
      {g->text text="* Please enter your city"}
    {/if}
    {if isset($form.error.custState.bad)}
      <br />
      {g->text text="* Please enter your state"}
    {/if}
    {if isset($form.error.custCcZip.bad)}
      <br />
      {g->text text="* Please enter a valid ZIP code"}
    {/if}
    {if isset($form.error.custCcCountry.bad)}
      <br />
      {g->text text="* Please enter your country"}
    {/if}
    {if isset($form.error.custAbaCode.bad)}
      <br />
      {g->text text="* Please enter a valid 9-digit routing number (ABA code)"}
    {/if}
    {if isset($form.error.custAccountNumber.bad)}
      <br />
      {g->text text="* Please enter a valid checking account number"}
    {/if}
  </div>
  {/if}

  <p class="giDescription">Please enter your details below to complete the transaction</p>

	<table class="gbDataTable">
	  <tr>
	    <th colspan="2">{g->text text="Contact Information (required)"}</th>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="First Name"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custFirstName]"}" value="{if isset($form.custFirstName)}{$form.custFirstName}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Last Name"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custLastName]"}" value="{if isset($form.custLastName)}{$form.custLastName}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="E-mail Address"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custEmail]"}" value="{if isset($form.custEmail)}{$form.custEmail}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Phone Number"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custPhone]"}" value="{if isset($form.custPhone)}{$form.custPhone}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <th colspan="2">{g->text text="Billing Information (required)"}</th>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Credit Card Number"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custCcNumber]"}" value="{if isset($form.custCcNumber)}{$form.custCcNumber}{/if}" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Expiry Date"}
	    </td>
	    <td>
	        <select id="{g->formVar var="form[custCcMonth]"}" name="{g->formVar var="form[custCcMonth]"}">
	          <option value="00"{if isset($form.custCcMonth) && $form.custCcMonth == "00"} selected="selected"{/if}>
	            {g->text text="Month"}
	          </option>
	          <option value="01"{if isset($form.custCcMonth) && $form.custCcMonth == "01"} selected="selected"{/if}>
	            {g->text text="Jan"}
	          </option>
	          <option value="02"{if isset($form.custCcMonth) && $form.custCcMonth == "02"} selected="selected"{/if}>
	            {g->text text="Feb"}
	          </option>
	          <option value="03"{if isset($form.custCcMonth) && $form.custCcMonth == "03"} selected="selected"{/if}>
	            {g->text text="Mar"}
	          </option>
	          <option value="04"{if isset($form.custCcMonth) && $form.custCcMonth == "04"} selected="selected"{/if}>
	            {g->text text="Apr"}
	          </option>
	          <option value="05"{if isset($form.custCcMonth) && $form.custCcMonth == "05"} selected="selected"{/if}>
	            {g->text text="May"}
	          </option>
	          <option value="06"{if isset($form.custCcMonth) && $form.custCcMonth == "06"} selected="selected"{/if}>
	            {g->text text="Jun"}
	          </option>
	          <option value="07"{if isset($form.custCcMonth) && $form.custCcMonth == "07"} selected="selected"{/if}>
	            {g->text text="Jul"}
	          </option>
	          <option value="08"{if isset($form.custCcMonth) && $form.custCcMonth == "08"} selected="selected"{/if}>
	            {g->text text="Aug"}
	          </option>
	          <option value="09"{if isset($form.custCcMonth) && $form.custCcMonth == "09"} selected="selected"{/if}>
	            {g->text text="Sep"}
	          </option>
	          <option value="10"{if isset($form.custCcMonth) && $form.custCcMonth == "10"} selected="selected"{/if}>
	            {g->text text="Oct"}
	          </option>
	          <option value="11"{if isset($form.custCcMonth) && $form.custCcMonth == "11"} selected="selected"{/if}>
	            {g->text text="Nov"}
	          </option>
	          <option value="12"{if isset($form.custCcMonth) && $form.custCcMonth == "12"} selected="selected"{/if}>
	            {g->text text="Dec"}
	          </option>
	        </select>
          <select id="{g->formVar var="form[custCcYear]"}" name="{g->formVar var="form[custCcYear]"}">
            {foreach from=$Details.ccYears item=year}
            <option value="{$year}"{if isset($form.custCcYear) && $year == $form.custCcYear} selected="selected"{/if}>
              {$year}
            </option>
            {/foreach}
          </select>
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Billing Address"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custAddress]"}" value="{if isset($form.custAddress)}{$form.custAddress}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="City"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custCity]"}" value="{if isset($form.custCity)}{$form.custCity}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="State"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custState]"}" value="{if isset($form.custState)}{$form.custState}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="ZIP Code"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custCcZip]"}" value="{if isset($form.custCcZip)}{$form.custCcZip}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Country"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custCcCountry]"}" value="{if isset($form.custCcCountry)}{$form.custCcCountry}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <th colspan="2">{g->text text="Shipping Information (if different)"}</th>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Recipient's Name"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custRecipientName]"}" value="{if isset($form.custRecipientName)}{$form.custRecipientName}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Postal Address"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custAddress1]"}" value="{if isset($form.custAddress1)}{$form.custAddress1}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      &nbsp;
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custAddress2]"}" value="{if isset($form.custAddress2)}{$form.custAddress2}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      &nbsp;
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custAddress3]"}" value="{if isset($form.custAddress3)}{$form.custAddress3}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      &nbsp;
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custAddress4]"}" value="{if isset($form.custAddress4)}{$form.custAddress4}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      &nbsp;
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custAddress5]"}" value="{if isset($form.custAddress5)}{$form.custAddress5}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="ZIP Code"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custZip]"}" value="{if isset($form.custZip)}{$form.custZip}{/if}" size="30" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Country"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custCountry]"}" value="{if isset($form.custCountry)}{$form.custCountry}{/if}" size="30" />
	    </td>
	  </tr>
{* Check processing can be added relatively easily, given the time.
	  <tr>
	    <th colspan="2">{g->text text="Check"}
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Routing Number (ABA code)"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custAbaCode]"}" value="{if isset($form.custAbaCode)}{$form.custAbaCode}{/if}" />
	    </td>
	  </tr>
	  <tr>
	    <td>
	      {g->text text="Account Number"}
	    </td>
	    <td>
	      <input type="text" name="{g->formVar var="form[custAccountNumber]"}" value="{if isset($form.custAccountNumber)}{$form.custAccountNumber}{/if}" />
	    </td>
	  </tr>
*}
	  <tr>
	    <th colspan="2">&nbsp;</th>
	  </tr>
	  <tr>
	    <td>
	      Comments
	    </td>
	    <td>
	      <textarea name="{g->formVar var="form[custComments]"}">{if isset($form.custComments)}{$form.custComments}{/if}</textarea>
	    </td>
	  </tr>
	</table>
  <br />
  <input type="submit" name="{g->formVar var="form[action][send]"}" value="{g->text text="Submit Order"}" />
</form>
</div>