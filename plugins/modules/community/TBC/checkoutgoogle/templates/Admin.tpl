{*
 * $Revision: 1687 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
 <div class="gbBlock gcBackground1">
  <h2>{g->text text="Google Checkout Settings"}</h2>
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
	{g->text text="Google Checkout Merchant Id and Key"}
      </h3>
      <p class="giDescription">
	{g->text text="This is your Google Checkout account identifier. If you get this wrong, someone else will get the money!"}
      </p>
      <input type="text" size="50" name="{g->formVar var="form[gcMerchantId]"}" value="{$form.gcMerchantId}">
       <p class="giDescription">
	{g->text text="This is your Google Checkout Merchant Key."}
      </p>
      <input type="text" size="80" name="{g->formVar var="form[gcMerchantKey]"}" value="{$form.gcMerchantKey}">
</div>
<div class="gbBlock">
       <h3>
	{g->text text="Sandbox Merchant Id and Key"}
      </h3>
      <p class="giDescription">
	{g->text text="This is your Google Checkout account identifier for testing in the Sandbox. You do not need to enter anything here unles you use want to use the Test mode."}
      </p>
      <input type="text" size="50" name="{g->formVar var="form[gcTestMerchantId]"}" value="{$form.gcTestMerchantId}">
       <p class="giDescription">
	{g->text text="This is your Google Checkout Sandbox Merchant Key."}
      </p>
      <input type="text" size="80" name="{g->formVar var="form[gcTestMerchantKey]"}" value="{$form.gcTestMerchantKey}">
</div>
<div class="gbBlock">
      <h3>
	{g->text text="Test the Secure notification URL"}
       </h3>
       <p class="giDescription">
	{g->text text="Google Checkout needs a secure (https://...) URL that reaches this Gallery installation to post payment notifications to."}
	{g->text text="Most hosting providers allow some kind of free SSL access although usually via a shared URL. Alternatively you can purchase and install an SSL certificate for your domain."}
	{g->text text="You can test your access to your gallery site via SSL here."}
      </p>
      <input type="text" size="60" name="{g->formVar var="form[gcCallbackURL]"}" value="{$form.gcCallbackURL}" size="60">&nbsp;&nbsp;<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][test]"}" value="{g->text text="Test notification URL"}"/>
      <p class="giDescription">
       {g->text text="If you are using the URL Rewrite Module rule for authorization in checkoutgoogle, you can override the detected path for the rewritten URL with the full version you have entered above."}
       {g->text text="After changing this setting you should go to the Rewrite Module admin page, make sure the Checkout Google rule is enabled, and resave that page."}
       </p>
       {g->text text="Override URL in URL Rewrite rule?"} <input type="checkbox" name="{g->formVar var="form[overrideURL]"}" value = "true" {if $form.overrideURL}checked{/if}>
      
    </div>
{if isset($status.testFailed)}
<div class="gbBlock"><h2 class="giError">
  {g->text text="The test failed. You cannot receive Google Checkout notifications at this url."}
</h2>
<pre>{$status.testResult}</pre>
</div>
{/if}
{if isset($status.testSuccessful)}
 <div class="gbBlock"><h2 class="giSuccess">
    {g->text text="The test was succesful. You can receive Google Checkout notifications at this url."}<br>
</h2>    {g->text text="Copy and paste the following line into the Google Checkout Settings -> Integration -> API Callback URL field:"}
<pre>{$status.testResult}</pre>
</div>
{/if}


<div class="gbBlock">
      <h3>
	{g->text text="Location and Currency"}
       </h3>
       <p class="giDescription">
	{g->text text="Google Checkout is for merchants location in the US and the United Kindom. Set your location here."}
      </p>
      <select name="{g->formVar var="form[gcLocation]"}">
        <option value="en_US"{if $form.gcCurrency=='USD'} selected{/if}>{g->text text="United States"}</option>
        <option value="en_GB"{if $form.gcCurrency=='GBP'} selected{/if}>{g->text text="United Kingdom"}</option>
      </select>
       <p class="giDescription">
	{g->text text="Google Checkout accepts the US Dollar and the UK Pound Sterling. Indicate here in which of these two currencies your products are priced."}
      </p>
      <select name="{g->formVar var="form[gcCurrency]"}">
        <option value="USD"{if $form.gcCurrency=='USD'} selected{/if}>{g->text text="US Dollar"}</option>
        <option value="GBP"{if $form.gcCurrency=='GBP'} selected{/if}>{g->text text="UK Pound Sterling"}</option>
      </select>
</div>
<div class="gbBlock">
      <h3>
	{g->text text="Google Checkout Operating Mode"}
       </h3>
       <p class="giDescription">
	{g->text text="Live mode charges actual funds, and credits them to your account. Test mode is for test purposes only, and requires a Google Checkout Sandbox account."}
	{g->text text="When in Test mode, only Admin users will be able to see and use the Google Checkout option."}
      </p>
      <select name="{g->formVar var="form[gcMode]"}">
        <option value="test"{if $form.gcMode=='test'} selected{/if}>{g->text text="Test"} ({g->text text="no real transactions can occur"})</option>
        <option value="live"{if $form.gcMode=='live'} selected{/if}>{g->text text="Live"}</option>
      </select>
</div>

<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][save]"}"
     value="{g->text text="Save Settings"}"/>
    <input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][cancel]"}"
     value="{g->text text="Cancel"}"/>
  </div>

