{*
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <h3> {g->text text="Invoice (PDF Format)"} </h3>
 
<p>
{g->text text="Download an invoice for this order in Adobe PDF format here."}
</p>
<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[checkoutpdfinvoice][downloadPDF]"}" value="{g->text text="Download PDF-format invoice"}"/>
<input type="hidden" name="{g->formVar var="form[checkoutpdfinvoice][transactionId]"}" value="{$tpl.id}">
</div>


