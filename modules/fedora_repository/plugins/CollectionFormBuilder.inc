<?php
// $Id$

module_load_include('inc', 'fedora_repository', 'plugins/FormBuilder');
/*
 * Created on 19-Feb-08
 *
 *
 * implements methods from content model ingest form xml
 * builds a dc metadata form
 */
class CollectionFormBuilder extends FormBuilder {
  function CollectionFormBuilder() {
    module_load_include('inc', 'CollectionFormBuilder', '');
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  }

  function createFedoraDataStreams($form_values, &$dom, &$rootElement) {
    module_load_include('inc', 'fedora_repository', 'mimetype');
    global $base_url;

    $mimetype = new mimetype();
    $server = NULL;
    $file=$form_values['ingest-file-location'];
    $dformat = $mimetype->getType($file);
    $fileUrl = $base_url . '/'. drupal_urlencode($file);
    $beginIndex = strrpos($fileUrl, '/');
    $dtitle = substr($fileUrl, $beginIndex + 1);
    $dtitle =  substr($dtitle, 0, strpos($dtitle, "."));
    $ds1 = $dom->createElement("foxml:datastream");
    $ds1->setAttribute("ID", "COLLECTION_POLICY"); //set the ID
    $ds1->setAttribute("STATE", "A");
    $ds1->setAttribute("CONTROL_GROUP", "M");
    $ds1v= $dom->createElement("foxml:datastreamVersion");
    $ds1v->setAttribute("ID", "COLLECTION_POLICY.0");
    $ds1v->setAttribute("MIMETYPE", "$dformat");
    $ds1v->setAttribute("LABEL", "$dtitle");
    $ds1content = $dom->createElement('foxml:contentLocation');
    $ds1content->setAttribute("REF", "$fileUrl");
    $ds1content->setAttribute("TYPE", "URL");
    $ds1->appendChild($ds1v);
    $ds1v->appendChild($ds1content);
    $rootElement->appendChild($ds1);
  }
}
