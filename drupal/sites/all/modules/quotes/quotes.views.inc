<?php
// $Id: quotes.views.inc,v 1.1.2.2 2009/10/01 04:26:30 nancyw Exp $

/**
 * @file
 * The quotes module allows users to maintain a list of quotes that
 * can be displayed in any number of administrator-defined quote
 * blocks.
 */

/**
 * Implementation of hook_views_data().
 */
function quotes_views_data() {
  // Basic table information.
  // ----------------------------------------------------------------
  // quotes table
  $data['quotes']['table']['group']  = t('Quotes');
  $data['quotes']['table']['join'] = array(
    // ...to the node table
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['quotes_authors']['table']['group']  = t('Quotes');
  $data['quotes_authors']['table']['join'] = array(
    // ...to the quotes table
    'node' => array(
      'left_table' => 'quotes',
      'left_field' => 'aid',
      'field' => 'aid',
    ),
  );

  // Citation.
  $data['quotes']['citation'] = array(
    'title' => t('Citation'),
    'help' => t('The source of the quote.'),
    'field' => array(
//      'handler' => 'views_handler_field_quotes',
      'click sortable' => TRUE,
      ),
    'sort' => array('handler' => 'views_handler_sort'),
  );

  // Author.
  $data['quotes_authors']['name'] = array(
    'title' => t('Author'),
    'help' => t('The name of the quote\'s author.'),
    'field' => array(
      'handler' => 'views_handler_field_quotes',
      'click sortable' => TRUE,
      ),
    'sort' => array('handler' => 'views_handler_sort'),
    'skip base' => 'quotes',
  );

  // Bio.
  $data['quotes_authors']['bio'] = array(
    'title' => t('Biography'),
    'help' => t('The biography of the quote\'s author.'),
    'field' => array(
//      'handler' => 'views_handler_field_quotes',
      'click sortable' => TRUE,
      ),
    'sort' => array('handler' => 'views_handler_sort'),
    'skip base' => 'quotes',
  );

  return $data;
}
