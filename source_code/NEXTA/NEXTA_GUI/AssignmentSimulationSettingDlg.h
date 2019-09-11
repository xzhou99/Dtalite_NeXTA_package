#pragma once
#include "AssignmentSimulationTabDlg.h"
#include "afxcmn.h"
#include "TLiteDoc.h"


// CAssignmentSimulationSettingDlg dialog
class CAssignmentSimulationSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CAssignmentSimulationSettingDlg)

public:
	CAssignmentSimulationSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAssignmentSimulationSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ASSIGNMENTSIM_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CTLiteDoc* m_pDoc;
	
private:
	CTabCtrl m_TabCtrl;
	CAssignmentSimulationTabDlg** p_SubTabs;
	BOOL ReadScenarioCSVFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector);
	void SetRectangle();
	int m_SelectTab;
	int m_PrevTab;
public:
	afx_msg void OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonEditDataInExcel();
	afx_msg void OnBnClickedButtonDelete();
};
