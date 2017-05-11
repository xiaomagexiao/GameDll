#include "stdafx.h"

#include "StdAfx.h"
#include "HookMain.h"


CHookMain::CHookMain(void)
{
}


CHookMain::~CHookMain(void)
{
}

void CHookMain::hookExecute(DWORD hookAddress,DWORD functionAddress)
{
	if(checkAddress(hookAddress)&&checkAddress(functionAddress))
	{
		if(map.find(hookAddress)!=map.end())
		{
			OutputDbgInfo("[!] hookAddress:%08X 已经被hook过！",hookAddress);
			return;
		}
		HOOKHEAD hookHead;
		hookHead.hookAddress = hookAddress;
		CopyMemory(hookHead.reset,(LPVOID)hookAddress,5);
		hookHead.jmp[0]=0xe9;
		*(DWORD *)(hookHead.jmp+1)=functionAddress - hookAddress - 5;
		writeMemoryStatic((LPVOID)hookAddress,hookHead.jmp,5);
		map[hookAddress]=hookHead;
	}else
	{
		OutputDbgInfo("[!] 传入的参数不正确! hookAddress:%08X functionAddress:%08X",hookAddress,functionAddress);
	}
}

void CHookMain::unHookExecute(DWORD hookAddress)
{
	if(checkAddress(hookAddress))
	{
		if(map.find(hookAddress)==map.end())
		{
			OutputDbgInfo("[!] hookAddress:%08X 没有被hook！",hookAddress);
			return;
		}
		HOOKHEAD hookHead = map[hookAddress];
		writeMemoryStatic((LPVOID)hookAddress,hookHead.reset,5);
		map.erase(hookAddress);
	}else
	{
		OutputDbgInfo("[!] 传入的参数不正确! hookAddress:%08X ",hookAddress);
	}
}