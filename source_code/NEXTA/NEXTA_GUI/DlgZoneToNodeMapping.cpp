// DlgZoneToNodeMapping.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgZoneToNodeMapping.h"




// CDlgZoneToNodeMapping dialog

IMPLEMENT_DYNAMIC(CDlgZoneToNodeMapping, CDialog)

CDlgZoneToNodeMapping::CDlgZoneToNodeMapping(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZoneToNodeMapping::IDD, pParent)
{

}

CDlgZoneToNodeMapping::~CDlgZoneToNodeMapping()
{
}

void CDlgZoneToNodeMapping::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Mapping, m_ListMapping);
}


BEGIN_MESSAGE_MAP(CDlgZoneToNodeMapping, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgZoneToNodeMapping::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgZoneToNodeMapping message handlers

BOOL CDlgZoneToNodeMapping::OnInitDialog()
{
	CDialog::OnInitDialog();


	CString txt;
	txt.Format(_T("%10s\t%10s"),"TAZ","Node ID");
	m_ListMapping.AddString(txt);

	for (unsigned int i=0;i<m_pDlg->ZoneRecordSet.size();i++)
	{
		txt.Format(_T("%10d\t%10d"),m_pDlg->ZoneRecordSet[i].zone_num,m_pDlg->ZoneRecordSet[i].node_name);
		m_ListMapping.AddString(txt);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgZoneToNodeMapping::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
