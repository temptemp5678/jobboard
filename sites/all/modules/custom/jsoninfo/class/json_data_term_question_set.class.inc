<?php
/**
 * class
   $TermProgramQuestionBasic = new TermProgramQuestionBasic();
   TermProgramQuestionBasic->programSet();
 *
 */
class TermProgramQuestionBasic {
  protected $bg_class = 'bg-ffffff';
  protected $charts_column_6 = 6;

  protected $charts_type = 'piechart';
  protected $charts_type_class = 'chart-piechart';


  protected $color_class = array('#00a9e0', '#05d23e', '#f24b99', '#bfbfbf', '#e6e6e6');

  protected function legend() {
    $output = array(t('Very Effective'), t('Effective'), t('Neutral'), t('Ineffective'), t('Very Ineffective'));
    return $output;
  }
  protected function legendAgree() {
    $output = array(t('Strongly Agree'), t('Agree'), t('Neutral'), t('Disagree'), t('Strongly Disagree'));
    return $output;
  }
  protected function legendEffective() {
    $output = array(t('Very Effective'), t('Effective'), t('Neutral'), t('Ineffective'), t('Very Ineffective'));
    return $output;
  }
  protected function legendExcellent() {
    $output = array(t('Excellent'), t('Very Good'), t('Good'), t('Fair'), t('Poor'));
    return $output;
  }
  protected function legendLikely() {
    $output = array(t('Very Likely'), t('Likely'), t('Neutral'), t('Unlikely'), t('Very Unlikely'));
    return $output;
  }

  protected function subLegend1() {
    return t('RESPONSES');
  }
  protected function subLegend2() {
    return $legend[0] . ' ' . t('OR') . ' ' . $legend[1];
  }
  protected function subLegendAgree2() {
    $subLegend2 = $this->legendAgree();
    return $subLegend2[0] . ' ' . t('OR') . ' ' . $subLegend2[1];
  }
  protected function subLegendEffective2() {
    $subLegend2 = $this->legendEffective();
    return $subLegend2[0] . ' ' . t('OR') . ' ' . $subLegend2[1];
  }
  protected function subLegendExcellent2() {
    $subLegend2 = $this->legendExcellent();
    return $subLegend2[0] . ' ' . t('OR') . ' ' . $subLegend2[1];
  }
  protected function subLegendLikely2() {
    $subLegend2 = $this->legendLikely();
    return $subLegend2[0] . ' ' . t('OR') . ' ' . $subLegend2[1];
  }

}

/**
 *
  $TermProgramQuestionSet = new TermProgramQuestionSet();
  $TermProgramQuestionSet->programSet2243();
 */
class TermProgramQuestionSet extends TermProgramQuestionBasic {
  public function programSetContent($tid = NULL) {
    $output = NULL;
    if ($tid) {
      $method_name = 'programSet' . $tid;
      if ($this->{$method_name}()) {
        $output = $this->{$method_name}();
      }
    }

    return $output;
  }

  /**
   * @return Term Array data
   * Spotlight on SGLT2 inhibitors --- 2124
   */
  public function programSet2124() {
    $legend = $this->legendExcellent();

    $question_set = array(
      array(
        'questionTid' => 8,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Speaker Effectiveness'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 4,
        'legend' => $this->legendEffective(),
        'subLegend2' => $this->subLegendEffective2(),
      ),
      array(
        'questionTid' => 5,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Clinical Practice Impact'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 4,
        'legend' => $this->legendLikely(),
        'subLegend2' => $this->subLegendLikely2(),
      ),
      array(
        'questionTid' => 46,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Overall Satisfaction'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 4,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
      // array(
      //   'questionTid' => 2193,        // not sure question
      //   'questionFid' => 64,
      //   'questionAnswerArray' => array(5, 4, 3, 2, 1),
      //   'caption' => t('Clinical Practice Impact'),
      //   'chartType' => 'piechart',
      //   'chartTypeClass' => 'chart-piechart',
      //   'column' => 6,
      //   'legend' => $this->legendLikely(),
      //   'subLegend2' => $subLegend2,
      // ),
    );

    foreach ($question_set as $value) {
      $output[] = array(
        'questionTid' => $value['questionTid'],
        'questionFid' => $value['questionFid'],
        'questionAnswerArray' => $value['questionAnswerArray'],
        'caption' => $value['caption'],
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => '',
        'captionLegend' => '',
        'captionTitle' => '',
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => $value['chartType'],
            'chartTypeClass' => $value['chartTypeClass'],
            'colorClass' => $this->color_class,
            'data' => '',
            'legend' => $value['legend'],
            'widget' => array(
              'bgColorClass1' => $this->bg_class,
              'bgColorClass2' => $this->bg_class,
              'subLabel1' => $this->subLegend1(),
              'subValue1' => '',
              'subLabel2' => drupal_strtoupper($value['subLegend2']),
              'subValue2' => '',
              'title' => '',
              'value' => '',
            ),
          ),
        ),
        'column' => $value['column'],
      );
    }

    return $output;
  }

  /**
   * @return Term Array data
   * Update on Diagnosis & management of IPF --- 2238
   */
  public function programSet2238() {
    $legend = $this->legendExcellent();

    $question_set = array(
      array(
        'questionTid' => 2204,     // not sure question
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Speaker Effectiveness'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 4,
        'legend' => $this->legendEffective(),
        'subLegend2' => $this->subLegendEffective2(),
      ),
      array(
        'questionTid' => 5,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Clinical Practice Impact'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 4,
        'legend' => $this->legendLikely(),
        'subLegend2' => $this->subLegendLikely2(),
      ),
      array(
        'questionTid' => 2209,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Overall Satisfaction'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 4,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
    );

    foreach ($question_set as $value) {
      $output[] = array(
        'questionTid' => $value['questionTid'],
        'questionFid' => $value['questionFid'],
        'questionAnswerArray' => $value['questionAnswerArray'],
        'caption' => $value['caption'],
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => '',
        'captionLegend' => '',
        'captionTitle' => '',
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => $value['chartType'],
            'chartTypeClass' => $value['chartTypeClass'],
            'colorClass' => $this->color_class,
            'data' => '',
            'legend' => $value['legend'],
            'widget' => array(
              'bgColorClass1' => $this->bg_class,
              'bgColorClass2' => $this->bg_class,
              'subLabel1' => $this->subLegend1(),
              'subValue1' => '',
              'subLabel2' => drupal_strtoupper($value['subLegend2']),
              'subValue2' => '',
              'title' => '',
              'value' => '',
            ),
          ),
        ),
        'column' => $value['column'],
      );
    }

    return $output;
  }

  /**
   * @return Term Array data
   * Huffing and Puffing? New Approaches for Severe Adult Asthma -- 2243
   */
  public function programSet2243() {
    $charts_column = 4;
    $legend = $this->legendExcellent();

    $question_set = array(
      array(
        'questionTid' => 2244,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Overall Program Quality'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => $charts_column,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
      array(
        'questionTid' => 2245,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Quality of Content'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => $charts_column,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
      array(
        'questionTid' => 2246,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Met Educational Objectives'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => $charts_column,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
      array(
        'questionTid' => 2247,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Relevance to Clinical Practice'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
      array(
        'questionTid' => 2248,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Program Balance'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
      array(
        'questionTid' => 2249,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Sufficient Time for Discussion'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
      array(
        'questionTid' => 8,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Speaker Effectiveness'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
      array(
        'questionTid' => 2131,
        'questionFid' => 64,
        'questionAnswerArray' => array('No', 'Yes'),
        'caption' => t('Commercial Bias'),
        'chartType' => 'gauge',
        'chartTypeClass' => 'chart-gauge',
        'column' => 6,
        'legend' => '',
        'subLegend2' => t('NO BIAS'),
      ),
      array(
        'questionTid' => 2251,
        'questionFid' => 64,
        'questionAnswerArray' => array('No', 'Yes'),
        'caption' => t('Speaker Bias'),
        'chartType' => 'gauge',
        'chartTypeClass' => 'chart-gauge',
        'column' => 6,
        'legend' => '',
        'subLegend2' => t('NO BIAS'),
      ),
    );

    foreach ($question_set as $value) {
      $output[] = array(
        'questionTid' => $value['questionTid'],
        'questionFid' => $value['questionFid'],
        'questionAnswerArray' => $value['questionAnswerArray'],
        'caption' => $value['caption'],
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => '',
        'captionLegend' => '',
        'captionTitle' => '',
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => $value['chartType'],
            'chartTypeClass' => $value['chartTypeClass'],
            'colorClass' => $this->color_class,
            'data' => '',
            'legend' => $value['legend'],
            'widget' => array(
              'bgColorClass1' => $this->bg_class,
              'bgColorClass2' => $this->bg_class,
              'subLabel1' => $this->subLegend1(),
              'subValue1' => '',
              'subLabel2' => drupal_strtoupper($value['subLegend2']),
              'subValue2' => '',
              'title' => '',
              'value' => '',
            ),
          ),
        ),
        'column' => $value['column'],
      );
    }

    return $output;
  }

  /**
   * @return Term Array data
   * Respiratory Updates: Visiting New and Current Data -- 2263
   */
  public function programSet2263() {
    $legend = $this->legendExcellent();

    $question_set = array(
      array(
        'questionTid' => 2255,
        'questionFid' => 64,
        'questionAnswerArray' => array('Yes', 'No'),
        'caption' => t('Meet Learning Objectives'),
        'chartType' => 'doughnut',
        'chartTypeClass' => 'chart-donut',
        'column' => 4,
        'legend' => array(t('Yes'), t('No')),
        'subLegend2' => t('YES'),
      ),
      array(
        'questionTid' => 2256,
        'questionFid' => 64,
        'questionAnswerArray' => array('Yes', 'No'),
        'caption' => t('Sufficient Time for Discussion'),
        'chartType' => 'doughnut',
        'chartTypeClass' => 'chart-donut',
        'column' => 4,
        'legend' => array(t('Yes'), t('No')),
        'subLegend2' => t('YES'),
      ),
      array(
        'questionTid' => 2257,
        'questionFid' => 64,
        'questionAnswerArray' => array('Yes', 'No'),
        'caption' => t('Disclosures Presented'),
        'chartType' => 'doughnut',
        'chartTypeClass' => 'chart-donut',
        'column' => 4,
        'legend' => array(t('Yes'), t('No')),
        'subLegend2' => t('YES'),
      ),
      array(
        'questionTid' => 2133,
        'questionFid' => 64,
        'questionAnswerArray' => array('Yes', 'No'),
        'caption' => t('Compliance with Code of Ethics'),
        'chartType' => 'gauge',
        'chartTypeClass' => 'chart-gauge',
        'column' => 6,
        'legend' => '',
        'subLegend2' => t('NO BIAS'),
      ),
      array(
        'questionTid' => 2261,
        'questionFid' => 64,
        'questionAnswerArray' => array('Yes', 'No'),
        'caption' => t('Commercial Bias'),
        'chartType' => 'gauge',
        'chartTypeClass' => 'chart-gauge',
        'column' => 6,
        'legend' => '',
        'subLegend2' => t('NO BIAS'),
      ),
      array(
        'questionTid' => 2258,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Clarity of Presentation'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
      array(
        'questionTid' => 2259,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Educational Value'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendExcellent(),
        'subLegend2' => $this->subLegendExcellent2(),
      ),
    );

    foreach ($question_set as $value) {
      $output[] = array(
        'questionTid' => $value['questionTid'],
        'questionFid' => $value['questionFid'],
        'questionAnswerArray' => $value['questionAnswerArray'],
        'caption' => $value['caption'],
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => '',
        'captionLegend' => '',
        'captionTitle' => '',
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => $value['chartType'],
            'chartTypeClass' => $value['chartTypeClass'],
            'colorClass' => $this->color_class,
            'data' => '',
            'legend' => $value['legend'],
            'widget' => array(
              'bgColorClass1' => $this->bg_class,
              'bgColorClass2' => $this->bg_class,
              'subLabel1' => $this->subLegend1(),
              'subValue1' => '',
              'subLabel2' => drupal_strtoupper($value['subLegend2']),
              'subValue2' => '',
              'title' => '',
              'value' => '',
            ),
          ),
        ),
        'column' => $value['column'],
      );
    }

    return $output;
  }

  // 2268 learning objective

  /**
   * @return Term Array data
   * Changing the face of Anticoagulation: Reversing the NOAC Effect. What are the options? -- 2283
   */
  public function programSet2283() {
    $legend = $this->legendAgree();

    $question_set = array(
      array(
        'questionTid' => 2277,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Information was appropriate'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 4,
        'legend' => $this->legendAgree(),
        'subLegend2' => $this->subLegendAgree2(),
      ),
      array(
        'questionTid' => 2278,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Information was current and relevant'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 4,
        'legend' => $this->legendAgree(),
        'subLegend2' => $this->subLegendAgree2(),
      ),
      array(
        'questionTid' => 2131,
        'questionFid' => 64,
        'questionAnswerArray' => array('No', 'Yes'),
        'caption' => t('Speaker or Promotional Bias'),
        'chartType' => 'doughnut',
        'chartTypeClass' => 'chart-donut',
        'column' => 4,
        'legend' => array(t('Yes'), t('No')),
        'subLegend2' => t('NO BIAS'),
      ),
      array(
        'questionTid' => 2279,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Speakers were knowledgeable'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendAgree(),
        'subLegend2' => $this->subLegendAgree2(),
      ),
      array(
        'questionTid' => 2280,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t("Speaker's message was clear"),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendAgree(),
        'subLegend2' => $this->subLegendAgree2(),
      ),
      array(
        'questionTid' => 2281,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Speakers were responsive to questions'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendAgree(),
        'subLegend2' => $this->subLegendAgree2(),
      ),
      array(
        'questionTid' => 8,
        'questionFid' => 64,
        'questionAnswerArray' => array(5, 4, 3, 2, 1),
        'caption' => t('Speakers Effectiveness'),
        'chartType' => 'piechart',
        'chartTypeClass' => 'chart-piechart',
        'column' => 6,
        'legend' => $this->legendAgree(),
        'subLegend2' => $this->subLegendAgree2(),
      ),

    );

    foreach ($question_set as $value) {
      $output[] = array(
        'questionTid' => $value['questionTid'],
        'questionFid' => $value['questionFid'],
        'questionAnswerArray' => $value['questionAnswerArray'],
        'caption' => $value['caption'],
        'captionBgColorClass' => 'bg-ffffff',
        'captionColor' => '',
        'captionLegend' => '',
        'captionTitle' => '',
        'chartGroup' => '1',
        'charts' => array(
          array(
            'chartType' => $value['chartType'],
            'chartTypeClass' => $value['chartTypeClass'],
            'colorClass' => $this->color_class,
            'data' => '',
            'legend' => $value['legend'],
            'widget' => array(
              'bgColorClass1' => $this->bg_class,
              'bgColorClass2' => $this->bg_class,
              'subLabel1' => $this->subLegend1(),
              'subValue1' => '',
              'subLabel2' => drupal_strtoupper($value['subLegend2']),
              'subValue2' => '',
              'title' => '',
              'value' => '',
            ),
          ),
        ),
        'column' => $value['column'],
      );
    }

    return $output;
  }

}
