{*
 * $Revision: 1899 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}


        <div class="gbBlock gcBackground1">
          <h2 class="gbTitle">
            {g->text text="Contact"}::{g->text text="Enter Your Details"}
          </h2>
        </div>

<form action="{g->url}" method="post">
{g->hiddenFormVars}
  <input type="hidden" name="{g->formVar var="controller"}" value="{$controller}"/>
{*
  <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>
*}
  <p>{g->text text="Please enter your details below and we will get back to you as soon as possible."}</p>
  {if (!empty($form.error.contactOwner))}
    <p>
      <span class="giError">{g->text text="Please correct the required items below"}</span>
    </p>
  {/if}

	<table cellspacing="5">
	  <tr>
	    <td align="right"><strong>{g->text text="Name:"}</strong></td>
	    <td>
	      <input type="text" size="80" maxlength="80" name="{g->formVar var="form[custName]"}" value="{if isset($form.custName)}{$form.custName}{/if}" size="50" />
	    </td>
	    {if (!empty($form.error.contactOwner.name))}
	      <td style="padding-left:10px" align="left"><span class="giError">{g->text text="&lt;== REQUIRED"}</span>
	    {/if}
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Email Address:"}</strong></td>
	    <td>
	      <input type="text" size="80" maxlength="80" name="{g->formVar var="form[custEmail]"}" value="{if isset($form.custEmail)}{$form.custEmail}{/if}" size="50" />
	    </td>
	    {if (!empty($form.error.contactOwner.email.required))}
	      <td style="padding-left:10px" align="left"><span class="giError">{g->text text="&lt;== REQUIRED"}</span>
	    {/if}
	    {if (!empty($form.error.contactOwner.email.invalid))}
	      <td style="padding-left:10px" align="left"><span class="giError">{g->text text="&lt;== INVALID EMAIL ADDRESS"}</span>
	    {/if}
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Subject:"}</strong></td>
	    <td>
	      <input type="text" size="80" maxlength="80" name="{g->formVar var="form[custSubject]"}" value="{if isset($form.custSubject)}{$form.custSubject}{/if}" size="50" />
	    </td>
	    {if (!empty($form.error.contactOwner.subject))}<td style="padding-left:10px" align="left"><span class="giError">{g->text text="&lt;== REQUIRED"}</span>{/if}
	  </tr>
	  <tr>
	    <td align="right"><strong>{g->text text="Message:"}</strong></td>
	    <td>
	      <textarea rows="10" style="width:100%" name="{g->formVar var="form[custMessage]"}">{if isset($form.custMessage)}{$form.custMessage}{/if}</textarea>
	    </td>
	    {if (!empty($form.error.contactOwner.message))}<td style="padding-left:10px" align="left"><span class="giError">{g->text text="&lt;== REQUIRED"}</span>{/if}
	  </tr>
	</table>

 {* Include validation plugins *}
 {foreach from=$validationPlugins item=plugin}
    {include file="gallery:`$plugin.file`" l10Domain=$plugin.l10Domain}
 {/foreach}

  <br>
<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][send]"}" value="{g->text text="Send Email"}"/>
</div>
</form>


