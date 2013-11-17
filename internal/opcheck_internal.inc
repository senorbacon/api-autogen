<?php

// 
// opcheck_internal.inc 
//

class SCC_OPC_Check_Auth extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks() {}

	function doAuthorizationChecks() {}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Logout extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks() {}

	function doAuthorizationChecks() {}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Get_Emailed_Link extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks() {}

	function doAuthorizationChecks() {}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Create_Linked_Account extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks() {}

	function doAuthorizationChecks() {}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Get_FS_Folder extends SCC_Operation_Check
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

class SCC_OPC_Export_FS_Folder extends SCC_Operation_Check
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

class SCC_OPC_Unexport_FS_Folder extends SCC_Operation_Check
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

class SCC_OPC_Create_FS_Share extends SCC_Operation_Check
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

class SCC_OPC_Share_Printer extends SCC_Operation_Check
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

