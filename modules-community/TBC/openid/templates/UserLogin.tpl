{*
 * $Revision: 1574 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Login to your account"} </h2>
</div>

{if isset($form.error.openId.error)}
  <div class="gbBlock">
    <h2 class="giError">
      {if $responseStatus == Auth_OpenID_CANCEL}
        {g->text text="Authentication was cancelled."}
      {elseif $responseStatus == Auth_OpenID_FAILURE}
        {g->text text="Authentication failed."}
      {else}
        {g->text text="Authentication error."}
      {/if}
      <br/>
      {$responseMessage}
    </h2>
  </div>
{/if}

{if $user.isGuest}
  <input name="{g->formVar var="return"}" type="hidden" value="{$form.returnUrl}"/>
  <input name="{g->formVar var="form[returnUrl]"}" type="hidden" value="{$form.returnUrl}"/>
  <div class="gbBlock">
    <h4> {g->text text="OpenID"} </h4>

    <input id="giFormOpenId" name="{g->formVar var="form[openId]"}" type="text" value="{$form.openId}" size="16"/>

    <script type="text/javascript">
      // <![CDATA[
      document.getElementById('userAdminForm')['{g->formVar var="form[openId]"}'].focus();
      // ]]>
    </script>

    {if isset($form.error.openId.missing)}
      <div class="giError">
	{g->text text="You must enter an OpenID."}
      </div>
    {/if}
    {if isset($form.error.openId.invalid)}
      <div class="giError">
	{g->text text="Your login information is incorrect.  Please try again."}
      </div>
    {/if}
    {if isset($form.error.openId.no_register)}
      <div class="giError">
	{g->text text="This OpenID is not recognized and new IDs are not accepted at this time."}
      </div>
    {/if}
    {if isset($form.error.userName.missing)}
      <div class="giError">
	{g->text text="You must enter a username."}
      </div>
    {/if}
    {if isset($form.error.userName.exists)}
      <div class="giError">
	{g->text text="Username '%s' already exists." arg1=$status.userName}
      </div>
    {/if}
    {if isset($form.error.userName.pending)}
      <div class="giError">
	{g->text text="New account for this login has not yet been approved."}
      </div>
    {/if}
    {if isset($form.error.userName.disabled)}
      <div class="giError">
	{g->text text="This account is currently disabled."}
      </div>
    {/if}
    {if isset($form.error.fullName.missing)}
      <div class="giError">
	{g->text text="You must enter your full name."}
      </div>
    {/if}
    {if isset($form.error.email.missing)}
      <div class="giError">
	{g->text text="You must enter an email address."}
      </div>
    {/if}

    <br/>
    <input type="checkbox" name="{g->formVar var="form[setCookie]"}"
     {if $form.setCookie}checked="checked"{/if}/>
    {g->text text="Remember my OpenID"}
  </div>

  {* Include our validation plugins *}
  {g->callback type="core.LoadValidationPlugins" key="openid.UserLogin."|cat:$form.openId}
  {foreach from=$block.core.ValidationPlugins item=plugin}
    {include file="gallery:`$plugin.file`" l10Domain=$plugin.l10Domain}
  {/foreach}

  <div class="gbBlock gcBackground1">
    <input class="inputTypeSubmit" name="{g->formVar var="form[action][login]"}" type="submit" value="{g->text text="Login"}"/>
    <input class="inputTypeSubmit" name="{g->formVar var="form[action][cancel]"}" type="submit" value="{g->text text="Cancel"}"/>
  </div>
{else}
  {* User is already logged in *}
  <div class="gbBlock">
    <h4> {g->text text="Welcome, %s!" arg1=$user.fullName|default:$user.userName} </h4>
  </div>
{/if}
