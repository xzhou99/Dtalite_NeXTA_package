#pragma once
#include "afxwin.h"
#include "TLiteDoc.h"
#include "BaseDialog.h"
enum e_TDMOE_ColorMode { e_TDMOE_number_of_lanes = 0,e_TDMOE_speed_limit_mph,e_TDMOE_speed_limit_kmph,
e_TDMOE_link_capacity, e_TDMOE_lane_capacity,
e_TDMOE_separator_1,
e_TDMOE_volume,
e_TDMOE_volume_difference, 

e_TDMOE_user_defined_attribue}
;
// CDlg_TDMOE_Configuration dialog

class CDlg_TDMOE_Configuration : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlg_TDMOE_Configuration)

public:
	CDlg_TDMOE_Configuration(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_TDMOE_Configuration();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIG_TDMOE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	std::vector<CString> m_ProjectTitleVector;
	std::vector<CTLiteDoc*> m_pProjectDocVector;

	CTLiteDoc* m_pDoc;
	std::vector<int> m_AggregationValueVector;

	void UpdateCategoryValues();
	e_TDMOE_ColorMode m_KML_MOE_selection;
	afx_msg void OnBnClickedButtonSpaceevenly();
	float m_ColorCategoryValue[10];
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeList1();
	virtual BOOL OnInitDialog();
	CListBox m_List;
	CStatic m_ColorRampStatic;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonReverse();
	CListBox m_ProjectList;
	CComboBox m_AggregationIntervalList;
	afx_msg void OnCbnSelchangeComboAggInterval();
	afx_msg void OnLbnSelchangeProjectList();
};
