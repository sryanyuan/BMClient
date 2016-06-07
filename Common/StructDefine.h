#ifndef _STRUCTDEFINE_H_
#define _STRUCTDEFINE_H_
#include <Windows.h>
/*
 *	返回值定义
 */
#define SKNL3_ERROR_NOTFINDPROCESS	(-1)
#define SKNL3_ERROR_CANTCREATESNAP	(-2)
#define SKNL3_ERROR_CANTOPENFILE	(0)
#define SKNL3_ERROR_CANTFILEMAP		1
#define SKNL3_ERROR_SUCCESS			(0x11111111)

#define INVALID_VALUE				0x8000000

//=====================================================================================//
//Name: KSYSTEM_SERVICE_TABLE 和 KSERVICE_TABLE_DESCRIPTOR                             //
//Descripion: 用来定义 SSDT 结构                                                       //
//=====================================================================================//

typedef struct _KSYSTEM_SERVICE_TABLE
{

	PULONG  ServiceTableBase;        // SSDT (System Service Dispatch Table)的基地址
	PULONG  ServiceCounterTableBase; // 包含 SSDT 中每个服务被调用的次数
	ULONG   NumberOfService;     // 服务函数的个数, NumberOfService * 4 就是整个地址表的大小
	ULONG   ParamTableBase;          // SSPT(System Service Parameter Table)的基地址

} KSYSTEM_SERVICE_TABLE, *PKSYSTEM_SERVICE_TABLE;


typedef struct _KSERVICE_TABLE_DESCRIPTOR
{
	KSYSTEM_SERVICE_TABLE   ntoskrnl; // ntoskrnl.exe 的服务函数
	KSYSTEM_SERVICE_TABLE   win32k;   // win32k.sys 的服务函数(GDI32.dll/User32.dll 的内核支持)
	KSYSTEM_SERVICE_TABLE   notUsed1;
	KSYSTEM_SERVICE_TABLE   notUsed2;

} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;


//导出由 ntoskrnl.exe 所导出的 SSDT
extern PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;


#endif