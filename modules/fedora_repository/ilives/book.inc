<?php

class IslandoraBook {

  function  __construct($pid = '') {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if (!empty($pid)) {

      $this->pid = $pid;
      $this->item = new Fedora_Item($pid);
    }
  }

  public function buildDrupalForm($form = array(), $ingest_form = array(), $form_values = array(), &$form_state = array()) {
    $mods_save = '';
    if (isset($form_state['mods_save'])) {
      $mods_save = $form_state['mods_save'];
    }

    // Set #cache to true to create the $form_state cache
    $form['#cache'] = TRUE;
    // Prompt the user to enter a record ID to be looked up in Evergreen.
    $form['unapi_url'] = array(
      '#type' => 'textfield',
      '#title' => 'Catalogue item URL',
    );
    $form['unapi_url_submit'] = array(
	'#type' => 'submit',
	'#value' => t('Retrieve MODS record'),
	'#submit' => array('fedora_ilives_retrieve_unapi_submit'),
        '#ahah' => array(
	  'path' => 'fedora/ilives/retrieve_unapi/js', // path we defined in hook_menu
	  'wrapper' => 'mods-wrapper', // the HTML that wraps the element that needs to be replaced
	  'method' => 'replace', // the method we're going to use: a replace operation
	  'effect' => 'fade', // the effect used when replacing the element (try fade!)
        )
      );
    drupal_add_js('', 'inline');
    // We define a <div> wrapper. Everything in it will be replaced.
    $form['mods'] = array(
      '#tree' => TRUE,
      '#prefix' => '<div id="mods-wrapper">',
      '#suffix' => '</div>',
    );
    $form['mods']['mods_record'] = array(
      '#type' => 'textarea',
      '#title' => 'MODS Record to Import',
      '#value' => (!empty($mods_save) ? $mods_save['mods']['mods_record'] : ''),
    );
    return $form;
  }

  public function buildEditMetadataForm($form = array()) {

    $form['submit'] = array(
      '#type' => 'submit',
      '#weight' => 10,
      '#value' => 'Update'
    );
    $form['pid'] = array(
      '#type' => 'hidden',
      '#value' => $this->pid,
    );
    $form['dsid'] = array(
      '#type' => 'hidden',
      '#value' => "DARWIN_CORE",
    );

    return $this->buildDrupalForm($form);
  }

  public function handleEditMetadataForm($form_id, $form_values) {
    /*
     * Process the metadata form
     * Update the datastreams
     */

    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    module_load_include('inc', 'fedora_repository', 'plugins/DarwinCore');
    module_load_include('inc', 'fedora_repository', 'mimetype');
    global $user;
    $mimetype = new mimetype();
    $dwc = new DarwinCore($this->item);
    $dwc->handleForm($form_values);
    $this->item->purge_datastream('DARWIN_CORE');
    $this->item->add_datastream_from_string($dwc->darwinCoreXML, 'DARWIN_CORE',
            'Darwin Core Metadata', 'text/xml', 'X');
    return TRUE;
  }

  public function handleIngestForm($form_values, $form, &$form_state) {
    /*
     * process the metadata form
     * Create fedora object
     * Add the datastreams
     */
    module_load_include('inc', 'fedora_repository', 'mimetype');
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if ($form_state['clicked_button']['#value'] == 'Retrieve MODS record') {
      return;
    }


    $mods_simple = simplexml_load_string($form_values['mods']['mods_record']);
    $title = '';
    foreach ($mods_simple->children('http://www.loc.gov/mods/v3')->mods[0]->titleInfo[0]->children() as $child) {
      if ($child->getName() == 'subTitle') {
        // We don't care about subtitles for creating the item label.
        continue;
      }
      $title .= $child;
    }

    global $user;
    $mimetype = new mimetype();
    $new_item = Fedora_Item::ingest_new_item($form_values['pid'], 'A', $title,
            $user->name);

    $new_item->add_datastream_from_string($form_values['mods']['mods_record'], 'MODS',
            'MODS Metadata', 'text/xml', 'X');

    $new_item->add_relationship('hasModel', $form_values['content_model_pid'], FEDORA_MODEL_URI);
    $new_item->add_relationship(!empty($form_values['relationship']) ? $form_values['relationship'] : 'isMemberOfCollection', $form_values['collection_pid']);
  }

  public function showFieldSets() {
    module_load_include('inc', 'fedora_repository', 'plugins/tagging_form');
    module_load_include('inc', 'fedora_repository', 'plugins/DarwinCore');
    module_load_include('module', 'fedora_ilives');
    global $base_url;

    $tabset = array();

    $tabset['my_tabset'] = array(
      '#type' => 'tabset',
    );
    
    global $user;
    $qs = '';
    if ($user->uid != 0) {
      $qs = '?uid='. base64_encode($user->name . ':'. $user->pass);
    }

    $viewer_url = variable_get('fedora_base_url', '') . '/get/'. $this->pid . '/ilives:viewerSdef/getViewer'. $qs;
    $html = '<iframe src="'. $viewer_url . '" frameborder="0" style="width: 100%; height: 800px;">Errors: unable to load viewer</iframe>';
    $tabset['my_tabset']['second_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Full-size'),
      '#content' => $html
    );
    $tabset['my_tabset']['first_tab'] = array(
      // #type and #title are the minimum requirements.
      '#type' => 'tabpage',
      '#title' => t('Description'),
      // This will be the content of the tab.
      '#content' => fedora_ilives_create_book_view($this->pid),
    );

    return tabs_render($tabset);
  }
}