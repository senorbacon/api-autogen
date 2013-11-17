<?php

// 
// node.inc 
//

class API_Resource_Node
{
	static $node_hash = array();
	
	var $node_id;
	var $description;
	var $token;
	var $param_definition;
	var $public;
	var $version;

	var $parents = array();
	var $children = array();
	var $handlers = array();

	var $children_sorted = false;
	var $removed = false;

	function __construct($version, $node_id, $token, $param_definition, $parent, $public = API_PUBLIC)
	{
		$this->node_id = $node_id;
		$this->token = $token;
		$this->public = $public;
		$this->version = $version;

		if ($param_definition)
			$this->param_definition = $param_definition;
		
		$previous = null; 
		
		if (isset(self::$node_hash[$node_id]))
		{
			if (isset(self::$node_hash[$node_id][$version]))
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "version $version node [$node_id] already created");
			
			// save the previous version so we can copy its links to this node
			$previous = end(self::$node_hash[$node_id]);
			
			self::$node_hash[$node_id][$version] = $this;
		}
		else
			self::$node_hash[$node_id] = array($version => $this);
		
		if (!$previous)
		{
			// first time we're defining this node for current version
			if ($parent)
			{
				$parent_node = self::checkLatestVersion($parent, $version);
				
				if (isset($parent_node->children[$token]))
					throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "adding [$node_id]: parent node [$parent] already has a child link using [$token]");
						
				$parent_node->children[$token] = $node_id;
				
				$this->parents[$parent] = $parent;
			}
			else if ($node_id != "ROOT")
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "adding [$node_id]: No parent specified");
		}
		else
		{
			// replacing a previous version

			// copy parent links
			foreach ($previous->parents as $parent=>$value)
			{
				if (!self::$node_hash[$parent])
					throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "replacing [$node_id]: Couldn't find parent node [$parent]");

				$this->parents[$parent] = $parent;
			}
			
			// copy child links
			foreach ($previous->children as $token=>$child)
			{
				if (!self::$node_hash[$child])
					throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "replacing [$node_id]: Couldn't find child node [$child]");

				$this->children[$token] = $child;
			}
			
			// copy handlers
			foreach ($previous->handlers as $verb=>$handler)
			{
				$this->handlers[$verb] = $handler;
			}
		}
	}
	
	function addDescription($desc)
	{
		$this->description = $desc;
	}

	function addHandler($verb, $func)
	{
		if (isset($this->handlers[$verb]))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "[$verb] already defined for node [{$this->node_id}]");
		
		$this->handlers[strtoupper($verb)] = $func;
	}

	function updateHandler($verb, $func)
	{
		if (!isset($this->handlers[$verb]))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "update handler: [$verb] not yet defined for node [{$this->node_id}]");
		
		$this->handlers[strtoupper($verb)] = $func;
	}
		
	function removeHandler($verb)
	{
		if (!isset($this->handlers[$verb]))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "remove handler: [$verb] not yet defined for node [{$this->node_id}]");
		
		unset($this->handlers[strtoupper($verb)]);
	}
		
	function invokeHandler($verb)
	{
		$verb = strtoupper($verb);
		if (isset($this->handlers[$verb]))
		{
			list($module, $handler) = explode('|', $this->handlers[$verb]);
			
			if (!$module || !$handler)
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "API_Operation not defined for resource [{$this->node_id}], verb [$verb]");
			
			if (!isset($GLOBALS['SCC_API_MODULES'][$module]))
				require_once "$module/api_$module.inc";
			
			return $GLOBALS['SCC_API_MODULES'][$module]->invokeModuleHandler($this, $verb, $this->handlers[$verb]);
		}
		else
			throw new SCC_API_Bad_Request_Exception("NO_HANDLER", "[$verb] not defined for resource [{$this->node_id}]");
	}
		
	function setRemoved($version)
	{
		$this->removed = true;
		
		foreach ($this->children as $child)
		{
			if (!($child_versions = API_Resource_Node::getNodeVersions($child)) )
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Couldn't find child node [$child] of [$node_id]");
			
			$child_node = $this->getOrCreateNewVersion($version);
			
			if (!$child_node->removed)
				$child_node->setRemoved($version);
		}
	}
	
	function getOrCreateNewVersion($version)
	{
		$versions = self::getNodeVersions($this->node_id);
		
		if ($versions[$version])
			return $versions[$version];
		
		$n = new API_Resource_Node($version, $this->node_id, $this->token, $this->param_definition, current($this->parents), $this->public);
		$n->description = $this->description;
		
		return $n;
	}
	
	static function getLatestVersion($node_id)
	{
		if (!($node = end(self::$node_hash[$node_id])) )
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Couldn't find node [$node_id]");
		
		return $node;
	}
	
	static function getNodeVersions($node_id)
	{
		return self::$node_hash[$node_id];
	}
	
	// returns the most recent version of the node that is older than $version
	// if the only versions of the node are newer than $version, return NULL
	static function getMostRecentForVersion($node_id, $version)
	{
		if (!($node_versions = self::$node_hash[$node_id]))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Couldn't find node [$node_id]");

		$node = null;		
		// get most recent node that has equal or older version than given version
		foreach ($node_versions as $v=>$n)
		{
			if ($v <= $version)
				$node = $n;
		}	
		
		return $node;
	}
	
	static function updateVersion($version, $node_id)
	{
		if (!($node_versions = API_Resource_Node::getNodeVersions($node_id)) )
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Couldn't find node [$node_id]");
			
		$node = end($node_versions);
		
		return $node->getOrCreateNewVersion($version);
	}
	
	// check that 
	// a) $version is the latest version of $node_id
	// b) $version exists for $node_id
	static function checkLatestVersion($node_id, $version)
	{
		if (!($node_versions = API_Resource_Node::getNodeVersions($node_id)) )
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find node [$node_id]");

		$matched = null;
		foreach ($node_versions as $node_version=>$node)
		{
			if ($matched)
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "attempting to update old version of node [$node_id]");

			if ($version == $node_version)
				$matched = $node;
		}
		
		if (!$matched)
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find version [$version] of node [$node_id]");
		
		return $matched;
	}
	
	// always links the latest node to the latest parent
	static function link($version, $node_id, $parent)
	{
		if (!$parent)
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Null parent in link()");
		
		$node = API_Resource_Node::checkLatestVersion($node_id, $version);
	
		$parent_node = API_Resource_Node::checkLatestVersion($parent, $version);
	
		if (isset($node->parents[$parent]))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "linking [$node_id]: already linked to parent [$parent]");

		$node->parents[$parent] = $parent;

		if (isset($parent_node->children[$node->token]))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "linking [$node_id]: parent node [$parent] already linked using [{$node->token}]  {" . var_export($parent_node->children, true) . "}");

		$parent_node->children[$node->token] = $node_id;
	}

	// remove node from the tree, creating new versions of parents and children if necessary
	static function removeNode($version, $node_id)
	{
		$node = API_Resource_Node::checkLatestVersion($node_id, $version);

		// mark node and all its children removed for docs purposes
		$node->setRemoved($version);

		// unlink parent nodes from this one, updating parent versions if necessary		
		foreach ($node->parents as $parent=>$value)
		{
			if (!($parent_versions = API_Resource_Node::getNodeVersions($parent)) )
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Couldn't find parent node [$parent] of [$node_id]");
			
			if (!($parent_node = $parent_versions[$version]) )
			{
				$existing = end($parent_versions);

				$parent_node = $existing->getOrCreateNewVersion($version);
			}

			API_Resource_Node::unlink($version, $node_id, $parent);
		}
	}

	// always unlinks from the latest node
	static function unlink($version, $node_id, $parent)
	{
		if (!$parent)
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "Null parent in unlink()");
		
		$node = API_Resource_Node::checkLatestVersion($node_id, $version);
	
		$parent_node = API_Resource_Node::checkLatestVersion($parent, $version);
	
		if (!isset($parent_node->children[$node->token]))
			throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "unlinking [$node_id]: parent node [$parent] not currently linked");

		unset($parent_node->children[$node->token]);
	}
}

