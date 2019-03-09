{*
 * $Revision: 1188 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Checkout by Authorize.Net Settings"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock">
  <h2 class="giSuccess">{g->text text="Settings saved successfully."}</h2>
</div>
{/if}

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="Authorize.Net is a third-party payment gateway. You must have a valid account in
                   order to accept transactions, or a developer account to test transactions. You
                   must have cURL installed on the command line. Check with your host if you are
                   unsure."}
  </p>

  <table class="gbDataTable">
    <tr>
      <td>
        {g->text text="Authorize.Net login ID:"}
      </td>
      <td>
        <input type="text" id="{g->formVar var="form[authorizeNetId]"}"
         name="{g->formVar var="form[authorizeNetId]"}" value="{$form.authorizeNetId}" />

        {if isset($form.error.authorizeNetId.bad)}
        <div class="giError">
          {g->text text="You must enter a valid Authorize.Net login ID."}
        </div>
        {/if}
      </td>
    </tr>
    <tr>
      <td>
        {g->text text="Authorize.Net transaction key:"}
      </td>
      <td>
        <input type="text" id="{g->formVar var="form[tranKey]"}"
         name="{g->formVar var="form[tranKey]"}" value="{$form.tranKey}" />

        {if isset($form.error.tranKey.bad)}
        <div class="giError">
          {g->text text="You must enter an Authorize.Net transaction key."}
        </div>
        {/if}
      </td>
    </tr>
    <tr>
      <td>
        {g->text text="Authorize.Net operating mode:"}
      </td>
      <td>
        <select id="{g->formVar var="form[mode]"}" name="{g->formVar var="form[mode]"}">
          {foreach from=$AdminCheckout.modes item=modeName key=mode}
          <option value="{$mode}"{if $mode == $form.mode} selected="selected"{/if}>
            {$modeName}
          </option>
          {/foreach}
        </select>
      </td>
    </tr>
    <tr>
      <td>
        {g->text text="Path to cURL (the default usually works):"}
      </td>
      <td>
        <input type="text" id="giFormPath" size="40" autocomplete="off"
         name="{g->formVar var="form[curlPath]"}" value="{$form.curlPath}" />
        {g->autoComplete element="giFormPath"}
          {g->url arg1="view=core.SimpleCallback" arg2="command=lookupFiles"
           arg3="prefix=__VALUE__" forJavascript="true"}
        {/g->autoComplete}

        {if isset($form.error.curlPath.bad)}
        <div class="giError">
          {g->text text="You must enter the path to a valid cURL executable."}
        </div>
        {/if}
      </td>
    </tr>
    <tr>
      <td>
        {g->text text="'From' address for confirmation e-mails:"}
      </td>
      <td>
        <input type="text" id="{g->formVar var="form[fromEmail]"}"
         name="{g->formVar var="form[fromEmail]"}" value="{$form.fromEmail}" />

        {if isset($form.error.fromEmail.bad)}
        <div class="giError">
          {g->text text="You must enter a valid e-mail address."}
        </div>
        {/if}
      </td>
    </tr>
    <tr>
      <td>
        {g->text text="Subject line for confirmation e-mails:"}
      </td>
      <td>
        <input type="text" id="{g->formVar var="form[emailSubject]"}"
         name="{g->formVar var="form[emailSubject]"}" value="{$form.emailSubject}" />
      </td>
    </tr>
    <tr>
      <td>
        {g->text text="Address to send order for processing:"}
      </td>
      <td>
        <input type="text" id="{g->formVar var="form[storeEmail]"}"
         name="{g->formVar var="form[storeEmail]"}" value="{$form.storeEmail}" />

        {if isset($form.error.storeEmail.bad)}
        <div class="giError">
          {g->text text="You must enter a valid e-mail address."}
        </div>
        {/if}
      </td>
    </tr>
  </table>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save Settings"}"/>
  {if isset($AdminCheckout.isConfigure)}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
  {else}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
  {/if}
</div>