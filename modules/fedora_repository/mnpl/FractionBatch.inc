<?php

class FractionBatch {

  function  __construct($pid = '') {

  }

  public function buildDrupalForm($form = array()) {

    // We don't need to add anything beyond the standard Darwin Core form so just pass this through
    // If we wanted to we could add other fields.
    return $form;
  }

  public function buildEditMetadataForm($form = array()) {
    return $form;
  }

  public function handleEditMetadataForm($form_id, $form_values) {
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
    global $user;

    $file = $form_values['ingest-file-location'];

    if (!empty($file)) {
      // Parse CSV file and populate Fraction objects on each row.
      $row = 1;
      if (($handle = fopen($file, "r")) !== FALSE) {
        $batch = array(
            'title' => 'Updating Fractions',
            'operations' => array(),
            'file' => drupal_get_path('module', 'fedora_repository').'/mnpl/FractionBatch.inc',
        );
        while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
          $num = count($data);
          if ($row == 1) {
            $headers = $data;
            $headers_count = count($headers);
            $row++;
            continue;
          }
          if ($headers_count != $num ) {
            // The row is incomplete, skip to the next one
            $row++;
            continue;
          }
          $elements = array_combine($headers, $data);
          $batch['operations'][] = array('update_fraction_item', array($elements));
          $row++;
        }
        fclose($handle);
        batch_set($batch);
      }
    }
  }

  
  public function showFieldSets() {
    module_load_include('inc', 'fedora_repository', 'plugins/tagging_form');
    module_load_include('inc', 'fedora_repository', 'plugins/DarwinCore');
    global $base_url;


    return 'Nope';
  }
}

function update_fraction_item($elements) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    
    $fraction_pid = "vre:mnpl-fraction-".array_shift($elements);

    $item = new Fedora_Item($fraction_pid);
    if (empty($item->objectProfile)) {
      return FALSE;
    }
    $fraction_xml = $item->get_datastream_dissemination('FRACTION');
    $fraction_item = simplexml_load_string($fraction_xml);
    foreach ($elements as $name => $value) {
      $fraction_item->children('http://vre.upei.ca/fractions/')->$name = $value;
    }
    $item->modify_datastream_by_value($fraction_item->asXML(), 'FRACTION', $fraction_pid.' Fraction', 'text/xml');
    drupal_set_message("!fp updated successfully.", array('!fp' => $fraction_pid));
  }
