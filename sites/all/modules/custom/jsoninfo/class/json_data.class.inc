<?php

/**
 * class
   $JsonDataUnit = new JsonDataUnit();
   $JsonDataUnit->termCityJsonFile();
   $JsonDataUnit->userSpeakerDetails();
 *
 */
class JsonDataUnit{

  /** - - - - - Node Json- - - - - - - - - - - - - - - */


  /** - - - - - page - - - - - - - - - - - - - - - - - - - - - - */
  /**
   * @return
   */
  public function jobboardJobsMap($nids = array()) {
    $nodes = node_load_multiple($nids);

    $output = array();
    foreach ($nodes as $key => $node) {
      $NodeInfo = new NodeInfo($node->nid);

      if ($MeetingInfo->latitude() && $MeetingInfo->longitude()) {
        $output[$node->nid] = array(
          "latitude" => $MeetingInfo->latitude(),
          "longitude" => $MeetingInfo->longitude(),
          "pageUrl" => '#1',
          "unitName" => $TermProgramInfo->programUnitName(),
          "programName" => '<a href="' . base_path() . 'dashpage/program/snapshot/' . $MeetingInfo->programTid() . '" class="color-fff google-map-program-name">' . truncate_utf8($MeetingInfo->programName(), 30, FALSE, TRUE) . '</a>',
          "meetingDate" => $MeetingInfo->dateValue('M d, Y'),
          "meetingLocation" => $MeetingInfo->cityName() . ', ' . $MeetingInfo->provinceNameAbbr(),
          "speakerName" => $MeetingInfo->speakerName(),
          "speakerNameUrl" => '#',
          "repName" => $MeetingInfo->repName(),
          "evaluationCount" => $MeetingInfo->evaluationsCount(),
          "venuName"  => $MeetingInfo->venueName(),
          "venuType"  => $MeetingInfo->meetingTypeName(),
          "attendiesCount"  => $MeetingInfo->signatures(),
          "blockId"  => 'google-map-' . $node->nid,
          "619" => l('Event Link', '#'),
          "eventLink" => l(t('View'), 'dashpage/meeting/details/' . $node->nid),
        );
      }
    }
    return $output;
  }

  /** - - - - - Term Standard Json- - - - - - - - - - - - - - - - - - - - - - */
  /**
   * @return Term Array data by specify VID
   */
  public function termStandardJson($vid = NULL, $class_name = NULL, $basic = NULL) {
    if ($class_name) {
      $all_term_array = taxonomy_get_tree($vid);

      if (is_array($all_term_array)) {
        foreach ($all_term_array as $term_key => $term_row) {
          // $TermProgramInfo = new TermProgramInfo($term_row->tid);
          $TermClassInfo = new $class_name($term_row->tid);

          if ($basic) {
            $output[] = $TermClassInfo->basicMethodsValue();
          }
          else {
            $output[] = $TermClassInfo->allMethodsValue();
          }
        }
      }
    }

    return $output;
  }

  /** - - - - - Term True - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /** - - - - - Query Table Json- - - - - - - - - - - - - - - - - - - - -   */


  /** - - - - - User - - - - - - -- - - - - - - - - - - - - - - - - - - -   */

}

/**
 * class
   $JsonData = new JsonData();
   $JsonData->sourceArrayData();
 *
 */
class JsonData extends JsonDataUnit {

  /** - - - - - - page - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @return Term Array data
   */
  public function jsonJobboardJobsMap() {
    $NodeQuery = new NodeQuery();
    $nids = $NodeQuery->nodesByBundle('jobs');

    $output = $this->jobboardJobsMap($nids);
    return $output;
  }

  /** - - - - - - Program - - - - - - - - - - - - - - - - - - - - - - - -  */

  /** - - - - - - user - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @return Array data
   */
  public function jsonUserList() {
    $output = NULL;

    $output['userList'] = $this->allUserList();
    $output['termBusinessUnit'] = $this->termBusinessUnit();
    $output['termCity'] = $this->termCityArrayFile();
    $output['termProvince'] = $this->termProvince();
    $output['termTherapeuticArea'] = $this->termTherapeuticArea();

    return $output;
  }
  /**
   * @return Array data
   */
  public function jsonUserProfile($uid = NULL) {
    $output = $this->userProfileFields($uid);
    return $output;
  }
  /**
   * @return Array data
   */
  public function jsonUserSpeakerDetails($uid = NULL) {
    $output = $this->userSpeakerDetails($uid);
    return $output;
  }

}
