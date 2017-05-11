// MainView.cpp : implementation file
//

#include "stdafx.h"
#include "DLL.h"
#include "MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//有效地控制代码(宏开关)
//注释掉本宏,就把自己的代码都注释掉了
#define _MAINVIEW_CPP  //本CPP

/////////////////////////////////////////////////////////////////////////////
// CMainView dialog


CMainView::CMainView(CWnd* pParent /*=NULL*/)
	: CDialog(CMainView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMainView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainView)
	DDX_Control(pDX, IDC_MainWnd_TAB, m_MainWnd_Tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainView, CDialog)
	//{{AFX_MSG_MAP(CMainView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MainWnd_TAB, OnSelchangeMainWndTAB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainView message handlers

BOOL CMainView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
#ifdef _MAINVIEW_CPP
	VMProtectBeginUltra
	
	//初始化窗体控件
	CRect TabRect;
	m_MainWnd_Tab.GetClientRect(&TabRect);//获取TAB区域
	TabRect.top+=22;
	TabRect.bottom-=2;
	TabRect.left+=1;
	TabRect.right-=2;
	
	//功能页面1
	m_MainWnd_Tab.InsertItem(0,TEXT("通用功能"));
	m_Page1 = new CPAGE1();
	m_Page1->Create(IDD_PAGE1,this);
	m_Page1->MoveWindow(TabRect);
	m_Page1->ShowWindow(SW_SHOW);
	
	m_Page1->EnableWindow(TRUE);
	
	m_MainWnd_Tab.InsertItem(1,TEXT("Malloc"));
	m_Page2 = new CPAGE2();
	m_Page2->Create(IDD_PAGE2,this);
	m_Page2->MoveWindow(TabRect);
	m_Page2->ShowWindow(SW_HIDE);
	m_Page2->EnableWindow(TRUE);
	
	m_MainWnd_Tab.InsertItem(2,TEXT("HOOK"));
	m_Page3 = new CPAGE3();
	m_Page3->Create(IDD_PAGE3,this);
	m_Page3->MoveWindow(TabRect);
	m_Page3->ShowWindow(SW_HIDE);
	m_Page3->EnableWindow(TRUE);
	
	m_MainWnd_Tab.InsertItem(3,TEXT("游戏功能"));
	m_Page4 = new CPAGE4();
	m_Page4->Create(IDD_PAGE4,this);
	m_Page4->MoveWindow(TabRect);
	m_Page4->ShowWindow(SW_HIDE);	
	m_Page4->EnableWindow(TRUE);
	
	m_MainWnd_Tab.InsertItem(4,TEXT("功能5"));
	m_Page5 = new CPAGE5();
	m_Page5->Create(IDD_PAGE5,this);
	m_Page5->MoveWindow(TabRect);
	m_Page5->ShowWindow(SW_HIDE);
	m_Page5->EnableWindow(TRUE);
	
	m_MainWnd_Tab.InsertItem(5,TEXT("功能6"));
	m_Page6 = new CPAGE6();
	m_Page6->Create(IDD_PAGE6,this);
	m_Page6->MoveWindow(TabRect);
	m_Page6->ShowWindow(SW_HIDE);
	m_Page6->EnableWindow(TRUE);
	
	m_MainWnd_Tab.InsertItem(6,TEXT("功能7"));
	m_Page7 = new CPAGE7();
	m_Page7->Create(IDD_PAGE7,this);
	m_Page7->MoveWindow(TabRect);
	m_Page7->ShowWindow(SW_HIDE);
	m_Page7->EnableWindow(TRUE);
	
	m_MainWnd_Tab.InsertItem(7,TEXT("LUA脚本"));
	m_Page8 = new CPAGE8();
	m_Page8->Create(IDD_PAGE8,this);
	m_Page8->MoveWindow(TabRect);
	m_Page8->ShowWindow(SW_HIDE);	
	m_Page8->EnableWindow(TRUE);
	VMProtectEnd
#endif // _MAINVIEW_CPP

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainView::OnSelchangeMainWndTAB(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
#ifdef _MAINVIEW_CPP
	switch(m_MainWnd_Tab.GetCurSel())
	{
	case 0 :
		m_Page1->ShowWindow(SW_SHOW);
		m_Page2->ShowWindow(SW_HIDE);
		m_Page3->ShowWindow(SW_HIDE);
		m_Page4->ShowWindow(SW_HIDE);
		m_Page5->ShowWindow(SW_HIDE);
		m_Page6->ShowWindow(SW_HIDE);
		m_Page7->ShowWindow(SW_HIDE);
		m_Page8->ShowWindow(SW_HIDE);
		break;
	case 1 :
		m_Page1->ShowWindow(SW_HIDE);
		m_Page2->ShowWindow(SW_SHOW);
		m_Page3->ShowWindow(SW_HIDE);
		m_Page4->ShowWindow(SW_HIDE);
		m_Page5->ShowWindow(SW_HIDE);
		m_Page6->ShowWindow(SW_HIDE);
		m_Page7->ShowWindow(SW_HIDE);
		m_Page8->ShowWindow(SW_HIDE);
		break;
	case 2 :
		m_Page1->ShowWindow(SW_HIDE);
		m_Page2->ShowWindow(SW_HIDE);
		m_Page3->ShowWindow(SW_SHOW);
		m_Page4->ShowWindow(SW_HIDE);
		m_Page5->ShowWindow(SW_HIDE);
		m_Page6->ShowWindow(SW_HIDE);
		m_Page7->ShowWindow(SW_HIDE);
		m_Page8->ShowWindow(SW_HIDE);
		break;
	case 3 :
		m_Page1->ShowWindow(SW_HIDE);
		m_Page2->ShowWindow(SW_HIDE);
		m_Page3->ShowWindow(SW_HIDE);
		m_Page4->ShowWindow(SW_SHOW);
		m_Page5->ShowWindow(SW_HIDE);
		m_Page6->ShowWindow(SW_HIDE);
		m_Page7->ShowWindow(SW_HIDE);
		m_Page8->ShowWindow(SW_HIDE);
		break;
	case 4 :
		m_Page1->ShowWindow(SW_HIDE);
		m_Page2->ShowWindow(SW_HIDE);
		m_Page3->ShowWindow(SW_HIDE);
		m_Page4->ShowWindow(SW_HIDE);
		m_Page5->ShowWindow(SW_SHOW);
		m_Page6->ShowWindow(SW_HIDE);
		m_Page7->ShowWindow(SW_HIDE);
		m_Page8->ShowWindow(SW_HIDE);
		break;
	case 5 :
		m_Page1->ShowWindow(SW_HIDE);
		m_Page2->ShowWindow(SW_HIDE);
		m_Page3->ShowWindow(SW_HIDE);
		m_Page4->ShowWindow(SW_HIDE);
		m_Page5->ShowWindow(SW_HIDE);
		m_Page6->ShowWindow(SW_SHOW);
		m_Page7->ShowWindow(SW_HIDE);
		m_Page8->ShowWindow(SW_HIDE);
		break;
	case 6 :
		m_Page1->ShowWindow(SW_HIDE);
		m_Page2->ShowWindow(SW_HIDE);
		m_Page3->ShowWindow(SW_HIDE);
		m_Page4->ShowWindow(SW_HIDE);
		m_Page5->ShowWindow(SW_HIDE);
		m_Page6->ShowWindow(SW_HIDE);
		m_Page7->ShowWindow(SW_SHOW);
		m_Page8->ShowWindow(SW_HIDE);
		break;
	case 7 :
		m_Page1->ShowWindow(SW_HIDE);
		m_Page2->ShowWindow(SW_HIDE);
		m_Page3->ShowWindow(SW_HIDE);
		m_Page4->ShowWindow(SW_HIDE);
		m_Page5->ShowWindow(SW_HIDE);
		m_Page6->ShowWindow(SW_HIDE);
		m_Page7->ShowWindow(SW_HIDE);
		m_Page8->ShowWindow(SW_SHOW);
		break;
	}
#endif // _MAINVIEW_CPP

	*pResult = 0;
}
