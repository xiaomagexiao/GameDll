#include "stdafx.h"
#include "MyMsg.h"


CMyMsg::CMyMsg(void)
{
	Init();
}

CMyMsg::CMyMsg(HWND hwnd)
{
	this->m_hwnd = hwnd;
	Init();
}

CMyMsg::~CMyMsg(void)
{
}

void CMyMsg::LeftClick()
{
	POINT point;
	GetCursorPos(&point);
	LeftClick(&point);
}

void CMyMsg::LeftClick(PPOINT pPoint)
{
	MSG msg;
	msg.message = WM_LBUTTONDOWN;
	DefaultPositonHwndMsg(pPoint, &msg);
	Dispatch(&msg);

	msg.time = msg.time + 100;
	msg.message = WM_LBUTTONUP;
	Dispatch(&msg);
}

void CMyMsg::RightClick()
{
	MSG msg;
	msg.message = WM_RBUTTONDOWN;
	DefaultPositonHwndMsg(NULL, &msg);
	Dispatch(&msg);

	msg.time = msg.time + 120;
	msg.wParam = 0;
	msg.message = WM_RBUTTONUP;
	Dispatch(&msg);
}

void CMyMsg::RightClick(PPOINT pPoint)
{
	MSG msg;
	msg.message = WM_RBUTTONDOWN;
	DefaultPositonHwndMsg(pPoint, &msg);
	Dispatch(&msg);

	msg.time = msg.time + 120;
	msg.wParam = 0;
	msg.message = WM_RBUTTONUP;
	Dispatch(&msg);
}

void CMyMsg::LeftDown(PPOINT pPoint)
{
	MSG msg;
	msg.message = WM_LBUTTONDOWN;
	DefaultPositonHwndMsg(pPoint, &msg);
	Dispatch(&msg);
}

void CMyMsg::LeftUp(PPOINT pPoint)
{
	MSG msg;
	msg.message = WM_LBUTTONUP;
	DefaultPositonHwndMsg(pPoint, &msg);
	Dispatch(&msg);
}

void CMyMsg::RightDown(PPOINT pPoint)
{
	MSG msg;
	msg.message = WM_RBUTTONDOWN;
	DefaultPositonHwndMsg(pPoint, &msg);
	Dispatch(&msg);
}

void CMyMsg::RightUp(PPOINT pPoint)
{
	MSG msg;
	msg.message = WM_RBUTTONUP;
	DefaultPositonHwndMsg(pPoint, &msg);
	Dispatch(&msg);
}

void CMyMsg::MoveMouseTo(int x, int y)
{
	POINT point;
	point.x = x;
	point.y = y;
	MoveMouseTo(&point);
}

void CMyMsg::MoveMouseTo(PPOINT pPoint)
{
	if (pPoint == NULL)
	{
		return;
	}
	MSG msg;
	msg.message = WM_MOUSEMOVE;
	msg.hwnd = m_hwnd;
	msg.wParam=0;
	msg.lParam=0;
	msg.time=GetTickCount();
	msg.pt.x = pPoint->x;
	msg.pt.y = pPoint->y;
	Dispatch(&msg);
}

void CMyMsg::LeftDown()
{
	MSG msg;
	msg.message = WM_LBUTTONDOWN;
	DefaultPositonHwndMsg(NULL, &msg);
	Dispatch(&msg);
}

void CMyMsg::LeftUp()
{
	MSG msg;
	msg.message = WM_LBUTTONUP;
	DefaultPositonHwndMsg(NULL, &msg);
	msg.wParam = 0;
	Dispatch(&msg);
}

void CMyMsg::RightDown()
{
	MSG msg;
	msg.message = WM_RBUTTONDOWN;
	DefaultPositonHwndMsg(NULL, &msg);
	Dispatch(&msg);
}

void CMyMsg::RightUp()
{
	MSG msg;
	msg.message = WM_RBUTTONUP;
	DefaultPositonHwndMsg(NULL, &msg);
	msg.wParam = 0;
	Dispatch(&msg);
}

void CMyMsg::LeftClick(double preX, double preY)
{
	POINT point;
	point.x = CLIENT_WIDTH * preX;
	point.y = CLIENT_HEIGHT * preY;
	LeftClick(&point);
}

void CMyMsg::RightClick(double preX, double preY)
{
	POINT point;
	point.x = CLIENT_WIDTH * preX;
	point.y = CLIENT_HEIGHT * preY;
	RightClick(&point);
}

void CMyMsg::Init()
{
	 //初始化函数指针
	 pDispatchMessage = (MyDispatchMessage)getFunctionAddress("user32.dll","DispatchMessageW");
}

void CMyMsg::DefaultPositonHwndMsg(PPOINT pPoint, MSG *pmsg)
{
	if (pPoint == NULL)
	{
		POINT point;
		GetCursorPos(&point);
		pPoint = &point;
	}

	pmsg->lParam = (pPoint->y<<16) + pPoint->x;  //lParam如果为空就不会有效果

	if(ClientToScreen(m_hwnd,pPoint))
	{
		pmsg->pt.x = pPoint->x;
		pmsg->pt.y = pPoint->y;
	}else
	{
		pmsg->pt.x = 0;
		pmsg->pt.y = 0;
	}

	pmsg->hwnd = m_hwnd;
	pmsg->wParam=1;
	pmsg->time=GetTickCount();
}

void CMyMsg::Dispatch(MSG *pmsg)
{
	

		//dispatch消息
	//	DispatchMessageW(pmsg);
		//用 PostMessageW 可以实现  DispatchMessageW有时候会出现卡死的情况
		PostMessageW(pmsg->hwnd, pmsg->message, (WPARAM)pmsg->wParam, (LPARAM)pmsg->lParam);
		OutputDbgInfo("[!] HWAND: %08X  TYPE: %04X CODE: %04X  LPARAM: %08X TIME: %08X X: 0x%04d Y: %04d",
	    pmsg->hwnd, pmsg->message,pmsg->wParam,pmsg->lParam, pmsg->time, pmsg->pt.x,pmsg->pt.y);

}

void CMyMsg::SetHwnd(HWND newHwnd)
{
	this->m_hwnd = newHwnd;
}