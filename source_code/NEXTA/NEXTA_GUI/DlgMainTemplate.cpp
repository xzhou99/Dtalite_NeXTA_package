// DlgMainTemplate.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgMainTemplate.h"


// CDlgMainTemplate dialog

IMPLEMENT_DYNAMIC(CDlgMainTemplate, CDialog)

CDlgMainTemplate::CDlgMainTemplate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMainTemplate::IDD, pParent)
{

}

CDlgMainTemplate::~CDlgMainTemplate()
{
}

void CDlgMainTemplate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_TemplateList);
}


BEGIN_MESSAGE_MAP(CDlgMainTemplate, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgMainTemplate::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST1, &CDlgMainTemplate::OnLbnDblclkList1)
END_MESSAGE_MAP()


// CDlgMainTemplate message handlers

BOOL CDlgMainTemplate::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TemplateList.AddString("Traffic Assignment");
	m_TemplateList.AddString("Train Scheduling");
	m_TemplateList.SetCurSel (0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMainTemplate::OnBnClickedOk()
{
	m_TemplateNo = m_TemplateList.GetCurSel ();
	OnOK();
}

void CDlgMainTemplate::OnLbnDblclkList1()
{
	m_TemplateNo = m_TemplateList.GetCurSel ();
	OnOK();
}
