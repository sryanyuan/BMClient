#ifndef _INC_GLOBALLUACONFIG_
#define _INC_GLOBALLUACONFIG_
//////////////////////////////////////////////////////////////////////////
#include <map>
#include <string>
#include "../../CommonModule/MapConfigManager.h"
//////////////////////////////////////////////////////////////////////////
typedef std::map<std::string, std::string> GlobalConfigMap;
// wrapper for global config map
int LoadGlobalLuaConfig();
const std::string& GetGlobalLuaConfig(const char* _pszKey);
const std::string& GetGlobalLuaConfig(const std::string& _refKey);

MapConfigManager& GetMapConfigManager();
//////////////////////////////////////////////////////////////////////////
#endif