{*
 * $Revision: 1576 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
{*
 * These are the variables used to display the help for the admin pages
 * Part of the Google Map module
 *}
{* Note: This file must be included in a template inside a <script> block *}
/****************** Global Variables **************************************************/
var _HP_Default = '{g->text text="This has no help yet." forJavascript=true}';

/****************** Variables for the Site Admin General Tab **************************/
var _HP_KeyProfile = '{g->text text="This allows you to setup different keys in case your website is accessed via different addresses.<br/>The program will select the right key automatically provided that it was created.<br/><br/><b>For Example: </b>If users access your website via http://www.myweb.com and http://myweb.com, you need two separate keys.<br/><br/>Add Profile will add a new key for a specific server address.<br/><br/>To edit or delete a profile, just select it and click the button." forJavascript=true}';
var _HP_MapWidth = '{g->text text="This is the Width of the Map.<br/><br/>You have a choice between a size in <u>Pixels</u> or a size in <u>Percentage</u>.<br/><br/>Pixels will be an exact size, Percentage will be dependent on the window size and the things on the page that can reduce the size (sidebar, etc...)<br/><br/>Note: Percentage doesn't work very well with certain Themes." forJavascript=true}';
var _HP_MapHeight = '{g->text text="This is the Height of the Map.<br/><br/>You have a choice between a size in <u>Pixels</u> or a size in <u>Percentage</u>.<br/><br/>Pixels will be an exact size, Percentage will be dependent on the window size and the things on the page that can reduce the size (sidebar, etc...)<br/><br/>Note: Percentage doesn't work very well with certain Themes." forJavascript=true}';
var _HP_MapType = '{g->text text="The Map Type is the Default Map that will be displayed from Google.<br/><br/><b>Map</b> is a vector type with roads, boundaries, etc. similar to a paper map.<br/><br/><b>Satellite</b> will show imagery of the area (2 extra zoom levels are available for this Map Type)<br/><br/><b>Hybrid</b> will overlay the Map vector data on the satellite imagery." forJavascript=true}';
var _HP_ShowScale = '{g->text text="This will enable/disable the display of a scale automatically generated by the Google API on the bottom left corner of the map." forJavascript=true}';
var _HP_MapCenter = '{g->text text="These are the GPS coordinates of the location you would like to show at the center of the Map when first displayed.<br/><br/>The Get via a Map function allows you to click on a map to choose the center." forJavascript=true}';
var _HP_MapZoom = '{g->text text="This will set the default Zoom for the Map when first displayed." forJavascript=true}';
var _HP_AutoCZ = '{g->text text="This will automatically center and zoom the map to include all the items displayed.<br/><br/>It overrides the default Center and Zoom settings above." forJavascript=true}';
var _HP_ShowGE = '{g->text text="Select if you want to show the \"Export to Google Earth\" link for items.<br/> This is originally from the gps module that was included in this module. <br/><br/><a target=\"_new\" href=\"http://codex.gallery2.org/index.php/Gallery2:Modules:Gps\">More information on this module.</a>" forJavascript=true}';
var _HP_GMLink = '{g->text text="This lets you choose the behavior for the \"Show Google Map\" Link.<br/><br/><b>Disable the link</b> will remove the link from all locations in your Gallery. You will need to point to main.php?g2_view=map.ShowMap to view the map.<br/><br/><b>Show The Default Map</b> will simply show all items available in your gallery that have GPS coordinate set.<br/><br/><b>Dynamic Link</b> will show either the default map or a map filtered on the current album depending if there are items in the current album / sub-album.<br/>This adds some processing on the server and can be slow depending on your gallery size.<br/><br/><b>Show Map Filtered on current album</b> will always filter the map on the current album.<br/> If there are no items in the current album or sub-albums with GPS coordinates it will show a \"friendly\" error message." forJavascript=true}';
var _HP_AdminHelp = '{g->text text="This enables/disables the help on the admin pages." forJavascript=true}';
var _HP_UserHelp = '{g->text text="This enables/disables the help on the user pages." forJavascript=true}';
var _HP_DefCenter = '{g->text text="This sets the default center for the \"Get via a Map\" operations.<br/><br/>This is particularly useful if all of your markers will be in the same area." forJavascript=true}';
var _HP_DefZoom = '{g->text text="This sets the default zoom for the \"Get via a Map\" operations." forJavascript=true}';
var _HP_F_GoogleOverview = '{g->text text="This will enable/disable the Google Overview Settings tab.<br/><br/>The settings for the Overview Map from the Google API can be controlled on this tab." forJavascript=true}';
var _HP_F_GZoom = '{g->text text="This will enable/disable the GZoom Settings tab.<br/><br/>For more information, <a target=\"_new\" href=\"http://earthcode.com/blog/2006/07/gzoom.html\">click here</a>." forJavascript=true}'
var _HP_F_Theme = '{g->text text="This enables/disables the Theme Settings tab.<br/><br/>On this tab 3 things can be set:<br/><br/><b>Controls</b>: Choose between custom or built-in controls and select the position of the map controls.<br/><br/><b>InfoWindow</b>: The InfoWindows can be changed by selecting a template from a dropdown.<br/><br/><b>Thumbnail Bar</b>: The Thumbnail bar is a bar with small version of the pictures currently displayed on the map. Its size and position can be customized." forJavascript=true}';
var _HP_F_Marker = '{g->text text="This enables/disables the Markers Settings tab.<br/><br/>On this tab, you can:<br/><br/>Create a new marker set to be used for the map.<br/><br/>Select which marker set to use for photos, album or groups.<br/><br/>Enable or disable the automatic regroup feature." forJavascript=true}';
var _HP_F_Legend = '{g->text text="This enables/disables the Legend Management tab.<br/><br/>This is where you enter the legend to associate text with each marker color.<br/><br/>Also, you can enable and position the Legend Block." forJavascript=true}';
var _HP_F_Filter = '{g->text text="This enables/disables the Filter Management tab.<br/><br/>This permits you to enable and control filters. There are 2 types of filters:<br/><br/><b>Manual Filters</b> that simply lets you choose a center and a zoom on the map to show a specific area when selected.<br/><br/><b>Album Filters</b> are created automatically on the fly (when enabled) and display only items contained in the album and its subalbums. You can also choose how many subalbum levels to include." forJavascript=true}';
var _HP_F_Group = '{g->text text="This enables/disables the Group Management tab.<br/><br/>This allows you to create Groups which are items with GPS coordinates and a thumbnail picture. Each group can \"contain\" other groups, photos and albums.<br/><br/>When a group is selected on the map, it will show only the items contained into that group (it's like manual albums).<br/><br/>To see a group you have two choices: Either click on it on the default map and then click the thumbnail or else go directly to it via the address bar with g2_view=map.ShowMap&g2_group=G1 (for Group 1)." forJavascript=true}';
var _HP_F_Routes = '{g->text text="This enables/disables the Routes Management tab.<br/><br/>Routes link existing album, photos and groups in a path. You can select as many items as you'd like to be part of that route and select their order." forJavascript=true}';
var _HP_KP_Name = '{g->text text="This is simply the name of the profile. It will appear in the list of keys when it's fully created." forJavascript=true}';
var _HP_KP_Server = '{g->text text="Enter the URL of your server as users will access it.<br/><br/><b>For example:</b> if users access your server via http://myserver.mydomain.com, enter http://myserver.mydomain.com" forJavascript=true}';
var _HP_KP_GKey = '{g->text text="This is the key that Google gives you for your server URL; copy from the Google signup site and paste it here.<br/><br/>The Google signup site is here:<br/><a target=\"_new\" href=\"http://www.google.com/apis/maps/signup.html\">http://www.google.com/apis/maps/signup.html</a>" forJavascript=true}';

/****************** Variables for the Site Admin GZoom Tab **************************/
var _HP_GZ_Enable = '{g->text text="This allows you to enable/disable the GZoom feature.<br/><br/>If you enable this, you will see a Zoom Control on your Map. Instructions are provided when you select it." forJavascript=true}';
var _HP_OV_Enable = '{g->text text="This enables/disables the Overview Map that Google has implemented in Google Maps API version 2.<br/><br/>It will show a zoomed out version of the map on a corner (of your choice) so you can tell where you are on the \"global\" map." forJavascript=true}';
var _HP_OV_Size = '{g->text text="The size of the Overview Map window on the corner of the main map.<br/><br/>By default it is 200px x 200px." forJavascript=true}';
var _HP_OV_Position = '{g->text text="This is the position of the Overview Map.<br/><br/>By default it's on the bottom right-hand corner of the map.<br/><br/>Also, you can configure the window offset in pixels." forJavascript=true}';

/****************** Variables for the Site Admin Theme Tab **************************/
var _HP_T_ControlType = '{g->text text="This will let you choose which set of Controls to use on the map.<br/><br/>The controls include buttons to change the map Type (MAP, HYBRID, SATELLITE), Zoom In & Out and Pan the map.<br/><br/>By default it uses the Stock - Small which are included in the Google Map API." forJavascript=true}';
var _HP_T_ControlPosition = '{g->text text="This selects the position of the Controls. The default Anchor is one of the corners.<br/><br/>When an anchor is selected, you can also set an offset from that specific corner in pixels.<br/><br/>This is only for the zoom in, zoom out, zoom slider and panning controls." forJavascript=true}';
var _HP_T_ControlMapType = '{g->text text="This determines if the Map Type selector is shown.<br/>If this is activated, users will have the ability to change the map type between: MAP, SATELLITE or HYBRID." forJavascript=true}';
var _HP_T_InfoWindowTemplate = '{g->text text="This allows you choose the InfoWindow template to use for all items.<br/><br/>You can create new templates by following the examples in modules/map/templates/infowindow/; each template is an .inc file.<br/><br/>The InfoWindows are the bubbles that open up when you click a marker item on the map." forJavascript=true}';
var _HP_T_DefaultZoomIn = '{g->text text="When editing an item, you can add GPS coordinates, a zoom value and a color for the marker.<br/><br/>The zoom-in value is used by the Mini Map block and by links on the InfoWindows if activated below.<br/><br/>This lets you set a default zoom level in case a value was not set by the user. A zoom level of 5 is a good default value." forJavascript=true}';
var _HP_T_ZoomInLinks = '{g->text text="This will show the zoom-in value in the InfoWindow if the template supports it. All the included templates support this feature.<br/><br/>The link is Javascript and simply changes the zoom level of the map when clicked." forJavascript=true}';
var _HP_T_ItemSummaries = '{g->text text="This will show the item Summary (or short description) in the InfoWindow if the template supports it. All the included templates support this feature." forJavascript=true}';
var _HP_T_ItemDescription = '{g->text text="This will show the item Description in the InfoWindow if the template supports it. All the included templates support this feature." forJavascript=true}';
var _HP_T_ThumbnailBarPos = '{g->text text="This will show a bar of thumbnails for the items that are curently shown on the map.<br/><br/>You can select the visibility and position of the Thumbnail Bar (top, bottom, left, right).<br/><br/>By default it is hidden." forJavascript=true}';
var _HP_T_ThumbnailBarSize = '{g->text text="This controls the size of the Thumbnail Bar.<br/><br/>If the Thumbnail Bar is on the top or bottom of the map, the size will control the height of the bar (the width matches the map width).<br/><br/>If the Thumbnail Bar is on the left or right of the map, the size will control the width of the bar (the height matches the map height)." forJavascript=true}';

/****************** Variables for the Site Admin Legend Tab **************************/
var _HP_L_LegendPos = '{g->text text="This is the Legend Position.<br/><br/>If Left is selected it will be shown in the Sidebar (doesn't work with Gallery v2.2+).<br/><br/>If Top or Bottom is selected, it will be flat and will not have the expand feature explained below.<br/><br/>By default the Legend is hidden." forJavascript=true}';
var _HP_L_LegendPhoto = '{g->text text="This activates the Photo marker legend. Once activated, the colored markers will appear below with text boxes for descriptions.<br/><br/>If you enter a description for a color that does not have any items associated with it, it will still appear in the legend." forJavascript=true}';
var _HP_L_LegendPhotoExp = '{g->text text="When the Legend is used horizontally (left or right position), the sections can be expanded/collapsed. This option will make the Photo section expanded by default." forJavascript=true}';
var _HP_L_LegendAlbum = '{g->text text="This activates the Album marker legend (see Photo legend for more info)." forJavascript=true}';
var _HP_L_LegendAlbumExp = '{g->text text="This will make the Album section expanded by default (see Photo legend for more info)." forJavascript=true}';
var _HP_L_LegendGroup = '{g->text text="This activates the Group marker legend (see Photo legend for more info)." forJavascript=true}';
var _HP_L_LegendGroupExp = '{g->text text="This will make the Group section expanded by default (see Photo legend for more info)." forJavascript=true}';

/****************** Variables for the Site Admin Filter Management Tab **************************/
var _HP_FI_BlockPos = '{g->text text="This is the Filter Block Position<br/><br/>If Left is selected it will be shown in the Sidebar (doesn't work with Gallery v2.2+).<br/><br/>If Top or Bottom is selected, it will be flat.<br/><br/>By default it is hidden." forJavascript=true}';

var _HP_FI_ManualFilter = '{g->text text="This allows you to add Manual filters.<br/><br/>Manual filters are a set of coordinates and a zoom level to center the map on a specific area; it doesn't change the Map data.<br/><br/>You can add, remove or edit filters; Add or Edit will show the same window." forJavascript=true}';

var _HP_FI_AlbumFilter = '{g->text text="Album Filters will create a filter for each album that has items (or sub-items) with GPS coordinates.<br/><br/>Selecting an Album Filter will show a map with only items from that album, similar to \"Show Album on a Map\"" forJavascript=true}';
var _HP_FI_AlbumFilterDepth = '{g->text text="This controls the depth of sub-albums to show.<br/><br/>The Album Map will not show items that are nested deeper than the level entered.<br/><br/>By default, there is no limit to the depth." forJavascript=true}';
var _HP_FI_AlbumFilterDepthRoot = '{g->text text="This overrides the above settings for the Root album and limits the Album Map to show only top-level items included in the Root album and excludes items in sub-albums." forJavascript=true}';

/****************** Variables for the Site Admin Add/Edit Filter **************************/
var _HP_FI_AddName = '{g->text text="This is the name of the filter which will be shown in the dropdown inside the filter block." forJavascript=true}';
var _HP_FI_AddGPS = '{g->text text="These are the coordinates on which to center the map for this specific filter. Selecting the coordinates from a map will set both the coordinates and zoom." forJavascript=true}';
var _HP_FI_AddZoom = '{g->text text="This is the zoom to use when the filter is selected. It can be chosen from a map directly (see below)." forJavascript=true}';

/****************** Variables for the Site Admin Add/Edit Group **************************/
var _HP_G_Name = '{g->text text="This is the name and title of the Group.<br/><br/>It will be displayed on the top of the InfoWindow as well as in the tooltip when the mouse is on the marker." forJavascript=true}';
var _HP_G_Date = '{g->text text="This the date of the item to be displayed in the InfoWindow." forJavascript=true}';
var _HP_G_Summary = '{g->text text="This is the \"Summary\" in the Gallery sense.<br/><br/>This will show in the InfoWindow if configured in the Theme Settings." forJavascript=true}';
var _HP_G_Thumb = '{g->text text="Select a Thumbnail for this group.<br/><br/>The picture will be shown in the InfoWindow sized to fit.<br/><br/>Only JPG images seem to work correctly." forJavascript=true}';
var _HP_G_Description = '{g->text text="This is the \"Description\" in the Gallery sense.<br/><br/>This will show in the InfoWindow if configured in the Theme Settings." forJavascript=true}';
var _HP_G_GPS = '{g->text text="These are the coordinates at which the Group Marker will show up on the map.<br/><br/>The coordinates and zoom can be chosen from a map." forJavascript=true}';
var _HP_G_ZoomIn = '{g->text text="This is the zoom level at which the map will be displayed when the Zoom In link is clicked.<br/><br/>The Zoom In link will be shown in the InfoWindow if configured in the Theme Settings." forJavascript=true}';
var _HP_G_Color = '{g->text text="This is the color of the marker.<br/><br/>The list is created from the available colors for the Group Marker in the Marker Tab." forJavascript=true}'
var _HP_G_Items = '{g->text text="Check the items you want to display on the map when the group is selected.<br/><br/>If no items are selected, an error will be displayed when the group thumbnail is clicked. Pink items are photos, green items are albums and blue items are other groups." forJavascript=true}';

/****************** Variables for the Site Admin Route Management **************************/
var _HP_R_MainHelp = '{g->text text="According to the Google API documentation, if you want the routes (also known as polylines) to work properly in Internet Explorer, you must have the proper attributes in the web page HTML tag.<br/><br/>If you don't have this setup properly, the map will not work or the routes will not show up when viewed in Internet Explorer.<br/><br/>This button will change theme.tpl of the default Gallery theme to modify the HTML tag. A backup of the file called theme.bak will be saved in the same place.<br/><br/>If your Gallery becomes unusable after clicking this, restore theme.bak to theme.tpl (via FTP or shell access) to get everything back to normal.<br/><br/>You can then make the change by hand (see Wiki) which is a bit safer." forJavascript=true}';

var _HP_R_ActivateNumbers = '{g->text text="This will put numbers in each icon that belongs to a route.<br><br>Currently, this requires ImageMagick to be functional, otherwise the markers cannot be created and will not be shown.<br><br>If you're getting no markers at all, verify that the web server has rights to write to modules/map/images/routes/." forJavascript=true}'

/****************** Variables for the Site Admin Add/Edit Route **************************/
var _HP_R_Name = '{g->text text="This is a name for your route. It will be used on the Routes management page and shown in the filter list if \"Filter\" is selected below." forJavascript=true}';
var _HP_R_Color = '{g->text text="This is the color for the lines that will show as a route on the map.<br/><br/>The default is purple.<br/><br/>Click on the color to pick from a palette or enter the RGB hex value in the text box." forJavascript=true}';
var _HP_R_Size = '{g->text text="This is the width of the line in pixels.<br/><br/>The width is 3 pixels by default." forJavascript=true}';
var _HP_R_Opacity = '{g->text text="Change the opacity to adjust the transparency of the route line.<br/><br/>It is a number between 0 and 1, with 1 being the default.<br/><br/>0 is invisible and 1 is completely opaque." forJavascript=true}';
var _HP_R_Enabled = '{g->text text="Disabling the route allows you to remove it from the map without deleting the route itself." forJavascript=true}';
var _HP_R_Filter = '{g->text text="This setting will create a filter for this route which will appear in the filter block.<br/><br/>When the filter is selected it will auto-zoom and center the map to display items in the route." forJavascript=true}';
var _HP_R_items = '{g->text text="Select the items to include in this route and the order in which the items should appear. The markers for these items will be created dynamically to add a number for the item in the route sequence.<br/><br/>You must select at least 2 items to create a route.<br/><br/>Pink items are photos, green items are albums and blue items are groups." forJavascript=true}';

/**************** Variables for the Site Admin / Marker Setting ************************/
var _HP_M_Manage = '{g->text text="This allows you to see what markers are available.<br/><br/>Click View All to see all available marker sets.<br/><br/>Select a markerset from the dropdown and Click Delete to remove it.<br/><br/>Click Add to create a new marker set." forJavascript=true}'

var _HP_M_DefPhoto = '{g->text text="This is the Marker Set for Photos and also the default color if none is selected for a specific photo." forJavascript=true}'

var _HP_M_DefAlbum = '{g->text text="This is the Marker Set for Albums and also the default color if none is selected for a specific album." forJavascript=true}'

var _HP_M_DefGroup = '{g->text text="This is the Marker Set for Groups and also the default color if none is selected for a specific group." forJavascript=true}'

var _HP_M_UseParent = '{g->text text="When this is checked, an item without a color selected will use its parent album's color.<br><br>If the parent has no color selected, then it will use the default color above.<br><br>This allows you to use a specific color for an entire album." forJavascript=true}'

var _HP_M_AlbumRegroup = '{g->text text="When this is checked, all items close to their parent album will be hidden.<br><br>When the zoom level is high enough to show all of the children items, the album item will not shown on the map." forJavascript=true}'

var _HP_M_ItemRegroup = '{g->text text="When this is checked, all items too close together will be grouped into another item.<br><br>If the item is clicked, the correct zoom level will be used to show all items inside the group.<br><br>The regrouping happens dynamically when the zoom scale is changed.<br><br>This can slow down the map display if lots of items are present. (>1000)" forJavascript=true}'

var _HP_M_Distance = '{g->text text="This is the distance (in pixels) at which items get regrouped. This can be changed so that different icon sizes won't overlap.<br><br>The default is 15 pixels." forJavascript=true}'

var _HP_M_UploadNew = '{g->text text="This allows you to upload a new icon to use for the regrouped items." forJavascript=true}'

var _HP_M_IconForRegroup = '{g->text text="Select which icon you want to display on the map in place of the regrouped items.<br><br>This is only used if automatic regroup of items is enabled." forJavascript=true}'


/**************** Variables for the Site Admin / Marker Create ************************/
var _HP_MC_Base = '{g->text text="Select an existing base image from which to create a marker set.<br/><br/>The base color must be reddish in order for ImageMagick to properly process the colors. Otherwise, results will be unpredictable." forJavascript=true}'

var _HP_MC_UploadNew = '{g->text text="This allows you to upload a new icon from which to create a marker set.<br/><br/>Remember the color needs to be somewhat red/pink for the coloring to work correctly." forJavascript=true}'

var _HP_MC_SizeChange = '{g->text text="This allows you to adjust the size of all the markers created in the new set relative to the original (base) image used." forJavascript=true}'

var _HP_MC_ColorChange = '{g->text text="Here you can either choose all colors, or select specific colors. If you chooose specific colors, only those colors will be available in the color selection for each item." forJavascript=true}'

var _HP_MC_MarkerName = '{g->text text="This is the marker set name.<br/><br/>If the set already exists, you will be given the oportunity to overwrite or cancel." forJavascript=true}'

/**************** Variables for the User Item Admin ************************/

var _HP_U_GPS = '{g->text text="This is the GPS coordinates in the format latitude, longitude.<br><br>There are 4 options to get coordinates here:<br> - Enter them manually.<br><br> - Get them from the EXIF header (a button to get from EXIF header will show up if they're available).<br><br> - Enter an address and click 'Get Via Address'.<br><br> - Click the Get via map to manually select them.)" forJavascript=true}'

var _HP_U_ZoomLevel = '{g->text text="This is used with the 'Zoom-in' link.<br><br>If that option is activated in the Site Admin, it will show a link in the InfoWindow to zoom the map to the value entered for the item.<br><br>Higher values mean closer to the street level (from 0 to 19)."  forJavascript=true}'

var _HP_U_GELookAt = '{g->text text="This field allows you to customize the viewpoint for the item in Google Earth when the marker is double-clicked.<br><br>Enter the parameters in the format: heading,tilt,range." forJavascript=true}'

var _HP_U_Color = '{g->text text="This is simply the color of the marker to use for that item.<br><br>If no color is selected, either the default marker color is chosen or (if available and enabled) the parent album color will be used." forJavascript=true}'

var _HP_U_GetViaMap = '{g->text text="This simply shows a map and allows you to click on it to define where the marker location and associated zoom level." forJavascript=true}'

var _HP_U_GetViaAddress = '{g->text text="Enter the address in the format <br> '202 North Lasalle, Chicago, IL' for example.<br><br>Then click the button to retrieve the coordinates. Be sure to click the save button to keep the coordinates." forJavascript=true}'

var _HP_U_GetViaExif = '{g->text text="This button only appears when an EXIF header is present and GPS coordinates are found in the photo header data.<br><br>Simply click the button to populate and save the coordinates for that item." forJavascript=true}'

var _HP_U_WriteToExif = '{g->text text="This will modify the photo and write the current coordinates to its EXIF header.<br><br>It can be used as a form of backup for the coordinates if they get lost." forJavascript=true}'