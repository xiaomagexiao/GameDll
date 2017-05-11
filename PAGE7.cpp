// PAGE7.cpp : implementation file
//

#include "stdafx.h"
#include "DLL.h"
#include "PAGE7.h"
#include "winIO/winio.h"

#include "IOMsg.h"

#include "KeyTable.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern HWND g_hwnd;
/////////////////////////////////////////////////////////////////////////////
// CPAGE7 dialog


CPAGE7::CPAGE7(CWnd* pParent /*=NULL*/)
	: CDialog(CPAGE7::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPAGE7)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPAGE7::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAGE7)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_PAGE7_INFO, m_page7_info);
}


BEGIN_MESSAGE_MAP(CPAGE7, CDialog)
	//{{AFX_MSG_MAP(CPAGE7)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOADDRIVER, &CPAGE7::OnBnClickedButtonLoaddriver)
	ON_BN_CLICKED(IDC_BUTTON_UNLOAD, &CPAGE7::OnBnClickedButtonUnload)
	ON_BN_CLICKED(IDC_BUTTON_ANJIAN, &CPAGE7::OnBnClickedButtonAnjian)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPAGE7 message handlers


void CPAGE7::OnBnClickedButtonLoaddriver()
{
	bool result = InitializeWinIo();
	if (result)
	{
		m_page7_info.SetWindowTextW(_T("驱动安装成功！！！"));
	}else
	{
		m_page7_info.SetWindowTextW(_T("驱动安装失败！！！"));
	}
}

void CPAGE7::OnBnClickedButtonUnload()
{
	ShutdownWinIo();

	//_outp(wPortAddr, dwPortVal);
}


void CPAGE7::OnBnClickedButtonAnjian()
{
	SwitchToThisWindow(g_hwnd, TRUE);
	//这种形式的创建对象是分配在栈中，不用手动释放
	CIOMsg ioMsg;
	Sleep(2000);
// 	for (int i=0;i<1;i++)
// 	{
	
		ioMsg.KeyPress(VK_X);
		ioMsg.KeyPress(VK_I);
		ioMsg.KeyPress(VK_A);
		ioMsg.KeyPress(VK_O);
		ioMsg.KeyPress(VK_M);
		ioMsg.KeyPress(VK_A);
		ioMsg.KeyPress(VK_G);
		ioMsg.KeyPress(VK_E);
	//}
}
