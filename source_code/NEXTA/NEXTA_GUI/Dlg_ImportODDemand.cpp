// Dlg_ImportODDemand.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_ImportODDemand.h"


// CDlg_ImportODDemand dialog

IMPLEMENT_DYNAMIC(CDlg_ImportODDemand, CDialog)

CDlg_ImportODDemand::CDlg_ImportODDemand(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_ImportODDemand::IDD, pParent)
	, m_NumberOfZones(0)
{

}

CDlg_ImportODDemand::~CDlg_ImportODDemand()
{
}

void CDlg_ImportODDemand::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Number_of_Zones, m_NumberOfZones);
}


BEGIN_MESSAGE_MAP(CDlg_ImportODDemand, CDialog)
END_MESSAGE_MAP()


// CDlg_ImportODDemand message handlers
