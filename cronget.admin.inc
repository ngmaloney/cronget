<?php

/**
 * @file Functionality for administering cronget jobs
**/

/**
 * Function for displaying the cronget administration page
**/
function cronget_admin_overview() {
  $output = t("This is the cronget admin overview page.");
  $jobs = cronget_get_all_jobs();
  $rows = array();
  foreach($jobs as $job) {
    $rows[] = array(
      $job['title'], 
      $job['url'], 
      $job['type'], 
      l(t('Edit'), "admin/content/cronget/edit/".$job['jid']),
    );
  }
  $header = array(
    t('Title'), 
    t('URL'), 
    t('Job Type'),
    t('Operations'),
  );
  $output .= theme('table', $header, $rows);
  return $output;
}

/**
 * Function for creating the cronget job administration form
**/
function cronget_job_admin(&$form_state, $op, $edit = NULL) {
  global $user;
  if (empty($edit) || $op == 'add') {
    $edit = array(
      'title' => '',
      'url' => '',
      'refresh' => 0,
      'saveas' => '',
      'type' => '',
      'description' => '',
      'callback' => '',
      'jid' => NULL,
    );
  }
  else {
    $edit = cronget_load($edit);
  }
  //var_dump($edit);
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

/**
 * Implementation of hook_validate()
**/
function cronget_job_admin_validate($form, &$form_state) {
  //Validate Title
  if ($form_state['values']['title'] == '') {
    form_set_error('title', t('A title is required.'));
  }
  // Check for duplicate titles
  if (!isset($form_state['values']['jid'])) {
    $results = db_query("SELECT title FROM {cronget_jobs} WHERE title = '%s'",$form_state['values']['title']);
    while ($job = db_fetch_object($results)) {
      if (strcasecmp($job->title, $form_state['values']['title']) == 0) {
        form_set_error('title', t('A job named %title already exists. Please enter a unique title.', array('%title' => $form_state['values']['title'])));
      }
    }
  }
  //Validate url
  if ($form_state['values']['url'] == '') {
    form_set_error('url', t('A url is required.'));
  }
  if (!valid_url($form_state['values']['url'],TRUE)) {
    form_set_error('url', t('A valid url is required.'));
  } 
}

/**
 * Implementation of hook_submit()
**/
function cronget_job_admin_submit($form, &$form_state) {
  $job = array(
    'title' => $form_state['values']['title'],
    'url' => $form_state['values']['url'],
    'refresh' => $form_state['values']['refresh'],
    'type' => $form_state['values']['type'],
    'description' => $form_state['values']['description'],
    'callback' => $form_state['values']['callback'],
    'saveas' => $form_state['values']['saveas'],
    'uid' => $form_state['values']['uid'],
    'jid' => FALSE,
  );
  if (isset($form_state['values']['jid'])) {
    $job['jid'] = $form_state['values']['jid'];
  }
  if ($form_state['values']['op'] == t('Delete')) {
    //Return Delete Confirmation
    cronget_delete_job($job);
    drupal_set_message(t('Cronget Job Deleted.'));
  }
  else {
    cronget_save_job($job);
    drupal_set_message(t('Job Saved.'));
  }
  $form_state['redirect'] = 'admin/content/cronget/list';
  /* DEBUG
  var_dump($form_state['values']);
  exit();
  */
  return;
}