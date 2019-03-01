{*
 * $Revision: 17503 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}

<script type="text/javascript" src="{g->url href='modules/multimedia/lib/picker.js' forceFullUrl=true}"></script>
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Multimedia Settings"} </h2>
</div>


<div class="gbBlock">
  <p>
  <table class="gbDataTable">
  <tr>
    <td colspan="2">
      {g->text text="There are two colors used in the flash player's background gradiant."}
    </td>
  </tr>
  <tr>
    <td>{g->text text="Color1"}</td>
    <td>
      <input type="text" size="6" id="color1" name="{g->formVar var="form[color1]"}" value="{$form.color1}"/>
      <a href="javascript:TCP.popup(document.getElementById('color1'), '{g->url href='modules/multimedia/lib/' forceFullUrl=true}')">
      <img width="16" height="16" border="0" alt="Color1"
        title="Color1" src="{g->url href='modules/multimedia/lib/cp.png' forceFullUrl=true}"></a>
      <label for="color1"> {g->text text="Select color1"} </label>
    </td>
  </tr>
    <td>{g->text text="Color2"}</td>
    <td>
      <input type="text" size="6" id="color2" name="{g->formVar var="form[color2]"}" value="{$form.color2}"/>
      <a href="javascript:TCP.popup(document.getElementById('color2'), '{g->url href='modules/multimedia/lib/' forceFullUrl=true}')">
      <img width="16" height="16" border="0" alt="Color2"
        title="Color2" src="{g->url href='modules/multimedia/lib/cp.png' forceFullUrl=true}"></a>
      <label for="color2"> {g->text text="Select color2"} </label>
    </td>
  </tr>
  <tr>
    <td>{g->text text="Fullscreen background color"}</td>
    <td>
      <input type="text" size="6" id="color4" name="{g->formVar var="form[color4]"}" value="{$form.color4}"/>
      <a href="javascript:TCP.popup(document.getElementById('color4'), '{g->url href='modules/multimedia/lib/' forceFullUrl=true}')">
      <img width="16" height="16" border="0" alt="Fullscreen background color"
        title="Fullscreen background color" src="{g->url href='modules/multimedia/lib/cp.png' forceFullUrl=true}"></a>
      <label for="color2"> {g->text text="Select fullscreen background color"} </label>
    </td>
  </tr>
  <tr>
    <td>{g->text text="Allow download"}</td>
    <td>
      <input type="checkbox" id="cbAllowDownload"{if $form.allowDownload} checked="checked"{/if}
       name="{g->formVar var="form[allowDownload]"}"/>
      <label for="cbAllowDownload"> {g->text text="Show the download button?"} </label>
    </td>
  </tr>
  <tr>
    <td>{g->text text="Allow Sharing"}</td>
    <td>
      <input type="checkbox" id="cbShare"{if $form.share} checked="checked"{/if}
       name="{g->formVar var="form[share]"}"/>
      <label for="cbShare"> {g->text text="Show the share button?"} </label>
    </td>
  </tr>
  <tr>
    <td>{g->text text="Allow fullscreen"}</td>
    <td>
      <input type="checkbox" id="cbAllowFullscreen"{if $form.allowFullscreen} checked="checked"{/if}
       name="{g->formVar var="form[allowFullscreen]"}"/>
      <label for="cbAllowFullscreen"> {g->text text="Show the fullscreen button?"} </label>
    </td>
  </tr>
  <tr>
    <td>{g->text text="Autostart"}</td>
    <td>
      <input type="checkbox" id="cbAutoStart"{if $form.autoStart} checked="checked"{/if}
       name="{g->formVar var="form[autoStart]"}"/>
      <label for="cbAutoStart"> {g->text text="Play the media immediately?"} </label>
    </td>
  </tr>
  </table>
  {if !empty($status.saved)}
  <h2 class="giSuccess">{g->text text="Settings saved"}</h2>
  {/if}
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  </p>
</div>
