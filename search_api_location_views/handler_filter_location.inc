<?php

/**
 * @file
 * Provides the views handler for location fields
 */

class SearchApiViewsHandlerFilterLocation extends SearchApiViewsHandlerFilter {

  /**
   * Provide a form for setting the filter value.
   */
  public function value_form(&$form, &$form_state) {

    $field = $this->options['id'];
    $default_value = array();

    // Check if the input filter is filled in.
    if (isset($form_state['input']['options']['spatial'])) {
      //$default_value['radius'] = $form_state['input']['options']['spatial']['radius'];
      $this->options['radius'] = $form_state['input']['options']['spatial']['radius'];
      $this->options['address'] = $form_state['input']['options']['spatial']['address'];
      $default_value['lng'] = $form_state['input'][$field]['lng'];
    }
    else {
      $default_value['lat'] = $this->options['lat'];
      $default_value['lng'] = $this->options['lng'];
      $default_value['radius'] = $this->options['radius'];
    }
    error_log(print_r($this->options['radius'],1));
    $default_value['radius_min'] = $this->options['radius_min'];
    $default_value['radius_max'] = $this->options['radius_max'];
    $default_value['radius_units'] = $this->options['radius_units'];
    $default_value['address_display'] = TRUE;
    $default_value['radius_display'] = FALSE;
    $default_value['edit_mode'] = FALSE;
    $default_value['radius_units_display'] = FALSE;
    $default_value['address'] = $this->options['address'];
    $default_value['address_required'] = FALSE;

    $form['value'] = array(
      '#type' => 'search_api_location_pick',
      '#default_value' => $default_value,
    );



  }

  /**
   * Default location settings
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['default_location_type'] = array('default' => 'string');
    $options['lat'] = array('default' => '51.03826129999999');
    $options['lng'] = array('default' => '3.7125661000000036');
    $options['radius'] = array('default' => '50');
    $options['radius_min'] = array('default' => '5');
    $options['radius_max'] = array('default' => '100');
    $options['radius_units'] = array('default' => 'km');
    $options['address'] = array('default' => 'London, England');

    return $options;
  }

  /**
   * Alter and extend the options form
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    unset($form['operator']);
    //unset($form['value']);
    //$form['label'] = $form['expose']['label'];
    unset($form['expose']);



    $form['spatial'] = array(
      '#type' => 'search_api_location_pick',
      '#default_value' => array(
        'default_location_type' => $this->options['default_location_type'],
        'lat' => $this->options['lat'],
        'lng' => $this->options['lng'],
        'radius' => $this->options['radius'],
        'radius_min' => $this->options['radius_min'],
        'radius_max' => $this->options['radius_max'],
        'radius_units' => $this->options['radius_units'],
        'radius_measure' => $this->options['radius_measure'],
        'address' => $this->options['address'],
        'address_display' => $this->options['address_display'],
        'edit_mode' => TRUE,
      ),
      '#tree' => TRUE
    );
  }

  /**
   * Validate the options form.
   */
  public function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
  }

  /**
   * Add this filter to the query.
   */
  public function query() {

    $field = str_replace(":", "_", $this->real_field);

    if (isset($this->view->exposed_raw_input[$field]['radius']) && isset($this->view->exposed_data['address'])) {

      $address = $this->view->exposed_data['address'];
      $response = geocoder('google', $address);
      $spatial = array();
      $spatial['lat'] = $response->coords[1];
      $spatial['lng'] = $response->coords[0];     
      $spatial['radius'] = $this->view->exposed_raw_input[$field]['radius'];
      $spatial['radius_measure'] = $this->options['radius_measure'];
      $spatial['field'] = $this->real_field;
      $this->query->setOption('search_api_location', $spatial);

    }
  }
}
