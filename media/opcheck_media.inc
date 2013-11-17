<?php

// 
// opcheck_media.inc 
//


class SCC_OPC_Access_Media_Server extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks()
	{
		$this->getResourceOwner();		
		
		$this->host = $this->getHostByName($this->params[FORM_API_HOST_NAME]);
		
		$this->checkHostServiceDefined($this->host, SCC_SERVICE_DSL);
	}

	function doAuthorizationChecks()
	{
		$this->checkAuthenticated();
	}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Get_Bandwidth extends SCC_OPC_Access_Media_Server
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
}

class SCC_OPC_Access_Folder extends SCC_OPC_Access_Media_Server
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
	
	function doExistenceChecks()
	{
		parent::doExistenceChecks();
		
		$this->checkPathIsFolder($this->params[FORM_API_PATH]);
	}	
}

class SCC_OPC_Access_Folder_Or_Search_Results extends SCC_OPC_Access_Media_Server
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
	
	function doExistenceChecks()
	{
		parent::doExistenceChecks();
		
		if (!$this->isSearchNode())
			$this->checkPathIsFolder($this->params[FORM_API_PATH]);
	}	
}

class SCC_OPC_Access_File extends SCC_OPC_Access_Media_Server
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
	
	function doExistenceChecks()
	{
		parent::doExistenceChecks();
		
		$this->checkPathIsFile($this->params[FORM_API_PATH]);
	}	
}

class SCC_OPC_Access_Video extends SCC_OPC_Access_Media_Server
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
	
	function doExistenceChecks()
	{
		parent::doExistenceChecks();
		
		$this->checkPathIsVideo($this->params[FORM_API_PATH]);
	}	
}

class SCC_OPC_Access_Slideshow extends SCC_OPC_Access_Folder_Or_Search_Results
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}		
}

class SCC_OPC_Access_Music extends SCC_OPC_Access_Folder_Or_Search_Results
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}		
}

class SCC_OPC_Convert_Media extends SCC_OPC_Access_File
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
	
	function doExistenceChecks()
	{
		parent::doExistenceChecks();
		
		//$this->checkPathIsVideoOrMusic($this->params[FORM_API_PATH]);
	}	
}

class SCC_OPC_Get_Convert_Status extends SCC_OPC_Access_Media_Server
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
}

class SCC_OPC_Get_Converted_Media extends SCC_OPC_Access_Media_Server
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
}

class SCC_OPC_Search extends SCC_OPC_Access_Media_Server
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
}

class SCC_OPC_Get_Search_Result extends SCC_OPC_Access_Media_Server
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}	
}
