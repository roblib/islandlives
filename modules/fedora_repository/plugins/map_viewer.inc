<?php
// $Id$

class ShowMapStreamsInFieldSets {
  private $pid =NULL;

  function ShowMapStreamsInFieldSets($pid) {
    $this->pid = $pid;
  }

  function showJPG() {
    module_load_include('inc', 'fedora_repository', 'plugins/tagging_form');
    module_load_include('inc', 'fedora_repository', 'plugins/ShowStreamsInFieldSets');
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');

    global $base_url;

    $tabset = array();

    $tabset['my_tabset'] = array(
      '#type' => 'tabset',
    );

    global $user;
    $qs = '';
    if ($user->uid != 0) {
      $qs = '?uid=' . base64_encode($user->name . ':' . $user->pass);
    }

    $viewer_url = variable_get('fedora_base_url', '') . '/get/'. $this->pid . '/ilives:viewerSdef/getViewer'. $qs;
    $html = '<iframe src="'. $viewer_url . '" frameborder="0" style="width: 100%; height: 800px;">Errors: unable to load viewer</iframe>';

    drupal_add_css(path_to_theme() . '/header-viewer.css', 'theme');
    drupal_add_css(drupal_get_path('module', 'fedora_repository') . '/js/iiv/css/jquery-ui/smoothness/jquery-ui-1.7.2.custom.css');
    drupal_add_css(drupal_get_path('module', 'fedora_repository') . '/js/iiv/css/iiv.css');


    $tabset['my_tabset']['second_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Full-size'),
      '#content' => $html//'<div class="iiv"></div>'
    );

    $item = new Fedora_Item($this->pid);
    $tabset['my_tabset']['third_tab'] = array(
      '#type' => 'tabpage',
      '#title' => t('Description'),
      '#content' => $item->get_dissemination('islandora:mods2htmlSdef', 'mods2html'),
    );
    // Render the tabset.
    return tabs_render($tabset);

    return theme('fieldset', $collection_fieldset);
  }
}
