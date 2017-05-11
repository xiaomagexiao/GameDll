#include "afxwin.h"
#include "afxcmn.h"
#if !defined(AFX_PAGE3_H__CCF5F135_2DB5_41EA_B582_C4A34B4497DD__INCLUDED_)
#define AFX_PAGE3_H__CCF5F135_2DB5_41EA_B582_C4A34B4497DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPAGE3 dialog

class CPAGE3 : public CDialog
{

public:
	CPAGE3(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_PAGE3 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit_page3;
	afx_msg void OnBnClickedButtonHookDis();
	afx_msg void OnBnClickedButtonUnhookDis();
	virtual BOOL OnInitDialog();
	CComboBox m_message_type_box;
	afx_msg void OnCbnSelchangeComboPage3();
	CComboBox m_net_message_type;
	afx_msg void OnBnClickedButtonHookSend();
	afx_msg void OnBnClickedButtonUnhookSend();
	afx_msg void OnCbnSelchangeComboPage3Send();
	afx_msg void OnBnClickedButtonHookSys();
	afx_msg void OnBnClickedButtonUnhookSys();
	afx_msg void OnBnClickedButtonShowInfo();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_combox_sys_type;
	afx_msg void OnBnClickedButtonFunctionName();
	afx_msg void OnBnClickedButtonWatchSys();
	afx_msg void OnBnClickedButtonUnwatchSys();
	afx_msg void OnBnClickedButtonCallfunctionTest();
	afx_msg void OnBnClickedCheckWatch();
	afx_msg void OnBnClickedButtonSetwatch();
	afx_msg void OnBnClickedCheckWatchRecord();
	CListCtrl m_list_page3;
	afx_msg void OnNMDblclkListPage3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckWatch2();
	afx_msg void OnBnClickedCheckWatchRecord2();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedCheckWatchTcp();
	afx_msg void OnBnClickedCheckWatchRecordTcp();
	afx_msg void OnCbnSelchangeComboCompareudp();
	afx_msg void OnCbnSelchangeComboComparetcp();
	CComboBox m_tcp_compare_type;
	CComboBox m_udp_compare_type;
};



#endif 
