//////////exediy 8763232  2014年7月6日
#pragma once


class HookSystemCall
{
public:
	HookSystemCall(void);
	~HookSystemCall(void);
	
	//初始化
	BOOL Initialization(BOOL (__stdcall * myHookRet)(DWORD index));

	//调用函数GetDllFuncAddr以便获得服务号
	int GetSysCallIndex( PCHAR FuncName );

	// 安装钩子
	BOOL InstallHook(PCHAR FunName,			//NTAPI
					 DWORD pHookFunc,		//HOOK的函数
					 DWORD * pOrigFunc);	//返回的函数

	// Ex安装钩子
	BOOL InstallHookEx(DWORD dwIndex,		//NTAPI
						WORD dwRetIndex,	//返回堆栈平衡
						DWORD pHookFunc,		//HOOK的函数
						DWORD * pOrigFunc);	//返回的函数
	// 卸载HOOK
	BOOL UnHook(PCHAR FunName);

	// 卸载系统钩子
	BOOL UnSystemHook();

};

