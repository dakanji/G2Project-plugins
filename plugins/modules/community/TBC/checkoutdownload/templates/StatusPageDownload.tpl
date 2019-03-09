{*
 * $Revision: 1690 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
 
{g->callback type="checkoutdownload.downloadInfo" transactionId=$form.transactionId}
<div class="gbBlock">
<h3>
{g->text text="Download Information"}
</h3>
{if $block.checkoutDownload.downloadTest == 'permit'}

<p>{g->text text="Please use the button to download your items."}</p>
<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[checkoutdownload][downloadZip]"}" value="{g->text text="Download Items in ZIP format"}"/>
<input type="hidden" name="{g->formVar var="form[checkoutdownload][transactionId]"}" value="{$tpl.id}">
<input type="hidden" name="{g->formVar var="form[checkoutdownload][verify1]"}" value="{$block.checkoutDownload.verify1}">
<input type="hidden" name="{g->formVar var="form[checkoutdownload][verify2]"}" value="{$block.checkoutDownload.verify2}">
</a>
<br/>
<p>{g->text text="No. of times downloading has been attempted:"}{$block.checkoutDownload.downloadAttempts}</p>
{elseif $block.checkoutDownload.downloadTest == 'empty'}
<p>{g->text text="This order does not include any downloadable items."}</p>
{elseif $block.checkoutDownload.downloadTest == 'outOfTime'}
<p>{g->text text="The time period for downloading items in this order has expired."}</p>
{elseif $block.checkoutDownload.downloadTest == 'notPaid'}
<p>{g->text text="Items in this order can be downloaded when payment is cleared."}</p>
{elseif $block.checkoutDownload.downloadTest == 'tooManyDownloads'}
<p>{g->text text="The limit on the number of times this order may be downloaded has already been reached."}</p>
{else}
<p>{g->text text="Oh wie!"}</p>
{/if}
{if $block.checkoutDownload.downloaded > 1}
<p>{g->text text="The last download attempt was at"} {g->date timestamp=$block.checkoutDownload.downloaded style = "datetime"}</p>
{/if}
</div>