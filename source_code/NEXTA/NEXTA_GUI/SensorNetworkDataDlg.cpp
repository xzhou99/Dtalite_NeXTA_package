// NetworkDataDlg.cpp : implementation file
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
#include "SensorNetworkDataDlg.h"
#include "CSVParser.h"
#include <string>
using std::vector;
using std::string;


static LPTSTR SensorNetworkData_Element[MAX_NUM_OF_SENSOR_NETWORK_DATA_FILES] = {"Sensor Link", "Movement Count","Calibration Data"};
static LPTSTR SensorNetworkData_FileName[MAX_NUM_OF_SENSOR_NETWORK_DATA_FILES] = {"input_sensor", "input_movement_count","output_validation_results"};

// CSensorNetworkDataSettingDlg dialog

IMPLEMENT_DYNAMIC(CSensorNetworkDataSettingDlg, CDialog)

CSensorNetworkDataSettingDlg::CSensorNetworkDataSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSensorNetworkDataSettingDlg::IDD, pParent)
	, m_ZoomToSelectedObject(TRUE)
{
	m_SelectTab = 0;

	m_SelectedFromNodeName = -1;
	m_SelectedToNodeName = -1;

}

CSensorNetworkDataSettingDlg::~CSensorNetworkDataSettingDlg()
{
}

BOOL CSensorNetworkDataSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	p_SubTabs = new CSensorNetworkDataTabDlg*[MAX_NUM_OF_SENSOR_NETWORK_DATA_FILES];

	vector<string> name_vector;
	vector<vector<string>> value_vector;
	vector<string> DefaultHeader;
	vector<CString> DefaultValue;
	vector<std::string> LinkString;

	CWaitCursor wait;

	for (int i=0; i < MAX_NUM_OF_SENSOR_NETWORK_DATA_FILES; i++)
	{
		name_vector.clear();
		value_vector.clear();
		DefaultHeader.clear();
		DefaultValue.clear();
		LinkString.clear();

		ReadNetworkDataCSVFile(SensorNetworkData_FileName[i],name_vector,value_vector);

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T(SensorNetworkData_Element[i]);
		m_TabCtrl.InsertItem(i, &tcItem);

		p_SubTabs[i] = new CSensorNetworkDataTabDlg(name_vector, DefaultValue, value_vector);
		p_SubTabs[i]->m_pDoc = m_pDoc;

		p_SubTabs[i]->m_SelectedFromNodeName = this->m_SelectedFromNodeName;  // copy data
		p_SubTabs[i]->m_SelectedToNodeName = this->m_SelectedToNodeName;  // copy data
		p_SubTabs[i]->SetTabText(SensorNetworkData_Element[i]);
		p_SubTabs[i]->Create(IDD_DIALOG_SCENARIO_TAB,&m_TabCtrl);
		p_SubTabs[i]->m_ZoomToSelectedObject = m_ZoomToSelectedObject;


	}


	p_SubTabs[m_SelectTab]->ShowWindow(SW_SHOW);

	for (int i=1;i<MAX_NUM_OF_SENSOR_NETWORK_DATA_FILES;i++)
	{
		p_SubTabs[i]->ShowWindow(SW_HIDE);
	}

	m_PrevTab = m_SelectTab;

	m_TabCtrl.SetCurSel(m_SelectTab);

	SetRectangle();

	return TRUE;
}


void CSensorNetworkDataSettingDlg::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	m_TabCtrl.GetClientRect(&tabRect);
	m_TabCtrl.GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	for(int nCount=0; nCount < MAX_NUM_OF_SENSOR_NETWORK_DATA_FILES; nCount++)
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

void CSensorNetworkDataSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_TAB, m_TabCtrl);
	DDX_Check(pDX, IDC_CHECK_ZOOM_TO_SELECTED_OBJECT, m_ZoomToSelectedObject);
}


BEGIN_MESSAGE_MAP(CSensorNetworkDataSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSensorNetworkDataSettingDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SETTING_TAB, &CSensorNetworkDataSettingDlg::OnTcnSelchangeSettingTab)
	ON_BN_CLICKED(IDCANCEL, &CSensorNetworkDataSettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CSensorNetworkDataSettingDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CSensorNetworkDataSettingDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DATA_IN_EXCEL, &CSensorNetworkDataSettingDlg::OnBnClickedButtonEditDataInExcel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CSensorNetworkDataSettingDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_CHECK_ZOOM_TO_SELECTED_OBJECT, &CSensorNetworkDataSettingDlg::OnBnClickedCheckZoomToSelectedObject)
	ON_NOTIFY(NM_CLICK, IDC_SETTING_TAB, &CSensorNetworkDataSettingDlg::OnNMClickSettingTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_SETTING_TAB, &CSensorNetworkDataSettingDlg::OnTcnSelchangingSettingTab)
END_MESSAGE_MAP()


// CSensorNetworkDataSettingDlg message handlers

void CSensorNetworkDataSettingDlg::OnBnClickedOk()
{
	for (int i=0;i<MAX_NUM_OF_SENSOR_NETWORK_DATA_FILES;i++)
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
			std::ofstream outFile(m_pDoc->m_ProjectDirectory + SensorNetworkData_FileName[i] + ".csv");

			if (outFile.is_open())
			{
				outFile << Str;
			}

			outFile.close();
		}
		else
		{
			remove(m_pDoc->m_ProjectDirectory + SensorNetworkData_FileName[i] + ".csv");
		}
	}

	
	OnOK();
	{
	CWaitCursor wait;
	// reload the network 
	m_pDoc->ReadScenarioData();
	m_pDoc->UpdateAllViews(0);
	}
}


BOOL CSensorNetworkDataSettingDlg::ReadNetworkDataCSVFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector)
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

void CSensorNetworkDataSettingDlg::OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
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

void CSensorNetworkDataSettingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CSensorNetworkDataSettingDlg::OnBnClickedButtonAdd()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->AddRow();
}

void CSensorNetworkDataSettingDlg::OnBnClickedButtonCopy()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->CopyRow();
}

void CSensorNetworkDataSettingDlg::OnBnClickedButtonEditDataInExcel()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	std::string fileName = m_pDoc->m_ProjectDirectory + SensorNetworkData_FileName[cur_tab] + ".csv";
	std::vector<std::string> value;
	m_pDoc->OpenCSVFileInExcel(fileName.c_str());
}

void CSensorNetworkDataSettingDlg::OnBnClickedButtonDelete()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->DeleteRow();
}

void CSensorNetworkDataSettingDlg::OnBnClickedCheckZoomToSelectedObject()
{

	UpdateData(1);
		for (int i=0; i < MAX_NUM_OF_SENSOR_NETWORK_DATA_FILES; i++)
	{
		p_SubTabs[i]->m_ZoomToSelectedObject = m_ZoomToSelectedObject;
	}
}

void CSensorNetworkDataSettingDlg::OnNMClickSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
{

	*pResult = 0;
}

void CSensorNetworkDataSettingDlg::OnTcnSelchangingSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
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
