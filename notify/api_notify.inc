<?php

// 
// api_notify.inc 
//

require_once "opcheck_notify.inc";

class V1_API_Get_Notifications_Socket extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Notifications_Socket($this), 
		                    "Connect to socket on notification server.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Notifications extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Notifications($this), 
		                    "Get list of most recent notifications.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}


class SCC_API_Module_Notify
{
	function invokeModuleHandler($node, $verb, $handler)
	{
		switch ($handler)
		{
			case V1_API_GET_NOTIFICATIONS_SOCKET:
				$c = new V1_API_Get_Notifications_Socket();
				break;
				
			case V1_API_GET_NOTIFICATIONS:
				$c = new V1_API_Get_Notifications();
				break;

			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find handler [$handler]");
		}
		
		$c->setNode($node);
		$c->setVerb($verb);
		
		return $c->go();
	}
}

$GLOBALS['SCC_API_MODULES']['notify'] = new SCC_API_Module_Notify();
