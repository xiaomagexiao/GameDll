// DLL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "DLL.h"
#include "MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CDLLApp

BEGIN_MESSAGE_MAP(CDLLApp, CWinApp)
	//{{AFX_MSG_MAP(CDLLApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLLApp construction

CDLLApp::CDLLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDLLApp object

CDLLApp *theApp = new CDLLApp();
HANDLE g_ThreadHandle;

void WINAPI MainThread(void *)
{
	//资源切换,无此宏创建对话框不成功
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HINSTANCE hResOld = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp->m_hInstance);
	CMainView *MainWnd = new CMainView();
	//模态窗体
	MainWnd->DoModal();
	delete MainWnd;
	MainWnd = NULL; //为了防止内存泄露
	HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)FreeLibrary,theApp->m_hInstance,0,0);
	CloseHandle(hThread);
	//已被阻塞,当窗体关闭时继续以下内容
	CloseHandle(g_ThreadHandle);
	AfxSetResourceHandle(hResOld);
}



BOOL CDLLApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_ThreadHandle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, NULL);
	return CWinApp::InitInstance();
}


BOOL CDLLApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN://屏蔽回车
			return TRUE;
		case VK_ESCAPE://屏蔽Esc
			return TRUE;
		}
	}
	
	return CWinApp::PreTranslateMessage(pMsg);
}
