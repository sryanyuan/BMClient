#ifndef _INC_EXCEPTIONHANDLER_
#define _INC_EXCEPTIONHANDLER_
//////////////////////////////////////////////////////////////////////////
#include "../../CommonModule/StackWalker/StackWalker.h"
//////////////////////////////////////////////////////////////////////////
LONG WINAPI BM_UnhandledExceptionFilter(_EXCEPTION_POINTERS* pExceptionInfo);

/************************************************************************/
/* Stack walker
/************************************************************************/
class StackWalkerLog : public StackWalker
{
public:
	virtual void OnOutput(LPCSTR szText);
};
//////////////////////////////////////////////////////////////////////////
#endif