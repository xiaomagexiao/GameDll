#pragma once
class CLuaAPI
{
public:
	CLuaAPI(void);
	~CLuaAPI(void);
	/**************************************
	LeftClick 1
	RightClick 1
	LeftDoubleClick 1
	LeftDown 1
	LeftUp 1
	RightDown 1
	RightUp 1
	****************************************/
	VOID MoveTo(DWORD clientX,DWORD clientY);

	/**************************************
	*  功能：左键点击
	****************************************/
	VOID LeftClick(DWORD clientX,DWORD clientY);

	/**************************************
	*  功能：右键点击
	****************************************/
	VOID RightClick(DWORD clientX,DWORD clientY);

	/**************************************
	*  功能：左键点击
	****************************************/
	VOID LeftDown(DWORD clientX,DWORD clientY);

	/**************************************
	*  功能：左键弹起
	****************************************/
	VOID LeftUp(DWORD clientX,DWORD clientY);

	/**************************************
	*  功能：右键摁下
	****************************************/
	VOID RightDown(DWORD clientX,DWORD clientY);

	/**************************************
	*  功能：右键弹起
	****************************************/
	VOID RightUp(DWORD clientX,DWORD clientY);
};

