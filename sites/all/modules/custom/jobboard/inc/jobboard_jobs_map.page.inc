<?php

/**
 * Implements page_callback(), callback from hook_menu()
 */
function _jobboard_jobs_map() {
  drupal_add_library('libraryinfo', 'libraryinfo_leaflet');

  drupal_add_js(drupal_get_path('module', 'jobboard') . '/js/jobboard_jobs_map.js');

  drupal_add_css(drupal_get_path('module', 'jobboard') . '/css/jobboard_jobs_map.css');

  $output = '';
  $output .= '<div class="panel panel-default">';
    $output .= '<div class="panel-heading">
    1. 北京欢乐谷 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    2. 北京海碗居老北京炸酱面(松榆里店) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    3. 北京宏源南门涮肉 （天坛南门） &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    4. 和平门全聚德 <br />
    5. 江边城外烤全鱼(交大店) （北京动物园北门外）&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    6. 姚记炒肝店（鼓楼包子, 后海边上） &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    7. 北京簋街</div>';
    $output .= '<div class="panel-body bg-ffffff padding-0">';
      $output .= '<div class="leaflet-map-wrapper">';
        $output .= '<div id="mapid">';
        $output .=' </div>';
      $output .=' </div>';
    $output .=' </div>';
  $output .=' </div>';

  return $output;
}
