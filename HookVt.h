#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
using  namespace std;
#pragma once
//跳转表项的结构
typedef struct _JMP_TABLE_ITEM
{
	UCHAR    uPushEbx;    //push ebx 0x53, 用来存放真正虚函数的地址
	UCHAR    uPushad;     //pushad 0x60
	UCHAR    uPushfd;     //pushfd 0x9C
	UCHAR    uMovEaxFirst[5];  //mov eax, 0xffffffff = B8 ffffffff
	UCHAR    uPushEaxFirst;    //push eax = 虚表的编号
	UCHAR    uMovEax[5];  //mov eax, 0xffffffff = B8 ffffffff
	UCHAR    uPushEax;    //push eax = 虚表的编号
	UCHAR    uJmp[5];     //jmp 0xfffffffe = e9...
	UCHAR    uSave;       //保留以后用
}JMP_TABLE_ITEM, *PJMP_TABLE_ITEM;

typedef struct _HOOK_VT_TYPE
{
	DWORD    dwObject;         //对象地址
	DWORD    monitorFunction;  //
	PVOID	 g_pTrueVirtualTbl;//原来的虚表地址
	PVOID	 g_pFakeVirtualTbl;//假虚表地址
	HANDLE   hDstThread;
	HANDLE   g_hEventUnhook ;
	int justOneTime;
	bool firstTime;
} HOOK_VT_TYPE;

class CHookVt{
public:
	CHookVt(void);
	~CHookVt(void);
	/**************************************
	*  功能：Hook虚表
	****************************************/
	BOOL HookVirtualTable(DWORD dwPlayerBaseAddr);

	/**************************************
	*  功能：unHOOK
	****************************************/
	VOID UnHookVirtualTable(DWORD dwPlayerBaseAddr);
public:
	
	

private:
	map<DWORD,HOOK_VT_TYPE> vtMap;
	/**************************************
	*  功能：创建假的续表
	****************************************/
	PVOID CreateFakeVirtualTable(DWORD dwPlayerBaseAddr);


	std::map<int,PVOID> addressMap;

	



};












//下面4个为为实现函数
void AddLB( char szOut[]);
void MMFree(PVOID pJmpTable);
PDWORD MMalloc(DWORD dw);
//分发到真正的虚表函数
VOID   Dispatch(DWORD dwVirtualOffset);
BOOL HookVirtualTable(DWORD dwPlayerBaseAddr);
VOID   ProcessVirtualFuncCall(DWORD dwVFOffset, DWORD dwCalledAddr, DWORD dwCallerAddr);
VOID UnHookVirtualTable(DWORD dwPlayerBaseAddr);
bool HookVirtualIndex(int dwPlayerBaseAddr,int index);

DWORD _stdcall MonitorSingle();
VOID UnHookVirtualIndex(int dwPlayerBaseAddr,int index);



#define  JMP_TABLE_ITEM_SIZE sizeof(JMP_TABLE_ITEM)

//创建假的虚表
PVOID CreateFakeVirtualTable(DWORD dwPlayerBaseAddr);
//统计虚表的调用
enum
{
	INIT_CALL_LOG_MEM = 1,
	UNINIT_CALL_LOG_MEM,
	OPEN_CALL_LOG,
	CLOSE_CALL_LOG
};
static BYTE   g_bVCallStatus = UNINIT_CALL_LOG_MEM;
const  DWORD  CALL_LOG_MEM_SIZE = 1024 * 4 * 2;
static PBYTE  g_pbCallLogMem  = NULL;
VOID   ProcessVirtualFuncCall(DWORD dwVFOffset, DWORD dwCalledAddr, DWORD dwCallerAddr); // 保存唯一性的调用记录
VOID   InitVirtualFuncCall(); // 初始化数组用来保存记录
VOID   OpenVirtualFuncCall(); // 打开调用记录过滤掉记录里的调用
VOID   CloseVirtualFuncCall(); // 关闭调用记录的应用