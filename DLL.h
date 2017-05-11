// DLL.h : main header file for the DLL DLL
//

#if !defined(AFX_DLL_H__FDBC6305_6F83_4DB9_8748_09C746A3892D__INCLUDED_)
#define AFX_DLL_H__FDBC6305_6F83_4DB9_8748_09C746A3892D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDLLApp
// See DLL.cpp for the implementation of this class
//

class CDLLApp : public CWinApp
{
public:
	CDLLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLLApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLL_H__FDBC6305_6F83_4DB9_8748_09C746A3892D__INCLUDED_)
