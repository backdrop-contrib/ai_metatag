<?php

/**
 * @file
 * Tool definitions and executors for ai_metatag agent tools.
 */

/**
 * Returns tool definitions for hook_ai_tools().
 */
function ai_metatag_tools_definitions() {
  $tools = [];

  $tools['get_metatags'] = [
    'definition' => [
      'type' => 'function',
      'function' => [
        'name' => 'get_metatags',
        'description' => 'Return all metatag field values currently set on a node. Includes title, description, og:title, og:description, robots, canonical, and any other configured fields. Call this before proposing or writing changes. Token placeholders like [node:title] are returned as-is.',
        'parameters' => [
          'type' => 'object',
          'properties' => [
            'nid' => ['type' => 'integer', 'description' => 'The node ID.'],
          ],
          'required' => ['nid'],
          'additionalProperties' => FALSE,
        ],
      ],
    ],
    'execute' => 'ai_metatag_tool_get_metatags',
    'operation' => 'read',
    'permission' => 'access content',
    'module' => 'ai_metatag',
    'category' => 'seo',
  ];

  $tools['get_metatag_defaults'] = [
    'definition' => [
      'type' => 'function',
      'function' => [
        'name' => 'get_metatag_defaults',
        'description' => 'Return the default metatag configuration for a content type bundle. These are fallback values used when a node has no per-node overrides.',
        'parameters' => [
          'type' => 'object',
          'properties' => [
            'bundle' => ['type' => 'string', 'description' => 'Content type machine name (e.g. article, page).'],
          ],
          'required' => ['bundle'],
          'additionalProperties' => FALSE,
        ],
      ],
    ],
    'execute' => 'ai_metatag_tool_get_metatag_defaults',
    'operation' => 'read',
    'permission' => 'administer site configuration',
    'module' => 'ai_metatag',
    'category' => 'seo',
  ];

  $tools['list_nodes_missing_metatags'] = [
    'definition' => [
      'type' => 'function',
      'function' => [
        'name' => 'list_nodes_missing_metatags',
        'description' => 'Find published nodes that are missing one or more metatag fields. Use this to build a work list for the metatag_agent.',
        'parameters' => [
          'type' => 'object',
          'properties' => [
            'fields' => [
              'type' => 'array',
              'items' => ['type' => 'string'],
              'description' => 'Metatag field names to check (e.g. ["description", "og:description"]). Defaults to ["description"].',
            ],
            'bundle' => ['type' => 'string', 'description' => 'Limit to a content type machine name (optional).'],
            'limit' => ['type' => 'integer', 'description' => 'Max results (default 20, max 100).'],
          ],
          'required' => [],
          'additionalProperties' => FALSE,
        ],
      ],
    ],
    'execute' => 'ai_metatag_tool_list_nodes_missing_metatags',
    'operation' => 'read',
    'permission' => 'access content',
    'module' => 'ai_metatag',
    'category' => 'seo',
  ];

  $tools['list_duplicate_metatag_values'] = [
    'definition' => [
      'type' => 'function',
      'function' => [
        'name' => 'list_duplicate_metatag_values',
        'description' => 'Find nodes that share identical literal values for a metatag field. Excludes pure token placeholders. Duplicate titles and descriptions hurt SEO.',
        'parameters' => [
          'type' => 'object',
          'properties' => [
            'field' => ['type' => 'string', 'description' => 'Metatag field to check (e.g. "title", "description").'],
            'bundle' => ['type' => 'string', 'description' => 'Limit to a content type (optional).'],
            'limit' => ['type' => 'integer', 'description' => 'Max duplicate groups to return (default 10).'],
          ],
          'required' => ['field'],
          'additionalProperties' => FALSE,
        ],
      ],
    ],
    'execute' => 'ai_metatag_tool_list_duplicate_metatag_values',
    'operation' => 'read',
    'permission' => 'access content',
    'module' => 'ai_metatag',
    'category' => 'seo',
  ];

  $tools['set_metatag'] = [
    'definition' => [
      'type' => 'function',
      'function' => [
        'name' => 'set_metatag',
        'description' => 'Set a single metatag field value on a specific node. Always call get_metatags first to confirm the current value. Only overwrites if the field is empty or the user has explicitly asked.',
        'parameters' => [
          'type' => 'object',
          'properties' => [
            'nid' => ['type' => 'integer', 'description' => 'The node ID.'],
            'field' => ['type' => 'string', 'description' => 'The metatag field name (e.g. description, og:description).'],
            'value' => ['type' => 'string', 'description' => 'The value to set. Tokens like [node:title] are valid.'],
          ],
          'required' => ['nid', 'field', 'value'],
          'additionalProperties' => FALSE,
        ],
      ],
    ],
    'execute' => 'ai_metatag_tool_set_metatag',
    'operation' => 'write',
    'destructive' => FALSE,
    'permission' => 'administer site configuration',
    'module' => 'ai_metatag',
    'category' => 'seo',
  ];

  $tools['set_metatag_default'] = [
    'definition' => [
      'type' => 'function',
      'function' => [
        'name' => 'set_metatag_default',
        'description' => 'Update the default metatag value for a content type bundle. Affects all nodes of that type that do not have a per-node override.',
        'parameters' => [
          'type' => 'object',
          'properties' => [
            'bundle' => ['type' => 'string', 'description' => 'Content type machine name.'],
            'field' => ['type' => 'string', 'description' => 'Metatag field name (e.g. title, description).'],
            'value' => ['type' => 'string', 'description' => 'New default value. Tokens recommended (e.g. [node:title] | [site:name]).'],
          ],
          'required' => ['bundle', 'field', 'value'],
          'additionalProperties' => FALSE,
        ],
      ],
    ],
    'execute' => 'ai_metatag_tool_set_metatag_default',
    'operation' => 'write',
    'destructive' => FALSE,
    'permission' => 'administer site configuration',
    'module' => 'ai_metatag',
    'category' => 'seo',
  ];

  return $tools;
}

// ---------------------------------------------------------------------------
// Executors
// ---------------------------------------------------------------------------

/**
 * Executor: get_metatags.
 */
function ai_metatag_tool_get_metatags(array $args) {
  $nid = isset($args['nid']) ? (int) $args['nid'] : 0;
  if ($nid <= 0) {
    return ['error' => 'Invalid nid.'];
  }
  $node = node_load($nid);
  if (!$node) {
    return ['error' => 'Node not found.'];
  }

  $langcode = LANGUAGE_NONE;
  if (function_exists('metatag_entity_get_language')) {
    $langcode = metatag_entity_get_language('node', $node);
  }

  $metatags = [];
  if (!empty($node->metatags) && is_array($node->metatags)) {
    $data = isset($node->metatags[$langcode]) ? $node->metatags[$langcode] : (isset($node->metatags[LANGUAGE_NONE]) ? $node->metatags[LANGUAGE_NONE] : []);
    foreach ($data as $tag => $info) {
      $metatags[$tag] = isset($info['value']) ? $info['value'] : '';
    }
  }

  return [
    'nid' => $nid,
    'title' => $node->title,
    'bundle' => $node->type,
    'metatags' => $metatags,
    'metatag_count' => count($metatags),
  ];
}

/**
 * Executor: get_metatag_defaults.
 */
function ai_metatag_tool_get_metatag_defaults(array $args) {
  $bundle = isset($args['bundle']) ? trim((string) $args['bundle']) : '';
  if ($bundle === '') {
    return ['error' => 'Bundle is required.'];
  }

  $defaults = [];
  if (function_exists('metatag_config_load_with_defaults')) {
    $instance = 'node:' . $bundle;
    $config = metatag_config_load_with_defaults($instance);
    if (is_array($config)) {
      foreach ($config as $tag => $info) {
        $defaults[$tag] = isset($info['value']) ? $info['value'] : '';
      }
    }
  }

  return [
    'bundle' => $bundle,
    'defaults' => $defaults,
    'default_count' => count($defaults),
  ];
}

/**
 * Executor: list_nodes_missing_metatags.
 */
function ai_metatag_tool_list_nodes_missing_metatags(array $args) {
  $fields = !empty($args['fields']) && is_array($args['fields']) ? $args['fields'] : ['description'];
  $bundle = isset($args['bundle']) ? trim((string) $args['bundle']) : '';
  $limit = isset($args['limit']) ? min(100, max(1, (int) $args['limit'])) : 20;

  // Load published node IDs.
  $query = db_select('node', 'n')
    ->fields('n', ['nid', 'title', 'type'])
    ->condition('n.status', 1);
  if ($bundle !== '') {
    $query->condition('n.type', $bundle);
  }
  $query->range(0, 200);
  $rows = $query->execute()->fetchAll();

  $missing = [];
  foreach ($rows as $row) {
    $node = node_load($row->nid);
    if (!$node) {
      continue;
    }
    $langcode = LANGUAGE_NONE;
    if (function_exists('metatag_entity_get_language')) {
      $langcode = metatag_entity_get_language('node', $node);
    }
    $data = [];
    if (!empty($node->metatags[$langcode])) {
      $data = $node->metatags[$langcode];
    }
    elseif (!empty($node->metatags[LANGUAGE_NONE])) {
      $data = $node->metatags[LANGUAGE_NONE];
    }

    $missing_fields = [];
    foreach ($fields as $field) {
      $val = isset($data[$field]['value']) ? trim($data[$field]['value']) : '';
      if ($val === '') {
        $missing_fields[] = $field;
      }
    }
    if (!empty($missing_fields)) {
      $missing[] = [
        'nid' => (int) $row->nid,
        'title' => $row->title,
        'bundle' => $row->type,
        'missing_fields' => $missing_fields,
      ];
    }
    if (count($missing) >= $limit) {
      break;
    }
  }

  return [
    'fields_checked' => $fields,
    'returned' => count($missing),
    'nodes' => $missing,
  ];
}

/**
 * Executor: list_duplicate_metatag_values.
 */
function ai_metatag_tool_list_duplicate_metatag_values(array $args) {
  $field = isset($args['field']) ? trim((string) $args['field']) : '';
  if ($field === '') {
    return ['error' => 'Field is required.'];
  }
  $bundle = isset($args['bundle']) ? trim((string) $args['bundle']) : '';
  $limit = isset($args['limit']) ? min(50, max(1, (int) $args['limit'])) : 10;

  // Load published nodes and collect metatag values. Hard-capped: every row
  // here becomes a full node_load(), so an unbounded scan can exhaust memory
  // on large sites.
  $scan_cap = 1000;
  $query = db_select('node', 'n')
    ->fields('n', ['nid', 'title', 'type'])
    ->condition('n.status', 1)
    ->orderBy('n.changed', 'DESC')
    ->range(0, $scan_cap + 1);
  if ($bundle !== '') {
    $query->condition('n.type', $bundle);
  }
  $all_rows = $query->execute()->fetchAll();
  $scan_truncated = count($all_rows) > $scan_cap;
  $rows = $scan_truncated ? array_slice($all_rows, 0, $scan_cap) : $all_rows;

  $value_map = [];
  foreach ($rows as $row) {
    $node = node_load($row->nid);
    if (!$node) {
      continue;
    }
    $langcode = LANGUAGE_NONE;
    if (function_exists('metatag_entity_get_language')) {
      $langcode = metatag_entity_get_language('node', $node);
    }
    $data = !empty($node->metatags[$langcode]) ? $node->metatags[$langcode] : (!empty($node->metatags[LANGUAGE_NONE]) ? $node->metatags[LANGUAGE_NONE] : []);
    $val = isset($data[$field]['value']) ? trim($data[$field]['value']) : '';
    // Skip empty and pure-token values.
    if ($val === '' || (strpos($val, '[') === 0 && strpos($val, ']') !== FALSE)) {
      continue;
    }
    $value_map[$val][] = ['nid' => (int) $row->nid, 'title' => $row->title];
  }

  $groups = [];
  foreach ($value_map as $val => $nodes) {
    if (count($nodes) > 1) {
      $groups[] = [
        'value' => $val,
        'count' => count($nodes),
        'nodes' => $nodes,
      ];
    }
  }
  usort($groups, function($a, $b) { return $b['count'] - $a['count']; });
  $groups = array_slice($groups, 0, $limit);

  return [
    'field' => $field,
    'duplicate_groups' => count($groups),
    'groups' => $groups,
    'scanned_nodes' => count($rows),
    'scan_truncated' => $scan_truncated,
  ];
}

/**
 * Executor: set_metatag.
 */
function ai_metatag_tool_set_metatag(array $args) {
  $nid = isset($args['nid']) ? (int) $args['nid'] : 0;
  $field = isset($args['field']) ? trim((string) $args['field']) : '';
  $value = isset($args['value']) ? (string) $args['value'] : '';

  if ($nid <= 0 || $field === '') {
    return ['error' => 'nid and field are required.'];
  }
  $node = node_load($nid);
  if (!$node) {
    return ['error' => 'Node not found.'];
  }

  $langcode = LANGUAGE_NONE;
  if (function_exists('metatag_entity_get_language')) {
    $langcode = metatag_entity_get_language('node', $node);
  }
  if (!isset($node->metatags) || !is_array($node->metatags)) {
    $node->metatags = [];
  }
  if (!isset($node->metatags[$langcode]) || !is_array($node->metatags[$langcode])) {
    $node->metatags[$langcode] = [];
  }

  $old_value = isset($node->metatags[$langcode][$field]['value']) ? $node->metatags[$langcode][$field]['value'] : '';
  $node->metatags[$langcode][$field]['value'] = $value;
  node_save($node);

  return [
    'nid' => $nid,
    'field' => $field,
    'old_value' => $old_value,
    'new_value' => $value,
    'saved' => TRUE,
  ];
}

/**
 * Executor: set_metatag_default.
 */
function ai_metatag_tool_set_metatag_default(array $args) {
  $bundle = isset($args['bundle']) ? trim((string) $args['bundle']) : '';
  $field = isset($args['field']) ? trim((string) $args['field']) : '';
  $value = isset($args['value']) ? (string) $args['value'] : '';

  if ($bundle === '' || $field === '') {
    return ['error' => 'bundle and field are required.'];
  }

  if (!function_exists('metatag_config_load')) {
    return ['error' => 'Metatag module not available.'];
  }

  $instance = 'node:' . $bundle;
  $config = metatag_config_load($instance);
  $old_value = '';

  if ($config) {
    $old_value = isset($config->config[$field]['value']) ? $config->config[$field]['value'] : '';
    if (!isset($config->config)) {
      $config->config = [];
    }
    $config->config[$field] = ['value' => $value];
    metatag_config_save($config);
  }
  else {
    // Create new config for this instance.
    $config = new stdClass();
    $config->instance = $instance;
    $config->config = [$field => ['value' => $value]];
    metatag_config_save($config);
  }

  return [
    'bundle' => $bundle,
    'field' => $field,
    'old_value' => $old_value,
    'new_value' => $value,
    'saved' => TRUE,
  ];
}
