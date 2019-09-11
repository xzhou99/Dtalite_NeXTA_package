// Dlg_VehEmissions.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_VehEmissions.h"
#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"
#include "Dlg_VehicleClassification.h"
#define MAX_STRING_LENGTH  100

extern CDlg_VehPathAnalysis* g_pVehiclePathDlg;

// CDlg_VehPathAnalysis dialog

IMPLEMENT_DYNAMIC(CDlg_VehPathAnalysis, CBaseDialog)

CDlg_VehPathAnalysis::CDlg_VehPathAnalysis(CWnd* pParent /*=NULL*/)
: CBaseDialog(CDlg_VehPathAnalysis::IDD, pParent)
, m_SingleVehicleID(0)
, m_VMS_Responsive_Flag(FALSE)
{
	m_SelectedPath = -1;
	m_SelectedOrigin = -1;
	m_SelectedDestination = -1;
	m_ZoneNoSize  = 0;
	m_ODMOEMatrix = NULL;

	m_ProjectSize =  0;
	m_OldProjectSize = 0;

}

CDlg_VehPathAnalysis::~CDlg_VehPathAnalysis()
{
	m_pDoc->m_CriticalOriginZone = -1;
	m_pDoc->m_CriticalDestinationZone  = -1;

	if(m_ODMOEMatrix !=NULL)
		Deallocate3DDynamicArray<VehicleStatistics>(m_ODMOEMatrix,m_ProjectSize, m_pDoc->m_ZoneNoSize );

	std::vector<int> LinkVector;
	// empty vector
	m_pDoc->HighlightPath(LinkVector,1);

}

void CDlg_VehPathAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_PATHLIST, m_PathListCtrl);
	DDX_Control(pDX, IDC_LIST_VEHICLE, m_VehicleList);
	DDX_Control(pDX, IDC_COMBO_Origin, m_OriginBox);
	DDX_Control(pDX, IDC_COMBO_Destination, m_DestinationBox);
	DDX_Control(pDX, IDC_COMBO_DepartureTime, m_DepartureTimeBox);
	DDX_Control(pDX, IDC_COMBO_VehicleType, m_VehicleTypeBox);
	DDX_Control(pDX, IDC_COMBO_InformationClass, m_InformationClassBox);
	DDX_Control(pDX, IDC_COMBO_Min_Number_of_vehicles, m_MinVehicleSizeBox);
	DDX_Control(pDX, IDC_COMBO_Min_Travel_Time, m_MinDistanceBox);
	DDX_Control(pDX, IDC_COMBO_Min_TravelTimeIndex, m_MaxSpeedBox);
	DDX_Control(pDX, IDC_COMBO_TimeInterval, m_TimeIntervalBox);
	DDX_Control(pDX, IDC_COMBO_ImpactLink, m_ImpactLinkBox);
	DDX_Control(pDX, IDC_SUMMARY_INFO, m_Summary_Info_Edit);
	DDX_Control(pDX, IDC_COMBO_VOT_LB, m_ComboBox_VOT_LB);
	DDX_Control(pDX, IDC_COMBO_VOT_UB, m_ComboBox_VOT_UB);
	DDX_Control(pDX, IDC_COMBO_DemandType, m_DemandTypeBox);
	DDX_Control(pDX, IDC_COMBO_DayNo, m_DayNo_Combobox);
	DDX_Check(pDX, IDC_CHECK_VMS_RESPONSIVE, m_VMS_Responsive_Flag);
}


BEGIN_MESSAGE_MAP(CDlg_VehPathAnalysis, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_LINK, &CDlg_VehPathAnalysis::OnLbnSelchangeListLink)
	ON_LBN_SELCHANGE(IDC_LIST_VEHICLE, &CDlg_VehPathAnalysis::OnLbnSelchangeListVehicle)
	ON_LBN_SELCHANGE(IDC_LIST_LINK2, &CDlg_VehPathAnalysis::OnLbnSelchangeListLink2)
	ON_CBN_SELCHANGE(IDC_COMBO_Origin, &CDlg_VehPathAnalysis::OnCbnSelchangeComboOrigin)
	ON_CBN_SELCHANGE(IDC_COMBO_Destination, &CDlg_VehPathAnalysis::OnCbnSelchangeComboDestination)
	ON_CBN_SELCHANGE(IDC_COMBO_DepartureTime, &CDlg_VehPathAnalysis::OnCbnSelchangeComboDeparturetime)
	ON_CBN_SELCHANGE(IDC_COMBO_VehicleType, &CDlg_VehPathAnalysis::OnCbnSelchangeComboVehicletype)
	ON_CBN_SELCHANGE(IDC_COMBO_InformationClass, &CDlg_VehPathAnalysis::OnCbnSelchangeComboInformationclass)
	ON_CBN_SELCHANGE(IDC_COMBO_TimeInterval, &CDlg_VehPathAnalysis::OnCbnSelchangeComboTimeinterval)
	ON_CBN_SELCHANGE(IDC_COMBO_Min_Number_of_vehicles, &CDlg_VehPathAnalysis::OnCbnSelchangeComboMinNumberofvehicles)
	ON_CBN_SELCHANGE(IDC_COMBO_Min_Travel_Time, &CDlg_VehPathAnalysis::OnCbnSelchangeComboMinTravelTime)
	ON_CBN_SELCHANGE(IDC_COMBO_Min_TravelTimeIndex, &CDlg_VehPathAnalysis::OnCbnSelchangeComboMinTraveltimeindex)
	ON_LBN_SELCHANGE(IDC_LIST_OD, &CDlg_VehPathAnalysis::OnLbnSelchangeListOd)
	ON_LBN_SELCHANGE(IDC_LIST_PATH, &CDlg_VehPathAnalysis::OnLbnSelchangeListPath)
	ON_CBN_SELCHANGE(IDC_COMBO_ImpactLink, &CDlg_VehPathAnalysis::OnCbnSelchangeComboImpactlink)
	ON_LBN_DBLCLK(IDC_LIST_OD, &CDlg_VehPathAnalysis::OnLbnDblclkListOd)
	ON_BN_CLICKED(ID_EXPORT, &CDlg_VehPathAnalysis::OnBnClickedExport)
	ON_CBN_SELCHANGE(IDC_COMBO_VOT_LB, &CDlg_VehPathAnalysis::OnCbnSelchangeComboVotLb)
	ON_CBN_SELCHANGE(IDC_COMBO_VOT_UB, &CDlg_VehPathAnalysis::OnCbnSelchangeComboVotUb)
	ON_CBN_SELCHANGE(IDC_COMBO_DemandType, &CDlg_VehPathAnalysis::OnCbnSelchangeComboDemandtype)
	ON_BN_CLICKED(ID_EXPORT_PATH_DATA, &CDlg_VehPathAnalysis::OnBnClickedExportPathData)
	ON_BN_CLICKED(ID_EXPORT_VEHICLE_DATA, &CDlg_VehPathAnalysis::OnBnClickedExportVehicleData)
	ON_BN_CLICKED(ID_FindCriticalOD, &CDlg_VehPathAnalysis::OnBnClickedFindcriticalod)
	ON_BN_CLICKED(IDOK, &CDlg_VehPathAnalysis::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CDlg_VehPathAnalysis::OnLvnItemchangedList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PATHLIST, &CDlg_VehPathAnalysis::OnPathLvnItemchangedList)
	ON_CBN_SELCHANGE(IDC_COMBO_DayNo, &CDlg_VehPathAnalysis::OnCbnSelchangeComboDayno)
	ON_BN_CLICKED(ID_BarChart, &CDlg_VehPathAnalysis::OnBnClickedBarchart)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_VMS_RESPONSIVE, &CDlg_VehPathAnalysis::OnBnClickedCheckVmsResponsive)
END_MESSAGE_MAP()


// CDlg_VehPathAnalysis message handlers


BOOL CDlg_VehPathAnalysis::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;

	m_ImpactLinkBox.AddString ("N/A");
	std::list<DTALink*>::iterator iLink;
	for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
	{
		if((*iLink) ->CapacityReductionVector.size()>0)
		{
			str.Format ("%d->%d, Incident/Work Zone", (*iLink) ->m_FromNodeNumber,(*iLink) ->m_ToNodeNumber );
			m_ImpactLinkBox.AddString (str);

		}
	}

	for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
	{
		if((*iLink) ->MessageSignVector.size()>0)
		{
			str.Format ("%d->%d, VMS", (*iLink) ->m_FromNodeNumber,(*iLink) ->m_ToNodeNumber );
			m_ImpactLinkBox.AddString (str);

		}
	}
		


	for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
	{
		if((*iLink) ->TollVector.size()>0)
		{
			str.Format ("%d->%d, Toll", (*iLink) ->m_FromNodeNumber,(*iLink) ->m_ToNodeNumber );
			m_ImpactLinkBox.AddString (str);

		}
	}

	if(m_pDoc->m_SelectedLinkNo >=0)
	{
		DTALink* pLink= m_pDoc->m_LinkNoMap [m_pDoc->m_SelectedLinkNo];

			str.Format ("%d->%d, Selected Link", pLink ->m_FromNodeNumber,pLink ->m_ToNodeNumber );
			m_ImpactLinkBox.AddString (str);

	}

	m_ImpactLinkBox.SetCurSel (0);

	m_OriginBox.AddString("All");
	m_DestinationBox.AddString("All");

	int i;
	for(i=0; i <  m_pDoc->m_ZoneNoSize  ; i++)
	{
		str.Format ("%d", m_pDoc->m_ZoneNumberVector [i]);
		m_TAZVector.push_back (m_pDoc->m_ZoneNumberVector [i]);
		m_OriginBox.AddString(str);
		m_DestinationBox.AddString(str);
	}
	m_OriginBox.SetCurSel (0);
	m_DestinationBox.SetCurSel (0);



	std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	int project_index = 0 ;
	while (iDoc != g_DocumentList.end())
	{
		if((*iDoc)->m_ProjectTitle.GetLength () >0)  
		{

		project_index++;
		}
		iDoc++;
	}

	m_ProjectSize = max(1,project_index);



	m_VehicleTypeBox.AddString ("All");
	
	for(i = 0; i< m_pDoc->m_VehicleTypeVector.size(); i++)
	{
	m_VehicleTypeBox.AddString (m_pDoc->m_VehicleTypeVector[i].vehicle_type_name);
	}

	m_VehicleTypeBox.SetCurSel(0);

	m_DemandTypeBox.AddString("All");
	for(i = 0; i< m_pDoc->m_DemandTypeVector .size(); i++)
	{
	m_DemandTypeBox.AddString (m_pDoc->m_DemandTypeVector[i].demand_type_name);
	}
	m_DemandTypeBox.SetCurSel(0);

	m_InformationClassBox.AddString ("All");
	m_InformationClassBox.AddString ("Historical info");
	m_InformationClassBox.AddString ("Pretrip info");
	m_InformationClassBox.AddString ("En-route info");
	m_InformationClassBox.AddString("Personalized");
	m_InformationClassBox.AddString("Eco-SO info");


	m_InformationClassBox.SetCurSel (0);


	for(i=0; i <= 1440 ; i+=15)
	{
		str.Format ("%d (%s)", i,m_pDoc->GetTimeStampString24HourFormat (i));
		m_DepartureTimeBox.AddString(str);
	}
	m_DepartureTimeBox.SetCurSel (0);

	for(i=0; i<100; i++)
	{
		str.Format ("%d", i);
		m_DayNo_Combobox.AddString(str);

	}

	m_DayNo_Combobox.SetCurSel (0);

	m_TimeIntervalBox.AddString("1440");
	m_TimeIntervalBox.AddString("15");
	m_TimeIntervalBox.AddString("30");
	m_TimeIntervalBox.AddString("60");
	m_TimeIntervalBox.AddString("120");
	m_TimeIntervalBox.AddString("240");
	m_TimeIntervalBox.AddString("480");
	m_TimeIntervalBox.SetCurSel (0);

	m_MinVehicleSizeBox.AddString ("0");
	m_MinVehicleSizeBox.AddString ("2");
	m_MinVehicleSizeBox.AddString ("5");
	m_MinVehicleSizeBox.AddString ("10");
	m_MinVehicleSizeBox.AddString ("20");
	m_MinVehicleSizeBox.AddString ("50");
	m_MinVehicleSizeBox.AddString ("75");
	m_MinVehicleSizeBox.AddString ("100");
	m_MinVehicleSizeBox.AddString ("200");
	m_MinVehicleSizeBox.AddString ("500");

	if(m_pDoc->m_ZoneNoSize <100)
		m_MinVehicleSizeBox.SetCurSel (1);  //2
	else if (m_pDoc->m_ZoneNoSize <200)
		m_MinVehicleSizeBox.SetCurSel (2);  // 5
	else
		m_MinVehicleSizeBox.SetCurSel (4); // 20


	m_MinDistanceBox.AddString ("0");
	m_MinDistanceBox.AddString ("1");
	m_MinDistanceBox.AddString ("2");
	m_MinDistanceBox.AddString ("3");
	m_MinDistanceBox.AddString ("4");
	m_MinDistanceBox.AddString ("5");
	m_MinDistanceBox.AddString ("10");
	m_MinDistanceBox.AddString ("15");
	m_MinDistanceBox.AddString ("20");
	m_MinDistanceBox.AddString ("30");
	m_MinDistanceBox.AddString ("50");
	m_MinDistanceBox.AddString ("100");
	m_MinDistanceBox.SetCurSel (0);

    m_MaxSpeedBox.AddString("300"); 
	m_MaxSpeedBox.AddString("200"); 
	m_MaxSpeedBox.AddString("150"); 
    m_MaxSpeedBox.AddString("100"); 
	m_MaxSpeedBox.AddString("80"); 
	m_MaxSpeedBox.AddString("60"); 
	m_MaxSpeedBox.AddString("50"); 
	m_MaxSpeedBox.AddString("40"); 
	m_MaxSpeedBox.AddString("30"); 
	m_MaxSpeedBox.AddString("20"); 
	m_MaxSpeedBox.AddString("10"); 
	m_MaxSpeedBox.AddString("5"); 
	m_MaxSpeedBox.SetCurSel (0);

	for(int vot = 0; vot<=100; vot+=5)
	{
		str.Format ("%d", vot);
		m_ComboBox_VOT_LB.AddString (str);
		m_ComboBox_VOT_UB.AddString (str);
	}

	m_ComboBox_VOT_LB.SetCurSel (0);
	m_ComboBox_VOT_UB.SetCurSel (20);

	// initialize List Control
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);

	// initialize Path List Control
	m_PathListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTraitPath = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_PathListCtrl.SetDefaultRowTrait(pRowTraitPath);

	std::vector<std::string> m_Column_names;

	std::vector<CString> ColumnLabelVector;

	ColumnLabelVector.push_back ("Origin Zone");
	ColumnLabelVector.push_back ("Destination Zone");

	m_GPS_start_day = 3;
	m_GPS_end_day = 17;


	project_index = 0 ;

	iDoc = g_DocumentList.begin ();
	while (iDoc != g_DocumentList.end())
	{
		if((*iDoc)->m_ProjectTitle.GetLength () >0)  
		{

			CString str;
			str.Format ("%s Count:", (*iDoc)->m_ProjectTitle);
			ColumnLabelVector.push_back (str);
			ColumnLabelVector.push_back ("Avg Travel Time");
			ColumnLabelVector.push_back ("Avg Distance");
			ColumnLabelVector.push_back ("Avg Speed");
			ColumnLabelVector.push_back ("TT STD");
			ColumnLabelVector.push_back ("Travel Time Per Distance STD");


			if((*iDoc)-> m_bEmissionDataAvailable)
			{
			ColumnLabelVector.push_back ("Energy (KJ)");
			ColumnLabelVector.push_back ("CO2 (g)");
			ColumnLabelVector.push_back ("NOx (g)");
			ColumnLabelVector.push_back ("CO (g)");
			ColumnLabelVector.push_back ("HC (g)");
			ColumnLabelVector.push_back ("Miles Per Gallon");
			}

			if((*iDoc)-> m_bGPSDataSet)
			{

				for(int day = m_GPS_start_day; day<= m_GPS_end_day; day++)
				{
				
				CString day_count_str;
				day_count_str.Format ("day %d:Count", day);
				ColumnLabelVector.push_back (day_count_str);

				}

				ColumnLabelVector.push_back ("Count Range/Mean Ratio");

			}

			if(project_index>=1)
			{
				ColumnLabelVector.push_back ("Diff of Distance");
				ColumnLabelVector.push_back ("Diff of Travel Time");
			}

			project_index++;
		}
		iDoc++;
	}


	//Add Columns and set headers
	for (unsigned i=0;i< ColumnLabelVector.size();i++)
	{
		CGridColumnTraitText* pTrait = NULL;

//		pTrait = new CGridColumnTraitEdit();
		m_ListCtrl.InsertColumnTrait((int)i,ColumnLabelVector[i],LVCFMT_LEFT,-1,-1, pTrait);
//		pTrait->SetSortFormatNumber(true);	
//		m_ListCtrl.SetColumnWidth((int)i,LVSCW_AUTOSIZE_USEHEADER);
		m_ListCtrl.SetColumnWidth((int)i,80);
	}

	if(m_ODMOEMatrix == NULL  && m_pDoc->m_ZoneNoSize >0 )
	{
		m_ODMOEMatrix = Allocate3DDynamicArray<VehicleStatistics>(m_ProjectSize,m_pDoc->m_ZoneNoSize ,m_pDoc->m_ZoneNoSize );
			m_ZoneNoSize  = m_pDoc->m_ZoneNoSize ;
			m_OldProjectSize = m_ProjectSize;

	}
	else
	{
		if(  m_ZoneNoSize >0  && (m_ZoneNoSize !=  m_pDoc->m_ZoneNoSize  || m_OldProjectSize != m_ProjectSize))
		{
			Deallocate3DDynamicArray<VehicleStatistics>(m_ODMOEMatrix,m_OldProjectSize, m_ZoneNoSize );
			m_ODMOEMatrix = Allocate3DDynamicArray<VehicleStatistics>(m_ProjectSize,m_pDoc->m_ZoneNoSize ,m_pDoc->m_ZoneNoSize );
			m_OldProjectSize = m_ProjectSize;
			m_ZoneNoSize  = m_pDoc->m_ZoneNoSize ;
	
		}
	}
	
	std::vector<CString> ColumnPathLabelVector;

	ColumnPathLabelVector.push_back ("Path No");
	ColumnPathLabelVector.push_back ("Count");
	ColumnPathLabelVector.push_back ("Percentage");
	ColumnPathLabelVector.push_back ("Travel Time (min)");
	ColumnPathLabelVector.push_back ("Distance");
	ColumnPathLabelVector.push_back ("Speed");
	ColumnPathLabelVector.push_back ("Toll Cost($)");
	ColumnPathLabelVector.push_back ("Travel Time STD (min)");
	ColumnPathLabelVector.push_back ("Travel Time Per Mile STD (min/distance)");


	if(m_pDoc-> m_bEmissionDataAvailable)
	{
	ColumnPathLabelVector.push_back ("Energy (KJ)");
	ColumnPathLabelVector.push_back ("CO2 (g)");
	ColumnPathLabelVector.push_back ("NOx (g)");
	ColumnPathLabelVector.push_back ("CO (g)");
	ColumnPathLabelVector.push_back ("HC (g)");
	ColumnPathLabelVector.push_back ("Miles Per Gallon");
	}
	
//	ColumnPathLabelVector.push_back ("Day No");

	for (unsigned i=0;i< ColumnPathLabelVector.size();i++)
	{
		CGridColumnTraitText* pTrait = NULL;

		m_PathListCtrl.InsertColumnTrait((int)i,ColumnPathLabelVector[i],LVCFMT_LEFT,-1,-1, pTrait);
		m_PathListCtrl.SetColumnWidth((int)i,80);
	}
	FilterOriginDestinationPairs();



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_VehPathAnalysis::FilterOriginDestinationPairs()
{
	m_pDoc->ResetODMOEMatrix();


	m_ListCtrl.DeleteAllItems();

	if(m_ZoneNoSize=0)
		return;

	UpdateData(1);

	CString SummaryInfoString;

	CWaitCursor wait;
	int i,j;
	int p;

	if(m_ODMOEMatrix ==NULL)
		return;

	for(p=0;p<m_ProjectSize; p++)
	{
	for(i=0; i < m_pDoc->m_ZoneNoSize  ; i++)
		for(j=0; j< m_pDoc->m_ZoneNoSize  ; j++)
		{
			m_ODMOEMatrix[p][i][j].Reset ();
		}
	}

	int ImpactLinkNo = -1;
	if(m_ImpactLinkBox.GetCurSel()>0)
	{
		char m_Text[MAX_STRING_LENGTH];
		int FromNodeNumber, ToNodeNumber;
		m_ImpactLinkBox.GetLBText (m_ImpactLinkBox.GetCurSel(), m_Text);
		sscanf(m_Text, "%d->%d", &FromNodeNumber, &ToNodeNumber);
		DTALink* pLink = m_pDoc->FindLinkWithNodeNumbers(FromNodeNumber, ToNodeNumber);
		if(pLink!=NULL)
			ImpactLinkNo = pLink->m_LinkNo;
	}


		int Origin_Row = m_OriginBox.GetCurSel();

		int Destination_Row = m_DestinationBox.GetCurSel();

		int Origin = 0;
		int Destination = 0;
		if(Origin_Row > 0)
		{
		Origin = m_TAZVector[Origin_Row-1];
		}

		if(Destination_Row > 0)
		{
		Destination = m_TAZVector[Destination_Row-1];
		}

		//Origin = 1737;
		//Destination = 1767;

		int VehicleType = m_VehicleTypeBox.GetCurSel();
		int DemandType = m_DemandTypeBox.GetCurSel();

		int InformationClass = m_InformationClassBox.GetCurSel();

		char str[MAX_STRING_LENGTH];
		m_DepartureTimeBox.GetLBText(m_DepartureTimeBox.GetCurSel(), str);
		int DepartureTime = atoi(str);

		m_TimeIntervalBox.GetLBText(m_TimeIntervalBox.GetCurSel(), str);
		int TimeInterval = atoi(str);

		m_MinVehicleSizeBox.GetLBText(m_MinVehicleSizeBox.GetCurSel(), str);
		int MinVehicleSize = atoi(str);

		m_MinDistanceBox.GetLBText(m_MinDistanceBox.GetCurSel(), str);
		int MinDistance = atoi(str);

		m_MaxSpeedBox.GetLBText(m_MaxSpeedBox.GetCurSel(), str);
		float MaxSpeed = atof(str);

		int VOT_LB = m_ComboBox_VOT_LB.GetCurSel()*5;
		int VOT_UB = m_ComboBox_VOT_UB.GetCurSel()*5;

		int DayNo = m_DayNo_Combobox.GetCurSel();

		int count = 0;
		std::list<DTAVehicle*>::iterator iVehicle;


		

	std::list<CTLiteDoc*>::iterator  iDoc = g_DocumentList.begin ();

	 p = 0 ;
	while (iDoc != g_DocumentList.end())
	{
		if((*iDoc)->m_ProjectTitle.GetLength () >0)  
		{

			if(p >=m_ProjectSize)  // additional project loaded. 
				continue; 

		for (iVehicle = (*iDoc)->m_VehicleSet.begin(); iVehicle != (*iDoc)->m_VehicleSet.end(); iVehicle++, count++)
		{
			DTAVehicle* pVehicle = (*iVehicle);

			if(pVehicle->m_OriginZoneID>= (*iDoc)->m_ZoneIDVector.size() || pVehicle->m_DestinationZoneID >=(*iDoc)->m_ZoneIDVector.size())
			{
			AfxMessageBox("Please close and reload the data set, as the zone vector in the current document has been changed due to additional zones.");
			return;
			}

			if(m_VMS_Responsive_Flag == true)
			{
			
				if(pVehicle->m_number_of_VMS_response_links==0)
					continue;
		
			}

			int OrgNo = (*iDoc)->m_ZoneIDVector[pVehicle->m_OriginZoneID];
			int DesNo = (*iDoc)->m_ZoneIDVector[pVehicle->m_DestinationZoneID];


			if(OrgNo>=0 && DesNo >=0  && pVehicle->m_bComplete && (pVehicle->m_VOT >= VOT_LB && pVehicle->m_VOT < VOT_UB) )  // with physical path in the network
			{
				if( 
					(pVehicle->m_DayNo == DayNo || DayNo == 0 ) &&
					(pVehicle->m_OriginZoneID == Origin ||Origin ==0)&&
					(pVehicle->m_DestinationZoneID  == Destination ||Destination ==0)&&
					(pVehicle->m_DemandType  == DemandType ||DemandType ==0)&&
					(pVehicle->m_VehicleType  == VehicleType ||VehicleType ==0)&&
					(pVehicle->m_InformationClass  == InformationClass ||InformationClass ==0)&&
					(pVehicle->m_DepartureTime >= DepartureTime && pVehicle->m_DepartureTime <= DepartureTime+TimeInterval))
				{

					m_ODMOEMatrix[p][OrgNo][DesNo].TotalVehicleSize+=1;

					if( (*iDoc)->m_bGPSDataSet )
					{
					m_ODMOEMatrix[p][OrgNo][DesNo].day_count[pVehicle->m_DayNo ]+=1;
					}

					m_ODMOEMatrix[p][OrgNo][DesNo].TotalTravelTime += (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime);
					m_ODMOEMatrix[p][OrgNo][DesNo].TotalDistance += pVehicle->m_Distance;
					m_ODMOEMatrix[p][OrgNo][DesNo].TotalCost += pVehicle->m_TollDollarCost;
					
					m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.Energy += pVehicle->m_EmissionData .Energy;
					m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.CO2 += pVehicle->m_EmissionData .CO2;
					m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.NOX += pVehicle->m_EmissionData .NOX;
					m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.CO += pVehicle->m_EmissionData .CO;
					m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.HC += pVehicle->m_EmissionData .HC;

					if(ImpactLinkNo>=0)
					{
					for(int link= 1; link<pVehicle->m_NodeSize; link++)
					{
						if ( pVehicle->m_NodeAry[link].LinkNo  == ImpactLinkNo)
						{
							m_ODMOEMatrix[p][OrgNo][DesNo].bImpactFlag = true;
						}

					}
					}
				}
			}

		}

		count = 0;
//		m_ODList.ResetContent ();

		// variability measure
		for (iVehicle = (*iDoc)->m_VehicleSet.begin(); iVehicle != (*iDoc)->m_VehicleSet.end(); iVehicle++, count++)
		{
			DTAVehicle* pVehicle = (*iVehicle);
			int OrgNo = (*iDoc)->m_ZoneIDVector[pVehicle->m_OriginZoneID];
			int DesNo = (*iDoc)->m_ZoneIDVector[pVehicle->m_DestinationZoneID];


			if(OrgNo>=0 && DesNo >=0 /*pVehicle->m_NodeSize >= 2 && */ && pVehicle->m_bComplete && (pVehicle->m_VOT >= VOT_LB && pVehicle->m_VOT < VOT_UB) )  // with physical path in the network
			{
				if( 
					(pVehicle->m_DayNo == DayNo || DayNo == 0 ) &&
					(pVehicle->m_OriginZoneID == Origin ||Origin ==0)&&
					(pVehicle->m_DestinationZoneID  == Destination ||Destination ==0)&&
					(pVehicle->m_DemandType  == DemandType ||DemandType ==0)&&
					(pVehicle->m_VehicleType  == VehicleType ||VehicleType ==0)&&
					(pVehicle->m_InformationClass  == InformationClass ||InformationClass ==0)&&
					(pVehicle->m_DepartureTime >= DepartureTime && pVehicle->m_DepartureTime <= DepartureTime+TimeInterval))
				{
					float AvgTravelTime = m_ODMOEMatrix[p][OrgNo][DesNo].TotalTravelTime/max(1,m_ODMOEMatrix[p][OrgNo][DesNo].TotalVehicleSize );

					m_ODMOEMatrix[p][OrgNo][DesNo].AvgDistance = m_ODMOEMatrix[p][OrgNo][DesNo].TotalDistance /max(1,m_ODMOEMatrix[p][OrgNo][DesNo].TotalVehicleSize );;
					m_ODMOEMatrix[p][OrgNo][DesNo].AvgTravelTime = AvgTravelTime;
					m_ODMOEMatrix[p][OrgNo][DesNo].TotalTravelTimeVariance  += (pVehicle->m_TripTime - AvgTravelTime)*(pVehicle->m_TripTime- AvgTravelTime);
					m_ODMOEMatrix[p][OrgNo][DesNo].TotalTravelTimePerMileVariance  += (pVehicle->m_TripTime - AvgTravelTime)*(pVehicle->m_TripTime- AvgTravelTime)/max(0.01,pVehicle->m_Distance * pVehicle->m_Distance);
				}
			}
		}

		p++;
		}
		iDoc++;
	}


		VehicleStatistics total_summary;
		count = 0;


		m_SelectedOrigin = -1;
		m_SelectedDestination = -1;

		int row_index = 0;
		for(i=0; i < m_pDoc->m_ZoneNoSize  ; i++)
			for(j=0; j< m_pDoc->m_ZoneNoSize  ; j++)
			{

		int Index = 0;

		std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
		 p = 0 ;
		while (iDoc != g_DocumentList.end())
		{
			if((*iDoc)->m_ProjectTitle.GetLength () >0)  
			{

				
				if(  m_ODMOEMatrix[0][i][j].TotalVehicleSize>0 && 
					(ImpactLinkNo<0 || (ImpactLinkNo >=0 && m_ODMOEMatrix[0][i][j].bImpactFlag == true)))
				{
					float AvgDistance = m_ODMOEMatrix[p][i][j].TotalDistance /m_ODMOEMatrix[p][i][j].TotalVehicleSize;
					float AvgTravelTime = m_ODMOEMatrix[p][i][j].TotalTravelTime /m_ODMOEMatrix[p][i][j].TotalVehicleSize;
					float AvgCost = m_ODMOEMatrix[p][i][j].TotalCost  /m_ODMOEMatrix[p][i][j].TotalVehicleSize;
					float STDTravelTime = sqrt(m_ODMOEMatrix[p][i][j].TotalTravelTimeVariance   /m_ODMOEMatrix[p][i][j].TotalVehicleSize);
					float STDTravelTimePerMile = sqrt(m_ODMOEMatrix[p][i][j].TotalTravelTimePerMileVariance   /m_ODMOEMatrix[p][i][j].TotalVehicleSize);

					float AvgEnergy = m_ODMOEMatrix[p][i][j].emissiondata .Energy / m_ODMOEMatrix[p][i][j].TotalVehicleSize;
					float AvgCO2 = m_ODMOEMatrix[p][i][j].emissiondata.CO2  / m_ODMOEMatrix[p][i][j].TotalVehicleSize;
					float AvgNOX = m_ODMOEMatrix[p][i][j].emissiondata.NOX  / m_ODMOEMatrix[p][i][j].TotalVehicleSize;
					float AvgCO = m_ODMOEMatrix[p][i][j].emissiondata.CO  / m_ODMOEMatrix[p][i][j].TotalVehicleSize;
					float AvgHC = m_ODMOEMatrix[p][i][j].emissiondata.HC  / m_ODMOEMatrix[p][i][j].TotalVehicleSize;

				double Gasoline = AvgEnergy/1000/(121.7);  // convert energy from KJ to mega joules  then to gasline per gallon
				double MilesPerGallon = AvgDistance/max(0.1,Gasoline);

					float AvgSpeed = AvgDistance * 60 / max(0.1,AvgTravelTime);  // mph
					if(m_ODMOEMatrix[p][i][j].TotalVehicleSize >= MinVehicleSize && 
						AvgDistance >= MinDistance && AvgSpeed <=MaxSpeed)
					{

						CString ODInfoString;

						//if(m_pDoc->m_EmissionDataFlag )
						//	ODInfoString.Format ("%d->%d: %d vhc, %3.1f min, %3.1f mile, %.0f mph,$%4.3f, %5.1f(J), %4.1f(CO2_g) ",i,j,m_ODMOEMatrix[p][i][j].TotalVehicleSize, AvgTravelTime,AvgDistance,AvgSpeed, AvgCost,AvgEnergy, AvgCO2);
						//else
						//	ODInfoString.Format ("%d->%d: %d vhc, %3.1f min, %3.1f mile, %.0f mph, $%4.3f",i,j,m_ODMOEMatrix[p][i][j].TotalVehicleSize, AvgTravelTime,AvgDistance,AvgSpeed,AvgCost);


		int column_index  = 0;
		char text[100];

		if(p==0)
		{
		sprintf_s(text, "%d",m_pDoc->m_ZoneNumberVector [i]);
		Index = m_ListCtrl.InsertItem(LVIF_TEXT,row_index++,text , 0, 0, 0, NULL);
		column_index++;

		sprintf_s(text, "%d",m_pDoc->m_ZoneNumberVector [j]);
		m_ListCtrl.SetItemText(Index,column_index++,text );


		}


		if(p==0)
			column_index = 2;
		else
			column_index = 8 + (p-1)*8 ;  // -1 no diff for base line

		sprintf_s(text, "%d",m_ODMOEMatrix[p][i][j].TotalVehicleSize);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f",AvgTravelTime);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f",AvgDistance);
		m_ListCtrl.SetItemText(Index,column_index++,text );


		sprintf_s(text, "%3.1f",AvgSpeed);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f",STDTravelTime);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f",STDTravelTimePerMile);
		m_ListCtrl.SetItemText(Index,column_index++,text );


	if(m_pDoc-> m_bEmissionDataAvailable)
	{

		sprintf_s(text, "%3.1f",AvgEnergy);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f",AvgCO2);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.4f",AvgNOX);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.4f",AvgCO);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.4f",AvgHC);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f",MilesPerGallon);
		m_ListCtrl.SetItemText(Index,column_index++,text );

	}	
		
		if((*iDoc)-> m_bGPSDataSet)
			{

				int min = 10000;
				int max = 0;
				int total = 0;
				for(int day = m_GPS_start_day; day<=m_GPS_end_day; day++)
				{

					if(m_ODMOEMatrix[p][i][j].day_count [day] > max)
						max = m_ODMOEMatrix[p][i][j].day_count [day];

					if(m_ODMOEMatrix[p][i][j].day_count [day] < min)
						min = m_ODMOEMatrix[p][i][j].day_count [day];

					total +=  m_ODMOEMatrix[p][i][j].day_count [day];
				
				sprintf_s(text, "%d",m_ODMOEMatrix[p][i][j].day_count [day]);
				m_ListCtrl.SetItemText(Index,column_index++,text );

				}

				sprintf_s(text, "%.3f",(max-min)*1.0f/max(1,total)*(m_GPS_end_day+1-m_GPS_start_day));
				m_ListCtrl.SetItemText(Index,column_index++,text );
			
			}

		if(p>=1)
		{
		sprintf_s(text, "%3.1f",m_ODMOEMatrix[p][i][j].AvgDistance - m_ODMOEMatrix[0][i][j].AvgDistance);
		m_ListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f",m_ODMOEMatrix[p][i][j].AvgTravelTime - m_ODMOEMatrix[0][i][j].AvgTravelTime);
		m_ListCtrl.SetItemText(Index,column_index++,text );
		}


		count ++;

		if(count>=50000)
			break;
//
//						if(ImpactLinkNo<0)  // no impact link is selected
//						{
//						if(count<10000)
//						{
//							total_summary.TotalVehicleSize +=m_ODMOEMatrix[p][i][j].TotalVehicleSize;
//							total_summary.TotalTravelTime +=m_ODMOEMatrix[p][i][j].TotalTravelTime;
//							total_summary.TotalDistance +=m_ODMOEMatrix[p][i][j].TotalDistance;
//							total_summary.TotalCost +=m_ODMOEMatrix[p][i][j].TotalCost;
//							total_summary.emissiondata .Energy += m_ODMOEMatrix[p][i][j].emissiondata .Energy ;
//							total_summary.emissiondata .CO2  += m_ODMOEMatrix[p][i][j].emissiondata .CO2  ;
//
////							m_ODList.AddString (ODInfoString);
//						}
//
//						}else
//						{ // ImpactLinkNo>0: impact link is selected
//						
//						if(count<10000 && m_ODMOEMatrix[p][i][j].bImpactFlag == true)
//						{
//							total_summary.TotalVehicleSize +=m_ODMOEMatrix[p][i][j].TotalVehicleSize;
//							total_summary.TotalTravelTime +=m_ODMOEMatrix[p][i][j].TotalTravelTime;
//							total_summary.TotalDistance +=m_ODMOEMatrix[p][i][j].TotalDistance;
//							total_summary.TotalCost  +=m_ODMOEMatrix[p][i][j].TotalCost;
//							total_summary.emissiondata .Energy += m_ODMOEMatrix[p][i][j].emissiondata .Energy ;
//							total_summary.emissiondata .CO2  += m_ODMOEMatrix[p][i][j].emissiondata .CO2  ;
//
////							m_ODList.AddString (ODInfoString);
//						}

					
						}
					} // OD demand > 0 
			
		p++;
		}
			iDoc++;
		} //document
		}

		if(count < 50000)
		SummaryInfoString.Format("%d OD pair(s) selected.",count);
		else
		SummaryInfoString.Format("Up to %d OD pairs listed.",count);


		m_Summary_Info_Edit.SetWindowText (SummaryInfoString);

			FilterPaths();
			ShowSelectedPath();
			ShowVehicles();
}
void CDlg_VehPathAnalysis::FilterPaths()
{
	m_PathVector.clear();
	m_VehicleList.ResetContent ();
	m_PathListCtrl.DeleteAllItems();  // OD changed, refresh

	UpdateData(1);

	int Origin = m_SelectedOrigin;
	int Destination = m_SelectedDestination;

	int VehicleType = m_VehicleTypeBox.GetCurSel();
	int DemandType = m_DemandTypeBox.GetCurSel();
	
	int InformationClass = m_InformationClassBox.GetCurSel();

	char str[50];
	m_DepartureTimeBox.GetLBText(m_DepartureTimeBox.GetCurSel(), str);
	int DepartureTime = atoi(str);

	m_TimeIntervalBox.GetLBText(m_TimeIntervalBox.GetCurSel(), str);
	int TimeInterval = atoi(str);

	m_MinVehicleSizeBox.GetLBText(m_MinVehicleSizeBox.GetCurSel(), str);
	int MinVehicleSize = atoi(str);

	m_MinDistanceBox.GetLBText(m_MinDistanceBox.GetCurSel(), str);
	int MinDistance = atoi(str);


		int VOT_LB = m_ComboBox_VOT_LB.GetCurSel()*5;
		int VOT_UB = m_ComboBox_VOT_UB.GetCurSel()*5;
		int DayNo = m_DayNo_Combobox.GetCurSel();

	int count = 0;
	int vehicle_count = 0;
	std::list<DTAVehicle*>::iterator iVehicle;

	for (iVehicle = m_pDoc->m_VehicleSet.begin(); iVehicle != m_pDoc->m_VehicleSet.end(); iVehicle++, count++)
	{
		DTAVehicle* pVehicle = (*iVehicle);
			pVehicle->m_bODMarked = false;

		if(pVehicle->m_NodeSize >= 2 && pVehicle->m_bComplete &&(pVehicle->m_VOT >= VOT_LB && pVehicle->m_VOT < VOT_UB))  // with physical path in the network
		{
			if( 
				(pVehicle->m_DayNo == DayNo || DayNo == 0 ) &&
				(pVehicle->m_OriginZoneID == Origin)&&
				(pVehicle->m_DestinationZoneID  == Destination)&&
				(pVehicle->m_VehicleType  == VehicleType ||VehicleType ==0)&&
				(pVehicle->m_DemandType  == DemandType ||DemandType ==0)&&
				(pVehicle->m_InformationClass  == InformationClass ||InformationClass ==0)&&
				(pVehicle->m_DepartureTime >= DepartureTime && pVehicle->m_DepartureTime <= DepartureTime+TimeInterval))
			{

				if(m_VMS_Responsive_Flag == true)
				{
				
					if(pVehicle->m_number_of_VMS_response_links==0)
						continue;
			
				}


				bool bFingFlag =  false;



			pVehicle->m_bODMarked = true;

				for(int p = 0; p< m_PathVector.size(); p++)
				{
					

					//existing path
					if(pVehicle->m_NodeNumberSum == m_PathVector[p].NodeNumberSum  && pVehicle->m_NodeSize == m_PathVector[p].NodeSize )
					{
						
						m_PathVector[p].date_id = pVehicle->m_DayNo ;
						m_PathVector[p].departure_time_in_min = pVehicle->m_DepartureTime ;
						m_PathVector[p].TotalVehicleSize+=1;
						m_PathVector[p].TotalTravelTime  += (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime);
						m_PathVector[p].TotalDistance   += pVehicle->m_Distance;

						m_PathVector[p].m_TravelTimeVector.push_back(pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime);
						m_PathVector[p].m_TravelTimePerMileVector.push_back((pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime)/max(0.01,pVehicle->m_Distance));

						m_PathVector[p].TotalCost   += pVehicle->m_TollDollarCost;
						m_PathVector[p].TotalPM  += pVehicle->m_PM;
					
						m_PathVector[p].emissiondata.Energy += pVehicle->m_EmissionData .Energy;
						m_PathVector[p].emissiondata.CO2 += pVehicle->m_EmissionData .CO2;
						m_PathVector[p].emissiondata.NOX += pVehicle->m_EmissionData .NOX;
						m_PathVector[p].emissiondata.CO += pVehicle->m_EmissionData .CO;
						m_PathVector[p].emissiondata.HC += pVehicle->m_EmissionData .HC;
						vehicle_count ++;

						m_PathVector[p].m_VehicleVector.push_back(pVehicle);
						bFingFlag = true;
						break;
					}
				}
				if(bFingFlag == false)
				{
					// new path
					PathStatistics ps_element;
					ps_element.NodeNumberSum = pVehicle->m_NodeNumberSum;
					ps_element.NodeSize = pVehicle->m_NodeSize;
					ps_element.date_id = pVehicle->m_DayNo ;
					ps_element.departure_time_in_min = pVehicle->m_DepartureTime ;
					ps_element.TotalVehicleSize = 1;
					ps_element.TotalTravelTime  += (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime);
					ps_element.TotalDistance   += pVehicle->m_Distance;
					ps_element.TotalCost    += pVehicle->m_TollDollarCost ;
					ps_element.TotalPM   += pVehicle->m_PM ;

					ps_element.m_TravelTimeVector.push_back((pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime));
					ps_element.m_TravelTimeVector.push_back((pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime)/max(0.01,pVehicle->m_Distance));

						ps_element.emissiondata.Energy += pVehicle->m_EmissionData .Energy;
						ps_element.emissiondata.CO2 += pVehicle->m_EmissionData .CO2;
						ps_element.emissiondata.NOX += pVehicle->m_EmissionData .NOX;
						ps_element.emissiondata.CO += pVehicle->m_EmissionData .CO;
						ps_element.emissiondata.HC += pVehicle->m_EmissionData .HC;
						vehicle_count ++;


					ps_element.m_VehicleVector.push_back(pVehicle);


					if(true)
					{
						for(int link= 1; link<pVehicle->m_NodeSize; link++)
						{
							ps_element.m_LinkVector.push_back(pVehicle->m_NodeAry[link].LinkNo);
						}
					}

					m_PathVector.push_back (ps_element);
				}
			}
		}
	}

	int row_index = 0;
	for(int p = 0; p< m_PathVector.size(); p++)
	{
		float AvgDistance = m_PathVector[p].TotalDistance /m_PathVector[p].TotalVehicleSize;
		float AvgTravelTime = m_PathVector[p].TotalTravelTime /m_PathVector[p].TotalVehicleSize;
		float AvgTravelCost = m_PathVector[p].TotalCost /m_PathVector[p].TotalVehicleSize;

		float TravelTimeSTD = m_PathVector[p].GetSimulatedTravelTimeStandardDeviation();
		float TravelTimePerMileSTD = m_PathVector[p].GetSimulatedTravelTimePerMileStandardDeviation();

		float AvgEnergy = m_PathVector[p].emissiondata .Energy / m_PathVector[p].TotalVehicleSize;
		float AvgCO2 = m_PathVector[p].emissiondata.CO2  / m_PathVector[p].TotalVehicleSize;
		float AvgNOX = m_PathVector[p].emissiondata.NOX  / m_PathVector[p].TotalVehicleSize;
		float AvgCO = m_PathVector[p].emissiondata.CO  / m_PathVector[p].TotalVehicleSize;
		float AvgHC = m_PathVector[p].emissiondata.HC  / m_PathVector[p].TotalVehicleSize;

		double Gasoline = AvgEnergy/1000/(121.7);  // convert energy from KJ to mega joules  then to gasline per gallon
		double MilesPerGallon = AvgDistance/max(0.1,Gasoline);

		float AvgSpeed = AvgDistance * 60 / max(0.1,AvgTravelTime);  // mph

		int column_index  = 0;
		char text[100];
		int Index;


		sprintf_s(text, "%d",p+1);
		Index = m_PathListCtrl.InsertItem(LVIF_TEXT,row_index++,text , 0, 0, 0, NULL);
		column_index++;

		sprintf_s(text, "%d", m_PathVector[p].TotalVehicleSize);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%2.1f", m_PathVector[p].TotalVehicleSize*100.0/vehicle_count);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f", AvgTravelTime);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f", AvgDistance);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f", AvgSpeed);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.2f", AvgTravelCost);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.2f", TravelTimeSTD);
		m_PathListCtrl.SetItemText(Index,column_index++,text );
		
		sprintf_s(text, "%3.2f", TravelTimePerMileSTD);
		m_PathListCtrl.SetItemText(Index,column_index++,text );


		sprintf_s(text, "%3.1f", AvgEnergy);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.1f", AvgCO2);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.4f", AvgNOX);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.4f", AvgCO);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.4f", AvgHC);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

		sprintf_s(text, "%3.2f", MilesPerGallon);
		m_PathListCtrl.SetItemText(Index,column_index++,text );

	}

	
//	m_PathList.SetCurSel(0);


	ShowSelectedPath();
	ShowVehicles();

}
void CDlg_VehPathAnalysis::ShowVehicles()
{
	m_pDoc->m_SelectedVehicleID = -1;

	m_VehicleList.ResetContent ();
	int PathNo = m_SelectedPath;
	if(PathNo>=0 && PathNo < m_PathVector.size())
	{
		for(unsigned int v = 0; v< m_PathVector[PathNo].m_VehicleVector.size(); v++)
		{
			DTAVehicle* pVehicle = m_PathVector[PathNo].m_VehicleVector[v];
			CString VehicleInfoString;

			if( v == 0)  // only for the first  vehicle
			{

			m_pDoc->m_SelectedVehicleID = pVehicle->m_VehicleID ;



			}

			if(pVehicle->m_TollDollarCost >=0.001)
			{
			if(m_pDoc->m_EmissionDataFlag )
				VehicleInfoString.Format ("No. %d, %s, @%3.1f min, %3.1f min, $%3.2f,%5.1f(J), %4.1f(CO2_g)",pVehicle->m_VehicleID , m_pDoc->GetPricingTypeStr(pVehicle->m_PricingType ), pVehicle->m_DepartureTime, (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime),pVehicle->m_TollDollarCost, pVehicle->m_EmissionData .Energy , pVehicle->m_EmissionData.CO2  );
			else
				VehicleInfoString.Format ("No. %d, %s, @%3.1f min, %3.1f min, $%3.2f",pVehicle->m_VehicleID , m_pDoc->GetPricingTypeStr(pVehicle->m_PricingType ),pVehicle->m_DepartureTime, (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime),pVehicle->m_TollDollarCost);

			}
			else
			{
			if(m_pDoc->m_EmissionDataFlag )
				VehicleInfoString.Format ("No. %d, %s, @%3.1f min, %3.1f min,%5.1f(J), %4.1f(CO2_g)",pVehicle->m_VehicleID , m_pDoc->GetPricingTypeStr(pVehicle->m_PricingType ), pVehicle->m_DepartureTime, (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime),pVehicle->m_EmissionData .Energy , pVehicle->m_EmissionData.CO2  );
			else
				VehicleInfoString.Format ("No. %d, %s, @%3.1f min, %3.1f min, $%3.2f",pVehicle->m_VehicleID , m_pDoc->GetPricingTypeStr(pVehicle->m_PricingType ), pVehicle->m_DepartureTime, (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime),pVehicle->m_TollDollarCost);
			}

			if(m_VehicleList.GetCount () <= 20000)
			{
				if(m_VehicleList.GetCount () == 20000)
				{
					m_VehicleList.AddString ("Reaching maximum number of items in a list..");
				}else
				m_VehicleList.AddString (VehicleInfoString);
			}
		}
	}
}
void CDlg_VehPathAnalysis::OnLbnSelchangeListLink()
{
	// TODO: Add your control notification handler code here
}


void CDlg_VehPathAnalysis::OnLbnSelchangeListVehicle()
{
}

void CDlg_VehPathAnalysis::OnLbnSelchangeListLink2()
{
	// TODO: Add your control notification handler code here
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboOrigin()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboDestination()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboDeparturetime()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboVehicletype()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboInformationclass()
{
	FilterOriginDestinationPairs();

}




void CDlg_VehPathAnalysis::OnCbnSelchangeComboTimeinterval()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboMinNumberofvehicles()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboMinTravelTime()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboMinTraveltimeindex()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnLbnSelchangeListOd()
{
	FilterPaths();
	//show OD pair

		int ODPairNo = m_ODList.GetCurSel();
	int Origin, Destination;

	if(ODPairNo>=0)	// if one of "all" options is selected, we need to narrow down to OD pair
	{
		char m_Text[MAX_STRING_LENGTH];
		m_ODList.GetText (ODPairNo, m_Text);
		sscanf(m_Text, "%d->%d", &Origin, &Destination);
		
		m_pDoc->m_CriticalOriginZone = Origin;
		m_pDoc->m_CriticalDestinationZone  = Destination;

		m_pDoc->UpdateAllViews (0);


	}


}

void CDlg_VehPathAnalysis::OnLbnSelchangeListPath()
{
	ShowSelectedPath();
	ShowVehicles();

}

void CDlg_VehPathAnalysis::ShowSelectedPath()
{
	
	if(m_PathVector.size()==0)
		m_SelectedPath = -1;

	if(m_SelectedPath >= 0 && m_SelectedPath< m_PathVector.size())
		m_pDoc->HighlightPath(m_PathVector[m_SelectedPath].m_LinkVector,2);

}


void CDlg_VehPathAnalysis::OnCbnSelchangeComboImpactlink()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnLbnDblclkListOd()
{
	FilterPaths();
}


void CDlg_VehPathAnalysis::OnBnClickedExport()
{
	if(m_pDoc->m_ProjectDirectory .GetLength () > 0)
	{
	CString fname, str;
	
	fname = m_pDoc->m_ProjectDirectory + "export_OD_moe.csv"; 

      CWaitCursor wait;

      if(!ExportDataToCSVFileAllOD(fname))
      {
		 str.Format("The file %s could not be opened.\nPlease check if it is opened by Excel.", fname);
		 AfxMessageBox(str);
      }else
	  {
		m_pDoc->OpenCSVFileInExcel(fname);
	  }
	}
}
bool CDlg_VehPathAnalysis::ExportDataToCSVFileAllOD(CString csv_file)
{
     FILE* st;
     fopen_s(&st,csv_file,"w");
	 CWaitCursor wc;

     if(st!=NULL)
      {
			  for(int column = 0; column < m_ListCtrl.GetColumnCount (); column++)
			  {
			  fprintf(st,"%s,", m_ListCtrl.GetColumnHeading (column));
			  }
				fprintf(st,"\n");

		  for(int row = 0; row < m_ListCtrl.GetItemCount (); row++)
		  {

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


void CDlg_VehPathAnalysis::OnCbnSelchangeComboVotLb()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboVotUb()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnCbnSelchangeComboDemandtype()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnBnClickedFindSingleVehicleId()
{
	// TODO: Add your control notification handler code here
}

void CDlg_VehPathAnalysis::OnBnClickedExportPathData()
{
	if(m_pDoc->m_ProjectDirectory .GetLength () > 0)
	{
	CString fname;
	
	fname = m_pDoc->m_ProjectDirectory + "export_path_moe.csv"; 
      CWaitCursor wait;

      if(!ExportPathDataToCSVFile(fname))
      {  CString str;
		 str.Format("The file %s could not be opened.\nPlease check if it is opened by Excel.", fname);
		 AfxMessageBox(str);
      }else
	  {
	  m_pDoc->OpenCSVFileInExcel(fname);
	  }
   }	
}


bool CDlg_VehPathAnalysis::ExportPathDataToCSVFile(CString csv_file)
{
    FILE* st;
      fopen_s(&st,csv_file,"w");

     if(st!=NULL)
      {
			  for(int column = 0; column < m_PathListCtrl.GetColumnCount (); column++)
			  {
			  fprintf(st,"%s,", m_PathListCtrl.GetColumnHeading (column));
			  }
				fprintf(st,"\n");

		  for(int row = 0; row < m_PathListCtrl.GetItemCount (); row++)
		  {

			  for(int column = 0; column < m_PathListCtrl.GetColumnCount (); column++)
			  {
				char str[100];
				m_PathListCtrl.GetItemText (row,column,str,100);

				fprintf(st,"%s,",str);
			  }
				fprintf(st,"\n");

		  }

		fclose(st);
		return true;
	 }


	 return false;
}

bool CDlg_VehPathAnalysis::ExportVehicleDataToCSVFile(CString csv_file)
{
	int PathNo = m_SelectedPath;
	if(PathNo>=0)
	{
		return m_pDoc->WriteSelectVehicleDataToCSVFile(csv_file,m_PathVector[PathNo].m_VehicleVector);
	}
	return false;
}

void CDlg_VehPathAnalysis::OnBnClickedExportVehicleData()
{
	if(m_pDoc->m_ProjectDirectory .GetLength () > 0)
	{
	CString fname;
	
		fname = m_pDoc->m_ProjectDirectory + "export_vehicle_list.csv"; 
      CWaitCursor wait;

      if(!ExportVehicleDataToCSVFile(fname))
      { CString str;
		 str.Format("The data might not be available. \n The file %s might not be opened.\nPlease check if it is opened by Excel.", fname);
		 AfxMessageBox(str);
      }else
	  {
	  m_pDoc->OpenCSVFileInExcel(fname);
	  }
   }	
}

void CDlg_VehPathAnalysis::OnBnClickedFindcriticalod()
{
	m_MinVehicleSizeBox.SetCurSel(9);
	m_MinDistanceBox.SetCurSel(2);
	FilterOriginDestinationPairs();

}

void CDlg_VehPathAnalysis::OnBnClickedOk()
{
	OnOK();
	g_bShowVehiclePathDialog = false;
}

void CDlg_VehPathAnalysis::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
		int nSelectedRow = m_ListCtrl.GetNextSelectedItem(pos);
		char str[100];
		m_ListCtrl.GetItemText (nSelectedRow,0,str,20);

		m_SelectedOrigin = atoi(str);
		m_ListCtrl.GetItemText (nSelectedRow,1,str,20);
		m_SelectedDestination = atoi(str);

			FilterPaths();
			ShowSelectedPath();
			ShowVehicles();
	}
}

void CDlg_VehPathAnalysis::OnPathLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = m_PathListCtrl.GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
		int nSelectedRow = m_PathListCtrl.GetNextSelectedItem(pos);
		char str[100];
		m_PathListCtrl.GetItemText (nSelectedRow,0,str,20);

		m_SelectedPath = atoi(str)-1;  // start from 0

			ShowSelectedPath();
			ShowVehicles();
	}
}



void CDlg_VehPathAnalysis::OnCbnSelchangeComboDayno()
{
	FilterOriginDestinationPairs();
}

void CDlg_VehPathAnalysis::OnLvnItemchangedList5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDlg_VehPathAnalysis::OnBnClickedBarchart()
{

	CDlg_VehicleClassification dlg;
	dlg.m_pDoc = m_pDoc;
	m_pDoc->m_VehicleSelectionMode = CLS_OD;
	dlg.m_VehicleSelectionNo  = CLS_OD;
	dlg.DoModal ();

}

void CDlg_VehPathAnalysis::OnClose()
{
	// TODO: Add your message handler code here and/or call default

 	g_bShowVehiclePathDialog = false;
	int nRet = 5; 
   EndDialog(nRet); 

}

void CDlg_VehPathAnalysis::OnBnClickedCheckVmsResponsive()
{
	FilterOriginDestinationPairs();
}
