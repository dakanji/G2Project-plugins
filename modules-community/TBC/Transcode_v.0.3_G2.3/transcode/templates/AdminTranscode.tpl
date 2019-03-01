{*
 * $Revision:$
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Transcode Settings"} </h2>
</div>

{if !empty($status)}
<div class="gbBlock"><h2 class="giSuccess">
  {if isset($status.saved)}
    {g->text text="Settings saved successfully"}
  {/if}
</h2></div>
{/if}

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="Transcode handles converting videos."}
  </p>
</div>

<div class="gbBlock">
	<h3> {g->text text="Transcode defaults"} </h3>
  <table class="gbDataTable">
    <tr>
      <td>
        {g->text text="Output MIME Type"}
      </td>
      <td>
        <select name="{g->formVar var="form[outputMime]"}">
	{foreach from=$outputOptions item=option}
	<option value="{$option}"{if ($option==$form.outputMime)} selected="selected"{/if}>{g->text text="$option"}</option>
	{/foreach}
      </select>
      </td>
    </tr>
  <tr>
    <td>
      {g->text text="Max Width"}
    </td>
    <td>
      <input type="text" size="5" name="{g->formVar var="form[maxWidth]"}" value="{$form.maxWidth}"/>
    </td>
  </tr>
  <tr>
    <td>
      {g->text text="Max Height"}
    </td>
    <td>
      <input type="text" size="5" name="{g->formVar var="form[maxHeight]"}" value="{$form.maxHeight}"/>
    </td>
  </tr>
  <tr>
    <td>
      {g->text text="Default Video bitrate"}
    </td>
    <td>
        <select name="{g->formVar var="form[defaultBitrate]"}">
          <option value="" selected>{g->text text="Same as source"}</option>
          <option value="150"{if ($form.defaultBitrate==150)} selected{/if}>{g->text text="Low (150kbps)"}</option>
          <option value="400"{if ($form.defaultBitrate==400)} selected{/if}>{g->text text="Med (400kbps)"}</option>
          <option value="700"{if ($form.defaultBitrate==700)} selected{/if}>{g->text text="High (700kbps)"}</option>
          <option value="900"{if ($form.defaultBitrate==900)} selected{/if}>{g->text text="HD Low (900kbps)"}</option>
          <option value="1200"{if ($form.defaultBitrate==1200)} selected{/if}>{g->text text="HD High (1200kbps)"}</option>
        </select>
    </td>
  </tr>

  <tr>
    <td>
      {g->text text="Default Audio Channels"}
    </td>
    <td>
        <select name="{g->formVar var="form[defaultAudioChannels]"}">
          <option value="0"{if ($form.defaultAudioChannels==0)} selected{/if}>Mute</option>
          <option value="1"{if ($form.defaultAudioChannels==1)} selected{/if}>Mono</option>
          <option value="2"{if ($form.defaultAudioChannels==2)} selected{/if}>Stereo</option>
        </select>

    </td>
  </tr>
  <tr>
    <td>
      {g->text text="Default Audio Sample Rate (Hz)"}
    </td>
    <td>
        <select name="{g->formVar var="form[defaultAudioSampleRate]"}">
          <option value="11025"{if ($form.defaultAudioSampleRate==11025)} selected{/if}>11025</option>
          <option value="22050"{if ($form.defaultAudioSampleRate==22050)} selected{/if}>22050</option>
          <option value="44100"{if ($form.defaultAudioSampleRate==44100)} selected{/if}>44100</option>
        </select>
    </td>
  </tr>

	<tr>
		<td valign="top">{g->text text="Transcode these mimetypes"}:</td>
		<td>{foreach from=$transcodableMime item=mime}<input type="checkbox" {if in_array($mime, $selectedMimeTypes)} checked="checked"{/if} name="{g->formVar var="form[selectedMimeTypes][$mime]"}"/>{g->text text="$mime"}<br />{/foreach}</td>
	</tr>
	<tr>
  </table>

<div class="gbBlock">
  <h3> {g->text text="Item Upload"} </h3>

  <p class="giDescription">
    <input type="checkbox" id="cbOnUpload"{if $form.onUpload} checked="checked"{/if}
     name="{g->formVar var="form[onUpload]"}"/>
    <label for="cbOnUpload">
      {g->text text="Enable automatic video conversion on upload?"}
    </label>
  </p>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
</div>

