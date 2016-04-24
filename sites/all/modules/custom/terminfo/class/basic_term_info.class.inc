<?php
/**
 * @file
   $TermInfo = new TermInfo($tid);
   $TermInfo->termName();
 */

/**
 * class
 */
class TermInfo {
  public $tid;
  public $term = NULL;

  /**
   * @parameter is tid or term object
   */
  function __construct($param){

    if (is_numeric($param)) {
      $term = taxonomy_term_load($param);
      if ($term) {
        $this->term = $term;
        $this->tid = $param;
      }
    }
    else if (is_object($param)) {
      if (isset($param->tid)) {
        $term = taxonomy_term_load($param->tid);
        if ($term) {
          $this->term = $term;
          $this->tid = $param;
        }
      }
    }
  }

  /** - - - - - Basic Object - - - - - - - - - -  - - - - -   */
  /**
   * all Methods
   */
  protected function allMethods() {
    $methods = NULL;

    /**
     * similar with  $methods = get_class_methods($this);
     */
    $reflect = new ReflectionClass(get_class($this));
    $methods = $reflect->getMethods(ReflectionProperty::IS_PUBLIC);

    return $methods;
  }

  /**
   * all Methods with filter condition
   */
  protected function allMethodsArray() {
    $output = NULL;

    $methods = $this->allMethods();
    if (is_array($methods)) {
      foreach ($methods as $method) {
        if ($method->name != '__construct') {
          if ($method->name != 'allMethodsValue') {
            $output[] = $method->name;
          }
        }
      }
    }
    return $output;
  }

  /**
   * all Methods value
   * @return, array
     array{
       method1 => value1,
       method2 => value2,
     };
   */
  public function allMethodsValue() {
    $output = array();

    $methods = $this->allMethodsArray();
    if (is_array($methods)) {
      $row = '';
      foreach ($methods as $key => $method) {
        $row[$method] = $this->$method();
      }
      $output = $row;
    }
    return $output;
  }

  /** - - - - - Basic Property - - - - - - - - - -  - - - - -   */
  /**
   * @return Term Name
   */
  public function termTid() {
    $output = NULL;
    if ($this->term) {
      $output = $this->term->tid;
    }
    return $output;
  }
  /**
   * @return Term Name
   */
  public function termName() {
    $output = NULL;
    if ($this->term) {
      $output = $this->term->name;
    }
    return $output;
  }
  /**
   * @return Term Vid
   */
  public function termVid() {
    $output = NULL;
    if ($this->term) {
      $output = $this->term->vid;
    }
    return $output;
  }
  /**
   * @return Term Vid
   */
  public function termVidName() {
    $output = NULL;
    $vocabulary = taxonomy_vocabulary_load($this->termVid());
    if ($vocabulary) {
      $output = $vocabulary->name;
    }
    return $output;
  }
  /**
   * @return Term vocabulary
   */
  public function vocabularyName() {
    $output = NULL;

    $vocabulary = taxonomy_vocabulary_load($this->term->vid);
    if ($vocabulary) {
      $output = $vocabulary->name;
    }
    return $output;
  }

  /**
   * @return Term Description
   */
  public function termDescription() {
    $output = NULL;
    if ($this->term) {
      $output = $this->term->description;
    }
    return $output;
  }

  /**
   * @return Term Description
   */
  public function termEditLink() {
    $output = NULL;
    if ($this->term) {
      $output = base_path() . 'manage/term/edit/' . $this->termVid() . '/' . $this->termTid();
    }
    return $output;
  }

}
/**
 * class
   $TermClientInfo = new TermClientInfo($tid);
   $TermClientInfo->userExistClientManage($uid);
 */
class TermClientInfo extends TermInfo {
  /**
   * @return String, contact_person
   */
  public function address() {
    $output = NULL;
    if (isset($this->term->field_client_address['und'][0]['value'])) {
      $output = $this->term->field_client_address['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return array, integer, client Manage User UID Array
   */
  public function clientManageUserArray() {
    $output = NULL;

    if (isset($this->term->field_client_manage['und'][0]['target_id'])) {
      foreach ($this->term->field_client_manage['und'] as $key => $value) {
        $output[] = $value['target_id'];
      }
    }
    return $output;
  }
  /**
   * @return Boolean,
   * check user is exist on Client Manage or not
   */
  public function userExistClientManage($uid = NULL) {
    $output = FALSE;

    if (is_array($this->clientManageUserArray())) {
      if (in_array($uid, $this->clientManageUserArray())) {
        $output = TRUE;
      }
    }
    return $output;
  }

  /**
   * @return String, contact_person
   */
  public function postcode() {
    $output = NULL;
    if (isset($this->term->field_client_postcode['und'][0]['value'])) {
      $output = $this->term->field_client_postcode['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String, Term Property
   */
  public function provinceTid() {
    $output = NULL;
    if (isset($this->term->field_client_province['und'][0]['target_id'])) {
      $output = $this->term->field_client_province['und'][0]['target_id'];
    }
    return $output;
  }

  /**
   * @return String, Term Property
   */
  public function provinceName() {
    $output = NULL;

    $TermInfo = new TermInfo($this->provinceTid());
    if(!empty($TermInfo)) {
      $output = $TermInfo->termName();
    }

    return $output;
  }
  /** - - - - - - other - - - - - - - - - - - - - - - - - - - - - - - - - -  */

}

/**
 * class
   $TermCompanyInfo = new TermCompanyInfo($tid);
   $TermCompanyInfo->consigneeAdd($uid);
 */
class TermCompanyInfo extends TermInfo {
  /**
   * @return String, consignee address
   */
  public function companyTitle() {
    $output = NULL;

    // global language
    global $language;
    if (isset($this->term->field_company_title[$language->language][0]['value'])) {
      $output = $this->term->field_company_title[$language->language][0]['value'];
    }
    return $output;
  }
  /**
   * @return String, consignee address
   */
  public function address() {
    $output = NULL;

    global $language;

    if (isset($this->term->field_company_address[$language->language][0]['value'])) {
      $output = $this->term->field_company_address[$language->language][0]['value'];
    }
    return $output;
  }

  /**
   * @return String, contact_person
   */
  public function contact() {
    $output = NULL;
    if (isset($this->term->field_company_contact['und'][0]['value'])) {
      $output = $this->term->field_company_contact['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String, Fax No.
   */
  public function fax() {
    $output = NULL;
    if (isset($this->term->field_company_fax['und'][0]['value'])) {
      $output = $this->term->field_company_fax['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String, Phone No.
   */
  public function phone() {
    $output = NULL;
    if (isset($this->term->field_company_phone['und'][0]['value'])) {
      $output = $this->term->field_company_phone['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String, stamp.
   */
  public function stampFid() {
    $output = NULL;
    if (isset($this->term->field_company_stamp['und'][0]['fid'])) {
      $output = $this->term->field_company_stamp['und'][0]['fid'];
    }
    return $output;
  }
  /**
   * @return String, stamp.
   */
  public function stampImage() {
    $output = NULL;
    if($this->stampFid()){
      $file = file_load($this->stampFid());
      $picture_url = $file->uri;
      $image = array(
        'path' => $picture_url,
        'width' => '150',
        'height' => '150',
        'alt' => 'image',
        'attributes' => array('class' => 'stamp-image'),
      );
      $output = theme('image', $image);
    }
    return $output;
  }

}

/**
 * class
   $TermContactInfo = new TermContactInfo($tid);
   $TermContactInfo->email();
 */
class TermContactInfo extends TermInfo {
  /**
   * @return String, Term Property
   */
  public function clientTid() {
    $output = NULL;
    if (isset($this->term->field_contact_client['und'][0]['target_id'])) {
      $output = $this->term->field_contact_client['und'][0]['target_id'];
    }
    return $output;
  }

  /**
   * @return String, Term Property
   */
  public function clientName() {
    $output = NULL;

    $TermInfo = new TermInfo($this->clientTid());
    if(!empty($TermInfo)) {
      $output = $TermInfo->termName();
    }

    return $output;
  }
  /**
   * @return String, Fax No.
   */
  public function email() {
    $output = NULL;
    if (isset($this->term->field_contact_email['und'][0]['value'])) {
      $output = $this->term->field_contact_email['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String, Fax No.
   */
  public function fax() {
    $output = NULL;
    if (isset($this->term->field_contact_fax['und'][0]['value'])) {
      $output = $this->term->field_contact_fax['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String, Mobile No.
   */
  public function mobile() {
    $output = NULL;
    if (isset($this->term->field_contact_mobile['und'][0]['value'])) {
      $output = $this->term->field_contact_mobile['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return Array, Notes
   */
  public function notes() {
    $output = NULL;
    if (isset($this->term->field_contact_note['und'][0]['value'])) {
      foreach ($this->term->field_contact_note['und'] as $key => $value) {
        $output[] = $this->term->field_contact_note['und'][$key]['value'];
      }
    }
    return $output;
  }

  /**
   * @return String, Phone No.
   */
  public function phone() {
    $output = NULL;
    if (isset($this->term->field_contact_phone['und'][0]['value'])) {
      $output = $this->term->field_contact_phone['und'][0]['value'];
    }
    return $output;
  }
  /** - - - - - - other - - - - - - - - - - - - - - - - - - - - - - - - - -  */

}

