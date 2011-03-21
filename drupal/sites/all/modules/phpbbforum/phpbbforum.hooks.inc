<?php
// $Id: phpbbforum.hooks.inc,v 1.4 2009/04/04 17:51:40 vb Exp $
/**
 * Copyright 2007-2009 by Vadim G.B. (http://vgb.org.ru)
 */

//if (variable_get("phpbbforum_page_frame", 0) == '1' || variable_get("phpbbforum_page_frame", 0) == '2') {

/**
 * replaces msg links.
 */
function _phpbbforum_replace_msg_links($output) {
  //global $site_forum_url;

  //if (strpos($output, $site_forum_url) === false) {
    $output = _phpbbforum_replace_links($output);
  //}
  return $output;
}


/**
 * replaces urls.
 */
function _phpbbforum_replace_urls($output, $decode = false) {
  global $phpbb_config, $site_forum_url, $site_phpbb_page, $phpbb_root_path;

  $phpbb_url = $phpbb_config['forum_url'];
  //http://vgb.net.ru/phpbbforum/mcp.php?i=reports&mode=reports
  $q_phpbbforum = 'q='. $site_phpbb_page;
  $str = array('../', '/index.php?'. $q_phpbbforum);
  //$str = array('../', 'www/index.php?'. $q_phpbbforum, '/index.php?'. $q_phpbbforum, 'www/index.php');
  $output = str_replace($str, '', $output);
  //http://vgb.net.ru/phpbbforumforum/index.php?q=phpbbforum/mcp.php
  if (strpos($output, $site_forum_url) === false) {
    $output = str_replace($phpbb_url, $site_forum_url, $output);
    $output = str_replace($phpbb_root_path, $site_forum_url .'/', $output);
  }
  if ($decode) {
    $output = urldecode($output);
  }
  return $output;
}

/**
 * _phpbbforum_set_result.
 */
function _phpbbforum_set_result($result = array()) {
  /*global $_phpbb_result;
  if (empty($result))
    $_phpbb_result = $result;*/
  variable_set("phpbbforum_result", $result);
  return $result;
}

/**
 * _phpbbforum_get_result.
 */
function _phpbbforum_get_result() {
  $result = variable_get("phpbbforum_result", array());
  return $result;
}

/**
 * _phpbbforum_set_request.
 */
function _phpbbforum_set_request($request = array()) {
  variable_set("phpbbforum_request", $request);
  return $request;
}

/**
 * _phpbbforum_get_request.
 */
function _phpbbforum_get_request() {
  $request = variable_get("phpbbforum_request", array());
  return $request;
}

/**
 * _phpbbforum_set_destination.
 */
function _phpbbforum_set_destination($url = '', $query = array()) {
  $destination = array('url' => $url, 'query' => $query);
  variable_set("phpbbforum_destination", $destination);
  return $destination;
}

/**
 * _phpbbforum_get_destination.
 */
function _phpbbforum_get_destination() {
  $destination = variable_get("phpbbforum_destination", array());
  return $destination;
}

/**
 * _phpbbforum_goto.
 */
function _phpbbforum_goto($url, $drupal = true) {
  @restore_error_handler();
  phpbb_load();

  if (drupal)
    drupal_goto($url);
	header('Location: ' . $url);
  exit;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * _phpbbforum_hook
 */
function _phpbbforum_hook($hook) {
  $name = $hook['name'];
  $fn = '_phpbbforum_hook_'. $name;
  if (function_exists($fn)) {
    $fn($hook);
  }
}

/**
 * _phpbbforum_hook_login
 */
function _phpbbforum_hook_login($hook) {
  $redirect = _phpbbforum_replace_urls($hook['redirect']);
  _phpbbforum_login_external($redirect);
}

/**
 * _phpbbforum_hook_logout
 */
function _phpbbforum_hook_logout($hook) {
  $redirect = _phpbbforum_replace_forum_url($hook['redirect']);
  _phpbbforum_logout_external($redirect);
}

/**
 * _phpbbforum_hook_user_delete
 */
function _phpbbforum_hook_user_delete($hook) {
  _phpbbforum_user_delete($hook['username']);
}

//} // if (variable_get("phpbbforum_page_frame", 0) == '1' || variable_get("phpbbforum_page_frame", 0) == '2') {

////////////////////////////////////////////////////////////////////////////////

function _phpbbforum_login_external($redirect = '') {
  if (_phpbbforum_drupal_login()) {
    $url = $redirect;
    if (variable_get("phpbbforum_page_frame", 0) == '2') {
      $destination = _phpbbforum_get_destination();
      $url = $destination['url'];
      _phpbbforum_set_destination();
    }
    drupal_goto($url);
  }
}

function _phpbbforum_logout_external($redirect = '') {
  _phpbbforum_drupal_logout($redirect);
}

function _phpbbforum_user_delete($username) {
  $account = user_load(array('name' => $username));
  if ($account) {
    user_delete($account, $account->uid);
  }
}

