//#include "stdafx.h"
#include "SKnl3Helper.h"
#include <stdio.h>

/*
 *	提升进程权限 hToken: 进程token, lpszPrivilegeName: 权限名 fEnable: 是否启用该权限, pPrePrivileges: 替换前的权限
 *	返回值 true: 设置成功 false: 设置失败
 */
bool SKNL3EnablePrivilege(HANDLE hToken, LPCTSTR lpszPrivilegeName, bool bEnable, PTOKEN_PRIVILEGES pPrePrivileges)
{
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	//	查询 privilegeName 的访问令牌，初始化Luid
	::LookupPrivilegeValue(NULL, lpszPrivilegeName, &tp.Privileges[0].Luid);
	//	调整对应Luid的权限
	tp.Privileges[0].Attributes = (bEnable ? SE_PRIVILEGE_ENABLED : FALSE);
	//	应用权限 提权
	::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES) ,pPrePrivileges, NULL);

	return (GetLastError() == ERROR_SUCCESS);
}
/*
 *	提升当前进程调试权限
 */
bool SKNL3EnableCurrentDbgPrivilege(bool bEnable)
{
	HANDLE hCurToken = NULL;
	if(!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hCurToken))
	{
		return false;
	}

	bool bSuccess = SKNL3EnablePrivilege(hCurToken, SE_DEBUG_NAME, bEnable, NULL);
	if(!bSuccess)
	{
		::CloseHandle(hCurToken);
		return false;
	}

	return true;
}
/*
*	根据进程名获取进程PID
*/
DWORD SKNL3GetProcessPID(LPCTSTR lpszProcessName)
{
	//	创建进程快照
	HANDLE hProcessSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(hProcessSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	//	遍历获取进程信息
	PROCESSENTRY32 ps32;
	ps32.dwSize = sizeof(PROCESSENTRY32);

	if(::Process32First(hProcessSnapshot, &ps32))
	{
		if(!strcmp(ps32.szExeFile, lpszProcessName))
		{
			::CloseHandle(hProcessSnapshot);
			return ps32.th32ProcessID;
		}
		while(::Process32Next(hProcessSnapshot, &ps32))
		{
			if(!strcmp(ps32.szExeFile, lpszProcessName))
			{
				::CloseHandle(hProcessSnapshot);
				return ps32.th32ProcessID;
			}
		}
	}

	::CloseHandle(hProcessSnapshot);
	return 0;
}
/*
*	是否合法的PE文件
*/
bool SKNL3IsValidPeFile(HANDLE hFileMap)
{
	if(!hFileMap)		{return false;}

	//	DOS MZ 头 第一个成员为 MZ 魔数 ， e_lfanew 为指向 PE 文件头的文件偏移
	//	先检测魔数
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hFileMap;
	if(pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}

	//	用e_lfanew 偏移算出PE文件头
	IMAGE_NT_HEADERS* pPeHeader = (IMAGE_NT_HEADERS*)((char*)pDosHeader + pDosHeader->e_lfanew);
	if(pPeHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}

	return true;
}
/*
*	创建一个文件的内存映射
*/
HANDLE SKNL3CreateMemoryFileMapping(LPCTSTR lpszFileName)
{
	HANDLE hFile = NULL;
	HANDLE hMapFile = NULL;
	HANDLE hMapView = NULL;

#ifdef _DEBUG
	char szErr[10];
#endif

	do 
	{
		hFile = ::CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			hFile = NULL;
#ifdef _DEBUG
			sprintf(szErr, "%d", GetLastError());
			OutputDebugString(szErr);
#endif
			break;
		}

		hMapFile = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, 0);
		if(!hMapFile)
		{
			break;
		}

		::CloseHandle(hFile);
		hFile = NULL;
		hMapView = ::MapViewOfFile(hMapFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if(!hMapView)
		{
			break;
		}

		::CloseHandle(hMapFile);
		hMapFile = NULL;
	} while (0);

	//	句柄释放
	{
		if(hFile)
		{
			::CloseHandle(hFile);
		}
		if(hMapFile)
		{
			::CloseHandle(hMapFile);
		}
	}

	return hMapView;
}
/*
 *	创建一个文件的内存映射(只读)
 */
HANDLE SKNL3CreateMemoryFileMappingReadOnly(LPCTSTR lpszFileName)
{
	HANDLE hFile = NULL;
	HANDLE hMapFile = NULL;
	HANDLE hMapView = NULL;

#ifdef _DEBUG
	char szErr[10];
#endif

	do 
	{
		hFile = ::CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			hFile = NULL;
#ifdef _DEBUG
			sprintf(szErr, "%d", GetLastError());
			OutputDebugString(szErr);
#endif
			break;
		}

		hMapFile = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, 0);
		if(!hMapFile)
		{
			break;
		}

		::CloseHandle(hFile);
		hFile = NULL;
		hMapView = ::MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
		if(!hMapView)
		{
			break;
		}

		::CloseHandle(hMapFile);
		hMapFile = NULL;
	} while (0);

	//	句柄释放
	{
		if(hFile)
		{
			::CloseHandle(hFile);
		}
		if(hMapFile)
		{
			::CloseHandle(hMapFile);
		}
	}

	return hMapView;
}
/*
*	RVA转换为物理地址偏移
*/
DWORD SKNL3GetPhysicalOffsetByRVA(HANDLE hFileMap, DWORD rva)
{
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hFileMap;
	IMAGE_NT_HEADERS* pPeHeader = (IMAGE_NT_HEADERS*)((char*)pDosHeader + pDosHeader->e_lfanew);
	IMAGE_SECTION_HEADER* pSectionTable = (IMAGE_SECTION_HEADER*)((char*)pPeHeader + sizeof(IMAGE_NT_HEADERS));
	int nSectionSum = pPeHeader->FileHeader.NumberOfSections;
	//	VirtualAddress 该段映射入内存的虚拟地址
	//	PointerToRawData 该段要映射的文件物理地址
	//	VirtualSize 要映射入内存的大小
	//	物理地址 = 相对于该段RVA的偏移 + 该段的物理地址
	for(int i = 0; i < nSectionSum; ++i)
	{
		//	判断rva是否在段内
		if(rva >= (pSectionTable + i)->VirtualAddress &&
			rva <= (pSectionTable + i)->VirtualAddress + (pSectionTable + i)->Misc.VirtualSize)
		{
			//	根据偏移算出实际位置
			return ((pSectionTable + i)->PointerToRawData + rva - (pSectionTable + i)->VirtualAddress);
		}
	}

	return 0;
}
/*
*	VA转换为物理地址偏移
*/
DWORD SKNL3GetPhysicalOffsetByVA(HANDLE hFileMap, DWORD va)
{
	DWORD nImageBase = (DWORD)::GetModuleHandle(NULL);
	DWORD rva = va - nImageBase;
	return SKNL3GetPhysicalOffsetByRVA(hFileMap, rva);
}
/*
 *	获得PE文件的引用DLL数量
 */
DWORD SKNL3GetImportDllSum(HANDLE hFileMap)
{
	IMAGE_IMPORT_DESCRIPTOR* pImportTable = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)hFileMap + SKNL3GetImportTablePAOffset(hFileMap));

	int nCounter = 0;
	while(1)
	{
		if ((pImportTable + nCounter)->Characteristics == 0 &&
			(pImportTable + nCounter)->FirstThunk == 0 &&
			(pImportTable + nCounter)->ForwarderChain == 0 &&
			(pImportTable + nCounter)->Name == 0 &&
			(pImportTable + nCounter)->OriginalFirstThunk == 0 &&
			(pImportTable + nCounter)->TimeDateStamp == 0)
		{
			return nCounter;
		}
		++nCounter;
	}
	return nCounter;
}
/*
*	获得PE文件导入表的物理地址偏移
*/
DWORD SKNL3GetImportTablePAOffset(HANDLE hFileMap)
{
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hFileMap;
	IMAGE_NT_HEADERS* pPeHeader = (IMAGE_NT_HEADERS*)((char*)pDosHeader + pDosHeader->e_lfanew);

	//	获取导入表RVA与PA
	DWORD pImportTableRVA = pPeHeader->OptionalHeader.DataDirectory[1].VirtualAddress;
	DWORD pImportTablePA = SKNL3GetPhysicalOffsetByRVA(hFileMap, pImportTableRVA);
	return pImportTablePA;
}
/*
*	获得某个DLL的导入函数数量
*/
DWORD SKNL3GetImportFunctionSumByIndex(HANDLE hFileMap, DWORD dwDllIndex)
{
	//	获取导入表地址
	IMAGE_IMPORT_DESCRIPTOR* pImportTable = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)hFileMap + SKNL3GetImportTablePAOffset(hFileMap)) + dwDllIndex;
	IMAGE_THUNK_DATA* pThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->OriginalFirstThunk;
	//	防止OriginalFirstThunk为0
	if(pThunkDataRVA == NULL)
	{
		//	改用FirstThunk
		pThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->FirstThunk;
	}
	IMAGE_THUNK_DATA* pThunkDataPA = (IMAGE_THUNK_DATA*)((DWORD)hFileMap + SKNL3GetPhysicalOffsetByRVA(hFileMap, (DWORD)pThunkDataRVA));

	int nCounter = 0;
	while (1)
	{
		if((pThunkDataPA + nCounter)->u1.Function == 0)
		{
			return nCounter;
		}
		++nCounter;
	}
	return nCounter;
}
DWORD SKNL3GetImportFunctionSumByName(HANDLE hFileMap, const char* lpszFuncName)
{
	//	获取导入表地址
	IMAGE_IMPORT_DESCRIPTOR* pImportTable = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)hFileMap + SKNL3GetImportTablePAOffset(hFileMap));

	//	遍历判断Dll索引
	bool bFound = false;
	bool bEqu = false;
	const char* pImportDllName = NULL;
	for(int i = 0; i < SKNL3GetImportDllSum(hFileMap); ++i)
	{
		pImportDllName = (const char*)((DWORD)hFileMap + (DWORD)SKNL3GetPhysicalOffsetByRVA(hFileMap, pImportTable->Name));
		if(strcmp(lpszFuncName, pImportDllName))
		{
			pImportTable += 1;
		}
		else
		{
			bFound = true;
			break;
		}
	}

	if(!bFound)
	{
		return 0;
	}

	IMAGE_THUNK_DATA* pThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->OriginalFirstThunk;
	//	防止OriginalFirstThunk为0
	if(pThunkDataRVA == NULL)
	{
		//	改用FirstThunk
		pThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->FirstThunk;
	}

	IMAGE_THUNK_DATA* pThunkDataPA = (IMAGE_THUNK_DATA*)((DWORD)hFileMap + SKNL3GetPhysicalOffsetByRVA(hFileMap, (DWORD)pThunkDataRVA));

	int nCounter = 0;
	while (1)
	{
		if((pThunkDataPA + nCounter)->u1.Function == 0)
		{
			return nCounter;
		}
		++nCounter;
	}
	return nCounter;
}
/*
*	获得DLL导入函数名称
*/
const char* SKNL3GetImportFunctionName(HANDLE hFileMap, DWORD dwDllIndex, DWORD dwFuncIndex)
{
	IMAGE_IMPORT_DESCRIPTOR* pImportTable = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)hFileMap + SKNL3GetImportTablePAOffset(hFileMap)) + dwDllIndex;
	IMAGE_THUNK_DATA* pFristThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->OriginalFirstThunk;
	//	防止OriginalFirstThunk为0
	if(pFristThunkDataRVA == NULL)
	{
		//	改用FirstThunk
		pFristThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->FirstThunk;
	}
	//	获取对应函数的IMAGE_THUNK_DATA
	IMAGE_THUNK_DATA* pTargetThunkDataRVA = pFristThunkDataRVA + dwFuncIndex;
	//	根据RVA取到IMAGE_IMPORT_BY_NAME的PA
	IMAGE_THUNK_DATA* pTargetThunkDataPA = (IMAGE_THUNK_DATA*)((DWORD)hFileMap + SKNL3GetPhysicalOffsetByRVA(hFileMap, (DWORD)pTargetThunkDataRVA));
	IMAGE_IMPORT_BY_NAME* pTargetNameRVA = (IMAGE_IMPORT_BY_NAME*)(pTargetThunkDataPA->u1.Function);
	IMAGE_IMPORT_BY_NAME* pTargetNamePA = (IMAGE_IMPORT_BY_NAME*)((DWORD)hFileMap + SKNL3GetPhysicalOffsetByRVA(hFileMap, (DWORD)pTargetNameRVA));
	return (const char*)(pTargetNamePA->Name);
}
/*
*	获取导入DLL的文件名
*/
const char* SKNL3GetImportDllNameByIndex(HANDLE hFileMap, DWORD dwDllIndex)
{
	IMAGE_IMPORT_DESCRIPTOR* pImportTable = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)hFileMap + SKNL3GetImportTablePAOffset(hFileMap)) + dwDllIndex;
	const char* pNameRVA = (const char*)pImportTable->Name;
	const char* pNamePA = (const char*)((DWORD)hFileMap + SKNL3GetPhysicalOffsetByRVA(hFileMap, (DWORD)pNameRVA));
	return pNamePA;
}
/*
*	获取导入DLL索引
*/
DWORD SKNL3GetImportDllIndexByName(HANDLE hFileMap, const char* lpszDllName)
{
	DWORD dwCounter = 0;
	IMAGE_IMPORT_DESCRIPTOR* pFirstDescriptorPA = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)hFileMap + SKNL3GetImportTablePAOffset(hFileMap));
	const char* lpszNameRVA = NULL;
	const char* lpszNamePA = NULL;
	bool bFound = false;

	for(int i = 0; i < SKNL3GetImportDllSum(hFileMap); ++i)
	{
		lpszNameRVA = (const char*)(pFirstDescriptorPA + i)->Name;
		lpszNamePA = (const char*)((DWORD)hFileMap + SKNL3GetPhysicalOffsetByRVA(hFileMap, (DWORD)lpszNameRVA));
		if(!strcmp(lpszDllName, lpszNamePA))
		{
			bFound = true;
			return dwCounter;
		}
		++dwCounter;
	}

	if(bFound)
	{
		return dwCounter;
	}
	else
	{
		return INVALID_VALUE;
	}
}
/*
*	是否有导入函数名称信息(有些是由序号导入的)
*/
bool SKNL3IsImportByName(HANDLE hFileMap, DWORD dwDllIndex)
{
	IMAGE_IMPORT_DESCRIPTOR* pImportTable = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)hFileMap + SKNL3GetImportTablePAOffset(hFileMap)) + dwDllIndex;
	IMAGE_THUNK_DATA* pFristThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->OriginalFirstThunk;
	//	防止OriginalFirstThunk为0
	if(pFristThunkDataRVA == NULL)
	{
		//	改用FirstThunk
		pFristThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->FirstThunk;
	}
	IMAGE_THUNK_DATA* pFisrtThunkDataPA = (IMAGE_THUNK_DATA*)((DWORD)hFileMap + SKNL3GetPhysicalOffsetByRVA(hFileMap, (DWORD)pFristThunkDataRVA));
	DWORD dwRet = pFisrtThunkDataPA->u1.Function;
	dwRet &= IMAGE_ORDINAL_FLAG;
	if(dwRet == 0)
	{
		return true;
	}
	return false;
}
/*
 *	假若是没有名称的导入 获取导入序数
 */
DWORD SKNL3GetImportFunctionOrder(HANDLE hFileMap, DWORD dwDllIndex, DWORD dwFuncIndex)
{
	if(SKNL3IsImportByName(hFileMap, dwDllIndex))
	{
		return INVALID_VALUE;
	}

	IMAGE_IMPORT_DESCRIPTOR* pImportTable = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)hFileMap + SKNL3GetImportTablePAOffset(hFileMap)) + dwDllIndex;
	IMAGE_THUNK_DATA* pFristThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->OriginalFirstThunk;
	//	防止OriginalFirstThunk为0
	if(pFristThunkDataRVA == NULL)
	{
		//	改用FirstThunk
		pFristThunkDataRVA = (IMAGE_THUNK_DATA*)pImportTable->FirstThunk;
	}
	IMAGE_THUNK_DATA* pFisrtThunkDataPA = (IMAGE_THUNK_DATA*)((DWORD)hFileMap + SKNL3GetPhysicalOffsetByRVA(hFileMap, (DWORD)pFristThunkDataRVA)) + dwFuncIndex;
	DWORD dwRet = pFisrtThunkDataPA->u1.Function;
	dwRet &= (~IMAGE_ORDINAL_FLAG);

	return dwRet;
}
/*
*	获得程序入口点RVA
*/
DWORD SKNL3GetPeEntryPoint(HANDLE hFileMap)
{
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hFileMap;
	IMAGE_NT_HEADERS* pPeHeader = (IMAGE_NT_HEADERS*)((char*)pDosHeader + pDosHeader->e_lfanew);
	return pPeHeader->OptionalHeader.AddressOfEntryPoint;
}












/*
*	获得文件大小
*/
DWORD SKNL3GetFileSize(LPCTSTR lpszFileName)
{
	if(lpszFileName == NULL)
	{
		return -1;
	}

	if(lstrlen(lpszFileName) > MAX_PATH)
	{
		return -1;
	}

	WIN32_FIND_DATA wfd;
	HANDLE hFind = ::FindFirstFile(lpszFileName, &wfd);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	::FindClose(hFind);

	if(wfd.nFileSizeHigh != 0)
	{
		return -1;
	}

	return wfd.nFileSizeLow;
}
/*
*	获得运行目录
*/
BOOL SKNL3GetCurrentAppPath(LPSTR lpBuffer, size_t _buffer_size)
{
	::GetModuleFileName(NULL, lpBuffer, _buffer_size);
	
	//	调整至 0
	int nStrLen = strlen(lpBuffer);
	lpBuffer += nStrLen;

	for(int i = 0; i < nStrLen; ++i)
	{
		if(*(lpBuffer--) == '\\')
		{
			lpBuffer += 2;
			*lpBuffer = '\0';
			return TRUE;
		}
	}

	return TRUE;
}