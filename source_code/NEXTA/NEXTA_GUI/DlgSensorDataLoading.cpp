// DlgSensorDataLoading.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgSensorDataLoading.h"


// CDlgSensorDataLoading dialog

IMPLEMENT_DYNAMIC(CDlgSensorDataLoading, CDialog)

CDlgSensorDataLoading::CDlgSensorDataLoading(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSensorDataLoading::IDD, pParent)
	, m_ObsTimeInterval(5)
	, m_NumberOfDays(5)
	, m_Occ_to_Density_Coef(100)
	, m_AVISamplingTimeInterval(1)
{

}

CDlgSensorDataLoading::~CDlgSensorDataLoading()
{
}

void CDlgSensorDataLoading::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TimeInterval, m_ObsTimeInterval);
	DDV_MinMaxInt(pDX, m_ObsTimeInterval, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_Number_of_Days, m_NumberOfDays);
	DDV_MinMaxInt(pDX, m_NumberOfDays, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_Occ_to_Density_Coef, m_Occ_to_Density_Coef);
	DDX_Text(pDX, IDC_EDIT_TimeInterval_AVI, m_AVISamplingTimeInterval);
	DDV_MinMaxInt(pDX, m_AVISamplingTimeInterval, 0, 1000);
}


BEGIN_MESSAGE_MAP(CDlgSensorDataLoading, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSensorDataLoading::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSensorDataLoading message handlers

void CDlgSensorDataLoading::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
