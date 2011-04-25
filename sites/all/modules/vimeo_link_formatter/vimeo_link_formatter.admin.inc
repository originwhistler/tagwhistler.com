<?php
// $Id: vimeo_link_formatter.admin.inc,v 1.1 2011/02/03 11:04:39 bevan Exp $

/**
 * @file
 * Provides admin functions for the Vimeo Link Formatter.
 */


/**
 * Implementation of hook_field_formatter_settings_summary_FORMATTER().
 *
 * @return String
 *    Translated text string summarizing the formatter settings.
 */
function vimeo_link_formatter_field_formatter_settings_summary_vimeo_link_formatter_player($field, $instance, $view_mode) {
  // Access the formatter settings for this instance of the Link field.
  $settings = $instance['display'][$view_mode]['settings'];

  // Build a list of elements that are configured to show.
  $show = array(
    'portrait' => t('Portrait'),
    'title' => t('Title'),
    'byline' => t('Byline'),
  );
  foreach (array_keys($show) as $name) {
    if (!$settings[$name]) {
      unset($show[$name]);
    }
  }
  if (empty($show)) {
    // Zero (0) elements are shown.
    $show = t('none');
  }
  else {
    $last = array_pop($show);
    if (empty($show)) {
      // One (1) element is shown.
      $show = $last;
    }
    else {
      // Two (2) or more elements are shown.
      $list = implode(', ', $show);
      $show = t('!list and !last', array('!list' => $list, '!last' => $last));
    }
  }

  // Build a comma-separated list of options that are enabled.
  $options = array(
    'autoplay' => t('Autoplay'),
    'loop' => t('Loop'),
  );
  foreach (array_keys($options) as $name) {
    if (!$settings[$name]) {
      unset($options[$name]);
    }
  }
  // Join the enabled options with commas and end with a period.
  // Set $options to an empty string if none are enabled.
  $options = empty($options) ? '' : implode(', ', $options) . '.';

  // This CSS is (mostly) semantic, not style.  Inline CSS is appropriate.
  $style = '
    background-color: #' . $settings['color'] . ';
    color: black;
    font-weight: bold;
    padding-left: 0.2em;
    padding-right: 0.2em;
  ';
  $vars = array(
    '!width' => $settings['width'],
    '!height' => $settings['height'],
    // Inline CSS is not translatable.
    '!attributes' => drupal_attributes(compact('style')),
    '!color' => $settings['color'],
    '!show' => $show,
    '!options' => $options,
  );

  return t('!width x !height <code !attributes>!color</code>  Show !show.  !options', $vars);
}

/**
 * Implementation of hook_field_formatter_settings_form_FORMATTER().
 */
function vimeo_link_formatter_field_formatter_settings_form_vimeo_link_formatter_player($field, $instance, $view_mode, $form, &$form_state) {
  // Access the formatter settings for this instance of the Link field.
  $settings = $instance['display'][$view_mode]['settings'];
  $defaults = vimeo_link_formatter_default_settings_player();

  // A render()-compatible array that is embeddeble into a Form API form.
  $form = array();

  // Width textbox.
  $form['width'] = array(
    '#title' => t('Width'),
    '#description' => t('The width of the player in pixels or relative to the container element.  Do not include "<em>px</em>".  E.g. "<em>500</em>" or "<em>100%</em>".  Defaults to "<em>!default</em>".', array('!default' => $defaults['width'])),
  );

  // Height textbox.
  $form['height'] = array(
    '#title' => t('Height'),
    '#description' => t('The height of the player in pixels or relative to the container element.  Do not include "<em>px</em>".  E.g. "<em>280</em>" or "<em>56%</em>".  Defaults to "<em>!default</em>".', array('!default' => $defaults['height'])),
  );

  // Color textbox.
  // Inline CSS is not translatable.
  $style = 'font-weight: bold; padding-left: 0.2em; padding-right: 0.2em;';
  $example_style = "background-color: #F90; color: black; $style";
  $default_style = "background-color: #{$defaults['color']} ; color: white; $style;";
  $vars = array(
    '!default' => $defaults['color'],
    '!default_style' => drupal_attributes(array('style' => $default_style)),
    '!example_style' => drupal_attributes(array('style' => $example_style)),
  );
  $form['color'] = array(
    '#title' => t('Color'),
    '#description' => t('The color of links and controls (on hover) of the player, such as the title and byline.  CSS colors are not valid.  Six digit hexadecimal colors <em>without</em> the hash/pound character ("<em>#</em>") are valid.  E.g. <code !example_style>FF9900</code>. Defaults to <code !default_style>!default</code>.', $vars),
  );

  // Video information.
  $form['show'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video information'),
    '#description' => t('Show or hide data about the video in the player.'),
    'portrait' => array(
      '#title' => t('Portrait'),
      '#description' => t("The video submitter's picture or avatar."),
    ),
    'title' => array(
      '#title' => t('Title'),
      '#description' => t('The name of the video.'),
    ),
    'byline' => array(
      '#title' => t('Byline'),
      '#description' => t('Who the video is by.'),
    ),
  );

  // Process children of the video information fieldset.
  $parents = array('fields', $field['field_name'], 'settings_edit_form', 'settings');
  foreach (array_keys($form['show']) as $key) {
    // Ignore Form API attributes.
    if (substr($key, 0, 1) !== '#') {
      // All children are checkboxes.
      $form['show'][$key]['#type'] = 'checkbox';

      // Set #default_value too.
      $form['show'][$key]['#default_value'] = $settings[$key];

      // The formatter settings form is a sub-form element in the node type's
      // "Manage Display form".  Hack in the correct #parents so that the
      // children form elements get found in the correct place.
      $form['show'][$key]['#parents'] = array_merge($parents, array($key));
    }
  }

  // Autoplay checkbox.
  $form['autoplay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoplay'),
    '#description' => t('Automatically play the video on load.  This also causes the portrait, title and byline to be hidden.'),
  );

  // Loop textbox.
  $form['loop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop'),
    '#description' => t('Play the video repeatedly.'),
  );

  // Set some Form API attributes that apply to all elements.
  $size = max(strlen('000000'), strlen('100%'));
  foreach (array_keys($form) as $key) {
    // Textboxes are smaller than default.  #size is ignored for checkboxes.
    $form[$key]['#size'] = $size;

    // Default to #type => textfield if #type is not already set.
    if (!isset($form[$key]['#type'])) {
      $form[$key]['#type'] = 'textfield';
    }

    // Set #default_value too.
    if (isset($settings[$key])) {
      $form[$key]['#default_value'] = $settings[$key];
    }
  }

  return $form;
}
