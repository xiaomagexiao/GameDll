// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define LEN 1024

vector<DLLINFO> g_dllInfoVec;//全局的模块信息

//保存到文件
void saveFile(string fileFullPath,string str){
	ofstream out(fileFullPath,ios::out | ios::app); //ofstream out("out.txt",ios::out | ios::app);  
	//out<<str<<endl;
	out<<str;
	out.close();
}


//从文件中读取文件
void readFile(string fileFullPath,vector<string>& vec){

	ifstream inFile;
	inFile.open(fileFullPath,ios::in);

	if(!inFile)
	{
		cout<<"文件不存在： "<<fileFullPath<<endl;
		return;
	}

	char buffer[256];
	string message;
	while(!inFile.eof())
	{
		inFile.getline(buffer,256,'\n');//getline(char *,int,char) 表示该行字符达到256个或遇到换行就结束
		message = buffer;
	//	cout<<message<<endl;
		vec.push_back(message);
	}
	inFile.close();
}

vector<string>::iterator iter;
map<DWORD,DWORD>::iterator mapIter;
void getAddress(string str,map<DWORD,DWORD>& addressMap){


	vector<string> vecLine;

	readFile(str,vecLine);

	cout<<"获得的堆栈总数量： "<<vecLine.size()<<endl;

	string tempAddress;

	for (iter = vecLine.begin();iter!=vecLine.end();iter++)
	{
		try
		{
			tempAddress = *iter;
			if(tempAddress.length()>15)
			{
				DWORD value = stol(tempAddress.substr(11,8),FALSE,16);
				DWORD key = stol(tempAddress.substr(0,8),FALSE,16);
				addressMap[key]=value;
			}
		}catch (...){
			OutputDbgInfo("[!] 方法addressMap创建异常:%d\r\n",GetLastError());
		}
	}

}

// 深度优先递归遍历目录中所有的文件
// BOOL  DirectoryList(LPCSTR Path,vector<string>& fileVec)
// {
// 	WIN32_FIND_DATA FindData;
// 	HANDLE hError;
// 	int FileCount = 0;
// 	char FilePathName[LEN];
// 	// 构造路径
// 	char FullPathName[LEN];
// 	strcpy(FilePathName, Path);
// 	strcat(FilePathName, "\\*.*");
// 	hError = FindFirstFile(FilePathName, &FindData);
// 	if (hError == INVALID_HANDLE_VALUE)
// 	{
// 		printf("搜索失败!");
// 		return 0;
// 	}
// 	while(::FindNextFile(hError, &FindData))
// 	{
// 		// 过虑.和..
// 		if (strcmp(FindData.cFileName, ".") == 0 
// 			|| strcmp(FindData.cFileName, "..") == 0 )
// 		{
// 			continue;
// 		}
// 		// 构造完整路径
// 		wsprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
// 		FileCount++;
// 		// 输出本级的文件
// 		printf("\n%d  %s  ", FileCount, FullPathName);
// 
// 		fileVec.push_back(FullPathName);
// 
// 		/*if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
// 		{
// 			printf("<Dir>");
// 			
// 			DirectoryList(FullPathName);
// 		}*/
// 	}
// 	return 0;
// }

/**************************************
*  功能：验证地址是否可访问
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/4/27/ Administrator]
****************************************/
bool checkAddress(DWORD address){   
	if(!IsBadReadPtr((PVOID)address,4)){	//是o?有?D效?ì地ì?址?¤返¤|ì回?0
		return true;
	}else{
		return false;
	}
} 

/**************************************
*  功能：获得要挂起的线程
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/4/27/ Administrator]
****************************************/
DWORD getThreadId(){
	//DWORD  dwDstThread =0;
	//	HWND gameh=FindWindow(NULL,"QQ华夏");
	//if(gameh==0){
	//cout<<"没找到	"<<endl;
	//}
	//dwDstThread=::GetWindowThreadProcessId(gameh,NULL);
	// 
	int temp = 0;
	_asm
	{

		mov eax,dword ptr fs:[0x24]
		mov temp,eax

	}
	OutputDbgInfo("[!] 方法[!] getThreadId%d!\n",temp);
	return temp;
}



/**************************************
*  功能：输出调试信息
*  参数：
****************************************/
void OutputDbgInfo(LPCSTR format, ...)
{
	va_list arglist;
	char buffer[1024];
	va_start (arglist,format);
	vsprintf_s(buffer, format, arglist);
	va_end (arglist);
	strcat_s(buffer, "\n");
	OutputDebugStringA(buffer);
}


/**************************************
*  功能：获得特征码
*  参数：
****************************************/
DWORD GetSpecial(IN unsigned char* code,IN DWORD length,IN DWORD begin,IN DWORD scope)
{
	try
	{
		unsigned char *p ;
		p=(unsigned char *)begin;
		for (DWORD i = 0; i < scope; i++,p++){
			for(DWORD j=0;j<length;j++){
				if(code[j]==0xcc){
					continue;
				}
				if (!checkAddress(DWORD(p-length+j)))
				{
					continue;//地址不可访问
				}
				if(*(p-length+j)!=code[j]){
					break; //有一个不符合的就指向下一个
				}
				if(j==length-1){
					return  (DWORD)(p-length);//开始处地址
				}
			}
		}
	}catch (...){
		OutputDbgInfo("[!] 方法xxxxxxxx执行异常:%d\r\n",GetLastError());
	}
	return 0;
}

/**************************************
*  功能：获得特征码
*  参数：
****************************************/
VOID GetSpecial_temp(IN unsigned char* code,IN DWORD length,IN DWORD begin,IN DWORD end,OUT vector<DWORD>& vec)
{
	try
	{
		unsigned char *p ;
		p=(unsigned char *)begin;
		DWORD scope = end - begin;
		for (DWORD i = 0; i < scope; i++,p++){
			for(DWORD j=0;j<length;j++){
				if(code[j]==0xcc){
					continue;
				}
				if(*(p-length+j)!=code[j]){
					break; //有一个不符合的就指向下一个
				}

				if(j==length-1){
					vec.push_back((DWORD)(p-length));
				}
			}
		}
	}catch (...){
		OutputDbgInfo("[!] 方法xxxxxxxx执行异常:%d\r\n",GetLastError());
	}
}

_declspec(naked) VOID VirtualProtectDetail()
{
	_asm
	{
		mov     edi, edi
		push    ebp
		mov     ebp, esp
		push    esi
		mov     esi, 0x7C92d6ee  //7C8012C4 >7C92D6EE  ntdll.ZwProtectVirtualMemory
		mov     eax,0x7c801a6d
		jmp     eax
	}
}

#define WIN7

BOOL myVirtualProtect(DWORD p,DWORD lengthCode,int status,DWORD* lpflOldProtect)
{

	#ifdef  WIN7
		return VirtualProtect((LPVOID)p, lengthCode, status, lpflOldProtect);
	#endif

	int temp = 0;
	_asm
	{
		push    lpflOldProtect
			push    PAGE_EXECUTE_READWRITE
			push    lengthCode
			push    p
			push    -1
			call    VirtualProtectDetail
			mov temp,eax
	}
	if(temp){
		return TRUE;
	}else
	{
		return FALSE;
	}
}

/**************************************
*  功能：向地址写入
*  参数：
****************************************/
VOID writeMemoryStatic(LPVOID p,unsigned char replace[],int length){	 
	DWORD   lpflOldProtect;
	//改变读写状态
	if(myVirtualProtect((DWORD)p,length,PAGE_EXECUTE_READWRITE,&lpflOldProtect)){
		CopyMemory((void *)p,replace,length);
		//恢复读写状态
		myVirtualProtect((DWORD)p,length,lpflOldProtect,&lpflOldProtect);
		OutputDbgInfo("[!] 地址0x%x 修改成功!!!\r\n",(DWORD)p);
	}else
	{
		OutputDbgInfo("[!] 地址0x%x 改写状态失败!!! error: %d\r\n",(DWORD)p,GetLastError());
	}
}

VOID getAddress()
{
	OutputDbgInfo("[!] myVirtualProtect地址0x%x \r\n",myVirtualProtect);

}

/**************************************
*  功能：改变地址读写状态
*  参数：
****************************************/
BOOL changAddressStatusStatic(DWORD address,int length){	 
	DWORD   lpflOldProtect;
	//改变读写状态
	if(myVirtualProtect(address,length,PAGE_EXECUTE_READWRITE,&lpflOldProtect)){
		OutputDbgInfo("[!] 地址0x%x 读写状态修改成功!!!\r\n",address);
		return TRUE;
	}
	return FALSE;
}

/**************************************
*  功能：适用于多参数的虚函数调用，目前只适用于dword类型
*  参数：
****************************************/
DWORD  virtualCall(DWORD objBase,DWORD offSet_param, ...)
{
	//OutputDbgInfo("[!] 对象地址 %0x  偏移 %08x \n",objBase,offSet_param);  
	if (!checkAddress(objBase))
	{
		OutputDbgInfo("[!] 对象地址不正确 %0x\n",objBase);  
		return 0;
	}
	DWORD temp[10]={};
	va_list arg_ptr;  
	DWORD nArgValue = objBase;  
	DWORD nArgCount = 0;  
	va_start(arg_ptr,objBase);  
	do  
	{  
		
		//OutputDbgInfo("[!] the %d the arg:%08x\n",nArgCount,nArgValue);  
		temp[nArgCount]=nArgValue;
		nArgValue=va_arg(arg_ptr,DWORD);  
		nArgCount++;
		if (nArgCount>10)
		{
			break;
		}
		
	}while(nArgValue != VIRTUALCALL_END);  

	if (nArgCount<2||offSet_param>0x10000)
	{
		return 0;
	}
	for (int i=nArgCount-1;i>=2;i--)//从第三个开始是参数
	{
	//	OutputDbgInfo("[!] 传入的参数处理后遍历为 %0x\n",temp[i]);  
		nArgValue = temp[i];
		_asm
		{
			push nArgValue
		}
	}
	try
	{
		_asm
		{
			mov ecx,objBase
			mov eax,[ecx]//虚函数表地址
			add eax,offSet_param//虚函数偏移
			call [eax]   //虚函数表某偏移出的应该的函数地址
			mov nArgCount,eax
		}
	}catch (...){
		OutputDbgInfo("[!] 方法xxxxxxxx执行异常:%d\r\n",GetLastError());
	}
	return nArgCount;
}

//字符串分割函数
std::vector<std::string> split22(std::string str,std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str+=pattern;//扩展字符串以方便操作
	int size=str.size();

	for(int i=0; i<size; i++)
	{
		pos=str.find(pattern,i);
		if(pos<size)
		{
			std::string s=str.substr(i,pos-i);
			result.push_back(s);
			i=pos+pattern.size()-1;
		}
	}
	return result;
}

/**************************************
*  功能：获得导出函数的地址
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/6/4/ Administrator]
****************************************/
DWORD getFunctionAddress(char *dllName,char* functionName)
{
	HANDLE handle =  GetModuleHandleA(dllName);
	return (DWORD)GetProcAddress((HMODULE)handle,functionName);
}
BOOL checkAddressMy(DWORD address)
{
	if (address>0x100000&&address<0x80000000)
	{
		return TRUE;
	}
	return FALSE;
}

void GetAddressMoreInfo(DWORD ebpAddr, char *info,BOOL recodeParams)
{
	vector<DLLINFO>::iterator iter;
	DLLINFO dllInfo;
	DWORD returnAddr = *(DWORD *)(ebpAddr + 4);	//返回地址
	BOOL findModule = FALSE;		//是否找到
	BOOL isVTCall = FALSE;
	for(iter=g_dllInfoVec.begin();iter!=g_dllInfoVec.end();iter++)
	{
		dllInfo = *iter;
		if (returnAddr<=dllInfo.modelEnd && returnAddr>=dllInfo.modelBase)
		{
			findModule = TRUE;
			break;
		}
	}

	if (checkAddress(returnAddr))
	{
		//普通calle8，虚函数call FF
		isVTCall = (*((UCHAR *)returnAddr - 5) != 0xe8); 
	}

	if (!findModule)
	{
		dllInfo.modelBase = returnAddr;
		dllInfo.dllName = "====";
	}

	if (recodeParams)
	{
		char temp[100];
		if (isVTCall)
		{
			sprintf(temp,"%s","%08X[%06X |%16s]===[%08X, %08X, %08X, %08X] <VT>\r\n");
			
		}else
		{
			sprintf(temp,"%s","%08X[%06X |%16s]===[%08X, %08X, %08X, %08X]\r\n");
		}
		sprintf(info,temp, 
				returnAddr, 
				returnAddr - dllInfo.modelBase, 
				dllInfo.dllName.c_str(),
				*(DWORD *)(ebpAddr + 8),
				*(DWORD *)(ebpAddr + 0xc),
				*(DWORD *)(ebpAddr + 0x10),
				*(DWORD *)(ebpAddr + 0x14)
				);
	}else
	{
		sprintf(info,"%08X[%06X | %s]->", returnAddr, 
			returnAddr - dllInfo.modelBase, dllInfo.dllName.c_str());
	}

}



/**************************************
*  功能：hook中dump出堆栈
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/6/5/ Administrator]
****************************************/
void saveStatckDumpWithPath(string path,DWORD callAddress,DWORD ebp,char *info,BOOL recodeParams)
{
	DWORD nowEbp = ebp;
	DWORD index = 0;
	DWORD value = 0;
	char stackInfo[5000];
	int  count = 0;
	try
	{
		if (info!=NULL)
		{
			if (strlen(info)>20)
			{
				info[19]='\0';
			}
			sprintf(stackInfo,"\r\n%20s >>> \r\n", info);
		}
		while (checkAddress(ebp) && count<=50)//只要栈底地址有效就一直找下去
		{
			CHAR  ansii_str[100];
			ZeroMemory(ansii_str,100);
			GetAddressMoreInfo(ebp, ansii_str, recodeParams);
			strcat_s(stackInfo, ansii_str);

			ebp = *(DWORD *)ebp;
			count++;
		}

		OutputDbgInfo("[!] 堆栈回溯 集合大小:%d  \r\n", count);

		if (callAddress)
		{
			CHAR  ansii_str[50];
			ZeroMemory(ansii_str, 50);
			sprintf(ansii_str,"%08X(VT) -> END\r\n",callAddress);
			strcat_s(stackInfo,ansii_str);
		}

		saveFile(path,stackInfo);
	}catch (...){
		OutputDbgInfo("[!] 方法xxxxxxxx执行异常:%d \r\n",GetLastError());
	}
}
/**************************************
*  功能：hook中dump出堆栈
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/6/5/ Administrator]
****************************************/
void saveStatckDump(DWORD callAddress,DWORD ebp)
{
	saveStatckDumpWithPath("c:\\stack.txt",callAddress,ebp,"",FALSE);

}

/**************************************
*  功能：hook中dump出堆栈，不是虚函数的
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/6/5/ Administrator]
****************************************/
void saveStatckDump(DWORD ebp, BOOL recodeParams)
{
	saveStatckDumpWithPath("c:\\stack.txt",0,ebp,"",recodeParams);
}

/**************************************
*  功能：hook中dump出堆栈，不是虚函数的
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/6/5/ Administrator]
****************************************/
void saveStatckDump(DWORD ebp)
{
	saveStatckDump(0,ebp);
}

void saveStatckDump(DWORD ebp,char *info)
{
	saveStatckDumpWithPath("c:\\stack.txt",0,ebp,info,FALSE);
}
/**************************************
*  功能：游戏里都是unicode的，转为char*
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/6/11/ Administrator]
****************************************/
char*  myWideToChar(DWORD wideAddr){
	char*  tmpPT="";
	if (checkAddress(wideAddr))
	{
		int len = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wideAddr, -1, 0, 0, NULL, NULL);
		tmpPT= new char[len+1];
		::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wideAddr, -1, tmpPT, len, NULL, NULL);
		tmpPT[len] = 0;
	}else{
		OutputDbgInfo("[!] 宽字节转char*失败，地址：%08X 不可访问！",wideAddr);
	}
	return tmpPT;
}

/**************************************
*  功能：获得某一PE文件的某一区段的大小，如.text段
*  参数：filePath 文件全路径   dataType 区段类型
*
*  返回值：
*  负责人：
*  时间：[2014/6/21/ Administrator]
****************************************/
PEHEAD getPeHeadSize(const char * filePath,const char * dataType)
{
	DWORD returnSize = 0;
	PEHEAD item ;
	item.virtualOffset=0;
	item.virtualSize=0;
	IMAGE_DOS_HEADER DosHeader = {0};
	IMAGE_FILE_HEADER FileHeader = {0};
	HANDLE  hFile = INVALID_HANDLE_VALUE;
	DWORD  dwReadLen = 0;
	WORD  NumOfSec = 0; //区段表个数
	IMAGE_SECTION_HEADER *pSecHeader = NULL;
	//打开文件
	hFile = CreateFileA(filePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDbgInfo("无法打开文件，分析失败！ GetLastError: %d",GetLastError());
		return item;
	}

	//读取数据
	if (!ReadFile(hFile,&DosHeader,sizeof(DosHeader),&dwReadLen,NULL))
	{
		OutputDbgInfo("无法读取文件，分析失败！ GetLastError: %d",GetLastError());
		return  item;
	}
	SetFilePointer(hFile,DosHeader.e_lfanew + sizeof(IMAGE_NT_SIGNATURE),NULL,FILE_BEGIN);
	ReadFile(hFile,&FileHeader,sizeof(FileHeader),&dwReadLen,NULL);
	//得出区段个数
	NumOfSec = FileHeader.NumberOfSections;
	//分配区段内存
	DWORD SectionSize = NumOfSec * IMAGE_SIZEOF_SECTION_HEADER;
	char *pSecBuff = new char[SectionSize + 1];
	//移动指针到区段表
	SetFilePointer(hFile,FileHeader.SizeOfOptionalHeader,NULL,FILE_CURRENT);
	ReadFile(hFile,pSecBuff,SectionSize,&dwReadLen,NULL);

	//读取完毕，关闭句柄
	CloseHandle(hFile);

	int iIndexItem = 0;
	TCHAR tzBuff[10] = {0};
	char szSecName[IMAGE_SIZEOF_SHORT_NAME] = {0};
	//内存数据指针转换
	pSecHeader = (PIMAGE_SECTION_HEADER)pSecBuff;
	for (int i = 0; i < NumOfSec; i++)
	{
		memcpy(szSecName,pSecHeader->Name,8);
		if (strcmp(dataType,(const char *)pSecHeader->Name)==0)
		{
			item.virtualSize =  pSecHeader->Misc.VirtualSize;
			item.virtualOffset = pSecHeader->VirtualAddress;
			break;
		}
		//指针后移
		pSecHeader++;
	}
	//释放内存
	if (pSecBuff)
	{
		delete []pSecBuff;
	}
	return item;
}

/**************************************
*  功能：获得当前进程的所有模块
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/6/21/ Administrator]
****************************************/
void getProcessModules(OUT map<DWORD, string> &dllMap)
{
	PLDR_DATA_TABLE_ENTRY pLdrDataTable = NULL;
	__asm
	{
		mov eax, fs:[30h]       // eax==PEB
		mov eax, [eax + 0ch]    // eax==Ldr-->PEB_LDR_DATA
		// + 08h InLoadOrderModuleList-->LIST_ENTRY
		// + 10h InMemoryOrderModuleList-->LIST_ENTRY
		// + 14h InInitializationOrderModuleList-->LIST_ENTRY
		mov eax, [eax + 10h]    // InMemoryOrderModuleList
		mov pLdrDataTable, eax 
	}
	PLDR_DATA_TABLE_ENTRY pNextLdrTable = pLdrDataTable;
	// 遍历全部模块
	while ( ((PLIST_ENTRY)pLdrDataTable)->Flink != (PLIST_ENTRY)pNextLdrTable )
	{

		PWSTR pwstr = pNextLdrTable->FullDllName.Buffer;
		size_t len = wcslen(pwstr) + 1;
		size_t converted = 0;
		char *CStr;
		CStr=(char*)malloc(len*sizeof(char));
		wcstombs_s(&converted, CStr, len, pwstr, _TRUNCATE);

		string str(CStr);
		DWORD key = (DWORD)pNextLdrTable->DllBase;
		dllMap[key]=str;
		pNextLdrTable = (PLDR_DATA_TABLE_ENTRY)(PLIST_ENTRY(pNextLdrTable))->Blink;
	}

}
/**************************************
*  功能：获得主要模块的代码段地址
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/6/21/ Administrator]
****************************************/
void getModuleDetailInfo(vector<DLLINFO> &vec)
{
	PLDR_DATA_TABLE_ENTRY pLdrDataTable = NULL;
	__asm
	{
		mov eax, fs:[30h]       // eax==PEB
		mov eax, [eax + 0ch]    // eax==Ldr-->PEB_LDR_DATA
		// + 08h InLoadOrderModuleList-->LIST_ENTRY
		// + 10h InMemoryOrderModuleList-->LIST_ENTRY
		// + 14h InInitializationOrderModuleList-->LIST_ENTRY
		mov eax, [eax + 10h]    // InMemoryOrderModuleList
		mov pLdrDataTable, eax 
	}
	PLDR_DATA_TABLE_ENTRY pNextLdrTable = pLdrDataTable;
	DLLINFO dllInfo;
	// 遍历全部模块
	while ( ((PLIST_ENTRY)pLdrDataTable)->Flink != (PLIST_ENTRY)pNextLdrTable )
	{

		PWSTR pwstr = pNextLdrTable->FullDllName.Buffer;
		size_t len = wcslen(pwstr) + 1;
		size_t converted = 0;
		char *CStr;
		CStr=(char*)malloc(len*sizeof(char));
		wcstombs_s(&converted, CStr, len, pwstr, _TRUNCATE);
		string str(CStr);

		DWORD key = (DWORD)pNextLdrTable->DllBase;
		dllInfo.modelBase = (DWORD)pNextLdrTable->DllBase;
		dllInfo.modelEnd = (DWORD)pNextLdrTable->Reserved3[1] + (DWORD)pNextLdrTable->DllBase;
		dllInfo.fullPath=str;
		dllInfo.dllName = str.substr(str.find_last_of("\\")+1,str.length());
		vec.push_back(dllInfo);
		pNextLdrTable = (PLDR_DATA_TABLE_ENTRY)(PLIST_ENTRY(pNextLdrTable))->Blink;
	}

}
/**************************************
*  功能：获得主要模块的代码段地址
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/6/21/ Administrator]
****************************************/
void getModuleDetailInfo222(vector<DLLINFO> &vec)
{
	map<DWORD,string> dllMap;
	getProcessModules(dllMap);
	DLLINFO dllInfo;
	map<DWORD,string>::iterator iterMap;
	string exePath;

	OutputDbgInfo("[!] 获得模块信息  %d",dllMap.size());
	for (iterMap=dllMap.begin();iterMap!=dllMap.end();iterMap++)
	{
		DWORD key = iterMap->first;
		string value = iterMap->second;
		if(strstr(value.c_str(),".exe"))
		{
			OutputDbgInfo("[!] EntryPoint: %08X FullDllName: %s\r\n", key, value.c_str());
			exePath = value;
		}
	}
// 	if (exePath.length()<20)
// 	{
// 		OutputDbgInfo("[!] 没有找到\r\n");
// 		return;
// 	}
	//string first20 = exePath.substr(0,12);
	for (iterMap=dllMap.begin();iterMap!=dllMap.end();iterMap++)
	{
		DWORD key = iterMap->first;
		string value = iterMap->second;
		//OutputDbgInfo("[!] 模块路径：%s\r\n",  value.c_str());
	//	if (value.find(first20)!=value.npos)   //这个判断是寻找主模块
	//	{
			PEHEAD item = getPeHeadSize(value.c_str(),".text");
			if (item.virtualSize>0&&item.virtualOffset>0)
			{
				dllInfo.fullPath=value;
				dllInfo.dllName = value.substr(value.find_last_of("\\")+1,value.length());
				dllInfo.textStart=item.virtualOffset+key;
				dllInfo.textEnd = item.virtualOffset+key+item.virtualSize;
				vec.push_back(dllInfo);
			}
	//	}
	}
}



//将单字节char*转化为宽字节wchar_t*  
wchar_t* AnsiToUnicode( const char* szStr )  
{  
	int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );  
	if (nLen == 0)  
	{  
		return NULL;  
	}  
	wchar_t* pResult = new wchar_t[nLen];  
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );  
	return pResult;  
}  

//将宽字节wchar_t*转化为单字节char*  
char* UnicodeToAnsi( const wchar_t* szStr )  
{  
	int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );  
	if (nLen == 0)  
	{  
		return NULL;  
	}  
	char* pResult = new char[nLen];  
	WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );  
	return pResult;  
}  


DWORD ConverToHex(CString str, int jinzhi)
{
	DWORD dwObject = 0;
	try
	{
		dwObject = stol(str.GetBuffer(0),FALSE,jinzhi);  
	}
	catch (...)
	{
		OutputDbgInfo("[!] %s 转换为 %d 进制出现异常！！！\r\n",
			str.GetBuffer(str.GetLength()), jinzhi);
	}
	return dwObject;
}

extern SENDHOOKFILTER g_sHookFilter;
void PrintBuffer(BOOL isTcp,PVOID buffer, int length)
{
	DWORD ebpAddr = 0;

	char temp[800];
	if (isTcp)
	{
		sprintf(temp, "[!] <TCP> 封包信息 %02x : ", length);
	}else
	{
		sprintf(temp, "[!] <UDP> 封包信息 %02x : ", length);
	}
	
	//输出包内容
	//这里输出调试信息，可以用DbgView查看，如果有UI可以做成SendMessage形式~  
	char t[500];
	for (int i=0;i<length/2;i=i++)
	{
		
		sprintf(t, "%04x ", *((unsigned short*)buffer+i));
		strcat(temp,t);
		if (i>100)
		{
			strcat(temp,"*******");
			break;
		}
	}
	OutputDebugStringA(temp);

	if ((isTcp&&g_sHookFilter.bWatchTCPToStack)//tcp并且监控udp
		||(!isTcp&&g_sHookFilter.bWatchUDPToStack))//udp并且监控udp
	{
		_asm{
			mov ebpAddr, ebp
		}
		saveStatckDump(ebpAddr,true);
	}

}