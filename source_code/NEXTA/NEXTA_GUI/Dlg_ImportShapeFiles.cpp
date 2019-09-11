// Dlg_ImportShapeFiles.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_ImportShapeFiles.h"
#include "Dlg_GISDataExchange.h"
#include "MainFrm.h"
#include "Shellapi.h"

// CDlg_ImportShapeFiles dialog

IMPLEMENT_DYNAMIC(CDlg_ImportShapeFiles, CDialog)

CDlg_ImportShapeFiles::CDlg_ImportShapeFiles(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_ImportShapeFiles::IDD, pParent)
	, m_NodeShapeFile(_T(""))
	, m_LinkShapeFile(_T(""))
	, m_ZoneShapeFile(_T(""))
{

}

CDlg_ImportShapeFiles::~CDlg_ImportShapeFiles()
{
}

void CDlg_ImportShapeFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NodeShapeFile, m_NodeShapeFile);
	DDX_Text(pDX, IDC_EDIT_LinkShapeFile, m_LinkShapeFile);
	DDX_Text(pDX, IDC_EDIT_ZoneShapeFile, m_ZoneShapeFile);
	DDX_Control(pDX,IDC_GRIDLISTCTRLEX,m_ListCtrl);

}


BEGIN_MESSAGE_MAP(CDlg_ImportShapeFiles, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Find_Node_File, &CDlg_ImportShapeFiles::OnBnClickedButtonFindNodeFile)
	ON_BN_CLICKED(IDC_BUTTON_Find_Link_File, &CDlg_ImportShapeFiles::OnBnClickedButtonFindLinkFile)
	ON_BN_CLICKED(IDC_BUTTON_Find_Zone_File, &CDlg_ImportShapeFiles::OnBnClickedButtonFindZoneFile)
	ON_BN_CLICKED(IDOK, &CDlg_ImportShapeFiles::OnBnClickedOk)
	ON_BN_CLICKED(IDIMPORT_LINK, &CDlg_ImportShapeFiles::OnBnClickedLink)
END_MESSAGE_MAP()


// CDlg_ImportShapeFiles message handlers

void CDlg_ImportShapeFiles::OnBnClickedButtonFindNodeFile()
{
	static char BASED_CODE szFilter[] = "Node Shape CSV File (*.csv)|*.csv||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{
		m_NodeShapeFile = dlg.GetPathName();
		UpdateData(false);
	}
}

void CDlg_ImportShapeFiles::OnBnClickedButtonFindLinkFile()
{
	static char BASED_CODE szFilter[] = "Link Shape CSV File (*.csv)|*.csv||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{
		m_LinkShapeFile = dlg.GetPathName();
		UpdateData(false);
	}
}

void CDlg_ImportShapeFiles::OnBnClickedButtonFindZoneFile()
{
	static char BASED_CODE szFilter[] = "Zone Shape CSV File (*.csv)|*.csv||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{
		m_ZoneShapeFile = dlg.GetPathName();
		UpdateData(false);
	}
}

void CDlg_ImportShapeFiles::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlg_ImportShapeFiles::OnBnClickedLink()
{

}
