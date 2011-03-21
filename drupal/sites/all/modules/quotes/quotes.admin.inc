<?php
// $Id: quotes.admin.inc,v 1.1.2.15 2009/10/01 04:26:30 nancyw Exp $
/**
 * @file
 * The quotes module allows users to maintain a list of quotes that
 * can be displayed in any number of administrator-defined quote
 * blocks.
 *
 * @copyright Copyright (c) 2003-2007 Jim Riggs.  All rights reserved.
 * @author Jim Riggs <drupal at jim and lissa dot com>
 */

/**
 * Displays the admin settings page.
 */
function quotes_admin_settings_page() {
  return drupal_get_form('quotes_admin_settings');
}

/**
 * Displays the general admin settings form.
 *
 * @return
 *   An array containing the form elements to be displayed.
 */
function quotes_admin_settings() {
  $form = array();
  $yesno = array(1 => t('Yes'), 0 => t('No'));
  $count = db_result(db_query("SELECT COUNT(*) FROM {node} n where n.type='quotes' AND n.status=1"));
  $form['count'] = array(
    '#type' => 'item',
    '#value' => t('There are currently !count published quotations.', array('!count' => $count)),
    );

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('These options are for general theming.'),
    );

  $form['display']['quotes_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Quotes per page'),
    '#size' => 6,
    '#default_value' => variable_get('quotes_per_page', 10),
    '#description' => t('The number of quotes included on a page.'),
    '#prefix' => '<div class="clear-block"></div>',
    );

  $form['display']['quotes_leader'] = array(
    '#type' => 'textfield',
    '#title' => t('Author leader'),
    '#default_value' => variable_get('quotes_leader', '&mdash;'),
    '#description' => t('The text placed before the author attribution (e.g. "&amp;mdash;" for an em-dash or "&amp;#8226;" for a bullet).')
    );

  $form['display']['quotes_author_link'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="quotes-radios">',
    '#suffix' => '</div>',
    '#title' => t('Make author a link'),
    '#default_value' => variable_get('quotes_author_link', FALSE),
    '#description' => t('If selected, the author text will be a link to show all quotes by that author.'),
    );

  $form['display']['quotes_author_bio'] = array(
    '#type' => 'radios',
    '#options' => array(0 => t("Don't display"), 1 => t('Include text'), 2 => t('Include as a link')),
    '#title' => t("Include author's bio"),
    '#default_value' => variable_get('quotes_author_bio', FALSE),
    '#description' => t("If selected, the author's biography will be shown on the Quotes page."),
    '#prefix' => '<div class="quotes-radios">',
    '#suffix' => '</div>',
    );

  $format_list = filter_formats();
  $formats = array(FILTER_FORMAT_DEFAULT => '-node format-');
  foreach ($format_list as $number => $filter) {
    $formats[$number] = $filter->name;
  }
  $form['display']['quotes_format'] = array(
    '#type' => 'radios',
    '#options' => $formats,
    '#title' => t('Author, citation, and bio input format'),
    '#default_value' => variable_get('quotes_format', 0),
    '#description' => t('This will be used as the input format for the author, citation, and bio fields.'),
    '#prefix' => '<div class="quotes-radios">',
    '#suffix' => '</div>',
    );

  $form['display']['quotes_edit_link'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="quotes-radios">',
    '#suffix' => '</div>',
    '#title' => t('Add an "edit" link'),
    '#default_value' => variable_get('quotes_edit_link', TRUE),
    '#description' => t('If selected, an "edit" link will be shown for each quote.'),
    );

  $form['myquotes'] = array(
    '#type' => 'fieldset',
    '#title' => t('My Quotes links'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $form['myquotes']['quotes_showlink'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="quotes-radios">',
    '#suffix' => '</div>',
    '#title' => t('Show link to users\' quotes'),
    '#default_value' => variable_get('quotes_showlink', TRUE),
    '#description' => t('Uncheck this to disable the link to each users\' "my quotes" page when viewing a node.')
    );

  $form['myquotes']['quotes_show_myquotes_original'] = array(
    '#type' => 'value',
    '#value' => variable_get('quotes_show_myquotes', TRUE),
    );

  $form['myquotes']['quotes_show_myquotes'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="quotes-radios">',
    '#suffix' => '</div>',
    '#title' => t('Show the "my quotes" menu item'),
    '#default_value' => variable_get('quotes_show_myquotes', TRUE),
    '#description' => t('Uncheck this to disable the "My quotes" menu item for all users. Note, changing this setting will <a href="!url">require the menu to be rebuilt</a>.', array('!url' => url('admin/settings/performance')))
    );

  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('These options are for users.'),
    );

  $form['user']['quotes_user_recent'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="quotes-radios">',
    '#suffix' => '</div>',
    '#title' => t('Add a "Quotes" tab on the "My account" page'),
    '#default_value' => variable_get('quotes_user_recent', FALSE),
    );

  $form['#submit'][] = '_quotes_settings_form_submit';
  return system_settings_form($form);
}

/**
 *  Check the menu settings for changes..
 */
function _quotes_settings_form_submit($form, &$form_state) {
  if ($form_state['values']['quotes_show_myquotes'] != $form_state['values']['quotes_show_myquotes_original']) {
    // Force the menu to be rebuilt.
    menu_rebuild();
  }
}

/**
 *  Export function page.
 *
 * @return
 *   A form with a tab-delimited list of quotes.
 */
function quotes_export() {
  $form['intro'] = array(
    '#type' => 'item',
    '#title' => t('Copy and paste this list to the receiving site'),
    );

  $output = NULL;
  $count = 0;
  $sql = db_rewrite_sql("SELECT nr.body, nr.title, qa.name AS author, q.citation FROM {node} n INNER JOIN {node_revisions} nr USING (vid) INNER JOIN {quotes} q USING (vid) INNER JOIN {quotes_authors} qa USING (aid) WHERE n.status=1 AND n.type='quotes' ORDER BY qa.name");
  $result = db_query($sql);
  while ($row = db_fetch_object($result)) {
    ++$count;
    $output .= _quotes_escape_newlines($row->body) ."\t".
      _quotes_escape_newlines($row->author) ."\t".
      _quotes_escape_newlines($row->citation) ."\n";
  }
  drupal_set_message(t('Found !count quotes.', array('!count' => $count)));
  if ($count == 0) {
    $count = 1;
    $output = t('No quotes were found.');
  }

  $form['list'] = array(
    '#type' => 'textarea',
    '#value' => $output,
    '#rows' => min(30, $count),
    );

  return $form;
}

/**
 *  Helper function to strip Windows newline and format Unix newlines.
 *
 *  @param  $text
 *    The text to be scanned.
 *  @return 
 *    Text with newlines processed.
 */
function _quotes_escape_newlines($text) {
  // Get rid of Windows crap.
  $text = str_replace("\r", '', $text);
  return str_replace("\n", "\\\n", $text);
}

/**
 * Displays a list of currently-defined quote blocks.
 *
 * @return
 *   An array containing the form elements to be displayed.
 */
function quotes_blocks_settings() {
  $form = array();
  $form['name'] = array(
    '#type' => 'textfield',
    '#size' => 32,
    '#maxlength' => 64
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add block')
  );

  return $form;
}

/**
 * Validates that the new block name is valid.
 */
function quotes_blocks_settings_validate($form, &$form_state) {
  $name = trim($form_state['values']['name']);

  if (!$name) {
    form_set_error('name', t('You must specify a valid block name.'));
  }
  else {
    if (db_result(db_query("SELECT COUNT(*) FROM {quotes_blocks} qb WHERE qb.name = '%s'", $name))) {
      form_set_error('name', t('The block name %name already exists. Please choose another block name.', array('%name' => $name)));
    }
  }
}

/**
 * Creates the new quote block.
 *
 * @param $form
 *   The string specifying the form ID of the form that was submitted.
 * @param $form
 *   The array specifying the form values.
 */
function quotes_blocks_settings_submit($form, &$form_state) {
  db_query("INSERT INTO {quotes_blocks} (name, block_type, nid_filter, aid_filter, rid_filter, uid_filter, tid_filter, cron_interval, cron_step, cron_last, vid) VALUES ('%s', 0, '', '', '', '', '', 0, 0, 0, 0)", trim($form_state['values']['name']));
}

/**
 * Renders the quote block list, including the "Add block" row.
 *
 * @param $form
 *   The array specifying the form to be rendered.
 *
 * @result
 *   The formatted HTML table of blocks.
 */
function theme_quotes_blocks_settings($form) {
  $header = array(t('Name'), t('Filters'), array('data' => t('Operations'), 'colspan' => 2));
  $result = db_query('SELECT qb.* FROM {quotes_blocks} qb ORDER BY qb.name');
  $rows = array();

  while ($block = db_fetch_object($result)) {
    $filters = array();

    if ($block->nid_filter) {
      $filters[] = t('node');
    }

    if ($block->rid_filter) {
      $filters[] = t('role');
    }

    if ($block->uid_filter) {
      $filters[] = t('user');
    }

    if ($block->tid_filter) {
      $filters[] = t('term');
    }

    $rows[] = array(
      $block->name,
      implode(', ', (count($filters) ? $filters : array(t('none')))),
      l(t('configure block'), "admin/build/block/configure/quotes/$block->bid"),
      l(t('delete block'), "admin/settings/quotes/delete/$block->bid")
    );
  }

  $rows[] = array(
    drupal_render($form['name']),
    array('data' => drupal_render($form['submit']), 'colspan' => 3)
  );

  $output = drupal_render($form);

  if (count($rows)) {
    $output .= theme('table', $header, $rows);
  }
  else {
    $output .= theme('table', $header, array(array(array('data' => t('No blocks are defined.'), 'colspan' => 4))));
  }

  return $output;
}

/**
 * Confirms the deletion a quote block.
 *
 * @param $bid
 *   The block ID of the block being deleted.
 *
 * @return
 *   A string containing the confirmation form displayed to the user.
 */
function _quotes_block_delete($form_stuff, $bid) {
  $block = db_fetch_object(db_query('SELECT qb.name FROM {quotes_blocks} qb WHERE qb.bid = %d', $bid));
  
  $form = array();
  $form['bid'] = array(
    '#type' => 'value',
    '#value' => $bid
  );
  $form['block_name'] = array(
    '#type' => 'value',
    '#value' => $block->name
  );

  return confirm_form($form, t('Are you sure you want to delete the block %name?', array('%name' => $block->name)), 'admin/settings/quotes/blocks', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Deletes the specified block.
 *
 * @param $form
 *   The string specifying the form ID of the form that was submitted.
 * @param $form_state
 *   The array specifying the form values.
 *
 * @result
 *   A string specifying the page to which the user should be
 *   redirected (admin/settings/quotes/blocks).
 */
Function _quotes_block_delete_submit($form, &$form_state) {
  db_query("DELETE FROM {quotes_blocks} WHERE bid = %d", $form_state['values']['bid']);
  drupal_set_message(t('The block %name has been removed.', array('%name' => $form_state['values']['block_name'])));
  cache_clear_all();

  $form_state['redirect'] = 'admin/settings/quotes/blocks';
  return;
}

/**
 *  Bios maintenance page.
 *
 * @return
 *   A form with a tab-delimited list of quotes.
 */
function quotes_bios(&$form_state, $aid = NULL) {
  $form = array();
  $first_pass = is_null($aid);

  if ($first_pass) {
    $auths = quotes_get_authors();  
    $count = count($auths);
    $data = array('name' => NULL, 'bio' => NULL);
  }
  else {
    // Get the data.
    $data = db_fetch_array(db_query("SELECT * FROM {quotes_authors} WHERE aid=%d", $aid));
  }

  $form['aid'] = array(
    '#type' => 'hidden',
    '#value' => $aid,
    );

  $form['author'] = array(
    '#type' => 'select',
    '#options' => $auths,
    '#size' => min(20, $count),
    '#description' => t('Pick the author whose biography you wish to update.'),
    );

  $form['name'] = array(
    '#type' => 'markup',
    '#value' => '<h3>'. t('Biography for %name', array('%name' => $data['name'])) .'</h3>',
    );

  $form['bio'] = array(
    '#type' => 'textarea',
    '#description' => t("This is the author's biography."),
    '#default_value' => $data['bio'],
    );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $first_pass ? t('Get biography') : t('Update'),
    );

  // Show the fields at the right time.
  if ($first_pass) {
    $form['name']['#type'] = 'hidden';
    $form['bio']['#type'] = 'hidden';
  }
  else {
    $form['author']['#type'] = 'hidden';
  }

  return $form;
}

function quotes_bios_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'Update') {
    $vals = array($form_state['values']['bio'], $form_state['values']['aid']);
    $upd = db_query("UPDATE {quotes_authors} SET bio='%s' WHERE aid=%d", $vals);
    $form_state['redirect'] = BIOS_PATH;
  }
  else {
    $form_state['redirect'] = BIOS_PATH .'/'. $form_state['values']['author'];
  }
}
