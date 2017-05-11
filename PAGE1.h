#include "afxwin.h"
#if !defined(AFX_PAGE1_H__1081D3A8_E080_476D_9E3E_C57F0B220009__INCLUDED_)
#define AFX_PAGE1_H__1081D3A8_E080_476D_9E3E_C57F0B220009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPAGE1 dialog

class CPAGE1 : public CDialog
{
// Construction
public:
	CPAGE1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPAGE1)
	enum { IDD = IDD_PAGE1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPAGE1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD m_dwDisaptchMessage;
	// Generated message map functions
	//{{AFX_MSG(CPAGE1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonChange();
	CEdit m_edit_page1;
	afx_msg void OnBnClickedButtonHook();
	afx_msg void OnBnClickedButtonUnhook();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonAddshow();
	afx_msg void OnBnClickedButtonSubshow();
	afx_msg void OnBnClickedCheckOnly();
	afx_msg void OnBnClickedButtonPic();
	afx_msg void OnBnClickedButtonHwnd();
	afx_msg void OnBnClickedButtonMsg();
	afx_msg void OnBnClickedButtonMessage();
	afx_msg void OnBnClickedButtonStart();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE1_H__1081D3A8_E080_476D_9E3E_C57F0B220009__INCLUDED_)
