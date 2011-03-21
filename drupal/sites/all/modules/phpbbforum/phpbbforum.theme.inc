<?php
// $Id: phpbbforum.theme.inc,v 1.15 2009/05/18 18:13:30 vb Exp $
/**
 * Copyright 2008-2009 by Vadim G.B. (http://vgb.org.ru)
 */

if (PHPBB_DRUPAL_MODULE_VERSION != 5) {
  /**
   * Implementation of hook_theme().
   */
  function phpbbforum_theme() {
    return array(
      'phpbbforum_whos_online' => array(
        'arguments' => array('output' => NULL, 'options' => NULL),
      ),
      'phpbbforum_recent_posts' => array(
        'arguments' => array('output' => NULL, 'options' => NULL),
      ),
      'phpbbforum_recent_topics' => array(
        'arguments' => array('output' => NULL, 'options' => NULL),
      ),
      'phpbbforum_board_stats' => array(
        'arguments' => array('output' => NULL, 'options' => NULL),
      ),
      'phpbbforum_pm' => array(
        'arguments' => array('output' => NULL, 'options' => NULL),
      ),
      'phpbbforum_topposter' => array(
        'arguments' => array('output' => NULL, 'options' => NULL),
      ),
      'phpbbforum_style' => array(
        'arguments' => array('style' => NULL, 'phpbb_theme_path' => NULL, 'output' => NULL),
      ),
    );
  }
}

function phpbbforum_whos_online() {
  global $phpbb_config, $phpbb_user, $_phpbb_integration_mode;

  $options = array(
    'num_top' => (int)variable_get('phpbbforum_num_whos_online', PHPBB_NUM_WHOS_ONLINE), 
    'output_method' => 'array',
  );
  $output = phpbb_api_whos_online($options);
  $output = theme('phpbbforum_whos_online', $output, $options);
  if ($_phpbb_integration_mode != '0')
    $output = _phpbbforum_replace_links($output);

  return $output;
}

function phpbbforum_recent_posts() {
  global $phpbb_config, $phpbb_user, $_phpbb_integration_mode;

  if (variable_get('phpbbforum_recent_posts_br', 1))
    $delimeter = PHPBB_RECENT_DELIMITER_BR;
  else
    $delimeter = PHPBB_RECENT_DELIMITER_SPACE;
  
  $options = array(
    'num_recent' => (int)variable_get('phpbbforum_num_recent_posts', PHPBB_NUM_RECENT_POSTS),
    'search_id' => 'newposts',
    'sort_days' => (int)variable_get('phpbbforum_num_recent_sort_days', 0),
    'output_tag' => '<ul class="menu">', 
    'output_br' => $delimeter,
    'display_mode' => variable_get('phpbbforum_recent_posts_display_mode', 0),
    'display_topic_new_icon_pos' => variable_get('phpbbforum_recent_display_topic_new_icon_pos', 0),
    'output_method' => 'array',
  );
  $output = phpbb_api_recent_posts($options);
  $output = theme('phpbbforum_recent_posts', $output, $options);
  if ($_phpbb_integration_mode != '0')
    $output = _phpbbforum_replace_block_links($output);

  return $output;
}

function phpbbforum_recent_topics() {
  global $phpbb_config, $phpbb_user, $_phpbb_integration_mode;

  if (variable_get('phpbbforum_recent_topics_br', 1))
    $delimeter = PHPBB_RECENT_DELIMITER_BR;
  else
    $delimeter = PHPBB_RECENT_DELIMITER_SPACE;
  
  $options = array(
    'num_recent' => (int)variable_get('phpbbforum_num_recent_topics', PHPBB_NUM_RECENT_TOPICS),
    'search_id' => 'active_topics',
    'sort_days' => (int)variable_get('phpbbforum_num_recent_sort_days', 0),
    'output_tag' => '<ul class="menu">', 
    'output_br' => $delimeter,
    'display_mode' => variable_get('phpbbforum_recent_topics_display_mode', 0),
    'display_topic_new_icon_pos' => variable_get('phpbbforum_recent_display_topic_new_icon_pos', 0),
    'output_method' => 'array',
  );  
  $output = phpbb_api_recent_topics($options);
  $output = theme('phpbbforum_recent_topics', $output, $options);
  if ($_phpbb_integration_mode != '0')
    $output = _phpbbforum_replace_block_links($output);

  return $output;
}

function phpbbforum_board_stats() {
  global $phpbb_config, $phpbb_user, $_phpbb_integration_mode;

  $options = array(
    'output_method' => 'array',
  );
  $output = phpbb_api_board_stats($options);
  $output = theme('phpbbforum_board_stats', $output, $options);
  if ($_phpbb_integration_mode != '0')
    $output = _phpbbforum_replace_block_links($output);

  return $output;
}

function phpbbforum_pm() {
  global $phpbb_config, $phpbb_user, $_phpbb_integration_mode;

  if (variable_get('phpbbforum_pm_br', 1))
    $delimeter = PHPBB_RECENT_DELIMITER_BR;
  else
    $delimeter = PHPBB_RECENT_DELIMITER_SPACE;  
  $options = array(
    'output_br' => $delimeter,
    'display_mode' => variable_get('phpbbforum_pm_display_mode', 0),
    'output_method' => 'array',
  );
  $output = phpbb_api_pm($options);
  $output = theme('phpbbforum_pm', $output, $options);
  if ($_phpbb_integration_mode != '0')
    $output = _phpbbforum_replace_block_links($output);

  return $output;
}

function phpbbforum_topposter() {
  global $phpbb_config, $phpbb_user, $_phpbb_integration_mode;

  $options = array(
    'num_top' => (int)variable_get('phpbbforum_num_top_posters', PHPBB_NUM_TOP_POSTERS), 
    'output_method' => 'array',
  );
  $output = phpbb_api_topposter($options);
  $output = theme('phpbbforum_topposter', $output, $options);
  if ($_phpbb_integration_mode != '0')
    $output = _phpbbforum_replace_block_links($output);

  return $output;
}

 
function theme_phpbbforum_whos_online($output, $options) {
  return theme_phpbb_api_whos_online($output, $options);
}

function theme_phpbbforum_recent_posts($posts, $options) {
  return theme_phpbb_api_recent_posts($posts, $options);
}

function theme_phpbbforum_recent_topics($posts, $options) {
  return theme_phpbb_api_recent_topics($posts, $options);
}

function theme_phpbbforum_board_stats($output, $options) {
  return theme_phpbb_api_board_stats($output, $options);
}

function theme_phpbbforum_pm($output, $options) {
  return theme_phpbb_api_pm($output, $options);
}

function theme_phpbbforum_topposter($posters, $options) {
  return theme_phpbb_api_topposter($posters, $options);
}

function theme_phpbbforum_style($style, $phpbb_theme_path, $output) {
  if (variable_get("phpbbforum_page_frame", 0) == '0')
    return '';
  return phpbbforum_style($style, $phpbb_theme_path, $output);
}

if (variable_get("phpbbforum_page_frame", 0) == '1' || variable_get("phpbbforum_page_frame", 0) == '2') {

////////////////////////////////////////////////////////////////////////////////

function phpbbforum_set_style($style, $phpbb_theme_path) {

  global $phpbb_config, $phpbb_user, $phpbb_root_path;

  $style = str_replace('-', '_', $style);

  $fn = 'phpbbforum_set_style_'. $style;
  if (function_exists($fn)) {
    $fn($phpbb_theme_path);
  }
  else {
    $styleclass = (stripos($style, 'prosilver') || substr($style, 0, 3) == 'pro');
    if ($styleclass !== false) {
      phpbbforum_set_style_prosilver($phpbb_theme_path);
    }
    else {
      phpbbforum_set_style_subsilver2($phpbb_theme_path);
    }
  }
}

function phpbbforum_style($style, $phpbb_theme_path, $output) {

  global $phpbb_config, $phpbb_user, $phpbb_root_path, $_phpbb_result;
  global $language, $theme;
  
  drupal_set_title($_phpbb_result['page_title']);

  if ($_phpbb_result['meta_refresh'] > 0 ) {
    $url = $_phpbb_result['destination'];
    $time = $_phpbb_result['meta_refresh'];
    drupal_set_html_head('<meta http-equiv="refresh" content="' . $time . ';url=' . $url . '" />');
  }

  $type = 'theme';
  drupal_add_css(drupal_get_path('module', 'phpbbforum') .'/css/phpbbforum.css', $type);

  $style = str_replace('-', '_', $style);
  
  $fn = 'phpbbforum_style_'. $style;
  if (function_exists($fn)) {
    $output = $fn($phpbb_theme_path, $output);
  }
  else {
    $styleclass = (stripos($style, 'prosilver') || substr($style, 0, 3) == 'pro');
    if ($styleclass !== false) {
      $output = phpbbforum_style_prosilver($phpbb_theme_path, $output);
    }
    else {
      $output = phpbbforum_style_subsilver2($phpbb_theme_path, $output);
    }
  }

  drupal_add_js(drupal_get_path('module', 'phpbbforum') .'/js/phpbbforum.js', 'module');

  $phpbb_url = $phpbb_config['forum_url'] .'/';

  $output = str_replace($phpbb_root_path, $phpbb_url, $output);
  $output = _phpbbforum_replace_links($output);

  $output = '<div id="phpbbforum-page">'. $output .'</div><!-- /phpbbforum-page -->';

  return $output;
}



/*
function _phpbbforum_set_head_script1($endpos, $output) {

  $tag = '<!-- phpbbforum-page-head -->';
  $scripttag = '<script type="text/javascript">';

  $startpos = strpos($output, $tag, $endpos);
  if ($startpos !== false) {
    $startpos = strpos($output, $scripttag, $startpos + strlen($tag));
  }
  else {
    $startpos = strpos($output, $scripttag);
  }
  if ($startpos !== false) {
    $startpos = strpos($output, '// <![CDATA[', $startpos + strlen($scripttag));
    if ($startpos !== false) {
      $startpos += 12;
      $endpos = strpos($output, '// ]]>', $startpos);
      if ($endpos) {
        $script = substr($output, $startpos, ($endpos - $startpos));
        if ($script != '') {
          drupal_add_js($script, 'inline');
        }
      }
    }
  }

  return $endpos;
}
*/

function _phpbbforum_get_html_body($endpos, $output) {
  $startpos = strpos($output, '<body', $endpos);
  if ($startpos) {
    $startpos = strpos($output, '>', $startpos) + 1;
    $endpos = strpos($output, '</body>', $startpos);
    if ($endpos)
      $output = substr($output, $startpos, ($endpos - $startpos));
  }
  return $output;
}

function _phpbbforum_get_html_tag(&$endpos, $output, $tag, $endtag = '</div>', $mode = 0) {
  $startpos = strpos($output, $tag, $endpos);
  if ($startpos !== false) {
    $endpos2 = strpos($output, $endtag, $startpos);
    if ($endpos2 !== false) {
      $len = $endpos2 - $startpos;
      if ($mode == 0) {
        $len += strlen($endtag);
        $endpos2 += strlen($endtag);
      } elseif ($mode == 1) {
        $startpos += strlen($tag);
      }
      $output = substr($output, $startpos, $len);
      $endpos = $endpos2;
    }
  }
  return $output;
}

function _phpbbforum_set_theme_css($phpbb_theme_path, $phpbb_theme) {

  global $language, $theme;

  $type = 'theme';

  $module_css = '/'. $theme .'-phpbbforum-'. $phpbb_theme .'.css';
  $theme_css_file = drupal_get_path('theme', $theme) . $module_css;
  if (!file_exists($theme_css_file)) {
    $theme_css_file = drupal_get_path('module', 'phpbbforum') .'/css'.  $module_css;
    if (!file_exists($theme_css_file)) {
      $module_css = '/phpbbforum-'. $phpbb_theme .'.css';
      $theme_css_file = drupal_get_path('theme', $theme) .'/css'. $module_css;
      if (!file_exists($theme_css_file)) {
        $theme_css_file = drupal_get_path('module', 'phpbbforum') .'/css'. $module_css;
      }
    }
  }
  drupal_add_css($theme_css_file, $type);
}

////////////////////////////////////////////////////////////////////////////////

/*
 * prosilver
 */
 
function _phpbbforum_set_head_script_prosilver($endpos, $output) {

  global $_drupal_base_path;

  $scripttag = '<script type="text/javascript">';

  $startpos = strpos($output, $scripttag, $endpos);
  if ($startpos !== false) {
    $endpos = strpos($output, '<link', $startpos + strlen($scripttag));
    if ($endpos) {
      $script = substr($output, $startpos, ($endpos - $startpos));
      if ($script != '') {
        $path = str_replace('\\', '/', $_drupal_base_path);
        $str = array($path, strtolower($path));
        $script = str_replace($str, '', $script);
        $script = _phpbbforum_replace_forum_url($script);
        drupal_set_html_head($script);
      }
    }
  }

  return $endpos;
}
 
function _phpbbforum_set_head_links_prosilver($endpos, $output) {

  global $_drupal_base_path;

  $linkstag = '<link href="';

  $startpos = strpos($output, $linkstag, $endpos);
  if ($startpos !== false) {
    $endpos = strpos($output, 'printonly', $startpos + 1);
    if ($endpos !== false) {
      $startpos = strpos($output, $linkstag, $endpos);
      if ($startpos !== false) {
        $endpos = strpos($output, '</head>', $startpos + 1);
        if ($endpos) {
          $links = substr($output, $startpos, ($endpos - $startpos));
          if ($links != '') {
            $path = str_replace('\\', '/', $_drupal_base_path);
            $str = array($path, strtolower($path));
            $links = str_replace($str, '', $links);
            drupal_set_html_head($links);
          }
        }
      }
    }
  }

  return $endpos;
}
 
function phpbbforum_set_style_prosilver($phpbb_theme_path) {
  _phpbbforum_set_theme_css($phpbb_theme_path, 'prosilver');
}

function phpbbforum_style_prosilver($phpbb_theme_path, $output) {

  phpbbforum_set_style_prosilver($phpbb_theme_path);

  $endpos = _phpbbforum_set_head_script_prosilver(0, $output);
  $endpos = _phpbbforum_set_head_links_prosilver($endpos, $output);

  $output = _phpbbforum_get_html_body($endpos, $output);

  return $output;
}

/*
 * prosilver_embed
 */

function phpbbforum_set_style_prosilver_embed($phpbb_theme_path) {
  phpbbforum_set_style_prosilver($phpbb_theme_path);
}

function phpbbforum_style_prosilver_embed($phpbb_theme_path, $output) {

  phpbbforum_set_style_prosilver_embed($phpbb_theme_path);

  $endpos = _phpbbforum_set_head_script_prosilver(0, $output);
  $endpos = _phpbbforum_set_head_links_prosilver($endpos, $output);

  $output = _phpbbforum_get_html_body($endpos, $output);

  $endpos = 0;
  $search_box = _phpbbforum_get_html_tag($endpos, $output, '<div id="search-box">');
  $search_box = _phpbbforum_replace_forum_url($search_box);

  //global $search;
  //$search['search_theme_form'] = $search_box;

  /*
  $startpos = strpos($output, '<div id="page-header"');
  if ($startpos !== false) {
    $endpos = strpos($output, '<a name="start_here"></a>', $startpos + 1) + 6;
    if ($endpos !== false) {
      $outputlogo = substr($output, $startpos, ($endpos - 7 - $startpos));
      $output = str_replace($outputlogo, '', $output);
    }
  }
  */
  
  return $output;
}

////////////////////////////////////////////////////////////////////////////////

/*
 * subsilver2
 */

function _phpbbforum_set_head_links_subsilver2($endpos, $output) {

  global $_drupal_base_path;

  $linkstag = '<link';

  $startpos = strpos($output, $linkstag, $endpos);
  if ($startpos !== false) {
    $endpos = strpos($output, '/>', $startpos + 1);
    if ($endpos) {
      $links = substr($output, $startpos, ($endpos - $startpos) + 2);
      if ($links != '') {
        $path = str_replace('\\', '/', $_drupal_base_path);
        $str = array($path, strtolower($path));
        $links = str_replace($str, '', $links);
        drupal_set_html_head($links);
      }
    }
  }

  return $endpos;
}

function _phpbbforum_set_head_script_subsilver2($endpos, $output) {

  global $_drupal_base_path;

  $scripttag = '<script type="text/javascript">';

  $startpos = strpos($output, $scripttag, $endpos);
  if ($startpos !== false) {
    $endpos = strpos($output, '</head>', $startpos + strlen($scripttag));
    if ($endpos) {
      $script = substr($output, $startpos, ($endpos - $startpos));
      if ($script != '') {
        $script = _phpbbforum_replace_forum_url($script);
        drupal_set_html_head($script);
      }
    }
  }

  return $endpos;
}
 
function phpbbforum_set_style_subsilver2($phpbb_theme_path) {

  _phpbbforum_set_theme_css($phpbb_theme_path, 'subsilver2');

  $theme_js = $phpbb_theme_path .'/template/editor.js';
  drupal_add_js($theme_js, 'module');
}

function phpbbforum_style_subsilver2($phpbb_theme_path, $output) {

  phpbbforum_set_style_subsilver2($phpbb_theme_path);

  $endpos = _phpbbforum_set_head_links_subsilver2(0, $output);
  $endpos = _phpbbforum_set_head_script_subsilver2($endpos, $output);

  $output = _phpbbforum_get_html_body($endpos, $output);

  return $output;
}

/*
 * subsilver2_embed
 */
function phpbbforum_set_style_subsilver2_embed($phpbb_theme_path) {
  phpbbforum_set_style_subsilver2($phpbb_theme_path);
}

function phpbbforum_style_subsilver2_embed($phpbb_theme_path, $output) {

  phpbbforum_set_style_subsilver2_embed($phpbb_theme_path);

  $endpos = _phpbbforum_set_head_links_subsilver2(0, $output);
  $endpos = _phpbbforum_set_head_script_subsilver2($endpos, $output);

  $output = _phpbbforum_get_html_body($endpos, $output);
  
  /*
  $startpos = strpos($output, '<div id="logodesc">');
  if ($startpos !== false) {
    $endpos = strpos($output, '</div>', $startpos + 1) + 6;
    if ($endpos !== false) {
      $outputlogo = substr($output, $startpos, ($endpos - $startpos));
      $output = str_replace($outputlogo, '', $output);
    }
  }
  */
  
  return $output;
}

} // if (variable_get("phpbbforum_page_frame", 0) == '1' || variable_get("phpbbforum_page_frame", 0) == '2') {