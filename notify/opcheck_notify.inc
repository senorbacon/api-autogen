<?php

// 
// opcheck_notify.inc 
//


class SCC_OPC_Get_Notifications_Socket extends SCC_Operation_Check
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

class SCC_OPC_Get_Notifications extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);

		$this->define_param(array(
			'name' => FORM_API_PAGE_SIZE, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_INTEGER,
			'default' => 10,
			'low' => 1,
			'description' => "Number of results returned")
		);

		$this->define_param(array(
			'name' => FORM_API_OFFSET, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_INTEGER,
			'default' => 0,
			'low' => 0,
			'description' => "Output results beginning at offset")
		);
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

