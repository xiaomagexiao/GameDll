#include "stdafx.h"
#include "HookVt.h"
#include <stdarg.h>   
#include <stdio.h> 

CHookVt::CHookVt(void)
{

}

CHookVt::~CHookVt(void)
{

}
std::set<DWORD> S_OutSet;
void resetAddress(DWORD dwPlayerBaseAddr);

DWORD _stdcall Monitor(DWORD dwVirtualOffset,DWORD vtBaseAddr);
typedef DWORD (_stdcall *pMonitor)(DWORD,DWORD);
pMonitor g_pVirtualMonitor = Monitor;


VOID   Dispatch(DWORD dwVirtualOffset,DWORD vtBaseAddr);

/**************************************
*  功能：替换真虚表的线程回调
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/4/27/ Administrator]
****************************************/
DWORD WINAPI ThreadProc(LPVOID lpParameter){
	try{
		Sleep(500);
		//WaitForSingleObject(g_hEventUnhook, INFINITE);
		HOOK_VT_TYPE  *vtType = (HOOK_VT_TYPE*)lpParameter;
		//替换虚表
		vtType->g_pTrueVirtualTbl = (PVOID)(*(PDWORD)vtType->dwObject);	   //真虚表的基地址
		memcpy((PDWORD)vtType->g_pFakeVirtualTbl + 2, (PDWORD)vtType->g_pTrueVirtualTbl - 3, 12);
		OutputDbgInfo("[!] 方法[!] %08x %08x %08x %08x !\n",vtType->dwObject,vtType->g_pFakeVirtualTbl,vtType->g_pTrueVirtualTbl,vtType->hDstThread);
		*( PDWORD )vtType->dwObject = (DWORD)((PDWORD)vtType->g_pFakeVirtualTbl + 5);
		OutputDbgInfo("[!] 方法[!] 回调函数恢复线程!，修改后的值为：%08X\n",*( PDWORD )vtType->dwObject);
		//恢复线程 hDstThread
		ResumeThread(vtType->hDstThread);
	}catch( ... ){
		OutputDbgInfo("[!] 方法[!] ThreadProc Exception Error!\n");
		return false;
	}
	OutputDbgInfo("[!] 方法[!] ThreadProc Exit!\n");
	return true;
}

/**************************************
*  功能：解除替换真虚表的线程回调
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/4/27/ Administrator]
****************************************/
DWORD WINAPI ThreadProcUnHook(LPVOID lpParameter){
	HOOK_VT_TYPE  *vtType = (HOOK_VT_TYPE*)lpParameter;
	try{
		Sleep(500);
		*(PDWORD)vtType->dwObject = (DWORD)vtType->g_pTrueVirtualTbl;
		OutputDbgInfo("[!] 地址 %08X 从map中移除 。",vtType->dwObject);
		ResumeThread(vtType->hDstThread);
	}catch( ... ){
		OutputDbgInfo("[!] 方法[!] ThreadProc Exception Error!\n");
		return false;
	}
	OutputDbgInfo("[!] 方法[!] ThreadProc Exit!\n");
	return true;
}

/**************************************
*  功能：Hook虚表
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/4/27/ Administrator]
****************************************/
BOOL CHookVt::HookVirtualTable(DWORD dwPlayerBaseAddr)
{
	PVOID pFakeVTbl = NULL;
	HANDLE hNewThread = NULL;
	bool   bSuccess = false;
	PDWORD pdwFakeVtbl = NULL;
	
	if(vtMap.find(dwPlayerBaseAddr)!=vtMap.end())
	{
		OutputDbgInfo("[!] 方法[!] 对象：%08X 已经被HOOK了！ \n",dwPlayerBaseAddr);
		return FALSE;
	}
	OutputDbgInfo("[!] 方法[!] HookVirtualTable Enter!\n");
	HOOK_VT_TYPE vtType;
	vtType.dwObject = dwPlayerBaseAddr;
//	resetAddress(dwPlayerBaseAddr);
	try
	{
		//同步HOOK和UNHOOK
        vtType.g_hEventUnhook = CreateEvent(NULL, FALSE, TRUE, NULL);
		if ( !vtType.g_hEventUnhook )
		{
			OutputDbgInfo("[!] 方法[-] HookVirtualTable CreateEvent fail!");
			return FALSE;
		}
		
		//创建假的虚表
		PVOID pFakeVTbl = CreateFakeVirtualTable(dwPlayerBaseAddr);
		if ( !pFakeVTbl )
		{
			OutputDbgInfo("[!] 方法[-] HookVirtualTable CreateFakeVirtualTable Error!");
			return bSuccess;
		}
		vtType.g_pFakeVirtualTbl = pFakeVTbl;
		
		pdwFakeVtbl = (PDWORD)pFakeVTbl;
		*(pdwFakeVtbl + 1) = dwPlayerBaseAddr;
		

		DWORD  dwDstThread =getThreadId();
	
		cout<<"dwDstThread2"<<hex<<(int)dwDstThread<<endl;

		//悬挂线程
		vtType.hDstThread  = OpenThread(THREAD_SUSPEND_RESUME, NULL, dwDstThread);
		if ( !vtType.hDstThread)
		{
			OutputDbgInfo("[!] 方法[!] ThreadProc OpenThread error!\n");
			return false;
		}
		//创建线程来替换真虚表
		HANDLE hNewThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc,&vtType, 0, NULL);
		if (!hNewThread)
		{
			OutputDbgInfo("[!] 方法[-] CreateThread Error!");
			return bSuccess;
		}
		OutputDbgInfo("[!] 方法[-] 开始挂起线程!");
		if( (DWORD)-1 == SuspendThread(vtType.hDstThread))
		{  
			OutputDbgInfo("[!] 方法[!] ThreadProc SuspendThread error!\n");
			return false;
		}
		vtMap[dwPlayerBaseAddr]=vtType;
		OutputDbgInfo("[!] 方法[!] %08x %08x %08x %08x !\n",vtType.dwObject,vtType.g_pFakeVirtualTbl,vtType.g_pTrueVirtualTbl,vtType.hDstThread);
	}
	catch( ... )
	{
		OutputDbgInfo("[!] 方法[!] HookVirtualTable Exception Exit!\n");
		return NULL;
	}
	
	OutputDbgInfo("[!] 方法[!] HookVirtualTable Exit!\n");
	bSuccess = TRUE;
	return bSuccess;
}


/**************************************
*  功能：unHOOK
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/5/17/ Administrator]
****************************************/
VOID CHookVt::UnHookVirtualTable(DWORD dwPlayerBaseAddr){
	if(vtMap.find(dwPlayerBaseAddr)==vtMap.end())
	{
		OutputDbgInfo("[!] 坏了，地址 %08X 没有在map里==。",dwPlayerBaseAddr);
		return;
	}
	//悬挂线程
	HANDLE hDstThread  = OpenThread(THREAD_SUSPEND_RESUME, NULL, getThreadId());
	vtMap[dwPlayerBaseAddr].hDstThread = hDstThread;
	//创建线程来替换真虚表
	HANDLE hNewThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProcUnHook,&vtMap[dwPlayerBaseAddr], 0, NULL);
	if (!hNewThread)
	{
		OutputDbgInfo("[!] 方法[-] CreateThread Error!");
		return ;
	}

	OutputDbgInfo("[!] 方法[-] 开始挂起线程--取消hook!");
	
	if( (DWORD)-1 == SuspendThread(hDstThread))
	{  
		OutputDbgInfo("[!] 方法[!] ThreadProc SuspendThread error!\n");
		return ;
	}
	vtMap.erase(dwPlayerBaseAddr);
}

/**************************************
*  功能：针对所有虚表函数的分发函数
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/4/27/ Administrator]
****************************************/
VOID  __declspec(naked) Dispatch(DWORD offsetAddr, DWORD vtBaseAddr)
{
	
	//转向Monitor函数，实现你想实现的功能,返回值为真正的虚函数地址
    _asm
	{
		call  DWORD PTR [g_pVirtualMonitor]
	}

	//分发对真实虚表的调用或过滤掉该调用
	_asm
	{
		cmp eax, 0
		jz _NO_EXCUTE_VIRTUAL_FUC

		//进行分发到对应的虚函数，eax是真实虚函数的地址
		mov [esp+36], eax
		popfd
		popad
        ret

		//不执行该虚函数,由于不知道虚函数的参数个数，暂不支持
		//要支持的话就必须动态解析虚函数，看函数末尾的C3或C2来决定
        _NO_EXCUTE_VIRTUAL_FUC:
		popfd
		popad
		lea esp, dword ptr [esp+4]
		ret
	}
}
int justOneTime;
bool firstTime = TRUE;
std::set<DWORD> bset ;
DWORD _stdcall Monitor(DWORD dwVirtualOffset,DWORD vtBaseAddr)
{
	DWORD ebpAddr,dwCallerRetAddr;
	DWORD callAddress = *(DWORD *)(vtBaseAddr + dwVirtualOffset * 4);
	_asm
	{
		mov ebpAddr,ebp
			mov eax, [ebp+56]
		mov dwCallerRetAddr, eax
	}
	if (FilterSet.find(dwVirtualOffset*4)!=bset.end())
	{
		goto showDetail;
	}else if (IsOnly)
	{
		return callAddress;
	}

	if (firstTime)
	{
		justOneTime = GetTickCount();
		firstTime = FALSE;
	}
	if (bset.find(dwVirtualOffset)!=bset.end())
	{
		return callAddress;
	}

	if (GetTickCount()-justOneTime<5000)
	{
		bset.insert(dwVirtualOffset);
		OutputDbgInfo("[!] 放入黑名单的索引号：%04d  ",dwVirtualOffset);
		return  callAddress;
	}
	showDetail:
	DWORD param1,param2,param3,param4;
	_asm
	{
		mov eax, [ebp+60]
		mov param4, eax

			mov eax, [ebp+64]
		mov param3, eax

			mov eax, [ebp+68]
		mov param2, eax

			mov eax, [ebp+72]
		mov param1, eax
	}

	OutputDbgInfo("[!] 参数：offset: %04x p1: %08x p2: %08x p3: %08x p4: %08x  %08x -> %08x\n",dwVirtualOffset * 4,param1,param2,param3,param4,callAddress,dwCallerRetAddr);
	saveStatckDump(callAddress,ebpAddr);
		
	return callAddress;
}

/**************************************
*  功能：针对所有虚表函数的监控函数
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/4/27/ Administrator]
***************************************
DWORD _stdcall Monitor(DWORD dwVirtualOffset)
{
	DWORD dwResult = 0;
	DWORD dwCalledFuncAddr, dwCallerRetAddr,dwEbpAddr;
	DWORD param1,param2,param3,param4;
	try
	{
        dwResult = (DWORD)g_pTrueVirtualTbl + dwVirtualOffset * 4;
		dwCalledFuncAddr = *(DWORD *)dwResult;
		
		_asm
		{
			mov eax, [ebp+52]
			mov dwCallerRetAddr, eax
			mov dwEbpAddr,ebp
		}
		if(firstTime){
			justOneTime =  GetTickCount();
			firstTime=false;
		}
		if(bset.find(dwVirtualOffset)!=bset.end()){
			return dwCalledFuncAddr;
		}
		if(GetTickCount()-justOneTime<5000){//5s中之内的全部放入大黑名单
			bset.insert(dwVirtualOffset);
			OutputDbgInfo("[!] 偏移：0x%04x 调用地址：0x%08x 放入黑名单！\n",dwVirtualOffset * 4,dwCalledFuncAddr);
			return dwCalledFuncAddr;
		}
		if ( isAll||objOffset.find(dwVirtualOffset * 4) !=objOffset.end())
		{
			OutputDbgInfo("[!] EBP地址：0x%08x ！\n",dwEbpAddr);
			saveStatckDump(dwEbpAddr,TRUE);
		}
		_asm
			{
				mov eax, [ebp+56]
				mov param4, eax

					mov eax, [ebp+60]
				mov param3, eax

					mov eax, [ebp+64]
				mov param2, eax

					mov eax, [ebp+68]
				mov param1, eax
			}

			OutputDbgInfo("[!] 参数：offset: %04x p1: %08x p2: %08x p3: %08x p4: %08x  %08x -> %08x\n",dwVirtualOffset * 4,param1,param2,param3,param4,dwCalledFuncAddr,dwCallerRetAddr);
		
	}catch( ... )
	{
		OutputDbgInfo("[!] 方法[!] Monitor Exception Exit!\n");
		return dwCalledFuncAddr;
	}
	return dwCalledFuncAddr;
}
*/

/**************************************
*  功能：创建假的续表
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/4/27/ Administrator]
****************************************/
PVOID  CHookVt::CreateFakeVirtualTable(DWORD dwPlayerBaseAddr)
{
	PVOID  pFakeVTbl    = NULL, pJmpTable = NULL;
	PDWORD pTrueVTbl = NULL;
	PDWORD pdwFakeVTblItem = NULL;
	PJMP_TABLE_ITEM pJmpTblItem = NULL;
    DWORD dwPlayActObj = 0, dwVtbLenth = 0;
	char  szOut[MAX_PATH];

	//声明接收函数地址指针变量
	typedef VOID (*pMonitorFn)(DWORD,DWORD);
	pMonitorFn pMonFn;
	pMonFn     = Dispatch;
    PDWORD pdwMonitorFn   = (PDWORD)pMonFn;
    
	OutputDbgInfo("[!] 方法[!] CreateFakeVirtualTable Enter!\n");
	PVOID g_pFakeVirtualTbl = 0;
	try
	{

		dwPlayActObj = dwPlayerBaseAddr;
        pTrueVTbl    = (PDWORD)(*( PDWORD )dwPlayActObj);
		
		dwVtbLenth = 40000;
		sprintf(szOut, "Player Addr: 0x%.8x, True VTB:0x%.8x, VTbLen: %d\n", dwPlayActObj, pTrueVTbl, dwVtbLenth);
		AddLB(szOut);

		pJmpTable = VirtualAlloc(NULL, dwVtbLenth * JMP_TABLE_ITEM_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if ( !pJmpTable )
		{
			OutputDbgInfo("[!] 方法[!] CreateFakeVirtualTable MMalloc Jmp Table fail!\n");
		    return NULL;
		}
		cout<<"跳转表pJmpTable地址： 0x"<<hex<<pJmpTable<<endl;
		//分配假虚表
		pFakeVTbl = VirtualAlloc(NULL, (dwVtbLenth + 1 ) * 4 + 16, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if ( !pFakeVTbl )
		{
			OutputDbgInfo("[!] 方法[!] CreateFakeVirtualTable MMalloc Fake Virtual Table fail!\n");
			if ( pJmpTable )
			{
				MMFree( pJmpTable );
			}
		    return NULL;
		}
		cout<<"虚假表pFakeVTbl地址： 0x"<<hex<<pFakeVTbl<<endl;
		//初始化两张表
		ZeroMemory(pJmpTable, JMP_TABLE_ITEM_SIZE * dwVtbLenth);
		ZeroMemory(pFakeVTbl, (dwVtbLenth + 1) * 4 + 16);
        
		g_pFakeVirtualTbl  = pFakeVTbl;

		//假虚表的头四字节保存跳转表的基地址
		*(PDWORD)pFakeVTbl = (DWORD)pJmpTable;

		pdwFakeVTblItem = (PDWORD)pFakeVTbl;
		pdwFakeVTblItem += 5;

		pJmpTblItem     = (PJMP_TABLE_ITEM)pJmpTable;
		for ( DWORD dwLoop = 0; dwLoop < dwVtbLenth; dwLoop++ )
		{
             pJmpTblItem->uPushEbx = 0x53;
			 pJmpTblItem->uPushad = 0x60;
			 pJmpTblItem->uPushfd = 0x9C;
			 pJmpTblItem->uMovEaxFirst[0]=0xB8;
			  *((DWORD *)&(pJmpTblItem->uMovEaxFirst[1])) = (DWORD)pTrueVTbl;
			 pJmpTblItem->uPushEaxFirst= 0x50;
			 pJmpTblItem->uMovEax[0] = 0xB8;
			 *((DWORD *)&(pJmpTblItem->uMovEax[1])) = dwLoop;
			 pJmpTblItem->uPushEax = 0x50;
			 pJmpTblItem->uJmp[0]  = 0xE9;
             *((DWORD *)&(pJmpTblItem->uJmp[1])) = (DWORD)(pdwMonitorFn) - ( (DWORD)(pJmpTblItem) + FIELD_OFFSET(JMP_TABLE_ITEM, uSave) );
			 

			 *pdwFakeVTblItem = (DWORD)pJmpTblItem;

			 pdwFakeVTblItem++;
			 pJmpTblItem++;
		}
	    FlushInstructionCache( GetCurrentProcess(), pJmpTable, dwVtbLenth *  JMP_TABLE_ITEM_SIZE);//缓存里的数据重写回主内存里
		FlushInstructionCache( GetCurrentProcess(), pFakeVTbl, (dwVtbLenth + 1) * 4 );

	}
	catch( ... )
	{
       OutputDbgInfo("[!] 方法[!] CreateFakeVirtualTable Exception Exit!\n");

	   if (pJmpTable)
	   {
		   MMFree(pJmpTable);
	   }

	   if (pFakeVTbl)
	   {
           MMFree(pFakeVTbl);
	   }
	   return NULL;
	}

	OutputDbgInfo("[!] 方法[!] CreateFakeVirtualTable Exit!\n");
	return g_pFakeVirtualTbl;
}




//下面4个为为实现函数
void AddLB( char szOut[]){};
void MMFree(PVOID pJmpTable){};
PDWORD MMalloc(DWORD dw){return NULL;};
/*
void resetAddress(DWORD dwPlayerBaseAddr){
	//bset.insert(0xa0);
	//bset.insert(0x54);
	vector<VTINFO> vec;
	vector<VTINFO>::iterator iter;
	DecVtAddress(dwPlayerBaseAddr,0xa00/4,vec);
	for (iter = vec.begin();iter!=vec.end();iter++)
	{
		if (iter->type==FUNCTION_GET||iter->type==FUNCTION_UNIMPORTENT)
		{
			bset.insert(iter->offset/4);
		}
	}
	return;

	//bset.insert(0x934/4);//鼠标移动		65e19c0  200  1b  305  9405d0->8ef39d
	bset.insert(0xa80/4);//窗口获得焦点	a80  0  608aa  8ef546  2  93fb30->8eef70 
	bset.insert(0xa20/4);//左键摁下		25faa5c0  c4000b0  354f0640  8  940930->116ec0b
	bset.insert(0x938/4);//左键弹起		65e19c0   202      171      181  940780->8ef2a4
	bset.insert(0xa24/4);//左键弹起		25faa5c0  c4000b0  354f0640  8  940960->116ec29
	bset.insert(0xa28/4);//左键弹起		25faa5c0  c4000b0  354f0640  8  9480e0->116ecdc
	bset.insert(0xa2c/4);//右键弹起		25faa5c0  c4000b0  354f0640  8  940990->116ed56
	bset.insert(0x1bc/4);//右键弹起		3d1b2f80  14  95094ea1  3d1b2f80  ee6b60->b52864

	bset.insert(0x604/4);//改变坐标		381b7000  1c  147  316  ad9630->ad4a4c 
	bset.insert(0xf0/4);//改变坐标		12d26c  0  147  316  aec2d0->a9ff77 
	bset.insert(0x9c/4);//改变坐标		4ba08090  0  147  316  a76b00->b12212 
}*/

void resetAddress(DWORD dwPlayerBaseAddr){}