<?php

// 
// operation.inc - centralized code for validating requested user operations
//
// All input data should be subjected to the following:
//  - perform syntactic checks on all input data like making sure numbers are numbers and emails are emails,
//    sanitizing input where appropriate
//  - perform business-logic checks, like making sure referenced objects exist, and that the current user
//    is authorized to perform the operation
//
// In some cases a GUI or API operation may instantiate more than one op_check, if the op is
// a compound operation. 
//
// The point of the code in this file is to improve security. By creating a systematic and
// centralized means for checking user input in various ways, we make it much harder
// to ignore important security checks. 
//
// Furthermore, the systematic approach to discovering errored input allows for much more 
// consistent and user friendly error reporting.
//
// The attacks addressed by this scheme are XSS, injection, and Authorization attacks.
// The "common security programming errors" addressed by this file are referenced here:
// http://cwe.mitre.org/top25/#CWE-79
// http://cwe.mitre.org/top25/#CWE-89
// http://cwe.mitre.org/top25/#CWE-285
// http://cwe.mitre.org/top25/#CWE-807
// http://cwe.mitre.org/top25/#CWE-78
// http://cwe.mitre.org/top25/#CWE-754
// http://cwe.mitre.org/top25/#CWE-209
// http://cwe.mitre.org/top25/#CWE-306
// 

// Classes defined in this file should have the form:
//
// SCC_OPC_<VERB>_<NOUN>
// 
// NOTE: if adding new operations, make sure it hasn't already been implemented first!
// Also, look and see if other ops exist with the Noun you're working with.
//
// Try to be consistent with other VERBs and NOUNs.

define('API_PARAM_OPTIONAL', 0);
define('API_PARAM_MANDATORY', 1);

define("API_PARAM_ERR_MISSING", 1);
define("API_PARAM_ERR_INVALID", 2);
define("API_PARAM_ERR_CUSTOM", 3);

define("API_PARAM_DEFAULT_MAX_LEN", 256);
define("API_PARAM_MAX_PATH_LEN", 512);
define("API_PARAM_PASSWORD_MAX_LEN", 64); // sync with db

define("API_PARAM_STRING", "string");
define("API_PARAM_ALPHANUM", "alphanum");
define("API_PARAM_INTEGER", "integer");
define("API_PARAM_LARGE_INTEGER", "large_integer");
define("API_PARAM_BOOLEAN", "boolean");
define("API_PARAM_ENUM", "enum");
define("API_PARAM_EMAIL", "email");
define("API_PARAM_EMAIL_LIST", "email_list");
define("API_PARAM_FRIEND", "friend");
define("API_PARAM_FRIEND_LIST", "friend_list");
define("API_PARAM_PATH", "path");
define("API_PARAM_IP_ADDRESS", "ip_address");
define("API_PARAM_MAC_ADDRESS", "mac_address");
define("API_PARAM_PASSWORD", "password");

class API_Operation
{
	static $node_stack = array();
	static $url_params = array();
	static $version = null;
	static $docs_mode = null;
	
	var $publish;
	var $op_check;
	var $description;
	var $node;
	var $verb;
	
	function __construct($op_check, $description, $publish = API_PUBLIC)
	{
		$this->op_check = $op_check;
		$this->description = $description;
		$this->publish = $publish;
	}
	
	function setNode($node)
	{
		$this->node = $node;
	}
	
	function setVerb($verb)
	{
		$this->verb = $verb;
	}
	
	function go()
	{
		if (self::$docs_mode)
		{
			return self::$docs_mode->outputOperationDocs($this);
		}
		else
		{
			// do operation checks
			$this->op_check->run(self::$url_params);
			
			// do we need to proxy this request?
			if (is_media_server())
			{
				// we need to proxy if:
				// 1) requesting a user's resource and the user is not this media server's master user
				// 2) requesting a non-media-server resource
				if (false) //!$this->op_check->getResourceOwner() || !$this->op_check->isDSLService($this->op_check->getService()))
				{
					_api_translate_output(api_proxy_portal_request());
					return;
				}
			}
			else
			{
				// we need to proxy if:
				// 1) requesting a media-server resource
				if ($this->op_check->isDSLService($this->op_check->getService()))
				{
					_api_translate_output(api_proxy_dsl_request($this->op_check->getResourceOwner()));
					return;
				}
			}
			
			// run the operation
			return $this->run($this->op_check->sanitized_params);
		}
	}
	
	function run($params)
	{
		throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "run() not implemented in [" . get_class($this) . "]");
	}
	
	static function set_docs_mode($docs_op)
	{
		self::$docs_mode = $docs_op;
	}
	
	static function push_node($node)
	{
		array_push(self::$node_stack, $node);
	}

	static function pop_node()
	{
		return array_pop(self::$node_stack);
	}

	static function add_url_param($param_name, $param)
	{
		self::$url_params[$param_name] = $param;
	}

	static function remove_url_param($param_name)
	{
		unset(self::$url_params[$param_name]);
	}
}


class SCC_Operation_Check
{
	var $api_operation = null;
	
	var $param_definitions = array();

	var $url_params = array();
	var $params = array();
	
	var $sanitized = false;
	var $sanitized_params = array();

	var $resource_owner;
	var $network_info;
	var $host;
	var $path;
	var $service;
	
	function __construct($api_operation)
	{
		$this->api_operation = $api_operation;

		// collect URL param definitions
		foreach (API_Operation::$node_stack as $node)
		{
			if ($node->param_definition)
			{
				$node->param_definition['url_param'] = true;
				$node->param_definition['required'] = true;
				$this->define_param($node->param_definition);
			}
		}

		$this->define_param(array(
			'name' => 'debug', 
			'publish' => API_NOT_PUBLISHED,
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_STRING, 
			'description' => "Give a little extra information to the API call")
		);		
	}
	
	function run($url_params)
	{
		// skip param validation in docs mode
		if (!API_Operation::$docs_mode)
		{
			$this->prepareParams($url_params);

			$this->validateParams();
		}

		// do object existence/non-existence checks
		//  - throws exceptions for invalid cases
		$this->doExistenceChecks();
		
		// make sure user has authorization for requested operation
		//  - throws exceptions for invalid cases
		$this->doAuthorizationChecks();
		
		// make sure the operation is allowed in terms of the target account's status
		$this->doAccountStatusChecks();
		
		// other checks, such as checking network/host status before connect attempts
		// optional, does not need to be implemented
		$this->doOtherChecks();
	}

	function doExistenceChecks()
	{
		throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Operation [" . get_class($this) . "]: Unimplemented doExistenceChecks()"); 
	}

	function doAuthorizationChecks()
	{
		throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Operation [" . get_class($this) . "]: Unimplemented doAuthorizationChecks()"); 
	}
	
	function doAccountStatusChecks()
	{
		throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Operation [" . get_class($this) . "]: Unimplemented doAccountStatusChecks()"); 
	}
	
	function doOtherChecks()
	{
		// optional function, no need to throw an exception here
	}
	
	////////////////////////////
	//
	// node functions
	//
	////////////////////////////

	function getActiveNode()
	{
		return end(API_Operation::$node_stack);
	}
	
	function isSearchNode()
	{
		foreach (API_Operation::$node_stack as $node)
		{
			if ($node->node_id == "SEARCH")
				return true;
		}
		
		return false;
	}
	
	function isMe()
	{
		foreach (API_Operation::$node_stack as $node)
		{
			if ($node->node_id == "ME")
				return true;
		}
		
		return false;
	}

	
	function getNodeService($node)
	{
		$service = null;
		
		switch ($node->node_id)
		{
			case "RDP":
				$service = SCC_SERVICE_RDP;
				break;
				
			case "VNC":
				$service = SCC_SERVICE_VNC;
				break;

			case "HTTP":
				$service = SCC_SERVICE_HTTP;
				break;
				
			case "USB_DEVICE":
				$service = SCC_SERVICE_USB_DEVICE;
				break;
				
			case "PATH":
				$service = SCC_SERVICE_DSL;
				break;
				
			case "DOWNLOAD":
				$service = SCC_SERVICE_FILE;
				break;
				
			case "ZIP":
				$service = $this->isSearchNode()?SCC_SERVICE_SEARCH_ZIP:SCC_SERVICE_ZIP;
				break;
				
			case "SLIDESHOW":
				$service = $this->isSearchNode()?SCC_SERVICE_SEARCH_SLIDESHOW:SCC_SERVICE_SLIDESHOW;
				break;
				
			case "MUSIC":
				$service = $this->isSearchNode()?SCC_SERVICE_SEARCH_MUSIC:SCC_SERVICE_MUSIC;
				break;
				
			case "VIDEO":
				$service = $this->isSearchNode()?SCC_SERVICE_SEARCH_VIDEO:SCC_SERVICE_VIDEO;
				break;
		}
		
		return $service;
	}

	function getService()
	{
		if ($this->service)
			return $this->service;
			
		for ($n=count(API_Operation::$node_stack)-1; $n >= 0; $n--)
		{
			if (($this->service = $this->getNodeService(API_Operation::$node_stack[$n])) )
				break;
		}

		return $this->service;
	}
	
	function isDSLService($service)
	{
		switch ($service)
		{
			case SCC_SERVICE_DSL:
			case SCC_SERVICE_FILE:
			case SCC_SERVICE_ZIP:
			case SCC_SERVICE_SLIDESHOW:
			case SCC_SERVICE_MUSIC:
			case SCC_SERVICE_VIDEO:
			case SCC_SERVICE_SEARCH_ZIP:
			case SCC_SERVICE_SEARCH_SLIDESHOW:
			case SCC_SERVICE_SEARCH_MUSIC:
			case SCC_SERVICE_SEARCH_VIDEO:
				return true;
		}
		
		return false;
	}
	

	////////////////////////////
	//
	// param functions
	//
	////////////////////////////
	
	function define_param($param_def)
	{
		if (isset($this->param_definitions[$param_def['name']]))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "param [" . $param_def['name'] . "] already defined in [" . get_class($this) . "]");
		
		switch ($param_def['type'])
		{
			case API_PARAM_ENUM:
				if (!isset($param_def['constraints']))
					throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "enum param [" . $param_def['name'] . "] requires definition of allowed keywords in constraints");
					
				if (isset($param_def['constraints'][0]))
				{
					$constraints = array();
					foreach ($param_def['constraints'] as $keyword)
						$constraints[$keyword] = $keyword;
					
					$param_def['constraints'] = $constraints;
				}
				break;

			case API_PARAM_INTEGER:
			case API_PARAM_LARGE_INTEGER:
			case API_PARAM_ALPHANUM:
			case API_PARAM_STRING:
			case API_PARAM_BOOLEAN:
			case API_PARAM_EMAIL:
			case API_PARAM_EMAIL_LIST:
			case API_PARAM_FRIEND:
			case API_PARAM_FRIEND_LIST:
			case API_PARAM_PATH:
			case API_PARAM_IP_ADDRESS:
			case API_PARAM_MAC_ADDRESS:
			case API_PARAM_PASSWORD:
				// no mandatory constraints to check
				break;
			
			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "param [" . $param_def['name'] . "]: unknown type [" . $param_def['type'] . "]");
		}
			
		if (!isset($param_def['description']))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "param [" . $param_def['name'] . "] missing description");
			
		if (!isset($param_def['publish']))
			$param_def['publish'] = API_PUBLIC;
			
		$this->param_definitions[] = $param_def;
	}
	
	function prepareParams($url_params)
	{
		if ($url_params)
			$this->url_params = $url_params;

		if ($_SERVER['REQUEST_METHOD'] == "GET")
			$this->params = array_merge($_GET, $this->url_params);
		else
		{
			$req_headers = getallheaders();
			if (strstr($req_headers['Content-Type'], "application/json"))
			{
				$json = file_get_contents("php://input");
			}
			else if (isset($_POST[FORM_API_JSON_REQ]))
			{
				$json = $_POST[FORM_API_JSON_REQ];
			}
			
			if ($json)
			{
				$json_params = json_decode($json, true);
				
				if (!$json_params)
					throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Could not parse JSON request data");
					
				$this->params = array_merge($json_params, $this->url_params);
			}
			else
			{
				$this->params = array_merge($_POST, $this->url_params);
			}
		}
	}

	function validateParams()
	{
		foreach ($this->param_definitions as $param_def)
		{
			$name = $param_def['name'];
			$param = $this->params[$name];
			$value = null;
			
			if ($param_def['required'] && !isset($this->params[$name]))
				throw new SCC_API_Bad_Request_Exception("PARAM_MISSING", "Mandatory parameter [$name] missing");
				
			if (isset($this->params[$name]))
			{
				if ($param === '' && !$param_def['empty_string_ok'])
				{
					if ($param_def['required'])
						throw new SCC_API_Bad_Request_Exception("PARAM_MISSING", "Mandatory parameter [$name] missing");
				}
				else switch ($param_def['type'])
				{
					case API_PARAM_ENUM:
						$value = $this->validateEnum($param_def, $param);
						break;

					case API_PARAM_INTEGER:
						$value = $this->validateInteger($param_def, $param);
						break;					

					case API_PARAM_LARGE_INTEGER:
						$value = $this->validateLargeInteger($param_def, $param);
						break;					

					case API_PARAM_ALPHANUM:
						$value = $this->validateAlphaNumeric($param_def, $param);
						break;					

					case API_PARAM_STRING:
						$value = $this->sanitizeText($param_def, $param);
						break;					

					case API_PARAM_BOOLEAN:
						$value = $this->validateBoolean($param_def, $param);
						break;					

					case API_PARAM_EMAIL:
						$value = $this->validateEmail($param_def, $param);
						break;					

					case API_PARAM_EMAIL_LIST:
						$value = $this->validateEmailList($param_def, $param);
						break;					

					case API_PARAM_FRIEND:
						$value = $this->validateFriend($param_def, $param);
						break;					

					case API_PARAM_FRIEND_LIST:
						$value = $this->validateFriendList($param_def, $param);
						break;					

					case API_PARAM_PATH:
						$value = $this->validatePath($param_def, $param);
						break;					

					case API_PARAM_IP_ADDRESS:
						$value = $this->validateIPAddr($param_def, $param);
						break;					

					case API_PARAM_MAC_ADDRESS:
						$value = $this->validateMACAddr($param_def, $param);
						break;

					case API_PARAM_PASSWORD:
						$value = $this->validatePassword($param_def, $param);
						break;
				}
			}
			
			// set default value if defined
			if ($value === null && isset($param_def['default']))
				$value = $param_def['default'];
			
			if ($value !== null)
				$this->sanitized_params[$name] = $value;
		}
	}
	
	function getParam($name)
	{
		return isset($this->sanitized_params[$name])?$this->sanitized_params[$name]:null;
	}
	
	////////////////////////////////////////////////////////
	//
	// Param format validation checks & sanitization
	//
	////////////////////////////////////////////////////////
	
	function validateInteger($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		if (($param = filter_var($param, FILTER_VALIDATE_INT) === false))
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must be a number");

		$constraints = $param_def['constraints'];
		
		$low = ($constraints && isset($constraints['low']))?$constraints['low']:null;
		$high = ($constraints && isset($constraints['high']))?$constraints['high']:null;
			
		if ($low !== null && $param < (int) $low)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] out of range");

		if ($high !== null && $param > (int) $high)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] out of range");
			
		return $param;
	}

	function validateBoolean($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		$fail = false;
		
		if ( ($num = filter_var($param, FILTER_VALIDATE_INT)) !== false)
		{
			if ($num != 0 && $num != 1)
				$fail = true;
		}
		else switch ($param)
		{
			case 'true':
			case 'yes':
			case 'on':
				$num = 1;
				break;
				
			case 'false':
			case 'no':
			case 'off':
				$num = 0;
				break;
				
			default:
				$fail = true;
		}

		if ($fail)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Boolean parameter [$param_name] must have one of the following values: [0|1|true|false|yes|no|on|off]");

		return $num;
	}

	// validate numbers that may exceed PHP_INT_MAX
	function validateLargeInteger($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		if (!preg_match("/^-?\d+$/", $param))
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must be a number");
		
		$constraints = $param_def['constraints'];

		$low = ($constraints && isset($constraints['low']))?$constraints['low']:null;
		$high = ($constraints && isset($constraints['high']))?$constraints['high']:null;

		if ($low !== null && gmp_cmp($param, $low) == -1)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] out of range");

		if ($high !== null && gmp_cmp($param, $high) == 1)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] out of range");
		
		return $param;
	}
	
	function validateEnum($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		$matched = null;
		$keys_str = '';
		
		foreach ($param_def['constraints'] as $key => $values)
		{
			$keys_str .= "$key|";
			if ($param == $key)
				$matched = isset($values['value'])?$values['value']:$key;
		}
		
		if ($matched === null)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must be one of [" . substr($keys_str, 0, -1) . "]");
				
		return $matched;
	}
	
	function validateAlphaNumeric($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		if (!preg_match("/^[0-9a-zA-Z_]+$/", $param))
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must consist only of numbers, letters, and the underscore character '_' - " . var_export($this->url_params, true));
		
		$constraints = $param_def['constraints'];
		$max_len = ($constraints && isset($constraints['max_length']))?$constraints['max_length']:API_PARAM_DEFAULT_MAX_LEN;
		
		if (strlen($param) > $max_len)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must not exceed $max_len characters");
		
		return $param;
	}

	function validatePassword($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		if (strlen($param) < 8 || !preg_match("/[0-9]/", $param) || !preg_match("/[`~!@#$%^&*()-_=+\[{\]}|\\\\;:'\",<.>\/?]/", $param))
			throw new SCC_API_Bad_Request_Exception("WEAK_PASSWORD", "Password must be at least 8 characters long, contain at least one number, and at least one special character");
		
		$constraints = $param_def['constraints'];
		$max_len = ($constraints && isset($constraints['max_length']))?$constraints['max_length']:API_PARAM_PASSWORD_MAX_LEN;
		
		if (strlen($param) > $max_len)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must not exceed $max_len characters");
		
		return $param;
	}

	function validateIPAddr($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		if (!preg_match("/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/", $param))
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] is not a valid IP address");

		return $param;
	}

	function validateMACAddr($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		if (!preg_match("/^[0-9a-fA-F]{2}(:[0-9a-fA-F]{2}){5}$/", $param))
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] is not a valid MAC address");

		return $param;
	}

	function validateEmail($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		$constraints = $param_def['constraints'];
		$max_len = ($constraints && isset($constraints['max_length']))?$constraints['max_length']:API_PARAM_DEFAULT_MAX_LEN;
		
		if (strlen($param) > $max_len)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must not exceed $max_len characters");

		if (!preg_match("/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/", trim(strtolower($param))))
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must be a valid email address");

		return $param;
	}

	// returns array of email addresses
	function validateEmailList($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		$constraints = $param_def['constraints'];
		$max_len = ($constraints && isset($constraints['max_length']))?$constraints['max_length']:API_PARAM_DEFAULT_MAX_LEN;
		
		if (strlen($param) > $max_len)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must not exceed $max_len characters");

		$valid = array();

		foreach (split('[,;]', $param) as $email)
		{
			if (!preg_match("/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/", trim(strtolower($email))))
				throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must be a list of valid email addresses - [" . $this->cleanInputXSS($email) . "] is invalid.");
			else
				$valid[] = trim($email);
		}

		return $valid;
	}

	function validateFriend($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		$constraints = $param_def['constraints'];
		$max_len = ($constraints && isset($constraints['max_length']))?$constraints['max_length']:API_PARAM_DEFAULT_MAX_LEN;
		
		if (strlen($param) > $max_len)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must not exceed $max_len characters");

		if (preg_match("/^@[0-9 _a-zA-Z]+$/", $param))
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must take the form @username, where username consists only of letters, numbers, underscore (_), and spaces.");

		return $param;
	}
	
	// returns array of usernames
	function validateFriendList($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		$constraints = $param_def['constraints'];
		$max_len = ($constraints && isset($constraints['max_length']))?$constraints['max_length']:API_PARAM_DEFAULT_MAX_LEN;
		
		if (strlen($param) > $max_len)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must not exceed $max_len characters");

		$valid = array();

		foreach (split('[,;]', $param) as $friend)
		{
		if (preg_match("/^@[0-9 _a-zA-Z]+$/", trim($friend)))
				throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must be a list of valid usernames, of the form @username - [" . $this->cleanInputXSS($friend) . "] is invalid.");
			else
				$valid[] = substr(trim($friend, 1));
		}

		return $valid;
	}

	// prevent injection and path traversal attacks (like abc/../../../sensitive/dir)
	function validatePath($param_def, $param)
	{
		$param_name = $param_def['name'];

		// check for '..', '../rest/of/path', 'start/of/path/../rest', 'path/..'
		if ($param == '..' || preg_match('#^\.{2}/#', $param) || preg_match('#/\.{2}/#', $param) || preg_match('#/\.{2}$#', $param)) 
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] is not a valid folder or file name");

		// fs_name(/fs_name)*/?
		// where fs_name is [^/:\*\?<>\|\\\\]+  - i.e. valid filenames don't have the following chars: /:*?<>|\
		if (!preg_match('#^[^/:\*\?<>\|\\\\]+(/[^/:\*\?<>\|\\\\]+)*/?$#', $param))
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] is not a valid folder or file name");
		
		// note: if path has UTF8 chars strlen will overestimate the length
		// they fix this in PHP 5.3
		if (strlen($param) >= API_PARAM_MAX_PATH_LEN)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] is not a valid folder or file name (exceeds " . API_PARAM_MAX_PATH_LEN . " characters)");

		return $param;
	}
		
	// doesn't throw an exception since this is an optional value to pass to tc/tr params
	function checkForUnoptValue($param_def, $param)
	{
		return (strtolower($param) == 'unopt');
	}
	
	function sanitizeText($param_def, $param)
	{
		$param_name = $param_def['name'];
		
		$constraints = $param_def['constraints'];
		$max_len = ($constraints && isset($constraints['max_length']))?$constraints['max_length']:API_PARAM_DEFAULT_MAX_LEN;
		
		if (strlen($param) > $max_len)
			throw new SCC_API_Bad_Request_Exception("INVALID_PARAM", "Parameter [$param_name] must not exceed $max_len characters");

		$sanitized = $this->cleanInputXSS($param);
		
		return $sanitized;
	}
	
	function sanitizeSearchKeywords($param_def, $param)
	{
		$sanitized = $this->sanitizeText($param_def, $param);
		
		// clean up the query
		$terms = preg_split("/\s+/", $sanitized);
		$cleaned = array();
		
		foreach ($terms as $term)
			if ( ($term = preg_replace('/^\W+/', '', trim($term))) )
				$cleaned[] = $term;	
		
		$cleaned_str = implode(' ', $cleaned);
		
		return $cleaned_str;
	}

	function cleanInputXSS($string) 
	{
		$string = str_replace(array("&amp;","&lt;","&gt;"),array("&amp;amp;","&amp;lt;","&amp;gt;"),$string);
		// fix &entitiy\n;
		$string = preg_replace('#(&\#*\w+)[\x00-\x20]+;#u',"$1;",$string);
		$string = preg_replace('#(&\#x*)([0-9A-F]+);*#iu',"$1$2;",$string);
		$string = html_entity_decode($string, ENT_COMPAT, "UTF-8");

		// remove any attribute starting with "on" or xmlns
		$string = preg_replace('#(<[^>]+[\x00-\x20\"\'\/])(on|xmlns)[^>]*>#iUu', "$1>", $string);

		// remove javascript: and vbscript: protocol
		$string = preg_replace('#([a-z]*)[\x00-\x20\/]*=[\x00-\x20\/]*([\`\'\"]*)[\x00-\x20\/]*j[\x00-\x20]*a[\x00-\x20]*v[\x00-\x20]*a[\x00-\x20]*s[\x00-\x20]*c[\x00-\x20]*r[\x00-\x20]*i[\x00-\x20]*p[\x00-\x20]*t[\x00-\x20]*:#iUu', '$1=$2nojavascript...', $string);
		$string = preg_replace('#([a-z]*)[\x00-\x20\/]*=[\x00-\x20\/]*([\`\'\"]*)[\x00-\x20\/]*v[\x00-\x20]*b[\x00-\x20]*s[\x00-\x20]*c[\x00-\x20]*r[\x00-\x20]*i[\x00-\x20]*p[\x00-\x20]*t[\x00-\x20]*:#iUu', '$1=$2novbscript...', $string);
		$string = preg_replace('#([a-z]*)[\x00-\x20\/]*=[\x00-\x20\/]*([\`\'\"]*)[\x00-\x20\/]*-moz-binding[\x00-\x20]*:#Uu', '$1=$2nomozbinding...', $string);
		$string = preg_replace('#([a-z]*)[\x00-\x20\/]*=[\x00-\x20\/]*([\`\'\"]*)[\x00-\x20\/]*data[\x00-\x20]*:#Uu', '$1=$2nodata...', $string);

		//remove any style attributes, IE allows too much stupid things in them, eg.
		//<span style="width: expression(alert('Ping!'));"></span> 
		// and in general you really don't want style declarations in your UGC

		$string = preg_replace('#(<[^>]+[\x00-\x20\"\'\/])style[^>]*>#iUu', "$1>", $string);

		//remove namespaced elements (we do not need them...)
		$string = preg_replace('#</*\w+:\w[^>]*>#i',"",$string);
		//remove really unwanted tags

		do {
			$oldstring = $string;
			$string = preg_replace('#</*(applet|meta|xml|blink|link|style|script|embed|object|iframe|frame|frameset|ilayer|layer|bgsound|title|base)[^>]*>#i',"",$string);
		} while ($oldstring != $string);

		return $string;
	}
	
	////////////////////////////////////////////////////////
	//
	// Existence & authorization checks
	//
	////////////////////////////////////////////////////////

	function checkAuthenticated()
	{
		if (false) // (!($creds = get_client_credentials()) )
		{
			if (is_ra_client())
				// if we're coming from our web gui, we need a session
				throw new SCC_API_Unauthorized_Exception("INVALID_SESSION"); 
			else
				// otherwise this is a client app that needs an OAuth bearer token
				throw new SCC_API_Unauthorized_Exception("NO_CLIENT_ACCESS", "Client does not have valid credentials - use /oauth/req_token"); 
		}
		
		return true;
	}

	function checkIsMe()
	{
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation must be run as 'me', i.e. the currently authenticated user");
			
		foreach (API_Operation::$node_stack as $node)
		{
			if ($node->node_id == "ME")
				return true;
		}
		
		throw new SCC_API_Forbidden_Exception("FORBIDDEN", "This operation may only be performed on the user's own account"); 
	}
	
	function checkAccountIsPremium()
	{
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation requires a premium account.");

		// switch(get_account_status())
		// {
		//	case TRIAL_USAGE:
		//	case PAID_USAGE:
		//	case AUTOPAY_USAGE:
		//		break;
		//
		//	default:
		//		throw new SCC_API_Service_Unavailable_Exception
		// }
		
		return true;
	}


	////////////////////////////////////////////
	//
	// web portal checks
	//
	////////////////////////////////////////////

	function getResourceOwner()
	{
		if (!$this->resource_owner)
		{
			if (!is_media_server())
			{
				foreach (API_Operation::$node_stack as $node)
				{
					if ($node->node_id == 'ME')
						$username = get_auth_username();
					else if ($node->node_id == 'FRIEND')
						$username = $this->getParam(FORM_API_FRIEND);
				}
				
				if ($username)
				{
					if (!($this->resource_owner = dbu_get_user($username)) )
						throw new SCC_API_Not_Found_Exception("USER_NOT_FOUND", "Unknown user [$username]");
				}
			}
/*
			else
			{				
				$dsl_user = dbu_get_master_user();
				
				foreach (API_Operation::$node_stack as $node)
				{
					if ($node->node_id == 'ME')
						$this->resource_owner = $dsl_user;
					else if ($node->node_id == 'FRIEND' && $dsl_user['username'] == $this->getParam(FORM_API_FRIEND))
						$this->resource_owner = $dsl_user;
				}
			}
*/
		}
		
		return $this->resource_owner;
	}


	function checkNetworkInfoValid($acct = null)
	{	
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation requires the user to have installed the software on one or more PCs in their network.");

		if ($this->network_info)
			return $this->network_info;
			
		if (!is_media_server())
		{
			if ($acct == null)
				$acct = $this->getRequestedAccount();
			
			if (!($this->network_info = dbu_get_network_info($acct_id)) )
			{
				// the message for a user's own network info not existing is different than for someone else's network
				$code = (!defined("_CONTEXT_INC_") || $acct_id == get_auth_acct_id())?NETWORK_INVALID:NETWORK_DOWN;
				throw new RA_Existence_Exception("No network info defined for account [$acct_id]", $code);
			}
		}
				
		return $this->network_info;
	}

	function getHostByName($hostname)
	{
		$host = null;
		
		if (!is_media_server())
		{
			$acct = $this->getResourceOwner();
			
			$this->checkNetworkInfoValid($acct);
		
			$devices = dbu_get_network_devices($acct);
			
			foreach ($devices as $device)
			{
				if ($device['hostname'] == $hostname)
				{
					$host = $device;
					break;
				}
			}
			
			if (!$host)
				throw new SCC_API_Not_Found_Exception("HOST_NOT_FOUND", "Host [$hostname] not found.");
		}
		
		return $host;
	}

	function getHostByIPAddress($ip_addr)
	{
		$host = null;
		
		if (!is_media_server())
		{
			$acct = $this->getResourceOwner();
			
			$this->checkNetworkInfoValid($acct);
			
			$devices = dbu_get_network_devices($acct);
			
			foreach ($devices as $device)
			{
				if ($device['ip_addr'] == $ip_addr)
				{
					$host = $device;
					break;
				}
			}
			
			if (!$host)
				throw new SCC_API_Not_Found_Exception("HOST_NOT_FOUND", "Host with IP address [$ip_addr] not found.");
		}
		
		return $host;
	}

	function getHostByMACAddress($mac_addr)
	{
		$host = null;
		
		if (!is_media_server())
		{
			$acct = $this->getResourceOwner();
			
			$this->checkNetworkInfoValid($acct);
			
			$devices = dbu_get_network_devices($acct);
			
			foreach ($devices as $device)
			{
				if ($device['mac_addr'] == $mac_addr)
				{
					$host = $device;
					break;
				}
			}
			
			if (!$host)
				throw new SCC_API_Not_Found_Exception("HOST_NOT_FOUND", "Host with MAC address [$mac_addr] not found.");
		}
		
		return $host;
	}

	function checkHostServiceDefined($db_host, $service)	
	{
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation requires the specified host to offer " . get_host_service_description($service) . ".");

		if (!is_media_server())
		{
			require_once "include/common_map.inc";
			require_once "include/common_share.inc";
			
			// non-usb services are defined in the host entry itself
			if ($service & !RA_SERVICE_ANY_USB_DEVICE)
			{
				if (!check_share_permission(check_host_service($db_host, $service), $service))
					throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: Host [" . $db_host['ip_addr'] . "] does not offer service [" . $service . "]");
			}

			// usb services need to look up subdevices
			if ($service & RA_SERVICE_ANY_USB_DEVICE)
			{
				$sub_services = 0;
				$subdevices = dbu_get_host_subdevices($db_host['acct_id'], $db_host['mac_addr']);
				foreach ($subdevices as $d)
					$sub_services |= (int) $d['services'];
					
				if (!((int)$service & (int)$sub_services))
					throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: Host [" . $db_host['ip_addr'] . "] does not offer USB service [" . $service . "]", LOG_ALERT_SEVERITY_WARNING);
			}			
		}
	}

/*
	function checkAccessToHostService($db_host, $service)
	{
		global $global_context;
		
		// if share is set we must check that first
		if ( ($share = $global_context['share']) )
		{
			if ((int)$share['service'] & (int)$service == 0)
				throw new RA_Authorization_Exception("Operation [" . $this->op_name . "]: Current share [" . $share['share_id'] . "] does not allow service [$service]"); 
			
			if (!$share['host_mac'] || $share['host_mac'] != $db_host['mac_addr'])
				throw new RA_Authorization_Exception("Operation [" . $this->op_name . "]: Current share [" . $share['share_id'] . "] does not allow connections to host [" . $db_host['ip_addr'] . "]"); 
		}
		else
			$this->checkAuthenticated();
	}
	
	function checkShareRequiresRemoteConnection($share)
	{			
		if ($share['service'] & (RA_SERVICE_ANY_DEVICE_ACCESS | RA_SERVICE_ANY_DSL_ACCESS | RA_SERVICE_BACKUP) == 0)
			throw new RA_Authorization_Exception("Operation [" . $this->op_name . "]: current share [" . $share['share_id'] . "] does not require remote connection.");
		
		// device-oriented services require a host
		if ($share['service'] & RA_SERVICE_ANY_DEVICE_ACCESS && !$share['host_mac'])
			throw new RA_Authorization_Exception("Operation [" . $this->op_name . "]: current device share [" . $share['share_id'] . "] requires user to specify host & service.");
	}

	function checkShareService($share, $services)
	{			
		if (!$share || ((int)$share['service'] & (int)$services) == 0)
			throw new RA_Authorization_Exception("Operation [" . $this->op_name . "]: current share [" . $share['share_id'] . "] is not authorized for service(s) [$services].");
	}

	function checkShareExists($share_id)
	{			
		if (!$share_id || !($share = dbu_get_share($share_id)) )
			throw new RA_Authorization_Exception("Operation [" . $this->op_name . "]: Share with share id [$share_id] doesn't exist");

		return $share;
	}

	function checkShareOwner($share, $user_id)
	{
		if (!$share || !$user_id || $share['user_id'] != $user_id)
			throw new RA_Authorization_Exception("Operation [" . $this->op_name . "]: Share [{$share['share_id']}] is not owned by user [$user_id]");
	}

	function checkUserExists($user_id)
	{
		$db_user = null;
				
		if (!is_media_server())
		{
			if (!($db_user = dbu_get_user_by_id($user_id)) )
				throw new RA_Existence_Exception("Operation [" . $this->op_name . "]: Can't find user [$user_id]");
		}

		return $db_user;
	}

	function checkUserIsFriend($db_user, $confirmed_only = CONFIRMED_FRIENDS_ONLY)
	{
		$friends = array();
		
		if (!is_media_server())
		{
			// check that the current user is confirmed friends with the target (assuming the target is different user)
			if ( ($user_id = get_auth_user_id()) == $db_user['user_id'])
				throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: current user is specified user [" . $db_user['user_id'] . "]");

			$friends = dbu_get_friends($user_id, $confirmed_only);
			if (!isset($friends[$db_user['user_id']]))
				throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: current user not friends with user [" . $db_user['user_id'] . "]");
		}
		
		return $friends;
	}

	function checkUserIsNotFriend($db_user)
	{
		$friends = array();
		
		if (!is_media_server())
		{
			// check that the current user is confirmed friends with the target (assuming the target is different user)
			if ( ($user_id = get_auth_user_id()) == $db_user['user_id'])
				throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: current user is specified user [" . $db_user['user_id'] . "]");
				
			$friends = dbu_get_friends($user_id, INCLUDE_UNCONFIRMED_FRIENDS);
			if (isset($friends[$db_user['user_id']]))
				throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: current user is friends (may be unconfirmed) with user [" . $db_user['user_id'] . "]");
		}
				
		return $friends;
	}
	
	function checkUserIsReadyToBeConfirmed($db_user)
	{
		$friends = array();

		if (!is_media_server())
		{
			// check that the current user is confirmed friends with the target (assuming the target is different user)
			if ( ($user_id = get_auth_user_id()) == $db_user['user_id'])
				throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: current user is specified user [" . $db_user['user_id'] . "]");
				
			$friends = dbu_get_friends($user_id, INCLUDE_UNCONFIRMED_FRIENDS);
			if (!isset($friends[$db_user['user_id']]) || $friends[$db_user['user_id']]['status'] != FRIEND_STATUS_INVITER)
				throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: specified user cannot be confirmed as friend [" . $db_user['user_id'] . "]");
		}
				
		return $friends;
	}
	
	function checkDCExists($dc_id)
	{
		$db_dc = null;
		
		if (!is_media_server())
		{
			if (!($db_dc = dbu_get_dc($dc_id)) )
				throw new RA_Existence_Exception("Operation [" . $this->op_name. "]: Direct connection [" . $dc_id . "] does not exist.");
		}
		
		return $db_dc;
	}

	function checkDCOwned($dc_id)
	{
		$db_dc = null;
		
		if (!is_media_server())
		{
			$db_dc = $this->checkDCExists($dc_id);
				
			if (get_auth_user_id() != $db_dc['user_id'])
				throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: Direct connection [" . $dc_id . "] not owned by current user.");
		}
		
		return $db_dc;
	}

*/
	
	////////////////////////////////////////////
	//
	// media server checks
	//
	////////////////////////////////////////////

	function getFolderOrFile($path)
	{
		$user = $this->getResourceOwner();

		if (is_media_server() && $user)
		{
			// TODO: look up path
			
			if (!$path)
				throw new SCC_API_Not_Found_Exception("PATH_NOT_FOUND", "Couldn't locate folder or file with path [$path]");
		}
		
		return $path_resource;
	}

	function checkPathIsFolder($path)
	{
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation requires the specified path to be a folder.");

		return true;		
	}

	function checkPathIsFile($path)
	{
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation requires the specified path to be a file.");

		return true;		
	}

	function checkPathIsVideoFile($path)
	{
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation requires the specified path to be a video file.");

		return true;		
	}


	function getSearchResult($search_id)
	{
		$user = $this->getResourceOwner();

		if (is_media_server() && $user)
		{
			// TODO: look up search id
			
			if (!$search_result)
				throw new SCC_API_Not_Found_Exception("SEARCH_ID_NOT_FOUND", "No search result with ID [$search_id] exists.");
		}
		
		return $search_result;
	}

/*	
	function checkDSLSharePath($share_path)
	{
		if (is_media_server())
		{
			require_once "dsl/include/share.inc";
			
			if (!$share_path)
				throw new RA_Authorization_Exception("Operation [" . $this->op_name. "]: Currently network-wide shares not supported (DSL share with empty path)");
			
			// throws RA_Existence_Exception for invalid path elements
			list($host, $dir, $file) = define_share_path_elements($share_path);			
		}
	}

	function checkHostOnline($host, $code=0)
	{
		if (is_media_server())
		{
			if (!$host['state'])
				throw new RA_Unreachable_Exception($this->op_name, dbu_get_acct_id(), $host['host_name'], ($code?$code:HOST_DOWN), "Host [{$host['host_name']} is down, file cannot be downloaded.");
		}
	}
*/
		
}

class SCC_OPC_Authenticated_Op extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks() {}

	function doAuthorizationChecks()
	{
		$this->checkAuthenticated();		
	}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_User_Op extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks() 
	{
		$this->getResourceOwner();		
	}

	function doAuthorizationChecks()
	{
		$this->checkAuthenticated();		
	}
	
	function doAccountStatusChecks() {}
}
