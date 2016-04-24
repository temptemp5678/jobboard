<?php
/**
 * callback from hook_menu()
 */
function _jsoninfo_jobboard_jobs_map() {
  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonJobboardJobsMap());
}
