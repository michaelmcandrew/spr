<?php
// $Id: views_handler_field_weblinks.inc,v 1.1.2.7.2.1 2009/04/11 18:10:35 nancyw Exp $
/**
 * @file
 * Provide views data and handlers for weblinks.module
 */

/**
 * Field handler to provide an embedded image.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_weblinks extends views_handler_field {
  /**
   * Modify the query to make sure title is available.
   */
  function construct() {
    $this->additional_fields['title'] = array('table' => 'node', 'field' => 'title');
  }

  /**
   * Define options available for this field.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['link_type'] = array('default' => 'url');
    return $options;
  }

  /**
   * Build option configuration form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_type'] = array(
      '#title' => t('Show link as'),
      '#type' => 'select',
      '#options' => array(
        'url' => 'URL',
        'text' => 'Text',
        'title' => 'Title',
        'visit' => '"Link displays as" setting',
        ),
      '#default_value' => $this->options['link_type'],
    );
  }

  /**
   * Render field output to the browser.
   */
  function render($values) {
    $title = $values->{$this->aliases['title']};
    $url = $values->{$this->field_alias};
    $type = $this->options['link_type'];
    switch ($type) {
      case 'url':
        $link = l($url, $url, _weblinks_set_attributes());
        return l($url, $url, _weblinks_set_attributes());

      case 'title':
        return l($title, $url, _weblinks_set_attributes());

      case 'visit':
        $values->url = $url;
        $values->title = $title;
        return theme('weblinks_node_view', $values, _weblinks_set_attributes(), NULL);

      default:  // Also 'text'
        return check_plain($url);
    }
  }
}

function _weblinks_set_attributes() {
  $options = array('attributes' => array('rel' => 'tag'));
  if (variable_get('weblinks_external', TRUE)) {
    $options['attributes']['target'] = '_blank';
  }

  if (variable_get('weblinks_nofollow', FALSE)) {
    $options['attributes']['rel'] = 'nofollow, tag';
  }
  return $options;
}
