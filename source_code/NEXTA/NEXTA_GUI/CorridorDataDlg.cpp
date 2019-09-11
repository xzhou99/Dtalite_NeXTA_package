// CorridorDataTabDlg.cpp : implementation file
//  Portions Copyright 2011-2013 Hao Lei(haolei.sc@gmail.com), Xuesong Zhou (xzhou99@gmail.com)

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
#include "TLiteDoc.h"
#include "CSVParser.h"
#include <string>
#include "CorridorDataDlg.h"

using std::vector;
using std::string;


static LPTSTR CorridorData_Element[MAX_NUM_OF_CORRIDOR_DATA_FILES] = {"Input Segment", "Output Segment", "Summary"};
static LPTSTR CorridorData_FileName[MAX_NUM_OF_CORRIDOR_DATA_FILES] = {"FREEVAL_input_segment","FREEVAL_output_segment", "FREEVAL_summary"};

// CCorridorDataDlg dialog

IMPLEMENT_DYNAMIC(CCorridorDataDlg, CDialog)

CCorridorDataDlg::CCorridorDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCorridorDataDlg::IDD, pParent)
	, m_ZoomToSelectedObject(TRUE)
{
	m_SelectTab = 0;

	m_SelectedFromNodeName = -1;
	m_SelectedToNodeName = -1;

}

CCorridorDataDlg::~CCorridorDataDlg()
{
}

BOOL CCorridorDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	p_SubTabs = new CCorridorDataTabDlg*[MAX_NUM_OF_CORRIDOR_DATA_FILES];

	vector<string> name_vector;
	vector<vector<string>> value_vector;
	vector<string> DefaultHeader;
	vector<CString> DefaultValue;
	vector<std::string> LinkString;

	CWaitCursor wait;

	for (int i=0; i < MAX_NUM_OF_CORRIDOR_DATA_FILES; i++)
	{
		name_vector.clear();
		value_vector.clear();
		DefaultHeader.clear();
		DefaultValue.clear();
		LinkString.clear();

		ReadCorridorDataCSVFile(CorridorData_FileName[i],name_vector,value_vector);

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T(CorridorData_Element[i]);
		m_TabCtrl.InsertItem(i, &tcItem);

		p_SubTabs[i] = new CCorridorDataTabDlg(name_vector, DefaultValue, value_vector);
		p_SubTabs[i]->m_pDoc = m_pDoc;

		p_SubTabs[i]->m_SelectedFromNodeName = this->m_SelectedFromNodeName;  // copy data
		p_SubTabs[i]->m_SelectedToNodeName = this->m_SelectedToNodeName;  // copy data
		p_SubTabs[i]->SetTabText(CorridorData_Element[i]);
		p_SubTabs[i]->Create(IDD_DIALOG_SCENARIO_TAB,&m_TabCtrl);
		p_SubTabs[i]->m_ZoomToSelectedObject = m_ZoomToSelectedObject;


	}


	p_SubTabs[m_SelectTab]->ShowWindow(SW_SHOW);

	for (int i=1;i<MAX_NUM_OF_CORRIDOR_DATA_FILES;i++)
	{
		p_SubTabs[i]->ShowWindow(SW_HIDE);
	}

	m_PrevTab = m_SelectTab;

	m_TabCtrl.SetCurSel(m_SelectTab);

	SetRectangle();

	return TRUE;
}


void CCorridorDataDlg::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	m_TabCtrl.GetClientRect(&tabRect);
	m_TabCtrl.GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	for(int nCount=0; nCount < MAX_NUM_OF_CORRIDOR_DATA_FILES; nCount++)
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

void CCorridorDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_TAB_CORRIDOR, m_TabCtrl);
	DDX_Check(pDX, IDC_CHECK_ZOOM_TO_SELECTED_OBJECT, m_ZoomToSelectedObject);
}


BEGIN_MESSAGE_MAP(CCorridorDataDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCorridorDataDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SETTING_TAB_CORRIDOR, &CCorridorDataDlg::OnTcnSelchangeSettingTab)
	ON_BN_CLICKED(IDCANCEL, &CCorridorDataDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCorridorDataDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CCorridorDataDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DATA_IN_EXCEL, &CCorridorDataDlg::OnBnClickedButtonEditDataInExcel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCorridorDataDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_CHECK_ZOOM_TO_SELECTED_OBJECT, &CCorridorDataDlg::OnBnClickedCheckZoomToSelectedObject)
	ON_NOTIFY(NM_CLICK, IDC_SETTING_TAB_CORRIDOR, &CCorridorDataDlg::OnNMClickSettingTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_SETTING_TAB_CORRIDOR, &CCorridorDataDlg::OnTcnSelchangingSettingTab)
	ON_BN_CLICKED(IDOK2, &CCorridorDataDlg::OnBnClickedOk2)
	ON_BN_CLICKED(ID_GIS_FIELD_NAME, &CCorridorDataDlg::OnBnClickedGisFieldName)
	ON_BN_CLICKED(ID_TravelTimeReliability, &CCorridorDataDlg::OnBnClickedTraveltimereliability)
	ON_BN_CLICKED(IDC_BUTTON_COMBINE_SEGMENT_LEFT, &CCorridorDataDlg::OnBnClickedButtonCombineSegmentLeft)
	ON_BN_CLICKED(IDC_BUTTON_COMBINE_SEGMENT_RIGHT, &CCorridorDataDlg::OnBnClickedButtonCombineSegmentRight)
	ON_BN_CLICKED(IDC_BUTTON_SPLIT_SEGMENTS, &CCorridorDataDlg::OnBnClickedButtonSplitSegments)
END_MESSAGE_MAP()


// CCorridorDataDlg message handlers

void CCorridorDataDlg::OnBnClickedOk()
{

	// save data
	OnBnClickedOk2();

	OnOK();

	m_pDoc->UpdateAllViews(0);
}


BOOL CCorridorDataDlg::ReadCorridorDataCSVFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector)
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

void CCorridorDataDlg::OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	//if (p_SubTabs[m_PrevTab]->ValidityCheck() != 0)
	//{
	//	m_TabCtrl.SetCurSel(m_PrevTab);
	//	return;
	//}
	//else

	if (m_PrevTab == m_TabCtrl.GetCurSel())	return;

	//Disable and hide previous tab
	p_SubTabs[m_PrevTab]->EnableWindow(FALSE);
	p_SubTabs[m_PrevTab]->ShowWindow(SW_HIDE);

	//Enable and show current tab
	m_PrevTab = m_TabCtrl.GetCurSel();
	p_SubTabs[m_PrevTab]->EnableWindow(TRUE);
	p_SubTabs[m_PrevTab]->ShowWindow(SW_SHOW);

	*pResult = 0;
}

void CCorridorDataDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CCorridorDataDlg::OnBnClickedButtonAdd()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->AddRow();
}

void CCorridorDataDlg::OnBnClickedButtonCopy()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->CopyRow();
}

void CCorridorDataDlg::OnBnClickedButtonEditDataInExcel()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	std::string fileName = m_pDoc->m_ProjectDirectory + CorridorData_FileName[cur_tab] + ".csv";
	std::vector<std::string> value;
	m_pDoc->OpenCSVFileInExcel(fileName.c_str());
}

void CCorridorDataDlg::OnBnClickedButtonDelete()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->DeleteRow();
}

void CCorridorDataDlg::OnBnClickedCheckZoomToSelectedObject()
{

	UpdateData(1);

	for (int i=0; i < MAX_NUM_OF_CORRIDOR_DATA_FILES; i++)
	{
		p_SubTabs[i]->m_ZoomToSelectedObject = m_ZoomToSelectedObject;
	}
}

void CCorridorDataDlg::OnNMClickSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
{

	*pResult = 0;
}

void CCorridorDataDlg::OnTcnSelchangingSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
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

void CCorridorDataDlg::OnBnClickedOk2()
{
	for (int i=0;i<MAX_NUM_OF_CORRIDOR_DATA_FILES;i++)
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
			std::ofstream outFile(m_pDoc->m_ProjectDirectory + CorridorData_FileName[i] + ".csv");

			if (outFile.is_open())
			{
				outFile << Str;
			}

			outFile.close();
		}
		else
		{
			remove(m_pDoc->m_ProjectDirectory + CorridorData_FileName[i] + ".csv");
		}
	}
}

void CCorridorDataDlg::OnBnClickedGisFieldName()
{
	// TODO: Add your control notification handler code here
}

void CCorridorDataDlg::OnBnClickedAddColumn()
{
	
}

void CCorridorDataDlg::OnBnClickedTraveltimereliability()
{
	m_pDoc->PerformPathTravelTimeReliabilityAnalysis();
}

void CCorridorDataDlg::OnBnClickedTimedependentdemand()
{
	
}

void CCorridorDataDlg::OnBnClickedButtonCombineSegmentLeft()
{
	//  //add column
	//	CGridColumnTrait* pTrait = NULL;
	//	m_ListCtrl.InsertColumnTrait(m_NumOfCols,name.c_str (),LVCFMT_LEFT,-1,-1, pTrait);
	//	m_ListCtrl.SetColumnWidth(m_NumOfCols,LVSCW_AUTOSIZE_USEHEADER);

	//	CString default_str;
	//	default_str.Format ("%d",default_value);

	//  //add default value
	//	for(int row = 0; row < m_NumOfRows; row++)
	//	{

	//			m_ListCtrl.SetItemText(row,m_NumOfCols,default_str);

	//	}
	//
	//m_NumOfCols++;

	//names.push_back (name);
}

void CCorridorDataDlg::OnBnClickedButtonCombineSegmentRight()
{
	// TODO: Add your control notification handler code here
}

void CCorridorDataDlg::OnBnClickedButtonSplitSegments()
{
	// TODO: Add your control notification handler code here
}
