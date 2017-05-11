// PAGE6.cpp : implementation file
//

#include "stdafx.h"
#include "DLL.h"
#include "PAGE6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPAGE6 dialog


CPAGE6::CPAGE6(CWnd* pParent /*=NULL*/)
	: CDialog(CPAGE6::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPAGE6)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPAGE6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAGE6)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPAGE6, CDialog)
	//{{AFX_MSG_MAP(CPAGE6)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPAGE6 message handlers
