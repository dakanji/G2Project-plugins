{*
 * $Revision: 1649 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
--{$boundary}
{assign var=rendering value='text'}
Content-Type: text/plain; charset=utf-8;
Content-Transfer-Encoding: 8bit

{g->text text="Order Number:"} {$transactionId}

{$additionalText}

{g->text text="This is an email to let you know your order has been completed."}
{g->text text="The prints you ordered will be delivered shortly. Many thanks!"}

{*plugins*}
{foreach from=$emailPlugins item=plugin}
  {include file="gallery:`$plugin.emailTemplate`"  l10Domain=$plugin.l10Domain}
{/foreach}

--{$boundary}
Content-Type: text/html; charset=utf-8
Content-Transfer-Encoding: 8bit

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
{assign var=rendering value='html'}
<html>
<head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
	<title>{g->text text="Order information"}</title>
</head>
<body><br>
{g->text text="Order Number:"} {$transactionId}
<br>
<br>
<pre>{$additionalText}
</pre>
<br>
{g->text text="This is an email to let you know your order has been completed."}<br>
{g->text text="The prints you ordered will be delivered shortly. Many thanks!"}<br>
<br>
<br>

{*plugins*}
{foreach from=$emailPlugins item=plugin}
  {include file="gallery:`$plugin.emailTemplate`"  l10Domain=$plugin.l10Domain}
{/foreach}

</body>
</html>

--{$boundary}--
