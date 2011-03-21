<?php
// $Id: phpbb_api.php,v 1.19 2009/04/21 19:37:51 vb Exp $
/**
 * Copyright (C) 2007-2009 by Vadim G.B. (http://vgb.org.ru)
 */

define('IN_PHPBB', true);
define('PHPBB_API_EMBEDDED', true);
define('PHPBB_EMBEDDED', true);
define('PHPBB_API_DEBUG', 0);
define('VBRIDGE_DEBUG', 0);

//@define('PHPBB_DB_PCONNECT', true);
@define('PHPBB_DB_PCONNECT', false);
@define('PHPBB_DB_NEW_LINK', true);

define('PHPBB_BOARD_URL_WITHOUT_PORT', true);
//define('PHPBB_BOARD_URL_WITHOUT_PORT', false);

// Report all errors, except notices
//if (PHPBB_API_DEBUG)
//  error_reporting(E_ALL);
//else
error_reporting(E_ALL ^ E_NOTICE);

global $phpbb_config, $phpbb_user,
$user, $config, $cache, $auth, $template, $db, $module, $refresh, $submit, $preview,
$phpbb_root_path, $phpEx,
$vbridge_root_path, $_vbridge,
$phpbb_db_type, $phpbb_db, $phpbb_connection, $phpbb_db_prefix, $table_prefix,
$site_base_url, $_drupal_base_path, $site_phpbb_page, $site_forum_url, $_phpbb_forum_url,
$_phpbb_integration_mode, $_phpbb_acp_integration_mode,
$_phpbb_result, $_phpbb_embed_mode,
$site_user, $_site_error_handler, $_site_context_saved,
$phpbb_hook;

if (!isset($vbridge_root_path) || empty($vbridge_root_path))
  $vbridge_root_path = dirname(__FILE__) .'/phpbbvbridge';
  
if (!isset($phpbb_root_path) || empty($phpbb_root_path))
  $phpbb_root_path = dirname(__FILE__) .'/';

$phpEx = substr(strrchr(__FILE__, '.'), 1);

// If we are on PHP >= 6.0.0 we do not need some code
if (version_compare(PHP_VERSION, '6.0.0-dev', '>='))
{
	define('STRIP', false);
}
else
{
	set_magic_quotes_runtime(0);
	define('STRIP', (get_magic_quotes_gpc()) ? true : false);
}

if (!file_exists($phpbb_root_path . 'config.' . $phpEx))
{
	die("<p>The config.$phpEx file could not be found.</p><p><a href=\"{$phpbb_root_path}install/index.$phpEx\">Click here to install phpBB</a></p>");
}

require_once($phpbb_root_path . 'config.' . $phpEx);

// If PHPBB isn't defined, config.php is missing or corrupt
if (!defined('PHPBB_INSTALLED'))
{
	die("<p>The config.$phpEx file is not valid.</p><p><a href=\"{$phpbb_root_path}install/index.$phpEx\">Click here to install phpBB</a></p>");
}

$phpbb_db_type = $dbms;
$phpbb_db_prefix = $table_prefix;
//$phpbb_language_dir = $phpbb_root_path . 'language/';

// Load Extensions
if (!empty($load_extensions))
{
	$load_extensions = explode(',', $load_extensions);

	foreach ($load_extensions as $extension)
	{
		@dl(trim($extension));
	}
}

// Include files
require_once($phpbb_root_path .'includes/acm/acm_'. $acm_type .'.'. $phpEx);
require_once($phpbb_root_path .'includes/cache.'. $phpEx);
require_once($phpbb_root_path .'includes/template.'. $phpEx);
require_once($phpbb_root_path .'includes/session.'. $phpEx);
require_once($phpbb_root_path .'includes/auth.'. $phpEx);
require_once($phpbb_root_path .'includes/functions.'. $phpEx);
require_once($phpbb_root_path .'includes/functions_content.'. $phpEx);
require_once($phpbb_root_path .'includes/constants.'. $phpEx);
require_once($phpbb_root_path .'includes/db/'. $dbms .'.'. $phpEx);
require_once($phpbb_root_path .'includes/utf/utf_tools.'. $phpEx);
require_once($phpbb_root_path .'includes/hooks/index.'. $phpEx);
require_once($phpbb_root_path .'includes/functions_user.'. $phpEx);
require_once($phpbb_root_path .'includes/functions_posting.'. $phpEx);
/////////////////////////////////////////
require_once($vbridge_root_path .'/phpbb_api_subs.php');
require_once($vbridge_root_path .'/phpbb_api_hooks.php');
require_once($vbridge_root_path .'/phpbb_api_theme.php');
/////////////////////////////////////////
//
/////////////////////////////////////////
$bridgeData = array(
  '#name' => 'PhpBB+Drupal',
  '#class' => 'Phpbb',
  '#objclass' => array(
    'Db', 'Pass', 'Session', 'Qookie', 'Auth', 
    'AuthQookie', 'AuthQookieStorage', 
    'User'
   ),
  '#path' => $vbridge_root_path,
  '#url' => $site_forum_url,
  '#config' => array(
    'name' => 'val',
    '#parms' => array(
      array(
        'name' => 'val',
      ),
    ),
  )  
);

$bridgeAppData = array(
  array(
    '#name' => 'Drupal',
    '#type' => 'cms',
    '#path' => $_drupal_base_path,
    '#url' => $site_base_url,
    '#class' => 'Drupal',
    '#dir' => $vbridge_root_path,
    '#config' => array(
      'name' => 'val',
      '#hooks' => array(
        array(
          'login' => '',
          'logout' => '',
        ),
      ),
      '#settings' => array(
        array(
          'name' => 'val',
        ),
      ),
    ),
  ),
  array(
    '#name' => 'PhpBB',
    '#type' => 'forum',
    '#path' => $vbridge_root_path,
    '#url' => $site_forum_url,
    '#class' => 'Phpbb',
    '#dir' => $vbridge_root_path,
    '#config' => array(
      'name' => 'val',
      '#hooks' => array(
        array(
          'login' => '',
          'logout' => '',
        ),
      ),
      '#settings' => array(
        array(
          'name' => 'val',
        ),
      ),
    ),
  ),
);
/////////////////////////////////////////

// Load VBridge stuff.
require_once($vbridge_root_path .'/PhpbbVBridge.php');

function getBridge()
{
  global $_vbridge;
  return $_vbridge;
}

$_vbridge = PhpbbVBridge::getInstance($bridgeData, $bridgeAppData);

/*
if (VBRIDGE_DEBUG) {
  $message = 'Vbridge '. $_vbridge .' v'. getBridge()->getVersion() .', Drupal App='. getBridge()->getApp(0) .', PhpBB App='. getBridge()->getApp(1);
  drupal_set_message($message);  
}
*/

getBridge()->init();

getBridge()->run();

// Set PHP error handler to ours

if (!defined('PHPBB_ERROR_HANDLER')) {
  //define('PHPBB_ERROR_HANDLER', 'phpbb_error_msg_handler');
  define('PHPBB_ERROR_HANDLER', 'msg_handler');
}  

$site_user = $user;
$_site_context_saved = true;

$_site_error_handler = set_error_handler(PHPBB_ERROR_HANDLER);

// Instantiate some basic classes
$user	= new user();
$auth	= new auth();
$template	= new template();
$cache = new cache();

if (!is_object($db))
  $db	= new $sql_db();

if (!$phpbb_connection)
{
// Connect to DB
  $db->sql_connect($dbhost, $dbuser, $dbpasswd, $dbname, $dbport, 
    defined('PHPBB_DB_PCONNECT') ? PHPBB_DB_PCONNECT : false, 
    defined('PHPBB_DB_NEW_LINK') ? PHPBB_DB_NEW_LINK : false);
  if ($db->db_connect_id)
    $phpbb_connection = $db->db_connect_id;
}

// We do not need this any longer, unset for safety purposes
unset($dbpasswd);

// Grab global variables, re-cache if necessary
$config = $cache->obtain_config();
$phpbb_config = $config;

phpbb_hook_init();

// Start session management
$user->session_begin();
$auth->acl($user->data);
$user->setup();

$phpbb_config['forum_url'] = $_phpbb_forum_url = phpbb_generate_board_url(false, defined('PHPBB_BOARD_URL_WITHOUT_PORT') ? PHPBB_BOARD_URL_WITHOUT_PORT : false);

$phpbb_user	= $user;
$user = $site_user;

@restore_error_handler();
$_site_context_saved = false;
////////////////////////////////////////////////////////////////////////////////

function phpbb_api_get_user_name($user_id)
{
	global $phpbb_connection, $db;

  $username = "";
	if (!empty($user_id) && is_integer($user_id))
	{
		if (!$phpbb_connection)
			return $username;

		$sql = 'SELECT username
  		FROM ' . USERS_TABLE . '
  		WHERE user_id = ' . $user_id;
  	$result = $db->sql_query($sql);
  	list ($username) = $db->sql_fetchrow($result);
  	$db->sql_freeresult($result);
	}
	return $username;
}

function phpbb_api_get_user_id($username)
{
	global $phpbb_connection, $db;

  $user_id = 0;
	if (!empty($username))
	{
		if (!$phpbb_connection)
			return $user_id;

  	$sql = 'SELECT user_id
  		FROM ' . USERS_TABLE . "
  		WHERE username_clean = '" . $db->sql_escape(utf8_clean_string($username)) . "'";
  	$result = $db->sql_query($sql);
  	list ($user_id) = $db->sql_fetchrow($result);
  	$db->sql_freeresult($result);
	}
	return $user_id;
}

function phpbb_api_authenticate_user()
{
	return getBridge()->authenticateUser();
}

function phpbb_api_get_user($username, $password)
{
  return getBridge()->getAppUser($username, $password);
}

function phpbb_api_index($str)
{
	global $phpbb_connection, $phpbb_config, $phpbb_user, $_phpbb_result, $_phpbb_embed_mode, $_phpbb_forum_url;
	global $user, $config, $cache, $auth, $template, $db, $module, $refresh, $submit, $preview,
  $phpbb_root_path, $phpEx;

  if (!$phpbb_connection)
	  return false;
  
  $_phpbb_result['output'] = "";
  
  phpbb_save();

  // Set PHP error handler to ours
  $_site_error_handler = set_error_handler(PHPBB_ERROR_HANDLER);
  //$_site_error_handler = set_error_handler('msg_handler');
  
  try {

    include_once($phpbb_root_path . $str);

  } catch (PhpbbVBridgeException $e) {

     $code = $e->getCode();
     switch ($code) {
       case VBridgeException::THROW_OUTPUT:
         break;
       default:
         //$msg = $e->getMessage();
         break;
     }
  }

  @restore_error_handler();

  phpbb_load();

	return $_phpbb_result['output'];
}

/////////////////////////////////////////////////////////////////////////////////////

// Recent post list

function phpbb_api_recent_posts($options = array('output_method' => ''))
{
  global $vbridge_root_path;
  
  require_once($vbridge_root_path .'/phpbb_api_recent.php');
  
  return getBridge()->getRecentPosts($options);
}

// Recent topic list

function phpbb_api_recent_topics($options = array('output_method' => ''))
{
	global $vbridge_root_path;

  require_once($vbridge_root_path .'/phpbb_api_recent.php');
  
  return getBridge()->getRecentTopics($options);
}

// Show the top posters

function phpbb_api_topposter($options = array('output_method' => ''))
{
	return getBridge()->getTopPosters($options);
}

// Shows a list of online users...

function phpbb_api_whos_online($options = array('output_method' => ''))
{
  return getBridge()->getWhosOnline($options);
}

// Show some basic stats

function phpbb_api_board_stats($options = array('output_method' => ''))
{
	return getBridge()->getStatistics($options);
}

// Show Personal Messages

function phpbb_api_pm($options = array('output_method' => ''))
{
  return getBridge()->getPersonalMessages($options);
}


function phpbb_api_register($username, $password, $email, $data = array())
{
	return getBridge()->registerUser($username, $password, $email, $data);
}

function phpbb_api_update_user($id, $username = '', $password = '', $email = '', $data = array())
{
	return getBridge()->updateUser($id, $username, $password, $email, $data);
}

function phpbb_api_login($username = '', $password = '')
{
  return getBridge()->login($username, $password);
}

function phpbb_api_logout()
{
	getBridge()->logout();
}

function phpbb_api_user_delete($id, $mode = 'retain', $post_username = false)
{
	return getBridge()->deleteUser($id, $mode, $post_username);
}

function phpbb_api_user_name_validate($username)
{
  return getBridge()->validateUserName($username);
}

function phpbb_api_user_password_validate($password)
{
  return getBridge()->validateUserPassword($password);
}

function phpbb_api_user_email_validate($email)
{
  return getBridge()->validateUserEmail($email);
}


?>