// TLiteDoc.h : interface of the CTLiteDoc class
//
//  Portions Copyright 2010 Xuesong Zhou (xzhou99@gmail.com)

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

// Dlg_ImportNetwork.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"


#include "Dlg_ImportNetwork.h"
#include "DlgSensorDataLoading.h"
#include "MainFrm.h"
#include "Shellapi.h"
#include "Data-Interface\\XLEzAutomation.h"

// CDlg_ImportNetwork dialog

IMPLEMENT_DYNAMIC(CDlg_ImportNetwork, CDialog)

CDlg_ImportNetwork::CDlg_ImportNetwork(CWnd* pParent /*=NULL*/)
: CDialog(CDlg_ImportNetwork::IDD, pParent)
, m_Edit_Excel_File(_T(""))
, m_Edit_Demand_CSV_File(_T(""))
, m_Sensor_File(_T(""))
, m_bRemoveConnectors(FALSE)
, m_AutogenerateNodeFlag(FALSE)
, m_ImportZoneData(TRUE)
, m_bAddConnectorsForIsolatedNodes(TRUE)
, m_bUseLinkTypeForDefaultValues(FALSE)
, m_bLinkMOECheck(TRUE)
, m_TMCSpeedCheck(TRUE)
, m_SensorCountCheck(TRUE)
{
	m_bImportNetworkOnly = false;
}

CDlg_ImportNetwork::~CDlg_ImportNetwork()
{
}

void CDlg_ImportNetwork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ExcelFile, m_Edit_Excel_File);
	DDX_Control(pDX, IDC_LIST1, m_MessageList);
	DDX_Check(pDX, IDC_CHECK_ZONE_DATA, m_ImportZoneData);

	DDX_Check(pDX, IDC_CHECK_LINKMOE, m_bLinkMOECheck);
	DDX_Check(pDX, IDC_CHECK_TMC_SPEED, m_TMCSpeedCheck);
	DDX_Check(pDX, IDC_CHECK_SENSOR_COUNT, m_SensorCountCheck);
}


BEGIN_MESSAGE_MAP(CDlg_ImportNetwork, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Find_Exel_File, &CDlg_ImportNetwork::OnBnClickedButtonFindExelFile)
	ON_BN_CLICKED(IDC_BUTTON_Find_Demand_CSV_File, &CDlg_ImportNetwork::OnBnClickedButtonFindDemandCsvFile)
	ON_BN_CLICKED(ID_IMPORT, &CDlg_ImportNetwork::OnBnClickedImport)
	ON_BN_CLICKED(ID_IMPORT_Network_Only, &CDlg_ImportNetwork::OnBnClickedImportNetworkOnly)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlg_ImportNetwork::OnLbnSelchangeList1)
	ON_BN_CLICKED(ID_EXPORT_DATA, &CDlg_ImportNetwork::OnBnClickedExportData)
	ON_BN_CLICKED(IDC_BUTTON_View_Sample_File, &CDlg_ImportNetwork::OnBnClickedButtonViewSampleFile)
	ON_BN_CLICKED(IDC_BUTTON_Load_Sample_File, &CDlg_ImportNetwork::OnBnClickedButtonLoadSampleFile)
	ON_BN_CLICKED(IDC_BUTTON_View_Sample_CSV_File, &CDlg_ImportNetwork::OnBnClickedButtonViewSampleCsvFile)
	ON_BN_CLICKED(ID_IMPORT2, &CDlg_ImportNetwork::OnBnClickedImport2)
END_MESSAGE_MAP()

// CDlg_ImportNetwork message handlers

void CDlg_ImportNetwork::OnBnClickedButtonFindExelFile()
{
	static char BASED_CODE szFilter[] = "Excel File (*.xlsx)|*.xlsx||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{
		UpdateData(true);

		m_Edit_Excel_File = dlg.GetPathName();

		UpdateData(false);
	}

}

void CDlg_ImportNetwork::OnBnClickedButtonFindDemandCsvFile()
{
	static char BASED_CODE szFilter[] = "Demand CSV file (*.csv)|*.csv||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{
		m_Edit_Demand_CSV_File = dlg.GetPathName();

		UpdateData(false);
	}

}
void CDlg_ImportNetwork::OnBnClickedButtonFindSensorFile()
{

}


void CDlg_ImportNetwork::OnBnClickedImport()
{

	g_Simulation_Time_Horizon = 1440;

	CWaitCursor cursor;
	// Make sure the network is empty
	m_pDoc->ClearNetworkData();


	m_MessageList.ResetContent ();

	UpdateData(true);

	bool bExist=true;

	CString str_msg;

	UpdateData(true);

	if(m_Edit_Excel_File.GetLength () ==0)
	{
		
		AfxMessageBox("Please first provide Excel file to be imported.");

		return;
	}

	// Open the EXCEL file
	std::string itsErrorMessage;

	int aggregation_time_interval_in_min = 1;

	CXLEzAutomation rsConfiguration;
		if(rsConfiguration.OpenFile(m_Edit_Excel_File, "Configuration", 1))
		{
			int i = 1;
		while(rsConfiguration.ReadRecord())
		{
		
			CString category= rsConfiguration.GetCString("category");
			CString str_key= rsConfiguration.GetCString("key");
			CString str_value= rsConfiguration.GetCString("value");



			if(category.GetLength () == 0 )  // no category value being assigned
				break;

			if(category == "parameter")
			{
				if(str_key == "length_unit")
				{
				 if(str_value.MakeLower()== "km")
				 {
					m_pDoc->m_bUseMileVsKMFlag = false;
					m_MessageList.AddString ("Unit of link length: km");
				 }
				 else //mile
				 {
					m_pDoc->m_bUseMileVsKMFlag = true;
					m_MessageList.AddString ("Unit of link length: mile");
				 }
				}

				if(str_key == "aggregation_time_interval_in_min")
				{
					aggregation_time_interval_in_min = max(1,atoi(str_value));
					CString str;
					str.Format("aggregation_time_interval_in_min=%d",aggregation_time_interval_in_min );
					m_MessageList.AddString (str);
					
				}
			
			
			}

		
		
		
		}
	
		
			rsConfiguration.Close();
		}

	// detetect if nodes will be automatically generated. 

	CXLEzAutomation rsNode;
		if(rsNode.OpenFile(m_Edit_Excel_File, "Node", 2))
		{

		int i = 0;
		while(rsNode.ReadRecord())
		{
			int id = rsNode.GetLong("node_id",bExist,false);

			if(!bExist)
			{
				m_MessageList.AddString ("Field node_id cannot be found in the node table.");
				rsNode.Close();
				return;
			}

			if(id < 0)
			{
				str_msg.Format ( "node_id: %d at row %d is invalid. Please check node table.", id, i+1);
				m_MessageList.AddString (str_msg);
				rsNode.Close();
				return;
			}
			if(id == 0)  // reading empty line
				break;

			double x;
			double y;

			int control_type = 0;

				std::vector<CCoordinate> CoordinateVector;

				CString geometry_str;
				geometry_str= rsNode.GetCString("geometry");

				if(geometry_str.GetLength () > 0 )
				{

				CT2CA pszConvertedAnsiString (geometry_str);

				// construct a std::string using the LPCSTR input
				std::string geo_string (pszConvertedAnsiString);

				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();

				if(CoordinateVector.size() >=1)
				{
				x = CoordinateVector[0].X;
				y = CoordinateVector[0].Y;
				}else
				{
				
					AfxMessageBox("Processing error in parsing geometry string in 1_node data sheet.");
					return;
				}

				}else
				{

				 x = rsNode.GetDouble("x",bExist,false);
				if(!bExist) 
				{
					m_MessageList.AddString ("Field x cannot be found in the node table.");
					rsNode.Close();
					return;
				}

				y = rsNode.GetDouble("y",bExist,false);
				if(!bExist) 
				{
					m_MessageList.AddString ("Field y cannot be found in the node table.");
					rsNode.Close();
					return;
				}
				}

			// Create and insert the node
			DTANode* pNode = new DTANode;
			pNode->pt.x = x;
			pNode->pt.y = y;

			pNode->m_NodeNumber = id;
			pNode->m_NodeNo = i;
			pNode->m_ZoneID = 0;
			pNode->m_ControlType = control_type;

			m_pDoc->m_NodeSet.push_back(pNode);
			m_pDoc->m_NodeNoMap[i] = pNode;
			m_pDoc->m_NodeNotoNumberMap[i] = id;
			m_pDoc->m_NodeNumberMap[id] = pNode;

			m_pDoc->m_NodeNumbertoNodeNoMap[id] = i;
			i++;

		}	// end of while
		rsNode.Close();

	}else
	{
		str_msg.Format ( "Worksheet node cannot be found in the given Excel file");
		m_MessageList.AddString (str_msg);
		return;
	}

	int connector_link_type =  0 ;

	//// Read record
	//	CXLEzAutomation rsLinkType;
	//	rsLinkType.Open(dbOpenDynaset, strSQL);

	//	while(!rsLinkType.ReadRecord())
	//	{
	//		DTALinkType element;
	//		int link_type_number = rsLinkType.GetLong("link_type",bExist,false);
	//		if(!bExist) 
	//		{
	//			CString Message;
	//			Message.Format("Field link_type cannot be found in the link-type sheeet.");

	//			m_MessageList.AddString (Message);
	//			return;
	//		}
	//		if(link_type_number ==0)
	//			break;

	//		element.link_type = link_type_number;
	//		element.link_type_name  = rsLinkType.GetCString("link_type_name"));
	//		element.type_code    = rsLinkType.GetCString ("type_code"));

	//		if(element.type_code.find("c") != string::npos)
	//				connector_link_type = element.link_type;

	//		element.default_lane_capacity     = rsLinkType.GetLong("default_lane_capacity",bExist,false);
	//		element.default_speed      = rsLinkType.GetFloat  ("default_speed_limit"));
	//		element.default_number_of_lanes       = rsLinkType.GetLong ("default_number_of_lanes",bExist,false);
	//		m_pDoc->m_LinkTypeMap[element.link_type] = element;

	//		rsLinkType.MoveNext ();
	//	}
	//	rsLinkType.Close();
	//	str_msg.Format ("%d link type definitions imported.",m_pDoc->m_LinkTypeMap.size());
	//	m_MessageList.AddString(str_msg);
	//}else
	//{
	//	str_msg.Format ( "Worksheet 2-1-link-type cannot be found in the given Excel file");
	//	m_MessageList.AddString (str_msg);
	//	return;
	//}

	//if(m_pDoc->m_NodeSet.size() == 0 && m_AutogenerateNodeFlag == true)
	//{

	//	str_msg.Format ( "Worksheet 1-node contain 0 node.");
	//	m_MessageList.AddString (str_msg);
	//	str_msg.Format ( "The geometry field in the link table is used to generate node info.");
	//	m_MessageList.AddString (str_msg);

	//}else
	//{
	//	str_msg.Format ( "%d nodes have been successfully imported.",m_pDoc->m_NodeSet.size());
	//	m_MessageList.AddString (str_msg);
	//}
	
	/////////////////////////////////////////////////////////////////////

	//	// Read record to obtain the overall max and min x and y;

	//double min_x = 0;
	//double max_x = 0;
	//double min_y = 0;
	//double max_y = 0;

	//bool b_RectangleInitialized = false;

	//if(m_AutogenerateNodeFlag)
	//{
	//		CXLEzAutomation rsLink;
	//		rsLink.Open(dbOpenDynaset, strSQL);

	//		int from_node_id;
	//		int to_node_id ;
	//		while(!rsLink.ReadRecord())
	//		{
	//			std::vector<CCoordinate> CoordinateVector;

	//			CString geometry_str;
	//			geometry_str= rsLink.GetCString("geometry",false);

	//			if(m_AutogenerateNodeFlag && geometry_str.GetLength () ==0)
	//			{
	//				m_MessageList.AddString("Field geometry cannot be found in the link table. This is required when no node info is given.");
	//				rsLink.Close();
	//				return;
	//			}

	//			if(geometry_str.GetLength () > 0)
	//			{

	//			CT2CA pszConvertedAnsiString (geometry_str);

	//			// construct a std::string using the LPCSTR input
	//			std::string geo_string (pszConvertedAnsiString);

	//			CGeometry geometry(geo_string);
	//			CoordinateVector = geometry.GetCoordinateList();

	//			if(b_RectangleInitialized==false && CoordinateVector.size()>=1)
	//			{
	//			
	//				min_x = max_x = CoordinateVector[0].X;
	//				min_y = max_y = CoordinateVector[0].Y;
	//				b_RectangleInitialized = true;

	//			}else
	//			{

	//					min_x= min(min_x,CoordinateVector[0].X);
	//					min_x= min(min_x,CoordinateVector[CoordinateVector.size()-1].X);

	//					min_y= min(min_y,CoordinateVector[0].Y);
	//					min_y= min(min_y,CoordinateVector[CoordinateVector.size()-1].Y);

	//					max_x= max(max_x,CoordinateVector[0].X);
	//					max_x= max(max_x,CoordinateVector[CoordinateVector.size()-1].X);

	//					max_y= max(max_y,CoordinateVector[0].Y);
	//					max_y= max(max_y,CoordinateVector[CoordinateVector.size()-1].Y);
	//		
	//			}
	//			}

	//
	//			//				TRACE("reading line %d\n", line_no);
	//		}

	//		rsLink.Close();
	//	}
	//}

//	double min_distance_threadshold_for_overlapping_nodes = ((max_y- min_y) + (max_x - min_x))/100000.0;

	double min_distance_threadshold_for_overlapping_nodes = 0.0001;
	/////////////////////////////////////////////////////////////////////
		// Read record
		bool b_default_number_of_lanes_used = false;
		bool b_default_lane_capacity_used = false;

		int number_of_records_read = 0;

		int line_no = 2;

			m_pDoc->m_bLinkToBeShifted = true;
			CXLEzAutomation rsLink;
			if(rsLink.OpenFile(m_Edit_Excel_File, "Link", 3))
			{
			int i = 0;
			float default_distance_sum = 0;
			float length_sum = 0;
			int from_node_id;
			int to_node_id ;
			while(rsLink.ReadRecord())
			{
				if(m_AutogenerateNodeFlag == false)
				{

				from_node_id = rsLink.GetLong("from_node_id",bExist,false);
				if(!bExist ) 
				{
					if(m_pDoc->m_LinkSet.size() ==0)
					{
					// no link has been loaded
					m_MessageList.AddString ("Field from_node_id cannot be found in the link table.");
					rsLink.Close();
					return;
					}else
					{
					break;
					// moveon					
					}
				}else
				{
					if(from_node_id == 0 && m_pDoc->m_LinkSet.size() ==0)
					{
					
					m_MessageList.AddString ("Field from_node_id has no valid data in the link table.");

					AfxMessageBox("Field from_node_id has no valid data in the link table.\n");
					rsLink.Close();
					break;
					}
				

				}

				to_node_id = rsLink.GetLong("to_node_id",bExist,false);
				if(!bExist) 
				{
					m_MessageList.AddString("Field to_node_id cannot be found in the link table.");
					rsLink.Close();
					return;
				}


				if(from_node_id==0 && to_node_id ==0)  // test twice here for from and to nodes
					break;


				}
				long link_id =  rsLink.GetLong("link_id",bExist,false);
				if(!bExist)
					link_id = 0;


				int type = rsLink.GetLong("link_type",bExist,false);
				if(!bExist) 
				{
					str_msg.Format("Field link_type cannot be found or has no value at row %d in the link sheet. Skip record.", line_no);
					m_MessageList.AddString(str_msg);
					continue;
				}


				if(m_AutogenerateNodeFlag == false && m_pDoc->m_NodeNumbertoNodeNoMap.find(from_node_id)== m_pDoc->m_NodeNumbertoNodeNoMap.end())
				{
					str_msg.Format("from_node_id %d at row %d cannot be found in the link sheet!",from_node_id, line_no);
					m_MessageList.AddString(str_msg);
					continue;
				}

				if(m_AutogenerateNodeFlag == false && m_pDoc->m_NodeNumbertoNodeNoMap.find(to_node_id)== m_pDoc->m_NodeNumbertoNodeNoMap.end())
				{
					str_msg.Format("to_node_id %d at row %d cannot be found in the link sheet!",to_node_id, line_no);
					m_MessageList.AddString(str_msg);
					continue;
				}


				CString SpeedSensorID_str;
				SpeedSensorID_str= rsLink.GetCString("speed_sensor_id");

				CString CountSensorID_str;
				CountSensorID_str= rsLink.GetCString("count_sensor_id");

				CString link_key_str;
				link_key_str= rsLink.GetCString("link_key");

				std::vector<CCoordinate> CoordinateVector;

				CString geometry_str;
				geometry_str= rsLink.GetCString("geometry");

				if(m_AutogenerateNodeFlag && geometry_str.GetLength () ==0)
				{
				
					m_MessageList.AddString("Field geometry cannot be found in the link table. This is required when no node info is given.");
					rsLink.Close();
					return;
				}

				if(geometry_str.GetLength () > 0)
				{

					CT2CA pszConvertedAnsiString (geometry_str);

				// construct a std::string using the LPCSTR input
				std::string geo_string (pszConvertedAnsiString);

				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();

				if(m_AutogenerateNodeFlag&& CoordinateVector.size() > 0)  // add nodes
				{

					from_node_id = m_pDoc->FindNodeNumberWithCoordinate(CoordinateVector[0].X,CoordinateVector[0].Y,min_distance_threadshold_for_overlapping_nodes);
					// from node
					if(from_node_id == 0)
					{
							GDPoint	pt;
							pt.x = CoordinateVector[0].X;
							pt.y = CoordinateVector[0].Y;
							
							bool ActivityLocationFlag = false;
							if(m_pDoc->m_LinkTypeMap[type ].IsConnector ()) // adjacent node of connectors
								ActivityLocationFlag = true;

							DTANode* pNode = m_pDoc->AddNewNode(pt, from_node_id, 0,ActivityLocationFlag);
							from_node_id = pNode->m_NodeNumber;  // update to_node_id after creating new node
							pNode->m_bCreatedbyNEXTA = true;

					}

					// to node
					to_node_id =  m_pDoc->FindNodeNumberWithCoordinate(CoordinateVector[CoordinateVector.size()-1].X,CoordinateVector[CoordinateVector.size()-1].Y,min_distance_threadshold_for_overlapping_nodes);
					// from node
					if(to_node_id==0)
					{
							GDPoint	pt;
							pt.x = CoordinateVector[CoordinateVector.size()-1].X;
							pt.y = CoordinateVector[CoordinateVector.size()-1].Y;

							bool ActivityLocationFlag = false;
							if(m_pDoc->m_LinkTypeMap[type ].IsConnector ()) // adjacent node of connectors
								ActivityLocationFlag = true;

							DTANode* pNode = m_pDoc->AddNewNode(pt, to_node_id, 0,ActivityLocationFlag);
							to_node_id = pNode->m_NodeNumber;  // update to_node_id after creating new node
							pNode->m_bCreatedbyNEXTA = true;
					}

				}

				}

				if(m_bRemoveConnectors && m_pDoc->m_LinkTypeMap[type ].IsConnector()) 
				{  // skip connectors
					continue;
				}


				DTALink* pExistingLink =  m_pDoc->FindLinkWithNodeIDs(m_pDoc->m_NodeNumbertoNodeNoMap[from_node_id],m_pDoc->m_NodeNumbertoNodeNoMap[to_node_id]);

				if(pExistingLink)
				{
					str_msg.Format ("Link %d-> %d at row %d is duplicated with the previous link at row %d.\n", from_node_id,to_node_id, line_no, pExistingLink->input_line_no);
					if(m_MessageList.GetCount () < 3000)  // not adding and showing too many links
					{
						m_MessageList.AddString (str_msg);
						continue;
					}
				}

				float length = rsLink.GetDouble("length",bExist,false);
				if(!bExist) 
				{
					m_MessageList.AddString ("Field length cannot be found in the link table.");
					rsLink.Close();
					return;
				}
				if(length > 100)
				{
					str_msg.Format("The length of link %d -> %d is longer than 100 miles, please ensure the unit of link length in the link sheet is mile.",from_node_id,to_node_id);
					m_MessageList.AddString(str_msg);
					rsLink.Close();
					return;
				}

				int number_of_lanes = rsLink.GetLong("number_of_lanes",bExist,false);
				if(m_bUseLinkTypeForDefaultValues)
				{
				if(number_of_lanes<1)
				{
					if(m_pDoc->m_LinkTypeMap.find(type) != m_pDoc->m_LinkTypeMap.end())
					{

						number_of_lanes = m_pDoc->m_LinkTypeMap[type].default_number_of_lanes;

						if(b_default_number_of_lanes_used)
						{
						m_MessageList.AddString("Field number_of_lanes cannot be found in the link table.");
						m_MessageList.AddString("default_number_of_lanes from 2-link-type table is used.");
						b_default_number_of_lanes_used =true;
						}
					}else
					{
						CString link_type_str = rsLink.GetCString("link_type");


						if(m_pDoc->m_LinkTypeMap.size()>0)
						{
							std::map<int, DTALinkType>::iterator iter = m_pDoc->m_LinkTypeMap.begin ();
							type = iter->first;
						}

						CString link_type_message;
						link_type_message.Format ("link type %s for link %->%d is invald.", link_type_str);
						m_MessageList.AddString(link_type_message);

					}
					
				}else
					{
					m_MessageList.AddString("Field number_of_lanes cannot be found in the link table.");
					m_MessageList.AddString("default_number_of_lanes for this link type has not been defined in link_type table.");
					rsLink.Close();
					return;
					}
				}

				if(number_of_lanes <=0)
				{
					str_msg.Format ("number of lanes for link %d -> %d <= 0. Skip.",from_node_id,to_node_id);
					m_MessageList.AddString(str_msg);
					continue; 
				}



				float grade= 0;
				float speed_limit_in_mph= rsLink.GetLong("speed_limit",bExist,false);

				if(speed_limit_in_mph <1 && m_bUseLinkTypeForDefaultValues)
				{
					if(m_pDoc->m_LinkTypeMap.find(type) != m_pDoc->m_LinkTypeMap.end())
					{

						speed_limit_in_mph = m_pDoc->m_LinkTypeMap[type].default_speed ;
						bExist = true;


					}
				}
				if(!bExist) 
				{
					AfxMessageBox("Field speed_limit_in_mph cannot be found in the link table.");
					rsLink.Close();
					return;
				}

				if(speed_limit_in_mph ==0)
				{
					str_msg.Format ("Link %d -> %d has a speed limit of 0. Skip.",from_node_id,to_node_id);
					m_MessageList.AddString(str_msg);
					continue; 
				}

				float capacity_in_pcphpl= rsLink.GetDouble("lane_capacity_per_hour",bExist,false);
				if(capacity_in_pcphpl<0.1 && m_bUseLinkTypeForDefaultValues)
				{
					if(m_pDoc->m_LinkTypeMap.find(type) != m_pDoc->m_LinkTypeMap.end())
					{

						capacity_in_pcphpl = m_pDoc->m_LinkTypeMap[type].default_lane_capacity ;

						if(b_default_lane_capacity_used)
						{
						m_MessageList.AddString("Field capacity_in_veh_per_hour_per_lane cannot be found in the link table.");
						m_MessageList.AddString("default_lane_capacity from 2-link-type table is used.");
						b_default_lane_capacity_used =true;
						}
					
					}

				}

				if(capacity_in_pcphpl<0)
				{

					str_msg.Format ( "Link %d -> %d has a negative capacity, please sort the link table by capacity_in_veh_per_hour_per_lane and re-check it!",from_node_id,to_node_id);
					AfxMessageBox(str_msg, MB_ICONINFORMATION);
					rsLink.Close();
					return;
				}


				int direction = rsLink.GetLong("direction",bExist,false);
				if(!bExist) 
				{
					m_MessageList.AddString("Field direction cannot be found in the link table.");
					rsLink.Close();
					return;
				}

				CString name = rsLink.GetCString("name");

				float k_jam, wave_speed_in_mph;

				if(type==1)
				{
					k_jam = 220;
				}else
				{
					k_jam = 190;
				}

				wave_speed_in_mph = 12;


				int m_SimulationHorizon = 1;

				int link_code_start = 1;
				int link_code_end = 1;

				if (direction == -1) // reversed
				{
					link_code_start = 2; link_code_end = 2;
				}


				if (direction == 0 || direction ==2) // two-directional link
				{
					link_code_start = 1; link_code_end = 2;
				}

				if(m_AutogenerateNodeFlag == false)
				{
				// no geometry information
				CCoordinate cc_from, cc_to; 
				cc_from.X = m_pDoc->m_NodeNoMap[m_pDoc->m_NodeNumbertoNodeNoMap[from_node_id]]->pt.x;
				cc_from.Y = m_pDoc->m_NodeNoMap[m_pDoc->m_NodeNumbertoNodeNoMap[from_node_id]]->pt.y;

				cc_to.X = m_pDoc->m_NodeNoMap[m_pDoc->m_NodeNumbertoNodeNoMap[to_node_id]]->pt.x;
				cc_to.Y = m_pDoc->m_NodeNoMap[m_pDoc->m_NodeNumbertoNodeNoMap[to_node_id]]->pt.y;

				CoordinateVector.push_back(cc_from);
				CoordinateVector.push_back(cc_to);
				}

				for(int link_code = link_code_start; link_code <=link_code_end; link_code++)
				{

					bool bNodeNonExistError = false;
					int m_SimulationHorizon = 1;
					DTALink* pLink = new DTALink(m_SimulationHorizon);
					pLink->m_LinkNo = i;
					pLink->m_Name  = name;
					pLink->m_OrgDir = direction;
					pLink->m_LinkID = link_id;
					pLink->m_SpeedSensorID  = m_pDoc->CString2StdString(SpeedSensorID_str);
					pLink->m_LinkKey  = link_key_str;
					pLink->m_CountSensorID = m_pDoc->CString2StdString(CountSensorID_str);

					m_pDoc->m_LinkKeyMap[link_key_str] = pLink;
					m_pDoc->m_SpeedSensorIDMap[pLink->m_SpeedSensorID ] = pLink;
					m_pDoc->m_CountSensorIDMap[pLink->m_CountSensorID ] = pLink;


					pLink->ResetMOEAry(g_Simulation_Time_Horizon);  // use one day horizon as the default value

					if(link_code == 1)  //AB link
					{
						pLink->m_FromNodeNumber = from_node_id;

						pLink->m_ToNodeNumber = to_node_id;
						pLink->m_Direction  = 1;

						pLink->m_FromNodeID = m_pDoc->m_NodeNumbertoNodeNoMap[from_node_id];
						pLink->m_ToNodeID= m_pDoc->m_NodeNumbertoNodeNoMap[to_node_id];


						for(unsigned si = 0; si < CoordinateVector.size(); si++)
						{
							GDPoint	pt;
							pt.x = CoordinateVector[si].X;
							pt.y = CoordinateVector[si].Y;
							pLink->m_ShapePoints .push_back (pt);
						}

					}

					if(link_code == 2)  //BA link
					{
						pLink->m_FromNodeNumber = to_node_id;
						pLink->m_ToNodeNumber = from_node_id;
						pLink->m_Direction  = 1;
						pLink->m_FromNodeID = m_pDoc->m_NodeNumbertoNodeNoMap[to_node_id];
						pLink->m_ToNodeID= m_pDoc->m_NodeNumbertoNodeNoMap[from_node_id];


						for(int si = CoordinateVector.size()-1; si >=0; si--)  // we need to put int here as si can be -1. 
						{
							GDPoint	pt;
							pt.x = CoordinateVector[si].X;
							pt.y = CoordinateVector[si].Y;
							pLink->m_ShapePoints .push_back (pt);
						}
					}

					pLink->m_NumberOfLanes= number_of_lanes;
					pLink->m_SpeedLimit= speed_limit_in_mph;
					pLink->m_avg_simulated_speed = pLink->m_SpeedLimit;
					pLink->m_Length= length;  // minimum distance

					if(length < 0.00001) // zero value in length field, we consider no length info.
					{
						float distance_in_mile = g_CalculateP2PDistanceInMileFromLatitudeLongitude(pLink->m_ShapePoints[0], pLink->m_ShapePoints[pLink->m_ShapePoints.size()-1]);
						pLink->m_Length = distance_in_mile;
					}
					default_distance_sum+= pLink->DefaultDistance();
					length_sum += pLink ->m_Length;
	

					pLink->m_FreeFlowTravelTime = pLink->m_Length / max(1,pLink->m_SpeedLimit) *60.0f;
					pLink->m_StaticTravelTime = pLink->m_FreeFlowTravelTime;

					pLink->m_MaximumServiceFlowRatePHPL= capacity_in_pcphpl;
					pLink->m_LaneCapacity  = pLink->m_MaximumServiceFlowRatePHPL;
					pLink->m_link_type= type;
					pLink->m_Grade = grade;


					if(link_code == 2)  //BA link
					{

						int R_number_of_lanes = number_of_lanes;

						float R_speed_limit_in_mph= speed_limit_in_mph;

						float R_lane_cap= capacity_in_pcphpl;
						float R_grade= grade;
						pLink->m_NumberOfLanes= R_number_of_lanes;
						pLink->m_SpeedLimit= R_speed_limit_in_mph;
						pLink->m_MaximumServiceFlowRatePHPL= R_lane_cap;
						pLink->m_Grade = R_grade;

						pLink->m_avg_simulated_speed = pLink->m_SpeedLimit;
						pLink->m_Length= max(length, pLink->m_SpeedLimit*0.1f/60.0f);  // minimum distance
						pLink->m_FreeFlowTravelTime = pLink->m_Length / max(1,pLink->m_SpeedLimit) *60.0f;
						pLink->m_StaticTravelTime = pLink->m_FreeFlowTravelTime;
						pLink->m_LaneCapacity  = pLink->m_MaximumServiceFlowRatePHPL;
						pLink->m_link_type= type;



					}

	
					double BPR_alpha_term = 0.15;
					double BPR_beta_term = 4;
					double transit_transfer_time_in_min	= 1;
					double transit_waiting_time_in_min = 3;
					double transit_fare = 1;

					BPR_alpha_term = rsLink.GetDouble("BPR_alpha_term",bExist,false);

					if(BPR_alpha_term > 0.000001)
					pLink->m_BPR_alpha_term = BPR_alpha_term;

					BPR_beta_term = rsLink.GetDouble("BPR_beta_term",bExist,false);

					if(BPR_beta_term > 0.00000001)
					pLink->m_BPR_beta_term = BPR_beta_term;

					pLink->m_total_link_volume = rsLink.GetDouble("static_volume_per_hour",bExist,false); 


					if(pLink->m_total_link_volume >=1)
					{
					pLink->m_avg_simulated_speed = rsLink.GetDouble("static_speed_per_hour",bExist,false); 
					}else
					{
					pLink->m_avg_simulated_speed = pLink->m_SpeedLimit ;
					}
					


					pLink->m_Kjam = k_jam;
					pLink->m_Wave_speed_in_mph  = wave_speed_in_mph;

					m_pDoc->m_NodeNoMap[pLink->m_FromNodeID ]->m_TotalCapacity += (pLink->m_MaximumServiceFlowRatePHPL* pLink->m_NumberOfLanes);

					pLink->m_FromPoint = m_pDoc->m_NodeNoMap[pLink->m_FromNodeID]->pt;
					pLink->m_ToPoint = m_pDoc->m_NodeNoMap[pLink->m_ToNodeID]->pt;


					//			pLink->SetupMOE();
					pLink->input_line_no  = line_no;
					
					

					m_pDoc->m_LinkSet.push_back (pLink);
					m_pDoc->m_LinkNoMap[i]  = pLink;

					unsigned long LinkKey = m_pDoc->GetLinkKey( pLink->m_FromNodeID, pLink->m_ToNodeID);

					m_pDoc->m_NodeNotoLinkMap[LinkKey] = pLink;

					__int64  LinkKey2 = m_pDoc->GetLink64Key(pLink-> m_FromNodeNumber,pLink->m_ToNodeNumber);
					m_pDoc->m_NodeNumbertoLinkMap[LinkKey2] = pLink;

					m_pDoc->m_LinkNotoLinkMap[i] = pLink;

					m_pDoc->m_LinkIDtoLinkMap[link_id] = pLink;


					m_pDoc->m_NodeNoMap[pLink->m_FromNodeID ]->m_Connections+=1;
					m_pDoc->m_NodeNoMap[pLink->m_ToNodeID ]->m_Connections+=1;




					if(m_pDoc->m_LinkTypeMap[type ].IsConnector ()) // adjacent node of connectors
					{ 
						// mark them as activity location 
					m_pDoc->m_NodeNoMap[pLink->m_FromNodeID ]->m_bZoneActivityLocationFlag = true;					
					m_pDoc->m_NodeNoMap[pLink->m_ToNodeID ]->m_bZoneActivityLocationFlag = true;					
					}


					m_pDoc->m_NodeNoMap[pLink->m_FromNodeID ]->m_OutgoingLinkVector.push_back(i);
					m_pDoc->m_NodeNoMap[pLink->m_ToNodeID ]->m_IncomingLinkVector.push_back(i);

					if(m_pDoc->m_LinkTypeMap[pLink->m_link_type].IsConnector  () == false )
						m_pDoc->m_NodeNoMap[pLink->m_ToNodeID ]->m_IncomingNonConnectors++;


					i++;

				}

				//				TRACE("reading line %d\n", line_no);
				line_no ++;
				number_of_records_read ++;
			}

			rsLink.Close();

			m_pDoc->m_UnitMile  = 1.0f;

			if(length_sum>0.000001f)
				m_pDoc->m_UnitMile=  default_distance_sum /length_sum;

			double AvgLinkLength = length_sum / max(1,m_pDoc->m_LinkSet.size());

			if(m_pDoc->m_bUseMileVsKMFlag)
			{
				m_pDoc->m_UnitFeet = m_pDoc->m_UnitMile/5280.0f;  
				m_pDoc->m_NodeDisplaySize = max(100, AvgLinkLength*5280*0.05);  // in feet
			}
			else
			{
				m_pDoc->m_UnitFeet = m_pDoc->m_UnitMile/1000*3.28084f;  // meter to feet
				m_pDoc->m_NodeDisplaySize = max(100, AvgLinkLength/1.61*5280*0.05);  // in feet
			}


			
			m_pDoc->GenerateOffsetLinkBand();

			/*
			if(m_UnitMile>50)  // long/lat must be very large and greater than 62!
			{

			if(AfxMessageBox("Is the long/lat coordinate system used in this data set?", MB_YESNO) == IDYES)
			{
			m_LongLatCoordinateFlag = true;
			m_UnitFeet = m_UnitMile/62/5280.0f;  // 62 is 1 long = 62 miles
			}
			}
			*/

			m_pDoc->OffsetLink();

		}else
		{
			str_msg.Format ( "Worksheet Link cannot be found in the given Excel file");
			m_MessageList.AddString (str_msg);
			return;
		}


		str_msg.Format ( "%d nodes have been successfully imported.",m_pDoc->m_NodeSet.size());
		m_MessageList.AddString (str_msg);

		str_msg.Format ("%d links have been successfully imported from %d records.",m_pDoc->m_LinkSet.size(),number_of_records_read);
		m_MessageList.AddString(str_msg);
	

		// test if we need to add connectors for isolated nodes
		m_bAddConnectorsForIsolatedNodes = false;
		{
			for (std::list<DTANode*>::iterator  iNode = m_pDoc->m_NodeSet.begin(); iNode != m_pDoc->m_NodeSet.end(); iNode++)
			{
				if((*iNode)->m_bCreatedbyNEXTA == false)
				{

					if((*iNode)->m_Connections ==0)
					{
					
					m_bAddConnectorsForIsolatedNodes = true;

					str_msg.Format ("There are isoluated nodes in node table. Connectors will be added automatically.");
					m_MessageList.AddString(str_msg);
					break;
					}
				}

			}
		}

		if(m_bAddConnectorsForIsolatedNodes)
		{

			if(connector_link_type == 0)
			{
				AfxMessageBox("The link type for connectors has not been defined.");

				return;
			}
			m_pDoc->m_DefaultSpeedLimit = 10;
			m_pDoc->m_DefaultCapacity = 10000;
			m_pDoc->m_DefaultLinkType = connector_link_type; //connector

			int number_of_new_connectors = 0;

			for (std::list<DTANode*>::iterator  iNode = m_pDoc->m_NodeSet.begin(); iNode != m_pDoc->m_NodeSet.end(); iNode++)
			{
				if((*iNode)->m_Connections == 0)
				{

					int NodeNumber = m_pDoc->FindNonCentroidNodeNumberWithCoordinate((*iNode)->pt .x ,(*iNode)->pt .y , (*iNode)->m_NodeNumber);
					
					 m_pDoc->AddNewLinkWithNodeNumbers((*iNode)->m_NodeNumber , NodeNumber);
					 m_pDoc->AddNewLinkWithNodeNumbers(NodeNumber,(*iNode)->m_NodeNumber);
				
					number_of_new_connectors ++;
				}

			}
		
			str_msg.Format ("%d connectors have been successfully created",number_of_new_connectors);
			m_MessageList.AddString(str_msg);

		}

	//test if activity location has been defined


	// activity location table
	//bool bNodeNonExistError = false;
	m_pDoc->m_NodeNotoZoneNameMap.clear ();

	bool bNodeNonExistError = false;
	m_pDoc->m_NodeNotoZoneNameMap.clear ();
	m_pDoc->m_ODSize = 0;

	// Read record
	int activity_location_count = 0;

		CXLEzAutomation rsActivityLocation;
		rsActivityLocation.OpenFile(m_Edit_Excel_File, "ActivityLocation", 5);


		while(rsActivityLocation.ReadRecord())
		{
			int zone_number = rsActivityLocation.GetLong("zone_id",bExist,false);
			if(zone_number <=0)
				break;

			int node_name = rsActivityLocation.GetLong("node_id",bExist,false);

			map <int, int> :: const_iterator m_Iter = m_pDoc->m_NodeNumbertoNodeNoMap.find(node_name);

			if(m_Iter == m_pDoc->m_NodeNumbertoNodeNoMap.end( ))
			{
				CString m_Warning;
				m_Warning.Format("Node ID %d in the ActivityLocation sheet has not been defined in the Node sheet", node_name);
				AfxMessageBox(m_Warning);
				return;
			}
			m_pDoc->m_NodeNotoZoneNameMap[m_pDoc->m_NodeNumbertoNodeNoMap[node_name]] = zone_number;

			m_pDoc->m_NodeNoMap [ m_pDoc->m_NodeNumbertoNodeNoMap[node_name] ] ->m_bZoneActivityLocationFlag = true;
			m_pDoc->m_NodeNoMap [ m_pDoc->m_NodeNumbertoNodeNoMap[node_name] ] -> m_ZoneID = zone_number;
			// if there are multiple nodes for a zone, the last node id is recorded.


			DTAActivityLocation element;
			element.ZoneID  = zone_number;
			element.NodeNumber = node_name;

			element.External_OD_flag  = rsActivityLocation.GetLong("external_OD_flag",bExist,false);
			element.ActivityType  = rsActivityLocation.GetCString ("activity_type");

			m_pDoc->m_ZoneMap [zone_number].m_ActivityLocationVector .push_back (element);

			if(m_pDoc->m_ODSize < zone_number)
				m_pDoc->m_ODSize = zone_number;

			activity_location_count++;
		}
		rsActivityLocation.Close();

		str_msg.Format ( "%d activity location records imported.",activity_location_count);
		m_MessageList.AddString (str_msg);
	

		CXLEzAutomation rsZone;
		int count = 0;
		if(rsZone.OpenFile(m_Edit_Excel_File, "Zone", 4))
		{

		while(rsZone.ReadRecord())
		{
			int zone_number = rsZone.GetLong("zone_id",bExist,false);
			if(!bExist) 
			{
				AfxMessageBox("Field zone_id cannot be found in the zone table.");
				return;
			}

			if(zone_number ==0)
				break;

		//	 if there are multiple nodes for a zone, the last node id is recorded.
				std::vector<CCoordinate> CoordinateVector;

				CString geometry_str = rsZone.GetCString("geometry");

				if(geometry_str.GetLength () > 0)
				{

				CT2CA pszConvertedAnsiString (geometry_str);

				// construct a std::string using the LPCSTR input
				std::string geo_string (pszConvertedAnsiString);

				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();

				m_pDoc->m_ZoneMap [zone_number].m_ZoneID = zone_number;

			for(unsigned int f = 0; f < CoordinateVector.size(); f++)
			{
				GDPoint pt;
				pt.x = CoordinateVector[f].X;
				pt.y = CoordinateVector[f].Y;
				m_pDoc->m_ZoneMap [zone_number].m_ShapePoints.push_back (pt);
			}

				}
			if(m_pDoc->m_ODSize < zone_number)
				m_pDoc->m_ODSize = zone_number;
				

			count++;
		}
		rsZone.Close();

		str_msg.Format ( "%d zone boundary records are imported.",count);
		m_MessageList.AddString (str_msg);
	
		}
		// assign zone numbers to connectors

		if(count>=1)  // with boundary
		{
		std::list<DTALink*>::iterator iLink;

			for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
			{
				if(m_pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsConnector ())  // connectors
				{
					
					GDPoint pt_from = (*iLink)->m_FromPoint ;
					int ZoneID_from = m_pDoc->GetZoneID(pt_from);

					// assign id according to upstream node zone number first
					int ZoneID_to = 0;
					if(ZoneID_from <=0)
					{
					GDPoint pt_to = (*iLink)->m_ToPoint ;
					 ZoneID_to = m_pDoc->GetZoneID(pt_to);
					}

					// assign id according to downstream node zone number second

					int ZoneID = max(ZoneID_from, ZoneID_to);  // get large zone id under two different zone numbers
					if(ZoneID > 0)
						(*iLink)->m_ConnectorZoneID = ZoneID;

				}
			}
		}
		

		// determine activity locations if no activity locations have been provided

		if(activity_location_count == 0)
		{
		std::list<DTANode*>::iterator iNode;

		for (iNode = m_pDoc->m_NodeSet.begin(); iNode != m_pDoc->m_NodeSet.end(); iNode++)
		{


			if((*iNode )->m_bZoneActivityLocationFlag)
			{
					int ZoneID = m_pDoc->GetZoneID((*iNode)->pt);
					if(ZoneID>0)
					{
						(*iNode )->m_ZoneID = ZoneID;
						DTAActivityLocation element;
						element.ZoneID  = ZoneID;
						element.NodeNumber = (*iNode )->m_NodeNumber;
						m_pDoc->m_ZoneMap [ZoneID].m_ActivityLocationVector .push_back (element );

					}
			
				}
		
		}
		str_msg.Format ( "%d activity locations identified",activity_location_count);
		m_MessageList.AddString (str_msg);
			
		}

		if(m_bRemoveConnectors)
		{
		std::list<DTANode*>::iterator iNode;

		std::vector <int> CentroidVector;
		for (iNode = m_pDoc->m_NodeSet.begin(); iNode != m_pDoc->m_NodeSet.end(); iNode++)
		{
			if((*iNode )->m_bZoneActivityLocationFlag && (*iNode )->m_Connections ==0)  // has been as activity location but no link connected
				CentroidVector.push_back((*iNode )->m_NodeNo );

		}

		for(unsigned int i = 0; i < CentroidVector.size(); i++)
		{
			m_pDoc->DeleteNode (CentroidVector[i]);
		}
		
		str_msg.Format ( "%d centroids deleted",CentroidVector.size());
		m_MessageList.AddString (str_msg);
		
		}

		//import demand

	int demand_type = 0;

	if(m_ImportZoneData)
	{
	m_pDoc->m_DemandMatrixMap .clear ();

	CXLEzAutomation rsDemand;
		if(rsDemand.OpenFile(m_Edit_Excel_File, "DemandMatrix", 6))
		{

		while(rsDemand.ReadRecord())
		{
			int from_zone_id = rsDemand.GetLong("zone_id",bExist,false);

			if(from_zone_id==0)
			{
				break;
			}


			if(m_pDoc-> m_ZoneMap.find(from_zone_id)== m_pDoc->m_ZoneMap.end())
			{
				CString message;
				message.Format("from_zone_id %d in the demand matrix has not been defined.", from_zone_id );
				AfxMessageBox(message);
				break;
			}

			int to_zone_id = 0;
		std::map<int, DTAZone>	:: const_iterator itr;

			int to_zone_index = 1;
		for(itr = m_pDoc->m_ZoneMap.begin(); itr != m_pDoc->m_ZoneMap.end(); itr++,to_zone_index++)
		{
			to_zone_id = itr->first;

			CString str;
			str.Format ("%d",to_zone_id);
			std::string to_zone_str = m_pDoc->CString2StdString(str);

			float number_of_vehicles = rsDemand.GetDouble(to_zone_str,bExist,false);

			if(number_of_vehicles < -0.1)
			{
				CString message;
				message.Format("number_of_vehicles %f in the demand matrix is invalid.", number_of_vehicles);
				AfxMessageBox(message);
				break;
			}

			m_pDoc->SetODDemandValue (1,from_zone_id,to_zone_id,number_of_vehicles);
		}  // for all to zone id


		}
		rsDemand.Close();

		str_msg.Format ( "%d demand element records imported.", m_pDoc->m_DemandMatrixMap.size());
		m_MessageList.AddString (str_msg);
		
		}
	}
/// simulation Link MOE
		if(m_bLinkMOECheck)
		{
		CXLEzAutomation rsModelLinkMOE;
		if(rsModelLinkMOE.OpenFile(m_Edit_Excel_File, "ModelLinkMOE", 7))
		{

	g_Simulation_Time_Horizon = 1440;

		for (std::list<DTALink*>::iterator iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
		{
			(*iLink)->ResetMOEAry(g_Simulation_Time_Horizon);  // use one day horizon as the default value
		}

			int count = 0;
		while(rsModelLinkMOE.ReadRecord())
		{
			CString link_key = rsModelLinkMOE.GetCString("link_key");

			if(link_key.GetLength ()==0)
			{
			break;
			}

		if(m_pDoc->m_LinkKeyMap.find(link_key)!= m_pDoc->m_LinkKeyMap.end())
		{

		int start_time_in_min = rsModelLinkMOE.GetLong("start_time_in_min",bExist,false);
		int end_time_in_min = rsModelLinkMOE.GetLong("end_time_in_min",bExist,false);

		if(start_time_in_min > end_time_in_min)
		{
		str_msg.Format ( "Error: simulation link MOE record No.%d, start_time_in_min > end_time_in_min", count);
		m_MessageList.AddString (str_msg);
		break;
		
		}

			if(start_time_in_min <  m_pDoc->m_DemandLoadingStartTimeInMin)
				m_pDoc->m_DemandLoadingStartTimeInMin = start_time_in_min;

			if(end_time_in_min >  m_pDoc->m_DemandLoadingEndTimeInMin)
				m_pDoc->m_DemandLoadingEndTimeInMin = end_time_in_min ;



		float link_hourly_volume = rsModelLinkMOE.GetDouble ("link_hourly_volume",bExist,false);
		float density = rsModelLinkMOE.GetDouble ("density",bExist,false);
		float speed_per_hour = rsModelLinkMOE.GetDouble ("speed_per_hour",bExist,false);
		float queue_length_percentage = rsModelLinkMOE.GetDouble ("queue_length_percentage",bExist,false);
		float cumulative_arrival_count = rsModelLinkMOE.GetDouble ("cumulative_arrival_count",bExist,false);
		float cumulative_departure_count = rsModelLinkMOE.GetDouble ("cumulative_departure_count",bExist,false);

		float inflow_count = rsModelLinkMOE.GetDouble ("inflow_count",bExist,false);
		float outflow_count = rsModelLinkMOE.GetDouble ("outflow_count",bExist,false);

			DTALink* pLink = m_pDoc->m_LinkKeyMap[link_key];

				if(pLink!=NULL)
			{	

				if(start_time_in_min >= g_Simulation_Time_Horizon)
					start_time_in_min = g_Simulation_Time_Horizon -1;

				if(end_time_in_min >= g_Simulation_Time_Horizon)
					end_time_in_min = g_Simulation_Time_Horizon -1;

				if(start_time_in_min<0)
					start_time_in_min = 0;

				if(end_time_in_min <0)
					end_time_in_min = 0;


				for(int t = start_time_in_min; t < end_time_in_min; t++)
				{
				pLink->m_LinkMOEAry[t].LinkFlow = link_hourly_volume;
				pLink->m_LinkMOEAry[t].Density  = density;

				pLink->m_LinkMOEAry[t].Speed = speed_per_hour;
				pLink->m_LinkMOEAry[t].QueueLength = queue_length_percentage;


				float incount = inflow_count/max(1,end_time_in_min-start_time_in_min);  // min count
				float outcount = outflow_count/max(1,end_time_in_min-start_time_in_min);  // min count

				if(t>=1)
				{
				pLink->m_LinkMOEAry[t].ArrivalCumulativeFlow = pLink->m_LinkMOEAry[t-1].ArrivalCumulativeFlow + incount;
				pLink->m_LinkMOEAry[t].DepartureCumulativeFlow = pLink->m_LinkMOEAry[t-1].DepartureCumulativeFlow + outflow_count;
				}


				
				pLink->m_LinkMOEAry[t].TravelTime = pLink->m_Length / max(0.01,speed_per_hour);
				}
			
			}
			count ++;

			if(count%5000 == 0)
			{
				str_msg.Format ( "importing %d simulation link MOE records...", count);
				m_MessageList.AddString (str_msg);
	
			}

			}


		}
		rsModelLinkMOE.Close();

		str_msg.Format ( "%d simulation link MOE records imported.", count);
		m_MessageList.AddString (str_msg);
		
		}

		}
		//--- TMC

		for (std::list<DTALink*>::iterator iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
		{
			(*iLink)->m_LinkSensorMOEMap.clear ();  // use one day horizon as the default value
		}

		if(m_TMCSpeedCheck)
		{
		CXLEzAutomation rsTMCSpeed;

		if(rsTMCSpeed.OpenFile(m_Edit_Excel_File, "SensorSpeed", 8))
		{
			int speed_data_aggregation_interval = 15;


			int count = 0;
		while(rsTMCSpeed.ReadRecord())
		{
			CString TMC = rsTMCSpeed.GetCString("speed_sensor_id");

			if(TMC.GetLength () ==0)
				break;

		int day_no = rsTMCSpeed.GetLong ("day_no",bExist,false);

		if(day_no <1)
			day_no = 1;

			g_SensorDayDataMap[ day_no] = true;
			g_SensorLastDayNo= max(g_SensorLastDayNo,  day_no);
			g_SensorDayNo = g_SensorLastDayNo;


			DTALink* pLink = NULL;

			if(m_pDoc->m_SpeedSensorIDMap.find(m_pDoc->CString2StdString(TMC))!=m_pDoc->m_SpeedSensorIDMap.end())
			{
				pLink = m_pDoc->m_SpeedSensorIDMap[m_pDoc->CString2StdString(TMC)];
				pLink->m_bSensorData = true;
				pLink->m_bSpeedSensorData  = true;

				float max_speed = 10;
				float min_speed = 100;

				for (int t = 0; t< 1440; t+= speed_data_aggregation_interval)
				{
					CString timestamp;
					timestamp.Format ("Min_%d",t);
					std::string StdString = m_pDoc->CString2StdString(timestamp);
					float speed_in_mph  = rsTMCSpeed.GetDouble (StdString,bExist,false);
					
						//TRACE("speed = %f,\n",speed_in_mph);
						if(min_speed > speed_in_mph)
							min_speed = speed_in_mph;

						if(max_speed < speed_in_mph)
							max_speed = speed_in_mph;

						for(int s= 0 ; s<speed_data_aggregation_interval; s++)
						{

							int time = day_no*1440 + t +s;  // allow shift of start time
							// day specific value-----	

							if(pLink->m_LinkSensorMOEMap.find(time) == pLink->m_LinkSensorMOEMap.end()) // no traffic count data
								pLink->m_LinkSensorMOEMap[ time].LinkFlow = 1000;

							pLink->m_LinkSensorMOEMap[ time].Speed = speed_in_mph;

						}

					

				}
			count ++;

			}
			}
		rsTMCSpeed.Close();

		str_msg.Format ( "%d sensor speed records imported.", count);
		m_MessageList.AddString (str_msg);
		
		}
		}

		if(m_SensorCountCheck)
		{
		CXLEzAutomation rsSensorCount;

		if(rsSensorCount.OpenFile(m_Edit_Excel_File, "SensorCount", 9))
		{
			std::list<DTALink*>::iterator iLink;
			for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
			{
			(*iLink)->m_total_sensor_link_volume = 0;
			}

			int count = 0;
		while(rsSensorCount.ReadRecord())
		{
			CString sensor_id = rsSensorCount.GetCString("count_sensor_id");

			if(sensor_id.GetLength () ==0)
				break;

		int day_no = rsSensorCount.GetLong ("day_no",bExist,false);

		if(day_no <1)
			day_no = 1;

			g_SensorDayDataMap[ day_no] = true;
			g_SensorLastDayNo= max(g_SensorLastDayNo,  day_no);
			g_SensorDayNo = g_SensorLastDayNo;


			DTALink* pLink = NULL;

			if(m_pDoc->m_CountSensorIDMap .find(m_pDoc->CString2StdString(sensor_id))!=m_pDoc->m_CountSensorIDMap.end())
			{
				pLink = m_pDoc->m_CountSensorIDMap[m_pDoc->CString2StdString(sensor_id)];
				pLink->m_bSensorData = true;
				pLink->m_bCountSensorData  = true;



			float volume_count = rsSensorCount.GetLong ("count",bExist,false);
				pLink->m_total_sensor_link_volume +=volume_count;

		int start_time_in_min = rsSensorCount.GetLong("start_time_in_min",bExist,false);
		int end_time_in_min = rsSensorCount.GetLong("end_time_in_min",bExist,false);

		if(start_time_in_min > end_time_in_min)
		{
		str_msg.Format ( "Error: SensorCount record No.%d, start_time_in_min > end_time_in_min", count);
		m_MessageList.AddString (str_msg);
		break;
		
		}

			if(start_time_in_min <  m_pDoc->m_DemandLoadingStartTimeInMin)
				m_pDoc->m_DemandLoadingStartTimeInMin = start_time_in_min;

			if(end_time_in_min >  m_pDoc->m_DemandLoadingEndTimeInMin)
				m_pDoc->m_DemandLoadingEndTimeInMin = end_time_in_min;


				DTASensorData element;
				element.start_time_in_min = start_time_in_min;
				element.end_time_in_min = end_time_in_min;
				element.count = volume_count;


			CString second_count_sensor_id = rsSensorCount.GetCString("second_count_sensor_id");

				if(second_count_sensor_id.GetLength () >0)
				{
				
					element.second_count_sensor_id = second_count_sensor_id;
				}
			

				pLink->m_SensorDataVector.push_back(element);


				for(int t = max(0,start_time_in_min); t< min (1440,end_time_in_min); t++)
				{
					int time = day_no*1440 + t;  // allow shift of start time
					// day specific value	
					pLink->m_LinkSensorMOEMap[ time].LinkFlow = volume_count/(max(1.0,end_time_in_min-start_time_in_min));  // convert to per hour link flow
					// overall value 
					pLink->m_LinkSensorMOEMap[ t].LinkFlow = volume_count/(max(1.0,end_time_in_min-start_time_in_min));  // convert to per hour link flow

				}
			count ++;

			}
			}
		rsSensorCount.Close();

		str_msg.Format ( "%d sensor count records imported.", count);
		m_MessageList.AddString (str_msg);
		
		}
		}

		m_MessageList.AddString ("Done.");


}



void CDlg_ImportNetwork::OnBnClickedImportNetworkOnly()
{
	OnBnClickedImport();
	m_bImportNetworkOnly = false;  //reset flag
}


void CDlg_ImportNetwork::OnBnClickedImportSensorData()
{
	
}

void CDlg_ImportNetwork::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}

void CDlg_ImportNetwork::OnBnClickedExportData()
{
	CString m_CSV_FileName;
	CFileDialog dlg (FALSE, "*.csv", "*.csv",OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_LONGNAMES,
		"(*.csv)|*.csv||", NULL);
	if(dlg.DoModal() == IDOK)
	{
			FILE* st;
			fopen_s(&st,dlg.GetPathName(),"w");

			if(st!=NULL)
			{
				for(int i=0; i< m_MessageList.GetCount (); i++)	// if one of "all" options is selected, we need to narrow down to OD pair
				{
					char m_Text[200];
					m_MessageList.GetText (i, m_Text);
					fprintf(st,"%s\n",m_Text);
				}
				fclose(st);

			}else
			{   CString str;
				str.Format("The file %s could not be opened.\nPlease check if it is opened by Excel.", dlg.GetPathName());
				AfxMessageBox(str);
			}
	
	m_pDoc->OpenCSVFileInExcel (dlg.GetPathName());

	}
}

void CDlg_ImportNetwork::OnBnClickedButtonViewSampleFile()
{
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CString SampleExcelNetworkFile = pMainFrame->m_CurrentDirectory + m_pDoc->m_SampleExcelNetworkFile;
	m_pDoc->OpenCSVFileInExcel (SampleExcelNetworkFile);

}

void CDlg_ImportNetwork::OnBnClickedButtonLoadSampleFile()
{
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CString SampleExcelNetworkFile = pMainFrame->m_CurrentDirectory + m_pDoc->m_SampleExcelNetworkFile;

	static char BASED_CODE szFilter[] = "Excel File (*.xlsx)|*.xlsx||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);

	dlg.m_ofn.lpstrInitialDir = SampleExcelNetworkFile;

	if(dlg.DoModal() == IDOK)
	{
		UpdateData(true);

		m_Edit_Excel_File = dlg.GetPathName();

		UpdateData(false);
	}

	UpdateData(false);
}

void CDlg_ImportNetwork::OnBnClickedButtonViewSampleCsvFile()
{
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	m_pDoc->m_ProjectFile.Format("%s%s", pMainFrame->m_CurrentDirectory,m_pDoc->m_SampleExcelNetworkFile);

	CString str;
		str.Format("The current project file is saved as %s", MB_ICONINFORMATION);
	AfxMessageBox(str);

}

void CDlg_ImportNetwork::OnBnClickedButtonLoadSampleCsvFile()
{
	// TODO: Add your control notification handler code here
}

void CDlg_ImportNetwork::OnBnClickedButtonViewSampleProjectFolder()
{
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CString SampleProjectFolder = "\\Sample-Portland-SHRP2-C05-subarea";
	SampleProjectFolder = pMainFrame->m_CurrentDirectory + SampleProjectFolder;
	ShellExecute( NULL,  "explore", SampleProjectFolder, NULL,  NULL, SW_SHOWNORMAL );
}

void CDlg_ImportNetwork::OnBnClickedImport2()
{
	OnBnClickedImport();
}



BOOL CDlg_ImportNetwork::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


