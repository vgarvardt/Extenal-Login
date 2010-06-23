<?php
// $Id$

/**
 * @file
 * OpenID Extension helper routines.
 */

/**
 * Gets available OpenID Extensions providers list.
 *
 * @param $only_enabled
 *   Return only enabled providers or all
 * @return
 *   Array of available providers
 */
function openid_ext_get_providers($only_enabled = TRUE) {
  static $all_providers = array();
  static $enabled_providers = array();

  if (!$all_providers) {
    $all_providers = module_invoke_all('openid_providers');

    $enabled = variable_get('openid_ext_enabled_providers', array('openid' => 0));
    foreach ($enabled as $pname => $weight) {
      if (isset($all_providers[$pname])) {
        $enabled_providers[$pname] = $all_providers[$pname];
      }
    }
  }

  return ($only_enabled ? $enabled_providers : $all_providers);
}

/**
 * Gets available OpenID Extensions themes list.
 *
 * @return
 *   Array of themes
 */
function openid_ext_get_themes() {
  static $themes = array();

  if (!$themes) {
    $themes = module_invoke_all('openid_themes');
  }

  return $themes;
}

/**
 * Try to get valid Username from OpenID identifier.
 *
 * @param $openid_username
 *   OpenID identifier returned from provider
 * @return
 *   Valid Username of FALSE if failed to normalize
 */
function openid_ext_normalize_username($openid_username) {
  foreach (openid_ext_get_providers() as $pname => $data) {
    $hook = 'normalize_' . $pname;
    foreach (module_implements($hook) as $module) {
      $function = $module . '_' . $hook;
      $username = $function($openid_username);
      if ($username) return $username;
    }
  }
  return FALSE;
}

/**
 * Helper function for creating OpenID Extensions CSS cache.
 *
 * @param $styles
 *   Array with styles.
 * @param $cache_css
 *   CSS cache file path.
 */
function openid_ext_write_cache_styles($styles, $cache_css) {
  $fp = fopen($cache_css, 'w');
  foreach ($styles as $selector => $style) {
    fwrite($fp, sprintf("#openid-ext-block-form %s { %s }\r\n", $selector, $style));
  }
  fclose($fp);
}

/**
 * Helper function for generationg cache IDs according to current language.
 *
 * @return
 *   Array with proper cache IDs.
 */
function openid_ext_get_cids() {
  global $language;
  $lang_code = ($language ? $language->language : 'na');
  $cache_html_cid = 'openid_ext:themed_block_form:' . $lang_code;
  $cache_settings_cid = 'openid_ext:themed_block_settings:' . $lang_code;

  return array($cache_html_cid, $cache_settings_cid);
}

/**
 * Gets current theme cached values.
 *
 * @param $cache_html_cid
 *   Cache ID for themed form HTML.
 * @param $cache_settings_cid
 *   Cache ID for theme JS settings.
 * @return
 *   Array with 3 values:
 *   - HTML cache
 *   - Settings cache
 *   - Cached theme css path
 */
function openid_ext_get_cache_values($cache_html_cid, $cache_settings_cid) {
  $cache_html = cache_get($cache_html_cid);
  $cache_settings = cache_get($cache_settings_cid);
  $cache_css = file_directory_path() . '/openid_ext.providers.css';

  return array($cache_html, $cache_settings, $cache_css);
}
