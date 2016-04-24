<?php

/**
 * class
   OtherCommonJson::encodeJson();
   OtherCommonJson::saveJsonToFile();
   OtherCommonJson::saveDataToJson();
 *
 */
class OtherCommonJson {
  /**
   * @return json array
   */
  public static function encodeJson($raw_data = NULL) {
    $json_data = NULL;

    if (is_array($raw_data)) {
      $json_data = drupal_json_encode($raw_data);
    }

    return $json_data;
  }

  /**
   * @param  array.
   *
   * @return Save JSON to Json file
   */
  public static function saveJsonToFile($json_data = NULL, $file_name = NULL) {
    if (!$file_name) {
      return;
    }

    if ($json_data) {
      $file = file_save_data($json_data, 'public://json/' . $file_name, FILE_EXISTS_REPLACE);
    }
    else {
      // put empty content
      $file = file_save_data('', 'public://json/' . $file_name, FILE_EXISTS_REPLACE);
    }
  }

  /**
   * @param  array.
   *
   * @return Convert Data to Json and Save Json file
   */
  public static function saveDataToJson($raw_data = NULL, $file_name = NULL) {
    if (!$file_name) {
      return;
    }

    if (is_array($raw_data)) {
      $json_data = OtherCommonJson::encodeJson($raw_data);
      if ($json_data) {
        $file = file_save_data($json_data, 'public://json/' . $file_name, FILE_EXISTS_REPLACE);
      }
      else {
        // put empty content
        $file = file_save_data('', 'public://json/' . $file_name, FILE_EXISTS_REPLACE);
      }
    }
    else {
      // put empty content
      $file = file_save_data('', 'public://json/' . $file_name, FILE_EXISTS_REPLACE);
    }
  }
}

/**
 * class
   $SourceJson = new SourceJson();
   $SourceJson->sourceArrayData();
 *
 */
class SourceJson {
  private $vid;

  /** - - - - - Basic Object - - - - - - - - - -  - - - - -   */
  /**
   * @return data Array
   */
  public function sourceArrayData() {
    $output = NULL;

    $output['terms'] = $this->termArrayData();
    $output['users'] = $this->userArrayData();
    $output['nodes'] = $this->nodeOptionData();

    return $output;
  }

  /** - - - - - Node - - - - - - - - - - - - - - - - - - - - - - - - -   */
  /**
   * generate Node faq Data
   */
  public function nodeinfo_faq_data() {
    /**
     * data
     */
    $NodeQuery = new NodeQuery();
    $all_faq_nid = $NodeQuery->allSupportNid();
    $nodes = node_load_multiple($all_faq_nid);

    /**
     * data row array
     */
    $data_row = array();

    $data_row['nodes'] = array();
    if (is_array($nodes)) {
      foreach($nodes as $node) {
        $supportInfo = new supportInfo($node);

        // table row
        $data_row['nodes']['faq'][$node->nid] = array(
          'author' => user_load($node->uid)->name,
          'cretedTimeStamp' => $node->created,
          'faqAnswer' => $supportInfo->faqAnswer(),
          'nid' => $node->nid,
          'title' => $supportInfo->nodeTitle(),
          'type' => $node->type,
        );
      }
    }

    return $data_row;
  }

  /**
   * generate Meeting Data
   */
  public function nodeMeetingList() {
    /**
     * data
     */
    $NodeQuery = new NodeQuery();
    $all_meeting_nid = $NodeQuery->meetingNids();

    $nodes = node_load_multiple($all_meeting_nid);

    /**
     * data row array
     */
    $data_row = array();

    $data_row['nodes'] = array();
    if (is_array($nodes)) {
      foreach($nodes as $node) {
        $MeetingInfo = new MeetingInfo($node);

        // table row
        $data_row['nodes']['meeting'][] = array(
          'author' => user_load($node->uid)->name,
          'city' => $MeetingInfo->cityName(),
          'dateValue' => $MeetingInfo->dateValue('Y-m-d'),
          // 'dateLong' => $MeetingInfo->dateValue('l, F d, Y'),
          // 'datetime' => $MeetingInfo->dateValue('Y-m-d H:i:s'),
          // 'dateUnixStamp' => $MeetingInfo->dateUnixStamp(),
          'linkCreateEvaluation' => base_path() . 'manage/evaluation/create-form-page/' . $MeetingInfo->nodeNid()  . '?destination=manage/meeting/list',
          'linkEvaluationList' => base_path() . 'manage/evaluation/list/' . $MeetingInfo->nodeNid(),
          'linkMeetingEdit' => base_path() . 'manage/meeting/edit/' . $MeetingInfo->nodeNid() . '?destination=manage/meeting/list',
          'nid' => $MeetingInfo->nodeNid(),
          'nodeUid' => $MeetingInfo->nodeUid(),
          'evaluationsCount' => $MeetingInfo->evaluationsCount(),
          'programTid' => $MeetingInfo->programTid(),
          'programName' => $MeetingInfo->programName(),
          'speakerName' => $MeetingInfo->speakerName(),
        );
      }
    }

    return $data_row;
  }

  /**
   * @parameter array, $meeting_nids
   * @return, generate Evaluation Data by specify Meeting
   */
  public function specifyEvaluationData($meeting_nids = NULL) {
    /**
     * data
     */
    $NodeQuery = new NodeQuery();
    $all_evaluation_nid = $NodeQuery->evaluationNidsByMeeting($meeting_nids);

    $nodes = node_load_multiple($all_evaluation_nid);
    /**
     * data row array
     */
    $data_row = array();

    $data_row['nodes'] = array();
    if (is_array($nodes)) {
      foreach($nodes as $node) {
        $EvaluationInfo = new EvaluationInfo($node);

        // table row
        $data_row['nodes']['evaluation'][] = array(
          'author' => user_load($node->uid)->name,
          'evaluationAnswerSet' => $EvaluationInfo->evaluationAnswerSet(),
          'meetingNid' => $EvaluationInfo->meetingNid(),
          'nid' => $EvaluationInfo->nodeNid(),
          'nodeUid' => $EvaluationInfo->nodeUid(),
        );
      }
    }

    return $data_row;
  }

  /** - - - - - Term - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /**
   * @return Term Array data
   * category by bundle
   */
  public function termArrayData() {
    $output = NULL;

    $all_voc = taxonomy_vocabulary_get_names();
    foreach (taxonomy_vocabulary_get_names() as $voc_key => $voc_row) {
      // skip the current loop iteration
      // city tid is 9
      if ($voc_row->vid == 9) {
        continue;
      }

      $all_term_array = taxonomy_get_tree($voc_row->vid);
      $all_term = array();

      if (is_array($all_term_array)) {
        foreach ($all_term_array as $term_key => $term_row) {
          $all_term[$term_row->tid] = array(
            'tid'  => $term_row->tid,
            'name' => $term_row->name,
          );

          // only for city term, add one property
          if ($voc_row->vid == 9) {
            $TermInfo = new TermInfo($term_row->tid);

            $all_term[$term_row->tid] += array(
              'provinceTid' => $TermInfo->provinceTidByCity(),
            );
          }
        }

        $output[$voc_row->machine_name] = array(
          'vid'  => $voc_row->vid,
          'term' => $all_term,
        );
      }
    }

    return $output;
  }

  /**
   * @return Term Program Array data
   * only for meeting request with using JS datatable library to read
   */
  public function termProgramDatatable() {
    $output = NULL;

    $all_term_array = taxonomy_get_tree(2);

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermProgramInfo = new TermProgramInfo($term_row->tid);

        $output[] = array(
          'programTid'  => $term_row->tid,
          'programName' => $term_row->name,
          'programType' => $TermProgramInfo->programTypeName(),
          'programCategory' => 'IPF',
          'selectText' => t('SELECT'),
          'programObjective' => $term_row->description,
        );
      }
    }

    return $output;
  }

  /**
   * @return Term Question Array data
   */
  public function allQuestionField() {
    $output = NULL;

    $all_term_array = taxonomy_get_tree(3);
    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermQuestionFieldInfo = new TermQuestionFieldInfo($term_row->tid);

        $output['questionField'][] = $TermQuestionFieldInfo->allMethodsValue();
      }
    }

    return $output;
  }

  /**
   * @return Term Question Array data
   */
  public function questionLibraryList() {
    $output = NULL;

    // loop all Evaluation Form question
    $all_question_set = array();
    $evaluation_form_term_array = taxonomy_get_tree(6);
    foreach ($evaluation_form_term_array as $value) {
      $TermEvaluationFormInfo = new TermEvaluationFormInfo($value->tid);
      if (is_array($TermEvaluationFormInfo->questionSetArray())) {
        foreach ($TermEvaluationFormInfo->questionSetArray() as $key3 => $value3) {
          $all_question_set[] = $value3['quesset_question_tid'];
        }
      }
    }
    // get each question count frequency
    $all_question_frequency = array_count_values($all_question_set);

    $all_term_array = taxonomy_get_tree(4);
    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermQuestionLibraryInfo = new TermQuestionLibraryInfo($term_row->tid);

        $question_count = 0;
        if (isset($all_question_frequency[$term_row->tid])) {
          $question_count = $all_question_frequency[$term_row->tid];
        }
        $extra_array = array(
          'linkEdit' => $TermQuestionLibraryInfo->termEditLink() . '?destination=manage/question-lib/list',
          'referenceCount' => $question_count,
        );

        $output['questionLib'][] = $extra_array + ($TermQuestionLibraryInfo->allMethodsValue());
      }
    }

    return $output;
  }

  /**
   * @return Term Question Array data
   */
  public function allEvaluationForm() {
    $output = NULL;

    $all_term_array = taxonomy_get_tree(6);

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermEvaluationFormInfo = new TermEvaluationFormInfo($term_row->tid);

        $output[] = array(
          'linkEdit' => base_path() . 'manage/evaluation-form/edit/' . $term_row->tid . '?destination=manage/evaluation-form/list',
          'evaluationFormTid'  => $term_row->tid,
          'evaluationFormName' => $term_row->name,
          'evaluationFormDescrption' => $TermEvaluationFormInfo->termDescription(),
          'numberOfQuestion' => $TermEvaluationFormInfo->numberOfQuestion(),
        );
      }
    }

    return $output;
  }
  /**
   * @return Term Question Array data
   */
  public function evaluationFormQuestionList() {
    $output = NULL;

    $all_term_array = taxonomy_get_tree(6);

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermEvaluationFormInfo = new TermEvaluationFormInfo($term_row->tid);

        $output[] = array(
          'evaluationFormTid'  => $term_row->tid,
          'evaluationFormName' => $term_row->name,
          'questionSetList' => $TermEvaluationFormInfo->questionSetList(),
        );
      }
    }

    return $output;
  }

  /**
   * @return Term Program Array data
   */
  public function termProgramList() {
    $output = NULL;

    $all_term_array = taxonomy_get_tree(2);

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermProgramInfo = new TermProgramInfo($term_row->tid);

        $output[] = array(
          'programTid'  => $term_row->tid,
          'programName' => $term_row->name,
          'programDescrption' => $TermProgramInfo->termDescription(),
          'programTypeNames' => BasicClass::implode(" ", $TermProgramInfo->programTypeNames()),
          'programClassNames' => BasicClass::implode(" ", $TermProgramInfo->programClassNames()),
          'programUnitName' => $TermProgramInfo->programUnitName(),
          'therapAreaNames' => BasicClass::implode(" ", $TermProgramInfo->therapAreaNames()),
          'evaluationFormName' => $TermProgramInfo->evaluationFormName(),
          'evaluationFormNumber' => count($TermProgramInfo->questionSetArray()),
          'linkCreateMeeting' => base_path() . 'manage/meeting/create/' . $term_row->tid  . '?destination=manage/program/list',
          'linkEdit' => $TermProgramInfo->termEditLink() . '?destination=manage/program/list',
        );
      }
    }

    return $output;
  }

  /**
   * @return Term Program Unit Array data
   */
  public function allProgramData() {
    $output = NULL;

    /** - - - - -Unit  - - - - - - - - - - - - - - - - - - - - - - - - -   */
    $all_term_array = taxonomy_get_tree(11);

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermProgramUnitInfo = new TermProgramUnitInfo($term_row->tid);

        $extra_array = array(
          'linkEdit' => base_path() . 'manage/program-unit/edit/' . $term_row->tid . '?destination=manage/program/list',
        );

        $output['unit'][] = ($extra_array + ($TermProgramUnitInfo->allMethodsValue()));
      }
    }

    /** - - - - - Area  - - - - - - - - - - - - - - - - - - - - - - - - -   */
    $all_term_array = taxonomy_get_tree(15);

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermProgramTypeInfo = new TermProgramTypeInfo($term_row->tid);

        $output['area'][] = $TermProgramTypeInfo->allMethodsValue();
      }
    }

    /** - - - - - Brand  - - - - - - - - - - - - - - - - - - - - - - - - -   */
    $all_term_array = taxonomy_get_tree(19);

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermProgramTypeInfo = new TermProgramTypeInfo($term_row->tid);

        $output['brand'][] = $TermProgramTypeInfo->allMethodsValue();
      }
    }

    /** - - - - - Type  - - - - - - - - - - - - - - - - - - - - - - - - -   */
    $all_term_array = taxonomy_get_tree(12);

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermProgramTypeInfo = new TermProgramTypeInfo($term_row->tid);

        $output['type'][] = $TermProgramTypeInfo->allMethodsValue();
      }
    }

    /** - - - - - Class  - - - - - - - - - - - - - - - - - - - - - - - - -   */
    $all_term_array = taxonomy_get_tree(13);
    $output['class'] = array();

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermProgramClassInfo = new TermProgramClassInfo($term_row->tid);

        $output['class'][] = $TermProgramClassInfo->allMethodsValue();
      }
    }

    return $output;
  }

  /** - - - - - User  - - - - - - - - - - - - - - - - - - - - - - - - -   */
  /**
   * @return User Array data
   * category by user roles
   */
  public function userArrayData() {
    $output = NULL;

    foreach (entity_load('user') as $key => $row) {
      if ($key > 0) {
        if ($row->status == 1) {
          $UserInfo = new UserInfo($row->uid);

          $output['speaker']['rid'] = 6;
          if (user_has_role(6, $row)) {
            $output['speaker']['user'][$row->uid] = array(
              'affiliation' => $UserInfo->affiliation(),
              'email' => $UserInfo->userEmail(),
              'location' => $UserInfo->location(),
              'mobile' => $UserInfo->mobile(),
              'fullname' => $UserInfo->fullname(),
              'specialty' => $UserInfo->specialty(),
              'specialtyRepEmail' => $UserInfo->specialtyRepEmail(),
              'specialtyRepFullName' => $UserInfo->specialtyRepFullName(),
              'uid' => $row->uid,
              'userGroupTid' => $UserInfo->userGroupTid(),
            );
          }

          $output['rep']['rid'] = 7;
          if (user_has_role(7, $row)) {
            $output['rep']['user'][$row->uid] = array(
              'affiliation' => $UserInfo->affiliation(),
              'email' => $UserInfo->userEmail(),
              'location' => $UserInfo->location(),
              'mobile' => $UserInfo->mobile(),
              'fullname' => $UserInfo->fullname(),
              'specialty' => $UserInfo->specialty(),
              'specialtyRepEmail' => $UserInfo->specialtyRepEmail(),
              'specialtyRepFullName' => $UserInfo->specialtyRepFullName(),
              'uid' => $row->uid,
            );
          }

        }
      }
    }

    return $output;
  }

  /**
   * @return User List data
   * category by user roles
   */
  public function userListData() {
    $output = NULL;

    $province_term_array = taxonomy_get_tree(5);
    $province_term = array();

    // add province option
    if (is_array($province_term_array)) {
      foreach ($province_term_array as $term_key => $term_row) {
        $key_name = 'term_' . $term_row->tid;

        $province_term['province'][$key_name] = array(
          'tid'  => $term_row->tid,
          'name' => $term_row->name,
        );
      }

      $output['terms'] = $province_term;
    }

    $output['users']['option']['timezone'] = array(
      'America/Edmonton' => "America/Edmonton",
      'America/Halifax' => 'America/Halifax',
      'America/Regina' => 'America/Regina',
      'America/St_Johns' => 'America/St Johns',
      'America/Toronto' => 'America/Toronto',
      'America/Vancouver' => 'America/Vancouver',
      'America/Winnipeg' => 'America/Winnipeg',
    );

    foreach (entity_load('user') as $key => $row) {
      if ($key > 0) {
        if ($row->status == 1) {
          $UserInfo = new UserInfo($row->uid);

          $uid_key = 'user_' . $row->uid;

          $output['users']['profile'][$uid_key] = array(
            'affiliation' => $UserInfo->affiliation(),
            'city' => $UserInfo->city(),
            'email' => $UserInfo->userEmail(),
            'firstName' => $UserInfo->firstName(),
            'fullname' => $UserInfo->fullname(),
            'lastName' => $UserInfo->lastName(),
            'location' => $UserInfo->location(),
            'mobile' => $UserInfo->mobile(),
            'provinceName' => $UserInfo->provinceName(),
            'provinceTid' => $UserInfo->provinceTid(),
            'specialty' => $UserInfo->specialty(),
            'specialtyRepEmail' => $UserInfo->specialtyRepEmail(),
            'specialtyRepFullName' => $UserInfo->specialtyRepFullName(),
            'uid' => $row->uid,
            'userName' => $UserInfo->userName(),
            'timezone' => $UserInfo->userTimezoneValue(),
          );
        }
      }
    }

    return $output;
  }

  /**
   * @return User Array data
   * category by user roles
   */
  public function userReportData() {
    $output = NULL;

    foreach (entity_load('user') as $key => $row) {
      if ($key > 0) {
        if ($row->status == 1) {
          $UserInfo = new UserInfo($row->uid);

          $output['users']['speaker']['rid'] = 6;
          $output['users']['rep']['rid'] = 7;

          if (user_has_role(6, $row)) {
            $output['users']['speaker']['user'][$row->uid] = array(
              'avgResponseTime' => $UserInfo->randNum(1, 20),
              'email' => $UserInfo->userEmail(),
              'eventScheduled' => $UserInfo->randNum(1, 20),
              'fullname' => $UserInfo->fullname(),
              'lastVisit' => $UserInfo->lastLoginTimeInterval(),
              'someMetric' => array(
                'metric0' => array(
                  'color' => '#33bcde',
                  'value' => 78,
                ),
                'metric1' => array(
                  'color' => '#b6da52',
                  'value' => 78,
                ),
                'metric2' => array(
                  'color' => '#E72682',
                  'value' => 78,
                ),
              ),
              'supportRequests' => $UserInfo->randNum(1, 30),
              'uid' => $row->uid,
              'meetingActivity' => array(
                'completed' => $UserInfo->randNum(1, 20),
                'confirmed' => $UserInfo->randNum(1, 20),
                'cancelled' => $UserInfo->randNum(1, 20),
                'pending' => $UserInfo->randNum(1, 20),
                'declined' => $UserInfo->randNum(1, 20),
              ),
              'regionalMeeting' => array(
                'jan' => $UserInfo->randNum(1, 10),
                'feb' => $UserInfo->randNum(1, 10),
                'mar' => $UserInfo->randNum(1, 10),
                'apr' => $UserInfo->randNum(1, 10),
                'may' => $UserInfo->randNum(1, 10),
                'jun' => $UserInfo->randNum(1, 10),
                'jul' => $UserInfo->randNum(1, 10),
                'aug' => $UserInfo->randNum(1, 10),
                'sep' => $UserInfo->randNum(1, 10),
                'oct' => $UserInfo->randNum(1, 10),
                'nov' => $UserInfo->randNum(1, 10),
                'dec' => $UserInfo->randNum(1, 10),
              ),
            );
          }

          if (user_has_role(7, $row)) {
            $output['users']['rep']['user'][$row->uid] = array(
              'avgResponseTime' => $UserInfo->randNum(1, 20),
              'email' => $UserInfo->userEmail(),
              'eventScheduled' => $UserInfo->randNum(1, 20),
              'fullname' => $UserInfo->fullname(),
              'lastVisit' => $UserInfo->lastLoginTimeInterval() ? $UserInfo->lastLoginTimeInterval() : 'N/A',
              'someMetric' => array(
                'metric0' => array(
                  'color' => '#33bcde',
                  'value' => 78,
                ),
                'metric1' => array(
                  'color' => '#b6da52',
                  'value' => 78,
                ),
                'metric2' => array(
                  'color' => '#E72682',
                  'value' => 78,
                ),
              ),
              'supportRequests' => $UserInfo->randNum(1, 30),
              'uid' => $row->uid,
              'meetingActivity' => array(
                'completed' => $UserInfo->randNum(1, 20),
                'confirmed' => $UserInfo->randNum(1, 20),
                'cancelled' => $UserInfo->randNum(1, 20),
                'pending' => $UserInfo->randNum(1, 20),
                'declined' => $UserInfo->randNum(1, 20),
              ),
              'regionalMeeting' => array(
                'jan' => $UserInfo->randNum(1, 10),
                'feb' => $UserInfo->randNum(1, 10),
                'mar' => $UserInfo->randNum(1, 10),
                'apr' => $UserInfo->randNum(1, 10),
                'may' => $UserInfo->randNum(1, 10),
                'jun' => $UserInfo->randNum(1, 10),
                'jul' => $UserInfo->randNum(1, 10),
                'aug' => $UserInfo->randNum(1, 10),
                'sep' => $UserInfo->randNum(1, 10),
                'oct' => $UserInfo->randNum(1, 10),
                'nov' => $UserInfo->randNum(1, 10),
                'dec' => $UserInfo->randNum(1, 10),
              ),
            );
          }

        }
      }
    }

    return $output;
  }



}
