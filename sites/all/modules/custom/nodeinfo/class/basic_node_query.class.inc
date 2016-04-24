<?php
/**
 * @file
 */

/**
 * class
 * $NodeQuery = new NodeQuery($nid);
 */
class NodeQuery {

  /** - - - - - - common - - - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @parameter, $node_type is array()
   *
   * @return array,
   * all Product NID
   */
  public function specifyBundleNid($node_type = array()) {

    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', $node_type)
      ->propertyCondition('status', NODE_PUBLISHED);

    $result = $query->execute();

    $nid_array = NULL;
    if (isset($result['node'])) {
      if (count($result['node']) > 0 ) {
        $nid_array = array_keys($result['node']);
      }
    }

    return $nid_array;
  }

  /**
   * @parameter, $node_type is array()
   *
   * @return array,
   *  Node both for PUBLISHED or not
   */
  public function nodesByBundle($node_type = array()) {

    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', $node_type);

    $result = $query->execute();

    $nid_array = NULL;
    if (isset($result['node'])) {
      if (count($result['node']) > 0 ) {
        $nid_array = array_keys($result['node']);
      }
    }

    return $nid_array;
  }

  /** - - - - - - common - - - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @parameter, $node_type is array()
   *
   * @return array,
   * all Product NID
   */
  public function productByBrandNid($node_type = array(), $brand_tids = array()) {

    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', $node_type)
      ->propertyCondition('status', NODE_PUBLISHED);
    $query->fieldCondition('field_product_brand', 'target_id', $brand_tids, 'IN');

    $result = $query->execute();

    $nid_array = NULL;
    if (isset($result['node'])) {
      if (count($result['node']) > 0 ) {
        $nid_array = array_keys($result['node']);
      }
    }

    return $nid_array;
  }

  /** - - - - - - other - - - - - - - - - - - - - - - - - - - - - - - - - -  */
  /**
   * @parameter ,
   *
   * @return array,
   * all Meeting NID
   */
  public function allQuoteNid() {
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'quote')
      ->propertyCondition('status', NODE_PUBLISHED);

    $result = $query->execute();

    $nid_array = NULL;
    if (isset($result['node'])) {
      if (count($result['node']) > 0 ) {
        $nid_array = array_keys($result['node']);
      }
    }

    return $nid_array;
  }

}
