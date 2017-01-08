#include "LuaClientEngine.h"
#include "../../CommonModule/SaveFile.h"
#include "../Common/GlobalFunction.h"
#include <hge.h>
//////////////////////////////////////////////////////////////////////////
LuaClientEngine::LuaClientEngine()
{
	SetZipPassword(SaveFile::CalcInternalPassword());
#ifdef _DEBUG
	m_eFileExt = kLuaFileExt_Lua;
#else
	m_eFileExt = kLuaFileExt_Bjt;
#endif
}

void LuaClientEngine::Output(const char* _pszLog)
{
	AfxGetHge()->System_Log(_pszLog);
}

bool LuaClientEngine::ExecuteZip(const char* _pszFileName, const char* _pszSubFile)
{
	return LoadFileInZip(_pszFileName, _pszSubFile, SaveFile::CalcInternalPassword());
}

int LuaClientEngine::OnDispatchEvent(const LuaDispatchEvent* _pEvent, LuaDispatchInfo* _pInfo)
{
	if(_pEvent->nEventId == kLuaEvent_SwitchScene)
	{
		const LuaEvent_SwitchScene* pEvent = (const LuaEvent_SwitchScene*)_pEvent->pEvent;

		PushInt(pEvent->nMapId);
		_pInfo->nArgs = 1;
	}

	return 0;
}

void LuaClientEngine::OnDispatchEventResult(const LuaDispatchEvent* _pEvent, bool bResult)
{

}