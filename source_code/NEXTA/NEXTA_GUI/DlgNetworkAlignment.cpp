// DlgNetworkAlignment.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgNetworkAlignment.h"


// CDlgNetworkAlignment dialog

IMPLEMENT_DYNAMIC(CDlgNetworkAlignment, CDialog)

CDlgNetworkAlignment::CDlgNetworkAlignment(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNetworkAlignment::IDD, pParent)
	, m_Node1(0)
	, m_Node2(0)
	, m_NodeY1(0)
	, m_NodeY2(0)
	, m_NodeX1(0)
	, m_NodeX2(0)
{

}

CDlgNetworkAlignment::~CDlgNetworkAlignment()
{
}

void CDlgNetworkAlignment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NODEA, m_Node1);
	DDX_Text(pDX, IDC_EDIT_NODEB, m_Node2);
	DDX_Text(pDX, IDC_EDIT_A_Lat, m_NodeY1);
	DDX_Text(pDX, IDC_EDIT_B_Lat, m_NodeY2);
	DDX_Text(pDX, IDC_EDIT_A_Long, m_NodeX1);
	DDX_Text(pDX, IDC_EDIT_B_Long, m_NodeX2);
}


BEGIN_MESSAGE_MAP(CDlgNetworkAlignment, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgNetworkAlignment::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNetworkAlignment message handlers

void CDlgNetworkAlignment::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
