<?php

// 
// opcheck_oauth.inc 
//

class SCC_OPC_Get_OAuth_Request_Token extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks() {}

	function doAuthorizationChecks() 
	{
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation does not require the user to be authenticated.");
	}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Get_OAuth_Access_Token extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks() {}

	function doAuthorizationChecks() 
	{
		// in docs mode just describe what we're doing
		if (API_Operation::$docs_mode)
			return API_Operation::$docs_mode->addOperationNote("This operation does not require the user to be authenticated.");
	}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Grant_App extends SCC_Operation_Check
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

class SCC_OPC_Revoke_App extends SCC_Operation_Check
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

class SCC_OPC_Get_OAuth extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks() {}

	function doAuthorizationChecks() {}
	
	function doAccountStatusChecks() {}
}

