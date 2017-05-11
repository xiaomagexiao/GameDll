//////////exediy 8763232
#include "StdAfx.h"
#include "HookSystemCall.h"
#include "GetSSDTInformation.h"


DWORD	dwIndex;
DWORD	dwKiFastSystemCall;
DWORD	dwKiFastSystemCallReturn;


//HOOK后跳入的函数地址
DWORD dwHookSystemCall[0x4C8*10] = {0};

//保存原始函数调用地址
DWORD *dwOrigSystemCall[0x4C8*10] = {0};

//保存原始代码
BYTE	szCode[0xFF];

HookSystemCall::HookSystemCall(void)
{
	//Initialization();
}

HookSystemCall::~HookSystemCall(void)
{
	
}
__declspec(naked) void OrigCall()
{
	__asm  {
		lea esp,dword ptr ss:[esp]
		lea ecx,dword ptr ds:[ecx]
		mov edx,esp
		__emit 0x0F
		__emit 0x34
		retn
	}
}





__declspec(naked) void KiFastSystemCallEx()
{
	__asm  {
		mov edx,esp
		__emit 0x0F
		__emit 0x34
		retn
	}
}

//EX SystemCall
__declspec(naked) void SystemCall()
{
	__asm  {
		__emit 0x90
		mov eax,0x11B2
		call KiFastSystemCallEx
		retn 0xFF
		__emit 0x90
	}
}

DWORD HookFunctionAdress;
__declspec(naked) void KiFastSystemCall()
{
	
	__asm{
		pushad
			pushfd

			cmp         dword ptr [eax*4+dwHookSystemCall],0

			jz          Label
			popfd
			popad
			add         esp,4
			jmp         dword ptr [dwHookSystemCall+eax*4]

Label:

		push eax
		call HookFunctionAdress
		popfd
		popad
		jmp         KiFastSystemCallEx

	}
}


//初始化
BOOL HookSystemCall::Initialization(BOOL (__stdcall * myHookRet)(DWORD index))
{
	DWORD dwOldProtect;
	HMODULE hModule = GetModuleHandleA("NTDLL.dll");
	HookFunctionAdress = (DWORD)myHookRet; //设置回调函数了
	dwKiFastSystemCall = PtrToUlong(GetProcAddress(hModule, "KiFastSystemCall"));
	dwKiFastSystemCallReturn = dwKiFastSystemCall + 2;

	VirtualProtect(ULongToPtr(dwKiFastSystemCall - 0xA), 100, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	//保存原始代码
	memcpy(szCode,(VOID*)(dwKiFastSystemCall - 6),0xF);

	//	Hook KiFastSystemCall
	*(PWORD)ULongToPtr(dwKiFastSystemCall) = 0xF9EB;
	*(PBYTE)ULongToPtr(dwKiFastSystemCall - 0x5) = 0xE9;
	*(PDWORD)ULongToPtr(dwKiFastSystemCall - 0x4) = PtrToUlong(KiFastSystemCall) - (dwKiFastSystemCall - 0x5) - 5;
	
	VirtualProtect(ULongToPtr(dwKiFastSystemCall - 0xA), 100, dwOldProtect, &dwOldProtect);
	OutputDebugStringA("hook ok");

	return TRUE;
}

// 安装钩子
BOOL HookSystemCall::InstallHook(PCHAR FunName,		//NTAPI
								 DWORD pHookFunc,	//HOOK的函数
								 DWORD * pOrigFunc)	//返回的函数
{
	DWORD dwOldProtect;
	DWORD dwIndex = GetSysCallIndex(FunName);
	DWORD	dwFunadd =PtrToUlong(GetProcAddress(GetModuleHandleA("NTDLL.dll"), FunName));
	
	//保存各种信息
	dwHookSystemCall[dwIndex] = pHookFunc;
	dwOrigSystemCall[dwIndex] = (PDWORD)malloc(0x10);
	
	//将原本的ZW函数保存进去
	memcpy(dwOrigSystemCall[dwIndex],(PVOID)dwFunadd,0x10);
	
	//野猪改造开始
	VirtualProtect(ULongToPtr(dwOrigSystemCall[dwIndex]), 0x10, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	*(PBYTE)((DWORD)dwOrigSystemCall[dwIndex]	+ 5)	=	 0xE8;
	*(PDWORD)((DWORD)dwOrigSystemCall[dwIndex] + 6)		=	 (DWORD)KiFastSystemCallEx - (DWORD)dwOrigSystemCall[dwIndex] -0xA;
	*(PWORD)((DWORD)dwOrigSystemCall[dwIndex]	+ 10)	=	 0x9090;

	* pOrigFunc = (DWORD)dwOrigSystemCall[dwIndex];
	
	return 0;
}


// Ex安装钩子
BOOL HookSystemCall::InstallHookEx(DWORD dwIndex,		//NTAPI
								   WORD dwRetIndex,	//返回堆栈平衡
								   DWORD pHookFunc,		//HOOK的函数
								   DWORD * pOrigFunc)	//返回的函数
{
	DWORD dwOldProtect;

	//保存各种信息
	dwHookSystemCall[dwIndex] = pHookFunc;
	dwOrigSystemCall[dwIndex] = (PDWORD)malloc(0x10);

	//将原本的ZW函数保存进去
	memcpy(dwOrigSystemCall[dwIndex],(PVOID)SystemCall,0x10);

	//野猪改造开始
	VirtualProtect(ULongToPtr(dwOrigSystemCall[dwIndex]), 0x10, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	*(PDWORD)((DWORD)dwOrigSystemCall[dwIndex] + 2)		=	 (DWORD)dwIndex;
	*(PDWORD)((DWORD)dwOrigSystemCall[dwIndex] + 7)		=	 (DWORD)KiFastSystemCallEx - (DWORD)dwOrigSystemCall[dwIndex] - 0xB;
	*(PWORD)((DWORD)dwOrigSystemCall[dwIndex]	+ 12)	=	 dwRetIndex;

	* pOrigFunc = (DWORD)dwOrigSystemCall[dwIndex];

	return 0;
}

// 卸载HOOK
BOOL HookSystemCall::UnHook(PCHAR FunName)
{
	DWORD dwIndex = GetSysCallIndex(FunName);
	dwHookSystemCall[dwIndex] = NULL;
	free(dwOrigSystemCall[dwIndex]);
	return 0;
}

// 卸载系统钩子
BOOL HookSystemCall::UnSystemHook()
{
	DWORD dwOldProtect;

	DWORD dwSystemCall = dwKiFastSystemCallReturn-2;
	VirtualProtect(ULongToPtr(dwSystemCall - 6), 100, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy((VOID*)(dwSystemCall - 6),szCode,0xF);
	VirtualProtect(ULongToPtr(dwSystemCall - 6), 100, dwOldProtect, &dwOldProtect);
	OutputDebugStringA("unhook ok!");
	return TRUE;
}


//调用函数GetDllFuncAddr以便获得服务号
int HookSystemCall::GetSysCallIndex( PCHAR FuncName )
{
	DWORD          FuncAddr;    //函数地址
	int            SysCallIndex;//服务号
	FuncAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"),FuncName);
	SysCallIndex = *( (WORD*)(FuncAddr + 1) );
	return SysCallIndex;
}
