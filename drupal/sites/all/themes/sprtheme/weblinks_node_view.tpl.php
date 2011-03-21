<?php
// $Id: weblinks_node_view.tpl.php,v 1.5.2.7.2.4 2009/04/30 17:11:42 nancyw Exp $

/**
 * @file
 * weblinks_node_view.tpl.php
 * Theme implementation to display a list of related content.
 *
 * Available variables:
 * - $node: node object that contains the URL.
 * - $options: Options for the URL.
 */
if ($node->url != NULL) {
  switch (variable_get('weblinks_view_as', 'url')) {
    case 'url':
      if (variable_get('weblinks_strip', FALSE)) {
        $parts = parse_url($node->url);
        $url = str_replace('www.', '', $parts['host']) . $parts['path'];
      }
      else {
        $url = $node->url;
      }
      $title = _weblinks_trim($url, variable_get('weblinks_trim', 0));
      break;

    case 'visit':
      $title_text = variable_get('weblinks_visit_text', t('Visit [title]'));
      if (module_exists('token')) {
        $title = token_replace($title_text, 'node', $node);
      }
      else {
        $title = str_replace('[title]', $node->title, $title_text);
      }
      break;
  }
  if (variable_get('weblinks_redirect', FALSE)) {
    $link = l($title, 'weblinks/goto/'. $node->nid, $options);
  }
  else {
    $link = l($title, $node->url, $options);
  }
  echo '<div class="weblinks-linkview">'. $link .'</div><!--class="weblinks-linkview"-->';
}
