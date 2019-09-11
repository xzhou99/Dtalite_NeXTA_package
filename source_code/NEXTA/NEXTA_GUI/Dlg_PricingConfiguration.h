#pragma once

#include "Chart_Control\\NSChartCtrl.h"
// CDlg_PricingConfiguration dialog
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "afxwin.h"

class CDlg_PricingConfiguration : public CDialog
{
//	DECLARE_DYNAMIC(CDlg_PricingConfiguration)

public:
	CDlg_PricingConfiguration(CWnd* pParent = NULL);   // standard constructor

	CTLiteDoc* m_pDoc;
// Dialog Data
	enum { IDD = IDD_NSCHART_TOLL_DIALOG };

// Dialog Data
	//{{AFX_DATA(CDlg_PricingConfiguration)
	float	m_dValue;
	CString	m_sLabel;
	//}}AFX_DATA

	CNSChartCtrl m_chart;

	protected:
	void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	int m_StartTimeInterval, m_EndTimeInterval;
	INT m_iItem;
	HICON m_hIcon;
	void OnUpdateModify(CCmdUI* pCmdUI);
	void OnUpdateAdd(CCmdUI* pCmdUI);
	// Generated message map functions
	//{{AFX_MSG(CDlg_PricingConfiguration)
	BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRadioButtons();
	afx_msg void OnRadioColors();
	afx_msg void OnAdd();
	afx_msg void OnChartSelectedItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDel();
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_DemandTypeList;
	afx_msg void OnLbnSelchangeDemandProfileList();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedVotDistribution();
};
