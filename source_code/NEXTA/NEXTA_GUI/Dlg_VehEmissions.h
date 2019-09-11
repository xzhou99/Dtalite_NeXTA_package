#pragma once
#include "afxwin.h"
#include "TLiteDoc.h"
#include "BaseDialog.h"
#include "network.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"

// CDlg_VehPathAnalysis dialog





class CDlg_VehPathAnalysis : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlg_VehPathAnalysis)

public:
	CDlg_VehPathAnalysis(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_VehPathAnalysis();

	CGridListCtrlEx m_ListCtrl;
	CGridListCtrlEx m_PathListCtrl;
	CTLiteDoc* m_pDoc;
	VehicleStatistics*** m_ODMOEMatrix;

	int m_ProjectSize;
	int m_OldProjectSize;

	std::vector<int> m_TAZVector;
	int m_SelectedOrigin;
	int m_SelectedDestination;
	int m_ZoneNoSize ;
	int m_SelectedPath;

	std::vector<PathStatistics> m_PathVector;

	void FilterOriginDestinationPairs();
	void FilterPaths();
	void ShowSelectedPath();
	void ShowVehicles();

	bool ExportDataToCSVFileAllOD(CString fname);
	bool ExportPathDataToCSVFile(CString fname);
	bool ExportVehicleDataToCSVFile(CString fname);

	void ExportData(CString fname);

// Dialog Data
	enum { IDD = IDD_DIALOG_VEHICLE_PATH };
	int m_GPS_start_day;
	int m_GPS_end_day;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_VehicleList;
	afx_msg void OnLbnSelchangeListLink();
	CListBox m_LinkList;
	CComboBox m_OriginBox;
	CComboBox m_DestinationBox;
	CComboBox m_DepartureTimeBox;
	CComboBox m_VehicleTypeBox;
	CComboBox m_DemandTypeBox;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListVehicle();
	afx_msg void OnLbnSelchangeListLink2();
	CListBox m_OpModeList;
	afx_msg void OnCbnSelchangeComboOrigin();
	afx_msg void OnCbnSelchangeComboDestination();
	afx_msg void OnCbnSelchangeComboDeparturetime();
	afx_msg void OnCbnSelchangeComboVehicletype();
	afx_msg void OnCbnSelchangeComboInformationclass();
	CComboBox m_InformationClassBox;
	CComboBox m_MinVehicleSizeBox;
	CComboBox m_MinDistanceBox;
	CComboBox m_MaxSpeedBox;
	CComboBox m_TimeIntervalBox;
	CListBox m_ODList;
	afx_msg void OnCbnSelchangeComboTimeinterval();
	afx_msg void OnCbnSelchangeComboMinNumberofvehicles();
	afx_msg void OnCbnSelchangeComboMinTravelTime();
	afx_msg void OnCbnSelchangeComboMinTraveltimeindex();

	afx_msg void OnLbnSelchangeListOd();
	afx_msg void OnLbnSelchangeListPath();
	CComboBox m_ImpactLinkBox;
	afx_msg void OnCbnSelchangeComboImpactlink();
	afx_msg void OnLbnDblclkListOd();
	afx_msg void OnBnClickedExport();
	CEdit m_Summary_Info_Edit;
	CComboBox m_ComboBox_VOT_LB;
	CComboBox m_ComboBox_VOT_UB;
	afx_msg void OnCbnSelchangeComboVotLb();
	afx_msg void OnCbnSelchangeComboVotUb();

	afx_msg void OnCbnSelchangeComboDemandtype();
	long m_SingleVehicleID;
	afx_msg void OnBnClickedFindSingleVehicleId();
	afx_msg void OnBnClickedExportPathData();
	afx_msg void OnBnClickedExportVehicleData();
	afx_msg void OnBnClickedFindcriticalod();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPathLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_DayNo_Combobox;
	afx_msg void OnCbnSelchangeComboDayno();
	afx_msg void OnLvnItemchangedList5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBarchart();
	afx_msg void OnClose();
	BOOL m_VMS_Responsive_Flag;
	afx_msg void OnBnClickedCheckVmsResponsive();
};
