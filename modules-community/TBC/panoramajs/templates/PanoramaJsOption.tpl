{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <h3> {g->text text="Panorama (Js)"} </h3>

  <p class="giDescription">
    {g->text text="Note that panorama (Js) view only applies to full size photo, not resizes."}
  </p>

  <input type="checkbox" id="PanoramaJs_cb"{if $form.PanoramaJsOption.isPanoramaJs} checked="checked"{/if}
   name="{g->formVar var="form[PanoramaJsOption][isPanoramaJs]"}"/>
  <label for="PanoramaJs_cb">
    {g->text text="Activate panorama (Js) viewer for this photo"}
  </label>
</div>
