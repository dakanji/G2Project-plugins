{*
 * $Revision: 1720 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="tagtree.LoadAlbumData"}

{if isset($block.tagtree)}
{assign var="data" value=$block.tagtree.LoadAlbumData}

<div class="{$class}">
  <div id="{$data.blockId|escape}" class="tagtree">
    <script type="text/javascript">
      // <![CDATA[

      webFXTreeConfig.rootIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/foldericon.png"}";
      webFXTreeConfig.openRootIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/openfoldericon.png"}";
      webFXTreeConfig.folderIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/foldericon.png"}";
      webFXTreeConfig.openFolderIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/openfoldericon.png"}";
      webFXTreeConfig.fileIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/foldericon.png"}";
      webFXTreeConfig.iIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/I.png"}";
      webFXTreeConfig.lIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/L.png"}";
      webFXTreeConfig.lMinusIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/Lminus.png"}";
      webFXTreeConfig.lPlusIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/Lplus.png"}";
      webFXTreeConfig.tIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/T.png"}";
      webFXTreeConfig.tMinusIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/Tminus.png"}";
      webFXTreeConfig.tPlusIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/Tplus.png"}";
      webFXTreeConfig.blankIcon = "{g->url href="modules/tagtree/lib/xloadtree/images/blank.png"}";
      webFXTreeConfig.usePersistence = {if $data.params.usePersistence}true{else}false{/if};

      (function() {ldelim}
        {assign var="rootAlbumId" value=$data.rootAlbumId}
        var tree = new WebFXLoadTree("{$data.rootAlbumTitle|escape:"quotes"}", "{g->url arg1="view=tagtree.NodeExpansionCallback" arg2="albumId=$rootAlbumId" htmlEntities=0}", "{g->url arg1="view=core.ShowItem" arg2="itemId=$rootAlbumId" htmlEntities=0}", "explorer");

        {if $data.params.treeExpandCollapse}
          var expandLink = document.createElement('a');
          expandLink.href = '#';
          expandLink.onclick = function(e) {ldelim} tree.expandChildren(); this.blur(); return false; {rdelim};
          expandLink.appendChild(document.createTextNode('{g->text text="Expand"|escape:"quotes"}'));

          var collapseLink = document.createElement('a');
          collapseLink.href = '#';
          collapseLink.onclick = function(e) {ldelim} tree.collapseChildren(); this.blur(); return false; {rdelim};
          collapseLink.appendChild(document.createTextNode('{g->text text="Collapse"|escape:"quotes"}'));

          var container = document.createElement('p');
          container.appendChild(expandLink);
          container.appendChild(document.createTextNode(' | '));
          container.appendChild(collapseLink);
          document.getElementById("{$data.blockId|escape:"quotes"}").appendChild(container);
        {/if}

        document.write(tree);
      {rdelim})();
      // ]]>
    </script>
  </div>
</div>

{/if}
