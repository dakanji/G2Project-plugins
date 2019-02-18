{*
 * $Revision: 1869 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<div class="gbBlock gcBackground1">
  <h2> {g->text text="Contact Owner Settings"} </h2>
</div>

<div class="gbBlock">
{if !empty($status)}
  {if isset($status.saved)}
    <div class="giSuccess">
	{g->text text="Settings saved successfully"}
    </div>
  {/if}
{/if}

{if !empty($form.error)}
  {if !empty($form.error.contactOwner.email)}
    <div class="giError">
	{g->text text="Invalid Email"}
    </div>
  {/if}
  {if !empty($form.error.contactOwner.maxEmails.notnumeric)}
    <div class="giError">
	{g->text text="Maximum number of emails is not numeric"}
    </div>
  {/if}
  {if !empty($form.error.contactOwner.maxEmails.value)}
    <div class="giError">
	{g->text text="The maximum number of emails is not valid, please enter a number betwee 1 and 999"}
    </div>
  {/if}
{/if}

  <p class="giDescription">
    {g->text text="Enter the values you need to receive contact emails."}
  </p>
    <div class="gbDataEntry">
      <h3 class="giTitle">
	{g->text text="Email address to send contact information to"}
      </h3>
      <input type="text" name="{g->formVar var="form[toEmailAddress]"}" size="50" value="{$form.toEmailAddress}">
      <br>
      <h3 class="giTitle">
      {g->text text="Subject line to appear on the emails"}
      </h3>
      <input type="text" name="{g->formVar var="form[toSubject]"}" size="50" value="{$form.toSubject}">
      <br>
      <h3 class="giTitle">
      {g->text text="Maximum number of emails that can be sent per browser session"}
      </h3>
      <input type="text" name="{g->formVar var="form[maxEmailsPerSession]"}" size="3" value="{$form.maxEmailsPerSession}">
      <br><br>
    </div>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
</div>

