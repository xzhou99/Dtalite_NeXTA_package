#pragma once
#include "afxwin.h"
#include "TLiteDoc.h"

// CDlgLinkProperties dialog

class CDlgLinkProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgLinkProperties)

public:

	int m_NumLeftTurnLanes, m_NumRightTurnLanes;
	bool m_bEditChange;
	CDlgLinkProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLinkProperties();

	void EnableDataBasedOnLinkType();
// Dialog Data
	enum { IDD = IDD_DIALOG_LinkProperties };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTLiteDoc* m_pDoc;

	float m_LinkLocationX;
	float m_LinkLocationY;


	bool m_bTransitModeFlag;


	int FromNode;
	int ToNode;
	float LinkLength;
	float SpeedLimit;
	float FreeFlowTravelTime;
	int LaneCapacity;
	int nLane;
	int LinkType;

	CComboBox m_LinkTypeComboBox;

	
	float DefaultSpeedLimit;
	int DefaultCapacity;
	int DefaultnLane;
    int DefaultLinkType;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSaveasdefault();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCombo1();
	long m_LinkID;
	CString StreetName;
	float m_BPR_Alpha;
	float m_BPR_Beta;
	afx_msg void OnEnChangeEditLength();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnEnChangeEditStreetName();
	afx_msg void OnEnChangeEditSpeedlimit();
	afx_msg void OnEnChangeEditNumlanes();
	afx_msg void OnEnChangeEditLaneCapacity();
	afx_msg void OnEnChangeEditSaturationFlowRate();
	afx_msg void OnEnChangeEditEffectiveGreenTime();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	int m_AADT;
	int m_PeakHourlyVolume;
	BOOL m_bUpdateLinkAttributeBasedOnType;
	afx_msg void OnEnChangeEditAadt();
	afx_msg void OnEnChangeEditPeakhourlyvolume();
	afx_msg void OnClose();
	afx_msg void OnEnChangeEditNumlanesLeftTurn();
	afx_msg void OnEnChangeEditNumlanesRightTurn();
	CString m_ModeCode;
	afx_msg void OnEnChangeEdit8();
	BOOL m_prohibited_u_turn;
	afx_msg void OnBnClickedCheck2();
	double m_LeftTurnLength;
	double m_RightTurnLength;
	double m_KJam;
	double m_Grade;
	CString m_CountSensorID;
	CString m_SpeedSensorID;
	afx_msg void OnBnClickedCountsensorId();
	afx_msg void OnBnClickedSpeedsensorId();
};
