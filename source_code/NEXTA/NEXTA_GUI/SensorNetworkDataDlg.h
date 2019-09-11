#pragma once
#include "SensorNetworkDataTabDlg.h"
#include "afxcmn.h"
#include "TLiteDoc.h"


// CSensorNetworkDataSettingDlg dialog
class CSensorNetworkDataSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSensorNetworkDataSettingDlg)

public:
	CSensorNetworkDataSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSensorNetworkDataSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NETWORK_SENSOR_DATA};

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
	CSensorNetworkDataTabDlg** p_SubTabs;
	BOOL ReadNetworkDataCSVFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector);
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
};
