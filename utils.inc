<?php

//
// utils.inc - utility functions
//

function is_debug()
{
	return true;
}

function is_media_server()
{
	return true;
}

// json_encode converts unicode chars into their \u00xx representations. PHP 5.4.0 introduces JSON_UNESCAPED_UNICODE, this mimics that
function ra_json_encode($obj)
{
	if (is_array($obj))
		return html_entity_decode(json_encode(escape_utf8_array($obj)), ENT_NOQUOTES, 'UTF-8');
	else
		return html_entity_decode(json_encode(ascii_to_entities($obj)), ENT_NOQUOTES, 'UTF-8');
}

function escape_utf8_array(&$arr)
{
	if (!is_array($arr))
		throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "invalid object passed to escape_utf8_array: " . var_export($arr, true));
		
	foreach ($arr as $key => $value)
	{
	 	if (is_string($value))
	 		$arr[$key] = ascii_to_entities($value);
	 	else if (is_array($value))
	 		escape_utf8_array($arr[$key]);
	}
	
	return $arr;
}

function ascii_to_entities($str) 
{ 
	$count = 1; 
	$out = ''; 
	$temp = array(); 

	for ($i = 0, $s = strlen($str); $i < $s; $i++) 
	{ 
		$ordinal = ord($str[$i]); 

		if ($ordinal < 128) 
		{ 
			if (count($temp) == 1) 
			{ 
				$out  .= '&#'.array_shift($temp).';'; 
				$count = 1; 
			} 

			$out .= $str[$i]; 
		} 
		else 
		{ 
			if (count($temp) == 0) 
			{ 
				$count = ($ordinal < 224) ? 2 : 3; 
			} 

			$temp[] = $ordinal; 

			if (count($temp) == $count) 
			{ 
				$number = ($count == 3) ? (($temp['0'] % 16) * 4096) + 
					(($temp['1'] % 64) * 64) + 
					($temp['2'] % 64) : (($temp['0'] % 32) * 64) + 
					($temp['1'] % 64); 

				$out .= '&#'.$number.';'; 
				$count = 1; 
				$temp = array(); 
			} 
		} 
	} 

	return $out; 
}


function scc_get_dev_website()
{
	return "https://dev.singleclickconnect.com";
}

function scc_get_docs_url()
{
	return scc_get_dev_website() . "/docs";
}

function scc_get_error_doc_url($error_code)
{
	return scc_get_dev_website() . "/docs/errors/$error_code";
}

if (!function_exists('getallheaders'))
{
	function getallheaders() { 
		$headers = array();
		foreach ($_SERVER as $name => $value) 
		{ 
			if (substr($name, 0, 5) == 'HTTP_') 
			{ 
				$name = str_replace(' ', '-', ucwords(strtolower(str_replace('_', ' ', substr($name, 5))))); 
				$headers[$name] = $value; 
			} else if ($name == "CONTENT_TYPE") { 
				$headers["Content-Type"] = $value; 
			} else if ($name == "CONTENT_LENGTH") { 
				$headers["Content-Length"] = $value; 
			} 
		} 
		return $headers; 
	} 
}

if (!function_exists('http_response_code'))
{
	function http_response_code($code) 
	{
		switch ($code) {
			case 100: $text = 'Continue'; break;
			case 101: $text = 'Switching Protocols'; break;
			case 200: $text = 'OK'; break;
			case 201: $text = 'Created'; break;
			case 202: $text = 'Accepted'; break;
			case 203: $text = 'Non-Authoritative Information'; break;
			case 204: $text = 'No Content'; break;
			case 205: $text = 'Reset Content'; break;
			case 206: $text = 'Partial Content'; break;
			case 300: $text = 'Multiple Choices'; break;
			case 301: $text = 'Moved Permanently'; break;
			case 302: $text = 'Moved Temporarily'; break;
			case 303: $text = 'See Other'; break;
			case 304: $text = 'Not Modified'; break;
			case 305: $text = 'Use Proxy'; break;
			case 400: $text = 'Bad Request'; break;
			case 401: $text = 'Unauthorized'; break;
			case 402: $text = 'Payment Required'; break;
			case 403: $text = 'Forbidden'; break;
			case 404: $text = 'Not Found'; break;
			case 405: $text = 'Method Not Allowed'; break;
			case 406: $text = 'Not Acceptable'; break;
			case 407: $text = 'Proxy Authentication Required'; break;
			case 408: $text = 'Request Time-out'; break;
			case 409: $text = 'Conflict'; break;
			case 410: $text = 'Gone'; break;
			case 411: $text = 'Length Required'; break;
			case 412: $text = 'Precondition Failed'; break;
			case 413: $text = 'Request Entity Too Large'; break;
			case 414: $text = 'Request-URI Too Large'; break;
			case 415: $text = 'Unsupported Media Type'; break;
			case 429: $text = 'Too Many Requests'; break;
			case 500: $text = 'Internal Server Error'; break;
			case 501: $text = 'Not Implemented'; break;
			case 502: $text = 'Bad Gateway'; break;
			case 503: $text = 'Service Unavailable'; break;
			case 504: $text = 'Gateway Time-out'; break;
			case 505: $text = 'HTTP Version not supported'; break;
			default:
				exit('Unknown http status code "' . htmlentities($code) . '"');
			break;
		}

		$protocol = (isset($_SERVER['SERVER_PROTOCOL']) ? $_SERVER['SERVER_PROTOCOL'] : 'HTTP/1.0');

		header($protocol . ' ' . $code . ' ' . $text);

		return $code;
	}
}

function get_host_service_description($service)
{
	switch ($service)
	{
		case SCC_SERVICE_RDP:
			return "remote desktop service (RDP)";
			
		case SCC_SERVICE_VNC:
			return "remote desktop service (VNC)";
			
		case SCC_SERVICE_HTTP:
			return "a webpage";
			
		case SCC_SERVICE_USB_DEVICE:
			return "a USB webcam";
			
		case SCC_SERVICE_DSL:
		case SCC_SERVICE_FILE:
		case SCC_SERVICE_ZIP:
		case SCC_SERVICE_SLIDESHOW:
		case SCC_SERVICE_MUSIC:
		case SCC_SERVICE_VIDEO:
		case SCC_SERVICE_SEARCH_ZIP:
		case SCC_SERVICE_SEARCH_SLIDESHOW:
		case SCC_SERVICE_SEARCH_MUSIC:
		case SCC_SERVICE_SEARCH_VIDEO:
			return "exported folders and files";
	}
}
