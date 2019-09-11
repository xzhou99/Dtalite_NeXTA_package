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
#include "NetworkDataDlg.h"
#include "CSVParser.h"
#include <string>
using std::vector;
using std::string;


static LPTSTR NetworkData_Element[MAX_NUM_OF_NETWORK_DATA_FILES] = {"Node", "Link", "Zone", "Activity Location", "Movement"};
static LPTSTR NetworkData_FileName[MAX_NUM_OF_NETWORK_DATA_FILES] = {"input_node","input_link", "input_zone", "input_activity_location", "input_movement"};

// CNetworkDataSettingDlg dialog

IMPLEMENT_DYNAMIC(CNetworkDataSettingDlg, CDialog)

CNetworkDataSettingDlg::CNetworkDataSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetworkDataSettingDlg::IDD, pParent)
	, m_ZoomToSelectedObject(TRUE)
{
	m_SelectTab = 0;
	m_SelectRowByUser = -1;

	m_SelectedFromNodeName = -1;
	m_SelectedToNodeName = -1;

}

CNetworkDataSettingDlg::~CNetworkDataSettingDlg()
{
}

BOOL CNetworkDataSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	p_SubTabs = new CNetworkDataTabDlg*[MAX_NUM_OF_NETWORK_DATA_FILES];

	vector<string> name_vector;
	vector<vector<string>> value_vector;
	vector<string> DefaultHeader;
	vector<CString> DefaultValue;
	vector<std::string> LinkString;

	CWaitCursor wait;

	for (int i=0; i < MAX_NUM_OF_NETWORK_DATA_FILES; i++)
	{
		name_vector.clear();
		value_vector.clear();
		DefaultHeader.clear();
		DefaultValue.clear();
		LinkString.clear();

		ReadNetworkDataCSVFile(NetworkData_FileName[i],name_vector,value_vector);

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T(NetworkData_Element[i]);
		m_TabCtrl.InsertItem(i, &tcItem);

		p_SubTabs[i] = new CNetworkDataTabDlg(name_vector, DefaultValue, value_vector);

		if(i == m_SelectTab)
		{
		p_SubTabs[i]->m_SelectRowByUser = m_SelectRowByUser;
		}

		p_SubTabs[i]->m_pDoc = m_pDoc;

		p_SubTabs[i]->m_SelectedFromNodeName = this->m_SelectedFromNodeName;  // copy data
		p_SubTabs[i]->m_SelectedToNodeName = this->m_SelectedToNodeName;  // copy data
		p_SubTabs[i]->SetTabText(NetworkData_Element[i]);
		p_SubTabs[i]->Create(IDD_DIALOG_SCENARIO_TAB,&m_TabCtrl);
		p_SubTabs[i]->m_ZoomToSelectedObject = m_ZoomToSelectedObject; 



	}


	p_SubTabs[m_SelectTab]->ShowWindow(SW_SHOW);

	for (int i=1;i<MAX_NUM_OF_NETWORK_DATA_FILES;i++)
	{
		p_SubTabs[i]->ShowWindow(SW_HIDE);
	}

	m_PrevTab = m_SelectTab;

	m_TabCtrl.SetCurSel(m_SelectTab);

	SetRectangle();

	return TRUE;
}


void CNetworkDataSettingDlg::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	m_TabCtrl.GetClientRect(&tabRect);
	m_TabCtrl.GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	for(int nCount=0; nCount < MAX_NUM_OF_NETWORK_DATA_FILES; nCount++)
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

void CNetworkDataSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_TAB, m_TabCtrl);
	DDX_Check(pDX, IDC_CHECK_ZOOM_TO_SELECTED_OBJECT, m_ZoomToSelectedObject);
}


BEGIN_MESSAGE_MAP(CNetworkDataSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNetworkDataSettingDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SETTING_TAB, &CNetworkDataSettingDlg::OnTcnSelchangeSettingTab)
	ON_BN_CLICKED(IDCANCEL, &CNetworkDataSettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CNetworkDataSettingDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CNetworkDataSettingDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DATA_IN_EXCEL, &CNetworkDataSettingDlg::OnBnClickedButtonEditDataInExcel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CNetworkDataSettingDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_CHECK_ZOOM_TO_SELECTED_OBJECT, &CNetworkDataSettingDlg::OnBnClickedCheckZoomToSelectedObject)
	ON_NOTIFY(NM_CLICK, IDC_SETTING_TAB, &CNetworkDataSettingDlg::OnNMClickSettingTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_SETTING_TAB, &CNetworkDataSettingDlg::OnTcnSelchangingSettingTab)
END_MESSAGE_MAP()


// CNetworkDataSettingDlg message handlers

void CNetworkDataSettingDlg::OnBnClickedOk()
{
	for (int i=0;i<MAX_NUM_OF_NETWORK_DATA_FILES;i++)
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
			std::ofstream outFile(m_pDoc->m_ProjectDirectory + NetworkData_FileName[i] + ".csv");

			if (outFile.is_open())
			{
				outFile << Str;
			}

			outFile.close();
		}
		else
		{
			remove(m_pDoc->m_ProjectDirectory + NetworkData_FileName[i] + ".csv");
		}
	}

	
	OnOK();
	{
	CWaitCursor wait;
	// reload the network 
	m_pDoc->OnOpenTrafficNetworkDocument(m_pDoc->m_ProjectFile ,false, false);
	m_pDoc->ReadScenarioData();
	m_pDoc->UpdateAllViews(0);
	}
}


BOOL CNetworkDataSettingDlg::ReadNetworkDataCSVFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector)
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

void CNetworkDataSettingDlg::OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
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

void CNetworkDataSettingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CNetworkDataSettingDlg::OnBnClickedButtonAdd()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->AddRow();
}

void CNetworkDataSettingDlg::OnBnClickedButtonCopy()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->CopyRow();
}

void CNetworkDataSettingDlg::OnBnClickedButtonEditDataInExcel()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	std::string fileName = m_pDoc->m_ProjectDirectory + NetworkData_FileName[cur_tab] + ".csv";
	std::vector<std::string> value;
	m_pDoc->OpenCSVFileInExcel(fileName.c_str());
}

void CNetworkDataSettingDlg::OnBnClickedButtonDelete()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->DeleteRow();
}

void CNetworkDataSettingDlg::OnBnClickedCheckZoomToSelectedObject()
{

	UpdateData(1);
		for (int i=0; i < MAX_NUM_OF_NETWORK_DATA_FILES; i++)
	{
		p_SubTabs[i]->m_ZoomToSelectedObject = m_ZoomToSelectedObject;
	}
}

void CNetworkDataSettingDlg::OnNMClickSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
{

	*pResult = 0;
}

void CNetworkDataSettingDlg::OnTcnSelchangingSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
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
