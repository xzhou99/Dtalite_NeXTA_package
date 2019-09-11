// DlgGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry.h"
#include "CSVParser.h"
#include "TLite.h"
#include "DlgGridCtrl.h"
#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"

#include "DlgZoneToNodeMapping.h"
#include "Dlg_ImportODDemand.h"

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using std::map;
using std::list;
using std::string;
using std::ofstream;
using std::ifstream;
using std::istringstream;

// CDlgODDemandGridCtrl dialog

IMPLEMENT_DYNAMIC(CDlgODDemandGridCtrl, CDialog)

BEGIN_MESSAGE_MAP(CDlgODDemandGridCtrl, CDialog)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_CTRL, &CDlgODDemandGridCtrl::OnGridEndEdit)
	ON_BN_CLICKED(ID_GRID_SAVEQUIT, &CDlgODDemandGridCtrl::OnBnClickedGridSavequit)
	ON_BN_CLICKED(ID_GRID_QUIT, &CDlgODDemandGridCtrl::OnBnClickedGridQuit)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgODDemandGridCtrl::OnBnClickedButton1)
	ON_BN_CLICKED(ID_GRID_SAVEQUIT2, &CDlgODDemandGridCtrl::OnBnClickedGridSavequit2)
	ON_BN_CLICKED(ID_EDIT_META_DATABASE, &CDlgODDemandGridCtrl::OnBnClickedEditMetaDatabase)
	ON_BN_CLICKED(ID_EDIT_META_DATABASE2, &CDlgODDemandGridCtrl::OnBnClickedEditMetaDatabase2)
	ON_BN_CLICKED(ID_EDIT_META_DATABASE3, &CDlgODDemandGridCtrl::OnBnClickedEditMetaDatabase3)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CDlgODDemandGridCtrl::OnHdnItemdblclickDemandtypelist)
	ON_BN_CLICKED(ID_EDIT_META_DATABASE4, &CDlgODDemandGridCtrl::OnBnClickedEditMetaDatabase4)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DemandFileLIST, &CDlgODDemandGridCtrl::OnLvnItemchangedDemandfilelist)
END_MESSAGE_MAP()



CDlgODDemandGridCtrl::CDlgODDemandGridCtrl(CWnd* pParent /*=NULL*/)
: CDialog(CDlgODDemandGridCtrl::IDD, pParent)
{
	m_bLoadDemandForLargeScaleNetwork = false;
	m_bSizeChanged = false;
	m_SelectedDemandMetaType = -1;
}

CDlgODDemandGridCtrl::~CDlgODDemandGridCtrl()
{
}

void CDlgODDemandGridCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_DemandFileLIST,m_DemandFileGrid);
	DDX_Control(pDX, IDC_DemandTypeLIST2, m_DemandTypeGrid);
}

void CDlgODDemandGridCtrl::DisplayDemandTypeTable()
{
	if(m_pDoc==NULL)
		return;

	std::vector<std::string> m_Column_names;

	static _TCHAR *_gColumnFileLabel[8] =
	{
		_T("No."), _T("File Name"), _T("Format Type"), _T("Loading Multiplier"), _T("Start Time (min)"),
		_T("End Time (min)"), _T("With Time Profile"),_T("Numer of Demand Types")};

		LV_COLUMN lvc;

		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

		//Add Columns and set headers
		for (size_t i=0;i<7;i++)
		{

			lvc.iSubItem = i;
			lvc.pszText = _gColumnFileLabel[i];
			lvc.cx = 120;
			lvc.fmt = LVCFMT_LEFT;
			m_DemandFileGrid.InsertColumn(i,&lvc);
		}


		m_DemandFileGrid.SetExtendedStyle(LVS_EX_AUTOSIZECOLUMNS | LVS_EX_FULLROWSELECT |LVS_EX_HEADERDRAGDROP);

		m_DemandFileGrid.SetColumnWidth(0, 80);

		int FirstDemandTypeIndex= -1;


		char file_name[_MAX_PATH];
		sprintf(file_name,"%s//input_demand_file_list.csv",m_pDoc->m_ProjectDirectory);

		CCSVParser parser;
		if (parser.OpenCSVFile( file_name))
		{
			int i=0;
			while(parser.ReadRecord())
			{
				char text[500];
				int file_sequence_no;
				string file_name;
				string format_type;
				int number_of_lines_to_be_skipped = 0;
				int subtotal_in_last_column = 0;
				float loading_multiplier =1;
				int start_time_in_min = 0; 
				int end_time_in_min = 1440;
				int number_of_demand_types = 0;
				float local_demand_loading_multiplier = 1;
				char demand_type_field_name[20] ;
				int demand_type_code[20]={0};

				int demand_format_flag = 0;

				if(parser.GetValueByFieldName("file_sequence_no",file_sequence_no)==false)
					break;


				if(file_sequence_no <=-1)  // skip negative sequence no 
					continue;

				int Index = m_DemandFileGrid.InsertItem(LVIF_TEXT,i,text , 0, 0, 0, NULL);

				sprintf_s(text, "%d",file_sequence_no);
				m_DemandFileGrid.SetItemText(Index,0,text);

				parser.GetValueByFieldName("file_name",file_name);
				m_DemandFileGrid.SetItemText(Index,1,file_name.c_str ());
				DemandFileNameVector.push_back (file_name.c_str ());

				parser.GetValueByFieldName("format_type",format_type);
				m_DemandFileGrid.SetItemText(Index,2,format_type.c_str ());

				parser.GetValueByFieldName("loading_multiplier",local_demand_loading_multiplier);
				sprintf_s(text, "%.3f",local_demand_loading_multiplier);

				m_DemandFileGrid.SetItemText(Index,3,text);


				parser.GetValueByFieldName("start_time_in_min",start_time_in_min);
				parser.GetValueByFieldName("end_time_in_min",end_time_in_min);

				// can be also enhanced to edit the real time information percentage

				m_DemandFileGrid.SetItemText(Index,4,m_pDoc->GetTimeStampString(start_time_in_min));
				m_DemandFileGrid.SetItemText(Index,5,m_pDoc->GetTimeStampString(end_time_in_min));

				int apply_additional_time_dependent_profile =0;	
				parser.GetValueByFieldName("apply_additional_time_dependent_profile",apply_additional_time_dependent_profile);

				if(apply_additional_time_dependent_profile)
					m_DemandFileGrid.SetItemText(Index,6,"Yes");
				else
					m_DemandFileGrid.SetItemText(Index,6,"No");


				parser.GetValueByFieldName("number_of_demand_types",number_of_demand_types);
				sprintf_s(text, "%d",number_of_demand_types);
				m_DemandFileGrid.SetItemText(Index,7,text);

			i++;
			}


			if(FirstDemandTypeIndex >= 0)
				m_DemandFileGrid.SetItemState(FirstDemandTypeIndex,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

		}else if(m_pDoc->m_ZoneMap.size()>0)  // no such file but with zones
		{
			//generate default meta database record
			char text[500];
				int file_sequence_no = 1;
				string file_name = "input_demand.csv";
				string format_type = "column";
				int number_of_lines_to_be_skipped = 0;
				int subtotal_in_last_column = 0;
				float loading_multiplier =1;
				int start_time_in_min = 480; 
				int end_time_in_min = 540;
				int number_of_demand_types = 1;
				float local_demand_loading_multiplier = 1;
				char demand_type_field_name[20];
				int demand_type_code[20]={0};

				int demand_format_flag = 0;

				int i = 0;
				int Index = m_DemandFileGrid.InsertItem(LVIF_TEXT,i,text , 0, 0, 0, NULL);

				sprintf_s(text, "%d",file_sequence_no);
				m_DemandFileGrid.SetItemText(Index,0,text);

				parser.GetValueByFieldName("file_name",file_name);
				DemandFileNameVector.push_back (file_name.c_str ());
				m_DemandFileGrid.SetItemText(Index,1,file_name.c_str ());

				parser.GetValueByFieldName("format_type",format_type);
				m_DemandFileGrid.SetItemText(Index,2,format_type.c_str ());

				parser.GetValueByFieldName("loading_multiplier",local_demand_loading_multiplier);
				sprintf_s(text, "%.3f",local_demand_loading_multiplier);

				m_DemandFileGrid.SetItemText(Index,3,text);


				parser.GetValueByFieldName("start_time_in_min",start_time_in_min);
				parser.GetValueByFieldName("end_time_in_min",end_time_in_min);

				// can be also enhanced to edit the real time information percentage

				m_DemandFileGrid.SetItemText(Index,4,m_pDoc->GetTimeStampString(start_time_in_min));
				m_DemandFileGrid.SetItemText(Index,5,m_pDoc->GetTimeStampString(end_time_in_min));

				int apply_additional_time_dependent_profile =0;	
				parser.GetValueByFieldName("apply_additional_time_dependent_profile",apply_additional_time_dependent_profile);

				if(apply_additional_time_dependent_profile)
					m_DemandFileGrid.SetItemText(Index,6,"Yes");
				else
					m_DemandFileGrid.SetItemText(Index,6,"No");


				parser.GetValueByFieldName("number_of_demand_types",number_of_demand_types);
				sprintf_s(text, "%d",number_of_demand_types);
				m_DemandFileGrid.SetItemText(Index,7,text);
		
		
		}

	static _TCHAR *_gColumnTypeLabel[5] =
	{
		_T("No."), _T("Demand Type"), _T("Avg VOT"), _T("% of Pretrip Info"),
		_T("% of Enroutetrip Info"),

	};

	//for(int vt = 0; vt < m_pDoc->m_VehicleTypeVector.size(); vt++)
	//{
	//	DTAVehicleType element = m_pDoc->m_VehicleTypeVector[vt];
	//	char str_vehicle_type[100];

	//	sprintf_s(str_vehicle_type,"%% of %s", element.vehicle_type_name );
	//	m_Column_names.push_back(str_vehicle_type);

	//}

	//Add Columns and set headers
	//Add Columns and set headers
	for (size_t i = 0; i<5; i++)
	{

		lvc.iSubItem = i;
		lvc.pszText = _gColumnTypeLabel[i];
		lvc.cx = 50;
		if (i >= 1)
			lvc.cx = 120;

		lvc.fmt = LVCFMT_LEFT;
		m_DemandTypeGrid.InsertColumn(i, &lvc);
	}

	m_DemandTypeGrid.SetExtendedStyle(LVS_EX_AUTOSIZECOLUMNS | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	m_DemandTypeGrid.SetColumnWidth(0, 80);
	//
	for (int type = m_pDoc->m_DemandTypeVector.size()-1; type >= 0; type--)
	{
		DTADemandType elment = m_pDoc->m_DemandTypeVector[type];

		// can be also enhanced to edit the real time information percentage
		char text[100];
		sprintf_s(text, "%d", elment.demand_type);
		int Index = m_DemandTypeGrid.InsertItem(LVIF_TEXT, 0, text, 0, 0, 0, NULL);

		sprintf_s(text, "%s", elment.demand_type_name);
		m_DemandTypeGrid.SetItemText(Index, 1, text);

		sprintf_s(text, "%5.2f", elment.average_VOT);
		m_DemandTypeGrid.SetItemText(Index, 2, text);

		sprintf_s(text, "%5.2f", elment.info_class_percentage[1]);
		m_DemandTypeGrid.SetItemText(Index, 3, text);

		sprintf_s(text, "%5.2f", elment.info_class_percentage[2]);
		m_DemandTypeGrid.SetItemText(Index, 4, text);

	}

}



void CDlgODDemandGridCtrl::DisplayDemandMatrix()
{
	return; 
	if(m_pDoc==NULL)
		return;

	bool bReadDemandSuccess = true;
	int demand_type =  m_SelectedDemandMetaType;

	m_ODMatrixGrid.SetRowCount(m_pDoc->m_ZoneMap.size() + 2);
	m_ODMatrixGrid.SetColumnCount(m_pDoc->m_ZoneMap.size() + 2);

	m_ODMatrixGrid.SetFixedColumnCount(1);
	m_ODMatrixGrid.SetFixedRowCount(1);

	GV_ITEM item;
	item.mask = GVIF_TEXT;
	item.nState  = GVIS_READONLY;

	CString str;

		std::map<int, DTAZone>	:: iterator itr_o;
		std::vector<int> ZoneNumberVector;
	// stage 1: determine initial zone number mapping
	for(itr_o = m_pDoc->m_ZoneMap.begin(); itr_o != m_pDoc->m_ZoneMap.end(); itr_o++)  // for each origin
	{

		ZoneNumberVector.push_back (itr_o->first);
	}

		for (int i=0;i<m_ODMatrixGrid.GetRowCount();i++) //Row
		{
			for (int j=0;j<m_ODMatrixGrid.GetColumnCount();j++) //Column
			{
				if (i==0 && j==0) 
				{
					continue;
				}
				else
				{
					if (i==0 && j != 0)
					{
						item.row = i;
						item.col = j;

							if(j== m_ODMatrixGrid.GetColumnCount()-1)
								str.Format("Subtotal");
							else
								str.Format(_T("%d"),ZoneNumberVector[j-1]);

						item.strText = str;

						m_ODMatrixGrid.SetItem(&item);					
					}
					else
					{
						if (i != 0 && j == 0)
						{
							item.row = i;
							item.col = j;

							if(i== m_ODMatrixGrid.GetRowCount()-1)
								str.Format("Subtotal");
							else
								str.Format(_T("%d"),ZoneNumberVector[i-1]);

							item.strText = str;

							m_ODMatrixGrid.SetItem(&item);		
						}
						else
						{
							item.row = i;
							item.col = j;

								if(i== m_ODMatrixGrid.GetRowCount()-1)
								{
									
								// special condition for subtotal from origin 
									if (j-1<ZoneNumberVector.size())
									{
									CString str_key;
									str_key.Format("d%d",ZoneNumberVector[j-1]);
										
									if(m_ODMatrixMap.find(str_key)!= m_ODMatrixMap.end())
										str.Format(_T("%.3f"),m_ODMatrixMap[str_key]);
									else 
										str.Format(_T("%.2f"),0.0f);
									}else
									{
									CString str_key;
									str_key.Format("total");
										
									if(m_ODMatrixMap.find(str_key)!= m_ODMatrixMap.end())
										str.Format(_T("%.3f"),m_ODMatrixMap[str_key]);
									else 
										str.Format(_T("%.2f"),0.0f);
									
									}

								}
								if(j== m_ODMatrixGrid.GetColumnCount()-1)
								{
									
								// special condition for subtotal from origin 
									if (i-1<ZoneNumberVector.size())
									{
									CString str_key;
									str_key.Format("o%d",ZoneNumberVector[i-1]);
										
									if(m_ODMatrixMap.find(str_key)!= m_ODMatrixMap.end())
										str.Format(_T("%.3f"),m_ODMatrixMap[str_key]);
									else 
										str.Format(_T("%.2f"),0.0f);
									}else
									{
									CString str_key;
									str_key.Format("total");
										
									if(m_ODMatrixMap.find(str_key)!= m_ODMatrixMap.end())
										str.Format(_T("%.3f"),m_ODMatrixMap[str_key]);
									else 
										str.Format(_T("%.2f"),0.0f);
									
									}
								}
								//else if (j== m_ODMatrixGrid.GetColumnCount()-1)
								//{
								//	// special condition for subtotal to  destination 
								//	str.Format(_T("%.3f"),m_ODMatrixMap[GetODKey(-1,ZoneNumberVector[i-1])]);
								//
								//}

							if (i<=ZoneNumberVector.size() && j<= ZoneNumberVector.size())
							{
								if (m_ODMatrixMap.find(GetODKey(ZoneNumberVector[i-1],ZoneNumberVector[j-1]))!= m_ODMatrixMap.end())
								{
									str.Format(_T("%.3f"),m_ODMatrixMap[GetODKey(ZoneNumberVector[i-1],ZoneNumberVector[j-1])]);
								}else
								{
								str.Format(_T("%.2f"),0.0f);
								}
							}

							item.strText = str;
							m_ODMatrixGrid.SetItem(&item);
							m_ODMatrixGrid.SetItemState(i,j, m_ODMatrixGrid.GetItemState(i,j) | GVIS_READONLY);

						} // neither i nor j is zero
					} // i is not zero
				} // Either i or j is not zero
			} // End for j
		} //End for i
Invalidate();
}

BOOL CDlgODDemandGridCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWaitCursor wait;
	bool bReadDemandSuccess = true;

	if (m_pDoc != NULL)
	{

		UpdateData(0);

		DisplayDemandTypeTable();
		DisplayDemandMatrix();

	} // End if ()
	else
	{
		m_ODMatrixGrid.SetRowCount(1);
		m_ODMatrixGrid.SetColumnCount(1);

		m_ODMatrixGrid.SetFixedColumnCount(1);
		m_ODMatrixGrid.SetFixedRowCount(1);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}



// CDlgODDemandGridCtrl message handlers

void CDlgODDemandGridCtrl::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CString content = m_ODMatrixGrid.GetItemText(pItem->iRow,pItem->iColumn);

	stringstream strstr;
	strstr << content;
	float flow;

	strstr >> flow;

	if (strstr.fail())
	{
		MessageBox("Illegal input for OD demand!");
		*pResult = -1;
		return;
	}

	if (flow < 0)
	{
		MessageBox("OD demand can not be negative!");
		*pResult = -1;
		return;
	}

	*pResult = 0;
}


void CDlgODDemandGridCtrl::OnBnClickedGridSavequit()
{

	OnOK();
}

inline bool ConvertToInt(std::istringstream& sstream, int& n)
{
	if ((sstream >> n).fail() || !sstream.eof()) //Convert fail or non-integer part exists
	{
		return false;
	}
	else
	{
		return true;
	}
}

inline bool ConvertToDouble(std::istringstream& sstream, double& value)
{

	if ((sstream >> value).fail() || !sstream.eof())
	{
		return false;
	}
	else
	{
		return true;
	}
}

inline void ResetStringStream(std::istringstream& sstream, std::string s)
{
	sstream.str(s);
	sstream.clear();
}


bool CDlgODDemandGridCtrl::SaveZoneCSVFileExt(LPCTSTR lpszFileName)
{

	return true;
}


bool CDlgODDemandGridCtrl::SaveDemandMatrix()
{

	if(m_pDoc==NULL)
		return false;

	int demand_type = 	m_SelectedDemandMetaType;

	if(demand_type < 1)
		return false;

	// step 1:
	// clear all demand element

	for (int i=1;i<m_ODMatrixGrid.GetRowCount();i++)
	{
		CString originStr = m_ODMatrixGrid.GetItemText(i,0);
		for (int j=1;j<m_ODMatrixGrid.GetColumnCount();j++)
		{
			CString destStr = m_ODMatrixGrid.GetItemText(0,j);
			CString flowStr = m_ODMatrixGrid.GetItemText(i,j);

			int origin_zone_id = atoi(originStr);
			int destination_zone_id = atoi(destStr);
			float number_of_vehicles = atoi(flowStr);
			m_pDoc->m_ZoneMap[origin_zone_id].m_ODDemandMatrix [destination_zone_id].SetValue (demand_type, number_of_vehicles);
		}
	}


	return true;
}

void CDlgODDemandGridCtrl::OnBnClickedGridQuit()
{
	OnCancel();
}
void CDlgODDemandGridCtrl::OnBnClickedButtonCreatezones()
{
	CDlg_ImportODDemand dlg;
	if(dlg.DoModal() == IDOK)
	{

		unsigned int number_of_zones = dlg.m_NumberOfZones;
		if (number_of_zones > m_pDoc->m_NodeSet.size())
		{
			CString errorMsg = "The number of zones are greater than the number of nodes!\nPlease re-inpute it.";
			MessageBox(errorMsg);
			return;
		}

		m_ODMatrixGrid.SetRowCount(m_pDoc->m_ODSize + 1);
		m_ODMatrixGrid.SetColumnCount(m_pDoc->m_ODSize + 1);

		m_ODMatrixGrid.SetFixedColumnCount(1);
		m_ODMatrixGrid.SetFixedRowCount(1);

		GV_ITEM item;
		item.mask = GVIF_TEXT;
		item.nState  = GVIS_READONLY;


		CString str;

		for (int i=0;i<m_ODMatrixGrid.GetRowCount()+1;i++) //Row // with subtotal
		{
			for (int j=0;j<m_ODMatrixGrid.GetColumnCount()+1;j++) //Column // with subtotal
			{
				if (i==0 && j==0) 
				{
					continue;
				}
				else
				{
					if (i==0 && j != 0)
					{
						item.row = i;
						item.col = j;


							if(j== m_ODMatrixGrid.GetColumnCount()-1)
								str.Format("Subtotal");
							else
								str.Format(_T("%d"),j);
						
						item.strText = str;

						m_ODMatrixGrid.SetItem(&item);					
					}
					else
					{
						if (i != 0 && j == 0)
						{
							item.row = i;
							item.col = j;

							if(i== m_ODMatrixGrid.GetRowCount()-1)
								str.Format("Subtotal");
							else
								str.Format(_T("%d"),i);

							item.strText = str;
							

							m_ODMatrixGrid.SetItem(&item);		
						}
						else
						{
							item.row = i;
							item.col = j;

							str.Format(_T("%.2f"),0.0f);
							item.strText = str;

							m_ODMatrixGrid.SetItem(&item);
						} // neither i nor j is zero
					} // i is not zero
				} // Either i or j is not zero
			} // End for j
		} //End for i

		SaveDemandMatrix();

		// write input_activity_location.csv
		// zone number , node number
		// make sure node number exists, if the node number does not exist, skip zone-node mapping pair
		// fill zero value for OD table
		// allow user to edit
	}

}

void CDlgODDemandGridCtrl::OnBnClickedButtonEditZoneNodeMapping()
{
	CDlgZoneToNodeMapping dlg;
	dlg.m_pDlg = this;
	dlg.DoModal();

}

void CDlgODDemandGridCtrl::OnBnClickedButton1()
{

}

void CDlgODDemandGridCtrl::OnBnClickedGridSavequit2()
{
	CWaitCursor wait;
	bool ret = SaveDemandMatrix();

	if (!ret)
	{
		AfxMessageBox("Save Demand file failed!");
	}
}



void CDlgODDemandGridCtrl::OnLvnItemchangedDemandtypelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

OnBnClickedButtonReload();
}

void CDlgODDemandGridCtrl::OnBnClickedEditMetaDatabase()
{
	char file_name[_MAX_PATH];
	sprintf(file_name,"%s//input_demand_file_list.csv",m_pDoc->m_ProjectDirectory);
	m_pDoc->OpenCSVFileInExcel (file_name);
}

void CDlgODDemandGridCtrl::LoadDemandMatrixFromDemandFile(int DemandFileSequenceNo, int SelectedDemandMetaType)
{

	DemandFileNameVector.clear();
	float total_demand_in_demand_file = 0;
	float total_number_of_vehicles_to_be_generated = 0;

	int max_line_number_for_logging = 10;

	float DemandLoadingStartTimeInMin = 1440;
	float DemandLoadingEndTimeInMin = 0;

	char meta_file_name[_MAX_PATH];
	sprintf(meta_file_name,"%s//input_demand_file_list.csv",m_pDoc->m_ProjectDirectory);

	m_ODMatrixMap.clear();

	CString SelectedFileName;

	CCSVParser parser;
	if (parser.OpenCSVFile(meta_file_name))
	{
		int demand_file_seq_no = 0;
		while (parser.ReadRecord())
		{
			int file_sequence_no;
			string file_name;
			string format_type;
			int number_of_lines_to_be_skipped = 0;
			int subtotal_in_last_column = 0;
			float loading_multiplier = 1;
			int start_time_in_min = 0;
			int end_time_in_min = 1440;
			int number_of_demand_types = 0;
			float local_demand_loading_multiplier = 1;
			char demand_type_field_name[20];
			int demand_type_code[20] = { 0 };

			int demand_format_flag = 0;

			if (parser.GetValueByFieldName("file_sequence_no", file_sequence_no) == false)
				break;

			if (file_sequence_no <= -1)  // skip negative sequence no 
				continue;


			parser.GetValueByFieldName("file_name", file_name);

			SelectedFileName = file_name.c_str();
			DemandFileNameVector.push_back(SelectedFileName);

			parser.GetValueByFieldName("start_time_in_min", start_time_in_min);
			parser.GetValueByFieldName("end_time_in_min", end_time_in_min);

			// set DemandLoadingStartTimeInMin according the start time and end time of each record
			if (DemandLoadingStartTimeInMin > start_time_in_min)
				DemandLoadingStartTimeInMin = start_time_in_min;

			if (DemandLoadingEndTimeInMin < end_time_in_min)
				DemandLoadingEndTimeInMin = end_time_in_min;

			parser.GetValueByFieldName("format_type", format_type);

			{ // error checking

				if (file_name.find("AMS_OD_table.csv") != string::npos && format_type.find("column") == string::npos)
				{
					AfxMessageBox("Please specify column format for demand file AMS_OD_table.csv");
				}

				if (file_name.find("demand.dat") != string::npos && format_type.find("dynasmart") == string::npos)
				{
					AfxMessageBox("Please specify dynasmart format for demand file demand.dat.");
				}

				if (file_name.find("demand_HOV.dat") != string::npos && format_type.find("dynasmart") == string::npos)
				{
					AfxMessageBox("Please specify dynasmart format for file demand_HOV.dat.");
				}

				if (file_name.find("demand_truck.dat") != string::npos && format_type.find("dynasmart") == string::npos)
				{
					cout << "Please specify dynasmart format for demand_truck file demand.dat, other than " << format_type << endl;
				}

			}

			parser.GetValueByFieldName("number_of_lines_to_be_skipped", number_of_lines_to_be_skipped);
			parser.GetValueByFieldName("subtotal_in_last_column", subtotal_in_last_column);

			int apply_additional_time_dependent_profile = 0;
			parser.GetValueByFieldName("apply_additional_time_dependent_profile", apply_additional_time_dependent_profile);
			parser.GetValueByFieldName("loading_multiplier", local_demand_loading_multiplier);

			double time_dependent_ratio[MAX_TIME_INTERVAL_SIZE] = { 0 };

			double total_ratio = 0;
			if (apply_additional_time_dependent_profile == 1)
			{
				for (int time_index = start_time_in_min / 15; time_index < end_time_in_min / 15; time_index++)  // / 15 converts min to 15-min interval for demand patterns
				{
					std::string time_stamp_str = m_pDoc->GetTimeStampStrFromIntervalNo(time_index, true);

					time_dependent_ratio[time_index] = 0;
					parser.GetValueByFieldName(time_stamp_str, time_dependent_ratio[time_index]);
					total_ratio += time_dependent_ratio[time_index];
				}


				if (total_ratio < 0.001)
				{
					cout << "The total temporal ratio read from file input_temporal_demand_profile.csv is 0, which means no demand will be loaded. " << endl;
					g_ProgramStop();
				}

			}

			parser.GetValueByFieldName("number_of_demand_types", number_of_demand_types);

			if (demand_file_seq_no != DemandFileSequenceNo)  // not the selected matrix 
			{

				demand_file_seq_no++;
				continue;
			}







		}
	}




	//			bool bFileReady = false;
	//			int i;

	//			FILE* st;
	//			fopen_s(&st,m_pDoc->m_ProjectDirectory + file_name.c_str (), "r");
	//			if (st!=NULL)
	//			{
	//				char  str_line[2000]; // input string
	//				int str_line_size;

	//				// skip lines
	//				for(int line_skip = 0 ;line_skip < number_of_lines_to_be_skipped;line_skip++)
	//				{
	//					g_read_a_line(st,str_line, str_line_size); //  skip the first line
	//					cout << str_line << endl;
	//				}

	//				bFileReady = true;
	//				int line_no = 0;

	//				while(true)
	//				{
	//					int origin_zone = g_read_integer(st);

	//					if(origin_zone <=0)
	//					{

	//						if(line_no==1 && !feof(st))  // read only one line, but has not reached the end of the line
	//						{
	//							AfxMessageBox("Error: Only one line has been read from file. Are there multiple columns of demand type in the file?");
	//							return;

	//						}
	//						break;
	//					}

	//					int destination_zone = g_read_integer(st);
	//					float number_of_vehicles ;


	//					for(int type = 1; type <= number_of_demand_types; type++)
	//					{

	//						float demand_value = g_read_float(st);
	//						if(m_pDoc->m_ZoneMap.find(origin_zone)!= m_pDoc->m_ZoneMap.end() && m_pDoc->m_ZoneMap.find(destination_zone)!= m_pDoc->m_ZoneMap.end())
	//							SetODMatrx(origin_zone,destination_zone,demand_value);

	//					}  // for each demand type
	//					//if(line_no >= max_line_number)
	//					//break;
	//					line_no++;
	//				}  // scan lines

	//				fclose(st);
	//			}else  //open file
	//			{
	//				AfxMessageBox("Error: The demand file cannot be opened.\n It might be currently used and locked by EXCEL.");
	//			}

	//		}else if (format_type.compare("matrix")== 0)
	//		{
	//			if (g_detect_if_a_file_is_column_format(m_pDoc->m_ProjectDirectory + file_name.c_str()) == true)
	//			{
	//				CString str;
	//				str.Format("Demand input file %s looks to be based on column format but the format_type=matrix in input_demand_file_list.csv.\nPlease check the demand file format, and change format_type=column in input_demand_meta_data.cv.", file_name.c_str());
	//				AfxMessageBox(str);
	//				return;
	//			
	//			}
	//			bool bFileReady = false;
	//			int i;

	//			std::vector<int> zone_sequence_vector;

	//			FILE* st;
	//			fopen_s(&st,m_pDoc->m_ProjectDirectory+ file_name.c_str (), "r");
	//			if (st!=NULL)
	//			{
	//				// read the first line
	//				for(int dest = 1; dest <= m_pDoc->m_ODSize; dest++)
	//				{
	//					int zone_number = g_read_float(st);
	//					zone_sequence_vector.push_back(zone_number);
	//				}

	//				int line_no = 0;
	//				for(int origin_zone = 1; origin_zone <= m_pDoc->m_ODSize; origin_zone++)
	//				{
	//					int origin_zone_id = g_read_float(st); // read the origin zone number

	//					for(int destination_zone = 1; destination_zone <= m_pDoc->m_ODSize; destination_zone++)
	//					{
	//						float number_of_vehicles =  g_read_float(st);

	//						int destination_zone_id = zone_sequence_vector[destination_zone-1];
	//						line_no++;

	//							SetODMatrx(origin_zone_id,destination_zone_id,number_of_vehicles);
	//					}
	//					//
	//					if(subtotal_in_last_column==1)
	//						g_read_float(st); //read sub total value

	//				}


	//				fclose(st);
	//			}else  //open file
	//			{
	//				AfxMessageBox("Error: The demand file cannot be opened.\n It might be currently used and locked by EXCEL.");

	//			}

	//		}else if(format_type.find("agent")!= string::npos)
	//		{
	//			AfxMessageBox("Please use Excel or text editor to open the agent csv file.");
	//			return;
	//		}else if(format_type.find("agent_bin")!= string::npos)
	//		{
	//			AfxMessageBox("Please use DTALite open the agent in file.");

	//			return;
	//		}else if (format_type.find("dynasmart")!= string::npos)
	//		{
	//			int type = 1;  // first demand type definition
	//			if(demand_type_code[type]>=1)  // feasible demand type
	//			{

	//				bool bFileReady = false;
	//				int i;

	//				FILE* st;
	//				fopen_s(&st,m_pDoc->m_ProjectDirectory+file_name.c_str (), "r");
	//				if (st!=NULL)
	//				{
	//					int num_zones =  m_pDoc->m_ODSize;
	//					int num_matrices = 0;

	//					num_matrices = g_read_integer(st);
	//					float demand_factor = g_read_float(st);

	//					std::vector<int> TimeIntevalVector;
	//					// Start times
	//					int i;
	//					for(i = 0; i < num_matrices; i++)
	//					{
	//						int start_time = g_read_float(st);
	//						TimeIntevalVector.push_back(start_time);

	//					}

	//					int time_interval = 60; // min

	//					if(TimeIntevalVector.size() >=2)
	//						time_interval = TimeIntevalVector[1] - TimeIntevalVector[0];

	//					// read the last value
	//					int end_of_simulation_horizon = g_read_float(st);

	//					TimeIntevalVector.push_back(end_of_simulation_horizon);

	//					long line_no = 2;
	//					float total_demand = 0;
	//					i =0;
	//					//		for(i = 0; i < num_matrices; i++)
	//					{
	//						// Find a line with non-blank values to start
	//						// Origins
	//						double DSP_start_time= g_read_float(st) + start_time_in_min; // start time
	//						double DSP_end_time= DSP_start_time + time_interval; // end time

	//						for(int origin_zone=1; origin_zone<= num_zones; origin_zone++)
	//							for(int destination_zone=1; destination_zone<= num_zones; destination_zone++)
	//							{
	//								float number_of_vehicles = g_read_float(st);
	//								SetODMatrx(origin_zone,destination_zone,number_of_vehicles);

	//							}

	//					} // time-dependent matrix
	//					fclose(st);
	//				}}else
	//				{

	//					AfxMessageBox("Error: The demand file cannot be opened.\n It might be currently used and locked by EXCEL.");

	//					//
	//				}

	//		}

	//		demand_file_seq_no++;
	//	}  // for each meta data record

	//} // open csv file for parser
	//	else if (m_pDoc->m_ZoneMap .size() > 0)
	//	{  //default matrix data (not in file yet)

	//		DemandFileNameVector.push_back ("input_demand.csv");

	//			int line_no = 0;
	//			std::map<int, DTAZone>	:: const_iterator itr;

	//				for(itr = m_pDoc->m_ZoneMap.begin(); itr != m_pDoc->m_ZoneMap.end(); itr++)
	//				{

	//				std::map<int, DTAZone>	:: const_iterator itr_to_zone_id;

	//					int index = 0;
	//					for(itr_to_zone_id =m_pDoc-> m_ZoneMap.begin(); itr_to_zone_id != m_pDoc->m_ZoneMap.end(); itr_to_zone_id++)
	//					{

	//						float value =m_pDoc-> GetODDemandValue(1,itr->first,itr_to_zone_id->first );
	//						
	//						SetODMatrx(itr->first,itr_to_zone_id->first,value);
	//		
	//					}
	//				}
	//	}
	//// cout << "Total demand volume = " << total_demand_in_demand_file << endl;

	// create vehicle heres...
	return;
}
void CDlgODDemandGridCtrl::OnLvnItemchangedDemandtypelist2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDlgODDemandGridCtrl::OnBnClickedEditMetaDatabase2()
{
	char file_name[_MAX_PATH];
	sprintf(file_name,"%s\\input_demand_type.csv",m_pDoc->m_ProjectDirectory);
	m_pDoc->OpenCSVFileInExcel (file_name);
}

void CDlgODDemandGridCtrl::OnBnClickedEditVehicleTypeFile()
{
	char file_name[_MAX_PATH];
	sprintf(file_name,"%s\\input_vehicle_type.csv",m_pDoc->m_ProjectDirectory);
	m_pDoc->OpenCSVFileInExcel (file_name);
}

void CDlgODDemandGridCtrl::OnBnClickedEditVehicleEmissionsFile()
{
	char file_name[_MAX_PATH];
	sprintf(file_name,"%s\\optional_vehicle_emission_rate.csv",m_pDoc->m_ProjectDirectory);
	m_pDoc->OpenCSVFileInExcel (file_name);
}

void CDlgODDemandGridCtrl::OnBnClickedEditMetaDatabase3()
{
	char file_name[_MAX_PATH];
	sprintf(file_name,"%s\\%s",m_pDoc->m_ProjectDirectory,m_SelectedFileName);
	m_pDoc->OpenCSVFileInExcel (file_name);

}

void CDlgODDemandGridCtrl::OnHdnItemdblclickDemandtypelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnLvnItemchangedDemandtypelist(pNMHDR,pResult);
}

void CDlgODDemandGridCtrl::OnLvnLinkClickedDemandtypelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	OnLvnItemchangedDemandtypelist(pNMHDR,pResult);
}

void CDlgODDemandGridCtrl::OnBnClickedButtonReload()
{
	CWaitCursor wait;
		m_SelectedDemandMetaType = -1;
	POSITION pos = m_DemandFileGrid.GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
		int nSelectedRow = m_DemandFileGrid.GetNextSelectedItem(pos);
		char str[100];
		m_DemandFileGrid.GetItemText (nSelectedRow,1,str,20);
		m_SelectedDemandMetaType = atoi(str);
		//		TRACE("Select %d\n",m_SelectedDemandMetaType);
		if(m_pDoc->m_ZoneMap .size() >=200)
		{
			if(m_bLoadDemandForLargeScaleNetwork == false)
			{
				if(AfxMessageBox("It takes a while to load the demand file for this large-scale network.\nDo you want to continue?",MB_YESNO|MB_ICONINFORMATION)==IDNO)
				{
					return;
				}

				m_bLoadDemandForLargeScaleNetwork = true;

			}
		}

		CWaitCursor wait;
		LoadDemandMatrixFromDemandFile(nSelectedRow,1);

		m_SelectedFileName =  "input_demand.csv";
		
		if(nSelectedRow < DemandFileNameVector .size())
			m_SelectedFileName = DemandFileNameVector[nSelectedRow];


		return;
	}
}

void CDlgODDemandGridCtrl::OnBnClickedButtonExportMatrix()
{
	FILE* st;

	CString AMS_File = m_pDoc->m_ProjectDirectory +"AMS_demand_matrix_format.csv";
	fopen_s(&st,AMS_File,"w");
			if(st!=NULL)
			{
				fprintf(st,"zone_id");
				std::map<int, DTAZone>	:: const_iterator itr;

				for(itr = m_pDoc->m_ZoneMap.begin(); itr != m_pDoc->m_ZoneMap.end(); itr++)
				{
					fprintf(st,",%d",itr->first );
				}


				fprintf(st,"\n");

				std::map<int, DTAZone>	:: const_iterator itr_to_zone_id;

				int index = 0;
				for(itr = m_pDoc->m_ZoneMap.begin(); itr != m_pDoc->m_ZoneMap.end(); itr++)
				{
					fprintf(st,"%d,",itr->first );

					for(itr_to_zone_id = m_pDoc->m_ZoneMap.begin(); itr_to_zone_id !=  m_pDoc->m_ZoneMap.end(); itr_to_zone_id++)
					{
						float value = GetODValue(itr->first,itr_to_zone_id->first );
						{
							fprintf(st,"%f,", value);
						}
					}

					fprintf(st,"\n");

				}


		fclose(st);
	}else
	{
		AfxMessageBox("File AMS_demand_matrix_format.csv cannot be opened.");

	}

		m_pDoc->OpenCSVFileInExcel(AMS_File);
}

void CDlgODDemandGridCtrl::OnBnClickedButtonExportColumn()
{

	FILE* st;

	CString AMS_File = m_pDoc->m_ProjectDirectory +"AMS_demand_3_column_format.csv";
	fopen_s(&st,AMS_File,"w");
	if(st!=NULL)
	{
		// first line 
		fprintf(st,"origin,destination,volume\n");

				std::map<int, DTAZone>	:: const_iterator itr;

				std::map<int, DTAZone>	:: const_iterator itr_to_zone_id;


				for(itr = m_pDoc->m_ZoneMap.begin(); itr != m_pDoc->m_ZoneMap.end(); itr++)
				{

					for(itr_to_zone_id = m_pDoc->m_ZoneMap.begin(); itr_to_zone_id !=  m_pDoc->m_ZoneMap.end(); itr_to_zone_id++)
					{
						float value = GetODValue(itr->first,itr_to_zone_id->first );
						{
							fprintf(st,"%d,%d,%f\n",itr->first,itr_to_zone_id->first,value );

						}
					}

				}


		fclose(st);
	}else
	{
		AfxMessageBox("File AMS_demand_3_column_format.csv cannot be opened.");

	}

		m_pDoc->OpenCSVFileInExcel(AMS_File);

}


void CDlgODDemandGridCtrl::OnBnClickedEditMetaDatabase4()
{

	char file_name[_MAX_PATH];
	sprintf(file_name, "%s//input_demand_type.csv", m_pDoc->m_ProjectDirectory);
	m_pDoc->OpenCSVFileInExcel(file_name);
}


void CDlgODDemandGridCtrl::OnLvnItemchangedDemandfilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CWaitCursor wait;
	m_SelectedDemandMetaType = -1;
	POSITION pos = m_DemandFileGrid.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int nSelectedRow = m_DemandFileGrid.GetNextSelectedItem(pos);
		char str[100];
		m_SelectedFileName = "input_demand.csv";

		if (nSelectedRow < DemandFileNameVector.size())
			m_SelectedFileName = DemandFileNameVector[nSelectedRow];
	}
}
