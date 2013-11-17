<?php

// 
// opcheck_account.inc 
//

class SCC_OPC_Update_Account extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);
		
		$this->define_param(array(
			'name' => FORM_API_EMAIL, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_EMAIL,
			'description' => "Update account email address")
		);

		$this->define_param(array(
			'name' => FORM_API_PASSWORD,
			'publish' => API_PRIVATE, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_PASSWORD,
			'description' => "Update user password")
		);

		$this->define_param(array(
			'name' => FORM_API_FIRST_NAME,
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_STRING,
			'constraints' => array("max_length" => 128, "empty_string_ok" => true),
			'description' => "Update first name")
		);

		$this->define_param(array(
			'name' => FORM_API_LAST_NAME,
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_STRING,
			'constraints' => array("max_length" => 128, "empty_string_ok" => true),
			'description' => "Update last name")
		);

		$this->define_param(array(
			'name' => FORM_API_LANGUAGE,
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_ENUM,
			'constraints' => array("english", "portugeuse"), 
			'description' => "Update user's default language")
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

class SCC_OPC_Get_Log extends SCC_Operation_Check
{	
	function __construct($api_operation)
	{
		parent::__construct($api_operation);

		$this->define_param(array(
			'name' => FORM_API_PAGE_SIZE, 
			'required' => API_PARAM_OPTIONAL, 
			'type' => API_PARAM_INTEGER,
			'default' => 100,
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
