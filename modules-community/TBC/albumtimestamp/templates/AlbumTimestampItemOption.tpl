{*
 * $RCSfile: AlbumTimestampItemOption.tpl,v $
 * $Revision:  $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock">
  <h3> {g->text text="Automatically set Album Time"} </h3>

  <p class="giDescription">
    {g->text text="Automatically set the time of this album based on the times of its images."}
  </p>

  <select name="{g->formVar var="form[AlbumTimestampItemOption][mode]"}">
    {html_options options=$AlbumTimestampItemOption.settingsList selected=$form.AlbumTimestampItemOption.mode}
  </select>

  <p>
    Setting for new Sub-Albums:
  </p>
  <select name="{g->formVar var="form[AlbumTimestampItemOption][newAlbumMode]"}">
    {html_options options=$AlbumTimestampItemOption.newSettingsList selected=$form.AlbumTimestampItemOption.newAlbumMode}
  </select>

  <p>
    <input type="checkbox" id="AlbumTimestamp_update_cb" name="{g->formVar var="form[AlbumTimestampItemOption][forceUpdate]"}" />
    <label for="AlbumTimestamp_update_cb">
      {g->text text="Force timestamp updates of this album and all of its subalbums now."}
    </label>
  </p>
  <input type="hidden" name="{g->formVar var="form[AlbumTimestampItemOption][oldMode]"}"
     value="{$form.AlbumTimestampItemOption.mode}"/>
</div>
