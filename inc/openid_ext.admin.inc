<?php
// $Id$

/**
 * @file
 * Administration routines for OpenID Extension module.
 */

function openid_ext_settings_providers() {
  $all_providers = openid_ext_get_providers(FALSE);
  $enb_providers = openid_ext_get_providers(TRUE);
  $options = array();
  foreach ($enb_providers as $provider => $data) {
    $options[$provider] = $data['short'];
  }
  $weights = variable_get('openid_ext_enabled_providers', array('openid' => 0));
  $weight_delta = round(count($all_providers) / 2);

  $form = array('#tree' => TRUE);

  $na_providers = array();
  foreach ($all_providers as $pname => $data) {
    // if provider has verification function lets execute it and check if the provider is available now
    if (isset($data['verify'])) {
      $function = $data['verify'];
      $error_message = $function();
      // if provider is not available - remember provider name, the reason and skip it
      // information about N/A providers will be rendered in the end of the form
      if ($error_message) {
        $na_providers[] = '<strong>' . $data['short'] . '</strong>: ' . $error_message;
        continue;
      }
    }

    $key = 'openid_provider_' . $pname;
    $form[$key]['info'] = array(
      '#value' => $data['short']
    );
    $form[$key]['enabled'] = array(
      '#type' => 'select',
      '#default_value' => (isset($enb_providers[$pname]) ? 'yes' : 'no'),
      '#options' => array('yes' => t('Yes'), 'no' => t('No')),
    );
    $form[$key]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => (isset($weights[$pname]) ? $weights[$pname] : 0),
      '#delta' => $weight_delta,
    );
  }

  if ($na_providers) {
    $form['na_providers'] = array(
      '#type' => 'item',
      '#title' => t('N/A providers'),
      '#value' => theme('item_list', $na_providers),
    );
  }

  $form['op'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#theme'] = 'openid_ext_admin_list_prepare';

  return $form;
}

function openid_ext_settings_providers_validate($form, &$form_state) {
  $all_providers = openid_ext_get_providers(FALSE);
  foreach ($all_providers as $pname => $data) {
    $key = 'openid_provider_' . $pname;
    if ($form_state['values'][$key]['enabled'] == 'yes') return;
  }
  form_set_error('openid_provider_openid', t('At least one provider should be enabled.'));
}

function openid_ext_settings_providers_submit($form, &$form_state) {
  $all_providers = openid_ext_get_providers(FALSE);
  $enabled = array();
  foreach ($all_providers as $pname => $data) {
    $key = 'openid_provider_' . $pname;
    if ($form_state['values'][$key]['enabled'] == 'yes') {
      $enabled[$pname] = $form_state['values'][$key]['weight'];
    }
  }
  asort($enabled);
  variable_set('openid_ext_enabled_providers', $enabled);
  // reset all module cache so forms should be rebuilt on first call
  cache_clear_all('openid_ext:', 'cache', TRUE);
}

function openid_ext_settings_themes() {
  $form = array();

  $form['openid_ext_theme'] = array(
    '#type' => 'radios',
    '#title' => t('Theme'),
    '#required' => TRUE,
    '#options' => openid_ext_get_themes(),
    '#default_value' => variable_get('openid_ext_theme', 'openid_ext_default'),
  );

  $form['#submit'][] = 'openid_ext_settings_themes_submit';

  return system_settings_form($form);
}

function openid_ext_settings_themes_submit($form, &$form_state) {
  // reset all module cache so forms should be rebuilt on first call
  cache_clear_all('openid_ext:', 'cache', TRUE);
}
