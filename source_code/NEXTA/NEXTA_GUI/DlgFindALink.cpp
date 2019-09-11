// DlgFindALink.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgFindALink.h"

extern int read_2_integers_from_a_string(CString str, int &value1, int &value2);
// CDlgFindALink dialog

IMPLEMENT_DYNAMIC(CDlgFindALink, CDialog)

CDlgFindALink::CDlgFindALink(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgFindALink::IDD, pParent)
	, m_StrFind(_T(""))
	, m_UserInputString(_T(""))
{

}

CDlgFindALink::~CDlgFindALink()
{
}

void CDlgFindALink::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_FindObjectList);
	DDX_Text(pDX, IDC_EDIT1, m_UserInputString);
	DDX_Control(pDX, IDC_EDIT1, m_EditorControl);
}


BEGIN_MESSAGE_MAP(CDlgFindALink, CBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CDlgFindALink::OnBnClickedButtonFind)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgFindALink::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CDlgFindALink message handlers



BOOL CDlgFindALink::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	m_FindObjectList.AddString("Node ID");
	m_FindObjectList.AddString("Link: FromNodeID ToNodeID");
	m_FindObjectList.AddString("Path: FromNodeID ToNodeID");
	m_FindObjectList.AddString("Vehicle: VehicleID");
	m_FindObjectList.AddString("Sensor ID");
	m_FindObjectList.AddString("Path: Path node sequence");

	m_FindObjectList.SetCurSel(m_SearchMode);


	UpdateData(false);


	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFindALink::OnBnClickedFindVehicle()
{
	UpdateData(true);
	m_SearchMode = efind_vehicle;
	OnOK();
	
}
BOOL CDlgFindALink::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN &&
        pMsg->wParam == VK_RETURN &&
        GetFocus() == &(m_EditorControl))
    {
        // handle return pressed in edit control
		OnBnClickedButtonFind();
        return TRUE; // this doesn't need processing anymore
    }
    return FALSE; // all other cases still need default processing
}
void CDlgFindALink::OnBnClickedButtonFind()
{
	m_SearchMode = (eSEARCHMODE)m_FindObjectList.GetCurSel();
	CString sItem;
	
	UpdateData(true);

	
	m_StrFind = m_UserInputString;
	
	bool bFound = false;
	for(unsigned int i = 0; i <m_SearchHistoryVector.size() ; i++)
	{

		if(m_SearchHistoryVector[i].Compare (m_StrFind)>0)
		{
			bFound = true;
			break;
		}
	

	}
	
	if(bFound == false)
	m_SearchHistoryVector.push_back(m_StrFind);

	int value1 = 0; 
	int value2 = 0;



	switch(m_SearchMode)
	{
	case efind_node:
		read_2_integers_from_a_string(m_StrFind, value1, value2);
		m_NodeNumber = value1; break;
	case efind_link: 
		read_2_integers_from_a_string(m_StrFind, value1, value2);
		m_FromNodeNumber = value1; m_ToNodeNumber = value2; break;
	case efind_path: 
		read_2_integers_from_a_string(m_StrFind, value1, value2);
		m_FromNodeNumber = value1; m_ToNodeNumber = value2; break;
	case efind_vehicle: 
		read_2_integers_from_a_string(m_StrFind, value1, value2);
		m_VehicleNumber = value1; break;
	case efind_path_node_sequence: m_PathNodeSequence = m_StrFind; break;
	
	default: 
	m_NodeNumber = value1;
	}

	OnOK();

}

void CDlgFindALink::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}

