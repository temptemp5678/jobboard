<?php

/**
 * callback from hook_menu(), Therapeutic Area
 */
function _jsoninfo_program_snapshot($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonProgramSnapshot(array($tid), $start_time, $end_time));
}

/**
 * callback from hook_menu(), Therapeutic Area
 */
function _jsoninfo_therapeutic_area_snapshot($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $TermProgramQuery = new TermProgramQuery();
  $program_tids = $TermProgramQuery->programTidsByArea($tid);

  // only for Oncology Business Unit, it use Therapeutic Area layout
  if ($tid == 2090) {
    $program_tids = $TermProgramQuery->programTidsByUnit($tid);
  }

  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonTherapeuticAreaSnapshot($program_tids, $start_time, $end_time));
}
/**
 * callback from hook_menu(), Therapeutic Area and Type
 */
function _jsoninfo_program_area_accredited($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $TermProgramQuery = new TermProgramQuery();
  $program_tids = $TermProgramQuery->programTidsByArea($tid);
  $accredited_program_tids = $TermProgramQuery->programTidsByField($program_tids, $field_method = 'programTypeTids', $filter_tids = array(74));

  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonProgramAreaType($accredited_program_tids, $start_time, $end_time));
}
/**
 * callback from hook_menu(), Therapeutic Area and Type
 */
function _jsoninfo_program_area_ola($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $TermProgramQuery = new TermProgramQuery();
  $program_tids = $TermProgramQuery->programTidsByArea($tid);
  $ola_program_tids = $TermProgramQuery->programTidsByField($program_tids, $field_method = 'programTypeTids', $filter_tids = array(75));

  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonProgramAreaType($ola_program_tids, $start_time, $end_time));
}

/**
 * callback from hook_menu()
 */
function _jsoninfo_program_data() {
  $JsonData = new JsonData();
  drupal_json_output($JsonData->programData());
}

/**
 * callback from hook_menu()
 */
function _jsoninfo_program_list() {
  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonProgramlist());
}

/**
 * callback from hook_menu()
 * Business Unit
 */
function _jsoninfo_business_unit_snapshot($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $TermProgramQuery = new TermProgramQuery();
  $program_tids = $TermProgramQuery->programTidsByUnit($tid);

  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonBusinessUnitSnapshot($program_tids, $start_time, $end_time));
}

/**
 * callback from hook_menu()
 * Business Unit
 */
function _jsoninfo_division_snapshot($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $TermProgramQuery = new TermProgramQuery();
  $program_tids = $TermProgramQuery->programTidsByDivision($tid);

  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonDivisonSnapshot($program_tids, $start_time, $end_time));
}

/**
 * callback from hook_menu()
 */
function _jsoninfo_program_unit_program($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $TermProgramQuery = new TermProgramQuery();
  $program_tids = $TermProgramQuery->programTidsByUnit($tid);

  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonProgramUnitProgram($program_tids, $start_time, $end_time));
}

/**
 *
 */
function _jsoninfo_program_unit_json() {
  $json = '';
  return $json;
}







