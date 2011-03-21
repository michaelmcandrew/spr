<?php
// $Id: weblinks.cck.inc,v 1.1.2.1 2009/09/16 15:33:04 nancyw Exp $

/**
 * @file
 * Enable submission and display of categorized web links.
 * Drupal 6 Version
 */

 /**
 * Implementation of hook_weblinks_extra_content().
 * Tell CCK about our fields so the user can manage them.
 */
function weblinks_weblinks_extra_content() {
  $extras = array();
 
  $extras['url'] = array(
    'label' => t('Link URL'),
    'description' => t('Links to another site.'),
    'weight' => 100,
    );
  $extras['click_count'] = array(
    'label' => t('Click counter'),
    'description' => t('How many times the link has been clicked upon.'),
    'weight' => 100,
    );
  $extras['last_click'] = array(
    'label' => t('Last click date/time'),
    'description' => t('Date/time of the last click.'),
    'weight' => 100,
    );
  $extras['last_status'] = array(
    'label' => t('Last status'),
    'description' => t('Status from the last validity check'),
    'weight' => 100,
    );
  $extras['last_status_info'] = array(
    'label' => t('Last status date/time'),
    'description' => t('Last status date/time'),
    'weight' => 100,
    );
  $extras['last_checked'] = array(
    'label' => t('Last checked date/time'),
    'description' => t('Date/time of the last validity check'),
    'weight' => 100,
    );
  $extras['urlhash'] = array(
    'label' => t('URL hash'),
    'description' => t('The hashed value of the URL'),
    'weight' => 100,
    );
  $extras['reciprocal'] = array(
    'label' => t('Reciprocal link'),
    'description' => t('A reciprocal URL for matching referers'),
    'weight' => 100,
    );

  return $extras;
}

/**
 * Implementation of hook_content_extra_fields.
 * Tell CCK about our fields so the user can manage them.
 */
function weblinks_content_extra_fields() {
  $extras = module_invoke_all('weblinks_extra_content');
  return $extras;
}
