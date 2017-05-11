#pragma once
/**************************************
winuser.h里定义了类型

#define WM_MOUSEFIRST                   0x0200
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209

MSG msg;
msg.hwnd= (HWND)dwObject;	//句柄
msg.message=0x201;			//类型
msg.wParam=0;
msg.lParam=0x15301DC;
msg.time=11111;
msg.pt.x=999;				//位置
msg.pt.y=999;
****************************************/
typedef void (WINAPI * MyDispatchMessage)(MSG *pmsg);

class CMyMsg
{
public:
	CMyMsg(void);
	~CMyMsg(void);
	CMyMsg(HWND hwnd);
protected:
	MyDispatchMessage	pDispatchMessage;
	HWND				m_hwnd;
public:
	void SetHwnd(HWND newHwnd);

	void LeftClick();

	void DefaultPositonHwndMsg(PPOINT pPoint, MSG *msg);
	
	void RightClick();

	void MoveMouseTo(int x, int y);

	void MoveMouseTo(PPOINT pPoint);

	void LeftDown();

	void LeftUp();

	void RightDown();

	void RightUp();

	void Dispatch(MSG *pmsg);

	void Init();

	void LeftClick(PPOINT pPoint);

	void RightClick(PPOINT pPoint);

	void LeftDown(PPOINT pPoint);

	void LeftUp(PPOINT pPoint);

	void RightDown(PPOINT pPoint);

	void RightUp(PPOINT pPoint);

	void LeftClick(double preX, double preY);

	void RightClick(double preX, double preY);
};

