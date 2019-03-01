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
  <tr>
    <td>
      {g->text text="Audio Channels"}
    </td>
    <td>
      {g->text text=$form.native.achans}
    </td>
    <td>
        <select name="{g->formVar var="form[encode][achans]"}">
          <option value="0"{if ($form.encode.achans==0)} selected{/if}>Mute</option>
          <option value="1"{if ($form.encode.achans==1)} selected{/if}>Mono</option>
          <option value="2"{if ($form.encode.achans==2)} selected{/if}>Stereo</option>
        </select>
    </td>
    <td>
    </td>
  </tr>
  <tr>
    <td>
      {g->text text="Audio Sample Rate (Hz)"}
    </td>
    <td>
      {g->text text=$form.native.asr}
    </td>
    <td>
        <select name="{g->formVar var="form[encode][asr]"}">
          <option value="11025"{if ($form.encode.asr==11025)} selected{/if}>11025</option>
          <option value="22050"{if ($form.encode.asr==22050)} selected{/if}>22050</option>
          <option value="44100"{if ($form.encode.asr==44100)} selected{/if}>44100</option>
        </select>
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
        <td>
        <select name="{g->formVar var="form[encode][audiobitrate]"}">
          <option value="64">64</option>
          <option value="96">96</option>
          <option value="112">112</option>
          <option value="128">128</option>
          <option value="160">160</option>
          <option value="192" selected>192</option>
          <option value="320">320</option>
        </select>
    </td>
    <td>
    </td>
  </tr>
  <tr>
    <td>
      {g->text text="MIME Type"}
    </td>
    <td>
      {$form.native.mime}
    </td>
    <td>
        <select name="{g->formVar var="form[outputMime]"}">
    {foreach from=$outputOptions item=option}
    <option value="{$option}"{if ($option==$form.encoded.mime)} selected="selected"{/if}>{g->text text="$option"}</option>
    {/foreach}
      </select>
    </td>
  </tr>
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
