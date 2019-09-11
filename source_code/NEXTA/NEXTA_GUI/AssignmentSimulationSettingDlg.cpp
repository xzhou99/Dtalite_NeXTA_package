//AssignmentSimulationSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "AssignmentSimulationSettingDlg.h"
#include "CSVParser.h"
#include <string>
using std::vector;
using std::string;

enum Assignment_Simulation_Settings { SIMULATION = 0, DEMAND_META, DEMAND_TYPE, LINK_TYPE, NODE_CONTROL_TYPE, VEHICLE_TYPE, VEHICLE_EMISSIONS, VOT_DISTRIBUTION, MOE, MAX_NUM_OF_SETTINGS };
static LPTSTR Setting_Element[MAX_NUM_OF_SETTINGS] = { "Simulation", "Demand Meta Database", "Demand Type", "Link Type", "Node Control Type", "Vehicle Type", "Vehicle Emission Rate", "Value of Time Distribution", "MOE Output", };
static LPTSTR Setting_FileName[MAX_NUM_OF_SETTINGS] = {"input_scenario_settings","input_demand_meta_data", "input_demand_type",
														 "input_link_type", "input_node_control_type",
														 "input_vehicle_type",
														"optional_vehicle_emission_rate", "input_VOT", "input_MOE_settings",
														};

// CAssignmentSimulationSettingDlg dialog

IMPLEMENT_DYNAMIC(CAssignmentSimulationSettingDlg, CDialog)

CAssignmentSimulationSettingDlg::CAssignmentSimulationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssignmentSimulationSettingDlg::IDD, pParent)
{

}

CAssignmentSimulationSettingDlg::~CAssignmentSimulationSettingDlg()
{
}

BOOL CAssignmentSimulationSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	p_SubTabs = new CAssignmentSimulationTabDlg*[MAX_NUM_OF_SETTINGS];

	vector<string> name_vector;
	vector<vector<string>> value_vector;
	vector<string> DefaultHeader;
	vector<CString> DefaultValue;
	vector<std::string> LinkString;

	for (int i=0; i < MAX_NUM_OF_SETTINGS; i++)
	{
		name_vector.clear();
		value_vector.clear();
		DefaultHeader.clear();
		DefaultValue.clear();
		LinkString.clear();

		bool ReturnFlag = ReadScenarioCSVFile(Setting_FileName[i],name_vector,value_vector);

		if(ReturnFlag == true && name_vector.size() > 0)
		{
		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T(Setting_Element[i]);
		m_TabCtrl.InsertItem(i, &tcItem);

		p_SubTabs[i] = new CAssignmentSimulationTabDlg(name_vector, DefaultValue, value_vector);
		p_SubTabs[i]->SetTabText(Setting_Element[i]);
		p_SubTabs[i]->m_FileName = Setting_FileName[i];
		p_SubTabs[i]->Create(IDD_DIALOG_SCENARIO_TAB,&m_TabCtrl);
		}
	}

	//m_SelectTab = 0;
	//p_SubTabs[m_SelectTab]->ShowWindow(SW_SHOW);

	//for (int i=1;i<MAX_NUM_OF_SETTINGS;i++)
	//{
	//	p_SubTabs[i]->ShowWindow(SW_HIDE);
	//}

	//m_PrevTab = m_SelectTab;

	//m_TabCtrl.SetCurSel(m_SelectTab);

	//SetRectangle();

	return TRUE;
}


void CAssignmentSimulationSettingDlg::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	m_TabCtrl.GetClientRect(&tabRect);
	m_TabCtrl.GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	for(int nCount=0; nCount < MAX_NUM_OF_SETTINGS; nCount++)
	{
		if (m_SelectTab != nCount)
		{
			p_SubTabs[nCount]->SetWindowPos(&m_TabCtrl.wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
		}
		else
		{
			p_SubTabs[nCount]->SetWindowPos(&m_TabCtrl.wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
		}
	}
}

void CAssignmentSimulationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_TAB, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CAssignmentSimulationSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAssignmentSimulationSettingDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SETTING_TAB, &CAssignmentSimulationSettingDlg::OnTcnSelchangeSettingTab)
	ON_BN_CLICKED(IDCANCEL, &CAssignmentSimulationSettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CAssignmentSimulationSettingDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CAssignmentSimulationSettingDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DATA_IN_EXCEL, &CAssignmentSimulationSettingDlg::OnBnClickedButtonEditDataInExcel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CAssignmentSimulationSettingDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CAssignmentSimulationSettingDlg message handlers

void CAssignmentSimulationSettingDlg::OnBnClickedOk()
{
	for (int i=0;i<MAX_NUM_OF_SETTINGS;i++)
	{
		//if (p_SubTabs[i]->ValidityCheck() != 0)
		//{
		//	if (m_PrevTab != i)
		//	{
		//		p_SubTabs[m_PrevTab]->EnableWindow(FALSE);
		//		p_SubTabs[m_PrevTab]->ShowWindow(SW_HIDE);
		//		m_PrevTab = i;
		//		p_SubTabs[m_PrevTab]->EnableWindow(TRUE);
		//		p_SubTabs[m_PrevTab]->ShowWindow(SW_SHOW);
		//		m_TabCtrl.SetCurSel(i);
		//	}

		//	return;
		//}

		std::string Str = p_SubTabs[i]->GenerateRecordString();

		if (Str.length() > 0)
		{
			std::ofstream outFile(m_pDoc->m_ProjectDirectory + Setting_FileName[i] + ".csv");

			if (outFile.is_open())
			{
				outFile << Str;
			}else
			{
			
				CString ErrorMessage;
				ErrorMessage.Format("File %s cannot be opened.", Setting_FileName[i]);
				AfxMessageBox(ErrorMessage);
				return;

			
			}

			outFile.close();
		}
		else
		{
			remove(m_pDoc->m_ProjectDirectory + Setting_FileName[i] + ".csv");
		}
	}

	OnOK();

	m_pDoc->ReadScenarioData();
	m_pDoc->UpdateAllViews(0);
}


BOOL CAssignmentSimulationSettingDlg::ReadScenarioCSVFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector)
{
	std::string fileName = m_pDoc->m_ProjectDirectory + ElementType + ".csv";
	std::vector<std::string> value;

	CCSVParser csvParser;
	csvParser.OpenCSVFile(fileName);

	name_vector = csvParser.GetHeaderList();




	while(csvParser.ReadRecord())
	{
		value_vector.push_back(csvParser.GetLineRecord());
	}

	csvParser.CloseCSVFile();

	return TRUE;
}

void CAssignmentSimulationSettingDlg::OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	//if (p_SubTabs[m_PrevTab]->ValidityCheck() != 0)
	//{
	//	m_TabCtrl.SetCurSel(m_PrevTab);
	//	return;
	//}
	//else
	{
		if (m_PrevTab == m_TabCtrl.GetCurSel())	return;

		//Disable and hide previous tab
		p_SubTabs[m_PrevTab]->EnableWindow(FALSE);
		p_SubTabs[m_PrevTab]->ShowWindow(SW_HIDE);

		//Enable and show current tab
		m_PrevTab = m_TabCtrl.GetCurSel();
		p_SubTabs[m_PrevTab]->EnableWindow(TRUE);
		p_SubTabs[m_PrevTab]->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}

void CAssignmentSimulationSettingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CAssignmentSimulationSettingDlg::OnBnClickedButtonAdd()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->AddRow();
}

void CAssignmentSimulationSettingDlg::OnBnClickedButtonCopy()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->CopyRow();
}

void CAssignmentSimulationSettingDlg::OnBnClickedButtonEditDataInExcel()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	std::string fileName = m_pDoc->m_ProjectDirectory + Setting_FileName[cur_tab] + ".csv";
	std::vector<std::string> value;
	m_pDoc->OpenCSVFileInExcel(fileName.c_str());
}

void CAssignmentSimulationSettingDlg::OnBnClickedButtonDelete()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->DeleteRow();
}
