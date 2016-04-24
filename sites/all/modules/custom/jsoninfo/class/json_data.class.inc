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
  /**
   * @return Array data
   */
  public function nodeEventList($meetings = array()) {
    global $base_path;

    $nodes = node_load_multiple($meetings);

    $output = NULL;
    if (is_array($nodes)) {
      foreach ($nodes as $key => $node) {
        $MeetingInfo = new MeetingInfo($node->nid);

        $NodeQuery = new NodeQuery();
        $evaluations = $NodeQuery->evaluationNidsByMeeting(array($node->nid));

        $output[] = array(
          'editText' => 'EDIT',
          'eventNid' => $node->nid,
          'eventName' => $MeetingInfo->programName(),
          'eventProgramName' => $MeetingInfo->programName(),
          'eventDescrption' => '',
          'eventDate' => $MeetingInfo->dateValue(),
          'eventCity' => $MeetingInfo->cityName(),
          'eventProvince' => $MeetingInfo->provinceName(),
          'eventSpeaker' => $MeetingInfo->speakerName(),
          'eventRep' => $MeetingInfo->repName(),
          'eventEvaluationsCount' => $MeetingInfo->evaluationsCount(),
          'linkCreateEvaluation' => $base_path . 'page/evaluation/render/' . $node->nid,
          'linkEdit' => $base_path . 'manage/meeting/edit/' . $node->nid . '?destination=page/events/list/2180',
        );
      }
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function nodeMeeting() {
    $output = $this->termStandardJson(19, 'TermBrandInfo');
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function nodeMeetingComments($meetings = array(), $program_tid = NULL) {
    $output = NULL;
    $dataset = NULL;

    $NodeQuery = new NodeQuery();

    if (is_array($meetings)) {
      $MeetingInfo = new MeetingInfo(current($meetings));
      $TermEvaluationFormInfo = new TermEvaluationFormInfo($MeetingInfo->evaluationFormTid());

      if (is_array($TermEvaluationFormInfo->questionSetArray())) {
        foreach ($TermEvaluationFormInfo->questionSetArray() as $key => $value) {
          if (is_numeric($value['quesset_question_tid'])) {
            $TermQuestionLibraryInfo = new TermQuestionLibraryInfo($value['quesset_question_tid']);

            if (is_numeric($TermQuestionLibraryInfo->questionFieldTid())) {
              $TermQuestionFieldInfo = new TermQuestionFieldInfo($TermQuestionLibraryInfo->questionFieldTid());

              if (is_array($TermQuestionFieldInfo->multipleFieldsTidArray())) {
                foreach ($TermQuestionFieldInfo->multipleFieldsTidArray() as $key2 => $value2) {

                  if ($value2 == 65) {
                    $dataset[$value['quesset_question_tid']]['title'] = $TermQuestionLibraryInfo->termName();
                    $dataset[$value['quesset_question_tid']]['entry'] = array();

                    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

                    if (is_array($evaluations)) {
                      foreach ($evaluations as $key3 => $value3) {
                        $EvaluationInfo = new EvaluationInfo($value3);

                        if(is_array($EvaluationInfo->evaluationAnswerSet())) {
                          foreach ($EvaluationInfo->evaluationAnswerSet() as $key4 => $value4) {
                            if ($value4['answerset_queslibr_tid'] == $value['quesset_question_tid']) {
                              if ($value4['answerset_quesfield_tid'] == 65) {
                                if (!empty($value4['answerset_answer'])) {
                                  $dataset[$value['quesset_question_tid']]['entry'][] = $value4['answerset_answer'];
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    if (is_array($dataset)) {
      foreach ($dataset as $key => $row) {
        $header_class = 'bg-163c52';
        $header_value = $row['title'];

        $middle_value = '';
        if (is_array($row['entry'])) {
          $middle_value = '<li>' . implode("</li><li>", $row['entry']) . '</li>';
        }

        $output[] = $JsonDataBlock->comments('26663687' . $key, $header_value, $header_class, $middle_value);
      }
    }

    return $output;
  }

  /**
   * @return, Program Learning Objective result Average number
   */
  public function nodeMeetingLearningObjective($meetings = array()) {
    $learning_objective_question_tids = array();
    $evaluation_form_tids = array();

    // get all program Learning Objective
    if (is_array($meetings)) {
      foreach ($meetings as $meeting_nid) {
        $MeetingInfo = new MeetingInfo($meeting_nid);
        $evaluation_form_tids[] = $MeetingInfo->evaluationFormTid();
      }

      if (is_array($evaluation_form_tids)) {
        $evaluation_form_tids = $evaluation_form_tids;
      }
    }

    if (is_array($evaluation_form_tids)) {
      foreach ($evaluation_form_tids as $evaluation_form_tid) {
        $TermEvaluationFormInfo = new TermEvaluationFormInfo($evaluation_form_tid);

        if (is_array($TermEvaluationFormInfo->questionSetArray())) {
          foreach ($TermEvaluationFormInfo->questionSetArray() as $row) {
            $TermQuestionLibraryInfo = new TermQuestionLibraryInfo($row['quesset_question_tid']);
            if ($TermQuestionLibraryInfo->questionTypeTid() == 2213) {    // 2213 is Learning Objective
              $learning_objective_question_tids[] = $row['quesset_question_tid'];
            }
          }
        }
      }
    }

    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    $output = array(
      5 => 0,
      4 => 0,
      3 => 0,
      2 => 0,
      1 => 0,
    );

    if (is_array($learning_objective_question_tids)) {
      foreach ($learning_objective_question_tids as $question_tid) {
        $TermQuestionLibraryInfo = new TermQuestionLibraryInfo($question_tid);

        // Evaluation
        $EvaluationFieldQuery = new EvaluationFieldQuery();
        $learning_question_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $question_tid, 64);
        $learning_question_count = BasicClass::arrayCountValuesBySpecifyArray($learning_question_answer_set, array(1, 2, 3, 4, 5));

        $output[5] += $learning_question_count[5];
        $output[4] += $learning_question_count[4];
        $output[3] += $learning_question_count[3];
        $output[2] += $learning_question_count[2];
        $output[1] += $learning_question_count[1];
      }
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function nodeMeetingInfo($nid = NULL) {
    $MeetingInfo = new MeetingInfo($nid);

    $output = array(
      'cityName' => $MeetingInfo->cityName(),
      'dateValue' => $MeetingInfo->dateValue(),
      'evaluationsCount' => $MeetingInfo->evaluationsCount(),
      'participantsCount' => $MeetingInfo->signatures(),
      'programName' => $MeetingInfo->programName(),
      'provinceName' => $MeetingInfo->provinceName(),
      'repName' => $MeetingInfo->repName(),
      'speakerName' => $MeetingInfo->speakerName(),
      'venueName' => $MeetingInfo->venueName(),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function nodeMeetingEdit($nid = NULL) {
    $MeetingInfo = new MeetingInfo($nid);

    $output = array(
      'program' => array(
        'title' => array(
          'updateStatus' => 0,
          'defaultValue' => $MeetingInfo->programTid(),
        ),
        'type' => array(
          'updateStatus' => 0,
          'defaultValue' => $MeetingInfo->programTypeTid(),
        ),
        'class' => array(
          'updateStatus' => 0,
          'defaultValue' => $MeetingInfo->programClassTid(),
        ),
      ),
      'evaluationForm' => array(
        'updateStatus' => 0,
        'defaultValue' => $MeetingInfo->evaluationFormTid(),
      ),
      'timestamp' => array(
        'updateStatus' => 0,
        'defaultValue' => $MeetingInfo->dateUnixStamp(),
      ),
      'multiTherape' => array(
        'updateStatus' => 0,
        'defaultValue' => $MeetingInfo->multiTherapeTid(),
      ),
      'meetingType' => array(
        'updateStatus' => 0,
        'defaultValue' => $MeetingInfo->meetingTypeTid(),
      ),
      'repName' => array(
        'updateStatus' => 0,
        'defaultValue' => $MeetingInfo->repUid(),
      ),
      'speakerName' => array(
        'updateStatus' => 0,
        'defaultValue' => $MeetingInfo->speakerUid(),
      ),
      'signature' => array(
        'updateStatus' => 0,
        'defaultValue' => $MeetingInfo->signatures(),
      ),
      'meetingReceived' => array(
        'updateStatus' => 0,
        'defaultValue' => $MeetingInfo->meetingReceivedTid(),
      ),
    );

    return $output;
  }


  /**
   * @return Term Array data
   */
  public function nodeMeetingChart($nid = NULL) {
    $MeetingInfo = new MeetingInfo($nid);
    $MeetingInfo->programTid();

    $output = NULL;
    $output = array(
      'class' => 'font-size-12 col-xs-12',
      'type' => 'widget',
      'value' => array(
        array(
          'title' => t('Date') . ':',
          'value' => $MeetingInfo->dateValue(),
          'class' => 'bg-ffffff col-xs-6 col-md-3',
        ),
      ),
    );

    return $output;
  }

  /** - - - - - page - - - - - - - - - - - - - - - - - - - - - - */
  /**
   * @return
   */
  public function pageMeetingMap($meetings = array()) {
    $meeting_nodes = node_load_multiple($meetings);

    $output = array();
    foreach ($meeting_nodes as $key => $node) {
      $MeetingInfo = new MeetingInfo($node->nid);
      $TermProgramInfo = new TermProgramInfo($MeetingInfo->programTid());

      if ($MeetingInfo->latitude() && $MeetingInfo->longitude()) {
        $output[$node->nid] = array(
          "lat" => $MeetingInfo->latitude(),
          "lng" => $MeetingInfo->longitude(),
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
   * @return Term Array data
   */
  public function pushDefaultValue($import_array = NULL, $default_value = array()) {
    $output = NULL;
    if (is_array($import_array)) {
      $output = array_merge(array($default_value), $import_array);
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
    $nodes = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);

    $output = $this->jobboardJobsMap($nodes);

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
