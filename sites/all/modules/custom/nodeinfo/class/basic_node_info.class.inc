<?php
/**
 * @file
 */

/**
 * class
   $NodeInfo = new NodeInfo($nid);
   $NodeInfo->nodeTitle();
 */
class NodeInfo {
  public $nid;
  public $node = NULL;

  /**
   * @parameter is nid or node object
   */
  function __construct($para){
    if (is_numeric($para)) {
      $node = node_load($para);
      if ($node) {
        $this->node = $node;
        $this->nid = $this->node->nid;
      }
    }
    else if (is_object($para)) {
      if (isset($para->nid)) {
        $this->node = $para;
        $this->nid = $this->node->nid;
      }
    }
  }
  /** - - - - - - field - - - - - - - - - - - - - - - - - - - - - - - - - -  */
  // public function entityWrapper() {
  //   $output = NULL;
  //   if (isset($this->node->nid)) {
  //     $output = entity_metadata_wrapper('node', $this->node);
  //   }
  //   return $output;
  // }
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
   * @return String, Node Title
   */
  public function nodeNid() {
    $output = NULL;
    if (isset($this->nid)) {
      $output = $this->nid;
    }
    return $output;
  }

  /**
   * @return String, Node Title
   */
  public function nodeTitle() {
    $output = NULL;
    if (isset($this->node->title)) {
      $output = $this->node->title;
    }
    return $output;
  }

  /**
   * @return String, Node Title
   */
  public function nodeBody() {
    $output = NULL;
    if (isset($this->node->body['und'][0]['value'])) {
      $output = $this->node->body['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String, Node Title
   */
  public function nodeUid() {
    $output = $this->node->uid;
    return $output;
  }
  /**
   * @return String,
   */
  public function nodeUidName() {
    $output = '';
    $UserInfo = new UserInfo($this->nodeUid());
    $output = $UserInfo->userName();

    return $output;
  }

  /**
   * @return String, Node type
   */
  public function nodeType() {
    $output = NULL;
    if (isset($this->node->type)) {
      $output = $this->node->type;
    }
    return $output;
  }

  /**
   * @return String,
   */
  public function status() {
    $output = NULL;
    if (isset($this->node->status)) {
      $output = $this->node->status;
    }
    return $output;
  }

  /**
   * @return integer, date unix stamp for quote order time
   */
  public function createdUnixStamp() {
    $output = NULL;
    if (isset($this->node->created)) {
      $output = $this->node->created;
    }
    return $output;
  }
  /**
   * @return integer, date unix stamp
   */
  public function createdValue($format = NULL) {
    if (!$format) {
      $format = "Y-m-d";
    }

    $output = NULL;
    $stamp = $this->createdUnixStamp();
    if ($stamp) {
      $output = format_date($stamp, 'custom', $format);
    }
    return $output;
  }

  /** - - - - - - node link - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @return link, $this->nodeType()
   */
  public function nodeEditLink() {
    $output = base_path() . 'manage/node/edit/' . $this->nid;
    return $output;
  }
  /**
   * @return integer, date unix stamp
   */
  public function nodeViewLink() {
    $output = base_path() . $this->nodeType() . '/node/view/' . $this->nid;
    return $output;
  }
}

/** - - - - - - common node - - - - - - - - - - - - - - - - - - - - - - - -  */
/**
 * class
   $JobsInfo = new JobsInfo($nid);
   $JobsInfo->clientName();
 */
class JobsInfo extends NodeInfo {
  /**
   * @return Boolean,
   * access View Permission
   */
  public function accessView($user = NULL) {
    $output = FALSE;

    if (isset($user->uid)) {
      if ($user->uid == 1) {
        $output = TRUE;
      }
    }

    if(isset($user)) {
      if ($user->uid == $this->nodeUid()) {
        $output = TRUE;
      }

      if (array_intersect(array('manager', 'siteadmin', 'administrator'), array_values($user->roles))) {
        $output = TRUE;
      }

      if (is_array($this->accessPermissionUids())) {
        if (in_array($user->uid, $this->accessPermissionUids())) {
          $output = TRUE;
        }
      }
    }
    return $output;
  }

  /**
   * @return Integer, Term Tid
   */
  public function accessPermissionUids() {
    $output = NULL;
    if (isset($this->node->field_quote_access_permission['und'][0]['target_id'])) {
      foreach ($this->node->field_quote_access_permission['und'] as $key => $value) {
        $output[] = $value['target_id'];
      }
    }
    return $output;
  }

  /**
   * @return Integer, Term Tid
   */
  public function clientTid() {
    $output = NULL;
    if (isset($this->node->field_quote_client['und'][0]['target_id'])) {
      $output = $this->node->field_quote_client['und'][0]['target_id'];
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
   * @return Object, Term Property
   */
  public function clientObject() {
    $output = NULL;

    $TermClientInfo = new TermClientInfo($this->clientTid());
    if(!empty($TermClientInfo)) {
      $output = $TermClientInfo;
    }
    return $output;
  }

  /**
   * @return Integer, Term Tid
   */
  public function companyTid() {
    $output = NULL;
    if (isset($this->node->field_quote_company['und'][0]['target_id'])) {
      $output = $this->node->field_quote_company['und'][0]['target_id'];
    }

    return $output;
  }
  /**
   * @return Object, Term Property
   */
  public function companyObject() {
    $output = NULL;

    $TermCompanyInfo = new TermCompanyInfo($this->companyTid());
    if(!empty($TermCompanyInfo)) {
      $output = $TermCompanyInfo;
    }
    return $output;
  }

  /**
   * @return String,
   */
  public function contactPerson() {
    $output = NULL;
    if (isset($this->node->field_quote_contact_person['und'][0]['value'])) {
      $output = $this->node->field_quote_contact_person['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String,
   */
  public function contactAddress() {
    $output = NULL;
    if (isset($this->node->field_quote_contact_address['und'][0]['value'])) {
      $output = $this->node->field_quote_contact_address['und'][0]['value'];
    }
    return $output;
  }
  /**
   * @return String,
   */
  public function contactEmail() {
    $output = NULL;
    if (isset($this->node->field_quote_contact_email['und'][0]['value'])) {
      $output = $this->node->field_quote_contact_email['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String,
   */
  public function contactFax() {
    $output = NULL;
    if (isset($this->node->field_quote_contact_fax['und'][0]['value'])) {
      $output = $this->node->field_quote_contact_fax['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String,
   */
  public function contactMobile() {
    $output = NULL;
    if (isset($this->node->field_quote_contact_mobile['und'][0]['value'])) {
      $output = $this->node->field_quote_contact_mobile['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String,
   */
  public function contactPhone() {
    $output = NULL;
    if (isset($this->node->field_quote_contact_phone['und'][0]['value'])) {
      $output = $this->node->field_quote_contact_phone['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return String,
   */
  public function creditTime() {
    $output = NULL;
    if (isset($this->node->field_quote_credit_time['und'][0]['value'])) {
      $output = $this->node->field_quote_credit_time['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return Integer, currency Uid
   */
  public function currencyTid() {
    $output = NULL;
    if (isset($this->node->field_quote_currency['und'][0]['target_id'])) {
      $output = $this->node->field_quote_currency['und'][0]['target_id'];
    }
    return $output;
  }

  /**
   * @return Integer, currency Name is currency symbol
   */
  public function currencyName() {
    $output = NULL;
    $TermInfo = new TermInfo($this->currencyTid());
    if(!empty($TermInfo)) {
      $output = $TermInfo->termName();
    }
    return $output;
  }
  /**
   * @return Object, currency Property
   */
  public function currencyObject() {
    $output = NULL;

    $TermInfo = new TermInfo($this->currencyTid());
    if(!empty($TermInfo)) {
      $output = $TermInfo;
    }
    return $output;
  }

  /**
   * @return integer, date unix stamp for quote order time
   */
  public function dateTimeUnixStamp() {
    $output = NULL;
    if (isset($this->node->field_quote_datetime['und'][0]['value'])) {
      $output = $this->node->field_quote_datetime['und'][0]['value'];
    }
    return $output;
  }
  /**
   * @return integer, date unix stamp
   */
  public function dateTimeValue($format = NULL) {
    if (!$format) {
      $format = "Y-m-d";
    }

    $output = NULL;
    $stamp = $this->dateTimeUnixStamp();
    if ($stamp) {
      $output = format_date($stamp, 'custom', $format);
    }
    return $output;
  }

  /**
   * @return String, quote number
   */
  public function discount() {
    $output = NULL;
    if (isset($this->node->field_quote_discount['und'][0]['value'])) {
      $output = $this->node->field_quote_discount['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return integer, date unix stamp
   */
  public function nodeDuplicateLink() {
    $output = base_path() . 'quote/node/duplicate/' . $this->nid;
    return $output;
  }


  /**
   * @return Integer, user Uid
   */
  public function offerorUid() {
    $output = NULL;
    if (isset($this->node->field_quote_offeror['und'][0]['target_id'])) {
      $output = $this->node->field_quote_offeror['und'][0]['target_id'];
    }
    return $output;
  }
  /**
   * @return Object, User Property
   */
  public function offerorObject() {
    $output = NULL;

    $UserInfo = new UserInfo($this->offerorUid());

    if(!empty($UserInfo)) {
      $output = $UserInfo;
    }
    return $output;
  }

  /**
   * @return Array, quote product items
   */
  public function quoteProductItems() {
    $output = NULL;
    if (isset($this->node->field_quote_product_items['und'][0]['quotation_product_name'])) {
      $output = $this->node->field_quote_product_items['und'];
    }
    return $output;
  }

  /**
   * @return String, quote number
   */
  public function quoteNumber() {
    $output = NULL;
    if (isset($this->node->field_quote_number['und'][0]['value'])) {
      $output = $this->node->field_quote_number['und'][0]['value'];
    }
    return $output;
  }
  /**
   * @return Array, quote number
   */
  public function quoteProductItemsArray() {
    $output = NULL;
    if (isset($this->node->field_quote_product_items['und'][0]['quotation_id'])) {
      $output = $this->node->field_quote_product_items['und'];
    }
    return $output;
  }

  /**
   * @return Integer, Term Tid
   */
  public function referenecePersonTid() {
    $output = NULL;
    if (isset($this->node->field_quote_referenece_person['und'][0]['target_id'])) {
      $output = $this->node->field_quote_referenece_person['und'][0]['target_id'];
    }
    return $output;
  }
  /**
   * @return Object, Term Property
   */
  public function referenecePersonObject() {
    $output = NULL;

    $TermContactInfo = new TermContactInfo($this->referenecePersonTid());
    if(!empty($TermContactInfo)) {
      $output = $TermContactInfo;
    }
    return $output;
  }

  /**
   * @return Integer, user Uid
   */
  public function stamp() {
    $output = NULL;
    if (isset($this->node->field_quote_stamp['und'][0]['target_id'])) {
      $output = $this->node->field_quote_stamp['und'][0]['target_id'];
    }
    return $output;
  }

  /**
   * @return float, Terms
   */
  public function terms() {
    $output = NULL;
    if (isset($this->node->field_quote_terms['und'][0]['safe_value'])) {
      $output = $this->node->field_quote_terms['und'][0]['safe_value'];
    }
    $output = check_markup($output);
    return $output;
  }

  /**
   * @return float, Total Price
   */
  public function totalPrice() {
    $output = NULL;
    if (isset($this->node->field_quote_total_price['und'][0]['value'])) {
      $output = $this->node->field_quote_total_price['und'][0]['value'];
    }
    return $output;
  }

  /**
   * @return float, Warrant Time
   */
  public function warrantTime() {
    $output = NULL;
    if (isset($this->node->field_quote_warrant_time['und'][0]['value'])) {
      $output = $this->node->field_quote_warrant_time['und'][0]['value'];
    }
    return $output;
  }

  /** - - - - - - other - - - - - - - - - - - - - - - - - - - - - - - - - -  */
}
