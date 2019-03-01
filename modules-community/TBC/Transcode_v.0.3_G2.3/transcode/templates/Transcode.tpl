{*
 * $Revision:$
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock">
  <h3> {g->text text="Transcode"} </h3>
  {if isset($form.error.mime.invalid)}
  <div class="giError">
    {g->text text="Mimetype not enabled for transcoding."}
  </div>
  {/if}
  <p class="giDescription">
    {g->text text="Select transcoding settings."}
  </p>
<table class="gbDataTable">
  <tr>
    <td colspan="4">
      <input type="checkbox"{if $form.enabled} checked="checked"{/if}
       name="{g->formVar var="form[enabled]"}"/>
      {g->text text="Enable transcoding"}
    </td>
  </tr>
  <tr>
    <td>
    </td>
    <td>
      {g->text text="Native"}
    </td>
    <td>
      {g->text text="Transcoded"}
    </td>
    <td>
    </td>
  </tr>
{* TODO: Allow user to select new format *}
  {if isset($form.hasVideo)}
  <tr>
    <td>
      {g->text text="Width"}
    </td>
    <td>
      {g->text text=$form.native.width}
    </td>
    <td>
      <input id="form[encode][width]" type="text" size="5"
         name="{g->formVar var="form[encode][width]"}" value="{$form.encode.width}"
         onkeyup="var el=document.getElementById('form[encode][height]');el.value=Math.round(this.value/{$aspect});"/>
    </td>
    <td>
        <div id="widthWarning" class="giWarning" style="display:{$widthWarningDisplay};">
            {g->text text="Maximum width: "}{$maxWidth}
        </div>
    </td>
  </tr>

  {if isset($form.error.width.invalid)}
  <tr>
    <td colspan="4" class="giError">
      {g->text text="You must enter an even value greater than 0"}
    </td>
  </tr>
  {/if}

  <tr>
    <td>
      {g->text text="Height"}
    </td>
    <td>
      {g->text text=$form.native.height}
    </td>
    <td>
      <input id="form[encode][height]" type="text" size="5" name="{g->formVar var="form[encode][height]"}" value="{$form.encode.height}"
          onkeyup="var el=document.getElementById('form[encode][width]');el.value=Math.round(this.value*{$aspect});"/>
    </td>
    <td>
        <div id="heightWarning" class="giWarning" style="display:{$heightWarningDisplay};">
            {g->text text="Maximum height: "}{$maxHeight}
        </div>
    </td>
  </tr>

  {if isset($form.error.height.invalid)}
  <tr>
    <td colspan="4" class="giError">
    {g->text text="You must enter an even value greater than 0"}
    </td>
  </tr>
  {/if}

  <tr>
    <td>
      {g->text text="Frames / Second"}
    </td>
    <td>
      {g->text text=$form.native.fps}
    </td>
    <td>
      <input type="text" size="5" name="{g->formVar var="form[encode][fps]"}" value="{$form.encode.fps}"/>
    </td>
    <td>
    </td>
  </tr>

  {if isset($form.error.fps.invalid)}
  <tr>
    <td colspan="4" class="giError">
      {g->text text="You must enter a value greater than 0"}
    </td>
  </tr>
  {/if}

  {/if}
  <tr>
    <td>
      {g->text text="Video bitrate  (kb/s)"}
    </td>
    <td>{g->text text=$form.native.bitrate}</td>
    <td>
        <select name="{g->formVar var="form[encode][bitrate]"}">
          <option value="{$form.native.bitrate}" {if ($form.encode.bitrate==$form.native.bitrate)} selected{/if}>{g->text text="Native"}</option>
          <option value="150"{if ($form.encode.bitrate==150)} selected{/if}>{g->text text="Low (150kbps)"}</option>
          <option value="400"{if ($form.encode.bitrate==400)} selected{/if}>{g->text text="Med (400kbps)"}</option>
          <option value="700"{if ($form.encode.bitrate==700)} selected{/if}>{g->text text="High (700kbps)"}</option>
          <option value="900"{if ($form.encode.bitrate==900)} selected{/if}>{g->text text="HD Low (900kbps)"}</option>
          <option value="1200"{if ($form.encode.bitrate==1200)} selected{/if}>{g->text text="HD High (1200kbps)"}</option>
        </select>
    </td>
    <td>
    </td>
  </tr>

  {if isset($form.hasAudio)}
  <tr>
    <td>
      {g->text text="Audio Channels"}
    </td>
    <td>
      {g->text text=$form.native.achans}
    </td>
      {if isset($form.warn.achans)}
    <td class="giError">
        {g->text text="Audio support not available, video will be silent."}
      {else}
    <td>
        <select name="{g->formVar var="form[encode][achans]"}">
          <option value="0"{if ($form.encode.achans==0)} selected{/if}>Mute</option>
          <option value="1"{if ($form.encode.achans==1)} selected{/if}>Mono</option>
          <option value="2"{if ($form.encode.achans==2)} selected{/if}>Stereo</option>
        </select>
      {/if}
    </td>
      {if isset($form.warn.downmix)}
    <td class="giError">
        {g->text text="Downmixing audio channels may not be supported.  Resulting video may be silent."}
      {else}
    <td>
      {/if}
    </td>
  </tr>

  {if isset($form.error.achans.invalid)}
  <tr>
    <td colspan="4" class="giError">
    {g->text text="You must enter Mute, Mono, or Stereo."}
    </td>
  </tr>
  {/if}
   <tr>
    <td>
      {g->text text="Audio Sample Rate (Hz)"}
    </td>
    <td>
      {g->text text=$form.native.asr}
    </td>
      {if isset($form.warn.asr)}
    <td class="giError">
        {g->text text="Audio support not available, video will be silent."}
      {else}
        <td>
        <select name="{g->formVar var="form[encode][asr]"}">
          <option value="11025"{if ($form.encode.asr==11025)} selected{/if}>11025</option>
          <option value="22050"{if ($form.encode.asr==22050)} selected{/if}>22050</option>
          <option value="44100"{if ($form.encode.asr==44100)} selected{/if}>44100</option>
        </select>
      {/if}
    </td>
    <td>
    </td>
  </tr>
   <tr>
    <td>
      {g->text text="Audio Bitrate (kb/s)"}
    </td><td>
      {g->text text="n/a"}
    </td>
      {if isset($form.warn.asr)}
        <td class="giError">
        {g->text text="Audio support not available, video will be silent."}
      {else}
        <td>
        <select name="{g->formVar var="form[encode][audiobitrate]"}">
          <option value="64">64</option>
          <option value="96">96</option>
          <option value="112">112</option>
          <option value="128" selected>128</option>
        </select>
      {/if}
    </td>
    <td>
    </td>
  </tr>

  {if isset($form.error.asr.invalid)}
  <tr><td colspan="4" class="giError">
    {g->text text="You must enter a value of either 11025, 22050, or 44100."}
  </td></tr>
  {/if}

  {/if}
  <tr>
    <td>
      {g->text text="File size (bytes)"}
    </td><td>
      {$form.native.size}
    </td>
    <td>
      {if ($form.encoded.size>0)}
        {$form.encoded.size}
      {else}
        {g->text text="To be determined."}
      {/if}
    </td>
    <td>
    </td>
  </tr>
  <tr>
    <td colspan="4">
      <input type="checkbox"{if $form.sameq} checked="checked"{/if}
       name="{g->formVar var="form[sameq]"}"/>
      {g->text text="Preserve original quality."}
    </td>
  </tr>
  <tr>
    <td colspan="4">
      <input type="checkbox"{if $form.onDemand} checked="checked"{/if}
       name="{g->formVar var="form[onDemand]"}"/>
      {g->text text="Wait to transcode when requested."}
    </td>
  </tr>
</table>
</div>
  <script type="text/javascript" src="{g->url href='lib/yui/container-min.js'}"></script>
  {literal}
  <script type="text/javascript">
    // <![CDATA[

    // Initialize the temporary Panel to display while waiting for external content to load
    YAHOO.namespace("example.container");
    YAHOO.example.container.wait =
        new YAHOO.widget.Panel("wait",
            { width:"240px",
              fixedcenter:true,
              close:false,
              draggable:false,
              zindex:4,
              modal:true,
              visible:false
            }
        );

    YAHOO.example.container.wait.setHeader("Processing, please wait...");
    YAHOO.example.container.wait.setBody('<img src="http://us.i1.yimg.com/us.yimg.com/i/us/per/gr/gp/rel_interstitial_loading.gif" />');
    YAHOO.example.container.wait.render(document.body);

      // ]]>
  </script>
  {/literal}
<div class="gbBlock gcBackground1">
  <input type="hidden" name="{g->formVar var="form[videoDerivativeId]"}" value="{$form.videoDerivativeId}"/>
  <input type="submit" class="inputTypeSubmit" onclick="YAHOO.example.container.wait.show();"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
</div>
