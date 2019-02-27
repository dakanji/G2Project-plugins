{*
 * $Revision: 1253 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{if $map.ThumbBarPos eq "1" or $map.ThumbBarPos eq "2"}
<script type="text/javascript">
document.write('<div id="thumbs" class="thumbbar" style="{if $map.ThumbBarPos eq "1"}margin-bottom:{else}margin-top:{/if}4px;width: '+myWidth+'px;"><\/div>');
</script>
{/if}
{if $map.ThumbBarPos eq "3" or $map.ThumbBarPos eq "4"}
<script type="text/javascript">
document.write('<div id="thumbs" class="thumbbar gcBackground1" style="{if $map.ThumbBarPos eq "3"}position:absolute;right:{if ($map.LegendPos eq '2' and $map.LegendFeature neq '0' and ($map.AlbumLegend or $map.PhotoLegend or $map.regroupItems)) or ($map.FilterFeature neq '0' and isset($map.ShowFilters) and $map.ShowFilters eq '2')}205{else}10{/if}px;margin-left:4px;{/if}height: '+myHeight+'px;"><\/div>');
</script>
{/if}