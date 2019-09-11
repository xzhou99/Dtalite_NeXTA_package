// DlgDefaultLinkProperties.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgDefaultLinkProperties.h"


// CDlgDefaultLinkProperties dialog

IMPLEMENT_DYNAMIC(CDlgDefaultLinkProperties, CDialog)

CDlgDefaultLinkProperties::CDlgDefaultLinkProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDefaultLinkProperties::IDD, pParent)
	, SpeedLimit(0)
	, LaneCapacity(0)
	, NumLanes(0)
{
	m_bLongLatSystem = 0;
}

CDlgDefaultLinkProperties::~CDlgDefaultLinkProperties()
{
}

void CDlgDefaultLinkProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPEEDLIMIT, SpeedLimit);
	DDV_MinMaxFloat(pDX, SpeedLimit, 0, 200);
	DDX_Text(pDX, IDC_EDIT_LANE_CAPACITY, LaneCapacity);
	DDX_Text(pDX, IDC_EDIT_NUMLANES, NumLanes);
	DDV_MinMaxInt(pDX, NumLanes, 1, 20);
	DDX_Control(pDX, IDC_LinkTypeLIST, m_LinkTypeList);
	DDX_Control(pDX, IDC_COMBO1, m_CoordinateSystemList);
	DDX_Text(pDX, IDC_EDIT_NewNodeNumber, m_StartNodeNumberForNewNodes);
}


BEGIN_MESSAGE_MAP(CDlgDefaultLinkProperties, CDialog)
	ON_LBN_SELCHANGE(IDC_LinkTypeLIST, &CDlgDefaultLinkProperties::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDOK, &CDlgDefaultLinkProperties::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CDlgDefaultLinkProperties::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgDefaultLinkProperties::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDlgDefaultLinkProperties message handlers

void CDlgDefaultLinkProperties::OnLbnSelchangeList1()
{
	int SelectLinkType = m_LinkTypeList.GetCurSel();

	int count = 0;
			for(std::map<int, DTALinkType>::iterator itr = m_pDoc->m_LinkTypeMap.begin(); itr != m_pDoc->m_LinkTypeMap.end(); itr++)
		{
			if(itr->second .link_type_name.length () > 0 && itr->second .type_code.length () > 0)
			{

				if(count == SelectLinkType)
				{
				LinkType = itr->first ;
				SpeedLimit =  itr->second .default_speed;  
				LaneCapacity = itr->second .default_lane_capacity ;  
				NumLanes = itr->second .default_number_of_lanes;  
				break;
				}
				 count++;

			}
		}
	UpdateData(false);
}

BOOL CDlgDefaultLinkProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;
		for(std::map<int, DTALinkType>::iterator itr = m_pDoc->m_LinkTypeMap.begin(); itr != m_pDoc->m_LinkTypeMap.end(); itr++)
		{
			if(itr->second .link_type_name.length () > 0 && itr->second .type_code.length () > 0)
			{

				str.Format("%d:%s,%s", itr->second .link_type  , 
					itr->second .link_type_name.c_str () , 
					itr->second .type_code .c_str ());

				m_LinkTypeList.AddString (str);
			}
		}
		

		m_LinkTypeList.SetCurSel (LinkType-1);
		
		m_CoordinateSystemList.AddString ("Mile");
		m_CoordinateSystemList.AddString ("Long/Lat");

		m_CoordinateSystemList.SetCurSel (0);


	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDefaultLinkProperties::OnBnClickedOk()
{

	LinkType = 1; // default
	int CurSel = m_LinkTypeList.GetCurSel ();
	int count = 0;
			for(std::map<int, DTALinkType>::iterator itr = m_pDoc->m_LinkTypeMap.begin(); itr != m_pDoc->m_LinkTypeMap.end(); itr++)
		{
			if(itr->second .link_type_name.length () > 0 && itr->second .type_code.length () > 0)
			{

				if(count == CurSel)
				{
				LinkType = itr->first ;
				break;
				}
				 count++;

			}
		}

OnOK();
}

void CDlgDefaultLinkProperties::OnBnClickedCancel()
{
	OnCancel();
}

void CDlgDefaultLinkProperties::OnCbnSelchangeCombo1()
{
	m_bLongLatSystem = m_CoordinateSystemList.GetCurSel ();
}
