

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
#include "..//Geometry.h"
#include "..//TLite.h"
#include "..//CSVParser.h"
//#include "..//pugXML.h"

#include "..//Network.h"
#include "..//TLiteDoc.h"
#include "..//TLiteView.h"
#include "..//MainFrm.h"
#include <iostream>                          // for cout, endl
#include <fstream>                           // for ofstream
#include <sstream>
#include <vector>
#include <string>

using namespace std;


void CTLiteDoc::OnFileOpenNewRailDoc()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Rail Train Schedule (*.xml)|*.xml||"));
	if(dlg.DoModal() == IDOK)
	{
		OnOpenRailNetworkDocument(dlg.GetPathName(),false);
	}
}

BOOL CTLiteDoc::OnOpenRailNetworkDocument(CString ProjectFileName, bool bNetworkOnly)
{

	ClearNetworkData();
	CTime LoadingStartTime = CTime::GetCurrentTime();

	m_bLoadNetworkDataOnly = bNetworkOnly;
	FILE* st = NULL;

	CString directory;
	m_ProjectFile = ProjectFileName;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	m_ProjectDirectory = directory;
	m_ProjectTitle = GetWorkspaceTitleName(ProjectFileName);
	SetTitle(m_ProjectTitle);

	CWaitCursor wc;
	OpenWarningLogFile(directory);

	if(!ReadRailNodeCSVFile(directory+"input_node.csv")) return false;
	if(!ReadRailLinkCSVFile(directory+"input_link.csv",false,false))
		return false;

	CalculateDrawingRectangle();
	m_bFitNetworkInitialized  = false;

	ReadTrainInfoCSVFile(directory+"input_train_info.csv");
	ReadRailMOWCSVFile(directory+"input_MOW.csv");

	ReadRailTrainXMLFile(ProjectFileName);


	CTime LoadingEndTime = CTime::GetCurrentTime();

	CTimeSpan ts = LoadingEndTime  - LoadingStartTime;
	CString str_running_time;

	str_running_time.Format ("Network loading time: %d min(s) %d sec(s)...",ts.GetMinutes(), ts.GetSeconds());


	SetStatusText(str_running_time);
	UpdateAllViews(0);


	return true;
}

bool CTLiteDoc::ReadRailNodeCSVFile(LPCTSTR lpszFileName)
{

	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		int i=0;
		while(parser.ReadRecord())
		{
			int node_id = -1;
			string name;
			DTANode* pNode = 0;

			int control_type;
			double X;
			double Y;
			if(parser.GetValueByFieldName("node_id",node_id) == false)
				break;

			if(node_id < 0)
				break;

			if(!parser.GetValueByFieldName("name",name))
				name = "";

			control_type = 0;

			// use the X and Y as default values first
			parser.GetValueByFieldName("x",X);
			parser.GetValueByFieldName("y",Y);

			string geo_string;

			if(parser.GetValueByFieldName("geometry",geo_string))
			{
				// overwrite when the field "geometry" exists
				CGeometry geometry(geo_string);

				std::vector<CCoordinate> CoordinateVector = geometry.GetCoordinateList();

				if(CoordinateVector.size()>0)
				{
					X = CoordinateVector[0].X;
					Y = CoordinateVector[0].Y;
				}

			}

			if(m_NodeNumbertoNodeNoMap.find(node_id) != m_NodeNumbertoNodeNoMap.end())
			{
				CString error_message;
				error_message.Format ("Node %d in input_node.csv has been defined twice. Please check.", node_id);
				AfxMessageBox(error_message);
				return 0;
			}

			pNode = new DTANode;
			pNode->m_Name = name;
			pNode->m_ControlType = control_type;
			pNode->pt.x = X;
			pNode->pt.y = Y;

			parser.GetValueByFieldName("TSdiagram_x",X);
			parser.GetValueByFieldName("TSdiagram_y",Y);
			pNode->schedule_pt.x = X;
			pNode->schedule_pt.y = Y;

			pNode->m_NodeNumber = node_id;
			pNode->m_NodeNo = i;
			pNode->m_ZoneID = 0;
			m_NodeSet.push_back(pNode);
			m_NodeNoMap[i] = pNode;
			m_NodeNumberMap[node_id] = pNode;
			m_NodeNotoNumberMap[i] = node_id;
			m_NodeNumbertoNodeNoMap[node_id] = i;
			i++;


		}

		m_NodeDataLoadingStatus.Format ("%d nodes are loaded from file %s.",m_NodeSet.size(),lpszFileName);
		return true;
	}else
	{
		AfxMessageBox("Error: File input_rail_node.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
		//		g_ProgramStop();
	}

}

bool CTLiteDoc::ReadTrainInfoCSVFile(LPCTSTR lpszFileName)
{
	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		train_info element;
		while(parser.ReadRecord())
		{
			parser.GetValueByFieldName("train_header",element.train_header);
			parser.GetValueByFieldName("entry_time",element.entry_time);
			parser.GetValueByFieldName("origin_node_id",element.origin_node_id);
			parser.GetValueByFieldName("destination_node_id",element.destination_node_id);
			parser.GetValueByFieldName("direction",element.direction);
			parser.GetValueByFieldName("speed_multiplier",element.speed_multiplier);
			parser.GetValueByFieldName("train_length",element.train_length);
			parser.GetValueByFieldName("tob",element.tob);
			parser.GetValueByFieldName("hazmat",element.hazmat);
			parser.GetValueByFieldName("sa_status_at_origin",element.sa_status_at_origin);
			parser.GetValueByFieldName("terminal_want_time",element.terminal_want_time);

			m_train_map[element.train_header] = element;
		}

	}
	return true;
}

bool CTLiteDoc::ReadRailMOWCSVFile(LPCTSTR lpszFileName)
{
	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		MaintenanceOfWay element;
		while(parser.ReadRecord())
		{

			parser.GetValueByFieldName("A_node_id",element.from_node_id);
			parser.GetValueByFieldName("B_node_id",element.to_node_id);
			parser.GetValueByFieldName("start_time_in_min",element.start_time_in_min);
			parser.GetValueByFieldName("end_time_in_min",element.end_time_in_min);

			m_RailMOW_vector.push_back(element);
		}

	}
	return true;
}




bool CTLiteDoc::ReadRailLinkCSVFile(LPCTSTR lpszFileName, bool bCreateNewNodeFlag = false,  int LayerNo = 0)
{

	long i = 0;
	DTALink* pLink = 0;
	float default_distance_sum=0;
	float length_sum = 0;

	CString error_message;

	bool bTwoWayLinkFlag = false;
	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		bool bNodeNonExistError = false;
		while(parser.ReadRecord())
		{

			int link_id = 0;
			int from_node_id;
			int to_node_id;
			int bidirectional_flag = 1;
			double length_in_mile;
			float default_AB_speed_per_hour= 60;
			float default_BA_speed_per_hour= 60;

			string track_type = "";
			string name;

			if(!parser.GetValueByFieldName("name",name))
				name = "";

			if(!parser.GetValueByFieldName("from_node_id",from_node_id)) 
			{
				AfxMessageBox("Field A_node_id has not been defined in file input_rail_arc.csv. Please check.");
				break;
			}
			if(!parser.GetValueByFieldName("to_node_id",to_node_id))
			{
				AfxMessageBox("Field B_node_id has not been defined in file input_rail_arc.csv. Please check.");
				break;
			}

			if(m_NodeNumbertoNodeNoMap.find(from_node_id)== m_NodeNumbertoNodeNoMap.end())
			{
				if(bCreateNewNodeFlag == false)  // not create new node
				{
					m_NEXTALOGFile<< "A Node ID "  << from_node_id << " in input_rail_link.csv has not been defined in input_node.csv"  << endl; 
					bNodeNonExistError = true;
					break;
				}else  // need to create a new node with geometry information
				{
					string geo_string;
					std::vector<CCoordinate> CoordinateVector;
					if(parser.GetValueByFieldName("geometry",geo_string))
					{
						// overwrite when the field "geometry" exists
						CGeometry geometry(geo_string);
						CoordinateVector = geometry.GetCoordinateList();
						if(CoordinateVector.size()>=2)
						{
							GDPoint	pt; 
							pt.x = CoordinateVector[0].X;
							pt.y = CoordinateVector[0].Y;
							AddNewNode(pt, from_node_id, LayerNo);

						}else
						{

							error_message.Format ("Field geometry in Link %s has less than 2 feature points, coordinate information of upstream node cannot be extracted. Please check.",name.c_str ());
							AfxMessageBox(error_message);
							return false;
						}

					}else
					{
						AfxMessageBox("Field geometry cannot be found in input_rail_arc.csv. Please check.");
						return false;
					}

				}	
			}

			if(m_NodeNumbertoNodeNoMap.find(to_node_id)== m_NodeNumbertoNodeNoMap.end())
			{
				if(bCreateNewNodeFlag == false)  // not create new node
				{
					m_NEXTALOGFile<< "B Node ID "  << to_node_id << " in input_rail_arc.csv has not been defined in input_node.csv"  << endl; 
					bNodeNonExistError = true;
					break;
				}else  // create new node
				{
					string geo_string;
					std::vector<CCoordinate> CoordinateVector;
					if(parser.GetValueByFieldName("geometry",geo_string))
					{
						// overwrite when the field "geometry" exists
						CGeometry geometry(geo_string);
						CoordinateVector = geometry.GetCoordinateList();
						if(CoordinateVector.size()>=2)
						{
							GDPoint	pt; 
							pt.x = CoordinateVector[CoordinateVector.size()-1].X;
							pt.y = CoordinateVector[CoordinateVector.size()-1].Y;
							AddNewNode(pt, to_node_id, LayerNo);

						}else
						{
							error_message.Format ("Field geometry in Link %s has less than 2 feature points, coordinate information of upstream node cannot be extracted. Please check.",name.c_str ());
							AfxMessageBox(error_message);
							return false;
						}

					}else
					{
						AfxMessageBox("Field geometry cannot be found in input_rail_arc.csv. Please check.");
						return false;
					}


				}

			}

			DTALink* pExistingLink =  FindLinkWithNodeIDs(m_NodeNumbertoNodeNoMap[from_node_id],m_NodeNumbertoNodeNoMap[to_node_id]);

			if(pExistingLink)
			{
				continue;
			}


			if(!parser.GetValueByFieldName("length",length_in_mile))
			{
				AfxMessageBox("Field length_in_mile has not been defined in file input_rail_arc.csv. Please check.");
				break;
			}


			if(!parser.GetValueByFieldName("direction",bidirectional_flag))
				bidirectional_flag = 1;

			//if(!parser.GetValueByFieldName("default_AB_speed_per_hour",default_AB_speed_per_hour))
			//{
			//	error_message.Format ("Link %s: Field default_AB_speed_per_hour has not been defined in file input_rail_arc.csv. Please check.",name.c_str ());
			//	AfxMessageBox(error_message);
			//	break;
			//}

			//if(!parser.GetValueByFieldName("default_BA_speed_per_hour",default_BA_speed_per_hour))
			//{
			//	error_message.Format ("Link %s: Field default_BA_speed_per_hour has not been defined in file input_rail_arc.csv. Please check.",name.c_str ());
			//	AfxMessageBox(error_message);
			//	break;
			//}

			//if(!parser.GetValueByFieldName("track_type",track_type))
			//{
			//	AfxMessageBox("Field track_type has not been defined in file input_rail_arc.csv. Please check.");
			//	break;
			//}


			if(!parser.GetValueByFieldName("arc_id",link_id))  // no value
			{
				// mark it as 0 first, and we then find a new unique link id after reading all links
				link_id = 0;
			}
			string geo_string;

			bool bToBeShifted = true;
			std::vector<CCoordinate> CoordinateVector;
			if(parser.GetValueByFieldName("geometry",geo_string))
			{
				// overwrite when the field "geometry" exists
				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();
				m_bLinkToBeShifted = false;
				bToBeShifted = false;
			}else
			{
				// no geometry information
				CCoordinate cc_from, cc_to; 
				cc_from.X = m_NodeNoMap[m_NodeNumbertoNodeNoMap[from_node_id]]->pt.x;
				cc_from.Y = m_NodeNoMap[m_NodeNumbertoNodeNoMap[from_node_id]]->pt.y;

				cc_to.X = m_NodeNoMap[m_NodeNumbertoNodeNoMap[to_node_id]]->pt.x;
				cc_to.Y = m_NodeNoMap[m_NodeNumbertoNodeNoMap[to_node_id]]->pt.y;

				CoordinateVector.push_back(cc_from);
				CoordinateVector.push_back(cc_to);
			}

			bool bNodeNonExistError = false;
			int m_SimulationHorizon = 1;
			pLink = new DTALink(m_SimulationHorizon);
			pLink->m_LayerNo = LayerNo;
			pLink->m_LinkNo = i;
			pLink->m_Name  = name;
			pLink->m_RailBidirectionalFlag = bidirectional_flag;
			pLink->m_LinkID = link_id;

			pLink->m_FromNodeNumber = from_node_id;

			pLink->m_ToNodeNumber = to_node_id;
			pLink->m_Direction  = 1;

			pLink->m_FromNodeID = m_NodeNumbertoNodeNoMap[from_node_id];
			pLink->m_ToNodeID= m_NodeNumbertoNodeNoMap[to_node_id];

			int si;

			for(si = 0; si < CoordinateVector.size(); si++)
			{
				GDPoint	pt;
				pt.x = CoordinateVector[si].X;
				pt.y = CoordinateVector[si].Y;
				pLink->m_ShapePoints .push_back (pt);

			}

			pLink->m_SpeedLimit= max(1,default_AB_speed_per_hour);  // minimum speed limit is 1 mph
			pLink->m_ReversedSpeedLimit= max(1,default_BA_speed_per_hour);  // minimum speed limit is 1 mph

			pLink->m_Length= length_in_mile;
			pLink->m_FreeFlowTravelTime = pLink->m_Length/pLink->m_SpeedLimit*60.0f;  // convert from hour to min
			pLink->m_StaticTravelTime = pLink->m_FreeFlowTravelTime;

			pLink->m_NumberOfLanes = 1;
			pLink->m_TrackType = track_type;

			m_NodeNoMap[pLink->m_FromNodeID ]->m_Connections+=1;
			m_NodeNoMap[pLink->m_FromNodeID ]->m_OutgoingLinkVector.push_back(pLink->m_LinkNo);
			m_NodeNoMap[pLink->m_ToNodeID ]->m_Connections+=1;

			unsigned long LinkKey = GetLinkKey( pLink->m_FromNodeID, pLink->m_ToNodeID);

			m_NodeNotoLinkMap[LinkKey] = pLink;

			__int64  LinkKey2 = pLink-> m_FromNodeNumber* pLink->m_ToNodeNumber;
			m_NodeNumbertoLinkMap[LinkKey2] = pLink;


			m_LinkNotoLinkMap[i] = pLink;
			m_LinkIDtoLinkMap[i] = pLink;

			m_NodeNoMap[pLink->m_FromNodeID ]->m_TotalCapacity += (pLink->m_MaximumServiceFlowRatePHPL* pLink->m_NumberOfLanes);

			pLink->m_FromPoint = m_NodeNoMap[pLink->m_FromNodeID]->pt;
			pLink->m_ToPoint = m_NodeNoMap[pLink->m_ToNodeID]->pt;

			pLink->m_ScheduleFromPoint = m_NodeNoMap[pLink->m_FromNodeID]->schedule_pt;
			pLink->m_ScheduleToPoint = m_NodeNoMap[pLink->m_ToNodeID]->schedule_pt;

			default_distance_sum+= pLink->DefaultDistance();
			length_sum += pLink ->m_Length;
			if(!bNodeNonExistError)
			{
//				TRACE("\nAdd link no.%d,  %d -> %d",i,pLink->m_FromNodeNumber, pLink->m_ToNodeNumber );
				m_LinkSet.push_back (pLink);
				m_LinkNoMap[i]  = pLink;
				i++;
			}


		}

		if(bNodeNonExistError)
			AfxMessageBox("Some nodes in input_rail_arc.csv have not been defined in input_node.csv. Please check NeXTA.log in the project folder.");

		m_UnitMile  = 1.0f;

		if(length_sum>0.000001f)
			m_UnitMile= default_distance_sum / length_sum ;

		m_UnitFeet = m_UnitMile/5280.0f;  

		m_LongLatCoordinateFlag = false;

		m_LinkDataLoadingStatus.Format ("%d links are loaded from file %s.",m_LinkSet.size(),lpszFileName);

		AssignUniqueLinkIDForEachLink();

		return true;
	}else
	{
		AfxMessageBox("Error: File input_rail_arc.csv cannot be opened.\n It might be currently used and locked by EXCEL.");
		return false;
		//		g_ProgramStop();
	}

	

}



bool ParseDestinationString(string destinationString,float& entry_time_in_sec )
{
	string entryString = "entry=";

	int startIdx = destinationString.find(entryString);;
	if (startIdx != string::npos)
	{
		int s,e;
		s = destinationString.find("'",startIdx+entryString.length());
		if (s != string::npos)
		{
			e = destinationString.find("'",s + 1);
			if (e != string::npos)
			{
				string value = destinationString.substr(s+1,e - s - 1);
				entry_time_in_sec = atof(value.c_str());
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool ParseMovementString(string movementString,train_movement_arc& movement)
{
	string arcString = "arc=";
	string entryString = "entry=";
	string exitString = "exit=";

	int from_node_id;
	int to_node_id;
	float entry_time_in_sec;
	float exit_time_in_sec;

	int startIdx;
	startIdx = movementString.find(arcString);
	if (startIdx != string::npos)
	{
		int s,e;
		s = movementString.find("'",startIdx+arcString.length());
		if (s != string::npos)
		{
			e = movementString.find("'",s + 1);
			if (e != string::npos)
			{
				string value = movementString.substr(s,e-s);

				int idx1 = value.find("(");
				int idx2 = value.find(",");
				int idx3 = value.find(")");

				if (idx1 != string::npos && idx2 != string::npos && idx3 != string::npos)
				{
					string tmp1,tmp2;
					tmp1 = value.substr(idx1+1,idx2 - idx1 - 1);
					tmp2 = value.substr(idx2+1,idx3 - idx2 - 1);
					from_node_id = atoi(tmp1.c_str());
					to_node_id = atoi(tmp2.c_str());
				}

			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	//entry
	startIdx = movementString.find(entryString);
	if (startIdx != string::npos)
	{
		int s,e;
		s = movementString.find("'",startIdx+entryString.length());
		if (s != string::npos)
		{
			e = movementString.find("'",s + 1);
			if (e != string::npos)
			{
				string value = movementString.substr(s+1,e-s-1);
				entry_time_in_sec = (float)atof(value.c_str());				
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	//exit
	startIdx = movementString.find(exitString);
	if (startIdx != string::npos)
	{
		int s,e;
		s = movementString.find("'",startIdx+exitString.length());
		if (s != string::npos)
		{
			e = movementString.find("'",s + 1);
			if (e != string::npos)
			{
				string value = movementString.substr(s+1,e-s-1);
				exit_time_in_sec = (float)atof(value.c_str());
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	movement.from_node_id = from_node_id;
	movement.to_node_id = to_node_id;
	movement.entry_time_in_sec = entry_time_in_sec;
	movement.exit_time_in_sec = exit_time_in_sec;


	return true;
}

bool ParseTrainString(string trainString, train_schedule& tSchedule)
{
	int idx;
	int offset = 0;

	train_movements movement;

	movement.train_id = "";

	idx = trainString.find("id=");
	if (idx != string::npos)
	{
		int startIdx = trainString.find("'",idx);
		if (startIdx != string::npos)
		{
			int endIdx = trainString.find("'",startIdx+1);
			if (endIdx != string::npos)
			{
				string train_id = trainString.substr(startIdx + 1,endIdx - startIdx - 1);
				movement.train_id = train_id;
			}
		}

	}

	while((idx = trainString.find("<movement ",offset)) != string::npos)
	{
		int endIdx = trainString.find("/>",idx);
		if (endIdx != string::npos)
		{
			string movementString = trainString.substr(idx,endIdx + 2 - idx);
			train_movement_arc arc;
			if (ParseMovementString(movementString,arc))
			{
				movement.movement_arc_vector.push_back(arc);
			}

			offset = endIdx + 1;
		}
		else
		{
			return false;
		}

	}

	idx = trainString.find("<destination");
	if (idx != string::npos)
	{
		int endIdx = trainString.find("/>",idx);
		if (endIdx != string::npos)
		{
			string destinationString = trainString.substr(idx,endIdx + 2 - idx);
			float entry_time_in_sec;
			if (ParseDestinationString(destinationString,entry_time_in_sec))
			{
				movement.destination_entry_time_in_sec = entry_time_in_sec;
			}
			else
			{
				movement.destination_entry_time_in_sec = 0.f;
			}

		}
	}
	else
	{
		movement.destination_entry_time_in_sec = 0.f;
	}


	tSchedule.movement_vector.push_back(movement);
	return true;
}

bool ReadTrainsFromFile(CString fileName,std::vector<train_schedule>& ts_vector)
{
	ifstream in(fileName);
	string line,s;
	if (in.is_open())
	{
		while(getline(in,line))
		{
			s += line;
		}
	}

	in.close();

	int startIdx,endIdx;
	string subString = s;
	int count  = 0;
	while((startIdx = subString.find("<train ")) != string::npos && (endIdx = subString.find("</train>")) != string::npos)
	{

		string trainString = subString.substr(startIdx,endIdx + string("</train>").length() - startIdx);
		train_schedule tSchedule;

		if (ParseTrainString(trainString,tSchedule))
		{ 
			TRACE("trainString: %s", trainString.c_str ());
			ts_vector.push_back(tSchedule);
			count++;
		}

		subString = subString.substr(endIdx + string("</train>").length() + 1);
	}
	return true;

}


bool CTLiteDoc::ReadRailTrainXMLFile(CString FileName)
{
	// create timetable element for each link
	std::list<DTALink*>::iterator iLink;

	double max_length = 0;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		if((*iLink)->m_Length > max_length )
			max_length = (*iLink)->m_Length;
	}


	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		GDPoint pt_from_start, pt_from_end, pt_to_start, pt_to_end;

		double rotate_theta  = 0;

		double timetable_space_unit  = max_length / 2;
		double min_offset = timetable_space_unit;
		double max_offset = 6*timetable_space_unit;

		pt_from_start.x = (*iLink)->m_ScheduleFromPoint.x - min_offset* cos(rotate_theta-PI/2.0f);
		pt_from_start.y = (*iLink)->m_ScheduleFromPoint.y - min_offset* sin(rotate_theta-PI/2.0f);

		pt_from_end.x = (*iLink)->m_ScheduleFromPoint.x - max_offset* cos(rotate_theta-PI/2.0f);
		pt_from_end.y = (*iLink)->m_ScheduleFromPoint.y - max_offset* sin(rotate_theta-PI/2.0f);

		pt_to_start.x = (*iLink)->m_ScheduleToPoint.x - min_offset* cos(rotate_theta-PI/2.0f);
		pt_to_start.y = (*iLink)->m_ScheduleToPoint.y - min_offset* sin(rotate_theta-PI/2.0f);

		pt_to_end.x = (*iLink)->m_ScheduleToPoint.x - max_offset* cos(rotate_theta-PI/2.0f);
		pt_to_end.y = (*iLink)->m_ScheduleToPoint.y - max_offset* sin(rotate_theta-PI/2.0f);

		int start_time = 0;
		int end_time = 1440;

		std::vector<train_movement_arc> train_movement_vector;

		CLinkTimeTable timetable(pt_from_start, pt_from_end, pt_to_start, pt_to_end,start_time, end_time, train_movement_vector);

		(*iLink)->m_TimeTable = timetable;

	}

	//read train information

	ReadTrainsFromFile(FileName,m_train_schedule_vector);

	for (int i=0;i< m_train_schedule_vector.size();i++)
	{
		float current_time  = -100;


		for (int j=0;j< m_train_schedule_vector[i].movement_vector.size();j++)
		{
			train_movements mv = m_train_schedule_vector[i].movement_vector[j];

			
			bool bFindLink = false;
			for (int k=0;k< mv.movement_arc_vector.size();k++)
			{


				train_movement_arc arc = mv.movement_arc_vector[k];

				DTALink* pLink = FindLinkWithNodeNumbers(arc.from_node_id, arc.to_node_id);
				bFindLink = true;

				float arc_traveling_time = 100;

				// assignment arc to each link
				train_movement_arc element;
				element.train_id = mv.train_id ;
				element.arc_sequence_no = k;
				element.entry_time_in_sec = arc.entry_time_in_sec;

				train_info train;

				if(m_train_map.find(element.train_id) != m_train_map.end())
					train = m_train_map[element.train_id];

				element.exit_time_in_sec_with_train_length_time = arc.exit_time_in_sec;

				if(k==0 || k== mv.movement_arc_vector.size()-1)
				{   
					element.b_train_label = true;
				}

				double train_length_time = 0;

				if(pLink!=NULL)
				{
					element.direction = 1;

					float speed_multilier = train.speed_multiplier;
					if( pLink->m_TrackType.find("S") != string::npos || pLink->m_TrackType.find("SW")!= string::npos || pLink->m_TrackType.find("C")!= string::npos)
						speed_multilier = 1.0f; // constant speed on switch  


					arc_traveling_time=   pLink->m_Length *3600/ (max(1,pLink->m_SpeedLimit *speed_multilier));
					element.exit_time_in_sec = element.exit_time_in_sec_with_train_length_time - train_length_time;


					int speed_difference_delay =  0 ;
					
					if(pLink->m_TrackType.find_first_of ("0123456789") !=  std::string::npos) // main track
					{
						speed_difference_delay = (element.exit_time_in_sec - element.entry_time_in_sec ) - arc_traveling_time;
						m_train_map[mv.train_id ]. main_track_speed_difference_delay_in_second  += speed_difference_delay;
					}
					if(k==0)
					{   
						pLink->m_bTrainFromTerminal  = true;
					}

					if(k== mv.movement_arc_vector.size()-1)
					{   
						pLink->m_bTrainToTerminal  = true;

						m_train_map[mv.train_id ].terminal_arrival_time  =  max( m_train_map[mv.train_id ].terminal_arrival_time,element.exit_time_in_sec/60);
					
					}



					pLink->m_TimeTable .m_train_movement_vector .push_back (element);
				}else
				{  // reversed direction
					pLink = FindLinkWithNodeNumbers(arc.to_node_id,arc.from_node_id);

					if(pLink!=NULL)
					{
						element.direction = -1;
						float speed_multilier = train.speed_multiplier;
						if( pLink->m_TrackType.find("S") != string::npos || pLink->m_TrackType.find("SW")!= string::npos || pLink->m_TrackType.find("C")!= string::npos)
							speed_multilier = 1.0f; // constant speed on switch  

//						train_length_time=  train.train_length *3600/ (max(1,pLink->m_ReversedSpeedLimit *speed_multilier));
 					arc_traveling_time=   pLink->m_Length *3600/ (max(1,pLink->m_ReversedSpeedLimit *speed_multilier));
						train_length_time = 0;  // we do not consider train length
						element.exit_time_in_sec = element.exit_time_in_sec_with_train_length_time - train_length_time;


					int speed_difference_delay =  0 ;
					
					if(pLink->m_TrackType.find_first_of ("0123456789") !=  std::string::npos) // main track
					{
						speed_difference_delay = (element.exit_time_in_sec - element.entry_time_in_sec ) - arc_traveling_time;
						m_train_map[mv.train_id ]. main_track_speed_difference_delay_in_second  += speed_difference_delay;
					}
						// resersed direction
						if(k==0)
						{   
							pLink->m_bTrainToTerminal  = true;
							TRACE("train no.%d: %s\n", i,element.train_id.c_str ());
						}

						if(k== mv.movement_arc_vector.size()-1)
						{   
							pLink->m_bTrainFromTerminal  = true;
							m_train_map[mv.train_id ].terminal_arrival_time  =  max( m_train_map[mv.train_id ].terminal_arrival_time,element.exit_time_in_sec/60);

						}


						pLink->m_TimeTable .m_train_movement_vector .push_back (element);

					}else
					{

							CString msg;
							msg.Format ("arc (%d,%d) in file %s has not been defined in input_rail_arc.csv.\nPlease check",arc.from_node_id, arc.to_node_id, FileName);
							AfxMessageBox(msg);
							return false;
					
					}

				}
				if(current_time < arc.exit_time_in_sec )
					current_time = arc.exit_time_in_sec;

			}
		}

	}

	//
	return true;
}


bool CTLiteView::DrawLinkTimeTable(DTALink* pLink, CDC* pDC, int DisplayMode)
{
	char buff[100];
	pDC->SetTextColor(RGB(0,255,0));

	CPoint cpt_from_start = NPtoSP(pLink ->m_TimeTable.pt_from_start );
	CPoint cpt_from_end = NPtoSP(pLink ->m_TimeTable.pt_from_end );
	CPoint cpt_to_start = NPtoSP(pLink ->m_TimeTable.pt_to_start );
	CPoint cpt_to_end = NPtoSP(pLink ->m_TimeTable.pt_to_end );

	CPen PenRectColor(PS_SOLID,1,RGB(0,255,0));

	pDC->SelectObject(PenRectColor);

	// step 1: draw outside rectangle
	pDC->MoveTo(cpt_from_start); 
	pDC->LineTo(cpt_from_end); 
	pDC->LineTo(cpt_to_end); 
	pDC->LineTo(cpt_to_start); 
	pDC->LineTo(cpt_from_start); 

	// step 2: draw time grid
	int TimeXPosition;

	// step 4: draw time axis

	CPen PenTimeGridColor(PS_SOLID,1,RGB(0,255,0));
	pDC->SelectObject(PenTimeGridColor);
	pDC->SetTextColor(RGB(0, 0, 0));

	for(int t= pLink ->m_TimeTable.start_time_in_min ; t<= pLink ->m_TimeTable.end_time_in_min; t+=120)
	{
		GDPoint from_pt, to_pt;
		double ratio = (t - pLink ->m_TimeTable.start_time_in_min)*1.0 /  max(1,pLink ->m_TimeTable.end_time_in_min - pLink ->m_TimeTable.start_time_in_min);
		from_pt.x = pLink ->m_TimeTable.pt_from_start.x + ratio* (pLink ->m_TimeTable.pt_from_end.x - pLink ->m_TimeTable.pt_from_start.x);
		from_pt.y = pLink ->m_TimeTable.pt_from_start.y + ratio* (pLink ->m_TimeTable.pt_from_end.y - pLink ->m_TimeTable.pt_from_start.y);

		to_pt.x = pLink ->m_TimeTable.pt_to_start.x + ratio* (pLink ->m_TimeTable.pt_to_end.x - pLink ->m_TimeTable.pt_to_start.x);
		to_pt.y = pLink ->m_TimeTable.pt_to_start.y + ratio* (pLink ->m_TimeTable.pt_to_end.y - pLink ->m_TimeTable.pt_to_start.y);

		CPoint from_cpt =  NPtoSP(from_pt);
		CPoint to_cpt  =  NPtoSP(to_pt);

		pDC->MoveTo(from_cpt);
		pDC->LineTo(to_cpt);

		char buff[100];

		wsprintf(buff,"%2d:00",t/60);

		if(pLink -> m_bTrainFromTerminal)
		{
			pDC->TextOut(from_cpt.x-10,from_cpt.y-5,buff);
		}
		if(pLink -> m_bTrainToTerminal)
		{
			pDC->TextOut(to_cpt.x,to_cpt.y-5,buff);
		}

	}

	// draw MOW

	for(int m = 0 ; m < GetDocument()->m_RailMOW_vector.size(); m++)
	{
		CPoint RectPt[5];
		MaintenanceOfWay mow = GetDocument()->m_RailMOW_vector[m];
		if( (mow.from_node_id == pLink->m_FromNodeNumber && mow.to_node_id == pLink->m_ToNodeNumber)
			|| (mow.from_node_id == pLink->m_ToNodeNumber && mow.to_node_id == pLink->m_FromNodeNumber))
		{
			GDPoint from_pt, to_pt;
			double ratio = (mow.start_time_in_min - pLink ->m_TimeTable.start_time_in_min )*1.0 /  max(1,pLink ->m_TimeTable.end_time_in_min - pLink ->m_TimeTable.start_time_in_min);

			from_pt.x = pLink ->m_TimeTable.pt_from_start.x + ratio* (pLink ->m_TimeTable.pt_from_end.x - pLink ->m_TimeTable.pt_from_start.x);
			from_pt.y = pLink ->m_TimeTable.pt_from_start.y + ratio* (pLink ->m_TimeTable.pt_from_end.y - pLink ->m_TimeTable.pt_from_start.y);

			to_pt.x = pLink ->m_TimeTable.pt_to_start.x + ratio* (pLink ->m_TimeTable.pt_to_end.x - pLink ->m_TimeTable.pt_to_start.x);
			to_pt.y = pLink ->m_TimeTable.pt_to_start.y + ratio* (pLink ->m_TimeTable.pt_to_end.y - pLink ->m_TimeTable.pt_to_start.y);

			RectPt[0] =  NPtoSP(from_pt);
			RectPt[1]  =  NPtoSP(to_pt);

			ratio = (mow.end_time_in_min - pLink ->m_TimeTable.start_time_in_min)*1.0 /  max(1,pLink ->m_TimeTable.end_time_in_min - pLink ->m_TimeTable.start_time_in_min);

			from_pt.x = pLink ->m_TimeTable.pt_from_start.x + ratio* (pLink ->m_TimeTable.pt_from_end.x - pLink ->m_TimeTable.pt_from_start.x);
			from_pt.y = pLink ->m_TimeTable.pt_from_start.y + ratio* (pLink ->m_TimeTable.pt_from_end.y - pLink ->m_TimeTable.pt_from_start.y);

			to_pt.x = pLink ->m_TimeTable.pt_to_start.x + ratio* (pLink ->m_TimeTable.pt_to_end.x - pLink ->m_TimeTable.pt_to_start.x);
			to_pt.y = pLink ->m_TimeTable.pt_to_start.y + ratio* (pLink ->m_TimeTable.pt_to_end.y - pLink ->m_TimeTable.pt_to_start.y);

			RectPt[3] =  NPtoSP(from_pt);
			RectPt[2]  =  NPtoSP(to_pt);
			RectPt[4] = RectPt[0];

			CPen PenWOWColor(PS_SOLID,3,RGB(255,165,0));
			pDC->SelectObject(PenWOWColor);
			pDC->SetTextColor(RGB(255,165,0));
			pDC->SetBkMode(TRANSPARENT);
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->Polygon (RectPt,5);
			pDC->TextOut(RectPt[0].x,RectPt[0].y-20,"MOW");
		}

	}

	CPen PenTrainColor(PS_SOLID,1,RGB(0,0,0));
	CPen PenTrainColor2(PS_DOT,1,RGB(0,0,0));

	CPen PenSelectedTrain(PS_SOLID,3,RGB(255,0,0));



	int size_for_display_timestamp = 100*100;
	for(int train = 0; train < pLink->m_TimeTable .m_train_movement_vector.size(); train++)
	{


	if( pLink->m_TrackType.find("S") != string::npos
		|| pLink->m_TrackType.find("SW")!= string::npos
		|| pLink->m_TrackType.find("C")!= string::npos)
	{
		pDC->SelectObject(PenTrainColor2);
	}else
	{
		pDC->SelectObject(PenTrainColor);
	}


	pDC->SetTextColor(RGB(0, 0, 0));

		train_movement_arc element = pLink->m_TimeTable .m_train_movement_vector[train];

		if(element.entry_time_in_sec < 0 || element.exit_time_in_sec < 0)
			continue;

		if(GetDocument()->m_SelectedTrainHeader.compare (element.train_id )==0)
		{
		
		pDC->SelectObject(PenSelectedTrain);

		// draw select links
		FromPoint = NPtoSP(pLink->m_FromPoint);
		ToPoint = NPtoSP(pLink->m_ToPoint);

		pDC->MoveTo(FromPoint);
		pDC->LineTo(ToPoint);

		
		}


		GDPoint from_pt, to_pt;
		double ratio;
		int t ;
		CPoint from_cpt, to_cpt;
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
		bool m_bShowText = pMainFrame->m_bShowLayerMap[layer_movement];

		// draw train line
		if(element.direction ==1)
		{
			t = element.entry_time_in_sec / 60;
			ratio = (t - pLink ->m_TimeTable.start_time_in_min)*1.0 /  max(1,pLink ->m_TimeTable.end_time_in_min - pLink ->m_TimeTable.start_time_in_min);
			from_pt.x = pLink ->m_TimeTable.pt_from_start.x + ratio* (pLink ->m_TimeTable.pt_from_end.x - pLink ->m_TimeTable.pt_from_start.x);
			from_pt.y = pLink ->m_TimeTable.pt_from_start.y + ratio* (pLink ->m_TimeTable.pt_from_end.y - pLink ->m_TimeTable.pt_from_start.y);

			t = element.exit_time_in_sec  / 60;
			ratio = (t - pLink ->m_TimeTable.start_time_in_min)*1.0 /  max(1,pLink ->m_TimeTable.end_time_in_min - pLink ->m_TimeTable.start_time_in_min);
			to_pt.x = pLink ->m_TimeTable.pt_to_start.x + ratio* (pLink ->m_TimeTable.pt_to_end.x - pLink ->m_TimeTable.pt_to_start.x);
			to_pt.y = pLink ->m_TimeTable.pt_to_start.y + ratio* (pLink ->m_TimeTable.pt_to_end.y - pLink ->m_TimeTable.pt_to_start.y);

			from_cpt =  NPtoSP(from_pt);
			to_cpt  =  NPtoSP(to_pt);

			pDC->MoveTo(from_cpt);
			pDC->LineTo(to_cpt);

			CSize size = from_cpt - to_cpt;
			if(size.cx*size.cx+size.cy*size.cy >= size_for_display_timestamp && m_bShowText)
			{
				wsprintf(buff,"%dm %ds",(int)(element.entry_time_in_sec/60),(int)(element.entry_time_in_sec));
				pDC->TextOut(from_cpt.x,from_cpt.y-5,buff);
				wsprintf(buff,"%dm %ds",(int)(element.exit_time_in_sec/60),(int)(element.exit_time_in_sec));
				pDC->TextOut(to_cpt.x,to_cpt.y-5,buff);
			}

		}else // -1
		{
			// entry from the to node
			t = element.entry_time_in_sec / 60;
			ratio = (t - pLink ->m_TimeTable.start_time_in_min)*1.0 /  max(1,pLink ->m_TimeTable.end_time_in_min - pLink ->m_TimeTable.start_time_in_min);

			from_pt.x = pLink ->m_TimeTable.pt_to_start.x + ratio* (pLink ->m_TimeTable.pt_to_end.x - pLink ->m_TimeTable.pt_to_start.x);
			from_pt.y = pLink ->m_TimeTable.pt_to_start.y + ratio* (pLink ->m_TimeTable.pt_to_end.y - pLink ->m_TimeTable.pt_to_start.y);

			t = element.exit_time_in_sec  / 60;
			// exit from the from node
			ratio = (t - pLink ->m_TimeTable.start_time_in_min)*1.0 /  max(1,pLink ->m_TimeTable.end_time_in_min - pLink ->m_TimeTable.start_time_in_min);
			to_pt.x = pLink ->m_TimeTable.pt_from_start.x + ratio* (pLink ->m_TimeTable.pt_from_end.x - pLink ->m_TimeTable.pt_from_start.x);
			to_pt.y = pLink ->m_TimeTable.pt_from_start.y + ratio* (pLink ->m_TimeTable.pt_from_end.y - pLink ->m_TimeTable.pt_from_start.y);

			from_cpt =  NPtoSP(from_pt);
			to_cpt  =  NPtoSP(to_pt);

			pDC->MoveTo(from_cpt);
			pDC->LineTo(to_cpt);

			CSize size = from_cpt - to_cpt;
			if(size.cx*size.cx+size.cy*size.cy >= size_for_display_timestamp && m_bShowText)
			{
				wsprintf(buff,"%dm %ds",(int)(element.exit_time_in_sec/60),(int)(element.exit_time_in_sec));
				pDC->TextOut(from_cpt.x,from_cpt.y-5,buff);
				wsprintf(buff,"%dm %ds",(int)(element.entry_time_in_sec/60),(int)(element.entry_time_in_sec));
				pDC->TextOut(to_cpt.x,to_cpt.y-5,buff);
			}


		}

		if(element.b_train_label == true)
		{ 
			CPoint id_pt;
			id_pt.x = (from_cpt.x + to_cpt.x)/2;
			id_pt.y = (from_cpt.y + to_cpt.y)/2;

			pDC->SetTextColor(RGB(0, 0, 255));

			wsprintf(buff,"%s",element.train_id.c_str ());
			pDC->TextOut(id_pt.x,id_pt.y-5,buff);

		}
	}
	return true;
}

void CTLiteDoc::OnToolsCheckingfeasibility()
{
	CWaitCursor wait;
	CheckFeasibility();
}
bool CTLiteDoc::CheckFeasibility()
{

	int error_count = 0;

	ofstream feasibility_check_file;
	bool b_check_feasibility_flag = true;


	CString check_file_name;

	check_file_name.Format ("%s\\feasibility_checking.txt",m_ProjectDirectory);
	
	feasibility_check_file.open(check_file_name);

	if(feasibility_check_file.is_open ()==false)
	{

		AfxMessageBox("File feasibility_check.log cannot be opened.");
		return false;

	}


	for (int i=0;i<m_train_schedule_vector.size();i++)
	{
		float current_time  = -100;
		for (int j=0;j< m_train_schedule_vector[i].movement_vector.size();j++)
		{
			train_movements mv = m_train_schedule_vector[i].movement_vector[j];

			
			bool bFindLink = false;
			for (int k=0;k< mv.movement_arc_vector.size();k++)
			{


				train_movement_arc arc = mv.movement_arc_vector[k];

				DTALink* pLink = FindLinkWithNodeNumbers(arc.from_node_id, arc.to_node_id);
				bFindLink = true;

				float arc_traveling_time = 100;

				// assignment arc to each link
				train_movement_arc element;
				element.train_id = mv.train_id ;
				element.arc_sequence_no = k;
				element.entry_time_in_sec = arc.entry_time_in_sec;

				train_info train;

				if(m_train_map.find(element.train_id) != m_train_map.end())
					train = m_train_map[element.train_id];

				element.exit_time_in_sec_with_train_length_time = arc.exit_time_in_sec;

				if(k==0 || k== mv.movement_arc_vector.size()-1)
				{   
					element.b_train_label = true;
				}

				double train_length_time = 0;

				if(arc.entry_time_in_sec>=0 && current_time-1 > arc.entry_time_in_sec )
				{

					CString msg;
					msg.Format("Train %s: <movement arc='(%d,%d)' entry='%5.1f' exit='%5.1f'/>: Previous exit time %5.1f > entry time %5.1f.\n",train.train_header.c_str () ,
						arc.from_node_id, arc.to_node_id,element.entry_time_in_sec,element.exit_time_in_sec,current_time,element.entry_time_in_sec);
					feasibility_check_file << msg;
					b_check_feasibility_flag = false;
				}



				if(pLink!=NULL)
				{
					element.direction = 1;

					float speed_multilier = train.speed_multiplier;
					if( pLink->m_TrackType.find("S") != string::npos || pLink->m_TrackType.find("SW")!= string::npos || pLink->m_TrackType.find("C")!= string::npos)
						speed_multilier = 1.0f; // constant speed on switch  


					arc_traveling_time=   pLink->m_Length *3600/ (max(1,pLink->m_SpeedLimit *speed_multilier));
					element.exit_time_in_sec = element.exit_time_in_sec_with_train_length_time - train_length_time;

					if(element.exit_time_in_sec >=0 &&(element.exit_time_in_sec - (element.entry_time_in_sec + arc_traveling_time))<-1)
					{
						CString msg;
						msg.Format("Train %s: <movement arc='(%d,%d)' entry='%5.1f' exit='%5.1f'/> does not satisfy arc travel time requirement: %5.1f seconds\n.",train.train_header.c_str () ,
							arc.from_node_id, arc.to_node_id,element.entry_time_in_sec ,element.exit_time_in_sec,arc_traveling_time);
						feasibility_check_file << msg;
						b_check_feasibility_flag = false;

					}

					if(k==0)
					{   
						pLink->m_bTrainFromTerminal  = true;
					}

					if(k== mv.movement_arc_vector.size()-1)
					{   
						pLink->m_bTrainToTerminal  = true;
					}

				}else
				{  // reversed direction
					pLink = FindLinkWithNodeNumbers(arc.to_node_id,arc.from_node_id);

					if(pLink!=NULL)
					{
						element.direction = -1;
						float speed_multilier = train.speed_multiplier;
						if( pLink->m_TrackType.find("S") != string::npos || pLink->m_TrackType.find("SW")!= string::npos || pLink->m_TrackType.find("C")!= string::npos)
							speed_multilier = 1.0f; // constant speed on switch  

//						train_length_time=  train.train_length *3600/ (max(1,pLink->m_ReversedSpeedLimit *speed_multilier));
 					arc_traveling_time=   pLink->m_Length *3600/ (max(1,pLink->m_ReversedSpeedLimit *speed_multilier));
						train_length_time = 0;  // we do not consider train length
						element.exit_time_in_sec = element.exit_time_in_sec_with_train_length_time - train_length_time;

						if(element.exit_time_in_sec - (element.entry_time_in_sec + arc_traveling_time) <-1)
						{
							if(element.exit_time_in_sec>=0)
							{

							CString msg;
							msg.Format(" Train %s: <movement arc='(%d,%d)' entry='%5.1f' exit='%5.1f'/> does not satisfy arc travel time requirement: %5.1f seconds.\n",train.train_header.c_str () ,
								arc.from_node_id, arc.to_node_id,element.entry_time_in_sec ,element.exit_time_in_sec,arc_traveling_time);
							feasibility_check_file << msg;
							b_check_feasibility_flag = false;

							}

						}


						// resersed direction
						if(k==0)
						{   
							pLink->m_bTrainToTerminal  = true;
						}

						if(k== mv.movement_arc_vector.size()-1)
						{   
							pLink->m_bTrainFromTerminal  = true;
						}



					}else
					{

							CString msg;
							msg.Format ("arc (%d,%d) has not been defined in input_rail_arc.csv.\nPlease check",arc.from_node_id, arc.to_node_id);
							AfxMessageBox(msg);
							b_check_feasibility_flag = false;

					
					}

				}
				if(current_time < arc.exit_time_in_sec )
					current_time = arc.exit_time_in_sec;

			}
		}

	}


	int safety_headway  = 5*60-1; // 5*60 -1 seconds
	std::list<DTALink*>::iterator iLink;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		DTALink* pLink = (*iLink);

		std::map<int,CString > TrackUsageMap;

		// check MOW
		for(int m = 0 ; m <m_RailMOW_vector.size(); m++)
		{
			CPoint RectPt[5];
			MaintenanceOfWay mow = m_RailMOW_vector[m];
			if( (mow.from_node_id == pLink->m_FromNodeNumber && mow.to_node_id == pLink->m_ToNodeNumber)
				|| (mow.from_node_id == pLink->m_ToNodeNumber && mow.to_node_id == pLink->m_FromNodeNumber))
			{

				for(int time = mow.start_time_in_min*60+1; time <  mow.end_time_in_min*60-1; time ++)
				{
					TrackUsageMap[time] = "MOW";  // -1 as a mark for MOW
				}
			}

		}

		for(int train = 0; train < pLink->m_TimeTable .m_train_movement_vector.size(); train++)
		{
			train_movement_arc element = pLink->m_TimeTable .m_train_movement_vector[train];

			int entry_time_in_sec = element.entry_time_in_sec ;
			int exit_time_in_sec = element.exit_time_in_sec  ;

			if(entry_time_in_sec > exit_time_in_sec)
			{
				swap(entry_time_in_sec,exit_time_in_sec);
			}

			for(int time = entry_time_in_sec; time < exit_time_in_sec; time++)
			{
				int time_in_a_day  = time;
				if(time_in_a_day < 0)
					time_in_a_day+=1440*60;

				if(time_in_a_day > 1440*60)  // in second
					time_in_a_day-=1440*60;

				if(TrackUsageMap.find(time_in_a_day)!= TrackUsageMap.end())  // this time stamp has been used before
				{
					b_check_feasibility_flag = false;
					if(TrackUsageMap[time_in_a_day].Compare  ("MOW")== 0)
					{
						feasibility_check_file << "Trains cannot occupy tracks inside the MOW window during the duration of the MOW. Conflict beween train " << element.train_id << " and MOW " << " at time " << (float) (time_in_a_day/60.0) << " (unit: min) " << time_in_a_day << " (unit: sec) at track " <<  pLink->m_FromNodeNumber  << " -> " <<  pLink->m_ToNodeNumber << "." << endl;
						break;
					}

					if( TrackUsageMap[time_in_a_day].Find ("headway")!= -1 )
					{
						feasibility_check_file << "Headway constraints: when an arc is traversed by a train, there must be at least 5 minutes of separation. " << " Conflict beween train " << element.train_id << " and " << TrackUsageMap[time_in_a_day]  << " at time " << (float)(time_in_a_day/60.0) << " (unit: min) " << time_in_a_day << " (unit: sec) at track " <<  pLink->m_FromNodeNumber  << " -> " <<  pLink->m_ToNodeNumber << "." << endl;
						break;
					}else if(TrackUsageMap[time_in_a_day].Find ("train")!= -1)
					{  // no headway substring here
						feasibility_check_file << "Nonconcurrency constraints: No more than 1 train can occupy an arc at any given time.  " 
							<< " Conflict beween train " << element.train_id << " and  " << TrackUsageMap[time_in_a_day] << " at time " << float(time_in_a_day/60.0) << " (unit: min) " << time_in_a_day << " (unit: sec)  at track " <<  pLink->m_FromNodeNumber  << " -> " <<  pLink->m_ToNodeNumber << "." << endl;
						break;
					}


				}
			}
			
			for(int time = entry_time_in_sec-(5*60-1); time < exit_time_in_sec+(5*60-1); time+=10)  //every 10 seconds
			{

					CString str;
					if(entry_time_in_sec>=0 &&  exit_time_in_sec>=0 && time >= entry_time_in_sec && time < exit_time_in_sec) // we only check feasibility conditions for non-negative values, so we can output -1 value as default values to test routes
					{
						str.Format("train %s",element.train_id.c_str ());
					}else
					{
						str.Format("headway of train %s",element.train_id.c_str ());					
					}

					if(time<0)
						time+=1400*60;

					if(time>1400*60)
						time-=1400*60;

					TrackUsageMap[time] = str;
			}

		}

		}  // for each train in a link

		for (int i=0;i<m_train_schedule_vector.size();i++)
	{
		for (int j=0;j< m_train_schedule_vector[i].movement_vector.size();j++)
		{
			train_movements mv = m_train_schedule_vector[i].movement_vector[j];
			for (int k=0;k< mv.movement_arc_vector.size();k++)
			{
				train_movement_arc arc = mv.movement_arc_vector[k];

//				TRACE("direction: %d\n",arc.direction );
			}

		}
	}
	feasibility_check_file.close();

	if(b_check_feasibility_flag == false)
	{
		CString msg;
		msg.Format("The train schedule does not pass feasibility check. Please check log file %s",check_file_name);
		if(AfxMessageBox(msg))
		{
		HINSTANCE result = ShellExecute(NULL, _T("open"),check_file_name , NULL,NULL, SW_SHOW);;	
		}
	}else
	{
	   AfxMessageBox("This train schedule passes preliminary feasibility check.\nTo check a complete set of constraint requirements, please follow the document released by the 2012 RAS Problem Solving Competition organizing committee.");
	}
	return 0;
}