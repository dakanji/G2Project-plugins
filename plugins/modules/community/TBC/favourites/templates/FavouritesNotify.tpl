{* $Revision: 1371 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Favorites"} </h2>
</div>

<div class="gbBlock">

  {if !empty($form.error.isGuest) }
  <h2 class="giError">
	{g->text text="You are not logged in."}
  </h2>
  <p>
  {g->text text="To bookmark an item as one of your favorites, and to view them later, you need to be logged in as a registered user!"}
  {g->text text="Use the register link above to create an account (it only takes a second) or login if you are already registered."}
  </p>
  {/if}

  {if isset($status.isEmpty)}
  <h2 class="giError">
    {g->text text="You have not marked any favorites."}
  </h2>
  <p>
  {g->text text="Use the add item to favorites link below pictures or albums that you like. The show favorites page will show you everything that you have marked."}
  </p>
  {/if}

  {if isset($status.added)}
  <h2 class="giSuccess">
    {g->text text="%s added to favorites." arg1=$itemDetails.itemTypeName.1}
  </h2>
  {/if}
  
  {if isset($status.removed)}
  <h2 class="giSuccess">
    {g->text text="%s removed from favorites." arg1=$itemDetails.itemTypeName.1}
  </h2>
  {/if}

  {if isset($status.cleared)}
  <h2 class="giSuccess">
    {g->text text="Favorites Cleared."}
  </h2>
  {/if}


  {if isset ($itemDetails.itemId)}
  <p>
    <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$itemDetails.itemId`"}">
      {g->text text="Back to %s" arg1=$itemDetails.itemTypeName.1}
    </a>
  </p>
  {/if}
 
</div>
