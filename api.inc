<?php

// 
// api.inc 
//

include_once "common.inc";
include_once "errors.inc";
include_once "utils.inc";
include_once "operation.inc";
include_once "node.inc";

define("API_ELEMENT_PARAM", "__param");
define("API_ELEMENT_PATH_PARAM", "__path_param");

define("API_PRIVATE", 0);
define("API_PUBLIC", 1);
define("API_NOT_PUBLISHED", 2); // i.e. doesn't require security hash, but we're not making it public (backup, new features)

define("API_VERB_GET", "GET");
define("API_VERB_UPDATE", "UPDATE");
define("API_VERB_GRANT", "GRANT");
define("API_VERB_REVOKE", "REVOKE");
define("API_VERB_PRINT", "PRINT");
define("API_VERB_CREATE", "CREATE");
define("API_VERB_RESTORE", "RESTORE");
define("API_VERB_REMOVE", "REMOVE");
define("API_VERB_UPLOAD", "UPLOAD");
define("API_VERB_DELETE", "DELETE");
define("API_VERB_INVITE", "INVITE");
define("API_VERB_CONFIRM", "CONFIRM");
define("API_VERB_LOGIN", "LOGIN");
define("API_VERB_LOGOUT", "LOGOUT");
define("API_VERB_SHARE", "SHARE");
define("API_VERB_EXPORT", "EXPORT");
define("API_VERB_UNEXPORT", "UNEXPORT");

// resources
define('API_RES_ROOT', 'ROOT');
define('API_RES_OAUTH', 'OAUTH');
define('API_RES_REQ_TOKEN', 'REQ_TOKEN');
define('API_RES_ACCESS_TOKEN', 'ACCESS_TOKEN');
define('API_RES_ME', 'ME');
define('API_RES_LOG', 'LOG');
define('API_RES_HOSTS', 'HOSTS');
define('API_RES_HOST', 'HOST');
define('API_RES_RDP', 'RDP');
define('API_RES_VNC', 'VNC');
define('API_RES_HTTP', 'HTTP');
define('API_RES_USB_DEVICES', 'USB_DEVICES');
define('API_RES_USB_DEVICE', 'USB_DEVICE');
define('API_RES_PERMISSIONS', 'PERMISSIONS');
define('API_RES_PERM_FRIENDS', 'PERM_FRIENDS');
define('API_RES_PERM_FRIEND', 'PERM_FRIEND');
define('API_RES_PERM_LINKS', 'PERM_LINKS');
define('API_RES_PERM_LINK', 'PERM_LINK');
define('API_RES_BANDWIDTH', 'BANDWIDTH');
define('API_RES_FOLDERS', 'FOLDERS');
define('API_RES_PATH', 'PATH');
define('API_RES_DOWNLOAD', 'DOWNLOAD');
define('API_RES_ZIP', 'ZIP');
define('API_RES_SLIDESHOW', 'SLIDESHOW');
define('API_RES_MUSIC', 'MUSIC');
define('API_RES_VIDEO', 'VIDEO');
define('API_RES_CONVERT', 'CONVERT');
define('API_RES_CONVERT_FILE', 'CONVERT_FILE');
define('API_RES_BACKUP', 'BACKUP');
define('API_RES_SEARCH', 'SEARCH');
define('API_RES_SEARCH_RESULT', 'SEARCH_RESULT');
define('API_RES_FRIENDS', 'FRIENDS');
define('API_RES_FRIEND', 'FRIEND');
define('API_RES_SHARES', 'SHARES');
define('API_RES_SHARE', 'SHARE');
define('API_RES_NOTIFICATIONS', 'NOTIFICATIONS');
define('API_RES_NOTIFICATION_SOCKET', 'NOTIFICATION_SOCKET');
define('API_RES_NOTIFICATION_POLL', 'NOTIFICATION_POLL');
define('API_RES_FRIEND_FRIENDS', 'FRIEND_FRIENDS');
define('API_RES_FRIEND_SHARES', 'FRIEND_SHARES');
define('API_RES_DOCS', 'DOCS');
define('API_RES_AUTH', 'AUTH');
define('API_RES_LINKS', 'LINKS');
define('API_RES_LINK', 'LINK');
define('API_RES_ACCOUNTS', 'ACCOUNTS');
define('API_RES_FILE_SYS', 'FILE_SYS');
define('API_RES_FS_PATH', 'FS_PATH');
define('API_RES_APPS', 'APPS');
define('API_RES_APP', 'APP');

// operation constants take the form <module>|<op name> where <module> is used to load <module>/api_<module>.inc
define("V1_API_GET_OAUTH_REQ_TOKEN", "oauth|v1_api_get_oauth_req_token");
define("V1_API_GET_OAUTH_ACCESS_TOKEN", "oauth|v1_api_get_oauth_access_token");
define("V1_API_GRANT_APP", "oauth|v1_api_grant_app");
define("V1_API_REVOKE_APP", "oauth|v1_api_revoke_app");

define("V1_API_GET_ACCOUNT", "account|v1_api_get_account");
define("V1_API_UPDATE_ACCOUNT", "account|v1_api_update_account");
define("V1_API_GET_LOG", "account|v1_api_get_log");
define("V1_API_GET_APPS", "account|v1_api_get_apps");

define("V1_API_GET_NOTIFICATIONS_SOCKET", "notify|v1_api_get_notifications_socket");
define("V1_API_GET_NOTIFICATIONS", "notify|v1_api_get_notifications");

define("V1_API_GET_HOSTS", "host|v1_api_get_hosts");
define("V1_API_GET_HOST", "host|v1_api_get_host");
define("V1_API_PRINT", "host|v1_api_print");
define("V1_API_CONNECT_HOST_RDP", "host|v1_api_connect_host_rdp");
define("V1_API_CONNECT_HOST_VNC", "host|v1_api_connect_host_vnc");
define("V1_API_CONNECT_HOST_HTTP", "host|v1_api_connect_host_http");
define("V1_API_GET_USB_DEVICES", "host|v1_api_get_usb_devices");
define("V1_API_USB_DEVICE_VIDEO", "host|v1_api_usb_device_video");

define("V1_API_GET_BANDWIDTH", "media|v1_api_get_bandwidth");
define("V1_API_GET_FOLDERS", "media|v1_api_get_folders");
define("V1_API_GET_PATH_INFO", "media|v1_api_get_path_info");
define("V1_API_UPLOAD_FILE", "media|v1_api_upload_file");
define("V1_API_DELETE_FILE", "media|v1_api_delete_file");
define("V1_API_DOWNLOAD_FILE", "media|v1_api_download_file");
define("V1_API_GET_BULK_DOWNLOAD", "media|v1_api_get_bulk_download");
define("V1_API_GET_SLIDESHOW_IMAGES", "media|v1_api_get_slideshow_images");
define("V1_API_GET_MUSIC", "media|v1_api_get_music");
define("V1_API_GET_VIDEO", "media|v1_api_get_video");
define("V1_API_GET_CONVERT_STATUS", "media|v1_api_get_convert_status");
define("V1_API_CONVERT_MEDIA", "media|v1_api_convert_media");
define("V1_API_GET_CONVERTED_MEDIA", "media|v1_api_get_converted_media");
define("V1_API_BACKUP_RESOURCE", "media|v1_api_backup_resource");
define("V1_API_RESTORE_RESOURCE", "media|v1_api_restore_resource");
define("V1_API_REMOVE_BACKUP", "media|v1_api_remove_backup");
define("V1_API_SEARCH", "media|v1_api_search");
define("V1_API_GET_SEARCH_RESULT", "media|v1_api_get_search_result");

define("V1_API_GET_PERMISSIONS", "share|v1_api_get_permissions");
define("V1_API_GRANT_PERMISSIONS", "share|v1_api_grant_permissions");
define("V1_API_GET_PERMISSIONS_FRIENDS", "share|v1_api_get_permissions_friends");
define("V1_API_REVOKE_PERMISSIONS_FRIEND", "share|v1_api_revoke_permissions_friend");
define("V1_API_GET_PERMISSIONS_LINKS", "share|v1_api_get_permissions_links");
define("V1_API_REVOKE_PERMISSIONS_LINK", "share|v1_api_revoke_permissions_link");
define("V1_API_GET_SHARES", "share|v1_api_get_shares");
define("V1_API_GET_FRIEND_SHARES", "share|v1_api_get_friend_shares");

define("V1_API_GET_FRIENDS", "friend|v1_api_get_friends");
define("V1_API_GET_FRIEND", "friend|v1_api_get_friend");
define("V1_API_GET_FRIEND_FRIENDS", "friend|v1_api_get_friend_friends");
define("V1_API_INVITE_FRIEND", "friend|v1_api_invite_friend");
define("V1_API_CONFIRM_FRIEND", "friend|v1_api_confirm_friend");
define("V1_API_REMOVE_FRIEND", "friend|v1_api_remove_friend");

define("V1_API_GET_DOCS", "docs|v1_api_get_docs");

define("V1_API_CHECK_AUTH", "internal|v1_api_check_auth");
define("V1_API_LOGOUT", "internal|v1_api_logout");
define("V1_API_GET_EMAILED_LINK", "internal|v1_api_get_emailed_link");
define("V1_API_CREATE_LINKED_ACCOUNT", "internal|v1_api_create_linked_account");
define("V1_API_CREATE_FS_SHARE", "internal|v1_api_create_fs_share");
define("V1_API_GET_FS_FOLDER", "internal|v1_api_get_fs_folder");
define("V1_API_EXPORT_FS_FOLDER", "internal|v1_api_export_fs_folder");
define("V1_API_UNEXPORT_FS_FOLDER", "internal|v1_api_unexport_fs_folder");
define("V1_API_SHARE_PRINTER", "internal|v1_api_share_printer");

// version 2 example ops (just for test purposes)
define("V2_API_GET_OAUTH", "oauth|v2_api_get_oauth_req_token");
define("V2_API_GET_OAUTH_REQ_TOKEN", "oauth|v2_api_get_oauth_req_token");

$GLOBALS['SCC_API_MODULES'] = array();

/************************************************
 *     API Resource tree
 ***********************************************/

function _api_build_all_versions()
{
	$root = _api_build_v1_tree();
	$root = _api_build_v2_tree();
	
	return $root;
}

function _api_build_v2_tree()
{
	$v = 'v2';
	
	$root_node = API_Resource_Node::updateVersion($v, API_RES_ROOT);
	
	// example: adding a "GET /oauth" for V2 only
	// attach a v2 version of the OAUTH node
	API_Resource_Node::updateVersion($v, API_RES_OAUTH);
		_api_add_handler($v, API_RES_OAUTH, API_VERB_GET, V2_API_GET_OAUTH);

		// example: updating existing "GET /oauth/req_token"
		API_Resource_Node::updateVersion($v, API_RES_REQ_TOKEN);
			_api_update_handler($v, API_RES_REQ_TOKEN, API_VERB_GET, V2_API_GET_OAUTH_REQ_TOKEN);

	// example: remove handler
	API_Resource_Node::updateVersion($v, API_RES_PATH);
		_api_remove_handler($v, API_RES_PATH, API_VERB_DELETE);
	
	// example: unlink ZIP from SEARCH_RESULT
	API_Resource_Node::updateVersion($v, API_RES_SEARCH_RESULT);
	API_Resource_Node::updateVersion($v, API_RES_ZIP);
		API_Resource_Node::unlink($v, API_RES_ZIP, "SEARCH_RESULT");
	
	// example: remove FRIEND and all children
	//API_Resource_Node::updateVersion($v, API_RES_FRIEND);
	//	API_Resource_Node::removeNode($v, API_RES_FRIEND);
	
	return $root_node;
}

function _api_build_v1_tree()
{
	$v = 'v1';
	
	$friend_param = array(
			'name' => FORM_API_FRIEND, 
			'required' => API_PARAM_MANDATORY, 
			'type' => API_PARAM_FRIEND, 
			'description' => "Username of friend, in the form @username, similar to a twitter handle"
	);

	$email_param = array(
			'name' => FORM_API_EMAIL, 
			'required' => API_PARAM_MANDATORY, 
			'type' => API_PARAM_EMAIL, 
			'description' => "A valid email address, as in user@domain.com"
	);

	$app_param = array(
			'name' => FORM_API_APP, 
			'required' => API_PARAM_MANDATORY, 
			'type' => API_PARAM_ALPHANUM,
			'constraints' => array("max_length" => 32), 
			'description' => "Name of the app for granting/revoking access"
	);
	
	$host_param = array(
			'name' => FORM_API_HOST_NAME, 
			'required' => API_PARAM_MANDATORY, 
			'type' => API_PARAM_ALPHANUM,
			'constraints' => array("max_length" => 32), 
			'description' => "Name of network host or device"
	);
	
	$usb_device_param = array(
			'name' => FORM_API_USB_DEVICE, 
			'required' => API_PARAM_MANDATORY, 
			'type' => API_PARAM_STRING,
			'constraints' => array("max_length" => 64), 
			'description' => "Name of USB Device"
	);
	
	$path_param = array(
			'name' => FORM_API_PATH, 
			'required' => API_PARAM_MANDATORY, 
			'type' => API_PARAM_PATH, 
			'description' => "Specifies a unique folder or file. Consists of folder names separated by the slash '/' character, followed optionally by a filename, such as /Documents/wedding/invitation.doc. Paths are terminated by a double slash (//) so that API resources (e.g. 'video' or 'permissions') are not interpreted as folder or file names."
	);

	$search_id_param = array(
			'name' => FORM_API_SEARCH_ID, 
			'required' => API_PARAM_MANDATORY, 
			'type' => API_PARAM_ALPHANUM, 
			'constraints' => array("max_length" => 32), 
			'description' => "A search ID, as returned by GET <host>/search operation"
	);

	$link_id_param = array(
			'name' => FORM_API_LINK_ID, 
			'publish' => API_PRIVATE,
			'required' => API_PARAM_MANDATORY, 
			'type' => API_PARAM_ALPHANUM, 
			'constraints' => array("max_length" => 32), 
			'description' => "A link ID, as part of a generated URL to a resource"
	);

	$root_node = new API_Resource_Node($v, API_RES_ROOT, null, null, null);
	
	new API_Resource_Node($v, API_RES_OAUTH, 'oauth', null, "ROOT");
	_api_add_description($v, API_RES_OAUTH, "OAuth token request operations.");
	
		new API_Resource_Node($v, API_RES_REQ_TOKEN, 'req_token', null, "OAUTH");
			_api_add_handler($v, API_RES_REQ_TOKEN, API_VERB_GET, V1_API_GET_OAUTH_REQ_TOKEN);
		new API_Resource_Node($v, API_RES_ACCESS_TOKEN, 'access_token', null, "OAUTH");
			_api_add_handler($v, API_RES_ACCESS_TOKEN, API_VERB_GET, V1_API_GET_OAUTH_ACCESS_TOKEN);

	new API_Resource_Node($v, API_RES_ME, 'me', null, "ROOT");
	_api_add_description($v, API_RES_ME, "All resources descended from 'me' assume the currently validated user.");
		_api_add_handler($v, API_RES_ME, API_VERB_GET, V1_API_GET_ACCOUNT);
		_api_add_handler($v, API_RES_ME, API_VERB_UPDATE, V1_API_UPDATE_ACCOUNT);
	
		new API_Resource_Node($v, API_RES_LOG, 'log', null, "ME");
			_api_add_handler($v, API_RES_LOG, API_VERB_GET, V1_API_GET_LOG);
		
		new API_Resource_Node($v, API_RES_HOSTS, 'hosts', null, "ME");
		_api_add_description($v, API_RES_HOSTS, "Entry point for all remote resources");
			_api_add_handler($v, API_RES_HOSTS, API_VERB_GET, V1_API_GET_HOSTS);
	
			new API_Resource_Node($v, API_RES_HOST, API_ELEMENT_PARAM, $host_param, "HOSTS");
				_api_add_handler($v, API_RES_HOST, API_VERB_GET, V1_API_GET_HOST);
				_api_add_handler($v, API_RES_HOST, API_VERB_PRINT, V1_API_PRINT);
				_api_add_handler($v, API_RES_HOST, API_VERB_SHARE, V1_API_SHARE_PRINTER);

				new API_Resource_Node($v, API_RES_RDP, 'rdp', null, "HOST");
					_api_add_handler($v, API_RES_RDP, API_VERB_GET, V1_API_CONNECT_HOST_RDP);

					new API_Resource_Node($v, API_RES_PERMISSIONS, 'permissions', null, "RDP");
					_api_add_description($v, API_RES_PERMISSIONS, "Manage permissions to the specified resource");
						_api_add_handler($v, API_RES_PERMISSIONS, API_VERB_GET, V1_API_GET_PERMISSIONS);
						_api_add_handler($v, API_RES_PERMISSIONS, API_VERB_GRANT, V1_API_GRANT_PERMISSIONS);
						
						new API_Resource_Node($v, API_RES_PERM_FRIENDS, 'friends', null, "PERMISSIONS");
							_api_add_handler($v, API_RES_PERM_FRIENDS, API_VERB_GET, V1_API_GET_PERMISSIONS_FRIENDS);
							new API_Resource_Node($v, API_RES_PERM_FRIEND, API_ELEMENT_PARAM, $friend_param, "PERM_FRIENDS");
								_api_add_handler($v, API_RES_PERM_FRIEND, API_VERB_REVOKE, V1_API_REVOKE_PERMISSIONS_FRIEND);
							
						new API_Resource_Node($v, API_RES_PERM_LINKS, 'emailed_links', null, "PERMISSIONS");
							_api_add_handler($v, API_RES_PERM_LINKS, API_VERB_GET, V1_API_GET_PERMISSIONS_LINKS);
							new API_Resource_Node($v, API_RES_PERM_LINK, API_ELEMENT_PARAM, $email_param, "PERM_LINKS");
								_api_add_handler($v, API_RES_PERM_LINK, API_VERB_REVOKE, V1_API_REVOKE_PERMISSIONS_LINK);
				
				new API_Resource_Node($v, API_RES_VNC, 'vnc', null, "HOST");
					_api_add_handler($v, API_RES_VNC, API_VERB_GET, V1_API_CONNECT_HOST_VNC);
					API_Resource_Node::link($v, API_RES_PERMISSIONS, "VNC");
					
				new API_Resource_Node($v, API_RES_HTTP, 'http', null, "HOST");
					_api_add_handler($v, API_RES_HTTP, API_VERB_GET, V1_API_CONNECT_HOST_HTTP);
					API_Resource_Node::link($v, API_RES_PERMISSIONS, "HTTP");
				
				new API_Resource_Node($v, API_RES_USB_DEVICES, 'usb_devices', null, "HOST");
					_api_add_handler($v, API_RES_USB_DEVICES, API_VERB_GET, V1_API_GET_USB_DEVICES);

					new API_Resource_Node($v, API_RES_USB_DEVICE, API_ELEMENT_PARAM, $usb_device_param, "USB_DEVICES");
						new API_Resource_Node($v, API_RES_USB_VIDEO, 'video', null, "USB_DEVICE");
							API_Resource_Node::link($v, API_RES_PERMISSIONS, API_RES_USB_VIDEO);
							_api_add_handler($v, API_RES_USB_VIDEO, API_VERB_GET, V1_API_USB_DEVICE_VIDEO);

				new API_Resource_Node($v, API_RES_BANDWIDTH, 'bandwidth', null, "HOST");
					_api_add_handler($v, API_RES_BANDWIDTH, API_VERB_GET, V1_API_GET_BANDWIDTH);
				
				new API_Resource_Node($v, API_RES_FOLDERS, 'folders', null, "HOST");
				_api_add_description($v, API_RES_FOLDERS, "Entry point for all folder and file based resources");
					_api_add_handler($v, API_RES_FOLDERS, API_VERB_GET, V1_API_GET_FOLDERS);
					
					new API_Resource_Node($v, API_RES_PATH, API_ELEMENT_PATH_PARAM, $path_param, "FOLDERS");
						_api_add_handler($v, API_RES_PATH, API_VERB_GET, V1_API_GET_PATH_INFO);
						_api_add_handler($v, API_RES_PATH, API_VERB_UPLOAD, V1_API_UPLOAD_FILE);
						_api_add_handler($v, API_RES_PATH, API_VERB_DELETE, V1_API_DELETE_FILE);

						API_Resource_Node::link($v, API_RES_PERMISSIONS, "PATH");

						new API_Resource_Node($v, API_RES_DOWNLOAD, 'download', null, "PATH");
							API_Resource_Node::link($v, API_RES_PERMISSIONS, "DOWNLOAD");
							_api_add_handler($v, API_RES_DOWNLOAD, API_VERB_GET, V1_API_DOWNLOAD_FILE);
						
						new API_Resource_Node($v, API_RES_ZIP, 'zip', null, "PATH");
							_api_add_handler($v, API_RES_ZIP, API_VERB_GET, V1_API_GET_BULK_DOWNLOAD);

						new API_Resource_Node($v, API_RES_SLIDESHOW, 'slideshow', null, "PATH");
							API_Resource_Node::link($v, API_RES_PERMISSIONS, "SLIDESHOW");
							_api_add_handler($v, API_RES_SLIDESHOW, API_VERB_GET, V1_API_GET_SLIDESHOW_IMAGES);

						new API_Resource_Node($v, API_RES_MUSIC, 'music', null, "PATH");
							API_Resource_Node::link($v, API_RES_PERMISSIONS, "MUSIC");
							_api_add_handler($v, API_RES_MUSIC, API_VERB_GET, V1_API_GET_MUSIC);

						new API_Resource_Node($v, API_RES_VIDEO, 'video', null, "PATH");
							API_Resource_Node::link($v, API_RES_PERMISSIONS, "VIDEO");
							_api_add_handler($v, API_RES_VIDEO, API_VERB_GET, V1_API_GET_VIDEO);

						new API_Resource_Node($v, API_RES_CONVERT, 'convert', null, "PATH");
							_api_add_handler($v, API_RES_CONVERT, API_VERB_GET, V1_API_GET_CONVERT_STATUS);
							_api_add_handler($v, API_RES_CONVERT, API_VERB_CREATE, V1_API_CONVERT_MEDIA);
							new API_Resource_Node($v, API_RES_CONVERT_FILE, 'file', null, "CONVERT");
								_api_add_handler($v, API_RES_CONVERT_FILE, API_VERB_GET, V1_API_GET_CONVERTED_MEDIA);

						new API_Resource_Node($v, API_RES_BACKUP, 'backup', null, "PATH", API_NOT_PUBLISHED);
							_api_add_handler($v, API_RES_BACKUP, API_VERB_CREATE, V1_API_BACKUP_RESOURCE);
							_api_add_handler($v, API_RES_BACKUP, API_VERB_RESTORE, V1_API_RESTORE_RESOURCE);
							_api_add_handler($v, API_RES_BACKUP, API_VERB_REMOVE, V1_API_REMOVE_BACKUP);
						
				new API_Resource_Node($v, API_RES_SEARCH, 'search', null, "HOST");
					_api_add_handler($v, API_RES_SEARCH, API_VERB_GET, V1_API_SEARCH);
					
					new API_Resource_Node($v, API_RES_SEARCH_RESULT, API_ELEMENT_PARAM, $search_id_param, "SEARCH");
						_api_add_handler($v, API_RES_SEARCH_RESULT, API_VERB_GET, V1_API_GET_SEARCH_RESULT);
						API_Resource_Node::link($v, API_RES_SLIDESHOW, "SEARCH_RESULT");
						API_Resource_Node::link($v, API_RES_MUSIC, "SEARCH_RESULT");
						API_Resource_Node::link($v, API_RES_ZIP, "SEARCH_RESULT");
						
				API_Resource_Node::link($v, API_RES_SEARCH, "PATH");
	
		new API_Resource_Node($v, API_RES_FRIENDS, 'friends', null, "ME");
			_api_add_handler($v, API_RES_FRIENDS, API_VERB_GET, V1_API_GET_FRIENDS);
			_api_add_handler($v, API_RES_FRIENDS, API_VERB_INVITE, V1_API_INVITE_FRIEND);
			
		new API_Resource_Node($v, API_RES_SHARES, 'shares', null, "ME");
			_api_add_handler($v, API_RES_SHARES, API_VERB_GET, V1_API_GET_SHARES);
		
		new API_Resource_Node($v, API_RES_NOTIFICATIONS, 'notifications', null, "ME");
		_api_add_description($v, API_RES_NOTIFICATIONS, "Entry point for getting real-time notifications about friend actions and resource updates");
			new API_Resource_Node($v, API_RES_NOTIFICATIONS_SOCKET, 'socket', null, "NOTIFICATIONS");
				_api_add_handler($v, API_RES_NOTIFICATIONS_SOCKET, API_VERB_GET, V1_API_GET_NOTIFICATIONS_SOCKET);
			new API_Resource_Node($v, API_RES_NOTIFICATIONS_POLL, 'poll', null, "NOTIFICATIONS");
				_api_add_handler($v, API_RES_NOTIFICATIONS_POLL, API_VERB_GET, V1_API_GET_NOTIFICATIONS);

	new API_Resource_Node($v, API_RES_FRIEND, API_ELEMENT_PARAM, $friend_param, "ROOT");
		_api_add_handler($v, API_RES_FRIEND, API_VERB_GET, V1_API_GET_FRIEND);
		_api_add_handler($v, API_RES_FRIEND, API_VERB_INVITE, V1_API_INVITE_FRIEND);
		_api_add_handler($v, API_RES_FRIEND, API_VERB_CONFIRM, V1_API_CONFIRM_FRIEND);
		_api_add_handler($v, API_RES_FRIEND, API_VERB_REMOVE, V1_API_REMOVE_FRIEND);
		
		API_Resource_Node::link($v, API_RES_HOSTS, "FRIEND"); // provide access to shared resources
		
		new API_Resource_Node($v, API_RES_FRIEND_FRIENDS, 'friends', null, "FRIEND");
			_api_add_handler($v, API_RES_FRIEND_FRIENDS, API_VERB_GET, V1_API_GET_FRIEND_FRIENDS);
			
		new API_Resource_Node($v, API_RES_FRIEND_SHARES, 'shares', null, "FRIEND");
			_api_add_handler($v, API_RES_FRIEND_SHARES, API_VERB_GET, V1_API_GET_FRIEND_SHARES);
		
	// Private API calls - requires shared secret, must originate from our servers
	
	new API_Resource_Node($v, API_RES_DOCS, 'docs', null, "ROOT", API_PRIVATE); // generate docs JSON
		_api_add_handler($v, API_RES_DOCS, API_VERB_GET, V1_API_GET_DOCS);

	new API_Resource_Node($v, API_RES_AUTH, 'auth', null, "ROOT", API_PRIVATE); // user/password auth from web apps
		_api_add_handler($v, API_RES_AUTH, API_VERB_LOGIN, V1_API_CHECK_AUTH);
		_api_add_handler($v, API_RES_AUTH, API_VERB_LOGOUT, V1_API_LOGOUT);

	new API_Resource_Node($v, API_RES_LINKS, 'links', null, "ROOT", API_PRIVATE); 
		new API_Resource_Node($v, API_RES_LINK, API_ELEMENT_PARAM, $link_id_param, "LINKS", API_PRIVATE); // for processing emailed links
			_api_add_handler($v, API_RES_LINK, API_VERB_GET, V1_API_GET_EMAILED_LINK);

	new API_Resource_Node($v, API_RES_ACCOUNTS, 'accounts', null, "ROOT", API_PRIVATE);
		_api_add_handler($v, API_RES_ACCOUNTS, API_VERB_CREATE, V1_API_CREATE_LINKED_ACCOUNT);

	new API_Resource_Node($v, API_RES_FILE_SYS, 'fs', null, "HOST", API_PRIVATE);
		new API_Resource_Node($v, API_RES_FS_PATH, API_ELEMENT_PARAM, $path_param, "FILE_SYS", API_PRIVATE);
			_api_add_handler($v, API_RES_FS_PATH, API_VERB_SHARE, V1_API_CREATE_FS_SHARE);
			_api_add_handler($v, API_RES_FS_PATH, API_VERB_GET, V1_API_GET_FS_FOLDER);
			_api_add_handler($v, API_RES_FS_PATH, API_VERB_EXPORT, V1_API_EXPORT_FS_FOLDER);
			_api_add_handler($v, API_RES_FS_PATH, API_VERB_UNEXPORT, V1_API_UNEXPORT_FS_FOLDER);

	new API_Resource_Node($v, API_RES_APPS, 'apps', null, "ME", API_PRIVATE);
		_api_add_handler($v, API_RES_APPS, API_VERB_GET, V1_API_GET_APPS);
		
		new API_Resource_Node($v, API_RES_APP, API_ELEMENT_PARAM, $app_param, "APPS", API_PRIVATE);
		_api_add_description($v, API_RES_APP, "Manage OAuth tokens for specific apps");
			_api_add_handler($v, API_RES_APP, API_VERB_GRANT, V1_API_GRANT_APP);
			_api_add_handler($v, API_RES_APP, API_VERB_REVOKE, V1_API_REVOKE_APP);

	return $root_node;
}


function _api_add_description($version, $node_id, $desc)
{
	if (!($node = API_Resource_Node::checkLatestVersion($node_id, $version)) )
		throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "adding/updating description to old version of node [$node_id]");
	
	$node->addDescription($desc);
}

function _api_add_handler($version, $node_id, $verb, $func)
{
	if (!($node = API_Resource_Node::checkLatestVersion($node_id, $version)) )
		throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "adding [$verb] handler to old version of node [$node_id]");
	
	$node->addHandler($verb, $func);
}

function _api_update_handler($version, $node_id, $verb, $func)
{
	if (!($node = API_Resource_Node::checkLatestVersion($node_id, $version)) )
		throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "updating [$verb] handler with old version of node [$node_id]");
	
	$node->updateHandler($verb, $func);
}

function _api_remove_handler($version, $node_id, $verb)
{
	if (!($node = API_Resource_Node::checkLatestVersion($node_id, $version)) )
		throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "adding [$verb] handler to old version of node [$node_id]");
	
	$node->removeHandler($verb);
}

