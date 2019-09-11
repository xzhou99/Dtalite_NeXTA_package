#pragma once

#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "afxwin.h"

// CDlg_SignalDataExchange dialog

class CDlg_SignalDataExchange : public CDialog
{
	DECLARE_DYNAMIC(CDlg_SignalDataExchange)

public:
	CDlg_SignalDataExchange(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_SignalDataExchange();

	CTLiteDoc* m_pDoc;
// Dialog Data
	enum { IDD = IDD_DIALOG_SIGNAL_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGenerateData();
	afx_msg void OnBnClickedButtonQem();
	afx_msg void OnBnClickedButtonGenerateVissimData();
	afx_msg void OnBnClickedOk();
	CListBox m_InfoList;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListInfo();
	float m_PeakHourFactor;
	afx_msg void OnEnChangeEditPeakHourFactor();
};
