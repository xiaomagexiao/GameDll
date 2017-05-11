
#ifndef  _PIC_UTIL
#define  _PIC_UTIL

class CPicUtil
{
public:
	CPicUtil();
	~CPicUtil();

	/************************************************************************
	HWND hwnd---截图的窗口句柄(0为当前屏幕)
	int left---相对于窗口句柄左边距
	int top---相对于 窗口句柄顶边距
	int width---截图宽度(宽度高度同时为0截取全屏)
	int height---截图高度
	CString path---文件保存路径                                                         	************************************************************************/
	bool CPicUtil::GetScreenBmp(HWND hwnd,char *savePath,int left,int top,int width,int height);

};



#endif
