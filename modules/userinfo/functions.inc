function numPages($myView, $mySubView, $count, $myTotalRecords) {
	global $gallery;
	/*
	*   $urlGenerator =& $gallery->getUrlGenerator();
	*   $theme2['themeUrl'] =
	*       $urlGenerator->generateUrl(array('href' => 'themes/' . $this->getId()));
	*   if (!isset($theme2['pageUrl'])) {
	*       $theme2['pageUrl'] = array('view' => 'core.ShowItem', 'itemId' => $item->getId());
	*   }
	*/

	/* Variables to push through into the function */
	/*
	* $view = "core.SiteAdmin";
	* $subView = "userinfo.Userinfo3";
	* $count = "20";
	* $totalRecords = "24";
	*/

	$view = $myView;
	$subView = $mySubView;
	$count = $count;
	$totalRecords = $myTotalRecords;


	if (!isset($theme2['pageUrl'])) {
		$theme2['pageUrl'] = array(
'view' => $view,
'subView' => $subView);
	}


	/* If we care about pagination, then figure out the current page and total pages */
	$perPage = $count;

	$page = GalleryUtilities::getRequestVariables('page');
	if ($perPage == 0 || empty($page)) {
		$page = 1;


		/* Redirect to the new page */
		/*
		*       $redirect = $theme['pageUrl'];
		*       if ($page != 1) {
		*           $redirect['page'] = $page;
		*       }
		*       return array(null, array('redirect' => $redirect));
		*/
	}


		/* Store the total pages in the theme2 */
		$theme2['totalPages'] = ceil($totalRecords / $perPage);
		$theme2['currentPage'] = $page;

	/*
	*       if ($page > $theme2['totalPages'] && $theme2['totalPages'] > 0) {
	*           if ($theme2['totalPages'] != 1) {
	*               $redirect['view'] = 'core.SiteAdmin';
	*               $redirect['subView'] = 'userinfo.Userinfo3';
	*               $redirect['page'] = $theme2['totalPages'];
	*
	*               $results['redirect'] = $redirect;
	*
	*               return array(null, $results);
	*           }   
	*       }
	*/

	/*
	 * --------------------------------------------------------------------------------------
	 * 'jumpRange'
	 */
	if (isset($theme2['totalPages'])) {
		$page = GalleryUtilities::getRequestVariables('page');

		if (empty($page)) {
			$page = 1;
		}

		$windowSize = isset($params['pageWindowSize']) ? $params['pageWindowSize'] : 6;
		$jumpRange = array();
		$lowerPage = max($page - (int)($windowSize / 2), 1);
		$upperPage = min($page + (int)($windowSize / 2), $theme2['totalPages']);

		if ($upperPage == 0) {
			$upperPage = 1;
		}

		if ($upperPage == $theme2['totalPages']) {
			$lowerPage = max($upperPage - $windowSize, 1);
		} elseif ($lowerPage == 1) {
			$upperPage = min($lowerPage + ($windowSize -1), $theme2['totalPages']);
		}

		for ($i = $lowerPage; $i <= $upperPage; $i++) {
			$jumpRange[] = $i;
		}

		if ($lowerPage > 1) {
			array_unshift($jumpRange, 1);
		}

		if ($upperPage < $theme2['totalPages']) {
			$jumpRange[] = $theme2['totalPages'];
		}

		$theme2['jumpRange'] = $jumpRange;
	}

	return $theme2;
}

