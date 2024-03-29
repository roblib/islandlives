<?php

/*
 * contains methods to search solr and display results.  depends on Apache_Solr_Php client.
 */

/**
 * Initialize a pager for theme('pager') without running an SQL query.
 *
 * The nuts and bolts of this function are borrowed from z3950.module
 * and also apachesolr.module.
 *
 * @see pager_query()
 *
 * @param $total
 *  The total number of items found.
 * @param $limit
 *  The number of items you will display per page.
 * @param $element
 *  An optional integer to distinguish between multiple pagers on one page.
 *
 * @return
 *  The current page for $element. 0 by default if $_GET['page'] is empty.
 */
function islandora_solr_search_pager_init($total, $limit = 10, $element = 0) {
  global $pager_page_array, $pager_total, $pager_total_items;
  $page = isset($_GET['page']) ? $_GET['page'] : '';

  // Convert comma-separated $page to an array, used by other functions.
  $pager_page_array = explode(',', $page);

  // We calculate the total of pages as ceil(items / limit).
  $pager_total_items[$element] = $total;
  $pager_total[$element] = ceil($pager_total_items[$element] / $limit);
  $pager_page_array[$element] = max(0, min((int) $pager_page_array[$element], ((int) $pager_total[$element]) - 1));

  // return the current position
  return $pager_page_array[$element];
}

class IslandoraSolrResults {

  static $facetSeparator = '~'; //used to separate facets in url
  static $slashReplacement = '~slsh~'; // a pattern used to replace / in url's the slash breaks drupal clean url's
  public static $SEARCH_CLASS_ADVANCED_SEARCH_NUMBER_FIELDS = 5;

  function IslandoraSolrResults() {
    module_load_include('php', 'islandora_solr_search', 'Solr/Service');
  }

  function parseQuery($solr, $query, $dismax=false) {
    $query = str_replace(IslandoraSolrResults::$slashReplacement, '/', $query);   //can't have a slash in the url breaks drupal clean urls
    return $query;
  }

  //default implementation for solr search.  You can configure others in the block config settings


  function searchAndDisplay($query, $fq=null, $dismax=null) {
    $startPage = isset($_GET['page']) ? $_GET['page'] : 0;
    $limit = variable_get('islandora_solr_search_num_of_results', 20);
    global $islandora_solrQuery, $islandora_solr_page_number, $base_url, $facetArray; //used by facet block as well
    $configs = explode('~', variable_get('islandora_solr_config_options', 'islandora_solr_search~IslandoraSolrResults.inc~IslandoraSolrResults~displayResults'));
    $classFileInfo = pathinfo($configs[1]);
    $classFileName = $classFileInfo['filename'];
    $classFileType = $classFileInfo['extension'];
    module_load_include($classFileType, $configs[0], $classFileName);

    $solrClass = $configs[2];
    $solrFunction = $configs[3];
    $islandora_solr_page_number = $startPage;
    $islandora_solrQuery = str_replace('/', IslandoraSolrResults::$slashReplacement, $query); //replace the slash so we can use this in a url without breaking drupal clean urls
    if (empty($islandora_solrQuery)) {
      $islandora_solrQuery = '%20'; //so we can allow empty queries to dismax
    }

    $url = variable_get('islandora_solr_search_block_url', 'localhost:8080/solr');
    $pathParts = parse_url($url);
    $facetArray = array();
    $facetFields = '';
    $rawFacetVals = variable_get("islandora_solr_search_block_facets", 'dc.subject,dc.type');
    $lines = preg_split('/[,|\n|\r]/', $rawFacetVals);
    foreach ($lines as $line) {
      $lineArray = explode('~', $line);
      $key = trim($lineArray[0]);
      $value = trim($lineArray[1]);
      if (!$value) {
        $value = $key;
      }
      $facetArray[$key] = $value;
      if ($facetFields) {
        $facetFields .= ",";
      }
      $facetFields .= $key;
    }
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
      'facet.field' => explode(',', $facetFields), //comma separated list configured in the block config
    );


    $debugQuery = (variable_get('islandora_solr_search_debug_mode', 0) ? "true" : null ); //any val incl. 'false' is treated as true by Solr

    global $islandora_fq;
    if ($fq != NULL && $fq != '-') {
      $fq = str_replace(IslandoraSolrResults::$slashReplacement, '/', $fq); //put the slash back
      # THIS IS NO LONGER NECESSARY, OR USEFUL.
      # HYPHENS SHOULD BE USED FOR FACET EXCLUSION, NOT FILTER REMOVAL.
      #if(strpos($fq,'-')===0) {//remove the requested facet from the list
      #  $fq = substr($fq,strpos($fq,IslandoraSolrResults::$facetSeparator)+1);
      #}
      $fqs = $this->csv_explode(IslandoraSolrResults::$facetSeparator, $fq, '"', true); //to filter by more then one facet we will separate them by~ for nowseparate them by~ for now
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
    $recordStart = $startPage;
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
    $end = min(($limit + $recordStart), $total);

    // Initialize drupal-style pager
    islandora_solr_search_pager_init($total, $limit);

    // Render Drupal pager (top of search results)
    // $output .= theme('pager');
    $output .= '<h2>Search Results</h2>';
    $output .= '<div id="numresults">(' . ($recordStart + 1) . ' - ' . $end . ' of ' . $total . ')</div>';

    if (class_exists($solrClass)) {
      $implementation = new $solrClass();
    }
    else {
      drupal_set_message(t('Error loading solr results class. Class ' . $solrClass . ' could not be loaded. Check the Islandora Solr search blocks configuration.'), 'error');
      return ' ';
    }

    if (method_exists($implementation, $solrFunction)) {
      $output.= $implementation->$solrFunction($results);
      if (variable_get('islandora_solr_search_debug_mode', 0)) { // debug dump
        drupal_set_message('Params: <br/><pre>' . print_r($additionalParams, true) . '</pre>', 'status');
      }
    }
    else {
      drupal_set_message(t('Error loading solr results class. Could not find the specified method.  Check the Islandora Solr search blocks configuration.'), 'error');
    }

    // Render Drupal pager (bottom of search results)
    $output .= theme('pager');

    global $islandora_facets; //make facets available to the islandora facet block
    $islandora_facets = $results->facet_counts->facet_fields;
    return $output;
  }

//default implementation of display results can be changed in the blocks config

  function displayResults($results) {
    $items = array();
    $type = "ol";
    $title = null;
    $recordStart = $results->response->start;

    global $base_url;
    if (empty($results)) {
      return "no results";
    }

    foreach ($results->response->docs as $doc) {
      $rows = array();
      $row = 0;

      foreach ($doc as $field => $value) {
        $rows[$row][] = array(
          'data' => $field,
          'header' => true,
        );
        if (is_array($value)) {
          $value = implode(", ", $value);
        }

        if ($field == 'PID') {
          $l = l($value, 'fedora/repository/' . htmlspecialchars($value, ENT_QUOTES, 'utf-8'));
          $rows[$row][] = $l;
        }
        else {
          $rows[$row][] = $value;
        }
        $row++;
      }
      $items[] = theme_table(null, $rows, array('class' => 'islandora_solr_search_results_object'));
    }
    $output .= theme_item_list($items, $title, $type, array('class' => 'stuff', 'start' => ($recordStart + 1)));
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
    return $output;
  }

//called from the islandora facet block

  function displayFacets() {
    global $islandora_facets, $islandora_defType, $islandora_solrQuery, $base_url, $islandora_fq, $facetArray;
    $islandora_solrQuery = str_replace('~slsh~', '/', $islandora_solrQuery);

    if (strlen(trim($islandora_solrQuery))) {

      // Variables to build a drupal-ish unordered list
      $query_list_title = "Query";
      $query_list_items = array();
      $query_list_type = "ul";
      $query_list_attributes = array('class' => 'islandora_solr_search_query_list query_list');

      // OK, so it's a list of one.
      // In the future, we could reasonably break the query on boolean operators
      // and allow one part to be removed at a time.
      $query_list_items[] .= l(
              '<img class="islandora_solr_add_remove_link" src="' . $base_url . '/' . drupal_get_path('module', 'islandora_solr_search') . '/images/delete.png" alt="Remove search term">',
              'islandora/solr/search/ /1/' . $islandora_fq . '/dismax',
              array('html' => true)
          ) . ' ' . stripslashes($islandora_solrQuery);

      $output .= '<div class="islandora_solr_search_query_wrap">';

      $output .= theme_item_list($query_list_items, $query_list_title, $query_list_type, $query_list_attributes);
      $output .= '</div>'; // 'islandora_solr_search_query_wrap'
    }
    if (empty($islandora_facets)) {
      return ''; //no facets to show
    }
    foreach ($islandora_facets as $key => $field) {
      $list_items = array();
      $list_type = "ul";
      $list_title = null;

      $test = get_object_vars($field); //get the number of fields if there aren't any don't show the key
      if (count($test) > 0) {
        $facet_count = 0;
        unset($normal_facet_output);
        $filter_include = null;
        $filter_exclude = null;
        foreach ($field as $name => $number) {
          if ($islandora_fq && $islandora_fq != '-') {//there are existing facets in the query
            $disable_link = strstr($islandora_fq, $key . ':"' . str_replace('/', IslandoraSolrResults::$slashReplacement, $name) . '"'); //we don't want a link for this facet as we already used it
            if ($disable_link) {
              //don't show link to this facet but include a link to remove it
            }
            else {
              $filter_include = $key . ':' . '"' . $name . '"' . IslandoraSolrResults::$facetSeparator . $islandora_fq;
              $filter_exclude = '-' . $key . ':' . '"' . $name . '"' . IslandoraSolrResults::$facetSeparator . $islandora_fq;
            }
          }
          else { //no existing facets chosen
            $filter_include = $key . ':' . '"' . $name . '"';
            $filter_exclude = '-' . $key . ':' . '"' . $name . '"';
          }
          $filter_include = str_replace('/', IslandoraSolrResults::$slashReplacement, $filter_include); //replace the slash so url does not break
          $filter_exclude = str_replace('/', IslandoraSolrResults::$slashReplacement, $filter_exclude); //replace the slash so url does not break
          if ($disable_link) {
            // we don't want to create a link, because we're already filtering on this value.
            // Links to remove enabled facet filters are created down below.
          }
          else {//normal link
            $evenodd = ( $facet_count % 2 ? 'odd' : 'even' );
            $lplus = 'islandora/solr/search/' . $islandora_solrQuery . '/1/' . $filter_include . '/' . $islandora_defType;
            $lminus = 'islandora/solr/search/' . $islandora_solrQuery . '/1/' . $filter_exclude . '/' . $islandora_defType;
            $text = $name;
            $attrs = array();
            $list_items[] = l($text, $lplus, $attrs) . ' (' . $number . ')' .
                '<span class="plusminus">' .
                l('+', $lplus, array('attributes' => array('class' => 'plus'))) . ' ' .
                l('-', $lminus, array('attributes' => array('class' => 'minus'))) .
                '</span>';
            $facet_count++;
          }
        }
        if ($facet_count >= 2) {
          $facet_output .='<div class="islandora_solr_search_facet">';
          $list_title = $facetArray[$key];
          $list_type = "ul";
          $list_attributes = array('class' => 'islandora_solr_search_facet_list facet_list');
          $facet_output .= theme_item_list($list_items, $list_title, $list_type, $list_attributes);
          $facet_output .='</div>';
        }
      }
    }

    //////////////////////////////////////////////////////////////////////////////
    // Here we will build links to remove currently enforced facet filters ...  //
    //////////////////////////////////////////////////////////////////////////////
    $filters = explode(IslandoraSolrResults::$facetSeparator, $islandora_fq);
    if (!empty($filters) && $islandora_fq != '-') {
      $filter_output = null;

      // Variables to build a drupal-ish unordered list, which will be rendered
      // a little bit farther down
      $filter_list_title = "Enabled Filters";
      $filter_list_items = array();
      $filter_list_type = "ul";
      $filter_list_attributes = array('class' => 'islandora_solr_search_filter_list filter_list');

      foreach ($filters as $key => $filter) {
        $edit_filters = $filters;
        unset($edit_filters[$key]);
        $filter_disable = implode(IslandoraSolrResults::$facetSeparator, $edit_filters);
        if (empty($filter_disable)) {
          $filter_disable = '-';
        }
        $filter_name_and_value = explode(":", $filter, 2);
        $filter_name = $filter_name_and_value[0];
        $filter_value = $filter_name_and_value[1];
        if (strncmp($filter_name, '-', 1))
          $exclude = false;
        else {
          $exclude = true;
          $filter_name = substr($filter_name, 1);
        }
        $filter_list_items[] = l(
                '<img class="islandora_solr_add_remove_link" src="' . $base_url . '/' . drupal_get_path('module', 'islandora_solr_search') . '/images/delete.png" alt="Remove this filter">',
                'islandora/solr/search/' . $islandora_solrQuery . '/' . $filter_disable . '/' . $islandora_defType,
                array('html' => true)
            ) . ' ' . $facetArray[$filter_name] . ($exclude ? ' != ' : ' = ') . $filter_value;
      }
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // as we add additional facets, we're repeatedly URL-encoding old facet             //
    // strings. when we double-encode quotation marks they're incomprehensible to solr. //
    // This is a quick workaround:                                                      //
    //////////////////////////////////////////////////////////////////////////////////////
    $facet_output = str_replace('%2B', '%252B', $facet_output);
    if (!empty($filter_list_items)) {
      $filter_output = str_replace('%2B', '%252B', $filter_output);
      $output .= '<div class="islandora_solr_search_filter_wrap">';
      $output .= theme_item_list($filter_list_items, $filter_list_title, $filter_list_type, $filter_list_attributes);
      $output .= '</div>'; // 'islandora_solr_search_filter_wrap'
    }
    $output .='<div class="islandora_solr_search_facet_wrap">';
    $output .= $facet_output;
    $output .= '</div>';

    return $output;
  }

//a better explode method allows quotes in the returned strings.  taken from the php.net site
  function csv_explode($delim=',', $str, $enclose='"', $preserve=false) {
    $resArr = array();
    $n = 0;
    $expEncArr = explode($enclose, $str);
    foreach ($expEncArr as $EncItem) {
      if ($n++ % 2) {
        array_push($resArr, array_pop($resArr) . ($preserve ? $enclose : '') . $EncItem . ($preserve ? $enclose : ''));
      }
      else {
        $expDelArr = explode($delim, $EncItem);
        array_push($resArr, array_pop($resArr) . array_shift($expDelArr));
        $resArr = array_merge($resArr, $expDelArr);
      }
    }
    return $resArr;
  }

  //copied from apache solr service and added space as a value to escape.  We are using this to escape the field being searched not the text being searched for so we need both.
  public function lesser_escape($value) {
    //list taken from http://lucene.apache.org/java/docs/queryparsersyntax.html#Escaping%20Special%20Characters
    // Zac removed the quotation mark and whitespace from this list.
    // a) Spaces in our search strings don't need escaping
    // b) We want to allow users to use quotation marks to indicate multi-word phrases

    $pattern = '/(\+|-|&&|\|\||!|\(|\)|\{|}|\[|]|\^|~|\*|\?|:|\\\)/';
    $replace = '\\\$1';

    return preg_replace($pattern, $replace, $value);
  }

  //escape characters in field names of facets
  function solr_escape($facets) {
    $returnFacets = array();
    foreach ($facets as $facet) {
      $tmp = substr($facet, 0, strpos($facet, ':"'));
      $tmp = $this->lesser_escape(trim($tmp));

      $returnFacets[] = $tmp . substr($facet, strpos($facet, ':"'), strlen($facet));
    }
    return $returnFacets;
  }

  function build_solr_search_form($repeat = NULL, $pathToSearchTerms = NULL, $query = NULL) {
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
    global $islandora_query;
    if(!strpos($islandora_query, ':')){
      $default = $islandora_query;
    }
    $form["islandora_simple_search_query"] = array(
      '#size' => '24%',
      '#type' => 'textfield',
      '#title' => t(''),
      '#default_value' => $default,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('search')
    );
    return $form;
  }

  function theme_solr_search_form($form) {
    if (!isset($repeat)) {
      $repeat = variable_get('islandora_solr_search_block_repeat', t('3'));
    }

    $output = drupal_render($form['search_type']['type1']);
    $output .= drupal_render($form['fedora_terms1']);
    $output .= drupal_render($form['andor1']) . drupal_render($form['search_type']['type2']);
    $output .= drupal_render($form['fedora_terms2']);
    if ($repeat > 2 && $repeat < 9) {
      for ($i = 3; $i < $repeat + 1; $i++) {
        $t = $i - 1;
        $output .= drupal_render($form["andor$t"]) . drupal_render($form['search_type']["type$i"]);
        $output .= drupal_render($form["fedora_terms$i"]);
      }
    }
    $output .= drupal_render($form['submit']);
    $output .= drupal_render($form);
    return $output;
  }

  function mapsDisplay($results) {
    $output.= '<ol start="' . ($recordStart + 1) . '">';
    global $base_url;
    if (empty($results)) {
      return "no results";
    }

    foreach ($results->response->docs as $doc) {
      $output .= '<li><table>';
      $rowspan = 0;
      $pidValue = null;
      $output .= '<tr>';
      $output .= '<td ><img src="' . $base_url . '/fedora/repository/' . htmlspecialchars($doc->PID, ENT_QUOTES, 'utf-8') . '/TN"/></td>';
      $output .= '<td><table>';
      foreach ($doc as $field => $value) {
        $rowspan++;
        $output .= '<tr><th>' . $field . '</th>';

        if (is_array($value)) {
          $value = implode(", ", $value);
        }
        if ($field == 'PID') {
          $pidValue = $value;
          $output.='<td><a href="' . $base_url . '/fedora/repository/' . htmlspecialchars($value, ENT_QUOTES, 'utf-8') . '">' . $value . '</td></tr>';
        }
        else {
          $output .= '<td>' . $value . '</td></tr>';
        }
      }

      $output .='</table></td></tr></table></li>';
    }
    $output .= '</ol>';
    return $output;
  }

//this function will be removed.  class has been refactored so we only need the display loop.  leaving here until mnpl is updated.
  function mnplSearchAndDisplay($query, $startPage=1, $fq=null, $dismax=null) {
    $limit = variable_get('islandora_solr_search_num_of_results', 20);
    global $islandora_solrQuery, $islandora_solr_page_number, $base_url; //used by facect block as well
    $islandora_solr_page_number = $startPage;
    $islandora_solrQuery = str_replace('/', IslandoraSolrResults::$slashReplacement, $query); //replace the slash so we can use this in a url without breaking drupal clean urls
    if (empty($islandora_solrQuery)) {
      $islandora_solrQuery = '%20'; //so we can allow empty queries to dismax
    }
    // module_load_include('php', 'fedora_repository', 'Solr/Service');
    $host = variable_get('islandora_solr_search_block_host', 'localhost');
    $port = variable_get('islandora_solr_search_block_port', '8080');
    $appName = variable_get('islandora_solr_search_block_app_name', 'solr');
    $solr = new Apache_Solr_Service($host, $port, '/' . $appName . '/');
    $query = $this->parseQuery($solr, $query, $dismax, $solr); //fix the query as some characters will break the search : and / slash are examples
    $facetlimit = variable_get('islandora_solr_search_block_facet_limit', '12');
    $facetMinCount = variable_get('islandora_solr_search_block_facet_min_count', '2');
    $requestHandler = variable_get("islandora_solr_search_block_request_handler", t("standard"));
    $additionalParams = array(
      'facet' => 'true',
      'facet.mincount' => $facetMinCount,
      'facet.limit' => $facetlimit,
      'qt' => $requestHandler,
      'facet.field' => explode(',', variable_get("islandora_solr_search_block_facets", 'dc.subject,dc.type')), //comma separated list configured in the block config
    );
    global $islandora_fq;
    if ($fq != NULL && $fq != '-') {
      $fq = str_replace(IslandoraSolrResults::$slashReplacement, '/', $fq); //put the slash back
      if (strpos($fq, '-') === 0) {//remove the requested facet from the list
        $fq = substr($fq, strpos($fq, IslandoraSolrResults::$facetSeparator) + 1);
      }
      $fqs = $this->csv_explode(IslandoraSolrResults::$facetSeparator, $fq, '"', true); //to filter by more then one facet we will separate them by~ for now
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
    $total = (int) $results->response->numFound;
    $start = min(1, $total);
    $end = min(($limit + $recordStart), $total);
    $output .='<div>Results ' . ($recordStart + 1) . ' - ' . $end . ' of ' . $total . '</div>';
    $output .='<div class="item-list"><ul class = "pager">'; //opening div for paging links

    if ($recordStart > 0) {
      $output .='<li class="pager-previous"><a href="' . $base_url . '/fedora/repository/solr_search/' . htmlspecialchars(drupal_urlencode($islandora_solrQuery), ENT_QUOTES, 'utf-8', false) . '/' . max(0, ($startPage - 1)) . '/' . htmlspecialchars($islandora_fq, ENT_QUOTES, 'utf-8', false) . '/' . $dismax . '">‹ previous</a></li> ';
    }
    if ($recordStart < ($total - $limit)) {
      $output .='<li class = "pager-next"><a href="' . $base_url . '/fedora/repository/solr_search/' . htmlspecialchars(drupal_urlencode($islandora_solrQuery), ENT_QUOTES, 'utf-8', false) . '/' . ($startPage + 1) . '/' . htmlspecialchars($islandora_fq, ENT_QUOTES, 'utf-8', false) . '/' . $dismax . '">next ›</a></li>';
    }
    $output .= '</div></ul>'; //added closing div for drupal paging here
    //most formatting of results happens in the loop below.  maybe pull this out to a function
    $output.= '<ol start="' . ($recordStart + 1) . '">';
    global $base_url;
    if (empty($results)) {
      return "no results";
    }
    foreach ($results->response->docs as $doc) {
      $output .= '<li>';
      $pidValue = '';
      $genusValue = '';
      $speciesValue = '';
      $keyValue;

      foreach ($doc as $field => $value) {
        //var_dump($doc);
        if (is_array($value)) {
          $value = implode(" ", $value);
        }
        if ($field == 'PID') {
          $pidValue = $value;
        }
        else if ($field == 'genus') {
          $genusValue = $value;
        }
        else if ($field == 'specimen.species') {
          $speciesValue = $value;
        }
        else if ($field == 'lab_id') {
          $keyValue = $value;
        }
      }
      $typeString = 'Specimen';
      if ($index = (strpos($pidValue, '-F-', TRUE))) {
        $pidForImage = substr($pidValue, 0, $index);
        $pidForImage = str_replace('fraction', 'specimen', $pidForImage);
        $typeString = 'Fraction';
      }
      else if ($index = (strpos($pidValue, '-C-', TRUE))) {
        $pidForImage = strstr($pidValue, 0, $index);
        $pidForImage = str_replace('compound', 'specimen', $pidForImage);
        $typeString = 'Compound';
      }
      else {
        $pidForImage = $pidValue;
      }
      $output.='<img src="' . $base_url . '/fedora/repository/' . htmlspecialchars($pidForImage, ENT_QUOTES, 'utf-8') . '/TN"/><a href="' . $base_url . '/fedora/repository/' . htmlspecialchars($pidValue, ENT_QUOTES, 'utf-8') . '"><strong>' . $typeString . '</strong>' . $keyValue . ': ' . $genusValue . ' ' . $speciesValue . '</a> ';


      $output .='</li>';
    }
    $output .= '</ol>';

    global $islandora_facets; //make facets available to the islandora facet block
    $islandora_facets = $results->facet_counts->facet_fields;
    return $output;
  }

}

