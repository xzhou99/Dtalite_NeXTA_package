// Dlg_ImageSettings.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"

#include "Dlg_ImageSettings.h"


// CDlg_ImageSettings dialog

IMPLEMENT_DYNAMIC(CDlg_ImageSettings, CDialog)

CDlg_ImageSettings::CDlg_ImageSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_ImageSettings::IDD, pParent)
	, m_Image_File_Message(_T(""))
	, m_X1(0)
	, m_Y1(0)
	, m_X2(0)
	, m_Y2(0)
{

}

CDlg_ImageSettings::~CDlg_ImageSettings()
{
}

void CDlg_ImageSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Left, m_X1);
	DDX_Text(pDX, IDC_EDIT_Top, m_Y2);
	DDX_Text(pDX, IDC_EDIT_Right, m_X2);
	DDX_Text(pDX, IDC_EDIT_Bottom, m_Y1);
	DDX_Text(pDX, IDC_Image_File_Messge, m_Image_File_Message);
}


BEGIN_MESSAGE_MAP(CDlg_ImageSettings, CDialog)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &CDlg_ImageSettings::OnEnChangeEditWidth)
	ON_BN_CLICKED(IDOK, &CDlg_ImageSettings::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_Left, &CDlg_ImageSettings::OnEnChangeEditLeft)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CDlg_ImageSettings::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_Reset, &CDlg_ImageSettings::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CDlg_ImageSettings message handlers

void CDlg_ImageSettings::OnEnChangeEditWidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CDlg_ImageSettings::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlg_ImageSettings::OnEnChangeEditLeft()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

BOOL CDlg_ImageSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_ImageSettings::OnBnClickedButtonExit()
{
	OnCancel();
}

void CDlg_ImageSettings::OnBnClickedButtonReset()
{
	m_X1  = m_pDoc->m_NetworkRect.left;
	m_Y1  = m_pDoc->m_NetworkRect.bottom;
	m_X2 = m_pDoc->m_NetworkRect.right;
	m_Y2 = m_pDoc->m_NetworkRect.top;

	UpdateData(0);
	
}
