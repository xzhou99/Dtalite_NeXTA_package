// Dlg_LinkVisualizationConfig.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_LinkVisualizationConfig.h"


// CDlg_LinkVisualizationConfig dialog

IMPLEMENT_DYNAMIC(CDlg_LinkVisualizationConfig, CDialog)

CDlg_LinkVisualizationConfig::CDlg_LinkVisualizationConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_LinkVisualizationConfig::IDD, pParent)
{

}

CDlg_LinkVisualizationConfig::~CDlg_LinkVisualizationConfig()
{
}

void CDlg_LinkVisualizationConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINK_BAR_WIDTH_LIST, m_LinkBarWidthModeList);
}


BEGIN_MESSAGE_MAP(CDlg_LinkVisualizationConfig, CDialog)
	ON_LBN_SELCHANGE(IDC_LINK_BAR_WIDTH_LIST, &CDlg_LinkVisualizationConfig::OnLbnSelchangeLinkBarWidthList)
	ON_BN_CLICKED(IDC_Back_GroundColorBUTTON, &CDlg_LinkVisualizationConfig::OnBnClickedBackGroundcolorbutton)
END_MESSAGE_MAP()


// CDlg_LinkVisualizationConfig message handlers

void CDlg_LinkVisualizationConfig::OnLbnSelchangeLinkBarWidthList()
{
	m_link_band_width_mode = (LINK_BAND_WIDTH_MODE)m_LinkBarWidthModeList.GetCurSel();
	m_pDoc->m_LinkBandWidthMode = m_link_band_width_mode;
	m_pDoc->GenerateOffsetLinkBand();
	m_pDoc->UpdateAllViews(0);
}

BOOL CDlg_LinkVisualizationConfig::OnInitDialog()
{

	CDialog::OnInitDialog();

	m_LinkBarWidthModeList.AddString("Number of lanes");
	m_LinkBarWidthModeList.AddString("Total link volume");
	m_LinkBarWidthModeList.AddString("Total impacted volume");

	m_LinkBarWidthModeList.SetCurSel ((int)m_link_band_width_mode);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_LinkVisualizationConfig::OnBnClickedBackGroundcolorbutton()
{
	CColorDialog dlg(RGB(0, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_BackgroundColor= dlg.GetColor();
		m_pDoc->UpdateAllViews(0);
	}
}
