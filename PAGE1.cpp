// PAGE1.cpp : implementation file
//

#include "stdafx.h"
#include "DLL.h"
#include "PAGE1.h"

#include "PicUtil.h"
#include "HttpClient.h"
#include "HwndUtil.h"
#include "FileUtil.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define shellcodeLength  20
/////////////////////////////////////////////////////////////////////////////
// CPAGE1 dialog


CPAGE1::CPAGE1(CWnd* pParent /*=NULL*/)
	: CDialog(CPAGE1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPAGE1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwDisaptchMessage = 0;
}


void CPAGE1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAGE1)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_PAGE1, m_edit_page1);
}


BEGIN_MESSAGE_MAP(CPAGE1, CDialog)
	//{{AFX_MSG_MAP(CPAGE1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CPAGE1::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDC_BUTTON_HOOK, &CPAGE1::OnBnClickedButtonHook)
	ON_BN_CLICKED(IDC_BUTTON_UNHOOK, &CPAGE1::OnBnClickedButtonUnhook)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CPAGE1::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADDSHOW, &CPAGE1::OnBnClickedButtonAddshow)
	ON_BN_CLICKED(IDC_BUTTON_SUBSHOW, &CPAGE1::OnBnClickedButtonSubshow)
	ON_BN_CLICKED(IDC_CHECK_ONLY, &CPAGE1::OnBnClickedCheckOnly)
	ON_BN_CLICKED(IDC_BUTTON_PIC, &CPAGE1::OnBnClickedButtonPic)
	ON_BN_CLICKED(IDC_BUTTON_HWND, &CPAGE1::OnBnClickedButtonHwnd)
	ON_BN_CLICKED(IDC_BUTTON_MSG, &CPAGE1::OnBnClickedButtonMsg)
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE, &CPAGE1::OnBnClickedButtonMessage)
	ON_BN_CLICKED(IDC_BUTTON_START, &CPAGE1::OnBnClickedButtonStart)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPAGE1 message handlers


CHookVt hookVt;

void CPAGE1::OnBnClickedButtonChange()
{
	
	CString strTmp,strBuf;
	GetDlgItemText(IDC_EDIT_ADDRESS,strBuf);
	try
	{
		DWORD dwObject = stol(strBuf.GetBuffer(0),FALSE,16);  
		if (checkAddress(dwObject))
		{
			strTmp.Format(L"%08X",*(DWORD *)dwObject);
			SetDlgItemText(IDC_EDIT_VALUE,strTmp);
		}else
		{
			SetDlgItemText(IDC_EDIT_VALUE,L"UNREAD");
		}
	}catch (...){
		SetDlgItemText(IDC_EDIT_VALUE,L"ERROR");
		OutputDbgInfo("[!] 方法xxxxxxxx执行异常:%d\r\n",GetLastError());
	}
	
}


void CPAGE1::OnBnClickedButtonHook()
{
	
	try
	{
		CString strBuf;
		GetDlgItemText(IDC_EDIT_HOOKADDR,strBuf);
		DWORD dwObject = stol(strBuf.GetBuffer(0),FALSE,16);  
		if (checkAddress(dwObject))
		{
			
			hookVt.HookVirtualTable(dwObject);
			OutputDbgInfo("[!] 方法hook start :%x\r\n",dwObject);
		}else{
			SetDlgItemText(IDC_EDIT_HOOKADDR,L"UNREAD");
		}
	}
	catch (...){
		SetDlgItemText(IDC_EDIT_HOOKADDR,L"ERROR");
		OutputDbgInfo("[!] 方法OnBnClickedButtonHook执行异常\r\n");
	}

}


void CPAGE1::OnBnClickedButtonUnhook()
{
	try
	{
		CString strBuf;
		GetDlgItemText(IDC_EDIT_HOOKADDR,strBuf);
		DWORD dwObject = stol(strBuf.GetBuffer(0),FALSE,16);  
		if (checkAddress(dwObject))
		{
			hookVt.UnHookVirtualTable(dwObject);
			OutputDbgInfo("[!] 方法hook end :%x\r\n",dwObject);
		}else{
			SetDlgItemText(IDC_EDIT_HOOKADDR,L"UNREAD");
		}
	}
	catch (...){
		SetDlgItemText(IDC_EDIT_HOOKADDR,L"ERROR");
		OutputDbgInfo("[!] 方法OnBnClickedButtonUnhook执行异常\r\n");
	}
}




void CPAGE1::OnBnClickedButtonUpdate()
{
	UpdateBaseToFile();
}


void CPAGE1::OnBnClickedButtonAddshow()
{
	CString strBuf;
	GetDlgItemText(IDC_EDIT_SHOWOFFSET,strBuf);
	DWORD dwObject = stol(strBuf.GetBuffer(0),FALSE,16);  
	FilterSet.insert(dwObject);
}


void CPAGE1::OnBnClickedButtonSubshow()
{
	CString strBuf;
	GetDlgItemText(IDC_EDIT_SHOWOFFSET,strBuf);
	DWORD dwObject = stol(strBuf.GetBuffer(0),FALSE,16);  
	FilterSet.erase(dwObject);
}


void CPAGE1::OnBnClickedCheckOnly()
{
	if (((CButton *)GetDlgItem(IDC_CHECK_ONLY))->GetCheck())
	{
		IsOnly = TRUE;
	}
}

UINT _stdcall WINAPI AfterGetKey(CString str)
{
	AfxMessageBox(L"执行到了处理验证码！");
	AfxMessageBox(str);
	return 0;
}

void CPAGE1::OnBnClickedButtonPic()
{
	CPicUtil picUtil;
	CHwndUtil hwndUtil;
	HWND hwnd =  hwndUtil.GetHwndUnInjectByIni();;
	char path[MAX_PATH];
	//picUtil.screenPrint(hwnd, path, 33, 33, 33, 33);
	bool isOk = picUtil.GetScreenBmp(hwnd, path, 570, 0, 250, 140);
	if(isOk)
	{
		OutputDbgInfo("returnPath : %s ", path);
		CHttpClient httpClient;
		CString str(path);
		httpClient.DealCodeOnline(str, 100, AfterGetKey);
	}else
	{
		AfxMessageBox(L"截图失败！");
	}
}

void CPAGE1::OnBnClickedButtonHwnd()
{
	 CString strTmp, strBuf;
	

	 CHwndUtil hwndUtil;
	 HWND hwnd;
	 hwnd = hwndUtil.GetHwndInjectByIni();

	 strTmp.Format(L"注入方式获得的hwnd: %X\r\n",hwnd);
	 strBuf += strTmp;
	 hwnd = hwndUtil.GetHwndUnInjectByIni();
	 
	 strTmp.Format(L"未注入方式获得的hwnd: %X\r\n\r\n",hwnd);
	 strBuf += strTmp;
	
	 

	 HWND hwndArray[50];
	 int size;
	 DWORD pid = GetCurrentProcessId();
	 hwndUtil.GetHwndByPid(pid, hwndArray, &size);

	 for (int i=0; i<size; i++)
	 {
		 strTmp.Format(L"%d -> hwnd: 0x%08X\r\n", i, hwndArray[i]);
		 strBuf += strTmp;
	 }
	 SetDlgItemText(IDC_EDIT_PAGE1,strBuf);
}

void CPAGE1::OnBnClickedButtonMsg()
{
	CString strBuf, strX, strY;
	GetDlgItemText(IDC_EDIT_SHOWOFFSET,strBuf);
	GetDlgItemText(IDC_EDIT_POINTX,strX);
	GetDlgItemText(IDC_EDIT_POINTY,strY);
	DWORD dwObject = 0,x = 0, y = 0;
	try
	{
		dwObject = stol(strBuf.GetBuffer(0),FALSE,16);  
		x = stol(strX.GetBuffer(0),FALSE,10);  
		y = stol(strY.GetBuffer(0),FALSE,10);  
	}
	catch (...)
	{
		OutputDebugStringA("[!] [!] 数值转换异常");
	}
	if (!dwObject)
	{
		return;
	}
	CMyMsg myMsg((HWND)dwObject);

	strBuf.Format(_T(" 在%d - %d 处点击。 "), x, y);
	 SetDlgItemText(IDC_EDIT_PAGE1,strBuf);
	POINT point;
	point.x = x;
	point.y = y;

	myMsg.MoveMouseTo(&point);
	myMsg.LeftClick(&point);

}


void CPAGE1::OnBnClickedButtonMessage()
{
	CString strBuf, strX, strY;
	GetDlgItemText(IDC_EDIT_SHOWOFFSET,strBuf);
	GetDlgItemText(IDC_EDIT_POINTX,strX);
	GetDlgItemText(IDC_EDIT_POINTY,strY);
	DWORD dwObject = 0,x = 0, y = 0;

	dwObject = ConverToHex(strBuf, 16);  
	x = ConverToHex(strX, 16);  
	y = ConverToHex(strY, 16);  

	CMyMsg myMsg((HWND)dwObject);

	strBuf.Format(_T(" 在%d - %d 处点击。 "), x, y);
	SetDlgItemText(IDC_EDIT_PAGE1,strBuf);
	POINT point;
	point.x = x;
	point.y = y;

	myMsg.MoveMouseTo(&point);
	myMsg.LeftClick(&point);


	//::PostMessageA((HWND)dwObject, WM_LBUTTONDOWN, (LPARAM)&point, (LPARAM)&point);

	//::PostMessageA((HWND)dwObject, WM_LBUTTONUP, (LPARAM)&point,  (LPARAM)&point);
}

DWORD dwObject;
UINT WINAPI test(PVOID param)
{
	CMyMsg myMsg((HWND)dwObject);


	myMsg.LeftClick(0.45, 0.05);
		Sleep(1000);
	myMsg.LeftClick(0.39, 0.19);
		Sleep(300);
	myMsg.LeftClick(0.58, 0.19);
	Sleep(300);
	myMsg.LeftClick(0.74, 0.19 + 0.055);
		Sleep(3000);

	myMsg.LeftClick(0.59, 0.895);
	return 0;

}

HANDLE scriptThreadId2;
void CPAGE1::OnBnClickedButtonStart()
{
	CString strBuf, strX, strY;
	GetDlgItemText(IDC_EDIT_SHOWOFFSET,strBuf);

	dwObject = ConverToHex(strBuf, 16);  


	scriptThreadId2 = (HANDLE)_beginthreadex(NULL,0,test,0,0,0);
	// 0.45 * 0.05
	//0.39 * 0.19
	// 0.58 * 0.19 
	// 0.74 * 0.19
	// 0.59*0.895
	

	//afxcreateth
}
