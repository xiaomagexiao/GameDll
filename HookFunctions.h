#ifndef _HOOK_FUNCTIONS_HEADER_
#define _HOOK_FUNCTIONS_HEADER_


/**************************************
HOOK FUNCTIONS
****************************************/
extern BOOL HookFunctionInHeader(
	char *ModuleName,
	char *FunctionName,
	PVOID HookFunctionProc,
	PVOID HookZone,
	int *PatchLength,
	PVOID *FunctionRet);

extern BOOL UnHookFunctionInHeader(
	char *ModuleName,
	char *FunctionName,
	PVOID HookZone,
	int PatchLength);

BOOL HookFunctionInAddress(
	PBYTE FunctionAddress,
	PVOID HookZone,
	int *PatchLength,
	PVOID *FunctionRet);

BOOL UnHookFunctionInAddress(
	PBYTE FunctionAddress,
	PVOID HookZone,
	int PatchLength);



extern BOOL SuperHookParseFileFunction(
	char *ModuleName,
	char *FunctionName,
	PVOID HookFunctionProc,
	PVOID *FunctionRet,
	PVOID tableIndexAdd);

extern BOOL SuperUnHookParseFileFunction(
	PVOID FunctionRet,
	PVOID tableIndexAdd);

/**************************************
*  功能： HookDispatchMessageW
*  参数：
*
*  返回值：
*  时间：[2015/1/11 14:25 Administrator]
****************************************/
 BOOL HookDispatchMessageW();

 /**************************************
 *  功能： UnHookDispatchMessageW
 *  参数：
 *
 *  返回值：
 *  时间：[2015/1/11 14:25 Administrator]
 ****************************************/
BOOL UnHookDispatchMessageW();

 /**************************************
 *  功能： HookNtDeviceIoControl
 *  参数：
 *
 *  返回值：
 *  时间：[2015/1/11 14:25 Administrator]
 ****************************************/
BOOL HookNtDeviceIoControl();

 /**************************************
 *  功能： UnHookNtDeviceIoControl
 *  参数：
 *
 *  返回值：
 *  时间：[2015/1/11 14:25 Administrator]
 ****************************************/
BOOL UnHookNtDeviceIoControl();

 /**************************************
 *  功能： HookSystemCallEx
 *  参数：
 *
 *  返回值：
 *  时间：[2015/1/11 14:25 Administrator]
 ****************************************/
BOOL HookSystemCallEx(BOOL (__stdcall * myHookRet)(DWORD index));
//void DealCodeOnline(CString str, int delatyTime, unsigned (__stdcall * AfterGetKey) (CString str));

BOOL UnHookSystemCallEx();

BOOL InLineHookCommon(DWORD InFunctionAddress, PVOID FunctionCallBack);

BOOL UnInLineHookCommon(DWORD InFunctionAddress);

DWORD HookWithSpecialCode(char *dllName,unsigned char data_chars[], 
	int length, CEdit *editInfo);

DWORD HookWithSpecialCode(char *dllName,unsigned char data_chars[], 
	int length, CEdit *editInfo, DWORD scope);

DWORD HookWithString(char *dllName, char *info, CEdit *editInfo, DWORD scope);
#endif