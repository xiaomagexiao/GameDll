#include "stdafx.h"
#include "HwndUtil.h"
#include "FileUtil.h"

CHwndUtil::CHwndUtil()
{

}
CHwndUtil::~CHwndUtil()
{

}


HWND* hWnd = new HWND[255];
int iLength = 0;

typedef struct _WINDOW_INFO
{
	HWND hwnd1;
	TCHAR WindowText[255];
	TCHAR ClassName[255];
	DWORD dwProcessId;
	DWORD dwThreadId;
}WindowInfo;

typedef WindowInfo * PWindowInfo;
typedef WindowInfo * LPWindowInfo;

DWORD EnumWindowInfo(WindowInfo WInfo[]);
HWND m_hwndFind[255];
int m_num = 0 ;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//判断窗口是否可见
	if(::GetWindowLong(hWnd,GWL_STYLE)& WS_VISIBLE)
	{
		m_hwndFind[m_num] = hWnd;
		m_num++;
	}
	return true;
}



//进程ID取窗口句柄
bool CHwndUtil::GetHwndByPid(DWORD ProcessId, HWND* hWnd, int* iLength)
{
	bool bRet = false;
	*iLength = 0;
	WindowInfo WinInfo1[255];
	DWORD aa=EnumWindowInfo(WinInfo1);
	for(int i=0; i<aa; i++)
	{
		if (WinInfo1[i].dwProcessId == ProcessId)
		{
			bRet = true;
			hWnd[(*iLength)] = WinInfo1[i].hwnd1;
			(*iLength)++;

			if (NULL == ::GetWindowLong(WinInfo1[i].hwnd1, GWL_HWNDPARENT))
			{
				//return WinInfo1[i].hwnd1;
			}
		}
	}

	return bRet;
}

//枚举窗口信息，成功返回窗口数量，失败返回
DWORD EnumWindowInfo(WindowInfo WInfo[])
{
	WindowInfo Winpro;
	m_num = 0;
	::EnumWindows(EnumWindowsProc, NULL);

	int i;
	for(i = 0; i < m_num; i++)
	{
		Winpro.hwnd1 = m_hwndFind[i];

		::GetWindowText(m_hwndFind[i], Winpro.WindowText, 128);
		::GetClassName(m_hwndFind[i], Winpro.ClassName, MAX_PATH-1);

		Winpro.dwThreadId = ::GetWindowThreadProcessId(m_hwndFind[i], &Winpro.dwProcessId);
		WInfo[i] = Winpro;
	}

	return i;
}

HWND CHwndUtil::GetHwndUnInjectByIni()
{
	bool bRet = false;
	WindowInfo WinInfo1[255];
	DWORD aa=EnumWindowInfo(WinInfo1);
	TCHAR ch[128];
	TCHAR value[MAX_PATH];
	getConfigFromIni(L"窗口标题", value);

	for(int i=0; i<aa; i++)
	{
		::GetWindowText (WinInfo1[i].hwnd1, ch, 128);
		if (wcscmp(ch, value)==0)
		{
			return WinInfo1[i].hwnd1;
		}
	}
	return 0;
}

//注入获得句柄方式
HWND CHwndUtil::GetHwndInjectByIni()
{
	HWND hwndArray[50];
	int size;
	DWORD pid = GetCurrentProcessId();
	GetHwndByPid(pid, hwndArray, &size);

	TCHAR value[MAX_PATH];
	getConfigFromIni(L"窗口标题", value);
	TCHAR ch[128];
	for (int i=0; i<size; i++)
	{

		// 		 	HWND hParent = ::GetParent (hwndArray[i]);
		// 
		// 			int nID = GetWindowLong(hWnd, GWL_ID);
		// 
		// 			DWORD style = (DWORD)::GetWindowLong (hWnd, GWL_STYLE);
		// 
		// 			RECT rc; 
		// 			::GetWindowRect(hWnd, &rc);
		// 			::GetWindowText (hParent, ch, 128);
		// 
		// 			::GetClassName (hParent, ch, 128);
		
		::GetWindowText (hwndArray[i], ch, 128);
		if (wcscmp(ch, value)==0)
		{
			return hwndArray[i];
		}
	}
	if(size>0)
	{
		return hwndArray[0];
	}
	return 0;
}