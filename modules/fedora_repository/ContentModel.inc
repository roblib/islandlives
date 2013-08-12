<?php
// $Id$

/*
 * Created on 18-Feb-08
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
class ContentModel {
  function ContentModel() {
  }
  public $pidNamespace;
  public $contentModelPid;
  public $contentModelDsid;
  public $contentModelName;

  public function getIdentifier() {
    return $this->contentModelPid . '/'. $this->contentModelDsid;
  }

  public static function getPidFromIdentifier($identifier) {
    return substr($identifier, 0, strpos($identifier, "/"));
  }

  public static function getDSIDFromIdentifier($identifier) {
    $temp = strstr($identifier, "/");
    return substr($temp, 1);
  }
}

