

typedef struct _INLINE_HOOK
{
	DWORD FunctionAddress;
	PVOID FunctionCallBack;
	PVOID InLineCommonHookZone;
	int InLineCommonLength;
}INLINEHOOKINFO, *PINLINEHOOKINFO;


typedef struct _DLLINFO
{
	string fullPath;
	string dllName;
	DWORD  textStart;
	DWORD  textEnd;
	DWORD  modelBase;
	DWORD  modelEnd;
}DLLINFO;
typedef struct _PEHEAD
{
	
	DWORD  virtualSize;
	DWORD  virtualOffset;
}PEHEAD;
/**************************************
*  功能：保存到文件
****************************************/
void saveFile(string fileFullPath,string str);

/**************************************
*  功能：从文件中读取
****************************************/
void readFile(string fileFullPath,vector<string>& vec);

/**************************************
*  功能：从堆栈文件中获取地址
****************************************/
void getAddress(string str,map<DWORD,DWORD>& addressMap);

/**************************************
*  功能：深度优先递归遍历目录中所有的文件
****************************************/
//BOOL  DirectoryList(LPCSTR Path,vector<string>& fileVec);

/**************************************
*  功能：调试输出
****************************************/
void OutputDbgInfo(LPCSTR format, ...);

/**************************************
*  功能：验证地址是否可访问
****************************************/
bool checkAddress(DWORD address);

/**************************************
*  功能：获得要挂起的线程
****************************************/
DWORD getThreadId();

/**************************************
*  功能：输出调试信息
****************************************/
void OutputDbgInfo(LPCSTR format, ...);

/**************************************
*  功能：获得特征码
****************************************/
DWORD GetSpecial(IN unsigned char* code,IN DWORD length,IN DWORD begin,IN DWORD scope);

/**************************************
*  功能：向地址写入
****************************************/
VOID writeMemoryStatic(LPVOID p,unsigned char replace[],int length);


VOID getAddress();

/**************************************
*  功能：改变地址读写状态
*  参数：
****************************************/
BOOL changAddressStatusStatic(DWORD address,int length);

/**************************************
*  功能：适用于多参数的虚函数调用
****************************************/
DWORD  virtualCall(DWORD objBase,DWORD offSet, ...);

/**************************************
*  功能：获得导出函数的地址
****************************************/
DWORD getFunctionAddress(char *dllName,char* functionName);

/**************************************
*  功能：hook中dump出堆栈
****************************************/
void saveStatckDump(DWORD callAddress,DWORD ebp);

/**************************************
*  功能：hook中dump出堆栈，不是虚函数的
****************************************/
void saveStatckDump(DWORD ebp);
/**************************************
*  功能：hook中dump出堆栈，外加自己的提示信息
****************************************/
void saveStatckDump(DWORD ebp,char *info);
/**************************************
*  功能：hook中dump出堆栈，不是虚函数的
****************************************/
void saveStatckDumpWithPath(string path,DWORD callAddress,DWORD ebp,char *info, BOOL recodeParams);

void saveStatckDump(DWORD ebp, BOOL recodeParams);//添加记录参数

/**************************************
*  功能：游戏里都是unicode的，转为char*
****************************************/
char*  myWideToChar(DWORD wideAddr);

/**************************************
*  功能：获得某一PE文件的某一区段的大小，如.text段
****************************************/
PEHEAD getPeHeadSize(const char * filePath,const char * dataType);

/**************************************
*  功能：获得当前进程的所有模块
****************************************/
void getProcessModules(OUT map<DWORD, string> &dllMap);

/**************************************
*  功能：获得主要模块的代码段地址
****************************************/
void getModuleDetailInfo(vector<DLLINFO> &vec);


//将单字节char*转化为宽字节wchar_t*  
wchar_t* AnsiToUnicode( const char* szStr )  ;

//将宽字节wchar_t*转化为单字节char*  
char* UnicodeToAnsi( const wchar_t* szStr )  ;

/**************************************
*  功能：cstring转为指定进制 ，可能有异常，单抽出来的一个方法
*  参数：
*
*  返回值：
*  时间：[2015/1/11 9:07 Administrator]
****************************************/
DWORD ConverToHex(CString str, int jinzhi);

/**************************************
*  功能：		char *转换成short类型输出显示， 主要是字符串的拼接及格式化 
*  参数：		buffer 输入的字符串	length 长度
*
*  返回值：	void
*  时间：[2015/1/1 22:10 Administrator]
****************************************/
void PrintBuffer(BOOL isTcp,PVOID buffer, int length);