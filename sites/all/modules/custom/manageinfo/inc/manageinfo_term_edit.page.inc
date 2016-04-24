<?php
/**
 * @file for edit term
 */

/**
 * callback from hook_menu()
 */
function _terminfo_term_edit($vid = NULL, $tid = NULL) {
  drupal_add_css(drupal_get_path('module', 'manageinfo') . '/css/manageinfo_term_create.css');

  $TermForm = new TermForm();
  $form = $TermForm->editForm($vid, $tid);


  $output = $TermForm->formStyle($form);

  return $output;
}


