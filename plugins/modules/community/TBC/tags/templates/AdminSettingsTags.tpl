{*
 * $Revision: 1054 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{* Add the Color Chooser DIVs *}
<div id="Markup_colorChooser">
  <div id="Markup_colorHandle">&nbsp;</div>
  <div id="Markup_pickerDiv">
    <img id="Markup_pickerBg" src="{g->url href="modules/core/data/pickerbg.png"}" alt=""/>
    <div id="Markup_selector"><img src="{g->url href="modules/core/data/select.gif"}" alt=""/></div>
  </div>

  <div id="Markup_hueBg">
    <div id="Markup_hueThumb"><img src="{g->url href="modules/core/data/hline.png"}" alt=""/></div>
  </div>

  <div id="Markup_valdiv">
    <br/>
    R <input name="rval" id="Markup_rval" type="text" value="0" size="3" maxlength="3"/>
    H <input name="hval" id="Markup_hval" type="text" value="0" size="3" maxlength="3"/>
    <br/>
    G <input name="gval" id="Markup_gval" type="text" value="0" size="3" maxlength="3"/>
    S <input name="gsal" id="Markup_sval" type="text" value="0" size="3" maxlength="3"/>
    <br/>
    B <input name="bval" id="Markup_bval" type="text" value="0" size="3" maxlength="3"/>
    V <input name="vval" id="Markup_vval" type="text" value="0" size="3" maxlength="3"/>
    <br/>
    <br/>
    # <input name="hexval" id="Markup_hexval" type="text" value="0" size="6" maxlength="6"/>
    <br/>
    <input value="{g->text text="Done"}" class="yui-log-button" style="font-size: 11px;" type="button"
      onclick="ColorChooserUpdate()"/>
  </div>
  <div id="Markup_swatch">&nbsp;</div>
  <div id="Markup_hint">{g->text text="You can also use the %scolor name%s for example: %sYour Text%s" arg1="<a href=\"http://www.w3.org/TR/2002/WD-css3-color-20020418/#html4\" target=\"_new\">" arg2="</a>" arg3="[color=red]" arg4="[/color]"}</div>
</div>


<div class="gbBlock gcBackground1">
  <h2> {g->text text="Tag Settings"} </h2>
</div>

<div class="gbTabBar">
  {if ($tagAdminMode == 'General')}
    <span class="giSelected o"><span>
      {g->text text="General Settings"}
    </span></span>
  {else}
    <span class="o"><span>
	  <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=tags.AdminSettingsTags" arg3="tagAdminMode=General"}">{g->text text="General Settings"}</a>
    </span></span>
  {/if}
  {if ($tagAdminMode == 'TagCloud')}
    <span class="giSelected o"><span>
      {g->text text="Cloud Settings"}
    </span></span>
  {else}
    <span class="o"><span>
        <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=tags.AdminSettingsTags" arg3="tagAdminMode=TagCloud"}">{g->text text="Cloud Settings"}</a>
    </span></span>
  {/if}
  </div>  

{if !empty($form.error)}
<div class="gbBlock">
   <h2 class="giError">{g->text text="There was a problem processing your request."}</h2>
</div>
{/if}

{if isset($status.saved)}
<div class="gbBlock">
   <h2 class="giSuccess">{g->text text="The Settings were successfully saved"}</h2>
</div>
{/if}

<div>
<input type="hidden" name="{g->formVar var="form[tagAdminMode]"}" value="{$tagAdminMode}"/>
</div>

{if ($tagAdminMode == 'General')}
<div class="gbBlock">
  {g->text text="This allows to manage the settings of the tags module"}
</div>

  <div class="gbBlock">
     <table class="gbDataTable">
        <tr>
           <th><label for="TagCloudLink">{g->text text="Show the tag Cloud Link"}</label></th>
           <td><input id="TagCloudLink" type="checkbox" name="{g->formVar var="form[TagCloudLink]"}"
             {if $Settings.TagCloudLink}checked = "checked"{/if} /></td>
        </tr>
        <tr>
           <th><label for="SummaryShow">{g->text text="Show Links & tags below Pictures in album view"}</label></th>
           <td><input id="SummaryShow" type="checkbox" name="{g->formVar var="form[SummaryShow]"}"
             {if $Settings.SummaryShow}checked = "checked"{/if} /></td>
        </tr>
        <tr>
           <th><label for="SummaryAddTag">{g->text text="Allow Tags to be added directly under pictures"}</label></th>
           <td><input id="SummaryAddTag" type="checkbox" name="{g->formVar var="form[SummaryAddTag]"}"
             {if $Settings.SummaryAddTag}checked = "checked"{/if} /></td>
        </tr>
        <tr>
           <th><label for="LimitPopularTag">{g->text text="Limit the amount of popular tags shown"}</label></th>
           <td><input id="LimitPopularTag" type="checkbox" name="{g->formVar var="form[LimitPopularTag]"}"
             {if $Settings.LimitPopularTag}checked = "checked"{/if} /></td>
        </tr>
        <tr>
           <th><label for="PopularTaglimit">{g->text text="Amount to limit to (only valid is above is checked)"}</label></th>
           <td><input id="PopularTaglimit" type="text" width="10" name="{g->formVar var="form[PopularTaglimit]"}"
           {if $Settings.LimitPopularTag}
              value="{$Settings.PopularTaglimit}"
           {/if}
           />
           {if isset($form.error.badlimit)}<span class="giError">{g->text text="Please enter a valid limit"}</span>{/if}
           </td>
        </tr>
     </table>
  </div>
{/if}

{if ($tagAdminMode == 'TagCloud')}
<div class="gbBlock">
  {g->text text="This allows to manage the settings for the tag cloud (both block and main one)"}
</div>

  <div class="gbBlock">
     <table class="gbDataTable">
        <tr>
           <th><label for="tagCloudTemplate">{g->text text="Select a Template to use"}</label></th>
           <td><select id="tagCloudTemplate" name="{g->formVar var="form[tagCloudTemplate]"}">
             <option value="DefaultTextTemplate">{g->text text="Text Template"}</option>
           </select>
		   </td>
        </tr>
     </table>
  </div>
  <div class="gbBlock" id="TemplateSpecificSettings">
  <h3>{g->text text="Template Specific Settings"}</h3>
     <table id="tagClasses" class="gbDataTable">
        <tr id="classNumberTR">
           <th><label for="classNumber">{g->text text="Enter the number of CSS classes wanted"}</label></th>
           <td><input id="classNumber" type="text" name="{g->formVar var="form[classNumber]"}" value="{$Settings.classNumber}"
		   /><a href="javascript:updateTagClassesNumber('{g->url arg1="view=tags.TagsAdminCallback" arg2="tagClassValues=__CLASSES__" arg3="classNumber=__CLASSNUM__" useAuthToken=1}')">{g->text text="Update"}</a>
		   </td>
        </tr>
        {foreach from=$Settings.tagCssClass key=num item=classInfo}
          <tr name="tagTR">
             <th><label for="tagClass{$num}">{g->text text="Tag Class %s" arg1=$num+1}</label></th>
             <td>
                {g->text text="Font Size: "}<input id="tagFont{$num}" type="text" size="4" name="{g->formVar var="form[tagCssClass][`$num`][font]"}" value="{$classInfo[0]}" />{g->text text="em"} - {g->text text="Font Color: "}<input type="text" size="8" id="colorPicker{$num}field" name="{g->formVar var="form[tagCssClass][`$num`][color]"}" value="{$classInfo[1]}" onChange="relateColor('colorPicker{$num}');" />
        		<a href="javascript:ColorChooserShow('colorPicker{$num}');" id="colorPicker{$num}" style="padding:5px">{g->text text="Select"}</a>
        <script language="javascript">relateColor('colorPicker{$num}');</script>
			 </td>
		  </tr>
        {/foreach}
     </table>
  </div>
{/if}

<div class="gbBlock gcBackground2">
  <input class="inputTypeSubmit" type="submit" name="{g->formVar var="form[action][save]"}" value="{g->text text="Save Settings"}" />
  <input class="inputTypeSubmit" type="submit" name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Reset"}" />
</div>