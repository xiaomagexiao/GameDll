#include "stdafx.h"
#include "windows.h"
#include "winnt.h"



//////////////////////////////////////////////////////////////////////////
//
//  Hook mswsock.dll导出表的Ntdll!NtDeviceIoControlFile
//  并过滤其对TDI Cilent的请求来过滤封包
//  稳定，隐蔽，RING3下最底层的包过滤~
//
//////////////////////////////////////////////////////////////////////////
BOOL SuperHookParseFileFunction(
	char *ModuleName,
	char *FunctionName,
	PVOID HookFunctionProc,
	PVOID *FunctionRet,
	PVOID tableIndexAdd)
{
	//得到ws2_32.dll的模块基址
	HMODULE hMod = LoadLibraryA(ModuleName);
	if (hMod == 0 )
	{
		return FALSE;
	}

	//得到DOS头

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hMod ; 

	//如果DOS头无效
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}

	//得到NT头

	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG)hMod + pDosHeader->e_lfanew);

	//如果NT头无效
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	//检查输入表数据目录是否存在
	if (pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0 ||
		pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0 )
	{
		return FALSE;
	}
	//得到输入表描述指针

	PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((ULONG)hMod + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	PIMAGE_THUNK_DATA ThunkData ; 

	//检查每个输入项
	while(ImportDescriptor->FirstThunk)
	{
		//检查输入表项是否为ntdll.dll

		char* dllname = (char*)((ULONG)hMod + ImportDescriptor->Name);

		//如果不是，则跳到下一个处理

		if (stricmp(dllname , "ntdll.dll") !=0)
		{
			ImportDescriptor ++ ; 
			continue;
		}

		ThunkData = (PIMAGE_THUNK_DATA)((ULONG)hMod + ImportDescriptor->OriginalFirstThunk);

		int no = 1;
		while(ThunkData->u1.Function)
		{
			//检查函数是否为NtDeviceIoControlFile

			char* functionnameIn = (char*)((ULONG)hMod + ThunkData->u1.AddressOfData + 2);
			if (stricmp(functionnameIn , FunctionName) == 0 )
			{
				//
				//如果是，那么记录原始函数地址
				//HOOK我们的函数地址
				//
				ULONG myaddr = (ULONG)HookFunctionProc;
				ULONG btw ; 
				PDWORD lpAddr = (DWORD *)((ULONG)hMod + (DWORD)ImportDescriptor->FirstThunk) +(no-1);
				*(DWORD *)tableIndexAdd = (DWORD)lpAddr;
				*FunctionRet = (PVOID)(*(ULONG*)lpAddr) ; 
				WriteProcessMemory(GetCurrentProcess() , lpAddr , &myaddr , sizeof(ULONG), &btw );
				return TRUE; 

			}

			no++;
			ThunkData ++;
		}
		ImportDescriptor ++;
	}
	return FALSE;
}

BOOL SuperUnHookParseFileFunction(
	PVOID FunctionRet,
	PVOID tableIndexAdd)
{
	if(FunctionRet == NULL || tableIndexAdd==NULL)
	{
		return FALSE;
	}
	ULONG btw ; 
	//把这个函数指针写回去
	WriteProcessMemory(GetCurrentProcess() , tableIndexAdd ,
		&FunctionRet , sizeof(ULONG), &btw );
	return TRUE;
}