{*
 * $RCSfile: KeywordAnalysis.tpl,v $
 * $Revision: 1851 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<style type="text/css">
{literal}
.submit {
	background: transparent;
	border-top: 0;
	border-right: 0;
	border-bottom: 1px solid #00F;
	border-left: 0;
	color: #00F;
	display: inline;
	margin: 0;
	padding: 0;
}

*:first-child+html .submit {		/* hack needed for IE 7 */
	border-bottom: 0;
	text-decoration: underline;
}

* html .submit {				/* hack needed for IE 5/6 */
	border-bottom: 0;
	text-decoration: underline;
}
{/literal}
</style>

<div class="gbBlock gcBackground1">
  <h2 class="gbTitle"> {g->text text="Keyword Analysis"} </h2>
</div>


  <div class="gbAdmin">
<br>

    <h1>{g->text text="Warning:&nbsp;This page is under development so do not trust its results!"}</h1>

    {if count($baditems) > 0}
	{g->text text="The following items have keyword issues:"}

	<ol>
		{foreach from=$baditems item=baditempair}
			{assign var="baditem" value=$baditempair.item }
			{assign var="baditemparents" value=$baditempair.parent }
			<li>
			{foreach from=$baditemparents item=baditemparent}
				{$baditemparent->title|markup:strip|default:$parent.pathComponent}&nbsp;::&nbsp;
			{/foreach}

			{g->text text="%s" arg1=$baditem->title|markup:strip|default:$parent.pathComponent} :

			<a href="{g->url arg1="view=core.ShowItem"
						  arg2="itemId=`$baditem->id`"}">

	  	        {g->text text="View"}
	    		</a>
			&nbsp;
			<a href="{g->url arg1="view=core.ItemAdmin"
					 arg2="core.ItemEdit"
					 arg3="itemId=`$baditem->id`"}">

	  	        {g->text text="Edit"}
	    		</a>
		
			{assign var="missingkeywords" value=$baditempair.missing }
			{assign var="typos" value=$baditempair.typo }

			{if count( $missingkeywords ) > 0 }
				<br>{g->text text="Missing Keywords:"}
				<ul>
				{foreach from=$missingkeywords item=missingkeywordlist key=reason}
					<li>Found '{$reason}':
                                        <ul>
                                        {foreach from=$missingkeywordlist key=missingkeyword item=canset}
				            {if $canset}
						    <form action="{g->url}" method="post" id="userAdminForm"  enctype="{$UserAdmin.enctype|default:"application/x-www-form-urlencoded"}">
                                                        {g->hiddenFormVars}

                                                        <input type="hidden" name="{g->formVar var="controller"}" value="{$controller}"/>
                                                        <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>

                                                        <input type="hidden" name="{g->formVar var="form[itemId]"}" value="{$baditem->id}" />
                                                        <input type="hidden" name="{g->formVar var="form[changetype]"}" value="AddKeyword" />
                                                        <input type="hidden" name="{g->formVar var="form[keyword]"}" value="{$missingkeyword}" />
                                                    
                                                        <li>{$missingkeyword}&nbsp;<input type="submit" class="submit" name="{g->formVar var="form[action][save]"}" value="{g->text text="Add"}"/></li>
						    </form>
					    {else}
						    <form action="{g->url}" method="post" id="userAdminForm"  enctype="{$UserAdmin.enctype|default:"application/x-www-form-urlencoded"}">
                                                        {g->hiddenFormVars}

                                                        <input type="hidden" name="{g->formVar var="controller"}" value="{$controller}"/>
                                                        <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>

                                                        <input type="hidden" name="{g->formVar var="form[itemId]"}" value="{$baditem->id}" />
                                                        <input type="hidden" name="{g->formVar var="form[changetype]"}" value="AddKeyword" />
                                                        
                                                    
                                                        <li>{$missingkeyword}&nbsp;<input type="text" name="{g->formVar var="form[keyword]"}" value="{$missingkeyword}" />&nbsp;<input type="submit" class="submit" name="{g->formVar var="form[action][save]"}" value="{g->text text="Add"}"/></li>
						    </form>
					    {/if}
                                        {/foreach}
                                        </ul>
				{/foreach}
				</ul>
			{/if}

			{if count( $typos ) > 0 }
				<br>{g->text text="Typos:"}
				<ul>
				{foreach from=$typos item=typolist key=reason}
                                        {foreach from=$typolist item=typo key=type}
						    <form action="{g->url}" method="post" id="userAdminForm"  enctype="{$UserAdmin.enctype|default:"application/x-www-form-urlencoded"}">
                                                        {g->hiddenFormVars}

                                                        <input type="hidden" name="{g->formVar var="controller"}" value="{$controller}"/>
                                                        <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}"/>

                                                        <input type="hidden" name="{g->formVar var="form[itemId]"}" value="{$baditem->id}" />
                                                        <input type="hidden" name="{g->formVar var="form[changetype]"}" value="CorrectTypo" />
                                                        <input type="hidden" name="{g->formVar var="form[originalkeyword]"}" value="{$reason}" />
                                                        <input type="hidden" name="{g->formVar var="form[replacekeyword]"}" value="{$typo}" />
                                                    
                                                        <li>'{$reason}' should be '{$typo}'&nbsp;<input type="submit" class="submit" name="{g->formVar var="form[action][save]"}" value="{g->text text="Change"}"/></li>
						    </form>

                                        {/foreach}
				{/foreach}
				</ul>
			{/if}

			</li>
		{/foreach}
	</ol>

    {else}
	{g->text text="There are no items that have keyword problems."}
    {/if}	

  </div>
