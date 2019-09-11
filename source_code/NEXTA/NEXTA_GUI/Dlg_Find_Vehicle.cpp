// Dlg_Find_Vehicle.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_Find_Vehicle.h"


// CDlg_Find_Vehicle dialog

IMPLEMENT_DYNAMIC(CDlg_Find_Vehicle, CDialog)

CDlg_Find_Vehicle::CDlg_Find_Vehicle(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Find_Vehicle::IDD, pParent)
	, m_VehicleID(0)
{

}

CDlg_Find_Vehicle::~CDlg_Find_Vehicle()
{
}

void CDlg_Find_Vehicle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Vehicle_ID, m_VehicleID);
}


BEGIN_MESSAGE_MAP(CDlg_Find_Vehicle, CDialog)
END_MESSAGE_MAP()


// CDlg_Find_Vehicle message handlers
