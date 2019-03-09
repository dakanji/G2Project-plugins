{*
 * $Revision: 0.9 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Duplicate Images"} </h2>
</div>

{*
{if !empty($status)}
<div class="gbBlock"><h2 class="giSuccess">
  {if ($status.ignored.count > 0)}
    {g->text text="Pair marked as ignored"}<BR>
  {/if}
  {if ($status.deleted.count > 0)}
    {g->text text="Images were deleted"}<BR>
  {/if}
  {if ($status.linked.count > 0)}
    {g->text text="Images were linked"}<BR>
  {/if}
</h2></div>
{/if}
*}

<div class="gbBlock">
  <table class="gbDataTable" width="100%" CELLSPACING="0">
  <tr>
    <th> {g->text text="Image 1"} </th>
    <th> {g->text text="Select action"} </th>
    <th> {g->text text="Image 2"} </th>
    <th> {g->text text="Duplication type"} </th>
    <th> {g->text text="Status"} </th>
  </tr>
  {foreach from=$form.list item=item}
  <input type="hidden" name="{g->formVar var="form[pairs][`$item.pairId`]"}" 
        value="{$item.srcItemData.id},{$item.dupItemData.id}"/>
  {cycle values="gbEven,gbOdd" assign="trClass"}
  <tr class="{$trClass}">
    <td class="giItemCell">
    <div>
     <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$item.srcItemData.id`"}">
      {g->image item=$item.srcItemData image=$item.srcItemData.thumbnail maxSize=100 class="giThumbnail"}
      </a>
    </div>
      {if !empty($item.srcItemData.title)}
      <p class="giTitle">
        {$item.srcItemData.title|markup}
      </p>
      {/if}
      {g->block type="core.ItemInfo"
                item=$item.srcItemData
                showDate=true
                showSize=true
                class="giInfo"}
        
    </td><td>
      <input type="radio" name="{g->formVar var="form[actions][`$item.pairId`]"}" value="leaveAsIs" CHECKED/>
        {g->text text="Do nothing"}
        <br>
      <input type="radio" name="{g->formVar var="form[actions][`$item.pairId`]"}" value="ignore"/>
        {g->text text="Ignore"}
    </td>
    <td class="giItemCell">
    <div>
     <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$item.dupItemData.id`"}">
      {g->image item=$item.dupItemData image=$item.dupItemData.thumbnail maxSize=100 class="giThumbnail"}
      </a>
    </div>
      {if !empty($item.dupItemData.title)}
      <p class="giTitle">
        {$item.dupItemData.title|markup}
      </p>
      {/if}
      {g->block type="core.ItemInfo"
                item=$item.dupItemData
                showDate=true
                showSize=true
                class="giInfo"}
    </td><td>
      {g->text text=$form.dupTypeList[$item.dupType]}
    </td><td>
      {g->text text=$form.dupStatusList[$item.userStatus]}
    </td>
  </tr>
  
  <tr valign="top" class="{$trClass}">
  <td>
        <input type="radio" name="{g->formVar var="form[actions][`$item.pairId`]"}" value="deleteSrc"/>
        {g->text text="Delete this image"}
        <br>
        <input type="radio" name="{g->formVar var="form[actions][`$item.pairId`]"}" value="linkToSrc"/>
        {g->text text="Link to this image"}
  </td><td>
  </td><td>
    <input type="radio" name="{g->formVar var="form[actions][`$item.pairId`]"}" value="deleteDup"/>
        {g->text text="Delete this image"}
        <br>
        <input type="radio" name="{g->formVar var="form[actions][`$item.pairId`]"}" value="linkToDup"/>
        {g->text text="Link to this image"}
  </td><td></td>
  <td></td>
  </tr>
  {/foreach}
  <tr>
      <td colspan="2">
      <input type="checkbox" id="cbShowIgnored" name="{g->formVar var="form[showIgnored]"}"         
        {if $form.showIgnored} checked="checked"{/if}/>
      <label for="cbShowIgnored">
    {g->text text="Show previously ignored duplicate pairs"}
      </label>
    </td>
  </tr>
</table>
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][submit]"}" value="{g->text text="Submit"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
  <input type="hidden" name="{g->formVar var="form[action][showIgnored]"}" value="unset"/>
</div>

