#include "stdafx.h"
#include "UpdateBase.h"

typedef struct _CODE
{
	char* arr;
	DWORD begin;
	DWORD length;
	CString name;
	CString introduce;
	int type;  //1为call要进行特殊处理
}CODE;

typedef struct _ADDRESSINFO
{
	DWORD dwObject;
	DWORD ODAddress;

}ADDRESSINFO;

//Role_Base
//8B 3D ?? ?? ?? ?? 85 FF 75 ?? 5F 5E 33 C0 5B 5D C2 04 00      3    
static char Role_Base_Code[88] ={0x8B,0x3D,-2,-1,-1,-1,0x85,0xFF,0x75,-1,0x5F,0x5E,0x33,0xC0,0x5B,0x5D,0xC2,0x04,0x00};//length=19


//LikeHuman_Name_Call
//8B 85 ?? ?? ?? ?? 8B CB 89 85 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 8D ?? ?? ?? ?? 50 51
//0x8B,0x85,0x4？,0x8B,0xCB,0x89,0x85,0x4？,0xE8,0x4？,0x8B,0x8D,0x4？,0x50,0x51
static char LikeHuman_Name_Call_Code[88] ={0x8B,0x85,-1,-1,-1,-1,0x8B,0xCB,0x89,0x85,-1,-1,-1,-1,
	0xE8,-2,-1,-1,-1,0x8B,0x8D,-1,-1,-1,-1,0x50,0x51};//length=27

//Bag_Base
//0x8B,0x4D,0xEC,0x8B,0xD8,0x51,0x8B,0x0D,0x??,0x??,0x??,0x??,0x89,0x5D,0xCC
static char Bag_Base_Code[88] = {0x8B,0x4D,0xEC,0x8B,0xD8,0x51,0x8B,0x0D,-2,-1,-1,-1,0x89,0x5D,0xCC};//length=15

//const DWORD Auction_Base
//51 8D 8B DC 00 00 00 51 8B C8 FF D2 8B 0D ?? ?? ?? ?? 6A 0C
static char Auction_Base_Code[88] = {0x51,0x8D,0x8B,0xDC,0x00,0x00,0x00,0x51,0x8B,0xC8,0xFF,0xD2,0x8B,0x0D,-2,-1,-1,-1,0x6A,0x0C};//length=20

//#define DecParam
//cc cc cc 55 8B EC 8B 01 56 8B 35 88 66 01 02
static char DecParam_Code[88] = {0xcc,0xcc,0xcc,0x55,0x8B,0xEC,0x8B,0x01,0x56,0x8B,0x35};//length=11

//const DWORD   RoleBurden_Base
//CC CC 55 8B EC 51 8D 45 FC 50 68 74 5B 00 02 B9
static char RoleBurden_Base_Code[88] = {0xCC,0xCC,0x55,0x8B,0xEC,0x51,0x8D,0x45,0xFC,0x50,0x68,-2,-1,-1,-1,0xB9};//length=16

//const DWORD   RoleBurdenMax_Call
//50 FF D2 5F 39 5E 08 0F 85 E8 ?? ?? ?? E8 ?? ?? ?? ?? 50
static char RoleBurdenMax_Call_Code[88] = {0x50,0xFF,0xD2,0x5F,0x39,0x5E,0x08,0x0F,0x85,0xE8,-1,-1,-1,0xE8,-2,-1,-1,-1,0x50};//length=19

//const DWORD   Monster_Base ; 
//F6 05 ?? ?? ?? ?? 01 75 57 83 0D ?? ?? ?? ?? 01 68 D0 03 0B 00 C7 45 FC 00 00 00 00 E8?? ?? ?? ?? 83 C4 04
static char Monster_Base_Code[36] ={0xF6,0x05,-1,-1,-1,-1,0x01,0x75,0x57,0x83,0x0D,-1,-1,-1,-1,
	0x01,0x68,0xD0,0x03,0x0B,0x00,0xC7,0x45,0xFC,0x00,0x00,0x00,0x00,0xe8,-1,-1,-1,-1,0x83,0xC4,0x04};//length=36

//const DWORD   RoleTiredElapse1_Base = 0x01FD3E68;  //已经消耗疲劳基址     偏移:1    //OD地址:0x0094E743
//68 ?? ?? ?? 01 B9 ?? ?? ?? 01 E8 ?? ?? ?? ?? 8B 45 FC 8B E5 5D C3 CC CC 55 8B EC 51 81 C1 C4 03 00 00
static char RoleTiredElapse1_Base_Code[34] = {0x68,-2,-1,-1,0x01,0xB9,-1,-1,-1,0x01,0xE8,-1,-1,-1,-1,
	0x8B,0x45,0xFC,0x8B,0xE5,0x5D,0xC3,0xCC,0xCC,0x55,0x8B,0xEC,0x51,0x81,0xC1,0xC4,0x03,0x00,0x00};

VOID  createCode(IN char* code_array,IN DWORD length,OUT CODE& code)
{
	code.arr=code_array;
	code.length=length;
	for(DWORD j=0;j<length;j++){
		if(code_array[j]==-2){
			code.begin=j+1;
			return;
		}
	}
}


vector<CODE>  initVec()
{
	vector<CODE> temp;
	CODE Role_Base_Type;
	Role_Base_Type.name="const DWORD Role_Base";
	Role_Base_Type.introduce="角色基址";
	createCode(Role_Base_Code,19,Role_Base_Type);


	CODE RoleTiredElapse1_Base_Type;
	RoleTiredElapse1_Base_Type.name="const DWORD RoleTiredElapse1_Base";
	RoleTiredElapse1_Base_Type.introduce="已经消耗疲劳基址";
	createCode(RoleTiredElapse1_Base_Code,34,RoleTiredElapse1_Base_Type);



	CODE LikeHuman_Name_Call_Type;
	//	LikeHuman_Name_Call_Type.begin=15;
	LikeHuman_Name_Call_Type.type=1;
	LikeHuman_Name_Call_Type.name="const DWORD LikeHuman_Name_Call";
	LikeHuman_Name_Call_Type.introduce="获得物品名称call";
	LikeHuman_Name_Call_Type.arr=LikeHuman_Name_Call_Code;
	createCode(LikeHuman_Name_Call_Code,27,LikeHuman_Name_Call_Type);

	CODE Bag_Base_Code_Type;
	//	Bag_Base_Code_Type.begin=9;
	Bag_Base_Code_Type.name="const DWORD Bag_Base";
	Bag_Base_Code_Type.introduce="背包基址";
	Bag_Base_Code_Type.arr=Bag_Base_Code;
	createCode(Bag_Base_Code,15,Bag_Base_Code_Type);


	CODE Auction_Base_Code_Type;
	//Auction_Base_Code_Type.begin=15;
	Auction_Base_Code_Type.name="const DWORD Auction_Base";
	Auction_Base_Code_Type.introduce="拍卖行基址";
	Auction_Base_Code_Type.arr=Auction_Base_Code;
	createCode(Auction_Base_Code,20,Auction_Base_Code_Type);

	CODE RoleBurden_Base_Code_Type;
	//RoleBurden_Base_Code_Type.begin=12;
	RoleBurden_Base_Code_Type.name="const DWORD RoleBurden_Base";
	RoleBurden_Base_Code_Type.introduce="当前负重基址基址";
	RoleBurden_Base_Code_Type.arr=RoleBurden_Base_Code;
	createCode(RoleBurden_Base_Code,16,RoleBurden_Base_Code_Type);

	CODE RoleBurdenMax_Call_Code_Type;
	//RRoleBurdenMax_Call_Code_Type.begin=15;
	RoleBurdenMax_Call_Code_Type.type=1;
	RoleBurdenMax_Call_Code_Type.name="const DWORD RoleBurdenMax_Call";
	RoleBurdenMax_Call_Code_Type.introduce="最大负重call";
	RoleBurdenMax_Call_Code_Type.arr=RoleBurdenMax_Call_Code;
	createCode(RoleBurdenMax_Call_Code,19,RoleBurdenMax_Call_Code_Type);

	CODE Monster_Base_Code_Type;
	Monster_Base_Code_Type.begin=0x38;
	Monster_Base_Code_Type.length=36;
	Monster_Base_Code_Type.name="const DWORD Monster_Base";
	Monster_Base_Code_Type.introduce="怪物基址";
	Monster_Base_Code_Type.arr=Monster_Base_Code;


	CODE DecParam1_Code_Type;
	DecParam1_Code_Type.begin=12;
	DecParam1_Code_Type.length=11;
	DecParam1_Code_Type.name="#define DecParam1";
	DecParam1_Code_Type.introduce="解密参数1";
	DecParam1_Code_Type.arr=DecParam_Code;

	CODE DecParam2_Code_Type;
	DecParam2_Code_Type.begin=0x4c;
	DecParam2_Code_Type.length=11;
	DecParam2_Code_Type.name="#define DecParam2";
	DecParam2_Code_Type.introduce="解密参数2";
	DecParam2_Code_Type.arr=DecParam_Code;

	CODE DecParam3_Code_Type;
	DecParam3_Code_Type.begin=0x68;
	DecParam3_Code_Type.length=11;
	DecParam3_Code_Type.name="#define DecParam3";
	DecParam3_Code_Type.introduce="解密参数3";
	DecParam3_Code_Type.arr=DecParam_Code;

	CODE DecParam4_Code_Type;
	DecParam4_Code_Type.begin=0x70;
	DecParam4_Code_Type.length=11;
	DecParam4_Code_Type.name="#define DecParam4";
	DecParam4_Code_Type.introduce="解密参数4";
	DecParam4_Code_Type.arr=DecParam_Code;

	temp.push_back(Role_Base_Type);
	temp.push_back(LikeHuman_Name_Call_Type);
	temp.push_back(Bag_Base_Code_Type);
	temp.push_back(Auction_Base_Code_Type);

	temp.push_back(RoleBurden_Base_Code_Type);
	temp.push_back(RoleBurdenMax_Call_Code_Type);
	temp.push_back(Monster_Base_Code_Type);
	temp.push_back(RoleTiredElapse1_Base_Type);



	temp.push_back(DecParam1_Code_Type);
	temp.push_back(DecParam2_Code_Type);
	temp.push_back(DecParam3_Code_Type);
	temp.push_back(DecParam4_Code_Type);
	return temp;
}

DWORD scope = 0x1800000-0x401000;
/**************************************
*  功能：获得角色名字
*  参数：
*
*  返回值：
*  负责人：
*  时间：[2014/4/12/ Administrator]
****************************************/
VOID  GetAddressInfo(char* code,DWORD length,DWORD begin,ADDRESSINFO& RoleInfo,int type)
{
	DWORD temp = 0;
	DWORD startAddress = 0x401000;
	try
	{
		char *p ;
		p=(char *)startAddress;
		for (DWORD i = 0; i < scope; i++,p++){
			for(DWORD j=0;j<length;j++){
				if(code[j]==-1||code[j]==-2){
					//j++;
					continue;
				}
				if(*(p-length+j)!=code[j]){
					break; //有一个不符合的就指向下一个
				}
				if(j==length-1){
					RoleInfo.ODAddress = (DWORD)(p-length+begin-1);
					if(type==1)
					{
						RoleInfo.dwObject = *(DWORD*)(p-length+begin-1)+RoleInfo.ODAddress+4;
					}else{
						RoleInfo.dwObject = *(DWORD*)(p-length+begin-1);
					}
					return;
				}
			}
		}
	}catch (...){
		OutputDbgInfo("方法GetRoleInfo执行异常\r\n");
	}
	OutputDbgInfo("没有找到\r\n");
}

void UpdateBaseToFile()
{
	string fileName = "D:\\VS_project\\SVN_DNF\\base.txt";
	vector<CODE> vec = initVec();
	vector<CODE>::iterator iter;
	OutputDbgInfo("[!] 更新基地址开始！");
	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "//%Y/%m/%d %X %A 本年第%j天 %z",localtime(&t) ); 
	string timeStr(tmp);
	saveFile(fileName,tmp	);
	
	saveFile(fileName,"//=================================== [更新基地址开始] =======================================");

	char temp[500];
	for(iter=vec.begin();iter!=vec.end();iter++)
	{
		CODE mycode = *iter;
		ADDRESSINFO RoleInfo;
		RoleInfo.dwObject=0;
		RoleInfo.ODAddress=0;
		GetAddressInfo(mycode.arr,mycode.length,mycode.begin,RoleInfo,mycode.type);
		string format="%s";
		for (int i=0;i<5;i++)
		{
			if (i<(mycode.name.GetLength())/8)
			{
			}else
			{
				format+="\t";
			}
		}
		if (mycode.name.Find(L"#define")!=string::npos)
		{
			format+="       0X%08X     //%s";
		}else
		{
			format+="   =   0X%08X ;   //%s";
		}
		for (int i=0;i<3;i++)
		{
			if (i<(mycode.introduce.GetLength())/8)
			{
			}else
			{
				format+="\t";
			}
		}
		format+="OD地址  0X%08X";
		sprintf_s(temp,format.c_str(),mycode.name,RoleInfo.dwObject,mycode.introduce,RoleInfo.ODAddress);
		string str(temp);
		saveFile("D:\\VS_project\\SVN_DNF\\base.txt",str);
	}
	saveFile(fileName,"//=================================== [更新基地址结束] =======================================\r\n\r\n\r\n\r\n");
	OutputDbgInfo("[!] 更新基地址结束！");
}
