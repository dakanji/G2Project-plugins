{*
 * $Revision: 1389 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="checkout.transactionCustomerDetails" transactionId=$transactionId}
{if $rendering == 'text'}
 {g->text text="Customer"}: {$block.checkout.name}
    {g->text text="Email"}: <{$block.checkout.email}>
{g->text text="Recipient"}: {$block.checkout.recipientName}
  {g->text text="Address"}: {$block.checkout.address1}
           {$block.checkout.address2}
           {$block.checkout.address3}
           {$block.checkout.address4}
           {$block.checkout.address5}
      {g->text text="Zip"}: {$block.checkout.addressZip}
  {g->text text="Country"}: {$block.checkout.country}
  {g->text text="Telephone"}: {$block.checkout.telephone}
 {g->text text="Comments"}: {$block.checkout.custComments} {$block.checkout.custNotes}
{/if}
{if $rendering == 'html'}
<table cellspacing="5">
  <tr>
    <td align="right"><strong>{g->text text="Name"}:</strong></td>
    <td>{$block.checkout.name}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Email Address"}:</strong></td>
    <td>{$block.checkout.email}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Recipient's Name"}:</strong></td>
    <td>{$block.checkout.recipientName}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Postal Address"}:</strong></td>
    <td>{$block.checkout.address1}</td>
  </tr>
  <tr>
    <td align="right">&nbsp;</td>
    <td>{$block.checkout.address2}</td>
  </tr>
  <tr>
    <td align="right">&nbsp;</td>
    <td>{$block.checkout.address3}</td>
  </tr>
  <tr>
    <td align="right">&nbsp;</td>
    <td>{$block.checkout.address4}</td>
  </tr>
  <tr>
    <td align="right">&nbsp;</td>
    <td>{$block.checkout.address5}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Postal/Zip Code"}:</strong></td>
    <td>{$block.checkout.addressZip}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Country"}:</strong></td>
    <td>{$block.checkout.country}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Telephone"}:</strong></td>
    <td>{$block.checkout.telephone}</td>
  </tr>
  <tr>
    <td align="right"><strong>{g->text text="Comments"}:</strong></td>
    <td>{$block.checkout.custComments} {$block.checkout.custNotes}</td>
  </tr>
</table>
{/if}
