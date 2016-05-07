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
    $output .= '<div class="panel-heading bg-4ab2de color-fff">';
      $output .= 'Job Map';
    $output .='</div>';
    $output .= '<div class="panel-body bg-ffffff padding-0">';
      $output .= '<div class="leaflet-map-wrapper">';
        $output .= '<div id="mapid">';
        $output .='</div>';
      $output .='</div>';
    $output .='</div>';
    $output .= '<div class="panel-heading">';
      $output .= 'Legend';
    $output .='</div>';
  $output .='</div>';

  return $output;
}
