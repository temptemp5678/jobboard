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

  /** - - - - - Term Edit - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /**
   * @return Term Array data
   */
  public function termBusinessUnitEdit($tid = NULL) {
    $output = NULL;
    if ($tid) {
      $TermProgramUnitInfo = new TermProgramUnitInfo($tid);

      $selectedTherapeuticAreaArray = array();
      if (is_array($TermProgramUnitInfo->therapeuticAreaTids())) {
        foreach ($TermProgramUnitInfo->therapeuticAreaTids() as $key => $value) {
          $TermInfo = new TermInfo($value);
          $selectedTherapeuticAreaArray[] = array(
            'termTid' => $value,
            'termName' => $TermInfo->termName(),
          );
        }
      }
      $selectedBrandArray = array();
      if (is_array($TermProgramUnitInfo->brandTids())) {
        foreach ($TermProgramUnitInfo->brandTids() as $key => $value) {
          $TermInfo = new TermInfo($value);
          $selectedBrandArray[] = array(
            'termTid' => $value,
            'termName' => $TermInfo->termName(),
          );
        }
      }

      $output = array(
        'name' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermProgramUnitInfo->termName(),
        ),
        'therapeuticArea' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermProgramUnitInfo->therapeuticAreaTids(),
          'selectedTherapeuticArea' => $selectedTherapeuticAreaArray,
        ),
        'brand' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermProgramUnitInfo->brandTids(),
          'selectedBrands' => $selectedBrandArray,
        ),
      );
    }

    return $output;
  }
  /**
   * @return Term Array data
   */
  public function termEvaluationFormEdit($tid = NULL) {
    $output = NULL;
    if ($tid) {
      $TermEvaluationFormInfo = new TermEvaluationFormInfo($tid);

      $questionSetList = array();
      $questionSetList = $TermEvaluationFormInfo->questionSetListOnce();

      $output = array(
        'evaluationFormTid' => $TermEvaluationFormInfo->termTid(),
        'defaultValue' => $TermEvaluationFormInfo->termName(),
        'updateStatus' => 0,
        'questionSetList' => array(
          'updateStatus' => 0,
          'selectedQuestionSetList' => $questionSetList,
        ),
      );
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termEvaluationFormQuestionSet($tid = NULL) {
    $output = NULL;
    if ($tid) {
      $TermEvaluationFormInfo = new TermEvaluationFormInfo($tid);

      $output[] = array(
        'evaluationFormTid'  => $tid,
        'evaluationFormName' => $TermEvaluationFormInfo->termName(),
        'questionSetList' => $TermEvaluationFormInfo->questionSetList(),
      );
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termProgramEdit($tid = NULL) {
    $output = NULL;
    if ($tid) {
      $TermProgramInfo = new TermProgramInfo($tid);

      $selectedTherapeuticAreaArray = array();
      if (is_array($TermProgramInfo->therapeuticAreaTids())) {
        foreach ($TermProgramInfo->therapeuticAreaTids() as $key => $value) {
          $TermInfo = new TermInfo($value);
          $selectedTherapeuticAreaArray[] = array(
            'termTid' => $value,
            'termName' => $TermInfo->termName(),
          );
        }
      }
      $selectedBrandArray = array();
      if (is_array($TermProgramInfo->brandTids())) {
        foreach ($TermProgramInfo->brandTids() as $key => $value) {
          $TermInfo = new TermInfo($value);
          $selectedBrandArray[] = array(
            'termTid' => $value,
            'termName' => $TermInfo->termName(),
          );
        }
      }

      $selectedProgramTypeArray = array();
      if (is_array($TermProgramInfo->programTypeTids())) {
        foreach ($TermProgramInfo->programTypeTids() as $key => $value) {
          $TermInfo = new TermInfo($value);
          $selectedProgramTypeArray[] = array(
            'termTid' => $value,
            'termName' => $TermInfo->termName(),
          );
        }
      }
      $selectedProgramClassArray = array();
      if (is_array($TermProgramInfo->programClassTids())) {
        foreach ($TermProgramInfo->programClassTids() as $key => $value) {
          $TermInfo = new TermInfo($value);
          $selectedProgramClassArray[] = array(
            'termTid' => $value,
            'termName' => $TermInfo->termName(),
          );
        }
      }
      $evaluationFormArray = array();
      if (is_array($TermProgramInfo->evaluationFormTids())) {
        foreach ($TermProgramInfo->evaluationFormTids() as $key => $value) {
          $TermInfo = new TermInfo($value);
          $evaluationFormArray[] = array(
            'termTid' => $value,
            'termName' => $TermInfo->termName(),
          );
        }
      }

      $output = array(
        'name' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermProgramInfo->termName(),
        ),
        'businessUnit' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermProgramInfo->programUnitTid(),
          'termTid' => $TermProgramInfo->programUnitTid(),
          'termName' => $TermProgramInfo->termName(),
          'brand' => array(
            'updateStatus' => 0,
            'defaultValue' => $TermProgramInfo->brandTids(),
            'selectedBrand' => $selectedBrandArray,
          ),
          'therapeuticArea' => array(
            'updateStatus' => 0,
            'defaultValue' => $TermProgramInfo->therapeuticAreaTids(),
            'selectedTherapeuticArea' => $selectedTherapeuticAreaArray,
          ),
        ),
        'programType' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermProgramInfo->programTypeTid(),
        ),
        'programClass' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermProgramInfo->programClassTids(),
          'selectedProgramClass' => $selectedProgramClassArray,
        ),
        'division' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermProgramInfo->divisionTid(),
          'termTid' => $TermProgramInfo->divisionTid(),
          'termName' => $TermProgramInfo->divisionName(),
        ),
        'evaluationForm' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermProgramInfo->evaluationFormTids(),
          'selectedEvaluationsForm' => $evaluationFormArray,
        ),
      );
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termQuestionLibraryEdit($tid = NULL) {
    $output = NULL;
    if ($tid) {
      $TermQuestionLibraryInfo = new TermQuestionLibraryInfo($tid);

      $output = array(
        'question' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermQuestionLibraryInfo->termName(),
        ),
        'scaleType' => array(          // questionField
          'updateStatus' => 0,
          'defaultValue' => $TermQuestionLibraryInfo->questionFieldTid(),
          'selectedQuestionField' => array(
            'termTid' => $TermQuestionLibraryInfo->questionFieldTid(),
            'termName' => $TermQuestionLibraryInfo->questionFieldName(),
          ),
        ),
        'measurement' => array(         // question Label
          'updateStatus' => 0,
          'defaultValue' => $TermQuestionLibraryInfo->questionLabelTid(),
          'selectedLabel' => array(
            'termTid' => $TermQuestionLibraryInfo->questionLabelTid(),
            'termName' => $TermQuestionLibraryInfo->questionLabelName(),
          ),
        ),
        'variables' => array(          // length
          'count' => array(
            'updateStatus' => 0,
            'defaultValue' => $TermQuestionLibraryInfo->questionScale(),
          ),
        ),
        'questionType' => array(
          'updateStatus' => 0,
          'defaultValue' => $TermQuestionLibraryInfo->questionTypeTid(),
          'selectedScale' => array(
            'termTid' => $TermQuestionLibraryInfo->questionTypeTid(),
            'termName' => $TermQuestionLibraryInfo->questionTypeName(),
          ),
        ),
      );
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termQuestionLibraryFieldOption() {
    $output = NULL;

    for ($i = 1; $i < 10; $i++) {
      $variables_option[] = array(
        'tid' => $i,
        'name' => $i,
      );
    }

    $empty_value = new stdClass();
    $empty_value->tid = '_none';
    $empty_value->name = '- None -';
    $questionType[] = $empty_value;
    $questionType += taxonomy_get_tree(24);

    $output = array(
      'scale' => taxonomy_get_tree(3),            // question_field
      'measurement' => taxonomy_get_tree(5),
      'variables' => $variables_option,           // scale length 1, 2, 3, 4, 5
      'questionType' => taxonomy_get_tree(24),    // Learning Objective
      // 'questionType' => $questionType,    // Learning Objective
    );

    return $output;
  }

  /** - - - - - Term - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /**
   * @return Term Array data
   */
  public function termBusinessUnitOld() {
    $output = NULL;

    $all_term_array = taxonomy_get_tree(11);

    if (is_array($all_term_array)) {
      foreach ($all_term_array as $term_key => $term_row) {
        $TermProgramUnitInfo = new TermProgramUnitInfo($term_row->tid);

        $extra_array = array(
          'linkEdit' => base_path() . 'manage/program-unit/edit/' . $term_row->tid . '?destination=manage/program/list',
        );

        $output[] = ($extra_array + ($TermProgramUnitInfo->allMethodsValue()));
      }
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termBusinessUnitTree() {
    $output = NULL;

    $unit_tids = taxonomy_get_tree(11);

    if (is_array($unit_tids)) {
      foreach ($unit_tids as $unit_tid) {
        $TermProgramUnitInfo = new TermProgramUnitInfo($unit_tid);

        if (is_array($TermProgramUnitInfo->therapeuticAreaTids())) {
          foreach ($TermProgramUnitInfo->therapeuticAreaTids() as $therapeuticAreaTid) {
            $TermTherapeuticAreaInfo = new TermTherapeuticAreaInfo($therapeuticAreaTid);

            if (is_array($TermProgramUnitInfo->brandTids())) {
              foreach ($TermProgramUnitInfo->brandTids() as $brandTid) {
                $TermBrandInfo = new TermBrandInfo($brandTid);

                $output[] = array(
                  'businessUnitTermTid' => $TermProgramUnitInfo->termTid(),
                  'businessUnitTermName' => $TermProgramUnitInfo->termName(),
                  'therapeuticAreaTermTid' => $TermTherapeuticAreaInfo->termTid(),
                  'therapeuticAreaTermName' => $TermTherapeuticAreaInfo->termName(),
                  'brandTermTid' => $TermBrandInfo->termTid(),
                  'brandTermName' => $TermBrandInfo->termName(),
                  'programNumber' => rand(1, 10),
                  'eventNumber' => rand(10, 40),
                );
              }
            }
            else {
              $output[] = array(
                'businessUnitTermTid' => $TermProgramUnitInfo->termTid(),
                'businessUnitTermName' => $TermProgramUnitInfo->termName(),
                'therapeuticAreaTermTid' => $TermTherapeuticAreaInfo->termTid(),
                'therapeuticAreaTermName' => $TermTherapeuticAreaInfo->termName(),
                'programNumber' => rand(1, 10),
                'eventNumber' => rand(10, 40),
              );
            }
          }
        }
      }
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termCityJsonFile() {
    $url = 'public://json/cache/term_city_basic.json';
    $output = file_get_contents($url);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termCityArrayFile() {
    $url = 'public://json/cache/term_city_basic.json';
    if (file_exists($url)) {
      $output = drupal_json_decode(file_get_contents($url));
    }
    else {
      $output = $this->termCity(TRUE);
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termEventRegionFilterList() {
    $output = NULL;

    $tids = taxonomy_get_tree(26);
    if (is_array($tids)) {
      foreach ($tids as $tid) {
        $TermEventRegionInfo = new TermEventRegionInfo($tid);
        $output[] = array(
          'termName' => $TermEventRegionInfo->termName(),
          'imageFlag' => $TermEventRegionInfo->countryFlagImage(),
        );
      }
    }
    return $output;
  }

  /** - - - - - Term True - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /**
   * @return Term Array data
   */
  public function termBrand($basic = NULL) {
    $output = $this->termStandardJson(19, 'TermBrandInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termBusinessUnit($basic = NULL) {
    $output = $this->termStandardJson(11, 'TermProgramUnitInfo', $basic);

    // $extra_array = array(
    //   'linkEdit' => base_path() . 'manage/program-unit/edit/' . $term_row->tid . '?destination=manage/program/list',
    // );

    // $output = $extra_array + ($this->termStandardJson(11, 'TermProgramUnitInfo', $basic));

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termCity($basic = NULL) {
    $output = $this->termStandardJson(18, 'TermCityInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termEvaluationForm($basic = NULL) {
    $output = $this->termStandardJson(6, 'TermEvaluationFormInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termMeetingType($basic = NULL) {
    $output = $this->termStandardJson(16, 'TermMeetingTypeInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termMeetingReceived($basic = NULL) {
    $output = $this->termStandardJson(20, 'TermMeetingReceivedInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data, Multi-Therapeutic Event
   */
  public function termMultiTherape($basic = NULL) {
    $output = $this->termStandardJson(21, 'TermMultiTherapeInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termProgram($basic = NULL) {
    $output = $this->termStandardJson(2, 'TermProgramInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termProgramList() {
    global $base_path;

    $output = NULL;

    $program_tids = taxonomy_get_tree(2);
    if (is_array($program_tids)) {
      foreach ($program_tids as $program_tid) {
        $TermProgramInfo = new TermProgramInfo($program_tid);
        // if ($TermProgramInfo->termTid() != 2103) {
        //   continue;
        // }

        $programTypeTermName = '';
        $programTypeTermNameArray = array();
        if (is_array($TermProgramInfo->programTypeTids())) {
          foreach ($TermProgramInfo->programTypeTids() as $therapeuticAreaTid) {
            $TermProgramTypeInfo = new TermProgramTypeInfo($therapeuticAreaTid);
            $programTypeTermNameArray[] = $TermProgramTypeInfo->termName();
          }
          $programTypeTermName = implode(' ', $programTypeTermNameArray);
        }

        $programClassName = '';
        $programClassNameArray = array();
        if (is_array($TermProgramInfo->programClassTids())) {
          foreach ($TermProgramInfo->programClassTids() as $programClassTid) {
            $TermProgramClassInfo = new TermProgramClassInfo($programClassTid);
            $programClassNameArray[] = $TermProgramClassInfo->termName();
          }
          $programClassName = implode(' ', $programClassNameArray);
        }

        $output[] = array(
          'programTermTid' => $TermProgramInfo->termTid(),
          'programTermName' => $TermProgramInfo->termName(),
          'businessUnitName' => $TermProgramInfo->programUnitName(),
          'eventNumber' => $TermProgramInfo->eventNumber(),
          'evaluationsCount' => $TermProgramInfo->evaluationsCount(),
          'programTypeTermName' => $programTypeTermName,
          'programClassName' => $programClassName,
          'linkEdit' => $base_path .  'page/programs/edit/' . $TermProgramInfo->termTid(),
        );
      }
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termProgramTree() {
    global $base_path;

    $output = NULL;

    $program_tids = taxonomy_get_tree(2);
    if (is_array($program_tids)) {
      foreach ($program_tids as $program_tid) {
        $TermProgramInfo = new TermProgramInfo($program_tid);

        if (is_array($TermProgramInfo->programTypeTids())) {
          foreach ($TermProgramInfo->programTypeTids() as $therapeuticAreaTid) {
            $TermProgramTypeInfo = new TermProgramTypeInfo($therapeuticAreaTid);

            if (is_array($TermProgramInfo->programClassTids())) {
              foreach ($TermProgramInfo->programClassTids() as $programClassTid) {
                $TermProgramClassInfo = new TermProgramClassInfo($programClassTid);

                $output[] = array(
                  'programTermTid' => $TermProgramInfo->termTid(),
                  'programTermName' => $TermProgramInfo->termName(),
                  'programTypeTermTid' => $TermProgramTypeInfo->termTid(),
                  'programTypeTermName' => $TermProgramTypeInfo->termName(),
                  'programClassTid' => $TermProgramClassInfo->termTid(),
                  'programClassName' => $TermProgramClassInfo->termName(),
                  'linkEdit' => $base_path .  'page/programs/edit/' . $TermProgramInfo->termTid(),
                );
              }
            }
            else {
              $output[] = array(
                'programTermTid' => $TermProgramInfo->termTid(),
                'programTermName' => $TermProgramInfo->termName(),
                'programTypeTermTid' => $TermProgramTypeInfo->termTid(),
                'programTypeTermName' => $TermProgramTypeInfo->termName(),
                'linkEdit' => $base_path .  'page/programs/edit/' . $TermProgramInfo->termTid(),
              );
            }
          }
        }
      }
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termProgramClass($basic = NULL) {
    $output = $this->termStandardJson(13, 'TermProgramClassInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termProgramType($basic = NULL) {
    $output = $this->termStandardJson(12, 'TermProgramTypeInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termProvince($basic = NULL) {
    $output = $this->termStandardJson(8, 'TermProvinceInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termQuestionLibrary($basic = NULL) {
    $output = $this->termStandardJson(4, 'TermQuestionLibraryInfo', $basic);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termTherapeuticArea() {
    $output = $this->termStandardJson(15, 'TermTherapeuticAreaInfo');
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termDivision() {
    $output = $this->termStandardJson(30, 'TermDivisionInfo');
    return $output;
  }

  /** - - - - - Page Json block - - - - - - - - - - - - - - - - - - - - - -   */
  public function blockTotalEventDistribution($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    // tab one - - Total Event Distribution(All Business Unit)
    $tabOne_monthly_value = array(
      'labels' => array("JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"),
      'datasets' => array(
        array(
          "fillColor" => "rgba(151,187,205,0)",
          "strokeColor" => "#37d8b3",
          "pointColor" => "#37d8b3",
          "pointStrokeColor" => "#fff",
          "data" => array(
            count($NodeQuery->meetingNidsByMonth(array(1), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(2), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(3), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(4), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(5), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(6), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(7), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(8), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(9), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(10), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(11), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(12), $meetings)),
          ),
          "title"=> '',
        ),
      ),
    );
    $tabOne_monthly_chart = $JsonDataBlock->chartLine(102, t('MONTH'), 'bg-ffffff', $tabOne_monthly_value);

    // disable Horizontal Bar for quarterly, divide by Accredited and Non Accredited
    // $tabOne_quarterly_value = array(
    //   "labels" => array("Q1", "Q2", "Q3", "Q4"),
    //   "datasets" => array(
    //     array(
    //       "fillColor" => "#00a9e0",
    //       "strokeColor" => "#00a9e0",
    //       "pointColor" => "#05d23e",
    //       "pointStrokeColor" => "#fff",
    //       "data" => array(
    //         rand(0, 20),
    //         rand(0, 20),
    //         rand(0, 20),
    //         rand(0, 20),
    //       ),
    //       "title"=> t("Accredited"),
    //     ),
    //     array(
    //       "fillColor" => "#05d23e",
    //       "strokeColor" => "#05d23e",
    //       "pointColor" => "#00a9e0",
    //       "pointStrokeColor" => "#fff",
    //       "data" => array(
    //         rand(0, 20),
    //         rand(0, 20),
    //         rand(0, 20),
    //         rand(0, 20),
    //       ),
    //       "title"=> t("Non Accredited"),
    //     ),
    //   )
    // );
    // $tabOne_quarterly_chart = $JsonDataBlock->chartHorizontalBar(103, t('QUARTERLY'), 'bg-ffffff', $tabOne_quarterly_value);
    $tabOne_quarterly_value = array(
      'labels' => array("Q1", "Q2", "Q3", "Q4"),
      'title' => "",
      'datasets' => array(
        array(
          "fillColor" => "#344a5e",
          "strokeColor" => "#344a5e",
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            count($NodeQuery->meetingNidsByMonth(array(1), $meetings)) + count($NodeQuery->meetingNidsByMonth(array(2), $meetings)) + count($NodeQuery->meetingNidsByMonth(array(3), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(4), $meetings)) + count($NodeQuery->meetingNidsByMonth(array(5), $meetings)) + count($NodeQuery->meetingNidsByMonth(array(6), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(7), $meetings)) + count($NodeQuery->meetingNidsByMonth(array(8), $meetings)) + count($NodeQuery->meetingNidsByMonth(array(9), $meetings)),
            count($NodeQuery->meetingNidsByMonth(array(10), $meetings)) + count($NodeQuery->meetingNidsByMonth(array(11), $meetings)) + count($NodeQuery->meetingNidsByMonth(array(12), $meetings)),
          ),
          "title"=> '',
        ),
      ),
    );
    $tabOne_quarterly_chart = $JsonDataBlock->chartBarCustomInGraph(103, t('QUARTERLY'), 'bg-ffffff', $tabOne_quarterly_value);

    $tabOne_province_value = array(
      'labels' => array("BC","AB","SK","MB","ON","QC","NB","PEI","NS","NL"),
      'title' => "",
      'datasets' => array(
        array(
          "fillColor" => "#344a5e",
          "strokeColor" => "#344a5e",
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            count($NodeQuery->meetingNidsByProvince(array(20), $meetings)),
            count($NodeQuery->meetingNidsByProvince(array(19), $meetings)),
            count($NodeQuery->meetingNidsByProvince(array(30), $meetings)),
            count($NodeQuery->meetingNidsByProvince(array(21), $meetings)),
            count($NodeQuery->meetingNidsByProvince(array(27), $meetings)),
            count($NodeQuery->meetingNidsByProvince(array(29), $meetings)),
            count($NodeQuery->meetingNidsByProvince(array(22), $meetings)),
            count($NodeQuery->meetingNidsByProvince(array(28), $meetings)),
            count($NodeQuery->meetingNidsByProvince(array(25), $meetings)),
            count($NodeQuery->meetingNidsByProvince(array(23), $meetings)),
          ),
          "title"=> '',
        ),
        // array(
        //   "fillColor" => "#05d23e",
        //   "strokeColor" => "#05d23e",
        //   "pointColor" => "#00a9e0",
        //   "pointStrokeColor" => "#fff",
        //   "data" => array(
        //     rand(0, 20),
        //     rand(0, 20),
        //     rand(0, 20),
        //     rand(0, 20),
        //     rand(0, 20),
        //     rand(0, 20),
        //     rand(0, 20),
        //     rand(0, 20),
        //     rand(0, 20),
        //     rand(0, 20),
        //   ),
        //   "title"=> t('OLA'),
        // ),
      ),
    );
    $tabOne_province_chart_options['barValueSpacing'] = 40;
    $tabOne_province_chart = $JsonDataBlock->chartBarCustomInGraph(104, t('PROVINCE'), 'bg-ffffff', $tabOne_province_value, $tabOne_province_chart_options);

    $tabOne_googlemap_chart = $JsonDataBlock->googleMap(105, t('MAP'), 'bg-ffffff');

    $tabOne = array(
      $tabOne_monthly_chart,
      $tabOne_quarterly_chart,
      $tabOne_province_chart,
      $tabOne_googlemap_chart,
    );

    return $tabOne;
  }

  public function blockTopRatedTable($meetings = array()) {
    $tabFour_program_table = array(
      'title' => array(t('Program'), t('BU'), t('Event'), t('Evaluation'), t('Reach'),t('Rating')),  // the header should be return by function
      'value' => $this->tableTopAccreditedProgramNoHeader($meetings),
    );
    $tabFour_speaker_table = array(
      'title' => array(t('Name'), t('Events'), t('Evaluation'), t('Reach'), t('Rating')),
      'value' => $this->tableTopRatedSpeakersNoHeader($meetings),
    );

    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    $tabFour_program_content = $JsonDataBlock->table(401, t('PROGRAMS'), 'bg-ffffff', $tabFour_program_table);
    $tabFour_speaker_content = $JsonDataBlock->table(402, t('SPEAKERS'), 'bg-ffffff', $tabFour_speaker_table);

    $tabFour = array(
      $tabFour_program_content,
      $tabFour_speaker_content,
    );

    return $tabFour;
  }

  public function blockTopRatedTableOld($meetings = array()) {
    $output = array();
    $output['tableTopRatedSpeakers'] = $this->tableTopRatedSpeakers($meetings);
    $output['tableTopAccreditedProgram'] = $this->tableTopAccreditedProgram($meetings);

    return $output;
  }

  /**
   * 8 widget on page top
   */
  public function blockWidgetOne($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    $completed_meetings = $NodeQuery->meetingNidsByEventStatus('Evaluated', $meetings);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();

    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    $question_5_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 5, 64);
    $question_5_count = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set, array(1, 2, 3, 4, 5));
    $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 8, 64);
    $question_8_count = BasicClass::arrayCountValuesBySpecifyArray($question_8_answer_set, array(1, 2, 3, 4, 5));
    $question_46_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 46, 64);
    $question_2193_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2193, 64);
    $question_2193_count = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set, array(1, 2, 3, 4, 5));

    $output = array(
      $JsonDataBlock->widgetOne(001, t('Total Events'), 'bg-344a5e', count($meetings)),
      $JsonDataBlock->widgetOne(002, t('Completed Events'), 'bg-2fa9e0', count($completed_meetings)),
      $JsonDataBlock->widgetOne(003, t('Total HCP Reach'), 'bg-99dc3b', $NodeQuery->meetingNidsCountSignature($meetings)),
      $JsonDataBlock->widgetOne(004, t('Evaluation Received'), 'bg-f3c848', count($evaluations)),
      $JsonDataBlock->widgetOne(005, t('Average Program NTS'), 'bg-f34b99', BasicClass::calcNTSScore($question_46_answer_set)),
      $JsonDataBlock->widgetOne(006, t('Likely to Change Practice'), 'bg-37d8b3', BasicClass::percentage(($question_5_count[5] + $question_5_count[4]), array_sum($question_5_count)) . '%'),
      $JsonDataBlock->widgetOne(007, t('Likely to Recommend'), 'bg-5577fd', BasicClass::percentage(($question_2193_count[5] + $question_2193_count[4]), array_sum($question_5_count)) . '%'),
      $JsonDataBlock->widgetOne(008, t('Average Speaker Rating'), 'bg-4fc1ff', number_format(BasicClass::arrayAverage($question_8_answer_set), 2)),
    );

    return $output;
  }

  /**
   * 4 widget on page top on progarm page
   */
  public function blockWidgetTwo($meetings = array(), $program_tid = NULL) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    $output = array(
      $JsonDataBlock->widgetOne(001, t('Total Events'), 'bg-344a5e', count($meetings)),
      $JsonDataBlock->widgetOne(002, t('Total HCP Reach'), 'bg-2fa9e0', $NodeQuery->meetingNidsCountSignature($meetings)),
      $JsonDataBlock->widgetOne(003, t('Evaluation Received'), 'bg-99dc3b', count($evaluations)),
      $JsonDataBlock->widgetTwo(004, '<a href="' . base_path() . 'dashpage/datatable/evaluations/'. $program_tid . '" class="color-fff">' . t('Speaker Evaluations') . '</a>', 'bg-f34b99', '28'),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function blockWidgetMeeting($nid = NULL) {
    $MeetingInfo = new MeetingInfo($nid);

    $output = array(
      'class' => 'font-size-12 col-xs-12',
      'title' => array(
        'value' => $MeetingInfo->programName() . ': ' . t('Speaker Evaluation'),
        'class' => 'bg-ffffff',
      ),
      'type' => 'widget',
      'value' => array(
        array(
          'title' => t('Date') . ':',
          'value' => $MeetingInfo->dateValue(),
          'class' => 'bg-ffffff col-xs-6 col-md-3',
        ),
        array(
          'title' => t('Speaker') . ':',
          'value' => $MeetingInfo->speakerName(),
          'class' => 'bg-ffffff col-xs-6 col-md-3',
        ),
        array(
          'title' => t('City') . ':',
          'value' => $MeetingInfo->cityName(),
          'class' => 'bg-ffffff col-xs-6 col-md-3',
        ),
        array(
          'title' => t('Participants') . ':',
          'value' => $MeetingInfo->signatures(),
          'class' => 'bg-ffffff col-xs-6 col-md-3',
        ),
        array(
          'title' => t('Venue') . ':',
          'value' => $MeetingInfo->venueName(),
          'class' => 'bg-ffffff col-xs-6 col-md-3',
        ),
        array(
          'title' => t('Rep') . ':',
          'value' => $MeetingInfo->repName(),
          'class' => 'bg-ffffff col-xs-6 col-md-3',
        ),
        array(
          'title' => t('Province') . ':',
          'value' => $MeetingInfo->provinceName(),
          'class' => 'bg-ffffff col-xs-6 col-md-3',
        ),
        array(
          'title' => t('Evaluations') . ':',
          'value' => $MeetingInfo->evaluationsCount(),
          'class' => 'bg-ffffff col-xs-6 col-md-3',
        ),
      ),
    );

    return $output;
  }

  /**
   * block Program Snapshot Chart
   * block Meeting Snapshot Chart
   */
  public function blockProgramChart($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_5_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 5, 64);
    $question_5_count = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set, array(1, 2, 3, 4, 5));

    $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 8, 64);
    $question_8_count = BasicClass::arrayCountValuesBySpecifyArray($question_8_answer_set, array(1, 2, 3, 4, 5));

    $question_46_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 46, 64);
    $question_46_count = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set, array(1, 2, 3, 4, 5));

    $question_2193_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2193, 64);
    $question_2193_count = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set, array(1, 2, 3, 4, 5));

    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    // $chart Section
    $chartSection_header_bg_color = 'bg-ffffff color-000000';
    $chartSection_block_class = 'col-md-6 col-xs-12';

    $chartSection_one_value = array(
      'datasets' => array(
        array(
          'value' => $question_46_count[5],
          'title' => '5 (' . $question_46_count[5] . ')',          // chart legend
        ),
        array(
          'value' => $question_46_count[4],
          'title' => '4 (' . $question_46_count[4] . ')',
        ),
        array(
          'value' => $question_46_count[3],
          'title' => '3 (' . $question_46_count[3] . ')',
        ),
        array(
          'value' => $question_46_count[2],
          'title' => '2 (' . $question_46_count[2] . ')',
        ),
        array(
          'value' => $question_46_count[1],
          'title' => '1 (' . $question_46_count[1] . ')',
        ),
      ),
      'title' => '',              // chart caption
    );
    $chartSection_one_bottom_value = $JsonDataBlock->sectionOne(
      array(
        'titleOne' => t('RESPONSES'),
        'titleTwo' => t('PROGRAM NTS'),
        'valueOne' => count($question_46_answer_set),
        'valueTwo' => BasicClass::calcNTSScore($question_46_answer_set),
      )
    );

    $chartSection_one_chart = $JsonDataBlock->chartPieNew(201, t('Overall Satisfaction'), $chartSection_header_bg_color, $chartSection_one_value, $chartSection_one_bottom_value, $chartSection_block_class);

    $chartSection_two_value = array(
      'datasets' => array(
        array(
          'value' => $question_5_count[5],
          'title' => '5 (' . $question_5_count[5] . ')',          // chart legend
        ),
        array(
          'value' => $question_5_count[4],
          'title' => '4 (' . $question_5_count[4] . ')',
        ),
        array(
          'value' => $question_5_count[3],
          'title' => '3 (' . $question_5_count[3] . ')',
        ),
        array(
          'value' => $question_5_count[2],
          'title' => '2 (' . $question_5_count[2] . ')',
        ),
        array(
          'value' => $question_5_count[1],
          'title' => '1 (' . $question_5_count[1] . ')',
        ),
      ),
      'title' => '',              // chart caption
    );
    $chartSection_two_bottom_value = $JsonDataBlock->sectionOne(
      array(
        'titleOne' => t('RESPONSES'),
        'titleTwo' => t('LIKELY OR VERY LIKELY'),
        'valueOne' => count($question_5_answer_set),
        'valueTwo' => BasicClass::percentage(($question_5_count[5] + $question_5_count[4]), array_sum($question_5_count)) . '% ',
      )
    );
    $chartSection_two_chart = $JsonDataBlock->chartPieNew(202, t('Likelihood of Changing Clinical Practice'), $chartSection_header_bg_color, $chartSection_two_value, $chartSection_two_bottom_value, $chartSection_block_class);

    $chartSection_three_value = array(
      'datasets' => array(
        array(
          'value' => $question_8_count[5],
          'title' => '5 (' . $question_8_count[5] . ')',          // chart legend
        ),
        array(
          'value' => $question_8_count[4],
          'title' => '4 (' . $question_8_count[4] . ')',
        ),
        array(
          'value' => $question_8_count[3],
          'title' => '3 (' . $question_8_count[3] . ')',
        ),
        array(
          'value' => $question_8_count[2],
          'title' => '2 (' . $question_8_count[2] . ')',
        ),
        array(
          'value' => $question_8_count[1],
          'title' => '1 (' . $question_8_count[1] . ')',
        ),
      ),
      'title' => '',              // chart caption
    );
    $chartSection_three_bottom_value = $JsonDataBlock->sectionOne(
      array(
        'titleOne' => t('RESPONSES'),
        'titleTwo' => t('EFFECTIVE OR VERY EFFECTIVE'),
        'valueOne' => count($question_8_answer_set),
        'valueTwo' => BasicClass::percentage(($question_8_count[5] + $question_8_count[4]), array_sum($question_8_count)) . '% ',
      )
    );
    $chartSection_three_chart = $JsonDataBlock->chartDoughnutNew(203, t('Speaker Effectiveness'), $chartSection_header_bg_color, $chartSection_three_value, $chartSection_three_bottom_value, $chartSection_block_class);

    $chartSection_four_value = array(
      'datasets' => array(
        array(
          'value' => $question_2193_count[5],
          'title' => '5 (' . $question_2193_count[5] . ')',          // chart legend
        ),
        array(
          'value' => $question_2193_count[4],
          'title' => '4 (' . $question_2193_count[4] . ')',
        ),
        array(
          'value' => $question_2193_count[3],
          'title' => '3 (' . $question_2193_count[3] . ')',
        ),
        array(
          'value' => $question_2193_count[2],
          'title' => '2 (' . $question_2193_count[2] . ')',
        ),
        array(
          'value' => $question_2193_count[1],
          'title' => '1 (' . $question_2193_count[1] . ')',
        ),
      ),
      'title' => '',              // chart caption
    );
    $chartSection_four_bottom_value = $JsonDataBlock->sectionOne(
      array(
        'titleOne' => t('RESPONSES'),
        'titleTwo' => t('LIKELY OR VERY LIKELY'),
        'valueOne' => count($question_2193_answer_set),
        'valueTwo' => BasicClass::percentage(($question_2193_count[5] + $question_2193_count[4]), array_sum($question_2193_count)) . '% ',
      )
    );
    $chartSection_four_chart = $JsonDataBlock->chartDoughnutNew(204, t('Likelihood of Recommending Program'), $chartSection_header_bg_color, $chartSection_four_value, $chartSection_four_bottom_value, $chartSection_block_class);

    // Speaker Bias
    $question_2304_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2304, 64);
    $question_2304_count = BasicClass::arrayCountValuesBySpecifyArray($question_2304_answer_set, array(0, 1));
    $chartSection_five_value = array(
      'datasets' => array(
        array(
          'value' => $question_2304_count[0],
          'title' => "Yes",          // chart legend
        ),
        array(
          'value' => $question_2304_count[1],
          'title' => "No",
        ),
      ),
      'crossTextValue' => BasicClass::percentage(($question_2304_count[0]), array_sum($question_2304_count)) . '%',              // chart value
      'title' => '',              // chart caption
    );
    $chartSection_five_bottom_value = $JsonDataBlock->sectionOne(
      array(
        'titleOne' => t('RESPONSES'),
        'titleTwo' => t('NO SPEAKER BIAS'),
        'valueOne' => count($question_2304_answer_set),
        'valueTwo' => BasicClass::percentage(($question_2304_count[0]), array_sum($question_2304_count)) . '% ',
      )
    );
    $chartSection_five_chart = $JsonDataBlock->chartDoughnutGauge(205, t('Speaker Bias'), $chartSection_header_bg_color, $chartSection_five_value, $chartSection_five_bottom_value, $chartSection_block_class);

    // Commercial Bias
    $question_2131_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2131, 64);
    $question_2131_count = BasicClass::arrayCountValuesBySpecifyArray($question_2131_answer_set, array(0, 1));
    $chartSection_six_value = array(
      'datasets' => array(
        array(
          'value' => $question_2131_count[0],
          'title' => "Yes",          // chart legend
        ),
        array(
          'value' => $question_2131_count[1],
          'title' => "No",
        ),
      ),
      'crossTextValue' => BasicClass::percentage(($question_2131_count[0]), array_sum($question_2131_count)) . '%',              // chart value
      'title' => '',              // chart caption
    );
    $chartSection_six_bottom_value = $JsonDataBlock->sectionOne(
      array(
        'titleOne' => t('RESPONSES'),
        'titleTwo' => t('NO COMMERCIAL BIAS'),
        'valueOne' => count($question_2131_answer_set),
        'valueTwo' => BasicClass::percentage(($question_2131_count[0]), array_sum($question_2131_count)) . '% ',
      )
    );
    $chartSection_six_chart = $JsonDataBlock->chartDoughnutGauge(206, t('Commercial Bias'), $chartSection_header_bg_color, $chartSection_six_value, $chartSection_six_bottom_value, $chartSection_block_class);

    /** - - - - - Learning Objectives - - - - - - - - - -  - - - - -   */

    // base on program Learning Objective
    $MeetingInfo = new MeetingInfo(current($meetings));
    $program_tid = $MeetingInfo->programTid();

    $TermProgramInfo = new TermProgramInfo($program_tid);
    $TermEvaluationFormInfo = new TermEvaluationFormInfo($TermProgramInfo->evaluationFormTid());

    $learning_objective_question_tids = array();
    if (is_array($TermEvaluationFormInfo->questionSetArray())) {
      foreach ($TermEvaluationFormInfo->questionSetArray() as $row) {
        $TermQuestionLibraryInfo = new TermQuestionLibraryInfo($row['quesset_question_tid']);
        if ($TermQuestionLibraryInfo->questionTypeTid() == 2213) {  // 2213 is Learning Objective
          $learning_objective_question_tids[] = $row['quesset_question_tid'];
        }
      }
    }

    // chart column
    // chart
    $containerTwo_block_class = 'col-md-4 col-xs-12';
    $containerTwo_block_chart_options['barValueSpacing'] = 2;

    if (count($learning_objective_question_tids) > 4) {
      $learning_objective_question_tids = array_slice($learning_objective_question_tids, 0, 5);
    }
    if (is_array($learning_objective_question_tids)) {
      switch (count($learning_objective_question_tids)) {
        case 2:
          $containerTwo_block_class = 'col-md-6 col-xs-12';
          break;
        case 4:
          $containerTwo_block_class = 'col-md-3 col-xs-12';
          break;
        case 5:
          $containerTwo_block_class = 'col-md-custom-20-p col-xs-12';
          break;

        default:
          break;
      }
    }

    $learning_objective_chart = NULL;
    if (is_array($learning_objective_question_tids)) {
      foreach ($learning_objective_question_tids as $question_tid) {
        $TermQuestionLibraryInfo = new TermQuestionLibraryInfo($question_tid);

        // Evaluation
        // $EvaluationFieldQuery = new EvaluationFieldQuery();
        $learning_question_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $question_tid, 64);
        $learning_question_count = BasicClass::arrayCountValuesBySpecifyArray($learning_question_answer_set, array(1, 2, 3, 4, 5));

        $bar_chart_value = array(
          'labels' => array("5", "4", "3", "2", "1"),
          'title' => $TermQuestionLibraryInfo->termName(),
          'chartTitle' => $TermQuestionLibraryInfo->termName(),
          'datasets' => array(
            array(
              "fillColor" => ColorClass::plate2Array(),
              "strokeColor" => ColorClass::plate2Array(),
              "pointColor" => "#05d23e",
              "pointStrokeColor" => "#fff",
              "data" => array(
                BasicClass::percentage($learning_question_count[5], array_sum($learning_question_count)),
                BasicClass::percentage($learning_question_count[4], array_sum($learning_question_count)),
                BasicClass::percentage($learning_question_count[3], array_sum($learning_question_count)),
                BasicClass::percentage($learning_question_count[2], array_sum($learning_question_count)),
                BasicClass::percentage($learning_question_count[1], array_sum($learning_question_count)),
              ),
              "title"=> $TermQuestionLibraryInfo->termName(),
            ),
          ),
        );

        $chart_id = 300 . $question_tid;
        $learning_objective_chart[] = $JsonDataBlock->chartBarByPercentage($chart_id , NULL, 'bg-ffffff', $bar_chart_value, $containerTwo_block_chart_options, $containerTwo_block_class);
      }
    }

    $containerTwo = $learning_objective_chart;
    $containerTwo_bottom_value = array(
      'legend' => array(
        array(
          'title' => t('Strongly Agree'),
          'color' => '#00a9e0',
        ),
        array(
          'title' => t('Agree'),
          'color' => '#05d23e',
        ),
        array(
          'title' => t('Neutral'),
          'color' => '#f24b99',
        ),
        array(
          'title' => t('Disagree'),
          'color' => '#bfbfbf',
        ),
        array(
          'title' => t('Strongly Disagree'),
          'color' => '#e6e6e6',
        ),
      ),
    );

    /** - - - - - Learning Objectives end - - - - - - - - - -  - - - - -   */

    // block
    $tabOne = $this->blockTotalEventDistribution($meetings);

    // output
    $output = NULL;
    if (arg(2) == 'program') {
      $output[] = $JsonDataBlock->tabOne(22, t('Total Event Distribution'), 'bg-344a5f', $tabOne);
    }
    $output[] = $chartSection_one_chart;
    $output[] = $chartSection_two_chart;
    $output[] = $chartSection_three_chart;
    $output[] = $chartSection_four_chart;
    $output[] = $JsonDataBlock->multiContainer(24, t('Learning Objectives'), 'bg-344a5f', $containerTwo, $containerTwo_bottom_value);
    $output[] = $chartSection_five_chart;
    $output[] = $chartSection_six_chart;

    return $output;
  }

  /** - - - - - Page Json block end - - - - - - - - - - - - - - - - - - - - - -   */

  /**
   * @return Term Array data
   */
  public function pageLanding($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    $meetings_unit_72 = $NodeQuery->meetingNidsByBusinessUnitTid(72, $meetings);             // Diabetes
    $evaluations_unit_72 = $NodeQuery->evaluationNidsByMeeting($meetings_unit_72);
    $meetings_unit_2088 = $NodeQuery->meetingNidsByBusinessUnitTid(2088, $meetings);         // Biomedicines
    $evaluations_unit_2088 = $NodeQuery->evaluationNidsByMeeting($meetings_unit_2088);
    $meetings_unit_2090 = $NodeQuery->meetingNidsByBusinessUnitTid(2090, $meetings);         // Oncology
    $evaluations_unit_2090 = $NodeQuery->evaluationNidsByMeeting($meetings_unit_2090);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();

    $question_5_answer = $EvaluationFieldQuery->questionFieldValues($evaluations, 5, 64);
    $question_5_count = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer, array(1, 2, 3, 4, 5));
    $question_46_answer = $EvaluationFieldQuery->questionFieldValues($evaluations, 46, 64);
    $question_46_count = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer, array(1, 2, 3, 4, 5));
    $question_2193_answer = $EvaluationFieldQuery->questionFieldValues($evaluations, 2193, 64);
    $question_2193_count = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer, array(1, 2, 3, 4, 5));

    $question_5_answer_set_unit_72 = $EvaluationFieldQuery->questionFieldValues($evaluations_unit_72, 5, 64);
    $question_5_count_unit_72 = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set_unit_72, array(1, 2, 3, 4, 5));
    $question_46_answer_set_unit_72 = $EvaluationFieldQuery->questionFieldValues($evaluations_unit_72, 46, 64);
    $question_46_count_unit_72 = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set_unit_72, array(1, 2, 3, 4, 5));
    $question_2193_answer_set_unit_72 = $EvaluationFieldQuery->questionFieldValues($evaluations_unit_72, 2193, 64);
    $question_2193_count_unit_72 = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set_unit_72, array(1, 2, 3, 4, 5));

    $question_5_answer_set_unit_2088 = $EvaluationFieldQuery->questionFieldValues($evaluations_unit_2088, 5, 64);
    $question_5_count_unit_2088 = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set_unit_2088, array(1, 2, 3, 4, 5));
    $question_46_answer_set_unit_2088 = $EvaluationFieldQuery->questionFieldValues($evaluations_unit_2088, 46, 64);
    $question_46_count_unit_2088 = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set_unit_2088, array(1, 2, 3, 4, 5));
    $question_2193_answer_set_unit_2088 = $EvaluationFieldQuery->questionFieldValues($evaluations_unit_2088, 2193, 64);
    $question_2193_count_unit_2088 = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set_unit_2088, array(1, 2, 3, 4, 5));

    $question_5_answer_set_unit_2090 = $EvaluationFieldQuery->questionFieldValues($evaluations_unit_2090, 5, 64);
    $question_5_count_unit_2090 = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set_unit_2090, array(1, 2, 3, 4, 5));
    $question_46_answer_set_unit_2090 = $EvaluationFieldQuery->questionFieldValues($evaluations_unit_2090, 46, 64);
    $question_46_count_unit_2090 = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set_unit_2090, array(1, 2, 3, 4, 5));
    $question_2193_answer_set_unit_2090 = $EvaluationFieldQuery->questionFieldValues($evaluations_unit_2090, 2193, 64);
    $question_2193_count_unit_2090 = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set_unit_2090, array(1, 2, 3, 4, 5));

    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    // tab two - - Business Unit Event Summary -- on home page
    $tabTwo_total_events_value = array(
      "labels" => array(t("Oncology"), t("Diabetes"), t("Biomedicines")),
      "datasets" => array(
        array(
          "fillColor" => ColorClass::plate(5),
          "strokeColor" => ColorClass::plate(5),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            count($meetings_unit_2090),           // Oncology
            count($meetings_unit_72),             // Diabetes
            count($meetings_unit_2088),           // Biomedicines
          ),
          "title"=> t("All Events"),
        ),
        array(
          "fillColor" => ColorClass::plate(4),
          "strokeColor" => ColorClass::plate(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            count($meetings_unit_2090),           // Oncology
            count($meetings_unit_72),             // Diabetes
            count($meetings_unit_2088),           // Biomedicines
          ),
          "title"=> t("HEM"),
        ),
        array(
          "fillColor" => ColorClass::plate(3),
          "strokeColor" => ColorClass::plate(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            rand(0, 0),
            rand(0, 0),
            rand(0, 0),
          ),
          "title"=> t('Ind. Sponsorships'),
        ),
        array(
          "fillColor" => ColorClass::plate(2),
          "strokeColor" => ColorClass::plate(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            rand(0, 0),
            rand(0, 0),
            rand(0, 0),
          ),
          "title"=> t('Ad Boards'),
        ),
        array(
          "fillColor" => ColorClass::plate(1),
          "strokeColor" => ColorClass::plate(1),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            rand(0, 0),
            rand(0, 0),
            rand(0, 0),
          ),
          "title"=> t('Scientific Exchange'),
        ),
      )
    );
    $tabTwo_total_events_chart = $JsonDataBlock->chartHorizontalBar(201, t('TOTAL EVENTS'), 'bg-ffffff', $tabTwo_total_events_value);

    $tabTwo_hcp_reach_value = array(
      "labels" => array(t('Oncology'), t("Diabetes"), t("Biomedicines"), t("All BU's")),
      'datasets' => array(
        array(
          "fillColor" => "#344a5e",
          "strokeColor" => "#344a5e",
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $NodeQuery->meetingNidsCountSignature($NodeQuery->meetingNidsByBusinessUnitTid(2090, $meetings)),  // Oncology
            $NodeQuery->meetingNidsCountSignature($NodeQuery->meetingNidsByBusinessUnitTid(72, $meetings)),    // Diabetes
            $NodeQuery->meetingNidsCountSignature($NodeQuery->meetingNidsByBusinessUnitTid(2088, $meetings)),  // Biomedicines
            $NodeQuery->meetingNidsCountSignature($meetings),   // All BU's
          ),
          "title"=> '',
        ),
        // meeting type/event type
      ),
    );
    $tabTwo_hcp_reach_chart = $JsonDataBlock->chartHorizontalBar(202, t('HCP REACH'), 'bg-ffffff', $tabTwo_hcp_reach_value);

    $tabTwo_event_reach_value = array(
      "labels" => array(t("All"), t("Biomedicines"), t("Diabetes"), t("Oncology")),
      "title"=> '',
      'datasets' => array(
        array(
          "fillColor" => ColorClass::plate(5),
          "strokeColor" => ColorClass::plate(5),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            count($meetings),   // All BU's
            count($NodeQuery->meetingNidsByBusinessUnitTid(2088, $meetings)),  // Biomedicines
            count($NodeQuery->meetingNidsByBusinessUnitTid(72, $meetings)),    // Diabetes
            count($NodeQuery->meetingNidsByBusinessUnitTid(2090, $meetings)),  // Oncology
          ),
          "title"=> t('All Events'),
        ),
        array(
          "fillColor" => ColorClass::plate(4),
          "strokeColor" => ColorClass::plate(4),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            count($meetings),   // All BU's
            count($NodeQuery->meetingNidsByBusinessUnitTid(2088, $meetings)),  // Biomedicines
            count($NodeQuery->meetingNidsByBusinessUnitTid(72, $meetings)),    // Diabetes
            count($NodeQuery->meetingNidsByBusinessUnitTid(2090, $meetings)),  // Oncology
          ),
          "title"=> t('Health Education'),
        ),
        array(
          "fillColor" => ColorClass::plate(2),
          "strokeColor" => ColorClass::plate(2),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            rand(0, 20),  // All BU's
            rand(0, 20),  // Biomedicines
            rand(0, 20),  // Diabetes
            rand(0, 20),  // Oncology
          ),
          "title"=> t('Ad Boards'),
        ),
        array(
          "fillColor" => ColorClass::plate(3),
          "strokeColor" => ColorClass::plate(3),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            rand(0, 20),  // All BU's
            rand(0, 20),  // Biomedicines
            rand(0, 20),  // Diabetes
            rand(0, 20),  // Oncology
          ),
          "title"=> t('Ind. Sponsorships'),
        ),
        array(
          "fillColor" => ColorClass::plate(1),
          "strokeColor" => ColorClass::plate(1),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            rand(0, 20),  // All BU's
            rand(0, 20),  // Biomedicines
            rand(0, 20),  // Diabetes
            rand(0, 20),  // Oncology
          ),
          "title"=> t('Scientific Exchange'),
        ),

      ),
    );
    $tabTwo_event_reach_chart_options['barValueSpacing'] = 40;
    $tabTwo_event_reach_chart = $JsonDataBlock->chartBar(203, t('EVENT REACH'), 'bg-ffffff', $tabTwo_event_reach_value, $tabTwo_event_reach_chart_options);

    $tabTwo = array(
      $tabTwo_total_events_chart,
      $tabTwo_hcp_reach_chart,
      $tabTwo_event_reach_chart,
    );

    // tab three - - Key Touchpoints
    $tabThree_one_value = array(
      "labels" => array(t("Oncology"), t("Diabetes"), t('Biomedicines')),
      'title' => "",
      "datasets" => array(
        array(
          "fillColor" => ColorClass::plate(1),
          "strokeColor" => ColorClass::plate(1),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_unit_2090[1],           // Oncology
            $question_46_count_unit_72[1],             // Diabetes
            $question_46_count_unit_2088[1],           // Biomedicines
          ),
          "title" => '1 (' . $question_46_count[1] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(2),
          "strokeColor" => ColorClass::plate(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_unit_2090[2],           // Oncology
            $question_46_count_unit_72[2],             // Diabetes
            $question_46_count_unit_2088[2],           // Biomedicines
          ),
          "title" => '2 (' . $question_46_count[2] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(3),
          "strokeColor" => ColorClass::plate(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_unit_2090[3],           // Oncology
            $question_46_count_unit_72[3],             // Diabetes
            $question_46_count_unit_2088[3],           // Biomedicines
          ),
          "title" => '3 (' . $question_46_count[3] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(4),
          "strokeColor" => ColorClass::plate(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_unit_2090[4],           // Oncology
            $question_46_count_unit_72[4],             // Diabetes
            $question_46_count_unit_2088[4],           // Biomedicines
          ),
          "title" => '4 (' . $question_46_count[4] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(5),
          "strokeColor" => ColorClass::plate(5),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_unit_2090[5],           // Oncology
            $question_46_count_unit_72[5],             // Diabetes
            $question_46_count_unit_2088[5],           // Biomedicines
          ),
          "title" => '5 (' . $question_46_count[5] . ')',
        ),
      )
    );
    $tabThree_one_chart = $JsonDataBlock->chartHorizontalStackedBar(301, t('OVERALL SATISFACTION'), 'bg-ffffff', $tabThree_one_value);

    $tabThree_two_value = array(
      'labels' => array(t("Biomedicines"), t("Diabetes"), t('Oncology')),
      'title' => "",
      'datasets' => array(
        array(
          "fillColor" => "#344a5e",
          "strokeColor" => "#344a5e",
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::calcNTSScore($question_46_answer_set_unit_2088, FALSE),  // Biomedicines
            BasicClass::calcNTSScore($question_46_answer_set_unit_72, FALSE),  // Diabetes
            BasicClass::calcNTSScore($question_46_answer_set_unit_2090, FALSE),  // Oncology
          ),
          "title"=> '',
        ),
      ),
    );
    $tabThree_chartBar_chart_options['barValueSpacing'] = 120;
    $tabThree_two_chart = $JsonDataBlock->chartHorizontalBarCustomInGraph(302, t('NTS'), 'bg-ffffff', $tabThree_two_value, $tabThree_chartBar_chart_options);

    $tabThree_three_value = array(
      'labels' => array(t("Biomedicines"), t("Diabetes"), t("Oncology")),
      'title' => "",
      'chartTitle' => "",
      "datasets" => array(
        array(
          "fillColor" => ColorClass::plate2(5),
          "strokeColor" => ColorClass::plate2(5),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_unit_2088[5], array_sum($question_5_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_5_count_unit_72[5], array_sum($question_5_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_5_count_unit_2090[5], array_sum($question_5_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Very Likely') . ' (' . $question_5_count[5] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate2(4),
          "strokeColor" => ColorClass::plate2(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_unit_2088[4], array_sum($question_5_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_5_count_unit_72[4], array_sum($question_5_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_5_count_unit_2090[4], array_sum($question_5_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Likely') . ' (' . $question_5_count[4] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate2(3),
          "strokeColor" => ColorClass::plate2(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_unit_2088[3], array_sum($question_5_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_5_count_unit_72[3], array_sum($question_5_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_5_count_unit_2090[3], array_sum($question_5_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Neutral') . ' (' . $question_5_count[3] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate2(2),
          "strokeColor" => ColorClass::plate2(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_unit_2088[2], array_sum($question_5_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_5_count_unit_72[2], array_sum($question_5_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_5_count_unit_2090[2], array_sum($question_5_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Unlikely') . ' (' . $question_5_count[2] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate2(1),
          "strokeColor" => ColorClass::plate2(1),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_unit_2088[1], array_sum($question_5_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_5_count_unit_72[1], array_sum($question_5_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_5_count_unit_2090[1], array_sum($question_5_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Very Unlikely') . ' (' . $question_5_count[1] . ')',
        ),
      )
    );
    $tabThree_chartBar_chart_options['barValueSpacing'] = 60;
    $tabThree_three_chart = $JsonDataBlock->chartBarByPercentage(303, t('CLINICAL CHANGE'), 'bg-ffffff', $tabThree_three_value, $tabThree_chartBar_chart_options);

    $tabThree_four_value = array(
      'labels' => array(t("Biomedicines"), t("Diabetes"), t("Oncology")),
      'title' => "",
      'chartTitle' => "",
      "datasets" => array(
        array(
          "fillColor" => ColorClass::plate2(5),
          "strokeColor" => ColorClass::plate2(5),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_unit_2088[5], array_sum($question_2193_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_2193_count_unit_72[5], array_sum($question_2193_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_2193_count_unit_2090[5], array_sum($question_2193_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Very Likely') . ' (' . $question_2193_count[5] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate2(4),
          "strokeColor" => ColorClass::plate2(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_unit_2088[4], array_sum($question_2193_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_2193_count_unit_72[4], array_sum($question_2193_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_2193_count_unit_2090[4], array_sum($question_2193_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Likely') . ' (' . $question_2193_count[4] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate2(3),
          "strokeColor" => ColorClass::plate2(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_unit_2088[3], array_sum($question_2193_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_2193_count_unit_72[3], array_sum($question_2193_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_2193_count_unit_2090[3], array_sum($question_2193_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Neutral') . ' (' . $question_2193_count[3] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate2(2),
          "strokeColor" => ColorClass::plate2(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_unit_2088[2], array_sum($question_2193_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_2193_count_unit_72[2], array_sum($question_2193_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_2193_count_unit_2090[2], array_sum($question_2193_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Unlikely') . ' (' . $question_2193_count[2] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate2(1),
          "strokeColor" => ColorClass::plate2(1),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_unit_2088[1], array_sum($question_2193_count_unit_2088)),  // Biomedicines
            BasicClass::percentage($question_2193_count_unit_72[1], array_sum($question_2193_count_unit_72)),      // Diabetes
            BasicClass::percentage($question_2193_count_unit_2090[1], array_sum($question_2193_count_unit_2090)),  // Oncology
          ),
          "title"=> t('Very Unlikely') . ' (' . $question_2193_count[1] . ')',
        ),
      )
    );
    $tabThree_four_chart = $JsonDataBlock->chartBarByPercentage(304, t('RECOMMEND PROGRAM'), 'bg-ffffff', $tabThree_four_value, $tabThree_chartBar_chart_options);

    $tabThree = array(
      $tabThree_two_chart,   // NTS
      $tabThree_one_chart,
      $tabThree_three_chart,
      $tabThree_four_chart,
    );

    // block
    $tabOne = $this->blockTotalEventDistribution($meetings);
    $tabFour = $this->blockTopRatedTableOld($meetings);

    // output
    $output = NULL;
    $output['fixedSection'] = $this->blockWidgetOne($meetings);

    $output['contentSection'] = array(
      $JsonDataBlock->tabOne(22, t('Total Event Distribution (All Business Units)'), 'bg-344a5f', $tabOne),
      $JsonDataBlock->tabOne(23, t('Business Unit Event Summary'), 'bg-344a5f', $tabTwo),
      $JsonDataBlock->tabOne(24, t('Key Touchpoints'), 'bg-344a5f', $tabThree),
      $JsonDataBlock->tabTwo(25, t('Top Rated'), 'bg-344a5f', $tabFour),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function businessUnitSnapshotBiomedicines($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // meeting by therapeutic area
    $meetings_area_2291 = $NodeQuery->meetingNidsByTherapeuticArea(2291, $meetings);             // Osteoporosis
    $evaluations_area_2291 = $NodeQuery->evaluationNidsByMeeting($meetings_area_2291);

    $meetings_area_2095 = $NodeQuery->meetingNidsByTherapeuticArea(2095, $meetings);             // Men’s Health
    $evaluations_area_2095 = $NodeQuery->evaluationNidsByMeeting($meetings_area_2095);

    $meetings_area_2092 = $NodeQuery->meetingNidsByTherapeuticArea(2092, $meetings);             // Neuroscience
    $evaluations_area_2092 = $NodeQuery->evaluationNidsByMeeting($meetings_area_2092);

    $meetings_area_2094 = $NodeQuery->meetingNidsByTherapeuticArea(2094, $meetings);             // Rheumatology
    $evaluations_area_2094 = $NodeQuery->evaluationNidsByMeeting($meetings_area_2094);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();

    $question_46_answer = $EvaluationFieldQuery->questionFieldValues($evaluations, 46, 64);
    $question_46_count = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer, array(1, 2, 3, 4, 5));

    // Osteoporosis
    $question_5_answer_set_area_2291 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2291, 5, 64);
    $question_5_count_area_2291 = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set_area_2291, array(1, 2, 3, 4, 5));
    $question_46_answer_set_area_2291 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2291, 46, 64);
    $question_46_count_area_2291 = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set_area_2291, array(1, 2, 3, 4, 5));
    $question_2193_answer_set_area_2291 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2291, 2193, 64);
    $question_2193_count_area_2291 = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set_area_2291, array(1, 2, 3, 4, 5));

    // Men’s Health
    $question_5_answer_set_area_2095 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2095, 5, 64);
    $question_5_count_area_2095 = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set_area_2095, array(1, 2, 3, 4, 5));
    $question_46_answer_set_area_2095 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2095, 46, 64);
    $question_46_count_area_2095 = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set_area_2095, array(1, 2, 3, 4, 5));
    $question_2193_answer_set_area_2095 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2095, 2193, 64);
    $question_2193_count_area_2095 = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set_area_2095, array(1, 2, 3, 4, 5));

    // Neuroscience
    $question_5_answer_set_area_2092 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2092, 5, 64);
    $question_5_count_area_2092 = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set_area_2092, array(1, 2, 3, 4, 5));
    $question_46_answer_set_area_2092 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2092, 46, 64);
    $question_46_count_area_2092 = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set_area_2092, array(1, 2, 3, 4, 5));
    $question_2193_answer_set_area_2092 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2092, 2193, 64);
    $question_2193_count_area_2092 = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set_area_2092, array(1, 2, 3, 4, 5));

    // Rheumatology
    $question_5_answer_set_area_2094 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2094, 5, 64);
    $question_5_count_area_2094 = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set_area_2094, array(1, 2, 3, 4, 5));
    $question_46_answer_set_area_2094 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2094, 46, 64);
    $question_46_count_area_2094 = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set_area_2094, array(1, 2, 3, 4, 5));
    $question_2193_answer_set_area_2094 = $EvaluationFieldQuery->questionFieldValues($evaluations_area_2094, 2193, 64);
    $question_2193_count_area_2094 = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set_area_2094, array(1, 2, 3, 4, 5));

    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    // tab three - - Key Touchpoints
    $tabThree_one_value = array(
      'labels' => array(t("Rheumatology"), t("Neuroscience"), t("Men’s Health"), t("Osteoporosis")),
      'title' => "",
      'datasets' => array(
        array(
          "fillColor" => "#344a5e",
          "strokeColor" => "#344a5e",
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::calcNTSScore($question_46_answer_set_area_2094, FALSE),  // Rheumatology
            BasicClass::calcNTSScore($question_46_answer_set_area_2092, FALSE),  // Neuroscience
            BasicClass::calcNTSScore($question_46_answer_set_area_2095, FALSE),  // Men’s Health
            BasicClass::calcNTSScore($question_46_answer_set_area_2291, FALSE),  // Osteoporosis
          ),
          "title"=> '',
        ),
      ),
    );
    $tabThree_chartBar_chart_options['barValueSpacing'] = 80;
    $tabThree_one_chart = $JsonDataBlock->chartHorizontalBarCustomInGraph(301, t('NTS'), 'bg-ffffff', $tabThree_one_value, $tabThree_chartBar_chart_options);

    $tabThree_two_value = array(
      "labels" => array(t("Rheumatology"), t("Neuroscience"), t("Men’s Health"), t("Osteoporosis")),
      "datasets" => array(
        array(
          "fillColor" => ColorClass::plate(1),
          "strokeColor" => ColorClass::plate(1),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_area_2094[1],   // Rheumatology
            $question_46_count_area_2092[1],   // Neuroscience
            $question_46_count_area_2095[1],   // Men’s Health
            $question_46_count_area_2291[1],   // Osteoporosis
          ),
          "title" => '1 (' . $question_46_count[1] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(2),
          "strokeColor" => ColorClass::plate(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_area_2094[2],   // Rheumatology
            $question_46_count_area_2092[2],   // Neuroscience
            $question_46_count_area_2095[2],   // Men’s Health
            $question_46_count_area_2291[2],   // Osteoporosis
          ),
          "title" => '2 (' . $question_46_count[2] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(3),
          "strokeColor" => ColorClass::plate(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_area_2094[3],   // Rheumatology
            $question_46_count_area_2092[3],   // Neuroscience
            $question_46_count_area_2095[3],   // Men’s Health
            $question_46_count_area_2291[3],   // Osteoporosis
          ),
          "title" => '3 (' . $question_46_count[3] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(4),
          "strokeColor" => ColorClass::plate(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_area_2094[4],   // Rheumatology
            $question_46_count_area_2092[4],   // Neuroscience
            $question_46_count_area_2095[4],   // Men’s Health
            $question_46_count_area_2291[4],   // Osteoporosis
          ),
          "title" => '4 (' . $question_46_count[4] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(5),
          "strokeColor" => ColorClass::plate(5),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_area_2094[5],   // Rheumatology
            $question_46_count_area_2092[5],   // Neuroscience
            $question_46_count_area_2095[5],   // Men’s Health
            $question_46_count_area_2291[5],   // Osteoporosis
          ),
          "title" => '5 (' . $question_46_count[5] . ')',
        ),
      )
    );
    $tabThree_two_chart_options['barValueSpacing'] = 22;
    $tabThree_two_chart = $JsonDataBlock->chartHorizontalStackedBar(302, t('OVERALL SATISFACTION'), 'bg-ffffff', $tabThree_two_value, $tabThree_two_chart_options);

    $tabThree_three_value = array(
      'labels' => array(t("Osteoporosis"), t("Men’s Health"), t("Neuroscience"), t("Rheumatology")),
      'title' => "",
      'chartTitle' => "",
      'datasets' => array(
        array(
          "fillColor" => ColorClass::plate2(5),
          "strokeColor" => ColorClass::plate2(5),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_5_count_area_2291[5], array_sum($question_5_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_5_count_area_2095[5], array_sum($question_5_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_5_count_area_2092[5], array_sum($question_5_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_5_count_area_2094[5], array_sum($question_5_count_area_2094)),
          ),
          "title"=> t("Very Likely"),
        ),
        array(
          "fillColor" => ColorClass::plate2(4),
          "strokeColor" => ColorClass::plate2(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_5_count_area_2291[4], array_sum($question_5_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_5_count_area_2095[4], array_sum($question_5_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_5_count_area_2092[4], array_sum($question_5_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_5_count_area_2094[4], array_sum($question_5_count_area_2094)),
          ),
          "title"=> t('Likely'),
        ),
        array(
          "fillColor" => ColorClass::plate2(3),
          "strokeColor" => ColorClass::plate2(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_5_count_area_2291[3], array_sum($question_5_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_5_count_area_2095[3], array_sum($question_5_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_5_count_area_2092[3], array_sum($question_5_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_5_count_area_2094[3], array_sum($question_5_count_area_2094)),
          ),
          "title"=> t('Neutral'),
        ),
        array(
          "fillColor" => ColorClass::plate2(2),
          "strokeColor" => ColorClass::plate2(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_5_count_area_2291[2], array_sum($question_5_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_5_count_area_2095[2], array_sum($question_5_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_5_count_area_2092[2], array_sum($question_5_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_5_count_area_2094[2], array_sum($question_5_count_area_2094)),
          ),
          "title"=> t('Unlikely'),
        ),
        array(
          "fillColor" => ColorClass::plate2(1),
          "strokeColor" => ColorClass::plate2(1),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_5_count_area_2291[1], array_sum($question_5_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_5_count_area_2095[1], array_sum($question_5_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_5_count_area_2092[1], array_sum($question_5_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_5_count_area_2094[1], array_sum($question_5_count_area_2094)),
          ),
          "title"=> t('Very Unlikely'),
        ),
      ),
    );
    $tabThree_three_chart_options['barValueSpacing'] = 40;
    $tabThree_three_chart = $JsonDataBlock->chartBarByPercentage(303, t('CLINICAL CHANGE'), 'bg-ffffff', $tabThree_three_value, $tabThree_three_chart_options);

    $tabThree_four_value = array(
      'labels' => array(t("Osteoporosis"), t("Men’s Health"), t("Neuroscience"), t("Rheumatology")),
      'title' => "",
      'chartTitle' => "",
      'datasets' => array(
        array(
          "fillColor" => ColorClass::plate2(5),
          "strokeColor" => ColorClass::plate2(5),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_2193_count_area_2291[5], array_sum($question_2193_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_2193_count_area_2095[5], array_sum($question_2193_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_2193_count_area_2092[5], array_sum($question_2193_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_2193_count_area_2094[5], array_sum($question_2193_count_area_2094)),
          ),
          "title"=> t("Very Likely"),
        ),
        array(
          "fillColor" => ColorClass::plate2(4),
          "strokeColor" => ColorClass::plate2(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_2193_count_area_2291[4], array_sum($question_2193_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_2193_count_area_2095[4], array_sum($question_2193_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_2193_count_area_2092[4], array_sum($question_2193_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_2193_count_area_2094[4], array_sum($question_2193_count_area_2094)),
          ),
          "title"=> t('Likely'),
        ),
        array(
          "fillColor" => ColorClass::plate2(3),
          "strokeColor" => ColorClass::plate2(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_2193_count_area_2291[3], array_sum($question_2193_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_2193_count_area_2095[3], array_sum($question_2193_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_2193_count_area_2092[3], array_sum($question_2193_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_2193_count_area_2094[3], array_sum($question_2193_count_area_2094)),
          ),
          "title"=> t('Neutral'),
        ),
        array(
          "fillColor" => ColorClass::plate2(2),
          "strokeColor" => ColorClass::plate2(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_2193_count_area_2291[2], array_sum($question_2193_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_2193_count_area_2095[2], array_sum($question_2193_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_2193_count_area_2092[2], array_sum($question_2193_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_2193_count_area_2094[2], array_sum($question_2193_count_area_2094)),
          ),
          "title"=> t('Unlikely'),
        ),
        array(
          "fillColor" => ColorClass::plate2(1),
          "strokeColor" => ColorClass::plate2(1),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            // Osteoporosis
            BasicClass::percentage($question_2193_count_area_2291[1], array_sum($question_2193_count_area_2291)),
            // Men’s Health
            BasicClass::percentage($question_2193_count_area_2095[1], array_sum($question_2193_count_area_2095)),
            // Neuroscience
            BasicClass::percentage($question_2193_count_area_2092[1], array_sum($question_2193_count_area_2092)),
            // Rheumatology
            BasicClass::percentage($question_2193_count_area_2094[1], array_sum($question_2193_count_area_2094)),
          ),
          "title"=> t('Very Unlikely'),
        ),
      ),
    );
    $tabThree_four_chart = $JsonDataBlock->chartBarByPercentage(304, t('RECOMMEND PROGRAM'), 'bg-ffffff', $tabThree_four_value, $tabThree_three_chart_options);

    $tabThree = array(
      $tabThree_one_chart,
      $tabThree_two_chart,
      $tabThree_three_chart,
      $tabThree_four_chart,
    );

    // block
    $tabFirst = $this->blockTotalEventDistribution($meetings);
    $tabFourth = $this->blockTopRatedTableOld($meetings);

    // output
    $output = NULL;
    $output['fixedSection'] = $this->blockWidgetOne($meetings);

    $output['contentSection'] = array(
      $JsonDataBlock->tabOne(22, t('Total Event Distribution'), 'bg-344a5f', $tabFirst),
      $JsonDataBlock->tabOne(24, t('Key Touchpoints'), 'bg-344a5f', $tabThree),
      $JsonDataBlock->tabTwo(25, t('Top Rated'), 'bg-344a5f', $tabFourth),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function businessUnitSnapshotDiabetes($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // meeting by division - therapeutic area
    $meetings_division_2321 = $NodeQuery->meetingNidsByDivision(2321, $meetings);             // Lilly Diabetes
    $evaluations_division_2321 = $NodeQuery->evaluationNidsByMeeting($meetings_division_2321);

    $meetings_division_2320 = $NodeQuery->meetingNidsByDivision(2320, $meetings);             // Alliance
    $evaluations_division_2320 = $NodeQuery->evaluationNidsByMeeting($meetings_division_2320);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();

    $question_46_answer = $EvaluationFieldQuery->questionFieldValues($evaluations, 46, 64);
    $question_46_count = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer, array(1, 2, 3, 4, 5));

    // Lilly Diabetes
    $question_5_answer_set_division_2321 = $EvaluationFieldQuery->questionFieldValues($evaluations_division_2321, 5, 64);
    $question_5_count_division_2321 = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set_division_2321, array(1, 2, 3, 4, 5));
    $question_46_answer_set_division_2321 = $EvaluationFieldQuery->questionFieldValues($evaluations_division_2321, 46, 64);
    $question_46_count_division_2321 = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set_division_2321, array(1, 2, 3, 4, 5));
    $question_2193_answer_set_division_2321 = $EvaluationFieldQuery->questionFieldValues($evaluations_division_2321, 2193, 64);
    $question_2193_count_division_2321 = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set_division_2321, array(1, 2, 3, 4, 5));

    // Alliance
    $question_5_answer_set_division_2320 = $EvaluationFieldQuery->questionFieldValues($evaluations_division_2320, 5, 64);
    $question_5_count_division_2320 = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set_division_2320, array(1, 2, 3, 4, 5));
    $question_46_answer_set_division_2320 = $EvaluationFieldQuery->questionFieldValues($evaluations_division_2320, 46, 64);
    $question_46_count_division_2320 = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set_division_2320, array(1, 2, 3, 4, 5));
    $question_2193_answer_set_division_2320 = $EvaluationFieldQuery->questionFieldValues($evaluations_division_2320, 2193, 64);
    $question_2193_count_division_2320 = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set_division_2320, array(1, 2, 3, 4, 5));


    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    // tab three - - Key Touchpoints
    $tabThree_one_value = array(
      'labels' => array(t("Alliance"), t("Lilly Diabetes")),
      'title' => "",
      'datasets' => array(
        array(
          "fillColor" => "#344a5e",
          "strokeColor" => "#344a5e",
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::calcNTSScore($question_46_answer_set_division_2320, FALSE),  // Alliance
            BasicClass::calcNTSScore($question_46_answer_set_division_2321, FALSE),  // Lilly Diabetes
            0,    // for axis start position
          ),
          "title"=> '',
        ),
      ),
    );
    $tabThree_chartBar_chart_options['barValueSpacing'] = 80;
    $tabThree_one_chart = $JsonDataBlock->chartHorizontalBarCustomInGraph(301, t('NTS'), 'bg-ffffff', $tabThree_one_value, $tabThree_chartBar_chart_options);

    $tabThree_two_value = array(
      "labels" => array(t("Alliance"), t("Lilly Diabetes")),
      "datasets" => array(
        array(
          "fillColor" => ColorClass::plate(1),
          "strokeColor" => ColorClass::plate(1),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_division_2320[1],   // Alliance
            $question_46_count_division_2321[1],   // Lilly Diabetes
          ),
          "title" => '1 (' . $question_46_count[1] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(2),
          "strokeColor" => ColorClass::plate(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_division_2320[2],   // Alliance
            $question_46_count_division_2321[2],   // Lilly Diabetes
          ),
          "title" => '2 (' . $question_46_count[2] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(3),
          "strokeColor" => ColorClass::plate(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_division_2320[3],   // Alliance
            $question_46_count_division_2321[3],   // Lilly Diabetes
          ),
          "title" => '3 (' . $question_46_count[3] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(4),
          "strokeColor" => ColorClass::plate(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_division_2320[4],   // Alliance
            $question_46_count_division_2321[4],   // Lilly Diabetes
          ),
          "title" => '4 (' . $question_46_count[4] . ')',
        ),
        array(
          "fillColor" => ColorClass::plate(5),
          "strokeColor" => ColorClass::plate(5),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $question_46_count_division_2320[5],   // Alliance
            $question_46_count_division_2321[5],   // Lilly Diabetes
          ),
          "title" => '5 (' . $question_46_count[5] . ')',
        ),
      )
    );
    $tabThree_two_chart_options['barValueSpacing'] = 50;
    $tabThree_two_chart = $JsonDataBlock->chartHorizontalStackedBar(302, t('OVERALL SATISFACTION'), 'bg-ffffff', $tabThree_two_value, $tabThree_two_chart_options);

    $tabThree_three_value = array(
      'labels' => array(t("Lilly Diabetes"), t("Alliance")),
      'title' => "",
      'chartTitle' => "",
      'datasets' => array(
        array(
          "fillColor" => ColorClass::plate2(5),
          "strokeColor" => ColorClass::plate2(5),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_division_2321[5], array_sum($question_5_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_5_count_division_2320[5], array_sum($question_5_count_division_2320)),  // Alliance
          ),
          "title"=> t("Very Likely"),
        ),
        array(
          "fillColor" => ColorClass::plate2(4),
          "strokeColor" => ColorClass::plate2(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_division_2321[4], array_sum($question_5_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_5_count_division_2320[4], array_sum($question_5_count_division_2320)),  // Alliance
          ),
          "title"=> t('Likely'),
        ),
        array(
          "fillColor" => ColorClass::plate2(3),
          "strokeColor" => ColorClass::plate2(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_division_2321[3], array_sum($question_5_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_5_count_division_2320[3], array_sum($question_5_count_division_2320)),  // Alliance
          ),
          "title"=> t('Neutral'),
        ),
        array(
          "fillColor" => ColorClass::plate2(2),
          "strokeColor" => ColorClass::plate2(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_division_2321[2], array_sum($question_5_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_5_count_division_2320[2], array_sum($question_5_count_division_2320)),  // Alliance
          ),
          "title"=> t('Unlikely'),
        ),
        array(
          "fillColor" => ColorClass::plate2(1),
          "strokeColor" => ColorClass::plate2(1),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_5_count_division_2321[1], array_sum($question_5_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_5_count_division_2320[1], array_sum($question_5_count_division_2320)),  // Alliance
          ),
          "title"=> t('Very Unlikely'),
        ),
      ),
    );
    $tabThree_three_chart_options['barValueSpacing'] = 80;
    $tabThree_three_chart = $JsonDataBlock->chartBarByPercentage(303, t('CLINICAL CHANGE'), 'bg-ffffff', $tabThree_three_value, $tabThree_three_chart_options);

    $tabThree_four_value = array(
      'labels' => array(t("Lilly Diabetes"), t("Alliance")),
      'title' => "",
      'chartTitle' => "",
      'datasets' => array(
        array(
          "fillColor" => ColorClass::plate2(5),
          "strokeColor" => ColorClass::plate2(5),
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_division_2321[5], array_sum($question_2193_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_2193_count_division_2320[5], array_sum($question_2193_count_division_2320)),  // Alliance
          ),
          "title"=> t("Very Likely"),
        ),
        array(
          "fillColor" => ColorClass::plate2(4),
          "strokeColor" => ColorClass::plate2(4),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_division_2321[4], array_sum($question_2193_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_2193_count_division_2320[4], array_sum($question_2193_count_division_2320)),  // Alliance
          ),
          "title"=> t('Likely'),
        ),
        array(
          "fillColor" => ColorClass::plate2(3),
          "strokeColor" => ColorClass::plate2(3),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_division_2321[3], array_sum($question_2193_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_2193_count_division_2320[3], array_sum($question_2193_count_division_2320)),  // Alliance
          ),
          "title"=> t('Neutral'),
        ),
        array(
          "fillColor" => ColorClass::plate2(2),
          "strokeColor" => ColorClass::plate2(2),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_division_2321[2], array_sum($question_2193_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_2193_count_division_2320[2], array_sum($question_2193_count_division_2320)),  // Alliance
          ),
          "title"=> t('Unlikely'),
        ),
        array(
          "fillColor" => ColorClass::plate2(1),
          "strokeColor" => ColorClass::plate2(1),
          "pointColor" => "#00a9e0",
          "pointStrokeColor" => "#fff",
          "data" => array(
            BasicClass::percentage($question_2193_count_division_2321[1], array_sum($question_2193_count_division_2321)),  // Lilly Diabetes
            BasicClass::percentage($question_2193_count_division_2320[1], array_sum($question_2193_count_division_2320)),  // Alliance
          ),
          "title"=> t('Very Unlikely'),
        ),
      ),
    );
    $tabThree_four_chart = $JsonDataBlock->chartBarByPercentage(304, t('RECOMMEND PROGRAM'), 'bg-ffffff', $tabThree_four_value, $tabThree_three_chart_options);

    $tabThree = array(
      $tabThree_one_chart,
      $tabThree_two_chart,
      $tabThree_three_chart,
      $tabThree_four_chart,
    );

    // block
    $tabFirst = $this->blockTotalEventDistribution($meetings);
    $tabFourth = $this->blockTopRatedTableOld($meetings);

    // output
    $output = NULL;
    $output['fixedSection'] = $this->blockWidgetOne($meetings);

    $output['contentSection'] = array(
      $JsonDataBlock->tabOne(22, t('Total Event Distribution'), 'bg-344a5f', $tabFirst),
      $JsonDataBlock->tabOne(24, t('Key Touchpoints'), 'bg-344a5f', $tabThree),
      $JsonDataBlock->tabTwo(25, t('Top Rated'), 'bg-344a5f', $tabFourth),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function therapeuticAreaSnapshot($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    $meetings_program_type_74 = $NodeQuery->meetingNidsByProgramType(array(74), $meetings); // Health Education
    $meetings_program_type_2355 = $NodeQuery->meetingNidsByProgramType(array(2355), $meetings); // Ad Boards
    $meetings_program_type_2349 = $NodeQuery->meetingNidsByProgramType(array(2349), $meetings); // Ind. Sponsorships
    $meetings_program_type_2352 = $NodeQuery->meetingNidsByProgramType(array(2352), $meetings); // Scientific Exchange

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_5_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 5, 64);
    $question_5_count = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set, array(1, 2, 3, 4, 5));
    $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 8, 64);
    $question_8_count = BasicClass::arrayCountValuesBySpecifyArray($question_8_answer_set, array(1, 2, 3, 4, 5));
    $question_46_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 46, 64);
    $question_46_count = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set, array(1, 2, 3, 4, 5));
    $question_2193_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2193, 64);
    $question_2193_count = BasicClass::arrayCountValuesBySpecifyArray($question_2193_answer_set, array(1, 2, 3, 4, 5));

    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    // tab two - - Therapeutic Area Event Summary
    $tabTwo_one_value = array(
      'labels' => array(t("Health Education"), t("Ad Boards"), t("Ind. Sponsorships"), t("Scientific Exchange")),
      'title' => "",
      'datasets' => array(
        array(
          "fillColor" => "#344a5e",
          "strokeColor" => "#344a5e",
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            count($meetings_program_type_74),    // Health Education
            count($meetings_program_type_2355),  // Ad Boards
            count($meetings_program_type_2349),  // Ind. Sponsorships
            count($meetings_program_type_2352),  // Scientific Exchange
          ),
          "title"=> '',
        ),
      ),
    );
    $tabTwo_one_chart = $JsonDataBlock->chartBarCustomInGraph(201, t('EVENT TOTALS'), 'bg-ffffff', $tabTwo_one_value);

    $tabTwo_two_value = array(
      'labels' => array(t("Scientific Exchange"), t("Ind. Sponsorships"), t("Ad Boards"), t("Health Education")),
      'title' => "",
      'datasets' => array(
        array(
          "fillColor" => "#344a5e",
          "strokeColor" => "#344a5e",
          "pointColor" => "#05d23e",
          "pointStrokeColor" => "#fff",
          "data" => array(
            $NodeQuery->meetingNidsCountSignature($meetings_program_type_2352),  // Scientific Exchange
            $NodeQuery->meetingNidsCountSignature($meetings_program_type_2349),  // Ind. Sponsorships
            $NodeQuery->meetingNidsCountSignature($meetings_program_type_2355),  // Ad Boards
            $NodeQuery->meetingNidsCountSignature($meetings_program_type_74),    // Health Education
          ),
          "title"=> '',
        ),
      ),
    );
    $tabTwo_two_chart = $JsonDataBlock->chartHorizontalBar(202, t('HCP REACH'), 'bg-ffffff', $tabTwo_two_value);

    $tabTwo = array(
      $tabTwo_one_chart,
      $tabTwo_two_chart,
    );

    // chart three - -
    // $chart Section
    $chartSection_header_bg_color = 'bg-ffffff color-000000';
    $chartSection_block_class = 'col-md-4 col-xs-12';

    $chartSection_one_value = array(
      'datasets' => array(
        array(
          'value' => $question_46_count[5],
          'title' => '5 (' . $question_46_count[5] . ')',          // chart legend
        ),
        array(
          'value' => $question_46_count[4],
          'title' => '4 (' . $question_46_count[4] . ')',
        ),
        array(
          'value' => $question_46_count[3],
          'title' => '3 (' . $question_46_count[3] . ')',
        ),
        array(
          'value' => $question_46_count[2],
          'title' => '2 (' . $question_46_count[2] . ')',
        ),
        array(
          'value' => $question_46_count[1],
          'title' => '1 (' . $question_46_count[1] . ')',
        ),
      ),
      'title' => '',              // chart caption
    );
    $chartSection_one_bottom_value = $JsonDataBlock->sectionOne(
      array(
        'titleOne' => t('RESPONSES'),
        'titleTwo' => t('PROGRAM NTS'),
        'valueOne' => count($question_46_answer_set),
        'valueTwo' => BasicClass::calcNTSScore($question_46_answer_set),
      )
    );
    $chartSection_one_chart = $JsonDataBlock->chartPieNew(301, t('Overall Satisfaction'), $chartSection_header_bg_color, $chartSection_one_value, $chartSection_one_bottom_value, $chartSection_block_class);

    $chartSection_two_value = array(
      'datasets' => array(
        array(
          'value' => $question_5_count[5],
          'title' => '5 (' . $question_5_count[5] . ')',           // chart legend
        ),
        array(
          'value' => $question_5_count[4],
          'title' => '4 (' . $question_5_count[4] . ')',
        ),
        array(
          'value' => $question_5_count[3],
          'title' => '3 (' . $question_5_count[3] . ')',
        ),
        array(
          'value' => $question_5_count[2],
          'title' => '2 (' . $question_5_count[2] . ')',
        ),
        array(
          'value' => $question_5_count[1],
          'title' => '1 (' . $question_5_count[1] . ')',
        ),
      ),
      'title' => '',              // chart caption
    );
    $chartSection_two_bottom_value = $JsonDataBlock->sectionOne(
      array(
        'titleOne' => t('RESPONSES'),
        'titleTwo' => t('LIKELY OR VERY LIKELY'),
        'valueOne' => count($question_5_answer_set),
        'valueTwo' => BasicClass::percentage(($question_5_count[5] + $question_5_count[4]), array_sum($question_5_count)) . '% ',
      )
    );
    $chartSection_two_chart = $JsonDataBlock->chartPieNew(302, t('Likelihood of Clinical Change '), $chartSection_header_bg_color, $chartSection_two_value, $chartSection_two_bottom_value, $chartSection_block_class);

    $chartSection_three_value = array(
      'datasets' => array(
        array(
          'value' => $question_2193_count[5],
          'title' => '5 (' . $question_2193_count[5] . ')',          // chart legend
        ),
        array(
          'value' => $question_2193_count[4],
          'title' => '4 (' . $question_2193_count[4] . ')',
        ),
        array(
          'value' => $question_2193_count[3],
          'title' => '3 (' . $question_2193_count[3] . ')',
        ),
        array(
          'value' => $question_2193_count[2],
          'title' => '2 (' . $question_2193_count[2] . ')',
        ),
        array(
          'value' => $question_2193_count[1],
          'title' => '1 (' . $question_2193_count[1] . ')',
        ),
      ),
      'title' => '',              // chart caption
    );
    $chartSection_three_bottom_value = $JsonDataBlock->sectionOne(
      array(
        'titleOne' => t('RESPONSES'),
        'titleTwo' => t('LIKELY OR VERY LIKELY'),
        'valueOne' => count($question_2193_answer_set),
        'valueTwo' => BasicClass::percentage(($question_2193_count[5] + $question_2193_count[4]), array_sum($question_2193_count)) . '% ',
      )
    );
    $chartSection_three_chart = $JsonDataBlock->chartPieNew(303, t('Likelihood of Recommending'), $chartSection_header_bg_color, $chartSection_three_value, $chartSection_three_bottom_value, $chartSection_block_class);

    // block
    $tabOne = $this->blockTotalEventDistribution($meetings);
    $tabFour = $this->blockTopRatedTableOld($meetings);

    // output
    $output = NULL;
    $output['fixedSection'] = $this->blockWidgetOne($meetings);

    $output['contentSection'] = array(
      $JsonDataBlock->tabOne(22, t('Total Event Distribution'), 'bg-344a5f', $tabOne),
      $JsonDataBlock->tabOne(23, t('Therapeutic Area Event Summary'), 'bg-344a5f', $tabTwo),
      $chartSection_one_chart,
      $chartSection_two_chart,
      $chartSection_three_chart,
      $JsonDataBlock->tabTwo(25, t('Top Rated'), 'bg-344a5f', $tabFour),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function programSnapshot($meetings = array(), $program_tid = NULL) {
    // output
    $output = NULL;
    $output['fixedSection'] = $this->blockWidgetTwo($meetings, $program_tid);
    $output['contentSection'] = $this->blockProgramChart($meetings);

    return $output;
  }

  public function nodeMeetingDetailContent($meetings = NULL, $program_tid = NULL) {
    // JSON data
    $JsonDataBlock = new JsonDataBlock();

    $TermProgramInfo = new TermProgramInfo($program_tid);
    $header_value = $TermProgramInfo->termName() . t('Speaker Evaluation');

    $output = NULL;
    $output['fixedSection'] = array(
      $JsonDataBlock->widgetThree(113, $header_value, 'bg-ffffff', $this->blockWidgetMeeting(current($meetings))),
    );
    $output['contentSection'] = $this->blockProgramChart($meetings);

    if (is_array($this->nodeMeetingComments($meetings, $program_tid))) {
      foreach ($this->nodeMeetingComments($meetings, $program_tid) as $value) {
        $output['contentSection'][] = $value;
      }
    }

    return $output;
  }

  /** - - - - - Page Query Widget Json- - - - - - - - - - - - - - -   */
  /**
   * @return Term Array data
   */
  public function pageWidgets($meetings = array()) {
    $NodeQuery = new NodeQuery();

    // Accredited & OLA
    $acc_meetings = $NodeQuery->meetingNidsByProgramType(array(74), $meetings);
    $ola_meetings = $NodeQuery->meetingNidsByProgramType(array(75), $meetings);

    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);
    $acc_evaluations = $NodeQuery->evaluationNidsByMeeting($acc_meetings);
    $ola_evaluations = $NodeQuery->evaluationNidsByMeeting($ola_meetings);

    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 8, 64);
    $acc_question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($acc_evaluations, 8, 64);
    $ola_question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($ola_evaluations, 8, 64);

    $output = array(
      'class' => array('font-size-12'),
      'type' => 'widget',
      'value' => array(
        array(
          'title' => t('Total Events'),
          'value' => count($meetings),
          'bgColorClass1' => 'bg-00a9e0',
          'bgColorClass2' => 'bg-05d23e',
          'subLabel1' => t('Accredited'),
          'subValue1' => count($acc_meetings),
          'subLabel2' => t('OLA'),
          'subValue2' => count($ola_meetings),
        ),
        array(
          'title' => t('Total Attendees'),
          'value' => intval(BasicClass::arraySum($NodeQuery->meetingFieldValueArray('signatures', $meetings))),
          'bgColorClass1' => 'bg-00a9e0',
          'bgColorClass2' => 'bg-05d23e',
          'subLabel1' => t('Accredited'),
          'subValue1' => intval(BasicClass::arraySum($NodeQuery->meetingFieldValueArray('signatures', $acc_meetings))),
          'subLabel2' => t('OLA'),
          'subValue2' => intval(BasicClass::arraySum($NodeQuery->meetingFieldValueArray('signatures', $ola_meetings))),
        ),
        array(
          'title' => t('Evaluations Received'),
          'value' => count($evaluations),
          'bgColorClass1' => 'bg-00a9e0',
          'bgColorClass2' => 'bg-05d23e',
          'subLabel1' => t('Accredited'),
          'subValue1' => count($NodeQuery->evaluationNidsByMeeting($acc_meetings)),
          'subLabel2' => t('OLA'),
          'subValue2' => count($NodeQuery->evaluationNidsByMeeting($ola_meetings)),
        ),
        array(
          'title' => t('Avg. Speaker Rating'),
          'value' => number_format(BasicClass::arrayAverage($question_8_answer_set), 2),
          'bgColorClass1' => 'bg-00a9e0',
          'bgColorClass2' => 'bg-05d23e',
          'subLabel1' => t('Accredited'),
          'subValue1' => number_format(BasicClass::arrayAverage($acc_question_8_answer_set), 2),
          'subLabel2' => t('OLA'),
          'subValue2' => number_format(BasicClass::arrayAverage($ola_question_8_answer_set), 2),
        ),
      ),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function pageWidgetsProgram($meetings = array()) {
    $NodeQuery = new NodeQuery();

    // Accredited & OLA
    $acc_meetings = $NodeQuery->meetingNidsByProgramType(array(74), $meetings);
    $ola_meetings = $NodeQuery->meetingNidsByProgramType(array(75), $meetings);

    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);
    $acc_evaluations = $NodeQuery->evaluationNidsByMeeting($acc_meetings);
    $ola_evaluations = $NodeQuery->evaluationNidsByMeeting($ola_meetings);

    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 8, 64);
    $acc_question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($acc_evaluations, 8, 64);
    $ola_question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($ola_evaluations, 8, 64);

    $meeting_nid = current($meetings);
    $MeetingInfo = new MeetingInfo($meeting_nid);

    if ($MeetingInfo->programTid() == 2238) {   // Update on Diagnosis & management of IPF Summary
      $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2204, 64);
      $acc_question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($acc_evaluations, 2204, 64);
      $ola_question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($ola_evaluations, 2204, 64);
    }

    $output = array(
      'class' => array('font-size-12'),
      'type' => 'widget',
      'value' => array(
        array(
          'title' => t('Total Events'),
          'value' => count($meetings),
          'bgColorClass1' => 'bg-00a9e0',
          'bgColorClass2' => 'bg-05d23e',
          'subLabel1' => t('Accredited'),
          'subValue1' => count($acc_meetings),
          'subLabel2' => t('OLA'),
          'subValue2' => count($ola_meetings),
        ),
        array(
          'title' => t('Evaluations Received'),
          'value' => count($evaluations),
          'bgColorClass1' => 'bg-00a9e0',
          'bgColorClass2' => 'bg-05d23e',
          'subLabel1' => t('Accredited'),
          'subValue1' => count($NodeQuery->evaluationNidsByMeeting($acc_meetings)),
          'subLabel2' => t('OLA'),
          'subValue2' => count($NodeQuery->evaluationNidsByMeeting($ola_meetings)),
        ),
        array(
          'title' => t('Avg. Speaker Rating'),
          'value' => number_format(BasicClass::arrayAverage($question_8_answer_set), 2),
          'bgColorClass1' => 'bg-00a9e0',
          'bgColorClass2' => 'bg-05d23e',
          'subLabel1' => t('Accredited'),
          'subValue1' => number_format(BasicClass::arrayAverage($acc_question_8_answer_set), 2),
          'subLabel2' => t('OLA'),
          'subValue2' => number_format(BasicClass::arrayAverage($ola_question_8_answer_set), 2),
        ),
        array(
          'title' => t('View Event Evaluations'),
          'value' => t('View Event Evaluations'),
          'class' => 'bg-f24b99 color-fff font-size-14',
        ),
      ),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function pageCharts($meetings = array(), $twotwozeronine = NULL, $eight = NULL, $five= NULL) {
    // not sure, temporary
    if(!$twotwozeronine) {
      $twotwozeronine = 2209;
    }
    if(!$eight) {
      $eight = 8;
    }
    if(!$five) {
      $five = 5;
    }

    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // TermProgram
    $TermProgramQuery = new TermProgramQuery();

    // Accredited & OLA
    $acc_meetings = $NodeQuery->meetingNidsByProgramType(array(74), $meetings);
    $ola_meetings = $NodeQuery->meetingNidsByProgramType(array(75), $meetings);
    $symposia_meetings = $NodeQuery->meetingNidsByProgramType(array(2138), $meetings);

    $learning_objective_count = $this->nodeMeetingLearningObjective($meetings);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_5_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $five, 64);
    $question_5_count = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set, array(1, 2, 3, 4, 5));

    // Attendee Satisfaction
    $question_46_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 46, 64);
    $question_46_count = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set, array(1, 2, 3, 4, 5));
    $question_2209_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $twotwozeronine, 64);
    $question_2209_count = BasicClass::arrayCountValuesBySpecifyArray($question_2209_answer_set, array(1, 2, 3, 4, 5));
    $question_46_plus_2209_count = array(
      1 => $question_46_count[1] + $question_2209_count[1],
      2 => $question_46_count[2] + $question_2209_count[2],
      3 => $question_46_count[3] + $question_2209_count[3],
      4 => $question_46_count[4] + $question_2209_count[4],
      5 => $question_46_count[5] + $question_2209_count[5],
    );

    $output = array(
      array(
        'caption' => t('Learning Objective'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($learning_objective_count[5], $learning_objective_count[4], $learning_objective_count[3], $learning_objective_count[2], $learning_objective_count[1]),
            'legend' => array(t('Strongly Agree'), t('Agree'), t('Neutral'), t('Disagree'), t('Strongly Disagree')),
            'title' => '',
          ),
        ),
      ),
      array(
        'caption' => t('Likelihood of Changing Clinical Practice'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_5_count[5], $question_5_count[4], $question_5_count[3], $question_5_count[2], $question_5_count[1]),
            'legend' => array(t('Very Likely'), t('Likely'), t('Neutral'), t('Unlikely'), t('Very Unlikely')),
            'title' => '',
          ),
        ),
      ),
      array(
        'caption' => t('Event Type Summary'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'doughnut',
            'chartTypeClass' => 'chart-donut',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99'),
            'data' => array(count($acc_meetings), count($ola_meetings), count($symposia_meetings)),
            'legend' => array(t('Accredited'), t('OLA'), t('Symposia')),
            'title' => BasicClass::percentage(count($acc_meetings), count($meetings)) . '% ' . t('of program is Accredited'),
          ),
        ),
      ),
      array(
        'caption' => t('HCP Overall Satisfaction with BI Programs'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'doughnut',
            'chartTypeClass' => 'chart-donut',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_46_plus_2209_count[5], $question_46_plus_2209_count[4], $question_46_plus_2209_count[3], $question_46_plus_2209_count[2], $question_46_plus_2209_count[1]),
            'legend' => array(t('Best'), t('Good'), t('Same'), t('Fair'), t('Worst')),
            'title' => BasicClass::percentage(($question_46_plus_2209_count[5] + $question_46_plus_2209_count[4]), array_sum($question_46_plus_2209_count)) . '% ' . t('of speakers are Best or Good'),
          ),
        ),
      ),

    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function pageChartsRespirology($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // TermProgram
    $TermProgramQuery = new TermProgramQuery();

    // Accredited & OLA
    $acc_meetings = $NodeQuery->meetingNidsByProgramType(array(74), $meetings);
    $ola_meetings = $NodeQuery->meetingNidsByProgramType(array(75), $meetings);

    $learning_objective_count = $this->nodeMeetingLearningObjective($meetings);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_5_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 5, 64);
    $question_5_count = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set, array(1, 2, 3, 4, 5));
    $question_2209_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2209, 64);
    $question_2209_count = BasicClass::arrayCountValuesBySpecifyArray($question_2209_answer_set, array(1, 2, 3, 4, 5));

    $learning_objective_count_2093 = $this->nodeMeetingLearningObjective($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2093), $meetings));
    $learning_objective_count_2094 = $this->nodeMeetingLearningObjective($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2094), $meetings));
    $learning_objective_count_2095 = $this->nodeMeetingLearningObjective($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2095), $meetings));

    $output = array(
      array(
        'caption' => t('Learning Objectivessss'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'Bar',
            'chartTypeClass' => 'chart-bar',
            'colorClass' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'chartId' => 101,
            "chartOptions" => array(
              "animation" => true,
              "responsive" => true,
              "showScale" => false,
              "responsiveMinHeight" => 250,
              "responsiveMaxHeight" => 380,
              "bezierCurveTension" => 0.1,
              "graphMax" => 100,
              "annotateDisplay" => true,
              "annotateLabel" => "<%=v3+'%'%>",
              "annotateClassName" => "my11001799tooltip",
              "datasetStrokeWidth" => 2,
              "inGraphDataTmpl" => "'<%=v6+'%'%>'",
              "datasetFill" => false,
              "pointDotRadius" => 6,
              "maxBarWidth" => 30,
              "barStrokeWidth" => 2,
              "legend" => true,
              "legendBorders" => false,
              "legendFontColor" => "#000",
              "legendFontFamily" => "'Roboto,'Helvetica Neue',sans-serif'",
              "barValueSpacing" => 5,
              "percentageInnerCutout" => 99,
              "yAxisMinimumInterval" => 20
            ),
            'data' => array(
              "labels" => array("Asthma", "COPD", "IPF"),
              "datasets" => array(
                array(
                  "fillColor" => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
                  "strokeColor" => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
                  "pointColor" => "#05d23e",
                  "pointStrokeColor" => "#fff",
                  "data" => array(
                    BasicClass::percentage(($learning_objective_count_2093[5] + $learning_objective_count_2093[4]), array_sum($learning_objective_count_2093)),
                    BasicClass::percentage(($learning_objective_count_2094[5] + $learning_objective_count_2094[4]), array_sum($learning_objective_count_2094)),
                    BasicClass::percentage(($learning_objective_count_2095[5] + $learning_objective_count_2095[4]), array_sum($learning_objective_count_2095)),
                  ),
                  "title"=> '',
                ),
              )
            ),
            'legend' => array(t('Strongly Agree'), t('Agree'), t('Neutral'), t('Disagree'), t('Strongly Disagree')),
            'title' => '',
          ),
        ),
      ),
      array(
        'caption' => t('Likelihood of Changing Clinical Practice'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_5_count[5], $question_5_count[4], $question_5_count[3], $question_5_count[2], $question_5_count[1]),
            'legend' => array('Very Likely', 'Likely', 'Neutral', 'Unlikely', 'Very Unlikely'),
            'title' => '',
            // 'title' => BasicClass::percentage(($question_5_count[5] + $question_5_count[4]), array_sum($question_5_count)) . '% ' . t('of HCP\'s are Likely or Very Likely to Change Clinical Practice'),
          ),
        ),
      ),
      array(
        'caption' => t('Therapeutic Area Program Summary'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'Bar',
            'chartTypeClass' => 'chart-bar',
            'colorClass' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'chartId' => 102,
            'chartOptions' => array(
              "animation" => true,
              "responsive" => true,
              "responsiveMinHeight" => 280,
              "responsiveMaxHeight" => 357,
              "bezierCurveTension" => 0.1,
              "annotateDisplay" => true,
              "annotateClassName" => "my11001799tooltip",
              "datasetStrokeWidth" => 2,
              "inGraphDataTmpl" => '<%=v6+"%"%>',
              "annotateDisplay" => true,
              "annotateLabel" => '<%=v3%>',
              "datasetFill" => false,
              "pointDotRadius" => 6,
              "legendSpaceBeforeText" => 10,
              "legendSpaceAfterText" => 0,
              "legendSpaceBetweenTextHorizontal" => 15,
              "legendSpaceBetweenBoxAndText" => 9,
              "legendBlockSize" => 14,
              "spaceBottom" => 10,
              "spaceTop" => 10,
              "legend" => true,
              "legendBorders" => false,
              "legendFontColor" => '#000',
              "legendFontFamily" => 'Roboto,\'Helvetica Neue\',sans-serif',
              "barValueSpacing" => 20,
              "percentageInnerCutout" => 99,
              "yAxisMinimumInterval" => 1,
            ),
            'data' => array(
              "labels" => array("Asthma", "COPD", "IPF"),
              "datasets" => array(
                array(
                  "fillColor" => "#00a9e0",
                  "strokeColor" => "#00a9e0",
                  "pointColor" => "#05d23e",
                  "pointStrokeColor" => "#fff",
                  "data" => array(
                    count($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2093), $meetings)),
                    count($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2094), $meetings)),
                    count($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2095), $meetings)),
                  ),
                  "title"=> t('All Programs'),
                ),
                array(
                  "fillColor" => "#05d23e",
                  "strokeColor" => "#05d23e",
                  "pointColor" => "#00a9e0",
                  "pointStrokeColor" => "#fff",
                  "data" => array(
                    count($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2093), $acc_meetings)),
                    count($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2094), $acc_meetings)),
                    count($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2095), $acc_meetings)),
                  ),
                  "title"=> t('Accredited'),
                ),
                array(
                  "fillColor" => "#f24b99",
                  "strokeColor" => "#f24b99",
                  "pointColor" => "#00a9e0",
                  "pointStrokeColor" => "#fff",
                  "data" => array(
                    count($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2093), $ola_meetings)),
                    count($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2094), $ola_meetings)),
                    count($NodeQuery->meetingNidsByPrograms($TermProgramQuery->programTidsByArea(2095), $ola_meetings)),
                  ),
                  "title"=> t('OLA'),
                ),
              )
            ),
            'legend' => array('Very Likely', 'Likely', 'Neutral', 'Unlikely', 'Very Unlikely'),
            'title' => '',
          ),
        ),
      ),
      array(
        'caption' => t('HCP Overall Satisfaction with BI Programs'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'doughnut',
            'chartTypeClass' => 'chart-donut',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_2209_count[5], $question_2209_count[4], $question_2209_count[3], $question_2209_count[2], $question_2209_count[1]),
            'legend' => array(t('Best'), t('Good'), t('Same'), t('Fair'), t('Worst')),
            'title' => BasicClass::percentage(($question_2209_count[5] + $question_2209_count[4]), array_sum($question_2209_count)) . '% ' . t('of speakers are Best or Good'),
          ),
        ),
      ),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function pageChartsMeeting($meetings = array(), $question_terms = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();

    $question_answer_set = array();
    $question_count = array();
    foreach ($question_terms as $key => $row) {
      $question_answer_set[$key] = $EvaluationFieldQuery->questionFieldValues($evaluations, $row['tid'], 64);
      $question_count = BasicClass::arrayCountValuesBySpecifyArray($question_answer_set[$key], array(1, 2, 3, 4, 5));
    }

    $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 8, 64);
    $question_8_count = BasicClass::arrayCountValuesBySpecifyArray($question_8_answer_set, array(1, 2, 3, 4, 5));
    $question_5_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 5, 64);
    $question_5_count = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set, array(1, 2, 3, 4, 5));
    $question_46_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 46, 64);
    $question_46_count = BasicClass::arrayCountValuesBySpecifyArray($question_46_answer_set, array(1, 2, 3, 4, 5));
    $question_56_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 56, 64);
    $question_56_count = BasicClass::arrayCountValuesBySpecifyArray($question_56_answer_set, array(1, 2, 3, 4, 5));

    $output = array(
      array(
        'caption' => t('Overall Satisfaction'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_46_count[5], $question_46_count[4], $question_46_count[3], $question_46_count[2], $question_46_count[1]),
            'legend' => array(t('Excellent'), t('Very Good'), t('Good'), t('Fair'), t('Poor')),
            'title' => '',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_46_answer_set),
              'subLabel2' => t('EXCELLENT OR VERY GOOD'),
              'subValue2' => BasicClass::percentage(($question_46_count[5] + $question_46_count[4]), array_sum($question_46_count)) . '%',
            ),
          ),
        ),
      ),
      array(
        'caption' => t('Speaker Effectiveness'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_8_count[5], $question_8_count[4], $question_8_count[3], $question_8_count[2], $question_8_count[1]),
            'legend' => array('Very Effective', 'Effective', 'Neutral', 'Ineffective', 'Very Ineffective'),
            'title' => '',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_8_answer_set),
              'subLabel2' => t('VERY EFFECTIVE OR EFFECTIVE'),
              'subValue2' => BasicClass::percentage(($question_8_count[5] + $question_8_count[4]), array_sum($question_8_count)) . '%',
            ),
          ),
        ),
      ),
      array(
        'caption' => t('Clinical Practice Impact'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_5_count[5], $question_5_count[4], $question_5_count[3], $question_5_count[2], $question_5_count[1]),
            'legend' => array('Very Likely', 'Likely', 'Neutral', 'Unlikely', 'Very Unlikely'),
            'title' => '',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_5_answer_set),
              'subLabel2' => t('VERY LIKELY OR LIKELY'),
              'subValue2' => BasicClass::percentage(($question_5_count[5] + $question_5_count[4]), array_sum($question_5_count)) . '%',
            ),
          ),
        ),
      ),
      array(
        'caption' => t('Recommending Program'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_56_count[5], $question_56_count[4], $question_56_count[3], $question_56_count[2], $question_56_count[1]),
            'legend' => array(t('Excellent'), t('Very Good'), t('Good'), t('Fair'), t('Poor')),
            'title' => '',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_56_answer_set),
              'subLabel2' => t('EXCELLENT OR VERY GOOD'),
              'subValue2' => BasicClass::percentage(($question_56_count[5] + $question_56_count[4]), array_sum($question_56_count)) . '%',
            ),
          ),
        ),
      ),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function pageChartsOla($meetings = array(), $eight = NULL, $five= NULL, $twotwozeronine = NULL) {
    // not sure, temporary
    if(!$eight) {
      $eight = 8;
    }
    if(!$five) {
      $five = 5;
    }
    if(!$twotwozeronine) {
      $twotwozeronine = 2209;
    }

    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $eight, 64);
    $question_8_count = BasicClass::arrayCountValuesBySpecifyArray($question_8_answer_set, array(1, 2, 3, 4, 5));
    $question_5_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $five, 64);
    $question_5_count = BasicClass::arrayCountValuesBySpecifyArray($question_5_answer_set, array(1, 2, 3, 4, 5));
    $question_2209_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $twotwozeronine, 64);
    $question_2209_count = BasicClass::arrayCountValuesBySpecifyArray($question_2209_answer_set, array(1, 2, 3, 4, 5));

    $output = array(
      array(
        'caption' => t('Speaker Effectiveness'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 4,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_8_count[5], $question_8_count[4], $question_8_count[3], $question_8_count[2], $question_8_count[1]),
            'legend' => array('Very Effective', 'Effective', 'Neutral', 'Ineffective', 'Very Ineffective'),
            'title' => '',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_8_answer_set),
              'subLabel2' => t('VERY EFFECTIVE OR EFFECTIVE'),
              'subValue2' => BasicClass::percentage(($question_8_count[5] + $question_8_count[4]), array_sum($question_8_count)) . '%',
            ),
          ),
        ),
      ),
      array(
        'caption' => t('Clinical Practice Impact'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 4,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_5_count[5], $question_5_count[4], $question_5_count[3], $question_5_count[2], $question_5_count[1]),
            'legend' => array('Very Likely', 'Likely', 'Neutral', 'Unlikely', 'Very Unlikely'),
            'title' => '82.86% of HCP\'s are Likely or Very Likely to Change Clinical Practice',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_5_answer_set),
              'subLabel2' => t('VERY LIKELY OR LIKELY'),
              'subValue2' => BasicClass::percentage(($question_5_count[5] + $question_5_count[4]), array_sum($question_5_count)) . '%',
            ),
          ),
        ),
      ),
      array(
        'caption' => t('Overall Satisfaction'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 4,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_2209_count[5], $question_2209_count[4], $question_2209_count[3], $question_2209_count[2], $question_2209_count[1]),
            'legend' => array(t('Excellent'), t('Very Good'), t('Good'), t('Fair'), t('Poor')),
            'title' => '82.86% of HCP\'s are Likely or Very Likely to Change Clinical Practice',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_2209_answer_set),
              'subLabel2' => t('EXCELLENT OR VERY GOOD'),
              'subValue2' => BasicClass::percentage(($question_2209_count[5] + $question_2209_count[4]), array_sum($question_2209_count)) . '%',
            ),
          ),
        ),
      ),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function pageChartsOla_2093($meetings = array()) {
    $first = 8;
    $second = 2244;
    $third = 2245;

    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_first_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $first, 64);
    $question_first_count = BasicClass::arrayCountValuesBySpecifyArray($question_first_answer_set, array(1, 2, 3, 4, 5));

    $question_second_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $second, 64);
    $question_second_count = BasicClass::arrayCountValuesBySpecifyArray($question_second_answer_set, array(1, 2, 3, 4, 5));
    $question_third_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $third, 64);
    $question_third_count = BasicClass::arrayCountValuesBySpecifyArray($question_third_answer_set, array(1, 2, 3, 4, 5));

    $output = array(
      array(
        'caption' => t('Speaker Effectiveness'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 4,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_first_count[5], $question_first_count[4], $question_first_count[3], $question_first_count[2], $question_first_count[1]),
            'legend' => array('Very Effective', 'Effective', 'Neutral', 'Ineffective', 'Very Ineffective'),
            'title' => '',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_first_answer_set),
              'subLabel2' => t('VERY EFFECTIVE OR EFFECTIVE'),
              'subValue2' => BasicClass::percentage(($question_first_count[5] + $question_first_count[4]), array_sum($question_first_count)) . '%',
            ),
          ),
        ),
      ),
      array(
        'caption' => t('Overall Program Quality'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 4,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_second_count[5], $question_second_count[4], $question_second_count[3], $question_second_count[2], $question_second_count[1]),
            'legend' => array('Very Likely', 'Likely', 'Neutral', 'Unlikely', 'Very Unlikely'),
            'title' => '82.86% of HCP\'s are Likely or Very Likely to Change Clinical Practice',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_second_answer_set),
              'subLabel2' => t('VERY LIKELY OR LIKELY'),
              'subValue2' => BasicClass::percentage(($question_second_count[5] + $question_second_count[4]), array_sum($question_second_count)) . '%',
            ),
          ),
        ),
      ),
      array(
        'caption' => t('Quality of Content'),
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 4,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'piechart',
            'chartTypeClass' => 'chart-piechart',
            'colorClass' => array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6'),
            'data' => array($question_third_count[5], $question_third_count[4], $question_third_count[3], $question_third_count[2], $question_third_count[1]),
            'legend' => array(t('Excellent'), t('Very Good'), t('Good'), t('Fair'), t('Poor')),
            'title' => '82.86% of HCP\'s are Likely or Very Likely to Change Clinical Practice',
            'widget' => array(
              'title' => t('Total Events'),
              'value' => 22,
              'bgColorClass1' => 'bg-ffffff',
              'bgColorClass2' => 'bg-ffffff',
              'subLabel1' => t('RESPONSES'),
              'subValue1' => count($question_third_answer_set),
              'subLabel2' => t('EXCELLENT OR VERY GOOD'),
              'subValue2' => BasicClass::percentage(($question_third_count[5] + $question_third_count[4]), array_sum($question_third_count)) . '%',
            ),
          ),
        ),
      ),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function termProgramQuestionChartStyleSet($meetings = array(), $program_tid = NULL) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // Evaluation Field Query
    $EvaluationFieldQuery = new EvaluationFieldQuery();

    $TermProgramQuestionSet = new TermProgramQuestionSet();
    $term_program_question_set = $TermProgramQuestionSet->programSetContent($program_tid);

    $output = array();
    if (is_array($term_program_question_set)) {
      foreach ($term_program_question_set as $value) {
        $question_answer_set = '';
        $question_count = '';

        $question_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $value['questionTid'], $value['questionFid']);
        // $question_count = BasicClass::arrayCountValuesBySpecifyArray($question_answer_set, array(5, 4, 3, 2, 1));
        $question_count = BasicClass::arrayCountValuesBySpecifyArray($question_answer_set, $value['questionAnswerArray']);

        // Yes or No
        if ( $value['charts'][0]['chartType'] == 'gauge' && (count($value['questionAnswerArray']) == 2) && $value['questionAnswerArray'][0] == 'Yes' ) {
          $color_class = array($value['charts'][0]['colorClass'][0]);

          $question_count_data = strval(BasicClass::percentage(($question_count['Yes']), array_sum($question_count)));
          $sub_value_2 = BasicClass::percentage(($question_count['Yes']), array_sum($question_count)) . '%';
        }
        elseif ( $value['charts'][0]['chartType'] == 'gauge' && (count($value['questionAnswerArray']) == 2) && $value['questionAnswerArray'][0] == 'No' ) {
          $color_class = array($value['charts'][0]['colorClass'][0]);

          $question_count_data = strval(BasicClass::percentage(($question_count['No']), array_sum($question_count)));
          $sub_value_2 = BasicClass::percentage(($question_count['No']), array_sum($question_count)) . '%';

        }
        elseif ( $value['charts'][0]['chartType'] == 'doughnut' && (count($value['questionAnswerArray']) == 2) ) {     // doughnut chart
          $color_class = array($value['charts'][0]['colorClass'][0], $value['charts'][0]['colorClass'][1]);

          $question_count_data = array($question_count['Yes'], $question_count['No']);
          $sub_value_2 = BasicClass::percentage(($question_count[$value['questionAnswerArray'][0]]), array_sum($question_count)) . '%';

        }
        else {    // default scale is 5, array(5, 4, 3, 2, 1) piechart
          $color_class = $value['charts'][0]['colorClass'];

          $question_count_data = array($question_count[5], $question_count[4], $question_count[3], $question_count[2], $question_count[1]);
          $sub_value_2 = BasicClass::percentage(($question_count[5] + $question_count[4]), array_sum($question_count)) . '%';
        }

        $output[] = array(
          'caption' => $value['caption'],
          'captionBgColorClass' => 'bg-ffffff',
          'captionColor' => '',
          'captionLegend' => '',
          'captionTitle' => '',
          'column' => $value['column'],
          'chartGroup' => '1',
          'charts' => array(
            array(
              'chartType' => $value['charts'][0]['chartType'],
              'chartTypeClass' => $value['charts'][0]['chartTypeClass'],
              'colorClass' => $color_class,
              'data' => $question_count_data,
              'legend' => $value['charts'][0]['legend'],
              'title' => '',
              'widget' => array(
                'title' => '',
                'value' => '',
                'bgColorClass1' => 'bg-ffffff',
                'bgColorClass2' => 'bg-ffffff',
                'subLabel1' => $value['charts'][0]['widget']['subLabel1'],
                'subValue1' => count($question_answer_set),
                'subLabel2' => $value['charts'][0]['widget']['subLabel2'],
                'subValue2' => $sub_value_2,
              ),
            ),
          ),
        );
      }
    }


    return $output;
  }

  /**
   * @return Term Array data
   */
  public function pageChartsAccredited($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // Evaluation
    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_2131_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2131, 63);
    $question_2131_count = BasicClass::arrayCountValuesBySpecifyArray($question_2131_answer_set, array(0, 1));
    $question_2163_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2163, 63);
    $question_2163_count = BasicClass::arrayCountValuesBySpecifyArray($question_2163_answer_set, array(0, 1));
    $question_2133_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, 2133, 63);
    $question_2133_count = BasicClass::arrayCountValuesBySpecifyArray($question_2133_answer_set, array(0, 1));

    $output = array(
      array(
        'caption' => t('Precieved Program Bias'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'gauge',
            'chartTypeClass' => 'chart-gauge',
            'colorClass' => array('#00a9e0'),
            'data' => (string)(BasicClass::percentage(($question_2131_count[0]), array_sum($question_2131_count))),
            'legend' => array('Accredited', 'OLA'),
            'title' => BasicClass::percentage($question_2131_count[0], array_sum($question_2131_count)) . '% '. t('Agree or Strongly Agree'),
          ),
        ),
      ),
      array(
        'caption' => t('The program met the CHE Code of Ethics'),
        'captionBgColorClass' => 'bg-344a5f',
        'captionColor' => array('#344a5f', '#56bfb5', '#f24b99', '#bfbfbf', '#e6e6e6'),
        'captionLegend' => '',
        'captionTitle' => '',
        'column' => 6,
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => 'gauge',
            'chartTypeClass' => 'chart-gauge',
            'colorClass' => array('#00a9e0'),
            'data' => (string)(BasicClass::percentage($question_2133_count[1], array_sum($question_2133_count))),
            'legend' => array('Accredited', 'OLA'),
            'title' => BasicClass::percentage($question_2133_count[1], array_sum($question_2133_count)) . '% '. t('Agree or Strongly Agree'),
          ),
        ),
      ),
    );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function timeGeographyCharts($meetings = array()) {
    $NodeQuery = new NodeQuery();
    // Accredited & OLA
    $acc_meetings = $NodeQuery->meetingNidsByProgramType(array(74), $meetings);
    $ola_meetings = $NodeQuery->meetingNidsByProgramType(array(75), $meetings);
    $symposia_meetings = $NodeQuery->meetingNidsByProgramType(array(2138), $meetings);

    $output = array(
      'programByMonth' => array(
        'chartId' => '100',
        'chartType' => 'Line',
        'chartData' => array(
          "labels" => array("JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"),
          "datasets" => array(
            array(
              "fillColor" => "rgba(151,187,205,0)",
              "strokeColor" => "#f24b99",
              "pointColor" => "#f24b99",
              "pointStrokeColor" => "#fff",
              "data" => array(
                count($NodeQuery->meetingNidsByMonth(array(1), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(2), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(3), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(4), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(5), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(6), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(7), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(8), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(9), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(10), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(11), $symposia_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(12), $symposia_meetings)),
              ),
              "title"=> t('Symposia'),
            ),
            array(
              "fillColor" => "rgba(151,187,205,0)",
              "strokeColor" => "#05d23e",
              "pointColor" => "#05d23e",
              "pointStrokeColor" => "#fff",
              "data" => array(
                count($NodeQuery->meetingNidsByMonth(array(1), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(2), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(3), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(4), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(5), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(6), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(7), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(8), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(9), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(10), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(11), $ola_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(12), $ola_meetings)),
              ),
              "title"=> t('OLA'),
            ),
            array(
              "fillColor" => "rgba(151,187,205,0)",
              "strokeColor" => "#00a9e0",
              "pointColor" => "#00a9e0",
              "pointStrokeColor" => "#fff",
              "data" => array(
                count($NodeQuery->meetingNidsByMonth(array(1), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(2), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(3), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(4), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(5), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(6), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(7), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(8), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(9), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(10), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(11), $acc_meetings)),
                count($NodeQuery->meetingNidsByMonth(array(12), $acc_meetings)),
              ),
              "title"=> t("Accredited"),
            ),
          )
        ),
      ),
      'programByRegion' => array(
        'chartId' => '102',
        'chartType' => 'HorizontalBar',
        'chartData' => array(
          "labels" => array( "R1", "R2", "R3"),
          "datasets" => array(
            array(
              "fillColor" => "#00a9e0",
              "strokeColor" => "#00a9e0",
              "pointColor" => "#05d23e",
              "pointStrokeColor" => "#fff",
              "data" => array(
                count($NodeQuery->meetingNidsByProvince(array(20, 19, 30, 21), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(27, 22, 28, 25, 23), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(29), $acc_meetings)),
              ),
              "title"=> t("Accredited"),
            ),
            array(
              "fillColor" => "#05d23e",
              "strokeColor" => "#05d23e",
              "pointColor" => "#00a9e0",
              "pointStrokeColor" => "#fff",
              "data" => array(
                count($NodeQuery->meetingNidsByProvince(array(20, 19, 30, 21), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(27, 22, 28, 25, 23), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(29), $ola_meetings)),
              ),
              "title"=> t('OLA'),
            ),
            array(
              "fillColor" => "#f24b99",
              "strokeColor" => "#f24b99",
              "pointColor" => "#00a9e0",
              "pointStrokeColor" => "#fff",
              "data" => array(
                count($NodeQuery->meetingNidsByProvince(array(20, 19, 30, 21), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(27, 22, 28, 25, 23), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(29), $symposia_meetings)),
              ),
              "title"=> t('Symposia'),
            ),
          )
        ),
      ),
      'programByProvince' => array(
        'chartId' => '103',
        'chartType' => 'Bar',
        'chartData' => array(
          "labels" => array("BC","AB","SK","MB","ON","QC","NB","PEI","NS","NL"),
          "datasets" => array(
            array(
              "fillColor" => "#00a9e0",
              "strokeColor" => "#00a9e0",
              "pointColor" => "#05d23e",
              "pointStrokeColor" => "#fff",
              "data" => array(
                count($NodeQuery->meetingNidsByProvince(array(20), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(19), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(30), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(21), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(27), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(29), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(22), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(28), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(25), $acc_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(23), $acc_meetings)),
              ),
              "title"=> t("Accredited"),
            ),
            array(
              "fillColor" => "#05d23e",
              "strokeColor" => "#05d23e",
              "pointColor" => "#00a9e0",
              "pointStrokeColor" => "#fff",
              "data" => array(
                count($NodeQuery->meetingNidsByProvince(array(20), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(19), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(30), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(21), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(27), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(29), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(22), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(28), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(25), $ola_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(23), $ola_meetings)),
              ),
              "title"=> t('OLA'),
            ),
            array(
              "fillColor" => "#f24b99",
              "strokeColor" => "#f24b99",
              "pointColor" => "#00a9e0",
              "pointStrokeColor" => "#fff",
              "data" => array(
                count($NodeQuery->meetingNidsByProvince(array(20), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(19), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(30), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(21), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(27), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(29), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(22), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(28), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(25), $symposia_meetings)),
                count($NodeQuery->meetingNidsByProvince(array(23), $symposia_meetings)),
              ),
              "title"=> t('Symposia'),
            ),
          )
        ),
      ),
    );

    return $output;
  }

  /**
   * @return, Program Learning Objective
   */
  public function programLearningObjective($meetings = array(), $program_tid = NULL) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // base on program Learning Objective
    $TermProgramInfo = new TermProgramInfo($program_tid);
    $TermEvaluationFormInfo = new TermEvaluationFormInfo($TermProgramInfo->evaluationFormTid());

    $learning_objective_question_tids = array();
    if (is_array($TermEvaluationFormInfo->questionSetArray())) {
      foreach ($TermEvaluationFormInfo->questionSetArray() as $row) {
        $TermQuestionLibraryInfo = new TermQuestionLibraryInfo($row['quesset_question_tid']);
        if ($TermQuestionLibraryInfo->questionTypeTid() == 2213) {  // 2213 is Learning Objective
          $learning_objective_question_tids[] = $row['quesset_question_tid'];
        }
      }
    }

    // chart column
    $chart_row_column = 'col-md-4';
    if (is_array($learning_objective_question_tids)) {
      switch (count($learning_objective_question_tids)) {
        case 2:
          $chart_row_column = 'col-md-6';
          break;
        case 4:
          $chart_row_column = 'col-md-3';
          break;
        case 5:
          $chart_row_column = 'col-md-custom-20-p';
          break;

        default:
          break;
      }
    }

    $output = NULL;
    // overall
    $output['overall'] = array();

    $output['overall']['charts'] = array();
    if (is_array($learning_objective_question_tids)) {
      foreach ($learning_objective_question_tids as $question_tid) {
        $TermQuestionLibraryInfo = new TermQuestionLibraryInfo($question_tid);

        // Evaluation
        $EvaluationFieldQuery = new EvaluationFieldQuery();
        $learning_question_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations, $question_tid, 64);
        $learning_question_count = BasicClass::arrayCountValuesBySpecifyArray($learning_question_answer_set, array(1, 2, 3, 4, 5));

        $output['overall']['charts'][] = array(
          "chartTitle" => $TermQuestionLibraryInfo->termName(),
          'chartId' => $question_tid,
          'chartType' => 'Bar',
          "ChartOptions" => array(
            "animation" => true,
            "responsive" => true,
            "showScale" => false,
            "responsiveMinHeight" => 250,
            "responsiveMaxHeight" => 380,
            "bezierCurveTension" => 0.1,
            "graphMax" => 100,
            "annotateDisplay" => true,
            "annotateLabel" => "<%=v3+'%'%>",
            "annotateClassName" => "my11001799tooltip",
            "datasetStrokeWidth" => 2,
            "inGraphDataTmpl" => "'<%=v6+'%'%>'",
            "datasetFill" => false,
            "pointDotRadius" => 6,
            "maxBarWidth" => 30,
            "barStrokeWidth" => 2,
            "legend" => true,
            "legendBorders" => false,
            "legendFontColor" => "#000",
            "legendFontFamily" => "'Roboto,'Helvetica Neue',sans-serif'",
            "barValueSpacing" => 5,
            "percentageInnerCutout" => 99,
            "yAxisMinimumInterval" => 20
          ),
          'chartData' => array(
            "labels" => array(" ", " ", " ", " ", " "),
            "datasets" => array(
              array(
                'fillColor' => array('#00a9e0', "#05d23e", "#f24b99", "#bfbfbf", "#e6e6e6"),
                'pointColor' => '#05d23e',
                'pointStrokeColor' => '#fff',
                'strokeColor' => '#fff',
                'data' => array(
                  BasicClass::percentage($learning_question_count[5], array_sum($learning_question_count)),
                  BasicClass::percentage($learning_question_count[4], array_sum($learning_question_count)),
                  BasicClass::percentage($learning_question_count[3], array_sum($learning_question_count)),
                  BasicClass::percentage($learning_question_count[2], array_sum($learning_question_count)),
                  BasicClass::percentage($learning_question_count[1], array_sum($learning_question_count)),
                ),
                'title' => t('Strongly Agree'),
                'Longtitle' => t('I have greater confidence in using a LAMA as add on to ICS/LABA for asthma patients who are symptomatic'),
              ),
            )
          ),
        );
      }
    }

    return $output;
  }

  /** - - - - - Query Table Json- - - - - - - - - - - - - - - - - - - - -   */
  /**
   * @return Term Array data
   */
  public function tableEventTracker($meetings = array()) {
    $output = NULL;
    if (is_array($meetings)) {
      foreach ($meetings as $key => $nid) {
        if (isset($nid)) {
          $NodeQuery = new NodeQuery();
          $evaluations = $NodeQuery->evaluationNidsByMeeting(array($nid));

          $MeetingInfo = new MeetingInfo($nid);
          $eventStatus = $MeetingInfo->eventStatus();

          $output[] = array(
            'date' => $MeetingInfo->dateValue(),
            'programTid' => $MeetingInfo->programTid(),
            'programName' => $MeetingInfo->programName(),
            'speakerUid' => $MeetingInfo->speakerUid(),
            'speakerName' => $MeetingInfo->speakerName(),
            'repName' => $MeetingInfo->repName(),
            'city' => $MeetingInfo->cityName(),
            'province' => $MeetingInfo->provinceNameAbbr(),
            'eventStatus' => $eventStatus,
            'eventStatusColor' => $MeetingInfo->eventStatusColor($eventStatus),
            'nodeNid' => $MeetingInfo->nodeNid(),
          );
        }
      }
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function tableSpeakerEvaluations($meetings = array()) {
    $output = NULL;
    if (is_array($meetings)) {
      foreach ($meetings as $key => $nid) {
        if (isset($nid)) {
          $NodeQuery = new NodeQuery();
          $evaluations = $NodeQuery->evaluationNidsByMeeting(array($nid));

          if (count($evaluations) > 0) {
            $MeetingInfo = new MeetingInfo($nid);
            $eventStatus = $MeetingInfo->eventStatus();

            $output[] = array(
              'date' => $MeetingInfo->dateValue(),
              'programTid' => $MeetingInfo->programTid(),
              'programName' => $MeetingInfo->programName(),
              'speakerUid' => $MeetingInfo->speakerUid(),
              'speakerName' => $MeetingInfo->speakerName(),
              'repName' => $MeetingInfo->repName(),
              'city' => $MeetingInfo->cityName(),
              'province' => $MeetingInfo->provinceNameAbbr(),
              'nodeNid' => $MeetingInfo->nodeNid(),
            );
          }
        }
      }
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function tableTopAccreditedProgram($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // not use taxonomy_get_tree(), use parameters $program_tids
    $terms = taxonomy_get_tree(2);

    /**
     * only filter current program_tids
     */
    $output = NULL;
    if (is_array($meetings)) {
      if (count($meetings) > 0) {
        if (is_array($terms)) {
          foreach ($terms as $key => $term) {
            $meetings_by_term = $NodeQuery->meetingNidsByPrograms(array($term->tid), $meetings);
            $meetings_by_evaluated = $NodeQuery->meetingNidsByEventStatus('Evaluated', $meetings_by_term);

            if (count($meetings_by_term) > 0) {
              $NodeQuery = new NodeQuery();
              $evaluations_by_term = $NodeQuery->evaluationNidsByMeeting($meetings_by_term);

              // Evaluation
              $EvaluationFieldQuery = new EvaluationFieldQuery();
              $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations_by_term, 8, 64);

              $TermProgramInfo = new TermProgramInfo($term->tid);

              $output[] = array(
                'businessUnitName' => $TermProgramInfo->programUnitName(),
                'completedEvents' => count($meetings_by_evaluated),
                'eventNumber' => count($meetings_by_term),
                'evaluationNumber' => count($evaluations_by_term),
                'ntsScore' => number_format(BasicClass::arrayAverage($question_8_answer_set), 2),
                'participantNumber' => BasicClass::arraySum($NodeQuery->meetingFieldValueArray('signatures', $meetings_by_term)),
                'programName' => $TermProgramInfo->termName(),
                'programTid' => $TermProgramInfo->termTid(),
              );
            }

          }
        }
      }
    }

    // the header should be return by function
    // $output = array(
    //   'title' => array(t('Program'), t('BU'), t('Event'), t('Evaluation'), t('Reach'),t('Rating')),
    //   'content' => $table_content,
    // );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function tableTopRatedSpeakers($meetings = array()) {
    $UserQuery = new UserQuery();
    $user_uids = $UserQuery->allUserUidByRole(array(6));

    $NodeQuery = new NodeQuery();

    $output = NULL;
    if (is_array($user_uids) && is_array($meetings)) {
      $users = user_load_multiple($user_uids);
      foreach ($users as $key => $user) {
        $meetings_by_user = $NodeQuery->meetingNidsBySpeakers(array($user->uid), $meetings);

        if (count($meetings_by_user) > 0) {
          $NodeQuery = new NodeQuery();
          $evaluations_by_user = $NodeQuery->evaluationNidsByMeeting($meetings_by_user);
          $UserInfo = new UserInfo($user->uid);

          $EvaluationFieldQuery = new EvaluationFieldQuery();
          $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations_by_user, 8, 64);

          $output[] = array(
            'userName' => $UserInfo->userName(),
            'userUid' => $user->uid,
            'eventNumber' => count($meetings_by_user),
            'evaluationNumber' => count($evaluations_by_user),
            'participantNumber' => BasicClass::arraySum($NodeQuery->meetingFieldValueArray('signatures', $meetings_by_user)),
            'rateScore' => number_format(BasicClass::arrayAverage($question_8_answer_set), 2),
          );
        }
      }
    }

    return $output;
  }

  /** - - - - - User - - - - - - -- - - - - - - - - - - - - - - - - - - -   */

  /**
   * @return Term Array data
   */
  public function tableTopAccreditedProgramNoHeader($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // not use taxonomy_get_tree(), use parameters $program_tids
    $terms = taxonomy_get_tree(2);

    /**
     * only filter current program_tids
     */
    $output = NULL;
    if (is_array($meetings)) {
      if (count($meetings) > 0) {
        if (is_array($terms)) {
          foreach ($terms as $key => $term) {
            $meetings_by_term = $NodeQuery->meetingNidsByPrograms(array($term->tid), $meetings);

            if (count($meetings_by_term) > 0) {
              $NodeQuery = new NodeQuery();
              $evaluations_by_term = $NodeQuery->evaluationNidsByMeeting($meetings_by_term);

              // Evaluation
              $EvaluationFieldQuery = new EvaluationFieldQuery();
              $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations_by_term, 8, 64);

              $TermProgramInfo = new TermProgramInfo($term->tid);

              $output[] = array(
                '<a href="' . base_path() . 'dashpage/program/snapshot/' . $term->tid . '">'. $TermProgramInfo->termName() . '</a>',
                $TermProgramInfo->programUnitName(),
                count($meetings_by_term),
                count($evaluations_by_term),
                BasicClass::arraySum($NodeQuery->meetingFieldValueArray('signatures', $meetings_by_term)),
                number_format(BasicClass::arrayAverage($question_8_answer_set), 2),
                $TermProgramInfo->termTid(),
              );
            }

          }
        }
      }
    }

    // the header should be return by function
    // $output = array(
    //   'title' => array(t('Program'), t('BU'), t('Event'), t('Evaluation'), t('Reach'),t('Rating')),
    //   'content' => $table_content,
    // );

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function tableTopRatedSpeakersNoHeader($meetings = array()) {
    $UserQuery = new UserQuery();
    $user_uids = $UserQuery->allUserUidByRole(array(6));

    $NodeQuery = new NodeQuery();

    $output = NULL;
    if (is_array($user_uids) && is_array($meetings)) {
      $users = user_load_multiple($user_uids);
      foreach ($users as $key => $user) {
        $meetings_by_user = $NodeQuery->meetingNidsBySpeakers(array($user->uid), $meetings);

        if (count($meetings_by_user) > 0) {
          $NodeQuery = new NodeQuery();
          $evaluations_by_user = $NodeQuery->evaluationNidsByMeeting($meetings_by_user);
          $UserInfo = new UserInfo($user->uid);

          $EvaluationFieldQuery = new EvaluationFieldQuery();
          $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations_by_user, 8, 64);

          $output[] = array(
            $UserInfo->userName(),
            count($meetings_by_user),
            count($evaluations_by_user),
            BasicClass::arraySum($NodeQuery->meetingFieldValueArray('signatures', $meetings_by_user)),
            number_format(BasicClass::arrayAverage($question_8_answer_set), 2),
          );
        }
      }
    }

    // the header should be return by function
    // $output = array(
    //   'title' => array(t('Name'), t('Events'), t('Evaluation'), t('Reach'), t('Rating')),
    //   'content' => $table_content,
    // );

    return $output;
  }
  /** - - - - - User - - - - - - -- - - - - - - - - - - - - - - - - - - -   */

  /**
   * @return Term Array data
   */
  public function tableProgramUnitProgram($meetings = array()) {
    $NodeQuery = new NodeQuery();
    $evaluations = $NodeQuery->evaluationNidsByMeeting($meetings);

    // not use taxonomy_get_tree(), use parameters $program_tids
    $terms = taxonomy_get_tree(2);

    /**
     * only filter current program_tids
     */
    $output = NULL;
    if (is_array($meetings)) {
      if (count($meetings) > 0) {
        if (is_array($terms)) {
          foreach ($terms as $key => $term) {
            $meetings_by_term = $NodeQuery->meetingNidsByPrograms(array($term->tid), $meetings);

            if (count($meetings_by_term) > 0) {
              $NodeQuery = new NodeQuery();
              $evaluations_by_term = $NodeQuery->evaluationNidsByMeeting($meetings_by_term);

              // Evaluation
              $EvaluationFieldQuery = new EvaluationFieldQuery();
              $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations_by_term, 8, 64);

              $TermProgramInfo = new TermProgramInfo($term->tid);

              $output[] = array(
                'programTid' => $TermProgramInfo->termTid(),
                'programName' => $TermProgramInfo->termName(),
                'eventNumber' => count($meetings_by_term),
                'evaluationNumber' => count($evaluations_by_term),
                'participantNumber' => BasicClass::arraySum($NodeQuery->meetingFieldValueArray('signatures', $meetings_by_term)),
                'therapeuticAreaName' => $TermProgramInfo->therapAreaName(),
                'programTypeName' => $TermProgramInfo->programTypeName(),
              );
            }

          }
        }
      }
    }

    return $output;
  }

  /** - - - - - User - - - - - - -- - - - - - - - - - - - - - - - - - - -   */
  /**
   * @return Term Array data
   */
  public function userBasicInfo($uids = NULL) {
    $output = NULL;

    if (is_array($uids)) {
      foreach ($uids as $uid) {
        $UserInfo = new UserInfo($uid);
        $output[] = $UserInfo->basicMethodsValue();
      }
    }

    return $output;
  }
  /**
   * @return Term Array data
   */
  public function userFullInfo($uids = NULL) {
    $output = NULL;

    if (is_array($uids)) {
      foreach ($uids as $uid) {
        $UserInfo = new UserInfo($uid);
        $output[] = $UserInfo->allMethodsValue();
      }
    }

    return $output;
  }
  /**
   * @return Term Array data
   */
  public function allUserList() {
    $output = NULL;

    $UserQuery = new UserQuery();
    $uids = $UserQuery->allUserUid();

    $output = $this->userBasicInfo($uids);

    return $output;
  }
  /**
   * @return Term Array data
   */
  public function speakerUserList() {
    $output = NULL;

    $UserQuery = new UserQuery();
    $uids = $UserQuery->allUserUidByRole(array(6));

    $output = $this->userBasicInfo($uids);

    return $output;
  }
  /**
   * @return Term Array data
   */
  public function repUserList() {
    $output = NULL;

    $UserQuery = new UserQuery();
    $uids = $UserQuery->allUserUidByRole(array(8));

    $output = $this->userBasicInfo($uids);

    return $output;
  }
  /**
   * @return Term Array data
   */
  public function userProfileFields($uid = NULL) {
    $output = NULL;
    $fields = NULL;

    $UserInfo = new UserInfo($uid);

    $fields["firstName"] = array(
      'fieldName' => 'field_user_first_name',
      'title' => t('First Name'),
      'type' => 'textfield',
      'defaultValue' => $UserInfo->firstName(),
      'updateStatus' => 0,
      'disabled' => FALSE,
      'required' => FALSE,
    );
    $fields["lastName"] = array(
      'fieldName' => 'field_user_last_name',
      'title' => t('Last Name'),
      'type' => 'textfield',
      'defaultValue' => $UserInfo->lastName(),
      'updateStatus' => 0,
      'disabled' => FALSE,
      'required' => FALSE,
    );
    $fields['mail'] = array(
      'fieldName' => 'mail',
      'title' => t('Email'),
      'type' => 'textfield',
      'defaultValue' => $UserInfo->userEmail(),
      'updateStatus' => 0,
      'disabled' => FALSE,
      'required' => FALSE,
    );
    $fields['pass'] = array(
      'fieldName' => 'pass',
      'title' => t('Password'),
      'type' => 'textfield',
      'defaultValue' => NULL,
      'updateStatus' => 0,
      'disabled' => FALSE,
    );
    $fields['province'] = array(
      'fieldName' => 'field_user_province',
      'title' => t('Province'),
      'type' => 'select',
      'defaultValue' => $UserInfo->provinceTid(),
      'updateStatus' => 0,
      'disabled' => FALSE,
      'required' => FALSE,
      'options' => $this->pushDefaultValue($this->termProvince(TRUE), array('termTid' => NULL, 'termName' => t('Select'))),
    );
    $fields['city'] = array(
      'fieldName' => 'field_user_city',
      'title' => t('City'),
      'type' => 'select',
      'defaultValue' => $UserInfo->cityTid(),
      'updateStatus' => 0,
      'disabled' => FALSE,
      'required' => FALSE,
      'options' => $this->pushDefaultValue($this->termCityArrayFile(TRUE), array('termTid' => NULL, 'termName' => t('Select'))),
    );
    $fields['programUnit'] = array(
      'fieldName' => 'field_user_program_unit',
      'title' => t('Business Unit'),
      'type' => 'select',
      'defaultValue' => $UserInfo->programUnitTid(),
      'updateStatus' => 0,
      'disabled' => FALSE,
      'required' => FALSE,
      'options' => $this->pushDefaultValue($this->termBusinessUnit(TRUE), array('termTid' => NULL, 'termName' => t('Select'))),
    );

    $output = $fields;

    return $output;
  }

  /**
   * @return Term Array data
   * speakerSummaryPopup, speaker Summary Pop up
   */
  public function userSpeakerDetails($uid = NULL) {
    $UserInfo = new UserInfo($uid);

    $NodeQuery = new NodeQuery();
    $all_meetings = $NodeQuery->meetingNids();
    $meetings_by_user = $NodeQuery->meetingNidsBySpeakers(array($uid), $all_meetings);

    // set YTD, 'Year To Date - YTD' The period beginning January 1st of the current year up until today's date.
    $DateTimeClass = new DateTimeClass();
    $ytd_meetings = $NodeQuery->meetingNidsByTime($all_meetings, $DateTimeClass->timeStampYearFirstDay(), $DateTimeClass->timeStampToday());
    $ytd_meetings_by_user = $NodeQuery->meetingNidsBySpeakers(array($uid), $ytd_meetings);

    $city_tid_count = $NodeQuery->meetingNidsCountCityTid($meetings_by_user);
    $ytd_city_tid_count = $NodeQuery->meetingNidsCountCityTid($ytd_meetings_by_user);

    $evaluations_by_user = $NodeQuery->evaluationNidsByMeeting($meetings_by_user);
    $ytd_evaluations_by_user = $NodeQuery->evaluationNidsByMeeting($ytd_meetings_by_user);

    $EvaluationFieldQuery = new EvaluationFieldQuery();
    $question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($evaluations_by_user, 8, 64);
    $ytd_question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($ytd_evaluations_by_user, 8, 64);

    // get program term tree
    $terms = taxonomy_get_tree(2);

    // only filter current program_tids
    // program_table
    $program_table = array();
    if (is_array($meetings_by_user)) {
      if (count($meetings_by_user) > 0) {
        if (is_array($terms)) {
          foreach ($terms as $key => $term) {
            $meetings_by_term = $NodeQuery->meetingNidsByPrograms(array($term->tid), $meetings_by_user);

            if (count($meetings_by_term) > 0) {
              $evaluations_by_term = $NodeQuery->evaluationNidsByMeeting($meetings_by_term);
              $question_8_answer_set_by_term = $EvaluationFieldQuery->questionFieldValues($evaluations_by_term, 8, 64);

              $TermInfo = new TermInfo($term->tid);
              $program_table[] = array(
                $TermInfo->termName(),
                count($meetings_by_term),
                number_format(BasicClass::arrayAverage($question_8_answer_set_by_term), 2),
              );
            }
          }
        }
      }
    }

    // ytd_program_table
    $ytd_program_table = array();
    if (is_array($ytd_meetings_by_user)) {
      if (count($ytd_meetings_by_user) > 0) {
        if (is_array($terms)) {
          foreach ($terms as $key => $term) {
            $ytd_meetings_by_term = $NodeQuery->meetingNidsByPrograms(array($term->tid), $ytd_meetings_by_user);

            if (count($ytd_meetings_by_term) > 0) {
              $ytd_evaluations_by_term = $NodeQuery->evaluationNidsByMeeting($ytd_meetings_by_term);
              $ytd_question_8_answer_set_by_term = $EvaluationFieldQuery->questionFieldValues($ytd_evaluations_by_term, 8, 64);

              $TermInfo = new TermInfo($term->tid);
              $ytd_program_table[] = array(
                $TermInfo->termName(),
                count($ytd_meetings_by_term),
                number_format(BasicClass::arrayAverage($ytd_question_8_answer_set_by_term), 2),
              );
            }
          }
        }
      }
    }

    // location table
    $location_table = array();
    if (is_array($city_tid_count)) {
      foreach ($city_tid_count as $city_tid => $count_value) {
        $TermCityInfo = new TermCityInfo($city_tid);

        $city_meetings = $NodeQuery->meetingNidsByCityTids(array($city_tid), $meetings_by_user);
        $city_evaluations = $NodeQuery->evaluationNidsByMeeting($city_meetings);
        $city_question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($city_evaluations, 8, 64);

        $location_table[] = array(
          $TermCityInfo->termName(),
          $TermCityInfo->provinceNameAbbr(),
          count($city_evaluations),
          number_format(BasicClass::arrayAverage($city_question_8_answer_set), 2),
        );
      }
    }

    // YTD location table
    $ytd_location_table = array();
    if (is_array($ytd_city_tid_count)) {
      foreach ($ytd_city_tid_count as $ytd_city_tid => $ytd_count_value) {
        $TermCityInfo = new TermCityInfo($ytd_city_tid);

        $ytd_city_meetings = $NodeQuery->meetingNidsByCityTids(array($ytd_city_tid), $ytd_meetings_by_user);
        $ytd_city_evaluations = $NodeQuery->evaluationNidsByMeeting($ytd_city_meetings);
        $ytd_city_question_8_answer_set = $EvaluationFieldQuery->questionFieldValues($ytd_city_evaluations, 8, 64);

        $ytd_location_table[] = array(
          $TermCityInfo->termName(),
          $TermCityInfo->provinceNameAbbr(),
          count($ytd_city_evaluations),
          number_format(BasicClass::arrayAverage($ytd_city_question_8_answer_set), 2),
        );
      }
    }

    $output = array(
      'pageTitle' => array(
        'pageTitle' => $UserInfo->userName(),
        'class' => 'font-size-16',
      ),
      'pageTabs' => array(
        array(
          'tabTitle' => t('ALL TIME'),
          'class' => 'font-size-12 active',
          'tabContent' => array(
            'widget' => array(
              array(
                'value' => count($meetings_by_user),
                'label' => t('EVENTS'),
              ),
              array(
                'value' => count($evaluations_by_user),
                'label' => t('EVALUATIONS'),
              ),
              array(
                'value' => number_format(BasicClass::arrayAverage($question_8_answer_set), 2),
                'label' => t('RATING'),
              ),
              array(
                'value' => '$' . number_format($NodeQuery->meetingNidsCountHonorarium($meetings_by_user)),
                'label' => t('HONORARIUM'),
              ),
            ),
            'contentTabs' => array(
              array(
                'tabTitle' => t('PROGRAMS'),
                'class' => 'font-size-12 active',
                'tabContent' => array(
                  'table' => array(
                    'thead' => array(
                      array(
                        'label' => "Title",
                        'width' => "50%",
                      ),
                      array(
                        'label' => "Events",
                        'width' => "10%",
                      ),
                      array(
                        'label' => "Rating",
                        'width' => "10%",
                      ),
                    ),
                    'tbody' => $program_table,
                  ),
                ),
              ),
              array(
                'tabTitle' => t('LOCATIONS'),
                'class' => 'font-size-12',
                'tabContent' => array(
                  'table' => array(
                    'thead' => array(
                      array(
                        'label' => "City",
                        'width' => "30%",
                      ),
                      array(
                        'label' => "Prov",
                        'width' => "10%",
                      ),
                       array(
                        'label' => "Evaluations",
                        'width' => "10%",
                      ),
                      array(
                        'label' => "Rating",
                        'width' => "10%",
                      ),
                    ),
                    'tbody' => $location_table,
                  ),
                ),
              ),
            ),
          ),
        ),

        array(
          'tabTitle' => t('YTD'),
          'class' => 'font-size-12 active',
          'tabContent' => array(
            'widget' => array(
              array(
                'value' => count($ytd_meetings_by_user),
                'label' => t('EVENTS'),
              ),
              array(
                'value' => count($ytd_evaluations_by_user),
                'label' => t('EVALUATIONS'),
              ),
              array(
                'value' => number_format(BasicClass::arrayAverage($ytd_question_8_answer_set), 2),
                'label' => t('RATING'),
              ),
              array(
                'value' => '$' . number_format($NodeQuery->meetingNidsCountHonorarium($ytd_meetings_by_user)),
                'label' => t('HONORARIUM'),
              ),
            ),
            'contentTabs' => array(
              array(
                'tabTitle' => t('PROGRAMS'),
                'class' => 'font-size-12 active',
                'tabContent' => array(
                  'table' => array(
                    'thead' => array(
                      array(
                        'label' => "Title",
                        'width' => "50%",
                      ),
                      array(
                        'label' => "Events",
                        'width' => "10%",
                      ),
                      array(
                        'label' => "Rating",
                        'width' => "10%",
                      ),
                    ),
                    'tbody' => $ytd_program_table,
                  ),
                ),
              ),
              array(
                'tabTitle' => t('LOCATIONS'),
                'class' => 'font-size-12',
                'tabContent' => array(
                  'table' => array(
                    'thead' => array(
                      array(
                        'label' => "City",
                        'width' => "30%",
                      ),
                      array(
                        'label' => "Prov",
                        'width' => "10%",
                      ),
                       array(
                        'label' => "Evaluations",
                        'width' => "10%",
                      ),
                      array(
                        'label' => "Rating",
                        'width' => "10%",
                      ),
                    ),
                    'tbody' => $ytd_location_table,
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    );

    return $output;
  }
}

/**
 * class
   $JsonData = new JsonData();
   $JsonData->sourceArrayData();
 *
 */
class JsonData extends JsonDataUnit {
  /** - - - - - - node - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @return Term Array data
   */
  public function jsonNodeEvaluationFormQuestionSet($meeting_nid = NULL) {
    $MeetingInfo = new MeetingInfo($meeting_nid);

    $output = $this->termEvaluationFormQuestionSet($MeetingInfo->evaluationFormTid());
    $output['meeting'] = $this->nodeMeetingInfo($meeting_nid);
    $output['pageWidgets'] = $this->blockWidgetMeeting($meeting_nid);

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonNodeEventList($program_tids = array(), $start_time = NULL, $end_time = NULL) {
    $NodeQuery = new NodeQuery();
    $all_meetings = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);
    $meetings = $NodeQuery->meetingNidsByPrograms($program_tids, $all_meetings);

    $output = NULL;
    $output['nodeEventList'] = $this->nodeEventList($meetings);

    return $output;
  }

  public function jsonNodeMeetingDetail($nid = NULL) {
    $MeetingInfo = new MeetingInfo($nid);
    $program_tid = $MeetingInfo->programTid();
    $meetings = array($nid);

    $output = $this->nodeMeetingDetailContent($meetings, $program_tid);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonNodeMeetingEdit($nid = NULL) {
    $output = $this->nodeMeetingEdit($nid);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function nodeMeetingCreate() {
    $output = NULL;
    $output['evaluationForm'] = $this->termEvaluationForm();
    $output['businessUnitTree'] = $this->termBusinessUnitTree();
    $output['meetingType'] = $this->termMeetingType();
    $output['meetingReceived'] = $this->termMeetingReceived();
    $output['multiTherape'] = $this->termMultiTherape();
    $output['program'] = $this->termProgram();
    $output['programClass'] = $this->termProgramClass();
    $output['programType'] = $this->termProgramType();
    $output['programTree'] = $this->termProgramTree();
    $output['speakerUserList'] = $this->speakerUserList();
    $output['repUserList'] = $this->repUserList();

    return $output;
  }

  /** - - - - - - page - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @return Term Array data
   */
  public function jsonPageLanding($program_tids = array(), $start_time = NULL, $end_time = NULL) {
    $NodeQuery = new NodeQuery();
    $all_meetings = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);
    $meetings = $NodeQuery->meetingNidsByPrograms($program_tids, $all_meetings);

    $output = $this->pageLanding($meetings);

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonPageMeetingMap($program_tids = array(), $start_time = NULL, $end_time = NULL) {
    $NodeQuery = new NodeQuery();
    $all_meetings = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);
    $meetings = $NodeQuery->meetingNidsByPrograms($program_tids, $all_meetings);

    $output = $this->pageMeetingMap($meetings);
    return $output;
  }

  /** - - - - - - Program - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @return Term Array data
   */
  public function jsonProgramSnapshot($program_tids = array(), $start_time = NULL, $end_time = NULL) {
    $NodeQuery = new NodeQuery();

    $all_meetings = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);
    $meetings = $NodeQuery->meetingNidsByPrograms($program_tids, $all_meetings);
    $program_tid = current($program_tids);

    $output = $this->programSnapshot($meetings, $program_tid);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonProgramlist() {
    $output = NULL;
    $output['programTree'] = $this->termProgramList();

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonBusinessUnitSnapshot($program_tids = array(), $start_time = NULL, $end_time = NULL) {
    $NodeQuery = new NodeQuery();
    $all_meetings = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);
    $meetings = $NodeQuery->meetingNidsByPrograms($program_tids, $all_meetings);

    $output = NULL;
    switch (arg(4)) {
      case 2088:
        $output = $this->businessUnitSnapshotBiomedicines($meetings);
        break;
      case 72:
        $output = $this->businessUnitSnapshotDiabetes($meetings);
        break;

      default:
        break;
    }

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonDivisonSnapshot($program_tids = array(), $start_time = NULL, $end_time = NULL) {
    $NodeQuery = new NodeQuery();
    $all_meetings = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);
    $meetings = $NodeQuery->meetingNidsByPrograms($program_tids, $all_meetings);

    // go to Therapeutic Area
    $output = $this->therapeuticAreaSnapshot($meetings);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonTherapeuticAreaSnapshot($program_tids = array(), $start_time = NULL, $end_time = NULL) {
    $NodeQuery = new NodeQuery();
    $all_meetings = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);
    $meetings = $NodeQuery->meetingNidsByPrograms($program_tids, $all_meetings);

    $output = $this->therapeuticAreaSnapshot($meetings);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonProgramUnitProgram($program_tids = array(), $start_time = NULL, $end_time = NULL) {
    $NodeQuery = new NodeQuery();
    $all_meetings = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);
    $meetings = $NodeQuery->meetingNidsByPrograms($program_tids, $all_meetings);

    $TermProgramInfo = new TermProgramInfo(current($program_tids));

    $output = NULL;
    $output['programTableHeader'] = $TermProgramInfo->programUnitName() . ' ' . t('Program');
    $output['pageWidgets'] = $this->pageWidgets($meetings);
    $output['tablePrograms'][] = array(
      'tabLabel' => t('ALL PROGRAM'),
      'tableContent' => $this->tableProgramUnitProgram($meetings),
    );

    // only show underneath Oncology
    if (arg(4) == 2090) {
      $TermProgramQuery = new TermProgramQuery();
      $asthma_meetings = $NodeQuery->meetingNidsByPrograms($asthma_program_tids, $meetings);
    }

    return $output;
  }

  /** - - - - - - program is special term - - - - - - - - - - - - - - - - -  */
  /**
   * @return Term Array data. for admin page to create/edit program
   */
  public function programData() {
    $output = NULL;
    $output['brand'] = $this->termBrand();
    $output['businessUnit'] = $this->termBusinessUnit();
    $output['businessUnitTree'] = $this->termBusinessUnitTree();
    $output['evaluationForm'] = $this->termEvaluationForm();
    $output['programClass'] = $this->termprogramClass();
    $output['programType'] = $this->termProgramType();
    $output['therapeuticArea'] = $this->termTherapeuticArea();
    $output['division'] = $this->termDivision();

    return $output;
  }

  /** - - - - - - table - - - - - - - - - - - - - - - - - - - - - - - -  */

  /**
   * @return Term Array data
   */
  public function jsonTableProgram($program_tids = array(), $start_time = NULL, $end_time = NULL) {
    $NodeQuery = new NodeQuery();
    $all_meetings = $NodeQuery->meetingNidsByTime($NodeQuery->meetingNids(), $start_time, $end_time);
    $meetings = $NodeQuery->meetingNidsByPrograms($program_tids, $all_meetings);

    $output = NULL;
    $output['tableEventTracker'] = $this->tableEventTracker($meetings);
    $output['tableSpeakerEvaluations'] = $this->tableSpeakerEvaluations($meetings);
    $output['tableTopRatedSpeakers'] = $this->tableTopRatedSpeakers($meetings);
    $output['tableTopAccreditedProgram'] = $this->tableTopAccreditedProgram($meetings);

    return $output;
  }

  /** - - - - - - term - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @return Term Array data
   */
  public function termQuestionFieldCreate() {
    $output = NULL;

    $output['questionField'] = $this->termStandardJson(3, 'TermQuestionFieldInfo', TRUE);
    $output['questionLabel'] = $this->termStandardJson(5, 'TermQuestionLabelInfo', TRUE);

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function questionLibrary() {
    $output = NULL;
    $output['questionLibrary'] = $this->termQuestionLibrary();

    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonTermBusinessUnitEdit($tid = NULL) {
    $output = $this->termBusinessUnitEdit($tid);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonTermEvaluationFormEdit($tid = NULL) {
    $output = $this->termEvaluationFormEdit($tid);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonTermEvaluationFormQuestionSet($tid = NULL) {
    $output = $this->termEvaluationFormQuestionSet($tid);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonTermEventRegionFilterList() {
    $output = $this->termEventRegionFilterList();
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonTermEventHubFilterList($event_region_tid = NULL) {
    $output = $this->termEventHubFilterList($event_region_tid);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonTermProgramEdit($tid = NULL) {
    $output = $this->termProgramEdit($tid);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonTermQuestionLibraryEdit($tid = NULL) {
    $output = $this->termQuestionLibraryEdit($tid);
    return $output;
  }

  /**
   * @return Term Array data
   */
  public function jsonTermQuestionLibraryFieldOption() {
    $output['dashpage']['questionLibraryForm'] = $this->termQuestionLibraryFieldOption();
    return $output;
  }
  /**
   * @return Term Array data
   */
  public function jsonTermQuestionLibraryTable() {
    $output = $this->termQuestionLibrary();
    return $output;
  }

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
