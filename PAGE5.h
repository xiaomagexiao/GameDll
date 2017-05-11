#if !defined(AFX_PAGE5_H__2F5ED1CA_ED81_4CAA_BDA3_DF9848FB01AC__INCLUDED_)
#define AFX_PAGE5_H__2F5ED1CA_ED81_4CAA_BDA3_DF9848FB01AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE5.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPAGE5 dialog

class CPAGE5 : public CDialog
{
// Construction
public:
	CPAGE5(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPAGE5)
	enum { IDD = IDD_PAGE5 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPAGE5)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPAGE5)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE5_H__2F5ED1CA_ED81_4CAA_BDA3_DF9848FB01AC__INCLUDED_)
