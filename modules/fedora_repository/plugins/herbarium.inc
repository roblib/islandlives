<?php

class Herbarium {

  function  __construct($pid = '') {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    if (!empty($pid)) {
      
      $this->pid = $pid;
      $this->item = new Fedora_Item($pid);
    }
  }

  public function buildDrupalForm($form = array()) {

    // We don't need to add anything beyond the standard Darwin Core form so just pass this through
    // If we wanted to we could add other fields.
    module_load_include('inc', 'fedora_repository', 'plugins/DarwinCore');
    
    $dwc = new DarwinCore($this->item);
    return $dwc->buildDrupalForm($form);
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

  public function handleIngestForm($form_values) {
    /*
     * process the metadata form
     * Create fedora object
     * Add the datastreams
     */
    module_load_include('inc', 'fedora_repository', 'mimetype');
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    module_load_include('inc', 'fedora_repository', 'plugins/DarwinCore');
    global $user;
    $mimetype = new mimetype();
    $dwc = new DarwinCore();
    $dwc->handleForm($form_values);
    $label = $form_values['dwc:institutionCode'].':'
            .$form_values['dwc:collectionCode'].':'
            .$form_values['dwc:catalogNumber'];

    $new_item = Fedora_Item::ingest_new_item($form_values['pid'], 'A', $label,
            $user->name);

    $new_item->add_datastream_from_string($dwc->darwinCoreXML, 'DARWIN_CORE',
            'Darwin Core Metadata', 'text/xml', 'X');
    $file = $form_values['ingest-file-location'];

    if (!empty( $file)) {
      $dformat = $mimetype->getType($file);
      $new_item->add_datastream_from_file($file, 'FULL_SIZE',
              "$label-full-size", $dformat, 'M');
    }

    $new_item->add_relationship('hasModel', $form_values['content_model_pid'], FEDORA_MODEL_URI);
    $new_item->add_relationship(!empty($form_values['relationship']) ? $form_values['relationship'] : 'isMemberOfCollection', $form_values['collection_pid']);
    
    if (!empty($_SESSION['fedora_ingest_files'])) {
      foreach ($_SESSION['fedora_ingest_files'] as $dsid => $created_file) {
        $created_file_format = $mimetype->getType($created_file);
        $created_filename = strstr($created_file, $file);
        $new_item->add_datastream_from_file($created_file, $dsid,
                $created_filename, $created_file_format, 'M');
        
      }
    }
  }

  public function showFieldSets() {
    module_load_include('inc', 'fedora_repository', 'plugins/tagging_form');
    module_load_include('inc', 'fedora_repository', 'plugins/DarwinCore');
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
//     $collection_fieldset = array (
      '#type' => 'tabpage',
      '#title' => t('Full-size'),
      '#content' => $html
    );
    $tabset['my_tabset']['first_tab'] = array(
      // #type and #title are the minimum requirements.
      '#type' => 'tabpage',
      '#title' => t('View'),
      // This will be the content of the tab.
      '#content' => '<a href="'. $base_url . '/fedora/repository/'. $this->pid . '/FULL_JPG/"><img src="'. $base_url . '/fedora/imageapi/'.
        $this->pid . '/JPG/JPG.jpg'. '" /></a>'. '<p>'. drupal_get_form('fedora_repository_image_tagging_form', $this->pid) . '</p>',
    );

    $dwc = new DarwinCore($this->item);
    $tabset['my_tabset']['third_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Description'),
    );
    $tabset['my_tabset']['third_tab']['tabset'] = array(
      '#type' => 'tabset',
    );
    
    $tabset['my_tabset']['third_tab']['tabset']['view'] = array(
      '#type' => 'tabpage',
      '#title' => t('Darwin Core'),
      '#content' => $dwc->asHTML(),
    );
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $obj = new ObjectHelper();
    if (fedora_repository_access(OBJECTHELPER :: $EDIT_FEDORA_METADATA, $this->pid, $user)) {
      $editform = drupal_get_form('fedora_repository_edit_qdc_form', $this->pid, 'DARWIN_CORE');
      $tabset['my_tabset']['third_tab']['tabset']['edit'] = array(
        '#type' => 'tabpage',
        '#title' => t('Edit'),
        '#content' => $editform,
      );
      $tabset['my_tabset']['third_tab']['tabset']['source'] = array(
        '#type' => 'tabpage',
        '#title' => t('Vew Source'),
      );
      $xmlsrc = $dwc->asXML();
      $tabset['my_tabset']['third_tab']['tabset']['source']['srctext'] = array(
        '#name' => 'source',
        '#type' => 'textarea',
        '#value' => $xmlsrc,
        '#id' => 'source',
        '#height' => 50,
      );
    }

    return tabs_render($tabset);
  }
}