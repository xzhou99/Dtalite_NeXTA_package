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
#include "DlgScenarioTab.h"

#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"

#include <string>
#include <sstream>

// CDlgScenarioTab dialog

IMPLEMENT_DYNAMIC(CDlgScenarioTab, CDialog)

CDlgScenarioTab::CDlgScenarioTab(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgScenarioTab::IDD, pParent)
{

}

CDlgScenarioTab::CDlgScenarioTab(std::vector<std::string> Names, std::vector<CString> default_value,
								 std::vector<std::vector<std::string>> Values, std::vector<std::string> linkstring)
{
	names = Names;
	values = Values;
	this->default_value = default_value;
	LinkString = linkstring;
}

CDlgScenarioTab::~CDlgScenarioTab()
{
}

void CDlgScenarioTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GRIDLISTCTRLEX,m_ListCtrl);
}

BOOL CDlgScenarioTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Give better margin to editors
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);

	//Add Columns and set headers
	for (size_t i=0;i<names.size();i++)
	{

		CGridColumnTrait* pTrait = NULL;
		if (i == 0)	// Link
		{
			CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;

			for (size_t n=0;n<LinkString.size();n++)
			{
				pComboTrait->AddItem((int)n,LinkString.at(n).c_str());
			}

			pTrait = pComboTrait;
		}
		else
		{
			pTrait = new CGridColumnTraitEdit();
		}

		m_ListCtrl.InsertColumnTrait((int)i,names.at(i).c_str(),LVCFMT_LEFT,-1,-1, pTrait);
		m_ListCtrl.SetColumnWidth((int)i,LVSCW_AUTOSIZE_USEHEADER);

		if (names.at(i)=="Start Time (min)")
		{
			m_StartTime_Idx = i;
		}

		if (names.at(i)=="End Time (min)")
		{
			m_EndTime_Idx = i;
		}

		if (names.at(i).find("%")!=std::string::npos) 
		{
			m_Rate_Idx = i;
		}
	}

	m_ListCtrl.SetColumnWidth(0, 80);

	m_NumOfCols = names.size();


	//Add Rows
	for (size_t i=0;i<values.size();i++)
	{
		int Index = m_ListCtrl.InsertItem(LVIF_TEXT,i,values.at(i).at(0).c_str() , 0, 0, 0, NULL);

		for (size_t j=1;j<values.at(i).size();j++)
		{
			m_ListCtrl.SetItemText(Index, j,values.at(i).at(j).c_str());
		}
	}

	m_NumOfRows = values.size();

	return TRUE;
}

void CDlgScenarioTab::SetTabText(CString s)
{
	m_TabText = "Scenario_" + s;
}

CString CDlgScenarioTab::GetTabText()
{
	return m_TabText;
}

BOOL CDlgScenarioTab::CopyRow()
{
	if(m_NumOfRows==0)
		return false;

	int OriginalNumOfRows = m_NumOfRows;
	int Index = m_NumOfRows;

	int k;
	int number_of_selected_rows = 0;

		for(k = 0; k<  OriginalNumOfRows; k++)
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

	for(k = 0; k<  OriginalNumOfRows; k++)
	{
	if(m_ListCtrl.IsRowSelected(k))  // if row is selected
	{
	//static CString
	Index = m_ListCtrl.InsertItem(LVIF_TEXT,m_NumOfRows,NULL,0,0,0,NULL);
	for (int i=0;i< min(default_value.size()+1,m_NumOfCols);i++)
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
BOOL CDlgScenarioTab::AddRow()
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

BOOL CDlgScenarioTab::AddColumn(std::string name, double default_value)
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

BOOL CDlgScenarioTab::CheckColumn(std::string name, CString message)
{
	for (int i=0;i<names.size();i++)
	{
		if(name.compare (names[i].c_str ())==0)  // field name exists in the file
			return true;
	}

	CString str;
	str.Format("Please update file %s from NEXTA installation folder\\default_data_folder: %s",message);
	AfxMessageBox(str, MB_ICONINFORMATION);
	return false;

}
BOOL CDlgScenarioTab::AddColumn(std::string name, int default_value)
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
int CDlgScenarioTab::ValidityCheck()
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

			if (text.GetLength() == 0)
			{
				m_ListCtrl.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
				ErrorMsg.Format("[%s] cannot be empty",names.at(j).c_str());
				MessageBox(ErrorMsg,NULL,MB_ICONERROR);
				ret = -1;
				return ret;
			}
			
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

CString CDlgScenarioTab::GenerateRecordString()
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
			if (j==0)
			{
				subStr += "\"";
			}

			subStr += std::string(m_ListCtrl.GetItemText(i,j));

			if (j == 0)
			{
				subStr += "\"";
			}

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

BOOL CDlgScenarioTab::DeleteRow()
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


BEGIN_MESSAGE_MAP(CDlgScenarioTab, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GRIDLISTCTRLEX, &CDlgScenarioTab::OnLvnItemchangedGridlistctrlex)
END_MESSAGE_MAP()


// CDlgScenarioTab message handlers

void CDlgScenarioTab::OnLvnItemchangedGridlistctrlex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
