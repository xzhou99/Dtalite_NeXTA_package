#pragma once

#include "GIS_Import_DataTabDlg.h"
#include "afxcmn.h"
#include "TLiteDoc.h"
// CGIS_Import_DataDlg dialog

class CGIS_Import_DataDlg : public CDialog
{
	DECLARE_DYNAMIC(CGIS_Import_DataDlg)

public:
	CGIS_Import_DataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGIS_Import_DataDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING_DATA_GIS_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CTLiteDoc* m_pDoc;
	int m_SelectTab;
	
	int m_SelectedFromNodeName;
	int m_SelectedToNodeName;

	CTabCtrl m_TabCtrl;
	CGIS_Import_DataTabDlg** p_SubTabs;
	BOOL ReadGIS_Import_DataCSVFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector);
	void SetRectangle();


	int m_PrevTab;
public:
	afx_msg void OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonEditDataInExcel();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCheckZoomToSelectedObject();
	BOOL m_ZoomToSelectedObject;
	afx_msg void OnNMClickSettingTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingSettingTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedGisFieldName();
};
