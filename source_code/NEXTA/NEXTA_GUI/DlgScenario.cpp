// DlgScenario.cpp : implementation file
//  Portions Copyright 2011 Hao Lei(haolei.sc@gmail.com), Xuesong Zhou (xzhou99@gmail.com)

//   If you help write or modify the code, please also list your names here.
//   The reason of having Copyright info here is to ensure all the modified version, as a whole, under the GPL 
//   and further prevent a violation of the GPL.

// More about "How to use GNU licenses for your own software"
// http://www.gnu.org/licenses/gpl-howto.html


//    This file is part of NeXTA Version 3 (Open-source).

//    NEXTA is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    NEXTA is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with NEXTA.  If not, see <http://www.gnu.org/licenses/>.



#include "stdafx.h"
#include "TLite.h"
#include "Geometry.h"
#include "CSVParser.h"
#include "DlgScenario.h"

#include <map>
#include <vector>
#include <string>
#include <sstream>

#define _MAX_SCENARIO_SIZE 3
// CDlgScenario dialog
static LPTSTR SCENARIO_ELEMENTS[_MAX_SCENARIO_SIZE] = { "Work_Zone", "Dynamic_Message_Sign","Link_Based_Toll"};

IMPLEMENT_DYNAMIC(CDlgScenario, CDialog)

CDlgScenario::CDlgScenario(int idx,CWnd* pParent)
: CDialog(CDlgScenario::IDD, pParent)
{
	if (idx >= 0 && idx < _MAX_SCENARIO_SIZE)
	{
		m_SelectTab = idx;
	}
	else
	{
		m_SelectTab = 0;
	}
}

CDlgScenario::~CDlgScenario()
{
}

void CDlgScenario::GetDefaultInfo(int i, std::vector<std::string>& HeaderList, std::vector<CString>& DefaultList)
{
	CString str;
	switch (i)
	{
	case 0:  //work zone
	case 4:  //evacuation zone
	case 5:  //weather zone
		HeaderList.push_back("Link");
		HeaderList.push_back("Scenario No");
		HeaderList.push_back("Start Day No");
		HeaderList.push_back("End Day No");
		HeaderList.push_back("Start Time in Min");
		HeaderList.push_back("End Time in min");
		HeaderList.push_back("Capacity Reduction Percentage (%)");
		HeaderList.push_back("Speed Limit");

		DefaultList.push_back("0");
		DefaultList.push_back("1");
		DefaultList.push_back("100");

		DefaultList.push_back("0");
		DefaultList.push_back("1440");
		DefaultList.push_back("50");
		DefaultList.push_back("50");
		break;
	case 1:  // VMS
		HeaderList.push_back("Link");
		HeaderList.push_back("Scenario No");
		HeaderList.push_back("Start Day No");
		HeaderList.push_back("End Day No");
		HeaderList.push_back("Start Time in Min");
		HeaderList.push_back("End Time in min");
		HeaderList.push_back("Response Percentage (%)");


		DefaultList.push_back("0");
		DefaultList.push_back("1");
		DefaultList.push_back("100");
		DefaultList.push_back("0");
		DefaultList.push_back("1440");
		DefaultList.push_back("100");
		break;
	case 2:
		// link toll
		HeaderList.push_back("Link");
		HeaderList.push_back("Scenario No");
		HeaderList.push_back("Start Day No");
		HeaderList.push_back("End Day No");
		HeaderList.push_back("Start Time in Min");
		HeaderList.push_back("End Time in min");

		DefaultList.push_back("0");
		DefaultList.push_back("1");
		DefaultList.push_back("100");
		DefaultList.push_back("0");
		DefaultList.push_back("1440");

		for (int dt = 1; dt <= m_pDoc->m_DemandTypeVector.size(); dt++)
		{
			str.Format("Toll for Demand Type %d", dt);
			HeaderList.push_back(m_pDoc->CString2StdString(str));
			DefaultList.push_back("0");
		}

		break;
	case 6:  //radio
		HeaderList.push_back("Link");
		HeaderList.push_back("Scenario No");
		HeaderList.push_back("Start Day No");
		HeaderList.push_back("End Day No");
		HeaderList.push_back("Start Time in Min");
		HeaderList.push_back("End Time in min");
		HeaderList.push_back("Number of Detour Routes");
		HeaderList.push_back("Detour Route 1");
		HeaderList.push_back("Detour Route 2");
		HeaderList.push_back("Detour Route 3");

		DefaultList.push_back("0");
		DefaultList.push_back("1");
		DefaultList.push_back("100");

		DefaultList.push_back("0");
		DefaultList.push_back("1440");
		DefaultList.push_back("0");
		DefaultList.push_back("");
		DefaultList.push_back("");
		DefaultList.push_back("");
		break;
	case 7:  //ramp meter
		HeaderList.push_back("Link");
		HeaderList.push_back("Scenario No");
		HeaderList.push_back("Start Day No");
		HeaderList.push_back("End Day No");
		HeaderList.push_back("Start Time in Min");
		HeaderList.push_back("End Time in min");
		HeaderList.push_back("Capacity (vph)");
		HeaderList.push_back("Speed Limit");

		DefaultList.push_back("0");
		DefaultList.push_back("1");
		DefaultList.push_back("100");

		DefaultList.push_back("0");
		DefaultList.push_back("1440");
		DefaultList.push_back("50");
		DefaultList.push_back("50");
		break;
		
		break;
	}
}
void CDlgScenario::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCENARIO_TAB, m_TabCtrl);
}

BOOL IsValidElement(LPTSTR element)
{
	BOOL Found = FALSE;

	for (int i=0;i<_MAX_SCENARIO_SIZE;i++)
	{
		if (strcmp(SCENARIO_ELEMENTS[i],element) == 0)
		{
			Found = TRUE;
		}
	}

	return Found;
}

BOOL CDlgScenario::OnInitDialog()
{
	using std::string;
	using std::map;
	using std::vector;

	CDialog::OnInitDialog();

	vector<string> name_vector;
	vector<vector<string>> value_vector;

	//Construct link string 
	std::vector<std::string> LinkString = GetLinkString();

	for (int i=0;i < _MAX_SCENARIO_SIZE;i++)
	{
		name_vector.clear();
		value_vector.clear();

		ReadScenarioCSVFile(SCENARIO_ELEMENTS[i],name_vector,value_vector);

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T(SCENARIO_ELEMENTS[i]);
		m_TabCtrl.InsertItem(i, &tcItem);

		std::vector<string> DefaultHeader;
		std::vector<CString> DefaultValue;

		GetDefaultInfo(i,DefaultHeader,DefaultValue);
		//add individual tab dialog
		p_SubTabs[i] = new CDlgScenarioTab(DefaultHeader, DefaultValue, value_vector,LinkString);
		p_SubTabs[i]->SetTabText(SCENARIO_ELEMENTS[i]);
		p_SubTabs[i]->Create(IDD_DIALOG_SCENARIO_TAB,&m_TabCtrl);

	}

	//
	p_SubTabs[m_SelectTab]->ShowWindow(SW_SHOW);

	for (int i=0;i<_MAX_SCENARIO_SIZE;i++)
	{
		if(i!= m_SelectTab)
		{
			p_SubTabs[i]->ShowWindow(SW_HIDE);
		}
	}

	m_PrevTab=m_SelectTab;

	m_TabCtrl.SetCurSel(m_SelectTab);

	SetRectangle();
	return TRUE;
}

void CDlgScenario::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	m_TabCtrl.GetClientRect(&tabRect);
	m_TabCtrl.GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	for(int nCount=0; nCount < _MAX_SCENARIO_SIZE; nCount++)
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

BEGIN_MESSAGE_MAP(CDlgScenario, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgScenario::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgScenario::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SCENARIO_TAB, &CDlgScenario::OnTcnSelchangeScenarioTab)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgScenario::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgScenario::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ADD_SCENARIO, &CDlgScenario::OnBnClickedButtonAddScenario)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DATA_IN_EXCEL, &CDlgScenario::OnBnClickedButtonEditDataInExcel)
END_MESSAGE_MAP()


// CDlgScenario message handlers

void CDlgScenario::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	//std::string FirstLine = "<?xml version=\"1.0\"?>\n";

	for (int i=0;i<_MAX_SCENARIO_SIZE;i++)
	{
		if (p_SubTabs[i]->ValidityCheck() != 0)
		{
			if (m_PrevTab != i)
			{
				p_SubTabs[m_PrevTab]->EnableWindow(FALSE);
				p_SubTabs[m_PrevTab]->ShowWindow(SW_HIDE);
				m_PrevTab = i;
				p_SubTabs[m_PrevTab]->EnableWindow(TRUE);
				p_SubTabs[m_PrevTab]->ShowWindow(SW_SHOW);
				m_TabCtrl.SetCurSel(i);
			}

			return;
		}

		std::string Str = p_SubTabs[i]->GenerateRecordString();

		if (Str.length() > 0)
		{
			std::ofstream outFile(m_pDoc->m_ProjectDirectory + p_SubTabs[i]->GetTabText() + ".csv");

			if (outFile.is_open())
			{
				//outFile << FirstLine;
				outFile << Str;
			}

			outFile.close();
		}
		else
		{
			remove(m_pDoc->m_ProjectDirectory + p_SubTabs[i]->GetTabText() + ".csv");
		}
	}

	OnOK();

	m_pDoc->ReadScenarioData();
	m_pDoc->UpdateAllViews(0);

}

void CDlgScenario::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


void CDlgScenario::OnTcnSelchangeScenarioTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	//Error Checking before leaving current tab
	if (p_SubTabs[m_PrevTab]->ValidityCheck() != 0)
	{
		m_TabCtrl.SetCurSel(m_PrevTab);
		return;
	}
	else
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

void CDlgScenario::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->AddRow();
}

void CDlgScenario::OnBnClickedButtonDelete()
{
	// TODO: Add your control notification handler code here
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->DeleteRow();
}

std::vector<std::string> CDlgScenario::GetLinkString()
{
	std::vector<std::string> linkstring;

	if (m_pDoc != NULL)
	{
		long i = 0;
		DTALink* pLink = 0;
			std::list<DTALink*>::iterator iLink;
		for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
		{
						std::stringstream from_node_id_out;
						std::stringstream to_node_id_out;

						from_node_id_out << (*iLink)->m_FromNodeNumber ;
						to_node_id_out <<  (*iLink)->m_ToNodeNumber;

						string subStr;
						string str = "[" + from_node_id_out.str() + "," + to_node_id_out.str() + "]";

						linkstring.push_back(str);
		}
	}	
	return linkstring;
}

BOOL CDlgScenario::ReadScenarioCSVFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector)
{
	std::string fileName = m_pDoc->m_ProjectDirectory + "Scenario_"+ElementType + ".csv";
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


BOOL CDlgScenario::ReadXMLFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector)
{
	std::string fileName = m_pDoc->m_ProjectDirectory + ElementType + ".xml";
	BOOL Recorded = FALSE;
	std::vector<std::string> value;

	char* buffer = new char[fileName.length()+1];

	strcpy(buffer,fileName.c_str());

	if (m_XMLFile.LoadFromFile(buffer))
	{
		CXMLElement* p_Root = m_XMLFile.GetRoot();

		CXMLElement* p_Child = p_Root->GetFirstChild();

		while(p_Child)
		{
			LPTSTR ElementType_Read = p_Child->GetElementName();

			if (strcmp(ElementType,ElementType_Read) == 0)
			{
				CXMLElement* p_Element = p_Child->GetFirstChild();

				while(p_Element)
				{
					if (p_Element->GetElementType() == XET_ATTRIBUTE)
					{
						LPTSTR ElementName = p_Element->GetElementName();
						if (!Recorded)
						{
							name_vector.push_back(string(ElementName));
						}

						LPTSTR ElementValue = p_Element->GetValue();
						value.push_back(string(ElementValue));
					}

					p_Element = p_Child->GetNextChild();
				}

				if (!Recorded)
				{
					Recorded = TRUE;
				}
				value_vector.push_back(value);
				value.clear();
			}
			p_Child = p_Root->GetNextChild();
		}
	}

	return TRUE;
}
void CDlgScenario::OnBnClickedButtonAddScenario()
{
	// TODO: Add your control notification handler code here
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->CopyRow();
}

void CDlgScenario::OnBnClickedButtonEditDataInExcel()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	std::string fileName = m_pDoc->m_ProjectDirectory + "Scenario_"+SCENARIO_ELEMENTS[cur_tab] + ".csv";
	std::vector<std::string> value;
	m_pDoc->OpenCSVFileInExcel(fileName.c_str());
}
