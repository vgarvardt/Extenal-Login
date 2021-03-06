<?php
// $Id$

/**
 * @file
 * Administration routines for External Login module.
 */

function external_login_settings_providers() {
  $all_providers = external_login_get_providers(FALSE);
  $enb_providers = external_login_get_providers(TRUE);
  $options = array();
  foreach ($enb_providers as $provider => $data) {
    $options[$provider] = $data['short'];
  }
  $weights = variable_get('external_login_enabled_providers', array('openid' => 0));
  $weight_delta = round(count($all_providers) / 2);

  $form = array('#tree' => TRUE);

  $na_providers = array();
  foreach ($all_providers as $pname => $data) {
    // if provider has verification function lets execute it and check if the provider is available now
    if (!empty($data['verify'])) {
      $error_messages = array();
      foreach ($data['verify'] as $function) {
        $error_message = $function();
        if (!empty($error_message)) {
          $error_messages[] = $error_message;
        }
      }
      // if provider is not available - remember provider name, the reason and skip it
      // information about N/A providers will be rendered in the end of the form
      if (!empty($error_messages)) {
        $na_providers[$data['short']] = $error_messages;
        continue;
      }
    }

    $key = 'external_login_provider_' . $pname;
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

  if (!empty($na_providers)) {
    $errors = array();
    foreach ($na_providers as $provider => $provider_errors) {
      $errors[] = theme('item_list', $provider_errors, $provider);
    }

    $form['na_providers'] = array(
      '#type' => 'item',
      '#title' => t('N/A providers'),
      '#value' => theme('item_list', $errors),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#theme'] = 'external_login_admin_list_prepare';

  return $form;
}

function external_login_settings_providers_validate($form, &$form_state) {
  $all_providers = external_login_get_providers(FALSE);
  foreach ($all_providers as $pname => $data) {
    $key = 'external_login_provider_' . $pname;
    if ($form_state['values'][$key]['enabled'] == 'yes') return;
  }
  form_set_error('external_login_provider_openid', t('At least one provider should be enabled.'));
}

function external_login_settings_providers_submit($form, &$form_state) {
  $all_providers = external_login_get_providers(FALSE);
  $enabled = array();
  foreach ($all_providers as $pname => $data) {
    $key = 'external_login_provider_' . $pname;
    if ($form_state['values'][$key]['enabled'] == 'yes') {
      $enabled[$pname] = $form_state['values'][$key]['weight'];
    }
  }
  asort($enabled);
  variable_set('external_login_enabled_providers', $enabled);
  // reset all module cache so forms should be rebuilt on first call
  cache_clear_all('external_login:', 'cache', TRUE);
}

function external_login_settings_themes() {
  $form = array();

  $form['external_login_theme'] = array(
    '#type' => 'radios',
    '#title' => t('Theme'),
    '#required' => TRUE,
    '#options' => external_login_get_themes(),
    '#default_value' => variable_get('external_login_theme', 'external_login_dropdown'),
  );

  $form['#submit'][] = 'external_login_settings_themes_submit';

  return system_settings_form($form);
}

function external_login_settings_themes_submit($form, &$form_state) {
  // reset all module cache so forms should be rebuilt on first call
  cache_clear_all('external_login:', 'cache', TRUE);
}

function external_login_settings_settings() {
  $form = array();

  $form['external_login_openid_ident2name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use OpenID identity as name when OpenID provider does not return username.'),
    '#default_value' => variable_get('external_login_openid_ident2name', FALSE),
  );

  return system_settings_form($form);
}