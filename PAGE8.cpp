// PAGE8.cpp : implementation file
//

#include "stdafx.h"
#include "DLL.h"
#include "PAGE8.h"
#include "LuaAPI.h"

#include <process.h>

//定义了此宏就说明加载lua51.dll  否则不加载


//#define _LUA_DLL 1

#ifdef _LUA_DLL

extern "C"
{ 
#include "luasrc/lua.h"
#include "luasrc/lualib.h"
#include "luasrc/lauxlib.h"
}   
lua_State *L;

#pragma comment(lib,"lua51.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLuaAPI luaAPI;

CPAGE8::CPAGE8(CWnd* pParent /*=NULL*/)
	: CDialog(CPAGE8::IDD, pParent)
	, lua_value(_T(""))
{
}

HANDLE scriptThreadId = 0;

void CPAGE8::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LUA, lua_value);
	DDX_Control(pDX, IDC_COMBO_PAGE8_COMMAND, m_page8_command);
	DDX_Control(pDX, IDC_EDIT_LUA, m_page8_info);
}


BEGIN_MESSAGE_MAP(CPAGE8, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LUA_EXECUTE, &CPAGE8::OnBnClickedButtonLuaExecute)
	ON_BN_CLICKED(IDC_BUTTON_LUA, &CPAGE8::OnBnClickedButtonLua)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE8_COMMAND, &CPAGE8::OnCbnSelchangeComboPage8Command)
END_MESSAGE_MAP()

#ifdef _LUA_DLL

int sleepLua(lua_State *L)
{
	int time = lua_tointeger(L,1);
	Sleep(time);
	return 1;
}

int moveToLua(lua_State *L)
{
	int clientX = lua_tointeger(L,1);
	int clientY = lua_tointeger(L,2);
	luaAPI.MoveTo(clientX,clientY);
	return 1;
}

int leftClickLua(lua_State *L)
{
	int clientX = lua_tointeger(L,1);
	int clientY = lua_tointeger(L,2);
	luaAPI.LeftClick(clientX,clientY);
	return 1;
}

int rightClickLua(lua_State *L)
{
	int clientX = lua_tointeger(L,1);
	int clientY = lua_tointeger(L,2);
	luaAPI.RightClick(clientX,clientY);

	return 1;
}

int infoLua(lua_State *L)
{
	int tag = lua_tointeger(L,1);
	OutputDbgInfo("[!] 执行到了-> %d .",tag);
	return 1;
}

#endif


void CPAGE8::OnBnClickedButtonLua()
{
	UpdateData();  
#ifdef _LUA_DLL
	luaL_dostring(L,UnicodeToAnsi(this->lua_value));
#endif
}

CString fileName = "";

UINT _stdcall DoScript(void *lPrarm)
{
	OutputDbgInfo("[!] %s",fileName);

#ifdef _LUA_DLL
	luaL_dofile(L,UnicodeToAnsi(fileName));
#endif
	_endthreadex(0);
	scriptThreadId = 0;
	return 0;

}
void CPAGE8::OnBnClickedButtonLuaExecute()
{

	if (scriptThreadId)
	{
		TerminateThread(scriptThreadId,0);
		scriptThreadId = 0;
	}else
	{
		//fileName = "D:\\VS_project\\new_DXF\\luatest.lua";

		if (((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
		{
			fileName = "D:\\VS_project\\new_DXF\\1-15.lua";
		}else if (((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
		{
			fileName = "D:\\VS_project\\new_DXF\\15-20.lua";
		}else if (((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck())
		{
			fileName = "D:\\VS_project\\new_DXF\\20-25.lua";
		}else if (((CButton *)GetDlgItem(IDC_RADIO4))->GetCheck())
		{
			fileName = "D:\\VS_project\\new_DXF\\peifang.lua";
		}else if (((CButton *)GetDlgItem(IDC_RADIO5))->GetCheck())
		{
			fileName = "D:\\VS_project\\new_DXF\\chaoge.lua";
		}else if (((CButton *)GetDlgItem(IDC_RADIO6))->GetCheck())
		{
			CString strBuf;
			GetDlgItemText(IDC_EDIT_LUA_PATH,strBuf);
			char temp[256] ;
			sprintf(temp,"D:\\VS_project\\new_DXF\\%s.lua",strBuf.GetBuffer());

			fileName=temp;
			//AfxMessageBox(fileName);
		}else{
			return;
		}
		CString strBuf,strTmp;
		GetDlgItemText(IDC_EDIT_LUA_PATH,strBuf);

		if (fileName.IsEmpty())
		{
			//AfxMessageBox("选择要操作的脚本！");
		}else{
			//	AfxMessageBox(fileName);
		}

		scriptThreadId = (HANDLE)_beginthreadex(NULL,0,DoScript,0,0,0);
	}
}

//注册lua
BOOL CPAGE8::OnInitDialog()
{
	CDialog::OnInitDialog();

#ifdef _LUA_DLL

	L=lua_open();
	lua_register(L,"sleep",sleepLua);
	lua_register(L,"MoveTo",moveToLua);
	lua_register(L,"LeftClick",leftClickLua);
	lua_register(L,"RightClick",rightClickLua);
#endif

	/*lua_register(L,"go",go);
	lua_register(L,"use",use);

	lua_register(L,"gos",goWithSleep);
	lua_register(L,"accept",acceptTask);
	lua_register(L,"finish",finishTask);
	lua_register(L,"levelUp",levelUpOne);
	lua_register(L,"keyPress",keyBd);
	lua_register(L,"keyPressS",keyBdS);
	lua_register(L,"diaoxiang",diaoxiang);
	lua_register(L,"isFinish",isTaskFinishLua);
	lua_register(L,"doTask",doTask);
	lua_register(L,"checkAmount",checkAmount);
	lua_register(L,"use",useLua);
	lua_register(L,"patFirst",patFirstLua);
	lua_register(L,"studySkill",studySkillLua);
	lua_register(L,"upEquipment",upEquipmentLua);
	lua_register(L,"init",initLua);
	lua_register(L,"studyWorkSkill",studyWorkSkillLua);
	lua_register(L,"qianBaiWan",qianBaiWanLua);
	lua_register(L,"storeOperate",storeOperateLua);
	lua_register(L,"clickLink",clickLinkLua);
	lua_register(L,"transfer",transferLua);
	lua_register(L,"tianTransfer",tianTransferLua);
	lua_register(L,"getNowFinish",getNowFinishLua);
	lua_register(L,"speak",speakLua);
	lua_register(L,"haveTask",haveTaskLua);
	lua_register(L,"mylog",mylogLua);
	lua_register(L,"haveStatus",haveStatusLua);
	lua_register(L,"checkMonter",checkMonterLua);
	lua_register(L,"checkMonterAmount",checkMonterAmountLua);

	lua_register(L,"skill",skill);
	lua_register(L,"buy",buy);
	lua_register(L,"sell",sell);
	lua_register(L,"npc",npc);
	lua_register(L,"clicknpc",clicknpc);
	lua_register(L,"link",link);//交接任务，主要是跟npc对话等   OK
	lua_register(L,"tran",tran);//传送  OK
	lua_register(L,"go",go);//自动寻路  OK
	lua_register(L,"open",open);//打开仓库   OK
	lua_register(L,"pick",mypick);//拾取物品
	lua_register(L,"sel",sel);//选怪
	lua_register(L,"live",live);//复活  OK
	lua_register(L,"yanzheng",yanzheng);//过验证
	lua_register(L,"change",change);//换武魂
	*/

	//根据等级设置默认的脚本
	int level=0;
	if (level<15)
	{
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	}else if (level>=15&&level<20)
	{
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
	}else{
		((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
	}

	m_page8_command.InsertString(0, _T("LeftClick (  1, 1 )"));
	m_page8_command.InsertString(1, _T("RightClick (  1, 1 )"));
	m_page8_command.InsertString(2, _T("LeftClickDown (  1, 1 )"));
	m_page8_command.InsertString(3, _T("LeftClickUp (  1, 1 )"));
	m_page8_command.InsertString(4, _T("RightClickDown (  1, 1 )"));
	m_page8_command.InsertString(5, _T("RightClickUp (  1, 1 )"));
	m_page8_command.InsertString(6, _T("MoveTo (  1, 1 )"));


	return TRUE; 
}

void CPAGE8::OnCbnSelchangeComboPage8Command()
{
	int nIndex = m_page8_command.GetCurSel();
	CString str;
	m_page8_command.GetLBText( nIndex, str);
	m_page8_info.SetWindowTextW(str);
}
