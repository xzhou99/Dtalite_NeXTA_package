#pragma once
#include "afxwin.h"
#include "TLiteDoc.h"
enum eKML_ColorMode { e_KML_green = 0, e_KML_yellow, e_KML_red ,e_KML_speed_limit,e_KML_avg_speed,e_KML_avg_speed_to_speed_limit_raio, e_KML_user_defined_attribue};
// CDlg_KML_Configuration dialog

class CDlg_KML_Configuration : public CDialog
{
	DECLARE_DYNAMIC(CDlg_KML_Configuration)

public:
	CDlg_KML_Configuration(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_KML_Configuration();

	void SelchangeComboHeight();
// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIG_KML_LINK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:


	CTLiteDoc* m_pDoc;

	void UpdateCategoryValues();
	eKML_ColorMode m_KML_MOE_selection;
	float m_KML_Height_Ratio;
	int m_Transparency;
	afx_msg void OnBnClickedButtonSpaceevenly();
	float m_ColorCategoryValue[10];
	afx_msg void OnBnClickedOk();
	int m_BandWidth;
	afx_msg void OnLbnSelchangeList1();
	virtual BOOL OnInitDialog();
	CListBox m_List;
	CStatic m_ColorRampStatic;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonReverse();
	CComboBox m_Height_ComboBox;

	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedOk4();
	afx_msg void OnCbnSelchangeComboHeight();
	float m_MaxHeightValue;
	afx_msg void OnEnChangeEditHeightMax();
	afx_msg void OnEnChangeEditHeight();
};

