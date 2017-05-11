//
//  HttpClient.cpp
//  Design
//
//  Created by cwliu on 14-12-5.
//  Copyright (c) 2014-12-08  cwliu. All rights reserved.
//
#include "stdafx.h"
#include "HttpClient.h"
#include <afxinet.h>

CHttpClient::CHttpClient()
{
}

CHttpClient::~CHttpClient()
{
}

void AfxOutInfoToFile(TCHAR* tag, TCHAR* info)
{
	//	OutputDbgInfo(" %s = %s ", UnicodeToAnsi(tag), UnicodeToAnsi(info));
	//OutputDbgInfo(" %s = %s ", tag, info);
}

CString   CHttpClient::UpFileOfHttp(TCHAR* strSourceName,TCHAR* strDestName,TCHAR* _strFtpSite)
{
	CString szError=L"";
	TCHAR* pName=new TCHAR[256]; 
	wsprintf(pName,L"%s?uploadFileName=%s",_strFtpSite,strDestName);
	AfxOutInfoToFile(L"===上传文件名：",strSourceName);
	AfxOutInfoToFile(L"===上传地址：",pName);
	DWORD dwType = 0;
	CString strServer;
	CString strObject;
	bool bResult = false;
	INTERNET_PORT wPort = 80;
	DWORD dwFileLength = 0;
	char * pFileBuff = NULL;
	CHttpConnection * pHC = NULL;
	CHttpFile * pHF = NULL;
	CInternetSession cis;

	bResult =  AfxParseURL(pName, dwType, strServer, strObject, wPort);

	AfxOutInfoToFile(L"===解析的服务器：",strServer.GetBuffer(strServer.GetLength()));
	AfxOutInfoToFile(L"===解析的地址：",strObject.GetBuffer(strObject.GetLength()));
	//AfxOutInfoToFile("===解析的地址：",m_strFtpSite);
	if(!bResult)
		return szError;
	CFile file;
	try
	{
		if(!file.Open(strSourceName, CFile::shareDenyNone | CFile::modeRead))
			return szError;
		dwFileLength = file.GetLength();
		if(dwFileLength <= 0)
			return szError;

		pFileBuff = new char[dwFileLength];
		memset(pFileBuff, 0, sizeof(char) * dwFileLength);
		file.Read(pFileBuff, dwFileLength);
		const int nTimeOut = 5000;
		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //联接超时设置

		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);  //重试1次
		pHC = cis.GetHttpConnection(strServer, wPort);  //取得一个Http联接
		pHF = pHC->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);

		if(!pHF->SendRequest(NULL, 0, pFileBuff, dwFileLength))
		{
			delete[]pFileBuff;
			pFileBuff = NULL;
			pHF->Close();
			pHC->Close();
			cis.Close();
			AfxOutInfoToFile(L"上传中，SendRequest error/n", L"");
			return szError;
		}

		
		char szBuff[1024];
		UINT nRead;
		while (nRead = pHF->Read(szBuff,1023))
		{
			szError=CString(szBuff,nRead);
			AfxOutInfoToFile(L"返回值:",szError.GetBuffer(szError.GetLength()));
		}


		/*
		DWORD   dwError,   dwBufferLength;   
		TCHAR szError[256]={_T("0")};

		while(pHF->Read(szError,256)) 
		{
		lstrcpy(szError,L"error");
		AfxOutInfoToFile(L"返回的值 ：",szError);
		}
		*/
		//if(lstrcmp(_T("0"),szError)==0)
		// {
		//  return false;
		// }

		DWORD dwStateCode = 0;
		pHF->QueryInfoStatusCode(dwStateCode);
		if(dwStateCode == HTTP_STATUS_OK)
			bResult = TRUE;
		else
		{

			AfxOutInfoToFile(L"上传中，服务器返回的代码为", L"");
			return szError;
		}

		// AfxOutInfoToFile(L"上传中，服务器返回的代码为");
		// AfxOutInfoToFile(L"上传中，服务器返回的代码为");
	}catch(...)
	{
		char sz[256] = "";
		// pEx->GetErrorMessage(sz, 25);
		CString str;
		// str.Format("InternetException occur!/r/n%s", sz);
		//AfxMessageBox(str);
		// g_writelog.WriteLine("上传中，%s",str);

	}

	delete[]pFileBuff;
	pFileBuff = NULL;
	file.Close();
	pHF->Close();
	pHC->Close();
	cis.Close();
	return szError;
}


CString CHttpClient::GetOfHttp(TCHAR* pName)
{
	CString szError=L"";
	BOOL bResult = FALSE;
	DWORD dwType = 0;
	CString strServer;
	CString strObject;

	INTERNET_PORT wPort = 80;
	DWORD dwFileLength = 0;
	char * pFileBuff = NULL;
	CHttpConnection * pHC = NULL;
	CHttpFile * pHF = NULL;
	CInternetSession cis(L"online" );

	bResult =  AfxParseURL(pName, dwType, strServer, strObject, wPort);

	AfxOutInfoToFile(L"===解析的服务器：",strServer.GetBuffer(strServer.GetLength()));
	AfxOutInfoToFile(L"===解析的地址：",strObject.GetBuffer(strObject.GetLength()));
	//AfxOutInfoToFile("===解析的地址：",m_strFtpSite);
	if(!bResult)
		return szError;

	try
	{
		const int nTimeOut = 5000;
		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //联接超时设置

		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);  //重试1次
		pHC = cis.GetHttpConnection(strServer, wPort);  //取得一个Http联接
		pHF = pHC->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject);

		if(!pHF->SendRequest(NULL, 0, NULL, 0))
		{

			pHF->Close();
			pHC->Close();
			cis.Close();
			AfxOutInfoToFile(L"上传中，SendRequest error/n", L"");
			return szError;
		}


		char szBuff[1024];
		UINT nRead;
		while (nRead = pHF->Read(szBuff,1023))
		{
			szError=CString(szBuff,nRead);
			AfxOutInfoToFile(L"返回值:",szError.GetBuffer(szError.GetLength()));
		}

		DWORD dwStateCode = 0;
		pHF->QueryInfoStatusCode(dwStateCode);
		if(dwStateCode == HTTP_STATUS_OK)
			bResult = TRUE;
		else
		{

			AfxOutInfoToFile(L"上传中，服务器返回的代码为", L"");
			return szError;
		}

	}

	catch(...)
	{

	}

	delete[]pFileBuff;
	pFileBuff = NULL;

	pHF->Close();
	pHC->Close();
	cis.Close();
	return szError;
}


void CHttpClient::testGet()
{
	TCHAR* _strFtpSite = L"http://192.168.1.103/entity/function/uploadFileManager_getCodeKey.action?bean.id=402893814a4dc598014a4dd9160d0002d";
	CString str = GetOfHttp( _strFtpSite);
	OutputDebugStringA("[!] [!][!][!][!]");
	OutputDebugStringW(str.GetBuffer(str.GetLength()));
	AfxMessageBox(str);
}

void CHttpClient::testUpload()
{
	TCHAR* strSourceName = L"C:\\psb.jpg";//文件名
	TCHAR* strDestName = L"big.png";;
	TCHAR* _strFtpSite = L"http://192.168.1.103/entity/function/uploadFileManager_uploadFile.action";
	UpFileOfHttp(strSourceName, strDestName, _strFtpSite);
}

CString CHttpClient::UploadFile(TCHAR* strSourceName)
{
	TCHAR path_buffer[_MAX_PATH];  
	TCHAR drive[_MAX_DRIVE];  
	TCHAR dir[_MAX_DIR];  
	TCHAR fname[_MAX_FNAME];  
	TCHAR ext[_MAX_EXT];  
	//TCHAR* strSourceName = L"C:\\psb.jpg";//文件名
	//TCHAR* strDestName = L"QQ华夏.bmp";
	_wsplitpath( strSourceName, drive, dir, fname, ext );  

	wcscat(fname, ext);
	//char *p = strrchr(strSourceName, '/')
	TCHAR* _strFtpSite = L"http://192.168.1.103/entity/function/uploadFileManager_uploadFileStream.action";
	return UpFileOfHttp(strSourceName, fname, _strFtpSite);
}

CString CHttpClient::GetCodeResult(CString beanId)
{
	TCHAR _strFtpSite[MAX_PATH] = L"http://192.168.1.103/entity/function/uploadFileManager_getCodeKey.action?bean.id=";
	wcscat(_strFtpSite, beanId.GetBuffer(beanId.GetLength()));
	//AfxMessageBox(_strFtpSite);
	return GetOfHttp( _strFtpSite);
}

typedef struct _KEY_FUNCTION
{
	TCHAR beanId[50];//验证码图片在服务器的id
	unsigned (__stdcall * AfterGetKeyBack) (CString str);//获得验证码后的回调函数
	int totalTime;//总的等待时长
}KeyFunction, *PKeyFunction;
UINT _stdcall WINAPI WaitForCodeResult(void *lparam)
{
	PKeyFunction pFunction = (PKeyFunction)lparam;
	CString key;
	CHttpClient httpClient;
	bool keyOk = false;
	CString idStr(pFunction->beanId);
	int delatyTime = pFunction->totalTime;
	if(delatyTime > 180)
	{
		delatyTime = 180;//最多等待5分钟
	}
	int startTime = GetTickCount();
	while (true)
	{
		if (GetTickCount()-startTime>=delatyTime*1000)
		{
			//AfxMessageBox(L"等待时间太久了，放弃治疗");
			break;
		}
		key = httpClient.GetCodeResult(idStr);

		if (key.GetLength()==0 )
		{
			Sleep(3000);
			continue;
		}else if (wcscmp(L"ERROR", key.GetBuffer(key.GetLength()))==0)
		{
			break;
		}else
		{
			keyOk = true;
			break;
		}
	}
	if (keyOk)
	{
		pFunction->AfterGetKeyBack(key);
	}
	
	return 0;
}

void CHttpClient::DealCodeOnline(CString str, int delatyTime, unsigned (__stdcall * AfterGetKey) (CString str))
{
	CString result = UploadFile(str.GetBuffer(str.GetLength()));

	//AfxMessageBox(result);

	if(result.GetLength()==32)
	{
		PKeyFunction pFunction = (PKeyFunction)malloc(sizeof(KeyFunction));
		wcscpy(pFunction->beanId, result.GetBuffer(result.GetLength()));
		pFunction->totalTime = delatyTime;
		pFunction->AfterGetKeyBack = AfterGetKey;
		HANDLE handle = (HANDLE)_beginthreadex(NULL,0,WaitForCodeResult,(PVOID)pFunction,0,0);
		
	}
}