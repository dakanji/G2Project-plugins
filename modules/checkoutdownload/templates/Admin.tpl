{*
 * $Revision: 1664 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
 
 <div class="gbBlock gcBackground1">
  <h2> {g->text text="Checkout Downloadable Items Settings"} </h2>
</div>


{if !empty($status)}
<div class="gbBlock"><h2 class="giSuccess">
{if isset($status.saved)}
{g->text text="Settings saved successfully"}
{/if}
</h2></div>
{/if}

<div class="gbBlock">
  <h3> {g->text text="Products"} </h3>
      <p class="giDescription">
      {g->text text="To enable a photo to be downloaded after purchase, tick the box."}
      </p>
      <p class="giDescription">
      {g->text text="Downloads will be resized to a maximum dimension that you enter. To download the original size, leave the size box empty."}
   {*   {g->text text="Advanced users can enter a Gallery-style operation string to further alter the image that's downloaded."} *}
      </p>
        <table class="gbDataTable">
      	<tr>
      	<th align="center">{g->text text="Product Name"}</th>
        <th align="center">{g->text text="Downloadable"}</th>
        <th align="center">{g->text text="Download Size (pixels)"}</th>
      {*  <th align="center">{g->text text="Extra Operation String"}</th> *}
        </tr>
        	
          	{foreach from=$form.products item=product }
	<tr class="{cycle values="gbEven,gbOdd"}">
	<td>{$product.name}</td>
	<input type="hidden" name = "{g->formVar var="form[products][`$product.id`][exists]"}" value = "true">
	<td align="center"><input type="checkbox" name = "{g->formVar var="form[products][`$product.id`][download]"}" value = "true" {if $product.download == "true" }checked{/if}></td>
	<td align="center"><input type="text" size ="5" name = "{g->formVar var="form[products][`$product.id`][downloadSize]"}" value = "{$product.downloadSize}"></td>
{*	<td align="center"><input type="text" size = "80" name = "{g->formVar var="form[products][`$product.id`][downloadOperation]"}" value = "{$product.downloadOperation}"></td> *}
	</tr>
  		{/foreach}
   	  </table>
</div>

<div class="gbBlock">
	<h3>{g->text text="Download Options"}</h3>
      <p class="giDescription">
        {g->text text="Optionally users may be permitted to download items in their cart before payment is cleared."}
      </p>
      <table class="gbDataTable">
      <tr>
      <td width="50%">{g->text text="Allow download before payment cleared?"}</td>
      <td><input type="checkbox" name="{g->formVar var="form[allowDownloadsPrePurchase]"}"{if $form.allowDownloadsPrePurchase} checked="checked"{/if}/></td>
      </tr>
      </table>
	<p class="giDescription">
        {g->text text="You may optionally set your own filename to use for zipped file downloads - don't include the .zip ending - and/or have the date and time appended using the YYYMMDDHHMMSS format to make each filename unique. (Example: photos-20101231235959.zip)"}
      </p>
	<table class="gbDataTable">
      <tr>
      <td width="50%">{g->text text="Base filename"}</td>
      <td><input type="text" name="{g->formVar var="form[zipfilename]"}" value="{$form.zipfilename}"/></td>
      </tr>
      <tr>
      <td width="50%">{g->text text="Append date & time?"}</td>
      <td><input type="checkbox" name="{g->formVar var="form[useUniqueDLFilename]"}"{if $form.useUniqueDLFilename} checked="checked"{/if}/></td>
      </tr>
      </table> 
      <p class="giDescription">
     {g->text text="Download attempts can be limited in time and in number."}
     </p>
     <table class="gbDataTable">
      <tr>
      <td width="50%">{g->text text="Set time limit on downloads?"}</td>
      <td><input type="checkbox" name="{g->formVar var="form[limitDaysDLAvailable]"}"{if $form.limitDaysDLAvailable} checked="checked"{/if}/> 
      </tr>
      <tr>
      <td width="50%">{g->text text="Time Limit in days"}</td>
      <td><input type="text" name="{g->formVar var="form[daysDLAvailable]"}" value="{$form.daysDLAvailable}" size="3"/></td>
      </tr>
      <tr>
      <td width="50%">{g->text text="Limit download attempts?"}</td>
      <td><input type="checkbox" name="{g->formVar var="form[limitNumDLsAllowed]"}"{if $form.limitNumDLsAllowed} checked="checked"{/if}/></td>
      </tr>
      <tr>
      <td width="50%">{g->text text="Maximum attempts"}</td>
      <td><input type="text" name="{g->formVar var="form[numDLsAllowed]"}" value="{$form.numDLsAllowed}" size="3"/></td>
      </tr>
      </table>
     <p class="giDescription">
     {g->text text="Download images can be placed in nested sub-folders matching the album names and hierarchy they apear in in the Gallery."}
     </p>
     <table class="gbDataTable">
     <tr>
     <td width="50%">{g->text text="Download in nested folders?"}</td>
     <td><input type="checkbox" name="{g->formVar var="form[useFoldersInDL]"}"{if $form.useFoldersInDL} checked="checked"{/if}/> </td>
     </tr>
     </table>
</div>
      
<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][save]"}"
     value="{g->text text="Save Settings"}"/>
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][cancel]"}"
     value="{g->text text="Cancel"}"/>
  </div>
</div>

