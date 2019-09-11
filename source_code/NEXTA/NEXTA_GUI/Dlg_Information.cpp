// Dlg_Information.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_Information.h"
#include "MainFrm.h"

// CDlg_Information dialog

IMPLEMENT_DYNAMIC(CDlg_Information, CDialog)

CDlg_Information::CDlg_Information(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Information::IDD, pParent)
	, m_StringInfo(_T(""))
{
	m_OnLineDocumentLink = "https://docs.google.com/document/d/14tUa1I6Xf62zsiWf4lLfngqGqGJlIM_MSehLFMVXass/edit";

	m_bLoadGISSampleFile = false;
}

CDlg_Information::~CDlg_Information()
{
}

void CDlg_Information::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StringInfo);
}


BEGIN_MESSAGE_MAP(CDlg_Information, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ONLINE_DOCUMENT, &CDlg_Information::OnBnClickedButtonOnlineDocument)
	ON_BN_CLICKED(IDC_BUTTON_SampleFile, &CDlg_Information::OnBnClickedButtonSamplefile)
END_MESSAGE_MAP()


// CDlg_Information message handlers

void CDlg_Information::OnBnClickedButtonOnlineDocument()
{
   HINSTANCE result = ShellExecute(NULL, _T("open"), m_OnLineDocumentLink, NULL,NULL, SW_SHOW);;
}

void CDlg_Information::OnBnClickedButtonSamplefile()
{
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	CString FileFolder;
	FileFolder.Format("%s\\%s", pMainFrame->m_CurrentDirectory,m_SampleFileDirectory);

	HINSTANCE result = ShellExecute(NULL, _T("open"), FileFolder, NULL,NULL, SW_SHOW);
	m_bLoadGISSampleFile = true;
}
