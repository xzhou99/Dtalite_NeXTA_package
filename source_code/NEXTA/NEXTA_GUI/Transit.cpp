// Transit.cpp : Implementation file
//
//  Portions Copyright 2012 Shuguang Li (xalxlsg@gmail.com), Xuesong Zhou (zhou99@gmail.com)

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
#pragma once

#include "stdafx.h"
#include "TLite.h"
#include "Network.h"
#include "Transit.h"
#include "Geometry.h"
#include "CSVParser.h"
#include <vector>
#include <algorithm>
#include "TLiteDoc.h"
#include "TLiteView.h"
#include "MainFrm.h"
bool PT_Network::ReadGTFFiles(GDRect network_rect)  // Google Transit files
{

	//	// step 1: read  route files
	string str0 = m_ProjectDirectory +"routes.txt";
	//string str =  "h:/routes.csv";

	CT2CA pszConvertedAnsiString (str0.c_str());
	// construct a std::string using the LPCSTR input
	std::string  strStd (pszConvertedAnsiString);
	CCSVParser parser;

	if (parser.OpenCSVFile(strStd))
	{

		//	AfxMessageBox("Start reading Google Transit Feed files...", MB_ICONINFORMATION);

		int count =0;
		PT_Route route;
		while(parser.ReadRecord())
		{


			if(parser.GetValueByFieldName("route_id",route.route_id ) == false)
				break;
			if(parser.GetValueByFieldName("route_long_name",route.route_long_name) == false)
				route.route_long_name="";
			if(parser.GetValueByFieldName("route_short_name",route.route_short_name) == false)
				route.route_short_name="";			
			if(parser.GetValueByFieldName("route_url",route.route_url) == false)
				route.route_url="";			
			if(parser.GetValueByFieldName("route_type",route.route_type) == false)
				route.route_type="";		

			// make sure there is not duplicated key
			// If the requested key is not found, find() returns the end iterator for
			//the container, so:

			if(m_PT_RouteMap.find(route.route_id)  == m_PT_RouteMap.end() )
			{ 
				m_PT_RouteMap[route.route_id] = route;  
			}
			else
			{
				AfxMessageBox("Duplicated Route ID!");
			}


			count++;

		}
		parser.CloseCSVFile ();

		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

		pMainFrame->m_bShowLayerMap[layer_transit] = true;

	}else
	{
		return false;
	}

	CString missing_stops_message;
	// step 1: read stop information
	string str2 = m_ProjectDirectory +"stops.txt";


	CT2CA pszConvertedAnsiString2 (str2.c_str());
	// construct a std::string using the LPCSTR input
	std::string  strStd2 (pszConvertedAnsiString2);
	if (parser.OpenCSVFile(strStd2))
	{
		int count =0;
		while(parser.ReadRecord())
		{
			PT_Stop stop;

			if(parser.GetValueByFieldName("stop_id",stop.stop_id ) == false)
				break;


			bool NonnegativeFlag = false;
			if(parser.GetValueByFieldName("stop_lat",stop.m_ShapePoint.y, NonnegativeFlag) == false)
				break;

			if(parser.GetValueByFieldName("stop_lon",stop.m_ShapePoint.x , NonnegativeFlag) == false)
				break;

			if(parser.GetValueByFieldName("direction",stop.direction  ) == false)
				stop.direction="";

			if(parser.GetValueByFieldName("location_type",stop.location_type) == false)
				stop.location_type=0;

			if(parser.GetValueByFieldName("position",stop.position  ) == false)
				stop.position="";

			if(parser.GetValueByFieldName("stop_code",stop.stop_code ) == false)
				stop.stop_code=0;

			parser.GetValueByFieldName("stop_desc",stop.stop_desc);


			if(parser.GetValueByFieldName("stop_name",stop.stop_name) == false)
				stop.stop_name="";

			if(parser.GetValueByFieldName("zone_id",stop.zone_id) == false)
				stop.zone_id=0;


			if(m_PT_StopMap.find(stop.stop_id) == m_PT_StopMap.end())
			{
				m_PT_StopMap[stop.stop_id] = stop;

			}else
			{   CString msg;
			msg.Format("Duplicated Stop ID %d", stop.stop_id);

			AfxMessageBox(msg);
			}

			count++;

		}
		parser.CloseCSVFile ();




	}


	//read trip file
	string str3 = m_ProjectDirectory +"trips.txt";
	CT2CA pszConvertedAnsiString3 (str3.c_str());
	// construct a std::string using the LPCSTR input
	std::string  strStd3 (pszConvertedAnsiString3);
	if (parser.OpenCSVFile(strStd3))
	{
		int count =0;
		while(parser.ReadRecord())
		{
			PT_Trip trip;

			if(parser.GetValueByFieldName("trip_id",trip.trip_id) == false)
				break;

			if(parser.GetValueByFieldName("route_id",trip.route_id) == false)
				break;			

			if(parser.GetValueByFieldName("service_id",trip.service_id) == false)
				break;			

			if(parser.GetValueByFieldName("block_id",trip.block_id) == false)
				trip.block_id=0;			

			if(parser.GetValueByFieldName("direction_id",trip.direction_id) == false)
				trip.direction_id=0;

			if(parser.GetValueByFieldName("shape_id",trip.shape_id) == false)
				trip.shape_id=0;

			if(parser.GetValueByFieldName("trip_type",trip.trip_type) == false)
				trip.trip_type=0;	 	

			if(m_PT_TripMap.find(trip.trip_id) == m_PT_TripMap.end())
			{
				m_PT_TripMap[trip.trip_id] = trip;
			}else
			{
				AfxMessageBox("Duplicated Trip ID!");
			}
			count++;
		}
		parser.CloseCSVFile ();
	}



	// read stop_times.txt
	int stop_times_count =0;
	int max_stop_times_record  = 1000;

	string str4 = m_ProjectDirectory +"stop_times.txt";
	CT2CA pszConvertedAnsiString4 (str4.c_str());
	// construct a std::string using the LPCSTR input
	std::string  strStd4 (pszConvertedAnsiString4);
	if (parser.OpenCSVFile(strStd4))
	{

		while(parser.ReadRecord())
		{
			PT_StopTime TransitStopTime;

			if (parser.GetValueByFieldName("trip_id", TransitStopTime.trip_id) == false)
				break;

			TRACE("trip_id: %d\n", TransitStopTime.trip_id);

			if(parser.GetValueByFieldName("stop_id",TransitStopTime.stop_id) == false)
				break;	

			string time_string;
			char char_array[20];
			if(parser.GetValueByFieldName("arrival_time",time_string) == false)
				break;

			int hour,min,second;
			sprintf(char_array,"%s",time_string.c_str ());
			sscanf(char_array,"%d:%d:%d", &hour,&min,&second);

			TransitStopTime.arrival_time = hour*60+min;

			if(parser.GetValueByFieldName("departure_time",time_string) == false)
				break;	

			sprintf(char_array,"%s",time_string.c_str ());
			sscanf(char_array,"%d:%d:%d", &hour,&min,&second);
			TransitStopTime.departure_time =  hour*60+min;




/*			if(parser.GetValueByFieldName("stop_sequence",TransitStopTime.stop_sequence) == false)
				break; 	*/		

			/*
			if(parser.GetValueByFieldName("drop_off_type",TransitStopTime.drop_off_type) == false)
			TransitStopTime.drop_off_type=0; 

			if(parser.GetValueByFieldName("pickup_type",TransitStopTime.pickup_type) == false)
			TransitStopTime.pickup_type=0;

			if(parser.GetValueByFieldName("shape_dist_traveled",TransitStopTime.shape_dist_traveled) == false)
			TransitStopTime.shape_dist_traveled=0;	

			if(parser.GetValueByFieldName("stop_headsign",TransitStopTime.stop_headsign) == false)
			TransitStopTime.stop_headsign=0;	

			if(parser.GetValueByFieldName("timepoint",TransitStopTime.timepoint) == false)
			TransitStopTime.timepoint=0;*/	

			if(m_PT_StopMap.find(TransitStopTime.stop_id)!= m_PT_StopMap.end())
			{
				TransitStopTime.pt.x = m_PT_StopMap[TransitStopTime.stop_id].m_ShapePoint .x ;
				TransitStopTime.pt.y = m_PT_StopMap[TransitStopTime.stop_id].m_ShapePoint .y ;

			}else
			{
				if(missing_stops_message.GetLength ()<1000)
				{
					CString msg;
					msg.Format ("Mising stop %d\n",TransitStopTime.stop_id);
					missing_stops_message+=msg;

				}

			}

		//	m_PT_StopTimeVector.push_back(TransitStopTime) ;
			stop_times_count++;

			//if(stop_times_count >=max_stop_times_record)  // for testing purposes
			//	break;

				m_PT_TripMap[TransitStopTime.trip_id].m_PT_StopTimeVector .push_back(TransitStopTime);

		}
		parser.CloseCSVFile ();

	}

	CString message;

	int trip_size = m_PT_TripMap.size();
	int stop_size = m_PT_StopMap.size();
	int stop_time_size = stop_times_count;
	message.Format("%d transit trips, %d stops and %d stop time records are loaded.", trip_size, stop_size, stop_time_size);

	AfxMessageBox(message, MB_ICONINFORMATION);

	if(missing_stops_message.GetLength ()>0)
	{
		AfxMessageBox(missing_stops_message);	
	}

	/*

	//read transfer file
	string str6 = m_ProjectDirectory +"transfers.txt";
	CT2CA pszConvertedAnsiString6 (str6.c_str());
	// construct a std::string using the LPCSTR input
	std::string  strStd6 (pszConvertedAnsiString6);
	if (parser.OpenCSVFile(strStd6))
	{
	int count =0;
	while(parser.ReadRecord())
	{
	PT_transfers transfers;

	if(parser.GetValueByFieldName("from_stop_id",transfers.from_stop_id) == false)
	break;	

	if(parser.GetValueByFieldName("to_stop_id",transfers.to_stop_id) == false)
	break; 			

	if(parser.GetValueByFieldName("transfer_type",transfers.transfer_type) == false)
	break;

	m_PT_transfers.push_back(transfers) ;
	count++;
	}
	parser.CloseCSVFile ();
	}

	*/

	// map matching



	return true;
}

bool CTLiteDoc::ReadTransitFiles(CString TransitDataProjectFolder)
{

	m_PT_network.m_ProjectDirectory = TransitDataProjectFolder;
	m_PT_network.ReadGTFFiles(m_NetworkRect);  // google transit feed files


	m_TransitDataLoadingStatus.Format ("%d transit trips have been loaded.",m_PT_network.m_PT_TripMap.size() );

	/*	if( m_PT_network.m_PT_TripMap.size()>0 && AfxMessageBox("Do you want to generate bus trip data?",MB_YESNO|MB_ICONINFORMATION)==IDYES)
	{
	TransitTripMatching();
	}
	*/
	return true;
}


void CTLiteDoc::OnTransitGeneratetransitspace()
{
	CWaitCursor ws;

	FILE* st;
	fopen_s(&st, m_PT_network.m_ProjectDirectory + "transit_network.tnp", "w");
	fprintf(st, "transit network created by NEXTA");
	fclose(st);
	
	fopen_s(&st, m_PT_network.m_ProjectDirectory + "input_node.csv", "w");
	if (st == NULL)
	{
		AfxMessageBox("Error: File input_transit_node.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return;
	}
	std::map<int, int> stop_id_map;

	if (st != NULL)
	{

		fprintf(st, "name,node_id,x,y,geometry\n");

		std::map<int, PT_Stop>::iterator iPT_StopMap;
		for (iPT_StopMap = m_PT_network.m_PT_StopMap.begin(); iPT_StopMap != m_PT_network.m_PT_StopMap.end(); iPT_StopMap++)
		{
			fprintf(st, "%s,%d,%f,%f,\"<Point><coordinates>%f,%f</coordinates></Point>\"\n",
				(*iPT_StopMap).second.stop_name.c_str(),
				(*iPT_StopMap).second.stop_id,
				(*iPT_StopMap).second.m_ShapePoint.x,
				(*iPT_StopMap).second.m_ShapePoint.y,
				(*iPT_StopMap).second.m_ShapePoint.x,
				(*iPT_StopMap).second.m_ShapePoint.y
				);

			stop_id_map[(*iPT_StopMap).second.stop_id] = stop_id_map.size();
		}
		fclose(st);
	}

	fopen_s(&st, m_PT_network.m_ProjectDirectory + "input_link.csv", "w");
	if (st == NULL)
	{
		AfxMessageBox("Error: File input_link.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return;
	}

	std::map<CString, int> TransitLinkMap;

	if (st != NULL)
	{

		fprintf(st, "name,from_node_id,to_node_id,direction,length,number_of_lanes,speed_limit,lane_cap,link_type,demand_type_code\n");

		int count = 0;
		std::map<int, int> RouteMap;

		// step 1: for each trip
		std::map<int, PT_Trip>::iterator iPT_TripMap;
		for (iPT_TripMap = m_PT_network.m_PT_TripMap.begin(); iPT_TripMap != m_PT_network.m_PT_TripMap.end(); iPT_TripMap++)
	{

			// step 2: for each stop time pair
			if ((*iPT_TripMap).second.m_PT_StopTimeVector.size() >= 2)
			{
			
			for (int i = 0; i < (*iPT_TripMap).second.m_PT_StopTimeVector.size() - 1; i++)
			{
				int stopid_1 = (*iPT_TripMap).second.m_PT_StopTimeVector[i].stop_id;
				int stopid_2 = (*iPT_TripMap).second.m_PT_StopTimeVector[i + 1].stop_id;


				if (stop_id_map.find(stopid_1) != stop_id_map.end() && stop_id_map.find(stopid_2) != stop_id_map.end())
				{

					CString transit_link_key;
					transit_link_key.Format("%d->%d", stopid_1, stopid_2);

					if (TransitLinkMap.find(transit_link_key) == TransitLinkMap.end())  //find nothing
					{ //not defined yet
						// add a new link
						//	fprintf(st, "name,from_node_id,to_node_id,direction,length,number_of_lanes,speed_limit,lane_cap,link_type,demand_type_code");

						float link_length = g_CalculateP2PDistanceInMileFromLatitudeLongitude((*iPT_TripMap).second.m_PT_StopTimeVector[i].pt, 
							(*iPT_TripMap).second.m_PT_StopTimeVector[i+1].pt);
						fprintf(st, "%d,%d,%d,1,%.4f,1,40,1000,1,\n",
							(*iPT_TripMap).second.trip_id,
							stopid_1,
							stopid_2,
							link_length);

						//define this link
						TransitLinkMap[transit_link_key] = TransitLinkMap.size();
					}//else do nothing, do not add link
				}
				else
				{
					TRACE("Missing transit link, stop id not defined!");
				}
			}
			}

			}  // for each trip
		fclose(st);
	}

	//
	fopen_s(&st, m_PT_network.m_ProjectDirectory + "input_transit_trip.csv", "w");
	if (st == NULL)
	{
		AfxMessageBox("Error: File input_transit_trip.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return;
	}

	if (st != NULL)
	{

		fprintf(st, "day_no,agent_id,agent_type,trip_id,route_id,route_id_short_name,origin_zone,destination_zone,schedule_departure_time,schedule_node_sequence,schedule_stop_time_sequence,path_node_sequence,path_time_sequence\n");

		int count = 0;
		std::map<int, int> RouteMap;

		int agent_id = 1;

		// step 1: for each trip
		std::map<int, PT_Trip>::iterator iPT_TripMap;
		for (iPT_TripMap = m_PT_network.m_PT_TripMap.begin(); iPT_TripMap != m_PT_network.m_PT_TripMap.end(); iPT_TripMap++)
		{
			fprintf(st, "0,%d,transit,%d,%d,%s,%d,%d,",
				agent_id,
				(*iPT_TripMap).second.trip_id,
				(*iPT_TripMap).second.route_id,
				(*iPT_TripMap).second.service_id.c_str(),
				1,
				2)
				;

			agent_id++;

			// step 2: for each stop-time node
			if ((*iPT_TripMap).second.m_PT_StopTimeVector.size() >= 2)
			{
				fprintf(st, "%d,", (*iPT_TripMap).second.m_PT_StopTimeVector[0].departure_time);

				for (int i = 0; i < (*iPT_TripMap).second.m_PT_StopTimeVector.size(); i++)
				{
					fprintf(st, "%d;", (*iPT_TripMap).second.m_PT_StopTimeVector[i].stop_id);
				}
				fprintf(st, ",");
			}

			// step 2: for each stop-time time data
			if ((*iPT_TripMap).second.m_PT_StopTimeVector.size() >= 2)
			{
				for (int i = 0; i < (*iPT_TripMap).second.m_PT_StopTimeVector.size(); i++)
				{
					fprintf(st, "%d;%d;", 
						(*iPT_TripMap).second.m_PT_StopTimeVector[i].arrival_time,
						(*iPT_TripMap).second.m_PT_StopTimeVector[i].departure_time);
				}
				fprintf(st, ",");
			}

			// step 3:path_node_sequence

			if ((*iPT_TripMap).second.m_PT_StopTimeVector.size() >= 2)
			{
				for (int i = 0; i < (*iPT_TripMap).second.m_PT_StopTimeVector.size(); i++)
				{
					fprintf(st, "%d;", (*iPT_TripMap).second.m_PT_StopTimeVector[i].stop_id);
				}
				fprintf(st, ",");
			}

			// step 4: for each stop-time time data to generate path time seqeunce 
			if ((*iPT_TripMap).second.m_PT_StopTimeVector.size() >= 2)
			{
				for (int i = 0; i < (*iPT_TripMap).second.m_PT_StopTimeVector.size(); i++)
				{
					fprintf(st, "%d;%d;",
						(*iPT_TripMap).second.m_PT_StopTimeVector[i].arrival_time,
						(*iPT_TripMap).second.m_PT_StopTimeVector[i].departure_time);
				}
				fprintf(st, ",");
			}

			fprintf(st, "\n");
		}
		fclose(st);
	}

}


bool CTLiteDoc::TransitTripMatching()
{

	FILE* st;
	fopen_s(&st,m_PT_network.m_ProjectDirectory +"input_transit_trip.csv","w");
	if(st==NULL)
	{
		AfxMessageBox("Error: File input_transit_trip.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
	}

	if(st!=NULL)
	{

		fprintf(st,"route_id,trip_id,link_sequence_no,from_node_id,to_node_id,street_name,stop_id,stop_sequence,arrival_time,departure_time\n");


		std::list<DTALink*>::iterator iLink;

		// pass 0: initialization 
		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  // for each link in this network
		{
			(*iLink)-> m_OriginalLength = (*iLink)->m_Length;

		}

		int count = 0;
		std::map<int, int> RouteMap;

		// step 2: assemble data
		std::map<int, PT_Trip>::iterator iPT_TripMap;
		for ( iPT_TripMap= m_PT_network.m_PT_TripMap.begin() ; iPT_TripMap != m_PT_network.m_PT_TripMap.end(); iPT_TripMap++ )
		{
			if(m_pNetwork ==NULL)  // we only build the network once
			{
				m_pNetwork = new DTANetworkForSP(m_NodeSet.size(), m_LinkSet.size(), 1, 1, m_AdjLinkSize);  //  network instance for single processor in multi-thread environment
			}

			count++;
			if(count>=50)
				break;

			int stop_time_size  = (*iPT_TripMap).second .m_PT_StopTimeVector.size();
			if(stop_time_size>=2)
			{
				//find OD nodes for each vehicle
				m_OriginNodeID = FindClosestNode((*iPT_TripMap).second .m_PT_StopTimeVector[0].pt);
				m_DestinationNodeID = FindClosestNode((*iPT_TripMap).second .m_PT_StopTimeVector[stop_time_size-1].pt);
				// set new length for links

				if(m_OriginNodeID==-1 || m_DestinationNodeID==-1)
					continue;

				// pass 0: initialization 
				for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  // for each link in this network
				{
					(*iLink)->m_StopTimeRecord.stop_id = -1;
					(*iLink)->m_Length = 999999;

				}
				int max_link_id = 1;


				// first pass: link distance

				for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  // for each link in this network
				{

					////					if( m_LinkTypeMap[(*iLink)->m_link_type].IsTransit())
					//					{
					//
					//						(*iLink)->m_Length = 999999; // // not using light rail link
					//
					//					}else
					{
						double min_p_to_link_distance = 999999;

						GDPoint pfrom = (*iLink)->m_FromPoint;
						GDPoint pto = (*iLink)->m_ToPoint;

						GDPoint mid_point;
						mid_point.x = (pfrom.x + pto.x)/2;
						mid_point.y = (pfrom.y + pto.y)/2;


						int Intersection_Count  = 0;



						for(int i = 0; i < stop_time_size; i++ )
						{

							GDPoint pt0 = (*iPT_TripMap).second .m_PT_StopTimeVector[i].pt;
							GDPoint pt1 = (*iPT_TripMap).second .m_PT_StopTimeVector[min(i+1,stop_time_size-1)].pt;
							GDPoint pt2 = (*iPT_TripMap).second .m_PT_StopTimeVector[min(i+2,stop_time_size-1)].pt;


							float distance;

							distance = (g_GetPoint2Point_Distance(pfrom, pt0) +  g_GetPoint2Point_Distance(mid_point, pt1) +  g_GetPoint2Point_Distance(pto, pt2))/3;
							if(distance < min_p_to_link_distance)
							{
								min_p_to_link_distance = distance;
							}

						} 
						// determine the distance to GPS traces as length

						double dis_in_feet = min_p_to_link_distance /m_UnitFeet;

						(*iLink)->m_Length = dis_in_feet; // keep the original link length
					}
			 }


				m_RandomRoutingCoefficient  = 0;  // no random cost
				//build physical network
				m_pNetwork->BuildPhysicalNetwork(&m_NodeSet, &m_LinkSet, m_RandomRoutingCoefficient, false);
				int NodeNodeSum = 0;
				int PathLinkList[MAX_NODE_SIZE_IN_A_PATH];//save link ids in a path(a path means the trajactory of a vehicle obtained from GPS)
				float TotalCost;
				bool distance_flag = true;
				int  NodeSize;
				//get the total number of the nodes in the network we build,and give the link ids to PathLinkList
				NodeSize= m_pNetwork->SimplifiedTDLabelCorrecting_DoubleQueue(m_OriginNodeID, 0, m_DestinationNodeID, 1, 10.0f,PathLinkList,TotalCost, distance_flag, false, false,m_RandomRoutingCoefficient);   // Pointer to previous node (node)    
				//update m_PathDisplayList
				if(NodeSize <= 1)
				{
					TRACE("error");
				}

				NodeSize = min(NodeSize, MAX_NODE_SIZE_IN_A_PATH);

				std::vector<int> link_vector;// a vector to save link ids
				for (int i=0 ; i < NodeSize-1; i++)
				{
					link_vector.push_back (PathLinkList[i]);// save the link ids
					DTALink* pLink = m_LinkNotoLinkMap[PathLinkList[i]];
					if(pLink!=NULL)
					{ 
						//if(i==0)
						//	(*iPT_TripMap).second .m_PathNodeVector.push_back (pLink->m_FromNodeID );
						//
						//(*iPT_TripMap).second .m_PathNodeVector.push_back (pLink->m_ToNodeID );

						fprintf(st,"%d,%d,%d,%d,%d,%s,",(*iPT_TripMap).second.route_id, (*iPT_TripMap).second.trip_id ,
							i,pLink->m_FromNodeNumber , pLink->m_ToNodeNumber, pLink->m_Name.c_str ());

						for(int s_i = 0; s_i < stop_time_size; s_i++ )
						{
							GDPoint pfrom = pLink->m_FromPoint;
							GDPoint pto = pLink->m_ToPoint;

							float distance = g_GetPoint2LineDistance((*iPT_TripMap).second .m_PT_StopTimeVector[s_i].pt, pfrom, pto,m_UnitMile);  // go through each GPS location point

							if(distance < 90)  // with intersection
							{
								PT_StopTime element = (*iPT_TripMap).second .m_PT_StopTimeVector[s_i];
								fprintf(st,"%d,%d,%d,%d,", element.stop_id, element.stop_sequence ,element.arrival_time , element.departure_time);
								break;
							}

						}
						fprintf(st,"\n");

					}

				} // for all links

			}
		}  // for each transit trip


		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  // for each link in this network
		{
			(*iLink)->m_Length = (*iLink)-> m_OriginalLength;
		}

		fclose(st);
	}
	return true;
}
