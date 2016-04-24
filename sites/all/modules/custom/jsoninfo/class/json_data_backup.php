<?php

function block() {
  // tab two - - Business Unit Event Summary -- on home page
  $tabTwo_total_events_value = array(
    "labels" => array(t("Biomedicines"), t("Diabetes"), t('Oncology')),
    "datasets" => array(
      array(
        "fillColor" => "#00a9e0",
        "strokeColor" => "#00a9e0",
        "pointColor" => "#05d23e",
        "pointStrokeColor" => "#fff",
        "data" => array(
          rand(0, 20),
          rand(0, 20),
          rand(0, 20),
        ),
        "title"=> t("All Events"),
      ),
      array(
        "fillColor" => "#05d23e",
        "strokeColor" => "#05d23e",
        "pointColor" => "#00a9e0",
        "pointStrokeColor" => "#fff",
        "data" => array(
          rand(0, 20),
          rand(0, 20),
          rand(0, 20),
        ),
        "title"=> t("HEM"),
      ),
      array(
        "fillColor" => "#f24b99",
        "strokeColor" => "#f24b99",
        "pointColor" => "#00a9e0",
        "pointStrokeColor" => "#fff",
        "data" => array(
          rand(0, 20),
          rand(0, 20),
          rand(0, 20),
        ),
        "title"=> t('Ind. Sponsorships'),
      ),
      array(
        "fillColor" => "#f3c848",
        "strokeColor" => "#f3c848",
        "pointColor" => "#00a9e0",
        "pointStrokeColor" => "#fff",
        "data" => array(
          rand(0, 20),
          rand(0, 20),
          rand(0, 20),
        ),
        "title"=> t('Ad Boards'),
      ),
      array(
        "fillColor" => "#e6e6e6",
        "strokeColor" => "#e6e6e6",
        "pointColor" => "#00a9e0",
        "pointStrokeColor" => "#fff",
        "data" => array(
          rand(0, 20),
          rand(0, 20),
          rand(0, 20),
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

  // multiple group bar chart
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
          rand(0, 20),  // All BU's
          rand(0, 20),  // Biomedicines
          rand(0, 20),  // Diabetes
          rand(0, 20),  // Oncology
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

  $JsonDataBlock->tabOne(23, t('Business Unit Event Summary'), 'bg-344a5f', $tabTwo);
}

// on Business Unit page
function block() {
  // tab two - - Business Unit Event Summary
  $tabTwo_total_events_value = array(
    'labels' => array(t("Osteoporosis"), t("Men’s Health"), t("Neuroscience"), t("Rheumatology")),
    'title' => "",
    'datasets' => array(
      array(
        "fillColor" => ColorClass::plate(5),
        "strokeColor" => ColorClass::plate(5),
        "pointColor" => "#05d23e",
        "pointStrokeColor" => "#fff",
        "data" => array(
          rand(0, 20),
          rand(0, 20),
          rand(0, 20),
          rand(0, 20),
        ),
        "title"=> t('All Events'),
      ),
    ),
  );
  $tabTwo_total_events_chart = $JsonDataBlock->chartBar(201, t('TOTAL EVENTS'), 'bg-ffffff', $tabTwo_total_events_value);

  $tabTwo_hcp_reach_value = array(
    'labels' => array(t("Rheumatology"), t("Neuroscience"), t("Men’s Health"), t("Osteoporosis")),
    'title' => "",
    'datasets' => array(
      array(
        "fillColor" => ColorClass::plate(5),
        "strokeColor" => ColorClass::plate(5),
        "pointColor" => "#05d23e",
        "pointStrokeColor" => "#fff",
        "data" => array(
          rand(0, 20),
          rand(0, 20),
          rand(0, 20),
          rand(0, 20),
        ),
        "title"=> t('All Events'),
      ),
    ),
  );
  $tabTwo_hcp_reach_chart_options['barValueSpacing'] = 30;
  $tabTwo_hcp_reach_chart = $JsonDataBlock->chartHorizontalBar(202, t('HCP REACH'), 'bg-ffffff', $tabTwo_hcp_reach_value, $tabTwo_hcp_reach_chart_options);

  $tabTwo = array(
    $tabTwo_total_events_chart,
    $tabTwo_hcp_reach_chart,
  );

  $JsonDataBlock->tabOne(23, t('Bio-Medicines Event Summary'), 'bg-344a5f', $tabTwo);
}

// on Business Unit page
function block() {
  $JsonDataBlock->widgetOne(004, t('Active Programs'), 'bg-f3c848', $NodeQuery->meetingNidsCountActiveProgram($meetings)),
}
