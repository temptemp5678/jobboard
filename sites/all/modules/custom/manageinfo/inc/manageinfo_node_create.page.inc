<?php

/**
 * callback from hook_menu(), modal pop content with AJAX
 */
function _manageinfo_node_create($type = NULL) {
  if (!$type) {
    return drupal_not_found();
  }

  drupal_add_css(drupal_get_path('module', 'manageinfo') . '/css/manageinfo_node_create.css');

  $NodeForm = new NodeForm();
  $form = $NodeForm->createForm($type);

  $output = $NodeForm->formStyle($form);

  return $output;
}

