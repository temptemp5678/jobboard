<?php

/**
 * @file
 * template.php
 */

/**
 * pass "base path" to JS
 */
drupal_add_js('jQuery.extend(Drupal.settings, { "basePath": "' . base_path() . '" });', 'inline');
drupal_add_js('jQuery.extend(Drupal.settings, { "currentPath": "' . current_path() . '" });', 'inline');
