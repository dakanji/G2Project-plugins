{*
 * $RCSfile: Admin.tpl,v $
 * $Revision: 1874 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}


<div class="gbBlock gcBackground1">
  <h2 class="gbTitle"> {g->text text="About Page Settings"} </h2>
</div>

  {if !empty($status)}
  <div id="gsStatus">
    {if isset($status.saved)}
    <div class="giStatus">
<br>
      {g->text text="Settings saved successfully"}
<br>
    </div>
    {/if}
  </div>
  {/if}

  <div class="gbAdmin">
<br>
    <div class="gbDataEntry">
      <h3 class="giTitle">
	{g->text text="User Message"}
      </h3>

    {include file="gallery:modules/core/templates/MarkupBar.tpl" viewL10domain="modules_core"
	     element="message" firstMarkupBar=true}

      <textarea id="message" rows="10" cols="80" name="{g->formVar var="form[message]"}">{if isset($form.message)}{$form.message}{/if}</textarea>
	<br><br>
    </div>
  </div>

<div class="gbBlock gcBackground1">
    <input type="submit" name="{g->formVar var="form[action][save]"}"
     value="{g->text text="Save"}"/>
  </div>

