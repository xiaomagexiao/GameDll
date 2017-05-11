#ifndef  _FILE_UTIL
#define  _FILE_UTIL

	//获取父目录路径
	CString GetParentDir(CString P);

	//目录是否存在
	bool FolderExists(CString s);

	//创建目录，可以多层创建；创建成功或者已存在，返回true; 否则返回false
	bool SuperMkDir(CString P);

	void GetExePath(char* pExePath);

	void getConfigFromIni(TCHAR *key, TCHAR *value);
#endif
