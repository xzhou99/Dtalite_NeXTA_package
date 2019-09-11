#pragma once


#include "Chart_Control\\NSChartCtrl.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "afxwin.h"
#include "BaseDialog.h"
// CDlg_VehicleClassification dialog

class CDlg_VehicleClassification : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlg_VehicleClassification)

public:

	CDlg_VehicleClassification(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_VehicleClassification();

// Dialog Data

	CTLiteDoc* m_pDoc;
	float	m_dValue;
	CString	m_sLabel;
	CString m_PresetChartTitle;
	CNSChartCtrl m_chart;
	bool m_bHighlightSelectedVehicles;

	VEHICLE_X_CLASSIFICATION m_XSelectionNo;
	VEHICLE_Y_CLASSIFICATION m_YSelectionNo;
	VEHICLE_CLASSIFICATION_SELECTION m_VehicleSelectionNo;
	enum { IDD = IDD_DIALOG_Summary };

	void AddChartData();

	void ExportData(CString fname);
	void ExportAllData(CString fname);
protected:
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	INT m_iItem;
	HICON m_hIcon;
	void OnUpdateModify(CCmdUI* pCmdUI);
	void OnUpdateAdd(CCmdUI* pCmdUI);
	// Generated message map functions
	//{{AFX_MSG(CDlg_TDDemandProfile)
	BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRadioButtons();
	afx_msg void OnRadioColors();
	afx_msg void OnAdd();
	afx_msg void OnChartSelectedItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDel();
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboXaxis();
	afx_msg void OnCbnSelchangeComboYaxis();
	CComboBox m_ComboX;
	CComboBox m_ComboY;
	afx_msg void OnBnClickedExportData();
	CComboBox m_ComboVehicleSelection;
	afx_msg void OnCbnSelchangeComboVehicleselection();
	afx_msg void OnClose();
	CString m_Message;
	afx_msg void OnBnClickedHighlightVehicles();
	CButton m_HighlightVehicleButton;
	CListBox m_MessageList;
	int m_MaxColumnSize;

	afx_msg void OnCbnSelchangeComboMaxColumns();
	CComboBox m_ColumnSizeList;
	afx_msg void OnBnClickedExportAllData();
	CStatic m_MessageEdit;
};
