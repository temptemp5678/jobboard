<?php
/**
 * callback from hook_menu()
 */
function _jsoninfo_page_landing($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $TermProgramQuery = new TermProgramQuery();
  $program_tids = $TermProgramQuery->programTidsByUnit($tid);

  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonPageLanding($program_tids, $start_time, $end_time));
}
