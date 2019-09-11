// Transit.h :
//
//  Portions Copyright 2012 Shuguang Li (xalxlsg@gmail.com)

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

#include <vector>
#include <algorithm>

// data field << =   https://developers.google.com/transit/gtfs/reference#stop_times_fields
class PT_Stop
{

public:
	PT_Stop()
	{
	mapped_node_id = -1;
	stop_id = -1;
	number_of_nearby_transfer_stations = 0;
	}

	int number_of_nearby_transfer_stations;
	int stop_id;  // node id
	int stop_code; 
	string stop_name;  // node name
	string stop_desc;
	int zone_id;
	int location_type;
	string direction;
	string position;  //string
    GDPoint m_ShapePoint; //x,y

	int mapped_node_id;

};

class PT_StopTime
{
public:
	PT_StopTime()
	{
	stop_id = -1;
	number_of_nearby_transfer_stop_time = 0;
	}
	int trip_id;
	int arrival_time;  
	int departure_time;
	int stop_id;
	int stop_sequence;
	int stop_headsign;
	int pickup_type;
	int drop_off_type;
	float shape_dist_traveled;
	int timepoint;

	int number_of_nearby_transfer_stop_time;


	GDPoint pt;

};

class PT_Trip
{
public:
	int trip_id;
	int route_id;
	int shape_id;	
	string service_id;
	int direction_id;
	int block_id;

	int trip_type;

	 std::vector<PT_StopTime> m_PT_StopTimeVector;

	 std::vector<GDPoint> m_ShapePoints;
	 std::map<int,int > m_ShapeIDMap;

	 std::vector<int > m_PathNodeVector;



};



class PT_shape_feature_point
{
 public:
   int shape_id;
   float shape_pt_lat;
   float shape_pt_lon;
   int  shape_pt_sequence;

};
class PT_shapes
{
 public:
   int shape_id;
   std::vector<GDPoint> feature_point_vector;
};

class PT_transfers
{
 public:
   int from_stop_id;
   int to_stop_id;
   int transfer_type;
};

class PT_Route 
{
 public: 
	 PT_Route()
	 {
	 bInsideFreewayNetwork = false;
	 }
	int route_id;
	string route_short_name;
	string route_long_name;
	string route_type;
	string route_url;
	bool bInsideFreewayNetwork;

	//std::vector<PT_Trip> m_PT_TripVector;
	std::map<int, GDPoint> m_RouteBusStopShapePoints;  //bus stop list on one route
	std::map<int, GDPoint> m_RouteShapePoints;         //route line shape point, according to the sequence

};



class PT_Network
{
private:

    void BuildTransitNetwork();  //Build new transit network for short transit path calculation

    //short transit path algorithm, node time label: node cost label
	bool OptimalTDLabelCorrecting_transit(int origin, int departure_time, int destination); 

	//find optimal transit path ,stop list,and time according to short path tree.
	bool FindOptimalSolution(int origin, int departure_time, int destination,int PathNodeList[MAX_NODE_SIZE_IN_A_PATH]);

public:

	CString m_ProjectDirectory;
    int TransitOrigin;
	int TRansitDestination;
 

	std::map<int, PT_Route> m_PT_RouteMap;
	std::map<int, PT_Trip> m_PT_TripMap;
	std::map<int, PT_Stop> m_PT_StopMap;  // transit node layer

  	//bus stop schedule
	std::vector<PT_StopTime> m_PT_StopTimeVector;
    std::vector<PT_transfers> m_PT_transfers;
    std::map<int,PT_shapes>  m_PT_shapes_map;   
   
    //display route line, bus stop and short bus route
    //output bus data
    bool ReadGTFFiles(GDRect network_rect);  // Google Transit files

};

