// PAGE3.cpp : implementation file
//

#include "stdafx.h"
#include "DLL.h"
#include "PAGE3.h"
#include "HwndUtil.h"
#include "GetSSDTInformation.h"
#include "HookCallBack.h"
#include "HookFunctions.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyMsg	g_msg;
HWND	g_hwnd				=NULL;
int		g_FilterMessageType =	0;
int		g_FilterSendType	=	0;//0发、1收、2二者都有
GetSSDTInformation ssdtFunction;

extern vector<DLLINFO> g_dllInfoVec;
extern char g_cSysCallIndex[0x3000];
extern BOOL g_bSysStartRecord;
extern int  g_iSysWatchIndex;
extern BOOL first10c;
extern BOOL g_bSysWatchOne;//只监控指定的api
extern BOOL g_bSysRecordStack;

extern SENDHOOKFILTER g_sHookFilter;


void InitMyInfo()
{

	//第一次获得所有的模块信息
	getModuleDetailInfo(g_dllInfoVec);
	vector<DLLINFO>::iterator iter;
	DLLINFO dllInfo;
	for(iter=g_dllInfoVec.begin();iter!=g_dllInfoVec.end();iter++)
	{
		dllInfo = *iter;
		OutputDbgInfo("[!] 模块信息--- 开始： %08X  结束： %08X 名称：%s  路径：%s", 
			dllInfo.modelBase, 
			dllInfo.modelEnd,
			dllInfo.dllName.c_str(), 
			dllInfo.fullPath.c_str());
	}
	char temp[50];
	ssdtFunction.GetSSDTNameByIndex(temp, 1);
}
CPAGE3::CPAGE3(CWnd* pParent /*=NULL*/)
	: CDialog(CPAGE3::IDD, pParent)
{
	return;
	//构造函数中初始化 g_msg
	CHwndUtil hwndUtil;

	HWND hwndArray[50];
	int size;
	DWORD pid = GetCurrentProcessId();
	hwndUtil.GetHwndByPid(pid, hwndArray, &size);

	// 	for (int i=0; i<size; i++)
	// 	{
	// 		Rect rect;
	// 		GetWindowRect(&rect);
	// 	}

	g_hwnd = hwndArray[size-1];
	CMyMsg	msg(g_hwnd);
	g_msg = msg;

	OutputDbgInfo("[!] 主窗体的hwnd = %08X", g_hwnd);
}


void CPAGE3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_PAGE3, m_edit_page3);
	DDX_Control(pDX, IDC_COMBO_PAGE3, m_message_type_box);
	DDX_Control(pDX, IDC_COMBO_PAGE3_SEND, m_net_message_type);
	DDX_Control(pDX, IDC_COMBO1, m_combox_sys_type);
	DDX_Control(pDX, IDC_LIST_PAGE3, m_list_page3);
	DDX_Control(pDX, IDC_COMBO_COMPAREUDP, m_udp_compare_type);
	DDX_Control(pDX, IDC_COMBO_COMPARETCP, m_tcp_compare_type);
}


BEGIN_MESSAGE_MAP(CPAGE3, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_HOOK_DIS, &CPAGE3::OnBnClickedButtonHookDis)
	ON_BN_CLICKED(IDC_BUTTON_UNHOOK_DIS, &CPAGE3::OnBnClickedButtonUnhookDis)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE3, &CPAGE3::OnCbnSelchangeComboPage3)
	ON_BN_CLICKED(IDC_BUTTON_HOOK_SEND, &CPAGE3::OnBnClickedButtonHookSend)
	ON_BN_CLICKED(IDC_BUTTON_UNHOOK_SEND, &CPAGE3::OnBnClickedButtonUnhookSend)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE3_SEND, &CPAGE3::OnCbnSelchangeComboPage3Send)
	ON_BN_CLICKED(IDC_BUTTON_HOOK_SYS, &CPAGE3::OnBnClickedButtonHookSys)
	ON_BN_CLICKED(IDC_BUTTON_UNHOOK_SYS, &CPAGE3::OnBnClickedButtonUnhookSys)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_INFO, &CPAGE3::OnBnClickedButtonShowInfo)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPAGE3::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON_FUNCTION_NAME, &CPAGE3::OnBnClickedButtonFunctionName)
	ON_BN_CLICKED(IDC_BUTTON_WATCH_SYS, &CPAGE3::OnBnClickedButtonWatchSys)
	ON_BN_CLICKED(IDC_BUTTON_UNWATCH_SYS, &CPAGE3::OnBnClickedButtonUnwatchSys)
	ON_BN_CLICKED(IDC_BUTTON_CALLFUNCTION_TEST, &CPAGE3::OnBnClickedButtonCallfunctionTest)
	ON_BN_CLICKED(IDC_CHECK_WATCH, &CPAGE3::OnBnClickedCheckWatch)
	ON_BN_CLICKED(IDC_BUTTON_SETWATCH, &CPAGE3::OnBnClickedButtonSetwatch)
	ON_BN_CLICKED(IDC_CHECK_WATCH_RECORD, &CPAGE3::OnBnClickedCheckWatchRecord)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PAGE3, &CPAGE3::OnNMDblclkListPage3)
	ON_BN_CLICKED(IDC_CHECK_WATCH2, &CPAGE3::OnBnClickedCheckWatch2)
	ON_BN_CLICKED(IDC_CHECK_WATCH_RECORD2, &CPAGE3::OnBnClickedCheckWatchRecord2)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CPAGE3::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_CHECK_WATCH_TCP, &CPAGE3::OnBnClickedCheckWatchTcp)
	ON_BN_CLICKED(IDC_CHECK_WATCH_RECORD_TCP, &CPAGE3::OnBnClickedCheckWatchRecordTcp)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPAREUDP, &CPAGE3::OnCbnSelchangeComboCompareudp)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPARETCP, &CPAGE3::OnCbnSelchangeComboComparetcp)
END_MESSAGE_MAP()

typedef struct
{
	WCHAR	title[50];
	int		nWidth;
}COLUMNSTRUCT;

COLUMNSTRUCT g_Column_Data[] = 
{
	{_T("N"),20	},
	{_T("索引"),40	},
	{_T("函数名"),150	}
};
CListCtrl *g_pListCtrl;

typedef struct
{
	DWORD	ItemIndex;
	DWORD	CallIndex;
	WCHAR	FunctionName[50];
}ITEMDETAIL, *PITEMDETAIL;

void CPAGE3::OnBnClickedButtonHookDis()
{
	HookDispatchMessageW();
}


void CPAGE3::OnBnClickedButtonUnhookDis()
{
	UnHookDispatchMessageW();
}

void UpdateListControl()
{
	CString strBuf;

	for (int i=0;i<g_pListCtrl->GetItemCount();i++)
	{
		PITEMDETAIL pInfo = (PITEMDETAIL)g_pListCtrl->GetItemData(i);
		free(pInfo);
	}
	g_pListCtrl->DeleteAllItems();

	for (int i=0;i<2000;i++)
	{
		if (g_cSysCallIndex[i]==1)
		{
			PITEMDETAIL pInfo = (PITEMDETAIL)malloc(sizeof(ITEMDETAIL));

			int nItem = g_pListCtrl->GetItemCount();
			strBuf.Format(L"%02d",nItem+1);
			g_pListCtrl->InsertItem(nItem,strBuf);//插入行
			strBuf.Format(L"%04X",i);
			g_pListCtrl->SetItemText(nItem,1,strBuf);
			char temp[50];
			ssdtFunction.GetSSDTNameByIndex(temp, i);

			WCHAR *pFunctionInfo = AnsiToUnicode(temp);
			g_pListCtrl->SetItemText(nItem,2,pFunctionInfo);

			pInfo->CallIndex = i;
			pInfo->ItemIndex = nItem + 1;
			memcpy(pInfo->FunctionName, pFunctionInfo, 50);
			g_pListCtrl->SetItemData(nItem, (DWORD)pInfo);
		}
	}
}


BOOL CPAGE3::OnInitDialog()
{
	CDialog::OnInitDialog();

	//	SetDlgItemText(IDC_STATIC_INFO3, _T("功能说明：\r\n   主要负责HOOK DispatchMessageW，看程序的消息事件情况，Hook其他函数也同样可以。"));
	//初始化消息类型
	m_message_type_box.InsertString(0, _T("WM_MOUSEMOVE"));
	m_message_type_box.InsertString(1, _T("WM_LBUTTONDOWN"));
	m_message_type_box.InsertString(2, _T("WM_LBUTTONUP"));
	m_message_type_box.InsertString(3, _T("WM_LBUTTONDBLCLK"));
	m_message_type_box.InsertString(4, _T("WM_RBUTTONDOWN"));
	m_message_type_box.InsertString(5, _T("WM_RBUTTONUP"));
	m_message_type_box.InsertString(6, _T("WM_RBUTTONDBLCLK"));

	//初始化消息类型
	m_net_message_type.InsertString(TYPE_SEND, _T("发包"));
	m_net_message_type.InsertString(TYPE_RECEIVE, _T("收包"));
	m_net_message_type.InsertString(TYPE_ALL, _T("都监测"));

	//初始化消息类型
	m_combox_sys_type.InsertString(0, _T("记录"));
	m_combox_sys_type.InsertString(1, _T("不记录"));
	m_combox_sys_type.InsertString(2, _T("清空"));

	//udp判断方式
	m_udp_compare_type.InsertString(COMPARE_LESS, _T("小于"));
	m_udp_compare_type.InsertString(COMPARE_EQUIL, _T("等于"));
	m_udp_compare_type.InsertString(COMPARE_BIG, _T("大于"));
	
	

	//tcp判断方式
	m_tcp_compare_type.InsertString(COMPARE_LESS, _T("小于"));
	m_tcp_compare_type.InsertString(COMPARE_EQUIL, _T("等于"));
	m_tcp_compare_type.InsertString(COMPARE_BIG, _T("大于"));

	DWORD dwStyle = (DWORD)m_list_page3.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE,0,0);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
	m_list_page3.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)dwStyle);

	//初始化list_control
	m_list_page3.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//改变字体颜色

	m_list_page3.SetTextColor(RGB(15,113,88)); //显示颜色

	//m_list_page3.SetTextBkColor(-1); 
	for (int i = 0; i < 3; i++)
	{
		m_list_page3.InsertColumn(i, g_Column_Data[i].title);
		m_list_page3.SetColumnWidth(i, g_Column_Data[i].nWidth);
	}

	//控件全局化
	g_pListCtrl = &m_list_page3;
	//initCallIndex();
//	UpdateListControl();
	InitMyInfo();
	return TRUE; 
}


void CPAGE3::OnCbnSelchangeComboPage3()
{
	int index = m_message_type_box.GetCurSel();
	CString str;
	str.Format(_T(" 选择了第 %d 个。"), index);
	AppendInfoToEdit(&m_edit_page3, str);
	g_FilterMessageType = 0x200 + index;

}


void CPAGE3::OnBnClickedButtonHookSend()
{
	HookNtDeviceIoControl();
}


void CPAGE3::OnBnClickedButtonUnhookSend()
{
	UnHookNtDeviceIoControl();
}


void CPAGE3::OnCbnSelchangeComboPage3Send()
{
	int index = m_net_message_type.GetCurSel();
	CString str;
	str.Format(_T(" 选择了第 %d 个。"), index);
		AppendInfoToEdit(&m_edit_page3, str);
	g_sHookFilter.eSendType = (SENDTYPE)index;
}

void CPAGE3::OnBnClickedButtonHookSys()
{
	BOOL ret = HookSystemCallEx(HookSystemCallBack);
	if (ret)
	{
		AppendInfoToEdit(&m_edit_page3, _T("hook成功！"));
	}else
	{
		AppendInfoToEdit(&m_edit_page3, _T("hook失败！"));
	}
}


void CPAGE3::OnBnClickedButtonUnhookSys()
{
	BOOL ret = UnHookSystemCallEx();

	if (ret)
	{
		AppendInfoToEdit(&m_edit_page3, _T("unhook成功！"));
	}else
	{
		AppendInfoToEdit(&m_edit_page3, _T("unhook成功！"));
	}
}


void CPAGE3::OnBnClickedButtonShowInfo()
{
	CString str,buf;
	for (int i=0;i<=0x3000;i++)
	{
		if (g_cSysCallIndex[i]==1)
		{
			buf.Format(_T("%X\r\n"), i);
			str += buf;
		}
	}
	m_edit_page3.SetWindowTextW(str);

}

void CPAGE3::OnCbnSelchangeCombo1()
{
	int index = m_combox_sys_type.GetCurSel();
	if (index==0)
	{
		initCallIndex();
		g_bSysStartRecord = TRUE;
		OutputDebugStringA("记录");
		
	}else if(index==1)
	{
		g_bSysStartRecord = FALSE;
	}else
	{
		g_bSysStartRecord = FALSE;
		for (int i=0;i<0x3000;i++)
		{
			g_cSysCallIndex[i]=0;
		}
		initCallIndex();
	}
}

void CPAGE3::OnBnClickedButtonFunctionName()
{
	CString strBuf;
	GetDlgItemText(IDC_EDIT_HWND,strBuf);
	int dwObject = ConverToHex(strBuf, 16);  

	char temp[50];
	ssdtFunction.GetSSDTNameByIndex(temp, dwObject);

	strBuf.Format(_T("%04X -> %s "), dwObject, AnsiToUnicode(temp));
	m_edit_page3.SetWindowTextW(strBuf);
}


void CPAGE3::OnBnClickedButtonWatchSys()
{
	CString strBuf;
	GetDlgItemText(IDC_EDIT_HWND,strBuf);
	DWORD dwObject = ConverToHex(strBuf, 16);  
	if (dwObject&&dwObject<0x200&&g_cSysCallIndex[dwObject]==0)
	{
		g_cSysCallIndex[dwObject]=1;
		UpdateListControl();
		CString strBuf;
		char temp[50];
		ssdtFunction.GetSSDTNameByIndex(temp, dwObject);
		strBuf.Format(_T("ADD  : %04X|%s\r\n"), 
			dwObject, AnsiToUnicode(temp));
		AppendInfoToEdit(&m_edit_page3, strBuf);
	}
}


void CPAGE3::OnBnClickedButtonUnwatchSys()
{
	CString strBuf;
	GetDlgItemText(IDC_EDIT_HWND,strBuf);
	DWORD dwObject = ConverToHex(strBuf, 16);  

	if (dwObject&&dwObject<0x200&&g_cSysCallIndex[dwObject]==1)
	{
		g_cSysCallIndex[dwObject]=0;
		UpdateListControl();

		CString strBuf;
		char temp[50];
		ssdtFunction.GetSSDTNameByIndex(temp, dwObject);
		strBuf.Format(_T("remove: %04X|%s\r\n"), 
			dwObject, AnsiToUnicode(temp));
		AppendInfoToEdit(&m_edit_page3, strBuf);
	}
}


void CPAGE3::OnBnClickedButtonCallfunctionTest()
{
	OpenProcess(100,100,100);
}


void CPAGE3::OnBnClickedCheckWatch()
{
	if (((CButton *)GetDlgItem(IDC_CHECK_WATCH))->GetCheck())
	{
		g_bSysWatchOne = TRUE;
	}else
	{
		g_bSysWatchOne = FALSE;
	}
}


void CPAGE3::OnBnClickedButtonSetwatch()
{
	CString strBuf;
	GetDlgItemText(IDC_EDIT_HWND,strBuf);
	int dwObject = ConverToHex(strBuf, 16);  
	g_iSysWatchIndex = dwObject;
	char temp[50];
	ssdtFunction.GetSSDTNameByIndex(temp, dwObject);

	strBuf.Format(_T("监控 %04X  %s \r\n"), dwObject, AnsiToUnicode(temp));

	g_cSysCallIndex[dwObject]=1;
	m_edit_page3.SetWindowTextW(strBuf);
}


void CPAGE3::OnBnClickedCheckWatchRecord()
{
	if (((CButton *)GetDlgItem(IDC_CHECK_WATCH_RECORD))->GetCheck())
	{
		g_bSysRecordStack = TRUE;
	}else
	{
		g_bSysRecordStack = FALSE;
	}
}

void CPAGE3::OnNMDblclkListPage3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	
	int nSel=pNMItemActivate->iItem;

	PITEMDETAIL pInfo = (PITEMDETAIL)m_list_page3.GetItemData(nSel);

	CString strBuf;
	strBuf.Format(_T("remove: %04X|%s\r\n"), 
		pInfo->CallIndex, pInfo->FunctionName);
	AppendInfoToEdit(&m_edit_page3, strBuf);

	g_cSysCallIndex[pInfo->CallIndex] = 0;
	m_list_page3.DeleteItem(nSel);
	free(pInfo);

	*pResult = 0;
}

void CPAGE3::OnBnClickedCheckWatch2()
{
	if (((CButton *)GetDlgItem(IDC_CHECK_WATCH2))->GetCheck())
	{
		g_sHookFilter.bWatchUDP = TRUE;
	}else
	{
		g_sHookFilter.bWatchUDP = FALSE;
	}
}


void CPAGE3::OnBnClickedCheckWatchRecord2()
{
	if (((CButton *)GetDlgItem(IDC_CHECK_WATCH_RECORD2))->GetCheck())
	{
		g_sHookFilter.bWatchUDPToStack = TRUE;
	}else
	{
		g_sHookFilter.bWatchUDPToStack = FALSE;
	}
}

void CPAGE3::OnBnClickedButtonSet()
{
	CString strBuf, tempBuf;
	GetDlgItemText(IDC_EDIT_HWND_MINUDP,strBuf);
	GetDlgItemText(IDC_EDIT_HWND_MINTCP,tempBuf);
	int dwObject = ConverToHex(strBuf, 16);  
	if (dwObject)
	{
		g_sHookFilter.wUDPMinLen = dwObject;
	}

	dwObject = ConverToHex(tempBuf, 16);  
	if (dwObject)
	{
		g_sHookFilter.wTCPMinLen = dwObject;
	}
}

void CPAGE3::OnBnClickedCheckWatchTcp()
{
	if (((CButton *)GetDlgItem(IDC_CHECK_WATCH_TCP))->GetCheck())
	{
		g_sHookFilter.bWatchTCP = TRUE;
	}else
	{
		g_sHookFilter.bWatchTCP = FALSE;
	}
}


void CPAGE3::OnBnClickedCheckWatchRecordTcp()
{
	if (((CButton *)GetDlgItem(IDC_CHECK_WATCH_RECORD_TCP))->GetCheck())
	{
		g_sHookFilter.bWatchTCPToStack = TRUE;
	}else
	{
		g_sHookFilter.bWatchTCPToStack = FALSE;
	}
}


void CPAGE3::OnCbnSelchangeComboCompareudp()
{
	int index = m_udp_compare_type.GetCurSel();
	CString str;
	str.Format(_T(" 选择了第 %d 个。"), index);
	AppendInfoToEdit(&m_edit_page3, str);
	g_sHookFilter.eUDPCompare = (COMPARESULT)index;
}


void CPAGE3::OnCbnSelchangeComboComparetcp()
{
	int index = m_tcp_compare_type.GetCurSel();
	CString str;
	str.Format(_T(" 选择了第 %d 个。"), index);
	AppendInfoToEdit(&m_edit_page3, str);
	g_sHookFilter.eTCPCompare = (COMPARESULT)index;
}
