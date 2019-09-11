// Dlg_BackgroundImageLocation.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Utility.h"
#include "Dlg_BackgroundImageLocation.h"


// CDlg_BackgroundImageLocation dialog

IMPLEMENT_DYNAMIC(CDlg_BackgroundImageLocation, CDialog)

CDlg_BackgroundImageLocation::CDlg_BackgroundImageLocation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_BackgroundImageLocation::IDD, pParent)
	, m_LatLongA(_T(""))
	, m_PointA(_T(""))
	, m_PointB(_T(""))
	, m_LatLongB(_T(""))
{

	m_PointA_x = m_PointA_y = m_PointB_x = m_PointB_y = 0;
	m_bPointA_Initialized = m_bPointB_Initialized = false;
	m_PointA_long = m_PointA_lat= m_PointB_long = m_PointB_lat = 0;

}

CDlg_BackgroundImageLocation::~CDlg_BackgroundImageLocation()
{
}

void CDlg_BackgroundImageLocation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LatLongA, m_LatLongA);
	DDX_Text(pDX, IDC_EDIT1, m_PointA);
	DDX_Text(pDX, IDC_EDIT2, m_PointB);
	DDX_Text(pDX, IDC_EDIT_LatLongA2, m_LatLongB);
}


BEGIN_MESSAGE_MAP(CDlg_BackgroundImageLocation, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_BackgroundImageLocation::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CDlg_BackgroundImageLocation::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_BackgroundImageLocation message handlers

void CDlg_BackgroundImageLocation::OnBnClickedButton1()
{
	g_OpenDocument("http://maps.google.com/", SW_SHOW);

}



BOOL CDlg_BackgroundImageLocation::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bPointA_Initialized)
	{
		m_PointA.Format ("y=%f; x=%f",m_PointA_y,m_PointA_x);
		m_LatLongA.Format ("%f %f",m_PointA_long,m_PointA_lat);

	}

	if(m_bPointB_Initialized)
	{
		m_PointB.Format ("y=%f; x=%f",m_PointB_y,m_PointB_x);
		m_LatLongB.Format ("%f %f",m_PointB_long,m_PointB_lat);
	}

	UpdateData(0);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_BackgroundImageLocation::OnBnClickedOk()
{
	UpdateData(1);
	read_2_float_points_from_a_string(m_LatLongA, m_PointA_lat,m_PointA_long);
	read_2_float_points_from_a_string(m_LatLongB, m_PointB_lat,m_PointB_long);
	
	OnOK();
}
