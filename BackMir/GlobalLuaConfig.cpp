#include "GlobalLuaConfig.h"
#include "LuaClientEngine.h"
#include <Windows.h>
#include "../Common/GlobalFunction.h"
//////////////////////////////////////////////////////////////////////////
TOLUA_API int tolua_BackMirClient_open(lua_State* tolua_S);
//////////////////////////////////////////////////////////////////////////
static GlobalConfigMap s_xGlobalLuaConfigMap;
static MapConfigManager s_xMapConfigManager;
static std::string s_xNullValue;

int LoadGlobalLuaConfig()
{
	char szBuf[MAX_PATH];
	LuaClientEngine eng;
	//	设置脚本读取路径
#ifdef _DEBUG
	sprintf(szBuf, "%s\\Help\\",
		GetRootPath());
#else
	sprintf(szBuf, "%s\\Help\\dog.idx",
		GetRootPath());
#endif
	eng.SetLuaLoadPath(szBuf);
	
	// load script
	strcpy(szBuf, "client/main");
	eng.Create();
	lua_State* L = eng.GetVM();
	tolua_BackMirClient_open(L);

	if (!eng.DoModule(szBuf))
	{
		return 1;
	}

	// get global config
	std::string xCfgKey = "gHTTPKVAddress";
	lua_getglobal(L, xCfgKey.c_str());
	if (!lua_isstring(L, -1))
	{
		lua_pop(L, 1);
		return -1;
	}
	std::string xValue;
	xValue = lua_tostring(L, -1);
	lua_pop(L, 1);
	s_xGlobalLuaConfigMap.insert(std::make_pair(xCfgKey, xValue));

	// get map config
	if (!s_xMapConfigManager.Init(L))
	{
		return -1;
	}

	// close the engine
	eng.Destroy();

	return 0;
}

const std::string& GetGlobalLuaConfig(const std::string& _refKey)
{
	GlobalConfigMap::const_iterator it = s_xGlobalLuaConfigMap.find(_refKey);
	if (it == s_xGlobalLuaConfigMap.end())
	{
		return s_xNullValue;
	}
	return it->second;
}

const std::string& GetGlobalLuaConfig(const char* _pszKey)
{
	std::string xKey = _pszKey;
	return GetGlobalLuaConfig(xKey);
}

MapConfigManager& GetMapConfigManager()
{
	return s_xMapConfigManager;
}