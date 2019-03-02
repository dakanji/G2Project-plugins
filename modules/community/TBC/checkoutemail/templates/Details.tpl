{*
 * $Revision: 1583 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<div id="checkoutForm">
        <div class="gbBlock gcBackground1">
          <h2 class="gbTitle">
            {g->text text="Checkout"} {g->text text="Customer Details"}
          </h2>
        </div>
<form action="{g->url}" method="post">
  {g->hiddenFormVars}
  <input type="hidden" name="{g->formVar var="controller"}" value="{$form.controller}"/>
  <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>

  <p>{g->text text="Please enter your details below to complete the transaction"}:</p>

	<table cellspacing="5" class="gbDataTable gcBorder1 checkoutEmailDetailsTable" width="80%">
	{if isset($form.error.badEmail)}
	  <tr><td colspan="2" align="center">
	  <span class="checkoutErrorText" style="color:Red;font-weight:bold;">
	  {g->text text="You must enter a valid email address"}
	  </span>
	  </td>
	  </tr>
	{/if}
	  <tr>
	    <td align="right"><strong>{g->text text="First Name"}:</strong></td>
	    <td><input type="text" name="{g->formVar var="form[custFirstName]"}" value="{if isset($form.custFirstName)}{$form.custFirstName}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Last Name"}:</strong></td>
	    <td><input type="text" name="{g->formVar var="form[custLastName]"}" value="{if isset($form.custLastName)}{$form.custLastName}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Email Address (required)"}:</strong></td>
	    <td><input type="text" name="{g->formVar var="form[custEmail]"}" value="{if isset($form.custEmail)}{$form.custEmail}{/if}" size="50" /></td>
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Recipient's Name (if different to above)"}:</strong></td>
	    <td><input type="text" name="{g->formVar var="form[custRecipientName]"}" value="{if isset($form.custRecipientName)}{$form.custRecipientName}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Postal Address"}:</strong></td>
	    <td><input type="text" name="{g->formVar var="form[custAddress1]"}" value="{if isset($form.custAddress1)}{$form.custAddress1}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right">&nbsp;</td>
	    <td><input type="text" name="{g->formVar var="form[custAddress2]"}" value="{if isset($form.custAddress2)}{$form.custAddress2}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right">&nbsp;</td>
	    <td><input type="text" name="{g->formVar var="form[custAddress3]"}" value="{if isset($form.custAddress3)}{$form.custAddress3}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right">&nbsp;</td>
	    <td><input type="text" name="{g->formVar var="form[custAddress4]"}" value="{if isset($form.custAddress4)}{$form.custAddress4}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right">&nbsp;</td>
	    <td><input type="text" name="{g->formVar var="form[custAddress5]"}" value="{if isset($form.custAddress5)}{$form.custAddress5}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Postal/Zip Code"}:</strong></td>
	    <td><input type="text" name="{g->formVar var="form[custZip]"}" value="{if isset($form.custZip)}{$form.custZip}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Country"}:</strong></td>
	    <td><input type="text" name="{g->formVar var="form[custCountry]"}" value="{if isset($form.custCountry)}{$form.custCountry}{/if}" size="30" /></td>
	  </tr>
	   <tr>
	    <td align="right"><strong>{g->text text="Contact Telephone Number"}:</strong></td>
	    <td><input type="text" name="{g->formVar var="form[telephone]"}" value="{if isset($form.telephone)}{$form.telephone}{/if}" size="30" /></td>
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Comments"}:</strong></td>
	    <td><textarea name="{g->formVar var="form[custComments]"}">{if isset($form.custComments)}{$form.custComments}{/if}</textarea></td>
	  </tr>
	</table>
  <input type="submit" class="inputTypeSubmit checkoutButton" name="{g->formVar var="form[action][send]"}" value="{g->text text="Submit Order"}" />
</form>
</div>