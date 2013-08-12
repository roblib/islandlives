<?php
// $Id$

module_load_include('inc', 'fedora_repository', 'plugins/FormBuilder');
/*
 *
 **
 *
 * implements methods from content model ingest form xml
 * builds a dc metadata form
 */
class CompoundsFormBuilder extends FormBuilder {
  function CompoundsFormBuilder() {
    include_once 'includes/bootstrap.inc';
    drupal_bootstrap(DRUPAL_BOOTSRTAP_FULL);

  }

  function setPid($form_values) {
  // set pid custom pid for compounds... add identifier to base... finish with getNextPid pid is generated at form_values level as 'pid'

    $root_pid = substr($form_values['collection_pid'], 0, 17);
    $form_id = $this->setIdentifier($form_values);
    $pid = $root_pid . '-'. $form_id;
    return $pid;
  }
  function setIdentifier($form_values) {
    require_once(drupal_get_path('module', 'fedora_repository') . '/ObjectHelper.inc');
    require_once(drupal_get_path('module', 'fedora_repository') . '/CollectionClass.inc');
    $collectionHelper= new CollectionClass();
    $ppid = $_SESSION['pid'];
	$ncol = substr($ppid,18);
	$numbers = array("0","1","2","3","4","5","6","7","8","9");
	$newcol = str_replace($numbers,'',$ncol);

    $itqlquery = 'select $object from <#ri> where $object <fedora-rels-ext:isPartOf><info:fedora/'. $ppid . '> and $object <fedora-rels-ext:isMemberOfCollection><info:fedora/vre:mnpl-compounds-'.$newcol.'>and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>';
    $relatedItems=$collectionHelper->getRelatedItems($this->pid, $itqlquery);
    $sxe=new SimpleXMLElement($relatedItems);
    $nmspace = $sxe->getNamespaces(TRUE);
    $regspace = $sxe->registerXPathNamespace('ri', implode($nmspace));
    $link = $sxe->xpath('//@uri');
    $labid = $_SESSION['labid'];
    if (empty($link)) {
      $ident = $labid . '-C-0';
    }
    else { //loop through returns , trim to create identifier and increment highest value
      $xia = array();
      foreach ($link as $path) {
        $path1= substr($path, '30');
        $path2= strrchr($path1, "-");
        $path= str_replace($path2, '', $path1);
        $xi= ltrim($path2, "-");
        $xnew = array_push($xia, $xi);
      }
      $num = max($xia);
      $numinc = ($num+1);
      $ident = $labid . '-C-'. $numinc;
    }

    return $ident;
  }

/*
 * method overrides method in FormBuilder.  We changed the dsid from OBJ to COMPOUND 
 */
  function createCompoundStream($form_values, &$dom, &$rootElement) {
    $datastream = $dom->createElement("foxml:datastream");
    $datastream->setAttribute("ID", "COMPOUND");
    $datastream->setAttribute("STATE", "A");
    $datastream->setAttribute("CONTROL_GROUP", "X");
    $version = $dom->createElement("foxml:datastreamVersion");
    $version->setAttribute("ID", "COMPOUND.0");
    $version->setAttribute("MIMETYPE", "text/xml");
    $version->setAttribute("LABEL", $this->setIdentifier($form_values) . ' Compound');
    $datastream->appendChild($version);
    $content = $dom->createElement("foxml:xmlContent");
    $version->appendChild($content);
    ///begin writing compound
    $oai = $dom->createElement("compounds:sample");
    $oai->setAttribute('xmlns:compounds', "http://vre.upei.ca/compounds/");
    $content->appendChild($oai);
    //dc elements
    $element = $dom->createElement('compounds:identifier', $this->setIdentifier($form_values));
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:labid', $this->setIdentifier($form_values));
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:name', $form_values['compounds:name']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:author', $form_values['compounds:author']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:date', $form_values['compounds:date']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:new', $form_values['compounds:new']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:references', $form_values['compounds:references']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:weight', $form_values['compounds:weight']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:purification_com', $form_values['compounds:purification_com']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:inhibitors', $form_values['compounds:inhibitors']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:ptp1b', $form_values['compounds:ptp1b']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:ptp1b_com', $form_values['compounds:ptp1b_com']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:hct116', $form_values['compounds:hct116']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:hct116_com', $form_values['compounds:hct116_com']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:hela', $form_values['compounds:hela']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:hela_com', $form_values['compounds:hela_com']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:pc3', $form_values['compounds:pc3']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:pc3_com', $form_values['compounds:pc3_com']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:are', $form_values['compounds:are']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:are_com', $form_values['compounds:are_com']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:antiproliferative', $form_values['compounds:antiproliferative']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:antiproliferative_com', $form_values['compounds:antiproliferative_com']);
    $oai->appendChild($element);

       $element = $dom->createElement('compounds:staphylococcus', $form_values['compounds:staphylococcus']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:staphylococcus_com', $form_values['compounds:staphylococcus_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:enterococcus', $form_values['compounds:enterococcus']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:enterococcus_com', $form_values['compounds:enterococcus_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:pseudomonas', $form_values['compounds:pseudomonas']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:pseudomonas_com', $form_values['compounds:pseudomonas_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:candida', $form_values['compounds:candida']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:candida_com', $form_values['compounds:candida_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:mrsa', $form_values['compounds:mrsa']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:mrsa_com', $form_values['compounds:mrsa_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:vre', $form_values['compounds:vre']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:vre_com', $form_values['compounds:vre_com']);
        $oai->appendChild($element);    
        $element = $dom->createElement('compounds:mycodiern', $form_values['compounds:mycodiern']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:mycodiern_com', $form_values['compounds:mycodiern_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:mycosmeg', $form_values['compounds:mycosmeg']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:mycosmeg_com', $form_values['compounds:mycosmeg_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:mycoterrae', $form_values['compounds:mycoterrae']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:mycoterrae_com', $form_values['compounds:mycoterrae_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:lypoxygenase', $form_values['compounds:lypoxygenase']);
        $oai->appendChild($element);
        $element = $dom->createElement('compounds:lypoxygenase_com', $form_values['compounds:lypoxygenase_com']);
        $oai->appendChild($element);
    
 
    $element = $dom->createElement('compounds:location', $form_values['compounds:location']);
    $oai->appendChild($element);
    $element = $dom->createElement('compounds:notes', $form_values['compounds:notes']);
    $oai->appendChild($element);

    $rootElement->appendChild($datastream);
  }

  /**
   * creates the standard foxml properties
   */
  function createStandardFedoraStuff($form_values, &$dom, &$rootElement) {

    /*foxml object properties section */
    $objproperties = $dom->createElement("foxml:objectProperties");
    $prop2 = $dom->createElement("foxml:property");
    $prop2->setAttribute("NAME", "info:fedora/fedora-system:def/model#state");
    $prop2->setAttribute("VALUE", "A");
    $prop3 = $dom->createElement("foxml:property");
    $prop3->setAttribute("NAME", "info:fedora/fedora-system:def/model#label");
    $prop3->setAttribute("VALUE", $this->setIdentifier($form_values) . ' Compound');
    $prop5 = $dom->createElement("foxml:property");
    $prop5->setAttribute("NAME", "info:fedora/fedora-system:def/model#ownerId");
    $prop5->setAttribute("VALUE", $form_values['user_id']);
    //$objproperties->appendChild($prop1);
    $objproperties->appendChild($prop2);
    $objproperties->appendChild($prop3);
    $objproperties->appendChild($prop5);
    $rootElement->appendChild($objproperties);

  }


  /**
   * creates the RELS-EXT for the foxml
   */
  function createRelationShips($form_values, &$dom, &$rootElement) {

    $drdf = $dom->createElement("foxml:datastream");
    $drdf->setAttribute("ID", "RELS-EXT");
    $drdf->setAttribute("CONTROL_GROUP", "X");
    $dvrdf = $dom->createElement("foxml:datastreamVersion");
    $dvrdf->setAttribute("FORMAT_URI", "info:fedora/fedora-system:FedoraRELSExt-1.0");
    $dvrdf->setAttribute("ID", "RELS-EXT.0");
    $dvrdf->setAttribute("MIMETYPE", "application/rdf+xml");
    $dvrdf->setAttribute("LABEL", "RDF Statements about this Object");
    $dvcontent = $dom->createElement("foxml:xmlContent");
    $rdf = $dom->createElement("rdf:RDF");
    $rdf->setAttribute("xmlns:rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
    $rdf->setAttribute("xmlns:rdfs", "http://www.w3.org/2000/01/rdf-schema#");
    $rdf->setAttribute("xmlns:fedora", "info:fedora/fedora-system:def/relations-external#");
    $rdf->setAttribute("xmlns:dc", "http://purl.org/dc/elements/1.1/");
    $rdf->setAttribute("xmlns:oai_dc", "http://www.openarchives.org/OAI/2.0/oai_dc/");
    $rdf->setAttribute("xmlns:fedora-model", "info:fedora/fedora-system:def/model#");
    $rdfdesc = $dom->createElement("rdf:description");
    $rdfdesc->setAttribute("rdf:about", 'info:fedora/'. $this->setPid($form_values));
    session_start();
    $parent= $_SESSION['pid'];
	// create new collection suffix from parent
	$ncol = substr($parent,18);
	$numbers = array("0","1","2","3","4","5","6","7","8","9");
	$newcol = str_replace($numbers,'',$ncol);
    $part = $dom->createElement("fedora:isPartOf");
    $part->setAttribute("rdf:resource", "info:fedora/$parent");
    $member = $dom->createElement("fedora:isMemberOfCollection");
    $membr = $form_values['collection_pid'];
    $member->setAttribute("rdf:resource", "info:fedora/$membr-$newcol");
    $rdfHasModel = $dom->createElement("fedora-model:hasModel");
    $contentModelPid=$form_values['content_model_pid'];
    $rdfHasModel->setAttribute("rdf:resource", "info:fedora/$contentModelPid");

    $drdf->appendChild($dvrdf);
    $dvrdf->appendChild($dvcontent);
    $dvcontent->appendChild($rdf);
    $rdf->appendChild($rdfdesc);
    $rdfdesc->appendChild($member);
    $rdfdesc->appendChild($part);
    $rdfdesc->appendChild($rdfHasModel);
    $rootElement->appendChild($drdf);

  }

  function createReference(&$dom, &$rootElement) {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $objectHelper = new ObjectHelper();
    $parent= $_SESSION['pid'];
    $dsid = 'CRITTER';
    $critterStreamDoc = $objectHelper->getStream($parent, $dsid, TRUE);
    try {
      $xml = new SimpleXMLElement($critterStreamDoc);
    } 
    catch (Exception $e) {
      watchdog(t("Fedora_Repository"), "Problem getting Critter Stream!", WATCHDOG_ERROR);
      drupal_set_message(t('Problem getting Critter Stream! ') . $e->getMessage(), 'error');
      return FALSE;
    }
    $critterElement = $dom->createDocumentFragment();
    if (!$critterElement) {
      drupal_set_message(t('error parsing Critter stream!'));
      watchdog(t("Fedora_Repository"), "Error parsing Critter stream, could not parse policy stream!", WATCHDOG_NOTICE);
      return FALSE;
    }
    $dom->importNode($critterElement, TRUE);
    $value=$critterElement->appendXML($critterStreamDoc);
    if (!$value) {
      drupal_set_message(t('error creating specimen stream!'));
      watchdog(t("Fedora_Repository"), "Error creating specimen stream, could not parse critter template!", WATCHDOG_NOTICE);
      return FALSE;
    }

    $ds1 = $dom->createElement("foxml:datastream");
    $rootElement->appendChild($ds1);
    $ds1->setAttribute("ID", "CRITTER");
    $ds1->setAttribute("STATE", "A");
    $ds1->setAttribute("CONTROL_GROUP", "X");
    $ds1v = $dom->createElement("foxml:datastreamVersion");
    $ds1->appendChild($ds1v);
    $ds1v->setAttribute("ID", "CRITTER.0");
    $ds1v->setAttribute("MIMETYPE", "text/xml");
    $ds1v->setAttribute("LABEL", "Parent Critter");

    $content = $dom->createElement("foxml:xmlContent");
    $ds1v->appendChild($content);
    $content->appendChild($critterElement);
    return TRUE;

  }

  function createPolicy($collectionPid, &$dom, &$rootElement) {
    module_load_include('inc', 'fedora_repository', 'ObjectHelper');
    $objectHelper = new ObjectHelper();

    $dsid = 'CHILD_SECURITY';
    $policyStreamDoc = $objectHelper->getStream($collectionPid, $dsid, TRUE);

    try {
      $xml = new SimpleXMLElement($policyStreamDoc);
    } 
    catch (Exception $e) {
      watchdog(t("Fedora_Repository"), "Problem getting Security Policy!", WATCHDOG_ERROR);
      drupal_set_message(t('Problem getting Security Policy! ') . $e->getMessage(), 'error');
      return FALSE;
    }
    $policyElement = $dom->createDocumentFragment();
    if (!$policyElement) {
      drupal_set_message(t('error parsing security policy stream!'));
      watchdog(t("Fedora_Repository"), "Error parsing security policy stream, could not parse policy stream!", WATCHDOG_NOTICE);
      return FALSE;
    }
    $dom->importNode($policyElement, TRUE);
    $value=$policyElement->appendXML($policyStreamDoc);
    if (!$value) {
      drupal_set_message(t('error creating security policy stream!'));
      watchdog(t("Fedora_Repository"), "Error creating security policy stream, could not parse collection policy template file!", WATCHDOG_NOTICE);
      return FALSE;
    }

    $ds1 = $dom->createElement("foxml:datastream");
    $rootElement->appendChild($ds1);
    $ds1->setAttribute("ID", "POLICY");
    $ds1->setAttribute("STATE", "A");
    $ds1->setAttribute("CONTROL_GROUP", "X");
    $ds1v = $dom->createElement("foxml:datastreamVersion");
    $ds1->appendChild($ds1v);
    $ds1v->setAttribute("ID", "POLICY.0");
    $ds1v->setAttribute("MIMETYPE", "text/xml");
    $ds1v->setAttribute("LABEL", "POLICY");
    //$ds1v->setAttribute("FORMAT_URI", "info:fedora/fedora-system:format/xml.fedora.audit");
    $content = $dom->createElement("foxml:xmlContent");
    $ds1v->appendChild($content);
    $content->appendChild($policyElement);
    return TRUE;
  }



  function buildCompoundForm(&$form, $ingest_form_definition, &$form_values) {

              $radioValues = array ();
                $radioValues['No_Assay'] = 'No_Assay';
                $radioValues['Hit'] = 'Hit';
                $radioValues['Strong'] = 'Strong';
                $radioValues['Medium'] = 'Medium';
                $radioValues['Low'] = 'Low';
                $radioValues['Inactive'] = 'Inactive';

            
            $form['mainfieldset']=array(
            '#type'=>'fieldset',
            '#title'=>'Ingest Form',
            '#collapsible'=>FALSE,
            '#collapsed'=>FALSE,
            );
    drupal_add_css( drupal_get_path( 'module', 'fedora_repository' ) . '/mnpl/mnpl_forms.css', 'module');   
            $form['mainfieldset']['compounds:name']=array(
                '#type'=>'textfield',
                '#title'=>'<strong>Compound Name</strong>',
                );
		    $form['mainfieldset']['compounds:labid']=array(
                '#type'=>'textfield',
                '#title'=>'<strong>Compound Lab ID</strong>',
                );
           $form['mainfieldset']['compounds:author']=array(
               '#type'=>'textfield',
               '#title'=>'<strong>Author</strong>',
               );
            $form['mainfieldset']['compounds:date']=array(
                '#type'=>'textfield',
                '#title'=>'<strong>Date</strong>',
                '#description' => '<strong>Use date format yyyy-mm-dd only. Example: 2010-05-23</strong>'
                );

            $newopts = array('Yes'=>'Yes','No'=>'No');  
            $form['mainfieldset']['compounds:new']=array(
                '#type'=>'radios',
                '#prefix'=> '<div class="horiz_checkboxes">',
                '#suffix' => '</div><br /><br />',
                '#options'=> $newopts,
                '#title'=>'<strong>New</strong>',
                );
            $form['mainfieldset']['compounds:references']=array(
                '#type'=>'textfield',
                '#title'=>'<strong>References</strong>',
                );
            $form['mainfieldset']['compounds:weight']=array(
                '#type'=>'textfield',
                '#title'=>'<strong>Weight</strong>',
                );
            $form['mainfieldset']['compounds:purification_com']=array(
                '#type'=>'textfield',
                '#title'=>'<strong>Purification Comments</strong>',
                );
            $form['mainfieldset']['compounds:inhibitors']=array(
                '#type'=>'item',
                '#title'=>'<strong>Inhibitors</strong>',
                );
    // PTP1B
            $form['mainfieldset']['fs1'] = array(
                                '#type' => 'fieldset',
                                '#title'=> 'PTP1B',
                                '#collapsible' => TRUE,
                                '#collapsed' => TRUE,
                                );
            $form['mainfieldset']['fs1']['compounds:ptp1b'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>PTP1B</strong>'
            ));
            $form['mainfieldset']['fs1']['compounds:ptp1b_com'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>PTP1B Comments</strong>'
            ));
            // HCT116
            $form['mainfieldset']['fs2'] = array(
                '#type' => 'fieldset',
                '#title'=> 'HCT116',
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
                );
            $form['mainfieldset']['fs2']['compounds:hct116'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>HCT116</strong>'
            ));
            $form['mainfieldset']['fs2']['compounds:hct116_com'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>HCT116 Comments</strong>'
            ));
            // HELA
            $form['mainfieldset']['fs3'] = array(
                '#type' => 'fieldset',
                '#title'=> 'HELA',
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
                );
            $form['mainfieldset']['fs3']['compounds:hela'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>HELA</strong>'
            ));
            $form['mainfieldset']['fs3']['compounds:hela_com'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>HELA Comments</strong>'
            ));

            // PC3
            $form['mainfieldset']['fs4'] = array(
                '#type' => 'fieldset',
                '#title'=> 'PC3',
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
                );
            $form['mainfieldset']['fs4']['compounds:pc3'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>PC3</strong>'
            ));
            $form['mainfieldset']['fs4']['compounds:pc3_com'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>PC3 Comments</strong>'
            ));
            
            // ARE
            $form['mainfieldset']['fs5'] = array(
                '#type' => 'fieldset',
                '#title'=> 'ARE',
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
                );
            $form['mainfieldset']['fs5']['compounds:are'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>ARE</strong>'
            ));
            $form['mainfieldset']['fs5']['compounds:are_com'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>ARE Comments</strong>'
            ));
            
            
            // Antiproliferative
            $form['mainfieldset']['fs6'] = array(
                '#type' => 'fieldset',
                '#title'=> 'Antiproliferative',
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
                );
            $form['mainfieldset']['fs6']['compounds:antiproliferative'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>Antiproliferative</strong>'
            ));
            $form['mainfieldset']['fs6']['compounds:antiproliferative_com'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>Antiproliferative Comments</strong>'
            ));
            // Staphylococcus aureaus
            $form['mainfieldset']['fs7'] = array(
                '#type' => 'fieldset',
                '#title'=> 'Staphylococcus aureaus',
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
                );
            $form['mainfieldset']['fs7']['compounds:staphylococcus'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>Staphylococcus aureaus</strong>'
            ));
            $form['mainfieldset']['fs7']['compounds:staphylococcus_com'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>Staphylococcus aureaus Comments</strong>'
            ));
            // Enterococcus faecalis
            $form['mainfieldset']['fs8'] = array(
                '#type' => 'fieldset',
                '#title'=> 'Enterococcus faecalis',
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
                );
            $form['mainfieldset']['fs8']['compounds:enterococcus'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>PCR</strong>'
            ));
            $form['mainfieldset']['fs8']['compounds:enterococcus_com'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>Enterococcus faecalis Comments</strong>'
            ));
    
           $form['mainfieldset']['fs9'] = array(
               '#type' => 'fieldset',
               '#title'=> 'Pseudomonas aeruginosa',
               '#collapsible' => TRUE,
               '#collapsed' => TRUE,
               );
           $form['mainfieldset']['fs9']['compounds:pseudomonas'] = array (
               '#type' => 'radios',
               '#default_value' => 'No_Assay',
               '#options'=> $radioValues,
               '#title' => t('<strong>Pseudomonas aeruginosa</strong>'
           ));
           $form['mainfieldset']['fs9']['compounds:pseudomonas_com'] = array (
               '#type' => 'textarea',
               '#title' => t('<strong>Pseudomonas aeruginosa Comments</strong>'
           ));
        // Candida Albicans
          $form['mainfieldset']['fs10'] = array(
              '#type' => 'fieldset',
              '#title'=> 'Candida Albicans',
              '#collapsible' => TRUE,
              '#collapsed' => TRUE,
              );
          $form['mainfieldset']['fs10']['compounds:candida'] = array (
              '#type' => 'radios',
              '#default_value' => 'No_Assay',
              '#options'=> $radioValues,
              '#title' => t('<strong>Candida Albicans</strong>'
          ));
          $form['mainfieldset']['fs10']['compounds:candida_com'] = array (
              '#type' => 'textarea',
              '#title' => t('<strong>Candida Albicans Comments</strong>'
          ));

        // MRSA
        $form['mainfieldset']['fs11'] = array(
            '#type' => 'fieldset',
            '#title'=> 'MRSA',
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            );
        $form['mainfieldset']['fs11']['compounds:mrsa'] = array (
            '#type' => 'radios',
            '#default_value' => 'No_Assay',
            '#options'=> $radioValues,
            '#title' => t('<strong>MRSA</strong>'
        ));
        $form['mainfieldset']['fs11']['compounds:mrsa_com'] = array (
            '#type' => 'textarea',
            '#title' => t('<strong>MRSA Comments</strong>'
        ));
    
    // VRE
    $form['mainfieldset']['fs12'] = array(
        '#type' => 'fieldset',
        '#title'=> 'VRE',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        );
    $form['mainfieldset']['fs12']['compounds:vre'] = array (
        '#type' => 'radios',
        '#default_value' => 'No_Assay',
        '#options'=> $radioValues,
        '#title' => t('<strong>VRE</strong>'
    ));
    $form['mainfieldset']['fs12']['compounds:vre_com'] = array (
        '#type' => 'textarea',
        '#title' => t('<strong>VRE Comments</strong>'
    ));
    
    // Mycobacterium diernhoferi
    $form['mainfieldset']['fs13'] = array(
        '#type' => 'fieldset',
        '#title'=> 'Mycobacterium diernhoferi',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        );
    $form['mainfieldset']['fs13']['compounds:mycodiern'] = array (
        '#type' => 'radios',
        '#default_value' => 'No_Assay',
        '#options'=> $radioValues,
        '#title' => t('<strong>Mycobacterium diernhoferi</strong>'
    ));
    $form['mainfieldset']['fs13']['compounds:mycodiern_com'] = array (
        '#type' => 'textarea',
        '#title' => t('<strong>Mycobacterium diernhoferi Comments</strong>'
    ));
    
    
        // Mycobacterium smegmatis
        $form['mainfieldset']['fs14'] = array(
            '#type' => 'fieldset',
            '#title'=> 'Mycobacterium smegmatis',
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            );
        $form['mainfieldset']['fs14']['compounds:mycosmeg'] = array (
            '#type' => 'radios',
            '#default_value' => 'No_Assay',
            '#options'=> $radioValues,
            '#title' => t('<strong>Mycobacterium smegmatis</strong>'
        ));
        $form['mainfieldset']['fs14']['compounds:mycosmeg_com'] = array (
            '#type' => 'textarea',
            '#title' => t('<strong>Mycobacterium smegmatis Comments</strong>'
        ));
        // Mycobacterium terrae
        $form['mainfieldset']['fs15'] = array(
            '#type' => 'fieldset',
            '#title'=> ' Mycobacterium terrae',
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            );
        $form['mainfieldset']['fs15']['compounds:mycoterrae'] = array (
            '#type' => 'radios',
            '#default_value' => 'No_Assay',
            '#options'=> $radioValues,
            '#title' => t('<strong> Mycobacterium terrae</strong>'
        ));
        $form['mainfieldset']['fs15']['compounds:mycoterrae_com'] = array (
            '#type' => 'textarea',
            '#title' => t('<strong>Mycobacterium terrae Comments</strong>'
        ));
    
        // Lypoxygenase
        $form['mainfieldset']['fs16'] = array(
            '#type' => 'fieldset',
            '#title'=> 'Lypoxygenase',
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            );
        $form['mainfieldset']['fs16']['compounds:lypoxygenase'] = array (
            '#type' => 'radios',
            '#default_value' => 'No_Assay',
            '#options'=> $radioValues,
            '#title' => t('<strong>Lypoxygenase</strong>'
        ));
        $form['mainfieldset']['fs16']['compounds:lypoxygenase_com'] = array (
            '#type' => 'textarea',
            '#title' => t('<strong>Lypoxygenase</strong>'
        ));

        $form['mainfieldset']['compounds:location'] = array (
            '#type' => 'textfield',
            '#title' => t('<strong>Location</strong>'
        ));

            $form['mainfieldset']['compounds:notes'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>Notes</strong>'
            ));

            $form['mainfieldset']['pid'] = array (
                '#type' => 'hidden',
                '#value' => $this->setPid($form_values),
            );

            $form['mainfieldset']['dsid'] = array (
                        '#type' => 'hidden',
                        '#value' => 'COMPOUND'
                    );

            return $form;
                  }
    
    
  //   $form['indicator2'] = array(
  //       '#type' => 'fieldset',
  //       '#title' => t('Ingest Digital Object Step #2')
  //   );
  //   drupal_add_css( drupal_get_path( 'module', 'fedora_repository' ) . '/mnpl/mnpl_forms.css', 'module');
  //   foreach ($ingest_form_definition->form_elements->element as $element) {
  //     $name=strip_tags($element->name->asXML());
  //     $title=strip_tags($element->label->asXML());
  //     $required = strip_tags($element->required->asXML());
  //     $required=strtolower($required);
  //     if ($required!='TRUE') {
  //       $required='0';
  //     }
  //     $description = strip_tags($element->description->asXML());
  //     $type = strip_tags($element->type->asXML());
  //     $options=array();
  // 
  //     switch ($element->type) {
  //       case ($element->type == 'radios');
  //         foreach ($element->authoritative_list->item as $item) {
  //           $field = strip_tags($item->field->asXML());
  //           $value = strip_tags($item->value->asXML());
  //           $options["$field"] = $value;
  //         }
  //         $form['indicator2']["$name"] = array(
  //             '#title' => $title,
  //             '#required' => $required,
  //             '#description' => $description,
  //             '#type' => $type,
  //             '#options' => $options,
  //             '#default_value' => 'No_Assay'
  //         );
  //         if ( $element->name == 'compounds:new' ) {
  //           $form['indicator2']["$name"]['#prefix'] = '<div class="horiz_checkboxes">';
  //           $form['indicator2']["$name"]['#suffix'] = '</div><br /><br />';
  //           $form['indicator2']["$name"]['#default_value'] = NULL;
  //         }
  //     // $form['#redirect'] = 'fedora/repository/'.$_SESSION['pid'];
  //         break;
  //       default;
  //         $form['indicator2']["$name"]=array(
  //             '#title' => $title,
  //             '#required' => $required,
  //             '#description' => $description,
  //             '#type' => $type,
  // 
  //         );
  //     // $form['#redirect'] = 'fedora/repository/'.$_SESSION['pid'];
  // 
  //     }
  //   }
  //   return $form;
  // }


  function handleCompoundForm($form_values) {
    $dom = new DomDocument("1.0", "UTF-8");
    $dom->formatOutput = TRUE;
    $rootElement = $dom->createElement("foxml:digitalObject");
    $rootElement->setAttribute('VERSION', '1.1');
    $rootElement->setAttribute('PID', $this->setPid($form_values));
    $rootElement->setAttribute('xmlns:foxml', "info:fedora/fedora-system:def/foxml#");
    $rootElement->setAttribute('xmlns:xsi', "http://www.w3.org/2001/XMLSchema-instance");
    $rootElement->setAttribute('xsi:schemaLocation', "info:fedora/fedora-system:def/foxml# http://www.fedora.info/definitions/1/0/foxml1-0.xsd");
    $dom->appendChild($rootElement);
    //create standard fedora stuff
    $this->createStandardFedoraStuff($form_values, $dom, $rootElement);
    //create relationships
    $this->createRelationShips($form_values, $dom, $rootElement);
    //create compound
    $this->createCompoundStream($form_values, $dom, $rootElement);
    $this->createReference($dom, $rootElement);
    $parent= $form_values['collection_pid'];
    $this->createPolicy($parent, $dom, $rootElement);

    // // test output
    // $test = $dom->saveXML();
    // var_dump($test);exit(0);

    $params = array(
      'objectXML' => $dom->saveXML(),
      'format' => "info:fedora/fedora-system:FOXML-1.1",
      'logMessage' => "Fedora Object Ingested"
    );
    try {
      $soapHelper = new ConnectionHelper();
      $client=$soapHelper->getSoapClient(variable_get('fedora_soap_manage_url', 'http://localhost:8080/fedora/services/management?wsdl'));

      if ($client == NULL) {
        drupal_set_message(t('Error Getting Soap Client.'), 'error');
        return;
      }
      $object=$client->__soapCall('ingest', array($params));
    // $deleteFiles= $form_values['delete_file'];//remove files from drupal file system
    //
    // if($deleteFiles>0&&isset($_SESSION['fedora_ingest_files'][0])){
    //  foreach($_SESSION['fedora_ingest_files'] as $dsid => $createdFile){
    //    unlink($createdFile);
    //  }
    //    unlink($form_values['fullpath']);
    // }
    }
    catch (exception $e) {
      drupal_set_message(t('Error Ingesting Compound Object! ') . $e->getMessage(), 'error');
      watchdog(t("Fedora_Repository"), t("Error Ingesting Compound Object!") . $e->getMessage(), WATCHDOG_ERROR);

      return;
    }
  }
}

