{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Panorama (Js) Settings"} </h2>
</div>

{if !empty($status)}
<div class="gbBlock"><h2 class="giSuccess">
  {if isset($status.saved)}
    {g->text text="Settings saved successfully"}
  {/if}
  {if isset($status.reset)}
    {g->text text="Items reset successfully"}
  {/if}
  {if isset($status.deactivated)}
  <span class="giError">
    {g->text text="Reset panorama (Js) items to enable deactivation (see below)"}
  </span>
  {/if}
</h2></div>
{/if}

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="The panorama (Js) viewer can be activated in two ways: the first allows album administrators to select individual images for panorama display (Panorama section in \"edit photo\"), overriding the normal display of the entire image.  The second method retains the normal image display but gives users an option in the \"item actions\" to view the image in the panorama (Js) viewer."}
  </p>
  <p style="line-height: 2.5em; margin-left: 1em">
    <input type="checkbox" id="cbItemType"{if $form.itemType} checked="checked"{/if}
     name="{g->formVar var="form[itemType]"}"/>
    <label for="cbItemType">
      {g->text text="Use panorama (Js) to display wide images"}
    </label>
    <br/>

    <input type="checkbox" id="cbItemLink"{if $form.itemLink} checked="checked"{/if}
     name="{g->formVar var="form[itemLink]"}"/>
    <label for="cbItemLink">
      {g->text text="Add \"view panorama (Js)\" option in item actions for wide images"}
    </label>
    <br/>

    {g->text text="Width of panorama (Js) viewer: "}
    <input type="text" size="6" name="{g->formVar var="form[width]"}" value="{$form.width}"/>
    <br /><br />
    
    <input type="checkbox" id="cbRotate"{if $form.rotate} checked="checked"{/if}
     name="{g->formVar var="form[rotate]"}"/>
     <label for="cbRotate">
        {g->text text="Auto rotate panorama (Js) viewer"}
    </label>
     
    <h4>{g->text text="Speed rotate parameter: "}</h4>
    <br />
    
	<input id="cbSpeed" name="{g->formVar var="form[speed]"}" value="1" type="radio"{if $form.speed == 1} checked="checked"{/if}>
	<label for="cbSpeed">
	1
	</label>
	
	<input id="cbSpeed" name="{g->formVar var="form[speed]"}" value="2" type="radio"{if $form.speed == 2} checked="checked"{/if}>
	<label for="cbSpeed">
	2
	</label>
	
	<input id="cbSpeed" name="{g->formVar var="form[speed]"}" value="3" type="radio"{if $form.speed == 3} checked="checked"{/if}>
	<label for="cbSpeed">
	3
	</label>
	
	<input id="cbSpeed" name="{g->formVar var="form[speed]"}" value="4" type="radio"{if $form.speed == 4} checked="checked"{/if}>
	<label for="cbSpeed">
	4
	</label>
	
	<input id="cbSpeed" name="{g->formVar var="form[speed]"}" value="5" type="radio"{if $form.speed == 5} checked="checked"{/if}>
	<label for="cbSpeed">
	5
	</label>
	<br />

    {if isset($form.error.width)}
    <div class="giError">
      {g->text text="Invalid width value"}
    </div>
    {/if}
  </p>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][undo]"}" value="{g->text text="Reset"}"/>
</div>
