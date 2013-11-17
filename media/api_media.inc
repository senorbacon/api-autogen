<?php

// 
// api_media.inc 
//

require_once "opcheck_media.inc";

class V1_API_Get_Bandwidth extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Bandwidth($this), 
		                    "Get bandwidth from the remote network to this client.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Folders extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Media_Server($this), 
		                    "Get list of folders exported on the specified computer.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Path_Info extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Media_Server($this), 
		                    "Get information and services available for the specified folder or file.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Upload_File extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Folder($this), 
		                    "Upload a file to the specified folder.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Delete_File extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_File($this), 
		                    "Delete the specified file.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Download_File extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_File($this), 
		                    "Download the specified file.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Bulk_Download extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Folder_Or_Search_Results($this), 
		                    "Zip up the folder or search results and download it.");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
}

class V1_API_Get_Slideshow_Images extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Slideshow($this), 
		                    "Get a list of all images and thumbnails in the specified folder");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Get_Music extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Music($this), 
		                    "Create a playlist of all music in the folder or search results");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Get_Video extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Music($this), 
		                    "Get a link to a video stream of the specified file");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Get_Convert_Status extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Convert_Status($this), 
		                    "Get the status of an ongoing media conversion");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Convert_Media extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Convert_Media($this), 
		                    "Convert a music or video file to different formats, bitrates, resolutions, etc");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Get_Converted_Media extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Converted_Media($this), 
		                    "Download a converted media file");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Backup_Resource extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Media_Server($this), 
		                    "Back up a folder or file");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Restore_Resource extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Media_Server($this), 
		                    "Restore a missing folder or file");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Remove_Backup extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Access_Media_Server($this), 
		                    "Remove the online backup for the specified folder or file");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Search extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Search($this), 
		                    "Perform a keyword and/or file-type search");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 

class V1_API_Get_Search_Result extends API_Operation
{
	function __construct()
	{
		parent::__construct(new SCC_OPC_Get_Search_Result($this), 
		                    "Get the results of a prior search");
	}
	
	function run($params)
	{
		return __CLASS__ . ": " . var_export($params, true);
	}
} 


class SCC_API_Module_Media
{
	function invokeModuleHandler($node, $verb, $handler)
	{
		switch ($handler)
		{
			case V1_API_GET_BANDWIDTH:
				$c = new V1_API_Get_Bandwidth();
				break;
				
			case V1_API_GET_FOLDERS:
				$c = new V1_API_Get_Folders();
				break;
				
			case V1_API_GET_PATH_INFO:
				$c = new V1_API_Get_Path_Info();
				break;

			case V1_API_UPLOAD_FILE:
				$c = new V1_API_Upload_File();
				break;

			case V1_API_DELETE_FILE:
				$c = new V1_API_Delete_File();
				break;

			case V1_API_DOWNLOAD_FILE:
				$c = new V1_API_Download_File();
				break;

			case V1_API_GET_BULK_DOWNLOAD:
				$c = new V1_API_Get_Bulk_Download();
				break;

			case V1_API_GET_SLIDESHOW_IMAGES:
				$c = new V1_API_Get_Slideshow_Images();
				break;

			case V1_API_GET_MUSIC:
				$c = new V1_API_Get_Music();
				break;

			case V1_API_GET_VIDEO:
				$c = new V1_API_Get_Video();
				break;

			case V1_API_GET_CONVERT_STATUS:
				$c = new V1_API_Get_Convert_Status();
				break;

			case V1_API_CONVERT_MEDIA:
				$c = new V1_API_Convert_Media();
				break;

			case V1_API_GET_CONVERTED_MEDIA:
				$c = new V1_API_Get_Converted_Media();
				break;

			case V1_API_BACKUP_RESOURCE:
				$c = new V1_API_Backup_Resource();
				break;

			case V1_API_RESTORE_RESOURCE:
				$c = new V1_API_Restore_Resource();
				break;

			case V1_API_REMOVE_BACKUP:
				$c = new V1_API_Remove_Backup();
				break;

			case V1_API_SEARCH:
				$c = new V1_API_Search();
				break;

			case V1_API_GET_SEARCH_RESULT:
				$c = new V1_API_Get_Search_Result();
				break;

			default:
				throw new SCC_API_Internal_Error_Exception("INTERNAL_ERROR", "couldn't find handler [$handler]");
		}
		
		$c->setNode($node);
		$c->setVerb($verb);
		
		return $c->go();
	}
}

$GLOBALS['SCC_API_MODULES']['media'] = new SCC_API_Module_Media();
