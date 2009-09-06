<?php

/**
 * @file Functionality for administering cronget jobs
**/

/**
 * Function for displaying the cronget administration page
**/
function cronget_admin_overview() {
  $output = t("This is the cronget admin overview page.");
  return $output;
}

/**
 * Function for creating the cronget job administration form
**/
function cronget_form_job(&$form_state, $edit = array()) {
  $period = drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval');
  $form_types = array('static' => t('Static'), 'streaming' => t('Streaming'));
  $saveas = array('block' => t('Block'), 'node' => t('Node'));
  $form['title'] = array('#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $edit['title'],
    '#maxlength' => 255,
    '#description' => t('The job title.'),
    '#required' => TRUE,
  );
  $form['url'] = array('#type' => 'textfield',
    '#title' => t('URL'),
    '#default_value' => $edit['url'],
    '#maxlength' => 255,
    '#description' => t('The fully-qualified URL of the data you wish to retrieve.'),
    '#required' => TRUE,
  );
  $form['refresh'] = array('#type' => 'select',
    '#title' => t('Update interval'),
    '#default_value' => $edit['refresh'],
    '#options' => $period,
    '#description' => t('The refresh interval indicating how often you want to update this job. Requires crontab.'),
  );
  $form['saveas'] = array('#type' => 'checkboxes',
    '#title' => t('Save As'),
    '#default_value' => explode(",", $edit['saveas']), //This is hacky but it works
    '#options' => $saveas,
    '#description' => t('Will optionally save each job as a node or a block. If saved as a block, it will use the same title as the job title.')
  );
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Job Type'),
    '#options' => $form_types,
    '#description' => t('Static jobs update the data set on each execution. Streaming jobs create a new row on each submission.'),
    '#default_value' => $edit['type'],
    '#attributes' => array('class' => 'hidden'), //Only display this if saveas is of type node
  );
  $form['description'] = array('#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $edit['description'],
    '#description' => t("Description of cronget job.")
  );
  $form['callback'] = array(
    '#type' => 'fieldset',
    '#title' => t('Callback'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['callback']['callback'] = array('#type' => 'textarea',
    '#title' => t('Callback'),
    '#default_value' => $edit['callback'],
    '#description' => t("PHP code to execute after job has been run. Include the <?php ?> tags. The cronget data is stored in an associative array. The array contains the following fields: \$data['jid'] - Job ID, \$data['content'] - Remote data payload."),
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Submit'));
  $form['uid'] = array('#type' => 'hidden', '#value' => $user->uid);
  if ($edit['jid']) {
    $form['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
    $form['jid'] = array('#type' => 'hidden', '#value' => $edit['jid']);
  }
  return $form;
}