<?php

// 
// api_internal.inc 
//

require_once "opcheck_internal.inc";

class V1_API_Check_Auth extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Check_Auth($this), 
		                    "Authenticate username/password credentials");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Logout extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Logout($this), 
		                    "Logout user session");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Emailed_Link extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Emailed_Link($this), 
		                    "Get information about the specified emailed link");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Create_Linked_Account extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Create_Linked_Account($this), 
		                    "Create an account in response to a non-account holder clicking a private link");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_FS_Folder extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_FS_Folder($this), 
		                    "Get list of file system folders.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Export_FS_Folder extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Export_FS_Folder($this), 
		                    "Export a folder to be exposed by the API.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Unexport_FS_Folder extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Unexport_FS_Folder($this), 
		                    "Unexport a folder.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Create_FS_Share extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Create_FS_Share($this), 
		                    "Set up a folder as a shared folder in the file system.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Share_Printer extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Share_Printer($this), 
		                    "Share a printer so that it is accessible to other PCs in the network.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class SCC_API_Module_Internal
{
	function invokeModuleHandler($node, $verb, $handler)
	{
		switch ($handler)
		{
			case V1_API_CHECK_AUTH:
				$c = new V1_API_Check_Auth();
				break;
				
			case V1_API_LOGOUT:
				$c = new V1_API_Logout();
				break;
				
			case V1_API_GET_EMAILED_LINK:
				$c = new V1_API_Get_Emailed_Link();
				break;

			case V1_API_CREATE_LINKED_ACCOUNT:
				$c = new V1_API_Create_Linked_Account();
				break;

			case V1_API_GET_FS_FOLDER:
				$c = new V1_API_Get_FS_Folder();
				break;

			case V1_API_EXPORT_FS_FOLDER:
				$c = new V1_API_Export_FS_Folder();
				break;

			case V1_API_UNEXPORT_FS_FOLDER:
				$c = new V1_API_Unexport_FS_Folder();
				break;

			case V1_API_CREATE_FS_SHARE:
				$c = new V1_API_Create_FS_Share();
				break;

			case V1_API_SHARE_PRINTER:
				$c = new V1_API_Share_Printer();
				break;

			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find handler [$handler]");
		}
		
		$c->setNode($node);
		$c->setVerb($verb);
		
		return $c->go();
	}
}

$GLOBALS['SCC_API_MODULES']['internal'] = new SCC_API_Module_Internal();
