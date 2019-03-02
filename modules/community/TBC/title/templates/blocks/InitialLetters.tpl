{*
 * $Revision: 1420 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="{$class}">
{g->text text="Items beginning with:"}<br>
{assign var=letters value=' '|explode:'A B C D E F G H I J K L M N O P Q R S T U V W X Y Z'} 
{foreach from=$letters item=letter}
<a href="{g->url arg1="view=title.Begin" arg2="chars=`$letter`"}">{$letter}</a>
{/foreach}
</div>

 