<?php
// $Id $

/**
 * @file
 * Enable submission and display of categorized web links.
 * Drupal 6 Version
 */

/**
 * Implementation of hook_weblinks_extra_content().
 * Tell CCK about our fields so the user can manage them.
 */
function pralexa_weblinks_extra_content() {
  $extras = array();
 
  $extras['rank_checked'] = array(
    'label' => t('Last rank checked date/time'),
    'description' => t('Date/time of the last ranking check.'),
    'weight' => 100,
    );
  $extras['pagerank'] = array(
    'label' => t('Google pagerank'),
    'description' => t('Google pagerank.'),
    'weight' => 100,
    );
  $extras['alexa'] = array(
    'label' => t('Alexa traffic rank'),
    'description' => t('Alexa traffic rank.'),
    'weight' => 100,
    );

  return $extras;
}
