// PAGE5.cpp : implementation file
//

#include "stdafx.h"
#include "DLL.h"
#include "PAGE5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPAGE5 dialog


CPAGE5::CPAGE5(CWnd* pParent /*=NULL*/)
	: CDialog(CPAGE5::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPAGE5)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPAGE5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAGE5)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPAGE5, CDialog)
	//{{AFX_MSG_MAP(CPAGE5)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPAGE5 message handlers
