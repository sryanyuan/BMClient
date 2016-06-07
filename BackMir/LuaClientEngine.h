#ifndef _INC_LUACLIENTENGINE_
#define _INC_LUACLIENTENGINE_
//////////////////////////////////////////////////////////////////////////
#include "../../CommonModule/LuaBaseEngine.h"
//////////////////////////////////////////////////////////////////////////
enum LuaEvent
{
	kLuaEvent_SwitchScene,
};

struct LuaEvent_SwitchScene
{
	int nMapId;
};
//////////////////////////////////////////////////////////////////////////
class LuaClientEngine : public LuaBaseEngine
{
public:
	LuaClientEngine();
	virtual ~LuaClientEngine(){}

public:
	virtual void Output(const char* _pszLog);

	virtual int OnDispatchEvent(const LuaDispatchEvent* _pEvent, LuaDispatchInfo* _pInfo);
	virtual void OnDispatchEventResult(const LuaDispatchEvent* _pEvent, bool bResult);

public:
	bool ExecuteZip(const char* _pszFileName, const char* _pszSubFile);
};
//////////////////////////////////////////////////////////////////////////
#endif