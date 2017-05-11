// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E2D2E678_255B_4C0F_BC0E_E6B372FDB78F__INCLUDED_)
#define AFX_STDAFX_H__E2D2E678_255B_4C0F_BC0E_E6B372FDB78F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "VMProtectSDK.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E2D2E678_255B_4C0F_BC0E_E6B372FDB78F__INCLUDED_)


#include <Windows.h>
#include <Winternl.h>

//全局引用的 系统头文件
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;





//全局引用的 自己定义头文件
#include "util.h"
#include "HookVt.h"
#include "HookMain.h"
#include "ConstDef.h"
#include "UpdateBase.h"
#include "MyMsg.h"

static bool IsOnly;
static std::set<DWORD> FilterSet;

static void AppendInfoToEdit(CEdit *pEdit, CString strBuf)
{
	CString strOld;
	pEdit->GetWindowTextW(strOld);
	strOld += strBuf;
	pEdit->SetWindowTextW(strOld);
}

//枚举类型的比较结果
enum COMPARESULT{
	COMPARE_LESS,
	COMPARE_EQUIL,
	COMPARE_BIG
};

//枚举类型的发送类型
enum SENDTYPE{
	TYPE_SEND,
	TYPE_RECEIVE,
	TYPE_ALL
};

//封包hook的相关配置参数
typedef struct _SEND_PACKAGE_HOOK_FILTER
{
	BOOL bWatchUDP ;
	BOOL bWatchTCP ;
	DWORD wUDPMinLen ;
	DWORD wTCPMinLen ;
	BOOL bWatchUDPToStack ;
	BOOL bWatchTCPToStack ;
	SENDTYPE eSendType;
	COMPARESULT eUDPCompare;
	COMPARESULT eTCPCompare;
}SENDHOOKFILTER, *PSENDHOOKFILTER;

typedef struct _REGISTER_FILTER
{
	BOOL bWatchEAX;
	BOOL bWatchEBX;
	BOOL bWatchECX;
	BOOL bWatchEDX;
	BOOL bWatchESI;
	BOOL bWatchEDI;
	BOOL bWatchPARAMS;
}REGISTERFILTER, *PREGISTERFILTER;


//去除编译警告
#pragma warning (disable: 4996)
#pragma warning (disable: 4309)