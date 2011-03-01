<?php

/**
 * Implementation of hook_views_data()
 */
function cronget_views_data() {
  $data = array();

  $data['cronget']['table']['group'] = t('Cronget');

  $data['cronget']['table']['base'] = array(
    'field' => 'nid',
    'title' => t('Cronget Job'),
  );

  $data['cronget']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['cronget']['nid'] = array(
    'title' => t('Cronget ID'),
    'help' => t('The Cronget ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'label' => t('Cronget ID'),
      'base' => 'node',
      'base field' => 'nid',
      'skip base' => array(),
    ),
  );

  //Cronget Data Relationship
  $data['cronget_data']['table']['group'] = t('Cronget');

  $data['cronget_data']['table']['join'] = array(
    'node' => array(
      'left_table' => 'cronget',
      'left_field' => 'jid',
      'field' => 'jid',
    ),
    'cronget' => array(
      'left_field' => 'jid',
      'field' => 'jid',
    ),
  );

  $data['cronget_data']['content'] = array(
    'title' => t('Content'),
    'help' => t('Cronget Data Payload.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  return $data;
}
