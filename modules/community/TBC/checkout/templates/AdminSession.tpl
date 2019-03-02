{*
 * $Revision: 1689 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<input type="hidden" name="{g->formVar var="form[sessId]"}" value="{$form.sessId}"/>
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Cart Contents for Session "}{$form.sessId}</h2>
</div>
<div class="gbBlock">
<table class="gbDataTable">
<tr class="gbOdd">
<td><strong>{g->text text="user Name"}</strong></td>
<td>{$session.userName}</td>
</tr>
<tr class="gbEven">
<td><strong>{g->text text="Session Last Modified"}</strong></td>
<td>{g->date timestamp=$session.sessionModTime style="datetime"}</td>
</tr>
<tr class="gbOdd">
<td><strong>{g->text text="Session Created"}</strong></td>
<td>{g->date timestamp=$session.sessionCreateTime style="datetime"}</td>
</tr>
<tr class="gbEven">
<td><strong>{g->text text="IP address"}</strong></td>
<td>{$session.host}</td>
</tr>
<tr class="gbOdd">
<td><strong>{g->text text="Last Transaction Id"}</strong></td>
<td><a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=checkout.AdminOrder" arg3="transactionId=`$session.transactionId`"}">{$session.transactionId}</a></td>
</tr>
<tr class="gbEven">
<td><strong>{g->text text="Last Return Url"}</strong></td>
<td><a href="{$session.returnUrl}">{$session.returnURL}</a></td>
</tr>
</table>
</div>

{if !empty($status)}
<div class="gbBlock">
  <h2 class="giSuccess">
    {if isset($status.discarded)}
      {g->text text="Changes discarded."}
    {/if}
    {if isset($status.saved)}
      {g->text text="Cart saved."}
    {/if}
        {if isset($status.cleared)}
      {g->text text="Cart cleared."}
    {/if}
  </h2>
</div>
{/if}



{if !$tpl.itemCount}

<div class="gbBlock">
<p>{g->text text="This cart is currently empty."}</p>
</div>


{else}


{* ----------- Cart Contents ----------- *}
<div class="gbBlock">
<h3>{g->text text="Cart Contents"}</h3>
{* include $tpl.items as contents *}
{include file='gallery:modules/checkout/templates/SessionContents.tpl'}
</div>

{/if}
{* ----------- Buttons ----------- *}
<div class="gbBlock gcBackground1">
{if $tpl.itemCount}
          <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][emptycart]"}" value="{g->text text="Empty Cart"}"/>
          <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][savechanges]"}" value="{g->text text="Save Changes"}"/> 
   	  <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][discardchanges]"}" value="{g->text text="Discard Changes"}"/>
{/if}
   	  <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][addmine]"}" value="{g->text text="Add My Cart To This Cart"}"/>
   	  <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][return]"}" value="{g->text text="Return to Sessions List"}"/>
</div>
