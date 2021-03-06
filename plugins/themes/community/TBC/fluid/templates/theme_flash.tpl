{*
 * $Revision: 525 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
    {* Let Gallery print out anything it wants to put into the <head> element *}
    {g->head}

    {* If Gallery does not provide a header, we use the album/photo title (or filename) *}
    {if empty($head.title)}
      <title>Gallery: Fluid Theme (Flash Based){*$theme.item.title|default:$theme.item.pathComponent|markup:strip*}</title>
    {/if}

    {* Include this theme's style sheet *}
    <link rel="stylesheet" type="text/css" href="{g->theme url="theme.css"}"/>
	{*<script type="text/javascript">
	*	lzEmbed({literal}{{/literal}firstUrl: '{g->url arg1="view=core.ShowItem"
	*		arg2="itemId=`$theme.item.id`"
	*		arg3="serialNumber=`$theme.item.serialNumber`"
	*		arg4="showxml=album" htmlEntities=false}'{literal}}{/literal});
	</script>*}
  </head>
  <body class="gallery">
    <div id="gallery" {g->mainDivAttributes}>
      {*
       * Some module views (eg slideshow) want the full screen.  So for those, we
       * do not draw a header, footer, navbar, etc.  Those views are responsible for
       * drawing everything.
       *}
      {if $theme.useFullScreen}
	{include file="gallery:`$theme.moduleTemplate`" l10Domain=$theme.moduleL10Domain}
      {else}
      <!---<div id="gsHeader">
	<img src="{g->url href="images/galleryLogo_sm.gif"}" width="107" height="48" alt=""/>
      </div>

      <div id="gsNavBar" class="gcBorder1">
	<div class="gbSystemLinks">
	  {g->block type="core.SystemLinks"
		    order="core.SiteAdmin core.YourAccount core.Login core.Logout"
		    othersAt=4}
	</div>

	<div class="gbBreadCrumb">
	  {g->block type="core.BreadCrumb"}
	</div>
      </div>--->

      {* Include the appropriate content type for the page we want to draw. *}
      {if $theme.pageType == 'album'}
	{g->theme include="album_flash.tpl"}
      {elseif $theme.pageType == 'photo'}
	{g->theme include="photo.tpl"}
      {elseif $theme.pageType == 'admin'}
	{g->theme include="admin.tpl"}
      {elseif $theme.pageType == 'module'}
	{g->theme include="module.tpl"}
      {elseif $theme.pageType == 'progressbar'}
	{g->theme include="progressbar.tpl"}
      {/if}

      <div id="gsFooter">
	{g->logoButton type="validation"}
	{g->logoButton type="gallery2"}
	{g->logoButton type="gallery2-version"}
      </div>
      {/if}  {* end of full screen check *}
    </div>

    {*
     * Give Gallery a chance to output any cleanup code, like javascript that
     * needs to be run at the end of the <body> tag.  If you take this out, some
     * code would not work properly.
     *}
    {g->trailer}

    {* Put any debugging output here, if debugging is enabled *}
    {g->debug}
  </body>
</html>
