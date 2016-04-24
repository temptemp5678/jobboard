<?php
/**
 * @file for edit term
 */

/**
 * callback from hook_menu()
 */
function _manageinfo_term_table($vid = NULL) {
  /**
   * js
   */
  drupal_add_js(drupal_get_path('module', 'manageinfo') .'/ng_js/manageinfo_term_table.ng.js');

  /**
   * output
   */
  $output = '';
  $output .= '<div data-ng-app="termTable" class="row margin-0">';
    $output .= '<div data-ng-controller="TermTableCtrl" class="row margin-0">';

      /**
       * header filter
       */
      $output .= '<div class="clear-both margin-top-12">';
        $output .= '<span class="">';
          $output .= t('Search') . ': <input data-ng-model="inputFilter.$">';
        $output .= '</span>';
      $output .= '</div>';

      /**
       * table
       */
      $output .= '<div class="margin-top-12">';
      switch ($vid) {
        case 3:
          $output .= _manageinfo_term_table_content_brand();
          break;
        case 7:
          $output .= _manageinfo_term_table_content_company();
          break;

        default:
          break;
      }
      $output .= '</div>';

    $output .= '</div>';
  $output .= '</div>';

  return $output;
}

/**
 * Brand is 3
 */
function _manageinfo_term_table_content_brand() {
  $output = NULL;

  $output .= '<table class="table table-hover">';
    $output .= '<thead>';
      $output .= '<tr>';
        $output .= '<th>' . t('Title') . '</th>';
        $output .= '<th></th>';
        $output .= '<th></th>';
        $output .= '<th></th>';
      $output .= '</tr>';
    $output .= '</thead>';
    $output .= '<tbody data-ng-repeat="row in tableData | filter:inputFilter">';
      $output .= '<tr>';
        $output .= '<td>{{ row.companyTitle }}</td>';
        $output .= '<td>{{ row.address }}</td>';
        $output .= '<td>{{ row.address }}</td>';
      $output .= '</tr>';
    $output .= '</tbody>';
  $output .= '</table>';

  return $output;
}

/**
 * Company is 7
 */
function _manageinfo_term_table_content_company() {
  $output = NULL;

  $output .= '<table class="table table-hover">';
    $output .= '<thead>';
      $output .= '<tr>';
        $output .= '<th>' . t('Title') . '</th>';
        $output .= '<th></th>';
        $output .= '<th></th>';
      $output .= '</tr>';
    $output .= '</thead>';
    $output .= '<tbody data-ng-repeat="row in tableData | filter:inputFilter">';
      $output .= '<tr>';
        $output .= '<td>{{ row.companyTitle }}</td>';
        $output .= '<td>{{ row.address }}</td>';
        $output .= '<td><a href="{{ row.termEditLink }}"><i class="fa fa-pencil-square-o fa-lg color-767676"></i></a></td>';
      $output .= '</tr>';
    $output .= '</tbody>';
  $output .= '</table>';

  return $output;
}
