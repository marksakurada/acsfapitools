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
  $items['sf-setstatus'] = array(
    'description' => 'Modify the status.',
    'aliases' => array('sfsetst'),
  );
  $items['sf-sites'] = array(
    'description' => 'Gets a list of sites.',
    'aliases' => array('sfs'),
  );
  $items['sf-sitedetail'] = array(
    'description' => 'Get detailed information about a site.',
    'aliases' => array('sfsd'),
    'examples' => array(
      'drush sf-sitedetail XXX' => 'Display the data for the site id "XXX" from the API.',
    ),
  'arguments' => array(
    'id' => 'The id of the site to fetch.',
  ),
  'required-arguments' => 1,
  );
  $items['sf-backup'] = array(
    'description' => 'Create a site backup.',
    'aliases' => array('sfbu'),
    'examples' => array(
      'drush sf-sitebackup XXX' => 'The backup site id "XXX".',
    ),
  'arguments' => array(
    'id' => 'The id of the site to backup.',
     ),
  'required-arguments' => 1,
  );
  $items['sf-backuplist'] = array(
    'description' => 'List site backups.',
    'aliases' => array('sfblist'),
    'examples' => array(
      'drush sf-sitebackuplist XXX' => 'List the backups site id "XXX".',
    ),
    'arguments' => array(
      'id' => 'The id of the site.',
      ),
      'required-arguments' => 1,
  );
  $items['sf-backupurl'] = array(
    'description' => 'Get temporary site backup URL.',
    'aliases' => array('sfburl'),
    'examples' => array(
      'drush sf-sitebackuplist XXX YYY' => 'Return a URL for site id "XXX" backup id "YYY".',
    ),
    'arguments' => array(
      'id' => 'The id of the site.',
      'bkup' => 'The id of the backup.',
      ),
      'required-arguments' => 1,
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
  $message = dt('site_creation: ') . $data->site_creation . PHP_EOL
    . dt('site_duplication: ') . $data->site_duplication . PHP_EOL
    . dt('domain_management: ') . $data->domain_management . PHP_EOL
    . dt('bulk_operations: ') . $data->bulk_operations . PHP_EOL;
  drush_log($message, 'ok');
}

/**
 * Callback for the sf-setstatus get command
 * @TODO currently not working with test data.
 * @TODO merge setstatus and status into single command
 */
function drush_acsfapitools_sf_setstatus() {
  $testdata = array(
    'site_creation' => 'off',
    'site_duplication' => 'on',
    'domain_management' => 'on',
    'bulk_operations' => 'on',
  );

  $data = acsfapitools_getcmd('status', 'PUT', json_encode($testdata));
  var_dump($data);
  drush_log($message, 'ok');
}

/**
 * Callback for the sf-sites get command
 *
 * @TODO: implement:
 * Request parameters
 * Name	Data type	Mandatory	Possible values	Default
 * --limit	int	no	A positive integer (max 100).	10
 * --page	int	no	A positive integer.	1
 * --canary	bool	no	No value necessary.	false
 * also it would be nice if this was a menu.
**/
function drush_acsfapitools_sf_sites() {
  $data = acsfapitools_getcmd('sites');
  $message = dt('count: ') . $data->count . PHP_EOL;
  var_dump($data);
  foreach ($data->sites as $value => $site) {
    $message .= dt('id|site|domain: ') . implode(' | ', (array) $site) . PHP_EOL;
  }
  drush_log($message, 'ok');
}

/**
 * Callback for the sf-sitedetail get command
 *
 * @param $id
 *   The ID of the site to fetch.
**/
function drush_acsfapitools_sf_sitedetail($id = NULL) {
  $data = acsfapitools_getcmd('sites/' . $id);
  $message = dt('id|created|owner|site: ')
    . $data->id . ' | ' . $data->created . ' | ' . $data->owner . ' | ' . $data->site . PHP_EOL;
  foreach ($data->domains as $value => $domain) {
    $message .=  $domain . PHP_EOL;
  }

  drush_log($message, 'ok');
}

/**
 * Callback for the sf-backup command
 *  Name	Data type	Mandatory	Possible values	Default
 * label	string	no	The human-readable description of this backup.
 * callback_url	string	no	The callback URL, which is invoked upon completion.
 * callback_method	string	no	The callback method, "GET", or "POST". Uses "POST" if empty.
 * caller_data	string	no	Data that should be included in the callback, json encoded.
 *
**/
function drush_acsfapitools_sf_backup($id = NULL) {
  $testdata = array(
    'label' => 'drush_backup',
  );

  $data = acsfapitools_getcmd('sites/' . $id . '/backup', 'POST', drupal_http_build_query($testdata));
  drush_log('Task id:' . $data->task_id, 'ok');
}

/**
 * Callback for the sf-backuplist command
**/
function drush_acsfapitools_sf_backuplist($id = NULL) {
  $data = acsfapitools_getcmd('sites/' . $id . '/backups');
  $message = dt('id | nid | status | uid | timestamp | bucket | directory | file | label') . PHP_EOL;
  foreach ($data->backups as $value => $backup) {
    $message .= implode(' | ', (array ) $backup) . PHP_EOL;
  }

  drush_log($message, 'ok');
}

//GET /api/v1/sites/{site_id}/backups/{backup_id}/url
/**
 * Callback for the sf-backupurl command
**/
function drush_acsfapitools_sf_backupurl($id = NULL, $bkid = NULL) {
  $data = acsfapitools_getcmd('sites/' . $id . '/backups/' . $bkid . '/url');
  drush_log($data->url, 'ok');
}
