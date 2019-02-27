{*
 * $RCSfile: AlbumMissingMapLocation.tpl,v $
 * $Revision: 1824 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<div class="gbBlock gcBackground1">
  <h2 class="gbTitle"> {g->text text="Albums Missing Map Location"} </h2>
</div>


  <div class="gbAdmin">
<br>
    <div class="gbDataEntry">

    {if count($baditems) > 0}
	{g->text text="The following albums are missing map locations:"}

	<ol>
		{foreach from=$baditems item=baditempair}			
			{assign var="baditem" value=$baditempair.item }
			{assign var="baditemparents" value=$baditempair.parent }
			<li>
			{foreach from=$baditemparents item=baditemparent}
				{$baditemparent->title|markup:strip|default:$parent.pathComponent}&nbsp;::&nbsp;
			{/foreach}

			{g->text text="%s" arg1=$baditem->title|markup:strip|default:$parent.pathComponent} :

			<a href="{g->url arg1="view=core.ShowItem"
						  arg2="itemId=`$baditem->id`"}">

	  	        {g->text text="View"}
	    		</a>
			&nbsp;
			<a href="{g->url arg1="view=core.ItemAdmin"
					 arg2="core.ItemEdit"
					 arg3="itemId=`$baditem->id`"
                                         arg4="&g2_editPlugin=mapItemEdit" }">

	  	        {g->text text="Edit"}
	    		</a>

			</li>
		{/foreach}
	</ol>

    {else}
	{g->text text="There are no items that have missing map locations."}
    {/if}	

    </div>
  </div>
