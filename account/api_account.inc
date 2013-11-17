<?php

// 
// api_account.inc 
//

require_once "opcheck_account.inc";

class V1_API_Get_Account extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_User_Op($this), 
		                    "Get information about the authenticated user's account.");
	}
	
	function run($params)
	{
		return __CLASS__ . ":  " . var_export($params, true);
	}
}

class V1_API_Update_Account extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Update_Account($this), 
		                    "Get information about the authenticated user's account.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Log extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Log($this), 
		                    "Get usage log");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Apps extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_User_Op($this), 
		                    "Get list of apps that the user has granted access to.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class SCC_API_Module_Account
{
	function invokeModuleHandler($node, $verb, $handler)
	{
		switch ($handler)
		{
			case V1_API_GET_ACCOUNT:
				$c = new V1_API_Get_Account();
				break;
				
			case V1_API_UPDATE_ACCOUNT:
				$c = new V1_API_Update_Account();
				break;
				
			case V1_API_GET_LOG:
				$c = new V1_API_Get_Log();
				break;
				
			case V1_API_GET_APPS:
				$c = new V1_API_Get_Apps();
				break;

			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find handler [$handler]");
		}
		
		$c->setNode($node);
		$c->setVerb($verb);
		
		return $c->go();
	}
}

$GLOBALS['SCC_API_MODULES']['account'] = new SCC_API_Module_Account();
