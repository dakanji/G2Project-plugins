{*
 * $Revision: 847 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Import from Coppermine"} </h2>

  <p class="giDescription">
    {g->text text="Copy all or part of an existing Coppermine installation into your Gallery 2.  It won't modify your Coppermine data in any way."}
  </p>
</div>

{if (!$SelectCmAlbum.hasToolkit)}
<div class="gbBlock"><p class="giError">
  {capture name="url"}
    {g->url arg1="view=core.SiteAdmin" arg2="subView=core.AdminModules"}
  {/capture}
  {g->text text="You don't have any Graphics Toolkit activated to handle JPEG images.  If you import now, you will not have any thumbnails.  Visit the <a href=\"%s\">Modules</a> page to activate a Graphics Toolkit." arg1=$smarty.capture.url}
</p></div>
{/if}

<div class="gbBlock">
  <h1 class="giTitle"> {g->text text="Path to Coppermine directory"} </h1>
  <p class="giDescription">
    <i>{g->text text="Example: /var/www/coppermine"}</i>
  </p>

  <div>
    <input type="text" size="60"
     name="{g->formVar var="form[copperminePath]"}" value="{$form.copperminePath}"
      id='giFormPath' autocomplete="off"/>
    {g->autoComplete element="giFormPath"}
      {g->url arg1="view=core.SimpleCallback" arg2="command=lookupDirectories"
	      arg3="prefix=__VALUE__" htmlEntities=false}
    {/g->autoComplete}

    {if isset($form.error.copperminePath.missing)}
    <div class="giError">
      {g->text text="You did not enter a path."}
    </div>
    {/if}
    {if isset($form.error.copperminePath.invalid)}
    <div class="giError">
      {g->text text="The path that you entered is invalid."}
    </div>
    {/if}
  </div>

  {if !empty($SelectCmAlbum.recentPaths)}
  <script type="text/javascript">
    // <![CDATA[
    function selectPath(path) {ldelim}
      document.getElementById('siteAdminForm').elements['{g->formVar
	var="form[copperminePath]"}'].value = path;
    {rdelim}
    // ]]>
  </script>

  <h4 class="giTitle">
    {g->text text="Recently Used Paths"}
  </h4>
  <p>
  {foreach from=$SelectCmAlbum.recentPaths key=path item=count}
    {capture name="escapedPath"}{$path|replace:"\\":"\\\\"}{/capture}
    <a href="javascript:selectPath('{$smarty.capture.escapedPath}')">{$path}</a>
    <br/>
  {/foreach}
  </p>
  {/if}

</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][select]"}" value="{g->text text="Select"}"/>
</div>
