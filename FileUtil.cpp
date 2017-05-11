#include "stdafx.h"
#include "FileUtil.h"


//获取父目录路径
CString GetParentDir(CString P)
{
	return P.Left(P.ReverseFind('//'));
} 

//目录是否存在
bool FolderExists(CString s)
{
	DWORD attr;   
	attr   =   GetFileAttributes(s);  
	return   (attr   !=   (DWORD)(-1) )  && 
		( attr   &   FILE_ATTRIBUTE_DIRECTORY);   
}

//创建目录，可以多层创建；创建成功或者已存在，返回true; 否则返回false
bool SuperMkDir(CString P)
{
	int len=P.GetLength();
	if ( len <2 ) return false;  //如果字符串长度小于2，则不是有效的路径格式。

	if('//'==P[len-1])
	{
		P=P.Left(len-1);
		len=P.GetLength();
	}// 删除末尾的"/"
	if (  len <=0 ) return false;

	if (len <=3) //如果长度<=3，那么必定是根目录格式
	{
		if (FolderExists(P))return true;//如果根目录存在,返回true;
		else return false;              //如果不存在，根目录是无法创建的，返回失败；
	}

	if (FolderExists(P))return true;//如果目录存在,返回true;

	// 分开父目录和本身目录名称
	CString Parent;
	Parent=P.Left(P.ReverseFind('\\') );

	if(Parent.GetLength()<=0)return false; //目录名称错误

	bool Ret=SuperMkDir(Parent);  //递归创建父目录

	if(Ret)      // 父目录存在,直接创建目录
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength=sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor=NULL;
		sa.bInheritHandle=0;
		Ret=(CreateDirectory(P,&sa)==TRUE);
		return Ret;
	}
	else
		return false;
}

/************************************************************************/
/* 获得当前模块目录                                                        */
/************************************************************************/
void GetExePath(char* pExePath) 
{     
	HMODULE handle = GetModuleHandleA("001.dll");
	int  pathlen = GetModuleFileNameA(handle, pExePath, MAX_PATH);    
	while(1)     
	{         
		if(pExePath[pathlen--]=='\\')             
			break;     
	}     
	pExePath[++pathlen] = 0;     
}   


void getConfigFromIni(TCHAR *key, TCHAR *value)
{

	//获得配置文件配置的ini文件内容
	char iniPath[MAX_PATH] = {0};
	GetExePath(iniPath);
	strcat(iniPath,"\\");
	strcat(iniPath,"config.ini");
	TCHAR *utfPath = AnsiToUnicode(iniPath);
	
	GetPrivateProfileString(
		L"base", // 指向包含 Section 名称的字符串地址
		key, // 指向包含 Key 名称的字符串地址
		L"xxxx", // 如果 Key 值没有找到，则返回缺省的字符串的地址
		value, // 返回字符串的缓冲区地址
		MAX_PATH, // 缓冲区的长度
		utfPath // ini 文件的文件名
		);
	//AfxMessageBox(utfPath);
	//AfxMessageBox(value);
}