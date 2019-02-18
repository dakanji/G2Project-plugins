<div class="gbBlock gcBackground1">
  <h2> {g->text text="User Info Page"} </h2>
</div>

<div class="gbTabBar">
  <span class="giSelected o"><span>
    {g->text text="User Info"}
  </span></span>

  <span class="o"><span>
    {capture name=getMoreLink}
    <a href="{g->url arg1="view=core.SiteAdmin" arg2="subView=userinfo.UserinfoSetup"}">{g->text text="Setup"}</a>
    {/capture}
    {$smarty.capture.getMoreLink}
  </span></span>

</div>

<div class="gbBlock">
  <p class="giDescription">
    {g->text text="This will display information about users when they Login, Logout, and Failed Login Attempts."}
  </p>
</div>


<div class="gbBlock gcBackground1">
<table border="1">

<tr>
	{if ($Userinfo.data)}
	<td align="center" colspan="6">{g->text text="Number of Records:"}{$Userinfo.numRowsCount}</td>
	{else}
	<td align="center" colspan="6">{g->text text="Number of Records: 0"}</td>
	{/if}
</tr>

{if ($Userinfo.data)}
<tr>
	<td align="center" colspan="6">
		{if !empty($pagination.jumpRange)}
		<div class="gsPages">
			{g->text text="Page:"}
			{assign var="lastPage" value=0}
			{foreach name=jumpRange from=$pagination.jumpRange item=page}
				{if ($page - $lastPage >= 2)}
					<span>
					{if ($page - $lastPage == 2)}
						<a href="{g->url params=$pagination.pageUrl arg1="page=`$page-1`"}">{$page-1}</a>
					{else}
						...
					{/if}
					</span>
				{/if}

				<span>
				{if ($pagination.currentPage == $page)}
					{$page}
				{else}
					<a href="{g->url params=$pagination.pageUrl arg1="page=$page"}">{$page}</a>
				{/if}
				</span>
				{assign var="lastPage" value=$page}
			{/foreach}
		</div>
		{else}
			  &nbsp;
		{/if}
	</td>
</tr>
{else}
<tr>
	<td></td>
</tr>
{/if}

{foreach name=currentPage from=$pagination.currentPage item=pageNum}
<tr>
	<th>ID
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=id" arg3="order=DES"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=id" arg3="order=ASC"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>User ID
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=userId" arg3="order=DES"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=userId" arg3="order=ASC"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>User Name
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=userName" arg3="order=DES"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=userName" arg3="order=ASC"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>IP Address
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=ipAddress" arg3="order=DES"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=ipAddress" arg3="order=ASC"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>Time Stamp
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=timeStamp" arg3="order=DES"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=timeStamp" arg3="order=ASC"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>Action
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=action" arg3="order=DES"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->url params=$pagination.pageUrl arg1="page=$pageNum" arg2="sort=action" arg3="order=ASC"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
</tr>
{foreachelse}
<tr>
	<th>ID
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>User ID
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>User Name
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>IP Address
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>Time Stamp
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
	<th>Action
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/down.gif"}"/></a>
		<a href="{g->text text="#"}"><img src="{g->url href="modules/userinfo/images/up.gif"}"/></a>
	</th>
</tr>
{/foreach}

{foreach from=$Userinfo.data item=id}
<tr>
	<td>{$id.id}</td>
	<td>{$id.userId}</td>
	<td>
		{if ($id.userId)}
			<a href="{g->url arg1="view=members.MembersProfile" arg2="userId=`$id.userId`"}" target="_blank">{$id.userName}</a>
		{else}
			{$id.userName}
		{/if}
	</td>
	<td><a href="{g->text text="#"}" onclick="window.open('{g->url href="modules/userinfo/lookupip.php?ip=`$id.ipAddress`"}','popup','width=300,height=120,scrollbars');">{$id.ipAddress}</a></td>
	<td>
		{capture name="date"}{g->date timestamp=$id.timeStamp style="datetime" format=$Userinfo.uiDateFormat}{/capture}
		{g->text text="%s" arg1=$smarty.capture.date}
	</td>
	<td>
		{if ($id.action == "Login")}
			<h2 class="giSuccess">{$id.action}</h2>
		{else}
			{if ($id.action == "Logout")}
				{$id.action}
			{else}
				{if ($id.action == "Failed Login")}
					<h2 class="giError">{$id.action}</h2>
				{else}
					{$id.action}
				{/if}			
			{/if}
		{/if}
	</td>
</tr>
{foreachelse}
<tr>
	<td colspan="6"><center><b>{g->text text="No records in Database"}</b></center></td>
</tr>
{/foreach}

</table>
</div>