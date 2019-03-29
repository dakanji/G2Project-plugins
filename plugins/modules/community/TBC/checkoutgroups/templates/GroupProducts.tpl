
<div class="gbBlock">
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Group-based visibility and Group Access"} </h2>
</div>
 <p class="giDescription">
    {g->text text="You may choose to make a product visible only to members of a particular group, here."}
    {g->text text="Note that visibility of a product is additive. The visibility setting in this table only has effect if the product is not globally visible already."}
 </p>
 <p class="giDescription">
    {g->text text="If a customer purchases a product which has a group assigned to it as a purchase group, they will be added to that group when the order payment is cleared."}
    {g->text text="Groups can be used, for instance, to permit access to particular albums on payment, or to authorize discounts for returning customers."}
 </p>
 <p class="giDescription">
    {g->text text="If the customer is not logged in (i.e. the customer is the Anonymous User) they would not be added to any groups. If you want a product to be available only to logged-in users then "}
    {g->text text="set the global 'visible' setting to 'no', and make the product visible to the 'Registered Users' group here."}
 </p>
 
 <table class="gbDataTable">
    <tr>
      <th>{g->text text="Product Name"}</th>
      <th>{g->text text="Make visible to members of"}</th>
      <th>{g->text text="Group to which to add purchaser"}</th>
 
    </tr>
          	{foreach from=$form.products item=product }
    <tr>
      <td>{$product.rootName}</td>
       <td><select name="{g->formVar var="form[checkoutGroups][`$product.id`][groupVisibleId]"}">
      <option value = "" {if !(isset($product.groupVisibleId))}selected{/if}>{g->text text="(None)"}</option>
        {foreach from=$groups key=thisGroupVisibleId item=thisGroupName}
      <option value = "{$thisGroupVisibleId}" {if isset($product.groupVisibleId) && $product.groupVisibleId == $thisGroupVisibleId}selected{/if}>{$thisGroupName}</option>
        {/foreach}		
       </select></td>
      <td><select name="{g->formVar var="form[checkoutGroups][`$product.id`][groupId]"}">
      <option value = "" {if !(isset($product.groupId))}selected{/if}>{g->text text="(None)"}</option>
        {foreach from=$groups key=thisGroupId item=thisGroupName}
      <option value = "{$thisGroupId}" {if isset($product.groupId) && $product.groupId == $thisGroupId}selected{/if}>{$thisGroupName}</option>
        {/foreach}		
       </select></td>
      </tr>
       
  		{/foreach}
  </table>
</div>


