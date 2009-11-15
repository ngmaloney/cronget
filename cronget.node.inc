<?php
/**
 * @File contains cronget node functions
**/

/**
 * Implementation of hook_node_info()
 **/
function cronget_node_info() {
  return array(
    'cronget' => array(
      'name' => t('Cronget'),
      'module' => 'cronget',
      'description' => t('Nodes created by the cronget module'),
      'has_title' => TRUE,
      'title_label' => t('Title'), //Uses cronget job title
      'has_body' => FALSE, //We store body in {cronget_data}
      'locked' => TRUE,
    )
  );
}

/**
 * Implementation of hook_view()
**/
function cronget_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);
  $node->content['cronget_content'] = array(
    '#value' => $node->cronget_content,
    '#weight' => 1,
  );
  
  return $node;
}

/**
 * Implementation of hook_insert()
 **/
function cronget_insert($node) {
  $data = cronget_node_to_array($node); //Need this to save to cronget_data
  db_query(
    "INSERT INTO {cronget} (vid,nid,jid) VALUES (%d,%d,%d)",
    $node->vid,
    $node->nid,
    $node->jid
  );
  //Save to {cronget_data} table
  cronget_save_data($data);
}

/**
 * Implementation of hook_update()
 *
 **/
function cronget_update($node) {
  $data = cronget_node_to_array($node); //Need this to save to cronget_data
  if ($node->revision) {
    cronget_insert($node);
  }
  else {
    db_query(
      "UPDATE {cronget} SET nid = %d, jid = %d WHERE vid = %d",
      $node->nid,
      $node->jid,
      $node->vid
    );
  }
  cronget_save_data($data);
}


/**
 * Implemenation of hook_delete()
 **/
function cronget_delete($node) {
  $data = cronget_node_to_array($node); //Need this to save to cronget_data
  db_query(
    "DELETE FROM {cronget} WHERE nid = %d",
    $node->nid
  );
}

/**
 * Implementation of hook_load()
 **/
function cronget_load($node) {
  $result = db_query(
    "SELECT n.vid, n.jid, d.content AS cronget_content, j.type AS cronget_type FROM {cronget} n
     INNER JOIN {cronget_data} d ON n.jid = d.jid
     INNER JOIN {cronget_jobs} j ON n.jid = j.jid
     WHERE n.vid = %d",
     $node->vid
  );
  return db_fetch_object($result);
}

/**
 * Hook for handling node insert business logic
 * @param $job
 * Array representing a cronget job
 * @param $data
 * Array representing a node data object
**/
function cronget_nodehandler($job, $data) {
  $node = false;
  //Search for preexisting nodes
  if ($job['type'] == 'static') {
    $node = cronget_get_node_by_jid($data['jid']);    
  }
  //If no nodes are found create a new node obj.
  if (!$node) {
    $node = cronget_createnode($data);
  }
  //Update node fields on existing objs
  else {
    $node->content = $data['content'];
    $node->cronget_type = $data['type'];
    $node->jid = $data['jid'];
    $node->title = $data['title'];
  }
  node_save($node);
  watchdog('cronget', t('Node saved: %node', array('%node' => serialize($node))));
  //Update Last Update
  db_query("UPDATE {cronget_jobs} SET checked = %d WHERE jid = %d", time(), $data['jid']);
  //Add watchdog element
}


/**
 * Function to get a cronget node by $jid
 * @param $jid
 * An int containing the cronget $jid (job id)
**/
function cronget_get_node_by_jid($jid) {
  $result = db_fetch_array(db_query("SELECT nid FROM {cronget} WHERE jid = %d", $jid));
  if (isset($result['nid'])) {
    return node_load($result['nid']);
  }
  else {
    return FALSE;
  }
}

/**
 * Creates a cronget node from an array
 * @param $data
 * An array of data containing the following elements:
 *
 * $data['uid'] - cron_get node uid
 * $data['title'] - cron_get node title
 * $data['content'] - cron_get node content
 * $data['type'] - cron_get node type
 * $data['jid'] - cron_get job id
 * @return a node object
**/
function cronget_createnode($data) {
  $node = new stdClass();
  $node->type = CRONGET_NODETYPE;
  $node->language = '';
  $node->uid = $data['uid'];
  $node->status = 1;
  $node->created = time();
  $node->title = $data['title'];
  $node->name = $user->name;
  //Custom Node Attributess
  $node->content = $data['content'];
  $node->cronget_type = $data['type'];
  $node->jid = $data['jid'];
  return $node;
}

/**
 * Funtion that converts a node object to a cronget_job array
 * @param $node
 * A node boject to convert
 **/
function cronget_node_to_array($node) {
  return array(
    'jid' => $node->jid,
    'content' => $node->content,
    'timestamp' => $node->date,
    'type' => $node->cronget_type
  );
}

?>