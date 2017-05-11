#include "stdafx.h"
#include "IOMsg.h"
#include "winIO/winio.h"

CIOMsg::CIOMsg(void)
{
	KBC_KEY_CMD = 0x64;  
	KBC_KEY_DATA = 0x60;  
}

CIOMsg::~CIOMsg(void)
{
}

void CIOMsg::KBCWait4IBE()  
{  
	DWORD dwVal = 0;  
	do  
	{  
		bool flag = GetPortVal(0x64, &dwVal, 1);  
	}  
	while ((dwVal & 0x2) > 0);  
}  

void CIOMsg::KeyPressDown(int vKeyCoad)  
{  
	int btScancode = 0;  
	btScancode = MapVirtualKey((byte)vKeyCoad, 0);  
	KBCWait4IBE();  
	SetPortVal(KBC_KEY_CMD, 0xD2, 1);  
	KBCWait4IBE();  
	SetPortVal(KBC_KEY_DATA, 0xe2, 1);  
	KBCWait4IBE();  
	SetPortVal(KBC_KEY_CMD, 0xD2, 1);  
	KBCWait4IBE();  
	SetPortVal(KBC_KEY_DATA, btScancode, 1);  
}  

void CIOMsg::KeyPressUp(int vKeyCoad)  
{  
	int btScancode = 0;  
	btScancode = MapVirtualKey((byte)vKeyCoad, 0);  
	KBCWait4IBE();  
	SetPortVal(KBC_KEY_CMD, 0xD2, 1);  
	KBCWait4IBE();  
	SetPortVal(KBC_KEY_DATA, 0xe0, 1);  
	KBCWait4IBE();  
	SetPortVal(KBC_KEY_CMD, 0xD2, 1);  
	KBCWait4IBE();  
	SetPortVal(KBC_KEY_DATA, btScancode, 1);  
}  

void CIOMsg::KeyPress(int vKeyCoad)
{
	KeyPressDown(vKeyCoad);
	KeyPressUp(vKeyCoad);
}

void CIOMsg::KeyPress(int vKeyCoad, int times)
{
	for (int i=0; i<times; i++)
	{
		KeyPress(vKeyCoad);
	}
}









