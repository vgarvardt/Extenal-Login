<?php
// $Id$

/**
 * @file
 * External Login helper routines.
 */

/**
 * Gets available External Login providers list.
 *
 * @param $only_enabled
 *   Return only enabled providers or all
 * @return
 *   Array of available providers
 */
function external_login_get_providers($only_enabled = TRUE) {
  static $all_providers = array();
  static $enabled_providers = array();

  if (!$all_providers) {
    $all_providers = module_invoke_all('external_login_providers');

    $enabled = variable_get('external_login_enabled_providers', array('openid' => 0));
    foreach ($enabled as $pname => $weight) {
      if (isset($all_providers[$pname])) {
        $enabled_providers[$pname] = $all_providers[$pname];
      }
    }
  }

  return ($only_enabled ? $enabled_providers : $all_providers);
}

/**
 * Gets available External Login themes list.
 *
 * @return
 *   Array of themes
 */
function external_login_get_themes() {
  static $themes = array();

  if (!$themes) {
    $themes = module_invoke_all('external_login_themes');
  }

  return $themes;
}

/**
 * Try to get valid Username from external identifier.
 *
 * @param $openid_username
 *   External identifier returned from provider
 * @return
 *   Valid Username of FALSE if failed to normalize
 */
function external_login_normalize_username($openid_username) {
  foreach (external_login_get_providers() as $pname => $data) {
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
 * Helper function for creating External Login CSS cache.
 *
 * @param $styles
 *   Array with styles.
 * @param $cache_css
 *   CSS cache file path.
 */
function external_login_write_cache_styles($styles, $cache_css) {
  $fp = fopen($cache_css, 'w');
  foreach ($styles as $selector => $style) {
    fwrite($fp, sprintf("#external-login-block-form %s { %s }\r\n", $selector, $style));
  }
  fclose($fp);
}

/**
 * Helper function for generationg cache IDs according to current language.
 *
 * @return
 *   Array with proper cache IDs.
 */
function external_login_get_cids() {
  global $language;
  $lang_code = ($language ? $language->language : 'na');
  $cache_html_cid = 'external_login:themed_block_form:' . $lang_code;
  $cache_settings_cid = 'external_login:themed_block_settings:' . $lang_code;

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
function external_login_get_cache_values($cache_html_cid, $cache_settings_cid) {
  $cache_html = cache_get($cache_html_cid);
  $cache_settings = cache_get($cache_settings_cid);
  $cache_css = file_directory_path() . '/external_login.providers.css';

  return array($cache_html, $cache_settings, $cache_css);
}
