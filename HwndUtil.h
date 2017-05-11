
#ifndef  _HWND_UTIL
#define  _HWND_UTIL

class CHwndUtil
{
public:
	CHwndUtil();
	~CHwndUtil();

	bool CHwndUtil::GetHwndByPid(DWORD ProcessId, HWND* hWnd, int* iLength);
	HWND CHwndUtil::GetHwndInjectByIni();//注入获得句柄方式
	HWND CHwndUtil::GetHwndUnInjectByIni();//未注入获得句柄方式
};
#endif
