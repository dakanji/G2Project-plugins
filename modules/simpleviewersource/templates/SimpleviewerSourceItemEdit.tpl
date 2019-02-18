{*
 * $Revision: 1433 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Simpleviewer Source - XML Feed location"} </h2>
</div>

 
<div class="gbBlock">
  <h3> {g->text text="XML Generator URL"} </h3>
  <p class="giDescription">
    {g->text text="Use the following XML source path to display this album in Simpleviewer by, for instance, copying and pasting it into the HTML file where Simpleviewer is loaded:"}
    </p>
    <p class="giDescription">
    <b>{$XMLurl}</b>
    </p>
</div>

<div class="gbBlock gcBackground1">
  <h2> {g->text text="Simpleviewer Source - Album Settings"} </h2>
</div>

 <p class="giDescription">
    {g->text text="Name/Value pairs set in this section apply to this album and its children. New parameters can be entered in the blank boxes."}
    {g->text text="If a parameter is set, you have the option to delete (unset) it. It may still be applied by having been set in a parent album."}
 </p>

<div class="gbBlock">
  <table class="gbDataTable">
    <tr>
     <td colspan = "3">
	<h3>Module Settings</h3>
     </td>
   </tr>
   <th>{g->text text="Parameter Name"}</th><th>{g->text text="Parameter Value"}</th><th>{g->text text="Unset"}</th>
   <tr>
     <td colspan="3"> 
     <br>  
     {g->text text="Enable (or disable) simpleviewer from displaying the pictures in this album and its sub-albums."}
     <br>
     </td>
   </tr>   	
   <tr>
     <td><b>enabled</b><input type="hidden" name="{g->formVar var="form[private][0][id]"}" value="{$form.private.enabled.id}">
     		<input type="hidden" name="{g->formVar var="form[private][0][paramName]"}" value="enabled"></td>
     <td><input type="radio" name="{g->formVar var="form[private][0][paramValue]"}" value="1" {if $form.private.enabled.id && $form.private.enabled.paramValue eq '1'}checked{/if}> Yes 
         <input type="radio" name="{g->formVar var="form[private][0][paramValue]"}" value="0" {if $form.private.enabled.id && $form.private.enabled.paramValue eq '0'}checked{/if}> No
	<input type="hidden" name="{g->formVar var="form[private][0][category]"}" value="private"></td>
     <td>{if $form.private.enabled.id}<input type="checkbox" name="{g->formVar var="form[private][0][delete]"}">{else}{/if}</td>
   </tr>
   <tr>
     <td colspan="3">
     <br>  
     {g->text text="The performance of Simpleviewer is affected by loading too many large images. You can limit it to showing only a specified number of images from this album here."}
     {g->text text="A value of 0 will load all the images in the album."}
     </td>
   <tr>
     <td><b>maxItems</b><input type="hidden" name="{g->formVar var="form[private][1][id]"}" value="{$form.private.maxItems.id}">
     		<input type="hidden" name="{g->formVar var="form[private][1][paramName]"}" value="maxItems"></td>
     <td><input type="text" name="{g->formVar var="form[private][1][paramValue]"}" value="{$form.private.maxItems.paramValue}">
	<input type="hidden" name="{g->formVar var="form[private][1][category]"}" value="private"></td>
     <td>{if $form.private.maxItems.id}<input type="checkbox" name="{g->formVar var="form[private][1][delete]"}">{else}{/if}</td>
   </tr>

   
   <tr>
     <td colspan="3"> 
     <br>  
     {g->text text="Enable (or disable) the slideshow menu item for this album and its sub-albums."}
     <br>
     </td>
   </tr>   	
   <tr>
     <td><b>offerSlideshow</b><input type="hidden" name="{g->formVar var="form[private][2][id]"}" value="{$form.private.offerSlideshow.id}">
     		<input type="hidden" name="{g->formVar var="form[private][2][paramName]"}" value="offerSlideshow"></td>
     <td><input type="radio" name="{g->formVar var="form[private][2][paramValue]"}" value="1" {if $form.private.offerSlideshow.id && $form.private.offerSlideshow.paramValue eq '1'}checked{/if}> Yes 
         <input type="radio" name="{g->formVar var="form[private][2][paramValue]"}" value="0" {if $form.private.offerSlideshow.id && $form.private.offerSlideshow.paramValue eq '0'}checked{/if}> No
	<input type="hidden" name="{g->formVar var="form[private][2][category]"}" value="private"></td>
     <td>{if $form.private.offerSlideshow.id}<input type="checkbox" name="{g->formVar var="form[private][2][delete]"}">{else}{/if}</td>
   </tr>
   
   <tr>
     <td colspan="3">
       <h3Flash Parameters included in the html</h3>
       <p class="giDescription">
        {g->text text="These parameters are included in the html Simpleviewer loader if loaded through the Gallery link. If you use Simpleviewer externally they will have no effect, and you'll need to adjust your html file with the required values instead. "}
 	</p>

     </td>
   </tr>
   
  <tr>
     <td><b>width</b><input type="hidden" name="{g->formVar var="form[private][3][id]"}" value="{$form.private.width.id}">
     		<input type="hidden" name="{g->formVar var="form[private][3][paramName]"}" value="width"></td>
     <td><input type="text" name="{g->formVar var="form[private][3][paramValue]"}" value="{$form.private.width.paramValue}">
	<input type="hidden" name="{g->formVar var="form[private][3][category]"}" value="private"></td>
     <td>{if $form.private.width.id}<input type="checkbox" name="{g->formVar var="form[private][3][delete]"}">{else}{/if}</td>
   </tr>

  <tr>
     <td><b>height</b><input type="hidden" name="{g->formVar var="form[private][4][id]"}" value="{$form.private.height.id}">
     		<input type="hidden" name="{g->formVar var="form[private][4][paramName]"}" value="height"></td>
     <td><input type="text" name="{g->formVar var="form[private][4][paramValue]"}" value="{$form.private.height.paramValue}">
	<input type="hidden" name="{g->formVar var="form[private][4][category]"}" value="private"></td>
     <td>{if $form.private.height.id}<input type="checkbox" name="{g->formVar var="form[private][4][delete]"}">{else}{/if}</td>
   </tr>

  <tr>
     <td><b>backgroundColor</b><input type="hidden" name="{g->formVar var="form[private][5][id]"}" value="{$form.private.backgroundColor.id}">
     		<input type="hidden" name="{g->formVar var="form[private][5][paramName]"}" value="backgroundColor"></td>
     <td><input type="text" name="{g->formVar var="form[private][5][paramValue]"}" value="{$form.private.backgroundColor.paramValue}">
	<input type="hidden" name="{g->formVar var="form[private][5][category]"}" value="private"></td>
     <td>{if $form.private.backgroundColor.id}<input type="checkbox" name="{g->formVar var="form[private][5][delete]"}">{else}{/if}</td>
   </tr>

  <tr>
     <td><b>preloaderColor</b><input type="hidden" name="{g->formVar var="form[private][6][id]"}" value="{$form.private.preloaderColor.id}">
     		<input type="hidden" name="{g->formVar var="form[private][6][paramName]"}" value="preloaderColor"></td>
     <td><input type="text" name="{g->formVar var="form[private][6][paramValue]"}" value="{$form.private.preloaderColor.paramValue}">
	<input type="hidden" name="{g->formVar var="form[private][6][category]"}" value="private"></td>
     <td>{if $form.private.preloaderColor.id}<input type="checkbox" name="{g->formVar var="form[private][6][delete]"}">{else}{/if}</td>
   </tr>

   <tr>
     <td colspan="3">
       <h3>Simpleviewer Parameters</h3>
       <p class="giDescription">
        {g->text text="These parameters are forwarded directly to Simpleviewer."}
    {g->text text="More information about the syntax and range of parameters can be found %shere%s, at the Airtight Interactive website." arg1="<a href='http://www.airtightinteractive.com/simpleviewer/xml_options.html'>" arg2="</a>"}
 	</p>

     </td>
    </tr>
   	<th>{g->text text="Parameter Name"}</th><th>{g->text text="Parameter Value"}</th><th>{g->text text="Unset"}</th>
  {foreach name=x key=key item=item from=$form.fixed}
	<tr>
	<td><input type="hidden" name="{g->formVar var="form[fixed][`$smarty.foreach.x.iteration`][id]"}" value="{$item.id}">
	<b>{$key}</b><input type="hidden" name="{g->formVar var="form[fixed][`$smarty.foreach.x.iteration`][paramName]"}" value="{$key}"></td>
	<td><input type="text" name="{g->formVar var="form[fixed][`$smarty.foreach.x.iteration`][paramValue]"}" value="{$item.paramValue}">
	<input type="hidden" name="{g->formVar var="form[fixed][`$smarty.foreach.x.iteration`][category]"}" value="fixed"></td>
	<td>{if $item.id}<input type="checkbox" name="{g->formVar var="form[fixed][`$smarty.foreach.x.iteration`][delete]"}">{/if}</td>

	</tr>
  {/foreach}
  	<tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
  	<th>{g->text text="Custom Parameters"}</th><th>{g->text text="Parameter Value"}</th><th>{g->text text="Unset"}</th>
  {section name=x loop=$form.custom}
	<tr>
	<td><input type="hidden" name="{g->formVar var="form[custom][`$smarty.section.x.index`][id]"}" value="{$form.custom[x].id}">
	{if $form.custom[x].id || !$form.custom[x].paramName}
	<input type="text" name="{g->formVar var="form[custom][`$smarty.section.x.index`][paramName]"}" value="{$form.custom[x].paramName}">
	{else}
	<input type="hidden" name="{g->formVar var="form[custom][`$smarty.section.x.index`][paramName]"}" value="{$form.custom[x].paramName}">
	<b>{$form.custom[x].paramName}</b>
	{/if}
	</td>
	<td><input type="text" name="{g->formVar var="form[custom][`$smarty.section.x.index`][paramValue]"}" value="{$form.custom[x].paramValue}">
	<input type="hidden" name="{g->formVar var="form[custom][`$smarty.section.x.index`][category]"}" value=""></td>
	<td>{if $form.custom[x].id}<input type="checkbox" name="{g->formVar var="form[custom][`$smarty.section.x.index`][delete]"}">{/if}</td>
	</tr>
  {/section}
  </table>

  <br><br>
  
  <div class="gbButtons">
    <input type="submit" name="{g->formVar var="form[action][save]"}"
     value="{g->text text="Save Settings"}"/>
    <input type="submit" name="{g->formVar var="form[action][cancel]"}"
     value="{g->text text="Cancel"}"/>
  </div>
</div>


