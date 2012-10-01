<?php

/**
 * @file
 * Provides the location element to use in forms
 */

/**
 * Implements hook_element_info().
 */
function search_api_location_element_info() {
  $elements = array();
  $elements['search_api_location_pick'] = array(
    '#input' => FALSE,
    '#process' => array('search_api_location_element_process'),
    '#theme_wrappers' => array('form_element'),
    '#options' => array(),
  );

  return $elements;
}

/**
 * Processor for the search_api_location_pick field
 */
function search_api_location_element_process($element, $form_state, $complete_form) {

  // Shorten variable names for readability.
  $id       = $element['#id'];
  $form_id  = $complete_form['form_id'];
  $input    = &$form_state['input'];
  $defaults = &$element['#default_value'];


  // Set default values.
  $lat_value = isset($input['lat']) ? $input['lat'] : $defaults['lat'];
  $lng_value = isset($input['lng']) ? $input['lng'] : $defaults['lng'];
  $radius_value = isset($input['radius']) ? $input['radius'] : $defaults['radius'];
  $radius_min_value = isset($input['radius_min']) ? $input['radius_min'] : $defaults['radius_min'];
  $radius_max_value = isset($input['radius_max']) ? $input['radius_max'] : $defaults['radius_max'];
  $radius_step_value = isset($input['radius_step']) ? $input['radius_step'] : $defaults['radius_step'];
  $radius_measure_value = isset($input['radius_measure']) ? $input['radius_measure'] : $defaults['radius_measure'];
  $address_value = isset($input['address']) ? $input['address'] : $defaults['address'];

  $radius_measure_options = array(
    'm' => 'meters',
    'km' => 'kilometers',
    'mi' => 'miles',
  );

  $element['radius'] = array(
    '#type' => 'textfield',
    '#title' => $defaults['edit_mode'] ? t('Radius') : '',
    '#name' => 'spatial-radius',
    '#field_prefix' => t('Within'),
    '#size' => 3,
    '#default_value' => $radius_value,
    '#field_suffix' => $radius_measure_options[$radius_measure_value] . t(' of'),
    '#attributes' => array(
      'id' => $id . '-radius',
    ),
    '#parents' => array_merge($element['#parents'], array('radius')),
    '#required' => TRUE,
  );

  $element['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Near (city, state, or zip)'),
    '#maxlength' => 120,
    '#default_value' => $address_value,
    '#attributes' => array(
      'id' => $id . '-address',
    ),
    '#required' => TRUE,
  );

  $element['slider'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => $id . '-slider',
      'class' => array('form-item-spatial-radius-slider'),
    ),
  );

  $element['help'] = array(
    '#prefix' => '<div id="' . $id . '-help" class="form-item-spatial-help">',
    '#markup' => t('Enter an address / location in the textfield or click on the map to set the marker'),
    '#suffix' => '</div>',
  );

  $element['gmap'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => $id . '-gmap',
      'class' => array('form-item-spatial-gmap'),
      'style' => array('width:100%; height:400px;'),
    ),
  );

  // We only show adminstrative fields in 'edit_mode.'
  if (!empty($defaults['edit_mode'])) {
    $element['radius_min'] = array(
      '#type' => 'textfield',
      '#title' => t('Radius minimum value'),
      '#size' => 3,
      '#default_value' => $radius_min_value,
      '#parents' => array_merge($element['#parents'], array('radius_min')),
    );
    $element['radius_max'] = array(
      '#type' => 'textfield',
      '#title' => t('Radius maximum value'),
      '#size' => 3,
      '#default_value' => $radius_max_value,
      '#parents' => array_merge($element['#parents'], array('radius_max')),
    );
    $element['radius_step'] = array(
      '#type' => 'textfield',
      '#title' => t('Radius step value'),
      '#size' => 3,
      '#default_value' => $radius_step_value,
      '#parents' => array_merge($element['#parents'], array('radius_step')),
    );
    $element['radius_measure'] = array(
      '#type' => 'select',
      '#title' => t('Radius measure value'),
      '#options' => $radius_measure_options,
      '#default_value' => $radius_measure_value,
      '#parents' => array_merge($element['#parents'], array('radius_measure')),
    );
  }

  $element['lat'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'id' => $id . '-lat',
    ),
    '#default_value' => $lat_value,
    '#parents' => array_merge($element['#parents'], array('lat')),
  );

  $element['lng'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'id' => $id . '-lng',
    ),
    '#default_value' => $lng_value,
    '#parents' => array_merge($element['#parents'], array('lng')),
  );

  return $element;
}

/**
 * Returns spatial form element with default values.
 */
function search_api_page_get_spatial_element($defaults = array()) {

  $element = array(
    '#weight' => isset($defaults['edit_mode']) ? NULL : -1,
    '#type' => 'search_api_location_pick',
    '#default_value' => array(
      'lat' => '51.03826129999999',
      'lng' => '3.7125661000000036',
      'radius' => '5',
      'radius_min' => '5',
      'radius_max' => '100',
      'radius_step' => '5',
      'radius_measure' => 'km',
      'edit_mode' => FALSE,
    ),
    '#tree' => TRUE,
  );

  // Prioritize values passed through $defaults.
  // Using array_filter() ensures that empty strings do not replace non-empty strings.
  $element['#default_value'] = array_merge(array_filter($element['#default_value'], 'strval'), array_filter((array)$defaults, 'strval'));

  return $element;
}