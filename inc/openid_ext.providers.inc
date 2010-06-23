<?php
// $Id$

/**
 * @file
 * OpenID Extension core routines.
 */

/**
 * Implementatioin of hook_openid_providers().
 *
 * @return
 *   Assoc array of arrays where key is unique OpenID provider identifier used in system.
 *   Value is assoc array with the following key-value pairs:
 *   - short - short name of OpenID provider
 *   - title - full name of  OpenID provider
 *   - input (optional) - textfield title, if required
 *   - path - OpenID provider URL (when static) or function name that gets input value
 *     and returns provider URL. If URL is dynamic and path is function name validation
 *     function should be implemented also - {path}_validate - which takes input value
 *     as parameter and returns error text if value is not valid
 *   - icon - OpenID provider logo icon, path relative to Drupal installation root
 *   - verify (optional) - function to verify if provider is supported in your system
 */
function openid_ext_openid_providers() {
  $files_path = drupal_get_path('module', 'openid_ext') . '/files/';

  return array(
    'openid' => array(
      'short' => t('Any OpenID'),
      'title' => t('OpenID Provider'),
      'input' => t('Identifier'),
      'path' => 'openid_ext_provider_openid',
      'icon' => $files_path . 'openid.png',
    ),
    'livejournal' => array(
      'short' => t('LiveJournal'),
      'title' => t('LiveJournal User'),
      'input' => t('Username'),
      'path' => 'openid_ext_provider_livejournal',
      'icon' => $files_path . 'livejournal.png',
    ),
    'yandex' => array(
      'short' => t('Yandex'),
      'title' => t('Yandex User'),
      'path' => 'http://yandex.ru',
      'icon' => $files_path . 'yandex.png',
    ),
    'rambler' => array(
      'short' => t('Rambler'),
      'title' => t('Rambler User'),
      'path' => 'http://rambler.ru',
      'icon' => $files_path . 'rambler.png',
    ),
    'google' => array(
      'short' => t('Google'),
      'title' => t('Google User'),
      'path' => 'https://www.google.com/accounts/o8/id',
      'icon' => $files_path . 'google.png',
      'verify' => 'openid_ext_verify_openssl',
    ),
    'yahoo' => array(
      'short' => t('Yahoo!'),
      'title' => t('Yahoo! User'),
      'path' => 'http://www.yahoo.com',
      'icon' => $files_path . 'yahoo.png',
    ),
    'chimp' => array(
      'short' => t('Chi.mp'),
      'title' => t('Chi.mp User'),
      'input' => t('Your name'),
      'path' => 'openid_ext_provider_chimp',
      'icon' => $files_path . 'chimp.png',
      'verify' => 'openid_ext_verify_openssl',
    ),
    'myopenid' => array(
      'short' => t('myOpenID'),
      'title' => t('myOpenID User'),
      'input' => t('Username'),
      'path' => 'openid_ext_provider_myopenid',
      'icon' => $files_path . 'myopenid.png',
    ),
  );
}

/**
 * Implementatioin of hook_openid_themes().
 *
 * @return
 *   Assoc array with theme name from hook_theme() as key and theme title as value.
 */
function openid_ext_openid_themes() {
  return array(
      'openid_ext_default' => t('Default'),
      'openid_ext_dropdown' => t('Dropdown'),
      'openid_ext_accordion' => t('Accordion'),
  );
}

/**
 * Path function for 'Any OpenID' provider.
 *
 * @param $input_value
 *   Input value.
 * @return
 *   Provider URL.
 */
function openid_ext_provider_openid($input_value) {
  // We already validated if identifier is valid URL so return it as is
  return $input_value;
}

/**
 * Validation function for 'Any OpenID' provider.
 *
 * @param $input_value
 *   Input value.
 * @return
 *   Error text, if value is valid.
 */
function openid_ext_provider_openid_validate($input_value) {
  // both absolute url and domain name are valid urls for OpenID module
  if (!valid_url($input_value, TRUE) && !valid_url('http://' . $input_value, TRUE)) {
    return t('OpenID identifier should be valid URL');
  }
}

/**
 * Path function for 'LiveJournal' provider.
 *
 * @param $input_value
 *   Input value.
 * @return
 *   Provider URL.
 */
function openid_ext_provider_livejournal($input_value) {
  return 'http://' . $input_value . '.livejournal.com';
}

/**
 * Validation function for 'LiveJournal' provider.
 *
 * @param $input_value
 *   Input value.
 * @return
 *   Error text, if value is valid.
 */
function openid_ext_provider_livejournal_validate($input_value) {
  $lj_url = openid_ext_provider_livejournal($input_value);
  if (!valid_url($lj_url, TRUE)) {
    return t('LiveJournal username should be part of your LJ account (e.g. "dkrig" if your LJ address is "dkrig.livejournal.com")');
  }
}

/**
 * Path function for 'Chi.mp' provider.
 *
 * @param $input_value
 *   Input value.
 * @return
 *   Provider URL.
 */
function openid_ext_provider_chimp($input_value) {
  return 'https://' . $input_value . '.mp';
}

/**
 * Validation function for 'Chi.mp' provider.
 *
 * @param $input_value
 *   Input value.
 * @return
 *   Error text, if value is valid.
 */
function openid_ext_provider_chimp_validate($input_value) {
  $chimp_url = openid_ext_provider_chimp($input_value);
  if (!valid_url($chimp_url, TRUE)) {
    return t('Chi.mp name should be part of your .mp account (e.g. "bob" if your domain is "bob.mp")');
  }
}

/**
 * Path function for 'myOpenID' provider.
 *
 * @param $input_value
 *   Input value.
 * @return
 *   Provider URL.
 */
function openid_ext_provider_myopenid($input_value) {
  return 'http://' . $input_value . '.myopenid.com';
}

/**
 * Validation function for 'myOpenID' provider.
 *
 * @param $input_value
 *   Input value.
 * @return
 *   Error text, if value is valid.
 */
function openid_ext_provider_myopenid_validate($input_value) {
  $myopenid_url = openid_ext_provider_myopenid($input_value);
  if (!valid_url($myopenid_url, TRUE)) {
    return t('myOpenID user should be part of your myOpenID account (e.g. "krig" if your domain is "krig.myopenid.com")');
  }
}

/**
 * Normalization function for username field for LiveJournal OpenID.
 *
 * Function is useful when provider did not return Username and Drupal OpenID module
 * uses OpenID identifier for Username.
 *
 * @param $value
 *   OpenID identifier string received from provider
 * @return
 *   Normalized username if possible or FALSE if not
 */
function openid_ext_normalize_livejournal($value) {
  if (preg_match('#http:\/\/(.*)\.livejournal\.com#', $value, $matches)) {
    return $matches[1];
  }
  return FALSE;
}

/**
 * Normalization function for username field for Yandex OpenID
 *
 * Function is useful when provider did not return Username and Drupal OpenID module
 * uses OpenID identifier for Username
 *
 * @param $value
 *   OpenID identifier string received from provider
 * @return
 *   Normalized username if possible or FALSE if not
 */
function openid_ext_normalize_yandex($value) {
  if (preg_match('#http:\/\/openid\.yandex\.ru\/(.*)\/#', $value, $matches)) {
    return $matches[1];
  }
  return FALSE;
}

/**
 * Check if OpenSSL installed.
 *
 * This verification required to check if providers with HTTPS schema will work properly with
 * current PHP installation
 *
 * @return
 *   Error text if verification fails, NULL otherwise
 */
function openid_ext_verify_openssl() {
  if (!function_exists('openssl_open')) {
    return t('OpenSSL is not installed.');
  }
}