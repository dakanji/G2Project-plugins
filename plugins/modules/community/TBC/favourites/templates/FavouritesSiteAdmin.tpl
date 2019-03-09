{*
 * $Revision: 1371 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<div class="gbBlock gcBackground1">
  <h2> {g->text text="Users with Favorites"} </h2>
</div>
{if empty($form.list.users)}
<div class="gbBlock">
<table class="gbDataTable">
  <tr>
    <td>{g->text text="None"}</td>
  </tr>
  </table>
  </div>
{else}
<div class="gbBlock">
  <input type="text" id="giFormUsername" size="20" autocomplete="off"
   name="{g->formVar var="form[text][userName]"}" value="{$form.text.userName}"/>
  {g->autoComplete element="giFormUsername"}
    {g->url arg1="view=favourites.getNames" arg2="command=lookupUsername"
	    arg3="like=__VALUE__" htmlEntities=false}
  {/g->autoComplete}
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][displayFromText]"}" value="{g->text text="View"}"/>
</div>

<div class="gbBlock">
{if ($form.list.maxPages > 1)}
  <div style="margin-bottom: 10px"><span class="gcBackground1" style="padding: 5px">
    <input type="hidden" name="{g->formVar var="form[list][page]"}" value="{$form.list.page}"/>
    <input type="hidden" name="{g->formVar var="form[list][maxPages]"}" value="{$form.list.maxPages}"/>

    {if ($form.list.page > 1)}
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=favourites.FavouritesSiteAdmin" arg3="form[list][page]=1"}">{g->text text="&laquo; first"}</a>
    &nbsp;
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=favourites.FavouritesSiteAdmin" arg3="form[list][page]=`$form.list.backPage`"}">{g->text text="&laquo; back"}</a>
    {/if}

    &nbsp;
    {g->text text="Viewing page %d of %d" arg1=$form.list.page arg2=$form.list.maxPages}
    &nbsp;

    {if ($form.list.page < $form.list.maxPages)}
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=favourites.FavouritesSiteAdmin" arg3="form[list][page]=`$form.list.nextPage`"}">{g->text text="next &raquo;"}</a>
    &nbsp;
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=favourites.FavouritesSiteAdmin" arg3="form[list][page]=`$form.list.maxPages`"}">{g->text text="last &raquo;"}</a>
    {/if}
  </span></div>
{/if}

    <table class="gbDataTable">
    <tr><th>{g->text text="Name"}</th><th>{g->text text="UserName"}</th><th>{g->text text="Items"}</th></tr>
    {section name=x loop = $form.list.users start=$form.list.start max=$form.list.pageSize}
       <tr class="{cycle values="gbEven,gbOdd"}">
       <td><a href="{g->url arg1="view=favourites.FavouritesAlbum" arg2="targetUserId=`$form.list.users[x].id`" }">{$form.list.users[x].name}</a></td>
       <td>{$form.list.users[x].userName}</td>
       <td>{$form.list.users[x].count}</td>
       </tr>
    {/section}
    </table>
    
    
</div>


{if !empty($status) || !empty($form.error)}
<div class="gbBlock">
{if isset($status.saved)}
<h2 class="giSuccess">
  {g->text text="Settings saved successfully"}
</h2>
{/if}

{if !empty($form.error)}
<h2 class="giError">
  {g->text text="There was a problem processing your request."}
</h2>
{/if}

</div>
{/if}
{/if}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Module Settings"} </h2>
</div>


<div class="gbBlock">

  <table class="gbDataTable">
  <tr>
    <td colspan="2">{g->text text="Offer 'Add to Favorites' to guest users (with an appropriate message if they choose it)?"}</td>
  </tr>
  <tr>
   <td>
      <input type="checkbox" id="offerToGuests"{if !empty($form.offerToGuests)}
       checked="checked"{/if} name="{g->formVar var="form[offerToGuests]"}"/>
      <label for="offerToGuests">
	{g->text text="Offer Favorites to Guests"}
      </label>
    </td>
  </tr></table>
</div>

<div class="gbBlock gcBackground1">
  <h2> {g->text text="Theme-based Settings for Favorites"} </h2>
</div>


{capture assign="message"}
  {g->text text="Theme"} &nbsp; &nbsp; &nbsp;
  <select name="{g->formVar var="form[themeId]"}">
    {html_options options=$FavouritesSiteAdmin.themeList selected=$form.themeId}
  </select>
  <br/><br/>
  {g->text text="Settings for %s theme in Dynamic Albums" arg1=$ThemeSettingsForm.theme.name}
{/capture}

{g->block type="core.ThemeSettingsForm" class="gbBlock" message=$message formId="siteAdminForm"}

<div class="gbBlock gcBackground1">
  <input type="hidden" name="{g->formVar var="form[currentThemeId]"}" value="{$form.themeId}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][reset]"}" value="{g->text text="Reset"}"/>
</div>
