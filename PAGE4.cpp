#include "stdafx.h"
#include "DLL.h"
#include "PAGE4.h"
#include "HookCallBack.h"
#include "HookFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPAGE4::CPAGE4(CWnd* pParent /*=NULL*/)
	: CDialog(CPAGE4::IDD, pParent)
{

}


void CPAGE4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAGE4)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_PAGE4, m_edit_page4);
}


BEGIN_MESSAGE_MAP(CPAGE4, CDialog)
	//{{AFX_MSG_MAP(CPAGE4)
	// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PATCH_CHECK, &CPAGE4::OnBnClickedButtonPatchCheck)
	ON_BN_CLICKED(IDC_BUTTON_UNHOOK_CHECK, &CPAGE4::OnBnClickedButtonUnhookCheck)
	ON_BN_CLICKED(IDC_BUTTON_DATA_HOOK, &CPAGE4::OnBnClickedButtonDataHook)
	ON_BN_CLICKED(IDC_BUTTON_DATA_UNHOOK, &CPAGE4::OnBnClickedButtonDataUnhook)
	ON_BN_CLICKED(IDC_BUTTON3, &CPAGE4::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPAGE4::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_CANCLEALL, &CPAGE4::OnBnClickedButtonCancleall)
	ON_BN_CLICKED(IDC_BUTTON_LOLHOOK, &CPAGE4::OnBnClickedButtonLolhook)
	ON_BN_CLICKED(IDC_BUTTON6, &CPAGE4::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_CHECK_EAX, &CPAGE4::OnBnClickedCheckEax)
	ON_BN_CLICKED(IDC_CHECK_EBX, &CPAGE4::OnBnClickedCheckEbx)
	ON_BN_CLICKED(IDC_CHECK_ECX, &CPAGE4::OnBnClickedCheckEcx)
	ON_BN_CLICKED(IDC_CHECK_EDX, &CPAGE4::OnBnClickedCheckEdx)
	ON_BN_CLICKED(IDC_CHECK_EDI, &CPAGE4::OnBnClickedCheckEdi)
	ON_BN_CLICKED(IDC_CHECK_ESI, &CPAGE4::OnBnClickedCheckEsi)
	ON_BN_CLICKED(IDC_CHECK_PARA, &CPAGE4::OnBnClickedCheckPara)
END_MESSAGE_MAP()

unsigned char check_chars[] =
{
	0x8B, 0x45, 0xEC, 0x8B, 0x0C, 0x85, 0xCC, 0xCC, 0xCC, 0xCC, 
	0x89, 0x4D, 0xE0, 0xC7, 0x45, 0xE4, 0x00, 0x00, 0x00, 0x00, 
	0x83, 0x7D, 0xE0, 0x00
};
DWORD g_dFunctionBak[20];
DWORD g_dObjAdd =0 ;
void CPAGE4::OnBnClickedButtonPatchCheck()
{
	HMODULE module = GetModuleHandleA("ThingClient.dll");
	if (!module)
	{
		m_edit_page4.SetWindowTextW(_T("没有找到ThingClient！！！"));
		return;
	}
	// 根据特征码获取地址
	DWORD address = GetSpecial(check_chars, 24, (DWORD)module, 0xf0000);
	if (!checkAddress(address))
	{
		m_edit_page4.SetWindowTextW(_T("根据特征码没有找到有效地址！！！"));
		return;
	}

	//hook虚函数
	g_dObjAdd = *(DWORD *)(address + 6);

	OutputDbgInfo("[!] 特征码地址：%08X   找到的检测对象地址：%08X \r\n",
		address, g_dObjAdd);
	for (int i=0;i<5;i=i++)
	{
		g_dFunctionBak[i] = *(DWORD *)(g_dObjAdd+4*i);
		*(DWORD *)(g_dObjAdd+4*i) = (DWORD)HookCheckCallBack;
	}
	m_edit_page4.SetWindowTextW(_T("hook 检测函数 success！！！"));

}


void CPAGE4::OnBnClickedButtonUnhookCheck()
{
	if (g_dObjAdd)
	{
		for (int i=0;i<5;i=i++)
		{
			*(DWORD *)(g_dObjAdd+4*i) = g_dFunctionBak[i];
		}
		m_edit_page4.SetWindowTextW(_T("unhook成功！！！"));
		g_dObjAdd = 0;
	}else
	{
		m_edit_page4.SetWindowTextW(_T("没有被hook！！！"));
	}

}

unsigned char dec_chars[] =
{
	0x8B, 0x75, 0x0C, 0x8B, 0x4D, 0x10, 0x8B, 0x7D, 0x08, 0x8B, 
	0xC1, 0x8B, 0xD1, 0x03, 0xC6, 0x3B, 0xFE
};

DWORD g_DecHookAddress;

void CPAGE4::OnBnClickedButtonDataHook()
{
	g_DecHookAddress = HookWithSpecialCode("Socket.dll", 
		dec_chars, 
		17, 
		&m_edit_page4);
}


void CPAGE4::OnBnClickedButtonDataUnhook()
{
	if (g_DecHookAddress)
	{
		UnInLineHookCommon(g_DecHookAddress);
	}
}

unsigned char control_chars[] =
{
	0x8B,0x4D,0x0C,0x51,0x8B,0x55,0x08,0x52,0x8B,0x45,
	0xFC,0x8B,0x48,0x6C
};
DWORD g_ConHookAddress;
void CPAGE4::OnBnClickedButton3()
{
	g_ConHookAddress = HookWithSpecialCode("Controller.dll", 
		control_chars, 
		14, 
		&m_edit_page4);
}


void CPAGE4::OnBnClickedButton4()
{
	if (g_ConHookAddress)
	{
		UnInLineHookCommon(g_ConHookAddress);
	}
}

extern map<DWORD, PINLINEHOOKINFO> g_mHookInfo;
void CPAGE4::OnBnClickedButtonCancleall()
{
	map<DWORD, PINLINEHOOKINFO>::iterator iter;
	vector<DWORD> vec;
	
	//循环遍历，依次unhook
	for (iter = g_mHookInfo.begin();iter!=g_mHookInfo.end();iter++)
	{
		vec.push_back(iter->first);
	}

	vector<DWORD>::iterator vecIter;
	int count = 0;
	for (vecIter = vec.begin();vecIter!=vec.end();vecIter++)
	{
		DWORD functionAddress = *vecIter;
		if (functionAddress)
		{
			UnInLineHookCommon(functionAddress);
			count++;
		}
	}
	CString buf;
	buf.Format(_T("共取消 %d 个Hook！"), count);
	m_edit_page4.SetWindowTextW(buf);
}

unsigned char lol_chars[] ={0x88,0x47,0x01,0x8A,0x46,0x02,0xC1,0xE9,0x02,0x88,
	0x47,0x02,0x83,0xC6,0x03,0x83,0xC7,0x03,0x83,0xF9,0x08};
void CPAGE4::OnBnClickedButtonLolhook()
{
	g_ConHookAddress = HookWithSpecialCode("Adobe AIR.dll", 
		lol_chars, 
		21, 
		&m_edit_page4, 0xd156ad);
}


void CPAGE4::OnBnClickedButton6()
{
	CString moduleStr, codeStr, scopeStr;

	GetDlgItemTextW(IDC_EDIT_MODULE, moduleStr);
	GetDlgItemTextW(IDC_EDIT_SCOPE, scopeStr);
	GetDlgItemTextW(IDC_EDIT_SPECIAL_CODE, codeStr);

	HookWithString(
		UnicodeToAnsi(moduleStr), 
		UnicodeToAnsi(codeStr),
		&m_edit_page4, 
		ConverToHex(scopeStr, 16)
		);
}

extern REGISTERFILTER registerFilter;
void CPAGE4::OnBnClickedCheckEax()
{
	registerFilter.bWatchEAX = ((CButton *)
		GetDlgItem(IDC_CHECK_EAX))->GetCheck();
}


void CPAGE4::OnBnClickedCheckEbx()
{
	registerFilter.bWatchEBX = ((CButton *)
		GetDlgItem(IDC_CHECK_EBX))->GetCheck();
}


void CPAGE4::OnBnClickedCheckEcx()
{
	registerFilter.bWatchECX = ((CButton *)
		GetDlgItem(IDC_CHECK_ECX))->GetCheck();
}


void CPAGE4::OnBnClickedCheckEdx()
{
	registerFilter.bWatchEDX = ((CButton *)
		GetDlgItem(IDC_CHECK_EDX))->GetCheck();
}


void CPAGE4::OnBnClickedCheckEdi()
{
	registerFilter.bWatchEDI = ((CButton *)
		GetDlgItem(IDC_CHECK_EDI))->GetCheck();
}


void CPAGE4::OnBnClickedCheckEsi()
{
	registerFilter.bWatchESI = ((CButton *)
		GetDlgItem(IDC_CHECK_ESI))->GetCheck();
}


void CPAGE4::OnBnClickedCheckPara()
{
	registerFilter.bWatchPARAMS = ((CButton *)
		GetDlgItem(IDC_CHECK_PARA))->GetCheck();
}
