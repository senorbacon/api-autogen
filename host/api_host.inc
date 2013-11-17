<?php

// 
// api_host.inc 
//

require_once "opcheck_host.inc";

class V1_API_Get_Hosts extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Hosts($this), 
		                    "Get list of hosts in the remote network.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Host extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Host($this), 
		                    "Get information and services offered by the specified host.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Print extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Print($this), 
		                    "Print a document to the specified printer.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Connect_Host_RDP extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Network_URL($this, SCC_SERVICE_RDP), 
		                    "Connect to the Remote Desktop service (RDP) offered by the specified host.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Connect_Host_VNC extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Network_URL($this, SCC_SERVICE_VNC), 
		                    "Connect to the Remote Desktop service (VNC) offered by the specified host.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Connect_Host_HTTP extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Network_URL($this, SCC_SERVICE_HTTP), 
		                    "Connect to the webpage offered by the specified host.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_USB_Devices extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_USB_Devices($this), 
		                    "Get list of USB devices attached to the specified host.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_USB_Device_Video extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_USB_Device_Video($this), 
		                    "Connect to a video stream of the USB device (usually a webcam).");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($this->params, true);
	}
}


class SCC_API_Module_Host
{
	function invokeModuleHandler($node, $verb, $handler)
	{
		switch ($handler)
		{
			case V1_API_GET_HOSTS:
				$c = new V1_API_Get_Hosts();
				break;
				
			case V1_API_GET_HOST:
				$c = new V1_API_Get_Host();
				break;
				
			case V1_API_PRINT:
				$c = new V1_API_Print();
				break;

			case V1_API_CONNECT_HOST_RDP:
				$c = new V1_API_Connect_Host_RDP();
				break;

			case V1_API_CONNECT_HOST_VNC:
				$c = new V1_API_Connect_Host_VNC();
				break;

			case V1_API_CONNECT_HOST_HTTP:
				$c = new V1_API_Connect_Host_HTTP();
				break;

			case V1_API_GET_USB_DEVICES:
				$c = new V1_API_Get_USB_Devices();
				break;

			case V1_API_USB_DEVICE_VIDEO:
				$c = new V1_API_USB_Device_Video();
				break;
			
			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find handler [$handler]");
		}
		
		$c->setNode($node);
		$c->setVerb($verb);
		
		return $c->go();
	}
}

$GLOBALS['SCC_API_MODULES']['host'] = new SCC_API_Module_Host();
