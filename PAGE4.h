#include "afxwin.h"
#if !defined(AFX_PAGE4_H__8BA8B31E_A1A4_4339_A000_869F20B9308F__INCLUDED_)
#define AFX_PAGE4_H__8BA8B31E_A1A4_4339_A000_869F20B9308F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPAGE4 dialog

class CPAGE4 : public CDialog
{
// Construction
public:
	CPAGE4(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPAGE4)
	enum { IDD = IDD_PAGE4 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPAGE4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPAGE4)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit_page4;
	afx_msg void OnBnClickedButtonPatchCheck();
	afx_msg void OnBnClickedButtonUnhookCheck();
	afx_msg void OnBnClickedButtonDataHook();
	afx_msg void OnBnClickedButtonDataUnhook();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonCancleall();
	afx_msg void OnBnClickedButtonLolhook();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedCheckEax();
	afx_msg void OnBnClickedCheckEbx();
	afx_msg void OnBnClickedCheckEcx();
	afx_msg void OnBnClickedCheckEdx();
	afx_msg void OnBnClickedCheckEdi();
	afx_msg void OnBnClickedCheckEsi();
	afx_msg void OnBnClickedCheckPara();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE4_H__8BA8B31E_A1A4_4339_A000_869F20B9308F__INCLUDED_)
