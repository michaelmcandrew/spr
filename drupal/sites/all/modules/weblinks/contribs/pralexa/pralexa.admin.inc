<?php
// $Id: pralexa.admin.inc,v 1.1.2.2.2.8 2009/07/08 00:42:53 nancyw Exp $

/**
 * @file
 * Weblinks add-on to fetch Google and Alexa pageranks.
 */

 function pralexa_admin_settings() {
  drupal_add_css(drupal_get_path('module', 'weblinks') .'/weblinks.admin.css');
  $form = array();
  $form['#attributes'] = array('class' => 'weblinks-settings');
  $yesno = array(1 => t('Yes'), 0 => t('No'));

  $form['pralexa_show_title'] = array(
    '#type' => 'radios',
//    '#options' => $yesno,
    '#options' => array(1 => t('Link title'), 0 => t('URL')),
    '#title' => '<strong>'. t('Show title as') .'</strong>',
    '#default_value' => (int) variable_get('pralexa_show_title', 0),
    '#description' => t('This option determines how the link\'s title will be shown in the rankings page.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div><div class="clear-block"></div>',
    );

  $form['pralexa_link_title'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => '<strong>'. t('Link title to node') .'</strong>',
    '#default_value' => (int) variable_get('pralexa_link_title', FALSE),
    '#description' => t('If this option is selected, the row title will be linked to the node.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div><div class="clear-block"></div>',
    );

  $form['pralexa_show_rank'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => '<strong>'. t('Show ranking information') .'</strong>',
    '#default_value' => (int) variable_get('pralexa_show_rank', FALSE),
    '#description' => t('If this option is selected, the rankings will be displayed when the node is viewed.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div><div class="clear-block"></div>',
    );

  $form['pralexa_rows_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Rows per page'),
    '#default_value' => variable_get('pralexa_rows_per_page', 20),
    '#size' => 8,
    '#description' => t('This sets the number of links per page that will be shown on the Pralexa page.'),
    );

  $form['pralexa_url_trim'] = array(
    '#type' => 'textfield',
    '#title' => t('Title length shown'),
    '#default_value' => variable_get('pralexa_url_trim', variable_get('weblinks_trim_block', 50)),
    '#size' => 8,
    '#description' => t('This is the maximum length of the row title shown on the ranking page.'),
    );

  $form['pralexa_links_per_cron'] = array(
    '#type' => 'textfield',
    '#title' => t('Links per cron'),
    '#default_value' => variable_get('pralexa_links_per_cron', 20),
    '#size' => 8,
    '#description' => t('This sets the number of links that will be checked per Cron run. Checking too many may cause Cron to time out.'),
    );

  $interval = array(0, 30*60, 60*60, 2*60*60, 3*60*60, 4*60*60, 6*60*60, 12*60*60, 24*60*60, 2*24*60*60, 3*24*60*60, 7*24*60*60, 14*24*60*60);
  $form['pralexa_cron_interval'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc($interval, 'format_interval'),
    '#title' => t('Cron interval'),
    '#description' => t("This sets the frequency of checking. If you run Cron frequently, you may wish to set this higher than your Cron frequency so that you don't overload the ranking sites. Note that those sites do not update more than once a day. The check will be run at the next scheduled Cron run after this interval expires. An interval of 0 means every cron run."),
    '#size' => count($interval) / 2,
    '#default_value' => variable_get('pralexa_cron_interval', 86400),
    );

  $form['pralexa_show_when'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => '<strong>'. t('Include "Last checked" in list') .'</strong>',
    '#default_value' => variable_get('pralexa_show_when', FALSE),
    '#description' => t('If this option is selected, the time since the link was last checked will be included in the display.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div><div class="clear-block"></div>',
    );

  $form['#redirect'] = 'weblinks';
  return system_settings_form($form);
}
