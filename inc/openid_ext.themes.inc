<?php
// $Id$

/**
 * @file
 * OpenID Extension themes implementations.
 */

function theme_openid_ext_admin_list_prepare($form) {
  $providers = openid_ext_get_providers(FALSE);
  $enabled = openid_ext_get_providers();
  $prv_enabled = $prv_disabled = array();
  foreach ($enabled as $pname => $weight) {
    _theme_openid_ext_admin_list_sort_row($pname, &$form, &$prv_enabled, &$prv_disabled);
    unset($providers[$pname]);
  }
  foreach ($providers as $pname => $data) {
    _theme_openid_ext_admin_list_sort_row($pname, &$form, &$prv_enabled, &$prv_disabled);
  }

  $regions = array('yes' => t('Enabled'), 'no' => t('Disabled'));
  $providers = array('yes' => $prv_enabled, 'no' => $prv_disabled);
  $rest_elemnts = drupal_render($form);

  drupal_add_css(drupal_get_path('module', 'openid_ext') .'/files/openid_ext.css');
  return theme('openid_ext_admin_list', $regions, $providers, $rest_elemnts);
}

function _theme_openid_ext_admin_list_sort_row($pname, &$form, &$prv_enabled, &$prv_disabled) {
  $key = 'openid_provider_' . $pname;
  if (isset($form[$key])) {
    $form[$key]['enabled']['#attributes']['class'] = 'openid_provider-region-select openid_provider-region-enabled-' . $form[$key]['enabled']['#default_value'];
    $form[$key]['weight']['#attributes']['class'] = 'openid_provider-weight openid_provider-weight-enabled-' . $form[$key]['enabled']['#default_value'];

    $obj = new stdClass();
    $obj->title = drupal_render($form[$key]['info']);
    $obj->enable_select = drupal_render($form[$key]['enabled']);
    $obj->weight_select = drupal_render($form[$key]['weight']);

    if ($form[$key]['enabled']['#default_value'] == 'yes') {
      $prv_enabled[$pname] = $obj;
    }
    else {
      $prv_disabled[$pname] = $obj;
    }
  }
}

function theme_openid_ext_default($form) {
  list($cache_html_cid, $cache_settings_cid) = openid_ext_get_cids();
  list($cache_html, $cache_settings, $cache_css) = openid_ext_get_cache_values($cache_html_cid, $cache_settings_cid);

  if (!$cache_html || !$cache_settings || !is_file($cache_css)) {
    global $base_url;

    $styles = array();
    $settings = array();
    $providers = openid_ext_get_providers();
    
    foreach ($form['provider']['#options'] as $pname => $title) {
      if (isset($providers[$pname]['icon']) && is_file($providers[$pname]['icon'])) {
        $styles['.with-icon'] = 'background-repeat:no-repeat;padding-left:17px;';
        $styles['.' . $pname] = 'background-image:url(' . $base_url . '/' . $providers[$pname]['icon'] . ');';

        $settings[$pname]['icon'] = $providers[$pname]['icon'];
      }
      if (isset($providers[$pname]['input'])) {
        $settings[$pname]['title'] = $providers[$pname]['input'] . ': ';
      }
    }

    $out = drupal_render($form);

    // cache all required data
    cache_set($cache_html_cid, $out);
    cache_set($cache_settings_cid, $settings);
    openid_ext_write_cache_styles($styles, $cache_css);
  } else {
    $out = $cache_html->data;
    $settings = $cache_settings->data;
  }

  // using _default for setting because default is reserved JS word
  drupal_add_js(array('openid_ext' => array('_default' => $settings)), 'setting');
  drupal_add_js(drupal_get_path('module', 'openid_ext') . '/files/openid_ext.js');
  drupal_add_css($cache_css);

  return $out;
}

function theme_openid_ext_dropdown($form) {
  list($cache_html_cid, $cache_settings_cid) = openid_ext_get_cids();
  list($cache_html, $cache_settings, $cache_css) = openid_ext_get_cache_values($cache_html_cid, $cache_settings_cid);

  if (!$cache_html || !$cache_settings || !is_file($cache_css)) {
    global $base_url;

    $styles = array();
    $settings = array();
    $providers = openid_ext_get_providers();

    $def_val = FALSE;
    if (isset($form['provider']['#default_value']) && isset($providers[$form['provider']['#default_value']]['input'])) {
      $def_val = $providers[$form['provider']['#default_value']]['input'];
      $def_prv = $form['provider']['#default_value'];
    }
    if (!$def_val) {
      $renderable = array_keys($form['provider']['#options']);
      $def_val = isset($providers[$renderable[0]]['input']) ? $providers[$renderable[0]]['input'] : FALSE;
      $def_prv = $renderable[0];
    }

    if ($def_val) {
      $form['input']['#title'] = $def_val;
      if (isset($providers[$def_prv]['icon']) && is_file($providers[$def_prv]['icon'])) {
        $form['input']['#attributes']['class'] = $def_prv . ' dropdown';
        $styles['.' . $def_prv] = 'background-image:url(' . $base_url . '/' . $providers[$def_prv]['icon'] . ');';
      }
    }

    $options = array();
    foreach ($form['provider']['#options'] as $pname => $title) {
      $attrs = array();
      if ($pname == $def_prv) $attrs['selected'] = 'selected';
      if (isset($providers[$pname]['icon']) && is_file($providers[$pname]['icon'])) {
        $attrs['class'] = 'with-icon ' . $pname;

        $styles['.with-icon'] = 'background-repeat:no-repeat;background-position:left center;padding-left:17px;';
        $styles['.' . $pname] = 'background-image:url(' . $base_url . '/' . $providers[$pname]['icon'] . ');';

        $settings[$pname]['icon'] = $providers[$pname]['icon'];
      }
      if (isset($providers[$pname]['input'])) {
        $settings[$pname]['title'] = $providers[$pname]['input'] . ': ';
      }
      $options[] = '<option value="' . check_plain($pname) . '"' . drupal_attributes($attrs) . '>' . check_plain($title) . '</option>';
    }

    $select = array(
      '#prefix' => '<select name="provider" class="form-select dropdown" id="edit-provider">',
      '#value' => implode('', $options),
      '#suffix' => '</select>',
    );
    $form['provider'] = array(
      '#type' => 'item',
      '#value' => drupal_render($select),
    );

    $out = drupal_render($form);

    // cache all required data
    cache_set($cache_html_cid, $out);
    cache_set($cache_settings_cid, $settings);
    openid_ext_write_cache_styles($styles, $cache_css);
  }
  else {
    $out = $cache_html->data;
    $settings = $cache_settings->data;
  }

  drupal_add_js(array('openid_ext' => array('dropdown' => $settings)), 'setting');
  drupal_add_js(drupal_get_path('module', 'openid_ext') . '/files/openid_ext.js');
  drupal_add_css($cache_css);

  return $out;
}

function theme_openid_ext_accordion($form) {
  list($cache_html_cid, $cache_settings_cid) = openid_ext_get_cids();
  list($cache_html, $cache_settings, $cache_css) = openid_ext_get_cache_values($cache_html_cid, $cache_settings_cid);

  if (!$cache_html || !$cache_settings || !is_file($cache_css)) {
    global $base_url;

    $styles = array();
    $settings = array();
    $providers = openid_ext_get_providers();
    $files_path = $base_url . '/' . drupal_get_path('module', 'openid_ext') . '/files/';
    $settings['container_id'] = 'openid_ext_accordion_container';

    $items = array();
    foreach ($form['provider']['#options'] as $pname => $title) {
      $items[] = theme('openid_ext_accordion_item', $pname, $title);

      $styles['.' . $pname] = 'background-image:url(' . $base_url . '/' . $providers[$pname]['icon'] . ');';

      if (isset($providers[$pname]['input'])) {
        $settings[$pname]['title'] = $providers[$pname]['input'] . ': ';
      }
    }
    $out = theme('item_list', $items, NULL, 'ul', array('id' => $settings['container_id']));
    // need to pass this to JS settings because css is passed before main
    // and this style become overridden, so we just set required properties directly in JS
    $settings['list_style'] = 'square inside url(' . $files_path . 'arrow_right.gif)';
    $settings['list_style_active'] = 'square inside url(' . $files_path . 'arrow_down.gif)';
    $settings['padding'] = '0px 0px 0px 2px';
    $settings['margin'] = '2px 0px 2px 0px';

    if (isset($form['provider']['#default_value']) && isset($providers[$form['provider']['#default_value']]['input'])) {
      $def_prv = $form['provider']['#default_value'];
    }
    else {
      $renderable = array_keys($form['provider']['#options']);
      $def_prv = $renderable[0];
    }

    $form['provider'] = array(
      '#type' => 'hidden',
      '#name' => 'provider',
      '#id' => 'edit-provider',
      '#value' => $def_prv,
    );

    $styles['#edit-input-wrapper'] = 'display:none;';
    $styles['#edit-op'] = 'display:none;';

    $out .= drupal_render($form);

    // cache all required data
    cache_set($cache_html_cid, $out);
    cache_set($cache_settings_cid, $settings);
    openid_ext_write_cache_styles($styles, $cache_css);
  }
  else {
    $out = $cache_html->data;
    $settings = $cache_settings->data;
  }

  drupal_add_js(array('openid_ext' => array('accordion' => $settings)), 'setting');
  drupal_add_js(drupal_get_path('module', 'openid_ext') . '/files/openid_ext.js');
  drupal_add_css($cache_css);

  return $out;
}
