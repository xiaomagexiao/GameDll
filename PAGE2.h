#include "afxcmn.h"
#if !defined(AFX_PAGE2_H__46285B1E_F362_48C4_8085_64D13EF5F692__INCLUDED_)
#define AFX_PAGE2_H__46285B1E_F362_48C4_8085_64D13EF5F692__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPAGE2 dialog

class CPAGE2 : public CDialog
{
// Construction
public:
	CPAGE2(CWnd* pParent = NULL);   // standard constructor
    BOOL OnInitDialog();
// Dialog Data
	//{{AFX_DATA(CPAGE2)
	enum { IDD = IDD_PAGE2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPAGE2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPAGE2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonClear();
	CListCtrl m_DataListCtrl;
	afx_msg void OnBnClickedButtonTestMal();
	afx_msg void OnBnClickedButtonMalloc();
	afx_msg void OnBnClickedButtonUnmalloc();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE2_H__46285B1E_F362_48C4_8085_64D13EF5F692__INCLUDED_)
