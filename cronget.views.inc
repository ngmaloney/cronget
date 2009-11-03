<?php

/**
 * Implementation of hook_views_data()
 */
function cronget_views_data() {
  // Basic table information.
  $data['cronget']['table']['group']  = t('Cronget');

  // Join to 'node' as a base table.
  $data['cronget']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // ----------------------------------------------------------------
  // Fields

  // Job ID
  $data['cronget']['jid'] = array(
    'title' => t('Job ID'),
    'help' => t('Cronget Job ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'label' => t('Job ID'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // cronget content
  $data['cronget']['content'] = array(
    'title' => t('Job content'),
    'help' => t('Cronget Content'),
    'field' => array(
      'handler' => 'views_handler_field_text',
      'click sortable' => FALSE
    ),
  );

  return $data;
}

/**
 * @}
 */
