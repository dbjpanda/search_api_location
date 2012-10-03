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
    '#element_validate' => array('search_api_location_element_validate'),
    '#theme_wrappers' => array('form_element'),
    '#options' => array(),
  );

  return $elements;
}

/**
 * Processor for the search_api_location_pick field.
 */
function search_api_location_element_process($element, $form_state, $complete_form) {

  // Shorten variable names for readability.
  $id       = $element['#id'];
  $form_id  = $complete_form['form_id'];
  $input    = &$form_state['input'];
  $settings = &$element['#default_value'];
  $defaults = array_merge((array)$input, (array)$settings);

  // Radius settings.
  $radius_units_options = array(
    'm' => 'meters',
    'km' => 'kilometers',
    'mi' => 'miles',
  );

  // Determine element parents. This is used to correctly set #states arrays.
  if ($form_id['#value'] == 'search_api_page_admin_edit') {
    $element_parents = 'options[spatial]';
  }
  else {
    $element_parents = 'spatial';
  }

  $element['radius'] = array(
    '#type' => !empty($defaults['edit_mode']) || $defaults['radius_display'] ? 'textfield' : 'hidden',
    '#title' => $settings['edit_mode'] ? t('Default radius') : NULL,
    '#field_prefix' => t('Within'),
    '#size' => 3,
    '#default_value' => $defaults['radius'],
    '#field_suffix' => $radius_units_options[$defaults['radius_units']] . t(' of'),
    '#attributes' => array(
      'id' => $id . '-radius',
    ),
    '#parents' => array_merge($element['#parents'], array('radius')),
    '#required' => TRUE,
  );
  $element['radius_min'] = array(
    '#type' => !empty($defaults['edit_mode']) ? 'textfield' : 'hidden',
    '#title' => t('Minimum radius'),
    '#size' => 3,
    '#default_value' => $defaults['radius_min'],
    '#parents' => array_merge($element['#parents'], array('radius_min')),
  );
  $element['radius_max'] = array(
    '#type' => !empty($defaults['edit_mode']) ? 'textfield' : 'hidden',
    '#title' => t('Maximum radius'),
    '#size' => 3,
    '#default_value' => $defaults['radius_max'],
    '#parents' => array_merge($element['#parents'], array('radius_max')),
  );
  $element['radius_units'] = array(
    '#type' => !empty($defaults['edit_mode']) || $defaults['radius_units_display'] ? 'select' : 'hidden',
    '#title' => t('Radius units'),
    '#options' => $radius_units_options,
    '#default_value' => $defaults['radius_units'],
    '#parents' => array_merge($element['#parents'], array('radius_units')),
  );

  // We show this to permit administrators to select a geofield
  // for default user location.
  if (!empty($settings['edit_mode'])) {
    $element['default_location_type'] = array(
      '#type' => 'radios',
      '#title' => t('Default location'),
      '#options' => array('string' => t('Address String'), 'user_geofield' => t('User Geofield')),
      '#default_value' => $defaults['default_location_type'],
    );
    $element['user_geofield'] = array(
      '#type' => 'select',
      '#title' => t('User geofield'),
      '#description' => t('The latitude and longitude will be populated using values from this field'),
      '#options' => _search_api_location_get_user_fields('geofield'),
      '#default_value' => $defaults['user_geofield'],
      '#states' => array(
        'visible' => array(
         ':input[name="' . $element_parents . '[default_location_type]"]' => array('value' => 'user_geofield'),
        ),
      ),
    );
    if (module_exists('addressfield')) {
      $element['user_addressfield'] = array(
        '#type' => 'select',
        '#title' => t('User addressfield'),
        '#description' => t('The address string will be populated using values from this field'),
        '#options' => _search_api_location_get_user_fields('addressfield'),
        '#default_value' => $defaults['user_addressfield'],
        '#states' => array(
          'visible' => array(
           ':input[name="' . $element_parents . '[default_location_type]"]' => array('value' => 'user_geofield'),
          ),
        ),
      );
    }
  }

  // We show the address field to permit administrators to set a default
  // location, and to allow users to override default location.
  $element['address'] = array(
    '#type' => $settings['edit_mode'] || $defaults['address_display'] ? 'textfield' : 'hidden',
    '#title' => !empty($settings['edit_mode']) ? t('Default address') : t('Near (city, state, or zip)'),
    '#maxlength' => 120,
    '#default_value' => $defaults['address'],
    //'#default_value' => $defaults['default_location_type'] == 'string' ? $defaults['address'] : '',
    '#attributes' => array(
      'id' => $id . '-address',
    ),
    '#required' => $defaults['address_display'],
    '#states' => array(
      'visible' => array(
       ':input[name="' . $element_parents . '[default_location_type]"]' => array('value' => 'string'),
      ),
    ),
  );

  // Display settings.
  if (!empty($settings['edit_mode'])) {
    $element['address_display'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display address'),
      '#default_value' => $defaults['address_display'],
    );
    $element['radius_display'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display radius'),
      '#default_value' => $defaults['radius_display'],
    );
    $element['radius_units_display'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display radius units'),
      '#default_value' => $defaults['radius_units_display'],
    );
  }

  // Lat & Lng.
  $element['lat'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'id' => $id . '-lat',
    ),
    '#default_value' => $defaults['lat'],
    '#parents' => array_merge($element['#parents'], array('lat')),
  );

  $element['lng'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'id' => $id . '-lng',
    ),
    '#default_value' => $defaults['lng'],
    '#parents' => array_merge($element['#parents'], array('lng')),
  );

  return $element;
}

/**
 * Validation callback for the search_api_location_pick field.
 */
function search_api_location_element_validate($element, $form_state, $complete_form) {

}

/**
 * Returns spatial form element with default values.
 */
function search_api_page_get_spatial_element($settings = array()) {
  $element = array(
    '#weight' => isset($settings['edit_mode']) ? NULL : -1,
    '#type' => 'search_api_location_pick',
    '#default_value' => array(
      'default_location_type' => 'string',
      'address' => 'Washington DC, USA',
      'lat' => '51.03826129999999',
      'lng' => '3.7125661000000036',
      'radius' => '5',
      'radius_min' => '5',
      'radius_max' => '100',
      'radius_units' => 'km',
      'edit_mode' => FALSE,
      'address_display' => 1,
      'radius_display' => 1,
      'radius_units_display' => 0,
      'user_geofield' => NULL,
      'user_addressfield' => NULL,
    ),
    '#tree' => TRUE,
  );

  // Prioritize values passed through $settings.
  // Using array_filter() ensures that empty strings do not replace non-empty strings.
  $element['#default_value'] = array_merge(array_filter($element['#default_value'], '_search_api_filter_defaults'), array_filter((array)$settings, '_search_api_filter_defaults'));

  return $element;
}

/**
 *
 */
function _search_api_filter_defaults($value) {
  // We mostly want to exlude empty strings.
  return $value !== '';
}