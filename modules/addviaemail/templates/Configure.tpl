{*
 * $Revision: 896 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Add via Email Configuration"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Settings saved successfully"}
</h2></div>
{/if}

<div class="gbBlock">
  <h3> {g->text text="Backend/Code Check"} </h3>
  <p class="giDescription">
    {g->text text="Verification of all backend functions needed by plugin."}
  </p>

  {if !empty($AddviaEmailConfigure.funcsReport.fail)}
  <table class="gbDataTable">
    <tr>
      <th>
        {g->text text="Function name"}
      </th>
      <th>
        {g->text text="Pass/fail"}
      </th>
    </tr>

    {foreach from=$AddviaEmailConfigure.funcsReport.success item=func}
    <tr class="{cycle values="gbEven,gbOdd"}">
      <td>
        {$func}
      </td>
      <td>
        <div class="giSuccess">
          {g->text text="Available"}
        </div>
      </td>
    </tr>
    {/foreach}

    {foreach from=$AddviaEmailConfigure.funcsReport.fail item=func}
    <tr class="{cycle values="gbEven,gbOdd"}">
      <td>
        {$func}
      </td>
      <td>
        <div class="giError">
          {g->text text="Missing"}
        </div>
      </td>
    </tr>
    {/foreach}
  </table>

  <div class="giError">
    {g->text text="Critical email functions are not available.  Please ask your system administrator for help.<br/>"}
    {g->text text="No further configuration is possible until the above errors are resolved."}
  </div>
  {else}
  <div class="giSuccess">
    {g->text text="Your webserver is properly configured to use the addviaemail module."}
  </div>
  {/if}
</div>

{if empty($addviaemailfuncsreport.funcsReport.fail)}
<div class="gbBlock">
  <p class="giDescription">
    {g->text text="This part defines the source of the emailed images."}
  </p>

{literal}
<script type="text/javascript">
// <![CDATA[
function checkType() {
	var deleteSelect = document.getElementById('deleteSelect');
	if (document.getElementById('serverTypeSelect').selectedIndex == 0) {
		deleteSelect.selectedIndex = 0;
		deleteSelect.disabled = true;
	} else {
		deleteSelect.disabled = false;
	}

}

function validPollFloor(theEl) {
	if ( theEl.value.length == 0 ) { 
		alert('You have not entered a number') 
	} 
 
	if (!IsNumeric(theEl.value)) { 
		alert('Please enter only numbers Time Cap Value field') 
	} 
 
}

function IsNumeric(sText) {
	var ValidChars = "0123456789";
	var IsNumber=true;
	var Char;

	for (i = 0; i < sText.length && IsNumber == true; i++) { 
		Char = sText.charAt(i); 
		if (ValidChars.indexOf(Char) == -1) {
			IsNumber = false;
		}
	}
	return IsNumber;
}

// ]]>
</script>
{/literal}

  <h4> {g->text text="Server Type"} </h4>
  <p class="giDescription">
    {g->text text="Email server type. POP3 or IMAP."}
  </p>
  <select id="serverTypeSelect" onchange="checkType()" name="{g->formVar var="form[serverType]"}">
    <option value="1" {if $form.serverType==1}selected="selected"{/if}>POP3</option>
    <option value="2" {if $form.serverType==2}selected="selected"{/if}>IMAP</option>
  </select>

  <h4> {g->text text="Server Address"} </h4>
  <p class="giDescription">
    {g->text text="Fully qualified server address for email account."}
  </p>
  <input type="text" size="30" name="{g->formVar var="form[serverAddress]"}"
   value="{$form.serverAddress}"/>
  {if isset($form.error.serverAddress.missing)}
  <div class="giError">
    {g->text text="Missing Server Address"}
  </div>
  {/if}

  <h4> {g->text text="SSL"} </h4>
  <p class="giDescription">
    {g->text text="Does the server use Secure Socket Layers (SSL) for connection security?"}
  </p>
  <select name="{g->formVar var="form[ssl]"}">
    <option value="2" {if $form.ssl==2}selected="selected"{/if}>{g->text text="No"}</option>
    <option value="1" {if $form.ssl==1}selected="selected"{/if}>{g->text text="Yes"}</option>
  </select>

  <h4> {g->text text="Port"} </h4>
  <p class="giDescription">
    {g->text text="What port (Defaults: 110 POP, 995 POP/SSL, 143 IMAP, 993 IMAP/SSL)?"}
  </p>
  <input type="text" size="5" name="{g->formVar var="form[port]"}" value="{$form.port}"/>
  {if isset($form.error.port.missing)}
  <div class="giError">
    {g->text text="Missing server Port"}
  </div>
  {/if}

  <h4> {g->text text="Username"} </h4>
  <p class="giDescription">
    {g->text text="Complete username for email account."}
  </p>
  <input type="text" size="30" name="{g->formVar var="form[userName]"}" value="{$form.userName}"/>
  {if isset($form.error.userName.missing)}
  <div class="giError">
    {g->text text="Missing username"}
  </div>
  {/if}

  <h4> {g->text text="Password"} </h4>
  <p class="giDescription">
    {g->text text="Complete password for email account."}
  </p>
  <input type="password" size="30"
   name="{g->formVar var="form[passWord]"}" value="{$form.passWord}"/>
  {if isset($form.error.passWord.missing)}
  <div class="giError">
    {g->text text="Missing password"}
  </div>
  {/if}

  <h4> {g->text text="Subject to Match"} </h4>
  <p class="giDescription">
    {g->text text="Subject to look for in incoming emails that denotes a post to Gallery. This is useful as an anti-spam measure as it is not used in the post to Gallery. It is also useful in the situation where the email account is used for more than just Gallery posts."}
  </p>
  <input type="text" size="30" name="{g->formVar var="form[subjectToMatch]"}" value="{$form.subjectToMatch}"/>
  {if isset($form.error.subjectToMatch.missing)}
  <div class="giError">
    {g->text text="Missing Subject to Match"}
  </div>
  {/if}

  <h4> {g->text text="Path to TMP directory"} </h4>
  <p class="giDescription">
    {g->text text="During the process of processing emails, this plugin needs a place to store temporary files. This should be writeable by all. This should include a trailing slash: example: /tmp/"}
  </p>

  <input type="text" size="50"
   name="{g->formVar var="form[tmpLocation]"}" value="{$form.tmpLocation}"/>
  {if isset($form.error.tmpLocation.missing)}
  <div class="giError">
    {g->text text="Missing TMP Location"}
  </div>
  {/if}

  <h4> {g->text text="Owner of the Pictures"} </h4>
  <p class="giDescription">
    {g->text text="Who will own the pictures in the ablbum."}
  </p>
  <input type="text" id="giFormUsername" size="20" autocomplete="off"
   name="{g->formVar var="form[owner]"}" value="{$form.owner}"/>
  {g->autoComplete element="giFormUsername"}
    {g->url arg1="view=core.SimpleCallback" arg2="command=lookupUsername"
            arg3="prefix=__VALUE__" htmlEntities=false}
  {/g->autoComplete}
  {if isset($form.error.owner.missing)}
  <div class="giError">
    {g->text text="Missing Owner"}
  </div>
  {/if}
  {if isset($form.error.owner.bad)}
  <div class="giError">
    {g->text text="Unknown username"}
  </div>
  {/if}

  <h4> {g->text text="Delete Processed"} </h4>
  <p class="giDescription">
    {g->text text="After an email is process should it be deleted? If you choose POP3, this option is set automatically to deleted.."}
  </p>
  <select id="deleteSelect" name="{g->formVar var="form[deleteProcessed]"}">
    <option value="1" {if
      $form.deleteProcessed==1}selected="selected"{/if}>{g->text text="Yes"}</option>
    <option value="2" {if
      $form.deleteProcessed==2}selected="selected"{/if}>{g->text text="No"}</option>
  </select>

  <h4> {g->text text="Time Cap Value"} </h4>
  <p class="giDescription">
    {g->text text="Minimum number of minutes between mailbox polls. (minimum value=1)"}
  </p>
  <input id="pollFloor" type="text" size="5" onBlur="validPollFloor(this);" name="{g->formVar var="form[pollFloor]"}" value="{$form.pollFloor}"/>
  {if isset($form.error.pollFloor.missing)}
  <div class="giError">
    {g->text text="Missing value"}
  </div>
  {/if}

</div>


<div class="gbBlock">
  <h2> {g->text text="Destination Configuration"} </h2>
  <p class="giDescription">
    {g->text text="This part defines the target album for emailed images.  The album will be created using the information below."}
  </p>

  <h4> {g->text text="Upload Album"} </h4>
  <p class="giDescription">
    {g->text text="Enter a title for the Upload Album, if you want one created. Otherwise pick one from the list below."}
  </p>

  <input type="text" size="30" name="{g->formVar var="form[album]"}" value="{$form.album}"/>

  {if isset($form.error.album.missing)}
  <div class="giError">
    {g->text text="Missing album title"}
  </div>
  {/if}

  <p class="giDescription" style="margin-top:0.7em">
    {g->text text="Choose the Upload Album or if you typed a new album name above, choose where you want it created."}
  </p>
  <select name="{g->formVar var="form[destinationAlbum]"}">
    {foreach from=$AddviaEmailConfigure.albumTree item=album}
      <option value="{$album.data.id}"{if
       $album.data.id==$form.destinationAlbum} selected="selected"{/if}>
	{"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"|repeat:$album.depth}--
	{$album.data.title|default:$album.data.pathComponent}
      </option>
    {/foreach}
  </select>

  <h4> {g->text text="Body as Caption"} </h4>
  <p class="giDescription">
    {g->text text="Should the body of the email be used for the caption of the picture in Gallery""}
  </p>
  <select name="{g->formVar var="form[bodyAsCaption]"}">
    <option value="1"{if
     $form.bodyAsCaption==1} selected="selected"{/if}>{g->text text="Yes"}</option>
    <option value="2"{if
     $form.bodyAsCaption==2} selected="selected"{/if}>{g->text text="No"}</option>
  </select>
</div>


<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save Settings"}"/>
  {if $AddviaEmailConfigure.isConfigure}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
  {else}
    <input type="submit" class="inputTypeSubmit"
     name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
  {/if}
</div>

<script type="text/javascript">
	checkType();
</script>
{/if}
