// GIS_Import_DataTabDlg.cpp : implementation file
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
#ifndef _WIN64
#include "Data-Interface\\include\\ogrsf_frmts.h"
#endif
#include "GIS_Import_DataDlg.h"
using std::vector;
using std::string;


static LPTSTR GIS_Import_Data_Element[MAX_NUM_OF_GIS_IMPORT_DATA_FILES] = {"Input Node Type", "Input Link Type", "Input Demand Meta Data","GIS Import Setting"};
static LPTSTR GIS_Import_Data_FileName[MAX_NUM_OF_GIS_IMPORT_DATA_FILES] = {"input_node_control_type","input_link_type", "input_demand_meta_data", "import_GIS_settings"};

// CGIS_Import_DataDlg dialog

IMPLEMENT_DYNAMIC(CGIS_Import_DataDlg, CDialog)

CGIS_Import_DataDlg::CGIS_Import_DataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGIS_Import_DataDlg::IDD, pParent)
	, m_ZoomToSelectedObject(TRUE)
{
	m_SelectTab = 0;

	m_SelectedFromNodeName = -1;
	m_SelectedToNodeName = -1;

}

CGIS_Import_DataDlg::~CGIS_Import_DataDlg()
{
}

BOOL CGIS_Import_DataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	p_SubTabs = new CGIS_Import_DataTabDlg*[MAX_NUM_OF_GIS_IMPORT_DATA_FILES];

	vector<string> name_vector;
	vector<vector<string>> value_vector;
	vector<string> DefaultHeader;
	vector<CString> DefaultValue;
	vector<std::string> LinkString;

	CWaitCursor wait;

	for (int i=0; i < MAX_NUM_OF_GIS_IMPORT_DATA_FILES; i++)
	{
		name_vector.clear();
		value_vector.clear();
		DefaultHeader.clear();
		DefaultValue.clear();
		LinkString.clear();

		ReadGIS_Import_DataCSVFile(GIS_Import_Data_FileName[i],name_vector,value_vector);

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = _T(GIS_Import_Data_Element[i]);
		m_TabCtrl.InsertItem(i, &tcItem);

		p_SubTabs[i] = new CGIS_Import_DataTabDlg(name_vector, DefaultValue, value_vector);
		p_SubTabs[i]->m_pDoc = m_pDoc;

		p_SubTabs[i]->m_SelectedFromNodeName = this->m_SelectedFromNodeName;  // copy data
		p_SubTabs[i]->m_SelectedToNodeName = this->m_SelectedToNodeName;  // copy data
		p_SubTabs[i]->SetTabText(GIS_Import_Data_Element[i]);
		p_SubTabs[i]->Create(IDD_DIALOG_SCENARIO_TAB,&m_TabCtrl);
		p_SubTabs[i]->m_ZoomToSelectedObject = m_ZoomToSelectedObject;

		if(i == 3)
		{  //GIS importing 
		p_SubTabs[i]->m_ListCtrl.m_FirstColumnWithTitle  = true;
		p_SubTabs[i]->m_ListCtrl.m_SecondColumnWithKey = true;
		}



	}


	p_SubTabs[m_SelectTab]->ShowWindow(SW_SHOW);

	for (int i=1;i<MAX_NUM_OF_GIS_IMPORT_DATA_FILES;i++)
	{
		p_SubTabs[i]->ShowWindow(SW_HIDE);
	}

	m_PrevTab = m_SelectTab;

	m_TabCtrl.SetCurSel(m_SelectTab);

	SetRectangle();

	return TRUE;
}


void CGIS_Import_DataDlg::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	m_TabCtrl.GetClientRect(&tabRect);
	m_TabCtrl.GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	for(int nCount=0; nCount < MAX_NUM_OF_GIS_IMPORT_DATA_FILES; nCount++)
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

void CGIS_Import_DataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_DIALOG_SETTING_TAB_GIS_IMPORT, m_TabCtrl);

}


BEGIN_MESSAGE_MAP(CGIS_Import_DataDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGIS_Import_DataDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDD_DIALOG_SETTING_TAB_GIS_IMPORT, &CGIS_Import_DataDlg::OnTcnSelchangeSettingTab)
	ON_BN_CLICKED(IDCANCEL, &CGIS_Import_DataDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CGIS_Import_DataDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CGIS_Import_DataDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DATA_IN_EXCEL, &CGIS_Import_DataDlg::OnBnClickedButtonEditDataInExcel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CGIS_Import_DataDlg::OnBnClickedButtonDelete)
	ON_NOTIFY(NM_CLICK, IDD_DIALOG_SETTING_TAB_GIS_IMPORT, &CGIS_Import_DataDlg::OnNMClickSettingTab)
	ON_NOTIFY(TCN_SELCHANGING, IDD_DIALOG_SETTING_TAB_GIS_IMPORT, &CGIS_Import_DataDlg::OnTcnSelchangingSettingTab)
	ON_BN_CLICKED(IDOK2, &CGIS_Import_DataDlg::OnBnClickedOk2)
	ON_BN_CLICKED(ID_GIS_FIELD_NAME, &CGIS_Import_DataDlg::OnBnClickedGisFieldName)
END_MESSAGE_MAP()


// CGIS_Import_DataDlg message handlers

void CGIS_Import_DataDlg::OnBnClickedOk()
{

	// save data
	OnBnClickedOk2();

	OnOK();
}


BOOL CGIS_Import_DataDlg::ReadGIS_Import_DataCSVFile(const char* ElementType, std::vector<std::string>& name_vector,std::vector<std::vector<std::string>>& value_vector)
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

void CGIS_Import_DataDlg::OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
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

void CGIS_Import_DataDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CGIS_Import_DataDlg::OnBnClickedButtonAdd()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->AddRow();
}

void CGIS_Import_DataDlg::OnBnClickedButtonCopy()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->CopyRow();
}

void CGIS_Import_DataDlg::OnBnClickedButtonEditDataInExcel()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	std::string fileName = m_pDoc->m_ProjectDirectory + GIS_Import_Data_FileName[cur_tab] + ".csv";
	std::vector<std::string> value;
	m_pDoc->OpenCSVFileInExcel(fileName.c_str());
}

void CGIS_Import_DataDlg::OnBnClickedButtonDelete()
{
	int cur_tab = m_TabCtrl.GetCurSel();
	p_SubTabs[cur_tab]->DeleteRow();
}

void CGIS_Import_DataDlg::OnBnClickedCheckZoomToSelectedObject()
{

	UpdateData(1);
		for (int i=0; i < MAX_NUM_OF_GIS_IMPORT_DATA_FILES; i++)
	{
		p_SubTabs[i]->m_ZoomToSelectedObject = m_ZoomToSelectedObject;
	}
}

void CGIS_Import_DataDlg::OnNMClickSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
{

	*pResult = 0;
}

void CGIS_Import_DataDlg::OnTcnSelchangingSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
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

void CGIS_Import_DataDlg::OnBnClickedOk2()
{
	for (int i=0;i<MAX_NUM_OF_GIS_IMPORT_DATA_FILES;i++)
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
			std::ofstream outFile(m_pDoc->m_ProjectDirectory + GIS_Import_Data_FileName[i] + ".csv");

			if (outFile.is_open())
			{
				outFile << Str;
			}

			outFile.close();
		}
		else
		{
			remove(m_pDoc->m_ProjectDirectory + GIS_Import_Data_FileName[i] + ".csv");
		}
	}
}

void CGIS_Import_DataDlg::OnBnClickedGisFieldName()
{
	#ifndef _WIN64

	CString m_GIS_ShapeFile;
	 
	CString message_str;
	OGRRegisterAll();
	OGRDataSource       *poDS;

	poDS = OGRSFDriverRegistrar::Open(m_GIS_ShapeFile, FALSE );
	if( poDS == NULL )
	{
//		m_MessageList.AddString("Open file failed." );
		return;
	}

	ofstream CSVFile;
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("GIS Shape File|*.shp|"));
	if(dlg.DoModal() == IDOK)
	{
	CSVFile.open (dlg.GetPathName (), ios::out);

	if(CSVFile.is_open ()  == false)
	{
		AfxMessageBox("This file cannot be found or opened.\n It might be currently used and locked by EXCEL.");
		return;	 
	}else
	{
		CSVFile.width(15);
		CSVFile.precision(6) ;
		CSVFile.setf(ios::fixed);
	}

	int poLayers = ((OGRDataSource*)poDS)->GetLayerCount() ;
	for (int i=0; i < poLayers; i++) 
	{

		OGRLayer  *poLayer;

		poLayer = ((OGRDataSource*)poDS)->GetLayer(i);	

		if(poLayer == NULL)
		{
			return;			
		}

		OGRFeature *poFeature;
		int feature_count = 0;

		poLayer->ResetReading();

		while( (poFeature = poLayer->GetNextFeature()) != NULL )
		{
			OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
			int iField;

			if(feature_count == 0)    // first feature point, output field name;
			{
				for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
				{

					OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
					CString str = poFieldDefn->GetNameRef();
					str.Replace(" ", NULL);  // remove space
					CSVFile <<  str << "," ;

				}


			}
		}
	}
	}
#endif
}
