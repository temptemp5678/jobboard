<?php
/**
 * callback from hook_menu()
 */
function _jsoninfo_table_program($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $program_tids = array();

  $TermInfo = new TermInfo($tid);
  switch ($TermInfo->termVid()) {
    // Program
    case 2:
      $program_tids = array($tid);
      break;
    // Program Unit and Program Nation
    case 11:
    case 22:
      $TermProgramQuery = new TermProgramQuery();
      $program_tids = $TermProgramQuery->programTidsByUnit($tid);
      break;

    default:
      break;
  }

  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonTableProgram($program_tids, $start_time, $end_time));
}
