#pragma once
#include "afxwin.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"

// CDlgAssignmentSettings dialog

class CDlgAssignmentSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlgAssignmentSettings)

public:

	CTLiteDoc* m_pDoc;

	CDlgAssignmentSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAssignmentSettings();

// Dialog Data
	enum { IDD = IDD_DIALOG_Settings };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	bool m_bModifiedFlag;

	afx_msg void OnEnChangeEdit1();
	int m_NumberOfIterations;
	float m_DemandGlobalMultiplier;
	int m_SimultionMethod;
	int m_DemandLoadingMode;
	virtual BOOL OnInitDialog();
	CListBox m_SimulationMethodControl;
	afx_msg void OnLbnSelchangeListSimulationMethod();
	CListBox m_DemandLoadingModeList;
	afx_msg void OnBnClickedOk();
	void UpdateScenarioFile();
	afx_msg void OnBnClickedButtonCopyVehicleFile();
	int m_SimulationHorizon;
	int m_agent_based_assignment_flag;
	afx_msg void OnLbnSelchangeListDemandLoadingMode();
	afx_msg void OnLbnSelchangeListRoutingMethod();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnLbnSelchangeListDemandLoadingMode2();
	CListBox m_NetworkDataList;
	afx_msg void OnBnClickedCheckEmissionData();
	float m_DemandLoadingMultipler;
	afx_msg void OnEnChangeEditDemandLoadingmultiplier();
	afx_msg void OnLbnSelchangeListSignalControlRepresentation();
	afx_msg void OnEnChangeEditNumberIterations3();
	int m_NumberReportingDays;
	CListBox m_ScenarioList;
	afx_msg void OnLbnSelchangeListScenario();
	afx_msg void OnBnClickedButtonconfiguration();
	afx_msg void OnBnClickedConfig();
	CString m_SimulatorName;
	afx_msg void OnBnClickedButtonViewEditScenarioSetting();

	CListBox m_SignalMethodList;
};
