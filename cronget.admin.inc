<?php

/**
 * @file Functionality for administering cronget jobs
**/

/**
 * Function for displaying the cronget administration page
**/
function cronget_admin_overview() {
<<<<<<< HEAD
  $output = cronget_admin_list();
  return $output;
}

/**
 * Function for getting list of cronget jobs
 * TODO: Add get by user_id arg
**/
function cronget_admin_list() {
  global $user;
  $jobs = cronget_get_jobs($user);
  $output = '<h3>'. t('Cronget Job Listings') .'</h3>';
  $rows = array();
  $header = array(
    t('Title'), 
    t('URL'), 
    t('Type'), 
    array('data' => t('Operations'), 'colspan' => '3')
  );
  foreach($jobs as $job) {
    $rows[] = array(
      t($job->title), 
      t($job->url), 
      t($job->type), 
      l(t('Edit'), "admin/content/cronget/edit/$job->jid"), 
      l(t('Delete'), "admin/content/cronget/delete/$job->jid"), 
      l(t('Refresh'), "admin/content/cronget/refresh/$job->jid")
    );
  }
=======
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
>>>>>>> 164b20fc23e0ce235e6d3ddcec449902fc5779a5
  $output .= theme('table', $header, $rows);
  return $output;
}

/**
 * Function for creating the cronget job administration form
**/
<<<<<<< HEAD
function cronget_form_job(&$form_state, $edit = array()) {
  global $user;
=======
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
>>>>>>> 164b20fc23e0ce235e6d3ddcec449902fc5779a5
  $period = drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval');
  $form_types = array('static' => t('Static'), 'streaming' => t('Streaming'));
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
  $form['jid'] = array('#type' => 'value',
    '#value' => $edit['jid'],
  );
  return $form;
}

/**
 * Implementation of hook_validate()
**/
<<<<<<< HEAD
function cronget_form_job_validate($form, &$form_state) {
  if(!valid_url($form_state['values']['url'],TRUE)) {
    form_set_error('url', 'URL appears invalid.');
  }
=======
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
>>>>>>> 164b20fc23e0ce235e6d3ddcec449902fc5779a5
}

/**
 * Implementation of hook_submit()
**/
<<<<<<< HEAD
function cronget_form_job_submit($form, &$form_state) {
  cronget_save_job($form_state);  
  drupal_set_message(t('Your job has been saved.'));
  $form_state['redirect'] = 'admin/content/cronget';
}

/**
 * Form Delete Function
**/
function cronget_form_delete(&$form_state, $edit = array()) {
  $form = array();
  $form['cronget'] = array(
     '#type' => 'value',
     '#value' => $edit,
   );
  return confirm_form($form, t('Are you sure you want to delete job %edit?', array('%edit' => $edit['title'])), 'admin/content/cronget', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Form Delete Action
**/
function cronget_form_delete_submit($form, &$form_state) {
  $jid = $form_state['values']['cronget']['jid'];
  db_query("DELETE FROM {cronget_jobs} WHERE jid = %d", $jid);
  drupal_set_message(t("Cronget Job deleted"));
  watchdog('cronget', 'Cronget job '. $jid . ' deleted.');
  $form_state['redirect'] = 'admin/content/cronget';
=======
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
>>>>>>> 164b20fc23e0ce235e6d3ddcec449902fc5779a5
}