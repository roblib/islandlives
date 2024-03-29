<?php
// $Id$

// @file fedora_utils.inc
// Base utilities used by the Islansora fedora module.

/* 
 * Static functions used by the Fedora PHP API.
 */


function do_curl($url, $return_to_variable = 1, $number_of_post_vars = 0, $post = NULL) {
  global $user;
  // Check if we are inside Drupal and there is a valid user.
  if ((!isset ($user)) || $user->uid == 0) {
    $fedora_user = 'anonymous';
    $fedora_pass = 'anonymous';
  }
  else {
    $fedora_user = $user->name;
    $fedora_pass = $user->pass;
  }

  if (function_exists("curl_init")) {
    $ch = curl_init();
    $user_agent = "Mozilla/4.0 pp(compatible; MSIE 5.01; Windows NT 5.0)";
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_setopt($ch, CURLOPT_FAILONERROR, TRUE); // Fail on errors
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1); // allow redirects
    curl_setopt($ch, CURLOPT_TIMEOUT, 90); // times out after 90s
    curl_setopt($ch, CURLOPT_USERAGENT, $user_agent);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, $return_to_variable); // return into a variable
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_USERPWD, "$fedora_user:$fedora_pass");
    //curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    if ($number_of_post_vars>0&&$post) {
      curl_setopt($ch, CURLOPT_POST, $number_of_post_vars);
      curl_setopt($ch, CURLOPT_POSTFIELDS, "$post");
    }
    return curl_exec($ch);
  }
  else {
    if (function_exists(drupal_set_message)) {
      drupal_set_message(t('No curl support.'), 'error');
    }
    return NULL;
  }
}

function fedora_available() {
  $ret = do_curl(variable_get('fedora_soap_url', ''), 1);
  // A bit of a hack but the SOAP parser will cause a fatal error if you give it the wrong URL.
  return (strpos($ret, 'wsdl:definitions') != FALSE);
}

/**
 * Returns a UTF-8-encoded transcripiton of the string given in $in_str.
 * @param string $in_str
 * @return string A UTF-8 encoded string.
 */
function fix_encoding($in_str) {
  $cur_encoding = mb_detect_encoding($in_str) ;
  if ($cur_encoding == "UTF-8" && mb_check_encoding($in_str, "UTF-8")) {
    return $in_str;
  }
  else {
    return utf8_encode($in_str);
  }
} 
