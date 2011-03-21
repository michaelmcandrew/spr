<?php
// $Id: weblinks.admin.inc,v 1.34.2.43.2.30 2009/09/20 22:33:41 nancyw Exp $

/**
 * @file
 * Settings form.
 */

/**
 * Module settings page.
 */
function weblinks_settings_page() {
  return drupal_get_form('weblinks_admin_settings');
}
 
function weblinks_admin_settings() {
  drupal_add_js(drupal_get_path('module', 'weblinks') .'/weblinks.admin.js');
  drupal_add_css(drupal_get_path('module', 'weblinks') .'/weblinks.admin.css');
  $form = array('#attributes' => array('class' => 'weblinks-settings'));
  $taxo_avail = module_exists('taxonomy');

/**
 * Overall settings.
 */
  $yesno = array(1 => t('Yes'), 0 => t('No'));

  $vocabs = $taxo_avail ? taxonomy_get_vocabularies('weblinks') : array();
  if (count($vocabs) > 1) {
    $avail_vocs = array();
    foreach ($vocabs as $vid => $vocabulary) {
      $avail_vocs[$vid] = check_plain($vocabulary->name);
    }
    ksort($avail_vocs);
    $form['weblinks_nav_vocabulary'] = array(
      '#type' => 'radios',
      '#title' => t('Vocabulary to use for Weblinks'),
      '#default_value' => variable_get('weblinks_nav_vocabulary', ''),
      '#options' => $avail_vocs,
      '#required' => TRUE,
      '#prefix' => '<div class="weblinks-radios">',
      '#suffix' => '</div><div class="clear-block"></div>',
      );
  }
  else {
    if (count($vocabs) < 1 && $taxo_avail) {
      variable_del('weblinks_nav_vocabulary');
      _weblinks_get_vocid();
      drupal_set_message(t('No vocabulary for the weblinks content type was found. A new vocabulary was created.'));
    }
  }

  $len_opts = drupal_map_assoc(array(0, 10, 15, 20, 30, 40, 50, 60, 70, 80, 128, 256));

  $form['weblinks_trim_block'] = array(
    '#type' => 'select',
    '#options' => $len_opts,
    '#title' => t('Maximum block title length'),
    '#default_value' => variable_get('weblinks_trim_block', 0),
    '#description' => t('This controls how much of a node title will be shown in a block to conserve space. Zero (0) means unlimited.'),
    );

  $form['page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links page settings'),
    '#description' => t('These settings control the main links page display.'),
    '#attributes' => array('id' => 'weblinks-page-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

/**
 * Settings for the links page.
 */
  $form['page']['weblinks_collapsible'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Make groups collapsible'),
    '#default_value' => (int) variable_get('weblinks_collapsible', TRUE),
    '#description' => t('Enable this if you want the groups to be collapsible on the main links page. Note that if the "Unclassified" title is empty, that group will not be collapsed.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['page']['weblinks_collapsed'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Collapse all groups'),
    '#default_value' => (int) variable_get('weblinks_collapsed', FALSE),
    '#description' => t('Enable this if you want all the groups to be collapsed on the main links page.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks_collapse_hide weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['page']['weblinks_linktitle'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Link group title to group'),
    '#default_value' => (int) variable_get('weblinks_linktitle', TRUE),
    '#description' => t('Enable this if you want the group titles to be a link to show only that group.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks-link-title weblinks-radios">',
    '#suffix' => '</div>',
    );

  $sort_options = array(
    'standard' => t('Normal Drupal - by weight, sticky, created date.'),
    'title' => t('Legacy - by weight, sticky, title.'),
    );
  $form['page']['weblinks_page_sort'] = array(
    '#type' => 'radios',
    '#options' => $sort_options,
    '#title' => t('Page Sort order'),
    '#default_value' => variable_get('weblinks_page_sort', 'title'),
    '#description' => t('This determines how the links page will sort the links.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div><div class="clear-block"></div>',
    );

  $form['page']['weblinks_pagedesc'] = array(
    '#type' => 'textarea',
    '#title' => t('Page information'),
    '#rows' => 3,
    '#default_value' => variable_get('weblinks_pagedesc', NULL),
    '#description' => t('This description will appear at the top of the links page.  It will be filtered by your default input format.'),
    '#required' => FALSE,
    );

  // Disallow blank setting.
  $mfd_default = variable_get('weblinks_maxfrontdepth', 1);
  if (!is_numeric($mfd_default)) {
    $mfd_default = 1;
  }
  $form['page']['weblinks_maxfrontdepth'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum group depth (main links page)'),
    '#default_value' => $mfd_default,
    '#description' => t('This controls how many subgroups, if any, will display on the main links page. Blank, zero (0), and one (1) will all show only the top level.'),
    '#required' => FALSE,
    '#size' => 2,
    );

  // Add "rel=nofollow."
  $form['page']['weblinks_nofollow'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Spam link deterrent'),
    '#default_value' => (int) variable_get('weblinks_nofollow', FALSE),
    '#description' => t('If enabled, Drupal will add rel="nofollow" to all links, as a measure to reduce the effectiveness of spam links. Note: this will also prevent valid links from being followed by search engines, therefore it is likely most effective when enabled for anonymous users.'),
    );

/**
 * Settings for group display.
 */
  $form['group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Group settings'),
    '#description' => t('These settings control the way the groups display on the main links page.'),
    '#attributes' => array('id' => 'weblinks-group-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $form['group']['weblinks_catdesc'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Display group descriptions'),
    '#default_value' => (int) variable_get('weblinks_catdesc', TRUE),
    '#description' => t('Generally recommended.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['group']['weblinks_empty_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Empty container text'),
    '#rows' => 2,
    '#default_value' => variable_get('weblinks_empty_text', NULL),
    '#description' => t('This text will appear when a container is empty. Leave it blank to not show the group at all. It will be filtered by your default input format.'),
    );

  $form['group']['weblinks_unclassified_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for unclassified links'),
    '#default_value' => variable_get('weblinks_unclassified_title', t('Unclassified')),
    '#description' => t('This will be used as the group title for unclassified links. You may leave it blank for no title.'),
    );

  $form['group']['weblinks_unclassified_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Description for unclassified links'),
    '#rows' => 1,
    '#default_value' => variable_get('weblinks_unclassified_desc', t('These links have not been assigned a group.')),
    '#description' => t('This will be used as the description for the unclassified links group. It may be left blank.'),
    );

  $form['group']['weblinks_unpublished_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for unpublished links'),
    '#default_value' => variable_get('weblinks_unpublished_title', t('Unpublished')),
    '#description' => t('This will be used to describe the unpublished links. If you leave it blank, there will be no separate "unpublished" group on the links page; this can be desirable if your site\'s theme identifies unpublished content.'),
    );

  $form['group']['weblinks_unpublished_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Description for unpublished links'),
    '#rows' => 1,
    '#default_value' => variable_get('weblinks_unpublished_desc', t('These links are not published and need to be reviewed.')),
    '#description' => t('This will be used as the description for the unpublished links group.  It may be left blank.'),
    );

/**
 * Settings for link display.
 */
  $form['link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link settings'),
    '#description' => t('These settings control the way individual links display on the main links page.'),
    '#attributes' => array('id' => 'weblinks-link-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $form['link']['weblinks_show_title'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Show the node title'),
    '#default_value' => (int) variable_get('weblinks_show_title', TRUE),
    '#description' => t('If your theme automatically provides a node title, set this to "No." If you see duplicated titles, then set this to "No" and see if that resolves the problem.'),
    );

  $form['link']['weblinks_title_link'] = array(
    '#type' => 'radios',
    '#options' => array(1 => t('Node'), 0 => t('Site')),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Link the title to the node'),
    '#default_value' => (int) variable_get('weblinks_title_link', TRUE),
//    '#description' => t('This makes the node title link to the node. For some themes, this may not work without also making some simple theme changes.'),
    '#description' => t('When clicking the node title on the links page, "Node" takes the user to the standard node display; "Site" takes the user to the site (using click tracking if it is enabled). Note that "open in a new page" and "spam deterrent" are not available to the title link without theme changes.'),
    );

  $desc_opts = array(
    'none' => t('No description'),
    'teaser' => t('Teaser length'),
    'body' => t('Full length'),
    );
  $form['link']['weblinks_linkdesc'] = array(
    '#type' => 'radios',
    '#options' => $desc_opts,
    '#title' => t('Link description display'),
    '#default_value' => variable_get('weblinks_linkdesc', 'teaser'),
    '#description' => t('This sets how the description will be displayed on the main links page.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['link']['weblinks_redirect'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Enable click-tracking'),
    '#default_value' => (int) variable_get('weblinks_redirect', FALSE),
    '#description' => t('This turns on the feature to record the number of times a link has been clicked upon. It is a requirement for the "Popular" block.'),
    );

  $form['link']['weblinks_external'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Open links in a new window'),
    '#default_value' => (int) variable_get('weblinks_external', TRUE),
    '#description' => t('Recommended to keep your vistors on your site. Note that this will cause HTML "Strict" validation errors if used.'),
    );

  $form['link']['weblinks_user_links'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t("Display a link to other users' links"),
    '#default_value' => (int) variable_get('weblinks_user_links', FALSE),
    '#description' => t("Shows a link to another user's links."),
    );

  $form['link']['weblinks_show_url'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t("Display URL/Visit on links page"),
    '#default_value' => (int) variable_get('weblinks_show_url', TRUE),
    '#description' => t("Determines whether the URL or invitational link (next setting) will be used on the links page. It will always be shown on a node view page."),
    );

  $form['link']['weblinks_view_as'] = array(
    '#type' => 'radios',
    '#title' => t('Link displays as'),
    '#options' => array('url' => t('URL'), 'visit' => t('Visit')),
    '#default_value' => variable_get('weblinks_view_as', 'url'),
    '#description' => t('This option sets whether the URL itself will be displayed or will hide behind an invitational link, such as "Visit <em>title</em>".'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $token = module_exists('token');
  $form['link']['weblinks_visit_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Visit" text'),
    '#default_value' => variable_get('weblinks_visit_text', t('Visit [title]')),
    '#description' => t('This is the text that will be used with the node title (indicated by "[title]") to invite a click.'),
    '#prefix' => '<div class="weblinks_visit_hide">',
    );

  if ($token) {
    $form['link']['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      );

    $form['link']['token_help']['help'] = array(
      '#value' => theme('token_help', 'node'),
      );
  }

  // The end div here ends the "weblinks_visit_hide" above.
  $form['link']['weblinks_trim'] = array(
    '#type' => 'select',
    '#options' => $len_opts,
    '#title' => t('Maximum link title length'),
    '#default_value' => variable_get('weblinks_trim', 0),
    '#description' => t('This controls how much of long URLs will be shown in a link in full node view. Zero (0) means unlimited.'),
    '#prefix' => '</div><!-- /weblinks_visit_hide --><div class="weblinks_trim_hide">',
    );

  $form['link']['weblinks_strip'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
//    '#suffix' => '</div>',
    '#title' => t('Strip protocol and "www"'),
    '#default_value' => (int) variable_get('weblinks_strip', FALSE),
    '#description' => t('If enabled, the protocol (e.g. "http://") and "www" (if present) will be removed before displaying the URL.'),
    '#suffix' => '</div></div><!-- /weblinks_trim_hide -->',
    );

/**
 * Settings for field defaults.
 */
  $form['field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field defaults'),
    '#description' => t("These settings provide various defaults for fields that may be entered. Be careful when changing these as they can interfere with a user's input."),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    );

  $form['field']['weblinks_url_stub'] = array(
    '#type' => 'textfield',
    '#title' => t('Default for URL'),
    '#default_value' => variable_get('weblinks_url_stub', 'http://'),
    '#description' => t('This will be used as a stub in the URL field.'),
    );

  $form['field']['weblinks_body_stub'] = array(
    '#type' => 'textfield',
    '#title' => t('Default for description'),
    '#default_value' => variable_get('weblinks_body_stub', ''),
    '#description' => t('This will be used as a stub in the "Link description" field.'),
    );

  $format_list = filter_formats();
  $formats = array(FILTER_FORMAT_DEFAULT => '-default-');
  foreach ($format_list as $number => $filter) {
    $formats[$number] = $filter->name;
  }
  $form['field']['weblinks_format'] = array(
    '#type' => 'radios',
    '#options' => $formats,
    '#title' => t('Default input format'),
    '#default_value' => variable_get('weblinks_format', FILTER_FORMAT_DEFAULT),
    '#description' => t('This will be used as the default input format.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

//  $form['#redirect'] = 'weblinks';
  return system_settings_form($form);
}

