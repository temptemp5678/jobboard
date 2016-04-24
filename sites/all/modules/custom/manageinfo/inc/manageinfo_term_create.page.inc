<?php
/**
 * @file for Add new term
 */

/**
 * Implements hook_form(), callback from hook_menu()
 */
function _manageinfo_term_add($vid = NULL) {
  if (!$vid) {
    return drupal_not_found();
  }

  drupal_add_css(drupal_get_path('module', 'manageinfo') . '/css/manageinfo_term_create.css');

  $TermForm = new TermForm();
  $form = $TermForm->createForm($vid);
  $output = $TermForm->formStyle($form);
  return $output;
}

