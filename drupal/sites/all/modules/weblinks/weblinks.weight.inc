<?php
// $Id: weblinks.weight.inc,v 1.1.2.2 2009/09/20 22:33:41 nancyw Exp $

/**
 * @file
 * Enable submission and display of categorized web links.
 * This is an add-in that allows Web Links to act as though the Weight
 * module is present. "Borrowed" from the Weight module.
 */

/**
 * Convert our weight to 'encoded' sticky value for DB.
 * Stickiness is the inverse of weight - stickiness is sorted DESC while
 * weight is sorted ASC so we invert the weight before saving...
 * If the sticky box is checked, subtract weight from 100;
 * unweighted sticky nodes will have a value of 100.
 */
function weblinks_weight_encode(&$node) {
  if ($node->sticky == 1) {
    $node->sticky = 100 - $node->node_weight;
  }
  // Unweighted non-sticky nodes will have a value of -100.
  else {
    if ($node->sticky == 0) {
      $node->sticky = -($node->node_weight + 100);
    }
  }
}

/**
 * Convert our weight back out of sticky.
 */
function weblinks_weight_decode(&$node) {
  if ($node->sticky == 0 || $node->sticky == 1) {
    $node->node_weight = 0;
    return;
  }
  
  if ($node->sticky > 0) {
    $node->node_weight = 100 - $node->sticky;
    $node->sticky = 1;
  }
  else {
    $node->node_weight = -($node->sticky + 100);
    $node->sticky = 0;
  }
}
