{*
 * $Revision: 1822 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
 <div class="gbBlock gcBackground1">
  <h2>{g->text text="Checkout Paypal Settings"}</h2>
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
	<input type="radio" name="{g->formVar var="form[active]"}" value="1" {if $form.active}checked{/if}>{g->text text="Yes"}<br>
	<input type="radio" name="{g->formVar var="form[active]"}" value="0" {if !$form.active}checked{/if}>{g->text text="No"}<br>
</div>
 <div class="gbBlock">
       <h3>
	{g->text text="Paypal email address"}
      </h3>
      <p class="giDescription">
	{g->text text="This is your Paypal account identifier. If you get this wrong, someone else will get the money!"}
      </p>
      <input type="text" size="50" name="{g->formVar var="form[paypalname]"}" value="{$form.paypalname}">
</div>
<div class="gbBlock">
       <h3>
	{g->text text="Your logo"}
      </h3>
      <p class="giDescription">
	{g->text text="Paypal will display a small (150px x 90px max) image at the top of the payment pages, if you enter the full url here. If left blank, by default Paypal will use your email address or business name instead."}
      </p>
      <input type="text" size="50" name="{g->formVar var="form[imageUrl]"}" value="{$form.imageUrl}">
      {if isset($form.imageUrl)}
      <p class="giDescription"><img src="{$form.imageUrl}"></p>
      {/if}
      
</div>
<div class="gbBlock">
       <h3>
	{g->text text="Currency code for PayPal payments"}
       </h3>
        <p class="giDescription">
	{g->text text="Paypal operates in several different currencies. You can choose any of them."}
      </p>
      <select name="{g->formVar var="form[ccode]"}">
        <option value="AUD"{if $form.ccode=='AUD'} selected{/if}>AUD</option>
        <option value="CAD"{if $form.ccode=='CAD'} selected{/if}>CAD</option>
        <option value="EUR"{if $form.ccode=='EUR'} selected{/if}>EUR</option>
        <option value="GBP"{if $form.ccode=='GBP'} selected{/if}>GBP</option>
        <option value="JPY"{if $form.ccode=='JPY'} selected{/if}>JPY</option>
        <option value="USD"{if $form.ccode=='USD'} selected{/if}>USD</option>
        <option value="CHF"{if $form.ccode=='CHF'} selected{/if}>CHF</option>
        <option value="HKD"{if $form.ccode=='HKD'} selected{/if}>HKD</option>
        <option value="SGD"{if $form.ccode=='SGD'} selected{/if}>SGD</option>
        <option value="SEK"{if $form.ccode=='SEK'} selected{/if}>SEK</option>
        <option value="DKK"{if $form.ccode=='DKK'} selected{/if}>DKK</option>
        <option value="PLN"{if $form.ccode=='PLN'} selected{/if}>PLN</option>
        <option value="NOK"{if $form.ccode=='NOK'} selected{/if}>NOK</option>
        <option value="HUF"{if $form.ccode=='HUF'} selected{/if}>HUF</option>
        <option value="CZK"{if $form.ccode=='CZK'} selected{/if}>CZK</option>
        <option value="ILS"{if $form.ccode=='ILS'} selected{/if}>ILS</option>
        <option value="MXN"{if $form.ccode=='MXN'} selected{/if}>MXN</option>
        <option value="NZD"{if $form.ccode=='NZD'} selected{/if}>NZD</option>
      </select>
</div>
<div class="gbBlock">
      <h3>
	{g->text text="PayPal Operating Mode"}
       </h3>
       <p class="giDescription">
	{g->text text="Live mode charges actual funds, and credits them to your account. Sandbox mode is for test purposes only."}
      </p>
      <select name="{g->formVar var="form[ppurl]"}">
        <option value="www.sandbox.paypal.com"{if $form.ppurl=='www.sandbox.paypal.com'} selected{/if}>{g->text text="Sandbox"} ({g->text text="no real transactions can occur"})</option>
        <option value="www.paypal.com"{if $form.ppurl=='www.paypal.com'} selected{/if}>{g->text text="Live"}</option>
      </select>
</div>
<div class="gbBlock">
<h3>
  {g->text text="Text of return to site button"}
 </h3>
  <p class="giDescription">
  {g->text text="Paypal will display a button for the customer to return to this site. You can choose with what text it is labelled."}
 </p>
  <input type="text" size="60" maxlength="60" name="{g->formVar var="form[pprtnbuttontext]"}" 
	value="{$form.pprtnbuttontext}"></textarea>
</div>
<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][save]"}"
     value="{g->text text="Save Settings"}"/>
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][cancel]"}"
     value="{g->text text="Cancel"}"/>
  </div>

