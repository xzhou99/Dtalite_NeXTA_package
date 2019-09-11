// Dlg_DataImportWizard.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_DataImportWizard.h"


// CDlg_DataImportWizard dialog

IMPLEMENT_DYNAMIC(CDlg_DataImportWizard, CDialog)

CDlg_DataImportWizard::CDlg_DataImportWizard(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_DataImportWizard::IDD, pParent)
	, m_bNode(FALSE)
	, m_bLink(FALSE)
	, m_bZone(FALSE)
	, m_bCentroid(FALSE)
	, m_bConnector(FALSE)
	, m_bTMCSpeed(FALSE)
	, m_bSensorCount(FALSE)
	, m_bLinkMOE(FALSE)
	, m_bTrajectory(FALSE)
	, m_bGPS(FALSE)
	, m_bGoogleTransitFeed(FALSE)
{

}

CDlg_DataImportWizard::~CDlg_DataImportWizard()
{
}

void CDlg_DataImportWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECKNODE, m_bNode);
	DDX_Check(pDX, IDC_CHECKLINK, m_bLink);
	DDX_Check(pDX, IDC_CHECKZONE, m_bZone);
	DDX_Check(pDX, IDC_CHECKCentroid, m_bCentroid);
	DDX_Check(pDX, IDC_CHECKCONNECTOR, m_bConnector);
	DDX_Check(pDX, IDC_CHECKTMC, m_bTMCSpeed);
	DDX_Check(pDX, IDC_CHECKSENSORCOUNT, m_bSensorCount);
	DDX_Check(pDX, IDC_CHECKLINKMOE, m_bLinkMOE);
	DDX_Check(pDX, IDC_CHECK_TRAJECTORY, m_bTrajectory);
	DDX_Check(pDX, IDC_CHECKGPSDATA, m_bGPS);
	DDX_Check(pDX, IDC_CHECKZONE2, m_bGoogleTransitFeed);
	DDX_Control(pDX, IDC_LIST1, m_DemandDataList);
	DDX_Control(pDX, IDC_EDIT_Message, m_Message);
}


BEGIN_MESSAGE_MAP(CDlg_DataImportWizard, CDialog)
	ON_BN_CLICKED(IDC_CHECKNODE, &CDlg_DataImportWizard::OnBnClickedChecknode)
	ON_BN_CLICKED(IDC_CHECKLINK, &CDlg_DataImportWizard::OnBnClickedChecklink)
	ON_BN_CLICKED(IDC_CHECKZONE, &CDlg_DataImportWizard::OnBnClickedCheckzone)
	ON_BN_CLICKED(IDC_CHECKCentroid, &CDlg_DataImportWizard::OnBnClickedCheckcentroid)
	ON_BN_CLICKED(IDC_CHECKCONNECTOR, &CDlg_DataImportWizard::OnBnClickedCheckconnector)
	ON_BN_CLICKED(IDC_CHECKTMC, &CDlg_DataImportWizard::OnBnClickedChecktmc)
	ON_BN_CLICKED(IDC_CHECKSENSORCOUNT, &CDlg_DataImportWizard::OnBnClickedChecksensorcount)
	ON_BN_CLICKED(IDC_CHECKLINKMOE, &CDlg_DataImportWizard::OnBnClickedChecklinkmoe)
	ON_BN_CLICKED(IDC_CHECK_TRAJECTORY, &CDlg_DataImportWizard::OnBnClickedCheckTrajectory)
	ON_BN_CLICKED(IDC_CHECKGPSDATA, &CDlg_DataImportWizard::OnBnClickedCheckgpsdata)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_DataImportWizard::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECKZONE2, &CDlg_DataImportWizard::OnBnClickedCheckzone2)
	ON_BN_CLICKED(IDC_BUTTONOPEN_FOLDER, &CDlg_DataImportWizard::OnBnClickedButtonopenFolder)
	ON_BN_CLICKED(IDC_BUTTONOPEN_FOLDER2, &CDlg_DataImportWizard::OnBnClickedButtonopenFolder2)
	ON_BN_CLICKED(IDOK, &CDlg_DataImportWizard::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_DataImportWizard message handlers

BOOL CDlg_DataImportWizard::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_DemandDataList.AddString ("No Demand Required");
	m_DemandDataList.AddString ("No Existing Demand Files -> Start with Gravity Model");
	m_DemandDataList.AddString ("With Existing Demand Files -> Define Demand Meta Database");
	m_DemandDataList.AddString ("With Existing Demand Files and Sensor Data -> Perform Demand Calibration");

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_DataImportWizard::OnBnClickedChecknode()
{
	//m_Message = "Pleaes supply the node layer in shape file format. ";
	//UpdateData(0);
}

void CDlg_DataImportWizard::OnBnClickedChecklink()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedCheckzone()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedCheckcentroid()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedCheckconnector()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedChecktmc()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedChecksensorcount()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedChecklinkmoe()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedCheckTrajectory()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedCheckgpsdata()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedCheckzone2()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedButtonopenFolder()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedButtonopenFolder2()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DataImportWizard::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
