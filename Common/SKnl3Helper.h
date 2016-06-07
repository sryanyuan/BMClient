#ifndef _INC_SKNL3HELPER_
#define _INC_SKNL3HELPER_

#include <Windows.h>
#include <TlHelp32.h>
#include <WinNT.h>
#include <tchar.h>

#include "structdefine.h"

/*
 *	提升进程权限 hToken: 进程token, lpszPrivilegeName: 权限名 fEnable: 是否启用该权限, pPrePrivileges: 替换前的权限
 *	返回值 true: 设置成功 false: 设置失败
 */
bool SKNL3EnablePrivilege(HANDLE hToken, LPCTSTR lpszPrivilegeName, bool bEnable, PTOKEN_PRIVILEGES pPrePrivileges);
/*
 *	提升当前进程调试权限
 */
bool SKNL3EnableCurrentDbgPrivilege(bool bEnable);
/*
 *	根据进程名获取进程PID
 */
DWORD SKNL3GetProcessPID(LPCTSTR lpszProcessName);
/*
 *	是否合法的PE文件
 */
bool SKNL3IsValidPeFile(HANDLE hFileMap);
/*
 *	创建一个文件的内存映射
 */
HANDLE SKNL3CreateMemoryFileMapping(LPCTSTR lpszFileName);
/*
 *	创建一个文件的内存映射(只读)
 */
HANDLE SKNL3CreateMemoryFileMappingReadOnly(LPCTSTR lpszFileName);
/*
 *	RVA转换为物理地址偏移
 */
DWORD SKNL3GetPhysicalOffsetByRVA(HANDLE hFileMap, DWORD rva);
/*
 *	VA转换为物理地址偏移
 */
DWORD SKNL3GetPhysicalOffsetByVA(HANDLE hFileMap, DWORD va);
/*
 *	获得PE文件的引用DLL数量
 */
DWORD SKNL3GetImportDllSum(HANDLE hFileMap);
/*
 *	获得PE文件导入表的物理地址偏移
 */
DWORD SKNL3GetImportTablePAOffset(HANDLE hFileMap);
/*
 *	获得某个DLL的导入函数数量
 */
DWORD SKNL3GetImportFunctionSumByIndex(HANDLE hFileMap, DWORD dwDllIndex);
DWORD SKNL3GetImportFunctionSumByName(HANDLE hFileMap, const char* lpszFuncName);
/*
 *	获得DLL导入函数名称
 */
const char* SKNL3GetImportFunctionName(HANDLE hFileMap, DWORD dwDllIndex, DWORD dwFuncIndex);
/*
 *	获取导入DLL的文件名
 */
const char* SKNL3GetImportDllNameByIndex(HANDLE hFileMap, DWORD dwDllIndex);
/*
 *	获取导入DLL索引
 */
DWORD SKNL3GetImportDllIndexByName(HANDLE hFileMap, const char* lpszDllName);
/*
 *	是否有导入函数名称信息(有些是由序号导入的)
 */
bool SKNL3IsImportByName(HANDLE hFileMap, DWORD dwDllIndex);
/*
 *	假若是没有名称的导入 获取导入序数
 */
DWORD SKNL3GetImportFunctionOrder(HANDLE hFileMap, DWORD dwDllIndex, DWORD dwFuncIndex);
/*
 *	获得程序入口点RVA
 */
DWORD SKNL3GetPeEntryPoint(HANDLE hFileMap);




/*
 *	获得文件大小
 */
DWORD SKNL3GetFileSize(LPCTSTR lpszFileName);
/*
 *	获得运行目录
 */
BOOL SKNL3GetCurrentAppPath(LPSTR lpBuffer, size_t _buffer_size);

#endif