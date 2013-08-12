<?php
// $Id$

module_load_include('inc', 'fedora_repository', 'plugins/FormBuilder');
session_start();
/*
 * implements methods from content model ingest form xml
 * builds a fraction metadata form for ingest and edit
 */
class FractionsFormBuilder extends FormBuilder{
    function FractionsFormBuilder() {
        include_once('includes/bootstrap.inc');
        drupal_bootstrap(DRUPAL_BOOTSRTAP_FULL);

    }
    function setIdentifier($form_values) {
      require_once(drupal_get_path('module', 'fedora_repository') . '/ObjectHelper.inc');
      require_once(drupal_get_path('module', 'fedora_repository') . '/CollectionClass.inc');
      $collectionHelper= new CollectionClass();
      $ppid = $_SESSION['pid'];
      $itqlquery = 'select $object from <#ri> where $object <fedora-rels-ext:isPartOf><info:fedora/'. $ppid . '> and $object <fedora-rels-ext:isMemberOfCollection><info:fedora/vre:mnpl-fractions>and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>';
      $relatedItems=$collectionHelper->getRelatedItems($this->pid, $itqlquery);
      $sxe=new SimpleXMLElement($relatedItems);
      $nmspace = $sxe->getNamespaces(TRUE);
      $regspace = $sxe->registerXPathNamespace('ri', implode($nmspace));
      $link = $sxe->xpath('//@uri');
      $labid = $_SESSION['labid'];
      if (empty($link)) {
          $ident = $labid . '-F-0';
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
        $ident = $labid . '-F-'. $numinc;
      }
      return $ident;
    }

    function setPid($form_values) {
      // set pid custom pid for fractions... add identifier to base... finish with getNextPid pid is generated at form_values level as 'pid'
      $root_pid = substr($form_values['collection_pid'], 0, 17);
      $form_id = $this->setIdentifier($form_values);
      $pid = $root_pid . '-'. $form_id;
      return $pid;
    }

/*
 * method overrides method in FormBuilder.  We changed the dsid from OBJ to FRACTION
 */
    function createFractionStream($form_values, &$dom, &$rootElement) {
        $datastream = $dom->createElement("foxml:datastream");
        $datastream->setAttribute("ID", "FRACTION");
        $datastream->setAttribute("STATE", "A");
        $datastream->setAttribute("CONTROL_GROUP", "X");
        $version = $dom->createElement("foxml:datastreamVersion");
        $version->setAttribute("ID", "FRACTION.0");
        $version->setAttribute("MIMETYPE", "text/xml");
        $version->setAttribute("LABEL", $this->setIdentifier($form_values) . ' Fraction');
        $datastream->appendChild($version);
        $content = $dom->createElement("foxml:xmlContent");
        $version->appendChild($content);
        ///begin writing fraction
        $oai = $dom->createElement("fractions:sample");
        $oai->setAttribute('xmlns:fractions', "http://vre.upei.ca/fractions/");
        $content->appendChild($oai);
        //dc elements
        $element = $dom->createElement('fractions:identifier', $this->setIdentifier($form_values));
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:plate', $form_values['fractions:plate']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:weight', $form_values['fractions:weight']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:ptp1b', $form_values['fractions:ptp1b']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:ptp1b_com', $form_values['fractions:ptp1b_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:hct116', $form_values['fractions:hct116']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:hct116_com', $form_values['fractions:hct116_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:hela', $form_values['fractions:hela']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:hela_com', $form_values['fractions:hela_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:pc3', $form_values['fractions:pc3']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:pc3_com', $form_values['fractions:pc3_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:are', $form_values['fractions:are']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:are_com', $form_values['fractions:are_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:antiproliferative', $form_values['fractions:antiproliferative']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:antiproliferative_com', $form_values['fractions:antiproliferative_com']);
        $oai->appendChild($element);
       $element = $dom->createElement('fractions:staphylococcus', $form_values['fractions:staphylococcus']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:staphylococcus_com', $form_values['fractions:staphylococcus_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:enterococcus', $form_values['fractions:enterococcus']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:enterococcus_com', $form_values['fractions:enterococcus_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:pseudomonas', $form_values['fractions:pseudomonas']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:pseudomonas_com', $form_values['fractions:pseudomonas_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:candida', $form_values['fractions:candida']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:candida_com', $form_values['fractions:candida_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:mrsa', $form_values['fractions:mrsa']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:mrsa_com', $form_values['fractions:mrsa_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:vre', $form_values['fractions:vre']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:vre_com', $form_values['fractions:vre_com']);
        $oai->appendChild($element);    
        $element = $dom->createElement('fractions:mycodiern', $form_values['fractions:mycodiern']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:mycodiern_com', $form_values['fractions:mycodiern_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:mycosmeg', $form_values['fractions:mycosmeg']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:mycosmeg_com', $form_values['fractions:mycosmeg_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:mycoterrae', $form_values['fractions:mycoterrae']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:mycoterrae_com', $form_values['fractions:mycoterrae_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:lypoxygenase', $form_values['fractions:lypoxygenase']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:lypoxygenase_com', $form_values['fractions:lypoxygenase_com']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:location', $form_values['fractions:location']);
        $oai->appendChild($element);
        $element = $dom->createElement('fractions:notes', $form_values['fractions:notes']);
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
        $prop3->setAttribute("VALUE", $this->setIdentifier($form_values) . ' Fraction');
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
        $part = $dom->createElement("fedora:isPartOf");
        $part->setAttribute("rdf:resource", "info:fedora/$parent");
        $member = $dom->createElement("fedora:isMemberOfCollection");
        $membr = $form_values['collection_pid'];
        $member->setAttribute("rdf:resource", "info:fedora/$membr");
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
        // unset($_SESSION['pid']);
    }


    function createReference(&$dom, &$rootElement) {
         module_load_include('inc', 'fedora_repository', 'ObjectHelper');
        $objectHelper = new ObjectHelper();
         $parent= $_SESSION['pid'];
         $dsid = 'CRITTER';        
        $critterStreamDoc = $objectHelper->getStream($parent, $dsid, TRUE);        
        try {
            $xml = new SimpleXMLElement($critterStreamDoc);
        } catch (Exception $e) {
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
        } catch (Exception $e) {
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
    //$ds1v->setAttribute("FORMAT_URI","info:fedora/fedora-system:format/xml.fedora.audit");
    $content = $dom->createElement("foxml:xmlContent");
    $ds1v->appendChild($content);
    $content->appendChild($policyElement);
    return TRUE;
    }

  function buildFractionForm(&$form, $ingest_form_definition, &$form_values) {

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
            $form['mainfieldset']['fractions:identifier']=array(
                '#type'=>'hidden',
                '#title'=>'<strong>Identifier</strong>',
                );

            $form['mainfieldset']['fractions:plate']=array(
                '#type'=>'textfield',
                '#title'=>'<strong>Plate Number</strong>',
                );


	    // $form['mainfieldset']['fractions:labid']=array(
		    //                 '#type'=>'textfield',
		    //                 '#title'=>'<strong>Fraction Lab ID</strong>',
		    //                 );
		    //            $form['mainfieldset']['fractions:author']=array(
		    //                '#type'=>'textfield',
		    //                '#title'=>'<strong>Author</strong>',
		    //                );
		    //             $form['mainfieldset']['fractions:date']=array(
		    //                 '#type'=>'textfield',
		    //                 '#title'=>'<strong>Date</strong>',
		    //                 '#description' => '<strong>Use date format yyyy-mm-dd only. Example: 2010-05-23</strong>'
		    //                 );
		    // 
		    //             $newopts = array('Yes'=>'Yes','No'=>'No');  
		    //             $form['mainfieldset']['fractions:new']=array(
		    //                 '#type'=>'radios',
		    //                 '#prefix'=> '<div class="horiz_checkboxes">',
		    //                 '#suffix' => '</div><br /><br />',
		    //                 '#options'=> $newopts,
		    //                 '#title'=>'<strong>New</strong>',
		    //                 );
		    //             $form['mainfieldset']['fractions:references']=array(
		    //                 '#type'=>'textfield',
		    //                 '#title'=>'<strong>References</strong>',
                // );
            $form['mainfieldset']['fractions:weight']=array(
                '#type'=>'textfield',
                '#title'=>'<strong>Weight</strong>',
                );
            $form['mainfieldset']['fractions:inhibitors']=array(
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
            $form['mainfieldset']['fs1']['fractions:ptp1b'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>PTP1B</strong>'
            ));
            $form['mainfieldset']['fs1']['fractions:ptp1b_com'] = array (
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
            $form['mainfieldset']['fs2']['fractions:hct116'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>HCT116</strong>'
            ));
            $form['mainfieldset']['fs2']['fractions:hct116_com'] = array (
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
            $form['mainfieldset']['fs3']['fractions:hela'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>HELA</strong>'
            ));
            $form['mainfieldset']['fs3']['fractions:hela_com'] = array (
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
            $form['mainfieldset']['fs4']['fractions:pc3'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>PC3</strong>'
            ));
            $form['mainfieldset']['fs4']['fractions:pc3_com'] = array (
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
            $form['mainfieldset']['fs5']['fractions:are'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>ARE</strong>'
            ));
            $form['mainfieldset']['fs5']['fractions:are_com'] = array (
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
            $form['mainfieldset']['fs6']['fractions:antiproliferative'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>Antiproliferative</strong>'
            ));
            $form['mainfieldset']['fs6']['fractions:antiproliferative_com'] = array (
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
            $form['mainfieldset']['fs7']['fractions:staphylococcus'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>Staphylococcus aureaus</strong>'
            ));
            $form['mainfieldset']['fs7']['fractions:staphylococcus_com'] = array (
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
            $form['mainfieldset']['fs8']['fractions:enterococcus'] = array (
                '#type' => 'radios',
                '#default_value' => 'No_Assay',
                '#options'=> $radioValues,
                '#title' => t('<strong>PCR</strong>'
            ));
            $form['mainfieldset']['fs8']['fractions:enterococcus_com'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>Enterococcus faecalis Comments</strong>'
            ));
    
           $form['mainfieldset']['fs9'] = array(
               '#type' => 'fieldset',
               '#title'=> 'Pseudomonas aeruginosa',
               '#collapsible' => TRUE,
               '#collapsed' => TRUE,
               );
           $form['mainfieldset']['fs9']['fractions:pseudomonas'] = array (
               '#type' => 'radios',
               '#default_value' => 'No_Assay',
               '#options'=> $radioValues,
               '#title' => t('<strong>Pseudomonas aeruginosa</strong>'
           ));
           $form['mainfieldset']['fs9']['fractions:pseudomonas_com'] = array (
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
          $form['mainfieldset']['fs10']['fractions:candida'] = array (
              '#type' => 'radios',
              '#default_value' => 'No_Assay',
              '#options'=> $radioValues,
              '#title' => t('<strong>Candida Albicans</strong>'
          ));
          $form['mainfieldset']['fs10']['fractions:candida_com'] = array (
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
        $form['mainfieldset']['fs11']['fractions:mrsa'] = array (
            '#type' => 'radios',
            '#default_value' => 'No_Assay',
            '#options'=> $radioValues,
            '#title' => t('<strong>MRSA</strong>'
        ));
        $form['mainfieldset']['fs11']['fractions:mrsa_com'] = array (
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
    $form['mainfieldset']['fs12']['fractions:vre'] = array (
        '#type' => 'radios',
        '#default_value' => 'No_Assay',
        '#options'=> $radioValues,
        '#title' => t('<strong>VRE</strong>'
    ));
    $form['mainfieldset']['fs12']['fractions:vre_com'] = array (
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
    $form['mainfieldset']['fs13']['fractions:mycodiern'] = array (
        '#type' => 'radios',
        '#default_value' => 'No_Assay',
        '#options'=> $radioValues,
        '#title' => t('<strong>Mycobacterium diernhoferi</strong>'
    ));
    $form['mainfieldset']['fs13']['fractions:mycodiern_com'] = array (
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
        $form['mainfieldset']['fs14']['fractions:mycosmeg'] = array (
            '#type' => 'radios',
            '#default_value' => 'No_Assay',
            '#options'=> $radioValues,
            '#title' => t('<strong>Mycobacterium smegmatis</strong>'
        ));
        $form['mainfieldset']['fs14']['fractions:mycosmeg_com'] = array (
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
        $form['mainfieldset']['fs15']['fractions:mycoterrae'] = array (
            '#type' => 'radios',
            '#default_value' => 'No_Assay',
            '#options'=> $radioValues,
            '#title' => t('<strong> Mycobacterium terrae</strong>'
        ));
        $form['mainfieldset']['fs15']['fractions:mycoterrae_com'] = array (
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
        $form['mainfieldset']['fs16']['fractions:lypoxygenase'] = array (
            '#type' => 'radios',
            '#default_value' => 'No_Assay',
            '#options'=> $radioValues,
            '#title' => t('<strong>Lypoxygenase</strong>'
        ));
        $form['mainfieldset']['fs16']['fractions:lypoxygenase_com'] = array (
            '#type' => 'textarea',
            '#title' => t('<strong>Lypoxygenase</strong>'
        ));

        $form['mainfieldset']['fractions:location'] = array (
            '#type' => 'textfield',
            '#title' => t('<strong>Location</strong>'
        ));

            $form['mainfieldset']['fractions:notes'] = array (
                '#type' => 'textarea',
                '#title' => t('<strong>Notes</strong>'
            ));

            $form['mainfieldset']['pid'] = array (
                '#type' => 'hidden',
                '#value' => $this->setPid($form_values),
            );

            $form['mainfieldset']['dsid'] = array (
                        '#type' => 'hidden',
                        '#value' => 'FRACTION'
                    );
$form['#redirect'] = 'fedora/repository/'. $_SESSION['pid'];
            return $form;
                  }
    



   //  function buildFractionForm(&$form, $ingest_form_definition, &$form_values) {
   // 
   //       $form['indicator2']=array(
   //         '#type' => 'fieldset',
   //         '#title' => t('Add a New Fraction')
   //       );
   //       foreach ($ingest_form_definition->form_elements->element as $element) {
   //           $name=strip_tags($element->name->asXML());
   //           $title=strip_tags($element->label->asXML());
   //           $required = strip_tags($element->required->asXML());
   //           $required=strtolower($required);
   //           if ($required!='TRUE') {
   //               $required='0';
   //           }
   //           $description = strip_tags($element->description->asXML());
   //           $type = strip_tags($element->type->asXML());
   //           $options=array();
   //           if ($element->type=='radios') {
   //               foreach ($element->authoritative_list->item as $item) {
   //                   $field = strip_tags($item->field->asXML());
   //                   $value = strip_tags($item->value->asXML());
   //                   $options["$field"] = $value;
   //               }
   //               $form['indicator2']["$name"]=array(
   //               '#title' => $title,
   //               '#required' => $required,
   //               '#description' => $description,
   //               '#type' => $type,
   //               '#options' => $options,
   //               '#default_value' => 'No_Assay'
   //               );
   // $form['#redirect'] = 'fedora/repository/'. $_SESSION['pid'];
   //           }
   //           else {
   //               $form['indicator2']["$name"] = array(
   //               '#title' => $title,
   //               '#required' => $required,
   //               '#description' => $description,
   //               '#type' => $type
   //               );
   // $form['#redirect'] = 'fedora/repository/'. $_SESSION['pid'];
   //           }
   // 
   //       }
   // 
   //       return $form;
   //   }
 
    function handleFractionForm($form_values) {
        // ingest form
        $dom = new DomDocument("1.0", "UTF-8");
        $dom->formatOutput = TRUE;
        $rootElement = $dom->createElement("foxml:digitalObject");
        $rootElement->setAttribute('VERSION', '1.1');
        $pid=$this->setPid($form_values);
        $rootElement->setAttribute('PID', $pid);
        $rootElement->setAttribute('xmlns:foxml', "info:fedora/fedora-system:def/foxml#");
        $rootElement->setAttribute('xmlns:xsi', "http://www.w3.org/2001/XMLSchema-instance");
        $rootElement->setAttribute('xsi:schemaLocation', "info:fedora/fedora-system:def/foxml# http://www.fedora.info/definitions/1/0/foxml1-0.xsd");
        $dom->appendChild($rootElement);
        //create standard fedora stuff
        $this->createStandardFedoraStuff($form_values, $dom, $rootElement);
        //create relationships
        $this->createRelationShips($form_values, $dom, $rootElement);
        //create fraction
        $this->createFractionStream($form_values, $dom, $rootElement);
        $this->createReference($dom, $rootElement);
        $parent= $form_values['collection_pid'];
        $this->createPolicy($parent, $dom, $rootElement);
        // test form output
        //$test = $dom->saveXML();
        //var_dump($test);exit(0);


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
        }
        catch (exception $e) {
            drupal_set_message(t('Error Ingesting Fraction Object! ') . $e->getMessage(), 'error');
            watchdog(t("Fedora_Repository"), t("Error Ingesting Fraction Object!") . $e->getMessage(), WATCHDOG_ERROR);
            return;
        }
    }
}

