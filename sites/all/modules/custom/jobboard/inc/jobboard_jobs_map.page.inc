<?php

/**
 * Implements page_callback(), callback from hook_menu()
 */
function _jobboard_jobs_map() {
  drupal_add_library('libraryinfo', 'libraryinfo_leaflet');

  drupal_add_js(drupal_get_path('module', 'jobboard') . '/js/jobboard_jobs_map.js');
  drupal_add_js(drupal_get_path('module', 'jobboard') .'/ng_js/jobboard_jobs_list.ng.js');

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

    $output .= '<div class="panel-body">';
      $output .= '<div data-ng-app="jobboardJobsList" class="row margin-0">';
        $output .= '<div data-ng-controller="jobboardJobsListController" class="row margin-0">';

          /**
           * Job table
           */
          $output .= '<div class="margin-top-12">';
            $output .= '<table class="table table-hover">';
              $output .= '<thead>';
                $output .= '<tr class="bg-4ab2de color-fff hover-bg-0088cc">';
                  $output .= '<th></th>';
                  $output .= '<th class="font-weight-100">' . t('Date') . '</th>';
                  $output .= '<th class="font-weight-100">' . t('Job Title') . '</th>';
                  $output .= '<th class="font-weight-100">' . t('Town') . '</th>';
                  $output .= '<th class="font-weight-100">' . t('Industry') . '</th>';
                  $output .= '<th class="font-weight-100">' . t('Career Level') . '</th>';
                  $output .= '<th class="font-weight-100">' . t('Type') . '</th>';
                  // $output .= '<th></th>';
                  $output .= '<th></th>';
                $output .= '</tr>';
              $output .= '</thead>';

              $output .= '<tbody data-ng-repeat="row in tableRow | filter:inputFilter | orderBy:sortField:reverse">';
                $output .= '<tr class="">';
                  $output .= '<td>{{ $index + 1 }}</td>';
                  $output .= '<td>{{ row.datePostedTimeValue }}</td>';
                  $output .= '<td class="h5">{{ row.nodeTitle }}</td>';
                  $output .= '<td>{{ row.town }}</td>';
                  $output .= '<td>{{ row.industry }}</td>';
                  $output .= '<td>{{ row.careerLevel }}</td>';
                  $output .= '<td>{{ row.jobType }}</td>';
                  // $output .= '<td><a href="{{ row.nodeViewLink }}"><i class="fa fa-external-link fa-lg"></i></a></td>';
                  // $output .= '<td data-ng-click="addOrderList(row)" class="cursor-pointer"><i class="fa fa-plus fa-lg"></i></td>';
                  $output .= '<td class="cursor-pointer"><i class="fa fa-plus fa-lg"></i></td>';
                $output .= '</tr>';
              $output .= '</tbody>';

            $output .= '</table>';
          $output .= '</div>';

        $output .= '</div>';
      $output .= '</div>';
    $output .='</div>';
  $output .='</div>';

  return $output;
}
