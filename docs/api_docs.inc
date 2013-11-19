<?php

// 
// api_docs.inc 
//

require_once "opcheck_docs.inc";

class V1_API_Get_Docs extends API_Operation
{
	var $op_notes = array();
	
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Docs($this), 
		                    "Get API documentation data.");
	}
	
	function run($params)
	{
		API_Operation::set_docs_mode($this);
		API_Operation::$node_stack = array();
		API_Operation::$url_params = array();
		
		$root_node_versions = API_Resource_Node::getNodeVersions("ROOT");
		if (!($root_node = $root_node_versions[self::$version]))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "no root node with version [" . self::$version . "] exists");
		
		$doc_nodes = $this->getDocNodes($root_node);
		
		return $doc_nodes;
	}
	
	function getParamDefinition($param_def)
	{
		$docs_param = $param_def;
		unset($docs_param['publish']);
		return $docs_param;		
	}
	
	function getDocNodes($n)
	{
		API_Operation::push_node($n);

		$node_notes = array();
		$this->op_notes = array();
		
		$doc_node = array("version" => $n->version, "node_id" => $n->node_id, "description" => $n->description, "token" => $n->token);
		
		if ($n->param_definition)
			$doc_node['param_definition'] = $this->getParamDefinition($n->param_definition);
		
		$actions = array();
		foreach ($n->handlers as $verb => $handler)
		{
			// results in a call to outputOperationDocs($op)
			if ( ($op_info = $n->invokeHandler($verb)) )
				$actions[$verb] = $op_info;
		}

		if ($n->version != self::$version)
		{
			$node_notes[] = "This node has been unchanged since version " . $n->version;
			
			foreach ($actions as $action)
				$action['notes'][] = "This operation has been unchanged since version " . $n->version;
		}
	
		$latest = API_Resource_Node::getLatestVersion($n->node_id);
		if ($latest->version > self::$version)
		{
			if ($latest->removed)
			{
				$node_notes[] = "This node is deprecated. It has been removed in later versions.";

				foreach ($actions as $action)
					$action['notes'][] = "This operation is deprecated. It has been removed in later versions.";
			}
			else
			{
				$node_notes[] = "This node has been changed since version " . self::$version;

				foreach ($actions as $action)
				{
					if (!isset($latest->handlers[$action['verb']]))
						$action['notes'][] = "This operation is deprecated. It has been removed in later versions.";
					else if ($latest->handlers[$action['verb']] != $n->handlers[$action['verb']])
						$action['notes'][] = "This operation has been changed since version " . self::$version;
				}
			}
		}
				
		$children = array();
		foreach ($n->children as $token => $child_id)
		{
			if ( ($child = API_Resource_Node::getMostRecentForVersion($child_id, self::$version)) )
			{
				if ($child->public == API_PUBLIC && !$child->removed)
				{
					$children[] = $this->getDocNodes($child);
				}
			}
		}
		
		if (count($node_notes))
			$doc_node['notes'] = $node_notes;
		
		if (count($actions))
			$doc_node['actions'] = $actions;
			
		if (count($children))
			$doc_node['children'] = $children;

		API_Operation::pop_node();
		
		return $doc_node;
	}
	
	function outputOperationDocs($operation)
	{
		$op_info = null;
		
		if ($operation->publish == API_PUBLIC)
		{
			$op_info = array('verb' => $operation->verb, 'description' => $operation->description);
			
			$param_defs = array();
			foreach ($operation->op_check->param_definitions as $param_def)
				if ($param_def['publish'] == API_PUBLIC)
					$param_defs[] = $this->getParamDefinition($param_def);
			
			if (count($param_defs))
				$op_info['param_definitions'] = $param_defs;
			
			// collect notes
			$operation->op_check->run(null);
			
			if (count($this->op_notes))
				$op_info['notes'] = $this->op_notes;
		}
		
		return $op_info;
	}	
	
	function addOperationNote($note)
	{
		$this->op_notes[] = $note;
	}
}

class SCC_API_Module_Docs
{
	function invokeModuleHandler($node, $verb, $handler)
	{
		switch ($handler)
		{
			case V1_API_GET_DOCS:
				$c = new V1_API_Get_Docs();
				break;
				
			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find handler [$handler]");
		}
		
		$c->setNode($node);
		$c->setVerb($verb);
		
		return $c->go();
	}
}

$GLOBALS['SCC_API_MODULES']['docs'] = new SCC_API_Module_Docs();
