<?php

/************************************************************
*                                                           *
*  Copyright (c) SingleClick Systems. All rights reserved.  *
*                                                           *
*************************************************************/

//
// api.php - public API 
//

ob_start();

//require_once "../server_config.php";
//require_once "../web_common/api_common.inc";
//require_once "../web_common/ra_config.inc";
require_once "api.inc";

ob_end_clean();

try 
{
	$resource = _api_process_request();
	
	_api_output_response($resource);
}
catch (SCC_API_Exception $e)
{
	http_response_code($e->http_error);

	_api_output_error($e);
}

function _api_process_request()
{
	list($version, $path) = _api_get_path_elements();
	
	switch ($version)
	{
		case 'v1':
		case 'v2':
			break;
			
		default:
			throw new SCC_API_Bad_Request_Exception("INVALID_VERSION", "Invalid version [$version]");
	}
	
	API_Operation::$version = $version;

	$root_node = _api_build_all_versions();	
	
	if ($_SERVER['REQUEST_METHOD'] == 'POST')
	{
		if (!($verb = $_POST['verb']) )
			throw new SCC_API_Bad_Request_Exception("MISSING_VERB", "API request uses POST but does not specify 'verb' param");
	}
	else
		$verb = $_SERVER['REQUEST_METHOD'];
		
	$resource_node = _api_lookup_resource($version, $root_node, $path);

	return $resource_node->invokeHandler($verb);
}

function _api_lookup_resource($version, $root_node, $path)
{
	$node = $root_node;

	while ( ($cur = array_shift($path)) !== null)
	{
		API_Operation::push_node($node);
		
		$matched = false;
		$param = $cur;
		$debug = '';
		
		if (!$node->children_sorted)
		{
			krsort($node->children);
			$node->children_sorted = true;
		}
		
		foreach ($node->children as $token=>$child)
		{
			$debug .= ", $token";
			
			if ($token == $cur)
				$matched = true;
			else
			{
				switch ($token)
				{
					case API_ELEMENT_PARAM:
						$matched = true;
						break;
						
					case API_ELEMENT_PATH_PARAM:
						// consume path elements until we run out or we encounter a double slash
						// e.g.  /me/hosts/LAPTOP/folders/documents/a.doc
						// or    /me/hosts/LAPTOP/folders/pictures//slideshow
						while ( ($cur = array_shift($path)) !== null)
						{
							if ($cur === null || $cur === '')
								break;
							$param .= '/' . $cur;
						}
						$matched = true;
						break;
				}
			}
			
			if ($matched)
			{
				if ( ($node = API_Resource_Node::getMostRecentForVersion($child, $version)) )
				{
					// if we match a removed node then we didn't remove child links properly
					if ($node && $node->removed)
						throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "valid child link to removed node [{$node->node_id}]");
				
					if ($node->param_definition)
						API_Operation::add_url_param($node->param_definition['name'], $param);

					break;
				}
				else
					$matched = false;
			}
		}
		
		if (!$matched)
			throw new SCC_API_Bad_Request_Exception("INVALID_URL", "Invalid resource [$cur] ($debug)");
	}
	
	return $node;
}

function _api_get_path_elements()
{
	$path = array();
	$params = explode('/', $_SERVER['REQUEST_URI']);
	array_shift($params);
	array_shift($params); // knock the 6.0 off the URI
	
	$version = array_shift($params);	
	return array($version, $params);
}

function _api_output_error($error)
{
	header("Content-Type: application/json; charset=utf-8");

	$response = array();
	$response['error'] = array('code' => $error->error_code, 'dev_message' => $error->dev_message, 'user_message'=> $error->user_message, 'docs' => scc_get_error_doc_url($error->error_code));
	
	echo ra_json_encode($response);
}

function _api_output_response($resource)
{
	header("Content-Type: application/json; charset=utf-8");
	
	if (!is_array($resource))
		$resource = array('data' => $resource);

	$etag = '"' . md5(var_export($resource, true)) . '"';
	
	if (isset($_SERVER['HTTP_IF_NONE_MATCH']) && $_SERVER['HTTP_IF_NONE_MATCH'] == $etag)
	{
		header('Not Modified',true,304);
	}
	else
	{
		header("ETag: $etag");
		
		echo ra_json_encode($resource);
	}
}
