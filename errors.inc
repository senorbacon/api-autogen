<?php

// 
// errors.inc 
//

class SCC_API_Exception extends Exception 
{
	var $http_error = 500;
	var $error_code = null;
	var $dev_message = null;
	var $user_message = null;

	var $error_level = LOG_ALERT_SEVERITY_ERROR;
	var $do_alert_event = true;
	
	function __construct($error_code, $dev_message = null, $user_message = null, $error_level = null /*, $previous = null */)  // nested exceptions added in 5.3
	{
		parent::__construct($dev_message /*, $previous */);
		
		$this->error_code = $error_code;
		
		if (!$dev_message)
			$dev_message = get_default_dev_message($error_code, $http_error);
		
		$this->dev_message = $dev_message;
			
		if (!$user_message)
			$user_message = get_default_user_message($error_code, $http_error);
		
		$this->user_message = $user_message;
	
		if ($error_level)
			$this->error_level = $error_level;
		
		if (is_debug())
			$this->dev_message = trim($dev_message . " \n" . $this->getTraceAsString());
//
//		if ($this->do_alert_event)
//			new RA_Event_Alert($dev_message . " (error $error_code)", $this->getFile(), $this->getLine(), $this->error_level);
	}
}

class SCC_API_Bad_Request_Exception extends SCC_API_Exception
{
	function __construct($error_code, $dev_message = null, $user_message = null, $error_level = null)
	{
		$this->http_error = 400;
		
		parent::__construct($error_code, $dev_message, $user_message, $error_level);			
	}
}

class SCC_API_Unauthorized_Exception extends SCC_API_Exception
{
	function __construct($error_code, $dev_message = null, $user_message = null, $error_level = null)
	{
		$this->do_alert_event = false;
		$this->http_error = 401;

		//dbu_another_unauthorized();
		sleep(2);
		
		parent::__construct($error_code, $dev_message, $user_message, $error_level);			
	}
}

class SCC_API_Forbidden_Exception extends SCC_API_Exception
{
	function __construct($error_code, $dev_message = null, $user_message = null, $error_level = null)
	{
		$this->http_error = 403;

		//dbu_another_unauthorized();
		sleep(2);
		
		parent::__construct($error_code, $dev_message, $user_message, $error_level);			
	}
}

class SCC_API_Not_Found_Exception extends SCC_API_Exception
{
	function __construct($error_code, $dev_message = null, $user_message = null, $error_level = null)
	{
		$this->http_error = 404;

		//dbu_another_unauthorized();
		sleep(2);
		
		parent::__construct($error_code, $dev_message, $user_message, $error_level);			
	}
}

class SCC_API_Too_Many_Requests_Exception extends SCC_API_Exception
{
	function __construct($error_code, $dev_message = null, $user_message = null, $error_level = null)
	{
		$this->http_error = 429;

		parent::__construct($error_code, $dev_message, $user_message, $error_level);			
	}
}

class SCC_API_Internal_Error_Exception extends SCC_API_Exception
{
	function __construct($error_code, $dev_message = null, $user_message = null, $error_level = null)
	{
		$this->http_error = 500;
		$this->error_level = LOG_ALERT_SEVERITY_FATAL;
		
		parent::__construct($error_code, $dev_message, $user_message, $error_level);			
	}
}

class SCC_API_Service_Unavailable_Exception extends SCC_API_Exception
{
	function __construct($error_code, $dev_message = null, $user_message = null, $error_level = null)
	{
		$this->http_error = 503;
		$this->do_alert_event = false;
		
		parent::__construct($error_code, $dev_message, $user_message, $error_level);			
	}
}


function get_default_dev_message($error_code, $http_error)
{
	$msg = "Please see docs for more information";
	switch ($error_code)
	{
		default:
		{
			switch ($http_error)
			{
				case 400:
					$msg = "There was an error with the way the client constructed the API request.";
					break;
				case 401:
					$msg = "The client requested a resource that requires authentication first.";
					break;
				case 403:
					$msg = "The client requested a resource that it does not have access to.";
					break;
				case 404:
					$msg = "The client requested a resource that does not exist."; 
					break;
				case 429:
					$msg = "The client has exceeded the number of requests allowable in the given timeframe.";
					break;
				case 500:
					$msg = "There was an internal server error processing the request... please contact support if this does not resolve in a few minutes.";
					break;
				case 503:
					$msg = "The client requested a resource that is not available at this time.";
					break;
			}
		}
	}
	return $msg;
}
			
function get_default_user_message($error_code, $http_error)
{
	$msg = /* IDS_COM_TECHNICAL_DIFFICULTIES */ "Whoops! Something went wrong and we're not sure what. Our technical staff has been notified. Try again in a few minutes.";
	switch ($error_code)
	{
		default:
		{
			switch ($http_error)
			{
				case 400:
					// these are going to be the fault of the client in most cases
					$msg = /* IDS_COM_TECHNICAL_DIFFICULTIES */ "Whoops! Something went wrong and we're not sure what. Our technical staff has been notified. Try again in a few minutes.";
					break;
				case 401:
					// should provide specific error msgs by error code above
					$msg = /* IDS_COM_NEED_TO_AUTHENTICATE */ "Your session has expired. Please log in again.";
					break;
				case 403:
					// this is either a client error or a hack attempt
					$msg = /* IDS_COM_NO_ACCESS_TO_RESOURCE */ "You do not have access to that.";
					break;
				case 404:
					// some resources just disappear, or could be client error
					$msg = /* IDS_COM_RESOURCE_MISSING */ "Sorry, but we could not locate that.";
					break;
				case 429:
					// legitimate users should not see this error... but if they do they may want to pony up
					$msg = /* IDS_COM_TOO_MANY_REQUESTS */ "Usage of this software is limited to a certain number of requests per minute.";
					break;
				case 500:
					// technical difficulties msg is fine
					break;
				case 503:
					// lousy default message, should provide specific error msgs for all cases
					$msg = /* IDS_COM_RESOURCE_NOT_AVAILABLE */ "The resource you have requested is currently not available.";
					break;
			}
		}
	}
	return $msg;
}
