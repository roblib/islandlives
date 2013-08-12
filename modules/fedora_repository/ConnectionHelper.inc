<?php
// $Id$

/*
 * Created on Jan 24, 2008
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */

module_load_include('inc', 'ConnectionHelper', '');
class ConnectionHelper {
  function ConnectionHelper() {
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  }

  function _fixURL($url, $_name, $_pass) {
    if ($url == NULL) {
      $url=variable_get('fedora_soap_url', 'http://localhost:8080/fedora/services/access?wsdl');
    }

    if ($_name == '' || $_pass == '') {
      drupal_set_message(t('No credentials provided.'));
      return NULL;
    }

    $creds = urlencode($_name) . ':'. urlencode($_pass);

    if (strpos($url, 'http://') == 0) {
      $new_url = 'http://'. $creds . '@'. substr($url, 7);
    } 
    elseif (strpos($url, 'https://') == 0) {
      $new_url = 'https://'. $creds . '@'. substr($url, 8);
    } 
    else {
      drupal_set_message(t('Invalid URL: !url', array('!url' => $url)));
      return NULL;
    }

    return $new_url;
  }

  function getSoapClient($url = NULL) {
    if ($url == NULL) {
      $url=variable_get('fedora_soap_url', 'http://localhost:8080/fedora/services/access?wsdl');
    }
    global $user;
    if ($user->uid == 0) {
      //anonymous user.  We will need an entry in the fedora users.xml file
      //with the appropriate entry for a username of anonymous password of anonymous
      try {
        $client = new SoapClient($this->_fixURL($url, 'anonymous', 'anonymous'), array(
            'login' => 'anonymous',
            'password' => 'anonymous',
            'exceptions' => TRUE,
        ));
      } 
      catch (SoapFault $e) {
        drupal_set_message(t("!e", array('!e' => $e->getMessage())));
        return NULL;
      }
    }
    else {
      try {
        $client = new SoapClient($this->_fixURL($url, $user->name, $user->pass), array(
            'login' => $user->name,
            'password' => $user->pass,
            'exceptions' => TRUE,
        ));
      } 
      catch (SoapFault $e) {
        drupal_set_message(t("!e", array('!e' => $e->getMessage())));
        return NULL;
      }
    }
    return $client;
  }

  function getAdminSoapClient($url = NULL) {
    if ($url == NULL) {
      $url=variable_get('fedora_soap_url', 'http://localhost:8080/fedora/services/access?wsdl');
    }
    $username = array(
        'name' => variable_get('fedora_admin_user',
        'fedoraAdmin'
    ));
    $admin_user = user_load($username);
    if (!$admin_user) {
      drupal_set_message(t('Admin user not configured.'));
      watchdog(t("Fedora_Repository"), t("Admin user does not seem to be configured."), NULL, WATCHDOG_ERROR);
      return NULL;
    }
    try {
      $client = new SoapClient(_fixURL($this->$url, $admin_user->name, $admin_user->pass), array(
          'login' => $admin_user->name,
          'password' => $admin_user->pass
      ));
    } catch (exception $e) {
      drupal_set_message(t("!e", array('!e' => $e->getMessage())));
      return NULL;
    }
    return $client;
  }
}

