// Dlg_LeftTurnLaneSetup.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_LeftTurnLaneSetup.h"


// CDlg_LeftTurnLaneSetup dialog

IMPLEMENT_DYNAMIC(CDlg_LeftTurnLaneSetup, CDialog)

CDlg_LeftTurnLaneSetup::CDlg_LeftTurnLaneSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_LeftTurnLaneSetup::IDD, pParent)
	, m_LeftturnVolume1(150)
	, m_LeftturnVolume2(300)
	, m_LeftturnVolume3(600)
{

}

CDlg_LeftTurnLaneSetup::~CDlg_LeftTurnLaneSetup()
{
}

void CDlg_LeftTurnLaneSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_1, m_LeftturnVolume1);
	DDX_Text(pDX, IDC_EDIT2, m_LeftturnVolume2);
	DDX_Text(pDX, IDC_EDIT4, m_LeftturnVolume3);
}


BEGIN_MESSAGE_MAP(CDlg_LeftTurnLaneSetup, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_LeftTurnLaneSetup::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_LeftTurnLaneSetup message handlers

void CDlg_LeftTurnLaneSetup::OnBnClickedOk()
{
	OnOK();
}

BOOL CDlg_LeftTurnLaneSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
