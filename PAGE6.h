#if !defined(AFX_PAGE6_H__CC9EBB62_E329_469B_9522_B3335D7E26B5__INCLUDED_)
#define AFX_PAGE6_H__CC9EBB62_E329_469B_9522_B3335D7E26B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE6.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPAGE6 dialog

class CPAGE6 : public CDialog
{
// Construction
public:
	CPAGE6(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPAGE6)
	enum { IDD = IDD_PAGE6 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPAGE6)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPAGE6)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE6_H__CC9EBB62_E329_469B_9522_B3335D7E26B5__INCLUDED_)
