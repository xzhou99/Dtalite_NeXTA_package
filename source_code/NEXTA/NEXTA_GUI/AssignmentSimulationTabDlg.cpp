// DlgScenarioTab.cpp : implementation file

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
#include "AssignmentSimulationTabDlg.h"

#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"

#include <string>
#include <sstream>

// CAssignmentSimulationTabDlg dialog
enum Traffic_Flow_Model {BPR, VERTICAL_QUEUE, SPATIAL_QUEUE, NEWELL, MAX_FLOW_MODEL};


static LPSTR Traffic_Flow_Model_String[MAX_FLOW_MODEL] = {
"BPR Function", 
"Point Queue Model", 
"Spatial Queue Model",
"Newell's N-Curve Model",
};


enum traffic_analysis_method {Trip_Accessibiliy, Trip_Genration, Trip_Distribution, Traffic_Assignment,OD_Estimation,ABM_DTA_Integration, MAX_ASSIGNMENT_METHOD};


static LPSTR traffic_analysis_method_String[MAX_ASSIGNMENT_METHOD] = {
"Travel Time-based Accessibility",
"Trip Generation (TBD)",
"Trip Distribution (TBD)", 
"Traffic Assignment", 
"OD Demand Estimation",
"ABM+DTA Integration"};
static LPSTR UE_Gap_Method_String[2] = {"For Switched Agents Only-0", "For All Agents-1"};

IMPLEMENT_DYNAMIC(CAssignmentSimulationTabDlg, CDialog)

CAssignmentSimulationTabDlg::CAssignmentSimulationTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssignmentSimulationTabDlg::IDD, pParent)
{

}

CAssignmentSimulationTabDlg::CAssignmentSimulationTabDlg(std::vector<std::string> Names, std::vector<CString> default_value,
								 std::vector<std::vector<std::string>> Values)
{
	names = Names;
	values = Values;
	this->default_value = default_value;
	this->m_TrafficFlowModel_Idx = -1;
	m_traffic_analysis_method_Idx = -1;
	m_ue_gap_calculation_method_Idx = -1;
}

CAssignmentSimulationTabDlg::~CAssignmentSimulationTabDlg()
{
}

void CAssignmentSimulationTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GRIDLISTCTRLEX,m_ListCtrl);
}

BOOL CAssignmentSimulationTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Give better margin to editors
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);

	//Add Columns and set headers
	for (size_t i=0;i<names.size();i++)
	{
		if (names.at(i).find("traffic_flow_model") != std::string::npos)
		{
			m_TrafficFlowModel_Idx = i;
		}


		if (names.at(i).find("traffic_analysis_method") != std::string::npos)
		{
			m_traffic_analysis_method_Idx = i;
		}

		CGridColumnTrait* pTrait = NULL;

		if (i == m_TrafficFlowModel_Idx)
		{
			CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;

			pComboTrait->SetStyle (CBS_DROPDOWNLIST);

			for (size_t n=0; n < MAX_FLOW_MODEL ;n++)
			{
				pComboTrait->AddItem((int)n, Traffic_Flow_Model_String[n]);
			}

			pTrait = pComboTrait;
		}
		else
		{

			{
				if (i == m_traffic_analysis_method_Idx)
				{
					CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;
					for (size_t n=0; n < MAX_ASSIGNMENT_METHOD ;n++)
					{
						pComboTrait->AddItem((int)n, traffic_analysis_method_String[n]);
					}
					pTrait = pComboTrait;
				}else if (i == m_ue_gap_calculation_method_Idx)
				{
					CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;
					for (size_t n=0; n < MAX_ASSIGNMENT_METHOD ;n++)
					{
						pComboTrait->AddItem((int)n, UE_Gap_Method_String[n]);
					}
					pTrait = pComboTrait;
				
				}
				else
				{				
					pTrait = new CGridColumnTraitEdit();
				}
			}
		}

		m_ListCtrl.InsertColumnTrait((int)i,names.at(i).c_str(),LVCFMT_LEFT,-1,-1, pTrait);
		m_ListCtrl.SetColumnWidth((int)i,LVSCW_AUTOSIZE_USEHEADER);
	}

	m_ListCtrl.SetColumnWidth(0, 80);

	m_NumOfCols = names.size();


	//Add Rows
	for (size_t i=0;i<values.size();i++)
	{
		int Index = m_ListCtrl.InsertItem(LVIF_TEXT,i,values.at(i).at(0).c_str() , 0, 0, 0, NULL);
		for (size_t j=1; j < values.at(i).size();j++)
		{
			int nCol = j;
			if (j == m_TrafficFlowModel_Idx)
			{
				int traffic_flow_model = atoi(values.at(i).at(j).c_str());
				m_ListCtrl.SetItemText(Index, nCol,Traffic_Flow_Model_String[traffic_flow_model]);
			}
			else
			{
				{
					if (j == this->m_traffic_analysis_method_Idx)
					{
						int traffic_analysis_method = atoi(values.at(i).at(j).c_str());
						m_ListCtrl.SetItemText(Index, nCol,traffic_analysis_method_String[traffic_analysis_method]);
					}
					else if (j == this ->m_ue_gap_calculation_method_Idx )
					{
						int ue_gap_method = atoi(values.at(i).at(j).c_str());
						m_ListCtrl.SetItemText(Index, nCol,UE_Gap_Method_String[ue_gap_method]);

					}else
					{					
						m_ListCtrl.SetItemText(Index, nCol,values.at(i).at(j).c_str());
					}
				}
			}
		}
	}

	m_NumOfRows = values.size();

	if(m_TabText.Find("Simulation")!=-1)  //scenario settings
	{
	AddColumn("ue_gap_calculation_method",0);
	AddColumn("ODME_start_iteration",100);
	AddColumn("ODME_step_size",0.05);
	AddColumn("ODME_max_percentage_deviation_wrt_hist_demand",70);
	AddColumn("calibration_data_start_time_in_min",0);
	AddColumn("calibration_data_end_time_in_min",1440);
	}

	if(m_TabText.Find("Vehicle Type")!=-1)  //scenario settings
	{
	CheckColumn("percentage_of_age_0", "Field percentage_of_age_0 is missing.");
	}
	if(m_TabText.Find("Vehicle Emission Rate")!=-1)  //scenario settings
	{
	CheckColumn("age", "Field age is missing.");
	}

	return TRUE;
}

void CAssignmentSimulationTabDlg::SetTabText(CString s)
{
	m_TabText = "Scenario_" + s;
}

CString CAssignmentSimulationTabDlg::GetTabText()
{
	return m_TabText;
}

BOOL CAssignmentSimulationTabDlg::CopyRow()
{
	if(m_NumOfRows==0)
		return FALSE;

	int OriginalNumOfRows = m_NumOfRows;
	int Index = m_NumOfRows;

	int k;
	int number_of_selected_rows = 0;

	for(k = 0; k < OriginalNumOfRows; k++)
	{
		if(m_ListCtrl.IsRowSelected(k))  // if row is selected
		{
			number_of_selected_rows++;
		}
	}

	if(number_of_selected_rows ==0)
	{
		m_ListCtrl.SelectRow (m_NumOfRows-1,true);  // auto select last row
	}

	for(k = 0; k < OriginalNumOfRows; k++)
	{
		if(m_ListCtrl.IsRowSelected(k))  // if row is selected
		{
			//static CString
			Index = m_ListCtrl.InsertItem(LVIF_TEXT,m_NumOfRows,NULL,0,0,0,NULL);
			for (int i=0;i< m_NumOfCols;i++)
			{
				CString text = m_ListCtrl.GetItemText(k,i);
				m_ListCtrl.SetItemText(Index,i,text);
			}
			

			m_NumOfRows++;
		}
	}

	m_ListCtrl.SetItemState(Index,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_ListCtrl.SetFocus();
	return TRUE;
}

BOOL CAssignmentSimulationTabDlg::AddRow()
{
	//static CString
	int Index = m_ListCtrl.InsertItem(LVIF_TEXT,m_NumOfRows,NULL,0,0,0,NULL);
	for (int i=1;i< min(default_value.size()+1,m_NumOfCols);i++)
	{
		m_ListCtrl.SetItemText(Index,i,default_value.at(i-1));
	}
	
	m_NumOfRows++;

	m_ListCtrl.SetItemState(Index,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_ListCtrl.SetFocus();
	return TRUE;
}

int CAssignmentSimulationTabDlg::ValidityCheck()
{
	using std::string;
	using std::istringstream;

	int ret = 0;

	int col = m_ListCtrl.GetColumnCount();
	int row = m_ListCtrl.GetItemCount();

	int start_time, end_time;

	CString ErrorMsg;

	for (int i=0;i<row;i++)
	{
		start_time = end_time = -1;
		for (int j=0;j<col;j++)
		{
			CString text = m_ListCtrl.GetItemText(i,j);

			//if (text.GetLength() == 0)
			//{
			//	m_ListCtrl.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
			//	ErrorMsg.Format("[%s] cannot be empty",names.at(j).c_str());
			//	MessageBox(ErrorMsg,NULL,MB_ICONERROR);
			//	ret = -1;
			//	return ret;
			//}
			
			istringstream iss(text.GetBuffer());
			text.ReleaseBuffer();

			if (j != 0 && j != m_Rate_Idx)
			{
				int value;
				iss >> value;

				//if (!iss.eof())
				//{
				//	ErrorMsg.Format("Integer is required at row %d col %d",i+1, j+1);
				//	MessageBox(ErrorMsg,NULL,MB_ICONERROR);
				//	return -5;
				//}

				if (value < 0)
				{
					m_ListCtrl.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
					ErrorMsg.Format("Negative value is not valid at [%s]",names.at(j).c_str());
					MessageBox(ErrorMsg,NULL,MB_ICONERROR);
					ret = -2;
					return ret;
				}
				
				if (j == m_StartTime_Idx)
				{
					start_time = value;
				}

				if (j == m_EndTime_Idx)
				{
					end_time = value;
				}
			}

			if ( j == m_Rate_Idx)
			{
				double value;
				iss >> value;
				if (value < 0 || value >100)
				{
					m_ListCtrl.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
					ErrorMsg.Format("[%s] must be between 0 and 100!",names.at(j).c_str());
					MessageBox(ErrorMsg,NULL,MB_ICONERROR);
					ret = -3;
					return ret;
				}
			}

		}

		if (start_time > end_time)
		{
			m_ListCtrl.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
			ErrorMsg.Format("Row %d: Start Time must be less than End Time!",i+1);
			MessageBox(ErrorMsg,NULL,MB_ICONERROR);
			//MessageBox("Start Time must be less than End Time!");
			ret = -4;
			return ret;
		}
	}

	return ret;
}

CString CAssignmentSimulationTabDlg::GenerateRecordString()
{
	std::string Str;

	for (int i=0;i<names.size();i++)
	{
		Str += names.at(i);
		if (i != names.size() - 1)
		{
			Str += ",";
		}
	}

	Str += "\n";

	for (int i=0;i<m_ListCtrl.GetItemCount();i++)
	{
		std::string subStr;
		for (int j=0;j<m_NumOfCols;j++)
		{
			std::string tmp = std::string(m_ListCtrl.GetItemText(i,j));
			if (j == this->m_TrafficFlowModel_Idx)
			{
				for (int n=0;n<MAX_FLOW_MODEL;n++)
				{
					if (tmp.find(Traffic_Flow_Model_String[n]) != std::string::npos)
					{
						std::stringstream ss;
						ss << n;
						tmp = ss.str();
					}
				}
			}

			if (j == this->m_traffic_analysis_method_Idx)
			{
				for (int n=0;n<MAX_ASSIGNMENT_METHOD;n++)
				{
					if (tmp.find(traffic_analysis_method_String[n]) != std::string::npos)
					{
						std::stringstream ss;
						ss << n;
						tmp = ss.str();
					}
				}
			}

			if (j == this->m_ue_gap_calculation_method_Idx)
			{
				for (int n=0;n<2;n++)
				{
					if (tmp.find(UE_Gap_Method_String[n]) != std::string::npos)
					{
						std::stringstream ss;
						ss << n;
						tmp = ss.str();
					}
				}
			}

			subStr += tmp;

			if (j != m_NumOfCols - 1)
			{
				subStr += ",";
			}
			
		}
		subStr += "\n";

		Str += subStr;
	}

	return Str.c_str();
}

BOOL CAssignmentSimulationTabDlg::DeleteRow()
{
	int nItem = m_ListCtrl.GetSelectionMark();
	if (nItem == -1)
	{
		MessageBox("No record is selected!");
		return FALSE;
	}
	
	m_ListCtrl.DeleteItem(nItem);

	m_NumOfRows--;

	return TRUE;
}


BOOL CAssignmentSimulationTabDlg::AddColumn(std::string name, double default_value)
{

		for (int i=0;i<names.size();i++)
	{
		if(name.compare  (names[i].c_str ())==0)  // field name exist in the file
			return false;
	}

	  //add column
		CGridColumnTrait* pTrait = NULL;
		m_ListCtrl.InsertColumnTrait(m_NumOfCols,name.c_str (),LVCFMT_LEFT,-1,-1, pTrait);
		m_ListCtrl.SetColumnWidth(m_NumOfCols,LVSCW_AUTOSIZE_USEHEADER);

		CString default_str;
		default_str.Format ("%.2f",default_value);

	  //add default value
		for(int row = 0; row < m_NumOfRows; row++)
		{

				m_ListCtrl.SetItemText(row,m_NumOfCols,default_str);

		}
	
	m_NumOfCols++;

	names.push_back (name);

	return TRUE;
}
BOOL CAssignmentSimulationTabDlg::AddColumn(std::string name, int default_value)
{

		for (int i=0;i<names.size();i++)
	{
		if(name.compare (names[i].c_str ())==0)  // field name exists in the file
			return false;
	}

	  //add column
		CGridColumnTrait* pTrait = NULL;
		m_ListCtrl.InsertColumnTrait(m_NumOfCols,name.c_str (),LVCFMT_LEFT,-1,-1, pTrait);
		m_ListCtrl.SetColumnWidth(m_NumOfCols,LVSCW_AUTOSIZE_USEHEADER);

		CString default_str;
		default_str.Format ("%d",default_value);

	  //add default value
		for(int row = 0; row < m_NumOfRows; row++)
		{

				m_ListCtrl.SetItemText(row,m_NumOfCols,default_str);

		}
	
	m_NumOfCols++;

	names.push_back (name);

	return TRUE;
}

BOOL CAssignmentSimulationTabDlg::CheckColumn(std::string name, CString message)
{
	for (int i=0;i<names.size();i++)
	{
		if(name.compare (names[i].c_str ())==0)  // field name exists in the file
			return true;
	}

	CString str;
	str.Format("Please update file %s from NEXTA installation folder\\default_data_folder: %s",m_FileName,message);
	AfxMessageBox(str, MB_ICONINFORMATION);
	return false;

}
BEGIN_MESSAGE_MAP(CAssignmentSimulationTabDlg, CDialog)
END_MESSAGE_MAP()


// CDlgScenarioTab message handlers

