<?php

/**
 * Implements page_callback(), callback from hook_menu()
 */
function _jobboard_jobs_list() {
  /**
   * js
   */
  drupal_add_js(drupal_get_path('module', 'jobboard') .'/ng_js/jobboard_jobs_list.ng.js');
  drupal_add_css(drupal_get_path('module', 'jobboard') .'/css/jobboard_jobs_list.css');

  /**
   * output
   */
  $output = '';
  $output .= '<div data-ng-app="jobboardJobsList" class="row margin-0">';
    $output .= '<div data-ng-controller="jobboardJobsListController" class="row margin-0">';

      /**
       * selected order
       */
      $output .= '<div data-ng-if="selectedOrders.length" class="margin-bottom-12 animate-if">';

        // selected table
        $output .= '<div class="">';
          $output .= '<table class="table table-hover">';
            $output .= '<tr data-ng-repeat="item in selectedOrders track by $index">';
              $output .= '<td>{{ $index + 1 }}</td>';
              $output .= '<td>{{ item.dateValue }}</td>';
              $output .= '<td>{{ item.nodeTitle }}</td>';
              $output .= '<td>{{ item.productsTotaltown }}</td>';
              $output .= '<td>{{ item.nodeUidName }}</td>';
              $output .= '<td data-ng-click="removeOrderList($index)" class="cursor-pointer"><i class="fa fa-minus fa-lg"></i></a></td>';
            $output .= '</tr>';
          $output .= '</table>';
        $output .= '</div>';

        // button
        $output .= '<div class="float-left margin-bottom-12">';
          $output .= '<div class="margin-left-12">';
            $output .= '<span class="btn btn-primary" data-ng-click="toQuoteBtn()">Print</span>';
          $output .= '</div>';
        $output .= '</div>';

      $output .= '</div>';

      /**
       * Order header
       */
      $output .= '<div class="clear-both margin-top-12">';
        // filter
        $output .= '<span class="">';
          $output .= t('Search') . ': <input data-ng-model="inputFilter.$">';
        $output .= '</span>';

        // daterangepicker_controller
        $output .= '<span class="">';
          // $output .= DashDateRangePicker::dateRangePickerHeader();
        $output .= '</span>';
      $output .= '</div>';

      /**
       * Job table
       */
      $output .= '<div class="margin-top-12">';
        $output .= '<table class="table table-hover">';
          $output .= '<thead>';
            $output .= '<tr>';
              $output .= '<th class="font-weight-100"><a href="" data-ng-click="sortField = \'datePostedTimeValue\' ; reverse= !reverse ">' . t('Date') . '</a></th>';
              $output .= '<th class="font-weight-100"><a href="" data-ng-click="sortField = \'jobTitle\' ; reverse= !reverse ">' . t('Job Title') . '</a></th>';
              $output .= '<th class="font-weight-100"><a href="" data-ng-click="sortField = \'town\' ; reverse= !reverse ">' . t('Town') . '</a></th>';
              $output .= '<th class="font-weight-100"><a href="" data-ng-click="sortField = \'industry\' ; reverse= !reverse ">' . t('Industry') . '</a></th>';
              $output .= '<th class="font-weight-100"><a href="" data-ng-click="sortField = \'careerLevel\' ; reverse= !reverse ">' . t('Career Level') . '</a></th>';
              $output .= '<th class="font-weight-100"><a href="" data-ng-click="sortField = \'careerLevel\' ; reverse= !reverse ">' . t('Type') . '</a></th>';
              // $output .= '<th></th>';
              $output .= '<th></th>';
            $output .= '</tr>';
          $output .= '</thead>';

          $output .= '<tbody data-ng-repeat="row in tableRow | filter:inputFilter | orderBy:sortField:reverse">';
            $output .= '<tr class="bg-4ab2de color-fff hover-bg-0088cc">';
              $output .= '<td>{{ row.datePostedTimeValue }}</td>';
              $output .= '<td class="h5 color-fff">{{ row.nodeTitle }}</td>';
              $output .= '<td>{{ row.town }}</td>';
              $output .= '<td>{{ row.industry }}</td>';
              $output .= '<td>{{ row.careerLevel }}</td>';
              $output .= '<td>{{ row.jobType }}</td>';
              $output .= '<td class="cursor-pointer"><i class="fa fa-plus fa-lg"></i></td>';
            $output .= '</tr>';

            $output .= '<tr>';
              $output .= '<td colspan="100%" data-ng-show="!hoverInStatus" data-ng-bind-html="renderHtml(row.description)" class="hidden-text-description padding-left-24" style="border:none"></td>';
            $output .= '</tr>';
            $output .= '<tr class="hover-bg-ffffff">';
              $output .= '<td colspan="100%" data-ng-show="!hoverInStatus" data-ng-click="showMoreDescription($index)" class="cursor-pointer text-center color-0088cc" style="border:none"><i class="fa fa-angle-double-down fa-lg font-bold"></i></td>';
            $output .= '</tr>';
            $output .= '<tr>';
              $output .= '<td colspan="100%" data-ng-show="hoverInStatus" data-ng-bind-html="renderHtml(row.description)" class="margin-left-12" style="border:none"></td>';
            $output .= '</tr>';
            $output .= '<tr class="hover-bg-ffffff">';
              $output .= '<td colspan="100%" data-ng-show="hoverInStatus" data-ng-click="hideMoreDescription($index)" class="cursor-pointer text-center color-0088cc" style="border:none"><i class="fa fa-angle-double-up fa-lg font-bold"></i></td>';
            $output .= '</tr>';
            $output .= '<tr class="border-none height-24 hover-bg-ffffff border-bottom-ccc">';
              $output .= '<td colspan="100%" style="border:none">&nbsp;</td>';
            $output .= '</tr>';
          $output .= '</tbody>';

        $output .= '</table>';
      $output .= '</div>';

    $output .= '</div>';
  $output .= '</div>';

  return $output;
}
