#include "afxwin.h"
#if !defined(AFX_PAGE8_H__498FFC4F_79B8_46A0_B6F5_AF71B7D68242__INCLUDED_)
#define AFX_PAGE8_H__498FFC4F_79B8_46A0_B6F5_AF71B7D68242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE8.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPAGE8 dialog

class CPAGE8 : public CDialog
{
	// Construction
public:
	CPAGE8(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CPAGE8)
	enum { IDD = IDD_PAGE8 };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPAGE8)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPAGE8)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString lua_value;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLuaExecute();
	afx_msg void OnBnClickedButtonLua();
	afx_msg void OnCbnSelchangeComboPage8Command();
	CComboBox m_page8_command;
	CEdit m_page8_info;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE8_H__498FFC4F_79B8_46A0_B6F5_AF71B7D68242__INCLUDED_)
