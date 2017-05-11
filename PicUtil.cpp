#include "stdafx.h"
#include "PicUtil.h"

#include "FileUtil.h"

CPicUtil::CPicUtil()
{

}
CPicUtil::~CPicUtil()
{

}
/************************************************************************
	HWND hwnd---截图的窗口句柄(0为当前屏幕)
	int left---相对于窗口句柄左边距
	int top---相对于 窗口句柄顶边距
	int width---截图宽度(宽度高度同时为0截取全屏)
	int height---截图高度
	CString path---文件保存路径                                                         	************************************************************************/
bool CPicUtil::GetScreenBmp(HWND hwnd,char *savePath,int left,int top,int width,int height)
{
	if(!hwnd || NULL == savePath)
	{
		return false;
	}

	CDC *pDC;//屏幕DC
	pDC = CDC::FromHandle(::GetDC(hwnd));//获取屏幕DC(0为全屏，句柄则为窗口)

	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
	if(width==0 && height==0)//默认宽度和高度为全屏
	{
		width = pDC->GetDeviceCaps(HORZRES); //设置图像宽度全屏
		height= pDC->GetDeviceCaps(VERTRES); //设置图像高度全屏
	}


	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, width, height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	//memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC
	memDC.BitBlt(0, 0, width, height, pDC, left,top , SRCCOPY);//图像宽度高度和截取位置


	//以下代码保存memDC中的位图到文件
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//获得位图信息

	//获得路径
	CHAR dllPath[MAX_PATH] = {0};
	GetExePath(dllPath);
	strcat(dllPath,"\\CheckCode\\");

	//创建目录
	CString str(dllPath);
	bool isOk = SuperMkDir(str);

	char temp[50];
	sprintf(temp,"QQhx-%d.jpg",GetTickCount());
	strcat(dllPath,temp);

	memcpy(savePath, dllPath, MAX_PATH-1);


	FILE *fp = fopen(savePath, "w+b");//图片保存路径和方式

	BITMAPINFOHEADER bih = {0};//位图信息头
	bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
	bih.biWidth = bmp.bmWidth;//宽度

	BITMAPFILEHEADER bfh = {0};//位图文件头
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头
	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头
	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据
	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, height, p, 
		(LPBITMAPINFO) &bih, DIB_RGB_COLORS);//获取位图数据
	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据
	delete [] p;
	fclose(fp);
	memDC.SelectObject(oldmemBitmap);
	return true;
}