{*
 * $Revision: 1099 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div class="gbBlock">
  <p class="giDescription">{g->text text="Bulk edits multiple item properties at a time."}</p>
  <p>{g->text text="Run recursively, editing items in subalbums:"} <input type="checkbox" name="{g->formVar var="form[recursive]"}" /></p>
</div>

{if isset($status.done) && $status.done == 1}
<div class="gbBlock">
  <h2 class="giSuccess">
  {g->text text="Property edit run successfully, %d items changed" arg1=$status.count}
  </h2>
</div>
{/if}

{if isset($form.error.noFilters)}
<div class="gbBlock">
  <h2 class="giError">
  {g->text text="No filters were selected"}
  </h2>
</div>
{/if}

<div class="gbBlock">
  <table class="gbDataTable">
    <tr>
      <th class="gmParamProperty">Property</th>
      <th class="gmParamActive">Active</th>
      <th class="gmParamOptions">Options</th>
    </tr>

    <tr class="gbEven">
      <td>Enumerate Filename</td>
      <td><input type="checkbox" name="{g->formVar var="form[property][enumfilename][active]"}" /></td>
      <td>

        <table class="gbDataTable">
            <tr>
                <td><label>Pattern:</label></td>
                <td>
                    <input type="text" size="15"
                        name="{g->formVar var="form[property][enumfilename][pattern]"}"
                        value="{$form.property.enumfilename.pattern}"/>
                    <p>Uses <a href="http://php.net/sprintf">sprintf</a> syntax. Two parameters are passed, the sequence and the file extension.</p>
                </td>
            </tr>
            <tr>
                <td><label>Start Number:</label></td>
                <td>
                    <input type="text" size="5"
                        name="{g->formVar var="form[property][enumfilename][startnum]"}"
                        value="{$form.property.enumfilename.startnum}"/>
                </td>
            </tr>
            <tr>
                <td><label>Uppercase Extension:</label></td>
                <td>
                    <input type="checkbox"
                        name="{g->formVar var="form[property][enumfilename][uppercase]"}" />
                </td>
            </tr>
        </table>

      </td>
    </tr>

    <tr class="gbOdd">
      <td>Blank Title</td>
      <td><input type="checkbox" name="{g->formVar var="form[property][blanktitle][active]"}" /></td>
      <td>
        <input type="checkbox" name="{g->formVar var="form[property][blanktitle][blank]"}" />
      </td>
    </tr>

    <tr class="gbEven">
      <td>Blank Summary</td>
      <td><input type="checkbox" name="{g->formVar var="form[property][blanksummary][active]"}" /></td>
      <td>
         <input type="checkbox" name="{g->formVar var="form[property][blanksummary][blank]"}" />
      </td>
    </tr>

    <tr class="gbOdd">
      <td>Blank Keywords</td>
      <td><input type="checkbox" name="{g->formVar var="form[property][blankkeywords][active]"}" /></td>
      <td>
        <input type="checkbox" name="{g->formVar var="form[property][blankkeywords][blank]"}" />
      </td>
    </tr>

    <tr class="gbEven">
      <td>Blank Description</td>
      <td><input type="checkbox" name="{g->formVar var="form[property][blankdescription][active]"}" /></td>
      <td>
        <input type="checkbox" name="{g->formVar var="form[property][blankdescription][blank]"}" />
      </td>
    </tr>

    <tr class="gbOdd">
      <td>Reset Timestamp</td>
      <td><input type="checkbox" name="{g->formVar var="form[property][resettimestamp][active]"}" /></td>
      <td>
        <input type="checkbox" name="{g->formVar var="form[property][resettimestamp][reset]"}" />
      </td>
    </tr>

    <tr class="gbEven">
      <td>Shift Timestamp</td>
      <td><input type="checkbox" name="{g->formVar var="form[property][shifttimestamp][active]"}" /></td>
      <td>
        <table>
            <tr>
                <td>Add seconds:</td>
                <td>
                    <input type="text" size="10"
                        name="{g->formVar var="form[property][shifttimestamp][shift]"}" />
                </td>
            </tr>
        </table>
      </td>
    </tr>

  </table>
</div>