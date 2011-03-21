<?php
// $Id: quotes.user.inc,v 1.1.2.4 2009/10/01 04:26:30 nancyw Exp $
/**
 * @file
 * Displays a Drupal page containing quotes submitted by a given user.
 */
function quotes_user_page($account) {
  $save_hp = $account->homepage;
  unset($account->homepage);
  $trail = array(l(t('Home'), ''), theme('username', $account, array('levels' => FALSE)));
  $account->homepage = $save_hp;
  drupal_set_breadcrumb($trail);

  $output = drupal_get_form('quotes_user_form', $account);
  return $output;
}

function _quotes_user_access_check($user, $account) {
  if (user_access('edit own quotes', $user) && ($account->uid == $user->uid)) {
    return TRUE;
  }
  return user_access('administer quotes', $user);
}

function quotes_user_form($form_state, $account) {
  global $user;
  $destination = drupal_get_destination();
  $options = array();
  $now = $_SERVER['REQUEST_TIME'];
  $limit = variable_get('quotes_per_page', 10);
  $node_ops = module_invoke_all('node_operations');

  $query = db_rewrite_sql("SELECT n.* FROM {node} n WHERE n.uid=%d AND n.type='quotes' ORDER BY n.changed DESC");
  $result = pager_query($query, $limit, 0, NULL, $account->uid);
  while ($node = db_fetch_object($result)) {
    $status = array();
    $status[] = $node->status ? t('published') : t('not published');
    if ($node->promoted) {
      $status[] = t('promoted');
    }
    if ($node->sticky > 0) {  // >0 allows for sticky-encoded weighting.
      $status[] = t('sticky');
    }
    if ($node->moderated) {
      $status[] = t('moderated');
    }
    $form['title'][$node->nid] = array('#value' => l($node->title, 'node/'. $node->nid) .' '. theme('mark', node_mark($node->nid, $node->changed)));
    $form['status'][$node->nid] = array('#value' => implode(', ', $status));
    $form['updated'][$node->nid] = array('#value' => format_interval($now - $node->changed));

    $tid_list = array();
    $terms = db_query("SELECT tn.tid, td.name FROM {term_node} tn LEFT JOIN {term_data} td USING (tid) WHERE tn.nid=%d AND tn.vid=%d", $node->nid, $node->vid);
    while ($row = db_fetch_array($terms)) {
      $tid_list[] = check_plain($row['name']);
    }
    $form['group'][$node->nid] =  array('#value' => implode(', ', $tid_list));

    if (_quotes_user_access_check($user, $account)) {
      $form['operationse'][$node->nid] = array('#value' => l(t('edit'), 'node/'. $node->nid .'/edit', array('query' => $destination)));
      $form['operationsd'][$node->nid] = array('#value' => l(t('delete'), 'node/'. $node->nid .'/delete', array('query' => $destination)));
    }
  }
  $form['pager'] = array('#value' => theme('pager', NULL, $limit, 0));

  return $form;
}

/**
 * Theme node administration overview.
 */
function theme_quotes_user_form($form) {
  // Overview table:
  $header = array(t('Title'), t('Status'), t('Tags'), t('Last updated'), array('data' => t('Operations'), 'colspan' => '2'));
  $rows =array();

  if (isset($form['title']) && is_array($form['title'])) {
    foreach (element_children($form['title']) as $key) {
      $rows[] = array(
        drupal_render($form['title'][$key]),
        drupal_render($form['status'][$key]),
        drupal_render($form['group'][$key]),
        drupal_render($form['updated'][$key]),
        drupal_render($form['operationse'][$key]),
        drupal_render($form['operationsd'][$key]),
        );
    }
  }
  else  {
    $rows[] = array(array('data' => t('No posts available.'), 'colspan' => '6'));
  }
  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }
  $output .= drupal_render($form);
  return $output;
}

