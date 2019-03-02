{*
 * $Revision: 1711 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div id="gsContent" class = "gcBorder1">
<form action="{g->url}" method="post">
{g->hiddenFormVars}
<input type="hidden" name="{g->formVar var="controller"}" value="{$tpl.controller}"/>
<input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>

<div class="gbBlock gcBackground1">
  <h2 class="gbTitle">
    {g->text text="Checkout"} {g->text text="Step 1 - Adjust products and quantities"}
  </h2>
  <p>
{g->text text="Remove items or adjust quantities here, then continue on to confirm your order."}
</p>
</div>


{if $tpl.items|@count == 0}

<div class="gbBlock">
<p>{g->text text="Your cart is currently empty."}</p>
   <table class="checkoutButtonTable" width="100%">
      <tr>
        <td align="right">
          <input type="submit" class="inputTypeSubmit checkoutButton checkoutShoppingButton1" name="{g->formVar var="form[action][continueshopping]"}" value="{g->text text="Continue shopping"}"/>
        </td>
      </tr>
    </table>

</div>
</form>
</div>
{else}

{* ----------- Cart Contents ----------- *}
<div class="gbBlock">
{if isset($tpl.carttext)}
  <div class="checkoutDetailsText">{$tpl.carttext|markup}</div>
{/if}

{* include $tpl.items as contents *}
{include file='gallery:modules/checkout/templates/SessionContents.tpl'}
</div>

{* ----------- Buttons ----------- *}
<div class="gbBlock">
     <table class="checkoutButtonTable" width="100%">
      <tr>
        <td align="right">
          <input type="submit" class="inputTypeSubmit checkoutButton checkoutEmptyButton" name="{g->formVar var="form[action][emptycart]"}" value="{g->text text="Empty Cart"}"/>
          <input type="submit" class="inputTypeSubmit checkoutButton checkoutSaveButton" name="{g->formVar var="form[action][savechanges]"}" value="{g->text text="Save Changes"}"/> 
          <input type="submit" class="inputTypeSubmit checkoutButton checkoutContinueButton1" name="{g->formVar var="form[action][continue]"}" value="{g->text text="Continue to Checkout"}"/>
        </td>
      </tr>
      <tr>
        <td align="right">
          <input type="submit" class="inputTypeSubmit checkoutButton checkoutShoppingButton1" name="{g->formVar var="form[action][continueshopping]"}" value="{g->text text="Continue Shopping"}"/>
        </td>
      </tr>
    </table>
</div>
{/if}
</form>
</div>
