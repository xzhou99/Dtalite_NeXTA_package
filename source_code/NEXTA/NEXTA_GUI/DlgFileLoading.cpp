// DlgFileLoading.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgFileLoading.h"


// CDlgFileLoading dialog

IMPLEMENT_DYNAMIC(CDlgFileLoading, CDialog)

CDlgFileLoading::CDlgFileLoading(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileLoading::IDD, pParent)
{

}

CDlgFileLoading::~CDlgFileLoading()
{
}

void CDlgFileLoading::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NETEWORK_FILE, m_ListBox_NetworkData);
	DDX_Control(pDX, IDC_LIST_SENSOR_FILE, m_ListBox_SensorData);
	DDX_Control(pDX, IDC_LIST_OUTPUTDTA, m_ListBox_OutputData);
}


BEGIN_MESSAGE_MAP(CDlgFileLoading, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_SENSOR_FILE, &CDlgFileLoading::OnLbnSelchangeListSensorFile)
END_MESSAGE_MAP()


// CDlgFileLoading message handlers

BOOL CDlgFileLoading::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(unsigned int i = 0; i< m_pDoc->m_MessageStringVector.size(); i++)
	{
	m_ListBox_NetworkData.AddString (m_pDoc->m_MessageStringVector[i]);
	}

	m_ListBox_NetworkData.AddString (m_pDoc->m_NodeDataLoadingStatus);
	
	if(m_pDoc->m_SignalDataLoadingStatus.GetLength() > 0)
	{
		m_ListBox_NetworkData.AddString(m_pDoc->m_SignalDataLoadingStatus);
	}
	m_ListBox_NetworkData.AddString (m_pDoc->m_LinkDataLoadingStatus);
	
	if(m_pDoc->m_ConnectorDataLoadingStatus.GetLength () >=1)
	{
	m_ListBox_NetworkData.AddString (m_pDoc->m_ConnectorDataLoadingStatus);
	
	}

	m_ListBox_NetworkData.AddString (m_pDoc->m_ZoneDataLoadingStatus);
	
	m_ListBox_NetworkData.AddString (m_pDoc->m_ActivityLocationDataLoadingStatus);

	m_ListBox_NetworkData.AddString (m_pDoc->m_DemandDataLoadingStatus);
	
	if(m_pDoc->m_BackgroundBitmapLoaded)
	m_ListBox_NetworkData.AddString (m_pDoc->m_BackgroundImageFileLoadingStatus);
	
	if(m_pDoc->m_SensorLocationLoadingStatus.GetLength() > 0)
	{
		m_ListBox_SensorData.AddString(m_pDoc->m_SensorLocationLoadingStatus);
	}

	if(m_pDoc->m_SensorCountDataLoadingStatus.GetLength() > 0)
	{
		m_ListBox_SensorData.AddString(m_pDoc->m_SensorCountDataLoadingStatus);
	}

	if(m_pDoc->m_SensorSpeedDataLoadingStatus.GetLength() > 0)
	{
		m_ListBox_SensorData.AddString(m_pDoc->m_SensorSpeedDataLoadingStatus);
	}
	
	if (m_pDoc->m_SensorCountDataErrorMessage.GetLength() > 0)
	{
		m_ListBox_SensorData.AddString(m_pDoc->m_SensorCountDataErrorMessage);
	}
	
	if (m_pDoc->m_SensorSpeedDataErrorMessage.GetLength() > 0)
	{
		m_ListBox_SensorData.AddString(m_pDoc->m_SensorSpeedDataErrorMessage);
	}
	
	if(m_pDoc->m_EventDataLoadingStatus.GetLength() > 0)
	{
		m_ListBox_NetworkData.AddString(m_pDoc->m_EventDataLoadingStatus);
	}

	
	if(m_pDoc->m_TransitDataLoadingStatus.GetLength() > 0)
	{
		m_ListBox_NetworkData.AddString(m_pDoc->m_TransitDataLoadingStatus);
	}


	if(m_pDoc->m_ScenarioDataLoadingStatus.GetLength() > 0)
	{
		m_ListBox_NetworkData.AddString(m_pDoc->m_ScenarioDataLoadingStatus);
	}
	



	if(m_pDoc->m_LinkTrainTravelTimeDataLoadingStatus.GetLength () >0) // there are data being loaded
	{
		m_ListBox_SensorData.AddString (m_pDoc->m_LinkTrainTravelTimeDataLoadingStatus);	
	}

	if(m_pDoc->m_SimulatedLinkVolumeStatus.GetLength () >0) // there are data being loaded
	{
		m_ListBox_SensorData.AddString (m_pDoc->m_SimulatedLinkVolumeStatus);	
	}
	

	if(m_pDoc->m_TimetableDataLoadingStatus.GetLength () >0) // there are data being loaded
	{
		m_ListBox_SensorData.AddString (m_pDoc->m_TimetableDataLoadingStatus);	
	}

	if(m_pDoc->m_SimulationLinkMOEDataLoadingStatus.GetLength ()>0)
	{
	m_ListBox_OutputData.AddString (m_pDoc->m_SimulationLinkMOEDataLoadingStatus);
	}
	
	if(m_pDoc->m_SimulationLinkTDMOEDataLoadingStatus.GetLength ()>0)
	{
	m_ListBox_OutputData.AddString (m_pDoc->m_SimulationLinkTDMOEDataLoadingStatus);
	}
	
	

	if(m_pDoc->m_SimulationVehicleDataLoadingStatus.GetLength ()>0)
	{
	m_ListBox_OutputData.AddString(m_pDoc->m_SimulationVehicleDataLoadingStatus);
	}

	if(m_pDoc->m_PathDataLoadingStatus.GetLength ()>0)
	{
	m_ListBox_OutputData.AddString(m_pDoc->m_PathDataLoadingStatus);
	}
	
	if(m_pDoc->m_MovementDataLoadingStatus.GetLength ()>0)
	{
	m_ListBox_OutputData.AddString(m_pDoc->m_MovementDataLoadingStatus);
	}

	if(m_pDoc->m_AgentLocationLoadingStatus.GetLength ()>0)
	{
	m_ListBox_OutputData.AddString(m_pDoc->m_AgentLocationLoadingStatus);
	}

	m_ListBox_OutputData.AddString(m_pDoc->m_StrLoadingTime);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgFileLoading::OnLbnSelchangeListSensorFile()
{
	// TODO: Add your control notification handler code here
}
