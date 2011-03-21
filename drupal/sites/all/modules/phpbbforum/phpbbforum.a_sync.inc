<?php
// $Id: phpbbforum.a_sync.inc,v 1.1 2009/02/27 21:10:26 vb Exp $
/**
 * Copyright 2008 by Vadim G.B. (http://vgb.org.ru)
 */

function _phpbbforum_a_sync_set($val) {
  if (module_exists('a_sync'))
    module_invoke('a_sync', 'set', 'phpbbforum', 'user', $val);
}

function _phpbbforum_a_sync_get() {
  if (module_exists('a_sync'))
    return module_invoke('a_sync', 'get', 'user');
  return 0;  
}

function _phpbbforum_a_sync_get_module($var) {
  if (module_exists('a_sync'))
    return module_invoke('a_sync', 'get_module', $var);
  return false;
}

function _phpbbforum_a_sync_get_sync_to() {
  if (module_exists('a_sync'))
    return module_invoke('a_sync', 'get_sync_to', 'phpbbforum', 'user');
  return 0;  
}

function _phpbbforum_get_a_sync_to($syncto) {
  
  $module = _phpbbforum_a_sync_get_module('user');
  if ($module === false)
    return $syncto;
  
  if ($module != 'phpbbforum' && $module != '') {
    $a_syncto = _phpbbforum_a_sync_get_sync_to();
    if ($a_syncto == A_SYNC_TO_ME) {
      $syncto = PHPBB_SYNC_TO_DRUPAL;
    }  
    else {
      $syncto = PHPBB_SYNC_TO_PHPBB;
    }  
  }  
  
  return $syncto;  
}

function _phpbbforum_set_a_sync_to($sync) {
  
  $syncto = $sync;  
  if ($sync == PHPBB_SYNC_TO_MASTER) {
    $master = variable_get('phpbbforum_master', 1);
    if ($master) {
      $syncto = PHPBB_SYNC_TO_DRUPAL;
    }  
    else {
      $syncto = PHPBB_SYNC_TO_PHPBB;
    }  
  }
  
  $module = _phpbbforum_a_sync_get_module('user');
  if ($module === false)
    return $syncto;
  
  if ($module == '') {
    if ($syncto == PHPBB_SYNC_TO_DRUPAL) {
      $a_syncto = A_SYNC_TO_ME;
    }  
    else {
      $a_syncto = A_SYNC_TO_EXTERNAL;
    }
    _phpbbforum_a_sync_set($a_syncto);
  }
  
  return $syncto;  
}

