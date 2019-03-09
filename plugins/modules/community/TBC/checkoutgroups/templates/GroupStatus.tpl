
{*
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if $form.formName == 'AdminOrder'}
<div class="gbBlock">
  <h3> {g->text text="Group Access"} </h3>
 <p>
 {if $checkoutgroups.hasGroups}
 {if $checkoutgroups.isGuest}
  <h2 class="giError">
      {g->text text="This order was placed under the Anonymous User - group assignments will not be carried out when payment is cleared."}
  </h2>
  <br>
 {/if}
 
    {g->text text="This order includes the following group access:"}
      <table class="gbDataTable">
      <tr>
       <th align="center">{g->text text="Item"}</th>
       <th align="center">{g->text text="Product"}</th>
       <th align="center">{g->text text="Group"}</th>
       <th align="center">{g->text text="Member"}</th>
      </tr>
       {foreach from=$checkoutgroups.groupInfo item=row }
      <tr>
       <td>{$row.itemName}</td>
       <td>{$row.productName}</td>
       <td>{$row.groupName}</td>
       <td>{if $row.isInGroup}{g->text text="yes"}{/if}</td>
      </tr>
       {/foreach}
     </table>
 {else}
   {g->text text="This order includes no special group access."}
 {/if}
 </p>
</div>

{else}
{if $checkoutgroups.hasGroups}
<div class="gbBlock">
  <h3> {g->text text="Online Access"} </h3>
 <p>
    {g->text text="This order includes online access to larger images."}
 </p>
</div>

{/if}
{/if}
