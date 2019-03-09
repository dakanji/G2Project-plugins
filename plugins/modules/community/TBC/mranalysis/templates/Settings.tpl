{*
 * $RCSfile: Settings.tpl,v $
 * $Revision: 1811 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Analysis Settings"} </h2>
</div>

{if !empty($status)}
  {if isset($status.saved)}
  <div class="gbBlock"><h2 class="giSuccess">
    {g->text text="Settings saved successfully"}
  </h2></div>
  {/if}
{/if}

<div class="gbBlock">
    <div class="gbDataEntry">
      <h3 class="giTitle">{g->text text="Broken Images"}</h3>
      <input type="checkbox" name="{g->formVar var="form[TreatZeroSizedDerivativesAsBroken]"}" {if $form.TreatZeroSizedDerivativesAsBroken}checked="checked"{/if} value="1">
      {g->text text="Treat Zero Sized Derivatives as Broken."}
      </input>
      <br>
      <br>
      <h3 class="giTitle">{g->text text="Duplicate Titles"}</h3>
      <input type="checkbox" name="{g->formVar var="form[CheckForDuplicateTitlesAcrossAlbums]"}" {if $form.CheckForDuplicateTitlesAcrossAlbums}checked="checked"{/if} value="1">
      {g->text text="Expand check for duplicate titles across albums."}
      </input>
      <br>


    </div>
  </p>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
</div>
