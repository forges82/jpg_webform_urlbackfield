<?php

/**
 * @file
 * Webform module hidden component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_jpgurlbackfield() {
  return array(
    'name' => '',
      'form_key' => NULL,
        'pid' => 0,
        'weight' => 0,
        'value' => '',
        'extra' => array(
          'private' => FALSE,
          'analysis' => FALSE,
        ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_jpgurlbackfield() {
  return array(
    'webform_display_jpgurlbackfield' => array(
      'render element' => 'element',
      'file' => 'components/jpgurlbackfield.inc',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_jpgurlbackfield($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
      '#description' => t('The default value of the field.') . ' ' . theme('webform_token_help'),
      '#cols' => 60,
      '#rows' => 5,
      '#weight' => 0,
  );
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_jpgurlbackfield($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
  $default_value = $filter ? webform_replace_tokens($component['value'], $node) : $component['value'];
  if (isset($value[0])) {
    $default_value = $value[0];
  }
  $element = array(
    '#title' => $filter ? NULL : $component['name'],
    '#weight' => $component['weight'],
    '#translatable' => array('title'),
  );
  $element['#type'] = 'hidden';
  $element['#default_value'] = $default_value;
  $element['#theme_wrappers'] = array('webform_element');
  $element['#wrapper_attributes']['class'] = array();
  $element['#wrapper_attributes']['style'] = array('display: none');
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_jpgurlbackfield($component, $value, $format = 'html', $submission = array()) {
  $element = array(
    '#title' => $component['name'],
    '#markup' => isset($value[0]) ? $value[0] : NULL,
    '#weight' => $component['weight'],
    '#format' => $format,
    '#theme' => 'webform_display_jpgurlbackfield',
    '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_element'),
    '#translatable' => array('title'),
  );
  return $element;
}

function theme_webform_display_jpgurlbackfield($variables) {
  $element = $variables['element'];
  return $element['#format'] == 'html' ? check_plain($element['#markup']) : $element['#markup'];
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_jpgurlbackfield($component, $sids = array(), $single = FALSE, $join = NULL) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('no', 'data'))
    ->condition('wsd.nid', $component['nid'])
    ->condition('wsd.cid', $component['cid']);
  if (count($sids)) {
    $query->condition('wsd.sid', $sids, 'IN');
  }
  if ($join) {
    $query->innerJoin($join, 'ws2_', 'wsd.sid = ws2_.sid');
  }
  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;
  $result = $query->execute();
  foreach ($result as $data) {
    if (strlen(trim($data['data'])) > 0) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data['data']));
    }
    $submissions++;
  }
  $rows[0] = array(t('Empty'), ($submissions - $nonblanks));
  $rows[1] = array(t('Non-empty'), $nonblanks);
  $other[0] = array(t('Average submission length in words (ex blanks)'), ($nonblanks != 0 ? number_format($wordcount/$nonblanks, 2) : '0'));
  return array(
    'table_rows' => $rows,
    'other_data' => $other,
  );
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_table_jpgurlbackfield($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implements _webform_action_set_component().
 */
function _webform_action_set_jpgurlbackfield($component, &$element, &$form_state, $value) {
  $element['#value'] = $value;
  form_set_value($element, $value, $form_state);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_jpgurlbackfield($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_jpgurlbackfield($component, $export_options, $value) {
  return isset($value[0]) ? $value[0] : '';
}