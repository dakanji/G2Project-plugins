{*
 * $RCSfile: Sent.tpl,v $
 * $Revision: 1887 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

{if ($emailsent)}
<div class="gbBlock gcBackground1">
  <h2 class="gbTitle">
    {g->text text="Contact"}::{g->text text="Email Sent"}
  </h2>
</div>
<p><strong>{g->text text="Thank you - your message has been sent."}</strong></p><p>&nbsp;
<p>{g->text text="If necessary, we will be in contact as soon as possible."}</p>
{else}
<div class="gbBlock gcBackground1">
  <h2 class="gbTitle">
    {g->text text="Contact"}::{g->text text="Email Failed"}
  </h2>
</div>
<p><strong><font color="#ff0000">{g->text text="Sorry, your message cannot be sent."}</strong></p><p>&nbsp;
<p>{g->text text="You have sent too many emails using this browser session, please close and retry."}</font></p>
{/if}