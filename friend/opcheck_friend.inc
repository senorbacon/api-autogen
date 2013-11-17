<?php

// 
// opcheck_friend.inc 
//

class SCC_OPC_Get_Friend extends SCC_Operation_Check
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
		
		//$this->checkUserIsFriend();	
	}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Invite_Friend extends SCC_Operation_Check
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
		
		// can be called from either /me/friends or /@friend
		
		if ($this->isMe())
		{
			// need to get email list 
		}
		else
		{
			//$this->checkUserIsNotFriend();	
		}
	}
	
	function doAccountStatusChecks() {}
}

class SCC_OPC_Confirm_Friend extends SCC_Operation_Check
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
		
		//$this->checkInvitedByFriend();	
	}
	
	function doAccountStatusChecks() {}
}
