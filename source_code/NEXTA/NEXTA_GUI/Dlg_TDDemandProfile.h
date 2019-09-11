#pragma once

#include "Chart_Control\\NSChartCtrl.h"
// CDlg_TDDemandProfile dialog
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "afxwin.h"

class CDlg_TDDemandProfile : public CDialog
{
//	DECLARE_DYNAMIC(CDlg_TDDemandProfile)

public:
	CDlg_TDDemandProfile(CWnd* pParent = NULL);   // standard constructor

	CTLiteDoc* m_pDoc;
	int m_DemandLoading_StartHour;
	int m_DemandLoading_EndHour;

// Dialog Data
	enum { IDD = IDD_NSCHART_DEMAND_DIALOG };

// Dialog Data
	//{{AFX_DATA(CDlg_TDDemandProfile)
	float	m_dValue;
	CString	m_sLabel;
	//}}AFX_DATA

	CNSChartCtrl m_chart;

	protected:
	void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	INT m_iItem;
	HICON m_hIcon;
	void OnUpdateModify(CCmdUI* pCmdUI);
	void OnUpdateAdd(CCmdUI* pCmdUI);
	// Generated message map functions
	//{{AFX_MSG(CDlg_TDDemandProfile)
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
	CListBox m_ODProfileList;
	afx_msg void OnLbnSelchangeDemandProfileList();
	afx_msg void OnBnClickedOk();
	CComboBox m_StartTimeCombo;
	CComboBox m_EndTimeCombo;
};
