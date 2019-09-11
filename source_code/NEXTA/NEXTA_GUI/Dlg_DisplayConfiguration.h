#pragma once
#include "afxwin.h"
#include "TLiteDoc.h"
#include "TLiteView.h"
#include "BaseDialog.h"

// CDlg_DisplayConfiguration dialog

class CDlg_DisplayConfiguration : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlg_DisplayConfiguration)

public:
	CDlg_DisplayConfiguration(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_DisplayConfiguration();

	CTLiteView* pView;
// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIG_GIS_TEXT_DISPLAY };

	std::vector<int> aggregation_interval_vector;

	std::vector<int> movement_text_size_vector;
	std::vector<float> walking_distance_vector;
	std::vector<int> transit_time_vector;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	std::vector<int> m_AggregationValueVector;
	afx_msg void OnLbnSelchangeListLinkTextLabel();
	node_display_mode m_ShowNodeTextMode;

	CListBox m_Link_Label, m_Node_Label;
	CListBox m_Movement_Label;

	afx_msg void OnLbnSelchangeListNodeTextLabel();
	virtual BOOL OnInitDialog();
	CListBox m_List_Node_Label;
	CListBox m_AggregationIntervalList;
	afx_msg void OnLbnSelchangeMoeAggregationIntervalList();
	afx_msg void OnLbnSelchangeListZoneTextLabel();
	afx_msg void OnBnClickedOk();

	afx_msg void OnLbnSelchangeListMovementTextLabel();
	CListBox m_GPS_Label;
	afx_msg void OnLbnSelchangeListGpsTextLabel();
	afx_msg void OnBnClickedButtonIncreasenodesize();
	afx_msg void OnBnClickedButtonDecreasenodesize();
	afx_msg void OnBnClickedButtonIncreasenodetextsize();
	afx_msg void OnBnClickedButtonDecreasenodetextsize();
	CListBox m_SizeTextControl_List;
	afx_msg void OnLbnDblclkListSizeTextControl();
	afx_msg void OnCbnSelchangeComboWalkingdistance();
	afx_msg void OnCbnSelchangeComboTransittime();
	afx_msg void OnLbnSelchangeListSizeTextControl();
	afx_msg void OnBnClickedButtonIncreaseSize();
	afx_msg void OnBnClickedButtonDecreaseSize();
	BOOL m_bShowSignalNodeMovementOnly;
	afx_msg void OnBnClickedCheckSignalNodeOnly();
	afx_msg void OnBnClickedButtonIncreaseTextSize();
	afx_msg void OnBnClickedButtonDecreaseTextSize();
	afx_msg void OnBnClickedButtonIncreaseNodeTextSize();
	afx_msg void OnBnClickedButtonDecreaseNodeTextSize();
	afx_msg void OnBnClickedButtonIncreaseLinkTextSize2();
	afx_msg void OnBnClickedButtonDecreaseLinkTextSize2();
	CComboBox m_TimingPlanComboBox;
	afx_msg void OnCbnSelchangeCombotimingplan();
	CComboBox m_ComboDataSourceType;
	afx_msg void OnCbnSelchangeComboDatasource();
	CComboBox m_Combo_Aggregation_Interval;
	afx_msg void OnCbnSelchangeComboAggregationInterval();
	CListBox m_VehicleIDList;
	afx_msg void OnLbnSelchangeListGpsCarTextLabel();
	afx_msg void OnLbnDblclkListGpsCarTextLabel();
	CListBox m_PassengerIDList;
	afx_msg void OnLbnSelchangeListGpsDayTextLabel2();
	afx_msg void OnLbnDblclkListGpsDayTextLabel2();
};
