// DlgCarFollowingModelSettings.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgCarFollowingModelSettings.h"


// CDlgCarFollowingModelSettings dialog

IMPLEMENT_DYNAMIC(CDlgCarFollowingModelSettings, CDialog)

CDlgCarFollowingModelSettings::CDlgCarFollowingModelSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCarFollowingModelSettings::IDD, pParent)
	, m_Kjam(0)
	, m_WaveSpeed(0)
	, m_FreeflowSpeed(0)
{

}

CDlgCarFollowingModelSettings::~CDlgCarFollowingModelSettings()
{
}

void CDlgCarFollowingModelSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Kjam, m_Kjam);
	DDV_MinMaxFloat(pDX, m_Kjam, 0, 400);
	DDX_Text(pDX, IDC_EDIT_WaveSpeed, m_WaveSpeed);
	DDV_MinMaxFloat(pDX, m_WaveSpeed, 0, 100);
	DDX_Text(pDX, IDC_EDIT_WaveSpeed2, m_FreeflowSpeed);
	DDV_MinMaxFloat(pDX, m_FreeflowSpeed, 0, 100);
}


BEGIN_MESSAGE_MAP(CDlgCarFollowingModelSettings, CDialog)
END_MESSAGE_MAP()


// CDlgCarFollowingModelSettings message handlers

void CDlgCarFollowingModelSettings::OnBnClickedOk()
{
	OnOK();
}
