<?php

/**
 * callback from hook_menu()
 * debug output json for speaker_page
 */
function _jsoninfo_debug_directive_demo() {
  $JsonData = new JsonData();
  drupal_json_output(_jsonDebugDirectiveContent());
}

/**
 * @return, debug json for speaker_page
 */
function _jsonDebugDirectiveContent() {
  $output = array(
    'directiveName' => 'widget-square',      // chart-pie, chart-bar, table-
    'class' => 'col-md-6 font-size-12 color-fff',
    'data' => array(
      'class' => 'height-280',
      'tabsNum' => 1,
      'tabs' => array(
        array(
          'name' => '',
          'class' => '',
          'content' => array(
            'title' => array(
              'name' => 'Attendee Satisfaction',
              'class' => 'bg-163c52 color-fff height-40',
            ),
            'body' => array(
              'name' => '',
              'class' => 'bg-163c52 color-fff height-240',
              'value' => array(
                'title' => t('Total Events'),
                'value' => 22,
                'bgColorClass1' => 'bg-00a9e0',
                'bgColorClass2' => 'bg-05d23e',
                'subLabel1' => t('Accredited'),
                'subValue1' => 12,
                'subLabel2' => t('OLA'),
                'subValue2' => 10,
              ),
            ),
            'bottom' => array(
              'name' => '',
              'class' => 'bg-163c52 color-fff height-80',
              'widget' => array(
                'title' => t('Total Events'),
                'value' => 22,
                'bgColorClass1' => 'bg-ffffff',
                'bgColorClass2' => 'bg-ffffff',
                'subLabel1' => t('RESPONSES'),
                'subValue1' => 10,
                'subLabel2' => t('VERY EFFECTIVE OR EFFECTIVE'),
                'subValue2' => '20%',
              ),
            ),
          ),
        ),
      ),
    ),
  );

  $output1 = array(
    'directiveName' => 'data-table',      // chart-pie, chart-bar, data-table
    'class' => 'col-md-12 font-size-12 color-fff',
    'data' => array(
      'class' => 'height-280',
      'tabsNum' => 2,
      'tabs' => array(
        array(
          'name' => 'TOP SPEAKERS',
          'class' => 'font-size-14',
          'content' => array(
            'title' => array(
            ),
            'body' => array(
              'name' => 'TOP SPEAKERS',
              'class' => 'bg-163c52 color-fff height-240',
              'column' => 5,
              'thead' => array(
                'Name', 'Event', 'Evaluation', 'Reach', 'Rating',
              ),
              'tbody' => array(
                array('Stuart Ross', 5, 20, 2, 3.5),
                array('Peter Ross', 3, 10, 2, 4.5),
              ),
            ),
            'bottom' => array(
            ),
          ),
        ),
        array(
          'name' => 'TOP PROGRAMS',
          'class' => 'font-size-14',
          'content' => array(
            'title' => array(
            ),
            'body' => array(
              'name' => 'TOP PROGRAMS',
              'class' => 'bg-163c52 color-fff height-240',
              'column' => 6,
              'thead' => array(
                'Program', 'BU', 'Event', 'Evaluation', 'Reach', 'Rating',
              ),
              'tbody' => array(
                array('Diabetes in Distinct Populations', 'Metabolic', 5, 20, 2, 3.5),
                array('New vs Old: Antihyperglycemic Treatment', 'Metabolic', 3, 10, 2, 4.5),
              ),
            ),
            'bottom' => array(
            ),
          ),
        ),
      ),
    ),
  );

  return $output;
}
