// DlgPathList.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "CSVParser.h"
#include "DlgPathList.h"
#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"
#include "Dlg_VehicleClassification.h"
#include "Dlg_UserInput.h"
#include <string>
#include <sstream>
#include <vector>

extern void g_SelectColorCode(CDC* pDC, int ColorCount);
extern float g_Simulation_Time_Stamp;
extern CPen s_PenSimulationClock;

extern void g_SelectSuperThickPenColor(CDC* pDC, int ColorCount);
extern void g_SelectThickPenColor(CDC* pDC, int ColorCount);
extern void g_SelectBrushColor(CDC* pDC, int ColorCount);


extern CDlgPathList* g_pPathListDlg;
// CDlgPathList dialog

IMPLEMENT_DYNAMIC(CDlgPathList, CDialog)

CDlgPathList::CDlgPathList(CWnd* pParent /*=NULL*/)
: CBaseDialog(CDlgPathList::IDD, pParent)
, m_StrPathMOE(_T(""))
{
	m_TimeLeft = 0;
	m_TimeRight = 1440;
	m_MOEAggregationIntervalInMin = 1;
}


CDlgPathList::~CDlgPathList()
{
	g_pPathListDlg = NULL;
}

void CDlgPathList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Check(pDX, IDC_CHECK_ZOOM_TO_SELECTED_LINK, m_ZoomToSelectedLink);
	DDX_Text(pDX, IDC_PATHMOE, m_StrPathMOE);
	DDX_Control(pDX, IDC_LIST1, m_PathList);
	DDX_Control(pDX, IDC_COMBO_StartHour, m_StartHour);
	DDX_Control(pDX, IDC_COMBO_EndHour, m_EndHour);
	DDX_Control(pDX, IDC_COMBO_AggIntrevalList, m_AggregationIntervalList);
	DDX_Control(pDX, IDC_COMBO_PLOT_TYPE, m_PlotType);
}


BEGIN_MESSAGE_MAP(CDlgPathList, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CDlgPathList::OnLvnItemchangedList)
	ON_BN_CLICKED(IDOK, &CDlgPathList::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgPathList::OnBnClickedCancel)
	ON_BN_CLICKED(ID_FIND_RANDOM_ROUTE, &CDlgPathList::OnBnClickedFindRandomRoute)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgPathList::OnCbnSelchangeCombo1)
	ON_COMMAND(ID_PATH_DATA_EXPORT_CSV, &CDlgPathList::OnPathDataExportCSV)
	ON_COMMAND(ID_DATA_IMPORT_CSV, &CDlgPathList::OnDataImportCsv)
	ON_BN_CLICKED(IDC_CHECK_ZOOM_TO_SELECTED_LINK, &CDlgPathList::OnBnClickedCheckZoomToSelectedLink)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgPathList::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDDATA_Analysis, &CDlgPathList::OnBnClickedDataAnalysis)
	ON_COMMAND(ID_DATA_GENERATESAMPLEINPUTPATHCSV, &CDlgPathList::OnDataGeneratesampleinputpathcsv)
	ON_COMMAND(ID_DATA_CLEANALLPATHS, &CDlgPathList::OnDataCleanallpaths)
	ON_CBN_SELCHANGE(IDC_COMBO_StartHour, &CDlgPathList::OnCbnSelchangeComboStarthour)
	ON_CBN_SELCHANGE(IDC_COMBO_EndHour, &CDlgPathList::OnCbnSelchangeComboEndhour)
	ON_CBN_SELCHANGE(IDC_COMBO_AggIntrevalList, &CDlgPathList::OnCbnSelchangeComboAggintrevallist)
	ON_CBN_SELCHANGE(IDC_COMBO_PLOT_TYPE, &CDlgPathList::OnCbnSelchangeComboPlotType)
	ON_BN_CLICKED(IDDATA_Analysis2, &CDlgPathList::OnBnClickedAnalysis2)
	ON_BN_CLICKED(IDDATA_FREEVAL_Analysis_Generate_File, &CDlgPathList::OnBnClickedFreevalAnalysisGenerateFile)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_CHANGELANECAPACITY, &CDlgPathList::OnChangeattributesforlinksalongpathChangelanecapacity)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_CHANGE, &CDlgPathList::OnChangeattributesforlinksalongpathChange)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_CHANGELINKTYPE, &CDlgPathList::OnChangeattributesforlinksalongpathChangelinktype)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_CHANGESPEEDLIMIT, &CDlgPathList::OnChangeattributesforlinksalongpathChangespeedlimit)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_CHANGESPEEDLIMIT_KMPH, &CDlgPathList::OnChangeattributesforlinksalongpathChangespeedlimitKmph)
		ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_CHANGEJAMDENSITY, &CDlgPathList::OnChangeattributesforlinksalongpathChangejamdensity)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_CHANGEBACKWAVESPEED, &CDlgPathList::OnChangeattributesforlinksalongpathChangebackwavespeed)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_CHANGESATURATIONFLOWRATE, &CDlgPathList::OnChangeattributesforlinksalongpathChangesaturationflowrate)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_CHANGEJAMDENSITY33625, &CDlgPathList::OnChangeattributesforlinksalongpathChangejamdensity33625)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_EFFECTIVEGREENTIME, &CDlgPathList::OnChangeattributesforlinksalongpathEffectivegreentime)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_DELETELINKSALONGPATH, &CDlgPathList::OnChangeattributesforlinksalongpathDeletelinksalongpath)
	ON_BN_CLICKED(IDDATA_DYNAMIC_Density_Contour, &CDlgPathList::OnBnClickedDynamicDensityContour)
	ON_BN_CLICKED(IDDATA_DYNAMIC_Speed_Contour, &CDlgPathList::OnBnClickedDynamicSpeedContour)
	ON_BN_CLICKED(IDDATA_DYNAMIC_Flow_Contour, &CDlgPathList::OnBnClickedDynamicFlowContour)
	ON_COMMAND(ID_DATA_SAVECURRENTPATH, &CDlgPathList::OnDataSavecurrentpath)
	ON_COMMAND(ID_DATA_EXPORTFREEVALSEGMENTFILE, &CDlgPathList::OnDataExportfreevalsegmentfile)
	ON_BN_CLICKED(IDC_BUTTON_GOOGLE_EARTH_KML, &CDlgPathList::OnBnClickedButtonGoogleEarthKml)
	ON_COMMAND(ID_DATA_DELETEEXISTINGPATHSININPUTPATHCSVFILE, &CDlgPathList::OnDataDeleteexistingpathsininputpathcsvfile)
	ON_COMMAND(ID_DATA_VIEWINPUTPATHFILEINEXCEL, &CDlgPathList::OnDataViewinputpathfileinexcel)
	ON_COMMAND(ID_DATA_ADDANEWPATH, &CDlgPathList::OnDataAddanewpath)
	ON_COMMAND(ID_DATA_CHANGEPATHNAMEFORSELECTEDPATH, &CDlgPathList::OnDataChangepathnameforselectedpath)
	ON_COMMAND(ID_DATA_DELETESELECTEDPATH, &CDlgPathList::OnDataDeleteselectedpath)
	ON_LBN_DBLCLK(IDC_LIST1, &CDlgPathList::OnLbnDblclkList1)
	ON_COMMAND(ID_DATA_GENERATEEMISSIONDETOURPLOTS, &CDlgPathList::OnDataGenerateemissiondetourplots)
	ON_COMMAND(ID_DATA_CO2CONTOURPLOT, &CDlgPathList::OnDataCo2contourplot)
	ON_COMMAND(ID_DATA_COCONTOURPLOT, &CDlgPathList::OnDataCocontourplot)
	ON_COMMAND(ID_CONTOUR_NOXCONTOURPLOT, &CDlgPathList::OnContourNoxcontourplot)
	ON_COMMAND(ID_CONTOUR_HCCONTOURPLOT, &CDlgPathList::OnContourHccontourplot)
	ON_COMMAND(ID_DATA_GENERATEENERGYUSE, &CDlgPathList::OnDataGenerateenergyuse)
	ON_BN_CLICKED(ID_PATH_DATA_EXPORT_CSV, &CDlgPathList::OnBnClickedPathDataExportCsv)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_LOOPCODE, &CDlgPathList::OnChangeattributesforlinksalongpathLoopcode)
	ON_COMMAND(ID_CHANGEATTRIBUTESFORLINKSALONGPATH_DIRECTIONCODE, &CDlgPathList::OnChangeattributesforlinksalongpathDirectioncode)
END_MESSAGE_MAP()


// CDlgPathList message handlers
#define PATHDATACOLUMNSIZE 14

BOOL CDlgPathList::OnInitDialog()
{

	CDialog::OnInitDialog();

	m_PlotType.AddString ("Simulated Travel Time (min)");
	m_PlotType.AddString ("Simulated and Observed Travel Time (min)");
	m_PlotType.AddString ("Energy (KJ)");
	m_PlotType.AddString ("CO2 (g)");
	m_PlotType.AddString ("NOX (g)");
	m_PlotType.AddString ("CO (g)");
	m_PlotType.AddString ("HC (g)");
	//m_PlotType.AddString ("Gasline (Gallon)");
	//m_PlotType.AddString ("Miles Per Gallon");

	m_PlotType.SetCurSel (0);

	m_TimeLeft = int(m_pDoc->m_SimulationStartTime_in_min/30)*30 ;
	m_TimeRight = int(m_pDoc->m_SimulationEndTime_in_min/30)*30;

	for (int i = 0; i<= 24; i++)
	{
		CString str;
		str.Format("%d",i);

		m_StartHour.AddString (str);
		m_EndHour.AddString (str);
	}

	m_StartHour.SetCurSel(m_TimeLeft/60);
	m_EndHour.SetCurSel(min(24,m_TimeRight/60+1));

	m_AggregationValueVector.push_back(1);
	m_AggregationValueVector.push_back(5);
	m_AggregationValueVector.push_back(15);
	m_AggregationValueVector.push_back(30);
	m_AggregationValueVector.push_back(60);
	m_AggregationValueVector.push_back(120);
	m_AggregationValueVector.push_back(1440);

	for(unsigned int i = 0;  i< m_AggregationValueVector.size (); i++)
	{
		CString str;
		str.Format("%d min",m_AggregationValueVector[i]);

		m_AggregationIntervalList.AddString (str);

		if(g_MOEAggregationIntervalInMin  == m_AggregationValueVector[i])
		{
			m_AggregationIntervalList.SetCurSel (i);

		}

	}



	// Give better margin to editors
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);



	std::vector<std::string> m_Column_names;

	_TCHAR *ColumnMOELabel[PATHDATACOLUMNSIZE] =
	{
		_T("No."),_T("From->To"),_T("Street Name"), _T("Length"), _T("Speed Limit"), _T("Free-flow Travel Time (min)"),_T("# of lanes"),_T("Lane Saturation Flow Rate"),_T("Lane Capacity"), _T("Link Type"), _T("Sensor Type"),  _T("Count") 
	};


	//Add Columns and set headers
	for (size_t i=0;i< PATHDATACOLUMNSIZE;i++)
	{
		CGridColumnTrait* pTrait = NULL;
		//		pTrait = new CGridColumnTraitEdit();
		m_ListCtrl.InsertColumnTrait((int)i,ColumnMOELabel[i],LVCFMT_LEFT,-1,-1, pTrait);
		m_ListCtrl.SetColumnWidth((int)i,LVSCW_AUTOSIZE_USEHEADER);
	}
	m_ListCtrl.SetColumnWidth(0, 80);

	ReloadData();

	return true;
}
void CDlgPathList::ReloadData()
{
	CWaitCursor cursor;

	m_PathList.ResetContent ();

	for(unsigned int i= 0; i< m_pDoc->m_PathDisplayList.size(); i++)
	{
		CString str;

		if(m_pDoc->m_PathDisplayList[i].m_LinkVector .size() ==0 )
			str.Format("Path No.%d: %s, %d link ",i+1, m_pDoc->m_PathDisplayList[i].m_path_name.c_str (), m_pDoc->m_PathDisplayList[i].m_LinkVector .size());
		else
			str.Format("Path No.%d: %s, %d links ",i+1, m_pDoc->m_PathDisplayList[i].m_path_name.c_str (), m_pDoc->m_PathDisplayList[i].m_LinkVector .size());

		m_PathList.AddString (str);

	}

	if(m_pDoc->m_SelectPathNo>=0 && m_pDoc->m_SelectPathNo< m_PathList.GetCount ())
	{
		m_PathList.SetCurSel(m_pDoc->m_SelectPathNo);
	}

	m_ListCtrl.DeleteAllItems();

	if(m_pDoc->m_PathDisplayList.size() > m_pDoc->m_SelectPathNo && m_pDoc->m_SelectPathNo!=-1)
	{
		float total_distance = 0;
		float total_travel_time = 0;
		int number_of_sensors = 0;
		float total_count = 0;

		for (int i=0 ; i<m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_LinkVector.size(); i++)
		{

			int column_count  = 1;
			DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_LinkVector[i]];
			char text[100];


			sprintf_s(text, "%d",i+1);
			int Index = m_ListCtrl.InsertItem(LVIF_TEXT,i,text , 0, 0, 0, NULL);

			// from -> to
			sprintf_s(text, "%d->%d",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );
			m_ListCtrl.SetItemText(Index,column_count++,text );

			// street name
			m_ListCtrl.SetItemText(Index,column_count++,pLink->m_Name.c_str () );

			//length
			sprintf_s(text, "%5.3f",pLink->m_Length);
			m_ListCtrl.SetItemText(Index,column_count++,text);
			total_distance+= pLink->m_Length;

			//speed limit
			sprintf_s(text, "%4.0f",pLink->m_SpeedLimit  );
			m_ListCtrl.SetItemText(Index,column_count++,text);

			//free flow travel time
			sprintf_s(text, "%4.2f",pLink->m_FreeFlowTravelTime);
			m_ListCtrl.SetItemText(Index,column_count++,text);
			total_travel_time+=pLink->m_FreeFlowTravelTime;

			//# of lanes
			sprintf_s(text, "%d",pLink->m_NumberOfLanes );
			m_ListCtrl.SetItemText(Index,column_count++,text);

			//saturation flow rate
			sprintf_s(text, "%4.2f",pLink->m_Saturation_flow_rate_in_vhc_per_hour_per_lane );
			m_ListCtrl.SetItemText(Index,column_count++,text);

			//capacity
			sprintf_s(text, "%4.2f",pLink->m_LaneCapacity);
			m_ListCtrl.SetItemText(Index,column_count++,text);

			if(m_pDoc->m_LinkTypeMap.find(pLink->m_link_type) != m_pDoc->m_LinkTypeMap.end())
			{

				if(m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.size()>=1)
				{
					sprintf_s(text, "%s", m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.c_str ());
					m_ListCtrl.SetItemText(Index,column_count++,text);
				}
				else
				{
					sprintf_s(text, "%s", m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.c_str ());
					m_ListCtrl.SetItemText(Index,column_count++,text);
				}

			}

			if(pLink->m_bSensorData == true)
			{
				sprintf_s(text, "%s",pLink->m_SensorTypeString .c_str ());
				number_of_sensors += 1;
			}else
				sprintf_s(text, "");

			m_ListCtrl.SetItemText(Index,column_count++,text);


			if(pLink->m_bSensorData == true)
			{
				sprintf_s(text, "%.3f",pLink->GetSensorLaneVolume(g_Simulation_Time_Stamp));
				total_count+= pLink->GetSensorLaneVolume(g_Simulation_Time_Stamp);
			}else
				sprintf_s(text, "");

			m_ListCtrl.SetItemText(Index,column_count++,text);

		}


		if(m_PlotType.GetCurSel ()==0)
		{
			m_StrPathMOE.Format("Distance=%4.2f mi, Free-flow Travel Time=%4.2f min",
				total_distance,total_travel_time);
		}


		if(m_PlotType.GetCurSel ()==1)  // with sensor data
		{


			float avg_error =  m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].GetErrorStatistics(m_TimeLeft, m_TimeRight, m_MOEAggregationIntervalInMin);
			float avg_percentage_error = m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].GetRelativeErrorStatistics(m_TimeLeft, m_TimeRight, m_MOEAggregationIntervalInMin);

			m_StrPathMOE.Format("Distance=%4.2f mile, Free flow travel Time=%4.2f min, Avg abs error= %.2f (min), Avg perc error=%.1f%%",
				total_distance,total_travel_time, avg_error,avg_percentage_error);
		}


		UpdateData(0);
	}



}

void CDlgPathList::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{

	UpdateData(1);

	m_pDoc->m_SelectedLinkNo = -1;
	g_ClearLinkSelectionList();

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
		int nSelectedRow = m_ListCtrl.GetNextSelectedItem(pos);
		char str[100];
		m_ListCtrl.GetItemText (nSelectedRow,2,str,20);
		int LinkNo = atoi(str);
		m_pDoc->m_SelectedLinkNo = LinkNo;
		DTALink* pLink = m_pDoc->m_LinkNoMap [LinkNo];

		g_AddLinkIntoSelectionList(pLink, LinkNo, m_pDoc->m_DocumentNo );

	}
	if(m_ZoomToSelectedLink == true)
		m_pDoc->ZoomToSelectedLink(m_pDoc->m_SelectedLinkNo);

	Invalidate();

	m_pDoc->UpdateAllViews(0);

}

void CDlgPathList::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

void CDlgPathList::OnBnClickedCancel()
{
	CDialog::OnOK();
}



void CDlgPathList::OnBnClickedFindAlternative()
{
	// TODO: Add your control notification handler code here
}

void CDlgPathList::OnBnClickedFindRandomRoute()
{
	m_pDoc->m_RandomRoutingCoefficient = m_ComboRandomCoef.GetCurSel ()*0.2f;
	m_pDoc->Routing(false);
	m_pDoc->UpdateAllViews(NULL);

	ReloadData();
	Invalidate();


}

void CDlgPathList::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}

void CDlgPathList::OnBnClickedExportRoute()
{
	// TODO: Add your control notification handler code here
}

void CDlgPathList::OnPathDataExportCSV()
{
	// calculate time-dependent travel time
	int time_step = 1;

	CString export_file_name;

	export_file_name = m_pDoc->m_ProjectDirectory +"export_path_MOE.csv";
	// save demand here

	FILE* st;
	fopen_s(&st,export_file_name,"w");
	if(st!=NULL)
	{
		fprintf(st,"Summary\n");
		fprintf(st,"path index,path_name,distance,free_flow_travel_time(min),path_name,avg_simulated_travel_time(min),avg_sensor_travel_time(min),percentage_difference,avg_time-dependent_error(min),avg_time-dependent_percentage_error (%)\n");
		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

			float avg_simulated_travel_time = path_element.GetTimeDependentMOE (m_TimeLeft,0,m_TimeRight-m_TimeLeft);
			float avg_sensor_travel_time = path_element.GetTimeDependentMOE (m_TimeLeft,1,m_TimeRight-m_TimeLeft);

			fprintf(st,"\nPath %d,%s,%.2f,%.2f,%s,%.2f,%.2f,%.1f,%.2f,%.2f",
				p+1,path_element.m_path_name .c_str (), path_element.total_distance,path_element.total_free_flow_travel_time,
				path_element.m_path_name .c_str (),
				avg_simulated_travel_time,avg_sensor_travel_time, avg_simulated_travel_time/max(0.1,avg_simulated_travel_time)*100,
				path_element.GetErrorStatistics (m_TimeLeft,m_TimeRight,15), 
				path_element.GetRelativeErrorStatistics  (m_TimeLeft,m_TimeRight,15));
		}		


		fprintf(st,"\n\nPart I,time-dependent travel time");
		// 

		time_step= 15;


		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

			fprintf(st,"\nTime,,,");

			for(int t = m_TimeLeft ; t< m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%s,", m_pDoc->GetTimeStampString24HourFormat (t));
			}

			fprintf(st,"\nPath %d, %s,simulated travel time,",p+1,path_element.m_path_name .c_str ());

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentTravelTime[t]);

			}  // for each time


			if(path_element.m_bWithSensorTravelTime  == true);
			{
				fprintf(st,"\n,,observed travel time,");

				for(int t = m_TimeLeft ; t< m_TimeRight; t+= time_step)  // for each starting time
				{
					fprintf(st,"%.2f,", path_element.m_SensorTimeDependentTravelTime[t]);
				}

				//fprintf(st,"\nRelative Difference,,",p+1,path_element.m_path_name .c_str ());

				//	for(int t = m_TimeLeft ; t< m_TimeRight; t+= time_step)  // for each starting time
				//	{
				//		if(path_element.m_SensorTimeDependentTravelTime[t] >0.001f)
				//							fprintf(st,"%.2f,", path_element.m_TimeDependentTravelTime[t]/max(0.001,path_element.m_SensorTimeDependentTravelTime[t]));
				//		else //no data
				//							fprintf(st,",");

				//	}

				fprintf(st,"\n");

			}


		}

		// travel time index


		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];
			float total_free_flow_travel_time = 0;
			for (int i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				total_free_flow_travel_time += pLink->m_FreeFlowTravelTime ;
			}
			m_pDoc->m_PathDisplayList[p].total_free_flow_travel_time = total_free_flow_travel_time;
		}

		fprintf(st,"\n\nTravel Time Tndex,,");

		for(int t = m_TimeLeft ; t< m_TimeRight; t+= time_step)  // for each starting time
		{
			fprintf(st,"%s,", m_pDoc->GetTimeStampString24HourFormat (t));
		}


		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];


			fprintf(st,"\nPath %d, %s,",p+1, path_element.m_path_name .c_str ());
			for(int t = m_TimeLeft ; t< m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentTravelTime[t]/max(1,path_element.total_free_flow_travel_time));

			}  // for each time

		}

		fprintf(st,"\n\nPart II,link sequence\n\n");

		fprintf(st, "path_id,path_node_sequence\n");


		for (unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];
			fprintf(st, "%d,", p+1);

			for (int i = 0; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if (pLink != NULL)
				{
					if (i == 0)
					{
						fprintf(st, "%d;", pLink->m_FromNodeNumber);
					}
					fprintf(st, "%d;", pLink->m_ToNodeNumber);
				}

			}
			fprintf(st, ",\n");
		} //for each path

		fprintf(st, "\npath_id,link_sequence_no,from_node_id->to_node_id,from_node_id,to_node_id,name,length,speed_limit,free-flow travel_time,# of lanes,Lane Saturation Flow Rate,Lane Capacity,Link Type\n");

		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];
			for (int i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink != NULL)
				{

					fprintf(st,"%d,%d,\"[%d,%d]\",%d,%d,%s,%5.3f,%5.0f,%5.3f,%d,%5.0f,%5.1f,",
						p+1,i+1,pLink->m_FromNodeNumber , pLink->m_ToNodeNumber, pLink->m_FromNodeNumber , pLink->m_ToNodeNumber,   pLink->m_Name.c_str (), pLink->m_Length ,
						pLink->m_SpeedLimit, pLink->m_FreeFlowTravelTime , pLink->m_NumberOfLanes,  pLink->m_Saturation_flow_rate_in_vhc_per_hour_per_lane ,pLink->m_LaneCapacity );

					if(m_pDoc->m_LinkTypeMap.find(pLink->m_link_type) != m_pDoc->m_LinkTypeMap.end())
					{
						fprintf(st, "%s", m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.c_str ());
					}
				}

				fprintf(st,"\n");


			}
		} //for each path

		// part II: time-dependent speed contour
		int step_size = 1;

		for(int aggregation_index = 0; aggregation_index<4; aggregation_index++)
		{
			switch (aggregation_index)
			{
			case 0: step_size = 1; break;
			case 1: step_size = 5; break;
			case 2: step_size = 15; break;
			case 3: step_size = 30; break;

			default: step_size = 30;
			}

			int previous_MOEAggregationIntervalInMin = g_MOEAggregationIntervalInMin;
			g_MOEAggregationIntervalInMin = step_size;

			fprintf(st,"path_id,link_sequence_no,from_node_id->to_node_id,name,length,speed_limit,free-flow travel_time,# of lanes,Lane Saturation Flow Rate,Lane Capacity,Link Type\n");

			for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
			{
				DTAPath path_element = m_pDoc->m_PathDisplayList[p];

				fprintf(st,"\n\nPart III,time-dependent speed contour for path %d, %s,(%d min)\n\n", p+1, path_element.m_path_name .c_str (),step_size);


					fprintf(st,",,,");

				for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
				{
					fprintf(st,"%s,", m_pDoc->GetTimeStampString24HourFormat (t));
				}

					fprintf(st,"\n");

				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];


					if(pLink != NULL)
					{

						for(int s = 0; s< pLink->m_Length / 0.2; s++)
						{
							 
							CString label = pLink->m_Name .c_str ();

							if(pLink->m_Name  == "(null)")
							{
							label.Format ("%d->%d",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber);
							}

							fprintf(st,"%d->%d,%s,%s,", pLink->m_FromNodeNumber , pLink->m_ToNodeNumber , m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.c_str (),label);

							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{

								fprintf(st, "%.1f,", pLink->GetSimulatedSpeed (t), m_pDoc->m_PrimaryDataSource);

							}
							fprintf(st,"\n");
						}

					}


				}

				fprintf(st,"\n\n");

				fprintf(st,"\n\nPart III,time-dependent density contour for path %d, %s,(%d min)\n\n", p+1, path_element.m_path_name .c_str (),step_size);

					fprintf(st,",,,");
				for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
				{
					fprintf(st,"%s,", m_pDoc->GetTimeStampString24HourFormat (t));
				}

					fprintf(st,"\n");

				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];


					if(pLink != NULL)
					{

						for(int s = 0; s< pLink->m_Length / 0.2; s++)
						{
							 
							CString label = pLink->m_Name .c_str ();

							if(pLink->m_Name  == "(null)")
							{
							label.Format ("%d->%d",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber);
							}

							fprintf(st,"%d->%d,%s,%s,", pLink->m_FromNodeNumber , pLink->m_ToNodeNumber , m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.c_str (),label);

							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{

								fprintf(st, "%.1f,", pLink->GetSimulatedDensity (t));

							}
							fprintf(st,"\n");
						}

					}


				}

				// time axis
				fprintf(st,",,,");
				fprintf(st,"\n");


			} //for each path

			g_MOEAggregationIntervalInMin = previous_MOEAggregationIntervalInMin;
		}


		fprintf(st,"\n\nPart IV");
		fprintf(st,"\n\npath_no,departure time,simulated_travel_time_in_min,observed_travel_time_in_min,travel_time_index,total observed count on passing links\n");
		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

			for(int t = m_TimeLeft ; t< m_TimeRight; t+= time_step)  // for each starting time
			{
				path_element.m_TimeDependentCount[t] = 0;
				path_element.m_TimeDependentTravelTime[t] = t;  // t is the departure time

				for (int i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					if(pLink == NULL)
						break;

					path_element.m_TimeDependentTravelTime[t] += pLink->GetDynamicTravelTime(path_element.m_TimeDependentTravelTime[t],m_pDoc->m_PrimaryDataSource );

					if(i==0)// first link
						path_element.m_TimeDependentCount[t] +=  pLink->GetSensorLaneVolume(t);
					else
						path_element.m_TimeDependentCount[t] +=  pLink->GetSensorLaneVolume(path_element.m_TimeDependentTravelTime[t]);


					// current arrival time at a link/node along the path, t in [t] is still index of departure time, t has a dimension of 0 to 1440* number of days

					//			    TRACE("\n path %d, time at %f, TT = %f",p, path_element.m_TimeDependentTravelTime[t], pLink->GetDynamicTravelTime(path_element.m_TimeDependentTravelTime[t]) );

				}

				path_element.m_TimeDependentTravelTime[t] -= t; // remove the starting time, so we have pure travel time;

				fprintf(st,"%d,%s,%.2f,%.2f,%.2f,%.2f\n",p+1, m_pDoc->GetTimeStampString24HourFormat(t),
					path_element.GetTimeDependentMOE (t,0,15),path_element.GetTimeDependentMOE (t,1,1),
					path_element.m_TimeDependentTravelTime[t]/max(1,path_element.total_free_flow_travel_time), path_element.m_TimeDependentCount [t]);


				ASSERT(path_element.m_TimeDependentTravelTime[t]>=0);

				if( path_element.m_MaxTravelTime < path_element.m_TimeDependentTravelTime[t])
					path_element.m_MaxTravelTime = path_element.m_TimeDependentTravelTime[t];

				for(int tt=1; tt<time_step; tt++)
				{
					path_element.m_TimeDependentTravelTime[t+tt] = path_element.m_TimeDependentTravelTime[t];
				}


			}  // for each time
			fprintf(st,"\n");

		} // for each path

		fprintf(st,"\n\nPart V,time-dependent emission data");
		// 

		time_step= 15;


		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

			fprintf(st,"\nTime,,,");

			for(int t = m_TimeLeft ; t< m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%s,", m_pDoc->GetTimeStampString24HourFormat (t));
			}

			fprintf(st,"\nPath %d, %s,path engery,",p+1,path_element.m_path_name .c_str ());

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentEnergy [t]);
			} 

			fprintf(st,"\nPath,,path CO2,");

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentCO2 [t]);
			} 

			fprintf(st,"\nPath,,path NOX,");

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentNOX [t]);
			} 

			fprintf(st,"\nPath,,path CO,");

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentCO [t]);
			} 


			fprintf(st,"\nPath,,path HC,");

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentHC[t]);
			} 
			// for each time

			fprintf(st,"\nPath %d, %s,path engery per distance,",p+1,path_element.m_path_name .c_str ());

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentEnergy [t]/max(0.1,path_element.total_distance ));

			} 

			fprintf(st,"\nPath %d, %s,path mile per gallon,",p+1,path_element.m_path_name .c_str ());

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.total_distance /max(0.01,path_element.m_TimeDependentEnergy [t]/1000/121.7));

			} 
			fprintf(st,"\nPath,,path CO2 per distance,");

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentCO2 [t]/max(0.1,path_element.total_distance ));
			} 

			fprintf(st,"\nPath,,path NOx  per distance,");

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentNOX [t]/max(0.1,path_element.total_distance ));
			} 

			fprintf(st,"\nPath,,path CO  per distance,");

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentCO [t]/max(0.1,path_element.total_distance ));
			} 


			fprintf(st,"\nPath,,path HC  per distance,");

			for(int t = m_TimeLeft ; t<m_TimeRight; t+= time_step)  // for each starting time
			{
				fprintf(st,"%.2f,", path_element.m_TimeDependentHC[t]/max(0.1,path_element.total_distance ));
			} 
			// for each time
			fprintf(st,"\n");

		}


		fclose(st);
	}else
	{
		AfxMessageBox("File cannot be opened.");
		return;
	}
	m_pDoc->OpenCSVFileInExcel (export_file_name);
}


void CDlgPathList::OnDataImportCsv()
{
	OnDataCleanallpaths();
	m_pDoc->ReadInputPathCSVFile(m_pDoc->m_ProjectDirectory  + "optional_path.csv");

	ReloadData();

	Invalidate();
	m_pDoc->UpdateAllViews (0);


}

void CDlgPathList::OnBnClickedCheckZoomToSelectedLink()
{
	// TODO: Add your control notification handler code here
}

void CDlgPathList::OnLbnSelchangeList1()
{
	m_pDoc->m_SelectPathNo = m_PathList.GetCurSel();

	ReloadData();

	Invalidate();
	m_pDoc->UpdateAllViews (0);

}

void CDlgPathList::OnBnClickedDataAnalysis()
{
	std::list<DTALink*>::iterator iLink;

	for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
	{

		(*iLink)-> m_bIncludedBySelectedPath = false;
		(*iLink)-> m_bFirstPathLink = false;
		(*iLink)-> m_bLastPathLink = false;
	}

	// mark all links in the selected path
	if(m_pDoc->m_PathDisplayList.size() > m_pDoc->m_SelectPathNo && m_pDoc->m_SelectPathNo!=-1)
	{
		for (int i=0 ; i<m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_LinkVector.size(); i++)
		{

			DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_LinkVector[i]];
			pLink-> m_bIncludedBySelectedPath = true;

			if(i==0)
			{
				pLink-> m_bFirstPathLink = true;
			}
			if(i== m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_LinkVector.size()-1)
			{
				pLink-> m_bLastPathLink = true;
			}
		}
	}

	CDlg_VehicleClassification* m_pDlg = new CDlg_VehicleClassification; 

	m_pDlg->m_PresetChartTitle.Format ("Path No.%d %s", m_pDoc->m_SelectPathNo+1, m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_path_name .c_str());

	m_pDlg->m_pDoc = m_pDoc;
	m_pDoc->m_VehicleSelectionMode = CLS_path_trip;
	m_pDlg->m_XSelectionNo = CLS_time_interval_15_min;
	m_pDlg->m_VehicleSelectionNo  = CLS_path_trip;
	m_pDlg->SetModelessFlag(true); // voila! this is all it takes to make your dlg modeless!
	m_pDlg->Create(IDD_DIALOG_Summary); 
	m_pDlg->ShowWindow(SW_SHOW); 

}

void CDlgPathList::OnDataGeneratesampleinputpathcsv()
{
	// calculate time-dependent travel time
	if(m_pDoc->m_PathDisplayList.size()==0)
	{
		AfxMessageBox("To generate the file optional_path.csv, Please first define one path by selecting the origin and destination nodes.", MB_ICONINFORMATION);
		return;
	}

	if(m_pDoc->m_ProjectDirectory.GetLength () == 0 )
	{
	
		AfxMessageBox("Please first specify a folder for the new project.", MB_ICONINFORMATION);
		return;

	}


	m_pDoc->SaveInputPathCSVFile(m_pDoc->m_ProjectDirectory +"optional_path.csv");

}

void CDlgPathList::OnDataCleanallpaths()
{
	for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
	{
		m_pDoc->m_PathDisplayList[p].m_LinkVector.clear ();
	}
	m_pDoc->m_PathDisplayList.clear ();

	m_PathList.ResetContent ();

	m_ListCtrl.DeleteAllItems();

	m_pDoc->m_OriginNodeID = -1;
	m_pDoc->m_DestinationNodeID = -1;
	m_StrPathMOE.Format ("");
	UpdateData(0);
	m_pDoc->UpdateAllViews(0);

}

void CDlgPathList::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect PlotRect;
	GetClientRect(PlotRect);

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_LIST1);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect); 

	CRect PlotRectOrg = PlotRect;

	if(m_TimeLeft<0)
		m_TimeLeft = 0;

	if(m_TimeRight< m_TimeLeft+60)
		m_TimeRight= m_TimeLeft+60;

	PlotRect.top += 70;
	PlotRect.bottom = PlotRect.top + 160;
	PlotRect.left = rect.right +50 ;

	PlotRect.right -= 40;

	DrawPlot(&dc, PlotRect);

}

void CDlgPathList::DrawPlot(CPaintDC* pDC,CRect PlotRect)
{
	if(m_PathList.GetCount () ==0)
	{

		pDC->TextOut(PlotRect.left,PlotRect.top+0 ,"No path has been defined.");
		pDC->TextOut(PlotRect.left,PlotRect.top+30 ,"Two methods for generating paths:");
		pDC->TextOut(PlotRect.left,PlotRect.top+50 ,"select GIS node layer and right click origin/destination nodes.");
		pDC->TextOut(PlotRect.left,PlotRect.top+70 ,"load path csv file from the menu Data->Import CSV.");

		return;

	}
	int p = 	m_PathList.GetCurSel();

	CalculateTimeDependentTravelTime();

	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));

	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_DOT,0,RGB(0,0,0));

	CString str_MOE;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&NormalPen);


	// step 1: calculate m_YUpperBound;
	m_YUpperBound = 0;
	int i;


	// step 2: calculate m_UnitDistance;
	// data unit
	m_UnitDistance = 1;
	if((m_YUpperBound - m_YLowerBound)>0)
		m_UnitDistance = (float)(PlotRect.bottom - PlotRect.top)/(m_YUpperBound - m_YLowerBound);


	// step 3: time interval
	int TimeXPosition;

	int TimeInterval = g_FindClosestTimeResolution(m_TimeRight - m_TimeLeft);

	// time unit
	m_UnitTime = 1;
	if((m_TimeRight - m_TimeLeft)>0)
		m_UnitTime = (float)(PlotRect.right - PlotRect.left)/(m_TimeRight - m_TimeLeft);


	// step 4: draw time axis

	pDC->SelectObject(&TimePen);



	char buff[20];
	for(i=m_TimeLeft;i<=m_TimeRight;i+=TimeInterval)
	{
		if(i == m_TimeLeft || i==m_TimeRight)
		{
			pDC->SelectObject(&NormalPen);

			//			i = int((m_TimeLeft/TimeInterval)+0.5)*TimeInterval; // reset time starting point
		}
		else
			pDC->SelectObject(&DataPen);

		TimeXPosition=(long)(PlotRect.left+(i-m_TimeLeft)*m_UnitTime);

		if(i>= m_TimeLeft)
		{
			pDC->MoveTo(TimeXPosition,PlotRect.bottom+2);
			pDC->LineTo(TimeXPosition,PlotRect.top);

			if(i/2 <10)
				TimeXPosition-=5;
			else
				TimeXPosition-=3;

			if(TimeInterval < 60)
			{
				int hour, min;
				hour = i/60;
				min =  i- hour*60;
				wsprintf(buff,"%2d:%02d",hour, min);
			}
			else
			{
				int min_in_a_day = i-int(i/1440*1440);

				wsprintf(buff,"%dh",min_in_a_day/60 );

			}
			pDC->TextOut(TimeXPosition,PlotRect.bottom+3,buff);
		}
	}

	pDC->SelectObject(&s_PenSimulationClock);
	if(g_Simulation_Time_Stamp >=m_TimeLeft && g_Simulation_Time_Stamp <= m_TimeRight )
	{
		TimeXPosition=(long)(PlotRect.left+(g_Simulation_Time_Stamp -m_TimeLeft)*m_UnitTime);
		pDC->MoveTo(TimeXPosition,PlotRect.bottom+2);
		pDC->LineTo(TimeXPosition,PlotRect.top);
	}


	m_YUpperBound = 0;
	m_YLowerBound = 0;
	TimeInterval = 1;

	int value_type = m_PlotType.GetCurSel ();

	if(p <0)
		return;

	for(int t=m_TimeLeft;t<m_TimeRight;t+=TimeInterval)
	{
		if( m_YUpperBound < m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, value_type, m_MOEAggregationIntervalInMin) )
			m_YUpperBound =  m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, value_type,m_MOEAggregationIntervalInMin);

	}

	if(m_PlotType.GetCurSel () == 1)  //additional data
	{
		for(int t=m_TimeLeft;t<m_TimeRight;t+=TimeInterval)
		{
			if( m_YUpperBound < m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, 0,m_MOEAggregationIntervalInMin) )
				m_YUpperBound =  m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, 0,m_MOEAggregationIntervalInMin);

			if( m_YUpperBound < m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, 1,m_MOEAggregationIntervalInMin) )
				m_YUpperBound =  m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, 1,m_MOEAggregationIntervalInMin);

		}


	}


	if(m_YUpperBound>=10.0f)
		m_YUpperBound = (m_YUpperBound/10.0f+1.0f)*10.f;
	else
		m_YUpperBound = int(m_YUpperBound)+2.0f;

	float YInterval = 0.2f;

	if(m_YUpperBound>=2.0f)
		YInterval = 0.5f;

	if(m_YUpperBound>=5.0f)
		YInterval = 1.f;

	if(m_YUpperBound>=10.0f)
		YInterval = 2.0f;

	if(m_YUpperBound>=20.0f)
		YInterval = 5.0f;

	if(m_YUpperBound>=50.f)
		YInterval = 10.f;

	if(m_YUpperBound>=80.f)
		YInterval = 20.f;

	if(m_YUpperBound>=150.f)
		YInterval = 30.f;

	if(m_YUpperBound>=500.f)
		YInterval = int(m_YUpperBound/5);

	// data unit
	m_UnitData = 1.f;
	if((m_YUpperBound - m_YLowerBound)>0.0f)
		m_UnitData = (float)(PlotRect.bottom - PlotRect.top)/(m_YUpperBound - m_YLowerBound);

	// draw Y axis

	for(float ii=m_YLowerBound; ii <= m_YUpperBound; ii+= YInterval)
	{
		if( ii > m_YUpperBound)
			ii = m_YUpperBound;

		if(ii == m_YLowerBound)
			pDC->SelectObject(&NormalPen);
		else
			pDC->SelectObject(&DataPen);

		int TimeYPosition= PlotRect.bottom - (int)((ii*m_UnitData)+0.50f);

		pDC->MoveTo(PlotRect.left-2, TimeYPosition);
		pDC->LineTo(PlotRect.right,TimeYPosition);

		if(ii <= m_YUpperBound)
		{
			if(YInterval>=1)
				sprintf_s(buff,"%3.0f",ii);
			else
				sprintf_s(buff,"%3.1f",ii);

			pDC->TextOut(PlotRect.left-45,TimeYPosition-5,buff);
		}
	}

	int MOEType = m_PlotType.GetCurSel ();


	if(m_PlotType.GetCurSel ()!=1)
	{

		CPen BluePen(PS_SOLID,1,RGB(0,0,255));

		pDC->SelectObject(&BluePen);


		for(int t=m_TimeLeft;t<m_TimeRight;t+=TimeInterval)
		{
			int TimeYPosition= PlotRect.bottom - (int)((m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, MOEType, m_MOEAggregationIntervalInMin)*m_UnitData)+0.50);
			TimeXPosition=(long)(PlotRect.left+(t-m_TimeLeft)*m_UnitTime);


			if(t==m_TimeLeft)
				pDC->MoveTo(TimeXPosition,TimeYPosition);
			else
				pDC->LineTo(TimeXPosition,TimeYPosition);

		}

	}


	if(m_PlotType.GetCurSel () ==1)  // draw sensor travel time
	{

		MOEType = 0;  // simulation
		CPen BluePen(PS_SOLID,1,RGB(0,0,255));

		pDC->SelectObject(&BluePen);


		for(int t=m_TimeLeft;t<m_TimeRight;t+=TimeInterval)
		{
			int TimeYPosition= PlotRect.bottom - (int)((m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, MOEType, m_MOEAggregationIntervalInMin)*m_UnitData)+0.50);
			TimeXPosition=(long)(PlotRect.left+(t-m_TimeLeft)*m_UnitTime);


			if(t==m_TimeLeft)
				pDC->MoveTo(TimeXPosition,TimeYPosition);
			else
				pDC->LineTo(TimeXPosition,TimeYPosition);

		}

		MOEType = 1;  // observation
		CPen SensorPen(PS_SOLID,1,RGB(255,64,64));	 // brown
		pDC->SelectObject(&SensorPen);


		for(int t=m_TimeLeft;t<m_TimeRight;t+=TimeInterval)
		{
			int TimeYPosition= PlotRect.bottom - (int)((m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, MOEType, m_MOEAggregationIntervalInMin)*m_UnitData)+0.50);
			TimeXPosition=(long)(PlotRect.left+(t-m_TimeLeft)*m_UnitTime);


			if(t==m_TimeLeft)
				pDC->MoveTo(TimeXPosition,TimeYPosition);
			else
				pDC->LineTo(TimeXPosition,TimeYPosition);

		}

	}

	//// max travel time  // draw band (mean, max) for selected paths
	//if(MOEType==3)  
	//{
	//	
	//	g_SelectSuperThickPenColor(pDC,p);


	//	CPoint pt[192];

	//	int pt_count = 0;
	//	int t;
	//	for(t=0;t<1440;t+=15)
	//		{
	//		int TimeYPosition= PlotRect.bottom - (int)((m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, 1, m_MOEAggregationIntervalInMin)*m_UnitData)+0.50);
	//			TimeXPosition=(long)(PlotRect.left+(t-m_TimeLeft)*m_UnitTime);

	//		pt[pt_count].x =TimeXPosition;
	//		pt[pt_count].y =TimeYPosition;
	//		
	//		pt_count++;

	//		}

	//	for(t=1440-15;t>=0;t-=15)
	//		{
	//		int TimeYPosition= PlotRect.bottom - (int)((m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, 0, m_MOEAggregationIntervalInMin)*m_UnitData)+0.50);
	//		TimeXPosition=(long)(PlotRect.left+(t-m_TimeLeft)*m_UnitTime);

	//		pt[pt_count].x =TimeXPosition;
	//		pt[pt_count].y =TimeYPosition;
	//		
	//		pt_count++;

	//		}

	//	g_SelectThickPenColor(pDC,p);
	//	g_SelectBrushColor(pDC, p);
	//	pDC->Polygon (pt,192);

	//}


	/*
	// step 5: draw segments along the select path

	m_SegmentDistanceVector.resize(m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkSize);

	for (i=0 ; i<m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkSize; i++)
	{
	DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkVector[i]];
	if(pLink!=NULL)
	{
	m_YUpperBound+=pLink->m_Length ;

	m_SegmentDistanceVector[i] = m_YUpperBound;
	}
	}

	int TimeYPosition;
	int TimeYPositionPrev;

	for (i=0 ; i<m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkSize; i++)
	{
	DTALink* pLink;

	// extra bottom line
	if(i == 0)
	{

	pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkVector[i]];

	pDC->SelectObject(&NormalPen);
	TimeYPosition= PlotRect.bottom;
	TimeYPositionPrev = TimeYPosition;

	if(pLink!=NULL)
	{
	wsprintf(buff,"A: %d",pLink->m_FromNodeNumber );
	pDC->TextOut(PlotRect.left-50,TimeYPosition-5,buff);
	}

	pDC->MoveTo(PlotRect.left-2, TimeYPosition);
	pDC->LineTo(PlotRect.right,TimeYPosition);
	}


	pDC->SelectObject(&DataPen);

	TimeYPosition= PlotRect.bottom - (int)((m_SegmentDistanceVector[i]*m_UnitDistance)+0.50);

	pDC->MoveTo(PlotRect.left-2, TimeYPosition);
	pDC->LineTo(PlotRect.right,TimeYPosition);

	pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkVector[i]];
	if(pLink!=NULL)
	{
	if(i== m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkSize-1)
	{
	wsprintf(buff,"B: %d",pLink->m_ToNodeNumber );
	pDC->TextOut(PlotRect.left-50,TimeYPosition-5,buff);
	}
	else
	{
	if(TimeYPosition < TimeYPositionPrev-10 && TimeYPosition >= PlotRect.bottom-10)
	{
	wsprintf(buff,"%d",pLink->m_ToNodeNumber );
	pDC->TextOut(PlotRect.left-40,TimeYPosition-5,buff);
	}
	}

	TimeYPositionPrev = TimeYPosition;

	}
	}

	*/
}

void CDlgPathList::CalculateTimeDependentTravelTime()
{
	int time_step  =1;
	for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
	{
		DTAPath path_element = m_pDoc->m_PathDisplayList[p];

		m_pDoc->m_PathDisplayList[p].total_free_flow_travel_time = 0;
		m_pDoc->m_PathDisplayList[p].total_distance = 0;

		for (int i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
		{
			DTALink* pLink = NULL;
			int link_id = m_pDoc->m_PathDisplayList[p].m_LinkVector[i];
			if(m_pDoc->m_LinkNoMap.find(link_id)== m_pDoc->m_LinkNoMap.end())
				return;

			pLink= m_pDoc->m_LinkNoMap[link_id];

			if(pLink == NULL)
				break;

			m_pDoc->m_PathDisplayList[p].total_free_flow_travel_time += pLink->m_FreeFlowTravelTime ;
			m_pDoc->m_PathDisplayList[p].total_distance += pLink->m_Length ;

		}

		for(int t = m_pDoc->m_SimulationStartTime_in_min ; t< min(1440,m_pDoc->m_SimulationEndTime_in_min); t+= time_step)  // for each starting time
		{

			path_element.m_TimeDependentCount[t] = 0;
			path_element.m_TimeDependentTravelTime[t] = t;  // t is the departure time
			path_element.m_TimeDependentEnergy[t]= 0;
			path_element.m_TimeDependentCO2[t]= 0;
			path_element.m_TimeDependentCO[t]= 0;
			path_element.m_TimeDependentHC[t]=0;
			path_element.m_TimeDependentNOX[t] =0;

			for (int i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{


			DTALink* pLink = NULL;
			int link_id = m_pDoc->m_PathDisplayList[p].m_LinkVector[i];
			if(m_pDoc->m_LinkNoMap.find(link_id)== m_pDoc->m_LinkNoMap.end())
				return;

			pLink= m_pDoc->m_LinkNoMap[link_id];
				if(pLink == NULL)
					break;

				path_element.m_TimeDependentTravelTime[t] += pLink->GetDynamicTravelTime(path_element.m_TimeDependentTravelTime[t],m_pDoc->m_PrimaryDataSource );

				int current_time = path_element.m_TimeDependentTravelTime[t];
				if(current_time >=1440)
					current_time = 1439;


				if(current_time < pLink->m_LinkMOEArySize -1 )
				{
				path_element.m_TimeDependentEnergy[t] += pLink->m_LinkMOEAry [current_time].Energy;
				path_element.m_TimeDependentCO2[t] += pLink->m_LinkMOEAry [current_time].CO2;
				path_element.m_TimeDependentCO[t] += pLink->m_LinkMOEAry [current_time].CO;
				path_element.m_TimeDependentHC[t] += pLink->m_LinkMOEAry [current_time].HC;
				path_element.m_TimeDependentNOX[t] += pLink->m_LinkMOEAry [current_time].NOX;
				}


			}

			path_element.m_TimeDependentTravelTime[t] -= t; // remove the starting time, so we have pure travel time;
			m_pDoc->m_PathDisplayList[p].m_TimeDependentTravelTime[t] =   path_element.m_TimeDependentTravelTime[t] ;
			m_pDoc->m_PathDisplayList[p].m_TimeDependentEnergy[t] =   path_element.m_TimeDependentEnergy[t] ;
			m_pDoc->m_PathDisplayList[p].m_TimeDependentCO2[t] =   path_element.m_TimeDependentCO2[t] ;
			m_pDoc->m_PathDisplayList[p].m_TimeDependentCO[t] =   path_element.m_TimeDependentCO[t] ;
			m_pDoc->m_PathDisplayList[p].m_TimeDependentHC[t] =   path_element.m_TimeDependentHC[t] ;
			m_pDoc->m_PathDisplayList[p].m_TimeDependentNOX[t] =   path_element.m_TimeDependentNOX[t] ;



		}  // for each time

	}

}
void CDlgPathList::OnCbnSelchangeComboStarthour()
{
	int sel = m_StartHour.GetCurSel();

	if((sel)* 60 < m_TimeRight)
	{
		m_TimeLeft = (sel)* 60;
	}else
	{// restore
		m_StartHour.SetCurSel (m_TimeLeft/60);

	}
	Invalidate();
}

void CDlgPathList::OnCbnSelchangeComboEndhour()
{
	int sel = m_EndHour.GetCurSel();

	if(m_TimeLeft <(sel)* 60)
	{
		m_TimeRight = (sel)* 60;
	}else
	{  // restore
		m_EndHour.SetCurSel(m_TimeRight/60);

	}

	Invalidate();
}

void CDlgPathList::OnCbnSelchangeComboAggintrevallist()
{
	m_MOEAggregationIntervalInMin = m_AggregationValueVector [ m_AggregationIntervalList.GetCurSel()];
	Invalidate();
}

void CDlgPathList::OnCbnSelchangeComboPlotType()
{
	Invalidate();
}
void CDlgPathList::OnSize(UINT nType, int cx, int cy)
{
	RedrawWindow();

	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}


void CDlgPathList::OnBnClickedAnalysis2()
{
	std::list<DTALink*>::iterator iLink;

	for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
	{

		(*iLink)-> m_bIncludedBySelectedPath = false;
		(*iLink)-> m_bFirstPathLink = false;
		(*iLink)-> m_bLastPathLink = false;

	}

	// mark all links in the selected path
	if(m_pDoc->m_PathDisplayList.size() > m_pDoc->m_SelectPathNo && m_pDoc->m_SelectPathNo!=-1)
	{
		for (int i=0 ; i<m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_LinkVector.size(); i++)
		{

			DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_LinkVector[i]];
			pLink-> m_bIncludedBySelectedPath = true;

			if(i==0)
			{
				pLink-> m_bFirstPathLink = true;
			}
			if(i== m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_LinkVector.size()-1)
			{
				pLink-> m_bLastPathLink = true;
			}

		}
	}

	CDlg_VehicleClassification* m_pDlg = new CDlg_VehicleClassification; 

	m_pDlg->m_PresetChartTitle.Format ("End-to-End Path No.%d", m_pDoc->m_SelectPathNo+1);

	m_pDlg->m_pDoc = m_pDoc;
	m_pDoc->m_VehicleSelectionMode = CLS_path_partial_trip;
	m_pDlg->m_XSelectionNo = CLS_time_interval_15_min;
	m_pDlg->m_VehicleSelectionNo  = CLS_path_partial_trip;
	m_pDlg->SetModelessFlag(true); // voila! this is all it takes to make your dlg modeless!
	m_pDlg->Create(IDD_DIALOG_Summary); 
	m_pDlg->ShowWindow(SW_SHOW); 

}

void CDlgPathList::OnBnClickedFreevalAnalysisGenerateData()
{
	// TODO: Add your control notification handler code here
}

void CDlgPathList::OnBnClickedFreevalAnalysisGenerateFile()
{
	// calculate time-dependent travel time
	int time_step = 1;

	CString export_file_name;

	export_file_name = m_pDoc->m_ProjectDirectory +"FREEVAL_input_segment.csv";

	FILE* st;
	fopen_s(&st,export_file_name,"w");
	if(st!=NULL)
	{
		m_pDoc->m_SelectPathNo = m_PathList.GetCurSel();

		int time_stamp = 0;
		for(time_stamp = m_pDoc->m_DemandLoadingStartTimeInMin;  time_stamp < m_pDoc->m_DemandLoadingEndTimeInMin; time_stamp+=15)
		{
		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(m_pDoc->m_SelectPathNo!=p)
				continue;
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];
			
			int i;

			if(time_stamp == m_pDoc->m_DemandLoadingStartTimeInMin)
			{

				fprintf(st,"Time Interval,Segment Number,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink != NULL)
				{
					fprintf(st,"%d,",i+1 );
				}else
				{
				AfxMessageBox("missing link for in the path file. Please check!");
				fclose(st);
				return;
				}
			}
				fprintf(st,"\n");

			}


		// 1st row: segment label
				fprintf(st,"%s",m_pDoc->GetTimeStampString24HourFormat (time_stamp));

			fprintf(st,",Segment Label,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				fprintf(st,"S%0d,",i+1);
			}
				fprintf(st,"\n");
			
			// 2nd row: type
			fprintf(st,",Type (B;ONR;OFR;R or W),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"%s,", pLink->GetFREEVALCode());
			}
			fprintf(st,"\n");

			// 3rd row: 
			fprintf(st,",Length (ft),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"%.0f,",pLink->m_Length *5280);
			}
			fprintf(st,"\n");

			// 3rd row: 
			fprintf(st,",Number of Lanes,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"%d,",pLink->m_NumberOfLanes );
			}

			fprintf(st,"\n");

			// 4th row: 
			fprintf(st,",FF Speed (Mi/hr),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"%.0f,",pLink->m_SpeedLimit  );
			}

			fprintf(st,"\n");


			// 5th row: 
			fprintf(st,",Segment Demand (vph),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"%.0f,",pLink->GetAvgLinkHourlyVolume(time_stamp, time_stamp+15));
			}

			fprintf(st,"\n");


			// 6th row: 
			fprintf(st,",Capacity Adjustment Factor,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"1.00,");
			}

			fprintf(st,"\n");

			// 7th row: 
			fprintf(st,",Origin Demand Adjustment Factor,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"1.11,");
			}

			fprintf(st,"\n");

			// 8th row: 
			fprintf(st,",Destination Demand Adjustment Factor,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"1.11,");
			}

			fprintf(st,"\n");
			fprintf(st,",Detailed Info\n");

			//
			fprintf(st,",%% Trucks,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"5.0,");
			}

			fprintf(st,"\n");

			//-----------------------------------------
			fprintf(st,",%% of RV's,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"0,");
			}

			fprintf(st,"\n");

			//-----------------------------------------
			fprintf(st,",On-Ramp Demand (vph),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];

				if(pLink->m_FREEVALSegmentCode == FREEVAL_ONR)
					fprintf(st,"%.0f,",pLink->GetAvgLinkHourlyVolume(time_stamp, time_stamp+15));
				else 
					fprintf(st,",");
			}

			fprintf(st,"\n");

		//-----------------------------------------
			fprintf(st,",On-Ramp %% Trucks,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink->m_FREEVALSegmentCode == FREEVAL_ONR)
					fprintf(st,"5,");
				else 
					fprintf(st,",");

			}

			fprintf(st,"\n");


		//-----------------------------------------
			fprintf(st,",On-Ramp %% RV's,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink->m_FREEVALSegmentCode == FREEVAL_ONR)
					fprintf(st,"0,");
				else 
					fprintf(st,",");
			}

			fprintf(st,"\n");

		//-----------------------------------------
			fprintf(st,",Off-Ramp Demand (vph),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink->m_FREEVALSegmentCode == FREEVAL_OFR)
					fprintf(st,"%.0f,",pLink->GetAvgLinkHourlyVolume(time_stamp, time_stamp+15));
				else 
					fprintf(st,",");
			}

			fprintf(st,"\n");

		//-----------------------------------------
			fprintf(st,",Off-Ramp %% Trucks,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink->m_FREEVALSegmentCode == FREEVAL_OFR)
					fprintf(st,"5,");
				else
					fprintf(st,",");


			}

			fprintf(st,"\n");

		//-----------------------------------------
			fprintf(st,",Off-Ramp %% RVs,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink->m_FREEVALSegmentCode == FREEVAL_OFR)
					fprintf(st,"0,");
				else
					fprintf(st,",");
			}

			fprintf(st,"\n");


		//-----------------------------------------
			fprintf(st,",Acc/Dec Lane Length (ft),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];

				if(pLink->IsRampAttached())
					fprintf(st,"%.0f,",pLink->GetRelatedRampInfo().m_Length *5280);
				else
					fprintf(st,",");
			}

			fprintf(st,"\n");

		//-----------------------------------------
			fprintf(st,",Number of Lanes on Ramps,");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink->IsRampAttached())
					fprintf(st,"%d,",pLink->GetRelatedRampInfo().m_NumberOfLanes);
				else
					fprintf(st,",");
			}

			fprintf(st,"\n");

		//-----------------------------------------
			fprintf(st,",Ramp on Left or Right (L/R),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink->IsRampAttached())
					fprintf(st,"Right,");
				else
					fprintf(st,",");
			}

			fprintf(st,"\n");


		//-----------------------------------------
			fprintf(st,",Ramp FFS(mi/hr),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink->IsRampAttached())
					fprintf(st,"%.0f,",pLink->GetRelatedRampInfo().m_SpeedLimit);
				else
					fprintf(st,",");
			}
			fprintf(st,"\n");

			fprintf(st,"%s,",m_pDoc->GetTimeStampString24HourFormat (time_stamp) );

				fprintf(st,",Link (from->to),");
			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{
				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
					fprintf(st,"%d->%d,",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );
			}
				fprintf(st,"\n");


		} //for each path

		}
		fclose(st);

		if(AfxMessageBox("File FREEVAL_input_segment.csv is generated in the project folder.", MB_ICONINFORMATION)==IDOK)
		{m_pDoc->OnFreewaytoolsView();}


		m_pDoc->OpenCSVFileInExcel (export_file_name);

		
	}else
		{
		
			AfxMessageBox("File FREEVAL_input_segment.csv cannot be opened to write. Please check if it is currently opened in EXCEL.", MB_ICONINFORMATION);
	
			return;
		}
	

}



void CDlgPathList::OnChangeattributesforlinksalongpathChangelanecapacity()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_lane_capacity;
	ChangeLinkAttributeDialog();
}

void CDlgPathList::OnChangeattributesforlinksalongpathChange()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_number_of_lanes;
	ChangeLinkAttributeDialog();

}

void CDlgPathList::OnChangeattributesforlinksalongpathChangelinktype()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_link_type;
	ChangeLinkAttributeDialog();
}

void CDlgPathList::OnChangeattributesforlinksalongpathChangespeedlimit()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_speed_limit_mph;
	ChangeLinkAttributeDialog();
	
}

void CDlgPathList::OnChangeattributesforlinksalongpathChangespeedlimitKmph()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_speed_limit_kmph;
	ChangeLinkAttributeDialog();
}



void CDlgPathList::OnChangeattributesforlinksalongpathChangejamdensity()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_jam_density_vhcpm;
	ChangeLinkAttributeDialog();

}

void CDlgPathList::OnChangeattributesforlinksalongpathChangebackwavespeed()
{
		m_ChangeLinkAttributeMode = eChangeLinkAttribute_backwardwave_speed_mph;
	ChangeLinkAttributeDialog();

}

void CDlgPathList::OnChangeattributesforlinksalongpathChangesaturationflowrate()
{
		m_ChangeLinkAttributeMode = eChangeLinkAttribute_saturation_flow_rate;
	ChangeLinkAttributeDialog();
}



void CDlgPathList::OnChangeattributesforlinksalongpathEffectivegreentime()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_effective_green_time;
	ChangeLinkAttributeDialog();
}
void CDlgPathList::OnChangeattributesforlinksalongpathChangejamdensity33625()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_jam_density_vhcpm;
	ChangeLinkAttributeDialog();

}


void CDlgPathList::ChangeLinkAttributeDialog()
{
		CDlg_UserInput dlg;

		dlg.m_StrQuestion  = "Please specify the input value:";


		float value = 0;

					switch(m_ChangeLinkAttributeMode)
					{
					case eChangeLinkAttribute_lane_capacity: dlg.m_InputValue = "2000"; break;
					case eChangeLinkAttribute_number_of_lanes: dlg.m_InputValue = "3"; break;
					case eChangeLinkAttribute_link_type: dlg.m_InputValue = "1"; break;
					case eChangeLinkAttribute_speed_limit_mph : dlg.m_InputValue = "65";break;
					case eChangeLinkAttribute_speed_limit_kmph: dlg.m_InputValue = "100"; break; 
					case eChangeLinkAttribute_jam_density_vhcpm : dlg.m_InputValue = "180"; break;
					case eChangeLinkAttribute_jam_density_vhcpkm: dlg.m_InputValue = "110"; break; 
					case eChangeLinkAttribute_backwardwave_speed_mph: dlg.m_InputValue = "12";; break;
					case eChangeLinkAttribute_backwardwave_speed_kmph: dlg.m_InputValue = "7.4"; break;
					case eChangeLinkAttribute_saturation_flow_rate: dlg.m_InputValue = "1800"; break;
					case eChangeLinkAttribute_effective_green_time : dlg.m_InputValue = "60"; break; 
					case eChangeLinkAttribute_loop_code: dlg.m_InputValue = "inner or outter"; break;
					case eChangeLinkAttribute_orientation_code: dlg.m_InputValue = "up or down"; break;

					}
		

		if(dlg.DoModal ()==IDOK)
		{
			value = atof(dlg.m_InputValue) ;


			if(AfxMessageBox("Are you sure to make the change?",  MB_YESNO|MB_ICONINFORMATION)==IDYES)
			{
				ChangeLinkAttributeAlongPath(value, dlg.m_InputValue);
			}
		}
	

}


void CDlgPathList::ChangeLinkAttributeAlongPath(float value, CString value_string)
{
		m_pDoc->m_SelectPathNo = m_PathList.GetCurSel();

		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(m_pDoc->m_SelectPathNo!=p)
				continue;
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];
			
			int i;

			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{

				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink != NULL)
				{
					m_pDoc->Modify (true);

					int ToNodeID; 
					DTANode* pNode =NULL;
					switch(m_ChangeLinkAttributeMode)
					{
					case eChangeLinkAttribute_lane_capacity: pLink->m_LaneCapacity = value; break;
					case eChangeLinkAttribute_number_of_lanes: pLink->m_NumberOfLanes = value; break;
					case eChangeLinkAttribute_link_type: pLink->m_link_type = value; break;
					case eChangeLinkAttribute_speed_limit_mph : pLink->m_SpeedLimit = value; break;
					case eChangeLinkAttribute_speed_limit_kmph: pLink->m_SpeedLimit = value*0.621371; break; 
					case eChangeLinkAttribute_jam_density_vhcpm : pLink->m_Kjam = value; break;
					case eChangeLinkAttribute_jam_density_vhcpkm: pLink->m_Kjam = value*0.621371; break; 
					case eChangeLinkAttribute_backwardwave_speed_mph: pLink->m_Wave_speed_in_mph = value; break;
					case eChangeLinkAttribute_backwardwave_speed_kmph: pLink->m_Wave_speed_in_mph = value*0.621371; break;
					case eChangeLinkAttribute_saturation_flow_rate: pLink->m_Saturation_flow_rate_in_vhc_per_hour_per_lane = value; break;

					case eChangeLinkAttribute_loop_code:
						pLink->m_loop_code = m_pDoc->CString2StdString(value_string); break;
					case eChangeLinkAttribute_orientation_code: 
						pLink->m_orientation_code = m_pDoc->CString2StdString(value_string); break;

					case eChangeLinkAttribute_effective_green_time:

						ToNodeID = pLink->m_ToNodeID ;
						pNode = m_pDoc->m_NodeNoMap[ToNodeID];
				
					
					}


				}  // for all links
			}
		}

		m_pDoc->UpdateAllViews(0);
}




void CDlgPathList::OnChangeattributesforlinksalongpathDeletelinksalongpath()
{
		m_pDoc->m_SelectPathNo = m_PathList.GetCurSel();

		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(m_pDoc->m_SelectPathNo!=p)
				continue;
			DTAPath path_element = m_pDoc->m_PathDisplayList[p];
			
			int i;

			for (i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
			{

				DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];
				if(pLink != NULL)
				{

					m_pDoc->DeleteLink(pLink);
					m_pDoc->Modify (true);


				}  // for all links
			}
		}

		m_pDoc->m_PathDisplayList.clear ();

		m_pDoc->UpdateAllViews(0);
}

void CDlgPathList::OnBnClickedDynamicDensityContour()
{
	CString PathTitle;

	int ytics_stepsize  = 1;

	CString export_file_name, export_plt_file_name;

	export_file_name = m_pDoc->m_ProjectDirectory +"export_path_density.txt";

	export_plt_file_name = m_pDoc->m_ProjectDirectory +"export_path_density.plt";

	int yrange = 0;
	int xrange = m_TimeRight - m_TimeLeft +1;


	FILE* st;
	fopen_s(&st,export_file_name,"w");
	if(st!=NULL)
	{
		
	int step_size = 1;

	
	for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(p != m_PathList.GetCurSel())
				continue;

			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

			PathTitle.Format("Path %d %s", p+1,  path_element.m_path_name.c_str () );

			if(path_element.m_LinkVector.size() >=15)
				ytics_stepsize = 4;

			if(path_element.m_LinkVector.size() >=30)
				ytics_stepsize = 10;

				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];


					if(pLink != NULL)
					{

						for(int s = 0; s< pLink->m_Length / 0.1; s++)
						{

							 
							CString label = pLink->m_Name .c_str ();

							//if(pLink->m_Name  == "(null)")
							//{
							//label.Format ("%d->%d",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber);
							//}

							//fprintf(st,"%d->%d,%s,%s,", pLink->m_FromNodeNumber , pLink->m_ToNodeNumber , m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.c_str (),label);

							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{

								fprintf(st, "%.1f ", pLink->GetSimulatedDensity (t));

							}
							fprintf(st,"\n");

							yrange++;
						}

					}


				}

							// last line with zero
							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{

								fprintf(st, "0.0 ");

							}

							fprintf(st,"\n");

			} //for each path
		
	fclose(st);
	}


	FILE* st_plt;
	fopen_s(&st_plt,export_plt_file_name,"w");
	if(st_plt!=NULL)
	{

		CString xtics_str; 

		fprintf(st_plt,"set title \"Dynamic Density Contour (%s) Unit: veh/mile/lane\" \n", PathTitle);

		fprintf(st_plt,"set xlabel \"Time Horizon\"\n");
		fprintf(st_plt,"set ylabel \"Space (Node Sequence)\"  offset -1\n");

		int xtics_stepsize  = 30;

		if(xrange/xtics_stepsize >20)
			xtics_stepsize = 120;  // 2 hour interval
		else if(xrange/xtics_stepsize >10)
			xtics_stepsize = 60;   // 1 hour interval
		else if(xrange/xtics_stepsize < 5)
			xtics_stepsize = 10;   // 1 hour interval

			 
		for(int t = m_TimeLeft ; t<= m_TimeRight; t+= xtics_stepsize)  
		{
			CString str;
			str.Format("\"%s\" %d ",m_pDoc->GetTimeStampString24HourFormat (t), t-m_TimeLeft);

			if(t+ xtics_stepsize> m_TimeRight ) 
				xtics_str += str;
			else 
			{
				xtics_str += str ;
				xtics_str += "," ;
			}
		}

		fprintf(st_plt,"set xtics (%s) \n",xtics_str);

		CString ytics_str; 

		int yrange_i  = 0;

		CString last_node_number = " ";

		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(p != m_PathList.GetCurSel())
				continue;

			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];

					if(pLink != NULL)
					{

						for(int s = 0; s< pLink->m_Length / 0.1; s++)
						{

							CString label = pLink->m_Name .c_str ();


							if(pLink->m_Name  == "(null)" || pLink->m_Name.size() ==0)
							{
							label.Format ("%d",pLink->m_FromNodeNumber);

							last_node_number.Format ("%d",pLink->m_ToNodeNumber);
							}

							if(s==0 && (i%ytics_stepsize) ==0)   // first segment 
							{
							CString str;
							str.Format("\"%s\" %d, ",label, yrange_i) ;

							ytics_str += str;
							
							}

							yrange_i++;
						}

					}


				}

				
			} //for each path


		CString str;
		str.Format("\"%s\" %d", last_node_number, yrange_i);
		ytics_str +=str;
		fprintf(st_plt,"set ytics (%s)\n",ytics_str);

		fprintf(st_plt,"set xrange [0:%d] \n",xrange);
		fprintf(st_plt,"set yrange [0:%d] \n",yrange);

		fprintf(st_plt,"set palette defined (0 \"white\", 10 \"green\", 30 \"yellow\", 50 \"red\")\n");


		fprintf(st_plt,"set pm3d map\n");
		fprintf(st_plt,"splot '%s' matrix\ notitle\n",export_file_name);

		fclose(st_plt);
	}else
	{
	AfxMessageBox("File export_path_density.plt cannot be opened. Please check");
	}

	HINSTANCE result = ShellExecute(NULL, _T("open"), export_plt_file_name, NULL,NULL, SW_SHOW);
	

}
void CDlgPathList::OnBnClickedDynamicSpeedContour()
{
	CString export_file_name, export_plt_file_name;

	export_file_name = m_pDoc->m_ProjectDirectory +"export_path_speed.txt";
	// 

	export_plt_file_name = m_pDoc->m_ProjectDirectory +"export_path_speed.plt";

	int yrange = 0;

	CString PathTitle;

	int ytics_stepsize  = 1;
	bool bFreewayFlag = true;
	FILE* st;
	fopen_s(&st,export_file_name,"w");
	if(st!=NULL)
	{
		
	int step_size = 1;
	
	for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{
			if(p != m_PathList.GetCurSel())
				continue;

			DTAPath path_element = m_pDoc->m_PathDisplayList[p];
			PathTitle.Format("Path %d %s", p+1,  path_element.m_path_name.c_str () );


			if(path_element.m_LinkVector.size() >=15)
				ytics_stepsize = 4;

			if(path_element.m_LinkVector.size() >=30)
				ytics_stepsize = 10;
				//for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
				//{
				//	fprintf(st,"%s,", m_pDoc->GetTimeStampString24HourFormat (t));
				//}

				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];


					if(pLink != NULL)
					{

						if(m_pDoc->m_LinkTypeMap[pLink->m_link_type].IsFreeway () == false)
							bFreewayFlag = false;

						for(int s = 0; s< pLink->m_Length / 0.1; s++)
						{
							 
							CString label = pLink->m_Name .c_str ();

							//if(pLink->m_Name  == "(null)")
							//{
							//label.Format ("%d->%d",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber);
							//}

							//fprintf(st,"%d->%d,%s,%s,", pLink->m_FromNodeNumber , pLink->m_ToNodeNumber , m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.c_str (),label);

							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{

								fprintf(st, "%.1f ", pLink->GetDynamicSpeed (t, m_pDoc->m_PrimaryDataSource));

							}
							fprintf(st,"\n");
							yrange++;
						}

					}


				}
							// last line with zero
							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{

								fprintf(st, "0.0 ");

							}

							fprintf(st,"\n");

			} //for each path
		
	fclose(st);
	}

	int xrange = m_TimeRight - m_TimeLeft +1;


	FILE* st_plt;
	fopen_s(&st_plt,export_plt_file_name,"w");
	if(st_plt!=NULL)
	{

		CString xtics_str; 

		fprintf(st_plt,"set title \"Dynamic Speed Contour (%s) Unit: mph\" \n", PathTitle);


		fprintf(st_plt,"set xlabel \"Time Horizon\"\n");
		fprintf(st_plt,"set ylabel \"Space (Node Sequence)\"  offset -1\n");

		int xtics_stepsize  = 30;

		if(xrange/xtics_stepsize >20)
			xtics_stepsize = 120;  // 2 hour interval
		else if(xrange/xtics_stepsize >10)
			xtics_stepsize = 60;   // 1 hour interval
		else if(xrange/xtics_stepsize < 5)
			xtics_stepsize = 10;   // 1 hour interval

			 
		for(int t = m_TimeLeft ; t<= m_TimeRight; t+= xtics_stepsize)  
		{
			CString str;
			str.Format("\"%s\" %d ",m_pDoc->GetTimeStampString24HourFormat (t), t-m_TimeLeft);

			if(t+ xtics_stepsize> m_TimeRight ) 
				xtics_str += str;
			else 
			{
				xtics_str += str ;
				xtics_str += "," ;
			}
		}

		fprintf(st_plt,"set xtics (%s) \n",xtics_str);

		CString ytics_str; 

		int yrange_i  = 0;

	CString last_node_number = " ";

		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(p != m_PathList.GetCurSel())
				continue;

			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];

					if(pLink != NULL)
					{

						for(int s = 0; s< pLink->m_Length / 0.1; s++)
						{

							CString label = pLink->m_Name .c_str ();


							if(pLink->m_Name  == "(null)" || pLink->m_Name.size() ==0)
							{
							label.Format ("%d",pLink->m_FromNodeNumber);

							last_node_number.Format ("%d",pLink->m_ToNodeNumber);
							}

							if(s==0 && (i%ytics_stepsize) ==0)   // first segment 
							{
							CString str;
							str.Format("\"%s\" %d, ",label, yrange_i) ;

							ytics_str += str;
							
							}

							yrange_i++;
						}

					}


				}

				
			} //for each path


		CString str;
		str.Format("\"%s\" %d", last_node_number, yrange_i);
		ytics_str +=str;
		fprintf(st_plt,"set ytics (%s)\n",ytics_str);

		fprintf(st_plt,"set xrange [0:%d] \n",xrange);
		fprintf(st_plt,"set yrange [0:%d] \n",yrange);

		if(bFreewayFlag)
			fprintf(st_plt,"set palette defined (0 \"white\", 0.1 \"red\", 40 \"yellow\", 50 \"green\")\n");
		else
			fprintf(st_plt,"set palette defined (0 \"white\", 0.1 \"red\", 20 \"yellow\", 40 \"green\")\n");

		fprintf(st_plt,"set pm3d map\n");
		fprintf(st_plt,"splot '%s' matrix\ notitle\n",export_file_name);

		fclose(st_plt);
	}else
	{
	AfxMessageBox("File export_path_speed.plt cannot be opened. Please check");
	}



	HINSTANCE result = ShellExecute(NULL, _T("open"), export_plt_file_name, NULL,NULL, SW_SHOW);
}

void CDlgPathList::OnBnClickedDynamicFlowContour()
{
	int ytics_stepsize = 1;
	CString export_file_name, export_plt_file_name;

	export_file_name = m_pDoc->m_ProjectDirectory +"export_path_v_over_c.txt";
	// 

	export_plt_file_name = m_pDoc->m_ProjectDirectory +"export_path_v_over_c.plt";

	int yrange = 0;
	int xrange = m_TimeRight - m_TimeLeft +1;

	CString PathTitle;

	FILE* st;
	fopen_s(&st,export_file_name,"w");
	if(st!=NULL)
	{
		
	int step_size = 1;
	
	for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(p != m_PathList.GetCurSel())
				continue;



			DTAPath path_element = m_pDoc->m_PathDisplayList[p];
			PathTitle.Format("Path %d %s", p+1,  path_element.m_path_name.c_str () );

			if(path_element.m_LinkVector.size() >=15)
				ytics_stepsize = 4;

			if(path_element.m_LinkVector.size() >=30)
				ytics_stepsize = 10;

				//for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
				//{
				//	fprintf(st,"%s,", m_pDoc->GetTimeStampString24HourFormat (t));
				//}

				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];


					if(pLink != NULL)
					{

						for(int s = 0; s< pLink->m_Length / 0.1; s++)
						{
							 
							CString label = pLink->m_Name .c_str ();

							//if(pLink->m_Name  == "(null)")
							//{
							//label.Format ("%d->%d",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber);
							//}

							//fprintf(st,"%d->%d,%s,%s,", pLink->m_FromNodeNumber , pLink->m_ToNodeNumber , m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.c_str (),label);

							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{
								float LaneClosurePercentage = pLink->GetImpactedFlag(t); // check capacity reduction event;
								float Capacity=  pLink->m_NumberOfLanes * pLink->m_LaneCapacity *(1-	LaneClosurePercentage);		

								float volume  = pLink->GetSimulatedLinkOutVolume (t);
								float voc = volume/max(1,Capacity);
								if(voc>1.01 && m_pDoc->m_traffic_flow_model!=0)  // not BPR function 
									voc = 1.0;
								fprintf(st, "%.2f ", voc);

								if( pLink->m_FromNodeNumber == 30 && pLink->m_ToNodeNumber == 31)
								{
						
								TRACE("\n%f; %f; %f", volume, Capacity, voc);
								}

							}
							fprintf(st,"\n");

							yrange++;
						}

					}


				}

						//last line with zero
							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{

								fprintf(st, "0.00 ");

							}

							fprintf(st,"\n");
			} //for each path
		
	fclose(st);
	}


	FILE* st_plt;
	fopen_s(&st_plt,export_plt_file_name,"w");
	if(st_plt!=NULL)
	{

		CString xtics_str; 

		fprintf(st_plt,"set title \"Dynamic Volume Over Capcity Contour (%s)\" \n", PathTitle);


		fprintf(st_plt,"set xlabel \"Time Horizon\"\n");
		fprintf(st_plt,"set ylabel \"Space (Node Sequence)\"  offset -1\n");

		int xtics_stepsize  = 30;

		if(xrange/xtics_stepsize >20)
			xtics_stepsize = 120;  // 2 hour interval
		else if(xrange/xtics_stepsize >10)
			xtics_stepsize = 60;   // 1 hour interval
		else if(xrange/xtics_stepsize < 5)
			xtics_stepsize = 10;   // 1 hour interval

			 
		for(int t = m_TimeLeft ; t<= m_TimeRight; t+= xtics_stepsize)  
		{
			CString str;
			str.Format("\"%s\" %d ",m_pDoc->GetTimeStampString24HourFormat (t), t-m_TimeLeft);

			if(t+ xtics_stepsize> m_TimeRight ) 
				xtics_str += str;
			else 
			{
				xtics_str += str ;
				xtics_str += "," ;
			}
		}

		fprintf(st_plt,"set xtics (%s) \n",xtics_str);

		CString ytics_str; 

		int yrange_i  = 0;

		CString last_node_number = " ";

		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(p != m_PathList.GetCurSel())
				continue;

			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];

					if(pLink != NULL)
					{

						for(int s = 0; s< pLink->m_Length / 0.1; s++)
						{

							CString label = pLink->m_Name .c_str ();

							last_node_number.Empty ();
							if(pLink->m_Name  == "(null)" || pLink->m_Name.size() ==0)
							{
							label.Format ("%d",pLink->m_FromNodeNumber);

							last_node_number.Format ("%d",pLink->m_ToNodeNumber);
							}

							if(s==0 && (i%ytics_stepsize) ==0)   // first segment 
							{
							CString str;
							str.Format("\"%s\" %d, ",label, yrange_i) ;

							ytics_str += str;
							
							}

							yrange_i++;
						}

					}


				}

				
			} //for each path


		CString str;
		str.Format("\"%s\" %d", last_node_number, yrange_i);
		ytics_str +=str;
		fprintf(st_plt,"set ytics (%s)\n",ytics_str);

		fprintf(st_plt,"set xrange [0:%d] \n",xrange);
		fprintf(st_plt,"set yrange [0:%d] \n",yrange);

		fprintf(st_plt,"set palette defined (0 \"white\", 0.4 \"green\", 0.6 \"yellow\", 1 \"red\")\n");
		fprintf(st_plt,"set pm3d map\n");
		fprintf(st_plt,"splot '%s' matrix\ notitle\n",export_file_name);

		fclose(st_plt);
	}else
	{
	AfxMessageBox("File export_path_v_over_c.plt cannot be opened. Please check");
	}

	HINSTANCE result = ShellExecute(NULL, _T("open"), export_plt_file_name, NULL,NULL, SW_SHOW);
}

void CDlgPathList::OnDataSavecurrentpath()
{
		if(m_pDoc->m_SelectPathNo >=0 && m_pDoc->m_SelectPathNo < m_pDoc->m_PathDisplayList.size())
			m_pDoc->m_PathDisplayList [ m_pDoc->m_SelectPathNo].m_bSavedPath = true;
}

void CDlgPathList::OnDataExportfreevalsegmentfile()
{
	OnBnClickedFreevalAnalysisGenerateFile();
}

void CDlgPathList::OnBnClickedGoogleearth()
{
	m_pDoc->OnExportGenerateshapefilesPathData();
}

void CDlgPathList::OnBnClickedGoogleearthoutput()
{
	m_pDoc->OnExportGenerateshapefilesPathData();
}

void CDlgPathList::OnBnClickedButtonGoogleEarthKml()
{
	CWaitCursor wait;
	m_pDoc->OnExportGenerateshapefilesPathData();
}

void CDlgPathList::OnDataDeleteexistingpathsininputpathcsvfile()
{
	if( AfxMessageBox("Are you sure to delete all the paths in the path file? Click on Yes to continue this task.", MB_YESNO| MB_ICONINFORMATION) == IDYES)
	{
		OnDataCleanallpaths();

		if( m_pDoc->SaveInputPathCSVFile (m_pDoc->m_ProjectDirectory + "optional_path.csv"))
		{
		AfxMessageBox("All paths in file optional_path.csv have been deleted.",  MB_ICONINFORMATION );
		}
	}
}

void CDlgPathList::OnDataViewinputpathfileinexcel()
{
	m_pDoc->OpenCSVFileInExcel (m_pDoc->m_ProjectDirectory + "optional_path.csv");

}

void CDlgPathList::OnDataAddanewpath()
{
	DTAPath path_element;

	path_element.m_path_name  = "New Path";

	m_pDoc->m_PathDisplayList.push_back (path_element);

	m_pDoc->m_SelectPathNo = m_pDoc->m_PathDisplayList.size()- 1;
	ReloadData();


}

void CDlgPathList::OnDataChangepathnameforselectedpath()
{

	CDlg_UserInput dlg;
	dlg.m_StrQuestion  = "Please specify the name of the selected path.";

	if(m_pDoc->m_SelectPathNo < m_pDoc->m_PathDisplayList.size())
	dlg.m_InputValue = m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_path_name .c_str ();


	if(dlg.DoModal ()==IDOK)
	{

		dlg.m_InputValue.Replace(",", "_");
		m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo].m_path_name = dlg.m_InputValue;

	}

	ReloadData();
}

void CDlgPathList::OnDataDeleteselectedpath()
{
	std::vector<DTAPath>	m_TempPathDisplayList = m_pDoc->m_PathDisplayList;

	m_pDoc->m_PathDisplayList.clear();

	for(unsigned int p = 0; p< m_TempPathDisplayList.size(); p++)
	{
	 if(p!= m_pDoc->m_SelectPathNo)
	 {
		 m_pDoc->m_PathDisplayList.push_back (m_TempPathDisplayList[p]);
	 }

	}

	 m_pDoc->m_SelectPathNo = min( m_pDoc->m_SelectPathNo, m_pDoc->m_PathDisplayList.size()-1);
	 	ReloadData();
}

void CDlgPathList::OnLbnDblclkList1()
{
	OnDataChangepathnameforselectedpath();
}


void CDlgPathList::GenerateDynamicMOEContour(DTA_EMISSION_TYPE emission_type )
{
	CString PathTitle, UnitStr;

	int ytics_stepsize  = 1;

	CString export_file_name, export_plt_file_name;

	CString emission_type_str;

			switch(emission_type)
			{
			case DTA_CO2:  emission_type_str = "CO2"; UnitStr = "kg/vehicle/distance unit";
				break;
			case DTA_NOX: emission_type_str = "NoX"; UnitStr = "g/vehicle/distance unit";
				break;
			case DTA_CO: emission_type_str = "CO"; UnitStr = "g/vehicle/distance unit";
				break;
			case DTA_HC:  emission_type_str = "HC"; UnitStr = "g/vehicle/distance unit";
				break;
			}


	export_file_name = m_pDoc->m_ProjectDirectory + "export_path_" + emission_type_str +".txt";

	export_plt_file_name = m_pDoc->m_ProjectDirectory +  "export_path_" + emission_type_str +".plt";


	int yrange = 0;
	int xrange = m_TimeRight - m_TimeLeft +1;

			double min_value = 1000000;
			double max_value = 0;
	FILE* st;
	fopen_s(&st,export_file_name,"w");
	if(st!=NULL)
	{
		
	int step_size = 1;

	
	for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(p != m_PathList.GetCurSel())
				continue;

			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

			PathTitle.Format("Path %d %s", p+1,  path_element.m_path_name.c_str () );

			if(path_element.m_LinkVector.size() >=15)
				ytics_stepsize = 4;

			if(path_element.m_LinkVector.size() >=30)
				ytics_stepsize = 10;

			min_value = 1000000;
			max_value = 0;
				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];


					if(pLink != NULL)
					{

						for(int s = 0; s< pLink->m_Length / 0.1; s++)
						{

							 
							CString label = pLink->m_Name .c_str ();

							//if(pLink->m_Name  == "(null)")
							//{
							//label.Format ("%d->%d",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber);
							//}

							//fprintf(st,"%d->%d,%s,%s,", pLink->m_FromNodeNumber , pLink->m_ToNodeNumber , m_pDoc->m_LinkTypeMap[pLink->m_link_type ].link_type_name.c_str (),label);

							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{

								double value = pLink->GetEmissions (t,emission_type);
								fprintf(st, "%.4f ",value);
								if(value > max_value) max_value = value;
								if(value < max_value) min_value = value;


							}
							fprintf(st,"\n");

							yrange++;
						}

					}


				}

							// last line with zero
							for(int t = m_TimeLeft ; t< m_TimeRight; t+= step_size)  // for each starting time
							{

								fprintf(st, "0.0 ");

							}

							fprintf(st,"\n");

			} //for each path
		
	fclose(st);
	}


	FILE* st_plt;
	fopen_s(&st_plt,export_plt_file_name,"w");
	if(st_plt!=NULL)
	{

		CString xtics_str; 

		fprintf(st_plt,"set title \"Dynamic %s Contour (%s) Unit:%s\" \n", emission_type_str, PathTitle,UnitStr);

		fprintf(st_plt,"set xlabel \"Time Horizon\"\n");
		fprintf(st_plt,"set ylabel \"Space (Node Sequence)\"  offset -1\n");

		int xtics_stepsize  = 30;

		if(xrange/xtics_stepsize >20)
			xtics_stepsize = 120;  // 2 hour interval
		else if(xrange/xtics_stepsize >10)
			xtics_stepsize = 60;   // 1 hour interval
		else if(xrange/xtics_stepsize < 5)
			xtics_stepsize = 10;   // 1 hour interval

			 
		for(int t = m_TimeLeft ; t<= m_TimeRight; t+= xtics_stepsize)  
		{
			CString str;
			str.Format("\"%s\" %d ",m_pDoc->GetTimeStampString24HourFormat (t), t-m_TimeLeft);

			if(t+ xtics_stepsize> m_TimeRight ) 
				xtics_str += str;
			else 
			{
				xtics_str += str ;
				xtics_str += "," ;
			}
		}

		fprintf(st_plt,"set xtics (%s) \n",xtics_str);

		CString ytics_str; 

		int yrange_i  = 0;

		CString last_node_number = " ";

		for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++) // for each path
		{

			if(p != m_PathList.GetCurSel())
				continue;

			DTAPath path_element = m_pDoc->m_PathDisplayList[p];

				for (int i= 0 ; i < path_element.m_LinkVector.size() ; i++)  // for each pass link
				{
					DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[p].m_LinkVector[i]];

					if(pLink != NULL)
					{

						for(int s = 0; s< pLink->m_Length / 0.1; s++)
						{

							CString label = pLink->m_Name .c_str ();


							if(pLink->m_Name  == "(null)" || pLink->m_Name.size() ==0)
							{
							label.Format ("%d",pLink->m_FromNodeNumber);

							last_node_number.Format ("%d",pLink->m_ToNodeNumber);
							}

							if(s==0 && (i%ytics_stepsize) ==0)   // first segment 
							{
							CString str;
							str.Format("\"%s\" %d, ",label, yrange_i) ;

							ytics_str += str;
							
							}

							yrange_i++;
						}

					}


				}

				
			} //for each path


		CString str;
		str.Format("\"%s\" %d", last_node_number, yrange_i);
		ytics_str +=str;
		fprintf(st_plt,"set ytics (%s)\n",ytics_str);

		fprintf(st_plt,"set xrange [0:%d] \n",xrange);
		fprintf(st_plt,"set yrange [0:%d] \n",yrange);

		if(emission_type == DTA_Energy)
			fprintf(st_plt,"set palette defined (0 \"white\", 1 \"red\", 15 \"yellow\", 50 \"green\")\n");
		else
			fprintf(st_plt,"set palette defined (0 \"white\", %f \"green\", %f \"yellow\", %f \"red\")\n",0.2*max_value,0.6*max_value,max_value);


		fprintf(st_plt,"set pm3d map\n");
		fprintf(st_plt,"splot '%s' matrix\ notitle\n",export_file_name);

		fclose(st_plt);
	}else
	{

		CString error_msg;
		error_msg.Format("File %s cannot be opened. Please check",export_plt_file_name);

		
	AfxMessageBox(error_msg);
	}

	HINSTANCE result = ShellExecute(NULL, _T("open"), export_plt_file_name, NULL,NULL, SW_SHOW);
	

}
void CDlgPathList::OnDataGenerateemissiondetourplots()
{

	GenerateDynamicMOEContour(DTA_Energy);
	GenerateDynamicMOEContour(DTA_CO2);
	GenerateDynamicMOEContour(DTA_NOX);
	GenerateDynamicMOEContour(DTA_CO);
	GenerateDynamicMOEContour(DTA_HC);

}

void CDlgPathList::OnDataCo2contourplot()
{
	GenerateDynamicMOEContour(DTA_CO2);
}

void CDlgPathList::OnDataCocontourplot()
{
	GenerateDynamicMOEContour(DTA_CO);
}

void CDlgPathList::OnContourNoxcontourplot()
{
	GenerateDynamicMOEContour(DTA_NOX);
}

void CDlgPathList::OnContourHccontourplot()
{
	GenerateDynamicMOEContour(DTA_HC);
}

void CDlgPathList::OnDataGenerateenergyuse()
{
	GenerateDynamicMOEContour(DTA_Energy);
}

void CDlgPathList::OnBnClickedPathDataExportCsv()
{
	// TODO: Add your control notification handler code here
}


void CDlgPathList::OnChangeattributesforlinksalongpathLoopcode()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_loop_code;
	ChangeLinkAttributeDialog();
}


void CDlgPathList::OnChangeattributesforlinksalongpathDirectioncode()
{
	m_ChangeLinkAttributeMode = eChangeLinkAttribute_orientation_code;
	ChangeLinkAttributeDialog();
}
