#pragma once


// CDlg_ImportPlanningModel dialog

class CDlg_ImportPlanningModel : public CDialog
{
	DECLARE_DYNAMIC(CDlg_ImportPlanningModel)

public:
	CDlg_ImportPlanningModel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ImportPlanningModel();

// Dialog Data
	enum { IDD = IDD_DIALOG_PLANNING_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonViewDspFile();
	afx_msg void OnBnClickedButtonImportDspFile();
	afx_msg void OnBnClickedViewSampleTransimsFile();
	afx_msg void OnBnClickedButtonImportTransims();
};
