{*
 * $Revision: 1664 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="checkoutdownload.downloadInfo" transactionId=$transactionId}
{if $block.checkoutDownload.downloadTest == 'permit'}
{if $rendering == 'text'}
{g->text text="Your downloads have been authorised. You can get the files here:"}
{g->url arg1="view=checkoutdownload.Download" arg2="transactionId=`$transactionId`" arg3="verify1=`$block.checkoutDownload.verify1`" arg4="verify2=`$block.checkoutDownload.verify2`" forceFullUrl=true forceSessionId=false htmlEntities=false}


{/if}

{if $rendering == 'html'}
{g->text text="Your downloads have been authorised. You can get the files here:"}
<br/>
<form action="{g->url arg1="controller=checkoutdownload.BuildDownload" arg2="transactionId=`$transactionId`" arg3="verify1=`$block.checkoutDownload.verify1`" arg4="verify2=`$block.checkoutDownload.verify2`" forceFullUrl=true forceSessionId=false}" method="post">
    <button type="submit">{g->text text="Download as Zip"}</button>
</form>
<br/>
<br/>
{/if}
{/if}
