<?php
// $Id: phpbbforum.pages.inc,v 1.14 2009/05/10 14:29:39 vb Exp $
/**
 * Copyright 2007-2009 by Vadim G.B. (http://vgb.org.ru)
 */
 
/**
 * @file
 * User page callbacks for the forum module.
 */
if (variable_get("phpbbforum_page_frame", 0) == '1' || variable_get("phpbbforum_page_frame", 0) == '2') {

  function phpbbforum_page() {
    global $phpbb_config, $phpbb_user, $user, $phpbb_root_path, $phpEx, $_phpbb_base_path,
    $site_base_url, $site_forum_url, $site_phpbb_page, $_phpbb_result, $_phpbb_embed_mode,
    $_phpbb_integration_mode, $_phpbb_acp_integration_mode;

    if (!_phpbbforum_settings())
      return '';

    if ($_REQUEST['form_id'] == 'user_login_block' && $_REQUEST['op'] == 'Log in') {
      return '';
    }
    
    $query_string = $_SERVER['QUERY_STRING'];
    
    $query_string = urldecode($query_string);
    
    $tags = array('<iframe>', '</iframe>', '/iframe',
                  '&lt;iframe&gt;', '&lt;/iframe&gt;',
                  '<script>', '</script>',
                  '&lt;script&gt;', '&lt;/script&gt;',
                  );
    $query_string = str_replace($tags, '', $query_string);
    
    $query_string = check_plain($query_string);
    $query_string = str_replace('&amp;', '&', $query_string);
    

//http://vgb.net.ru/phpbbforum/viewtopic.php?f=2&t=13
//http://vgb.net.ru/phpbbforum/viewtopic.php?f=2&t=13&hilit="></iframe><script>javascript:alert(document.cookie)</script>

   // if (PHPBB_MODULE_DEBUG)
    //drupal_set_message('QUERY_STRING:'.$query_string .' q= '. $_GET['q']);

    $phpbbaction = $query_string;

    $query = array();

    parse_str($query_string, $query);

    /*
    $args = array(
      "/viewtopic.".$phpEx,
      "/viewforum.".$phpEx,
      "/index.".$phpEx,
      );
    */
    $args = array(
      "/viewonline.php",
      "/memberlist.php",
      "/viewtopic.php",
      "/viewforum.php",
      "/posting.php",
      "/search.php",
      "/ucp.php",
      "/report.php",
      "/mcp.php",
      "/adm/index.php",
      "/index.php",
      "/faq.php",
    );

    $isamp = false;
    $isamp1 = false;
    $strfound = false;

    $str = 'q='. $site_phpbb_page;
    $len = strlen($str);
    $stra = $str .'&';
    $lena = $len + 1;
    if (substr($phpbbaction, 0, $lena) == $stra) {
      $phpbbaction = substr($phpbbaction, $lena);
      $isamp1 = true;
    }
    elseif (substr($phpbbaction, 0, $len) == $str)
      $phpbbaction = substr($phpbbaction, $len);

    //if (PHPBB_MODULE_DEBUG)
    //drupal_set_message('QUERY_STRING='.$phpbbaction);

    foreach ($args as $str) {
      $isamp = false;
      $len = strlen($str);
      $stra = $str .'&';
      $lena = $len + 1;
      if (substr($phpbbaction, 0, $lena) == $stra) {
        $phpbbaction = substr($phpbbaction, $lena);
        $isamp = true;
        $strfound = true;
        break;
      }
      elseif (substr($phpbbaction, 0, $len) == $str) {
        $phpbbaction = substr($phpbbaction, $len);
        $strfound = true;
        break;
      }
    }

    if (!$strfound)
      $str = '';

    //if (PHPBB_MODULE_DEBUG)
    //  drupal_set_message('pa='.$phpbbaction);

    $path = $phpbb_config['forum_url'];
    $phpbbforum_url = $site_forum_url;
    //if (PHPBB_MODULE_DEBUG)
    //  drupal_set_message('path='.$path);
    //if (PHPBB_MODULE_DEBUG)
    //  $path = "http://phpbb.vgb.net.ru";

    if ($strfound) {
      $path .= $str;
      $phpbbforum_url .= $str;
    }

    if ($phpbbaction != "") {
      if ($isamp1 || $isamp) {
        $path .= '?';
        $phpbbforum_url .= '?';
      }
      $path .= $phpbbaction;
      $phpbbforum_url .= $phpbbaction;
    }

/*
    if (PHPBB_MODULE_DEBUG) {
      if ($isamp1 || $isamp)
        $amp = '?';
      else
        $amp = '';
      //drupal_set_message('path='.$path.' str='.$str.' action='.$str.$amp.$phpbbaction);
      drupal_set_message('phpbbforum='.$phpbbforum_url);
    }
 */
    //$output .= $path;
    //return $output;

    //drupal_set_message('1 str='.$str.' Frame='. $path);

    if ($str == '/adm/index.php') {
      if ($_phpbb_acp_integration_mode == '0') {
        drupal_goto($path);
      }
      elseif ($_phpbb_acp_integration_mode == '1') {
        return _phpbbforum_page_inframe($path);
      }
    }  

    if ($_phpbb_integration_mode == '1') {
      return _phpbbforum_page_inframe($path);
    }

    $_phpbb_result = _phpbbforum_get_result();
    if (!empty($_phpbb_result)) {
      _phpbbforum_set_result();
    }

    if (isset($_phpbb_result['hook']) && $_phpbb_result['status'] == 'redirect') {
        // execute hook
        _phpbbforum_hook($_phpbb_result['hook']);

        unset($_phpbb_result['hook']);
    }

    if (phpbb_user_new_privmsg()) {
      //$url = $site_base_url .'/'. $site_phpbb_page .'/ucp.php?i=pm&mode=popup';
      //drupal_goto($url);
      $str = '/ucp.php';
      $_REQUEST['i'] = 'pm';
      $_REQUEST['mode'] = 'popup';
    }
    if (isset($_phpbb_result['status']) && $_phpbb_result['status'] == 'exit') {
      // get previous output
      $output = $_phpbb_result['output'];
    }
    else {

      $query['path'] = $str;
      $_phpbb_result['query'] = $query;
      $_phpbb_embed_mode['query'] = $query;
      $_phpbb_embed_mode['redirect'] = variable_get('phpbbforum_redirect', 0);
      $destination_prev = _phpbbforum_get_destination();
      _phpbbforum_set_destination($phpbbforum_url, $query);

      switch($str) {

        case '/index.php':
        case '':
          $output = phpbb_api_index('/index.php');
        break;

        case '/viewforum.php':
          $output = phpbb_api_index($str);
        break;
        case '/viewtopic.php':
          //$query['path'] = $str;
          //_phpbbforum_set_destination($phpbbforum_url, $query);
          _phpbbforum_set_extra($query);

          $update_topic_view = false;
          $query_prev = $destination_prev['query'];
          //$start_prev = $query_prev['start'];
          $start = $query['start'];
          if ($query_prev['path'] != $str || $query_prev['t'] != $query['t'] || $query_prev['f'] != $query['f'] || !isset($start)) {
            $update_topic_view = true;
          }
          $_phpbb_embed_mode['update_topic_view'] = $update_topic_view;

          $output = phpbb_api_index($str);
        break;
        case '/viewonline.php':
          $output = phpbb_api_index($str);
        break;

        case '/memberlist.php':
          $output = phpbb_api_index($str);
        break;

        case '/ucp.php':
          //$query['path'] = $str;
          //_phpbbforum_set_destination($phpbbforum_url, $query);
          $output = phpbb_api_index($str);
        break;

        case '/mcp.php':
          $query_prev = $destination_prev['query'];
          $t = $query_prev['t'];
          if ($t) {
            $query['t'] = $t;
          }
          $query['path'] = $str;
          _phpbbforum_set_destination($phpbbforum_url, $query);
          _phpbbforum_set_extra($query);
          $request['action'] = $_REQUEST['action'];
          _phpbbforum_set_request($request);
          $output = phpbb_api_index($str);
        break;

        case '/search.php':
          $output = phpbb_api_index($str);
        break;

        case '/posting.php':
          $output = phpbb_api_index($str);
        break;

        case '/report.php':
        case '/faq.php':
          $output = phpbb_api_index($str);
        break;

        default:
          $output = _phpbbforum_page_inframe($path);
          return $output;
      }

      $_phpbb_result = _phpbbforum_get_result();

      if (isset($_phpbb_result['hook'])) {
        // execute hook
        _phpbbforum_hook($_phpbb_result['hook']);
      }

    } //***

    $_phpbb_theme_base_path = $_phpbb_base_path .'styles/'. $phpbb_user->theme['theme_path'];

    $output = theme('phpbbforum_style', $phpbb_user->theme['theme_path'], $_phpbb_theme_base_path, $output);
    
    $_phpbb_result = _phpbbforum_set_result(array());

    return $output;
  }

  /**
   * prints page in a iframe.
   */
  function _phpbbforum_page_inframe($path) {

    // Added this to allow the iframe to resize dynamically according to the
    // page height. -- Daen
    drupal_add_js(drupal_get_path('module', PHPBB_DRUPAL_MODULE) .'/js/phpbbframe.js', 'module');

    $width = variable_get("phpbbforum_page_width", "100%");
    $height = variable_get("phpbbforum_page_height", "1024");
    $scrolling = variable_get("phpbbforum_page_scroll", 2);

    $output = '<iframe id="forumFrame" src="'. $path .'"'; // name="phpbbforum"';

    if ($width && $height) {
      $output .= ' width="'. $width .'"'.' height="'. $height .'"';
    }
    $output .= ' frameborder="0" framespacing="0"';
    if ($scrolling == 2)
      $output .= ' scrolling="auto"';
    elseif ($scrolling == 1)
      $output .= ' scrolling="yes"';
    else
      $output .= ' scrolling="no"';
    $output .= ' allowtransparency="false"';

    $output .= '><p>'. t('Sorry your browser does not work') .'</p></iframe>';

    return $output;
  }


/**
 * replaces forum url.
 */
function _phpbbforum_replace_forum_url($output) {
  global $phpbb_config, $site_forum_url;

  $phpbb_url = $phpbb_config['forum_url'];
  $output = str_replace($phpbb_url, $site_forum_url, $output);
  return $output;
}

/**
 * replaces block links.
 */
function _phpbbforum_replace_block_links($output) {
  $output = _phpbbforum_replace_forum_url($output);
  $output = _phpbbforum_restore_block_links($output);
  return $output;
}
/**
 * restore block links.
 */
function _phpbbforum_restore_block_links($output) {
  global $phpbb_config, $site_forum_url, $site_phpbb_page;

  $phpbb_url = $phpbb_config['forum_url'];

  // replace back phpbb images
  $frameurl = $site_forum_url;
  $imgsrc = '<img src="';
  $frameurl = $imgsrc . $frameurl;
  $phpbburl = $imgsrc . $phpbb_url;
  $output = str_replace($frameurl, $phpbburl, $output);

  $path = '/images';
  $frameurl = $site_forum_url . $path;
  $phpbburl = $phpbb_url . $path;
  $output = str_replace($frameurl, $phpbburl, $output);

  // replace relative phpbb images paths
  $relurl = '<img src="images';
  $phpbburl = '<img src="'. $phpbb_url .'/images';
  $output = str_replace($relurl, $phpbburl, $output);

  return $output;
}

/**
 * replaces links.
 */
function _phpbbforum_replace_links($output) {
  $output = _phpbbforum_replace_forum_url($output);
  $output = _phpbbforum_restore_links($output);
  return $output;
}

/**
 * restore links.
 */
function _phpbbforum_restore_links($output) {
  global $phpbb_config, $site_forum_url, $site_phpbb_page,
  $_phpbb_result, $_phpbb_embed_mode, $_phpbb_acp_integration_mode;

  $phpbb_url = $phpbb_config['forum_url'];
  //$str = $_phpbb_embed_mode['path'];

  // replace back phpbb images
  $frameurl = $site_forum_url;
  $imgsrc = '<img src="';
  $frameurl = $imgsrc . $frameurl;
  $phpbburl = $imgsrc . $phpbb_url;
  $output = str_replace($frameurl, $phpbburl, $output);

  $path = '/styles';
  $frameurl = $site_forum_url . $path;
  $phpbburl = $phpbb_url . $path;
  $output = str_replace($frameurl, $phpbburl, $output);

  $path = '/images';
  $frameurl = $site_forum_url . $path;
  $phpbburl = $phpbb_url . $path;
  $output = str_replace($frameurl, $phpbburl, $output);

  // replace relative phpbb images paths
  $relurl = '<img src="images';
  $phpbburl = '<img src="'. $phpbb_url .'/images';
  $output = str_replace($relurl, $phpbburl, $output);

  // Find a member in popup
  $mode = '/memberlist.php?mode=searchuser';
  $frameurl = $site_forum_url . $mode;
  $phpbburl = $phpbb_url . $mode;
  $output = str_replace($frameurl, $phpbburl, $output);

  //http://drupalbridge.org/forum/posting.php?mode=smilies&f=3
  $mode = '/posting.php?mode=smilies';
  $frameurl = $site_forum_url . $mode;
  $phpbburl = $phpbb_url . $mode;
  $output = str_replace($frameurl, $phpbburl, $output);

  if (/*$str == '/adm/index.php' &&*/ $_phpbb_acp_integration_mode == '0') {
    $mode = '/adm/index.php';
    $frameurl = $site_forum_url . $mode;
    $phpbburl = $phpbb_url . $mode;
    $output = str_replace($frameurl, $phpbburl, $output);
  }
  // Download
  $path = '/download';
  $frameurl = $site_forum_url . $path;
  $phpbburl = $phpbb_url . $path;
  $output = str_replace($frameurl, $phpbburl, $output);

  return $output;
}

} // if (variable_get("phpbbforum_page_frame", 0) == '1' || variable_get("phpbbforum_page_frame", 0) == '2') {

if (variable_get("phpbbforum_page_frame", 0) == '2') {
  /**
   * sets extra vars.
   */
  function _phpbbforum_set_extra($query) {

    global $forum_id, $topic_id, $post_id, $user_id;

    if ($query['f']) {
      $forum_id = $query['f'];
    }
    if ($query['t']) {
      $topic_id = $query['t'];
    }
    if ($query['p']) {
      $post_id = $query['p'];
    }
    if ($query['u']) {
      $user_id = $query['u'];
    }
  }

} // if (variable_get("phpbbforum_page_frame", 0) == '2') {
