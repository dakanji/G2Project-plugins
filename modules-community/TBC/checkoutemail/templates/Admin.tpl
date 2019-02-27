{*
 * $Revision: 1583 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Checkoutemail Settings"} </h2>
</div>


 {if  isset($status.saved)}
<div class="gbBlock"><h2 class="giSuccess">
    {g->text text="Settings saved successfully"} <br/>
</h2></div>
{/if}

<div class="gbBlock">
       <h3>
	{g->text text="Activate"}
      </h3>
	<p class="giDescription">
	{g->text text="Activate this method of payment?"}
	</p>
	<input type="radio" name="{g->formVar var="form[active]"}" value="true" {if $form.active}checked{/if}>{g->text text="Yes"}<br>
	<input type="radio" name="{g->formVar var="form[active]"}" value="false" {if !$form.active}checked{/if}>{g->text text="No"}<br>
</div>
<div class="gbBlock">
       <h3>
	{g->text text="Description"}
      </h3>
	<p class="giDescription">
	{g->text text="Description of payment method to appear on checkout page"}
	</p>
	<textarea name="{g->formVar var="form[paymentText]"}" cols="70" rows="3">{$form.paymentText}</textarea>
</div>
<div class="gbBlock">
       <h3>
	{g->text text="Button Text"}
       </h3>
	<p class="giDescription">
	{g->text text="Caption of the button to appear on checkout page"}
	</p>
	<input type="text" name="{g->formVar var="form[paymentButtonText]"}" size="20" value="{$form.paymentButtonText}">
</div>


<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][save]"}"
     value="{g->text text="Save Settings"}"/>
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][cancel]"}"
     value="{g->text text="Cancel"}"/>
  </div>
</div>
