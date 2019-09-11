
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgLinkList.h"
#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"
#include "Dlg_VehicleClassification.h"

#include <string>
#include <sstream>


extern CDlgLinkList* g_pLinkListDlg;
// CDlgLinkList dialog


IMPLEMENT_DYNAMIC(CDlgLinkList, CDialog)

CDlgLinkList::CDlgLinkList(CWnd* pParent /*=NULL*/)
: CBaseDialog(CDlgLinkList::IDD, pParent)
, m_ZoomToSelectedLink(FALSE)
, m_StrDocTitles(_T(""))
{
	m_AVISensorFlag = false;

	m_pDoc = NULL;
	m_pDoc2 = NULL;
	m_bDoc2Ready = false;

}

CDlgLinkList::~CDlgLinkList()
{ 
	g_pLinkListDlg = NULL;
}

void CDlgLinkList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Check(pDX, IDC_CHECK_ZOOM_TO_SELECTED_LINK, m_ZoomToSelectedLink);
	DDX_Text(pDX, IDC_DOC_TITLE, m_StrDocTitles);
	DDX_Control(pDX, IDC_COMBO_Link_Selection, m_ComboBox);
	DDX_Control(pDX, IDC_COMBO_StartTime, m_StartHour);
	DDX_Control(pDX, IDC_COMBO_EndTime, m_EndHour);
}


BEGIN_MESSAGE_MAP(CDlgLinkList, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CDlgLinkList::OnLvnItemchangedList)
	ON_BN_CLICKED(IDOK, &CDlgLinkList::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgLinkList::OnBnClickedCancel)
	ON_COMMAND(ID_LINKSELECTION_SHOWALLLINKS, &CDlgLinkList::OnLinkselectionShowalllinks)
	ON_COMMAND(ID_LINKSELECTION_SHOWHIGHWAYLINKSONLY, &CDlgLinkList::OnLinkselectionShowhighwaylinksonly)
	ON_COMMAND(ID_LINKSELECTION_SHOWRAMPLINKSONLY, &CDlgLinkList::OnLinkselectionShowramplinksonly)
	ON_COMMAND(ID_LINKSELECTION_SHOWARTERIALLINKSONLY, &CDlgLinkList::OnLinkselectionShowarteriallinksonly)
	ON_COMMAND(ID_LINKSELECTION_SHOWALLLINKSEXCEPTCONNECTORS, &CDlgLinkList::OnLinkselectionShowalllinksexceptconnectors)
	ON_UPDATE_COMMAND_UI(ID_LINKSELECTION_SHOWALLLINKS, &CDlgLinkList::OnUpdateLinkselectionShowalllinks)
	ON_UPDATE_COMMAND_UI(ID_LINKSELECTION_SHOWHIGHWAYLINKSONLY, &CDlgLinkList::OnUpdateLinkselectionShowhighwaylinksonly)
	ON_UPDATE_COMMAND_UI(ID_LINKSELECTION_SHOWRAMPLINKSONLY, &CDlgLinkList::OnUpdateLinkselectionShowramplinksonly)
	ON_UPDATE_COMMAND_UI(ID_LINKSELECTION_SHOWARTERIALLINKSONLY, &CDlgLinkList::OnUpdateLinkselectionShowarteriallinksonly)
	ON_UPDATE_COMMAND_UI(ID_LINKSELECTION_SHOWALLLINKSEXCEPTCONNECTORS, &CDlgLinkList::OnUpdateLinkselectionShowalllinksexceptconnectors)
	ON_BN_CLICKED(IDC_CHECK_ZOOM_TO_SELECTED_LINK, &CDlgLinkList::OnBnClickedCheckZoomToSelectedLink)
	ON_BN_CLICKED(IDBARCHARTPIECHART, &CDlgLinkList::OnBnClickedBarchartpiechart)
	ON_COMMAND(ID_LINKSELECTION_SHOWSELECTEDLINKSONLY, &CDlgLinkList::OnLinkselectionShowselectedlinksonly)
	ON_UPDATE_COMMAND_UI(ID_LINKSELECTION_SHOWSELECTEDLINKSONLY, &CDlgLinkList::OnUpdateLinkselectionShowselectedlinksonly)
	ON_CBN_SELCHANGE(IDC_COMBO_Link_Selection, &CDlgLinkList::OnCbnSelchangeComboLinkSelection)
	ON_BN_CLICKED(ID_EXPORT, &CDlgLinkList::OnBnClickedExport)
	ON_CBN_SELCHANGE(IDC_COMBO_StartTime, &CDlgLinkList::OnCbnSelchangeComboStarttime)
	ON_CBN_SELCHANGE(IDC_COMBO_EndTime, &CDlgLinkList::OnCbnSelchangeComboEndtime)
	ON_STN_CLICKED(IDC_DOC_TITLE, &CDlgLinkList::OnStnClickedDocTitle)

END_MESSAGE_MAP()


// CDlgLinkList message handlers
#define LINKCOLUMNSIZE 11
#define LINKCOLUMNSIZE_AVI 7

BOOL CDlgLinkList::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_ComboBox.AddString ("All Links");
	m_ComboBox.AddString ("Selected Links");
	m_ComboBox.AddString ("Within Subarea");
	m_ComboBox.AddString ("Freeway");
	m_ComboBox.AddString ("Highway");
	m_ComboBox.AddString ("Arterial");
	m_ComboBox.AddString ("Ramp");
	m_ComboBox.AddString ("Connectors");
	m_ComboBox.AddString ("All Links Except Connectors");


	for (int i = 0; i<= 24; i++)
	{
		CString str;
		str.Format("%d",i);
	
		m_StartHour.AddString (str);
		m_EndHour.AddString (str);
	}

	m_StartHour.SetCurSel(0);
	m_EndHour.SetCurSel(24);

	m_ComboBox.SetCurSel (0);

	// Give better margin to editors
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);


	ReloadData();

	return true;
}
void CDlgLinkList::ReloadData()
{
	if(m_pDoc==NULL)
		return;

	CWaitCursor cursor;

	m_ListCtrl.DeleteAllItems();

	while(m_ListCtrl.GetColumnCount ()>0)
	{
		m_ListCtrl.DeleteColumn (0);
	}

	m_ListedLinkNoVector.clear();
	if(m_bDoc2Ready)
	{
		m_StrDocTitles.Format("1: %s; 2: %s", m_pDoc->m_ProjectTitle , m_pDoc2->m_ProjectTitle); 
		UpdateData(0);
	}

	std::vector<std::string> m_Column_names;

	m_Column_names.push_back ("No.");
	m_Column_names.push_back ("Link Name");
	m_Column_names.push_back ("From Node");
	m_Column_names.push_back ("To Node");

	if(m_pDoc->m_bUseMileVsKMFlag )
		m_Column_names.push_back ("Length");

	m_Column_names.push_back ("NumLanes");

	if(m_pDoc->m_bUseMileVsKMFlag )
		m_Column_names.push_back ("Speed Limit");
	else
		m_Column_names.push_back ("Speed Limit (km/h)");

	m_Column_names.push_back ("Lane Capacity");
	m_Column_names.push_back ("Link Type");

	m_Column_names.push_back ("VOC");
	m_Column_names.push_back ("LOS");
	m_Column_names.push_back ("Speed");
	m_Column_names.push_back ("Simu Total Volume");
	m_Column_names.push_back ("Simu Hourly Volume");
	m_Column_names.push_back ("Simu Hourly Lane Volume");

	m_Column_names.push_back ("Avg Simu Density");
	m_Column_names.push_back ("Max Simu Density");


	m_Column_names.push_back ("Waiting Time at Origin");
	m_Column_names.push_back ("AADT Conversion Factor");
	m_Column_names.push_back ("Simulated AADT");
	m_Column_names.push_back ("Observed AADT");
	m_Column_names.push_back ("Observed Peak Hourly Volume");


	m_Column_names.push_back ("Obs Volume");
	m_Column_names.push_back ("Volume Error");
	m_Column_names.push_back ("Error %");


	if(m_bDoc2Ready)
	{
		m_Column_names.push_back ("P2 SimuVolume");
		m_Column_names.push_back ("P2 AADT");
		m_Column_names.push_back ("P2 Speed");

		CString DiffStr;
		DiffStr.Format("Diff Volume: [P2,%s]-[P1,%s]", m_pDoc->m_ProjectTitle , m_pDoc2->m_ProjectTitle); 

		m_Column_names.push_back (m_pDoc->CString2StdString (DiffStr));
		m_Column_names.push_back ("Diff AADT");
		m_Column_names.push_back ("Diff Speed");
		m_Column_names.push_back ("Diff Total Travel time (hour)");
	}

	//Add Columns and set headers
	for (size_t i=0;i<m_Column_names.size();i++)
	{

		CGridColumnTraitText* pTrait = NULL;
		//		pTrait = new CGridColumnTraitEdit();
		m_ListCtrl.InsertColumnTrait((int)i,m_Column_names.at(i).c_str(),LVCFMT_LEFT,-1,-1, pTrait);
		m_ListCtrl.SetColumnWidth((int)i,LVSCW_AUTOSIZE_USEHEADER);
	}
	m_ListCtrl.SetColumnWidth(0, 80);

	int MOE_start_time_in_min = m_StartHour.GetCurSel()*60;
	int MOE_end_time_in_min =  m_EndHour.GetCurSel()*60;

	std::list<DTALink*>::iterator iLink;
	int i = 0;
	for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++, i++)
	{

		DTALink* pLink1 = (*iLink);
		int type  = pLink1 ->m_link_type ;
		if(m_LinkSelectionMode == eLinkSelection_SelectedLinksOnly) 
		{
			if(pLink1->m_DisplayLinkID <0)  // not selected
				continue; 
		}


		if(m_LinkSelectionMode == eLinkSelection_WithinSubarea) 
		{
			if(pLink1->m_bIncludedinSubarea == false)  // not within subarea
				continue; 
		}


		if(m_LinkSelectionMode == eLinkSelection_FreewayOnly) 
		{
			if(m_pDoc->m_LinkTypeMap[type ].IsFreeway () == false) 
				continue; 
		}



		if(m_LinkSelectionMode == eLinkSelection_HighwayOnly) 
		{
			if(m_pDoc->m_LinkTypeMap[type ].IsHighway () == false) 
				continue; 
		}
		if(m_LinkSelectionMode == eLinkSelection_RampOnly) 
		{
			if(m_pDoc->m_LinkTypeMap[type ].IsRamp () == false) 
				continue; 
		}

		if(m_LinkSelectionMode == eLinkSelection_ArterialOnly) 
		{
			if(m_pDoc->m_LinkTypeMap[type ].IsArterial () == false) 
				continue; 
		}

		if(m_LinkSelectionMode == eLinkSelection_NoConnectors) 
		{
			if(m_pDoc->m_LinkTypeMap[type ].IsConnector()) 
				continue; 
		}

		char text[100];
		sprintf_s(text, "%d",pLink1->m_LinkNo );
		int Index = m_ListCtrl.InsertItem(LVIF_TEXT,i,text , 0, 0, 0, NULL);
		int column_index = 1;
		m_ListCtrl.SetItemText(Index,column_index++,pLink1->m_Name .c_str ());

		sprintf_s(text, "%d",pLink1->m_FromNodeNumber);
		m_ListCtrl.SetItemText(Index,column_index++,text);

		sprintf_s(text, "%d",pLink1->m_ToNodeNumber);
		m_ListCtrl.SetItemText(Index,column_index++,text);

		if(m_pDoc->m_bUseMileVsKMFlag )
			sprintf_s(text, "%5.2f",pLink1->m_Length);
		else
			sprintf_s(text, "%5.2f",pLink1->m_Length*1.60934);

		m_ListCtrl.SetItemText(Index,column_index++,text);

		sprintf_s(text, "%d",pLink1->m_NumberOfLanes );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		if(m_pDoc->m_bUseMileVsKMFlag )
			sprintf_s(text, "%4.0f",pLink1->m_SpeedLimit  );
		else
			sprintf_s(text, "%4.0f",pLink1->m_SpeedLimit*1.60934  );

		m_ListCtrl.SetItemText(Index,column_index++,text);

		sprintf_s(text, "%4.0f",pLink1->m_LaneCapacity  );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		if(m_pDoc->m_LinkTypeMap.find(pLink1->m_link_type) != m_pDoc->m_LinkTypeMap.end())
		{
			sprintf_s(text, "%s", m_pDoc->m_LinkTypeMap[pLink1->m_link_type].link_type_name.c_str ());
			m_ListCtrl.SetItemText(Index,column_index,text);
		}

		column_index++;


		sprintf_s(text, "%5.2f",pLink1->m_volume_over_capacity_ratio    );
		m_ListCtrl.SetItemText(Index,column_index++,text);


		sprintf_s(text, "%c",pLink1->m_LevelOfService      );
		m_ListCtrl.SetItemText(Index,column_index++,text);

			if(m_pDoc->m_bUseMileVsKMFlag )
				sprintf_s(text, "%5.2f",pLink1->m_avg_simulated_speed    );
			else
				sprintf_s(text, "%5.2f",pLink1->m_avg_simulated_speed*1.60934   );

		m_ListCtrl.SetItemText(Index,column_index++,text);

		float max_volume , avg_volume, avg_lane_volume;

		avg_volume = pLink1->GetSimulationVolumeMOE(MOE_start_time_in_min,MOE_end_time_in_min,max_volume);

		sprintf_s(text, "%.0f",pLink1->m_total_link_volume     );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		sprintf_s(text, "%.0f",avg_volume    );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		sprintf_s(text, "%.0f",avg_volume/max(1,pLink1->m_NumberOfLanes) );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		float max_density , avg_density;

		avg_density = pLink1->GetSimulatedDensityMOE(MOE_start_time_in_min,MOE_end_time_in_min,max_density);

		sprintf_s(text, "%.1f",avg_density );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		sprintf_s(text, "%.1f",max_density );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		sprintf_s(text, "%.1f",pLink1->m_avg_waiting_time_on_loading_buffer       );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		sprintf_s(text, "%.2f",pLink1->m_AADT_conversion_factor       );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		sprintf_s(text, "%.0f",pLink1->m_simulated_AADT        );
		m_ListCtrl.SetItemText(Index,column_index++,text);


		sprintf_s(text, "%.0f",pLink1->m_total_sensor_link_volume        );
		m_ListCtrl.SetItemText(Index,column_index++,text);

		float error = 0;
		if( pLink1->m_ReferenceFlowVolume >=1)
			error = pLink1->m_total_link_volume - pLink1->m_total_sensor_link_volume ;
		sprintf_s(text, "%.0f", error);
		m_ListCtrl.SetItemText(Index,column_index++,text);

		float error_percentage = error / max(1,pLink1->m_total_sensor_link_volume)*100 ;
		sprintf_s(text, "%.1f", error_percentage);
		m_ListCtrl.SetItemText(Index,column_index++,text);

		if(m_bDoc2Ready)
		{
			DTALink* pLink2 = m_pDoc2->FindLinkWithNodeNumbers (pLink1->m_FromNodeNumber ,pLink1->m_ToNodeNumber );
			if(pLink2!=NULL)  // a link is found in the second document
			{
				sprintf_s(text, "%.0f",pLink2->m_total_link_volume        );
				m_ListCtrl.SetItemText(Index,column_index++,text);

				sprintf_s(text, "%.0f",pLink2->m_simulated_AADT        );
				m_ListCtrl.SetItemText(Index,column_index++,text);

		if(m_pDoc->m_bUseMileVsKMFlag )
				sprintf_s(text, "%5.2f",pLink2->m_avg_simulated_speed        );
		else
				sprintf_s(text, "%5.2f",pLink2->m_avg_simulated_speed*1.60934        );

				m_ListCtrl.SetItemText(Index,column_index++,text);
			// calculate difference
				sprintf_s(text, "%.0f",pLink2->m_total_link_volume  - pLink1->m_total_link_volume     );
				m_ListCtrl.SetItemText(Index,column_index++,text);

				sprintf_s(text, "%.0f",pLink2->m_simulated_AADT   - pLink1->m_simulated_AADT     );
				m_ListCtrl.SetItemText(Index,column_index++,text);

				sprintf_s(text, "%5.2f",pLink2->m_avg_simulated_speed   - pLink1->m_avg_simulated_speed    );
				m_ListCtrl.SetItemText(Index,column_index++,text);

				double total_travel_time = pLink2->m_total_link_volume* pLink2->m_Length / max (0.1, pLink2->m_avg_simulated_speed);
				double total_travel_time_2 = pLink1 ->m_total_link_volume*pLink1->m_Length / max (0.1, pLink1->m_avg_simulated_speed);
				sprintf_s(text, "%.2f",total_travel_time -  total_travel_time_2   );
				m_ListCtrl.SetItemText(Index,column_index++,text);

			}
		} // second document
		// 16


		m_ListedLinkNoVector.push_back(pLink1->m_LinkNo );
	}


}

void CDlgLinkList::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{

	UpdateData(1);

	// test valid documents
	if(g_TestValidDocument(m_pDoc)==false)
		return;

	if(m_bDoc2Ready)
	{
		if(g_TestValidDocument(m_pDoc2)==false)
			return;
	}

	// 
	m_pDoc->m_SelectedLinkNo = -1;

	if(m_bDoc2Ready)
		m_pDoc2->m_SelectedLinkNo = -1;

	g_ClearLinkSelectionList();

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
		int nSelectedRow = m_ListCtrl.GetNextSelectedItem(pos);
		char str[100];
		m_ListCtrl.GetItemText (nSelectedRow,0,str,20);
		int LinkNo = atoi(str);
		m_pDoc->m_SelectedLinkNo = LinkNo;

		if(m_pDoc2)
			m_pDoc2->m_SelectedLinkNo = LinkNo;
		DTALink* pLink = m_pDoc->m_LinkNoMap [LinkNo];

		g_AddLinkIntoSelectionList(pLink,LinkNo,m_pDoc->m_DocumentNo );

	}

	if(m_ZoomToSelectedLink == TRUE)
	{

		m_pDoc->ZoomToSelectedLink(m_pDoc->m_SelectedLinkNo);

		if(m_bDoc2Ready)
			m_pDoc2->ZoomToSelectedLink(m_pDoc->m_SelectedLinkNo);
	}

	Invalidate();

	m_pDoc->UpdateAllViews(0);

	if(m_bDoc2Ready)
		m_pDoc2->UpdateAllViews(0);

}

void CDlgLinkList::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

void CDlgLinkList::OnBnClickedCancel()
{
	CDialog::OnOK();
	g_bShowLinkList = false;

}


void CDlgLinkList::OnLinkselectionShowalllinks()
{
	m_LinkSelectionMode = eLinkSelection_AllLinks;
	ReloadData();
}


void CDlgLinkList::OnLinkselectionShowhighwaylinksonly()
{
	m_LinkSelectionMode = eLinkSelection_FreewayOnly;
	ReloadData();
}
void CDlgLinkList::OnLinkselectionShowramplinksonly()
{
	m_LinkSelectionMode = eLinkSelection_RampOnly;
	ReloadData();
}

void CDlgLinkList::OnLinkselectionShowarteriallinksonly()
{
	m_LinkSelectionMode = eLinkSelection_ArterialOnly;
	ReloadData();

}

void CDlgLinkList::OnLinkselectionShowalllinksexceptconnectors()
{
	m_LinkSelectionMode = eLinkSelection_NoConnectors;
	ReloadData();
}

void CDlgLinkList::OnUpdateLinkselectionShowalllinks(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkSelectionMode == eLinkSelection_AllLinks);
}

void CDlgLinkList::OnUpdateLinkselectionShowhighwaylinksonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkSelectionMode == eLinkSelection_FreewayOnly);
}

void CDlgLinkList::OnUpdateLinkselectionShowramplinksonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkSelectionMode == eLinkSelection_RampOnly);
}

void CDlgLinkList::OnUpdateLinkselectionShowarteriallinksonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkSelectionMode == eLinkSelection_ArterialOnly);
}

void CDlgLinkList::OnUpdateLinkselectionShowalllinksexceptconnectors(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkSelectionMode == eLinkSelection_NoConnectors);
}


void CDlgLinkList::OnBnClickedCheckZoomToSelectedLink()
{
	// TODO: Add your control notification handler code here
}

void CDlgLinkList::OnBnClickedBarchartpiechart()
{
	CDlg_VehicleClassification dlg;
	dlg.m_pDoc = m_pDoc;
	m_pDoc->m_VehicleSelectionMode = CLS_link_set;

	dlg.m_VehicleSelectionNo  = CLS_link_set;
	dlg.DoModal ();

}

void CDlgLinkList::OnLinkselectionShowselectedlinksonly()
{
	m_LinkSelectionMode = eLinkSelection_SelectedLinksOnly;
	ReloadData();

}

void CDlgLinkList::OnUpdateLinkselectionShowselectedlinksonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkSelectionMode == eLinkSelection_SelectedLinksOnly);
}

void CDlgLinkList::OnCbnSelchangeComboLinkSelection()
{

	m_LinkSelectionMode = (eLinkSelectionMode) m_ComboBox.GetCurSel();
	ReloadData();

}

void CDlgLinkList::OnBnClickedExport()
{

	if(m_pDoc->m_ProjectDirectory .GetLength () > 0)
	{
	CString fname;
	
		fname = m_pDoc->m_ProjectDirectory + "export_link_moe_list.csv"; 
		CWaitCursor wait;

		if(!ExportDataToCSVFile(fname))
		{ CString str;
			str.Format("The file %s could not be opened.\nPlease check if it is opened by Excel.", fname);
			AfxMessageBox(str);
		}else
		{
			m_pDoc->OpenCSVFileInExcel(fname);
		}
	}

}


bool CDlgLinkList::ExportDataToCSVFile(CString csv_file)
{
	FILE* st;
	fopen_s(&st,csv_file,"w");
	CWaitCursor wc;

	if(st!=NULL)
	{
		fprintf(st,"geometry,");  // for GIS data conversion
		for(int column = 0; column < m_ListCtrl.GetColumnCount (); column++)
		{
			fprintf(st,"%s,", m_ListCtrl.GetColumnHeading (column));
		}
		fprintf(st,"\n");  // for GIS data conversion

		for(int row = 0; row < m_ListCtrl.GetItemCount (); row++)
		{
			std::string  geo_string = "\"" + m_pDoc->m_LinkNoMap[m_ListedLinkNoVector[row]]->m_geo_string + "\"";

			fprintf(st,"%s,",geo_string.c_str ());

			for(int column = 0; column < m_ListCtrl.GetColumnCount (); column++)
			{
				char str[100];
				m_ListCtrl.GetItemText (row,column,str,100);

				fprintf(st,"%s,",str);
			}

			fprintf(st,"\n");

		}

		fclose(st);
		return true;
	}

	return false;
}


void CDlgLinkList::OnCbnSelchangeComboStarttime()
{

	if(m_StartHour.GetCurSel() >= m_EndHour.GetCurSel())
		m_EndHour.SetCurSel(min(24,m_StartHour.GetCurSel()+1));

	ReloadData();

}

void CDlgLinkList::OnCbnSelchangeComboEndtime()
{
	if(m_StartHour.GetCurSel() >= m_EndHour.GetCurSel())
		m_EndHour.SetCurSel(min(24,m_StartHour.GetCurSel()+1));

	ReloadData();
}

void CDlgLinkList::OnStnClickedDocTitle()
{
	// TODO: Add your control notification handler code here
}

