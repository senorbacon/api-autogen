<?php

// 
// opcheck_share.inc 
//

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

class SCC_OPC_Get_Permissions extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
	}
	
	function doExistenceChecks()
	{
		$this->getResourceOwner();
		
		$this->host = $this->getHostByName($this->getParam(FORM_API_HOST_NAME));
		
		$this->service = $this->getService();
		
		if ($this->isDSLService($this->service) && isset($this->sanitized_params[FORM_API_PATH]))
			$this->path = $this->getFolderOrFile($this->getParam(FORM_API_PATH));
			
		if ($this->isSearchNode())
			$this->search_result = $this->getSearchResult($this->getParam(FORM_API_SEARCH_ID));
	}

	function doAuthorizationChecks()
	{
		$this->checkAuthenticated();		
		
		// only 'me' has access to this function
		$this->checkIsMe();
	}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Grant_Permissions extends SCC_OPC_Get_Permissions
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
		
		$enum = array("private" => array("value" => SHARE_PRIVATE, "description" => "Requires user to be logged in to access the resource"),
		              "public" => array("value" => SHARE_PUBLIC, "description" => "Allows access to the resource without requiring login"),
		              "public_link" => array("value" => SHARE_PUBLIC_LINK_ONLY, "description" => "Used in the generation of public links (see " . scc_get_docs_url() . "/public_links)"));
		
		$this->define_param(array(
			'name' => FORM_API_VISIBILITY, 
			'required' => API_PARAM_MANDATORY, 
			'type' => API_PARAM_ENUM, 
			'constraints' => $enum, 
			'description' => "Specifies whether users are required to log in to access the resource")
		);
		
		$this->define_param(array(
			'name' => FORM_API_FRIENDS, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_FRIEND_LIST, 
			'description' => "List of friends to whom to grant the specified resource, separated by comma (,)")
		);

		$this->define_param(array(
			'name' => FORM_API_EMAIL_LIST, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_EMAIL_LIST, 
			'description' => "List of email addresses that will get an invite to the resource, separated by comma or semicolon (,;)")
		);

		$this->define_param(array(
			'name' => FORM_API_MSG, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_STRING, 
			'constraints' => array('max-length' => 256),
			'description' => "Message or description of the resource that will accompany the notification")
		);

		$this->define_param(array(
			'name' => FORM_API_SHARE_SUBFOLDERS, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_BOOLEAN,
			'default' => 1,
			'description' => "Specify whether to grant permission to subfolders of the specified folder resource")
		);

		$this->define_param(array(
			'name' => FORM_API_SHARE_READ_ONLY, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_BOOLEAN, 
			'default' => 1,
			'description' => "Specify whether permissions granted allow the user to modify the resource")
		);
	}
	
	function doAccountStatusChecks()
	{
		// only trial or paid accounts can do this
		$this->checkAccountIsPremium();
	}
}

class SCC_OPC_Get_Shares extends SCC_Operation_Check
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

