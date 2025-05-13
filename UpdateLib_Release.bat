

xcopy			/y/s			.\Engine\Public\*.*									 .\EngineSDK\Inc\

xcopy			/y				.\Engine\Bin\Release\Engine_R.dll					 .\Client\Bin\Release\

xcopy			/y				.\Engine\Bin\Release\Engine_R.lib					 .\EngineSDK\Lib\

xcopy			/y				.\Engine\ThirdPartyLib\*.lib						 .\EngineSDK\Lib\

xcopy			/y				.\Engine\Bin\ShaderFiles\*.*						 .\Engine\Bin\Release\ShaderFiles\