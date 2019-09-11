#pragma once
#include "afxwin.h"


// CDlg_RealWorldWidth dialog

class CDlg_RealWorldWidth : public CDialog
{
	DECLARE_DYNAMIC(CDlg_RealWorldWidth)

public:

	CDlg_RealWorldWidth(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_RealWorldWidth();

// Dialog Data
	enum { IDD = IDD_DIALOG_Image_World_Width };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonok();
	virtual BOOL OnInitDialog();
	double m_RealWorldWidth;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonLoadFile();
	afx_msg void OnPaint();
	CComboBox m_DistanceUnitCombo;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_DistanaceUnitCombo;
};
