// Dlg_GIS_Setting_Config.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_GIS_Setting_Config.h"
#ifndef _WIN64
#include "Data-Interface\\include\\ogrsf_frmts.h"
#endif
#include "MainFrm.h"

// CDlg_GIS_Setting_Config dialog

IMPLEMENT_DYNAMIC(CDlg_GIS_Setting_Config, CDialog)

CDlg_GIS_Setting_Config::CDlg_GIS_Setting_Config(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_GIS_Setting_Config::IDD, pParent)
	, m_LinkShapeFileString(_T(""))
	, m_bGenerateFromToNodes(TRUE)
	, m_bRFields(FALSE)
	, m_bManageLanes(FALSE)
	, m_bCentroidConnector(FALSE)
	, m_bWithSensorData(FALSE)
	, m_DemandData(FALSE)
{

}

CDlg_GIS_Setting_Config::~CDlg_GIS_Setting_Config()
{
}

void CDlg_GIS_Setting_Config::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LINKID, m_LinkIDList);
	DDX_Control(pDX, IDC_COMBO_LINKNAMEFIELD, m_LinkNameList);
	DDX_Control(pDX, IDC_COMBO_LINKTMC, m_TMCList);
	DDX_Control(pDX, IDC_COMBO_LINKSENSOR_ID, m_SensorIDList);
	DDX_Control(pDX, IDC_COMBO_DIRECTION2, m_LinkDirection);
	DDX_Control(pDX, IDC_COMBO_LENGTH, m_LengthList);
	DDX_Control(pDX, IDC_COMBO_FROM_NODE, m_FromNodeList);
	DDX_Control(pDX, IDC_COMBO_TO_NODE, m_ToNodeList);
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_LinkDirectionOption);
	DDX_Control(pDX, IDC_COMBO_LENGTH_UNIT, m_UnitLengthOption);
	DDX_Check(pDX, IDC_CHECK_AUTO_GENERATE_FROM_TO_NODES, m_bGenerateFromToNodes);
	DDX_Control(pDX, IDC_COMBO_NUMBER_OF_LANES2, m_RNumberOfLanes);
	DDX_Control(pDX, IDC_COMBO_FREEFLOWSPEED2, m_RSpeedLimit);
	DDX_Control(pDX, IDC_COMBO_CAPACITY2, m_RCapacity);
	DDX_Control(pDX, IDC_COMBO_LINKTMC2, m_RTMC);
	DDX_Control(pDX, IDC_COMBO_LINKSENSOR_ID2, m_RSensorID);
	DDX_Check(pDX, IDC_ReservedFields, m_bRFields);
	DDX_Check(pDX, IDC_CHECK_AUTO_GENERATE_FROM_TO_NODES3, m_bManageLanes);
	DDX_Control(pDX, IDC_COMBO_NUMBER_OF_LANES, m_NumberOfLanes);
	DDX_Control(pDX, IDC_COMBO_FREEFLOWSPEED, m_SpeedLimit);
	DDX_Control(pDX, IDC_COMBO_CAPACITY, m_Capacity);
	DDX_Control(pDX, IDC_COMBO_ONEWAY_TWOWAY, m_LaneVSLinkCapacity);
	DDX_Control(pDX, IDC_COMBO_LINK_VS_LANE_CAPACITY, m_OneWayLanes);
	DDX_Control(pDX, IDC_COMBO_LINK_TYPE, m_LinkType);
	DDX_Control(pDX, IDC_COMBO_MODE_TYPE, m_ModeType);

	DDX_Control(pDX, IDC_EDIT_CENTROID_LAYER, m_CentroidFile);
	DDX_Control(pDX, IDC_EDIT_CONNECTOR_LAYER, m_ConnectorFile);
	DDX_Control(pDX, IDC_COMBO_TAZID_CENTROID, m_CentroidTAZ);
	DDX_Control(pDX, IDC_COMBO_NODEID_CENTROID, m_CentroidNodeID);
	DDX_Control(pDX, IDC_COMBO_FROM_NODE_CONNECTOR, m_ConnectorZoneEnd);
	DDX_Control(pDX, IDC_COMBO_TO_NODE2, m_ConnectorNodeEnd);
	DDX_Check(pDX, IDC_CHECK_Centroid_Connector, m_bCentroidConnector);
	DDX_Check(pDX, IDC_CHECK_WITH_SENSOR_DATA, m_bWithSensorData);

	DDX_Control(pDX, IDC_COMBO_NODEID, m_NodeID);
	DDX_Control(pDX, IDC_COMBO_NODENAMEFIELD, m_NodeName);
	DDX_Control(pDX, IDC_COMBO_TAZFIELD, m_NodeTAZ);
	DDX_Control(pDX, IDC_EDIT_NODE_THRESHOLD_ZONE, m_TAZThreshold);
	DDX_Text(pDX, IDC_EDIT_NODE_LAYER, m_NodeShapeFileString);
	DDX_Text(pDX, IDC_EDIT_ZONE_LAYER, m_ZoneShapeFileString);
	DDX_Text(pDX, IDC_EDIT_LINK_LAYER, m_LinkShapeFileString);

	DDX_Control(pDX, IDC_COMBO_TAZID2, m_ZoneTAZ);
	DDX_Control(pDX, IDC_COMBO_NODEID2, m_ZoneNodeID);
}


BEGIN_MESSAGE_MAP(CDlg_GIS_Setting_Config, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LINK_FILE, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkFile)
	ON_CBN_SELCHANGE(IDC_COMBO_LINKID, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboLinkid)
	ON_CBN_SELCHANGE(IDC_COMBO_LINKNAMEFIELD, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboLinknamefield)
	ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboDirection)
	ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION2, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboDirection2)
	ON_CBN_SELCHANGE(IDC_COMBO_LENGTH_UNIT, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboLengthUnit)
	ON_CBN_SELCHANGE(IDC_COMBO_LENGTH, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboLength)
	ON_BN_CLICKED(IDC_CHECK_AUTO_GENERATE_FROM_TO_NODES, &CDlg_GIS_Setting_Config::OnBnClickedCheckAutoGenerateFromToNodes)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE_TYPE, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboModeType)
	ON_BN_CLICKED(IDC_CHECK_AUTO_GENERATE_FROM_TO_NODES3, &CDlg_GIS_Setting_Config::OnBnClickedCheckAutoGenerateFromToNodes3)
	ON_CBN_SELCHANGE(IDC_COMBO_ONEWAY_TWOWAY, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboOnewayTwoway)
	ON_BN_CLICKED(IDC_ReservedFields, &CDlg_GIS_Setting_Config::OnBnClickedReservedfields)
	ON_BN_CLICKED(IDC_CHECK_Centroid_Connector, &CDlg_GIS_Setting_Config::OnBnClickedCheckCentroidConnector)
	ON_BN_CLICKED(IDC_CHECK_WITH_SENSOR_DATA, &CDlg_GIS_Setting_Config::OnBnClickedCheckWithSensorData)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_NODE_FILE2, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadNodeFile2)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ZONE_FILE, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadZoneFile)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_CONNECTOR_FILE, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadConnectorFile)
	ON_CBN_SELCHANGE(IDC_COMBO_LINKSENSOR_ID, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboLinksensorId)
	ON_BN_CLICKED(IDOK, &CDlg_GIS_Setting_Config::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlg_GIS_Setting_Config::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LINK_FILE3, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkFile3)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LINK_FILE_TMC_SPEED_FILE, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkFileTmcSpeedFile)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LINK_FILE_Sensor_Data_FILE, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkFileSensorDataFile)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SENSOR_DATA_HELP, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadSensorDataHelp)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LINK_SAMPLE_FILE, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkSampleFile)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_CENTROID_FILE, &CDlg_GIS_Setting_Config::OnBnClickedButtonLoadCentroidFile)
	ON_STN_CLICKED(IDC_STATIC_C3, &CDlg_GIS_Setting_Config::OnStnClickedStaticC3)
	ON_BN_CLICKED(IDOK2, &CDlg_GIS_Setting_Config::OnBnClickedOk2)
	ON_CBN_SELCHANGE(IDC_COMBO_NODEID, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboNodeid)
	ON_CBN_SELCHANGE(IDC_COMBO_NODENAMEFIELD, &CDlg_GIS_Setting_Config::OnCbnSelchangeComboNodenamefield)
END_MESSAGE_MAP()


// CDlg_GIS_Setting_Config message handlers

BOOL CDlg_GIS_Setting_Config::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_LinkDirectionOption.AddString ("Single-Way");
	m_LinkDirectionOption.AddString ("Two-Way");
	m_LinkDirectionOption.SetCurSel (0);

	m_LinkDirectionOption.ShowWindow (0);

	m_UnitLengthOption.AddString ("Mile");
	m_UnitLengthOption.AddString ("KM");
	m_UnitLengthOption.SetCurSel (0);
	m_UnitLengthOption.ShowWindow(0);


	m_LinkIDList.ShowWindow (0);
	m_LinkNameList.ShowWindow (0);
	m_TMCList.ShowWindow (0);
	m_SensorIDList.ShowWindow (0);
	m_LinkDirection.ShowWindow (0);
	m_LengthList.ShowWindow (0);
	m_FromNodeList.ShowWindow (0);
	m_ToNodeList.ShowWindow (0);


	m_RNumberOfLanes.ShowWindow (0);
	m_RSpeedLimit.ShowWindow (0);
	m_RCapacity.ShowWindow (0);
	m_RTMC.ShowWindow (0);
	m_RSensorID.ShowWindow (0);
	{
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC1);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC2);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_STATIC3);
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_STATIC4);
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_STATIC5);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_STATIC6);
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_STATIC7);
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_STATIC8);
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_STATIC9);
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_STATIC10);

	CEdit* pEdit13 = (CEdit*)GetDlgItem(IDC_STATIC13);


	CEdit* pEdit21 = (CEdit*)GetDlgItem(IDC_STATIC21);
	CEdit* pEdit22 = (CEdit*)GetDlgItem(IDC_STATIC22);
	CEdit* pEdit23 = (CEdit*)GetDlgItem(IDC_STATIC23);

	CEdit* pEdit100 = (CEdit*)GetDlgItem(IDC_CHECK_AUTO_GENERATE_FROM_TO_NODES);
	CEdit* pEdit101 = (CEdit*)GetDlgItem(IDC_CHECK_AUTO_GENERATE_FROM_TO_NODES3);
	CEdit* pEdit102 = (CEdit*)GetDlgItem(IDC_ReservedFields);
	CEdit* pEdit103 = (CEdit*)GetDlgItem(IDC_CHECK_Centroid_Connector);
	CEdit* pEdit104 = (CEdit*)GetDlgItem(IDC_CHECK_WITH_SENSOR_DATA);



	pEdit100->ShowWindow (0);
	pEdit101->ShowWindow (0);
	pEdit102->ShowWindow (0);
	pEdit103->ShowWindow (1);
	pEdit104->ShowWindow (0);


	pEdit1->ShowWindow (0);
	pEdit2->ShowWindow (0);
	pEdit3->ShowWindow (0);
	pEdit4->ShowWindow (0);
	pEdit5->ShowWindow (0);
	pEdit6->ShowWindow (0);
	pEdit7->ShowWindow (0);
	pEdit8->ShowWindow (0);
	pEdit9->ShowWindow (0);
	pEdit10->ShowWindow (0);

	pEdit13->ShowWindow (0);

	pEdit21->ShowWindow (0);
	pEdit22->ShowWindow (0);
	pEdit23->ShowWindow (0);
}

	{
		CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC_R1);
		CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC_R2);
		CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_STATIC_R3);
		CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_STATIC_R4);
		CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_STATIC_R5);

	pEdit1->ShowWindow (m_bRFields);
	pEdit2->ShowWindow (m_bRFields);
	pEdit3->ShowWindow (m_bRFields);
	pEdit4->ShowWindow (m_bRFields);
	pEdit5->ShowWindow (m_bRFields);
	
	}


	{
		CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC_C1);
		CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC_C2);
		CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_STATIC_C3);
		CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_STATIC_C4);
		CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_STATIC_C5);
		CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_STATIC_C6);
		CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_BUTTON_LOAD_CENTROID_FILE);
		CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_BUTTON_LOAD_CONNECTOR_FILE);

	

	pEdit1->ShowWindow (m_bCentroidConnector);
	pEdit2->ShowWindow (m_bCentroidConnector);
	pEdit3->ShowWindow (m_bCentroidConnector);
	pEdit4->ShowWindow (m_bCentroidConnector);
	pEdit5->ShowWindow (m_bCentroidConnector);
	pEdit6->ShowWindow (m_bCentroidConnector);
	pEdit7->ShowWindow (m_bCentroidConnector);
	pEdit8->ShowWindow (m_bCentroidConnector);
		

	m_CentroidFile.ShowWindow (m_bCentroidConnector);
	m_ConnectorFile.ShowWindow (m_bCentroidConnector);
	m_CentroidTAZ.ShowWindow (m_bCentroidConnector);
	m_CentroidNodeID.ShowWindow (m_bCentroidConnector);
	m_ConnectorZoneEnd.ShowWindow (m_bCentroidConnector);
	m_ConnectorNodeEnd.ShowWindow (m_bCentroidConnector);

	
	}

	m_NumberOfLanes.ShowWindow (0);
	m_Capacity.ShowWindow (0);
	m_SpeedLimit.ShowWindow (0);
	m_Capacity.ShowWindow (0);
	m_LaneVSLinkCapacity.ShowWindow (0);
	m_OneWayLanes.ShowWindow (0);

	m_LinkType.ShowWindow(0);
	m_ModeType.ShowWindow(0);

	{
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC_N1);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC_N2);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_STATIC_N3);
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_STATIC_N4);

	pEdit1->ShowWindow (0);
	pEdit2->ShowWindow (0);
	pEdit3->ShowWindow (0);
	pEdit4->ShowWindow (0);

	m_NodeID.ShowWindow (0);
	m_TAZThreshold.ShowWindow (0);
	m_NodeName.ShowWindow (0);
	m_NodeTAZ.ShowWindow (0);
	
	
	}

	{
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC_Z1);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC_Z2);

	pEdit1->ShowWindow (0);
	pEdit2->ShowWindow (0);

	m_ZoneTAZ.ShowWindow (0);
	m_ZoneNodeID.ShowWindow (0);	
	
	}

	{
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_BUTTON_LOAD_LINK_FILE_TMC_SPEED_FILE);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_BUTTON_LOAD_LINK_FILE_Sensor_Data_FILE);
	pEdit5->ShowWindow (m_bWithSensorData);
	pEdit6->ShowWindow (m_bWithSensorData);	
	}
	// TODO:  Add extra initialization here

	m_LaneVSLinkCapacity.AddString ("Lane Capacity");
	m_LaneVSLinkCapacity.AddString ("Link Capacity");
	m_LaneVSLinkCapacity.SetCurSel (0);

	m_OneWayLanes.AddString("One-directional");
	m_OneWayLanes.AddString("Bi-directional");
	m_OneWayLanes.SetCurSel (0);




	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkFile()
{

	#ifndef _WIN64


	CString str;

	static char BASED_CODE szFilter[] = "GIS Shape File (*.shp)|*.shp||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{

		m_LinkShapeFileString = dlg.GetPathName();
		UpdateData(false);

	}

	if(m_LinkShapeFileString.GetLength () == 0 )
	{
		return;
	}


	m_pDoc->m_ProjectFile = m_LinkShapeFileString;
	CString directory = m_LinkShapeFileString.Left(m_pDoc->m_ProjectFile.ReverseFind('\\') + 1);

	m_pDoc->m_ProjectDirectory = directory;
	m_pDoc->m_ProjectTitle = m_pDoc->GetWorkspaceTitleName(m_LinkShapeFileString);
	m_pDoc->SetTitle(m_pDoc->m_ProjectTitle);

	m_pDoc->CopyDefaultFiles();
	m_pDoc->ReadLinkTypeCSVFile(directory+"input_link_type.csv");


	CWaitCursor wait;
	OGRRegisterAll();
	OGRDataSource       *poDS;

	bool bFindOverlappingNode = false;

	poDS = OGRSFDriverRegistrar::Open(m_LinkShapeFileString, FALSE );
	if( poDS == NULL )
	{
		AfxMessageBox("Open file failed." );
		return;
	}

	int point_index = 0;
	int poLayers = ((OGRDataSource*)poDS)->GetLayerCount() ;
	for (int i=0; i < poLayers; i++) 
	{

		OGRLayer  *poLayer;

		poLayer = ((OGRDataSource*)poDS)->GetLayer(i);	

		if(poLayer == NULL)
		{
			message_str.Format("Open layer %d failed", i+1);
			AfxMessageBox(message_str);
			return;			
		}

		OGRFeature *poFeature;

		int feature_count = 0;

		// 0.03 miles
		// 0.02: shape length / miles
		double threashold = 0.02*0.001;

		poLayer->ResetReading();

		while( (poFeature = poLayer->GetNextFeature()) != NULL )
		{
			OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
			int iField;

			CString str = " ";
			m_LinkFieldStringVector.push_back (str);

			m_LinkIDList.ResetContent ();
			m_LinkNameList.ResetContent ();
			m_TMCList.ResetContent ();
			m_SensorIDList.ResetContent ();
			m_LinkDirection.ResetContent ();
			m_LengthList.ResetContent ();
			m_FromNodeList.ResetContent ();
			m_ToNodeList.ResetContent ();
			m_LinkType.ResetContent ();
			m_ModeType.ResetContent ();
			m_NumberOfLanes.ResetContent ();
			m_SpeedLimit.ResetContent ();
			m_Capacity.ResetContent ();
			m_RNumberOfLanes.ResetContent ();
			m_RSpeedLimit.ResetContent ();
			m_RCapacity.ResetContent ();
			m_RTMC.ResetContent ();
			m_RSensorID.ResetContent ();


			m_LinkIDList.AddString(str);
			m_LinkNameList.AddString(str);
			m_TMCList.AddString(str);
			m_SensorIDList.AddString(str);
			m_LinkDirection.AddString(str);
			m_LengthList.AddString(str);
			m_FromNodeList.AddString(str);
			m_ToNodeList.AddString(str);
			m_LinkType.AddString(str);
			m_ModeType.AddString(str);
			m_NumberOfLanes.AddString(str);
			m_SpeedLimit.AddString(str);
			m_Capacity.AddString(str);

			m_RNumberOfLanes.AddString(str);
			m_RSpeedLimit.AddString(str);
			m_RCapacity.AddString(str);
			m_RTMC.AddString(str);
			m_RSensorID.AddString(str);
			

			for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
			{

				OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
			
				str = poFieldDefn->GetNameRef();

				if(str.GetLength  ()!=0)
				{
				m_LinkFieldStringVector.push_back(str);
				m_LinkIDList.AddString(str);
				m_LinkNameList.AddString(str);
				m_TMCList.AddString(str);
				m_SensorIDList.AddString(str);
				m_LinkDirection.AddString(str);
				m_LengthList.AddString(str);
				m_FromNodeList.AddString(str);
				m_ToNodeList.AddString(str);
				m_LinkType.AddString(str);
				m_ModeType.AddString(str);
				m_RNumberOfLanes.AddString(str);
				m_RSpeedLimit.AddString(str);
				m_RCapacity.AddString(str);
				m_RTMC.AddString(str);
				m_RSensorID.AddString(str);

			m_NumberOfLanes.AddString(str);
			m_SpeedLimit.AddString(str);
			m_Capacity.AddString(str);

			m_RNumberOfLanes.AddString(str);
			m_RSpeedLimit.AddString(str);
			m_RCapacity.AddString(str);
			m_RTMC.AddString(str);
			m_RSensorID.AddString(str);

				}


			}
		
		break;
		}

	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC1);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC2);
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_STATIC5);
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_STATIC7);
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_STATIC8);
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_STATIC9);
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_STATIC10);
	
	CEdit* pEdit12 = (CEdit*)GetDlgItem(IDC_STATIC12);
	CEdit* pEdit13 = (CEdit*)GetDlgItem(IDC_STATIC13);


	pEdit1->ShowWindow (1);
	pEdit2->ShowWindow (1);
	pEdit5->ShowWindow (1);
	pEdit7->ShowWindow (1);
	pEdit8->ShowWindow (1);


	pEdit12->ShowWindow (1);
	pEdit13->ShowWindow (1);
			
	m_LinkIDList.ShowWindow (1);
	m_LinkNameList.ShowWindow (1);

	m_LengthList.ShowWindow (1);
	m_LinkType.ShowWindow(1);
	m_ModeType.ShowWindow(1);

	m_LinkDirectionOption.ShowWindow(1);
	m_UnitLengthOption.ShowWindow(1);

		m_LinkIDList.SetCurSel(0);
		m_LinkNameList.SetCurSel(0);
		m_TMCList.SetCurSel(0);
		m_SensorIDList.SetCurSel(0);
		m_LinkDirection.SetCurSel(0);
		m_LengthList.SetCurSel(0);
		m_FromNodeList.SetCurSel(0);
		m_ToNodeList.SetCurSel(0);
		m_LinkType.SetCurSel(0);
		m_ModeType.SetCurSel(0);

	CEdit* pEdit100 = (CEdit*)GetDlgItem(IDC_CHECK_AUTO_GENERATE_FROM_TO_NODES);
	CEdit* pEdit101 = (CEdit*)GetDlgItem(IDC_CHECK_AUTO_GENERATE_FROM_TO_NODES3);
	CEdit* pEdit103 = (CEdit*)GetDlgItem(IDC_CHECK_Centroid_Connector);
	CEdit* pEdit104 = (CEdit*)GetDlgItem(IDC_CHECK_WITH_SENSOR_DATA);
	CEdit* pEdit105 = (CEdit*)GetDlgItem(IDC_ReservedFields);




pEdit100->ShowWindow (1);
	pEdit101->ShowWindow (1);
	pEdit103->ShowWindow (1);
	pEdit104->ShowWindow (1);
	pEdit105->ShowWindow (1);
	}

	std::map<int, DTALinkType>:: const_iterator itr;


#endif 
}
void CDlg_GIS_Setting_Config::OnCbnSelchangeComboLinkid()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboLinknamefield()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboDirection()
{

	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_STATIC6);
	if(m_LinkDirectionOption.GetCurSel () ==0)  // ONE WAY
	{
	pEdit6->ShowWindow (0);

	m_LinkDirection.ShowWindow (0);
	}else  // 2 way
	{
	pEdit6->ShowWindow (1);
	m_LinkDirection.ShowWindow (1);
	
	
	}

}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboDirection2()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboLengthUnit()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboLength()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnBnClickedCheckAutoGenerateFromToNodes()
{

	UpdateData(1);

	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_STATIC9);
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_STATIC10);

	pEdit9->ShowWindow (!m_bGenerateFromToNodes);
	pEdit10->ShowWindow (!m_bGenerateFromToNodes);

	m_FromNodeList.ShowWindow(!m_bGenerateFromToNodes);
	m_ToNodeList.ShowWindow(!m_bGenerateFromToNodes);

	

}

void CDlg_GIS_Setting_Config::OnBnClickedCheckRFields()
{
	UpdateData(1);

		CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC_R1);
		CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC_R2);
		CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_STATIC_R3);
		CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_STATIC_R4);
		CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_STATIC_R5);

	pEdit1->ShowWindow (m_bRFields);
	pEdit2->ShowWindow (m_bRFields);
	pEdit3->ShowWindow (m_bRFields);
	pEdit4->ShowWindow (m_bRFields);
	pEdit5->ShowWindow (m_bRFields);

	m_RNumberOfLanes.ShowWindow (m_bRFields);
	m_RSpeedLimit.ShowWindow (m_bRFields);
	m_RCapacity.ShowWindow (m_bRFields);
	m_RTMC.ShowWindow (m_bRFields);
	m_RSensorID.ShowWindow (m_bRFields);
	
}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboModeType()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnBnClickedCheckAutoGenerateFromToNodes3()
{

	UpdateData(1);

	if(m_bManageLanes == false)
	{
		m_bRFields = false;
		CEdit* pEditR = (CEdit*)GetDlgItem(IDC_ReservedFields);

		pEditR->ShowWindow (0);
				
		OnBnClickedCheckRFields();


	}

	CEdit* pEdit21 = (CEdit*)GetDlgItem(IDC_STATIC21);
	CEdit* pEdit22 = (CEdit*)GetDlgItem(IDC_STATIC22);
	CEdit* pEdit23 = (CEdit*)GetDlgItem(IDC_STATIC23);

	pEdit21->ShowWindow (m_bManageLanes);
	pEdit22->ShowWindow (m_bManageLanes);
	pEdit23->ShowWindow (m_bManageLanes);

	m_NumberOfLanes.ShowWindow (m_bManageLanes);
	m_Capacity.ShowWindow (m_bManageLanes);
	m_SpeedLimit.ShowWindow (m_bManageLanes);
	m_Capacity.ShowWindow (m_bManageLanes);
	m_LaneVSLinkCapacity.ShowWindow (m_bManageLanes);
	m_OneWayLanes.ShowWindow (m_bManageLanes);


}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboOnewayTwoway()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnBnClickedReservedfields()
{
	UpdateData(1);

	OnBnClickedCheckRFields();


}

void CDlg_GIS_Setting_Config::OnBnClickedCheckCentroidConnector()
{

	UpdateData(1);
		CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC_C1);
		CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC_C2);
		CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_STATIC_C3);
		CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_STATIC_C4);
		CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_STATIC_C5);
		CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_STATIC_C6);
		CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_BUTTON_LOAD_CENTROID_FILE);
		CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_BUTTON_LOAD_CONNECTOR_FILE);

	


	pEdit1->ShowWindow (m_bCentroidConnector);
	pEdit2->ShowWindow (m_bCentroidConnector);
	pEdit3->ShowWindow (m_bCentroidConnector);
	pEdit4->ShowWindow (m_bCentroidConnector);
	pEdit5->ShowWindow (m_bCentroidConnector);
	pEdit6->ShowWindow (m_bCentroidConnector);
	pEdit7->ShowWindow (m_bCentroidConnector);
	pEdit8->ShowWindow (m_bCentroidConnector);
		

	m_CentroidFile.ShowWindow (m_bCentroidConnector);
	m_ConnectorFile.ShowWindow (m_bCentroidConnector);
	m_CentroidTAZ.ShowWindow (m_bCentroidConnector);
	m_CentroidNodeID.ShowWindow (m_bCentroidConnector);
	m_ConnectorZoneEnd.ShowWindow (m_bCentroidConnector);
	m_ConnectorNodeEnd.ShowWindow (m_bCentroidConnector);
}

void CDlg_GIS_Setting_Config::OnBnClickedCheckWithSensorData()
{

	UpdateData(1);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_STATIC3);
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_STATIC4);
	pEdit3->ShowWindow (m_bWithSensorData);
	pEdit4->ShowWindow (m_bWithSensorData);

	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_BUTTON_LOAD_LINK_FILE_TMC_SPEED_FILE);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_BUTTON_LOAD_LINK_FILE_Sensor_Data_FILE);
	pEdit5->ShowWindow (m_bWithSensorData);
	pEdit6->ShowWindow (m_bWithSensorData);



	m_TMCList.ShowWindow (m_bWithSensorData);
	m_SensorIDList.ShowWindow (m_bWithSensorData);
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadNodeFile2()
{
	#ifndef _WIN64
	static char BASED_CODE szFilter[] = "GIS Shape File (*.shp)|*.shp||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{

		m_NodeShapeFileString = dlg.GetPathName();
		UpdateData(false);

	}

	if(m_NodeShapeFileString.GetLength () == 0 )
	{
		return;
	}

	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC_N1);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC_N2);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_STATIC_N3);
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_STATIC_N4);

	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_NODE_THRESHOLD_ZONE);


	pEdit1->ShowWindow (1);
	pEdit2->ShowWindow (1);
	pEdit3->ShowWindow (1);
	pEdit4->ShowWindow (1);
	pEdit5->ShowWindow (1);


	m_ZoneTAZ.ShowWindow (1);
	m_ZoneNodeID.ShowWindow (1);
	m_NodeID.ShowWindow(1);
	m_NodeName.ShowWindow (1);
	m_NodeTAZ.ShowWindow (1);

	CWaitCursor wait;
	OGRRegisterAll();
	OGRDataSource       *poDS;

	bool bFindOverlappingNode = false;

	poDS = OGRSFDriverRegistrar::Open(m_NodeShapeFileString, FALSE );
	if( poDS == NULL )
	{
		AfxMessageBox("Open file failed." );
		return;
	}

	int point_index = 0;
	int poLayers = ((OGRDataSource*)poDS)->GetLayerCount() ;
	for (int i=0; i < poLayers; i++) 
	{
		OGRLayer  *poLayer;

		poLayer = ((OGRDataSource*)poDS)->GetLayer(i);	

		if(poLayer == NULL)
		{
			message_str.Format("Open layer %d failed", i+1);
			AfxMessageBox(message_str);
			return;			
		}

		OGRFeature *poFeature;

		int feature_count = 0;

		poLayer->ResetReading();

		while( (poFeature = poLayer->GetNextFeature()) != NULL )
		{
			OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
			int iField;

			CString str = " ";

			m_NodeID.ResetContent ();
			m_NodeName.ResetContent ();
			m_NodeTAZ.ResetContent ();



			m_NodeID.AddString  (str);
			m_NodeName.AddString  (str);
			m_NodeTAZ.AddString  (str);

			for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
			{

				OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
			
				str = poFieldDefn->GetNameRef();

				if(str.GetLength  ()!=0)
				{
				m_NodeID.AddString  (str);
				m_NodeName.AddString  (str);
				m_NodeTAZ.AddString  (str);
				}


			}
		
		break;
		}

	}
#endif
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadZoneFile()
{
	#ifndef _WIN64

	static char BASED_CODE szFilter[] = "GIS Shape File (*.shp)|*.shp||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{

		m_ZoneShapeFileString = dlg.GetPathName();
		UpdateData(false);

	}

	if(m_ZoneShapeFileString.GetLength () == 0 )
	{
		return;
	}

	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_STATIC_Z1);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_STATIC_Z2);

	pEdit1->ShowWindow (1);
	pEdit2->ShowWindow (1);

	m_ZoneTAZ.ShowWindow (1);
	m_ZoneNodeID.ShowWindow (1);


		CWaitCursor wait;
	OGRRegisterAll();
	OGRDataSource       *poDS;

	bool bFindOverlappingNode = false;

	poDS = OGRSFDriverRegistrar::Open(m_ZoneShapeFileString, FALSE );
	if( poDS == NULL )
	{
		AfxMessageBox("Open file failed." );
		return;
	}

	int point_index = 0;
	int poLayers = ((OGRDataSource*)poDS)->GetLayerCount() ;
	for (int i=0; i < poLayers; i++) 
	{
		OGRLayer  *poLayer;

		poLayer = ((OGRDataSource*)poDS)->GetLayer(i);	

		if(poLayer == NULL)
		{
			message_str.Format("Open layer %d failed", i+1);
			AfxMessageBox(message_str);
			return;			
		}

		OGRFeature *poFeature;

		int feature_count = 0;

		poLayer->ResetReading();

		while( (poFeature = poLayer->GetNextFeature()) != NULL )
		{
			OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
			int iField;

			CString str = " ";
			m_ZoneTAZ.ResetContent ();
			m_ZoneNodeID.ResetContent ();

			m_ZoneTAZ.AddString  (str);
			m_ZoneNodeID.AddString  (str);

			for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
			{

				OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
			
				str = poFieldDefn->GetNameRef();

				if(str.GetLength  ()!=0)
				{
					m_ZoneTAZ.AddString  (str);
					m_ZoneNodeID.AddString  (str);
				}


			}
		
		break;
		}

	}
#endif
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadConnectorFile()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonExternalexcel()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboLinksensorId()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnBnClickedOk()
{
	FILE* pFile = NULL;

	CString file_name;
	file_name.Format ("%s",m_pDoc->m_ProjectDirectory +"import_GIS_settings.csv");

	fopen_s(&pFile,file_name,"w");
	if(pFile !=NULL)
	{

	m_NodeShapeFileString = m_pDoc -> GetLocalFileName(m_NodeShapeFileString);
	m_LinkShapeFileString = m_pDoc -> GetLocalFileName(m_LinkShapeFileString);
	m_ZoneShapeFileString = m_pDoc -> GetLocalFileName(m_ZoneShapeFileString);


	fprintf(pFile, "section,key,value,required_or_optional,allowed_values,notes\n");
	fprintf(pFile, "file_name,node,%s,desired,,if no node layer is provided\n",m_NodeShapeFileString);
	fprintf(pFile, "file_name,link,%s,required,,this is the required data layer\n",m_LinkShapeFileString); 
	fprintf(pFile, "file_name,zone,%s,desired,,if no zone layer is provided\n",m_ZoneShapeFileString);

	CString CentroidFile,ConnectorFile;
    GetDlgItemText(IDC_EDIT_CENTROID_LAYER, CentroidFile);
    GetDlgItemText(IDC_EDIT_CONNECTOR_LAYER, ConnectorFile);


	CString TAZThreshold;
    GetDlgItemText(IDC_EDIT_NODE_THRESHOLD_ZONE, TAZThreshold);

	int nTAZThreshold = atoi(TAZThreshold);

	CentroidFile = m_pDoc -> GetLocalFileName(CentroidFile);
	ConnectorFile = m_pDoc -> GetLocalFileName(ConnectorFile);


	fprintf(pFile, "file_name,centroid,%s,,,additional centroid layer can help us build the activity location file\n",CentroidFile);
	fprintf(pFile, "file_name,connector,%s,,,additional centroid layer can help us build the activity location file that links centroid to physical nodes\n",ConnectorFile);
	fprintf(pFile, "configuration,with_decimal_long_lat,yes,,yes;no,\n");

	CString UnitLengthOption;
	if(m_UnitLengthOption.GetCurSel () ==0)	
		UnitLengthOption = "mile";
	else
		UnitLengthOption = "km";

	fprintf(pFile, "configuration,length_unit,%s,,km;mile,\n",UnitLengthOption);


	CString LinkDirectionOption;

	if(m_LinkDirectionOption.GetCurSel ()==0)
		LinkDirectionOption = "oneway";
	else
		LinkDirectionOption = "twoway";


	fprintf(pFile, "configuration,number_of_lanes_oneway_vs_twoway,%s,,oneway;twoway,\n",LinkDirectionOption);


	CString LaneVSLinkCapacity;

	if(m_LaneVSLinkCapacity.GetCurSel () ==0)
			LaneVSLinkCapacity = "lane";
	else
		LaneVSLinkCapacity = "link";

	fprintf(pFile, "configuration,lane_capacity_vs_link_capacity,link,,lane;link,\n");


	fprintf(pFile, "configuration,multiplier_for_obtaining_hourly_capacity,1,,,\n");
	fprintf(pFile, "configuration,direction_0_as_oneway_vs_twoway,twoway,,oneway;twoway,\n");
	fprintf(pFile, "configuration,default_link_direction,oneway,,oneway;twoway,\n");


	//m_TAZThreshold.GetDlgItemInt ();
	fprintf(pFile, "configuration,node_number_threshold_as_centroid,%d,,default 0,\n",nTAZThreshold);

	fprintf(pFile, "configuration,use_default_speed_limit_from_link_type,no,,yes;no,\n");
	fprintf(pFile, "configuration,use_default_lane_capacity_from_link_type,no,,yes;no,\n");
	fprintf(pFile, "configuration,use_default_number_of_lanes_from_link_type,no,,yes;no,\n");

	CString AutoFlag = "yes";

	fprintf(pFile, "configuration,identify_from_node_id_and_to_node_id_based_on_geometry,%s,,yes;no,\n",AutoFlag);

	fprintf(pFile, "configuration,use_boundary_nodes_as_activity_locations,no,,yes;no,\n");

	fprintf(pFile, "configuration,identify_signal_intersection,no,,yes;no,\n");
	fprintf(pFile, "configuration,minimum_speed_limit_for_signals,28,,,\n");
	fprintf(pFile, "configuration,maximum_speed_limit_for_signals,60,,,\n");
	fprintf(pFile, "configuration,default_cycle_length_in_second,110,,,\n");
	fprintf(pFile, "configuration,minimum_length_for_importing_links,0.00001,,,\n");
	fprintf(pFile, "configuration,minimum_network_size_ratio_for_overlapping_nodes,0.000001,,,\n");
	fprintf(pFile, "configuration,split_links_for_overlapping_nodes,no,,yes;no,,\n");



	fprintf(pFile, "node,node_id,%s,required when node file is provided,,\n",GetStringFromComboBox(&m_NodeID));
	fprintf(pFile, "node,name,%s,,,for display only\n",GetStringFromComboBox(&m_NodeName));
	fprintf(pFile, "node,TAZ,%s,desired,,if no TAZ id is provided\n",GetStringFromComboBox(&m_NodeTAZ));
	fprintf(pFile, "node,control_type,,,,used only when users have signal control type already\n");

	fprintf(pFile, "link,from_node_id,%s,desired,,if from_node_id is empty\n",GetStringFromComboBox(&m_FromNodeList));
	fprintf(pFile, "link,to_node_id,%s,desired,,if to_node_id is empty\n",GetStringFromComboBox(&m_ToNodeList));
	fprintf(pFile, "link,name,%s,,,for display only\n",GetStringFromComboBox(&m_LinkNameList));
	fprintf(pFile, "link,link_id,%s,,,used for integrating sensor data\n",GetStringFromComboBox(&m_LinkIDList));
	fprintf(pFile, "link,link_type,%s,,,default value = 1\n",GetStringFromComboBox(&m_LinkType));
	fprintf(pFile, "link,mode_code,%s,,,reserved for transit modeling\n",GetStringFromComboBox(&m_ModeType));
	fprintf(pFile, "link,direction,DIR,%s,,two comments here: if direction field is not available\n",GetStringFromComboBox(&m_LinkDirection));
	fprintf(pFile, "link,length,%s,desired,,if data is available\n",GetStringFromComboBox(&m_LengthList));
	fprintf(pFile, "link,number_of_lanes,%s,desired,,users can specify use_default_number_of_lanes_from_link_type based on link type\n",GetStringFromComboBox(&m_NumberOfLanes));
	fprintf(pFile, "link,hourly_capacity,%s,desired,,users can specify use_default_lane_capacity_from_link_type based on link type\n",GetStringFromComboBox(&m_Capacity));
	fprintf(pFile, "link,speed_limit,%s,desired,,users can specify use_default_speed_limit_from_link_type based on link type\n",GetStringFromComboBox(&m_SpeedLimit));

	fprintf(pFile, "link,speed_sensor_id,%s,desired,,if from_node_id is empty\n",GetStringFromComboBox(&m_TMCList));
	fprintf(pFile, "link,count_sensor_id,%s,desired,,if to_node_id is empty\n",GetStringFromComboBox(&m_SensorIDList));
	
	fprintf(pFile, "link,r_speed_sensor_id,%s,desired,,if from_node_id is empty\n",GetStringFromComboBox(&m_RTMC));
	fprintf(pFile, "link,r_count_sensor_id,%s,desired,,if to_node_id is empty\n",GetStringFromComboBox(&m_RSensorID));


	fprintf(pFile, "link,r_number_of_lanes,%s,,,used only when data for reserved links are given\n",GetStringFromComboBox(&m_RNumberOfLanes));
	fprintf(pFile, "link,r_hourly_capacity,%s,,,\n",GetStringFromComboBox(&m_RCapacity));
	fprintf(pFile, "link,r_speed_limit,%s,,,\n",GetStringFromComboBox(&m_RSpeedLimit));
	fprintf(pFile, "link,r_link_type,,,,\n");

	fprintf(pFile, "zone,zone_id,,desired,%s,required only if zone file is given in section file_name\n",GetStringFromComboBox(&m_ZoneTAZ));
	fprintf(pFile, "zone,node_id,,desired,%s",GetStringFromComboBox(&m_ZoneNodeID));


	fprintf(pFile, "centroid,name,,,,display only\n");
	fprintf(pFile, "centroid,node_id,%s,,,\n",GetStringFromComboBox(&m_CentroidNodeID));
	fprintf(pFile, "centroid,TAZ,%s,,,provide mapping for activity locations\n",GetStringFromComboBox(&m_CentroidTAZ));
	fprintf(pFile, "connector,zone_end,%s,,,provide mapping for activity locations\n",GetStringFromComboBox(&m_ConnectorZoneEnd));
	fprintf(pFile, "connector,node_end,%s,,,\n",GetStringFromComboBox(&m_ConnectorNodeEnd));
	fprintf(pFile, "connector,length,,,,\n");
	fprintf(pFile, "connector,number_of_lanes,,,,\n");
	fprintf(pFile, "connector,hourly_capacity,,,,\n");
	fprintf(pFile, "connector,link_type,,,,\n");
	fprintf(pFile, "connector,mode_code,,,,reserved for transit/walking modeling\n");
	fprintf(pFile, "connector,default_speed_limit,,,,\n");
	fprintf(pFile, "connector,default_link_type,,,,\n");
	fprintf(pFile, "connector,defaut_direction,0,,,\n");
	fprintf(pFile, "connector,defaut_direction,0,,,\n");
	fprintf(pFile, "connector,default_speed_limit,60,,,\n");
	fprintf(pFile, "connector,default_link_type,99,,,\n");
	fprintf(pFile, "connector,defaut_direction,0,,,\n");
	fprintf(pFile, "connector,default_number_of_lanes,7,,,\n");
	fprintf(pFile, "final_output,offset_link,yes,,,\n");
	fclose(pFile);

	m_pDoc->OpenCSVFileInExcel (file_name);
	}else
	{

	CString message;
	message.Format("File %s cannot be opened. Please check.", file_name);
	AfxMessageBox(message);
	
	}

}

void CDlg_GIS_Setting_Config::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkFile2()
{
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkTypeReload()
{
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkFile3()
{
	m_pDoc->OpenCSVFileInExcel (m_pDoc->m_ProjectDirectory +"input_link_type.csv");
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkFileTmcSpeedFile()
{
	m_pDoc->OpenCSVFileInExcel (m_pDoc->m_ProjectDirectory +"sensor_speed.csv");
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkFileSensorDataFile()
{
	m_pDoc->OpenCSVFileInExcel (m_pDoc->m_ProjectDirectory +"sensor_count.csv");
}

void CDlg_GIS_Setting_Config::OnBnClickedCheckWithDemandData()
{
	UpdateData(1);

}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkTypeHelp()
{
		g_OpenDocument("https://docs.google.com/document/d/1sR34XtJQ8DHf7-ImQKnRBMbLEUxgNRvqASbuM_vBHcw/edit?usp=sharing", SW_SHOW);
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadSensorDataHelp()
{
		g_OpenDocument("https://docs.google.com/document/d/17zTqlEsVvHx5_UESQKr9tH0N65iN-WwGJ0OVOl9Qv14/edit?usp=sharing", SW_SHOW);
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkShapeHelp()
{
		g_OpenDocument("https://docs.google.com/document/d/1Nx5fUHfnyfN4ebvlS7arxxHP-CcTv6xIQcBFlF4rjY8/edit?usp=sharing", SW_SHOW);

}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadLinkSampleFile()
{
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CString SampleShapeFileFolder = "\\sample_data_sets\\2.Importing_Creating_Traffic_Modeling_Data_Sets\\sample_link_layer\\";

	
	SampleShapeFileFolder = pMainFrame->m_CurrentDirectory + SampleShapeFileFolder;
	ShellExecute( NULL,  "explore", SampleShapeFileFolder, NULL,  NULL, SW_SHOWNORMAL );
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadDemandDataHelp()
{
		g_OpenDocument("https://docs.google.com/document/d/142bBL154Mh9ywS4Fgah7tULHVnuzLEWZnoWZybodpOE/edit?usp=sharing", SW_SHOW);
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadGisSettingDataHelp()
{
		g_OpenDocument("https://docs.google.com/document/d/1tn_vLFPG1XsmYlbWI_p3aXRMqQXVNIKjpbfKpYoRrnk/edit?usp=sharing", SW_SHOW);
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonLoadCentroidFile()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnStnClickedStaticC3()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnBnClickedOk2()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnBnClickedButtonEditDemandMetaDataBase()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboNodeid()
{
	// TODO: Add your control notification handler code here
}

void CDlg_GIS_Setting_Config::OnCbnSelchangeComboNodenamefield()
{
	// TODO: Add your control notification handler code here
}
