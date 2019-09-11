// Dlg_NodeProperties.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_NodeProperties.h"


// CDlg_NodeProperties dialog

IMPLEMENT_DYNAMIC(CDlg_NodeProperties, CDialog)

CDlg_NodeProperties::CDlg_NodeProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_NodeProperties::IDD, pParent)
	, NodeID(0)
	, NodeName(_T(""))
	, ZoneID(0)
	, m_CycleLength(0)
{

}

CDlg_NodeProperties::~CDlg_NodeProperties()
{
}

void CDlg_NodeProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NODE_ID, NodeID);
	DDX_Text(pDX, IDC_EDIT_STREET_NAME, NodeName);
	DDV_MaxChars(pDX, NodeName, 100);
	DDX_Text(pDX, IDC_EDIT_SPEEDLIMIT, ZoneID);
	DDV_MinMaxLong(pDX, ZoneID, 0, 1000000);
	DDX_Control(pDX, IDC_COMBO1, m_ControlTypeComboBox);
	DDX_Text(pDX, IDC_EDIT_CYCLE_LENGTH, m_CycleLength);
	DDV_MinMaxInt(pDX, m_CycleLength, 0, 500);
}


BEGIN_MESSAGE_MAP(CDlg_NodeProperties, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_NodeProperties::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDlg_NodeProperties::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_NodeProperties::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDlg_NodeProperties message handlers

void CDlg_NodeProperties::OnBnClickedOk()
{
	UpdateData();

	ControlType = m_ControlTypeVector[m_ControlTypeComboBox.GetCurSel ()];

	if(ControlType== m_pDoc->m_ControlType_PretimedSignal || ControlType== m_pDoc-> m_ControlType_ActuatedSignal)
	{

	}


	OnOK();
}

BOOL CDlg_NodeProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::map<int, DTALinkType>:: const_iterator itr;

	CString str;
	str.Format("Unknown Control (%d)",m_pDoc->m_ControlType_UnknownControl);
	m_ControlTypeComboBox.AddString (str);
	m_ControlTypeVector.push_back(m_pDoc->m_ControlType_UnknownControl);

	str.Format("No Control (%d)",m_pDoc->m_ControlType_NoControl);
	m_ControlTypeComboBox.AddString (str);
	m_ControlTypeVector.push_back(m_pDoc->m_ControlType_NoControl);

	str.Format("Yield Sign (%d)",m_pDoc->m_ControlType_YieldSign);
	m_ControlTypeComboBox.AddString (str);
	m_ControlTypeVector.push_back(m_pDoc->m_ControlType_NoControl);

	str.Format("2 Way Stop Sign (%d)",m_pDoc->m_ControlType_2wayStopSign);
	m_ControlTypeComboBox.AddString (str);
	m_ControlTypeVector.push_back(m_pDoc->m_ControlType_2wayStopSign);

	str.Format("4 Way Stop Sign (%d)",m_pDoc->m_ControlType_4wayStopSign);
	m_ControlTypeComboBox.AddString (str);
	m_ControlTypeVector.push_back(m_pDoc->m_ControlType_4wayStopSign);

	str.Format("Pretimed Signal (%d)",m_pDoc->m_ControlType_PretimedSignal);
	m_ControlTypeComboBox.AddString (str);
	m_ControlTypeVector.push_back(m_pDoc->m_ControlType_PretimedSignal);

	str.Format("Actuated Signal  (%d)",m_pDoc->m_ControlType_ActuatedSignal);
	m_ControlTypeComboBox.AddString (str);
	m_ControlTypeVector.push_back(m_pDoc->m_ControlType_ActuatedSignal);
	
	str.Format("Roundabout  (%d)",m_pDoc->m_ControlType_Roundabout);
	m_ControlTypeComboBox.AddString (str);
	m_ControlTypeVector.push_back(m_pDoc->m_ControlType_Roundabout);
	

	for(unsigned int i=0; i< m_ControlTypeVector.size(); i++)
	{
		if(ControlType == m_ControlTypeVector[i])
			m_ControlTypeComboBox.SetCurSel (i);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_NodeProperties::OnBnClickedCancel()
{
	OnCancel();
}




void CDlg_NodeProperties::OnCbnSelchangeCombo1()
{
	ControlType = m_ControlTypeVector[m_ControlTypeComboBox.GetCurSel()];

	if (ControlType != m_pDoc->m_ControlType_PretimedSignal && ControlType != m_pDoc->m_ControlType_ActuatedSignal)
	{
		m_CycleLength = 0;

	}

}
