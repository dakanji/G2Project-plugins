{*
 * $Revision: 896 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="addviaemail.AddviaEmailBlock"}

{if $user.isAdmin}
<div class="{$class}">
{g->text text="AddviaEmail results:"}<br/>

{if $block.addviaemail.checkperformed == '1'}
{g->text text="Mailbox Checked"}
{else}
{g->text text="Not time yet"}
{/if}
<br/>



{g->text one="1 picture added." many="%d pictures added." count=$block.addviaemail.emailsprocessed
    arg1=$block.addviaemail.emailsprocessed}<br/>
{if !empty($block.addviaemail.failure)}
  {capture assign="configureLink"}<a href="{g->url arg1="view=core.SiteAdmin"
   arg2="subView=addviaemail.Configure" arg3="return=true"}">{/capture}
  {g->text text="There was an error processing emails. Please check the %sconfiguration%s"
	   arg1=$configureLink arg2="</a>"}<br/>
{/if}
</div>
{/if}
