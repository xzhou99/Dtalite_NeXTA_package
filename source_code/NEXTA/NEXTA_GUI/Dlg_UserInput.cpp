// Dlg_UserInput.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_UserInput.h"


// CDlg_UserInput dialog

IMPLEMENT_DYNAMIC(CDlg_UserInput, CDialog)

CDlg_UserInput::CDlg_UserInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_UserInput::IDD, pParent)
	, m_StrQuestion(_T(""))
	, m_InputValue(_T(""))
{

}

CDlg_UserInput::~CDlg_UserInput()
{
}

void CDlg_UserInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_QUESTION, m_StrQuestion);
	DDX_Text(pDX, IDC_EDIT1, m_InputValue);
}


BEGIN_MESSAGE_MAP(CDlg_UserInput, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_UserInput::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_UserInput message handlers

void CDlg_UserInput::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
