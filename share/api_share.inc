<?php

// 
// api_share.inc 
//

require_once "opcheck_share.inc";

// permissions ops can be called from the following resources:
//
// <host>/rdp
// <host>/vnc
// <host>/http
// <host>/usb_devices/<usb_device>
// <host>/folders/<path>
// <host>/folders/<path>/download
// <host>/folders/<path>/slideshow
// <host>/folders/<path>/music
// <host>/folders/<path>/video
// <host>/search/<search_result>/slideshow
// <host>/search/<search_result>/music
// <host>/search/<search_result>/video

class V1_API_Get_Permissions extends API_Operation
{
	function __construct($description = null)
	{
		if (!$description)
			$description = "Get the list of friends and any public links with access to the specified resource.";
			
		parent::__construct(new SCC_OPC_Get_Permissions($this), $description);
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Permissions_Friends extends V1_API_Get_Permissions
{
	function __construct()
	{
		parent::__construct("Get the list of friends with access to the specified resource.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Permissions_Links extends V1_API_Get_Permissions
{
	function __construct()
	{
		parent::__construct("Get the list of public links with access to the specified resource.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Grant_Permissions extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Grant_Permissions($this), 
		                    "Grant permissions to the specified resource to a list of friends and/or send invites to the resource to a list of email addresses.");
	}
	
	function run($params)
	{
		return __CLASS__ . " : " . var_export($params, true);
	}
}

class V1_API_Revoke_Permissions_Friend extends V1_API_Get_Permissions
{
	function __construct()
	{
		parent::__construct("Revoke permissions to the specified friend on the specified resource");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Revoke_Permissions_Link extends V1_API_Get_Permissions
{
	function __construct()
	{
		parent::__construct("Revoke permissions to the specified email address on the specified resource");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Shares extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Shares($this), 
		                    "Get the list of resources you have granted access to.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Friend_Shares extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Shares($this), 
		                    "Get the list of resources the specified friend has granted you permissions to access.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}


class SCC_API_Module_Share
{
	function invokeModuleHandler($node, $verb, $handler)
	{
		switch ($handler)
		{
			case V1_API_GET_PERMISSIONS:
				$c = new V1_API_Get_Permissions();
				break;
				
			case V1_API_GRANT_PERMISSIONS:
				$c = new V1_API_Grant_Permissions();
				break;
				
			case V1_API_GET_PERMISSIONS_FRIENDS:
				$c = new V1_API_Get_Permissions_Friends();
				break;

			case V1_API_REVOKE_PERMISSIONS_FRIEND:
				$c = new V1_API_Revoke_Permissions_Friend();
				break;

			case V1_API_GET_PERMISSIONS_LINKS:
				$c = new V1_API_Get_Permissions_Links();
				break;

			case V1_API_REVOKE_PERMISSIONS_LINK:
				$c = new V1_API_Revoke_Permissions_Link();
				break;

			case V1_API_GET_SHARES:
				$c = new V1_API_Get_Shares();
				break;

			case V1_API_GET_FRIEND_SHARES:
				$c = new V1_API_Get_Friend_Shares();
				break;

			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find handler [$handler]");
		}
		
		$c->setNode($node);
		$c->setVerb($verb);
		
		return $c->go();
	}
}

$GLOBALS['SCC_API_MODULES']['share'] = new SCC_API_Module_Share();
