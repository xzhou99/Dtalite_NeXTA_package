//Portions Copyright 2012 Xuesong Zhou.
//
//   If you help write or modify the code, please also list your names here.


#include "stdafx.h"
#include "..//Geometry.h"
#include "..//TLite.h"
#include "..//Network.h"
#include "..//TLiteDoc.h"
#include "..//Geometry.h"
#include "..//CSVParser.h"
#include "..//MainFrm.h"
#include <iostream>                          // for cout, endl
#include <fstream>                           // for ofstream
#include <sstream>
#include "..//Dlg_UserInput.h"
#include <iostream>     // std::cout, std::endl
#include <iomanip>      // std::setw
using namespace std;

//#include "DYNASMART-interace.h"

// add description for DYNASMART
// Important References:
// Jayakrishnan, R., Mahmassani, H. S., and Hu, T.-Y., 1994a. An evaluation tool for advanced traffic information and management systems in urban networks. Transportation Research Part C, Vol. 2, No. 3, pp. 129-147.
// 
std::map<int, int> NodeIDtoZoneNameMap;

extern int g_read_number_of_numerical_values(char* line_string, int length, std::vector<float>& values);
float g_read_float_from_a_line(FILE *f, int &end_of_line);

bool CTLiteDoc::ReadGPSData(string FileName)
{
	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	m_ODSize  = 0;
	int good_vehicle_id = 1;

	CCSVParser parser;

	string str0 = FileName;
	CT2CA pszConvertedAnsiString (str0.c_str());
	// construct a std::string using the LPCSTR input
	std::string  strStd (pszConvertedAnsiString);

	if (parser.OpenCSVFile(strStd))
	{
		int i=0;
		while(parser.ReadRecord())
		{

			string Trajectory_ID, Trajectory_Start_Time;
			int Probe_ID,Origin_Area_ID,Destination_Area_ID;
			float Trajectory_Time,Trajectory_Length;

			if(parser.GetValueByFieldName("Trajectory ID",Trajectory_ID) == false)
				break;

			parser.GetValueByFieldName("Probe ID",Probe_ID);
			parser.GetValueByFieldName("Trajectory Time (s)",Trajectory_Time);
			parser.GetValueByFieldName("Trajectory Length (m)",Trajectory_Length);
			parser.GetValueByFieldName("Origin Area ID",Origin_Area_ID);
			parser.GetValueByFieldName("Destination Area ID",Destination_Area_ID);
			parser.GetValueByFieldName("Trajectory Start Time",Trajectory_Start_Time);

			int m_VehicleID= Probe_ID;
			//Trajectory ID,Probe ID,Trajectory Time (s),Trajectory Length (m),Origin Area ID,Destination Area ID,Trajectory Start Time

			//		64555_1,64555,2630.78,32963.56,468,579,Sat May 01 18:01:19 2010
			DTAVehicle* pVehicle = 0;
			pVehicle = new DTAVehicle;
			pVehicle->m_VehicleID		= m_VehicleID;

			int NodeSizeOffset = 0;
			pVehicle->m_bComplete = true;

			pVehicle->m_OriginZoneID	= Origin_Area_ID;
			pVehicle->m_DestinationZoneID = Destination_Area_ID;

			if( pVehicle->m_OriginZoneID > m_ODSize )
				m_ODSize = pVehicle->m_OriginZoneID ;

			if( pVehicle->m_DestinationZoneID > m_ODSize )
				m_ODSize = pVehicle->m_DestinationZoneID ;

			pVehicle->m_InformationClass = 0;

			pVehicle->m_DepartureTime	= m_SimulationStartTime_in_min ;

			if(g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
				g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

			// Total Travel Time=   0.34 # of Nodes=   2 VehType 1 LOO 1
			pVehicle->m_TripTime  = Trajectory_Time;
			pVehicle->m_ArrivalTime = pVehicle->m_DepartureTime + pVehicle->m_TripTime;

			pVehicle->m_NodeSize	= 0;
			pVehicle->m_DemandType = 1;

			pVehicle->m_VOT = 10;
			pVehicle->m_TollDollarCost = 0;
			pVehicle->m_PM = 0;
			pVehicle->m_Distance = Trajectory_Length/5280;

			//pVehicle->m_Distance = 0;

			//pVehicle->m_NodeAry = new SVehicleLink[pVehicle->m_NodeSize];

			//pVehicle->m_NodeNumberSum = 0;

			//m_PathNodeVectorSP[0] =  FirstNodeID;

			//int i;
			//for(i=1; i< pVehicle->m_NodeSize; i++)
			//{
			//	m_PathNodeVectorSP[i] = g_read_integer(pFile);

			//	pVehicle->m_NodeNumberSum += m_PathNodeVectorSP[i];
			//	DTALink* pLink = FindLinkWithNodeNumbers(m_PathNodeVectorSP[i-1],m_PathNodeVectorSP[i],directory+"VehTrajectory.dat");
			//	if(pLink==NULL)
			//	{
			//		CString str;
			//		str.Format ("Error in reading file Vehicle.csv, good vehicle id: %d",good_vehicle_id);
			//		AfxMessageBox(str);
			//		fclose(pFile);

			//		return false;
			//	}
			//	pVehicle->m_NodeAry[i].LinkNo  = pLink->m_LinkNo ;
			//	


			//// ==>Node Exit Time Point
			//for(i=1; i< pVehicle->m_NodeSize + NodeSizeOffset; i++)
			//{
			//	pVehicle->m_NodeAry[i].ArrivalTimeOnDSN = m_SimulationStartTime_in_min + g_read_float(pFile);
			//}


			//// ==>Link Travel Time
			//for(i=1; i< pVehicle->m_NodeSize + NodeSizeOffset; i++)
			//{
			//	g_read_float(pFile);  // // travel time
			//}
			//// ==>Accumulated Stop Time
			//for(i=1; i< pVehicle->m_NodeSize +NodeSizeOffset; i++)
			//{
			//	g_read_float(pFile);  // stop time
			//}

			m_VehicleSet.push_back (pVehicle);
			m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;

			good_vehicle_id = m_VehicleID;
		}

	}

	return true;
}
BOOL CTLiteDoc::OnOpenDYNASMARTProject(CString ProjectFileName, bool bNetworkOnly)
{
	m_OriginOnBottomFlag = -1;
	CTime LoadingStartTime = CTime::GetCurrentTime();

	m_bLoadNetworkDataOnly = bNetworkOnly;

	m_bDYNASMARTDataSet = true;

	m_YCorridonateFlag = 1;

	FILE* pProjectFile = NULL;


	fopen_s(&pProjectFile,ProjectFileName,"r");
	if(pProjectFile!=NULL)
	{
		char  str_line[2000]; // input string
		int str_line_size;
		g_read_a_line(pProjectFile,str_line, str_line_size); //  skip the first line
		g_read_a_line(pProjectFile,str_line, str_line_size); //  skip the first line

		CString str_origin;
		str_origin.Format("%s", str_line,str_line_size);

		if(str_origin.Find("top") != -1 || str_origin.Find("TOP") != -1)
		{
			m_YCorridonateFlag = -1;

		}

		if(str_origin.Find("bottom") != -1 || str_origin.Find("BOTTOM") != -1)
		{
			m_YCorridonateFlag = 1;

		}

		fclose(pProjectFile);
	}


	int bRead_long_lat_coordinate_flag = (int)(g_GetPrivateProfileDouble("coordinate", "read_AMS_files",0,ProjectFileName));	

	CString directory;
	m_ProjectFile = ProjectFileName;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	SetCurrentDirectory(directory);
	m_ProjectDirectory = directory;
	m_ProjectTitle = GetWorkspaceTitleName(ProjectFileName);
	SetTitle(m_ProjectTitle);

	FILE* pFile = NULL;

	FILE* pFileNodeXY = NULL;

	fopen_s(&pFileNodeXY,directory+"xy.dat","r");
	if(pFileNodeXY==NULL)
	{
		AfxMessageBox("File xy.dat cannot be opened.");
		return false;
	}

	int num_zones = 0;

	fopen_s(&pFile,directory+"network.dat","r");

	if(pFile!=NULL)
	{
		num_zones = g_read_integer(pFile);

		m_ODSize = num_zones;
		int num_nodes= g_read_integer(pFile);
		int num_links = g_read_integer(pFile);

		int KSPNumShortestPaths = g_read_integer(pFile);
		int UseSuperzonesFlag = g_read_integer(pFile);

		// Read node block
		int i;
		int node_id = 0, zoneNum = 0;
		DTANode* pNode = 0;
		for(i = 0; i < num_nodes; i++)
		{
			node_id			= g_read_integer(pFile);
			zoneNum	= g_read_integer(pFile);

			m_NodeNotoZoneNameMap[node_id] = zoneNum;
			NodeIDtoZoneNameMap[node_id] = zoneNum;

			// Create and insert the node
			pNode = new DTANode;
			std::stringstream streams;
			streams << node_id;

			pNode->m_Name = streams.str();
			pNode->m_ControlType = 0;

			// read xy.dat	
			int node_number = g_read_integer(pFileNodeXY);  //skip node number in xy.dta
			pNode->pt.x = g_read_float(pFileNodeXY);
			pNode->pt.y = g_read_float(pFileNodeXY)*m_YCorridonateFlag;

			pNode->m_NodeNumber = node_id;
			pNode->m_NodeNo = i;
			pNode->m_ZoneID = 0;
			m_NodeSet.push_back(pNode);
			m_NodeNoMap[i] = pNode;
			m_NodeNotoNumberMap[i] = node_id;
			m_NodeNumberMap[node_id] = pNode;
			m_NodeNumbertoNodeNoMap[node_id] = i;
		}

		m_NodeDataLoadingStatus.Format ("%d nodes are loaded.",m_NodeSet.size());


		// Read link block
		DTALink* pLink = 0;

		float default_distance_sum=0;
		float length_sum = 0;

		for(i = 0; i < num_links; i++)
		{
			int SimulationHorizon = 1;
			pLink = new DTALink(SimulationHorizon);
			pLink->m_LayerNo = 0;
			pLink->m_LinkNo = i;
			//			pLink->m_Name  = name;
			pLink->m_OrgDir = 1;
			pLink->m_LinkID = i;

			pLink->m_FromNodeNumber = g_read_integer(pFile);
			pLink->m_ToNodeNumber = g_read_integer(pFile);
			pLink->m_FromNodeID = m_NodeNumbertoNodeNoMap[pLink->m_FromNodeNumber];
			pLink->m_ToNodeID= m_NodeNumbertoNodeNoMap[pLink->m_ToNodeNumber];

			int m_LeftBays= g_read_integer(pFile);

			pLink->m_NumberOfLeftTurnLanes = m_LeftBays;
			int m_RightBays= g_read_integer(pFile);
			pLink->m_NumberOfRightTurnLanes = m_RightBays;

			pLink->m_Length = g_read_float(pFile)/5280;   // 5280 feet per mile, NEXTA use mile as default length unit
			pLink->m_NumberOfLanes= g_read_integer(pFile);

			int m_FlowModel= g_read_integer(pFile);
			int m_SpeedAdjustment= g_read_integer(pFile);
			pLink->m_SpeedLimit = g_read_integer(pFile)+m_SpeedAdjustment;
			pLink->m_avg_simulated_speed = pLink->m_SpeedLimit;
			pLink->m_FreeFlowTravelTime = pLink->m_Length*60 / max(1,pLink->m_SpeedLimit);  // * 60: hour -> min

			pLink->m_MaximumServiceFlowRatePHPL= g_read_float(pFile);
			pLink->m_LaneCapacity  = pLink->m_MaximumServiceFlowRatePHPL;

			int m_SaturationFlowRate= g_read_integer(pFile);

			pLink->m_Saturation_flow_rate_in_vhc_per_hour_per_lane = m_SaturationFlowRate;

			int DSP_link_type = g_read_integer(pFile);
			pLink->m_link_type = DSP_link_type;


			//if(DSP_link_type == 3 || DSP_link_type == 4) 
			//	pLink->m_LaneCapacity  = 1400;

			//if(DSP_link_type == 5) 
			//	pLink->m_LaneCapacity  = 1000;

			int m_grade= g_read_integer(pFile);

			m_NodeNoMap[pLink->m_FromNodeID ]->m_Connections+=1;
			m_NodeNoMap[pLink->m_FromNodeID ]->m_OutgoingLinkVector.push_back(pLink->m_LinkNo);

			m_NodeNoMap[pLink->m_ToNodeID ]->m_IncomingLinkVector.push_back(pLink->m_LinkNo);

			m_NodeNoMap[pLink->m_ToNodeID ]->m_Connections+=1;

			unsigned long LinkKey = GetLinkKey( pLink->m_FromNodeID, pLink->m_ToNodeID);
			m_NodeNotoLinkMap[LinkKey] = pLink;
			m_LinkNotoLinkMap[i] = pLink;

			m_NodeNoMap[pLink->m_FromNodeID ]->m_TotalCapacity += (pLink->m_MaximumServiceFlowRatePHPL* pLink->m_NumberOfLanes);

			pLink->m_FromPoint = m_NodeNoMap[pLink->m_FromNodeID]->pt;
			pLink->m_ToPoint = m_NodeNoMap[pLink->m_ToNodeID]->pt;
			default_distance_sum+= pLink->DefaultDistance();
			length_sum += (pLink ->m_Length); // convert feet to mile
			//			pLink->SetupMOE();


			std::vector<CCoordinate> CoordinateVector;
			// no geometry information
			CCoordinate cc_from, cc_to; 
			cc_from.X = pLink->m_FromPoint.x;
			cc_from.Y = pLink->m_FromPoint.y;

			cc_to.X = pLink->m_ToPoint.x;
			cc_to.Y = pLink->m_ToPoint.y;

			CoordinateVector.push_back(cc_from);
			CoordinateVector.push_back(cc_to);
			m_bLinkToBeShifted = true;

			int si;
			for(si = 0; si < CoordinateVector.size(); si++)
			{
				GDPoint	pt;
				pt.x = CoordinateVector[si].X;
				pt.y = CoordinateVector[si].Y;
				pLink->m_ShapePoints .push_back (pt);

			}

			pLink->m_Original_ShapePoints = pLink->m_ShapePoints ;


			TRACE("\nAdd link no.%d,  %d -> %d",i,pLink->m_FromNodeNumber, pLink->m_ToNodeNumber );
			m_LinkSet.push_back (pLink);
			m_LinkNoMap[i]  = pLink;
		}

		if(length_sum>0.000001f)
			m_UnitMile= default_distance_sum / length_sum ;

		m_UnitFeet = m_UnitMile/5280.0f;  

		m_LinkDataLoadingStatus.Format ("%d links are loaded.",m_LinkSet.size());
		ConstructMovementVector();



		fclose(pFile);
		fclose(pFileNodeXY);
	}else
	{
		AfxMessageBox("Error: File network.dat cannot be opened.");
		return false;
		//		return;
	}


	FILE *st;
	fopen_s(&st,directory+"workzone.dat","r");
	if(st!=NULL)
	{
		int total_number = g_read_integer(st);
		int i= 0 ;
		while(true)
		{
			int usn  = g_read_integer(st);
			if(usn == -1)
				break;

			int dsn =  g_read_integer(st);

			DTALink* plink = FindLinkWithNodeNumbers(usn,dsn,directory+"workzone.dat" );

			if(plink!=NULL)
			{
				CapacityReduction cs;
				cs.bWorkzone  = true; 

				cs.StartDayNo = 0;
				cs.EndDayNo = 100;
				cs.StartTime = g_read_float(st);
				cs.EndTime = g_read_float(st);
				cs.LaneClosurePercentage= g_read_float(st)*100;  //DYNASMART-P use ratio, DTALite use percentage
				cs.SpeedLimit = g_read_float(st);
				g_read_float(st);

				plink->CapacityReductionVector.push_back(cs);
				i++;
			}
		}
		fclose(st);
	}

	ResetODMOEMatrix();


	// read link xy data
	int ReadLinkXYFile = g_GetPrivateProfileInt("display","read_link_x_y",1,ProjectFileName);

	if(ReadLinkXYFile) 
	{
		FILE* pFileLinkXY = NULL;
		fopen_s(&pFileLinkXY,directory+"linkxy.dat","r");
		if(pFileLinkXY!=NULL)
		{

			m_bBezierCurveFlag = false; // do not apply bezier curve fitting 

			while (!feof(pFileLinkXY))
			{
				int from_node = g_read_integer(pFileLinkXY);

				if(from_node == -1)
					break; // ne
				int to_node = g_read_integer(pFileLinkXY);
				int number_of_shape_points = g_read_integer(pFileLinkXY);

				DTALink* pLink = FindLinkWithNodeNumbers(from_node, to_node,directory+"linkxy.dat");

				if(pLink != NULL)
				{
					pLink->m_ShapePoints.clear();  // clear up first

					for(int i = 0; i < number_of_shape_points; i++)
					{
						GDPoint	pt;
						pt.x =  g_read_float(pFileLinkXY);
						pt.y = g_read_float(pFileLinkXY)*m_YCorridonateFlag;
						pLink->m_ShapePoints .push_back (pt);
					}

					pLink->m_Original_ShapePoints = pLink->m_ShapePoints ;


				}else
				{
					break;
				}
			}
			fclose(pFileLinkXY);
		}  // end of reading link xy 
	}

	// test

	//read zone.dat
	FILE* pZoneXY = NULL;
	fopen_s(&pZoneXY,directory+"zone.dat","r");
	if(pZoneXY!=NULL)
	{
		int number_of_feature_points = g_read_integer(pZoneXY);
		int number_of_zones = g_read_integer(pZoneXY);

		std::map<int, GDPoint> FeaturePointMap;

		for(int f = 0; f< number_of_feature_points; f++)
		{
			int feature_point_id = g_read_integer(pZoneXY);

			FeaturePointMap[feature_point_id].x = g_read_float(pZoneXY);
			FeaturePointMap[feature_point_id].y = g_read_float(pZoneXY)*m_YCorridonateFlag;
		}

		for(int z = 0; z< number_of_zones; z++)
		{
			int zone_id = g_read_integer(pZoneXY);

			if(zone_id==-1)   // end of linke
				break;
			m_ZoneMap [zone_id].m_ZoneID = zone_id;
			m_ZoneMap [zone_id].m_DSPZoneNo = zone_id;

			m_ZoneMap [zone_id].m_ShapePoints .clear();

			int number_of_feature_points = g_read_integer(pZoneXY);
			if(number_of_feature_points==-1)   // end of linke
				break;

			for(int f = 0; f < number_of_feature_points; f++)
			{
				int feature_point_id = g_read_integer(pZoneXY);
				if(feature_point_id==-1)   // end of linke
					break;

				if(FeaturePointMap.find(feature_point_id)!= FeaturePointMap.end())
				{
					GDPoint pt = FeaturePointMap[feature_point_id];
					m_ZoneMap [zone_id].m_ShapePoints.push_back (pt);
				}
			}

		}
		m_ZoneDataLoadingStatus.Format ("%d zone info records are loaded.",m_ZoneMap.size());

		fclose(pZoneXY);
	}





	// read destination.dat

	fopen_s(&pFile,directory+"destination.dat","r");
	if(pFile!=NULL)
	{
		for(int z=0; z< num_zones; z++)
		{
			int zone_number = g_read_integer(pFile);

			int num_destinations= g_read_integer(pFile);

			for(int dest = 0;  dest < num_destinations; dest++)
			{
				int destination_node = g_read_integer(pFile);

				map <int, int> :: iterator m_Iter = m_NodeNumbertoNodeNoMap.find(destination_node);

				if(m_Iter == m_NodeNumbertoNodeNoMap.end( ))
				{
					m_NEXTALOGFile<< "Node Number "  << destination_node << " in destination.dat has not been defined in network.csv"  << endl; 
					fclose(pFile);
					return false;
				}

				int node_id  = m_NodeNumbertoNodeNoMap[destination_node];
				m_NodeNotoZoneNameMap[node_id] = zone_number;
				NodeIDtoZoneNameMap[node_id] = zone_number;
				m_NodeNoMap [node_id ] -> m_ZoneID = zone_number;

				m_NodeNoMap[node_id]->m_ZoneID = zone_number;

				// if there are multiple nodes for a zone, the last node id is recorded.
				if(m_ZoneMap [zone_number].FindANode(node_id,-1) == false)
				{

					DTAActivityLocation element;
					element.ZoneID  = zone_number;

					if(m_NodeNoMap[node_id]->m_IncomingLinkVector .size() > 0 || m_NodeNoMap[node_id]->m_OutgoingLinkVector .size() >0)
					{

						element.NodeNumber = destination_node;



						if(destination_node == 3688)
							TRACE("");

						element.External_OD_flag = 0;

						if(m_NodeNoMap[node_id]->m_IncomingLinkVector .size()  ==0)
							element.External_OD_flag = 1;

						if(m_NodeNoMap[node_id]->m_OutgoingLinkVector .size()  ==0)
							element.External_OD_flag = -1;


						element.pt = m_NodeNoMap [node_id ]  ->pt;


						m_ZoneMap [zone_number].m_ZoneID = zone_number;
						m_ZoneMap [zone_number].m_ActivityLocationVector .push_back (element);
					}
				}
			}


		}

		fclose(pFile);
	}

	// read destination.dat

	fopen_s(&pFile,directory+"origin.dat","r");
	if(pFile!=NULL)
	{
		for(int z=0; z< num_zones; z++)
		{
			int zone_number = g_read_integer(pFile);

			int num_of_origins= g_read_integer(pFile);

			int number= g_read_float(pFile);

			for(int origin = 0;  origin < num_of_origins; origin++)
			{
				int from_node = g_read_integer(pFile);
				int to_node = g_read_integer(pFile);
				float loading_ratio = g_read_float(pFile);

				int node_id  = m_NodeNumbertoNodeNoMap[from_node];
				m_NodeNotoZoneNameMap[from_node] = zone_number;
				m_NodeNumberMap[from_node]->m_ZoneID = zone_number;

				// if there are multiple nodes for a zone, the last node id is recorded.

				DTAGenerationLink element;
				element.FromNodeNumber = from_node;
				element.ToNodeNumber = to_node;

				element.LoadingFactor  = loading_ratio;

				m_ZoneMap [zone_number].m_DSPGenerationLinkVector.push_back (element);

				map <int, int> :: iterator m_Iter = m_NodeNumbertoNodeNoMap.find(from_node);
				if(m_Iter == m_NodeNumbertoNodeNoMap.end( ))
				{
					m_NEXTALOGFile<< "Node Number "  << from_node << " in origin.dat has not been defined in network.dat"  << endl; 
					fclose(pFile);
					return false;
				}

				m_Iter = m_NodeNumbertoNodeNoMap.find(to_node);
				if(m_Iter == m_NodeNumbertoNodeNoMap.end( ))
				{
					m_NEXTALOGFile<< "Node Number "  << to_node << " in origin.dat has not been defined in network.dat"  << endl; 
					fclose(pFile);
					return false;
				}

				// we only record the from node on a generation link as the activity node
				if(m_ZoneMap [zone_number].FindANode(from_node,1) == false 
					&& m_NodeNumberMap[from_node]->m_ControlType != m_ControlType_PretimedSignal 
					&& m_NodeNumberMap[from_node]->m_ControlType != m_ControlType_ActuatedSignal)
				{
					DTAActivityLocation element;
					element.ZoneID  = zone_number;
					element.NodeNumber = from_node;
					element.External_OD_flag = 0;
					m_ZoneMap [zone_number].m_ActivityLocationVector .push_back (element);
				}

				if(m_ZoneMap [zone_number].FindANode(to_node,1) == false
					&& m_NodeNumberMap[to_node]->m_ControlType != m_ControlType_PretimedSignal 
					&& m_NodeNumberMap[to_node]->m_ControlType != m_ControlType_ActuatedSignal)
				{
					DTAActivityLocation element;
					element.ZoneID  = zone_number;
					element.NodeNumber = to_node;
					element.External_OD_flag = 0;
					m_ZoneMap [zone_number].m_ActivityLocationVector .push_back (element);
				}



			}

		}

		fclose(pFile);
	}

	//	ReadZoneCSVFile(directory+"input_zone.csv");

	DTADemandType demand_type_element;
	demand_type_element.demand_type =1;
	demand_type_element.demand_type_name = "SOV";
	demand_type_element.average_VOT = 10;
	m_DemandTypeVector.push_back(demand_type_element);

	demand_type_element.demand_type =2;
	demand_type_element.demand_type_name = "HOV";
	demand_type_element.average_VOT = 10;
	m_DemandTypeVector.push_back(demand_type_element);

	demand_type_element.demand_type =3;
	demand_type_element.demand_type_name = "truck";
	m_DemandTypeVector.push_back(demand_type_element);



	int ReadDemandFile = 1;

	fopen_s(&pFile,directory+"demand.dat","rb");
	if(pFile!=NULL)
	{
		fseek(pFile, 0, SEEK_END );
		int Length = ftell(pFile);
		fclose(pFile);
		float LengthinMB= Length*1.0/1024/1024;
		if(LengthinMB>20)
		{
			CString msg;
			msg.Format("The demand.dat file is %5.1f MB in size.\nIt could take quite a while to load this file.\nWould you like to load this file?",LengthinMB);
			if(AfxMessageBox(msg,MB_YESNO|MB_ICONINFORMATION)==IDNO)
			{
				ReadDemandFile = 0;
			}
		}

		fclose(pFile);
	}

		// read demand.dat
		fopen_s(&pFile,directory+"demand.dat","r");
		if(pFile!=NULL)
		{
			// Number of matrices and the multiplication factor
			int num_matrices = 0;

			num_matrices = g_read_integer(pFile);
			float demand_factor = g_read_float(pFile);

			std::vector<int> TimeIntevalVector;
			// Start times
			int i;
			for(i = 0; i < num_matrices; i++)
			{
				int start_time = g_read_float(pFile);
				TimeIntevalVector.push_back(start_time);

			}

			int time_interval = 60; // min

			if(TimeIntevalVector.size() >=2)
				time_interval = TimeIntevalVector[1] - TimeIntevalVector[0];

			// read the last value
			int end_of_simulation_horizon = g_read_float(pFile);

			// update demand loading period
			m_DemandLoadingStartTimeInMin = 0;
			m_DemandLoadingEndTimeInMin = end_of_simulation_horizon;

			TimeIntevalVector.push_back(end_of_simulation_horizon);
			m_PeakHourFactor = 60.0/max(1,end_of_simulation_horizon);

		if(ReadDemandFile)
		{

			long RecordCount = 0;
			float total_demand = 0;
			for(i = 0; i < num_matrices; i++)
			{
				// Find a line with non-blank values to start
				// Origins
				double start_time= g_read_float(pFile); // start time

				int demand_table_type = GetDemandTableType(1,start_time,start_time+time_interval);
				for(int from_zone=1; from_zone<= num_zones; from_zone++)
					for(int to_zone=1; to_zone<= num_zones; to_zone++)
					{
						float demand_value = g_read_float(pFile) * demand_factor;
						total_demand += demand_value;

						DTADemandVolume element;

						// obtain demand table type

						m_ZoneMap[from_zone].m_ODDemandMatrix [to_zone].SetValue (demand_table_type,demand_value);


						RecordCount++;
					}

			} // time-dependent matrix

			m_AMSLogFile << RecordCount << "demand entries are loaded from file demand.dat "  ". Total demand =  " 
				<< total_demand << endl;
			m_DemandDataLoadingStatus.Format ("%d demand entries are loaded from file demand.dat. Total demand = %f",RecordCount,total_demand);

		}

		if(pFile!=NULL)
			fclose(pFile);
	}
	// set link type


	m_LinkTypeMap[1].type_code  = 'f';
	m_LinkTypeMap[2].type_code  = 'f';

	/// to do: 
	m_NodeTypeMap[0] = "";
	m_NodeTypeMap[1] = "No Control";
	m_NodeTypeMap[2] = "Yield Sign";
	m_NodeTypeMap[3] = "4-Way Stop Sign";
	m_NodeTypeMap[4] = "Pretimed Signal";
	m_NodeTypeMap[5] = "Actuated Signal";
	m_NodeTypeMap[6] = "2-Way Stop Sign";


	m_ControlType_UnknownControl = 0; 
	m_ControlType_NoControl = 1;
	m_ControlType_YieldSign = 2;
	m_ControlType_2wayStopSign =  6;
	m_ControlType_4wayStopSign = 3;
	m_ControlType_PretimedSignal = 4;
	m_ControlType_ActuatedSignal = 5;
	m_ControlType_Roundabout = 7;

	m_LinkTypeFreeway = 1;
	m_LinkTypeArterial = 3;
	m_LinkTypeHighway = 2;

	DTALinkType element;

	element.link_type = 1;
	element.type_code = 'f';
	element.link_type_name = "Freeway";
	m_LinkTypeMap[1] = element;

	element.link_type = 2;
	element.type_code = 'f';
	element.link_type_name = "Freeway with Detector";
	m_LinkTypeMap[2] = element;

	element.link_type = 3;
	element.type_code = 'r';
	element.link_type_name = "On Ramp";
	m_LinkTypeMap[3] = element;

	element.link_type = 4;
	element.type_code = 'f';
	element.link_type_name = "Off Ramp";
	m_LinkTypeMap[4] = element;

	element.link_type = 5;
	element.type_code = 'a';
	element.link_type_name = "Arterial";
	m_LinkTypeMap[5] = element;

	element.link_type = 6;
	element.type_code = 'f';
	element.link_type_name = "HOT";
	m_LinkTypeMap[6] = element;


	element.link_type = 7;
	element.type_code = 'h';
	element.link_type_name = "Highway";
	m_LinkTypeMap[7] = element;

	element.link_type = 8;
	element.type_code = 'f';
	element.link_type_name = "HOV";
	m_LinkTypeMap[8] = element;

	element.link_type = 9;
	element.type_code = 'f';
	element.link_type_name = "FreewayHOT";
	m_LinkTypeMap[9] = element;

	element.link_type = 10;
	element.type_code = 'f';
	element.link_type_name = "FreewayHOV";
	m_LinkTypeMap[10] = element;

	m_OffsetInFeet=15;
	OffsetLink();
	GenerateOffsetLinkBand();

	CalculateDrawingRectangle();
	BuildGridSystem();

	m_bFitNetworkInitialized  = false;

	//read control.dat

	// read control.dat

	ReadDYNASMART_ControlFile();


	CTime LoadingEndTime = CTime::GetCurrentTime();

	CTimeSpan ts = LoadingEndTime  - LoadingStartTime;
	CString str_running_time;

	str_running_time.Format ("Network loading time: %d min(s) %d sec(s)...",ts.GetMinutes(), ts.GetSeconds());

	SetStatusText(str_running_time);

	//	m_SimulationLinkMOEDataLoadingStatus.Format("Load %d GPS probe records.", m_VehicleIDMap.size());


	// read system.dat
	fopen_s(&pFile,directory+"system.dat","r");
	if(pFile!=NULL)
	{
		g_Simulation_Time_Horizon = g_read_integer(pFile);
		/*
		itsMaxIterations >> temp;
		itsVehicleGenMode = (veh_gen)temp;
		is >> itsIntervalsPerAggregation >> itsIntervalsPerAssignment
		>> itsMUCThreshold >> itsConvergeThreshold; */

		fclose(pFile);
	}


	if(m_bLoadNetworkDataOnly)
		return true;


	ReadDYNASMARTSimulationResults();
	return true;
}

BOOL CTLiteDoc::ReadDYNASMARTSimulationResults()
{

	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	FILE* pFile = NULL;

	// read speed data
	fopen_s(&pFile,directory+"fort.900","r");
	std::list<DTALink*>::iterator iLink;

	CString str;
	str.Format ("Start loading speed data...");
	SetStatusText(str);

	if(pFile!=NULL)
	{
		m_TrafficFlowModelFlag = 2; // enable dynamic display mode after reading speed data
		// read data every min
		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->ResetMOEAry(m_SimulationStartTime_in_min + g_Simulation_Time_Horizon);  // use one day horizon as the default value
		}

		float value = 0;
		for(int t = m_SimulationStartTime_in_min; t < m_SimulationStartTime_in_min+g_Simulation_Time_Horizon; t++)
		{
			int timestamp = (int)(g_read_float(pFile));  // read timestamp in min

			if(timestamp%10==0)
			{
				str.Format ("Start loading speed data at min %d", timestamp );
				SetStatusText(str);

			}


			if(timestamp < 0)  // end of file
				break;

			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				value = g_read_float(pFile);  // speed;

				if( (*iLink)->m_FromNodeNumber == 115 && (*iLink)->m_ToNodeNumber == 40) 
				{
				
					TRACE("");
				
				}
				if(t < (*iLink)->m_LinkMOEArySize -1 )
				{
					(*iLink)->m_LinkMOEAry[t+1].Speed = value;
					(*iLink)->m_LinkMOEAry[t+1].TravelTime = (*iLink)->m_Length * 60/ max(1,value);
				}
			}

		}
		fclose(pFile);
	}

	str.Format ("Start loading queue length data...");
	SetStatusText(str);

	// read queue length data
	fopen_s(&pFile,directory+"fort.600","r");
	if(pFile!=NULL)
	{
		// read data every min

		for(int t = m_SimulationStartTime_in_min; t < m_SimulationStartTime_in_min+g_Simulation_Time_Horizon; t++)
		{
			int timestamp = g_read_float(pFile);  // read timestamp in min

			if(timestamp < 0)  // end of file
				break;

			if(timestamp%10==0)
			{
				str.Format ("Start loading queue length data at min %d", timestamp );
				SetStatusText(str);

			}

			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				float value  = g_read_float(pFile);  // queue length;

				if(value < -0.5f)
					break;
				else if(t < (*iLink)->m_LinkMOEArySize -1 )
				{
					(*iLink)->m_LinkMOEAry[t+1].QueueLength = value;
				}
			}

		}
		fclose(pFile);
	}

	str.Format ("Start loading density data...");
	SetStatusText(str);

	// read density data
	fopen_s(&pFile,directory+"fort.700","r");
	if(pFile!=NULL)
	{
		// read data every min

		for(int t = m_SimulationStartTime_in_min; t < m_SimulationStartTime_in_min+g_Simulation_Time_Horizon; t++)
		{
			int timestamp = g_read_float(pFile);  // read timestamp in min

			if(timestamp < 0)  // end of file
				break;

			if(timestamp%10==0)
			{
				str.Format ("Start loading density data at min %d", timestamp );
				SetStatusText(str);

			}

			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				float value  = g_read_float(pFile);  // density

				if(value < -0.5f)
					break;
				else if(t < (*iLink)->m_LinkMOEArySize -1 )
				{
					(*iLink)->m_LinkMOEAry[t+1].Density  = value;
				}
			}

		}
		fclose(pFile);
	}

	str.Format ("Start loading volume data...");
	SetStatusText(str);


	// comment it out, use data from vehicle file 

	//bool CumulativeFlag = true;
	//// read flow rate
	//fopen_s(&pFile,directory+"OutAccuVol.dat","r");

	//if(pFile==NULL)
	//{
	//	fopen_s(&pFile,directory+"LinkVolume.dat","r");
	//	CumulativeFlag = false;
	//}


	//if(pFile!=NULL)
	//{
	//	g_read_float(pFile);  // read 10 min 
	//	// This file provides the accummulated number of veh. on of each link           10every sims ints

	//	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	//	{

	//		(*iLink)->m_total_link_volume = 0;
	//	}

	//	for(int t = m_SimulationStartTime_in_min; t < m_SimulationStartTime_in_min+g_Simulation_Time_Horizon; t++)
	//	{
	//		int timestamp = g_read_float(pFile);  // read timestamp in min

	//		//              TRACE("timestamp = %f\n",timestamp);

	//		if(timestamp < 0)  // end of file
	//			break;

	//		if(timestamp%10==0)
	//		{
	//			str.Format ("Start loading volume data at min %d", timestamp );
	//			SetStatusText(str);

	//		}


	//		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	//		{
	//			if(CumulativeFlag && t < (*iLink)->m_LinkMOEArySize -1 )

	//			{
	//				float value = g_read_float(pFile);  // cumulative flow;

	//				if(t < (*iLink)->m_LinkMOEArySize -1 )
	//					(*iLink)->m_LinkMOEAry[t].ArrivalCumulativeFlow = value;

	//				if(t>=m_SimulationStartTime_in_min+1)
	//				{
	//					float value =  max(0,((*iLink)->m_LinkMOEAry[t].ArrivalCumulativeFlow - (*iLink)->m_LinkMOEAry[t-1].ArrivalCumulativeFlow));

	//					(*iLink)->m_LinkMOEAry[t].LinkFlow = value *60;  //convert to hourly count
	//					if((*iLink)->m_FromNodeNumber == 115 && (*iLink)->m_ToNodeNumber == 40)
	//					{
	//						TRACE("%d,%f\n",t,value);

	//					}

	//				}
	//			}else 
	//			{
	//				float value  =  g_read_float(pFile);
	//				if( t < (*iLink)->m_LinkMOEArySize -1)
	//				{
	//					(*iLink)->m_LinkMOEAry[t].LinkFlow = value;
	//				}

	//			}
	//		}

	//	}
	//	fclose(pFile);
	//}


	// density_in_veh_per_mile_per_lane

	/*
	fopen_s(&st,fname,"r");
	if(st!=NULL)
	{
	int i;
	int id = 0, zoneNum = 0;
	DTANode* pNode = 0;
	for(i = 0; i < g_ODZoneSize; i++)
	{

	zoneNum = g_read_integer(pFile);
	int num_nodes= g_read_integer(pFile);

	if(num_nodes > g_AdjLinkSize)
	g_AdjLinkSize = num_nodes + 10;  // increaes buffer size


	for(int n = 0; n< num_nodes; n++)
	{
	int node_number= g_read_integer(pFile);


	g_NodeMap[node_number]->m_ZoneID = zoneNum;
	}
	}
	fclose(pFile);
	}
	}
	*/
	// read vehicle trajectory file

	// read additional data from sensor (AMS format)

	m_SimulationStartTime_in_min = (int)(g_GetPrivateProfileDouble("simulation_result", "simulation_start_time_in_min",0,m_ProjectFile));   
	//      ReadSensorCountData(directory+"sensor_count.csv",simulation_start_time_in_min);


	str.Format ("Start loading vehicle trajectory data...");
	SetStatusText(str);

	CString TrajectoryFileDayByDay;
	int start_day_no = (int)(g_GetPrivateProfileDouble("simulation_result", "start_day_no", 10, m_ProjectFile));
	int end_day_no = (int)(g_GetPrivateProfileDouble("simulation_result", "end_day_no", 20, m_ProjectFile));
	ReadBackgroundImageFile(m_ProjectFile);

	for (int day_no = start_day_no; day_no <= end_day_no; day_no++)
	{

		TrajectoryFileDayByDay.Format("vehtrajectory.dat%d", day_no);

	fopen_s(&pFile, directory + TrajectoryFileDayByDay, "r");

	/*float LengthinMB;
	bool bLoadVehicleData = true;
	fopen_s(&pFile, directory + TrajectoryFileDayByDay, "rb");
	if(pFile!=NULL)
	{
		fseek(pFile, 0, SEEK_END );
		long Length = ftell(pFile);
		fclose(pFile);
		LengthinMB= Length*1.0/1024/1024;
		if(LengthinMB>20)
		{
			CString msg;
			msg.Format("The vehtrajectory.dat file is %5.1f MB in size.\nIt could take quite a while to load this file.\nWould you like to load the vehicle trajector files?",LengthinMB);
			if(AfxMessageBox(msg,MB_YESNO|MB_ICONINFORMATION)==IDNO)
				bLoadVehicleData = false;
		}
	}*/


	//LoadGPSData();



		char  str_line[2000]; // input string
		DTALink* m_pPathLinkVector[MAX_NODE_SIZE_IN_A_PATH];

		fopen_s(&pFile, directory + TrajectoryFileDayByDay, "r");

		if(pFile != NULL)
		{
			int str_line_size;
			g_read_a_line(pFile,str_line, str_line_size); //  skip the two line
			g_read_a_line(pFile,str_line, str_line_size); //  skip the two line

			bool bformat_b_flag = false;
			CString Cstr_line;
			Cstr_line.Format("%s", str_line);
			if(Cstr_line.Find (" B")!=-1)
			{
				bformat_b_flag = true;  
			}



			int good_vehicle_id = 1;



			while(!feof(pFile) )
			{

				int m_VehicleID= g_read_integer(pFile, false);
				if(m_VehicleID == -1)
					break;
				// Veh #    144 Tag= 3 OrigZ=  10 DestZ=  11 Class= 3 UstmN=    150 DownN=     14 DestN=     13 STime=   0.50 

				DTAVehicle* pVehicle = 0;
				pVehicle = new DTAVehicle;
				pVehicle->m_VehicleID           = m_VehicleID;

				int CompleteFlag = g_read_integer(pFile);
				int NodeSizeOffset = 0;
				if(CompleteFlag==0 || CompleteFlag==1) 
				{
					pVehicle->m_bComplete = false;
					NodeSizeOffset = -1;
				}
				else 
				{
					pVehicle->m_bComplete = true;
					NodeSizeOffset = 0; 
				}

				pVehicle->m_OriginZoneID        = g_read_integer(pFile);
				pVehicle->m_DestinationZoneID = g_read_integer(pFile);

				pVehicle->m_InformationClass = (unsigned char)g_read_integer(pFile);

				int FirstNodeID = g_read_integer(pFile); 
				int SecondNodeID = g_read_integer(pFile);    // (4) first node
				int DestinationNodeID = g_read_integer(pFile);    // last node

				//pVehicle->m_OriginZoneID = NodeIDtoZoneNameMap[FirstNodeID];
				//pVehicle->m_DestinationZoneID = NodeIDtoZoneNameMap[DestinationNodeID];

				if( pVehicle->m_OriginZoneID > m_ODSize )
					m_ODSize = pVehicle->m_OriginZoneID ;

				if( pVehicle->m_DestinationZoneID > m_ODSize )
					m_ODSize = pVehicle->m_DestinationZoneID ;


				pVehicle->m_DepartureTime       = g_read_float(pFile);

				if(g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
					g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

				// Total Travel Time=   0.34 # of Nodes=   2 VehType 1 LOO 1
				pVehicle->m_TripTime  = g_read_float(pFile);
				pVehicle->m_ArrivalTime = pVehicle->m_DepartureTime + pVehicle->m_TripTime;

				pVehicle->m_NodeSize    = g_read_integer(pFile) +1;  // +1 as we need to allocate one more node for the first node
				pVehicle->m_DemandType = g_read_integer(pFile);


				if(bformat_b_flag==false)
				{
					g_read_integer(pFile);
				}

				pVehicle->m_TollDollarCost = 0;
				pVehicle->m_PM = 0;

				pVehicle->m_Distance = 0;

				pVehicle->m_VOT = 10;
				if(bformat_b_flag)
				{
					g_read_float(pFile);
					pVehicle->m_VOT =  g_read_float(pFile);
					g_read_float(pFile);
					g_read_float(pFile);
					g_read_float(pFile);
					g_read_float(pFile);
				}

				//                      pVehicle->m_VehicleType = (unsigned char)g_read_integer(pFile);


			TRACE("\nreading veh = %d",pVehicle->m_VehicleID );

				pVehicle->m_NodeAry = new SVehicleLink[pVehicle->m_NodeSize];

				pVehicle->m_NodeNumberSum = 0;

				m_PathNodeVectorSP[0] =  FirstNodeID;

				int i;
				for(i=1; i< pVehicle->m_NodeSize; i++)
				{
					m_PathNodeVectorSP[i] = g_read_integer(pFile);

					//// update origin destination data based on TAZ
					//              if(i==0 && m_NodeNoMap.find(m_PathNodeVectorSP[i]) != m_NodeNoMap.end())
					//              {
					//                      pVehicle->m_OriginZoneID = m_NodeNoMap[m_PathNodeVectorSP[i] ]-> m_ZoneID;
					//              
					//              }

					//              if(i== pVehicle->m_NodeSize -1 && m_NodeNoMap.find(m_PathNodeVectorSP[i]) != m_NodeNoMap.end())
					//              {
					//                      pVehicle->m_DestinationZoneID = m_NodeNoMap[m_PathNodeVectorSP[i] ]-> m_ZoneID;
					//              
					//              }

					pVehicle->m_NodeNumberSum += m_PathNodeVectorSP[i];
					DTALink* pLink = FindLinkWithNodeNumbers(m_PathNodeVectorSP[i-1],m_PathNodeVectorSP[i],directory+"VehTrajectory.dat");
					if(pLink==NULL)
					{
						CString str;
						str.Format ("Error in reading file Vehicle.csv, good vehicle id: %d",good_vehicle_id);
						AfxMessageBox(str);
						fclose(pFile);

						return false;
					}
					pVehicle->m_NodeAry[i].LinkNo  = pLink->m_LinkNo ;
					m_pPathLinkVector[i] = pLink;
					pVehicle->m_Distance +=pLink->m_Length ;
				}


				// ==>Node Exit Time Point
				for(i=1; i< pVehicle->m_NodeSize + NodeSizeOffset; i++)
				{
					pVehicle->m_NodeAry[i].ArrivalTimeOnDSN = pVehicle->m_DepartureTime + g_read_float(pFile);

					if(pVehicle->m_NodeAry[i].ArrivalTimeOnDSN < 10000) // feasible arrival time
					{
						DTALink* pLink = FindLinkWithNodeNumbers(m_PathNodeVectorSP[i-1],m_PathNodeVectorSP[i],directory+"VehTrajectory.dat");

						pLink->m_total_link_volume +=1;

						double travel_time = 0 ;
						if(i>=2)
							travel_time  = pVehicle->m_NodeAry[i].ArrivalTimeOnDSN - pVehicle->m_NodeAry[i-1].ArrivalTimeOnDSN;
						else // first link
							travel_time  = pVehicle->m_NodeAry[i].ArrivalTimeOnDSN - pVehicle->m_DepartureTime ;
						pLink->m_TotalTravelTime += travel_time;
						pLink->m_total_delay += ( travel_time- pLink->m_FreeFlowTravelTime );
						float delay = max(0,travel_time- pLink->m_FreeFlowTravelTime);
						pLink->AddNodeDelay(pVehicle->m_NodeAry[i].ArrivalTimeOnDSN ,delay);
					}

				}


				// ==>Link Travel Time
				for(i=1; i< pVehicle->m_NodeSize + NodeSizeOffset; i++)
				{
					g_read_float(pFile);  // // travel time
				}
				// ==>Accumulated Stop Time
				for(i=1; i< pVehicle->m_NodeSize +NodeSizeOffset; i++)
				{
					g_read_float(pFile);  // stop time
				}

				m_VehicleSet.push_back (pVehicle);
				m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;

				if(m_VehicleSet.size()%10000==0)
				{
					CString str_veh;
					str_veh.Format ("loading %d vehicles from day %d",m_VehicleSet.size(), day_no);
					SetStatusText(str_veh);
				}

				good_vehicle_id = m_VehicleID;
			}

			fclose(pFile);
			
		}	// file open
	}  // for each day

	m_SimulationVehicleDataLoadingStatus.Format("%d vehicles are loaded from VehTrajectory.dat .", m_VehicleSet.size());

		RecalculateLinkMOEFromVehicleTrajectoryFile();


	if(m_VehicleSet.size() > 500000)
	{
	CString msg;
	msg.Format("There are %d vehicles.\nIt could take quite a while to update movement data (for bottleneck visualization).\nWould you like to update movement data?",m_VehicleSet.size());
		if(AfxMessageBox(msg,MB_YESNO|MB_ICONINFORMATION)==IDYES)
		{
		UpdateMovementDataFromVehicleTrajector();
		}

	}else
	{
	UpdateMovementDataFromVehicleTrajector();
	}
	
	return true;
}
BOOL CTLiteDoc::SaveDYNASMARTProject(CString ProjectFileName, bool bNetworkOnly)
{
	CWaitCursor wait;
	CString OriginDirectory = m_ProjectDirectory;

	char format_string[100];

	CString directory;
	directory = ProjectFileName.Left(ProjectFileName.ReverseFind('\\') + 1);

	
	// xy.dat
	FILE* st;
	char fname[_MAX_PATH];
	st = fopen(ProjectFileName,"r");
	if(st!=NULL)  //no such file
	{
		fclose(st);

	}else
	{
		st = fopen(ProjectFileName,"w");
		if(st!=NULL)  
		{
			fprintf(st,"  ");
			fclose(st);
		}

	}




	std::ofstream network_dat_file(directory + "network.dat");

	int DSP_KSPNumShortestPaths = 5;
	int DSP_UseSuperzonesFlag = 0;
	network_dat_file << std::setw(7) << m_ZoneMap.size();
	network_dat_file << std::setw(7) << m_NodeSet.size();
	network_dat_file << std::setw(7) << m_LinkSet.size();
	network_dat_file << std::setw(7) << DSP_KSPNumShortestPaths;
	network_dat_file << std::setw(7) << DSP_UseSuperzonesFlag;
	network_dat_file << std::endl;

	sprintf(fname,"%s",(directory + "xy.dat"));
	st = fopen(fname,"w");
	if(st!=NULL)
	{

		int Y_axis_Flag = 1;
		std::list<DTANode*>::iterator iNode;
		for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{

			fprintf(st, "%7d %15.7f %15.7f\n", (*iNode)->m_NodeNumber , 
				(*iNode)->pt .x , 
				(*iNode)->pt .y);

			// network.dat
			network_dat_file << std::setw(7) << (*iNode)->m_NodeNumber;
			network_dat_file << std::setw(5) <<  (*iNode)->m_ZoneID ;
			network_dat_file << std::endl;

		}

		fclose(st);
	}


	// Link information
	std::ofstream linkname_dat_file((directory + "linkname.dat"));
	std::ofstream movement_dat_file((directory + "movement.dat"));

	// sprintf(fname,"%s",(directory + "linkxy.dat").c_str());
	// st = fopen(fname,"w");
	// if(st!=NULL)
	// {

	//    for(linkIt = itsLinks.begin(); linkIt != itsLinks.end(); linkIt++)
	//    {
	//// linkxy.dat

	//// Add
	//double Threshold = 0.1;
	//if((*linkIt)->fp_size())
	//{
	//   int AdditionalFeaturePoints=0;
	//   bool bUpstreamFlag=false;
	//   bool bDownstreamFlag=false;

	//   fpUpIt = (*linkIt)->fp_begin();

	//   //		DSPPoint pt  = (*linkIt)->UpstreamNode()->Location();
	//   float length  = ((*fpUpIt) - (*linkIt)->UpstreamNode()->Location()).Length();

	//   if(((*fpUpIt) - (*linkIt)->UpstreamNode()->Location()).Length() > Threshold)
	//   {
	//      bUpstreamFlag = true;
	//      AdditionalFeaturePoints++;
	//   }

	//   fpDownIt = (*linkIt)->fp_end();
	//   if(((*fpDownIt) - (*linkIt)->DownstreamNode()->Location()).Length() > Threshold)
	//   {
	//      bDownstreamFlag = true;
	//      AdditionalFeaturePoints++;
	//   }


	//   fprintf(st, "%7d %7d %3d", (*linkIt)->ID().first->ID(), (*linkIt)->ID().second->ID(), (*linkIt)->fp_size()+AdditionalFeaturePoints);

	//   if(bUpstreamFlag)
	//      fprintf(st, "%15.3f, %15.3f;",(*linkIt)->UpstreamNode()->Location().x, (*linkIt)->UpstreamNode()->Location().y*Y_axis_Flag);

	//   for(fpIt = (*linkIt)->fp_begin(); fpIt != (*linkIt)->fp_end(); fpIt++)
	//      fprintf(st, "%15.3f, %15.3f;", (*fpIt).x, (*fpIt).y*Y_axis_Flag);

	//   if(bDownstreamFlag)
	//      fprintf(st, "%15.3f, %15.3f;",(*linkIt)->DownstreamNode()->Location().x, (*linkIt)->DownstreamNode()->Location().y*Y_axis_Flag);

	//   fprintf(st, "\n");

	//}


	// network.dat

	std::list<DTALink*>::iterator iLink ;

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		if((*iLink)->m_FromNodeID != (*iLink)->m_ToNodeID)
		{
			int ToNodeID = (*iLink)->m_ToNodeID ;
			DTANode* pNode = m_NodeNoMap[ToNodeID];

			CString link_type_name = " ";

			if(m_LinkTypeMap.find((*iLink)->m_link_type) != m_LinkTypeMap.end())
			{
				link_type_name = m_LinkTypeMap[(*iLink)->m_link_type].link_type_name.c_str ();
			}

			std::replace( (*iLink)->m_Name.begin(), (*iLink)->m_Name.end(), ',', ' '); 

			network_dat_file << std::setw(7) << (*iLink)->m_FromNodeNumber;
			network_dat_file << std::setw(7) << (*iLink)->m_ToNodeNumber;
			network_dat_file << std::setw(5) << (*iLink)->m_NumberOfLeftTurnLanes;
			network_dat_file << std::setw(5) << (*iLink)->m_NumberOfRightTurnLanes;

			int length_in_feet  = (int)( (*iLink)->m_Length*5280 + 0.5);
			network_dat_file << std::setw(7) << length_in_feet;
			network_dat_file << std::setw(3) << (*iLink)->m_NumberOfLanes;


			int flow_model = 1;
			if( m_LinkTypeMap[(*iLink)->m_link_type].IsArterial  ())
			{
				flow_model = 2;
			}

			network_dat_file << std::setw(7) << flow_model;
			 network_dat_file.setf(std::ios::showpos);

			
			int free_flow_adjustment = 0;
	 network_dat_file << std::setw(4) << free_flow_adjustment;
	 network_dat_file.unsetf(std::ios::showpos);

			int speed_limit = (*iLink)->m_SpeedLimit;
			network_dat_file << std::setw(4) << speed_limit;

			int service_flow_rate = (*iLink)->m_LaneCapacity;
			int saturation_flow = (*iLink)->m_LaneCapacity;

			network_dat_file << std::setw(6) << service_flow_rate;
			network_dat_file << std::setw(6) << saturation_flow;

			network_dat_file.setf(std::ios::showpos);
			int dsp_link_type = 5;
			
			if( m_LinkTypeMap[(*iLink)->m_link_type].IsFreeway ())
				dsp_link_type = 1;
			if( m_LinkTypeMap[(*iLink)->m_link_type].IsHighway ())
				dsp_link_type = 7;
			if( m_LinkTypeMap[(*iLink)->m_link_type].IsArterial ())
				dsp_link_type = 5;
			if( m_LinkTypeMap[(*iLink)->m_link_type].IsRamp ())
				dsp_link_type = 3;


			network_dat_file << std::setw(4) << dsp_link_type ;
	 

			network_dat_file << std::setw(4) << (*iLink)->m_Grade;
			network_dat_file.setf(std::ios::showpos);
			network_dat_file.unsetf(std::ios::showpos);
			network_dat_file << std::endl;

			// movement.dat
			movement_dat_file << std::setw(7) << (*iLink)->m_FromNodeNumber;
			movement_dat_file << std::setw(7) << (*iLink)->m_ToNodeNumber;

			movement_dat_file << std::setw(7) <<  (*iLink)->m_DSPLeftNodeNumber;
			movement_dat_file << std::setw(7) <<  (*iLink)->m_DSPThroughNodeNumber;
			movement_dat_file << std::setw(7) <<  (*iLink)->m_DSPRightNodeNumber;
			movement_dat_file << std::setw(7) <<  (*iLink)->m_DSPOther1NodeNumber;
			movement_dat_file << std::setw(7) <<  (*iLink)->m_DSPOther2NodeNumber;

			// linkname.dat
			if(!(*iLink)->m_Name.empty())
			{
			   linkname_dat_file << std::setw(7) << (*iLink)->m_FromNodeNumber;
			   linkname_dat_file << std::setw(7) << (*iLink)->m_ToNodeNumber;
			   linkname_dat_file << ' ' << (*iLink)->m_Name .c_str ();
			   linkname_dat_file << std::endl;
			}


			bool bUturnAllowed  = true;
			if( (*iLink)->m_prohibited_u_turn && (*iLink)->m_bOneWayLink)
				bUturnAllowed = false;


			movement_dat_file << std::setw(7) << bUturnAllowed;
			movement_dat_file << std::endl;
		}
		fclose(st);
	}
	network_dat_file.close();
	// network.dat is done
	movement_dat_file.close();


	int number_of_features_points = 0;
	int DSPZoneNo= 1;
	std::map<int, DTAZone>	:: iterator itr;

	for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
	{
		number_of_features_points += itr->second.m_ShapePoints.size();

		if(itr->second.m_DSPZoneNo==0)
		{
			itr->second .m_DSPZoneNo = DSPZoneNo++;
		}

	}


	sprintf(fname,"%s\\zone.dat\0",directory);


	st=fopen(fname,"w");
	if(st!=NULL)
	{
		fprintf(st, " This file defines zone regions\n");
		fprintf(st, " number of feature points, number of zones\n");
		fprintf(st, " %7d %7d\n", number_of_features_points, m_ZoneMap.size());

		number_of_features_points = 1;

		// Feature Point information
		fprintf(st, "node #, x-coordinate, y-coordinate\n");
		std::map<int, DTAZone>	:: const_iterator itr;

		for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
		{
			for(unsigned int si = 0; si< itr->second.m_ShapePoints.size(); si++)
			{
				fprintf(st,"%7d %15.6f, %15.6f\n",number_of_features_points++,itr->second.m_ShapePoints[si].x, itr->second.m_ShapePoints[si].y);
			}

		}

		// Zone information
		fprintf(st, "zone #, number of nodes, node #'s\n");

		number_of_features_points =1;
		for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
		{
			// zone.dat
			fprintf(st,"%7d %7d: ", itr->second .m_DSPZoneNo, itr->second.m_ShapePoints.size()) ;
			for(unsigned int si = 0; si< itr->second.m_ShapePoints.size(); si++)
			{
				fprintf(st,"%7d,", number_of_features_points++);
			}
			fprintf(st,"\n");

		}
	fclose(st);
	}


std::ofstream origin_dat_file((directory + "origin.dat"));
std::ofstream destination_dat_file((directory + "destination.dat"));

bool bAutoConstructionFlag = false;
std::map<int, DTAZone>	::iterator itrZone;

for(itrZone = m_ZoneMap.begin(); itrZone != m_ZoneMap.end(); itrZone++)
{
		// zone.dat
		if(itrZone->second .m_DSPGenerationLinkVector.size() == 0)  // no generation link
		{

			for(int i = 0; i< itrZone->second.m_ActivityLocationVector .size(); i++)
		{
			DTAActivityLocation element = itrZone->second.m_ActivityLocationVector[i];
				
			DTANode*  pFromNode = m_NodeNoMap[m_NodeNumbertoNodeNoMap[element.NodeNumber ]];

			for(unsigned int i = 0; i< pFromNode->m_OutgoingLinkVector.size(); i++)
			{
			DTALink* pLink = m_LinkNoMap[ pFromNode->m_OutgoingLinkVector[i]];

			DTAGenerationLink gl;
			gl.FromNodeNumber = pLink->m_FromNodeNumber ;
			gl.ToNodeNumber = pLink->m_ToNodeNumber ;

			itrZone->second .m_DSPGenerationLinkVector.push_back(gl);


			}

		}
	
	
	}

	// origin.dat
	origin_dat_file << std::setw(5) << itrZone->second .m_DSPZoneNo ;
	origin_dat_file << std::setw(5) << itrZone->second .m_DSPGenerationLinkVector.size();
	origin_dat_file << std::setw(5) << 0.0;
	origin_dat_file << std::endl;



	// Generation links
	for(int glIt = 0; glIt < itrZone->second .m_DSPGenerationLinkVector.size(); glIt++)
	{
		DTAGenerationLink element =   itrZone->second .m_DSPGenerationLinkVector[glIt];

		origin_dat_file << std::setw(7) << element.FromNodeNumber;
		origin_dat_file << std::setw(7) << element.ToNodeNumber;
		sprintf(format_string, "%7.3f", element.LoadingFactor);
		origin_dat_file << format_string;
		origin_dat_file << std::endl;
	}

	// destination.dat
	destination_dat_file << std::setw(5) << itrZone->second .m_DSPZoneNo ;

	destination_dat_file << std::setw(5) << itrZone->second.m_ActivityLocationVector .size();

	for(int i = 0; i< itrZone->second.m_ActivityLocationVector .size(); i++)
	{
		DTAActivityLocation element = itrZone->second.m_ActivityLocationVector[i];
		destination_dat_file << std::setw(7) <<element.NodeNumber ;

	}
	destination_dat_file << std::endl;



}


origin_dat_file.close();
destination_dat_file.close();

std::ofstream TAZ_mapping_dat_file(directory + "TAZ_mapping.dat");
TAZ_mapping_dat_file << "Zone No, TAZ" << std::endl;

for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
{
	TAZ_mapping_dat_file << std::setw(5) << itr->second .m_DSPZoneNo;
	TAZ_mapping_dat_file << std::setw(5) << itr->first;
	TAZ_mapping_dat_file << std::endl;
}
TAZ_mapping_dat_file.close();

// zone.dat is done
// origin.dat is done
// destination.dat is done
// output_option.dat is done

// movement.dat is done
// write empty control file when file does not exist

if(CheckIfFileExsits(directory + "control.dat" )==false)
{
 std::ofstream control_dat_file((directory + "control.dat"));

	// Number of timing plans
    control_dat_file << std::setw(3) << 1 << std::endl;

    sprintf(format_string, "%10.2f", 0.0);
    control_dat_file << format_string ;
	 control_dat_file<< std::endl;

		std::list<DTANode*>::iterator iNode;
		for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{

			// Write the record
			control_dat_file << std::setw(7) <<  (*iNode)->m_NodeNumber ;
			control_dat_file << std::setw(2) << 1;  // no control
			control_dat_file << std::setw(2) << 0;
			control_dat_file << std::setw(4) << 0;
			control_dat_file << std::endl;
		}
			 control_dat_file.close();
			// control.dat is done
}

	// copy default files

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	CString DefaultDataFolder;

	DefaultDataFolder.Format ("%s\\default_data_folder\\DSP\\",pMainFrame->m_CurrentDirectory);

	if(directory.GetLength () == 0)
		directory = m_ProjectDirectory;

	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"bus.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"GradeLengthPCE.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"incident.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"leftcap.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"output_option.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"pricing.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"ramp.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"scenario.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"StopCap2Way.dat");

	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"StopCap4Way.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"subarea.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"system.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"toll.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"vms.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"WorkZone.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"YieldCap.dat");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"parameter.dat");



	SaveDSPDemandFiles(OriginDirectory, directory);

return true;
}


bool CTLiteDoc::ReadDYNASMART_ControlFile()
{

	m_TimingPlanVector.clear();

	FILE* st;
	fopen_s(&st,m_ProjectDirectory+"control.dat","r");
	if(st != NULL)
	{
		int num_timing_plan = g_read_integer(st);
		
		char  str_line[2000]; // input string
		int str_line_size;

		g_read_a_line(st,str_line, str_line_size); //  skip the second line


		//std::vector<float> values;
		//int number_of_values = g_read_number_of_numerical_values(str_line,str_line_size,values);

		//values.push_back (1440);

			std::string timing_plan_name = "0";
			AddNameIntoTimingPlanVector(0, 1440,timing_plan_name);

			for(int timing_plan  = 1; timing_plan <= num_timing_plan; timing_plan++)
		{

			if(timing_plan > 1)
			{
			CString str;
			str.Format ("Plan%d", timing_plan);

			timing_plan_name  =  CString2StdString(str);
			AddNameIntoTimingPlanVector(0, 1440,timing_plan_name);
			}

			

			// read the first block: Node - Control Type
			std::list<DTANode*>::iterator iNode;

			int last_good_node_number = 0;
			for ( iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
			{
				int node_name = g_read_integer(st);
				if(node_name == -1)
				{
					CString str;
					str.Format("Error in reading the node block of control.dat. Last valid node number = %d ", last_good_node_number);
					AfxMessageBox(str, MB_ICONINFORMATION);
					fclose(st);
				}

				last_good_node_number = node_name;

				DTANode*  pNode = m_NodeNoMap[m_NodeNumbertoNodeNoMap[node_name]];
				pNode->m_ControlType  = g_read_integer(st);
				pNode->m_NumberofPhases = g_read_integer(st);
				pNode->m_CycleLengthInSecond = g_read_integer(st);


	/*			if(pNode->m_ControlType == m_ControlType_PretimedSignal || pNode->m_ControlType == m_ControlType_ActuatedSignal)
				{

					if(pNode->m_ControlType == m_ControlType_PretimedSignal)
						SetupSignalValue(node_name,timing_plan_name,TIMING_ControlType, "pretimed");

					if(pNode->m_ControlType == m_ControlType_ActuatedSignal)
						SetupSignalValue(node_name,timing_plan_name,TIMING_ControlType, "actuated");

					SetupSignalValue(node_name,timing_plan_name,TIMING_RingType, "single_ring");
					SetupSignalValue(node_name,timing_plan_name,TIMING_CycleLength, pNode->m_CycleLengthInSecond);
					SetupSignalValue(node_name,timing_plan_name,TIMING_Offset, 0);
				}*/
			}

			// read the second block: Phase time and movement
			// read node by node

			int number_of_signals = 0;

			for ( iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
			{
				DTANode* pNode = (*iNode);

				if(pNode->m_ControlType == m_ControlType_PretimedSignal || pNode->m_ControlType == m_ControlType_ActuatedSignal)
				{
					pNode-> m_bSignalData = true;
					number_of_signals++;

					int time = 0; 
					int start_time = 0;
					int end_time = 0;

					for(int p  = 0; p < pNode->m_NumberofPhases; p++)
					{
						start_time = time;

						int node_name = g_read_integer(st);

						if(m_NodeNumbertoNodeNoMap.find(node_name) == m_NodeNumbertoNodeNoMap.end())
						{
							CString str;
							str.Format("Error in reading the signal data block of control.dat. Last valid node number = %d ", last_good_node_number);
							AfxMessageBox(str, MB_ICONINFORMATION);
							fclose(st);
						}
						last_good_node_number = node_name;


						DTANode*  pNode = m_NodeNoMap[m_NodeNumbertoNodeNoMap[node_name]];

						int phase_ID = g_read_integer(st);

						DTANodePhase phase;

						if( pNode->m_ControlType == m_ControlType_ActuatedSignal)
						{
							phase.max_green  = g_read_integer(st);
							phase.min_green   = g_read_integer(st);
						}
						if( pNode->m_ControlType == m_ControlType_PretimedSignal)
						{
							g_read_integer(st);  // offset
							phase.max_green   = g_read_integer(st);
							phase.min_green = phase.max_green ;
						}


						phase.amber  = g_read_integer(st);

	/*					SetupPhaseData(node_name, timing_plan_name, p+1, PHASE_MinGreen,phase.min_green);
						SetupPhaseData(node_name, timing_plan_name, p+1, PHASE_MaxGreen,phase.max_green);
						SetupPhaseData(node_name, timing_plan_name, p+1, PHASE_Yellow,phase.amber);*/


						int approach = g_read_integer(st);


						// approach node numbers (reserved 4 nodes)
						g_read_integer(st);
						g_read_integer(st);
						g_read_integer(st);
						g_read_integer(st);

						//
						// read all possible approaches

						std::string movement_vector; 

						for(int i=0; i< approach; i++)
						{

							int in_link_from_node_number = g_read_integer(st);
							int in_link_to_node_number = g_read_integer(st);

							int in_link_from_node_id = m_NodeNumbertoNodeNoMap[in_link_from_node_number];
							int in_link_to_node_id = m_NodeNumbertoNodeNoMap[in_link_to_node_number];

							int phase_ID2 = g_read_integer(st);     // remember to read redundant phase id

							int movement_size  = g_read_integer(st);

							for(int k=0; k<movement_size; k++)
							{
								int out_link_to_node_number =  g_read_integer(st);
								int out_link_to_node_id = m_NodeNumbertoNodeNoMap[out_link_to_node_number];

								movement_vector += string_format("%d",in_link_from_node_number);
								movement_vector += ":";
								movement_vector += string_format("%d",out_link_to_node_number); 
								movement_vector += ";";


							}  // movement

						} // approach 

						time += (phase.max_green ) ;
						end_time = time;
						time += (phase.amber) ;

						SetupPhaseData(node_name,timing_plan_name, p+1, PHASE_MOVEMENT_VECTOR, movement_vector);
						//SetupPhaseData(node_name,timing_plan_name, p+1, PHASE_Start, start_time);
						//SetupPhaseData(node_name,timing_plan_name, p+1, PHASE_End, end_time);
					} // phase

				}   // control data
			}  // for each node

			UpdateAllMovementGreenStartAndEndTime(timing_plan_name);

		}
		fclose(st);


		//      m_SignalDataLoadingStatus.Format ("%d signals are loaded.",number_of_signals);
	}


	return true;
}

bool CTLiteDoc::ReadDYNASMART_ControlFile_ForAMSHub()
{
	FILE* st;

	int number_of_nodes = 0;
	int number_of_signals = 0;

	std::vector<int> DSP_signal_node_vector;
	fopen_s(&st,m_ProjectDirectory+"dsp_control.dat","r");
	if(st != NULL)
	{
		CDlg_UserInput dlg;

		dlg.m_StrQuestion  = "Please specify the number of nodes to be loaded from file dsp_control.dat:";
		dlg.m_InputValue = "10";

		if(dlg.DoModal ()==IDOK)
		{
			number_of_nodes = atoi(dlg.m_InputValue) ;
		}

		CWaitCursor cursor;

		int num_timing_plan = g_read_integer(st);

		if(num_timing_plan > 1)
		{
			AfxMessageBox("Number of signal timing plan is greater than 1.  Only one signal timing plan is allowed to generate the actuated control data in the current version.", MB_ICONINFORMATION);
			fclose(st);
		}

		//              double start_time = g_read_float(st);
		char  str_line[2000]; // input string
		int str_line_size;
		g_read_a_line(st,str_line, str_line_size); //  skip the second line

		// read the first block: Node - Control Type
		int last_good_node_number = 0;
		for ( int i = 0; i< number_of_nodes; i++)
		{
			int node_name = g_read_integer(st);
			if(node_name == -1)
			{
				CString str;
				str.Format("Error in reading the node block of control.dat. Last valid node number = %d ", last_good_node_number);
				AfxMessageBox(str, MB_ICONINFORMATION);
				fclose(st);
			}

			last_good_node_number = node_name;

			if(m_NodeNumbertoNodeNoMap.find(node_name)!=m_NodeNumbertoNodeNoMap.end())
			{
				DTANode*  pNode = m_NodeNoMap[m_NodeNumbertoNodeNoMap[node_name]];
				pNode->m_ControlType  = g_read_integer(st);

				if(pNode->m_ControlType == m_ControlType_PretimedSignal || pNode->m_ControlType == m_ControlType_ActuatedSignal)
				{
					DSP_signal_node_vector.push_back (node_name);
				}


				pNode->m_NumberofPhases = g_read_integer(st);
				pNode->m_CycleLengthInSecond = g_read_integer(st);

				TRACE("\nNode Number = %d, type = %d",node_name,pNode->m_ControlType );
			}else
			{
				TRACE("\nMissing Node Number = %d",node_name);
				g_read_integer(st);
				g_read_integer(st);
				g_read_integer(st);



			}

		}

		// read the second block: Phase time and movement
		// read node by node

		for ( int i = 0; i< DSP_signal_node_vector.size(); i++)
		{

			int node_name = DSP_signal_node_vector[i];
			if(m_NodeNumbertoNodeNoMap.find(node_name)!=m_NodeNumbertoNodeNoMap.end())
			{
				DTANode*  pNode = m_NodeNoMap[m_NodeNumbertoNodeNoMap[node_name]];

				if(pNode->m_ControlType == m_ControlType_PretimedSignal || pNode->m_ControlType == m_ControlType_ActuatedSignal)
				{
					pNode-> m_bSignalData = true;
					number_of_signals++;

					for(int p  = 0; p < pNode->m_NumberofPhases; p++)
					{
						int node_name = g_read_integer(st);

						last_good_node_number = node_name;


						int phase_ID = g_read_integer(st);


						if(node_name == 54154)
						{
							TRACE("");

						}

						DTANodePhase phase;
						phase.max_green  = g_read_integer(st);
						phase.min_green   = g_read_integer(st);
						phase.amber  = g_read_integer(st);
						int approach = g_read_integer(st);


						// approach node numbers (reserved 4 nodes)
						g_read_integer(st);
						g_read_integer(st);
						g_read_integer(st);
						g_read_integer(st);

						//
						// read all possible approaches
						for(int i=0; i< approach; i++)
						{

							int in_link_from_node_number = g_read_integer(st);
							int in_link_to_node_number = g_read_integer(st);
							int in_link_from_node_id = m_NodeNumbertoNodeNoMap[in_link_from_node_number];
							int in_link_to_node_id = m_NodeNumbertoNodeNoMap[in_link_to_node_number];

							int phase_ID2 = g_read_integer(st);     // remember to read redundant phase id

							int movement_size  = g_read_integer(st);

							for(int k=0; k<movement_size; k++)
							{
								int out_link_to_node_id = m_NodeNumbertoNodeNoMap[g_read_integer(st)];

								CString movement_index = pNode->GetMovementIndex(in_link_from_node_id, in_link_to_node_id, out_link_to_node_id);
								if(movement_index>=0)
								{
									phase.movement_index_map[movement_index] = true;

								}
							}  // movement
						} // approach 
					} // phase

				}   // control data
			}
		}  // for each node
		fclose(st);


		m_SignalDataLoadingStatus.Format ("%d signals are loaded.",number_of_signals);
	}

	return true;
}

void CTLiteDoc::OnToolsReverseverticalcoordinate()
{

	if(m_bDYNASMARTDataSet== false)
	{

		AfxMessageBox("This memu/function is used for DYNASMARAT data sets.",MB_ICONINFORMATION);

		return;
	}

	char lpbuffer[64];

	sprintf_s(lpbuffer,"%d",m_YCorridonateFlag*(-1));

	WritePrivateProfileString("coordinate", "origin",lpbuffer,m_ProjectFile);

	CString str;
	str.Format("The horizional coordinate flag (y_coordinate_flag) has been reset to %s in project file %s.\nPlease close NEXTA and reload the project.",m_ProjectFile, lpbuffer );
	AfxMessageBox(str,MB_ICONINFORMATION);

}

bool CTLiteDoc::ReadDYNASMARTVehicleTrajectoryFile(LPCTSTR lpszFileName, int date_id)
{
	FILE* pFile;
	fopen_s(&pFile,lpszFileName,"r");

	if(pFile != NULL)
	{
		int good_vehicle_id = 1;
		while(!feof(pFile) )
		{
			int m_VehicleID= g_read_integer(pFile, false);
			if(m_VehicleID == -1)
				break;
			// Veh #    144 Tag= 3 OrigZ=  10 DestZ=  11 Class= 3 UstmN=    150 DownN=     14 DestN=     13 STime=   0.50 

			DTAVehicle* pVehicle = 0;
			pVehicle = new DTAVehicle;
			pVehicle->m_VehicleID           = m_VehicleID;

			int CompleteFlag = g_read_integer(pFile);
			int NodeSizeOffset = 0;
			if(CompleteFlag==0 || CompleteFlag==1) 
			{
				pVehicle->m_bComplete = false;
				NodeSizeOffset = -1;
			}
			else 
			{
				pVehicle->m_bComplete = true;
				NodeSizeOffset = 0; 
			}

			pVehicle->m_OriginZoneID        = g_read_integer(pFile);
			pVehicle->m_DestinationZoneID = g_read_integer(pFile);

			pVehicle->m_InformationClass = (unsigned char)g_read_integer(pFile);

			int FirstNodeID = g_read_integer(pFile); 
			int SecondNodeID = g_read_integer(pFile);    // (4) first node
			int DestinationNodeID = g_read_integer(pFile);    // last node


			pVehicle->m_OriginZoneID = NodeIDtoZoneNameMap[FirstNodeID];
			pVehicle->m_DestinationZoneID = NodeIDtoZoneNameMap[DestinationNodeID];

			if( pVehicle->m_OriginZoneID > m_ODSize )
				m_ODSize = pVehicle->m_OriginZoneID ;

			if( pVehicle->m_DestinationZoneID > m_ODSize )
				m_ODSize = pVehicle->m_DestinationZoneID ;


			pVehicle->m_DepartureTime       = m_SimulationStartTime_in_min + g_read_float(pFile);

			if(g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
				g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

			// Total Travel Time=   0.34 # of Nodes=   2 VehType 1 LOO 1
			pVehicle->m_TripTime  = g_read_float(pFile);
			pVehicle->m_ArrivalTime = pVehicle->m_DepartureTime + pVehicle->m_TripTime;

			pVehicle->m_NodeSize    = g_read_integer(pFile) +1;  // +1 as we need to allocate one more node for the first node
			pVehicle->m_DemandType = g_read_integer(pFile);
			g_read_integer(pFile);

			//                      pVehicle->m_VehicleType = (unsigned char)g_read_integer(pFile);

			pVehicle->m_VOT = 10;
			pVehicle->m_TollDollarCost = 0;
			pVehicle->m_PM = 0;

			pVehicle->m_Distance = 0;

			pVehicle->m_NodeAry = new SVehicleLink[pVehicle->m_NodeSize];

			pVehicle->m_NodeNumberSum = 0;

			m_PathNodeVectorSP[0] =  FirstNodeID;

			int i;
			for(i=1; i< pVehicle->m_NodeSize; i++)
			{
				m_PathNodeVectorSP[i] = g_read_integer(pFile);

				//// update origin destination data based on TAZ
				//if(i==0 )
				//{
				//      pVehicle->m_OriginZoneID = m_NodeNoMap[m_PathNodeVectorSP[i] ]-> m_ZoneID;

				//}

				//if(i== pVehicle->m_NodeSize -1 )
				//{
				//      pVehicle->m_DestinationZoneID = m_NodeNoMap[m_PathNodeVectorSP[i] ]-> m_ZoneID;

				//}

				pVehicle->m_NodeNumberSum += m_PathNodeVectorSP[i];
				DTALink* pLink = FindLinkWithNodeNumbers(m_PathNodeVectorSP[i-1],m_PathNodeVectorSP[i]);
				if(pLink==NULL)
				{
					CString str;
					str.Format ("Error in reading file Vehicle.csv, good vehicle id: %d",good_vehicle_id);
					AfxMessageBox(str);
					fclose(pFile);

					return false;
				}
				pVehicle->m_NodeAry[i].LinkNo  = pLink->m_LinkNo ;
				pVehicle->m_Distance +=pLink->m_Length ;

				if(pVehicle->m_NodeAry[i].ArrivalTimeOnDSN < 10000) // feasible arrival time
				{
					pLink->m_total_link_volume +=1;

					double travel_time = 0;
					if(i>=2)
						travel_time  = pVehicle->m_NodeAry[i].ArrivalTimeOnDSN - pVehicle->m_NodeAry[i-1].ArrivalTimeOnDSN;
					else // first link
						travel_time  = pVehicle->m_NodeAry[i].ArrivalTimeOnDSN - pVehicle->m_DepartureTime ;

					pLink->m_TotalTravelTime += travel_time;
					pLink->m_total_delay += ( travel_time- pLink->m_FreeFlowTravelTime );
					float delay = travel_time- pLink->m_FreeFlowTravelTime;
					pLink->AddNodeDelay(pVehicle->m_NodeAry[i].ArrivalTimeOnDSN ,delay);
				}
			}


			// ==>Node Exit Time Point
			for(i=1; i< pVehicle->m_NodeSize + NodeSizeOffset; i++)
			{
				pVehicle->m_NodeAry[i].ArrivalTimeOnDSN = m_SimulationStartTime_in_min + g_read_float(pFile);
			}


			// ==>Link Travel Time
			for(i=1; i< pVehicle->m_NodeSize + NodeSizeOffset; i++)
			{
				g_read_float(pFile);  // // travel time
			}
			// ==>Accumulated Stop Time
			for(i=1; i< pVehicle->m_NodeSize +NodeSizeOffset; i++)
			{
				g_read_float(pFile);  // stop time
			}

			m_VehicleSet.push_back (pVehicle);
			m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;

			if(m_VehicleSet.size()%10000==0)
			{
				CString str;
				str.Format ("loading %d vehicles",m_VehicleSet.size());
				SetStatusText(str);
			}

			good_vehicle_id = m_VehicleID;
		}

		fclose(pFile);

		m_SimulationVehicleDataLoadingStatus.Format ("%d vehicles are loaded from VehTrajectory.dat .",m_VehicleSet.size());
	}
	return true;
}

void CTLiteDoc::RecalculateLinkMOEFromVehicleTrajectoryFile()
{
	std::list<DTALink*>::iterator iLink ;

	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->ResetMOEAry(g_Simulation_Time_Horizon);  // use one day horizon as the default value
	}

	int count  = 0;
	std::list<DTAVehicle*>::iterator iVehicle;

	for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++, count++)
	{
		DTAVehicle* pVehicle = (*iVehicle);

		// ==>Node Exit Time Point

		int previous_link_t = -1;  // initial value: - 1
		for(int i=1; i< pVehicle->m_NodeSize ; i++)
		{
			if(pVehicle->m_NodeAry[i].LinkNo==-1) 
				continue;
			DTALink* pLinkCurrent = m_LinkNoMap[ pVehicle->m_NodeAry[i].LinkNo];

			int t = pVehicle->m_NodeAry[i].ArrivalTimeOnDSN+1;


			// timt t is the timestamp from the current link to the next link, in min 
			if(pLinkCurrent!=NULL && t >=0 && t <  pLinkCurrent->m_LinkMOEArySize )
			{
				pLinkCurrent->m_LinkMOEAry[t].VehicleOutflowCount+=1;

				if(previous_link_t>=0 && previous_link_t < pLinkCurrent->m_LinkMOEArySize  )
				pLinkCurrent->m_LinkMOEAry[previous_link_t].VehicleInflowCount+=1;

				
			}


			if(i==1)  // first link 
			{
				int dep_t = pVehicle->m_DepartureTime+1;
				if(pLinkCurrent!=NULL && dep_t>=0 && dep_t< pLinkCurrent->m_LinkMOEArySize  )
				{
					pLinkCurrent->m_LinkMOEAry[dep_t].VehicleInflowCount+=1;

				}

			}
			previous_link_t = t;

			if(t>=1400)
			{
			TRACE("");
			
			}



		}
	}

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		(*iLink)->RecalculateLinkMOEBasedOnVehicleCount();
	}


	

}


void CTLiteDoc::SaveDSPDemandFiles(CString OriginDirectory, CString DestinationDirectory)
{
	int ODZoneNumberSize = 0;

	std::map<int, DTAZone>	:: iterator itr;

	for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
	{

		if(itr->first > ODZoneNumberSize)
			ODZoneNumberSize = itr->first ;
	
	}

	char  str_line[2000]; // input string
	int str_line_size;

	int AggregationTimetInterval = 15;

	float total_demand_in_demand_file = 0;
	float total_number_of_vehicles_to_be_generated = 0;

	int max_line_number_for_logging = 10;

	m_DemandLoadingStartTimeInMin = 1440;
	m_DemandLoadingEndTimeInMin = 0;

	CCSVParser parser0;
	if (parser0.OpenCSVFile(CString2StdString(OriginDirectory + "input_demand_file_list.csv")))
	{
		int i=0;

		while(parser0.ReadRecord())
		{

			int file_sequence_no = -1;
			string file_name;
			string format_type;
			int number_of_lines_to_be_skipped = 0;
			int subtotal_in_last_column = 0;
			int demand_type_in_3rd_column = 0;
			float loading_multiplier =1;
			int start_time_in_min = -1; 
			int end_time_in_min = -1;
			int number_of_demand_types = 0;
			float local_demand_loading_multiplier = 1;
			char demand_type_field_name[20] ;
			int demand_type_code[20]={0};

			int demand_format_flag = 0;

			parser0.GetValueByFieldName("file_sequence_no",file_sequence_no);

			if(file_sequence_no <=-1)  // skip negative sequence no 
				break;

			parser0.GetValueByFieldName("file_name",file_name);
			if(file_name.length()==0)  // no file name input
			{
				break;
			}

			parser0.GetValueByFieldName("start_time_in_min",start_time_in_min);
			parser0.GetValueByFieldName("end_time_in_min",end_time_in_min);

			if(start_time_in_min ==-1)  // skip negative sequence no 
			{
				AfxMessageBox("Please provide start_time_in_min in file input_demand_file_list.csv");
				return;
			}
			if(end_time_in_min ==-1)  // skip negative sequence no 
			{
				AfxMessageBox( "Please provide end_time_in_min in file input_demand_file_list.csv");
				return;
			}

			if(end_time_in_min>1440)
			{
				AfxMessageBox( "end_time_in_min should be less than 1440 min in input_demand_file_list.csv" );
				return;
			}

			if(start_time_in_min < 0 )
			{
				AfxMessageBox( "start_time_in_min should be greater than 0 min in input_demand_file_list.csv" );
				return;
			}

			// set g_DemandLoadingStartTimeInMin according the start time and end time of each record
			if(m_DemandLoadingStartTimeInMin > start_time_in_min)
				m_DemandLoadingStartTimeInMin = start_time_in_min;

			if(m_DemandLoadingEndTimeInMin < end_time_in_min)
				m_DemandLoadingEndTimeInMin = end_time_in_min;

		}

	}  //determine loading horizon

	  int NumberOfZones = m_ZoneMap.size();
	int demand_data_size = max(1,(m_DemandLoadingEndTimeInMin - m_DemandLoadingStartTimeInMin)/15);
	TDDemandSOVMatrix = Allocate3DDynamicArray<float>(NumberOfZones+1, NumberOfZones+1, demand_data_size);
	TDDemandHOVMatrix = Allocate3DDynamicArray<float>(NumberOfZones+1, NumberOfZones+1, demand_data_size);
	TDDemandTruckMatrix = Allocate3DDynamicArray<float>(NumberOfZones+1, NumberOfZones+1, demand_data_size);


	//DSP NO start from 1
	for(int oz = 0; oz <= NumberOfZones; oz++)
		for(int dz = 0; dz <= NumberOfZones; dz++)
			for(int t = 0; t < demand_data_size; t++)
			{
			TDDemandSOVMatrix[oz][dz][t] = 0;
			TDDemandHOVMatrix[oz][dz][t] = 0;
			TDDemandTruckMatrix[oz][dz][t] = 0;
			
			}


	CCSVParser parser;

	//step 3:
	if (parser.OpenCSVFile(CString2StdString(OriginDirectory + "input_demand_file_list.csv")))
	{
		int i=0;

		while(parser.ReadRecord())
		{

			int file_sequence_no =1;
			string file_name;
			string format_type= "null";
			int number_of_lines_to_be_skipped = 0;
			int subtotal_in_last_column = 0;
			int demand_type_in_3rd_column = 0;

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

			std::string local_file_name;
			parser.GetValueByFieldName("file_name",local_file_name);

			file_name = CString2StdString(OriginDirectory) + local_file_name;

			if(file_name.length()==0)  // no file name input
			{
				break;
			}

			parser.GetValueByFieldName("start_time_in_min",start_time_in_min);
			parser.GetValueByFieldName("end_time_in_min",end_time_in_min);

			if(end_time_in_min>1440)
			{
				AfxMessageBox( "end_time_in_min should be less than 1440 min in input_demand_file_list.csv");
				return;
			}

			if(start_time_in_min < 0 )
			{
				AfxMessageBox( "start_time_in_min should be greater than 0 min in input_demand_file_list.csv");
				return;
			}


			parser.GetValueByFieldName("format_type",format_type);
			if(format_type.find ("null")!= string::npos)  // skip negative sequence no 
			{
				AfxMessageBox( "Please provide format_type in file input_demand_file_list.csv" );
				return;
			}

			parser.GetValueByFieldName("number_of_lines_to_be_skipped",number_of_lines_to_be_skipped);
			parser.GetValueByFieldName("subtotal_in_last_column",subtotal_in_last_column);
			parser.GetValueByFieldName("demand_type_in_3rd_column",demand_type_in_3rd_column);

			int apply_additional_time_dependent_profile =0;	
			parser.GetValueByFieldName("apply_additional_time_dependent_profile",apply_additional_time_dependent_profile);
			parser.GetValueByFieldName("loading_multiplier",local_demand_loading_multiplier);


			double time_dependent_ratio[MAX_TIME_INTERVAL_SIZE] = {0};

			double total_ratio = 0;
			if(apply_additional_time_dependent_profile==1)
			{
				for(int time_index = start_time_in_min/15; time_index < end_time_in_min/15; time_index++)  // / 15 converts min to 15-min interval for demand patterns
				{
					std::string time_stamp_str = "'" + GetTimeStampStrFromIntervalNo (time_index);

					time_dependent_ratio[time_index] = 0;
					parser.GetValueByFieldName(time_stamp_str,time_dependent_ratio[time_index]);
					total_ratio += time_dependent_ratio[time_index];
				}


				if( total_ratio < 0.001)
				{
					AfxMessageBox( "The total temporal ratio read from file input_temporal_demand_profile is 0, which means no demand will be loaded. ");
					return;
				}

			}

			parser.GetValueByFieldName("number_of_demand_types",number_of_demand_types);

			if(demand_type_in_3rd_column == 1)
			{
				if(number_of_demand_types != 1)
				{
					AfxMessageBox( "Error: number_of_demand_types should be 1 when demand_type_in_3rd_column is set to 1. The current value is "/* << number_of_demand_types <<*/);
					return;

				}

			}



			for(int type = 1; type <= number_of_demand_types; type++)
			{
				sprintf(demand_type_field_name,"demand_type_%d",type);
				int demand_type= -1;
				parser.GetValueByFieldName(demand_type_field_name,demand_type);

				if(demand_type == -1)
				{
					AfxMessageBox( "Missing input: no value has been specified for field << demand_type_field_name <<  file_name <<" ); 
					return;

				}

				demand_type_code[type]  = demand_type;
			}


			if(format_type.find("column")!= string::npos)
			{

				if(number_of_demand_types==0)
				{
					AfxMessageBox(  "number_of_demand_types = 0 in file input_demand_file_list.csv. Please check.");
					return;
				}

				bool bFileReady = false;
				int i;

				FILE* st;

				//test # of numerical values per line
				fopen_s(&st,file_name.c_str (), "r");
				if (st!=NULL)
				{

					char  str_line[2000]; // input string
					int str_line_size;

					// skip lines
					for(int line_skip = 0 ;line_skip < number_of_lines_to_be_skipped;line_skip++)
					{
						g_read_a_line(st,str_line, str_line_size); //  skip the first line
					}
					g_read_a_line(st,str_line, str_line_size); //  skip the first line

					std::vector<float> values;

					int number_of_values = g_read_number_of_numerical_values(str_line,str_line_size,values);

					if(demand_type_in_3rd_column == 1)
					{

						if(number_of_values != 4)
						{
							AfxMessageBox( "demand_type_in_3rd_column = 1, please make sure there are 4 values per line");
							return;

						}

					}

					if(number_of_values != 2 + number_of_demand_types + demand_type_in_3rd_column) // 2: origin, destination (demand type)
					{
					//	AfxMessageBox( "There are " << number_of_values << " values() per line in file " << file_name << "," << endl << "but " << number_of_demand_types << " demand type(s) are defined in file input_demand_file_list.csv. " << endl << 
						AfxMessageBox( "There are << number_of_values << values  per line in file ""Please check file input_demand_file_list.csv.");
						return;

					}
					fclose(st);
				}

				// read the file formaly after the test. 

				fopen_s(&st,file_name.c_str (), "r");
				if (st!=NULL)
				{
					char  str_line[2000]; // input string
					int str_line_size;

					// skip lines
					for(int line_skip = 0 ;line_skip < number_of_lines_to_be_skipped;line_skip++)
					{
						g_read_a_line(st,str_line, str_line_size); //  skip the first line
					}

					bFileReady = true;
					int line_no = number_of_lines_to_be_skipped;

					while(true)
					{
						int origin_zone = g_read_integer(st);

						if(origin_zone <=0)
						{

							if(line_no==1 && !feof(st))  // read only one line, but has not reached the end of the line
							{
								AfxMessageBox("Error: Only one line has been read from file. Are there multiple columns of demand type in file << file_name <<  per line?");
								return;

							}
							break;
						}

						if(origin_zone > ODZoneNumberSize) 
						{
							AfxMessageBox("Error: Line << line_no << origin zone = origin_zone which is greater than the maximum zone number. Please check.");
							break;
						}

						int destination_zone = g_read_integer(st);

						if(destination_zone > ODZoneNumberSize) 
						{
							AfxMessageBox("Error: Line line_no  destination zone =  destination_zone  which is greater than the maximum zone number of ODZoneNumberSize in input_zone.csv. Please check.");
							break;
						}


						if(demand_type_in_3rd_column == 1)
						{
							demand_type_code[1] =  g_read_integer(st);  // read the user specified demand type per row

						}

						float number_of_vehicles ;

						for(int type = 1; type <= number_of_demand_types; type++)
						{
							int end_of_line = 0;
							float demand_value = g_read_float_from_a_line(st, end_of_line);

							if(demand_value<-99) // encounter return 
							{
								break; 
							}

							number_of_vehicles =  demand_value*local_demand_loading_multiplier;

							if(demand_type_code[type]>=1)  // load this demand type
							{

								total_demand_in_demand_file += number_of_vehicles;


								// we generate vehicles here for each OD data line

								if(m_ZoneMap.find(origin_zone)!= m_ZoneMap.end())
								{
									m_ZoneMap[origin_zone].m_Demand += number_of_vehicles;

									if(apply_additional_time_dependent_profile==1)  // use time-dependent profile
									{
										for(int time_interval = start_time_in_min/15; time_interval < end_time_in_min/15; time_interval++)
										{
											if(time_dependent_ratio [time_interval] > 0.000001) // this is the last one applicable
											{
												// reset the time interval, create vehicles with the same origin, destination, changed # of vehicles, and time interval
												double number_of_vehicles_to_be_loaded = time_dependent_ratio [time_interval] * number_of_vehicles;
												CreateVehicles(origin_zone,destination_zone,number_of_vehicles_to_be_loaded,demand_type_code[type],time_interval*15,(time_interval+1)*15);
											}
										}
									}else // do not use time-dependent profile
									{
										float number_of_time_dependent_intervals= max(1,(end_time_in_min-start_time_in_min)/AggregationTimetInterval);
										CreateVehicles(origin_zone,destination_zone,number_of_vehicles,demand_type_code[type],start_time_in_min,end_time_in_min);

									}
								}
								//  given the number of OD demand voluem to be created. we need to apply time-dependent profile for each data block , 


							}else if(type!=0)  // if demand type == 0 then we will skip this value. By doing so, we can reading only one demand type per record with demand-type specific departure time loading profile. e.g. truck vs. HOV
							{
								AfxMessageBox( "demand type ?? in file input_demand_meta_data has not been defined. Please check.");
								break;

							}
						}  // for each demand type
						//if(line_no >= max_line_number)
						//break;
						line_no++;
					}  // scan lines

					if(total_demand_in_demand_file<0.5f)
					{
						AfxMessageBox( "Error: File  contain 0 trips.");
						break;					
					}

					fclose(st);
				}else  //open file
				{
					AfxMessageBox("File ?? cannot be opened.\n It might be currently used and locked by EXCEL.");
					return;

				}


			}else if (format_type.compare("matrix")== 0)
			{

				if (g_detect_if_a_file_is_column_format(file_name.c_str()) == true)
				{
					CString str;
					str.Format("Demand input file %s looks to be based on column format but the format_type=matrix in input_demand_file_list.csv.\nPlease check the demand file format, and change format_type=column in input_demand_meta_data.cv.", file_name);
					AfxMessageBox(str);
					return;

				}
				vector<int> LineIntegerVector;

				CCSVParser parser;
				parser.IsFirstLineHeader = false;
				if (parser.OpenCSVFile(file_name))
				{
					int control_type_code;
					int i=0;
					if(parser.ReadRecord())
					{
						parser.ConvertLineStringValueToIntegers ();
						LineIntegerVector = parser.LineIntegerVector ;
					}

				}

				int number_of_zones = LineIntegerVector.size();


				bool bFileReady = false;
				int i;

				FILE* st;
				fopen_s(&st,file_name.c_str (), "r");
				if (st!=NULL)
				{
					// read the first line
					g_read_a_line(st,str_line,str_line_size);


					//test if a zone has been defined. 
					for(int destination_zone_index = 0; destination_zone_index < number_of_zones; destination_zone_index++)
					{
						int zone = LineIntegerVector[destination_zone_index];

						if(m_ZoneMap.find(zone)== m_ZoneMap.end())
						{
//							AfxMessageBox( "Zone "  << zone << " (no." << destination_zone_index+1 << " in the first line) of file " << file_name << " has not been defined in input_zone.csv. Please check." <<endl;
							return;

						}
					}



					int line_no = 0;
					for(int origin_zone_index = 0; origin_zone_index < number_of_zones; origin_zone_index++)
					{
						int origin_zone = g_read_integer(st); // read the origin zone number
						for(int destination_zone_index = 0; destination_zone_index < number_of_zones; destination_zone_index++)
						{
							int destination_zone = LineIntegerVector[destination_zone_index];
							float value = g_read_float(st);

							float number_of_vehicles =  value*local_demand_loading_multiplier;  // read the value

							line_no++;
							int type = 1;  // first demand type definition
							if(demand_type_code[type]>=1)  // feasible demand type
							{
								total_demand_in_demand_file += number_of_vehicles;

								m_ZoneMap[origin_zone].m_Demand += number_of_vehicles;
								// condition 1: without time-dependent profile 

								if(apply_additional_time_dependent_profile==1)  // use time-dependent profile
								{
									for(int time_interval = start_time_in_min/15; time_interval < end_time_in_min/15; time_interval++)
									{
										if(time_dependent_ratio [time_interval] > 0.000001) // this is the last one applicable
										{
											// reset the time interval, create vehicles with the same origin, destination, changed # of vehicles, and time interval
											double number_of_vehicles_to_be_loaded = time_dependent_ratio [time_interval] * number_of_vehicles;

											CreateVehicles(origin_zone,destination_zone,number_of_vehicles_to_be_loaded,demand_type_code[type],time_interval*15,(time_interval+1)*15);
										}
									}
								}else // do not use time-dependent profile
								{

									float number_of_time_dependent_intervals= max(1,(end_time_in_min-start_time_in_min)/15);

									CreateVehicles(origin_zone,destination_zone,number_of_vehicles,demand_type_code[type],start_time_in_min,end_time_in_min);

								}


							}

						}
						//
						if(subtotal_in_last_column==1)
							g_read_float(st); //read sub total value

					}

					if(total_demand_in_demand_file<0.5f)
					{
						AfxMessageBox( "Error: File  contain 0 trips.");
						return;					
					}

					fclose(st);
				}else  //open file
				{
					AfxMessageBox( "Error: File cannot be opened.\n It might be currently used and locked by EXCEL.");
					return;

				}

			} else if (format_type.compare("full_matrix")== 0)
			{
				vector<int> LineIntegerVector;

				CCSVParser parser;
				parser.IsFirstLineHeader = false;
				if (parser.OpenCSVFile(file_name))
				{
					int control_type_code;
					int i=0;
					if(parser.ReadRecord())
					{
						parser.ConvertLineStringValueToIntegers ();
						LineIntegerVector = parser.LineIntegerVector ;
					}

				}

				int number_of_zones = LineIntegerVector.size();


				bool bFileReady = false;
				int i;

				FILE* st;
				fopen_s(&st,file_name.c_str (), "r");
				if (st!=NULL)
				{
					// read the first line
				g_read_a_line(st,str_line, str_line_size); //  skip the second line

					int line_no = 0;
					for(int origin_zone_index = 0; origin_zone_index < number_of_zones; origin_zone_index++)
					{
						int origin_zone = g_read_integer(st); // read the origin zone number
						for(int destination_zone_index = 0; destination_zone_index < number_of_zones; destination_zone_index++)
						{
							int destination_zone = LineIntegerVector[destination_zone_index];
							float value = g_read_float(st);

							if(m_ZoneMap.find(origin_zone)== m_ZoneMap.end() || m_ZoneMap.find(destination_zone)== m_ZoneMap.end() )
							{
								continue; // origin zone or destination zone has not been defined, skipped. 
							}


							float number_of_vehicles =  value*local_demand_loading_multiplier;  // read the value

							line_no++;
							int type = 1;  // first demand type definition
							if(demand_type_code[type]>=1)  // feasible demand type
							{
								total_demand_in_demand_file += number_of_vehicles;

								m_ZoneMap[origin_zone].m_Demand += number_of_vehicles;
								// condition 1: without time-dependent profile 

								if(apply_additional_time_dependent_profile==1)  // use time-dependent profile
								{
									for(int time_interval = start_time_in_min/15; time_interval < end_time_in_min/15; time_interval++)
									{
										if(time_dependent_ratio [time_interval] > 0.000001) // this is the last one applicable
										{
											// reset the time interval, create vehicles with the same origin, destination, changed # of vehicles, and time interval
											double number_of_vehicles_to_be_loaded = time_dependent_ratio [time_interval] * number_of_vehicles;

											CreateVehicles(origin_zone,destination_zone,number_of_vehicles_to_be_loaded,demand_type_code[type],time_interval*15,(time_interval+1)*15);
										}
									}
								}else // do not use time-dependent profile
								{

									float number_of_time_dependent_intervals= max(1,(end_time_in_min-start_time_in_min)/15);

									CreateVehicles(origin_zone,destination_zone,number_of_vehicles,demand_type_code[type],start_time_in_min,end_time_in_min);

								}


							}

						}
						//
						if(subtotal_in_last_column==1)
							g_read_float(st); //read sub total value

					}


					fclose(st);
				}else  //open file
				{
					AfxMessageBox( "Error: File file_name cannot be opened.\n It might be currently used and locked by EXCEL.");
					return;

				}


			}
		} 
	}

	//////////////// //write file
	double demand_factor = 1.0;

	FILE* st = fopen(m_ProjectDirectory + "demand.dat","w");

   if(st!=NULL)
   {
	m_AMSLogFile.open ( m_ProjectDirectory + "DSP_data_conversion_log.csv", ios::out);
	if (m_AMSLogFile.is_open())
	{
		m_AMSLogFile.width(12);
		m_AMSLogFile.precision(3) ;
		m_AMSLogFile.setf(ios::fixed);
		m_AMSLogFile << "Start outputing demand.dat..." << endl;
	}

      if(demand_factor < 0.00001)
	  demand_factor =  1.0;


      // Number of matrices and multiplication factor
      fprintf(st, "%5d %5.3f\n",demand_data_size, demand_factor);;

       for(int dp = 0; dp <= demand_data_size; dp++)
      {
	    fprintf(st, "%7.1f", dp*15.0);
	  }

      fprintf(st, "\n");


      // Demand matrices
      for(int dp = 0; dp < demand_data_size; dp++)
      {
	 // start time line
	 
		  float start_time_in_min = dp*15.0;
		  fprintf(st, "Start Time = %6.1f\n", start_time_in_min);
		m_AMSLogFile << "Start Time = " << start_time_in_min << endl;

	 for(int j = 1; j <= NumberOfZones; j++)
	 {
		m_AMSLogFile << "origin zone = " << j << endl;

	    int i=1;
	    // Destinations
	    for(int k = 1; k <= NumberOfZones; k++)
	    {
	       float demand = TDDemandSOVMatrix[j][k][dp];
	       fprintf(st, "%10.4f", demand);

	       if(i && !(i % 6) && k!= NumberOfZones)
		  fprintf(st, "\n");
	       i++;

	   }
	   fprintf(st, "\n");
	 }
     }
      fclose(st);
   }else
   {
      char str_message[_MAX_PATH];
      sprintf(str_message, "Error in opening file demand.dat.");
      AfxMessageBox(str_message);
   }

///////////////////
  	//////////////// //write file
	demand_factor = 1.0;

	st = fopen(m_ProjectDirectory + "demand_HOV.dat","w");

   if(st!=NULL)
   {
      if(demand_factor < 0.00001)
	  demand_factor =  1.0;


      // Number of matrices and multiplication factor
      fprintf(st, "%5d %5.3f\n",demand_data_size, demand_factor);;

       for(int dp = 0; dp <= demand_data_size; dp++)
      {
	    fprintf(st, "%7.1f", dp*15.0);
	  }

      fprintf(st, "\n");


      // Demand matrices
      for(int dp = 0; dp < demand_data_size; dp++)
      {
	 // start time line
	 fprintf(st, "Start Time = %6.1f\n", dp*15.0);

	 for(int j = 1; j <= NumberOfZones; j++)
	 {
	    int i=1;
	    // Destinations
	    for(int k = 1; k <= NumberOfZones; k++)
	    {
	       float demand = TDDemandHOVMatrix[j][k][dp];
	       fprintf(st, "%10.4f", demand);

	       if(i && !(i % 6) && k!= NumberOfZones)
		  fprintf(st, "\n");
	       i++;

	   }
	   fprintf(st, "\n");
	 }
     }
      fclose(st);
   }else
   {
      char str_message[_MAX_PATH];
      sprintf(str_message, "Error in opening file demand.dat.");
      AfxMessageBox(str_message);
   }

 
  	//////////////// //write file
	demand_factor = 1.0;

	st = fopen(m_ProjectDirectory + "demand_truck.dat","w");

   if(st!=NULL)
   {
      if(demand_factor < 0.00001)
	  demand_factor =  1.0;


      // Number of matrices and multiplication factor
      fprintf(st, "%5d %5.3f\n",demand_data_size, demand_factor);;

       for(int dp = 0; dp <= demand_data_size; dp++)
      {
	    fprintf(st, "%7.1f", dp*15.0);
	  }

      fprintf(st, "\n");


      // Demand matrices
      for(int dp = 0; dp < demand_data_size; dp++)
      {
	 // start time line
	 fprintf(st, "Start Time = %6.1f\n", dp*15.0);

	 for(int j = 1; j <= NumberOfZones; j++)
	 {
	    int i=1;
	    // Destinations
	    for(int k = 1; k <= NumberOfZones; k++)
	    {
	       float demand = TDDemandTruckMatrix[j][k][dp];
	       fprintf(st, "%10.4f", demand);

	       if(i && !(i % 6) && k!= NumberOfZones)
		  fprintf(st, "\n");
	       i++;

	   }
	   fprintf(st, "\n");
	 }
     }
      fclose(st);
   }else
   {
      char str_message[_MAX_PATH];
      sprintf(str_message, "Error in opening file demand.dat.");
      AfxMessageBox(str_message);
   }

m_AMSLogFile << " Deallocate3DDynamicArray.. " << endl;
   
   if(TDDemandSOVMatrix !=NULL)
		Deallocate3DDynamicArray<float>(TDDemandSOVMatrix,NumberOfZones+1, NumberOfZones+1);

 	if(TDDemandHOVMatrix !=NULL)
		Deallocate3DDynamicArray<float>(TDDemandHOVMatrix,NumberOfZones+1, NumberOfZones+1);

 	if(TDDemandTruckMatrix !=NULL)
		Deallocate3DDynamicArray<float>(TDDemandTruckMatrix,NumberOfZones+1, NumberOfZones+1);

m_AMSLogFile.close ();
}


void CTLiteDoc::WriteInputDemandMetaDataForDSP(CString DestinationProjectDirectory)
{
		FILE* st_meta_data = NULL;
		CString str;
		str.Format("%sinput_demand_file_list.csv",DestinationProjectDirectory);

		fopen_s(&st_meta_data,str,"w");


		if(st_meta_data!=NULL)
		{
			fprintf(st_meta_data,"scenario_no,file_sequence_no,file_name,format_type,number_of_lines_to_be_skipped,loading_multiplier,start_time_in_min,end_time_in_min,apply_additional_time_dependent_profile,subtotal_in_last_column,number_of_demand_types,demand_type_1,demand_type_2,demand_type_3,demand_type_4,'00:00,'00:15,'00:30,'00:45,'01:00,'01:15,'01:30,'01:45,'02:00,'02:15,'02:30,'02:45,'03:00,'03:15,'03:30,'03:45,'04:00,'04:15,'04:30,'04:45,'05:00,'05:15,'05:30,'05:45,'06:00,'06:15,'06:30,'06:45,'07:00,'07:15,'07:30,'07:45,'08:00,'08:15,'08:30,'08:45,'09:00,'09:15,'09:30,'09:45,'10:00,'10:15,'10:30,'10:45,'11:00,'11:15,'11:30,'11:45,'12:00,'12:15,'12:30,'12:45,'13:00,'13:15,'13:30,'13:45,'14:00,'14:15,'14:30,'14:45,'15:00,'15:15,'15:30,'15:45,'16:00,'16:15,'16:30,'16:45,'17:00,'17:15,'17:30,'17:45,'18:00,'18:15,'18:30,'18:45,'19:00,'19:15,'19:30,'19:45,'20:00,'20:15,'20:30,'20:45,'21:00,'21:15,'21:30,'21:45,'22:00,'22:15,'22:30,'22:45,'23:00,'23:15,'23:30,'23:45\n");
			fprintf(st_meta_data,"0,1,demand.dat,dynasmart,1,1,%d,%d,0,0,1,1\n",m_DemandLoadingStartTimeInMin,m_DemandLoadingEndTimeInMin);
			fprintf(st_meta_data,"0,1,demand_hov.dat,dynasmart,1,1,%d,%d,0,0,1,2\n",m_DemandLoadingStartTimeInMin,m_DemandLoadingEndTimeInMin);
			fprintf(st_meta_data,"0,1,demand_truck.dat,dynasmart,1,1,%d,%d,0,0,1,3\n",m_DemandLoadingStartTimeInMin,m_DemandLoadingEndTimeInMin);
			// m_DemandLoadingStartTimeInMin,m_DemandLoadingEndTimeInMin are read from the original project
			fclose(st_meta_data);
		}else
		{

		CString msg;
		msg.Format ("File %s cannot be opened. Please check.",str);
		AfxMessageBox(msg);
		
		}

} 
void CTLiteDoc::CreateVehicles(int origin_zone, int destination_zone, float number_of_vehicles, int demand_type, float starting_time_in_min, float ending_time_in_min)
{

	int time_interval = (starting_time_in_min - m_DemandLoadingStartTimeInMin)/15;
	int oz = m_ZoneMap [origin_zone].m_DSPZoneNo; 
	int dz = m_ZoneMap [destination_zone].m_DSPZoneNo;

	if(demand_type ==1)
		TDDemandSOVMatrix[oz][dz][time_interval] += number_of_vehicles;

	if(demand_type ==2)
		TDDemandHOVMatrix[oz][dz][time_interval] += number_of_vehicles;

	if(demand_type ==3)
		TDDemandTruckMatrix[oz][dz][time_interval] += number_of_vehicles;


}
void CTLiteDoc::OnFileTrafficdatasetting()
{	
	OpenCSVFileInExcel (m_ProjectDirectory + "AMS_traffic_data_settings.csv");
}

void CTLiteDoc::OnExportDynasmart()
{
	SaveDYNASMARTProject("Project.dws");

}

void CTLiteDoc::OnDemandUseroadcapacitytogeneratedefaultproductionandattractionforeachzone()
{
	// TODO: Add your command handler code here
}





