#ifndef __Design__HttpClient__
#define __Design__HttpClient__

class CHttpClient {
public:

	CHttpClient();

	~CHttpClient();

    CString   GetOfHttp(TCHAR* pName);

	CString   UpFileOfHttp(TCHAR* strSourceName, TCHAR* strDestName, TCHAR* _strFtpSite);

	void testGet();

	void testUpload();

	CString UploadFile(TCHAR* strSourceName);
	CString GetCodeResult(CString beanId);
	void DealCodeOnline(CString str, int delatyTime, unsigned (__stdcall * AfterGetKey) (CString str));
};
#endif /* defined(__Design__HttpClient__) */
