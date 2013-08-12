<?php

/*
 * contains methods to search solr and display results.  depends on Apache_Solr_Php client.
 */

module_load_include('inc', 'islandora_solr_search', 'IslandoraSolrResults');

/**
 * Extention of IslandoraSolrResults for templating purposes only.
 * No changes have been made to the original class.
 */
class iLivesSolrResults extends IslandoraSolrResults {

  function searchAndDisplay($query, $startPage=1, $fq=null, $dismax=null) {

    $limit = variable_get('islandora_solr_search_num_of_results', 20);
    global $islandora_solrQuery, $islandora_solr_page_number, $base_url, $facetArray; //used by facet block as well
    $configs = explode('~', variable_get('islandora_solr_config_options', 'islandora_solr_search~IslandoraSolrResults.inc~IslandoraSolrResults~displayResults'));
    $classFile = $configs[1];
    module_load_include('inc', $configs[0], $configs[1]);
    $solrClass = $configs[2];
    $solrFunction = $configs[3];
    $pristineQuery = $query;
    $query = str_replace(IslandoraSolrResults::$slashReplacement, '/', $query);
    $islandora_solr_page_number = $startPage;
    $islandora_solrQuery = str_replace('/', IslandoraSolrResults::$slashReplacement, $query); //replace the slash so we can use this in a url without breaking drupal clean urls
    if (empty($islandora_solrQuery)) {
      $islandora_solrQuery = '%20'; //so we can allow empty queries to dismax
    }


    $url = variable_get('islandora_solr_search_block_url', 'localhost:8080/solr');
    $pathParts = parse_url($url);
    if (!$pathParts['port']) {
      $pathParts['port'] = 80;
    }
        $facetArray = array();
    $facetFields = '';
    $rawFacetVals = variable_get("islandora_solr_search_block_facets", 'dc.subject,dc.type');
    $lines = explode(',', $rawFacetVals);
    foreach($lines as $line){
      $lineArray = explode('~', $line);
      $key = trim($lineArray[0]);
      $value = trim($lineArray[1]);
      if(!$value){
        $value = $key;
      }
      $facetArray[$key] = $value;
      if($facetFields){
        $facetFields .= ",";
      }
      $facetFields .= $key;
    }
    $resultsAnnounce = 'Search Results -';
    $solr = new Apache_Solr_Service($pathParts['host'], $pathParts['port'], $pathParts['path'] . '/');
    $query = $this->parseQuery($solr, $query, $dismax, $solr); //fix the query as some characters will break the search : and / slash are examples
    $facetlimit = variable_get('islandora_solr_search_block_facet_limit', '12');
    $facetMinCount = variable_get('islandora_solr_search_block_facet_min_count', '2');
    $requestHandler = variable_get("islandora_solr_search_block_request_handler", t("standard"));
    $additionalParams = array(
      'facet' => 'true',
      'facet.mincount' => $facetMinCount,
      'facet.limit' => $facetlimit,
      'qt' => $requestHandler,
      'facet.field' => explode(',',$facetFields), 
    );
    $pagerRef = '/islandora/solr/search/' . htmlspecialchars(drupal_urlencode($pristineQuery), ENT_QUOTES, 'utf-8', false);


    //include specialized behaviour for index queries
    $pagerRef = '/islandora/solr/search/' . htmlspecialchars(drupal_urlencode($pristineQuery), ENT_QUOTES, 'utf-8', false);

    $queryParts = explode(' ', $query);
    foreach ($queryParts as $part) {

      $position = preg_match('/mods\.indexTitle/', $part);
      if ($position) {
        $resultsAnnounce = '';
        $letter = substr($part, 16, 1);
        $pagerRef = '/fedora/ilives/index/' . $letter;
        $additionalParams['sort'] = 'mods.indexTitle asc';
      }
      if (preg_match('/rels\.isMemberOf/', $part)) {
        $bookPid = $GLOBALS['bookPid'];
        $pagerRef = "/fedora/ilives_page_results/$bookPid/" . htmlspecialchars(drupal_urlencode($pristineQuery), ENT_QUOTES, 'utf-8', false);
      }
    }

    $debugQuery = (variable_get('islandora_solr_search_debug_mode', 0) ? "true" : null ); //any val incl. 'false' is treated as true by Solr

    global $islandora_fq;
    if ($fq != NULL && $fq != '-') {
      $fq = str_replace(IslandoraSolrResults::$slashReplacement, '/', $fq); //put the slash back
      if (strpos($fq, '-') === 0) {//remove the requested facet from the list
        $fq = substr($fq, strpos($fq, IslandoraSolrResults::$facetSeparator) + 1);
      }
      $fqs = $this->solr_escape($this->csv_explode(IslandoraSolrResults::$facetSeparator, $fq, '"', true)); //to filter by more then one facet we will separate them by~ for now
      $additionalParams['fq'] = $fqs;
      $islandora_fq = str_replace('/', IslandoraSolrResults::$slashReplacement, $fq); //remove the slash here as we will be using this in url's
    }
    if (empty($islandora_fq)) {
      $islandora_fq = '-';
    }
    global $islandora_defType;
    if ($dismax != NULL) {
      $islandora_defType = $dismax;
      $additionalParams['defType'] = $dismax;
    }
    $recordStart = $startPage - 1;
    $recordStart = max(0, $recordStart); //don't want a negative number
    $recordStart = $recordStart * $limit;
    try {
      $results = $solr->search($query, $recordStart, $limit, $additionalParams);
    } catch (Exception $e) {
      drupal_set_message(t('error searching ') . $e->getMessage());
    }
    if (empty($results)) {
      drupal_set_message(t('Error searching solr index. Is the solr search block configured properly?'), 'error');
      return ' ';
    }
    $total = (int) $results->response->numFound;
    if($total ==0){
      $resultsAnnounce = "No matches were found - try revising your query.";
    }
    $start = min(1, $total);
    $end = min(($limit + $recordStart), $total);

    global $islandora_endPage;



    $openingHref = '<li class="pager-item"><a href = "' . $base_url . $pagerRef;
    $closingHref = htmlspecialchars($islandora_fq, ENT_QUOTES, 'utf-8', false) . '/' . $dismax . '">';
    $islandora_endPage = ceil($total / $limit);
//if user requested a page past the end.. fix the start page and try again... bad user!
    if ($startPage > $islandora_endPage) {
      $startPage = $islandora_endPage;
      $recordStart = $startPage - 1;
      $recordStart = max(0, $recordStart); //don't want a negative number
      $recordStart = $recordStart * $limit;

      try {
        $results = $solr->search($query, $recordStart, $limit, $additionalParams);
      } catch (Exception $e) {
        drupal_set_message(t('error searching ') . $e->getMessage());
      }
      if (empty($results)) {
        drupal_set_message(t('Error searching solr index. Is the solr search block configured properly?'), 'error');
        return ' ';
      }
    }
    $islandora_startPage = $startPage;

    if (preg_match('/mods.indexTitle:.\*/', $query)) {
      $colonPos = strpos($query, ':');
      $letter - substr($query, $colonPos, 1);
      $openingHref = '<li class="pager-item"><a href = "' . $base_url . "/fedora/ilives/index/$letter*";
    }


    $output .= $resultsAnnounce;
    $pager = '<div class="item-list"><ul class = "pager">';
    if ($recordStart > 0) {
      $pager .='<li class="pager-previous"> <a href="' . $base_url . $pagerRef . '/' . max(0, ($startPage - 1)) . '/' . htmlspecialchars($islandora_fq, ENT_QUOTES, 'utf-8', false) . '/' . $dismax . '">‹ previous</a></li>';
    }
    if ($islandora_endPage > 1) {
      if ($islandora_startPage == 1) {
        $pager .= '<li class ="pager-current">1</li>';
      }
      else {
        $pager .=$openingHref . '/1/' . $closingHref . '1</a></li>';
      }
    }


    $i = max(2, $startPage - 3);

    if ($i > 2) {
      $pager .= '<li class ="pager-current">...&nbsp;</li>';
    }

    for (; $i < min($startPage + 3, $islandora_endPage); $i++) {
      if ($i != $startPage) {
        $pager .=$openingHref . '/' . $i . '/' . $closingHref . $i . '</a></li>';
      }
      else {
        $pager.= '<li class ="pager-current">' . $i . '</li>';
      }
    }

    if ($islandora_endPage > 5 && $islandora_startPage < ($islandora_endPage - 3)) {
      $pager .= '<li>&nbsp;...</li>';
    }


    if ($islandora_endPage > 1) {
      if ($islandora_startPage == $islandora_endPage) {
        $pager .= '<li class ="pager-current">' . $islandora_endPage . '</li>';
      }
      else {
        $pager .=$openingHref . '/' . $islandora_endPage . '/' . $closingHref . '' . $islandora_endPage . '</a></li>';
      }
    }


    if ($recordStart < ($total - $limit)) {
      $pager .='<li class="pager-next"><a href="' . $base_url . $pagerRef . '/' . ($startPage + 1) . '/' . htmlspecialchars($islandora_fq, ENT_QUOTES, 'utf-8', false) . '/' . $dismax . '">next ›</a></li>';
    }
    $pager .= '</ul></div>'; //added closing div for drupal paging here
    $output .= '<div class="islandora-solr-search">';

    if (class_exists($solrClass)) {
      $implementation = new $solrClass();
    }
    else {
      drupal_set_message(t('Error loading solr results class. Class could not be loaded. Check the Islandora Solr search blocks configuration'), 'error');
      return ' ';
    }

    if (method_exists($implementation, $solrFunction)) {
      $output.= $implementation->$solrFunction($results, $pager);
      if (variable_get('islandora_solr_search_debug_mode', 0)) { // debug dump
        drupal_set_message('Params: <br/><pre>' . print_r($additionalParams, true) . '</pre>', 'status');
      }
    }
    else {
      drupal_set_message(t('Error loading solr results class. Could not find the specified method.  Check the Islandora Solr search blocks configuration.'), 'error');
    }
    global $islandora_facets; //make facets available to the islandora facet block
    $islandora_facets = $results->facet_counts->facet_fields;
    $output .= '</div>';
    //$output = $pager . $output . $pager;
    return $output;
  }

//default implementation of display results can be changed in the blocks config
  function displayResults($results, $pager) {

    if (!preg_match('/<li/', $pager)) {
      $pager = "";
    }
    $query = $GLOBALS['islandora_solrQuery'];
    if (preg_match('/rels.isMemberOf:info/', $query)) {
      return $this->displayPageResults($results, $query, $pager);
    }
    return $this->displayListResults($results, $query, $pager);
  }

// function to be removed

  function displayListResults($results, $query, $pager) {
    $base = base_path();
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $output.= '<div class="item-list">';
    global $base_url;
    if (empty($results)) {
      return "no results";
    }

    foreach ($results->response->docs as $doc) {
      $resultsArray = array();
      foreach ($doc as $field => $value) {

        if (is_array($value)) {
          $value = implode(", ", $value);
        }
        $resultsArray[$field] = $value;
      }

      $pid = $resultsArray['PID'];
      $escapedPid = str_replace(':', '\:', $pid);
      $pidParts = explode(':', $pid);
      $title = $resultsArray['mods.title'];
      $sor = $resultsArray['sor'];
      $dateIssued = $resultsArray['dateIssued'];
      $collection = $resultsArray['collection'];
      $collection = "info\:fedora~slsh~ilives\:$escapedPid";



      if (preg_match('/tei./', $query)) {
        $pageQuery = str_replace('AND dc.type:(collection)', '', $query);

        $pageQuery = $query . " AND rels.isMemberOf:info\:fedora~slsh~" . $escapedPid;
        $pageQuery = htmlspecialchars(drupal_urlencode($pageQuery), ENT_QUOTES, 'utf-8', false);
      }
      if ($pageQuery) {
        $titleLink = "<a href='${base}fedora/ilives_page_results/$pid/$pageQuery'>$title</a>";
        $TNLink = "<a href='${base}fedora/ilives_page_results/$pid/$pageQuery'><img src='${base}fedora/repository/$pid/TN' alt='No image available' /></a>";
      }
      else {
        $titleLink = "<a href='${base}fedora/ilives/$pid'>$title</a>";
        $TNLink = "<a href='${base}fedora/ilives/$pid'><img src='${base}fedora/repository/$pid/TN' alt='No image available' /></a>";
      }
      $fullLink = l('Full Record', 'fedora/ilives/' . $pid, array('attributes' => array('class' => 'solr-fullrec')));
      $ingested = preg_match('/ingested/', $resultsArray['type']);
//added check for pdf 
 if(!$ingested){// do a second check wrap this in an if so we don't have to do it everytime
	  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
	 $item = new Fedora_Item($pid);
	 $datastreams = $item->get_datastreams_list_as_array();
    
	if(array_key_exists('PDF', $datastreams)){
		$ingested = TRUE;
	  }
	}

      if ($ingested) {
        $viewLink = l('Read Online', 'fedora/ilives_book_viewer/' . $pid, array('attributes' => array('class' => 'solr-readonline')));
      }
      else {
        $viewLink = 'Not yet digitized';
      }

      $borrowLink = l('Print availability?', 'http://islandpines.roblib.upei.ca/opac/en-CA/skin/roblib/xml/rdetail.xml?r=' . $pidParts[1], array('attributes' => array('class' => 'solr-borrow')));


      $output .= <<< html

    <div class="islandora-solr-list" >
        <table>
          <tr>
            <td rowspan="2" class="solr-tn-small">$TNLink</td>
            <th colspan="3"><span class="solr-title">$titleLink </span><span class="solr-details"> $sor $dateIssued</span></th>
          </tr>
          <tr>
            <td>$fullLink</td> <td>$viewLink</td><td>$borrowLink</td>
          </tr>
        </table>
    </div>
html;
    }

    if (variable_get('islandora_solr_search_debug_mode', 0)) { // debug dump
      $results_r .= "<pre>Results: " . print_r($results, true) . "</pre>";
      $fieldset_r = array(
        '#title' => t("Raw Results"),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#value' => $results_r,
      );
      $output .= theme('fieldset', $fieldset_r);
    }
    $output .='</div>';
    return $pager . $output . $pager;
  }

  function DdisplayListResults($results, $query, $pager) {
    $base = base_path();
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $output.= '<div class="item-list">';
    global $base_url;
    if (empty($results)) {
      return "no results";
    }

    foreach ($results->response->docs as $doc) {
      $resultsArray = array();
      foreach ($doc as $field => $value) {

        if (is_array($value)) {
          $value = implode(", ", $value);
        }
        $resultsArray[$field] = $value;
      }

      $pid = $resultsArray['PID'];
      $escapedPid = str_replace(':', '\:', $pid);
      $pidParts = explode(':', $pid);
      $title = $resultsArray['mods.title'];
      $sor = $resultsArray['sor'];
      $dateIssued = $resultsArray['dateIssued'];
      $collection = $resultsArray['collection'];
      $collection = "info\:fedora~slsh~ilives\:$escapedPid";

      if (preg_match('/tei./', $query)) {
        $pageQuery = str_replace('AND dc.type:(collection)', '', $query);

        $pageQuery = $query . " AND rels.isMemberOf:info\:fedora~slsh~$escapedPid/-/";
        $pageQuery = htmlspecialchars(drupal_urlencode($pageQuery), ENT_QUOTES, 'utf-8', false);
      }
      if ($pageQuery) {
        $titleLink = "<a href='${base}fedora/ilives_page_results/$pid/$pageQuery'>$title</a>";
        $TNLink = "<a href='${base}fedora/ilives_page_results/$pid/$pageQuery'><img src='${base}fedora/repository/$pid/TN' alt='No image available' /></a>";
      }
      else {
        $titleLink = "<a href='${base}fedora/ilives/$pid'>$title</a>";
        $TNLink = "<a href='${base}fedora/ilives/$pid'><img src='${base}fedora/repository/$pid/TN' alt='No image available' /></a>";
      }
      $fullLink = l('Full Record', 'fedora/ilives/' . $pid, array('attributes' => array('class' => 'solr-fullrec')));
      $ingested = preg_match('/ingested/', $resultsArray['type']);
      if ($ingested) {
        $viewLink = l('Read Online', 'fedora/ilives_book_viewer/' . $pid, array('attributes' => array('class' => 'solr-readonline')));
      }
      else {
        $viewLink = 'Not yet digitized';
      }
      $borrowLink = l('Print availability?', 'http://islandpines.roblib.upei.ca/opac/en-CA/skin/roblib/xml/rdetail.xml?r=' . $pidParts[1], array('attributes' => array('class' => 'solr-borrow')));

      $rows = array();
      $rows[] = array(
        array('data' => $TNLink, 'rowspan' => 2, 'class' => 'solr-tn-small'),
        array('data' => "<span class='solr-title'>$titleLink </span><span class='solr-details'> $sor $dateIssued</span>", 'colspan' => 3),
      );
      $rows[] = array($fullLink, $viewLink, $borrowLink);
      $output .= '<div class="islandora-solr-list" >' . theme_table(null, $rows) . '</div>';
    }
    if (variable_get('islandora_solr_search_debug_mode', 0)) { // debug dump
      $results_r .= "<pre>Results: " . print_r($results, true) . "</pre>";
      $fieldset_r = array(
        '#title' => t("Raw Results"),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#value' => $results_r,
      );
      $output .= theme('fieldset', $fieldset_r);
    }
    $output .='</div>';

    return $pager . $output . $pager;
  }



  /**
   *
   * @param Apache_Solr_Response $results
   * @param string $query
   * @param string $pager
   * @return string
   *
   * Presents page output of Full Text search
   */

  function displayPageResults($results, $query, $pager) {
    
    global $base_url;
    $rows = 7;
    $bookTitle = $GLOBALS['solr_title'];
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $imageAvailable = FALSE;
    $objectHelper = new ObjectHelper;
    preg_match_all('/tei.fullText:.*\) /', $query, $matches);
    foreach ($matches[0] as $match) {
      $terms .= "$match ";
    }
    $terms = str_replace('tei.fullText:', '', $terms);
    $terms = str_replace('(', '', $terms);
    $terms = str_replace(')', '', $terms);

    $terms = str_replace('tei.fullText:', '', $terms);
    if ($terms) {
      $terms = 'for ' . $terms;
    }
    foreach ($results->response->docs as $doc) {
      $resultsArray = array();
      foreach ($doc as $field => $value) {

        if (is_array($value)) {
          $value = implode(", ", $value);
        }
        $resultsArray[$field] = $value;
      }
      $caption = $resultsArray['mods.caption'];
      $pid = $resultsArray['PID'];
      if(!$imageAvailable){
        $TN = $objectHelper->getStream($pid, 'TN');
        if($TN){
          $imageAvailable = TRUE;
        }
      }
      if ($imageAvailable) {
        $imageTag = "<img src='$base_url/fedora/repository/$pid/TN' alt='No image available' />";
      }
      else {
        $imageTag = "No image available";
      }

      $TN = "<a href='/fedora/ilives_book_viewer/$pid/'>$imageTag</a>";
      $pidTD = "<a href='/fedora/ilives_book_viewer/$pid/'>$caption</a>";
      $tdArray[] = "<td><div class = 'fedora-item'><div class='solr-tn-smallwidth'>$TN</div><div class= 'solr-field-title'>$pidTD</div><div class='clear'></div></div></td>";
    }

    if ($tdArray) {
      sort($tdArray);


      $output = "<h3>Searched $bookTitle $terms </h3>";
      $output .= '<table class="solr-grid"><tbody>';
      $colCount = 0;
      foreach ($tdArray as $td) {
        if ($colCount == 0) {
          $output .= "<tr>";
        }
        $output .= $td;
        if ($colCount == $rows - 1) {
          $colCount = -1;
          $output .= '</tr>';
        }
        $colCount++;
      }

      $reversed = strrev($output);
      if (!preg_match('/^>rt\/</', $reversed)) {
        for ($extra = 0; $extra < $rows - $colCount; $extra++) {
          $output .= '<td class="solr-blank"> </td>';
        }
        $output .= '</tr>';
      }
    }

    $output .= '</tbody></table>';
    return $pager . $output . $pager;
  }
    function build_solr_search_form($repeat = NULL, $pathToSearchTerms = NULL, $query = NULL) {
    global $islandora_solrQuery;
    if (!$query){
      $query = $islandora_solrQuery;
    }
    if (preg_match('/tei.fullText:\("/', $query)){
      $query = "";
    }
    $query = str_replace("AND dc.type:collection AND -type:image", "", $query);
    $query = preg_replace('/\)|\(/', '', $query);

    $types = array();
    $terms = trim(variable_get('islandora_solr_searchterms', 'dc.title ~ Title,dc.subject ~ Subject'));
    $termsArray = preg_split('/[\n]/', $terms);
    foreach ($termsArray as $term) {
      $vals = split('~', $term);
      if (!$vals[1]) {
        $vals[1] = $vals[0];
      }
      $types[trim($vals[0])] = trim($vals[1]);
    }
    $queryArray = NULL;
    if (isset($query)) {
      $queryArray = preg_split('/( OR | AND )/', $query, 0, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
      foreach ($queryArray as $key => $val) {
        if ($val === ' OR ' || $val === ' AND ') {
          $andOrs[] = trim($val); // that is, just 'OR' or 'AND'
          unset($queryArray[$key]); //remove that from the array
        }
      }
      $queryArray = array_values($queryArray);
    }

    $andOrArray = array(
      'AND' => 'and',
      'OR' => 'or',
    );
    $form = array();

    if (!isset($repeat)) {
      $repeat = variable_get('islandora_solr_search_block_repeat', t('3'));
    }
    $var0 = explode(':', $queryArray[0]);
    $form['search_type']['type1'] = array(
      '#title' => t(''),
      '#type' => 'select',
      '#options' => $types,
      '#default_value' => trim($var0[0])
    );
    $form['fedora_terms1'] = array(
      '#size' => '24',
      '#type' => 'textfield',
      '#title' => t(''),
      '#default_value' => (count($var0) >= 2 ? trim($var0[1], '()') : ''),
    );
    if ($repeat > 1 && $repeat < 9) { //don't want less then 2 or more then 9
      for ($i = 2; $i < $repeat + 1; $i++) {
        $t = $i - 1;
        $j = $i - 2;
        $andorj = ( isset($andOrs[$j]) ? $andOrs[$j] : 'AND' );
        $field_and_term = explode(':', $queryArray[$t]);
        $form["andor$t"] = array(
          '#title' => t(''),
          '#type' => 'select',
          '#default_value' => $andorj,
          '#options' => $andOrArray
        );
        $form['search_type']["type$i"] = array(
          '#title' => t(''),
          '#type' => 'select',
          '#options' => $types,
          '#default_value' => trim($field_and_term[0])
        );
        $form["fedora_terms$i"] = array(
          '#size' => '24',
          '#type' => 'textfield',
          '#title' => t(''),
          '#default_value' => (count($field_and_term) >= 2 ? trim($field_and_term[1], '()') : ''),
        );
      }
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('search')
    );
    return $form;
  }

  function build_simple_solr_form() {
    global $islandora_solrQuery;
    preg_match('/tei.fullText:\(".*"\) /', $islandora_solrQuery, $matches);
    $term = $matches[0];
    $term = str_replace('tei.fullText:("', "", $term);
    $term = str_replace('")', "", $term);

    $form["islandora_simple_search_query"] = array(
      '#size' => '24%',
      '#type' => 'textfield',
      '#title' => t(''),
      '#default_value' => $term,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('search')
    );
    return $form;
  }

}

?>
