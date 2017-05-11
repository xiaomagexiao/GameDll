#include "stdafx.h"
#include "HookCallBack.h"
#include "GetSSDTInformation.h"

extern GetSSDTInformation ssdtFunction;

char g_cSysCallIndex[2000];		//所有call的hash表，记录是否要过滤
BOOL g_bSysStartRecord = FALSE;		//是否记录
BOOL g_bSysWatchOne = FALSE;		//是否单个记录
int  g_iSysWatchIndex = 0;			//单个记录的编号
BOOL g_bSysRecordStack = FALSE;		//是否记录堆栈

BOOL _stdcall HookSystemCallBack(DWORD index)
{
	if (g_bSysStartRecord)
	{
		if (g_cSysCallIndex[index] == 1&&index<0x200)
		{
			if (g_bSysWatchOne && g_iSysWatchIndex!=index)//指定了监控指定的，不是指定的就返回
			{
				return TRUE;
			}
			char info[50];
			char temp[50];
			ssdtFunction.GetSSDTNameByIndex(temp, index);

			sprintf(info,"[!] FUNCTION: %04X -> %s",index, temp);
			OutputDebugStringA(info);
			if(g_bSysRecordStack)
			{
				DWORD ebpAddr=0;
				_asm
				{
					mov ebpAddr, ebp
				}
				sprintf(info,"%04X:%s",index, temp);
				saveStatckDump(ebpAddr, info);
			}
		}
	}

	return TRUE;
}

//需要监控的函数
void initCallIndex()
{
	return;
	g_cSysCallIndex[0x002C] = 1;     //ZwCallbackReturn       
//	g_cSysCallIndex[0x30] = 1;     //ZwCancelTimer          
//	g_cSysCallIndex[0x46] = 1;     //ZwCreateKey            
	g_cSysCallIndex[0x004F] = 1;     //ZwCreateProcess        
	g_cSysCallIndex[0x50] = 1;     //ZwCreateProcessEx      
	g_cSysCallIndex[0x57] = 1;     //ZwCreateThread         
	g_cSysCallIndex[0x58] = 1;     //ZwCreateThreadEx       
	g_cSysCallIndex[0x60] = 1;     //ZwDebugActiveProcess   
	g_cSysCallIndex[0x61] = 1;     //ZwDebugContinue        
//	g_cSysCallIndex[0x62] = 1;     //ZwDelayExecution       
//	g_cSysCallIndex[0x006B] = 1;     //ZwDeviceIoControlFile  
	//g_cSysCallIndex[0x006C] = 1;     //ZwDisableLastKnownGood 
	g_cSysCallIndex[0x00BE] = 1;     //ZwOpenProcess          
	g_cSysCallIndex[0x00BF] = 1;     //ZwOpenProcessToken     
	g_cSysCallIndex[0x00C0] = 1;     //ZwOpenProcessTokenEx   
	g_cSysCallIndex[0x00C4] = 1;     //ZwOpenSession          
	g_cSysCallIndex[0x00C6] = 1;     //ZwOpenThread           
	g_cSysCallIndex[0x00C7] = 1;     //ZwOpenThreadToken      
	g_cSysCallIndex[0x00C8] = 1;     //ZwOpenThreadTokenEx    
	g_cSysCallIndex[0x00C9] = 1;     //ZwOpenTimer            
	g_cSysCallIndex[0x162] = 1;     //ZwSetTimer             
	g_cSysCallIndex[0x163] = 1;     //ZwSetTimerEx           
	g_cSysCallIndex[0x016E] = 1;     //ZwSuspendProcess       
	g_cSysCallIndex[0x016F] = 1;     //ZwSuspendThread        
	g_cSysCallIndex[0x171] = 1;     //ZwTerminateJobObject   
	g_cSysCallIndex[0x172] = 1;     //ZwTerminateProcess     
	g_cSysCallIndex[0x173] = 1;     //ZwTerminateThread      
}

DWORD _stdcall HookCheckCallBack()
{
	OutputDebugStringA("执行到了检测函数，但是被俺拦截了！！！");
	return 0;
}
