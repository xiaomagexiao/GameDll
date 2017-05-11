#include "afxwin.h"
#if !defined(AFX_PAGE7_H__A6E54666_2CE0_43D1_A784_68CA53B41CCE__INCLUDED_)
#define AFX_PAGE7_H__A6E54666_2CE0_43D1_A784_68CA53B41CCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE7.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPAGE7 dialog

class CPAGE7 : public CDialog
{
// Construction
public:
	CPAGE7(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPAGE7)
	enum { IDD = IDD_PAGE7 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPAGE7)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPAGE7)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_page7_info;
	afx_msg void OnBnClickedButtonLoaddriver();
	afx_msg void OnBnClickedButtonUnload();
	afx_msg void OnBnClickedButtonAnjian();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE7_H__A6E54666_2CE0_43D1_A784_68CA53B41CCE__INCLUDED_)
