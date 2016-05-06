<?php

/**
 * callback from hook_menu()
 */
function _manageinfo_node_edit($nid = NULL) {
  global $user;
  if (!isset($account)) {
    $account = $GLOBALS['user'];
  }

  $NodeInfo = new NodeInfo($nid);
  $node_type = $NodeInfo->nodeType();

  /**
   * access permission
   */
  if ($node_type == 'order') {
    $OrderInfo = new OrderInfo($nid);
    if ($OrderInfo->accessView($account)) {
      // NODE_ACCESS_ALLOW;
      dpm($account);
    }
    else {
      // NODE_ACCESS_DENY;
      // return drupal_not_found();
    }
  }
  else if ($node_type == 'quote') {
    $QuoteInfo = new QuoteInfo($nid);
    if ($QuoteInfo->accessView($account)) {
      // NODE_ACCESS_ALLOW;
    }
    else {
      // NODE_ACCESS_DENY;
      return drupal_not_found();
    }
  }

  /**
   * css
   */
  drupal_add_css(drupal_get_path('module', 'manageinfo') . '/css/manageinfo_node_create.css');

  if ($node_type == 'order') {
    drupal_add_css(drupal_get_path('module', 'orderinfo') . '/css/order_import_products.css');
    drupal_add_css(drupal_get_path('module', 'manageinfo') . '/css/manageinfo_node_edit.css');
  }
  else if ($node_type == 'quote') {
    drupal_add_css(drupal_get_path('module', 'quoteinfo') . '/css/quote_import_order.css');
    drupal_add_js(drupal_get_path('module', 'quoteinfo') . '/js/quote_import_order.js');
  }
  else if ($node_type == 'card' || $node_type == 'software' || $node_type == 'chassis') {
    drupal_add_css(drupal_get_path('module', 'manageinfo') . '/css/manageinfo_node_edit.css');
  }

  $NodeForm = new NodeForm();
  $form = $NodeForm->editForm($node_type, $nid);
  $output = $NodeForm->formStyle($form);

  return $output;
}
