{*
 * @author Dayo Akanji <http://dakanji.com>
 * @copyright 2011 Dayo Akanji
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You can get a copy of the GNU General Public License by visiting 
 * http://www.gnu.org or by writing to:
 * The Free Software Foundation, Inc.,
 * 51 Franklin Street - Fifth Floor, 
 * Boston, MA  02110-1301, 
 * USA.
 *}
 
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Refine Module Setting"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock">
	<h2 class="giSuccess">{g->text text="Setting saved successfully."}</h2>
</div>
{/if}

<div class="gbBlock">
	<p class="giDescription">
{g->text text="This plugin sanitises gallery html output."}
	</p>
	<table class="gbDataTable">
		<tr>
			<td>
			{g->text text="HTML Post Processing:"}
			</td>
			<td>
				<select id="{g->formVar var="form[refineMode]"}" name="{g->formVar var="form[refineMode]"}">
					<option value="0" {if $form.refineMode == 0} selected="selected"{/if}>Rationalised</option>
					<option value="1" {if $form.refineMode == 1} selected="selected"{/if}>Beautified</option>
					<option value="2" {if $form.refineMode == 2} selected="selected"{/if}>Minified</option>
				</select>
			</td>
		</tr>
	</table>
</div>

<div class="gbBlock gcBackground1">
	<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}" />
	<input type="submit" class="inputTypeSubmit" name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
</div>