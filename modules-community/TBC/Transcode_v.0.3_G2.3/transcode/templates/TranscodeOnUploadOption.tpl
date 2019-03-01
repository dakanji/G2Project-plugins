{*
 * $Revision: 16235 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{if $onUpload}
{literal}
<script language="javascript">
	function checkOne(id){
		var checkBoxArray = ['TranscodeNever_cb', 'TranscodeOnUpload_cb', 'TranscodeLater_cb'];
		for( var i=0;i<checkBoxArray.length;i++ ){
			var el = document.getElementById(checkBoxArray[i]);
			if( el != id ){
				el.checked = false;
			}
		}
	}
</script>
{/literal}
<div class="gbBlock">
  <h3> {g->text text="Transcode"} </h3>
  <p class="giDescription">
    {g->text text="Gallery can transcode the following movietypes to $outputMime:"}<br />
    {foreach from=$selectedMimeTypes item=mime}{g->text text="$mime "}{/foreach}<br />
    {g->text text="You can choose to do it now on upload, or wait until first view request."}
    {g->text text="Either way, it will create the new video once and save it, but if you create 
	them at upload time it makes viewing albums for the first time go faster at the expense of a longer upload time."}
  </p>
  <input type="checkbox" id="TranscodeNever_cb" checked="checked" onclick="checkOne(this);"
   name="{g->formVar var="form[TranscodeOnUploadOption][transcodeNever]"}"/>
  <label for="TranscodeNever_cb">
    {g->text text="Don't transcode now, I can do it manually later."}
  </label>
  <input type="checkbox" id="TranscodeOnUpload_cb" onclick="checkOne(this);"
   name="{g->formVar var="form[TranscodeOnUploadOption][transcodeOnUpload]"}"/>
  <label for="TranscodeOnUpload_cb">
    {g->text text="Transcode now"}
  </label>
  <input type="checkbox" id="TranscodeLater_cb" onclick="checkOne(this);"
   name="{g->formVar var="form[TranscodeOnUploadOption][transcodeLater]"}"/>
  <label for="TranscodeLater_cb">
    {g->text text="Transcode later"}
  </label>
</div>
{/if}