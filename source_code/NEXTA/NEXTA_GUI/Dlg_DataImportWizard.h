#pragma once
#include "afxwin.h"


// CDlg_DataImportWizard dialog

class CDlg_DataImportWizard : public CDialog
{
	DECLARE_DYNAMIC(CDlg_DataImportWizard)

public:
	CDlg_DataImportWizard(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_DataImportWizard();

// Dialog Data
	enum { IDD = IDD_DIALOG_GIS_IMPORTING_GUIDE_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bNode;
	BOOL m_bLink;
	BOOL m_bZone;
	BOOL m_bCentroid;
	BOOL m_bConnector;
	BOOL m_bTMCSpeed;
	BOOL m_bSensorCount;
	BOOL m_bLinkMOE;
	BOOL m_bTrajectory;
	BOOL m_bGPS;
	BOOL m_bGoogleTransitFeed;
	afx_msg void OnBnClickedChecknode();
	afx_msg void OnBnClickedChecklink();
	afx_msg void OnBnClickedCheckzone();
	afx_msg void OnBnClickedCheckcentroid();
	afx_msg void OnBnClickedCheckconnector();
	afx_msg void OnBnClickedChecktmc();
	afx_msg void OnBnClickedChecksensorcount();
	afx_msg void OnBnClickedChecklinkmoe();
	afx_msg void OnBnClickedCheckTrajectory();
	afx_msg void OnBnClickedCheckgpsdata();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedCheckzone2();
	afx_msg void OnBnClickedButtonopenFolder();
	afx_msg void OnBnClickedButtonopenFolder2();
	afx_msg void OnBnClickedOk();
	CListBox m_DemandDataList;
	CEdit m_Message;
};
