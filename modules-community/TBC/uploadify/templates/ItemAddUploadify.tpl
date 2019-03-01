{*
 * $Revision: 17486 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}

<script type="text/javascript">
    // <![CDATA[
jQuery(document).ready(function() {ldelim}

    jQuery("#uploadify").uploadify({ldelim}
        'uploader'       : '{g->url href="modules/uploadify/lib/uploadify.swf"}',
        'script'         : '{$ItemAddUploadify.submitUrl}',
        'cancelImg'      : '{g->url href="modules/uploadify/lib/cancel.png"}',
        'buttonImg'      : '{g->url href="modules/uploadify/lib/browseBtnFlash.png"}',
        'width'          : 82,
        'height'         : 24,
        'rollover'       : true,
        'queueID'        : 'fileQueue',
        'fileDataName'   : 'g2_userfile',
        'auto'           : false,
        'multi'          : true,
        'simUploadLimit' : 4,
        'buttonText'     : '{g->text text="Select Media"}',
        {literal}onError: function (event, queueID ,fileObj, errorObj) {
            var msg;
            if (errorObj.status == 404) {
                alert('Could not find upload script.');
                msg = 'Could not find upload script.';
            } else if (errorObj.type === "HTTP")
                msg = errorObj.type+": Upload Failed";
            else if (errorObj.type ==="File Size")
                msg = fileObj.name+'<br>'+errorObj.type+' Limit: '+Math.round(errorObj.sizeLimit/1024)+'KB';
            else
                msg = errorObj.type+": "+errorObj.text;
            jQuery.jGrowl('<p></p>'+msg, {
                theme:  'error',
                header: 'ERROR',
                sticky: true
            });
            jQuery("#uploadify" + queueID).fadeOut(250, function() { jQuery("#uploadify" + queueID).remove()});
            return false;
        },
        onCancel: function (a, b, c, d) {
            var msg = "Cancelled uploading: "+c.name;
            jQuery.jGrowl('<p></p>'+msg, {
                theme:  'warning',
                header: 'Cancelled Upload',
                life:   4000,
                sticky: false
            });
        },
        onClearQueue: function (a, b) {
            var msg = "Cleared "+b.fileCount+" files from queue";
            jQuery.jGrowl('<p></p>'+msg, {
                theme:  'warning',
                header: 'Cleared Queue',
                life:   4000,
                sticky: false
            });
        },
        onComplete: function (a, b ,c, d, e) {
            var size = Math.round(c.size/1024);
            jQuery.jGrowl('<p></p>'+c.name+' - '+size+'KB', {
                theme:  'success',
                header: 'Upload Complete',
                life:   4000,
                sticky: false
            });
        }{/literal}
    {rdelim});
{rdelim});
{literal}
function startUpload(id){
    var $inputs = jQuery('#uploadOptions :input');
    var values = {};
    $inputs.each(function() {
        if(jQuery(this).attr('checked')){
            values[this.name] = 1;
        }else{
            values[this.name] = this.value;
        }
    });
    jQuery('#uploadify').uploadifySettings('scriptData', values);
    jQuery('#uploadify').uploadifyUpload();
}
{/literal}
  // ]]>
</script>

<div class="gbBlock">
    <div id="fileQueue"></div>
    <input type="file" name="uploadify" id="uploadify" />
    <input class="uploadifyButton" type="button" onclick="startUpload('uploadify');" value="{g->text text='Upload'}"/>
    <input class="uploadifyButton" type="button" onclick="jQuery('#uploadify').uploadifyClearQueue();" value="{g->text text='Clear'}"/>
</div>
</form>
<form id="uploadOptions">
<div class="gbBlock">
    <p class="giDescription">
      {g->text text="Copy base filenames to:"}
      <br/>
      <input type="checkbox" id="cbTitle" {if $form.set.title} checked="checked" {/if}
       name="{g->formVar var="form[set][title]"}"/>
      <label for="cbTitle"> {g->text text="Title"} </label>
      &nbsp;

      <input type="checkbox" id="cbSummary" {if $form.set.summary} checked="checked" {/if}
       name="{g->formVar var="form[set][summary]"}"/>
      <label for="cbSummary"> {g->text text="Summary"} </label>
      &nbsp;

      <input type="checkbox" id="cbDescription" {if $form.set.description} checked="checked" {/if}
       name="{g->formVar var="form[set][description]"}"/>
      <label for="cbDescription"> {g->text text="Description"} </label>
    </p>
</div>

{* Include our extra ItemAddOptions *}
{foreach from=$ItemAdd.options item=option}
  {include file="gallery:`$option.file`" l10Domain=$option.l10Domain}
{/foreach}
<div class="gbBlock">
  <h3> {g->text text="Resizes"} </h3>

  <p class="giDescription">
    {g->text text="Have gallery create resizes at upload time."}
  </p>

  <input type="checkbox" id="BuildResizes_cb" name="{g->formVar var="form[buildResizes]"}"/>
  <label for="BuildResizes_cb">
    {g->text text="Create resizes now"}
  </label>
</div>
