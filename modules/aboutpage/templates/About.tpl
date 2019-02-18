{*
 * $RCSfile: About.tpl,v $
 * $Revision: 1874 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}


        <div class="gbBlock gcBackground1">
          <h2 class="gbTitle">
            {g->text text="Contact"}::{g->text text="About"}
          </h2>
        </div>

        {if $form.message}
        {assign var="messageText" value=$form.message|markup}
	  {$messageText}
        {/if}

<div class="gbBlock gcBackground1">
&nbsp;
</div>



