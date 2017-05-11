#if !defined(AFX_MAINVIEW_H__1459D14B_8F57_4C4D_84C9_A6D459627125__INCLUDED_)
#define AFX_MAINVIEW_H__1459D14B_8F57_4C4D_84C9_A6D459627125__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainView.h : header file
//
#include "PAGE1.h"
#include "PAGE2.h"
#include "PAGE3.h"
#include "PAGE4.h"
#include "PAGE5.h"
#include "PAGE6.h"
#include "PAGE7.h"
#include "PAGE8.h"


/////////////////////////////////////////////////////////////////////////////
// CMainView dialog

class CMainView : public CDialog
{
// Construction
public:
	CMainView(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainView)
	enum { IDD = IDD_DIALOG_MainView };
	CTabCtrl	m_MainWnd_Tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainView)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeMainWndTAB(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CPAGE1 *m_Page1;
	CPAGE2 *m_Page2;
	CPAGE3 *m_Page3;
	CPAGE4 *m_Page4;
	CPAGE5 *m_Page5;
	CPAGE6 *m_Page6;
	CPAGE7 *m_Page7;
	CPAGE8 *m_Page8;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINVIEW_H__1459D14B_8F57_4C4D_84C9_A6D459627125__INCLUDED_)
