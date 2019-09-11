#pragma once

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <list> 
#include <omp.h>
#include <algorithm>
#include <time.h>
#include "CSVParser.h"
#include <functional>
#include<stdio.h>   
// some basic parameters setting

//Pls make sure the g_AgentLite_number_of_k_path > Agentlite.cpp's g_number_of_K_paths+g_reassignment_number_of_K_paths and the _MAX_ZONE remain the same with .cpp's defination
#define _MAX_LABEL_COST 99999


extern int g_AgentLite_number_of_max_zones;

#define _MAX_DEMANDTYPES 2 //because of the od demand store format,the MAX_demandtype must >=g_DEMANDTYPES.size()+1
#define MAX_INFO_CLASS_SIZE 7
#define MAX_VEHICLE_TYPE_SIZE 15
#define _MAX_TIMEPERIODS 1


// Linear congruential generator 
#define LCG_a 17364
#define LCG_c 0
#define LCG_M 65521  // it should be 2^32, but we use a small 16-bit number to save memory
#define _MAX_STRING_LINE 30000

#define _TIME_INTERVAL_IN_MIN 15 
#define _MAX_TIME_INTERVAL 96 

extern FILE* g_pFileDebugLog;
extern FILE* g_pFileOutputLog;
extern long g_precision_constant;


extern void g_ProgramStop_agentlite();

struct MyException : public exception {
	const char * what() const throw () {
		return "C++ Exception";
	}
};

enum e_traffic_flow_model { tfm_BPR = 0, tfm_point_queue, tfm_newells_model, tfm_spatial_queue, trm_car_following };

class Demand_Parameters {
public:
	Demand_Parameters()
	{
		for (int i = 0;i < g_AgentLite_number_of_max_zones;i++)
		{
			alpharatio[i] = 1.0;
			gammaratio[i] = 1.0;
		}
		
	}
	std::map<int, float> alpharatio;
	std::map<int, float> gammaratio;
};

class Assignment {
public:
	Assignment()
	{
		g_number_of_threads = 8;
		g_number_of_K_paths = 4;
		g_reassignment_number_of_K_paths = 2;
		b_debug_detail_flag = 0;
		g_DemandGlobalMultiplier = 1.0f;
		g_number_of_links = 0;
		g_number_of_nodes = 0;

	}
	void Assignment_Init();
	int g_number_of_threads;
	int g_number_of_K_paths;
	int g_reassignment_number_of_K_paths;

	int b_debug_detail_flag;
	std::map<int, int> g_internal_node_seq_no_map;  // hush table, map external node number to internal node sequence no. 
	std::map<int, int> g_zoneid_to_zone_seq_no_mapping;
	float*** g_demand_array;


	//StatisticOutput.cpp
	float total_demand_volume;
	//NetworkReadInput.cpp and ShortestPath.cpp
	std::map<int, float> g_VOT_PerDemandType_MAP;
	//ShortestPath.cpp
	//Utility.cpp
	std::vector<int> g_DemandTypeVector;
	float total_demand[_MAX_DEMANDTYPES];
	float g_DemandGlobalMultiplier;

	
	int g_number_of_links;
	int g_number_of_nodes;

};

class CAgent_event_item {
public:
	int agent_id;
	int agent_next_link_seq_no; // next link's downstream node number
	float event_time_stamp;
};

class CAgentListPerTimeInterval
{
public:
	std::vector<int> m_agent_id_vector;

};


class MergeIncomingLink
{
public:
	MergeIncomingLink()
	{
		m_LinkInCapacityRatio = 0;
		m_LinkInRemainingCapaityPerSimuInterval = 0;
	};
	int m_LinkNo;
	int m_link_type;
	int m_OutflowNumLanes;
	float m_LinkInCapacityRatio;
	int m_LinkInRemainingCapaityPerSimuInterval;  // derived from other incoming  demand
};


class SLinkMOE  // time-dependent link MOE
{
public:
	float UserDefinedTravelTime_in_min;
	float TotalTravelTime;   // cumulative travel time for vehicles departing at this time interval
	int TotalFlowCount;


	//int CumulativeArrivalCount_DemandType[MAX_DEMAND_TYPE_SIZE];
	//float CumulativeRevenue_DemandType[MAX_DEMAND_TYPE_SIZE];

	//int CumulativeArrivalCount_VehicleType[MAX_VEHICLE_TYPE_SIZE];

	int CumulativeArrivalCount;
	int CumulativeDepartureCount;
	int ExitQueueLength;
	int LeftExitQueueLength;

	int IntervalLeftArrivalCount;
	int IntervalLeftDepartureCount;


	int EndTimeOfPartialCongestion;  // time in min to the end of partial congestion
	int TrafficStateCode;  // 0: free-flow: 1: partial congested: 2: fully congested

	int LoadingBuffer_QueueLength;
	int LoadingBuffer_EndTimeOfCongestion;  // time in min to the end of partial congestion
	int LoadingBuffer_TrafficStateCode;  // 0: free-flow: 1: partial congested: 2: fully congested

										 //   Density can be derived from CumulativeArrivalCount and CumulativeDepartureCount
										 //   Flow can be derived from CumulativeDepartureCount
										 //   AvgTravel time can be derived from CumulativeArrivalCount and TotalTravelTime

	SLinkMOE()
	{

		TotalTravelTime = 0;
		TotalFlowCount = 0;
		CumulativeArrivalCount = 0;
		CumulativeDepartureCount = 0;
		ExitQueueLength = 0;
		LeftExitQueueLength = 0;

		IntervalLeftArrivalCount = 0;
		IntervalLeftDepartureCount = 0;

		EndTimeOfPartialCongestion = 0;
		TrafficStateCode = 0;  // free-flow
	};


	void SetupMOE(float FreeflowTravelTime)
	{

		TotalTravelTime = 0;
		TotalFlowCount = 0;
		CumulativeArrivalCount = 0;
		CumulativeDepartureCount = 0;
		ExitQueueLength = 0;
		LeftExitQueueLength = 0;

		EndTimeOfPartialCongestion = 0;
		TrafficStateCode = 0;  // free-flow

	}

};

class CLink
{
public:
	CLink()  // construction 
	{
		cost = 0;
		BRP_alpha = 0.15f;
		BRP_beta = 4.0f;
		link_capacity = 1000;
		free_flow_travel_time_in_min = 1;
		flow_volume = 0;
		link_spatial_capacity = 100;

		CFlowArrivalCount = 0;
		CFlowDepartureCount = 0;

		LinkOutCapacity = 0;
		LinkLeftOutCapacity = 0;
		LinkInCapacity = 0;
		m_LeftTurn_NumberOfLanes = 0;
		m_LeftTurn_link_seq_no = -1;
		m_OutflowNumLanes = 1;
		m_ObsVolume = 0;
		m_ObsVolumeError = 0;

		//for (int tau = 0; tau < _MAX_TIME_INTERVAL; tau++)  // tau is the time interval index
		//{
		//	m_total_volume_vector.push_back (0);
		//	m_avg_travel_time.push_back(0);
		//}

	}

	~CLink()
	{
		//if (flow_volume_for_each_o != NULL)
		//	delete flow_volume_for_each_o;
	}

	void free_memory()
	{
	}

	void AddAgentsToLinkVolume()
	{

	    
	}


	
	void tally_flow_volume_across_all_processors();
	void tally_flow_volume_across_all_processors_second(int iteration_no);

	float get_time_dependent_link_travel_time(int node_time_stamp_in_min)
	{ 
		int time_interval_no = node_time_stamp_in_min / 15;

		if (time_interval_no < m_avg_travel_time.size())
			return m_avg_travel_time[time_interval_no];
		else
			return cost;
 	
	}

	std::vector<int> m_total_volume_vector;
	std::vector<float> m_avg_travel_time;

	// 1. based on BPR. 


	int m_LeftTurn_link_seq_no;

	int m_RandomSeed;
	int link_seq_no;
	int from_node_seq_no;
	int to_node_seq_no;
	float cost;

	float fftt;
	float free_flow_travel_time_in_min;
	int type;
	float link_capacity;
	float link_spatial_capacity;

	float flow_volume;

	string demand_type_code;
	float travel_time;
	float BRP_alpha;
	float BRP_beta;
	float length;

	float m_ObsVolume;
	float m_ObsVolumeError;

	std::vector <SLinkMOE> m_LinkMOEAry;
	//beginning of simulation data 

	//toll related link
	//int m_TollSize;
	//Toll *pTollVector;  // not using SLT here to avoid issues with OpenMP
	std::map<int,float> TollMAP;

	std::map<int, CAgentListPerTimeInterval> m_time_dependent_agent_list_per_min;  // simulation time in min
	std::list<CAgent_event_item> LoadingBuffer;  //loading buffer of each link, to prevent grid lock
	std::list<CAgent_event_item> EntranceQueue, LeftEntrance_Queue;  //link-in queue  of each link
	std::list<CAgent_event_item> ExitQueue, LeftExitQueue;      // link-out queue of each link

	int CFlowArrivalCount; 
	int CFlowDepartureCount;

	int m_bMergeFlag;  // 1: freeway and freeway merge, 2: freeway and ramp merge
	std::vector<MergeIncomingLink> MergeIncomingLinkVector;
	int m_MergeOnrampLinkID;
	int m_MergeMainlineLinkID;

	int LinkOutCapacity;  // unit: number of vehiles
	int LinkLeftOutCapacity;  // unit: number of vehiles
	int LinkInCapacity;   // unit: number of vehiles
	
	int m_OutflowNumLanes;
	int m_LeftTurn_NumberOfLanes;

	float m_CumulativeOutCapacityCount;
	int m_CumulativeOutCapacityCountAtPreviousInterval;
	int m_CumulativeInCapacityCountAtPreviousInterval;

	float m_CumulativeLeftOutCapacityCount;
	float m_CumulativeMergeOutCapacityCount;
	float m_CumulativeInCapacityCount;
	
	float m_JamTimeStamp;


	void Reset()
	{
			

		m_RandomSeed = link_seq_no;

			m_CumulativeOutCapacityCount = 0;
			m_CumulativeOutCapacityCountAtPreviousInterval = 0;
			m_CumulativeInCapacityCountAtPreviousInterval = 0;
			m_CumulativeLeftOutCapacityCount = 0;
			m_CumulativeMergeOutCapacityCount = 0;
			m_CumulativeInCapacityCount = 0;

			m_time_dependent_agent_list_per_min.clear();
			LoadingBuffer.clear();  //loading buffer of each link, to prevent grid lock
			EntranceQueue.clear();
			LeftEntrance_Queue.clear();  //link-in queue  of each link
			ExitQueue.clear();
			LeftExitQueue.clear();      // link-out queue of each link

	}

	float GetNumberOfLanes(int DayNo = 0, double Time = 0, bool OutputFlowFlag = false, bool bConsiderIncident = true)  // with lane closure
	{

		int NumLanes = m_OutflowNumLanes;

		//for (unsigned int il = 0; il< CapacityReductionVector.size(); il++)
		//{
		//	if ((CapacityReductionVector[il].StartDayNo <= DayNo && DayNo <= CapacityReductionVector[il].EndDayNo) && (Time >= CapacityReductionVector[il].StartTime && Time <= CapacityReductionVector[il].EndTime))
		//	{
		//		return (1 - CapacityReductionVector[il].LaneClosureRatio)*NumLanes;
		//	}
		//}

		return (float)NumLanes;

	}

	void statistics_collection(int time_stamp_in_min);
	/// end of simulation data


	// signal data

	bool m_bSignalizedArterialType;

	int m_current_plan_no;
	int m_number_of_plans;

	int m_plan_startime_in_sec[20];
	int m_plan_endtime_in_sec[20];
	int m_to_node_cycle_in_sec[20];
	int m_to_node_offset_in_sec[20];

	int number_of_timing_plans;
	float m_SaturationFlowRate_In_vhc_per_hour_per_lane[20];
	int m_GreenStartTime_In_Second[20];
	int m_GreenEndTime_In_Second[20];
	float m_LeftTurn_SaturationFlowRate_In_vhc_per_hour_per_lane[20];
	int m_LeftTurnGreenStartTime_In_Second[20];
	int m_LeftTurnGreenEndTime_In_Second[20];
	void UpdateCellVehiclePosition(int current_time_interval, int cell_no, int agent_id);

	void SetupCell(int number_of_cells)
	{
		for (int i = 0; i < number_of_cells; i++)
		{
			m_cell_current_t.push_back(-1);
			m_cell_prev_t_1.push_back(-1);

		}
	}
	void CopyCell()
	{
		// copy cell states
		for (int c = 0; c < m_cell_current_t.size(); c++)
		{
			m_cell_prev_t_1[c] = m_cell_current_t[c];
			m_cell_current_t[c] = -1;

		}

	}

	float GetTimeDependentCapacityAtSignalizedIntersection(
		double current_timeInMin,
		int &current_plan_no,
		int plan_startime_in_sec[20],
		int plan_endtime_in_sec[20],
		int to_node_cycle_in_sec[20],
		int to_node_offset_in_sec[20],
		float SaturationFlowRate[20],
		int GreenStartTime_in_second[20],
		int GreenEndTime_in_second[20],
		float simulation_time_interval_in_second);
	//
	void CalculateBPRFunction()
	{
		travel_time = free_flow_travel_time_in_min*(1 + BRP_alpha*pow(flow_volume / max(0.00001, link_capacity), BRP_beta));
		//travel_time = free_flow_travel_time_in_min+ BRP_alpha*pow(flow_volume / max(0.00001, link_capacity), BRP_beta);//for SF gams
		cost = travel_time;
	}
	       
	float get_VOC_ratio()
	{
		return flow_volume / max(0.00001, link_capacity);

	}

	float get_speed()
	{
		return length / max(travel_time, 0.0001) * 60;  // per hour
	}

	float GetRandomRatio()
	{
		m_RandomSeed = (LCG_a * m_RandomSeed + LCG_c) % LCG_M;  //m_RandomSeed is automatically updated.

		return float(m_RandomSeed) / LCG_M;
	}
	std::vector<int> m_cell_current_t;
	std::vector<int> m_cell_prev_t_1;

};

extern std::map<int, CLink*> g_pLink_map;  // link seq no

class CNode
{
public:
	CNode()
	{
		zone_id = -1;
		accessible_node_count = 0;
	}

	int accessible_node_count;

	int node_seq_no;  // sequence number 
	int node_id;      //external node number 
	int zone_id=-1;
	double x;
	double y;

	std::vector<CLink> m_outgoing_link_vector;
	std::vector<int> m_incoming_link_seq_no_vector;

};

class CarFollowingData
{

public:
	int link_seq_no;
	int cell_no;


};
//class Toll
//{
//public:
//	int StartDayNo;
//	int EndDayNo;
//	float StartTime;
//	float EndTime;
//	float TollRate[_MAX_DEMANDTYPES];  // 4 is 3_+1 , as pricing 
//
//	Toll()
//	{
//		for (int vt = 1; vt<_MAX_DEMANDTYPES; vt++)
//			TollRate[vt] = 0;
//	}
//};
class CAgent
{
public:
	CAgent()
	{
		Reset();

	}

	void Reset()
	{
		PCE_factor = 1.0;
		path_cost = 0;
		m_random_seed = 1;

		m_bLoaded = false;
		m_SimLinkSequenceNoInPath = -1;

		m_ArrivalTime = 0;
		m_TripTime = 0;
		m_TripFFTT = 0;
		m_BufferWaitingTime = 0;
		m_TollDollarCost = 0;
		Energy = 0;
		CO2 = 0;
		NOX = 0;
		CO = 0;
		HC = 0;
		m_bComplete = false;
		m_Delay = 0;

		within_link_driving_distance = 0;
		m_TimeToRetrieveInfo = 99999;
	}
	void setup_random_seed(unsigned int value)
	{
		m_random_seed = value;
	}


	float get_random_ratio()
	{
		m_random_seed = (LCG_a * m_random_seed + LCG_c) % LCG_M;  //m_RandomSeed is automatically updated.

		return float(m_random_seed) / LCG_M;
	}
	int agent_id;
	unsigned int m_random_seed;
	float m_TimeToRetrieveInfo;
	float m_LeavingTimeFromLoadingBuffer;

	bool m_bComplete;
	float m_TollDollarCost;

	float within_link_driving_distance;

	int origin_node_seq_no;
	int destination_node_seq_no;
	float departure_time_in_min;

	float m_Distance;

	float m_ArrivalTime;
	float m_TripTime;
	float m_TripFFTT;
	float m_BufferWaitingTime;
	float m_TravelTime;
	float m_EstimatedTravelTime;
	float m_Delay;

	float Energy, CO2, NOX, CO, HC, PM, PM2_5;


	float PCE_factor;  // passenger car equivalent : bus = 3
	float path_cost;

	std::vector<int> path_link_seq_no_vector;
	std::vector<int> path_node_seq_no_vector; 
	std::vector<int> path_node_timestamp_vector;  // in min

	// for simulation 
	bool m_bLoaded;
	int m_SimLinkSequenceNoInPath;

	std::vector<CarFollowingData> m_CarFollowingDataVector;

};

extern std::vector<CNode> g_node_vector;
extern std::vector<CLink> g_link_vector;

class CNode2NodeAccessibility
{
public:
	int from_node_no;
	int to_node_no;
	float travel_cost;
};

class COZone
{
public:
	void COZone_Init();
	~COZone();

	int zone_seq_no;  // 0, 1, 
	int zone_id;  // external zone id // this is origin zone
	int node_seq_no;

	std::vector<int> m_memory_index_vector;  // containing the threads belong to this zone based on thread to zone mapping 

	float*** destination_OD_volume;  // to do: read OD volume // to do: read OD volume per OD demand type, e.g. truck, passenger car, and buses
	float**** destination_k_path_cost;  // _MAX_ZONE is for destination zone  //NOTES: (s) first K is iteration number, second one is the tree index
	float**** destination_k_path_distance;
	float**** destination_k_path_ODVolume_experienced_cost;  //NOTES: sum over s of 1/k*d*c(s)
	float**** destination_k_path_ODVolume_tree_distance;  //NOTES: sum over s of 1/k*d*d(s)
	float**** destination_k_path_pi_ODVolume_cost;  //NOTES:  d*pie
	float**** pie_used_in_relative_gap_function;

	float* destination_k_path_prob_forreassignment_ODVolume_experienced_cost;
	float* destination_k_path_prob;



	//NOTES: s is the index of sp tree at iteration k

	//beginning: obtained from shortest path tree
	//end: obtained from shortest path tree

	
	// experienced cost is calculated from the BRP link cost from the trees generated from iteration 0, 1, ..., k-1, (not including k)
	// pi is the shortest path cost calcuatated from the BRP link cost, at exactly iteration k


	vector<vector <int> > m_path_node_vector;  // used for debugging 

	float *link_volume;  // partial link volume
	float m_TotalObsError;
	float **link_volume_perhour;  // partial link volume

	void setup_link_volume(int number_of_links)
	{
		link_volume = new float[number_of_links];
		//link_volume_perhour = new float[number_of_links][24];

	}

	void reset_OD_volume(int zone_seq_no,Assignment& assignment)
	{


		for (int d = 0; d < g_AgentLite_number_of_max_zones; d++)
		{
			for (int j = 0; j < _MAX_DEMANDTYPES; j++)
			{
				for (int tau = 0; tau < _MAX_TIMEPERIODS; tau++)
				{
					destination_OD_volume[d][j][tau] = assignment.g_demand_array[zone_seq_no][d][j];  // be careful 
					if (destination_OD_volume[d][j][tau] >= 0)
					{
						//cout << g_demand_array[zone_seq_no][d] << " " << destination_OD_volume[d] << " ";
					}
				}
			}
		}
	}
	void reset_od_volume_with_alphagamma(int zone_seq_no, Assignment& assignment, float Alpha, Demand_Parameters alphagamma)
	{
		if (Alpha <= 0.0001)
			Alpha = 0.0001;
		
		for (int d = 0; d < g_AgentLite_number_of_max_zones; d++)
		{
			for (int j = 0; j < _MAX_DEMANDTYPES; j++)
			{
				for (int tau = 0; tau < _MAX_TIMEPERIODS; tau++)
				{
					destination_OD_volume[d][j][tau] = assignment.g_demand_array[zone_seq_no][d][j] * Alpha* alphagamma.alpharatio[zone_seq_no] * alphagamma.gammaratio[d];  // be careful 
					assignment.total_demand_volume += destination_OD_volume[d][j][tau];
				}
			}
		}
	}

	void update_od_volume_with_measurements()
	{
		float step_size = 0.05;
		for (int d = 0; d < g_AgentLite_number_of_max_zones; d++)
		{
			for (int j = 0; j < _MAX_DEMANDTYPES; j++)
			{
				for (int tau = 0; tau < _MAX_TIMEPERIODS; tau++)
				{
					destination_OD_volume[d][j][tau] = destination_OD_volume[d][j][tau] + step_size * m_TotalObsError ;  // be careful 

					destination_OD_volume[d][j][tau] = max(0, destination_OD_volume[d][j][tau]);
				}
			}
		}
	}
	void reset_link_volume(int number_of_links)
	{
		if (link_volume == NULL)
			return;
		for (int l = 0; l < number_of_links; l++)
		{

			link_volume[l] = 0;
			//link_volume_perhour = 0;
		}

	}

	
	void calculate_link_flow(int iteration_no,int b_debug_detail_flag, Assignment& assignment);  // main function 
	void calculate_link_flow_second(int iteration_no, int b_debug_detail_flag, Assignment& assignment);  // main function 
	void calculate_reassignment_link_flow(int iteration_no, int b_debug_detail_flag, Assignment& assignment);  // reassignment main function 
	
};

extern std::vector<COZone> g_zone_vector;
extern std::map<int, int> g_zoneid_to_zone_seq_no_mapping;

class DemandType
{
public:
	int demand_type;
	double demand_type_percentage;
	double vehicle_trip_multiplier_factor;

	double cumulative_demand_type_percentage;
	double vehicle_type_percentage[MAX_VEHICLE_TYPE_SIZE];
	double cumulative_type_percentage[MAX_VEHICLE_TYPE_SIZE];

	double info_class_percentage[MAX_INFO_CLASS_SIZE];
	double cumulative_info_class_percentage[MAX_INFO_CLASS_SIZE];
	double Avg_VOT;
	string demand_type_name;

	DemandType()
	{
		Avg_VOT = 12;
		vehicle_trip_multiplier_factor = 1;
		cumulative_demand_type_percentage = 0;
		demand_type_percentage = 0;
		for (int vehicle_type = 0; vehicle_type < MAX_VEHICLE_TYPE_SIZE; vehicle_type++)
		{
			vehicle_type_percentage[vehicle_type] = 0;
			cumulative_type_percentage[vehicle_type] = 0;
		}
		for (int info_class = 0; info_class < MAX_INFO_CLASS_SIZE; info_class++)
		{
			info_class_percentage[info_class] = 0;
			cumulative_info_class_percentage[info_class] = 0;
		}

	}

};


class NetworkForSP  // mainly for shortest path calculation
{
public:

	NetworkForSP()
	{
		pFileAgentPathLog = NULL;
	}

	int m_threadNo;  // internal thread number 

	int m_ListFront;
	int m_ListTail;

	int* m_SENodeList;

	//	std::list<int>  m_SENodeList;  //scan eligible list as part of label correcting algorithm 

	float*** m_node_label_cost_pertype;  // label cost // for shortest path calcuating 
	float* m_node_label_generalized_time;  // label time // for shortest path calcuating 

	int*** m_node_predecessor_pertype;  // predecessor for nodes
	int* m_node_status_array; // update status 

	int*** m_link_predecessor_pertype;  // predecessor for this node points to the previous link that updates its label cost (as part of optimality condition) (for easy referencing)

	FILE* pFileAgentPathLog;  // file output

	float* m_link_volume_array; // link volume for all agents assigned in this network (thread)
	float*** m_link_cost_array_pertype; // link cost 


	std::vector<int>  m_agent_vector; // assigned agents for computing 
	std::vector<int>  m_node_demandtype_vector; // assigned nodes for computing 
	std::vector<int>  m_demand_type_vector; // assigned demand_type for computing 
	std::vector<CNode2NodeAccessibility>  m_node2node_accessibility_vector;



	void AllocateMemory(int number_of_demand_types, int number_of_nodes, int number_of_links, int number_of_time_period)
	{

		m_SENodeList = new int[number_of_nodes];

		m_node_status_array = new int[number_of_nodes];

		m_node_predecessor_pertype = new int **[number_of_time_period];
		for (int i = 0;i < number_of_time_period;i++)
		{
			m_node_predecessor_pertype[i] = new int*[number_of_demand_types];
			for (int j = 0;j < number_of_demand_types;j++)
			{
				m_node_predecessor_pertype[i][j] = new int[number_of_nodes];
			}
		}

		m_link_predecessor_pertype = new int **[number_of_time_period];
		for (int i = 0;i < number_of_time_period;i++)
		{
			m_link_predecessor_pertype[i] = new int*[number_of_demand_types];
			for (int j = 0;j < number_of_demand_types;j++)
			{
				m_link_predecessor_pertype[i][j] = new int[number_of_nodes];
			}
		}

		m_node_label_cost_pertype = new float **[number_of_time_period];
		for (int i = 0;i < number_of_time_period;i++)
		{
			m_node_label_cost_pertype[i] = new float*[number_of_demand_types];
			for (int j = 0;j < number_of_demand_types;j++)
			{
				m_node_label_cost_pertype[i][j] = new float[number_of_nodes];
			}
		}
		
		m_link_cost_array_pertype = new float **[number_of_time_period];
		for (int i = 0;i < number_of_time_period;i++)
		{
			m_link_cost_array_pertype[i] = new float*[number_of_demand_types];
			for (int j = 0;j < number_of_demand_types;j++)
			{
				m_link_cost_array_pertype[i][j] = new float[number_of_links];
			}
		}


		//m_node_label_cost_pertype = new float[number_of_demand_types][number_of_nodes];

		m_node_label_generalized_time = new float[number_of_nodes];
		//m_link_predecessor_pertype = new int[number_of_demand_types][number_of_nodes];   // note that, the size is still the number of nodes, as each node has only one link predecessor

		//char buffer[256];
		//sprintf_s(buffer, "%s_%d.csv", "agent_path", m_threadNo);

		//pFileAgentPathLog = fopen(buffer, "w");

		m_link_volume_array = new float[number_of_links];

		for (int i =0; i< number_of_time_period; i++)
		{
			for (int j = 0; j < number_of_demand_types; j++)
			{
				for (int l = 0; l < number_of_links; l++)
				{
					m_link_volume_array[l] = 0.0;
					m_link_cost_array_pertype[i][j][l] = 1.0; //default value
				}
			}
		}

		//if (pFileAgentPathLog == NULL)
		//{
		//	cout << "Cannot open File" << buffer << endl;
		//	//g_ProgramStop();
		//}
		//else
		//{

		//	fprintf(pFileAgentPathLog, "assignment_iteration_no,agent_id,origin_node_id,destination_node_id,cost,path_sequence\n");

		//}

	}

	~NetworkForSP()
	{
		if (m_SENodeList != NULL)
			delete m_SENodeList;


		if (m_node_label_cost_pertype != NULL)
			delete m_node_label_cost_pertype;

		if (m_node_predecessor_pertype != NULL)
			delete m_node_predecessor_pertype;

		if (m_node_status_array != NULL)
			delete m_node_status_array;

		if (m_link_predecessor_pertype != NULL)
			delete m_link_predecessor_pertype;

		if (m_link_volume_array != NULL)
			delete m_link_volume_array;

		if (m_link_cost_array_pertype != NULL)
			delete m_link_cost_array_pertype;

		if (pFileAgentPathLog != NULL)
			fclose(pFileAgentPathLog);
	}


	// SEList: scan eligible List implementation: the reason for not using STL-like template is to avoid overhead associated pointer allocation/deallocation
	void SEList_clear()
	{
		m_ListFront = -1;
		m_ListTail = -1;
	}

	void SEList_push_front(int node)
	{
		if (m_ListFront == -1)  // start from empty
		{
			m_SENodeList[node] = -1;
			m_ListFront = node;
			m_ListTail = node;
		}
		else
		{
			m_SENodeList[node] = m_ListFront;
			m_ListFront = node;
		}
	}
	void SEList_push_back(int node)
	{
		if (m_ListFront == -1)  // start from empty
		{
			m_ListFront = node;
			m_ListTail = node;
			m_SENodeList[node] = -1;
		}
		else
		{
			m_SENodeList[m_ListTail] = node;
			m_SENodeList[node] = -1;
			m_ListTail = node;
		}
	}

	bool SEList_empty()
	{
		return(m_ListFront == -1);
	}

	int SEList_front()
	{
		return m_ListFront;
	}

	void SEList_pop_front()
	{
		int tempFront = m_ListFront;
		m_ListFront = m_SENodeList[m_ListFront];
		m_SENodeList[tempFront] = -1;
	}


	int optimal_label_correcting(int origin_node, int destination_node, int departure_time, int shortest_path_debugging_flag, FILE* pFileDebugLog, Assignment& assignment ,int time_period_no = 0, int demand_type = 1, float VOT = 10)
		// time-dependent label correcting algorithm with double queue implementation
	{
		int internal_debug_flag = 0;

		if (g_node_vector[origin_node].m_outgoing_link_vector.size() == 0)
		{
			return 0;
		}

		for (int i = 0; i < assignment.g_number_of_nodes; i++) //Initialization for all nodes
		{
			m_node_status_array[i] = 0;  // not scanned
			m_node_label_cost_pertype[time_period_no][demand_type][i] = _MAX_LABEL_COST;
			m_node_predecessor_pertype[time_period_no][demand_type][i] = -1;  // pointer to previous NODE INDEX from the current label at current node and time
			m_link_predecessor_pertype[time_period_no][demand_type][i] = -1;  // pointer to previous NODE INDEX from the current label at current node and time
		}

		//Initialization for origin node at the preferred departure time, at departure time, cost = 0, otherwise, the delay at origin node

		m_node_label_cost_pertype[time_period_no][demand_type][origin_node] = 0;

		SEList_clear();
		SEList_push_back(origin_node);

		while (!SEList_empty())
		{
			int from_node = SEList_front();//pop a node FromID for scanning

			SEList_pop_front();  // remove current node FromID from the SE list
			m_node_status_array[from_node] = 2;

			if (shortest_path_debugging_flag)
				fprintf(pFileDebugLog, "SP: SE node: %d\n", g_node_vector[from_node].node_id);

			//scan all outbound nodes of the current node
			for (int i = 0; i < g_node_vector[from_node].m_outgoing_link_vector.size(); i++)  // for each link (i,j) belong A(i)
			{

				int to_node = g_node_vector[from_node].m_outgoing_link_vector[i].to_node_seq_no;

				/*if (to_node == origin_node)
					continue;*/
				
				ASSERT(to_node <= assignment.g_number_of_nodes);
				bool  b_node_updated = false;

				float new_to_node_cost = m_node_label_cost_pertype[time_period_no][demand_type][from_node]+ m_link_cost_array_pertype[time_period_no][demand_type][g_node_vector[from_node].m_outgoing_link_vector[i].link_seq_no];

				if (shortest_path_debugging_flag)
				{
					fprintf(pFileDebugLog, "SP: checking from node %d, to node %d  cost = %d\n",
						g_node_vector[from_node].node_id,
						g_node_vector[to_node].node_id,
						new_to_node_cost, g_node_vector[from_node].m_outgoing_link_vector[i].cost);
				}



				if (new_to_node_cost < m_node_label_cost_pertype[time_period_no][demand_type][to_node]) // we only compare cost at the downstream node ToID at the new arrival time t
				{

					if (shortest_path_debugging_flag)
					{
						fprintf(pFileDebugLog, "SP: updating node: %d current cost: %.2f, new cost %.2f\n",
							g_node_vector[to_node].node_id,
							m_node_label_cost_pertype[time_period_no][demand_type][to_node], new_to_node_cost);
					}

					// update cost label and node/time predecessor

					m_node_label_cost_pertype[time_period_no][demand_type][to_node] = new_to_node_cost;
					int link_seq_no = g_node_vector[from_node].m_outgoing_link_vector[i].link_seq_no;
				
					m_node_predecessor_pertype[time_period_no][demand_type][to_node] = from_node;  // pointer to previous physical NODE INDEX from the current label at current node and time
					m_link_predecessor_pertype[time_period_no][demand_type][to_node] = g_node_vector[from_node].m_outgoing_link_vector[i].link_seq_no;  // pointer to previous physical NODE INDEX from the current label at current node and time
				
					b_node_updated = true;

					if (shortest_path_debugging_flag)
						fprintf(pFileDebugLog, "SP: add node %d into SE List\n",
							g_node_vector[to_node].node_id);

					//to_node is zone centroid and not origin_node,is to make sure no passing zones, only needed in network with connector
					/*if (g_node_vector[to_node].zone_id != -1 && to_node != origin_node)
					{
						m_node_status_array[to_node] = 1;
					}*/
					
					if (m_node_status_array[to_node] == 0) 
					{
						SEList_push_back(to_node);
						m_node_status_array[to_node] = 1;
					}
					if (m_node_status_array[to_node] == 2)
					{
						SEList_push_front(to_node);
						m_node_status_array[to_node] = 1;
					}

				}

			}
		}

		if (destination_node >= 0 && m_node_label_cost_pertype[time_period_no][demand_type][destination_node] < _MAX_LABEL_COST)
			return 1;
		else if (destination_node == -1)
			return 1;  // one to all shortest pat
		else
			return -1;
	}

	void optimal_label_correcting_for_all_nodes_assigned_to_processor(Assignment& assignment)
	{
		 if(m_node_demandtype_vector.size() > 0)
		 { 
				int origin = m_node_demandtype_vector[0];
				int demand_type = m_node_demandtype_vector[1];
				int time_period_no = m_node_demandtype_vector[2];
				int return_value = optimal_label_correcting(origin, -1, 0, 0, g_pFileDebugLog,assignment, time_period_no, demand_type, 10.0);  // one to all shortest path
		 }

	}
};


class CNetworkLoading  // mainly for simulation for given path
{
public:
	CNetworkLoading()
	{
		Number_of_CompletedVehicles = 0;
		Number_of_GeneratedVehicles = 0;
		SimululationReadyToEnd = 99999;
		SimulationInterval_InMin = 0.1; // 6 seconds
		SignalRepresentationFlag = 0;
		MinimumInFlowRatio = 2;
		MaxDensityRatioForVehicleLoading = 0.7;
	}
	int meso_simulation_time_interval_no;
	double SimulationInterval_InMin;

	float MaxDensityRatioForVehicleLoading;

	
	int SignalRepresentationFlag;

	void Load_agents_to_links_for_simulation(int simulation_time_horizon_in_min);
	void network_loading(e_traffic_flow_model traffic_flow_model_flag = tfm_spatial_queue, int SimulationMode = 0, int Iteration = 1);  // default spatial queue // SimulationMode= default 0: UE;  1: simulation from demand; 2: simulation from vehicle file;

	bool agent_simulation(int day_no, double current_time_in_min, int meso_simulation_time_interval_no,
		e_traffic_flow_model traffic_flow_model_flag, int );

	int Number_of_CompletedVehicles;
	int Number_of_GeneratedVehicles;
	int MinimumInFlowRatio;
	

	int demand_loading_start_time_in_min;  //e.g. 6AM
	int PlanningHorizon;  // 3PM+ 3 hours for cleaning the agents

	int SimululationReadyToEnd; // for early finish when the network is empty

	void output_debug_info_on_screen(int iteration, string str);
};



class NetworkMOE
{
public:
	int CumulativeInFlow;
	int CumulativeOutFlow;
	int Flow_in_a_min;
	float AbsArrivalTimeOnDSN_in_a_min;
	float TotalFreeFlowTravelTime;
	float AvgTripTime;
	float TotalDistance;
	float TotalBufferWaitingTime;


	NetworkMOE()
	{
		TotalFreeFlowTravelTime = 0;
		CumulativeInFlow = 0;
		CumulativeOutFlow = 0;
		Flow_in_a_min = 0;
		AbsArrivalTimeOnDSN_in_a_min = 0;
		AvgTripTime = 0;
		TotalDistance = 0;
		TotalBufferWaitingTime = 0;

	}
};