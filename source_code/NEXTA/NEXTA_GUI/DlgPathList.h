#pragma once
#include "afxcmn.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "BaseDialog.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include "afxwin.h"

// CDlgPathList dialog
enum ePathSelectionMode {ePathSelection_AllPaths,ePathSelection_FreewayOnly,ePathSelection_RampOnly,ePathSelection_ArterialOnly,ePathSelection_NoConnectors};
enum eChangeLinkAttributeMode {
eChangeLinkAttribute_lane_capacity=0,
eChangeLinkAttribute_number_of_lanes,
eChangeLinkAttribute_link_type,
eChangeLinkAttribute_speed_limit_mph,
eChangeLinkAttribute_speed_limit_kmph,
eChangeLinkAttribute_jam_density_vhcpm,
eChangeLinkAttribute_jam_density_vhcpkm,
eChangeLinkAttribute_backwardwave_speed_mph,
eChangeLinkAttribute_backwardwave_speed_kmph,
eChangeLinkAttribute_saturation_flow_rate,
eChangeLinkAttribute_effective_green_time,
eChangeLinkAttribute_loop_code,
eChangeLinkAttribute_orientation_code,
};

class CDlgPathList : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgPathList)
	CTLiteDoc* m_pDoc;
public:

	void ChangeLinkAttributeDialog();
	void ChangeLinkAttributeAlongPath(float value, CString value_string);

	eChangeLinkAttributeMode m_ChangeLinkAttributeMode;
	int m_MOEAggregationIntervalInMin;
	void CalculateTimeDependentTravelTime();
	ePathSelectionMode m_PathSelectionMode;
	bool m_AVISensorFlag;
	CDlgPathList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPathList();

	void ReloadData();
// Dialog Data
	enum { IDD = IDD_DIALOG_PATH_LIST };

   float m_UnitDistance, m_UnitTime, m_UnitData;
	void DrawPlot(CPaintDC* pDC, CRect PlotRect);

   int m_Range;
   int m_TimeLeft, m_TimeRight;
   float m_YUpperBound;
   float m_YLowerBound;

protected:
		std::vector<int> m_AggregationValueVector;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void GenerateDynamicMOEContour(DTA_EMISSION_TYPE emission_type );
	afx_msg void OnPaint();
	CGridListCtrlEx m_ListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL m_ZoomToSelectedLink;
	CString m_StrPathMOE;
	afx_msg void OnBnClickedFindAlternative();
	afx_msg void OnBnClickedFindRandomRoute();
	afx_msg void OnCbnSelchangeCombo1();

	CComboBox m_ComboRandomCoef;
	afx_msg void OnBnClickedExportRoute();
	afx_msg void OnPathDataExportCSV();
	afx_msg void OnDataImportCsv();
	afx_msg void OnBnClickedCheckZoomToSelectedLink();
	CListBox m_PathList;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedDataAnalysis();
	afx_msg void OnDataGeneratesampleinputpathcsv();
	afx_msg void OnDataCleanallpaths();
	CComboBox m_StartHour;
	CComboBox m_EndHour;
	afx_msg void OnCbnSelchangeComboStarthour();
	afx_msg void OnCbnSelchangeComboEndhour();
	CComboBox m_AggregationIntervalList;
	afx_msg void OnCbnSelchangeComboAggintrevallist();
	afx_msg void OnCbnSelchangeComboPlotType();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CComboBox m_PlotType;
	afx_msg void OnBnClickedAnalysis2();
	afx_msg void OnBnClickedFreevalAnalysisGenerateData();
	afx_msg void OnBnClickedFreevalAnalysisGenerateFile();
	afx_msg void OnChangeattributesforlinksalongpathChangelanecapacity();
	afx_msg void OnChangeattributesforlinksalongpathChange();
	afx_msg void OnChangeattributesforlinksalongpathChangelinktype();
	afx_msg void OnChangeattributesforlinksalongpathChangespeedlimit();
	afx_msg void OnChangeattributesforlinksalongpathChangespeedlimitKmph();
	afx_msg void OnChangeattributesforlinksalongpathChangejamdensity();
	afx_msg void OnChangeattributesforlinksalongpathChangebackwavespeed();
	afx_msg void OnChangeattributesforlinksalongpathChangesaturationflowrate();
	afx_msg void OnChangeattributesforlinksalongpathChangejamdensity33625();
	afx_msg void OnChangeattributesforlinksalongpathEffectivegreentime();
	afx_msg void OnChangeattributesforlinksalongpathDeletelinksalongpath();
	afx_msg void OnBnClickedDynamicDensityContour();
	afx_msg void OnBnClickedDynamicSpeedContour();
	afx_msg void OnBnClickedDynamicFlowContour();
	afx_msg void OnDataSavecurrentpath();
	afx_msg void OnDataExportfreevalsegmentfile();
	afx_msg void OnBnClickedGoogleearth();
	afx_msg void OnBnClickedGoogleearthoutput();
	afx_msg void OnBnClickedButtonGoogleEarthKml();
	afx_msg void OnDataDeleteexistingpathsininputpathcsvfile();
	afx_msg void OnDataViewinputpathfileinexcel();
	afx_msg void OnDataAddanewpath();
	afx_msg void OnDataChangepathnameforselectedpath();
	afx_msg void OnDataDeleteselectedpath();
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnDataGenerateemissiondetourplots();
	afx_msg void OnDataCo2contourplot();
	afx_msg void OnDataCocontourplot();
	afx_msg void OnContourNoxcontourplot();
	afx_msg void OnContourHccontourplot();
	afx_msg void OnDataGenerateenergyuse();
	afx_msg void OnBnClickedPathDataExportCsv();
	afx_msg void OnChangeattributesforlinksalongpathLoopcode();
	afx_msg void OnChangeattributesforlinksalongpathDirectioncode();
};
