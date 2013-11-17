<?php

// 
// api_oauth.inc 
//

require_once "opcheck_oauth.inc";

class V1_API_Get_OAuth_Request_Token extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_OAuth_Request_Token($this), 
		                    "Request a temporary OAuth request token, the first step of the OAuth process.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_OAuth_Access_Token extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_OAuth_Access_Token($this), 
		                    "Request a persistent OAuth access token, the last step of the OAuth process.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Grant_App extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Grant_App($this), 
		                    "Grant permission to an app, allowing the app to request an OAuth access token.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Revoke_App extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Revoke_App($this), 
		                    "Revoke permission to an app");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

//
// dummy v2 op
//
class V2_API_Get_Oauth extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_OAuth($this), 
		                    "Dummy V2 op for testing.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V2_API_Get_OAuth_Request_Token extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_OAuth_Request_Token($this), 
		                    "Request a temporary OAuth request token, the first step of the OAuth process. (version 2)");
	}
	
	function run($params)
	{
		return __CLASS__ . " (v2): " . var_export($params, true);
	}
}


class SCC_API_Module_OAuth
{
	function invokeModuleHandler($node, $verb, $handler)
	{
		switch ($handler)
		{
			case V1_API_GET_OAUTH_REQ_TOKEN:
				$c = new V1_API_Get_Oauth_Request_Token();
				break;
				
			case V1_API_GET_OAUTH_ACCESS_TOKEN:
				$c = new V1_API_Get_Oauth_Access_Token();
				break;
				
			case V1_API_GRANT_APP:
				$c = new V1_API_Grant_App();
				break;
				
			case V1_API_REVOKE_APP:
				$c = new V1_API_Revoke_App();
				break;
				
			// example v2 node - this doesn't really do anything
			case V2_API_GET_OAUTH:
				$c = new V2_API_Get_Oauth();
				break;
				
			case V2_API_GET_OAUTH_REQ_TOKEN:
				$c = new V2_API_Get_Oauth_Request_Token();
				break;
				
			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find handler [$handler]");
		}
		
		$c->setNode($node);
		$c->setVerb($verb);
		
		return $c->go();
	}
}

$GLOBALS['SCC_API_MODULES']['oauth'] = new SCC_API_Module_OAuth();
