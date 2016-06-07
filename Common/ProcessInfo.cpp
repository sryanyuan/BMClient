#include "ProcessInfo.h"
#include <Psapi.h>
//////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "psapi.lib")
//////////////////////////////////////////////////////////////////////////
DWORD GetCurrentProcessMemoryUsage()
{
	HANDLE hCurrentProcess = ::GetCurrentProcess();

	if(NULL == hCurrentProcess)
	{
		return 0;
	}

	PROCESS_MEMORY_COUNTERS pmc;
	ZeroMemory(&pmc, sizeof(pmc));
	pmc.cb = sizeof(pmc);

	if(GetProcessMemoryInfo(hCurrentProcess, &pmc, sizeof(pmc)))
	{
		return pmc.WorkingSetSize;
	}

	return 0;
}