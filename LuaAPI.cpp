#include "stdafx.h"
#include "LuaAPI.h"
#include "MyMsg.h"
extern CMyMsg g_msg;
CLuaAPI::CLuaAPI(void)
{

}

CLuaAPI::~CLuaAPI(void)
{
}

VOID CLuaAPI::MoveTo(DWORD clientX,DWORD clientY)
{
	POINT point;
	point.x = clientX;
	point.y = clientY;
	g_msg.MoveMouseTo(&point);
}

VOID CLuaAPI::LeftClick(DWORD clientX,DWORD clientY)
{
	POINT point;
	point.x = clientX;
	point.y = clientY;
	g_msg.LeftClick(&point);
}

VOID CLuaAPI::RightClick(DWORD clientX,DWORD clientY)
{
	POINT point;
	point.x = clientX;
	point.y = clientY;
	g_msg.RightClick(&point);
}

VOID CLuaAPI::LeftDown(DWORD clientX,DWORD clientY)
{
	POINT point;
	point.x = clientX;
	point.y = clientY;
	g_msg.LeftDown(&point);
}

VOID CLuaAPI::LeftUp(DWORD clientX,DWORD clientY)
{
	POINT point;
	point.x = clientX;
	point.y = clientY;
	g_msg.LeftUp(&point);
}

VOID CLuaAPI::RightDown(DWORD clientX,DWORD clientY)
{
	POINT point;
	point.x = clientX;
	point.y = clientY;
	g_msg.RightDown(&point);
}

VOID CLuaAPI::RightUp(DWORD clientX,DWORD clientY)
{
	POINT point;
	point.x = clientX;
	point.y = clientY;
	g_msg.RightUp(&point);
}