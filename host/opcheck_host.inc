<?php

// 
// opcheck_host.inc 
//

class SCC_OPC_Get_Hosts extends SCC_Operation_Check
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

class SCC_OPC_Get_Host extends SCC_Operation_Check
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

class SCC_OPC_Print extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks()
	{
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation requires the specified host to be a printer.");

		$this->getResourceOwner();
	}

	function doAuthorizationChecks()
	{
		$this->checkAuthenticated();		
	}
	
	function doAccountStatusChecks() {}
}


class SCC_OPC_Get_Network_URL extends SCC_Operation_Check
{
	var $host;
	var $service;
		
	function __construct($api_operation, $service)
	{
		$this->service = $service;
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks()
	{
		$this->getResourceOwner();		
	}

	function doAuthorizationChecks()
	{
		$this->checkAuthenticated();
		
		$this->host = $this->getHostByName($this->params[FORM_API_HOST_NAME]);
		
		$this->checkHostServiceDefined($this->host, $this->service);
	}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Get_USB_Devices extends SCC_Operation_Check
{
	var $host;
	var $service;
		
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
		
		$this->host = $this->getHostByName($this->params[FORM_API_HOST_NAME]);
	}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_USB_Device_Video extends SCC_Operation_Check
{
	var $host;
	var $service;
		
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
		
		$this->host = $this->getHostByName($this->params[FORM_API_HOST_NAME]);
	}
	
	function doAccountStatusChecks() {}
}

