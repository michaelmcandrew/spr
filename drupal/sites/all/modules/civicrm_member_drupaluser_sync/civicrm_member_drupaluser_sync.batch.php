<?php
//define a nice root
$druproot='/home/spradmin/public_html/main';

//bootstrap Drupal
chdir($druproot);
require_once('includes/bootstrap.inc');
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

//bootstrap CiviCRM
define( 'CIVICRM_CONFDIR', $druproot.'/sites' );
require_once CIVICRM_CONFDIR.'/all/modules/civicrm/civicrm.config.php';
require_once(CIVICRM_CONFDIR.'/default/civicrm.settings.php');
require_once 'CRM/Core/Config.php';
$config = CRM_Core_Config::singleton();

require_once('civicrm_member_drupaluser_sync.module');

// find all members without logins and call this
$params=array();
$query="SELECT uf_id, cc.id as id, ce.email
FROM civicrm_contact AS cc
JOIN civicrm_membership AS cm ON cc.id=cm.contact_id
JOIN civicrm_membership_status AS cms ON cms.id=cm.status_id
JOIN civicrm_email AS ce ON ce.contact_id=cc.id
LEFT JOIN civicrm_uf_match AS cuf ON cuf.contact_id=cc.id
WHERE uf_id IS NULL AND is_current_member AND email IS NOT NULL AND email !='' AND email COLLATE utf8_unicode_ci NOT IN (SELECT mail FROM drupal_users)
GROUP BY cc.id";
$result = CRM_Core_DAO::executeQuery( $query, $params );
while($result->fetch()){
	echo civicrm_member_drupaluser_sync_create_user($result->id);
}
