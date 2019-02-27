{*
 * $Revision: 1572 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="OpenID Settings"} </h2>
</div>

{if isset($status.saved)}
  <div class="gbBlock">
    <h2 class="giSuccess"> {g->text text="Settings saved successfully"} </h2>
  </div>
{/if}

<div class="gbBlock">
  <h3> Unfinished module! </h3>
  <p class="giDescription">
    Currently this module only has the ability to login <i>known</i> users using their OpenID.
    Add to list of known users using form below.  There is code in the module to integrate
    with the registration module, so that someone logging in with a new OpenID can be jumped
    into the registration process.  However, this portion is incomplete.
  </p>
  <h4> Add OpenID user </h4>
  OpenID: <input type="text" name="{g->formVar var="form[openId]"}" size="30"/>
  &nbsp; &nbsp;
  Username: <input type="text" name="{g->formVar var="form[username]"}" size="20"/>
  &nbsp; &nbsp;
  <input type="submit" name="{g->formVar var="form[action][addMapping]"}" value="Add"/>
</div>

<div class="gbBlock">
  <h3> {g->text text="Randomness Source"} </h3>

  {if $OpenIdSiteAdmin.randSource === null}
    <p class="giDescription">
      {g->text text="Using (insecure) pseudorandom number source, because Auth_OpenID_RAND_SOURCE has been defined as null."}
    </p>
  {else}
    <p class="giDescription">
      {g->text text="The module will try to access '%s' as a source of random data." arg1=$OpenIdSiteAdmin.randSource}
      {if $OpenIdSiteAdmin.randHandle === false}
        {g->text text="  '%s' could not be opened.  This could be because of restrictions on your PHP envirnment or that randomness source may not exist on this platform." arg1=$OpenIdSiteAdmin.randSource}
	{if !strncasecmp(PHP_OS, 'win', 3)}
	  {g->text text="  You seem to be running Windows.  This module does not have access to a good source of randomness on Windows."}
	{/if}
      {else}
	{g->text text="It seems to exist"}
        {if strlen($OpenIdSiteAdmin.randData) != $OpenIdSiteAdmin.expectedSize}
	  {g->text text=" but reading data failed."}
	{else}
	  {g->text text=" and be readable.  Here is some hex data:"} {$OpenIdSiteAdmin.randData|bin2hex}.
	{/if}
	{if $OpenIdSiteAdmin.randSize}
	  {g->text text="  This is a %s byte file.  Unless you know what you are doing, it is likely that you are making a mistake by using a regular file as a randomness source." arg1=$OpenIdSiteAdmin.randSize}
	{/if}
      {/if}
    </p>
  {/if}
  {if $OpenIdSiteAdmin.randSource === null || $OpenIdSiteAdmin.randHandle === false || strlen($OpenIdSiteAdmin.randData) != $OpenIdSiteAdmin.expectedSize || $OpenIdSiteAdmin.randSize}
    <p class="giDescription">
      {g->text text="To set a source of randomness, define Auth_OpenID_RAND_SOURCE to the path to the randomness source.  If your platform does not provide a secure randomness source, the module can operate in pseudorandom mode, but it is then vulnerable to theoretical attacks.  If you wish to operate in pseudorandom mode, define Auth_OpenID_RAND_SOURCE to null."}
    </p>

    <p class="giDescription">
      {g->text text="You are running on:"} <pre> {$OpenIdSiteAdmin.uname} </pre>
    </p>

    <p class="giDescription">
      {g->text text="There does not seem to be an available source of randomness.  On a Unix-like platform (including Mac OS X), try '/dev/random' and '/dev/urandom'."}
    </p>
  {/if}
</div>

<div class="gbBlock">
  <h3> {g->text text="XML Support"} </h3>

  {if $OpenIdSiteAdmin.xmlExtension === null}
    <p class="giDescription">
      {g->text text="XML parsing support is absent; please install one of the following PHP extensions:"}
      <ul>
	{foreach from=$OpenIdSiteAdmin.xmlExtensions item="class" key="name"}
	  <li><a href="http://php.net/{$name}"> {$name} </a></li>
	{/foreach}
      </ul>
    </p>
  {else}
    <p class="giDescription">
      {g->text text="XML parsing support is present using the %s interface." arg1=$OpenIdSiteAdmin.xmlExtension|get_class}
    </p>
  {/if}
</div>

<div class="gbBlock">
  <h3> {g->text text="HTTP Fetching"} </h3>

  {if !$OpenIdSiteAdmin.curlPresent}
    <p class="giDescription">
      {g->text text="This PHP installation does not have support for libcurl.  Some functionality, such as fetching https:// URLs, will be missing and performance will not be as good."}
    </p>

    <p class="giDescription">
      {g->text text="See %s about enabling the libcurl support for PHP." arg1="<a href=\"http://php.net/curl\"> http://php.net/curl </a>"}
    </p>
  {else}
    <p class="giDescription">
      {g->text text="This PHP installation has support for libcurl.  Good."}
    </p>
  {/if}
  {if !isset($OpenIdSiteAdmin.fetchResult)}
    <p class="giDescription">
      {g->text text="Fetching URL %s failed!" arg1=$OpenIdSiteAdmin.fetchUrl}
    </p>
  {else}
    <p class="giDescription">
      {g->text text="An HTTP request was completed."}
      {if $OpenIdSiteAdmin.fetchResult->status != 200}
        {g->text text="  Got %s instead of the expected HTTP status code (200)." arg1=$OpenIdSiteAdmin.fetchResult->status}
      {/if}
      {if $OpenIdSiteAdmin.fetchResult->final_url != $OpenIdSiteAdmin.expectedUrl}
        {if $OpenIdSiteAdmin.fetchResult->final_url != $OpenIdSiteAdmin.fetchUrl}
	  {g->text text="  An unexpected URL was returned:"} {$OpenIdSiteAdmin.fetchResult->final_url}
	{else}
	  {g->text text="  The redirected URL was not returned."}
	{/if}
      {/if}
      {if $OpenIdSiteAdmin.fetchResult->body != "Hello World!"}
        {g->text text="  Unexpected data was returned."}
      {/if}
    </p>
  {/if}
</div>
