<div class="gbBlock">
  <h3> {g->text text="Puzzle Options"} </h3>
  <p class="giDescription">
  {if ($PuzzleOptions.enabled == "yes")}
    <input type="checkbox" id="PuzzleOptions_enabled"
           name="{g->formVar var="form[PuzzleOptions][enabled]}" checked="checked"/>
  {else}
    <input type="checkbox" id="PuzzleOptions_enabled"
           name="{g->formVar var="form[PuzzleOptions][enabled]"}"/>
  {/if}
  <label for="PuzzleOptions_enabled">
    {g->text text="Enable puzzles for this album."}
  </label>
  </p>

</div>
