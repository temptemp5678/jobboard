<?php

/**
 * callback from hook_menu()
 */
function _jsoninfo_debug_program_unit($tid = NULL, $start_time = NULL, $end_time = NULL) {
  $TermProgramQuery = new TermProgramQuery();
  $program_tids = $TermProgramQuery->programTidsByUnit($tid);

  $JsonData = new JsonData();
  drupal_json_output($JsonData->jsonProgramUnit($program_tids, $start_time, $end_time));
}

/**
 * callback from hook_menu()
 * debug output json for speaker_page
 */
function _jsoninfo_debug_json_speaker_page() {
  $JsonData = new JsonData();
  drupal_json_output(userSpeakerDetailsJson());
}

/**
 * callback from hook_menu()
 * debug output json for speaker_page
 */
function _jsoninfo_debug_json_program_edit() {
  $JsonData = new JsonData();
  drupal_json_output(jsonDebugProgramEdit());
}

/**
 * @return, debug json for speaker_page
 */
function jsonDebugProgramEdit() {
  $output = array(
    'businessUnit' => array(
      'updateStatus' => 0,
      'defaultValue' => 2088,
      'selectedBusinessUnit' => array(
        array('termTid' => 2088, 'termName' => 'Cardiovascular'),
      ),
    ),
    'therapeuticArea' => array(
      'updateStatus' => 0,
      'defaultValue' => array("83", "2093"),
      'selectedTherapeuticArea' => array(
        array('termTid' => 83, 'termName' => 'Alliance'),
        array('termTid' => 2093, 'termName' => 'Asthma'),
      ),
    ),
    'brand' => array(
      'updateStatus' => 0,
      'defaultValue' => array("2100", "2099"),
      'selectedBrand' => array(
        array('termTid' => 2100, 'termName' => 'Giotrif'),
        array('termTid' => 2099, 'termName' => 'Inspiolto'),
      ),
    ),
    'name' => array(
      'updateStatus' => 0,
      'defaultValue' => 'Diabetes in Distinct Populations',
    ),
    'programType' => array(
      'updateStatus' => 0,
      'defaultValue' => 74,
      'selectedProgramType' => array(
        array('termTid' => 74, 'termName' => 'Accredited'),
      ),
    ),
    'programClass' => array(
      'updateStatus' => 0,
      'defaultValue' => array("76", "77"),
      'selectedProgramClass' => array(
        array('termTid' => 76, 'termName' => 'CFPC'),
        array('termTid' => 77, 'termName' => 'FMOQ'),
      ),
    ),
    'evaluationForm' => array(
      'updateStatus' => 0,
      'defaultValue' => array("61", "2188"),
      'selectedEvaluationForm' => array(
        array('termTid' => 61, 'termName' => 'CFPC Evaluation Form'),
        array('termTid' => 2188, 'termName' => 'Diabetes in Distinct Populations Form'),
      ),
    ),
  );

  return $output;
}
/**
 * @return, debug json for speaker_page
 */
function userSpeakerDetailsJson() {
  $output = array(
    'pageTitle' => array(
      'pageTitle' => 'Afshan Zahedi',
      'class' => 'font-size-16',
    ),
    'pageTabs' => array(
      array(
        'tabTitle' => t('ALL TIME'),
        'class' => 'font-size-12 active',
        'tabContent' => array(
          'widget' => array(
            array(
              'value' => 18,
              'label' => t('EVENTS'),
            ),
            array(
              'value' => 322,
              'label' => t('EVALUATIONS'),
            ),
            array(
              'value' => 4.74,
              'label' => t('RATING'),
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
                    )
                  ),
                  'tbody' => array(
                    array('Shortended Program Title', rand(2, 8), 4.65),
                    array('Shortended Program Title', rand(2, 8), 4.65),
                    array('Shortended Program Title', rand(2, 8), 4.65),
                    array('Shortended Program Title', rand(2, 8), 4.65),
                  ),
                 )
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
                    )
                  ),
                  'tbody' => array(
                    array('City Name', 'ON', rand(2, 8), 4.65),
                    array('City Name', 'ON', rand(2, 8), 4.65),
                    array('City Name', 'ON', rand(2, 8), 4.65),
                    array('City Name', 'ON', rand(2, 8), 4.65),
                    array('City Name', 'ON', rand(2, 8), 4.65),
                  ),
                 )
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
              'value' => 18,
              'label' => t('EVENTS'),
            ),
            array(
              'value' => 322,
              'label' => t('EVALUATIONS'),
            ),
            array(
              'value' => 4.74,
              'label' => t('RATING'),
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
                    )
                  ),
                  'tbody' => array(
                    array('Shortended Program Title', rand(2, 8), 4.65),
                    array('Shortended Program Title', rand(2, 8), 4.65),
                    array('Shortended Program Title', rand(2, 8), 4.65),
                    array('Shortended Program Title', rand(2, 8), 4.65),
                  ),
                 )
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
                    )
                  ),
                  'tbody' => array(
                    array('City Name', 'ON', rand(2, 8), 4.65),
                    array('City Name', 'ON', rand(2, 8), 4.65),
                    array('City Name', 'ON', rand(2, 8), 4.65),
                    array('City Name', 'ON', rand(2, 8), 4.65),
                    array('City Name', 'ON', rand(2, 8), 4.65),
                  ),
                 )
              ),
            ),
          ),
        ),
      ),
    ),
  );

  return $output;
}
