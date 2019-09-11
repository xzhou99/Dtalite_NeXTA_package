#pragma once
#include "Chart_Control\\NSChartCtrl.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "afxwin.h"
#include <vector>

using std::vector;

#define MAX_SAMPLE_SIZE 200

// CDlg_TravelTimeReliability dialog

class CDlg_TravelTimeReliability : public CDialog
{
	DECLARE_DYNAMIC(CDlg_TravelTimeReliability)

public:
	CDlg_TravelTimeReliability(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_TravelTimeReliability();

	CNSChartCtrl m_chart_traveltime;
	CNSChartCtrl m_chart_7factors;
	INT m_iItem;
	HICON m_hIcon;

	CTLiteDoc* m_pDoc;

	CString m_FactorLabel[10];
	int m_FactorSize;

	float m_PathFreeFlowTravelTime;
	
	vector<float> LinkCapacity;
	vector<float> LinkTravelTime;

	int CalculationMethod;

	int m_BottleneckIdx;

	int m_ImpactedLinkIdx;

	bool m_bImpacted;
	float m_ImpactDuration;
	float m_LaneClosurePercentage;

	float	m_dValue;
	CString	m_sLabel;

	float Capacity[MAX_SAMPLE_SIZE];
	float AdditionalDelay[MAX_SAMPLE_SIZE];
	float TravelTime[MAX_SAMPLE_SIZE];

// Dialog Data
	enum { IDD = IDD_NSCHART_TRAVEL_TIME_RELIABILITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void ExportData(CString fname);

	// Generated message map functions
	BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRadioButtons();
	afx_msg void OnRadioColors();
	DECLARE_MESSAGE_MAP()
public:
	void DisplayTravelTimeChart();
	void Display7FactorChart();

	void UpdateCapacityAndDelay();

	afx_msg void OnLbnSelchangeList1();
	CListBox m_ReliabilityMOEList;
	afx_msg void OnLbnSelchangeList2();
	CListBox m_7FactorMOEList;
	afx_msg void OnBnClickedExportData();
	afx_msg void OnBnClickedModify();
	afx_msg void OnChartSelectedItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedEditScenario();
};
