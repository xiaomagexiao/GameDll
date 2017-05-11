#pragma once
typedef struct _Hook
{
	unsigned char jmp[5];
	unsigned char reset[5];
	DWORD hookAddress;
} HOOKHEAD;
class CHookMain
{
public:
	CHookMain(void);
	~CHookMain(void);
	void hookExecute(DWORD hookAddress,DWORD functionAddress);
	void unHookExecute(DWORD hookAddress);
private:
	map<DWORD,HOOKHEAD> map;

};

