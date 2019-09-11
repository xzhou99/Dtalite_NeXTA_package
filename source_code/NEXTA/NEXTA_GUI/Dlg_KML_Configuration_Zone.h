#pragma once
#include "afxwin.h"
#include "TLiteDoc.h"

// CDlg_KML_Configuration_Zone dialog
enum eKML_ZoneColorMode { e_KML_zone_green = 0, e_KML_zone_yellow, e_KML_zone_red ,
e_KML_zone_total_vehicles_origin,
e_KML_zone_total_vehicles_destination,

e_KML_zone_total_distance_origin,
e_KML_zone_total_distance_time_destination,

e_KML_zone_total_travel_time_origin,
e_KML_zone_total_travel_time_destination,

e_KML_zone_avg_distance_origin,
e_KML_zone_avg_distance_destination,

e_KML_zone_avg_travel_time_origin,
e_KML_zone_avg_travel_time_destination,

e_KML_zone_avg_speed_origin,
e_KML_zone_avg_speed_destination,




};
class CDlg_KML_Configuration_Zone : public CDialog
{
	DECLARE_DYNAMIC(CDlg_KML_Configuration_Zone)

public:
	CDlg_KML_Configuration_Zone(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_KML_Configuration_Zone();

	void SelchangeComboHeight();
// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIG_KML_ZONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk3();
	DECLARE_MESSAGE_MAP()
public:

	int m_Zone_Transparency;
	int m_Zone_Height_Mode;
	int m_Zone_Color_Mode;

	CTLiteDoc* m_pDoc;

	void UpdateCategoryValues();

	float m_Zone_Height_Ratio;
	int m_Transparency;
	afx_msg void OnBnClickedButtonSpaceevenly();
	float m_ColorCategoryValue[10];
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeList1();
	virtual BOOL OnInitDialog();
	CListBox m_List;
	CStatic m_ColorRampStatic;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonReverse();
	CComboBox m_Zone_Height_Combo;

	float m_MaxValue;
	afx_msg void OnCbnSelchangeComboZoneHeight();
};

