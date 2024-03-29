<?php

/*
 * contains methods to search solr and display results.  depends on Apache_Solr_Php client.
 */

module_load_include('inc','islandora_solr_search','IslandoraSolrResults');

/**
 * Extention of IslandoraSolrResults for templating purposes only.
 * No changes have been made to the original class.
 */

class CustomizedIslandoraSolrResults extends IslandoraSolrResults {

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
  function searchAndDisplay($query, $startPage=1, $fq=null, $dismax=null) {
    $limit = variable_get('islandora_solr_search_num_of_results', 20);
    global $islandora_solrQuery, $islandora_solr_page_number, $base_url; //used by facet block as well
    $configs = explode('~', variable_get('islandora_solr_config_options'));
    $classFile = $configs[1];
    module_load_include('',$configs[0],$configs[1]);
    $solrClass = $configs[2];
    $solrFunction = $configs[3];
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
    $start = min(1, $total);
    $end = min(($limit + $recordStart), $total);
    $output .='<div>Results ' . ($recordStart + 1) . ' - ' . $end . ' of ' . $total . '</div>';
    $output .='<div class="item-list"><ul class = "pager">'; //opening div for paging links

    if ($recordStart > 0) {
      $output .='<li class="pager-previous"><a href="' . $base_url . '/islandora/solr/search/' . htmlspecialchars(drupal_urlencode($islandora_solrQuery), ENT_QUOTES, 'utf-8', false) . '/' . max(0, ($startPage - 1)) . '/' . htmlspecialchars($islandora_fq, ENT_QUOTES, 'utf-8', false) . '/' . $dismax . '">‹ previous</a></li> ';
    }
    if ($recordStart < ($total - $limit)) {
      $output .='<li class = "pager-next"><a href="' . $base_url . '/islandora/solr/search/' . htmlspecialchars(drupal_urlencode($islandora_solrQuery), ENT_QUOTES, 'utf-8', false) . '/' . ($startPage + 1) . '/' . htmlspecialchars($islandora_fq, ENT_QUOTES, 'utf-8', false) . '/' . $dismax . '">next ›</a></li>';
    }
    $output .= '</div></ul>'; //added closing div for drupal paging here

    if (class_exists($solrClass)) {
      $implementation = new $solrClass();
    } else {
      drupal_set_message(t('Error loading solr results class. Class could not be loaded. Check the Islandora Solr search blocks configuration.'), 'error');
      return ' ';
    }

    if (method_exists($implementation, $solrFunction)) {
      $output.= $implementation->$solrFunction($results);
      if (variable_get('islandora_solr_search_debug_mode', 0)) { // debug dump
        drupal_set_message('Params: <br/><pre>' . print_r($additionalParams, true) . '</pre>', 'status');
      }
    } else {
      drupal_set_message(t('Error loading solr results class. Could not find the specified method.  Check the Islandora Solr search blocks configuration.'), 'error');
    }
    global $islandora_facets; //make facets available to the islandora facet block
    $islandora_facets = $results->facet_counts->facet_fields;
    return $output;
  }

//default implementation of display results can be changed in the blocks config
  function displayResults($results) {
    $output.= '<ol start="' . ($recordStart + 1) . '">';
    global $base_url;
    if (empty($results)) {
      return "no results";
    }

    foreach ($results->response->docs as $doc) {
      $output .= '<li><table>';
      foreach ($doc as $field => $value) {
        $output .= '<tr><th>' . $field . '</th>';
        if (is_array($value)) {
          $value = implode(", ", $value);
        }

        if ($field == 'PID') {
          $output.='<td><a href="' . $base_url . '/fedora/repository/' . htmlspecialchars($value, ENT_QUOTES, 'utf-8') . '">' . $value . '</td>';
        } else {
          $output .= '<td>' . $value . '</td>';
        }
      }
      $output .='</table></li>';
    }
    $output .= '</ol>';
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
    global $islandora_facets, $islandora_defType, $islandora_solrQuery, $base_url, $islandora_fq;
    $output = '<div class = "item-list">';
    if (strlen(trim($islandora_solrQuery))) {
      $output .= '<div class="islandora_solr_search_search_wrap">';
      $output .= '<div class="islandora_solr_search_facet_title">' . t('Base Query') . ':</div>';
      $output .= '<div class="islandora_solr_search_facet_value_enabled">' . $islandora_solrQuery;
      $output .= ' <a href="' . $base_url . '/islandora/solr/search/' . htmlspecialchars(drupal_urlencode(" "), ENT_QUOTES, 'utf-8', false) . '/' . ($startPage + 1) . '/' . htmlspecialchars($islandora_fq, ENT_QUOTES, 'utf-8', false) . '/dismax"><img class="islandora_solr_add_remove_link" src="/' . drupal_get_path('module', 'islandora_solr_search') . '/images/delete.png" alt="Remove search term"></a>';
      $output .= '</div>'; // 'islandora_solr_search_facet_value_enabled'
      $output .= '</div>'; // 'islandora_solr_search_search_wrap'
    }
    if (empty($islandora_facets)) {
      return ''; //no facets to show
    }
    foreach ($islandora_facets as $key => $field) {
      $test = get_object_vars($field); //get the number of fields if there aren't any don't show the key
      if (count($test) > 0) {
        $facet_count = 0;
        unset($normal_facet_output);
        $filter_query;
        foreach ($field as $name => $number) {
          if ($islandora_fq && $islandora_fq != '-') {//there are existing facets in the query
            $disable_link = strstr($islandora_fq, $key . ':"' . str_replace('/', IslandoraSolrResults::$slashReplacement, $name) . '"'); //we don't want a link for this facet as we already used it
            if ($disable_link) { //don't show link to this facet but include a link to remove it
              $edited_list = str_replace($key . ':"' . str_replace('/', IslandoraSolrResults::$slashReplacement, $name) . '"', '', $islandora_fq); //remove the filter query from the list
              $edited_list = str_replace(IslandoraSolrResults::$facetSeparator . IslandoraSolrResults::$facetSeparator, IslandoraSolrResults::$facetSeparator, $edited_list); //cut out duplicate field separators
              $edited_list = trim($edited_list, IslandoraSolrResults::$facetSeparator); //cut off trailing field separators
              $filter_query = htmlspecialchars(drupal_urlencode($edited_list), ENT_QUOTES, 'utf-8', false);
            } else {
              $filter_query = $key . ':' . htmlspecialchars('"' . drupal_urlencode($name) . '"', ENT_QUOTES, 'utf-8', false) . IslandoraSolrResults::$facetSeparator . htmlspecialchars(drupal_urlencode($islandora_fq), ENT_QUOTES, 'utf-8', false);
            }
          } else { //no existing facets chosen
            $filter_query = $key . ':' . htmlspecialchars('"' . drupal_urlencode($name) . '"', ENT_QUOTES, 'utf-8');
          }
          $filter_query = str_replace('/', IslandoraSolrResults::$slashReplacement, $filter_query); //replace the slash so url does not break
          if (empty($filter_query)) {
            $filter_query = '-';
          }
          if ($disable_link) {//create a link to remove this facet as it has already been chosen
            $filter_output .= '<li><div>' . $key . ' = ' . $name; //no href here
            $filter_output .= ' <a href="' . $base_url . '/islandora/solr/search/' . htmlspecialchars(drupal_urlencode($islandora_solrQuery), ENT_QUOTES, 'utf-8', false) . '/1/' . $filter_query . '/' . $islandora_defType . '"/><img class="islandora_solr_add_remove_link" src="/' . drupal_get_path('module', 'islandora_solr_search') . '/images/delete.png" alt="Remove this filter"></a> ';
            $filter_output .= "</div></li>";
          } else {//normal link
            $normal_facet_output .= '<li><div><a href="' . $base_url . '/islandora/solr/search/' . htmlspecialchars(drupal_urlencode($islandora_solrQuery), ENT_QUOTES, 'utf-8', false) . '/1/' . $filter_query . '/' . $islandora_defType . '"/>' . $name . '</a> (' . $number . ')</div></li>';
            $facet_count++;
          }
        }
        if ($facet_count >= 2) {
          $facet_output .='<div class="islandora_solr_search_facet_wrap">';
          $facet_output .='<div class="islandora_solr_search_facet_name">' . htmlspecialchars($key, ENT_QUOTES, 'utf-8') . '</div>';
          $facet_output .='<ul>';
          $facet_output .= $normal_facet_output;
          $facet_output .='</ul>';
          $facet_output .='</div>';
        }
      }
    }
    if (!empty($filter_output)) {
      $output .= '<div class="islandora_solr_search_search_wrap">';
      $output .= '<div class="islandora_solr_search_facet_title">' . t('Enabled Filters') . ':</div>';
      $output .= '<div class="islandora_solr_search_enabled_filters"><ul class="filter_list">' . $filter_output . '</ul></div>';
      $output .= '</div>'; // 'islandora_solr_search_search_wrap'
    }
    $output .='<div class="islandora_solr_search_facets">';
    $output .= $facet_output;
    $output .= '</div>';
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
      } else {
        $expDelArr = explode($delim, $EncItem);
        array_push($resArr, array_pop($resArr) . array_shift($expDelArr));
        $resArr = array_merge($resArr, $expDelArr);
      }
    }
    return $resArr;
  }

  //copied from apache solr service and added space as a value to escape.  We are using this to escape the field being searched not the text being searched for so we need both.
  public function escape($value) {
    //list taken from http://lucene.apache.org/java/docs/queryparsersyntax.html#Escaping%20Special%20Characters
    $pattern = '/(\+|-|&&|\|\||!|\(|\)|\{|}|\[|]|\^|"|~|\*|\?|:| |\\\)/';
    $replace = '\\\$1';

    return preg_replace($pattern, $replace, $value);
  }

  //escape characters in field names of facets
  function solr_escape($facets) {
    $returnFacets = array();
    foreach ($facets as $facet) {
      $tmp = substr($facet, 0, strpos($facet, ':"'));
      $tmp = $this->escape(trim($tmp));
      $returnFacets[] = $tmp . substr($facet, strpos($facet, ':"'), strlen($facet));
    }
    return $returnFacets;
  }

  function build_solr_search_form($repeat = NULL, $pathToSearchTerms = NULL, $query = NULL) {
    $types = $this->get_search_terms_array(NULL, 'solrSearchTerms.xml');
    $queryArray = NULL;
    if (isset($query)) {
      $queryArray = explode('AND', $query);
    }

    $andOrArray = array(
        'AND' => 'and',
            //'OR' => 'or' //removed or for now as it would be a pain to parse
    );
    $form = array();

    if (!isset($repeat)) {
      $repeat = variable_get('islandora_solr_search_block_repeat', t('3'));
    }
    $var0 = explode(':', $queryArray[0]);
    $var1 = explode(':', $queryArray[1]);
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
        '#required' => TRUE,
        '#default_value' => (count($var0) >= 2 ? trim($var0[1]) : ''),
    );
    $form['andor1'] = array(
        '#title' => t(''),
        '#type' => 'select',
        '#default_value' => 'AND',
        '#options' => $andOrArray
    );
    $form['search_type']['type2'] = array(
        '#title' => t(''),
        '#type' => 'select',
        '#options' => $types,
        '#default_value' => (count($var1) >= 2 ? trim($var1[0]) : ''),
    );
    $form['fedora_terms2'] = array(
        '#size' => '24',
        '#type' => 'textfield',
        '#title' => t(''),
        '#default_value' => (count($var1) >= 2 ? $var1[1] : ''),
    );
    if ($repeat > 2 && $repeat < 9) { //don't want less then 2 or more then 9
      for ($i = 3; $i < $repeat + 1; $i++) {
        $t = $i - 1;
        $field_and_term = explode(':', $queryArray[$t]);
        $form["andor$t"] = array(
            '#title' => t(''),
            '#type' => 'select',
            '#default_value' => 'AND',
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
            '#default_value' => (count($field_and_term) >= 2 ? trim($field_and_term[1]) : ''),
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
    //$form = array();
    $form["islandora_simple_search_query"] = array(
        '#size' => '24%',
        '#type' => 'textfield',
        '#title' => t(''),
            // '#default_value' => (count($field_and_term) >= 2 ? trim($field_and_term[1]) : ''),
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
        } else {
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
        } else if ($field == 'genus') {
          $genusValue = $value;
        } else if ($field == 'specimen.species') {
          $speciesValue = $value;
        } else if ($field == 'lab_id') {
          $keyValue = $value;
        }
      }
      $typeString = 'Specimen';
      if ($index = (strpos($pidValue, '-F-', TRUE))) {
        $pidForImage = substr($pidValue, 0, $index);
        $pidForImage = str_replace('fraction', 'specimen', $pidForImage);
        $typeString = 'Fraction';
      } else if ($index = (strpos($pidValue, '-C-', TRUE))) {
        $pidForImage = strstr($pidValue, 0, $index);
        $pidForImage = str_replace('compound', 'specimen', $pidForImage);
        $typeString = 'Compound';
      } else {
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

  function get_search_terms_array($path = NULL, $file = NULL) {
    if (!isset($path)) {
      $path = drupal_get_path('module', 'islandora_solr_search');
    }
    $xmlDoc = new DomDocument();
    if (!isset($file)) {
      $file = 'searchTerms.xml';
    }
    $xmlDoc->load($path . '/' . $file);
    $nodeList = $xmlDoc->getElementsByTagName('term');
    $types = array();
    for ($i = 0; $i < $nodeList->length; $i++) {
      $field = $nodeList->item($i)->getElementsByTagName('field');
      $value = $nodeList->item($i)->getElementsByTagName('value');
      $fieldValue = $field->item(0)->nodeValue;
      $valueValue = $value->item(0)->nodeValue;
      $types["$fieldValue"] = "$valueValue";
    }
    return $types;
  }

}

?>
