#pragma once
#include <vector>
#include <string>
#include "afxwin.h"


// CDlg_LeftTurnLaneSetup dialog

class CDlg_LeftTurnLaneSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlg_LeftTurnLaneSetup)

public:
	CDlg_LeftTurnLaneSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_LeftTurnLaneSetup();

// Dialog Data
	enum { IDD = IDD_DIALOG_LEFT_TURN_LANES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_LeftturnVolume1;
	int m_LeftturnVolume2;
	int m_LeftturnVolume3;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
