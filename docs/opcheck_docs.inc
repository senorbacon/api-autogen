<?php

// 
// opcheck_docs.inc 
//

class SCC_OPC_Get_Docs extends SCC_Operation_Check
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

