// Dlg_NEXTA_Configuration.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_NEXTA_Configuration.h"
#include "afxdialogex.h"


// CDlg_NEXTA_Configuration dialog

IMPLEMENT_DYNAMIC(CDlg_NEXTA_Configuration, CDialog)

CDlg_NEXTA_Configuration::CDlg_NEXTA_Configuration(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_NEXTA_Configuration::IDD, pParent)
{
	m_bRightHandTrafficFlag = 1;
}

CDlg_NEXTA_Configuration::~CDlg_NEXTA_Configuration()
{
}

void CDlg_NEXTA_Configuration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Side_Driving, m_Side_Driving_Combo);
}


BEGIN_MESSAGE_MAP(CDlg_NEXTA_Configuration, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_Side_Driving, &CDlg_NEXTA_Configuration::OnCbnSelchangeComboSideDriving)
	ON_BN_CLICKED(IDOK, &CDlg_NEXTA_Configuration::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_NEXTA_Configuration message handlers


void CDlg_NEXTA_Configuration::OnCbnSelchangeComboSideDriving()
{
	// TODO: Add your control notification handler code here
}


BOOL CDlg_NEXTA_Configuration::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Side_Driving_Combo.AddString("Left hand traffic");
	m_Side_Driving_Combo.AddString("Right hand traffic");

	m_Side_Driving_Combo.SetCurSel(m_bRightHandTrafficFlag);


	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlg_NEXTA_Configuration::OnBnClickedOk()
{
	m_bRightHandTrafficFlag = m_Side_Driving_Combo.GetCurSel();
	CDialog::OnOK();
}
