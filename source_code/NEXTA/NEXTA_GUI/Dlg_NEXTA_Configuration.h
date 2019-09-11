#pragma once
#include "afxwin.h"


// CDlg_NEXTA_Configuration dialog

class CDlg_NEXTA_Configuration : public CDialog
{
	DECLARE_DYNAMIC(CDlg_NEXTA_Configuration)

public:
	CDlg_NEXTA_Configuration(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_NEXTA_Configuration();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEXTA_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	int m_bRightHandTrafficFlag;

	CComboBox m_Side_Driving_Combo;
	afx_msg void OnCbnSelchangeComboSideDriving();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
