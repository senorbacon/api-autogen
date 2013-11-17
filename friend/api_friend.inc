<?php

// 
// api_friend.inc 
//

require_once "opcheck_friend.inc";


class V1_API_Get_Friends extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_User_Op($this), 
		                    "Get the friends of the current user");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Friend_Friends extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_User_Op($this), 
		                    "Get the friends of the specified user");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Friend extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Friend($this), 
		                    "Get information about the specified user");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Invite_Friend extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Invite_Friend($this), 
		                    "Send an invitation to connect as friends");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Confirm_Friend extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Confirm_Friend($this), 
		                    "Confirm a request for friendship");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Remove_Friend extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Friend($this), 
		                    "Stop being friends with a user");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}




class SCC_API_Module_Friend
{
	function invokeModuleHandler($node, $verb, $handler)
	{
		switch ($handler)
		{
			case V1_API_GET_FRIENDS:
				$c = new V1_API_Get_Friends();
				break;
				
			case V1_API_GET_FRIEND_FRIENDS:
				$c = new V1_API_Get_Friend_Friends();
				break;
				
			case V1_API_GET_FRIEND:
				$c = new V1_API_Get_Friend();
				break;
				
			case V1_API_INVITE_FRIEND:
				$c = new V1_API_Invite_Friend();
				break;
				
			case V1_API_CONFIRM_FRIEND:
				$c = new V1_API_Confirm_Friend();
				break;

			case V1_API_REMOVE_FRIEND:
				$c = new V1_API_Remove_Friend();
				break;

			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find handler [$handler]");
		}
		
		$c->setNode($node);
		$c->setVerb($verb);
		
		return $c->go();
	}
}

$GLOBALS['SCC_API_MODULES']['friend'] = new SCC_API_Module_Friend();
