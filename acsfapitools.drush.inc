<?php

require_once DRUSH_BASE_PATH . '/commands/core/rsync.core.inc';

/**
 * Implements hook_drush_command().
 */
function acsfapitools_drush_command() {

  $items['sf-connect'] = array(
    'description' => 'Return the API url and user:password for the current site',
    'aliases' => array('sfc'),
  );
  $items['sf-ping'] = array(
    'description' => 'Ping the API per your alias or config',
    'aliases' => array('sfp'),
  );
  $items['sf-status'] = array(
    'description' => 'Get status information',
    'aliases' => array('sfst'),
  );
  return $items;
}

/**
 * Callback for the sf-connect command
 */
function drush_acsfapitools_sf_connect() {
  drush_log(dt(acsfapitools_conn()), 'ok');
}

/**
 * Callback for the sf-ping command
 */
function drush_acsfapitools_sf_ping() {
  $data = acsfapitools_getcmd('ping');
  drush_log(dt($data->message), 'ok');
}

/**
 * Callback for the sf-status get command
 */
function drush_acsfapitools_sf_status() {
  $data = acsfapitools_getcmd('status');
  $message = 'site_creation: ' . $data->site_creation . PHP_EOL
    . 'site_duplication: ' . $data->site_duplication . PHP_EOL
    . 'domain_management: ' . $data->domain_management . PHP_EOL
    . 'bulk_operations: ' . $data->bulk_operations . PHP_EOL;
  drush_log($message, 'ok');
}
