#pragma once
#include "Geometry.h"
#include "CSVParser.h"
#include "RecordSetExt.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "afxwin.h"


// CDlg_ImportNetwork dialog

class CDlg_ImportNetwork : public CDialog
{
	DECLARE_DYNAMIC(CDlg_ImportNetwork)

public:

	CDlg_ImportNetwork(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ImportNetwork();

	CTLiteDoc* m_pDoc;

	bool m_bImportNetworkOnly;

// Dialog Data
	enum { IDD = IDD_DIALOG_EXCEL_DATA_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void ImportAMSDataSet();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFindExelFile();
	afx_msg void OnBnClickedButtonFindDemandCsvFile();
	afx_msg void OnBnClickedImport();
	CString m_Edit_Excel_File;
	CString m_Edit_Demand_CSV_File;
	CListBox m_MessageList;
	afx_msg void OnBnClickedImportNetworkOnly();
	CString m_Sensor_File;
	afx_msg void OnBnClickedButtonFindSensorFile();
	afx_msg void OnBnClickedImportSensorData();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedExportData();
	afx_msg void OnBnClickedButtonViewSampleFile();
	afx_msg void OnBnClickedButtonLoadSampleFile();
	afx_msg void OnBnClickedButtonViewSampleCsvFile();
	afx_msg void OnBnClickedButtonLoadSampleCsvFile();
	afx_msg void OnBnClickedButtonViewSampleProjectFolder();
	afx_msg void OnBnClickedImport2();
	BOOL m_bRemoveConnectors;
	CListBox m_List_DemandFormat;
	virtual BOOL OnInitDialog();
	BOOL m_AutogenerateNodeFlag;
	BOOL m_ImportZoneData;
	BOOL m_bAddConnectorsForIsolatedNodes;
	BOOL m_bUseLinkTypeForDefaultValues;
	BOOL m_bLinkMOECheck;
	BOOL m_TMCSpeedCheck;
	BOOL m_SensorCountCheck;
};

