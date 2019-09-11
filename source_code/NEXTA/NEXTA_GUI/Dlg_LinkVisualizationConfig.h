#pragma once
#include "afxwin.h"
#include "stdafx.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"


// CDlg_LinkVisualizationConfig dialog

class CDlg_LinkVisualizationConfig : public CDialog
{
	DECLARE_DYNAMIC(CDlg_LinkVisualizationConfig)

public:
	CTLiteDoc* m_pDoc;

	LINK_BAND_WIDTH_MODE m_link_band_width_mode;
	CDlg_LinkVisualizationConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_LinkVisualizationConfig();

// Dialog Data
	enum { IDD = IDD_VISUALIZATION_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeLinkBarWidthList();
	CListBox m_LinkBarWidthModeList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBackGroundcolorbutton();
};
