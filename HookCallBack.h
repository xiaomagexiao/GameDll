#ifndef _HOOK_CALLBACK_HEADER_
#define _HOOK_CALLBACK_HEADER_

/**************************************
*  功能：Hook Systemcall 的过滤函数 
*  参数：
*
*  返回值：
*  时间：[2015/1/31 14:04 Administrator]
****************************************/
BOOL _stdcall HookSystemCallBack(DWORD index);

void initCallIndex();

DWORD _stdcall HookCheckCallBack();



#endif