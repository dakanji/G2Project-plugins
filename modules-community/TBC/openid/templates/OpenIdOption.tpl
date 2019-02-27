{*
 * $Revision: 996 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock">
  <h4> {g->text text="OpenID"} </h4>
  {foreach from=$form.openIds item="openId"}
    <input id="giFormOpenId" name="{"form[openIds][]"|formVar}" type="text" value="{$openId}"/>
    <br/>
  {/foreach}
  <input id="giFormOpenId" name="{"form[openIds][]"|formVar}" type="text"/>
</div>
