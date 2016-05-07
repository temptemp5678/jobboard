<?php

/**
 * Implements page_callback(), callback from hook_menu()
 */
function _jobboard_jobs_dashboard() {
  /**
   * js
   */
  drupal_add_library('libraryinfo', 'libraryinfo_chart_js');
  drupal_add_js(drupal_get_path('module', 'jobboard') .'/js/jobboard_jobs_dashboard.js');

  $output = '';
  $output .= '<div class="panel panel-default">';
    $output .= '<div class="panel-heading">' . t('Top Product') . '</div>';
    $output .= '<div class="panel-body bg-ffffff padding-0">';
      $output .= '<canvas class="padding-6-12 float-left" id="reportpage-summary-quote-bar-chart" width="390" height="150"></canvas>';
    $output .=' </div>';
  $output .=' </div>';

  $output .= '<div class="panel panel-default">';
    $output .= '<div class="panel-heading">Job Trend</div>';
    $output .= '<div class="panel-body bg-ffffff padding-0">';
      $output .= '<canvas class="padding-6-12" id="reportpage-summary-quote-line-chart" width="390" height="150"></canvas>';
    $output .=' </div>';
  $output .=' </div>';

  return $output;
}
