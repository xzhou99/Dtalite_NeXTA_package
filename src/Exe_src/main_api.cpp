//  Portions Copyright 2010-2020 Xuesong (Simon) Zhou

//   If you help write or modify the code, please also list your names here.
//   The reason of having copyright info here is to ensure all the modified version, as a whole, under the GPL
//   and further prevent a violation of the GPL.

// More about "How to use GNU licenses for your own software"
// http://www.gnu.org/licenses/gpl-howto.html

//    This file is part of DTALite.

//    DTALite is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    DTALite is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with DTALite.  If not, see <http://www.gnu.org/licenses/>.

// DTALite.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <fstream>
#include <omp.h>
#include <algorithm>

using namespace std;

/*************************************
How to build a simple DTA simulator

step 0: basic input functions


defintions of DTANode, DTALink and DTAAgent
utility function g_read_integer() g_read_float()
ReadNetworkData() and ReadAgentData()

step 1: dynamic memory management
FreeMemory() to deallocate dynamic memory for the whole simulation program
Allocate and deallocate dynamic arrays for DTANetworkForSP

step 2: network building for traffic assignment
DTANetworkForSP::BuildNetworkBasedOnZoneCentriod(int ZoneID)

step 3: convert time-dependent OD demand to Agents data, sorted by departure time
CreateAgents(int origin_zone, int destination_zone, float number_of_Agents, int agent_type, float starting_time_in_min, float ending_time_in_min)


step 4: Scan eligiable list and shortest path algorithm
SEList functions in DTANetworkForSP
TDLabelCorrecting_DoubleQueue(int origin, int departure_time)

step 5: assign path to Agents
GetLinkNoByNodeIndex(int usn_index, int dsn_index)


step 6: integerate network building, shortest path algorithm and path assignment to dynamic traffic assignment

step 7: output Agent trajectory file

step 8: NetworkLoading

step 9: VehicularSimulation

step 10: parallel computing: assign Agents to arrays with different origins/departure times, to speed up the calculation

step 11: parallel computing for different zones
OpenMP

step 12: prepare dynamic arrays for within-simulation shortest path for pre-trp and enroute info Agents
store the data for each destination node to global array as follows.
//int ***g_RTNodePredAry;  // Agent type, destination, node available in real-time simulation
//unsigned char *** g_RTLabelCostAry; // Agent type, destination, node available in real-time simulation


Step 13:
allow multiple centriods per zone in zone.csv,

step 13: stochastic capacity with random numbers for different zones

step 14: UE, SO algorithm, day to day learning algorithm
updating, estimating variance and making prediction

make this as a convergency process

route choice -> compare threshold

Step 15: GetTimeDependentCapacityAtSignalizedIntersection
Add dynamical outflow capacity for signalized intersection

step 16: assign pre-trip info shortest path to Agents

habitual paths vs. real time info
information provides


feature functions: link-based shortest path to consider turnning panalty

*************************************/

/**************************
menu -> project -> property -> configuraiton -> debugging -> setup working directory

***************************/
float g_simulation_time_interval_step = 6;  // in seconds
int g_simulation_time_intervals_per_min = (int)(60/ g_simulation_time_interval_step);
double g_DTASimulationInterval_InMin = g_simulation_time_interval_step / 60.0; // min


// The one and only application object
FILE *g_simulation_log_file = NULL;
int g_simulation_log_level = 0;


CWinApp theApp;

TCHAR g_DTASettingFileName[_MAX_PATH] = _T("./DTASettings.txt");


double g_UnitMile = 1; // to do: calculate this measure based on total link distance


NetworkSimulationResult g_SimulationResult;
std::vector<DTANode> g_NodeVector;
std::map<int, int> g_NodeID2NoMap;

std::vector<DTALink*> g_LinkVector;
std::map<string, DTALink*> g_LinkMap;
std::map<string, DTALink*> g_RoadLinkIDMap;
std::vector<DTAZone> g_ZoneVector;
std::map<int,int> g_ZoneID2NoMap;


std::vector<DTAAgent*>		g_AgentVector;

std::map<int, DTAAgent*> g_AgentMap;
std::map<int, DTAVehListPerTimeInterval> g_AgentTDListMap;

std::vector< DemandType> g_DemandTypeVector;


std::map<int, DTALinkType> g_LinkTypeMap;
std::map<int, string> g_NodeControlTypeMap;

// time inteval settings in assignment and simulation


int g_number_of_intervals_per_min = 10; // round to nearest integer
int g_number_of_car_following_intervals_per_min = 600; // round to nearest integer
int g_AggregationTimetInterval = 15; // min
int g_assessibility_computing_flag = 0;

int g_TDSPTimetIntervalSizeForMin = 10;  // 6 seconds
float g_DemandGlobalMultiplier = 1.0f;
// maximal # of adjacent links of a node (including physical nodes and centriods( with connectors))
int g_AdjLinkSize = 5; // initial value of adjacent links

int g_ZoneSize = 0;
int g_number_of_prohibited_movements = 0;
int g_StartIterationsForOutputPath = 2;
int g_EndIterationsForOutputPath = 2;

// assignment and simulation settings
int g_NumberOfIterations = 1;
int g_ProhibitUTurnOnFeewayLinkFlag = 0;
float g_ConvergencyRelativeGapThreshold_in_perc;

int g_PlanningHorizon = 120;  // short horizon for saving memory

int g_SimululationReadyToEnd = 120;

// assignment
float g_CurrentGapValue = 0.0; // total network gap value in the current iteration
float g_CurrentRelativeGapValue = 0.0;
float g_PrevRelativeGapValue = 0.0;
float g_PercentageCompleteTrips = 100.0;
int g_CurrentNumOfAgentsSwitched = 0; // total number of Agents switching paths in the current iteration; for MSA, g_UEAssignmentMethod = 0
int g_CurrentNumOfAgentsForUEGapCalculation = 0;
int g_PrevNumOfAgentsSwitched = 0; // // total number of Agents switching paths in last iteration; for MSA, g_UEAssignmentMethod = 0
int g_ConvergenceThreshold_in_Num_Switch; // the convergence threshold in terms of number of Agents switching paths; for MSA, g_UEAssignmentMethod = 0
int g_AgentExperiencedTimeGap = 1; // 1: Agent experienced time gap; 0: Avg experienced path time gap
int g_NewPathWithSwitchedAgents = 0; // number of new paths with Agents switched to them

float g_OverallPerceptionErrorRatio = 0;

int g_LearningPercentage = 15;
float g_TravelTimeDifferenceForSwitching = 1.0;  // min
float g_RelativeTravelTimePercentageDifferenceForSwitching = 15;  // min


int g_UseRandomCapacityMode = 0;
float g_MinimumInFlowRatio = 0.1f;
float g_RelaxInFlowConstraintAfterDemandLoadingTime = 60;
float g_MaxDensityRatioForAgentLoading = 0.8f;
float g_DefaultSaturationFlowRate_in_vehphpl;

int g_DemandLoadingStartTimeInMin = 99999;
int g_DemandLoadingEndTimeInMin = 0;

int g_number_of_warnings = 0;  // use a global count to avoid warning messages when running multiple scenarioss

double g_number_of_intra_zone_trips = 0;

int g_Number_of_CompletedAgents = 0;
int g_Number_of_CompletedAgentsThatSwitch = 0;
int g_Number_of_GeneratedAgents = 0;


int g_SystemOptimalStartingTimeinMin = 450;

AgentArrayForOriginDepartrureTimeInterval** g_TDOAgentArray = NULL; // TDO for time-dependent origin


FILE* g_DebugLogFile = NULL;



ofstream g_LogFile;


CCSVWriter g_SummaryStatFile;
CCSVWriter g_MultiScenarioSummaryStatFile;

ofstream g_WarningFile;

e_traffic_flow_model g_TrafficFlowModelFlag = tfm_BPR;
int g_UseDefaultLaneCapacityFlag = 1;
int g_UseFreevalRampMergeModelFlag = 0;
int g_CalculateUEGapForAllAgents = 0;
int g_AgentPathOutputFlag = 1;
int g_TimeDependentODMOEOutputFlag = 0;

int g_MergeNodeModelFlag = 1;
int g_FIFOConditionAcrossDifferentMovementFlag = 0;

std::vector<NetworkMOE>  g_NetworkMOEAry;
std::vector<NetworkLoadingOutput>  g_AssignmentMOEVector;

DTALinkToll** g_LinkTDCostAry = NULL;

DTASettings g_settings;  // global settings;


CTime g_AppStartTime;
CTime g_AppLastIterationStartTime;

int g_NumberOfSPCalculationPeriods = 1;

unsigned int g_RandomSeed = 100;

using namespace std;


// Use to convert bytes to MB
#define DIV 1048576

// Use to convert bytes to MB
//#define DIV 1024

// Specify the width of the field in which to print the numbers.
// The asterisk in the format specifier "%*I64d" takes an integer
// argument and uses it to pad and right justify the number.

#define WIDTH 7
int g_InitialFreeMemory = 0;

int g_GetFreeMemoryDataInMB()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);
	return statex.ullAvailPhys / DIV;
}

CString g_GetUsedMemoryDataInMB()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);
	int used_memory = g_InitialFreeMemory - statex.ullAvailPhys / DIV;
	CString str;
	str.Format("(Used RAM %d MB)", used_memory);
	return str;
}

using namespace std;

extern CTime g_AppStartTime;
extern CTime g_AppLastIterationStartTime;
// polar form of the Box-Muller transformation to get two random numbers that follow a standard normal distribution
unsigned int g_RandomSeedForAgentGeneration = 101;
// Linear congruential generator
#define g_LCG_a 17364
#define g_LCG_c 0
#define g_LCG_M 65521

long g_precision_constant = 1000L;
long g_precision_constant2 = 1000L;
extern float g_DemandGlobalMultiplier;
int g_ProgramStopFlag = 0;

#include<iostream>
#include<cmath>
using namespace std;





std::string CString2StdString(CString str)
{	 // Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(str);

	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);

	return strStd;
}

std::string GetTimeClockString(int time)
{
	int hour = ((int)(time)) / 60;
	int min = time - hour * 60;

	CString time_str;
	time_str.Format("%2d:%02d", hour, min);
	return (CString2StdString(time_str));
}

bool g_floating_point_value_less_than_or_eq_comparison(double value1, double value2)
{
	long lValue1 = (long)(value1*g_precision_constant);
	long lValue2 = (long)(value2*g_precision_constant);

	if (lValue1 < lValue2 + 2) // we take 2/100 sec accuracy
		return true;
	else
		return false;

}

bool g_floating_point_value_less_than(double value1, double value2)
{
	long lValue1 = (long)(value1*g_precision_constant2);
	long lValue2 = (long)(value2*g_precision_constant2);


	if (lValue1 < lValue2)
		return true;
	else
		return false;

}

bool g_Compare_Agent_Item(struc_Agent_item item1, struc_Agent_item item2)
{
	if (item1.event_time_stamp < item2.event_time_stamp)  // item1 is earlier than item2
		return true;
	else
		return false;
}

double g_GetRandomRatio()
{
	//	g_RandomSeed = (g_LCG_a * g_RandomSeed + g_LCG_c) % g_LCG_M;  //m_RandomSeed is automatically updated.
	//	return float(g_RandomSeed)/g_LCG_M;

	return WELLRNG512a();
}

int g_GetRandomInteger(double Value)
{
	int int_value = int(Value);
	double Residual = Value - int_value;
	double RandomNumber = g_GetRandomRatio(); //between 0 and 1
	if (RandomNumber < Residual)
	{
		return int_value + 1;
	}
	else
	{
		return int_value;
	}
}

float g_GetRandomRatioForAgentGeneration()
{
	g_RandomSeedForAgentGeneration = (g_LCG_a * g_RandomSeedForAgentGeneration + g_LCG_c) % g_LCG_M;  //m_RandomSeed is automatically updated.

	return float(g_RandomSeedForAgentGeneration) / g_LCG_M;

}

string GetLinkStringID(int FromNodeID, int ToNodeID)
{
	ostringstream ss;
	ss << FromNodeID << ":" << ToNodeID;
	return ss.str();
}

string GetMovementStringID(int FromNodeID, int ToNodeID, int DestNodeID)
{
	ostringstream string_movement;
	string_movement << FromNodeID << ":" << ToNodeID << ":" << DestNodeID;
	return string_movement.str();
}

float g_RNNOF()
{
	float x1, x2, w, y1, y2;

	do {
		x1 = 2.0f * g_GetRandomRatio() - 1.0f;
		x2 = 2.0f * g_GetRandomRatio() - 1.0f;
		w = x1 * x1 + x2 * x2;
	} while (w >= 1.0f);

	w = sqrt((-2.0f * log(w)) / w);
	y1 = x1 * w;
	y2 = x2 * w;

	return y1;  // we only use one random number
}


void g_ProgramStop()
{
	if (g_ProgramStopFlag == 0)
		g_ProgramStopFlag = 1;
	cout << "DTALite Program stops. Press any key to terminate. Thanks!" << endl;
	getchar();
	exit(0);
};

void g_ProgramTrace(CString str)
{
	//	cout << str << endl;
	//	getchar();

};
int g_read_integer(FILE* f, bool speicial_char_handling)
// read an integer from the current pointer of the file, skip all spaces
{
	char ch, buf[32];
	int i = 0;
	int flag = 1;
	/* returns -1 if end of file is reached */

	while (true)
	{
		ch = getc(f);
		if (ch == EOF || (speicial_char_handling && (ch == '*' || ch == '$')))
			return -1; // * and $ are special characters for comments
		if (isdigit(ch))
			break;
		if (ch == '-')
			flag = -1;
		else
			flag = 1;
	};
	if (ch == EOF) return -1;


	while (isdigit(ch)) {
		buf[i++] = ch;
		ch = fgetc(f);
	}
	buf[i] = 0;


	return atoi(buf) * flag;

}

int g_GetPrivateProfileInt(LPCTSTR section, LPCTSTR key, int def_value, LPCTSTR filename, bool print_out)
{
	char lpbuffer[64];
	int value = def_value;
	if (GetPrivateProfileString(section, key, "", lpbuffer, sizeof(lpbuffer), filename))
	{
		value = atoi(lpbuffer);
	}

	if (value == def_value)  //  the parameter might not exist
	{
		sprintf_s(lpbuffer, "%d", def_value);
		WritePrivateProfileString(section, key, lpbuffer, filename);
	}

	if (print_out)
		cout << "section <" << section << ">: " << key << " = " << value << endl;

	return value;
}

int g_WritePrivateProfileInt(LPCTSTR section, LPCTSTR key, int def_value, LPCTSTR filename)
{
	char lpbuffer[64];
	int value = def_value;
	sprintf_s(lpbuffer, "%d", def_value);
	WritePrivateProfileString(section, key, lpbuffer, filename);
	return value;
}

float g_GetPrivateProfileFloat(LPCTSTR section, LPCTSTR key, float def_value, LPCTSTR filename, bool print_out)
{
	char lpbuffer[64];
	float value = def_value;
	if (GetPrivateProfileString(section, key, "", lpbuffer, sizeof(lpbuffer), filename))
	{
		value = (float)(atof(lpbuffer));
	}

	if (value == def_value)  //  the parameter might not exist
	{
		sprintf_s(lpbuffer, "%5.2f", def_value);
		WritePrivateProfileString(section, key, lpbuffer, filename);
	}
	if (print_out)
		cout << "section <" << section << ">: " << key << " = " << value << endl;

	return value;
}

struct entity_deleter
{
	void operator()(DTAAgent*& e) // important to take pointer by reference!
	{
		delete e;
		e = NULL;
	}
};

void g_FreeMemoryForAgentVector()
{
	cout << "Free memory for Agent set... " << endl;
	for_each(g_AgentVector.begin(), g_AgentVector.end(), entity_deleter());

	//std::vector<DTAAgent*>::iterator iterAgent;		//this part of code needs to be carelfully reviewed, as it tries to delete pointers within STL
	//for (iterAgent = g_AgentVector.begin(); iterAgent != g_AgentVector.end();iterAgent++)
	//{
	//	delete *iterAgent;
	//	iterAgent = g_AgentVector.erase(iterAgent);
	//}

	g_AgentVector.clear();
	g_AgentMap.clear();
	g_AgentTDListMap.clear();
	cout << "Complete. " << endl;
}
CString g_timestamp_HHMM_convertor(int time_stamp_in_min)
{
	CString str;
	int hour = time_stamp_in_min / 60;
	int min = (time_stamp_in_min - hour * 60);

	if (hour < 10)
		str.Format("0%d:%02d", hour, min);
	else
		str.Format("%2d:%02d", hour, min);

	// Convert a TCHAR string to a LPCSTR
	return str;
}


CString g_GetAppRunningTime(bool with_title)
{
	CString str;
	CTime EndTime = CTime::GetCurrentTime();
	CTimeSpan ts = EndTime - g_AppStartTime;

	if (with_title)
		str = ts.Format("CPU Clock: %H:%M:%S --");
	else
		str = ts.Format("%H:%M:%S");
	return str;
}

CString g_GetAppRunningTimePerIteration(bool with_title)
{
	CString str;
	CTime EndTime = CTime::GetCurrentTime();
	CTimeSpan ts = EndTime - g_AppLastIterationStartTime;

	if (with_title)
		str = ts.Format("CPU Clock: %H:%M:%S --");
	else
		str = ts.Format("%H:%M:%S");
	return str;
}



int  DTANetworkForSP::GetLinkNoByNodeIndex(int usn_index, int dsn_index)
{
	int LinkNo = -1;
	for (int i = 0; i < m_OutboundSizeAry[usn_index]; i++)
	{

		if (m_OutboundNodeAry[usn_index][i] == dsn_index)
		{
			LinkNo = m_OutboundLinkAry[usn_index][i];
			return LinkNo;
		}
	}

	cout << " Error in GetLinkNoByNodeIndex " << g_NodeVector[usn_index].m_NodeID << "-> " << g_NodeVector[dsn_index].m_NodeID;

	g_ProgramStop();


	return MAX_LINK_NO;


}


/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */
#define W 32
#define R 16
#define P 0
#define M1 13
#define M2 9
#define M3 5

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define MAT3NEG(t,v) (v<<(-(t)))
#define MAT4NEG(t,b,v) (v ^ ((v<<(-(t))) & b))

#define V0            STATE[state_i                   ]
#define VM1           STATE[(state_i+M1) & 0x0000000fU]
#define VM2           STATE[(state_i+M2) & 0x0000000fU]
#define VM3           STATE[(state_i+M3) & 0x0000000fU]
#define VRm1          STATE[(state_i+15) & 0x0000000fU]
#define VRm2          STATE[(state_i+14) & 0x0000000fU]
#define newV0         STATE[(state_i+15) & 0x0000000fU]
#define newV1         STATE[state_i                 ]
#define newVRm1       STATE[(state_i+14) & 0x0000000fU]

#define FACT 2.32830643653869628906e-10

static unsigned int state_i = 0;
static unsigned int STATE[R];
static unsigned int z0, z1, z2;

void InitWELLRNG512a(unsigned int *init) {
	int j;
	state_i = 0;
	for (j = 0; j < R; j++)
		STATE[j] = init[j];
}

double WELLRNG512a(void) {
	z0 = VRm1;
	z1 = MAT0NEG(-16, V0) ^ MAT0NEG(-15, VM1);
	z2 = MAT0POS(11, VM2);
	newV1 = z1 ^ z2;
	newV0 = MAT0NEG(-2, z0) ^ MAT0NEG(-18, z1) ^ MAT3NEG(-28, z2) ^ MAT4NEG(-5, 0xda442d24U, newV1);
	state_i = (state_i + 15) & 0x0000000fU;
	return ((double)STATE[state_i]) * FACT;
}



void _proxy_simulation_log(int level, int simulation_interval_no, double simulation_in_min, int debug_agent_id, const char *fmt, ...)
{

		va_list arg;

	/* Check if the message should be logged */
	if (level > g_simulation_log_level)
		return;
#pragma omp critical
	{
	/* Write the error message */
	va_start(arg, fmt);
	if (g_simulation_log_file != NULL)
	{
		fprintf(g_simulation_log_file, "sim clock=%d (%.2f min),", simulation_interval_no, simulation_in_min);

		if (debug_agent_id >= 0)
			fprintf(g_simulation_log_file, "debug_agent_id=%d,", debug_agent_id);

		vfprintf(g_simulation_log_file, fmt, arg);
	}
	va_end(arg);

#ifdef DEBUG
	fflush(g_simulation_log_file);
	//	fsync(fileno(g_simulation_log_file));
#endif
}
}


double g_simulation_time = 0;

int g_path_error = 0;

int g_FindAssignmentIntervalIndexFromTime(float time_in_min)
{

	int return_value = 0;
	if (time_in_min < g_DemandLoadingStartTimeInMin)
		return 0;
	else if (time_in_min >= g_DemandLoadingEndTimeInMin)
	{
		return_value = max(0, g_NumberOfSPCalculationPeriods - 1);
	}

	return_value = (time_in_min - g_DemandLoadingStartTimeInMin) / g_AggregationTimetInterval;
	return min(return_value, g_NumberOfSPCalculationPeriods - 1);
}

void g_SetupTDTollValue(int DayNo)
{

	unsigned li;
	for (li = 0; li < g_LinkVector.size(); li++)
	{
		DTALink* pLink = g_LinkVector[li];

		//// ECOSO 2016
		//for (int t = g_DemandLoadingStartTimeInMin; t < g_PlanningHorizon; t += g_AggregationTimetInterval)
		//{
		//	int		link_entering_time_interval = g_FindAssignmentIntervalIndexFromTime(t);
		//	int LinkInFlow = pLink->GetArrivalFlow(t);

		//	if(g_LinkTDCostAry!=NULL)
		//	{
		//	g_LinkTDCostAry[pLink->m_LinkNo][link_entering_time_interval].TollValue[0] = pLink->m_LinkMOEAry[t].SystemOptimalMarginalCost;  // this is average energy per Agent for using this link at time t
		//	 }
		//}


		if (pLink->TollVector.size() == 0)
			continue;

		for (int t = g_DemandLoadingStartTimeInMin; t < g_PlanningHorizon; t += g_AggregationTimetInterval)
		{
			int		link_entering_time_interval = g_FindAssignmentIntervalIndexFromTime(t);

			// copy pricing type dependent link toll values
			for (int itoll = 0; itoll < pLink->TollVector.size(); itoll++)
			{


				if ((DayNo >= pLink->TollVector[itoll].StartDayNo && DayNo <= pLink->TollVector[itoll].EndDayNo)
					&& t >= pLink->TollVector[itoll].StartTime && t <= pLink->TollVector[itoll].EndTime)
				{
					g_LinkTDCostAry[pLink->m_LinkNo][link_entering_time_interval].m_bMonetaryTollExist = true;


					for (int agent_type = 1; agent_type <= g_DemandTypeVector.size(); agent_type++)
					{
						if (pLink->TollVector[itoll].TollRate[agent_type] >= 0.00001)
						{

							g_LinkTDCostAry[pLink->m_LinkNo][link_entering_time_interval].TollValue[agent_type] = pLink->TollVector[itoll].TollRate[agent_type];
						}
					}
				}
			}
		}



	}

}


void DTANetworkForSP::BuildPhysicalNetwork(int DayNo, int CurrentZoneNo, e_traffic_flow_model TraffcModelFlag, bool bUseCurrentInformation, double CurrentTime)  // for agent based
{

	//CurrentZoneNo >=0: called by zone based assignment

	if (m_OutboundSizeAry == NULL)
		return;

	bool bDebug = true;

	std::set<DTANode*>::iterator iterNode;
	std::set<DTALink*>::iterator iterLink;

	m_NodeSize = g_NodeVector.size();

	int IntervalLinkID = 0;
	int FromNo, ToNo;

	int i, t;

	for (i = 0; i < m_NodeSize; i++)
	{
		m_OutboundSizeAry[i] = 0;
		m_InboundSizeAry[i] = 0;
	}

	for (i = 0; i < m_LinkSize; i++)
	{
		m_OutboundMovementSizeAry[i] = 0;
	}

	// add physical links
	//cout << "add physical links " << endl;

	unsigned li;
	for (li = 0; li < g_LinkVector.size(); li++)
	{
		DTALink* pLink = g_LinkVector[li];
		FromNo = pLink->m_FromNodeNo;
		ToNo = pLink->m_ToNodeNo;


		//	cout << "add link no." << li <<  endl;

		if (pLink->m_FromNodeID == 6 && pLink->m_ToNodeID == 7)
		{
			TRACE("");
		}

		m_FromNoAry[pLink->m_LinkNo] = FromNo;
		m_ToNoAry[pLink->m_LinkNo] = ToNo;

		m_OutboundNodeAry[FromNo][m_OutboundSizeAry[FromNo]] = ToNo;
		m_OutboundLinkAry[FromNo][m_OutboundSizeAry[FromNo]] = pLink->m_LinkNo;

		int road_link_id = pLink->m_LinkNo;


		if (g_LinkTypeMap[g_LinkVector[road_link_id]->m_link_type].IsConnector())
		{
			m_LinkConnectorFlag[road_link_id] = 1;
			m_OutboundConnectorOriginZoneIDAry[FromNo][m_OutboundSizeAry[FromNo]] = g_NodeVector[g_LinkVector[road_link_id]->m_FromNodeNo].m_ZoneID;
			m_OutboundConnectorDestinationZoneIDAry[FromNo][m_OutboundSizeAry[FromNo]] = g_NodeVector[g_LinkVector[road_link_id]->m_ToNodeNo].m_ZoneID;
			m_LinkConnectorOriginZoneIDAry[road_link_id] = g_NodeVector[g_LinkVector[road_link_id]->m_FromNodeNo].m_ZoneID;
			m_LinkConnectorDestinationZoneIDAry[road_link_id] = g_NodeVector[g_LinkVector[road_link_id]->m_ToNodeNo].m_ZoneID;


		}
		else
		{
			m_LinkConnectorFlag[road_link_id] = 0;
			m_OutboundConnectorOriginZoneIDAry[FromNo][m_OutboundSizeAry[FromNo]] = -1; // default values
			m_OutboundConnectorDestinationZoneIDAry[FromNo][m_OutboundSizeAry[FromNo]] = -1; // default values
			m_LinkConnectorOriginZoneIDAry[road_link_id] = -1;
			m_LinkConnectorDestinationZoneIDAry[road_link_id] = -1;

		}

		m_OutboundSizeAry[FromNo] += 1;

		m_InboundLinkAry[ToNo][m_InboundSizeAry[ToNo]] = pLink->m_LinkNo;
		m_InboundSizeAry[ToNo] += 1;

		if (g_AdjLinkSize <= m_OutboundSizeAry[FromNo])
		{
			cout << "node " << g_NodeVector[FromNo].m_NodeID << " have more than " << m_OutboundSizeAry[FromNo] << " outbound links. Please check." << endl;

			g_ProgramStop();
		}

		m_LinkTDDistanceAry[pLink->m_LinkNo] = pLink->m_Length;
		m_LinkFFTTAry[pLink->m_LinkNo] = pLink->m_FreeFlowTravelTime;


		// set time-dependent link travel time, cost, transit values
		int link_entering_time_interval;

		//initialization
		for (int ti = 0; ti < m_NumberOfSPCalculationIntervals; ti += 1)
		{
			m_LinkTDTimeAry[pLink->m_LinkNo][ti] = g_DTASimulationInterval_InMin;
		}


		for (t = m_StartTimeInMin; t < m_PlanningHorizonInMin; t += g_AggregationTimetInterval)
		{
			link_entering_time_interval = g_FindAssignmentIntervalIndexFromTime(t);
			int aggregation_time_interval_in_min = g_AggregationTimetInterval;

			// we obtain simulated time-dependent travel time measurments from simulator, use that for time-dependent shortest path calculation
			float AvgTravelTime = pLink->GetTravelTimeByMin(t, aggregation_time_interval_in_min, TraffcModelFlag);

			AvgTravelTime *= g_LinkTypeMap[pLink->m_link_type].link_type_bias_factor;

			if (AvgTravelTime < g_DTASimulationInterval_InMin)  // to avoid possible loops
				AvgTravelTime = g_DTASimulationInterval_InMin;

			ASSERT(AvgTravelTime < 99999);

			if (bDebug)
			{
				if (g_NodeVector[FromNo].m_NodeID == 31 && g_NodeVector[ToNo].m_NodeID == 32)
					TRACE("FromNo %d -> ToNo %d, time: %d, %f\n", g_NodeVector[FromNo].m_NodeID, g_NodeVector[ToNo].m_NodeID, t, AvgTravelTime);
			}

			m_LinkTDTimeAry[pLink->m_LinkNo][link_entering_time_interval] = AvgTravelTime;


		}

	}

	//	cout << "construct outbound movement vector " << endl;

		// construct outbound movement vector
	for (li = 0; li < g_LinkVector.size(); li++)
	{
		// if CurrentZoneNo ==- 1, we do not run the following line (to skip zone outgoing connectors)
		if (CurrentZoneNo >= 0 && g_LinkTypeMap[g_LinkVector[li]->m_link_type].IsConnector()
			&& g_NodeVector[g_LinkVector[li]->m_FromNodeNo].m_ZoneID != CurrentZoneNo) // only for non connector-links
			continue;

		//find downstream node
		ToNo = g_LinkVector[li]->m_ToNodeNo;
		if (g_NodeVector[ToNo].m_MovementMap.size() == 0)  //without movement capacity input
		{
			ASSERT(m_AdjLinkSize >= m_OutboundSizeAry[ToNo]);

			for (int movement = 0; movement < m_OutboundSizeAry[ToNo]; movement++)
			{

				int outbound_link = m_OutboundLinkAry[ToNo][movement];
				m_OutboundMovementAry[li][movement] = outbound_link;
				m_OutboundMovementDelayAry[li][movement] = 0;   // we need to add time-dependent movement delay here
				m_OutboundMovementSizeAry[li]++;
			}

		}
		else //with movement capacity input
		{
			for (int movement = 0; movement < m_OutboundSizeAry[ToNo]; movement++)
			{
				int outbound_link = m_OutboundLinkAry[ToNo][movement];
				m_OutboundMovementAry[li][movement] = outbound_link;


				int from_node = g_LinkVector[li]->m_FromNodeID;
				int to_node = g_LinkVector[li]->m_ToNodeID;
				int dest_node = g_LinkVector[outbound_link]->m_ToNodeID;

				if (to_node == 14854)
					TRACE("");


				string movement_id = GetMovementStringID(from_node, to_node, dest_node);
				if (g_NodeVector[ToNo].m_MovementMap.find(movement_id) != g_NodeVector[ToNo].m_MovementMap.end()) // the capacity for this movement has been defined
				{
					DTAMovement movement_element = g_NodeVector[ToNo].m_MovementMap[movement_id];
					m_OutboundMovementDelayAry[li][movement] = movement_element.GetAvgDelay_In_Min();
				}
				else
				{
					m_OutboundMovementDelayAry[li][movement] = 0;   // we need to add time-dependent movement delay here
				}

				m_OutboundMovementSizeAry[li]++;
			}

		}

		//end of  constructing outbound movement vector
	}

	m_LinkSize = g_LinkVector.size();

	int iteration = 1;
	g_SetupTDTollValue(iteration);

}


bool DTANetworkForSP::TDLabelCorrecting_DoubleQueue(
	int origin, int origin_zone, int departure_time,
	int agent_type = 1, float VOT = 10, bool distance_cost_flag = false,
	bool debug_flag = false, bool bDistanceCostByProductOutput = true)
	// time -dependent label correcting algorithm with deque implementation
{
	debug_flag = false;
	// this is the standard shortest path algorithm
	int i;
	float AdditionalCostInMin = 0;

	if (m_OutboundSizeAry[origin] == 0)
		return false;

	for (i = 0; i < m_NodeSize; i++) // Initialization for all nodes
	{
		NodePredAry[i] = -1;
		LinkNoAry[i] = -1;

		NodeStatusAry[i] = 0;

		LabelTimeAry[i] = MAX_SPLABEL;
		LabelCostAry[i] = MAX_SPLABEL;

	}


	// Initialization for origin node
	LabelTimeAry[origin] = float(departure_time);
	LabelCostAry[origin] = 0;

	if (bDistanceCostByProductOutput)  // used mainly for accessibility calculation
	{

		for (i = 0; i < m_NodeSize; i++) // Initialization for all nodes
		{
			LabelDistanceAry[i] = MAX_SPLABEL;
			LabelDollarCostAry[i] = 0;
		}

		LabelDistanceAry[origin] = 0;
		LabelDollarCostAry[origin] = 0;


	}


	SEList_clear();
	SEList_push_front(origin);

	int FromID, LinkID, ToID;


	float NewTime, NewCost, NewDistance, NewDollarCost;
	float DollarCost = 0;
	while (!SEList_empty())
	{
		FromID = SEList_front();
		SEList_pop_front();

		if (debug_flag && FromID < m_PhysicalNodeSize)  // physical nodes
		{
			TRACE("\nScan from node %d", g_NodeVector[FromID].m_NodeID);
		}

		NodeStatusAry[FromID] = 2;        //scaned

		for (i = 0; i < m_OutboundSizeAry[FromID]; i++)  // for each arc (i,j) belong A(j)
		{
			LinkID = m_OutboundLinkAry[FromID][i];
			ToID = m_OutboundNodeAry[FromID][i];

			if (ToID == origin)
				continue;


			if (m_LinkConnectorFlag[LinkID] == 1)  // only check the following speical condition when a link is a connector
			{
				int OriginTAZ = m_OutboundConnectorOriginZoneIDAry[FromID][i];
				int DestinationTAZ = m_OutboundConnectorDestinationZoneIDAry[FromID][i];

				if (OriginTAZ >= 1 /* TAZ >=1*/ && DestinationTAZ <= 0 && OriginTAZ != origin_zone)
					continue;  // special feature 1: skip connectors with origin TAZ only and do not belong to this origin zone

				//if (DestinationTAZ >= 1 /* TAZ >=1*/ && OriginTAZ <= 0 && DestinationTAZ != destination_zone)
				//	continue;  // special feature 2: skip connectors with destination TAZ that do not belong to this destination zone

				//if (OriginTAZ >= 1 /* TAZ >=1*/ && OriginTAZ != origin_zone  && DestinationTAZ >= 1 /* TAZ >=1*/ && DestinationTAZ != destination_zone)
				//	continue;  // special feature 3: skip connectors (with both TAZ at two ends) that do not belong to the origin/destination zones

				if (ToID == origin) // special feature 2: no detour at origin
					continue;
			}

			if (debug_flag)  // physical nodes
			{
				TRACE("\n   to node %d", ToID);
			}
			// need to check here to make sure  LabelTimeAry[FromID] is feasible.


			int link_entering_time_interval = g_FindAssignmentIntervalIndexFromTime(LabelTimeAry[FromID]);

			link_entering_time_interval = min(link_entering_time_interval, m_NumberOfSPCalculationIntervals);

			NewDistance = LabelDistanceAry[FromID] + m_LinkTDDistanceAry[LinkID];


			if (distance_cost_flag)
				NewTime = LabelTimeAry[FromID];
			else // distance
				NewTime = LabelTimeAry[FromID] + m_LinkTDTimeAry[LinkID][link_entering_time_interval];  // time-dependent travel times come from simulator

			if (distance_cost_flag)
				NewCost = LabelCostAry[FromID] + m_LinkTDDistanceAry[LinkID];
			else
				NewCost = LabelCostAry[FromID] + m_LinkTDTimeAry[LinkID][link_entering_time_interval];

			DollarCost = 0;
			//if (VOT > 0.01 && g_LinkTDCostAry[LinkID][link_entering_time_interval].m_bMonetaryTollExist)
			//{ // with VOT and toll
			//	AdditionalCostInMin = g_LinkTDCostAry[LinkID][link_entering_time_interval].TollValue[agent_type] / VOT * 60.0f;       // 60.0f for 60 min per hour, costs come from time-dependent tolls, VMS, information provisions
			//	NewCost += AdditionalCostInMin;
			//	DollarCost = g_LinkTDCostAry[LinkID][link_entering_time_interval].TollValue[agent_type];
			//}

			//if (g_LinkTDCostAry[LinkID][link_entering_time_interval].m_bTravelTimeTollExist)
			//{
			//	agent_type = 1; // travel time SO
			//	AdditionalCostInMin = g_LinkTDCostAry[LinkID][link_entering_time_interval].TollValue[agent_type];
			//	if (debug_flag)
			//		TRACE("AdditionalCostInMin = %f\n", AdditionalCostInMin);

			//	NewCost += AdditionalCostInMin;
			//}


			NewDollarCost = LabelDollarCostAry[FromID] + DollarCost;

			if (NewCost < LabelCostAry[ToID]) // be careful here: we only compare cost not time
			{
				if (debug_flag)  // physical nodes
				{
					TRACE("\n         UPDATE to node %d, cost: %f, link travel time %f", ToID, NewCost, m_LinkTDTimeAry[LinkID][link_entering_time_interval]);
				}

				if (NewTime > g_PlanningHorizon - 1)
					NewTime = float(g_PlanningHorizon - 1);

				LabelTimeAry[ToID] = NewTime;
				LabelCostAry[ToID] = NewCost;


				if (bDistanceCostByProductOutput)
				{
					LabelDistanceAry[ToID] = NewDistance;
					LabelDollarCostAry[ToID] = NewDollarCost;


				}



				NodePredAry[ToID] = FromID;
				LinkNoAry[ToID] = LinkID;


				// Dequeue implementation
				//
				if (NodeStatusAry[ToID] == 2) // in the SEList_TD before
				{
					SEList_push_front(ToID);
					NodeStatusAry[ToID] = 1;
				}
				if (NodeStatusAry[ToID] == 0)  // not be reached
				{
					SEList_push_back(ToID);
					NodeStatusAry[ToID] = 1;
				}

				//another condition: in the SEList now: there is no need to put this node to the SEList, since it is already there.
			}

		}      // end of for each link

	} // end of while


	//if (bDistanceCostByProductOutput)  // used mainly for accessibility calculation
	//{

	//	for (i = 0; i < m_NodeSize; i++) // Initialization for all nodes
	//	{
	//		TRACE("Node %d, tt = %f, dist= %f,cost = %f\n", g_NodeVector[i].m_NodeNumber, LabelCostAry[i], LabelDistanceAry[i], LabelDollarCostAry[i]);
	//	}
	//}
	return true;
}


int DTANetworkForSP::FindBestPathWithVOTForSingleAgent_Movement(int origin_zone, int o_node_no, int departure_time, int destination_zone, int d_node_no, int agent_type, float VOT, int PathLinkList[MAX_NODE_SIZE_IN_A_PATH], float &TotalCost, bool bGeneralizedCostFlag, bool debug_flag = false, float PerceptionErrorRatio)
// time -dependent label correcting algorithm with deque implementation
{

	TotalCost = 0;
	debug_flag = false;

	// checking boundary condition for departure time changes
	if (departure_time < g_DemandLoadingStartTimeInMin)
		departure_time = g_DemandLoadingStartTimeInMin;

	if (departure_time > g_DemandLoadingEndTimeInMin)
		departure_time = g_DemandLoadingEndTimeInMin;


	int i;
	float AdditionalCostInMin = 0;

	if (m_OutboundSizeAry[o_node_no] == 0)
		return 0;

	if (m_InboundSizeAry[d_node_no] == 0)
		return 0;

	for (i = 0; i < m_LinkSize; i++) // Initialization for all links
	{
		LinkPredAry[i] = -1;
		LinkStatusAry[i] = 0;

		LinkLabelTimeAry[i] = MAX_SPLABEL;
		LinkLabelCostAry[i] = MAX_SPLABEL;


	}

	// Initialization for o_node_no node: for all outgoing links from o_node_no node

	LinkBasedSEList_clear();
	for (i = 0; i < m_OutboundSizeAry[o_node_no];i++)
	{
		int LinkID = m_OutboundLinkAry[o_node_no][i];

		int link_entering_time_interval = g_FindAssignmentIntervalIndexFromTime(departure_time);

		LinkLabelTimeAry[LinkID] = departure_time + m_LinkTDTimeAry[LinkID][link_entering_time_interval];

		if (bGeneralizedCostFlag == false)
			LinkLabelCostAry[LinkID] = departure_time + m_LinkTDTimeAry[LinkID][link_entering_time_interval];
		else  // for generalized cost, we start from cost of zero, other than start time
			LinkLabelCostAry[LinkID] = 0 + g_LinkTDCostAry[LinkID][link_entering_time_interval].TollValue[0];

		LinkBasedSEList_push_back(LinkID);

		if (m_ToNoAry[LinkID] == d_node_no)  //reach d_node_no on the first link
		{
			PathLinkList[0] = LinkID;
			return 2; // 2 nodes
		}
	}


	int FromLinkID, ToLinkID, NodeID;
	float CostUpperBound = MAX_SPLABEL;

	float NewTime, NewCost;
	while (!LinkBasedSEList_empty())
	{

		FromLinkID = LinkBasedSEList_front();
		LinkBasedSEList_pop_front();

		if (debug_flag)
		{
			TRACE("\nScan from link %d ->%d", g_NodeVector[m_FromNoAry[FromLinkID]].m_NodeID, g_NodeVector[m_ToNoAry[FromLinkID]].m_NodeID);
		}

		LinkStatusAry[FromLinkID] = 2;        //scaned

		for (i = 0; i < m_OutboundMovementSizeAry[FromLinkID]; i++)  // for each arc (i,j) belong to A(i)
		{
			ToLinkID = m_OutboundMovementAry[FromLinkID][i];
			int FromNo = m_FromNoAry[FromLinkID];

			if (debug_flag)  // physical nodes
			{
				if (g_NodeVector[m_ToNoAry[ToLinkID]].m_NodeID == 80125)
				{
					TRACE("Trace!");
				}
				//			TRACE("\n   to link %d, from node: %d, downstream node %d ", ToLinkID, g_NodeVector[FromNo].m_NodeID, g_NodeVector[m_ToNoAry[ToLinkID]].m_NodeID );
			}
			// need to check here to make sure  LabelTimeAry[FromNo] is feasible.


			if (m_LinkConnectorFlag[ToLinkID] == 1)  // only check the following speical condition when a link is a connector
			{
				int OriginTAZ = m_LinkConnectorOriginZoneIDAry[ToLinkID];
				int DestinationTAZ = m_LinkConnectorDestinationZoneIDAry[ToLinkID];

				if (OriginTAZ >= 1 /* TAZ >=1*/ && DestinationTAZ <= 0 && OriginTAZ != origin_zone)
					continue;  // special feature 1: skip connectors with o_node_no TAZ only and do not belong to this o_node_no zone

				if (DestinationTAZ >= 1 /* TAZ >=1*/ && OriginTAZ <= 0 && DestinationTAZ != destination_zone)
					continue;  // special feature 2: skip connectors with d_node_no TAZ that do not belong to this d_node_no zone

				if (OriginTAZ >= 1 /* TAZ >=1*/ && OriginTAZ != origin_zone && DestinationTAZ >= 1 /* TAZ >=1*/ && DestinationTAZ != destination_zone)
					continue;  // special feature 3: skip connectors (with both TAZ at two ends) that do not belong to the o_node_no/d_node_no zones

			}

			if (m_ToNoAry[ToLinkID] == o_node_no) // special feature 2: no detour at o_node_no
				continue;

			int link_entering_time_interval = g_FindAssignmentIntervalIndexFromTime(LinkLabelTimeAry[FromLinkID]);



			//  original code				NewTime	= LinkLabelTimeAry[FromLinkID] + m_LinkTDTimeAry[ToLinkID][link_entering_time_interval] + m_OutboundMovementDelayAry[FromLinkID][i];  // time-dependent travel times come from simulator

			if (bGeneralizedCostFlag)
				NewTime = LinkLabelTimeAry[FromLinkID] + m_LinkTDTimeAry[ToLinkID][link_entering_time_interval] + m_OutboundMovementDelayAry[FromLinkID][i];
			else
			{// non- distance

				float preceived_travel_time = m_LinkTDTimeAry[ToLinkID][link_entering_time_interval];

				NewTime = LinkLabelTimeAry[FromLinkID] + preceived_travel_time + m_OutboundMovementDelayAry[FromLinkID][i];  // time-dependent travel times come from simulator

				double movement_delay_in_min = m_OutboundMovementDelayAry[FromLinkID][i];
				if (movement_delay_in_min >= 90 && debug_flag)
				{
					TRACE("prohibited movement!");
				}
			}



			// original code			NewCost    = LinkLabelCostAry[FromLinkID] + m_LinkTDTimeAry[ToLinkID][link_entering_time_interval] + m_OutboundMovementDelayAry[FromLinkID][i];


						//  road pricing module
			float toll_in_min = 0;			// special feature 5: road pricing
			if (VOT > 0.01 && g_LinkTDCostAry[ToLinkID][link_entering_time_interval].m_bMonetaryTollExist)
			{ // with VOT and toll
				toll_in_min = g_LinkTDCostAry[ToLinkID][link_entering_time_interval].TollValue[agent_type] / VOT * 60.0f;       // 60.0f for 60 min per hour, costs come from time-dependent tolls, VMS, information provisions
			}


			if (debug_flag == 1 && ToLinkID == 5 && link_entering_time_interval >= 29)
			{
				TRACE("problematic node!");

			}

			if (g_LinkTDCostAry[ToLinkID][link_entering_time_interval].m_bTravelTimeTollExist)
			{
				toll_in_min = g_LinkTDCostAry[ToLinkID][link_entering_time_interval].TollValue[agent_type];
				if (debug_flag)
					TRACE("AdditionalCostInMin = %f\n", toll_in_min);
			}
			// special feature 6: update cost
			if (bGeneralizedCostFlag)
				NewCost = LinkLabelCostAry[FromLinkID] + g_LinkTDCostAry[ToLinkID][link_entering_time_interval].TollValue[0];  // we only consider the genralized cost value (without actual travel time)
			else
			{  // non distance cost
				NewCost = LinkLabelCostAry[FromLinkID] + m_LinkTDTimeAry[ToLinkID][link_entering_time_interval] + m_OutboundMovementDelayAry[FromLinkID][i] + toll_in_min;       // costs come from time-dependent tolls, VMS, information provisions

			}


			if (NewCost < LinkLabelCostAry[ToLinkID] && NewCost < CostUpperBound) // special feature 7.1  we only compare cost not time
			{
				if (debug_flag)  // physical nodes
				{
					TRACE("\n         UPDATE to link %d, downstream node %d, cost: %f, link travel time %f", ToLinkID, g_NodeVector[m_ToNoAry[ToLinkID]].m_NodeID, NewCost, m_LinkTDTimeAry[ToLinkID][link_entering_time_interval]);
				}

				if (NewTime > g_PlanningHorizon - 1)
					NewTime = float(g_PlanningHorizon - 1);

				LinkLabelTimeAry[ToLinkID] = NewTime;
				LinkLabelCostAry[ToLinkID] = NewCost;
				LinkPredAry[ToLinkID] = FromLinkID;

				if (m_ToNoAry[ToLinkID] == d_node_no) // special feature 7.2  : update upper bound cost
				{
					CostUpperBound = LinkLabelCostAry[ToLinkID];
				}

				// Dequeue implementation
				//
				if (LinkStatusAry[ToLinkID] == 2) // in the SEList_TD before
				{
					LinkBasedSEList_push_front(ToLinkID);
					LinkStatusAry[ToLinkID] = 1;
				}
				if (LinkStatusAry[ToLinkID] == 0)  // not be reached
				{
					LinkBasedSEList_push_back(ToLinkID);
					LinkStatusAry[ToLinkID] = 1;
				}

				//another condition: in the SEList now: there is no need to put this node to the SEList, since it is already there.
			}
			else
			{

				if (debug_flag == 1 && LinkLabelCostAry[ToLinkID] <= 900)  // physical nodes
				{
					TRACE("\n        not-UPDATEd to link %d, downstream node %d, new cost %f, old cost: %f, link travel time %f", ToLinkID, g_NodeVector[m_ToNoAry[ToLinkID]].m_NodeID, NewCost, LinkLabelCostAry[ToLinkID], m_LinkTDTimeAry[ToLinkID][link_entering_time_interval]);
				}

			}

		}      // end of for each link

	} // end of while


	// post processing: if d_node_no >=0: physical node

	//step 1: find the incoming link has the lowerest cost to the destinatin node

	float min_cost = MAX_SPLABEL;
	int link_id_with_min_cost = -1;
	for (i = 0; i < m_InboundSizeAry[d_node_no]; i++)
	{
		int incoming_link = m_InboundLinkAry[d_node_no][i];
		if (LinkLabelCostAry[incoming_link] < min_cost && LinkPredAry[incoming_link] >= 0)
		{
			min_cost = LinkLabelCostAry[incoming_link];
			link_id_with_min_cost = incoming_link;
		}
	}


	TotalCost = min_cost - departure_time;

	if (link_id_with_min_cost < 0)
	{

		int node_number = g_NodeVector[d_node_no].m_NodeID;
		cout << "d_node_no Node " << node_number << " cannot be reached from o_node_no Node " << g_NodeVector[o_node_no].m_NodeID << endl;
		g_LogFile << "Error: d_node_no Node " << node_number << " cannot be reached from o_node_no Node " << g_NodeVector[o_node_no].m_NodeID << endl;


		int number_of_nodes = FindBestPathWithVOTForSingleAgent_Movement(origin_zone, o_node_no, departure_time, destination_zone, d_node_no, agent_type,
			VOT, PathLinkList, TotalCost, bGeneralizedCostFlag, debug_flag);
		// check if the link(s) have the predecesssor

		for (int ii = 0; ii < number_of_nodes - 1; ii++)
		{

			if (PathLinkList[ii] <0 || PathLinkList[ii]> g_LinkVector.size())
			{
				g_ProgramStop();

			}
			cout << "Link " << g_LinkVector[PathLinkList[ii]]->m_FromNodeID << "->" << g_LinkVector[PathLinkList[ii]]->m_ToNodeID << " with cost " << LinkLabelCostAry[PathLinkList[ii]] << endl;
		}


		getchar();
	}

	//step 2 trace the incoming link to the first link in o_node_no node

	int LinkSize = 0;
	temp_reversed_PathLinkList[LinkSize++] = link_id_with_min_cost;  // last link to d_node_no
	int PrevLinkID = LinkPredAry[link_id_with_min_cost];

	if (PrevLinkID == -1)
	{
		TRACE("Error!");

	}
	temp_reversed_PathLinkList[LinkSize++] = PrevLinkID;   //second last link

	while (PrevLinkID != -1 && LinkSize < MAX_NODE_SIZE_IN_A_PATH) // scan backward in the predessor array of the shortest path calculation results
	{
		ASSERT(LinkSize < MAX_NODE_SIZE_IN_A_PATH - 1);

		if (LinkPredAry[PrevLinkID] != -1)
		{
			temp_reversed_PathLinkList[LinkSize++] = LinkPredAry[PrevLinkID];
		}
		PrevLinkID = LinkPredAry[PrevLinkID];
	}

	int j = 0;
	for (i = LinkSize - 1; i >= 0; i--)
	{
		PathLinkList[j++] = temp_reversed_PathLinkList[i];
		if (debug_flag)
		{
			TRACE("\n  link no. %d, %d, %d ->%d", i, temp_reversed_PathLinkList[i]
				, m_FromNoAry[temp_reversed_PathLinkList[i]], m_ToNoAry[temp_reversed_PathLinkList[i]]);
		}

	}

	if (debug_flag)
	{
		TRACE("\nPath sequence end, cost = ..%f\n", min_cost);
	}

	return LinkSize + 1;  // as node size

}




/// above are shortest path algorithms

extern ofstream g_LogFile;

extern double g_simulation_time;

void output_debug_info_on_screen(int simulation_time_in_min, int simulation_step, string str)
{
	int start_time__debugging = 32 * 60;

	if (simulation_time_in_min >= start_time__debugging)
	{
		cout << "simulation step = " << simulation_step << "" << str << endl;

	}
}

bool g_VehicularSimulation(int DayNo, double CurrentTime, int meso_simulation_time_interval_no, e_traffic_flow_model global_Traffic_Flow_Model_Flag)
{

	int simulation_step_no = 1;

	int trace_step = 14;

	std::set<DTANode*>::iterator iterNode;
	std::set<DTALink*>::iterator iterLink;
	std::map<int, DTAAgent*>::iterator iterVM;

	int time_stamp_in_min = int(CurrentTime + 0.0001);

	g_simulation_time = CurrentTime;

	std::list<struc_Agent_item>::iterator vii;

	int Agent_id_trace = 1000;
	int link_id_trace = -1;


	bool debug_flag = true;


	//DTALite:
	// vertical queue data structure
	// each link  ExitQueue, EntranceQueue: (AgentID, ReadyTime)

	// we update departure based information every 5 min
	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "update departure based information every 5 min");

	// user_defined information updating
	int time_clock_in_min = g_DemandLoadingStartTimeInMin + meso_simulation_time_interval_no / g_simulation_time_intervals_per_min;
	int time_clock_in_second = g_DemandLoadingStartTimeInMin * 60 + meso_simulation_time_interval_no * g_simulation_time_interval_step;;


	// load Agent into network

	// step 1: scan all the Agents, if a Agent's start time >= CurrentTime, and there is available space in the first link,
	// load this Agent into the ready queue

	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "scan all the Agents to load them into entry queue");
	// comment: we use std::map here as the g_AgentMap map is sorted by departure time.
	// At each iteration, we start  the last loaded id, and exit if the departure time of a Agent is later than the current time.


	int simulation_time_no = time_stamp_in_min * g_simulation_time_intervals_per_min + meso_simulation_time_interval_no % g_simulation_time_intervals_per_min;


	for (int Agent_v = 0; Agent_v < g_AgentTDListMap[simulation_time_no].m_AgentIDVector.size(); Agent_v++)
	{
		int agent_id = g_AgentTDListMap[simulation_time_no].m_AgentIDVector[Agent_v];


		//	cout <<" generated agent_id = " << agent_id << endl;

		DTAAgent* pAgent;
		if (g_AgentMap.find(agent_id) == g_AgentMap.end())
		{
			continue;

		}
		pAgent = g_AgentMap[agent_id];
	}

	int number_of_threads = 1;

	// calculate pre-trip paths for Agents
	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "operations in loading buffer ");


	for (int Agent_v = 0; Agent_v < g_AgentTDListMap[simulation_time_no].m_AgentIDVector.size(); Agent_v++)
	{
		int agent_id = g_AgentTDListMap[simulation_time_no].m_AgentIDVector[Agent_v];

		DTAAgent* pAgent;
		if (g_AgentMap.find(agent_id) == g_AgentMap.end())
		{
			continue;

		}
		pAgent = g_AgentMap[agent_id];
		_proxy_simulation_log(1, simulation_time_no, CurrentTime, pAgent->m_AgentID,
			"Step 0: Generate Agent id from TDlistMap = %d\n", pAgent->m_AgentID);



		if (pAgent->m_bLoaded == false)  // not being loaded
		{

			if (pAgent->m_NodeSize >= 2)  // with physical path
			{
				int FirstLink = pAgent->m_LinkAry[0].LinkNo;

				DTALink* p_link = g_LinkVector[FirstLink];

				if (p_link != NULL)
				{
					struc_Agent_item vi;
					vi.veh_id = pAgent->m_AgentID;

					int period_no = 0;
					if (p_link->m_traffic_flow_model == tfm_BPR)
						period_no = g_FindAssignmentIntervalIndexFromTime(pAgent->m_DepartureTime);


					vi.event_time_stamp = pAgent->m_DepartureTime + p_link->GetFreeMovingTravelTime(p_link->m_traffic_flow_model, DayNo, CurrentTime, period_no);  // unit: min


					vi.veh_next_node_number = -1;  // initialization

					if (pAgent->m_NodeSize >= 3)   // fetch the next link's downstream node number
					{
						int NextLink = pAgent->m_LinkAry[1].LinkNo;

						DTALink* p_next_link = g_LinkVector[NextLink];
						vi.veh_next_node_number = p_next_link->m_ToNodeID;
					}


					pAgent->m_bLoaded = true;
					pAgent->m_LeavingTimeFromLoadingBuffer = CurrentTime;

					pAgent->m_SimLinkSequenceNo = 0;

					_proxy_simulation_log(2, simulation_time_no, CurrentTime, vi.veh_id,
						"Step 0: Insert vhc %d to its first link %d's loading buffer, with departure time event stamp %5.2f -> %5.2f\n", vi.veh_id, FirstLink, pAgent->m_DepartureTime, vi.event_time_stamp);

					p_link->LoadingBuffer.push_back(vi);  // need to fine-tune
					g_Number_of_GeneratedAgents += 1;

					if (time_stamp_in_min < g_NetworkMOEAry.size())
					{

						g_NetworkMOEAry[time_stamp_in_min].Flow_in_a_min += 1;
						g_NetworkMOEAry[time_stamp_in_min].CumulativeInFlow = g_Number_of_GeneratedAgents;
					}


				}

			}  // with physical path


		}

	}

	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "operations in loading buffer ");

	// loading buffer
	if (meso_simulation_time_interval_no >= trace_step)
		g_ProgramTrace("loading buffer");

	int link_size = g_LinkVector.size();
#pragma omp parallel for
	for (int li = 0; li < link_size; li++)
	{
		DTALink * pLink = g_LinkVector[li];

		while (pLink->LoadingBuffer.size() > 0 && pLink->GetNumberOfLanes(DayNo, CurrentTime) > 0.01)  // no load Agent into a blocked link
		{
			struc_Agent_item vi = pLink->LoadingBuffer.front();
			// we change the time stamp here to reflect the actual loading time into the network, especially for blocked link
			if (g_floating_point_value_less_than_or_eq_comparison(vi.event_time_stamp, CurrentTime))
				vi.event_time_stamp = CurrentTime;

			_proxy_simulation_log(2, simulation_time_no, CurrentTime, vi.veh_id,
				"Step 1: Load vhc %d from loading buffer to physical link %d->%d\n", vi.veh_id, pLink->m_FromNodeID, pLink->m_ToNodeID);

			int NumberOfAgentsOnThisLinkAtCurrentTime = (int)(pLink->CFlowArrivalCount - pLink->CFlowDepartureCount);

			// determine link in capacity
			float AvailableSpaceCapacity = pLink->m_AgentSpaceCapacity - NumberOfAgentsOnThisLinkAtCurrentTime;

			// if we use BPR function, no density constraint is imposed --> TrafficFlowModelFlag == 0
			if (pLink->m_traffic_flow_model == tfm_point_queue || (pLink->m_AgentSpaceCapacity <= 2 /* micro simulation */ && AvailableSpaceCapacity >= pLink->m_AgentSpaceCapacity) || AvailableSpaceCapacity >= max(2, pLink->m_AgentSpaceCapacity*(1 - g_MaxDensityRatioForAgentLoading)))  // at least 10% remaining capacity or 2 Agent space is left
			{
				pLink->LoadingBuffer.pop_front();

				_proxy_simulation_log(3, simulation_time_no, CurrentTime, vi.veh_id,
					"Step 1: Add vhc %d to entrance queue of physical link %d->%d\n", vi.veh_id, pLink->m_FromNodeID, pLink->m_ToNodeID);

				ASSERT(vi.veh_id >= 0);

				pLink->EntranceQueue.push_back(vi);
				pLink->CFlowArrivalCount += 1;



				int agent_type = g_AgentMap[vi.veh_id]->m_DemandType;
				//				pLink->m_LoadingBufferWaitingTime += (CurrentTime - pAgent->m_DepartureTime);

				DTAAgent* pAgent = g_AgentMap[vi.veh_id];

				// mark the actual leaving time from the loading buffer, so that we can calculate the exact time for traversing the physical net

				pAgent->m_TollDollarCost += pLink->GetTollRateInDollar(DayNo, CurrentTime, agent_type);



			}
			else
			{
				_proxy_simulation_log(2, simulation_time_no, CurrentTime, vi.veh_id,
					"Step 1:  load vhc %d failed due to congested physical link %d->%d\n", vi.veh_id, pLink->m_FromNodeID, pLink->m_ToNodeID);

				break;  // physical road is too congested, wait for next time interval to load
			}

		}
	}

	// step 2: move Agents from EntranceQueue To ExitQueue, if ReadyTime <= CurrentTime)
	if (meso_simulation_time_interval_no >= trace_step)
		g_ProgramTrace("move Agents from EntranceQueue To ExitQueue");

	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "move Agents from EntranceQueue To ExitQueue ");

#pragma omp parallel for
	for (int li = 0; li < link_size; li++)
	{

		DTALink * pLink = g_LinkVector[li];




		while (pLink->EntranceQueue.size() > 0)  // if there are Agents in the entrance queue
		{

			struc_Agent_item vi = pLink->EntranceQueue.front();
			double PlannedArrivalTime = vi.event_time_stamp;

			if (pLink->m_FromNodeID == 1 && pLink->m_ToNodeID == 5)
			{
				TRACE("Time %f, PlannedArrivalTime = %f,  CurrentTime = %f, \n",
					CurrentTime, PlannedArrivalTime, CurrentTime);

			}

			if (g_floating_point_value_less_than_or_eq_comparison(PlannedArrivalTime, CurrentTime))  // link (downstream) arrival time within the simulation time interval
			{
				pLink->EntranceQueue.pop_front();
				pLink->ExitQueue.push_back(vi);

				_proxy_simulation_log(3, simulation_time_no, CurrentTime, vi.veh_id,
					"Step 2: Vhc %d with planned arrival time %f, moves from entrance queue to exit queue on link %d->%d\n", vi.veh_id, PlannedArrivalTime, g_LinkVector[li]->m_FromNodeID, g_LinkVector[li]->m_ToNodeID);

			}
			else
			{
				_proxy_simulation_log(4, simulation_time_no, CurrentTime, vi.veh_id,
					"Step 2: Vhc %d with planned arrival time %f, wait... on link %d->%d\n", vi.veh_id, PlannedArrivalTime, g_LinkVector[li]->m_FromNodeID, g_LinkVector[li]->m_ToNodeID);

				break;  // the Agent's actual arrival time is later than the current time, so we exit from the loop, stop searching
			}

		}

	}

	// step 3: determine link in and out capacity
	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "determine link in and out capacity ");



#pragma omp parallel for
	for (int li = 0; li < link_size; li++)
	{

		DTALink* pLink = g_LinkVector[li];

		if (CurrentTime >= 6 && pLink->m_FromNodeID == 355 && pLink->m_ToNodeID == 151)
		{
			TRACE("");
		}

		float PerHourCapacity = pLink->GetHourlyPerLaneCapacity(CurrentTime);  // static capacity from BRP function

		float PerHourCapacityAtCurrentSimulatioInterval = PerHourCapacity;


		// determine link out capacity

		bool OutFlowFlag = true;

		float number_of_lanes = pLink->GetNumberOfLanes(DayNo, CurrentTime, OutFlowFlag);


		float number_Agents_per_simulation_interval = PerHourCapacityAtCurrentSimulatioInterval * g_DTASimulationInterval_InMin / 60.0f * number_of_lanes; //60 --> cap per min --> unit # of Agent per simulation interval

		float Capacity = number_Agents_per_simulation_interval;
		// use integer number of Agents as unit of capacity

		if (CurrentTime >= (g_DemandLoadingEndTimeInMin + g_RelaxInFlowConstraintAfterDemandLoadingTime))  // g_RelaxInFlowConstraintAfterDemandLoadingTime min after demand loading period, do not apply in capacity constraint
		{
			Capacity = Capacity * 3;  // dramatically increase the capacity to send all Agents out after the demand horizon + 60 min
		}


		pLink->m_CumulativeOutCapacityCount += Capacity;  // floating point variable to record number of Agents can be sent out based on floating point capacity
		pLink->LinkOutCapacity = (int)(pLink->m_CumulativeOutCapacityCount) - pLink->m_CumulativeOutCapacityCountAtPreviousInterval;

		pLink->m_CumulativeOutCapacityCountAtPreviousInterval += pLink->LinkOutCapacity; // accumulate the # of out Agents from the capacity, this is an inter number



		int NumberOfAgentsOnThisLinkAtCurrentTime = (int)(pLink->CFlowArrivalCount - pLink->CFlowDepartureCount);

		float fLinkInCapacity = 99999.0; // by default we have point queue

		//output: fLinkInCapacity
		if (pLink->m_traffic_flow_model >= 2 && g_LinkTypeMap[pLink->m_link_type].IsFreeway())  // apply spatial link in capacity for spatial queue models( with spillback and shockwave)
		{
			// determine link in capacity
			float AvailableSpaceCapacity = pLink->m_AgentSpaceCapacity - NumberOfAgentsOnThisLinkAtCurrentTime;

			if (g_LinkTypeMap[pLink->m_link_type].IsFreeway())
				fLinkInCapacity = min(AvailableSpaceCapacity, 1800 * g_DTASimulationInterval_InMin / 60.0f* pLink->GetInflowNumberOfLanes(DayNo, CurrentTime));
			else // non freeway links
				fLinkInCapacity = AvailableSpaceCapacity;

			if (fLinkInCapacity < 0)
				fLinkInCapacity = 0;


			//			TRACE(" time %5.2f, SC: %5.2f, MFR %5.2f\n",CurrentTime, AvailableSpaceCapacity, MaximumFlowRate);

			// the inflow capcaity is the minimum of (1) incoming maximum flow rate (determined by the number of lanes) and (2) available space capacty  on the link.
			// use integer number of Agents as unit of capacity

			if (pLink->m_traffic_flow_model == tfm_newells_model && g_LinkTypeMap[pLink->m_link_type].IsFreeway())  // Newell's model on freeway only
			{
				if (meso_simulation_time_interval_no >= pLink->m_BackwardWaveTimeInSimulationInterval) /// we only apply backward wave checking after the simulation time is later than the backward wave speed interval
				{

					int t_residual_minus_backwardwaveTime = max(0, meso_simulation_time_interval_no - pLink->m_BackwardWaveTimeInSimulationInterval) % MAX_TIME_INTERVAL_ADCURVE;

					float VehCountUnderJamDensity = pLink->m_Length * pLink->GetNumberOfLanes(DayNo, CurrentTime) *pLink->m_KJam;
					// when there is a capacity reduction, in the above model, we assume the space capacity is also reduced proportional to the out flow discharge capacity,
					// for example, if the out flow capacity is reduced from 5 lanes to 1 lanes, say 10K Agents per hour to 2K Agents per hour,
					// our model will also reduce the # of Agents can be stored on the incident site by the equivalent 4 lanes.
					// this might cause the dramatic speed change on the incident site, while the upstream link (with the original space capacity) will take more time to propapate the speed change compared to the incident link.
					// to do list: we should add another parameter of space capacity reduction ratio to represent the fact that the space capacity reduction magnitude is different from the outflow capacity reduction level,
					// particularly for road construction areas on long links, with smooth barrier on the merging section.
					int N_Arrival_Now_Constrainted = (int)(pLink->m_CumuDeparturelFlow[t_residual_minus_backwardwaveTime] + VehCountUnderJamDensity);  //pLink->m_Length 's unit is mile
					int t_residual_minus_1 = max(0, meso_simulation_time_interval_no - 1) % MAX_TIME_INTERVAL_ADCURVE;

					ASSERT(t_residual_minus_1 >= 0);
					ASSERT(t_residual_minus_1 < MAX_TIME_INTERVAL_ADCURVE);
					int N_Now_minus_1 = (int)pLink->m_CumuArrivalFlow[t_residual_minus_1];
					int Flow_allowed = N_Arrival_Now_Constrainted - N_Now_minus_1;

					//TRACE("\ntime %d, D:%d,A%d",meso_simulation_time_interval_no,pLink->m_CumuDeparturelFlow[t_residual_minus_1],pLink->m_CumuArrivalFlow[t_residual_minus_1]);

					if (Flow_allowed < 0)
						Flow_allowed = 0;

					if (fLinkInCapacity > Flow_allowed)
					{
						fLinkInCapacity = Flow_allowed;

						if (Flow_allowed == 0 && N_Arrival_Now_Constrainted > 0)
						{
							if (pLink->m_JamTimeStamp > CurrentTime)
								pLink->m_JamTimeStamp = CurrentTime;

							//							g_LogFile << "Queue spillback"<< CurrentTime <<  g_NodeVector[pLink->m_FromNodeNo] << " -> " <<	g_NodeVector[pLink->m_ToNodeNo] << " " << pLink->LinkInCapacity << endl;

							// update traffic state
							int CurrentTime_int = (int)(CurrentTime);

							if (CurrentTime_int >= pLink->m_LinkMOEAry.size())
							{
								cout << "CurrentTime_int >= pLink->m_LinkMOEAry.size()";
								g_ProgramStop();
							}

							pLink->m_LinkMOEAry[CurrentTime_int].TrafficStateCode = 2;  // 2: fully congested

							//							TRACE("Queue spillback at %d -> %d\n", g_NodeVector[pLink->m_FromNodeNo], g_NodeVector[pLink->m_ToNodeNo]);
						}
					}
				}
			}




			float InflowRate = number_Agents_per_simulation_interval * g_MinimumInFlowRatio;

			if (CurrentTime >= (g_DemandLoadingEndTimeInMin + g_RelaxInFlowConstraintAfterDemandLoadingTime))  // g_RelaxInFlowConstraintAfterDemandLoadingTime min after demand loading period, do not apply in capacity constraint
			{
				InflowRate = number_Agents_per_simulation_interval;
			}

			if (fLinkInCapacity < InflowRate)  // minimum inflow capacity to keep the network flowing
			{
				fLinkInCapacity = InflowRate;
			}



		}




		// finally we convert the floating-point capacity to integral capacity in terms of number of Agents

		pLink->m_CumulativeInCapacityCount += fLinkInCapacity;
		pLink->LinkInCapacity = (int)(pLink->m_CumulativeInCapacityCount) - pLink->m_CumulativeInCapacityCountAtPreviousInterval;

		pLink->m_CumulativeInCapacityCountAtPreviousInterval += pLink->LinkInCapacity;


		//
	//special condition 1: ramp:

	//overwrite ramp in flow capacity
		if (g_LinkTypeMap[pLink->m_link_type].IsRamp())
		{

			if (pLink->m_bOnRampType == true && g_settings.use_point_queue_model_for_on_ramps)
			{
				pLink->LinkInCapacity = 99999; // reset in flow capacity constraint
			}

			if (pLink->m_bOnRampType == false && g_settings.use_point_queue_model_for_off_ramps)
			{
				pLink->LinkInCapacity = 99999; // reset outflow flow capacity constraint
			}
		}



		_proxy_simulation_log(4, simulation_time_no, CurrentTime, -1,
			"Step 3.1: Capacity calculation Link: %d -> %d: Incapacity int= %d, %f, OutCapacity: %d\n", g_NodeVector[pLink->m_FromNodeNo].m_NodeID, g_NodeVector[pLink->m_ToNodeNo].m_NodeID, pLink->LinkInCapacity, fLinkInCapacity, pLink->LinkOutCapacity);

	}
	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "distribute link in capacity to different incoming links ");


	// distribute link in capacity to different incoming links
#pragma omp parallel for
	for (int li = 0; li < link_size; li++)
	{
		DTALink* pLink = g_LinkVector[li];
		unsigned int il;


		if (pLink->m_bMergeFlag >= 1)
		{
			int TotalInFlowCount = 0;

			for (il = 0; il < pLink->MergeIncomingLinkVector.size(); il++)
			{
				TotalInFlowCount += g_LinkVector[pLink->MergeIncomingLinkVector[il].m_LinkNo]->ExitQueue.size();  // count vehiciles waiting at exit queue

			}

			if (TotalInFlowCount > pLink->LinkInCapacity)  // total demand > supply
			{


				if (pLink->m_bMergeFlag == 1 || pLink->m_bMergeFlag == 2)  // merge with mulitple freeway/onramp links only, or for freeway/on-ramp merge when not using freeval model
				{


					if (pLink->MergeIncomingLinkVector.size() == 2) // two incoming links
					{

						for (il = 0; il < pLink->MergeIncomingLinkVector.size(); il++)
						{
							int il_other_link = 1 - il; // il ==0 -> il_other_link = 1;, il ==1 -> il_other_link = 0;
							int demand_on_other_link = g_LinkVector[pLink->MergeIncomingLinkVector[il_other_link].m_LinkNo]->ExitQueue.size();
							pLink->MergeIncomingLinkVector[il].m_LinkInRemainingCapaityPerSimuInterval =
								pLink->LinkInCapacity - demand_on_other_link;
							// remaining capacity = total in capacity  - demand on other link
						}

					}

					// default to link in capacity ratio
					for (il = 0; il < pLink->MergeIncomingLinkVector.size(); il++)
					{

						float capacity1 = pLink->LinkInCapacity * pLink->MergeIncomingLinkVector[il].m_LinkInCapacityRatio;
						// capacity from proportional lane assignment

						float capacity2 = pLink->MergeIncomingLinkVector[il].m_LinkInRemainingCapaityPerSimuInterval;
						// remaining residual capacity from total inflow capacity

						float capacity3 = g_LinkVector[pLink->MergeIncomingLinkVector[il].m_LinkNo]->LinkOutCapacity;
						//// outflow capacity of the incoming link
						float LinkOutCapacity = min(capacity3, max(capacity1, capacity2));

						//							float capacity3 = g_LinkVector [pLink->MergeIncomingLinkVector[il].m_LinkNo]->LinkOutCapacity;
						// outflow capacity of the incoming link

						// priority based model is used here from  Daganzo's Priority-Based Merge Model  (1995)
						int LinkOutCapacity_int = 0;


						float PrevCumulativeMergeOutCapacityCount = g_LinkVector[pLink->MergeIncomingLinkVector[il].m_LinkNo]->m_CumulativeMergeOutCapacityCount;
						g_LinkVector[pLink->MergeIncomingLinkVector[il].m_LinkNo]->m_CumulativeMergeOutCapacityCount += LinkOutCapacity;
						LinkOutCapacity_int = (int)g_LinkVector[pLink->MergeIncomingLinkVector[il].m_LinkNo]->m_CumulativeMergeOutCapacityCount -
							(int)PrevCumulativeMergeOutCapacityCount;
						g_LinkVector[pLink->MergeIncomingLinkVector[il].m_LinkNo]->LinkOutCapacity = LinkOutCapacity_int;

						DTALink* p_incoming_link = g_LinkVector[pLink->MergeIncomingLinkVector[il].m_LinkNo];
						_proxy_simulation_log(3, simulation_time_no, CurrentTime, -1,
							"Step 3.2: Capacity calculation for merging incoming link: %d -> %d: OutCapacity: %d\n",
							g_NodeVector[p_incoming_link->m_FromNodeNo].m_NodeID,
							g_NodeVector[p_incoming_link->m_ToNodeNo].m_NodeID,
							p_incoming_link->LinkOutCapacity);

					} // for each incoming link
				}  //   merge with mulitple freeway/onramp links only, or for freeway/on-ramp merge when not using freeval model
			}  // total demand > supply
		} // m_bMergeFlag >= 1
	}  // 		// distribute link in capacity to different incoming links



// step 4: move Agents from ExitQueue to next link's EntranceQueue, if there is available capacity
// NewTime = ReadyTime + FFTT(next link)
	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "move Agents from ExitQueue to next link's EntranceQueue, if there is available capacity ");

	if (meso_simulation_time_interval_no >= trace_step)
		g_ProgramTrace("step 4: move Agents from ExitQueue to next link's EntranceQueue");

	// step 4.1: calculate movement capacity per simulation interval for movements defined in input_movement.csv

	int node_size = g_NodeVector.size();

	//for each node, we scan each incoming link in a randomly sequence, based on meso_simulation_time_interval_no
	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, " for each node, we scan each incoming link in a randomly sequence ");


	if (meso_simulation_time_interval_no >= trace_step)
		g_ProgramTrace("step 4: for each node, we scan each incoming link in a randomly sequence");

#pragma omp parallel for
	for (int node = 0; node < node_size; node++)
	{


		_proxy_simulation_log(4, simulation_time_no, CurrentTime, -1,
			"Step 4: node transfer at node %d\n", g_NodeVector[node].m_NodeID);


		int NextLink;
		DTALink* p_Nextlink;

		int IncomingLinkSize = g_NodeVector[node].m_IncomingLinkVector.size();
		int incoming_link_count = 0;

		int incoming_link_sequence = meso_simulation_time_interval_no % max(1, IncomingLinkSize);  // random start point

		//if (g_NodeVector[node].m_NodeID == 67 && CurrentTime >= 5)
		//{
		//	cout << "node 67, t=" << CurrentTime << " l=" << incoming_link_sequence << endl;
		//}


		while (incoming_link_count < IncomingLinkSize)
		{

			int li = g_NodeVector[node].m_IncomingLinkVector[incoming_link_sequence];

			DTALink* pLink = g_LinkVector[li];
			_proxy_simulation_log(4, simulation_time_no, CurrentTime, -1,
				"Step 4.1: node transfer, Link: %d -> %d: capacity=%d, queue size=%d \n",
				g_NodeVector[pLink->m_FromNodeNo].m_NodeID, g_NodeVector[pLink->m_ToNodeNo].m_NodeID,
				pLink->LinkOutCapacity, pLink->ExitQueue.size());

			if (pLink->m_FromNodeID == 1 && pLink->m_ToNodeID == 3 && CurrentTime >= 430)
			{
				TRACE("");
			}

			// Agent_out_count is the minimum of LinkOutCapacity and ExitQueue Size

			int Agent_out_count = pLink->LinkOutCapacity;


			float Agent_out_demand = 0;
			//pre-calculate the number of Agents can be sent out based on PCE
			// LinkOutCapacity is based on PCE

			float Agent_demand_counter = 0;
			int agent_counter = 0;
			for (std::list<struc_Agent_item>::iterator i = pLink->ExitQueue.begin(); i != pLink->ExitQueue.end(); ++i)
			{

				struc_Agent_item vi = (*i);
				DTAAgent* pAgent = g_AgentMap[vi.veh_id];

				Agent_demand_counter += pAgent->m_PCE;

				if (Agent_demand_counter > Agent_out_count)
				{
					float difference = Agent_demand_counter - Agent_out_count;
					float chance_to_go = (1 - difference);

					float RandomNumber = pLink->GetRandomRatio();

					if (RandomNumber < chance_to_go)
					{
						agent_counter++; // allow the agent to go
					}
					else
					{
					}

					break;
				}

				agent_counter++;

			}

			Agent_out_count = agent_counter;  // assign  the agent counter back to Agent_out_count.

			//			g_LogFile << "link out capaity:"<< CurrentTime << " "  << g_NodeVector[pLink->m_FromNodeNo] << " ->" << g_NodeVector[pLink->m_ToNodeNo]<<" Cap:" << Agent_out_count<< "queue:" << pLink->ExitQueue.size() << endl;
			int through_blocking_count = 0;
			int blocking_node_number = 0;


			// we first discharge through queue and then left-turn queue
			while ((pLink->ExitQueue.size() > 0 && Agent_out_count > 0))  // either condition 1 or condition 2 is met
			{
				struc_Agent_item vi;

				if (pLink->ExitQueue.size() >= 10)
					TRACE("");

				if (pLink->ExitQueue.size() > 0 && Agent_out_count > 0 && through_blocking_count == 0) // there is through capacity // condition 1  // not blocked
				{
					vi = pLink->ExitQueue.front();
				}
				else
				{ // there is not left out capacity
					break;

				}


				int Agent_id = vi.veh_id;

				if (Agent_id == Agent_id_trace)
					TRACE("simulation link sequence no. %d", g_AgentMap[Agent_id]->m_SimLinkSequenceNo);

				// record arrival time at the downstream node of current link
				int link_sequence_no = g_AgentMap[Agent_id]->m_SimLinkSequenceNo;

				int t_link_arrival_time = 0;

				if (link_sequence_no >= 1)
				{
					t_link_arrival_time = int(g_AgentMap[Agent_id]->m_LinkAry[link_sequence_no - 1].AbsArrivalTimeOnDSN);
				}
				else
				{
					t_link_arrival_time = int(g_AgentMap[Agent_id]->m_DepartureTime);
				}

				// not reach destination yet

				int number_of_links = g_AgentMap[Agent_id]->m_NodeSize - 1;
				float ArrivalTimeOnDSN = 0;
				if (g_floating_point_value_less_than_or_eq_comparison(CurrentTime - g_DTASimulationInterval_InMin, vi.event_time_stamp))
					// arrival at previous interval
				{  // no delay
					ArrivalTimeOnDSN = vi.event_time_stamp;
				}
				else
				{  // delayed at previous time interval, discharge at CurrentTime
					ArrivalTimeOnDSN = CurrentTime;
				}

				g_AgentMap[Agent_id]->m_LinkAry[link_sequence_no].AbsArrivalTimeOnDSN = ArrivalTimeOnDSN;

				float TravelTime = 0;

				if (link_sequence_no >= 1)
				{
					TravelTime = g_AgentMap[Agent_id]->m_LinkAry[link_sequence_no].AbsArrivalTimeOnDSN -
						g_AgentMap[Agent_id]->m_LinkAry[link_sequence_no - 1].AbsArrivalTimeOnDSN;
					ASSERT(TravelTime < 9000);

				}
				else
				{
					TravelTime = g_AgentMap[Agent_id]->m_LinkAry[link_sequence_no].AbsArrivalTimeOnDSN -
						g_AgentMap[Agent_id]->m_DepartureTime;
					ASSERT(TravelTime < 9000);

				}


				//perform drop off task regardless of reaching destination or not

				if (g_AgentMap[Agent_id]->m_SimLinkSequenceNo < number_of_links - 1)  // not reach destination yet
				{

					// advance to next linkx

					if (Agent_id == Agent_id_trace)
						TRACE("simulation link sequence no. %d", g_AgentMap[Agent_id]->m_SimLinkSequenceNo);

					NextLink = g_AgentMap[Agent_id]->m_LinkAry[g_AgentMap[Agent_id]->m_SimLinkSequenceNo + 1].LinkNo;
					p_Nextlink = g_LinkVector[NextLink];

					if (p_Nextlink == NULL)
					{
						TRACE("Error at Agent %d,", Agent_id);
						ASSERT(p_Nextlink != NULL);
					}

					//

					int NumberOfAgentsOnNextLinkAtCurrentTime = (int)(p_Nextlink->CFlowArrivalCount - p_Nextlink->CFlowDepartureCount);

					bool bNextlinkCapacity = true;

					if (p_Nextlink->LinkInCapacity <= 0)
						bNextlinkCapacity = false;

					if (bNextlinkCapacity) // if there is available spatial capacity on next link, then move to next link, otherwise, stay on the current link
					{

						float TimeOnNextLink = 0;

						// apply minimum moving time constraints: add extra travel time

						float minimum_moving_time = pLink->m_FreeFlowTravelTime;
						if (TravelTime < minimum_moving_time)
						{
							ArrivalTimeOnDSN += (minimum_moving_time - TravelTime);
							TravelTime = minimum_moving_time;
						}


						g_AgentMap[Agent_id]->m_Delay += (TravelTime - pLink->m_FreeFlowTravelTime);

						// finally move to next link
						g_AgentMap[Agent_id]->m_SimLinkSequenceNo = g_AgentMap[Agent_id]->m_SimLinkSequenceNo + 1;


						vi.veh_id = Agent_id;

						int period_no = 0;
						if (pLink->m_traffic_flow_model == tfm_BPR)
							period_no = g_FindAssignmentIntervalIndexFromTime(g_AgentMap[Agent_id]->m_DepartureTime);


						float FFTT = p_Nextlink->GetFreeMovingTravelTime(pLink->m_traffic_flow_model, DayNo, CurrentTime, period_no);


						vi.event_time_stamp = ArrivalTimeOnDSN + FFTT;

						vi.veh_next_node_number = -1;  // initialization

						// check service node condition
						// if the curr is a stop node, and it accepts the current demand type, then
						//send the agent id to the pick up list at node


						if (g_AgentMap[Agent_id]->m_SimLinkSequenceNo < number_of_links - 2)  // not reach the previous link before the destination yet
						{

							int NextLinkOfNextLink = g_AgentMap[Agent_id]->m_LinkAry[g_AgentMap[Agent_id]->m_SimLinkSequenceNo + 2].LinkNo;
							DTALink* p_NextNextlink = g_LinkVector[NextLinkOfNextLink];

							if (p_NextNextlink == NULL)
							{
								TRACE("Error at Agent %d,", Agent_id);
								ASSERT(p_Nextlink != NULL);
							}

							vi.veh_next_node_number = p_NextNextlink->m_ToNodeID;
						}

						// remark: when - TimeOnNextLink < 0, it means there are few seconds of left over on current link, which should be spent on next link
						///

						if (debug_flag && p_Nextlink->m_FromNodeID == 34 && p_Nextlink->m_ToNodeID == 30 && CurrentTime >= 860)
						{
							TRACE("Step 4: Time %f, Link: %d -> %d: vi %d, exit time: %f, FFTT = %f\n",
								CurrentTime, p_Nextlink->m_FromNodeID, p_Nextlink->m_ToNodeID,
								vi.veh_id, vi.event_time_stamp, FFTT);
						}



						pLink->ExitQueue.pop_front();
						p_Nextlink->EntranceQueue.push_back(vi);
						p_Nextlink->CFlowArrivalCount += 1;

						//for (int backward_link = g_AgentMap[Agent_id]->m_SimLinkSequenceNo +1; backward_link >= g_AgentMap[Agent_id]->m_SimLinkSequenceNo - g_AgentMap[Agent_id]->m_PCE; backward_link--)
	// comment out Xuesong, 2018					//{

						//	g_LinkVector[g_AgentMap[Agent_id]->m_LinkAry[backward_link]]->CFlowArrivalCount += 1;
						//	g_LinkVector[g_AgentMap[Agent_id]->m_LinkAry[backward_link-1]]->CFlowDepartureCount += 1;
						//}

						int agent_type = g_AgentMap[vi.veh_id]->m_DemandType;
						DTAAgent* pAgent = g_AgentMap[vi.veh_id];

						pAgent->m_TollDollarCost += p_Nextlink->GetTollRateInDollar(DayNo, CurrentTime, agent_type);

						if (t_link_arrival_time < pLink->m_LinkMOEAry.size())
						{
							pLink->m_LinkMOEAry[t_link_arrival_time].TotalTravelTime += TravelTime;
							pLink->m_LinkMOEAry[t_link_arrival_time].TotalFlowCount += 1;
						}


						pLink->CFlowDepartureCount += 1;
						pLink->departure_count += 1;
						pLink->total_departure_based_travel_time += TravelTime;

						p_Nextlink->LinkInCapacity -= 1; // reduce available space capacity by 1



					}
					else  // no inflow capacity
					{


						through_blocking_count++;

						blocking_node_number = p_Nextlink->m_ToNodeID;  // overwrite the blocking node number
						// no capcity, do nothing, and stay in the vertical exit queue

						if (pLink->m_traffic_flow_model == 4)  // spatial queue with shock wave and FIFO principle: FIFO, then there is no capacity for one movement, the following Agents cannot move even there are capacity
						{
							break;
						}

					}

					//			TRACE("move veh %d from link %d link %d %d\n",Agent_id,pLink->m_LinkNo, p_Nextlink->m_LinkNo);


				}
				else
				{

					// reach destination, increase the counter.

					float ArrivalTimeOnDSN = CurrentTime;

					// update statistics for traveled link
					int link_sequence_no = g_AgentMap[Agent_id]->m_SimLinkSequenceNo;
					g_AgentMap[Agent_id]->m_LinkAry[link_sequence_no].AbsArrivalTimeOnDSN = ArrivalTimeOnDSN;

					float TravelTime = 0;

					if (link_sequence_no >= 1)
					{
						TravelTime = g_AgentMap[Agent_id]->m_LinkAry[link_sequence_no].AbsArrivalTimeOnDSN -
							g_AgentMap[Agent_id]->m_LinkAry[link_sequence_no - 1].AbsArrivalTimeOnDSN;
					}
					else
					{
						TravelTime = g_AgentMap[Agent_id]->m_LinkAry[link_sequence_no].AbsArrivalTimeOnDSN -
							g_AgentMap[Agent_id]->m_DepartureTime;

					}
					g_AgentMap[Agent_id]->m_Delay += (TravelTime - pLink->m_FreeFlowTravelTime);


					// finally move to next link
					g_AgentMap[Agent_id]->m_SimLinkSequenceNo = g_AgentMap[Agent_id]->m_SimLinkSequenceNo + 1;


					g_AgentMap[Agent_id]->m_ArrivalTime = ArrivalTimeOnDSN;

					g_AgentMap[Agent_id]->m_TripTime = g_AgentMap[Agent_id]->m_ArrivalTime - g_AgentMap[Agent_id]->m_DepartureTime;
					g_AgentMap[Agent_id]->m_TravelTime = g_AgentMap[Agent_id]->m_ArrivalTime - g_AgentMap[Agent_id]->m_LeavingTimeFromLoadingBuffer;

					_proxy_simulation_log(1, simulation_time_no, CurrentTime, Agent_id,
						"Step 4.6: target Agent reaches the destination at node %d, at %f min, complete Agent trip time=%f, path link sequence no = %d\n",
						g_NodeVector[node].m_NodeID,
						ArrivalTimeOnDSN, g_AgentMap[Agent_id]->m_TripTime,
						g_AgentMap[Agent_id]->m_SimLinkSequenceNo);


					g_AgentMap[Agent_id]->m_bComplete = true;  // arrive at the destination and complete the trip

						// recalculate free-flow travel time
					g_AgentMap[Agent_id]->m_TripFFTT = 0;

					for (int j = 0; j < g_AgentMap[Agent_id]->m_NodeSize - 1; j++)
					{
						int LinkID = g_AgentMap[Agent_id]->m_LinkAry[j].LinkNo;
						DTALink* pLink = g_LinkVector[LinkID];
						g_AgentMap[Agent_id]->m_TripFFTT += pLink->m_FreeFlowTravelTime;
					}
					//end of recalculating free-flow travel time

					int OriginDepartureTime = (int)(g_AgentMap[Agent_id]->m_DepartureTime);
#pragma omp critical
					{
						g_Number_of_CompletedAgents += 1;

						if (time_stamp_in_min < g_NetworkMOEAry.size() && OriginDepartureTime < g_NetworkMOEAry.size())
						{


							g_NetworkMOEAry[time_stamp_in_min].CumulativeOutFlow = g_Number_of_CompletedAgents;
							g_NetworkMOEAry[OriginDepartureTime].AbsArrivalTimeOnDSN_in_a_min += g_AgentMap[Agent_id]->m_TripTime;
							g_NetworkMOEAry[OriginDepartureTime].TotalFreeFlowTravelTime += g_AgentMap[Agent_id]->m_TripFFTT;
							g_NetworkMOEAry[OriginDepartureTime].TotalDistance += g_AgentMap[Agent_id]->m_Distance;
							g_NetworkMOEAry[OriginDepartureTime].TotalBufferWaitingTime += (g_AgentMap[Agent_id]->m_TripTime - g_AgentMap[Agent_id]->m_TravelTime);
						}

					}

					pLink->CFlowDepartureCount += 1;
					pLink->departure_count += 1;
					pLink->total_departure_based_travel_time += TravelTime;


					if (t_link_arrival_time < pLink->m_LinkMOEAry.size())
					{
						pLink->m_LinkMOEAry[t_link_arrival_time].TotalTravelTime += TravelTime;
						pLink->m_LinkMOEAry[t_link_arrival_time].TotalFlowCount += 1;
					}


					// when we are about to reach destination, we do not put Agents to the left-turn queue, as there is no "next link of next link".
					pLink->ExitQueue.pop_front();

				}

				// we deduct the out flow counter here

				Agent_out_count--;


			}



			incoming_link_count++;
			incoming_link_sequence = (incoming_link_sequence + 1) % IncomingLinkSize;  // increase incoming_link_sequence by 1 within IncomingLinkSize
		}  // for each incoming link
	} // for each node




	//	step 5: statistics collection
	if (meso_simulation_time_interval_no >= trace_step)
		g_ProgramTrace("step 5: statistics collection");
	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "  statistics collection ");

#pragma omp parallel for
	for (int li = 0; li < link_size; li++)
	{

		// Cumulative flow counts

		int t_residual = meso_simulation_time_interval_no % MAX_TIME_INTERVAL_ADCURVE;

		DTALink* pLink = g_LinkVector[li];
		pLink->m_CumuArrivalFlow[t_residual] = pLink->CFlowArrivalCount;
		pLink->m_CumuDeparturelFlow[t_residual] = pLink->CFlowDepartureCount;

		if (meso_simulation_time_interval_no%g_number_of_intervals_per_min == 0)  // per min statistics
		{
			pLink->AgentCount = pLink->CFlowArrivalCount - pLink->CFlowDepartureCount;

			//pLink->UpdateMinimumMovingTravelTime(0.2);

			// queue is the number of Agents at the end of simulation interval

			if (time_stamp_in_min < pLink->m_LinkMOEAry.size())
			{

				pLink->m_LinkMOEAry[time_stamp_in_min].ExitQueueLength = pLink->ExitQueue.size();


				if (pLink->m_LinkMOEAry[time_stamp_in_min].ExitQueueLength >= 1 && pLink->m_LinkMOEAry[time_stamp_in_min].TrafficStateCode != 2)   // not fully congested
					pLink->m_LinkMOEAry[time_stamp_in_min].TrafficStateCode = 1;  // partially congested

				pLink->m_LinkMOEAry[time_stamp_in_min].LoadingBuffer_QueueLength = pLink->LoadingBuffer.size();

				if (pLink->m_LinkMOEAry[time_stamp_in_min].LoadingBuffer_QueueLength >= 1)
					pLink->m_LinkMOEAry[time_stamp_in_min].LoadingBuffer_TrafficStateCode = 1;  // partially congested

				// time_stamp_in_min+1 is because we take the stastistics to next time stamp
				pLink->m_LinkMOEAry[time_stamp_in_min].CumulativeArrivalCount = pLink->CFlowArrivalCount;

				pLink->m_LinkMOEAry[time_stamp_in_min].CumulativeDepartureCount = pLink->CFlowDepartureCount;

				if (debug_flag && link_id_trace == li && Agent_id_trace >= 0)
				{
					TRACE("step 5: statistics collection: Time %d, link %d -> %d, Cumulative arrival count %d, cumulative departure count %d \n", time_stamp_in_min, g_NodeVector[pLink->m_FromNodeNo].m_NodeID, g_NodeVector[pLink->m_ToNodeNo].m_NodeID,
						pLink->m_LinkMOEAry[time_stamp_in_min].CumulativeArrivalCount, pLink->m_LinkMOEAry[time_stamp_in_min].CumulativeDepartureCount);
				}

			}
		}
	}
	output_debug_info_on_screen(time_stamp_in_min, simulation_step_no++, "  end of simulation at each step");

	return true;
}


void output_debug_info_on_screen(int iteration, string str)
{
	int start_iteration_debugging = 0;

	if (iteration >= start_iteration_debugging)
	{
		cout << "network loading iteration = " << iteration << "" << str << endl;

	}
}
NetworkLoadingOutput g_NetworkLoading(e_traffic_flow_model TrafficFlowModelFlag = tfm_point_queue, int SimulationMode = 0, int Iteration = 1)  // default spatial queue // SimulationMode= default 0: UE;  1: simulation from demand; 2: simulation from Agent file
{


	NetworkLoadingOutput output;
	std::set<DTANode*>::iterator iterNode;
	std::set<DTALink*>::iterator iterLink;
	g_Number_of_CompletedAgents = 0;
	g_Number_of_GeneratedAgents = 0;


	for (unsigned li = 0; li < g_LinkVector.size(); li++)
	{
		DTALink* pLink = g_LinkVector[li];
		pLink->SetupMOE();

		pLink->m_FFTT_simulation_interval = int(pLink->m_FreeFlowTravelTime / g_DTASimulationInterval_InMin);

		if (TrafficFlowModelFlag == tfm_point_queue)
		{
			if (pLink->m_FFTT_simulation_interval >= MAX_TIME_INTERVAL_ADCURVE)
			{
				TrafficFlowModelFlag = tfm_point_queue;
			}
		}

	}

	//Calculate BPRLinkVolume based on previous Agent paths and previous travel time
	for (vector<DTAAgent*>::iterator vIte = g_AgentVector.begin(); vIte != g_AgentVector.end(); vIte++)
	{
		float arrival_time_at_link = 0;
		if ((*vIte)->m_NodeSize >= 2)
		{

			arrival_time_at_link = (*vIte)->m_DepartureTime;

			for (int i = 0; i < (*vIte)->m_NodeSize - 1; i++)
			{

				g_LinkVector[(*vIte)->m_LinkAry[i].LinkNo]->m_BPRLinkVolume += 1;
				arrival_time_at_link += g_LinkVector[(*vIte)->m_LinkAry[i].LinkNo]->m_BPRLinkTravelTime;
				// m_BPRLinkTravelTimeVector is from the previous iteration
			}
		}
	}

	g_Number_of_CompletedAgents = 0;
	g_Number_of_GeneratedAgents = 0;

	int Number_of_CompletedAgents = 0;
	for (std::vector<DTAAgent*>::iterator iterAgent = g_AgentVector.begin(); iterAgent != g_AgentVector.end(); iterAgent++)
	{
		DTAAgent* pAgent = (*iterAgent);


		//if(pAgent->m_NodeSize >=2)  // has feasible path
		//{
		//	int FirstLink =pAgent->m_LinkAry[0].LinkNo;

		//	DTALink* pLink = g_LinkVector[FirstLink];
		//	pLink ->LoadingBufferSize ++;
		//}
		//// load into loading buffer
	}

	cout << "start simulation process..." << endl;

	double time;
	int meso_simulation_time_interval_no = 0;
	bool bPrintOut = true;

	// generate historical info based shortst path, based on constant link travel time

	g_SimululationReadyToEnd = g_PlanningHorizon;

	meso_simulation_time_interval_no = 0;


	for (time = g_DemandLoadingStartTimeInMin; time < min(g_PlanningHorizon, g_SimululationReadyToEnd); meso_simulation_time_interval_no++)  // the simulation time clock is advanced by 0.1 seconds
	{
		time = g_DemandLoadingStartTimeInMin + meso_simulation_time_interval_no * g_DTASimulationInterval_InMin;


		if (g_Number_of_GeneratedAgents >= 1 && g_Number_of_GeneratedAgents == g_Number_of_CompletedAgents)
			break;

		g_VehicularSimulation(Iteration, time, meso_simulation_time_interval_no, TrafficFlowModelFlag);


		if (g_Number_of_GeneratedAgents > 0 && g_Number_of_CompletedAgents == g_Number_of_GeneratedAgents)
		{
			output.NetworkClearanceTimeStamp_in_Min = time;
			output.NetworkClearanceTimePeriod_in_Min = time - g_DemandLoadingStartTimeInMin;

		}
		if (meso_simulation_time_interval_no % (5 * g_simulation_time_intervals_per_min) == 0 && bPrintOut) // every 5 min
		{

			int hour = ((int)(time)) / 60;
			int min = time - hour * 60;

			CString time_str;
			time_str.Format("%2d:%02d", hour, min);

			cout << "simu clock: " << time_str << ",# of veh --Generated: " << g_Number_of_GeneratedAgents << ", Completed " << g_Number_of_CompletedAgents << ", In network: " << g_Number_of_GeneratedAgents - g_Number_of_CompletedAgents << endl;
			g_LogFile << "simulation clock: " << time_str << ", # of Agents  -- Generated: " << g_Number_of_GeneratedAgents << ", Completed " << g_Number_of_CompletedAgents << ", In network: " << g_Number_of_GeneratedAgents - g_Number_of_CompletedAgents << endl;
		}

		if (meso_simulation_time_interval_no % (60 * g_simulation_time_intervals_per_min) == 0 && bPrintOut) // every 60 min
		{
			cout << " -- " << g_GetAppRunningTime() << endl;
			g_LogFile << " -- " << g_GetAppRunningTime() << endl;
		}
	}

	cout << "--Simulation completes at this iteration.--" << endl;

	// generate EndTimeOfPartialCongestion
	// before this step, we already generate the state of each time stamp, in terms of free-flow, congestion and queue spill back case.
	for (unsigned li = 0; li < g_LinkVector.size(); li++)  // for each link
	{

		DTALink* pLink = g_LinkVector[li];

		int NextCongestionTransitionTimeStamp = g_PlanningHorizon + 10;  // // start with the initial value, no queue

		if (pLink->m_LinkMOEAry[g_PlanningHorizon - 1].ExitQueueLength >= 1)  // remaining queue at the end of simulation horizon
			NextCongestionTransitionTimeStamp = g_PlanningHorizon - 1;


		int time_min;

		for (time_min = g_DemandLoadingStartTimeInMin + 1; time_min <= min(pLink->m_LinkMOEAry.size(), g_PlanningHorizon) - 1; time_min++)  // move backward
		{

			pLink->m_LinkMOEAry[time_min].CumulativeArrivalCount = max(pLink->m_LinkMOEAry[time_min - 1].CumulativeArrivalCount, pLink->m_LinkMOEAry[time_min].CumulativeArrivalCount); // remove data holes
			pLink->m_LinkMOEAry[time_min].CumulativeDepartureCount = max(pLink->m_LinkMOEAry[time_min - 1].CumulativeDepartureCount, pLink->m_LinkMOEAry[time_min].CumulativeDepartureCount); // remove data holes		}
		}

		for (time_min = min(pLink->m_LinkMOEAry.size(), g_PlanningHorizon) - 1; time_min >= g_DemandLoadingStartTimeInMin; time_min--)  // move backward
		{
			// transition condition 1: from partial congestion to free-flow; action: move to the next lini
			if (time_min >= 1 && pLink->m_LinkMOEAry[time_min - 1].ExitQueueLength >= 1 && pLink->m_LinkMOEAry[time_min].ExitQueueLength == 0)  // previous time_min interval has queue, current time_min interval has no queue --> end of congestion
			{
				NextCongestionTransitionTimeStamp = time_min;
			}

			//transition condition 2: from partial congestion to fully congesed. action: --> move to the previous link
			if (time_min >= 1 && pLink->m_LinkMOEAry[time_min - 1].ExitQueueLength >= 1 && pLink->m_LinkMOEAry[time_min].TrafficStateCode == 2)
			{
				NextCongestionTransitionTimeStamp = time_min;
			}


			if (pLink->m_LinkMOEAry[time_min].ExitQueueLength > 0) // there is queue at time time_min, but it is not end of queue
			{
				pLink->m_LinkMOEAry[time_min].EndTimeOfPartialCongestion = NextCongestionTransitionTimeStamp;

				// TT_SO
				// pLink->m_LinkMOEAry[time_min].SystemOptimalMarginalCost = max(0, pLink->m_LinkMOEAry[time_min].EndTimeOfPartialCongestion - time_min);

				// ECO_SO

				// 1. Get TT, get FFTT, get TW, obtain EmissionCost_Self,
				// 2: mEcoCost: gammar * (EndTimeOfPartialCongestion - time_min - TT)

				float LinkTravelTime = max(pLink->m_FreeFlowTravelTime, pLink->m_LinkMOEAry[time_min].TotalTravelTime / max(1, pLink->m_LinkMOEAry[time_min].TotalFlowCount));
				float Tw = max(0, LinkTravelTime - pLink->m_FreeFlowTravelTime);
				float gammar = 7.08;

				float End2EndTravelTime = max(0, pLink->m_LinkMOEAry[time_min].EndTimeOfPartialCongestion - time_min);

			}

		}

		// for loading buffer
		NextCongestionTransitionTimeStamp = g_PlanningHorizon + 10;  // // start with the initial value, no queue

		for (time_min = min(pLink->m_LinkMOEAry.size(), g_PlanningHorizon) - 1; time_min >= g_DemandLoadingStartTimeInMin; time_min--)  // move backward
		{
			// transition condition 1: from partial congestion to free-flow; action: move to the next lini
			if (time_min >= 1 && pLink->m_LinkMOEAry[time_min - 1].LoadingBuffer_QueueLength >= 1 && pLink->m_LinkMOEAry[time_min].LoadingBuffer_QueueLength == 0)  // previous time_min interval has queue, current time_min interval has no queue --> end of congestion
			{
				NextCongestionTransitionTimeStamp = time_min;
			}

			if (pLink->m_LinkMOEAry[time_min].LoadingBuffer_QueueLength > 0) // there is queue at time time_min, but it is not end of queue
			{
				pLink->m_LinkMOEAry[time_min].LoadingBuffer_EndTimeOfCongestion = NextCongestionTransitionTimeStamp;
			}

		}


	}

	float TotalTripTime = 0;
	float TotalTripFFTT = 0;
	float TotalTravelTime = 0;
	float TotalDelay = 0;

	float TotalCO = 0;
	int AgentSizeComplete = 0;
	float TotalDistance = 0;
	int NumberofAgentsSwitched = 0;
	int NumberofAgentsConsideringToSwitch = 0;

	// data tally for all Agents
	output_debug_info_on_screen(Iteration, "--data tally for all Agents.--");

	for (std::vector<DTAAgent*>::iterator iterAgent = g_AgentVector.begin(); iterAgent != g_AgentVector.end(); iterAgent++)
	{

		DTAAgent* pAgent = (*iterAgent);
		if (pAgent != NULL && pAgent->m_bComplete)
		{
			pAgent->m_TripFFTT = 0;


			for (int j = 0; j < pAgent->m_NodeSize - 1; j++) // for each link along the path
			{
				int LinkID = pAgent->m_LinkAry[j].LinkNo;

				if (LinkID >= g_LinkVector.size())
					break;

				DTALink* pLink = g_LinkVector[LinkID];
				pAgent->m_TripFFTT += pLink->m_FreeFlowTravelTime;

				float LinkTravelTime = pAgent->GetLinkTravelTime(j);
			}



			TotalTripTime += pAgent->m_TripTime;
			TotalTripFFTT += pAgent->m_TripFFTT;

			TotalTravelTime += pAgent->m_TravelTime;
			TotalDelay += max(0, pAgent->m_Delay);
			TotalDistance += pAgent->m_Distance;

			AgentSizeComplete += 1;


			// calcaulate time-dependent MOE for simulation daily output
			int time_interval = pAgent->m_DepartureTime / 15;

			if ((*iterAgent)->m_DepartureTime < 1440)
			{

				output.TimeDedepentMOEMap[time_interval].TotalTripTime += pAgent->m_TripTime;
				output.TimeDedepentMOEMap[time_interval].TotalTripFFTT += pAgent->m_TripFFTT;

				output.TimeDedepentMOEMap[time_interval].TotalTravelTime += pAgent->m_TravelTime;
				output.TimeDedepentMOEMap[time_interval].TotalDelay += max(0, pAgent->m_Delay);
				output.TimeDedepentMOEMap[time_interval].TotalDistance += pAgent->m_Distance;
				output.TimeDedepentMOEMap[time_interval].AgentSizeComplete += 1;

				if ((*iterAgent)->m_gap_update)
				{
					output.TimeDedepentGapMap[time_interval].NumberofAgentsWithGapUpdate += 1;
					output.TimeDedepentGapMap[time_interval].total_gap += pAgent->m_gap;
					double relative_gap = max(0, pAgent->m_gap);
					output.TimeDedepentGapMap[time_interval].total_relative_gap += (relative_gap / max(0.1, pAgent->m_TripTime));
				}

			}

			//	 only calculate gap for Agents with updated gap measures



			if ((*iterAgent)->m_bConsiderToSwitch)
			{
				NumberofAgentsConsideringToSwitch += 1;
			}

			if ((*iterAgent)->m_bSwitch)
			{
				NumberofAgentsSwitched += 1;
			}


		}
	}

	double AvgTravelTime = 0;
	double AvgTripTime = 0;
	double AvgTripTimeIndex = 1;
	double AvgDelay = 0;
	double AvgDistance = 0;
	double AvgCO = 0;
	double SwitchPercentage = 0;
	double ConsideringSwitchPercentage = 0;

	output_debug_info_on_screen(Iteration, "--data tally for AgentSizeComplete .--");

	if (AgentSizeComplete >= 1)
	{
		AvgTripTime = TotalTripTime / max(1, AgentSizeComplete);
		AvgTravelTime = TotalTravelTime / max(1, AgentSizeComplete);
		AvgTripTimeIndex = TotalTripTime / max(0.1, TotalTripFFTT);
		AvgDelay = TotalDelay / max(1, AgentSizeComplete);
		AvgDistance = TotalDistance / max(1, AgentSizeComplete);
		AvgCO = TotalCO / max(1, AgentSizeComplete);

		SwitchPercentage = NumberofAgentsSwitched * 100.0f / max(1, AgentSizeComplete);
		ConsideringSwitchPercentage = NumberofAgentsConsideringToSwitch * 100.0f / max(1, AgentSizeComplete);
	}

	//tally statistics for time-dependent MOE
	output_debug_info_on_screen(Iteration, "-tally statistics for time-dependent MOE.--");

	std::map<int, NetworkLoadingTimeDepedentMOE>::iterator iterTD;
	for (iterTD = output.TimeDedepentMOEMap.begin(); iterTD != output.TimeDedepentMOEMap.end(); iterTD++)
	{
		if ((*iterTD).second.AgentSizeComplete >= 1)
		{
			(*iterTD).second.AvgTripTime = (*iterTD).second.TotalTripTime / max(1, (*iterTD).second.AgentSizeComplete);
			(*iterTD).second.AvgTravelTime = (*iterTD).second.TotalTravelTime / max(1, (*iterTD).second.AgentSizeComplete);
			(*iterTD).second.AvgTripTimeIndex = (*iterTD).second.TotalTripTime / max(0.1, (*iterTD).second.TotalTripFFTT);
			(*iterTD).second.AvgDelay = (*iterTD).second.TotalDelay / max(1, (*iterTD).second.AgentSizeComplete);
			(*iterTD).second.AvgDistance = (*iterTD).second.TotalDistance / max(1, (*iterTD).second.AgentSizeComplete);

		}
	}



	output_debug_info_on_screen(Iteration, "-tally statistics for output.NumberofAgentsCompleteTrips .--");

	output.NumberofAgentsCompleteTrips = AgentSizeComplete;
	output.NumberofAgentsGenerated = g_Number_of_GeneratedAgents;
	if (AvgTripTime > 0)
	{
		output.AvgTripTime = AvgTripTime;
		output.AvgTravelTime = AvgTravelTime;
		output.AvgDelay = AvgDelay;
		output.AvgTTI = AvgTripTimeIndex;  // (AvgTripTime - AvgDelay) is the free flow travel time
		output.AvgDistance = AvgDistance;
		output.AvgCO = AvgCO;

		output.SwitchPercentage = SwitchPercentage;
		output.ConsideringSwitchPercentage = ConsideringSwitchPercentage;

		output.AvgTravelTime = AvgTravelTime;
	}
	else
	{
		output.AvgTripTime = 0;
		output.AvgTravelTime = 0;
		output.AvgDelay = 0;
		output.AvgTTI = 1;
		output.AvgDistance = 0;
		output.AvgCO = 0;
		output.SwitchPercentage = 0;
		output.ConsideringSwitchPercentage = 0;
	}

	if (Iteration == g_NumberOfIterations)  // output results at the last iteration
	{
		g_SimulationResult.number_of_Agents = g_AgentVector.size();
		g_SimulationResult.avg_trip_time_in_min = output.AvgTripTime;
		g_SimulationResult.avg_distance_s = output.AvgDistance;
		g_SimulationResult.avg_speed = output.AvgDistance / (max(0.1, output.AvgTripTime) / 60.0f);


	}


	output_debug_info_on_screen(Iteration, "--return output at this iteration.--");

	return output;
}


float GetTimeDependentCapacityAtSignalizedIntersection(
	double CurrentTimeInMin,
	int &current_plan_no,
	int plan_startime_in_sec[20],
	int plan_endtime_in_sec[20],
	int to_node_cycle_in_sec[20],
	int to_node_offset_in_sec[20],
	float SaturationFlowRate[20],
	int GreenStartTime_in_second[20],
	int GreenEndTime_in_second[20]
)
{

	if (current_plan_no == -1)
		return 1800;

	int green_time_in_second = 0;

	if (CurrentTimeInMin >= 5)
		TRACE("");

	if (CurrentTimeInMin * 60 < plan_startime_in_sec[current_plan_no] || CurrentTimeInMin * 60 > plan_endtime_in_sec[current_plan_no])
	{
		// update plan number
		int current_plan_no_tbd = -1;
		for (int p = 0; p < 20; p++)
		{

			if (CurrentTimeInMin * 60 >= plan_startime_in_sec[p] && CurrentTimeInMin * 60 < plan_endtime_in_sec[p])
			{
				current_plan_no_tbd = p;
				current_plan_no = current_plan_no_tbd;
				break;
			}

		}

		if (current_plan_no_tbd == -1)
			return 1800;  // default value

	}

	float simulation_time_interval_in_second = g_DTASimulationInterval_InMin * 60;

	int simulation_start_time = CurrentTimeInMin * 60;

	// consider offset
	if (simulation_start_time >= (plan_startime_in_sec[current_plan_no] + to_node_offset_in_sec[current_plan_no]))
		simulation_start_time -= (plan_startime_in_sec[current_plan_no] + to_node_offset_in_sec[current_plan_no]);

	for (int t = simulation_start_time; t < simulation_start_time + simulation_time_interval_in_second; t++)
	{
		int mod_t = t % max(1, to_node_cycle_in_sec[current_plan_no]);
		if (mod_t >= GreenStartTime_in_second[current_plan_no] && mod_t <= GreenEndTime_in_second[current_plan_no])
		{

			green_time_in_second++;
		}

	}


	float DynamicCapacity = 0;

	// max(0,intersection_end_time-intersection_start_time)  is the duraiton of intersection of two time intervals
	DynamicCapacity = SaturationFlowRate[current_plan_no] * green_time_in_second / simulation_time_interval_in_second;

	return DynamicCapacity;
}



///// about are simulation codes

extern CTime g_AppLastIterationStartTime;
std::map<CString, int> g_path_index_map;


float g_GetSwitchingRate(int iteration, float ExperiencedTravelCost, float ComputedShortestPathCost)
{
	float relative_gap = (max(0, ExperiencedTravelCost - ComputedShortestPathCost)) / max(0.1, ComputedShortestPathCost);

	float adjustment_factor = 1.0;

	float switching_rate = max(0.0001, 1.0f / (iteration + 1)*adjustment_factor);

	return switching_rate;
}



DTANetworkForSP g_TimeDependentNetwork_MP[_MAX_NUMBER_OF_PROCESSORS]; //  network instance for single processor in multi-thread environment: no more than 8 threads/cores

void g_WithIterationPathBuildingForAllAgents(int iteration, bool bRebuildNetwork, bool bOutputLog, int DemandLoadingStartTime, int DemandLoadingEndTime)
{
	// assign different zones to different processors
	int number_of_threads = omp_get_max_threads();

	if (bOutputLog)
	{
		cout << "# of Computer Processors = " << number_of_threads << endl;
	}


#pragma omp parallel for
	for (int ProcessID = 0; ProcessID < number_of_threads; ProcessID++)
	{
		// create network for shortest path calculation at this processor
		int	id = omp_get_thread_num();  // starting from 0

		//special notes: creating network with dynamic memory is a time-consumping task, so we create the network once for each processors
		if (bRebuildNetwork || g_TimeDependentNetwork_MP[id].m_NodeSize == 0)
		{
			g_TimeDependentNetwork_MP[id].BuildPhysicalNetwork(iteration, -1, g_TrafficFlowModelFlag);  // build network for this zone, because different zones have different connectors...
		}
		if (bOutputLog)
		{
			cout << "---- agent-based routing and assignment at processor " << ProcessID + 1 << endl;
		}
		for (int z = 0; z < g_ZoneVector.size(); z++)
		{

			if ((z%number_of_threads) == ProcessID)  // if the remainder of a zone id (devided by the total number of processsors) equals to the processor id, then this zone id is
			{

				// scan all possible departure times
				for (int departure_time_index = 0; departure_time_index < g_NumberOfSPCalculationPeriods; departure_time_index++)
				{

					int Agent_size = g_TDOAgentArray[z][departure_time_index].AgentArray.size();

					if (Agent_size > 0)
					{
						float node_size = g_TimeDependentNetwork_MP[id].AgentBasedPathFindingForEachZoneAndDepartureTimeInterval(z, departure_time_index, iteration);

					}


				}  // for each departure time
			}
		}  // for each zone
	} // for each computer processor

	if (bOutputLog)
	{
		cout << ":: complete assignment " << g_GetAppRunningTime() << endl;
		g_LogFile << ":: complete assignment " << g_GetAppRunningTime() << endl;

	}
}
void g_AgentBasedDynamicTrafficAssignmentSimulation()  // this is an adaptation of OD trip based assignment, we now generate and assign path for each individual Agent (as an agent with personalized value of time, value of reliability)
{
	// reset random number seeds
	int node_size = g_NodeVector.size() + 1 + g_ZoneSize;
	int link_size = g_LinkVector.size() + g_NodeVector.size(); // maximal number of links including connectors assuming all the nodes are destinations

	cout << ":: start assignment " << g_GetAppRunningTime() << endl;
	g_LogFile << ":: start assignment " << g_GetAppRunningTime() << endl;


	int iteration = 0;
	bool NotConverged = true;
	int TotalNumOfAgentsGenerated = 0;

	//cout << "------- Allocating memory for networks for " number_of_threads << " CPU threads" << endl;
	//cout << "Tips: If your computer encounters a memory allocation problem, please open file DTASettings.txt in the project folder " << endl;
	//cout << "find section [computation], set max_number_of_threads_to_be_used=1 or a small value to reduce memory usage. " << endl;
	//cout << "This modification could significantly increase the total runing time as a less number of CPU threads will be used. " << endl;

	//
	//" number_of_threads << " CPU threads" << endl;

	int number_of_threads = omp_get_max_threads();

	cout << "# of Computer Processors = " << number_of_threads << endl;
	// ----------* start of outer loop *----------
	for (iteration = 0; NotConverged && iteration < g_NumberOfIterations; iteration++)  // we exit from the loop under two conditions (1) converged, (2) reach maximum number of iterations
	{
		cout << "------- Iteration = " << iteration + 1 << "--------" << endl;



		g_CurrentGapValue = 0.0;
		g_CurrentRelativeGapValue = 0.0;
		g_CurrentNumOfAgentsForUEGapCalculation = 0;
		g_CurrentNumOfAgentsSwitched = 0;
		g_NewPathWithSwitchedAgents = 0;
		g_SetupTDTollValue(iteration);

		g_WithIterationPathBuildingForAllAgents(iteration, true, true, g_DemandLoadingStartTimeInMin, g_DemandLoadingEndTimeInMin);



		cout << "---- Network Loading for Iteration " << iteration + 1 << "----" << endl;

		NetworkLoadingOutput SimuOutput;



		SimuOutput = g_NetworkLoading(g_TrafficFlowModelFlag, 0, iteration);
		g_GenerateSimulationSummary(iteration, NotConverged, TotalNumOfAgentsGenerated, &SimuOutput);

	}  // for each assignment iteration

	cout << "Writing Agent Trajectory and MOE File... " << endl;

	cout << "     outputing agent.csv... " << endl;
	OutputAgentTrajectoryData("agent.csv");
	g_OutputLinkPerformanceData();
}


float DTANetworkForSP::AgentBasedPathFindingForEachZoneAndDepartureTimeInterval(int zone_no, int AssignmentInterval, int iteration)
// for Agents starting from departure_time_begin to departure_time_end, assign them to shortest path using a proportion according to MSA or graident-based algorithms
{

	int TempPathLinkList[MAX_NODE_SIZE_IN_A_PATH];

	std::vector<DTAAgent*>::iterator iterAgent = g_AgentVector.begin();
	int NodeSize = 0;
	int TempNodeSize;

	int total_node_size = 0;
	// loop through the TDOAgentArray to assign or update Agent paths...

	int AgentSize = g_TDOAgentArray[zone_no][AssignmentInterval].AgentArray.size();
	for (int vi = 0; vi < AgentSize; vi++)
	{
		int AgentID = g_TDOAgentArray[zone_no][AssignmentInterval].AgentArray[vi];

		if (g_AgentMap.find(AgentID) == g_AgentMap.end())
			continue;

		DTAAgent* pAgent = g_AgentMap[AgentID];

		pAgent->m_bConsiderToSwitch = false;
		pAgent->m_bSwitch = false;
		pAgent->m_bLoaded = false;


		/// finding optimal path
		bool bDebugFlag = false;

		float TotalCost = 0;
		bool bGeneralizedCostFlag = false;

		// general settings

		TempNodeSize = 0;
		float switch_threshold = 0;   // 2 min for day to day learning, - 100 min for  special case when using MSA, so 15% of agents will be selected for swtiching for sure.

		// 2 min for day to day learning,
		// -100 min for  special case when using MSA
		// +100 will disallow any departure switch, because the benchmark (experienced travel time - 100) is too low.


		float switching_rate = g_GetSwitchingRate(iteration, pAgent->m_TripTime, TotalCost);   // default switching rate


		if (pAgent->m_o_ZoneNo == pAgent->m_d_ZoneNo)
		{  // do not simulate intra zone traffic
			continue;
		}

		if (pAgent->m_NodeSize >= 1 && iteration == 0)
		{
			// no need to re-assign the traffic path when loading Agent data from routing policy at first iteration
			continue;
		}

		double RandomNumber = pAgent->GetRandomRatio();  // Agent-dependent random number generator, very safe for multi-thread applications


		bool bSwitchFlag = false;

		float ExperiencedTravelTime = pAgent->m_TripTime;
		float ExperiencedGeneralizedTravelTime = pAgent->m_TripTime + pAgent->m_TollDollarCost / max(1, pAgent->m_VOT);  // unit: min


		if (pAgent->m_AgentID == 1001)
			TRACE("");

		if (iteration == 0)  //we always switch at the first iteration
		{
			if (pAgent->m_NodeSize == 0)  // without pre-loaded path
			{
				bSwitchFlag = true;


				NodeSize = FindBestPathWithVOTForSingleAgent_Movement(pAgent->m_o_ZoneNo, pAgent->m_o_NodeNo, pAgent->m_DepartureTime, pAgent->m_d_ZoneNo, pAgent->m_d_NodeNo, pAgent->m_DemandType, pAgent->m_VOT, PathLinkList, TotalCost, bGeneralizedCostFlag, bDebugFlag);

				total_node_size += NodeSize;


				pAgent->m_bSwitch = true;
			}
			else
			{

				bSwitchFlag = false;

			}


		}
		else
		{ // iteration >=1
			if (RandomNumber < switching_rate)  // g_Day2DayAgentLearningMethod==0: no learning, just switching
			{
				NodeSize = FindBestPathWithVOTForSingleAgent_Movement(pAgent->m_o_ZoneNo, pAgent->m_o_NodeNo, pAgent->m_DepartureTime, pAgent->m_d_ZoneNo, pAgent->m_d_NodeNo, pAgent->m_DemandType, pAgent->m_VOT, PathLinkList, TotalCost, bGeneralizedCostFlag, bDebugFlag);
				bSwitchFlag = true;
			}
		}

		pAgent->m_gap_update = false;

		if (bSwitchFlag)  // for all Agents that need to switch
		{


			/// get shortest path only when bSwitchFlag is true; no need to obtain shortest path for every Agent


			pAgent->m_bConsiderToSwitch = true;

			if (pAgent->m_LinkAry != NULL && pAgent->m_NodeSize > 0)  // delete the old path
			{
				delete pAgent->m_LinkAry;
			}

			pAgent->m_NodeSize = NodeSize;


			if (pAgent->m_NodeSize >= 2)  // for feasible path
			{
				pAgent->m_bConsiderToSwitch = true;


				pAgent->m_LinkAry = new SAgentLink[NodeSize];

				if (pAgent->m_LinkAry == NULL)
				{
					cout << "Insufficient memory for allocating Agent arrays!";
					g_ProgramStop();
				}

				int NodeNumberSum = 0;
				float Distance = 0;

				for (int i = 0; i < NodeSize - 1; i++)
				{
					pAgent->m_LinkAry[i].LinkNo = PathLinkList[i];

					if (i == 0)
						NodeNumberSum += g_LinkVector[PathLinkList[i]]->m_FromNodeID;

					NodeNumberSum += g_LinkVector[PathLinkList[i]]->m_ToNodeID;


					ASSERT(pAgent->m_LinkAry[i].LinkNo < g_LinkVector.size());
					Distance += g_LinkVector[pAgent->m_LinkAry[i].LinkNo]->m_Length;

				}
				if (NodeNumberSum != pAgent->m_NodeIDSum)
				{  //switch to different  paths
					pAgent->m_bSwitch = true;
				}


				float m_gap = ExperiencedGeneralizedTravelTime - TotalCost;

				if (m_gap < 0)
					m_gap = 0.0;

				if (NodeNumberSum == pAgent->m_NodeIDSum)  //same path
					m_gap = 0.0;


				pAgent->m_gap_update = true;
				pAgent->m_gap = m_gap;
#pragma omp critical
				{
					g_CurrentNumOfAgentsSwitched += 1;
					g_CurrentGapValue += m_gap; // Jason : accumulate g_CurrentGapValue only when iteration >= 1
					g_CurrentRelativeGapValue += m_gap / max(0.1, ExperiencedGeneralizedTravelTime);
					g_CurrentNumOfAgentsForUEGapCalculation += 1;
				}


				pAgent->m_Distance = Distance;
				pAgent->m_NodeIDSum = NodeNumberSum;



				//cout << pAgent->m_AgentID <<  " Distance" << pAgent->m_Distance <<  endl;;

			}

		}  // switch
		else
		{  // not switch but we calculate gap function
			if (g_CalculateUEGapForAllAgents == 1)
			{
				NodeSize = FindBestPathWithVOTForSingleAgent_Movement(pAgent->m_o_ZoneNo, pAgent->m_o_NodeNo, pAgent->m_DepartureTime, pAgent->m_d_ZoneNo, pAgent->m_d_NodeNo, pAgent->m_DemandType, pAgent->m_VOT, PathLinkList, TotalCost, bGeneralizedCostFlag, bDebugFlag);
				total_node_size += NodeSize;

				int NodeNumberSum = 0;

				for (int i = 0; i < NodeSize - 1; i++)
				{
					NodeNumberSum += PathLinkList[i];

				}

				float m_gap = ExperiencedGeneralizedTravelTime - TotalCost;

				if (m_gap < 0)
					m_gap = 0.0;

				if (NodeNumberSum == pAgent->m_NodeIDSum)  //same path
					m_gap = 0.0;


				pAgent->m_gap_update = true;
				pAgent->m_gap = m_gap;
#pragma omp critical
				{
					g_CurrentGapValue += m_gap; // Jason : accumulate g_CurrentGapValue only when iteration >= 1
					g_CurrentRelativeGapValue += m_gap / max(0.1, ExperiencedGeneralizedTravelTime);
					g_CurrentNumOfAgentsForUEGapCalculation += 1;

				}


			}


		}

	} // for each Agent on this OD pair

	return total_node_size / max(1, AgentSize);

}




void g_GenerateSimulationSummary(int iteration, bool NotConverged, int TotalNumOfAgentsGenerated, NetworkLoadingOutput* p_SimuOutput)
{

	cout << g_GetAppRunningTime();

	if (g_AssignmentMOEVector.size() == 0)  // no assignment being involved
		return;

	TotalNumOfAgentsGenerated = p_SimuOutput->NumberofAgentsGenerated; // need this to compute avg gap

	g_AssignmentMOEVector[iteration] = (*p_SimuOutput);

	if (iteration >= 1) // Note: we output the gap for the last iteration, so "iteration-1"
	{
		//agent based, we record gaps only for Agents switched (after they find the paths)
		p_SimuOutput->AvgUEGap = g_CurrentGapValue / max(1, g_CurrentNumOfAgentsForUEGapCalculation);
		p_SimuOutput->AvgRelativeUEGap = g_CurrentRelativeGapValue * 100 / max(1, g_CurrentNumOfAgentsForUEGapCalculation);
		g_PrevRelativeGapValue = p_SimuOutput->AvgRelativeUEGap;

	}

	float PercentageComplete = 0;

	if (p_SimuOutput->NumberofAgentsGenerated >= 1)
		PercentageComplete = p_SimuOutput->NumberofAgentsCompleteTrips*100.0f / max(1, p_SimuOutput->NumberofAgentsGenerated);

	g_LogFile << g_GetAppRunningTime() << "Iteration: " << iteration << ", Average Trip Time: " << p_SimuOutput->AvgTravelTime << ", Travel Time Index: " << p_SimuOutput->AvgTTI << ", Average Distance: " << p_SimuOutput->AvgDistance << ", Switch %:" << p_SimuOutput->SwitchPercentage << ", Number of Agents Complete Their Trips: " << p_SimuOutput->NumberofAgentsCompleteTrips << ", " << PercentageComplete << "%" << endl;
	cout << g_GetAppRunningTime() << "Iter: " << iteration << ", Avg Trip Time: " << p_SimuOutput->AvgTripTime << ", Avg Trip Time Index: " << p_SimuOutput->AvgTTI << ", Avg Dist: " << p_SimuOutput->AvgDistance << ", Switch %:" << p_SimuOutput->SwitchPercentage << ", # of veh Complete Trips: " << p_SimuOutput->NumberofAgentsCompleteTrips << ", " << PercentageComplete << "%" << endl;

	g_LogFile << g_GetAppRunningTime() << "," << iteration << "," << p_SimuOutput->AvgTravelTime << "," << p_SimuOutput->AvgTTI << "," << p_SimuOutput->AvgDistance << "," << p_SimuOutput->SwitchPercentage << "," << p_SimuOutput->NumberofAgentsCompleteTrips << "," << PercentageComplete << "%,";

	g_LogFile << p_SimuOutput->AvgUEGap << "," << p_SimuOutput->TotalDemandDeviation << "," << p_SimuOutput->LinkVolumeAvgAbsError << "," << /*p_SimuOutput->LinkVolumeRootMeanSquaredError <<*/ "," << p_SimuOutput->LinkVolumeAvgAbsPercentageError;

	g_LogFile << endl;

	int time_interval = 15; // min
	int start_time = (int)(g_DemandLoadingStartTimeInMin / time_interval)*time_interval;

	if (iteration == 0)
	{

		g_SummaryStatFile.Reset();
		g_SummaryStatFile.SetFieldName("Iteration #");
		g_SummaryStatFile.SetFieldName("CPU Running Time");
		g_SummaryStatFile.SetFieldName("Per Iteration CPU Running Time");
		g_SummaryStatFile.SetFieldName("# of agents");
		g_SummaryStatFile.SetFieldName("Avg Travel Time (min)");
		g_SummaryStatFile.SetFieldName("Avg Waiting Time at Origin (min)");
		g_SummaryStatFile.SetFieldName("Avg Trip Time Index=(Mean TT/Free-flow TT)");
		g_SummaryStatFile.SetFieldName("Avg Distance");
		g_SummaryStatFile.SetFieldName("Avg Speed");

		g_SummaryStatFile.SetFieldName("% considering to switch");
		g_SummaryStatFile.SetFieldName("% switched");
		g_SummaryStatFile.SetFieldName("% completing trips");
		g_SummaryStatFile.SetFieldName("network clearance time (in min)");
		g_SummaryStatFile.SetFieldName("Avg User Equilibirum (UE) gap (min)");
		g_SummaryStatFile.SetFieldName("Relative UE gap (%)");


		for (int time = start_time; time < g_DemandLoadingEndTimeInMin; time += time_interval)
		{

			std::string time_str = "travel_time_" + GetTimeClockString(time);
			g_SummaryStatFile.SetFieldName(time_str);

		}
		for (int time = start_time; time < g_DemandLoadingEndTimeInMin; time += time_interval)
		{

			std::string time_str = "Avg UE gap_" + GetTimeClockString(time);
			g_SummaryStatFile.SetFieldName(time_str);

		}

		for (int time = start_time; time < g_DemandLoadingEndTimeInMin; time += time_interval)
		{

			std::string time_str = "Relative UE gap_" + GetTimeClockString(time);
			g_SummaryStatFile.SetFieldName(time_str);

		}
		cout << "Avg Gap: " << p_SimuOutput->AvgUEGap;


		cout << endl;

		g_SummaryStatFile.WriteHeader(false, false);

	}


	int day_no = iteration + 1;

	g_SummaryStatFile.SetValueByFieldName("Iteration #", day_no);  // iteration from 0
	g_SummaryStatFile.SetValueByFieldName("CPU Running Time", g_GetAppRunningTime(false));
	g_SummaryStatFile.SetValueByFieldName("Per Iteration CPU Running Time", g_GetAppRunningTimePerIteration(false));

	g_AppLastIterationStartTime = CTime::GetCurrentTime();

	g_SummaryStatFile.SetValueByFieldName("# of agents", p_SimuOutput->NumberofAgentsGenerated);
	g_SummaryStatFile.SetValueByFieldName("Avg Travel Time (min)", p_SimuOutput->AvgTravelTime);

	float buffer_waiting_time = max(0, p_SimuOutput->AvgTripTime - p_SimuOutput->AvgTravelTime);
	g_SummaryStatFile.SetValueByFieldName("Avg Waiting Time at Origin (min)", buffer_waiting_time);
	g_SummaryStatFile.SetValueByFieldName("Avg Trip Time Index=(Mean TT/Free-flow TT)", p_SimuOutput->AvgTTI);
	g_SummaryStatFile.SetValueByFieldName("Avg Distance", p_SimuOutput->AvgDistance);

	float avg_speed = p_SimuOutput->AvgDistance / max(0.1, p_SimuOutput->AvgTravelTime) * 60;

	g_SummaryStatFile.SetValueByFieldName("Avg Speed", avg_speed);


	g_SummaryStatFile.SetValueByFieldName("% switched", p_SimuOutput->SwitchPercentage);
	g_SummaryStatFile.SetValueByFieldName("% considering to switch", p_SimuOutput->ConsideringSwitchPercentage);

	g_SummaryStatFile.SetValueByFieldName("network clearance time (in min)", p_SimuOutput->NetworkClearanceTimeStamp_in_Min);

	g_SummaryStatFile.SetValueByFieldName("% completing trips", PercentageComplete);

	g_PercentageCompleteTrips = PercentageComplete;


	if (iteration >= 1)
	{
		g_SummaryStatFile.SetValueByFieldName("Avg User Equilibirum (UE) gap (min)", p_SimuOutput->AvgUEGap);
		g_SummaryStatFile.SetValueByFieldName("Relative UE gap (%)", p_SimuOutput->AvgRelativeUEGap);
	}


	//-- time dependent MOE

	for (int time = start_time; time < g_DemandLoadingEndTimeInMin; time += time_interval)
	{

		std::string time_str = "travel_time_" + GetTimeClockString(time);

		int time_interval_ = time / 15;
		double travel_time = p_SimuOutput->GetTimeDependentAvgTravelTime(time);

		g_SummaryStatFile.SetValueByFieldName(time_str, travel_time);

	}

	if (iteration >= 1)
	{

		for (int time = start_time; time < g_DemandLoadingEndTimeInMin; time += time_interval)
		{

			std::string time_str = "Avg UE gap_" + GetTimeClockString(time);
			double travel_time_gap = p_SimuOutput->GetTimeDependentAvgGap(time);

			g_SummaryStatFile.SetValueByFieldName(time_str, travel_time_gap);

		}

		for (int time = start_time; time < g_DemandLoadingEndTimeInMin; time += time_interval)
		{

			std::string time_str = "Relative UE gap_" + GetTimeClockString(time);

			double travel_relative_time_gap = p_SimuOutput->GetTimeDependentAvgRelativeGapInPercentage(time);

			g_SummaryStatFile.SetValueByFieldName(time_str, travel_relative_time_gap);

		}
	}

	g_SummaryStatFile.WriteRecord();

	p_SimuOutput->ResetStatistics();

	// with or without inner loop
	// check outer loop convergence (without inner loop)
	if (g_CurrentNumOfAgentsSwitched < g_ConvergenceThreshold_in_Num_Switch)
		NotConverged = false; // converged!


}

void g_ShortestPathDataMemoryAllocation()
{


	int node_size = g_NodeVector.size() + 1 + g_ZoneSize;
	int link_size = g_LinkVector.size() + g_NodeVector.size(); // maximal number of links including connectors assuming all the nodes are destinations

	g_NumberOfSPCalculationPeriods = max(1, (g_DemandLoadingEndTimeInMin - g_DemandLoadingStartTimeInMin) / g_AggregationTimetInterval);
	cout << "Allocate Memory: node_size= " << node_size << ",link_size =" << link_size << endl;

	g_LogFile << " Allocate time-dependent large shortest path memory : node_size = " << node_size << ", link_size = " << link_size << g_GetUsedMemoryDataInMB() << endl;

	int number_of_threads = omp_get_max_threads();


	for (int ProcessID = 0; ProcessID < number_of_threads; ProcessID++)
	{
		g_TimeDependentNetwork_MP[ProcessID].Setup(node_size, link_size, g_PlanningHorizon, g_AdjLinkSize, g_DemandLoadingStartTimeInMin);
	}


}



////// assignment codez

void ReadNodeControlTypeCSVFile()
{

	g_NodeControlTypeMap[0] = "unknown_control";
	g_NodeControlTypeMap[1] = "no_control";
	g_NodeControlTypeMap[2] = "yield_sign";
	g_NodeControlTypeMap[3] = "2way_stop_sign";
	g_NodeControlTypeMap[4] = "4way_stop_sign";
	g_NodeControlTypeMap[5] = "pretimed_signal";
	g_NodeControlTypeMap[6] = "actuated_signal";
	g_NodeControlTypeMap[7] = "roundabout";


}

int FindNodeControlType(string control_type)
{
	for (std::map<int, string>::iterator iter_control_type = g_NodeControlTypeMap.begin(); iter_control_type != g_NodeControlTypeMap.end(); iter_control_type++)
	{
		if (iter_control_type->second.find(control_type) != string::npos)
		{
			return iter_control_type->first;
		}
	}

	return 0;
}

void g_ReadInputFiles()
{

	g_InitialFreeMemory = g_GetFreeMemoryDataInMB();

	unsigned int state[16];

	for (int k = 0; k < 16; ++k)
	{
		state[k] = k + g_RandomSeed;
	}

	InitWELLRNG512a(state);

	// step 0: check if output files are opened.



	//	g_ReadAssignmentPeriodSettings();
	//*******************************
	// step 1: node input
	cout << "Step 1: Reading file node.csv..." << endl;
	g_LogFile << "Step 1: Reading file node.csv..." << g_GetUsedMemoryDataInMB() << endl;



		ReadNodeControlTypeCSVFile();

	if (FindNodeControlType("pretimed_signal") > 0)  // valid record
	{
		g_settings.pretimed_signal_control_type_code = FindNodeControlType("pretimed_signal");
	}

	if (FindNodeControlType("actuated_signal") > 0)  //valid record
	{
		g_settings.actuated_signal_control_type_code = FindNodeControlType("actuated_signal");
	}

	if (FindNodeControlType("no_control") > 0)  //valid record
	{
		g_settings.no_signal_control_type_code = FindNodeControlType("no_control");
	}

	int NodeControlTypeCount[10];

	for (int control_type_i = 0; control_type_i < 10; control_type_i++)
	{
		NodeControlTypeCount[control_type_i] = 0;
	}

	std::map<int, int> local_zone_map;

	CCSVParser parser_node;
	if (parser_node.OpenCSVFile("node.csv"))
	{
		int i = 0;
		while (parser_node.ReadRecord())
		{
			int node_id;

			if (parser_node.GetValueByFieldNameRequired("node_id", node_id) == false)
				break;

			int zone_id = 0;
			parser_node.GetValueByFieldNameRequired("zone_id", zone_id);

			int control_type = 0;


			DTANode Node;
			Node.m_ZoneID = zone_id;

			local_zone_map[zone_id] = 1;

			parser_node.GetValueByFieldName("control_type", control_type);

			if (control_type == g_settings.pretimed_signal_control_type_code ||
				control_type == g_settings.actuated_signal_control_type_code)
			{

				int cycle_length_in_second = 0;
				parser_node.GetValueByFieldName("cycle_length_in_second", cycle_length_in_second);
				Node.m_CycleLength_In_Second = cycle_length_in_second;

			}

			Node.m_NodeNo = i;
			Node.m_NodeID = node_id;
			Node.m_ControlType = control_type;

			NodeControlTypeCount[control_type] += 1;
			g_NodeVector.push_back(Node);
			g_NodeID2NoMap[node_id] = i;
			i++;
		}
	}
	else
	{
		cout << "Error: File node.csv cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
		g_ProgramStop();

	}

	int z = 0;
	for (std::map<int, int>::iterator iterZone = local_zone_map.begin(); iterZone != local_zone_map.end(); iterZone++)
	{
		DTAZone zone;
		zone.m_ZoneID = iterZone->first;
		zone.m_ZoneNo = z++;

		g_ZoneVector.push_back(zone);

		g_ZoneID2NoMap[zone.m_ZoneID] = zone.m_ZoneNo;

	}



	g_SummaryStatFile.WriteParameterValue("# of Nodes", g_NodeVector.size());
	g_SummaryStatFile.WriteParameterValue("# of Zones", g_ZoneVector.size());


	//*******************************
	// step 2: link type input

	cout << "Step 2: Reading file link_type.csv..." << endl;
	g_LogFile << "Step 2: Reading file link_type.csv.." << g_GetUsedMemoryDataInMB() << endl;

		CCSVParser parser_link_type;

		if (parser_link_type.OpenCSVFile("link_type.csv"))
		{

			std::map<int, int> link_type_map;
			int line_no = 0;

			while (parser_link_type.ReadRecord())
			{
				DTALinkType element;

				if (parser_link_type.GetValueByFieldName("link_type", element.link_type) == false)
				{
					if (line_no == 0)
					{
						cout << "Error: Field link_type cannot be found in file link_type.csv." << endl;
						g_ProgramStop();
					}
					else
					{  // read empty line
						break;
					}
				}

				if (link_type_map.find(element.link_type) != link_type_map.end())
				{
					cout << "Error: Field link_type " << element.link_type << " has been defined more than once in file link_type.csv." << endl;
					g_ProgramStop();

					break;
				}

				link_type_map[element.link_type] = 1;



				if (parser_link_type.GetValueByFieldName("link_type_name", element.link_type_name) == false)
				{
					g_LogFile << "Warning: value link_type_name for link type " << element.link_type << " cannot be found in file link_type.csv." << endl;

				}
				if (parser_link_type.GetValueByFieldName("type_code", element.type_code) == false)
				{
					cout << "Error: Field type_code for link type " << element.link_type << "cannot be found in file link_type.csv." << endl;
					cout << "The corresponding links will not be added in the network." << endl;
					cout << "Please check and press any key to continue!" << endl;

					element.type_code = "n";
					getchar();
				}


				if (element.type_code.find_first_not_of("afrhwctnbp") != string::npos)
				{
					cout << "Error: invalid type_code for link type " << element.link_type_name << " in link_type.csv = " << element.type_code << endl;
					g_ProgramStop();
				}

				parser_link_type.GetValueByFieldName("travel_time_bias_factor", element.link_type_bias_factor);


				if (element.link_type_bias_factor <= 0.01 || element.link_type_bias_factor >= 100)
				{
					cout << "Error: invalid link_type_bias_factor for link type " << element.link_type_name << " in link_type.csv = " << element.link_type_bias_factor << endl;
					g_ProgramStop();

				}

				g_LinkTypeMap[element.link_type] = element;

				line_no++;
			}
		}
		else
		{
			cout << "Error: File link_type.csv cannot be opened.\n It might be currently used and locked by EXCEL." << endl;


		}

		g_SummaryStatFile.WriteParameterValue("# of Link Types", g_LinkTypeMap.size());




	//*******************************
	// step 3: link data input


	int AllowExtremelyLowCapacityFlag = 1; // g_GetPrivateProfileInt("input_checking", "allow_extremely_low_capacity", 1, g_DTASettingFileName);
	//	g_ProhibitUTurnOnFeewayLinkFlag = g_GetPrivateProfileInt("shortest_path", "prohibit_u_turn_on_freeway_link", 1, g_DTASettingFileName);


	cout << "Step 3: Reading file road_link.csv... = " << endl;
	cout << "g_PlanningHorizon = " << g_PlanningHorizon << endl;


	g_LogFile << "Step 3: Reading file road_link.csv..." << g_GetUsedMemoryDataInMB() << endl;

	int  count_effective_green_time = 0;
	double total_effective_green_time = 0;
	double total_signal_link_capacity = 0;
	double total_signal_link_cycle_length = 0;
	int i = 0;

	int max_number_of_warnings_to_be_showed = 5;
	DTALink* pLink = 0;
	CCSVParser parser_link;
	int signal_reset_count = 0;
	int control_node_number = 0;
	int missing_node_error = 0;

	double total_link_length = 0;
	double total_link_coordinate_length = 0;
	if (parser_link.OpenCSVFile("road_link.csv"))
	{
		bool bNodeNonExistError = false;
		while (parser_link.ReadRecord())
		{
			int from_node_id = 0;
			int to_node_id = 0;
			int dir_flag = 1;
			double length_ = 1;
			int number_of_lanes = 1;

			int free_speed_in_mph = 60;
			float KCritical = 10;
			double capacity = 0;
			int type;
			string name, mode_code, agent_type;
			double K_jam;
			double wave_speed_in_mph = 12;


			string road_link_id;
			if (!parser_link.GetValueByFieldName("from_node_id", from_node_id))
			{
				if (i == 0)
				{
					cout << "Field from_node_id has not been defined in file road_link.csv. Please check.";
					getchar();
					exit(0);
				}
				else
				{ // i>=1;
					break;  //read empty line.

				}
			}

			if (!parser_link.GetValueByFieldName("to_node_id", to_node_id))
			{
				cout << "Field to_node_id has not been defined in file road_link.csv. Please check.";
				getchar();
				exit(0);

			}

			//if (!parser_link.GetValueByFieldName("mode_code", mode_code))
			//	mode_code = "";

			if (!parser_link.GetValueByFieldName("agent_type", agent_type))
				agent_type = "";

			if (mode_code.compare("w") == 0)   // do not model walking-only link in this version
				continue;

			if (mode_code.compare("t") == 0)   // do not model trainsit-only link in this version
				continue;

			if (mode_code.compare("b") == 0)   // do not model pedestran-only link in this version
				continue;

			if (mode_code.compare("n") == 0)   // do not model pedestran-only link in this version
				continue;

			if (!parser_link.GetValueByFieldName("link_type", type))
			{

					cout << "Field link_type has not been defined in file road_link.csv. Please check.";
					getchar();
					exit(0);
			}

			if (g_LinkTypeMap[type].type_code.find_first_of('nb') != string::npos)   //type_code: no: not included in the network, b: bike
				continue;



			if (g_NodeID2NoMap.find(from_node_id) == g_NodeID2NoMap.end())
			{

				fprintf(g_DebugLogFile, "from_node_id %d in road_link.csv has not be defined in node.csv.", from_node_id);

				if (missing_node_error <= 3)
				{
					cout << "from_node_id " << from_node_id << " in road_link.csv has not be defined in node.csv." << endl;
					cout << "associated link_type : " << type << endl;
					cout << "Please check error.log." << endl;
					getchar();
				}
				missing_node_error++;
				continue;

			}

			if (g_NodeID2NoMap.find(to_node_id) == g_NodeID2NoMap.end())
			{

				if (g_DebugLogFile != NULL)
				{

				fprintf(g_DebugLogFile, "to_node_id %d in road_link.csv has not be defined in node.csv.", to_node_id);
				}
				if (missing_node_error <= 3)
				{
					cout << "to_node_id " << to_node_id << " in road_link.csv has not be defined in node.csv. " << endl;
					cout << "associated from_node_id: " << from_node_id << " ; link_type : " << type << endl;
					cout << "Please check error.log." << endl;
					getchar();
				}
				missing_node_error++;

				continue;
			}

			parser_link.GetValueByFieldName("road_link_id", road_link_id);

			if (!parser_link.GetValueByFieldName("dir_flag", dir_flag))
				dir_flag = 1;


			if (!parser_link.GetValueByFieldName("length", length_))
			{
					cout << "Field length has not been defined in file road_link.csv. Please check.";
					getchar();
					exit(0);
			}

			if (length_ > 2000)
			{
				cout << "Link: " << from_node_id << "->" << to_node_id << " in road_link.csv has " << "length_ = " << length_ << " , which might be too long. Please check.";
				//				sleep(5);
			}

			if (!parser_link.GetValueByFieldName("lanes", number_of_lanes))
			{
					cout << "Field number_of_lanes has not been defined in file road_link.csv. Please check.";
					getchar();
					exit(0);
			}


			if (g_LinkTypeMap[type].type_code.find('c') != string::npos && number_of_lanes == 0)
			{
				number_of_lanes = 7; // reset # of lanes for connectors to a positive value
			}

			parser_link.GetValueByFieldName("free_speed", free_speed_in_mph);



			if (!parser_link.GetValueByFieldName("capacity", capacity))
			{

					cout << "Field capacity has not been defined in file road_link.csv. Please check.";
					getchar();
					exit(0);

			}

			int SaturationFlowRate;

			float BPR_Alpha = 0.15f;
			float BPR_Beta = 4.0f;
			float BPR_Distance = 0.0f;

				//if a static_traffic_analysis_mode is used, then the following term are required
			parser_link.GetValueByFieldName("BPR_alpha", BPR_Alpha);
			parser_link.GetValueByFieldName("BPR_beta", BPR_Beta);



			if (!parser_link.GetValueByFieldName("jam_density", K_jam))
			{
					K_jam = 180;
			}

			int ProhibitedU_Turn = 0;

			parser_link.GetValueByFieldName("prohibited_u-turn,", ProhibitedU_Turn);


			int link_code_start = 1;
			int link_code_end = 1;

			if (dir_flag == -1) // reversed
			{
				link_code_start = 2; link_code_end = 2;
			}

			if (dir_flag == 0) // two-directional link
			{
				link_code_start = 1; link_code_end = 2;
			}


			if (number_of_lanes == 0)  // skip this link
			{
				g_WarningFile << "link with 0 lane, skip:" << from_node_id << " -> " << to_node_id << endl;
				continue;
			}

			if (capacity < 1)  // skip this link
			{
				g_WarningFile << "link with capacity " << capacity << ", skip: " << from_node_id << " -> " << to_node_id << endl;
				continue;
			}

			if (free_speed_in_mph < 1)  // skip this link
			{
				g_WarningFile << "link with speed limit " << free_speed_in_mph << ", skip: " << from_node_id << " -> " << to_node_id << endl;
				continue;
			}

			for (int link_code = link_code_start; link_code <= link_code_end; link_code++)
			{

				int FromID = from_node_id;
				int ToID = to_node_id;
				if (link_code == 1)  //AB link
				{
					FromID = from_node_id;
					ToID = to_node_id;
				}
				if (link_code == 2)  //BA link
				{
					FromID = to_node_id;
					ToID = from_node_id;
				}

				pLink = new DTALink(g_PlanningHorizon, length_, K_jam);
				if (pLink == NULL)
				{
					cout << "Allocating memory error at line " << i + 1 << endl;
					getchar();
					exit(0);

				}

				parser_link.GetValueByFieldName("name", pLink->m_Name, true);

				std::string link_key, count_sensor_id, speed_sensor_id;

				int traffic_flow_model_flag = 1;
				parser_link.GetValueByFieldName("traffic_flow_model", traffic_flow_model_flag);

				pLink->m_road_link_id = road_link_id;
				pLink->m_traffic_flow_model = (e_traffic_flow_model)traffic_flow_model_flag;


				pLink->m_LinkNo = i;
				pLink->m_RandomSeed = pLink->m_LinkNo; // assign a link specific random seed
				pLink->m_FromNodeID = FromID;
				pLink->m_ToNodeID = ToID;
				pLink->m_FromNodeNo = g_NodeID2NoMap[pLink->m_FromNodeID];
				pLink->m_ToNodeNo = g_NodeID2NoMap[pLink->m_ToNodeID];

				pLink->m_ProhibitedU_Turn = ProhibitedU_Turn;

				if (ProhibitedU_Turn == 1)
				{
					string movement_id = GetMovementStringID(FromID, ToID, FromID);
					int middle_node_id = g_NodeID2NoMap[ToID];

					g_NodeVector[middle_node_id].m_MovementMap[movement_id].in_link_from_node_id = FromID;
					g_NodeVector[middle_node_id].m_MovementMap[movement_id].in_link_to_node_id = ToID;
					g_NodeVector[middle_node_id].m_MovementMap[movement_id].out_link_to_node_id = FromID;

					g_NodeVector[middle_node_id].m_MovementMap[movement_id].b_turning_prohibited = true;   // assign movement to individual node

					g_number_of_prohibited_movements++;
				}


				pLink->m_BPR_Alpha = BPR_Alpha;
				pLink->m_BPR_Beta = BPR_Beta;
				pLink->m_BPR_Distance = BPR_Distance;

				string geo_string;

				double link_coordinate_length = 0;

				parser_link.GetValueByFieldName("geometry", pLink->m_geometry_string);


				total_link_length += length_;
				total_link_coordinate_length += link_coordinate_length;


				pLink->m_SpeedLimit = free_speed_in_mph;

				pLink->m_KCritical = KCritical;

				float	m_KCritical;
				float length = max(0.00001, length_);  // minimum link length 0.0001

				pLink->m_OutflowNumLanes = number_of_lanes;
				pLink->m_InflowNumLanes = number_of_lanes;

				pLink->m_Orginal_OutflowNumLanes = number_of_lanes;
				pLink->m_Orginal_InflowNumLane = number_of_lanes;



				pLink->m_Direction = 1;

				pLink->m_Length = length;



				if (AllowExtremelyLowCapacityFlag == 0 && capacity < 10 && g_number_of_warnings < max_number_of_warnings_to_be_showed)
				{
					cout << "In file road_link.csv, line " << i + 1 << " has capacity <10" << capacity << ", which might not be realistic. Please correct the error." << endl;
					getchar();
					g_number_of_warnings++;
				}



				pLink->m_link_type = type;

				pLink->agent_type = agent_type;



				if (g_LinkTypeMap.find(type) == g_LinkTypeMap.end())
				{
						cout << "In file road_link.csv, line " << i + 1 << " has link type " << type << ", which has not been defined in link_type.csv. Please correct. Use default link type: arterial street." << endl;
						g_LinkTypeMap[type].type_code = 'a';
						getchar();
				}


				pLink->m_LaneCapacity = capacity;

				pLink->m_LinkTypeName = g_LinkTypeMap[type].link_type_name;

				pLink->m_bFreewayType = g_LinkTypeMap[type].IsFreeway();
				pLink->m_bArterialType = g_LinkTypeMap[type].IsArterial();




				pLink->m_KJam = K_jam;
				pLink->m_BackwardWaveSpeed = wave_speed_in_mph;

				if (g_LinkTypeMap[type].IsConnector() == true && g_NodeVector[pLink->m_ToNodeNo].m_ControlType != g_settings.no_signal_control_type_code)
				{
					//"no_control" for the downstream node of a connector
					//							g_NodeVector[pLink->m_ToNodeNo]. m_ControlType = no_signal_control_type_code;
					if (control_node_number == 0)
						control_node_number = pLink->m_ToNodeID;

					//							signal_reset_count++;
				}

				pLink->m_AgentSpaceCapacity = max(1, pLink->m_Length * pLink->m_OutflowNumLanes *K_jam);

				g_NodeVector[pLink->m_FromNodeNo].m_TotalCapacity += (pLink->m_LaneCapacity* pLink->m_OutflowNumLanes);
				g_NodeVector[pLink->m_ToNodeNo].m_IncomingLinkVector.push_back(i);
				g_NodeVector[pLink->m_FromNodeNo].m_OutgoingLinkVector.push_back(i);

				if (g_NodeVector[pLink->m_FromNodeNo].m_OutgoingLinkVector.size() > g_AdjLinkSize)
					g_AdjLinkSize = g_NodeVector[pLink->m_FromNodeNo].m_OutgoingLinkVector.size();


				g_NodeVector[pLink->m_ToNodeNo].m_IncomingLinkDelay.push_back(0);

				// prevent U turns on freeway links
				if (g_ProhibitUTurnOnFeewayLinkFlag == 1 && g_LinkTypeMap[pLink->m_link_type].IsFreeway() == true)
				{
					string movement_id = GetMovementStringID(pLink->m_FromNodeID, pLink->m_ToNodeID, pLink->m_FromNodeID);
					int middle_node_id = g_NodeID2NoMap[pLink->m_ToNodeID];

					g_NodeVector[middle_node_id].m_MovementMap[movement_id].turning_direction = "U-turn";
					g_NodeVector[middle_node_id].m_MovementMap[movement_id].in_link_from_node_id = pLink->m_FromNodeID;
					g_NodeVector[middle_node_id].m_MovementMap[movement_id].in_link_to_node_id = pLink->m_ToNodeID;
					g_NodeVector[middle_node_id].m_MovementMap[movement_id].out_link_to_node_id = pLink->m_FromNodeID;

					g_NodeVector[middle_node_id].m_MovementMap[movement_id].b_turning_prohibited = true;   // assign movement to individual node



				}


				pLink->SetupMOE();

				// tally statics for each link type
				g_LinkTypeMap[pLink->m_link_type].number_of_links++;
				g_LinkTypeMap[pLink->m_link_type].total_lane_capacity += pLink->m_LaneCapacity;
				g_LinkTypeMap[pLink->m_link_type].total_speed_limit += pLink->m_SpeedLimit;
				g_LinkTypeMap[pLink->m_link_type].total_number_of_lanes += pLink->m_OutflowNumLanes;
				g_LinkTypeMap[pLink->m_link_type].total_k_jam += pLink->m_KJam;
				g_LinkTypeMap[pLink->m_link_type].total_length += pLink->m_Length;

				g_LinkVector.push_back(pLink);
				string link_string_id = GetLinkStringID(FromID, ToID);
				g_LinkMap[link_string_id] = pLink;
				g_RoadLinkIDMap[road_link_id] = pLink;

				i++;



				if (i % 1000 == 0)
				{
					cout << " loading " << i / 1000 << "K links..." << endl;
				}

				if (i == MAX_LINK_NO ) // g_AgentBasedAssignmentFlag == 2  -> no Agent simulation
				{
					cout << "The network has more than " << MAX_LINK_NO << " links." << endl << "Please contact the developers for a new 64 bit version for this large-scale network." << endl;
					getchar();
					exit(0);

				}

			}
		}

}
	else
	{
		cout << "Error: File road_link.csv cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
		g_ProgramStop();
	}


	for (int i = 0; i < g_NodeVector.size(); i++)
	{
		if (g_NodeVector[i].m_ZoneID > 0)
		{
			int ZoneNo = g_ZoneID2NoMap[g_NodeVector[i].m_ZoneID];

			if(g_NodeVector[i].m_IncomingLinkVector.size() > 0)
			{
				g_ZoneVector[ZoneNo].m_DestinationNodeNoVector.push_back(g_NodeVector[i].m_NodeNo);
			}

			if (g_NodeVector[i].m_OutgoingLinkVector.size() > 0)
			{
				g_ZoneVector[ZoneNo].m_OriginNodeNoVector.push_back(g_NodeVector[i].m_NodeNo);
			}

		}

	}

	cout << " total # of links loaded = " << g_LinkVector.size() << endl;

	g_SummaryStatFile.WriteParameterValue("# of Links", g_LinkVector.size());


		g_SummaryStatFile.WriteParameterValue("--Parameters for link effective green time mode --", "");

		g_SummaryStatFile.WriteParameterValue("# of Links with Effective Green Time", count_effective_green_time);

		float avg_effective_green_time = total_effective_green_time / max(1, count_effective_green_time);
		g_SummaryStatFile.WriteParameterValue("Avg Effective Green Time (sec)", avg_effective_green_time);

		float avg_lane_capacity = total_signal_link_capacity / max(1, count_effective_green_time);
		g_SummaryStatFile.WriteParameterValue("Avg Signalized Lane Capacity (vph)", avg_lane_capacity);

		float avg_cycle_length = total_signal_link_cycle_length / max(1, count_effective_green_time);
		g_SummaryStatFile.WriteParameterValue("Avg Cycle length (sec)", avg_cycle_length);

		g_SummaryStatFile.WriteParameterValue("---- ", "");

		//cout << "# of Links with Effective Green Time = " << count_effective_green_time << endl;
		//cout << "Avg Effective Green Time (sec) = " << avg_effective_green_time << endl;
	// freeway, types




	// step 3.2 movement input

	g_ReadAMSMovementData();


	//if (g_SignalRepresentationFlag == signal_model_timing_status)
	//{
	//	g_ReadSignalTimingStatusFile();
	//}

	g_SummaryStatFile.WriteParameterValue("# of Prohibited Movements", g_number_of_prohibited_movements);
	g_SummaryStatFile.WriteParameterValue("# of Signals", NodeControlTypeCount[g_settings.pretimed_signal_control_type_code]);

	g_SummaryStatFile.WriteParameterValue("# of Zones", g_ZoneVector.size());

	// check how many centroids




	//test if we have many activity locations per zone

	int max_connectors = 0;
	int max_connectors_zone_number = 0;

	for ( int z =0; z< g_ZoneVector.size(); z++)
	{
		if (g_ZoneVector[z].m_OriginNodeNoVector.size() > max_connectors)
		{
			max_connectors = g_ZoneVector[z].m_OriginNodeNoVector.size();
			max_connectors_zone_number = g_ZoneVector[z].m_ZoneID;
		}
	}


	g_LogFile << "Zone " << max_connectors_zone_number << " has a maximum of" << max_connectors << " connectors:" << endl;


	//*******************************
	// step 6: demand type input

	CCSVParser parser_agent_type;
	cout << "Step 7.1: Reading file agent_type.csv..." << endl;
	g_LogFile << "Step 7.1: Reading file agent_type.csv..." << g_GetUsedMemoryDataInMB() << endl;

	if ( parser_agent_type.OpenCSVFile("agent_type.csv"))
	{
		g_DemandTypeVector.clear();
		while (parser_agent_type.ReadRecord())
		{
			int agent_type = 1;
			float average_VOT = 10;

			if (parser_agent_type.GetValueByFieldName("agent_type", agent_type) == false)
				break;

			if (agent_type != g_DemandTypeVector.size() + 1)
			{
				cout << "Error: agent_type = " << agent_type << " in file agent_type.csv should be a sequential number: " << g_DemandTypeVector.size() + 1 << endl << "Please check!";

				g_ProgramStop();
			}

			if (agent_type >= MAX_agent_type_SIZE)
			{
				cout << "Error: agent_type = " << agent_type << " in file agent_type.csv is too large. " << "MAX_agent_type_SIZE = " << MAX_agent_type_SIZE << "Please contact program developers!";

				g_ProgramStop();
			}


			float ratio_pretrip = 0;
			float ratio_enroute = 0;

			float ratio_person_info = 0;
			float ratio_ecoSO = 0;

			float Avg_VOT = 12;
			parser_agent_type.GetValueByFieldNameRequired("VOT", Avg_VOT);
			parser_agent_type.GetValueByFieldNameRequired("agent_type", agent_type);



			parser_agent_type.GetValueByFieldName("percentage_of_pretrip_info", ratio_pretrip);
			parser_agent_type.GetValueByFieldName("percentage_of_enroute_info", ratio_enroute);

			DemandType element;
			element.agent_type = agent_type;
			element.agent_type = agent_type;

			parser_agent_type.GetValueByFieldName("agent_type_name", element.agent_type_name);

			element.Avg_VOT = Avg_VOT;

			element.info_class_percentage[1] = 0;  //learning
			element.info_class_percentage[2] = ratio_pretrip;
			element.info_class_percentage[3] = ratio_enroute;

			element.info_class_percentage[4] = ratio_person_info;
			element.info_class_percentage[5] = ratio_ecoSO;

			element.info_class_percentage[0] = 100 - ratio_enroute - ratio_pretrip - ratio_person_info - ratio_ecoSO;


			if (ratio_pretrip > 0 || ratio_enroute > 0)
			{
				if (g_NumberOfIterations >= 1)
				{
					cout << "Please use 1 iteration when pre-trip and en route information is enabled." << endl;
					g_ProgramStop();
				}
			}
			for (int ic = 1; ic < MAX_INFO_CLASS_SIZE; ic++)
			{
				element.cumulative_info_class_percentage[ic] = element.cumulative_info_class_percentage[ic - 1] + element.info_class_percentage[ic];
			}
			g_DemandTypeVector.push_back(element);

		}

	}
	else
	{
		cout << "Error: File agent_type.csv cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
		g_ProgramStop();
	}

	g_SummaryStatFile.WriteParameterValue("# of Demand Types", g_DemandTypeVector.size());




	// done with zone.csv

	if (g_TrafficFlowModelFlag != tfm_BPR && g_TrafficFlowModelFlag != tfm_point_queue)
	{
		DTANetworkForSP PhysicalNetwork(g_NodeVector.size(), g_LinkVector.size(), g_PlanningHorizon, g_AdjLinkSize);  //  network instance for single processor in multi-thread environment
		PhysicalNetwork.Setup(g_NodeVector.size(), g_LinkVector.size(), g_PlanningHorizon, g_AdjLinkSize, g_DemandLoadingStartTimeInMin);
		PhysicalNetwork.BuildPhysicalNetwork(0, 0, g_TrafficFlowModelFlag, true);
		PhysicalNetwork.IdentifyBottlenecks(0);
	}


	if(g_assessibility_computing_flag == 1)
	{
		g_AgentBasedShortestPathGeneration();
		exit(0);
	}


	CCSVWriter File_output_Agent;
	File_output_Agent.Open("agent.csv");

	CCSVWriter File_link_performance;
	File_link_performance.Open("link_performance.csv");

	// step 10: demand trip file input
		////////////////////////////////////// VOT

	int LinkSizeForLinkCostArray = g_LinkVector.size() + g_NodeVector.size(); // double the size to account for artificial connectors
	g_LinkTDCostAry = AllocateDynamicArray<DTALinkToll>(LinkSizeForLinkCostArray, g_NumberOfSPCalculationPeriods);


		// step 12: reading/initialing agent file input
			cout << "Step 12.2: Reading/Initializing agent trip file..." << endl;
			//------------------------------------------------------------//


			string c_str = "input_agent.csv";
			if (g_ReadInputAgentCSVFile(c_str, false) == false)
			{
				cout << "input_agent file  is missing. Please check." << endl;
				g_ProgramStop();
			}

			g_ShortestPathDataMemoryAllocation();

			g_AllocateDynamicArrayForAgents();

	int g_simulation_time_discharge_time_in_min = g_GetPrivateProfileInt("simulation", "discharge_time_in_min", 120, g_DTASettingFileName);

	if (g_PlanningHorizon < g_DemandLoadingEndTimeInMin + g_simulation_time_discharge_time_in_min)
	{
		//reset simulation horizon to make sure it is longer than the demand loading horizon
		g_PlanningHorizon = g_DemandLoadingEndTimeInMin + g_simulation_time_discharge_time_in_min;

		for (unsigned link_index = 0; link_index < g_LinkVector.size(); link_index++)
		{
			DTALink* pLink = g_LinkVector[link_index];
			pLink->ResizeData(g_PlanningHorizon);
		}

	}

	//use the user defined travel time
	//g_ReadInputLinkTravelTime();

	g_LogFile << " -- zone-specific demand data -- " << endl;
	cout << "Number of Zones = " << g_ZoneVector.size() << endl;
	cout << "Number of Nodes = " << g_NodeVector.size() << endl;
	cout << "Number of Links = " << g_LinkVector.size() << endl;

	cout << "Number of Agents to be Simulated = " << g_AgentVector.size() << endl;
	cout << "Demand Loading Period = " << g_DemandLoadingStartTimeInMin << " min -> " << g_DemandLoadingEndTimeInMin << " min." << endl;

	g_SummaryStatFile.WriteParameterValue("\nDemand multiplier in input_scenario_settings.csv file", g_DemandGlobalMultiplier);

	g_SummaryStatFile.WriteParameterValue("# of Agents to be simulated", g_AgentVector.size());
	g_SummaryStatFile.WriteParameterValue("# of Intra-zone Agents (not be simulated)", g_number_of_intra_zone_trips);
	g_SummaryStatFile.WriteTextLabel("Starting Time of Demand Loading (min)=,");
	g_SummaryStatFile.WriteNumber(g_DemandLoadingStartTimeInMin);

	g_SummaryStatFile.WriteTextLabel("Ending Time of Demand Loading (min)=,");
	g_SummaryStatFile.WriteNumber(g_DemandLoadingEndTimeInMin);

//
	g_NetworkMOEAry.clear();

	for (int time = 0; time <= g_PlanningHorizon; time++)
	{
		NetworkMOE element;
		g_NetworkMOEAry.push_back(element);
	}

	g_AssignmentMOEVector.clear();
	for (int iter = 0; iter < g_NumberOfIterations; iter++)
	{
		NetworkLoadingOutput element;
		g_AssignmentMOEVector.push_back(element);
	}


	cout << "Number of Demand Types = " << g_DemandTypeVector.size() << endl;

	g_LogFile << "Number of Zones = " << g_ZoneVector.size() << endl;
	g_LogFile << "Number of Nodes = " << g_NodeVector.size() << endl;
	g_LogFile << "Number of Links = " << g_LinkVector.size() << endl;
	g_LogFile << "Number of Agents to be Simulated = " << g_AgentVector.size() << endl;
	g_LogFile << "Demand Loading Period = " << g_DemandLoadingStartTimeInMin << " min -> " << g_DemandLoadingEndTimeInMin << " min." << endl;
	g_LogFile << "Number of Demand Types = " << g_DemandTypeVector.size() << endl;

	// link type summary

	CString title_str;
	title_str.Format("\n--Link Type Statistics--");
	g_SummaryStatFile.WriteTextString(title_str);
	g_SummaryStatFile.Reset();

	g_SummaryStatFile.SetFieldName("link_type");
	g_SummaryStatFile.SetFieldName("link_type_name");
	g_SummaryStatFile.SetFieldName("link_type_code");
	g_SummaryStatFile.SetFieldName("number_of_links");
	g_SummaryStatFile.SetFieldName("avg_lane_capacity");
	g_SummaryStatFile.SetFieldName("avg_number_of_lanes");
	g_SummaryStatFile.SetFieldName("avg_speed_limit");
	g_SummaryStatFile.SetFieldName("avg_link_length");
	g_SummaryStatFile.SetFieldName("avg_K_jam");
	g_SummaryStatFile.SetFieldName("total_link_length");

	g_SummaryStatFile.WriteHeader(false, false);

	for (std::map<int, DTALinkType>::iterator itr = g_LinkTypeMap.begin(); itr != g_LinkTypeMap.end(); itr++)
	{
		std::replace(itr->second.link_type_name.begin(), itr->second.link_type_name.end(), ',', ' ');

		g_SummaryStatFile.SetValueByFieldName("link_type", itr->first);
		std::string link_type_name = itr->second.link_type_name.c_str();
		std::string type_code = itr->second.type_code.c_str();
		g_SummaryStatFile.SetValueByFieldName("link_type_name", link_type_name);
		g_SummaryStatFile.SetValueByFieldName("link_type_code", type_code);
		g_SummaryStatFile.SetValueByFieldName("number_of_links", itr->second.number_of_links);

		float avg_lane_capacity = itr->second.total_lane_capacity / max(1, itr->second.number_of_links);
		float avg_number_of_lanes = itr->second.total_number_of_lanes / max(1, itr->second.number_of_links);
		float avg_speed_limit = itr->second.total_speed_limit / max(1, itr->second.number_of_links);
		float avg_K_jam = itr->second.total_k_jam / max(1, itr->second.number_of_links);
		float avg_link_length = itr->second.total_length / max(1, itr->second.number_of_links);
		float total_link_length = avg_link_length * itr->second.number_of_links;


		g_SummaryStatFile.SetValueByFieldName("avg_lane_capacity", avg_lane_capacity);
		g_SummaryStatFile.SetValueByFieldName("avg_number_of_lanes", avg_number_of_lanes);
		g_SummaryStatFile.SetValueByFieldName("avg_speed_limit", avg_speed_limit);
		g_SummaryStatFile.SetValueByFieldName("avg_K_jam", avg_K_jam);
		g_SummaryStatFile.SetValueByFieldName("avg_link_length", avg_link_length);
		g_SummaryStatFile.SetValueByFieldName("total_link_length", total_link_length);

		g_SummaryStatFile.WriteRecord();
	}
 		// read input link travel times
		g_ReadInputLinkAttribute_Parser();

}
// debug:

void g_ReadInputLinkAttribute_Parser()
{
	int line_no = 0;

	CCSVParser parser_link_attribute;

	if (parser_link_attribute.OpenCSVFile("input_link_attribute.csv", false))
	{

		while (parser_link_attribute.ReadRecord())
		{

			string road_link_id;

			int starting_time_in_min = 0;
			int ending_time_in_min = 1440;

			if (parser_link_attribute.GetValueByFieldName("road_link_id", road_link_id) == false)
				break;

			float travel_time = 0;

			if (parser_link_attribute.GetValueByFieldName("travel_time", travel_time) == false)
				break;


			string time_period;
			if (parser_link_attribute.GetValueByFieldNameRequired("time_period", time_period) == true)
			{

				vector<float> global_minute_vector;

				vector<string> input_string;

				input_string.push_back(time_period);
				//input_string includes the start and end time of a time period with hhmm format
				global_minute_vector = g_time_parser(input_string); //global_minute_vector incldue the starting and ending time
				if (global_minute_vector.size() == 2)
				{

					starting_time_in_min = global_minute_vector[0];
					ending_time_in_min = global_minute_vector[1];


					if (g_RoadLinkIDMap.find(road_link_id) != g_RoadLinkIDMap.end())
					{

						DTALink* pLink = g_RoadLinkIDMap[road_link_id];
						if (pLink != NULL)
						{

							if (starting_time_in_min >= 0 && starting_time_in_min < ending_time_in_min && ending_time_in_min < pLink->m_LinkMOEAry.size()
								&& travel_time >= 0.01)
							{
								for (int t = starting_time_in_min; t < ending_time_in_min; t++)
								{
									pLink->m_LinkMOEAry[t].UserDefinedTravelTime_in_min = travel_time;
								}
							}
						}

					}
				}

				if (line_no % 100000 == 0)
				{
					cout << g_GetAppRunningTime() << "Reading " << line_no / 1000 << "K lines..." << endl;
				}
				line_no++;

			}
		}
		cout << " total # of link travel time records loaded = " << line_no << endl;
		g_SummaryStatFile.WriteParameterValue("# of Input Link Travel Time Records", line_no);

	}

}
void g_CreateLinkTollVector()
{

	// transfer toll set to dynamic toll vector (needed for multi-core computation)
	int count = 0;
	for (unsigned li = 0; li< g_LinkVector.size(); li++)
	{

		if (g_LinkVector[li]->pTollVector != NULL)
			delete g_LinkVector[li]->pTollVector;

		if (g_LinkVector[li]->TollVector.size() >0)
		{
			g_LinkVector[li]->m_TollSize = g_LinkVector[li]->TollVector.size();
			g_LinkVector[li]->pTollVector = new Toll[g_LinkVector[li]->m_TollSize];

			for (int s = 0; s < g_LinkVector[li]->m_TollSize; s++)
			{
				g_LinkVector[li]->pTollVector[s] = g_LinkVector[li]->TollVector[s];
			}


			count++;
		}

	}
}


void ReadMovementScenarioFile(string FileName, int scenario_no)
{

	CCSVParser parser_movement;
	int count = 0;

	if (parser_movement.OpenCSVFile(FileName, false))
	{
		int i = 1;

		while (parser_movement.ReadRecord())
		{
			int record_scenario_no = 0;
			DTAMovement element;
			parser_movement.GetValueByFieldNameWithPrintOut("scenario_no", record_scenario_no);

			parser_movement.GetValueByFieldNameWithPrintOut("node_id", element.in_link_to_node_id);

			parser_movement.GetValueByFieldNameWithPrintOut("incoming_link_from_node_id", element.in_link_from_node_id);

			if (g_LinkMap.find(GetLinkStringID(element.in_link_from_node_id, element.in_link_to_node_id)) == g_LinkMap.end())
			{
				cout << "Link " << element.in_link_from_node_id << "-> " << element.in_link_to_node_id << " at line " << i + 1 << " of file input_movement.csv" << " has not been defined in road_link.csv. Please check.";
				g_ProgramStop();
			}


			parser_movement.GetValueByFieldNameWithPrintOut("outgoing_link_to_node_id", element.out_link_to_node_id);
			if (g_LinkMap.find(GetLinkStringID(element.in_link_to_node_id, element.out_link_to_node_id)) == g_LinkMap.end())
			{
				cout << "Link " << element.in_link_to_node_id << "-> " << element.out_link_to_node_id << " at line " << i + 1 << " of file input_movement.csv" << " has not been defined in road_link.csv. Please check.";
				g_ProgramStop();
			}

			parser_movement.GetValueByFieldNameWithPrintOut("effective_green_time_in_min", element.movement_effective_green_time_in_min);

			parser_movement.GetValueByFieldName("turning_direction", element.turning_direction);



			if (element.movement_hourly_capacity >= 0)
			{

				string movement_id = GetMovementStringID(element.in_link_from_node_id, element.in_link_to_node_id, element.out_link_to_node_id);
				int middle_node_id = g_NodeID2NoMap[element.in_link_to_node_id];

					g_NodeVector[middle_node_id].m_MovementMap[movement_id] = element;  // assign movement to individual node
					count++;

			}// skip -1 as no data available

			i++;
		}
	}

	g_SummaryStatFile.WriteTextLabel("# of movement records=");
	g_SummaryStatFile.WriteNumber(count);
}

void ReadLinkTollScenarioFile(string FileName, int scenario_no)
{

	for (unsigned li = 0; li < g_LinkVector.size(); li++)
	{

		g_LinkVector[li]->TollVector.clear(); // remove all previouly read records

	}


	// generate toll based on demand type code in road_link.csv file
	int demand_mode_type_count = 0;

	for (unsigned li = 0; li < g_LinkVector.size(); li++)
	{
		if (g_LinkVector[li]->agent_type.size() >= 1 && g_LinkVector[li]->agent_type[0] !='0')
		{  // with data string

			Toll tc;  // toll collection
			tc.StartDayNo = 0;
			tc.EndDayNo = 999;

			tc.StartTime = 0;
			tc.EndTime = 99999;

			std::string agent_type = g_LinkVector[li]->agent_type;


			for (int pt = 1; pt <= g_DemandTypeVector.size(); pt++)
			{
				CString number;
				number.Format("%d", pt);

				std::string str_number = CString2StdString(number);
				if (agent_type.find(str_number) == std::string::npos)   // do not find this number
				{
					tc.TollRate[pt] = 999;
					demand_mode_type_count++;
				}

			}  //end of pt
			g_LinkVector[li]->TollVector.push_back(tc);

		}

	}

	g_SummaryStatFile.WriteTextLabel("# of demand type code based toll records=");
	g_SummaryStatFile.WriteNumber(demand_mode_type_count);


}

void g_ReadScenarioInputFiles(int scenario_no)
{

}
void FreeMemory()
{

}




int g_InitializeLogFiles()
{
	g_AppStartTime = CTime::GetCurrentTime();
	g_AppLastIterationStartTime = g_AppStartTime;
//	g_ODME_result_file = fopen("ODME_zone_based_log.csv", "w");
	//g_ABM_log_file = fopen("debug_ABM.log", "w");
	//g_ABM_log_level = g_GetPrivateProfileInt("debug", "ABM_log_level", 0, g_DTASettingFileName);



	//g_LogFile.open("debug_simulation.log", ios::out);
	//if (g_LogFile.is_open())
	//{
	//	g_LogFile.width(12);
	//	g_LogFile.precision(3);
	//	g_LogFile.setf(ios::fixed);
	//}
	//else
	//{
	//	cout << "File output_simulation.log cannot be opened, and it might be locked by another program or the target data folder is read-only." << endl;
	//	cin.get();  // pause
	//	return 0;
	//}




	g_WarningFile.open("warning.log", ios::out);
	if (g_WarningFile.is_open())
	{
		g_WarningFile.width(12);
		g_WarningFile.precision(3);
		g_WarningFile.setf(ios::fixed);
	}
	else
	{
		cout << "File warning.log cannot be opened, and it might be locked by another program!" << endl;
		cin.get();  // pause
		return 0;
	}

	DWORD dwReturn;                                    //Declare needed variables
	char szBuffer[MAX_PATH];                           //Allocate (easily) a buffer
	//using MAX_PATH equate
	dwReturn = GetCurrentDirectory(MAX_PATH, szBuffer); //Call Api Function

	cout << "--- Current Directory: " << szBuffer << " ---" << endl << endl;
	g_LogFile << "--- Current Directory: " << szBuffer << " ---" << endl << endl;


	CCSVParser parser_MOE_settings;
	if (!parser_MOE_settings.OpenCSVFile("optional_MOE_settings.csv", false))
	{
		// use default files
		CCSVWriter csv_output("optional_MOE_settings.csv");
		csv_output.WriteTextString("moe_type,moe_group,notes,moe_category_label,cut_off_volume,agent_type,information_type,from_node_id,mid_node_id,to_node_id,origin_zone_id,destination_zone_id,exclude_link_no,link_list_string,departure_starting_time,departure_ending_time,entrance_starting_time,entrance_ending_time,impact_type");
		csv_output.WriteTextString("network,1,,network");
		csv_output.WriteTextString("network,2,,sov,,1,");
		csv_output.WriteTextString("network,2,,hov,,2,");
		csv_output.WriteTextString("network,2,,truck,,3,");
		csv_output.WriteTextString("network,2,,intermodal,,4,");

		csv_output.WriteTextString("od,4,,OD1,,,,,,,,1,2");
		csv_output.WriteTextString("od,4,,OD2,,,,,,,,2,1");

		csv_output.WriteTextString("link,5,,link_48_41_ NB freeway,,,,,48,,41,,,,,,,,,");
		csv_output.WriteTextString("link,5,,link_39_44_NB freeway,,,,,30,,25,,,,,,,,,");

		csv_output.WriteTextString("path_3point,6,,'Path1_Major freeway route, which carries a lot of the damand',,,,,200,41,198,,,,,,,,,");

		csv_output.WriteTextString("network_time_dependent,7,,for all demand types,,,,,120,91,162");

		csv_output.WriteTextString("link_critical,8,,critical link list,1000");
		csv_output.WriteTextString("od_critical,9,,critical OD list,50");

	}



	fopen_s(&g_DebugLogFile, "debug.log", "w");
	if (g_DebugLogFile == NULL)
	{
		cout << "Cannot open file debug.log! Another instance of DTALite might be running on the same folder." << endl;
		cin.get();  // pause
		return 0;
	}

	return 1;
}

void g_ReadDTALiteSettings()
{

	// if  ./DTASettings.ini does not exist, then we should print out all the default settings for user to change
	//
	g_AdjLinkSize = g_GetPrivateProfileInt("shortest_path", "max_size_of_adjacent_links", 30, g_DTASettingFileName);

	if (g_AdjLinkSize < 30)
		g_AdjLinkSize = 30;

	g_assessibility_computing_flag = g_GetPrivateProfileInt("accessibility", "accessibility_computing", 0, g_DTASettingFileName);

	g_AggregationTimetInterval = g_GetPrivateProfileInt("shortest_path", "travel_time_aggregation_interval_in_min", 15, g_DTASettingFileName);

	if (g_AggregationTimetInterval < 15)
		g_AggregationTimetInterval = 1;

	g_settings.AdditionalYellowTimeForSignals = 4; // g_GetPrivateProfileInt("simulation", "additional_amber_time_per_link_per_cycle", 4, g_DTASettingFileName);
	//	g_settings.IteraitonNoStartSignalOptimization = g_GetPrivateProfileInt("signal_optimization", "starting_iteration_no", 1, g_DTASettingFileName);
	//	g_settings.IteraitonStepSizeSignalOptimization = g_GetPrivateProfileInt("signal_optimization", "number_of_iterations_per_optimization", 5, g_DTASettingFileName);
	//	g_settings.DefaultCycleTimeSignalOptimization = g_GetPrivateProfileInt("signal_optimization", "default_cycle_time_in_sec", 60, g_DTASettingFileName);

	//	g_UseFreevalRampMergeModelFlag = g_GetPrivateProfileInt("simulation", "use_freeval_merge_model", 0, g_DTASettingFileName);
	g_settings.use_point_queue_model_for_on_ramps = g_GetPrivateProfileInt("simulation", "use_point_queue_for_on_ramps", 1, g_DTASettingFileName);
	g_settings.use_point_queue_model_for_off_ramps = g_GetPrivateProfileInt("simulation", "use_point_queue_for_off_ramps", 1, g_DTASettingFileName);

	g_simulation_time_interval_step = g_GetPrivateProfileFloat("simulation", "simulation_interval_in_sec", 6, g_DTASettingFileName);
	if (g_simulation_time_interval_step < 0.1)
		g_simulation_time_interval_step = 0.1;

	g_simulation_time_intervals_per_min = (int)(60 / g_simulation_time_interval_step);
	g_DTASimulationInterval_InMin = g_simulation_time_interval_step / 60.0; // min

	g_settings.use_mile_or_km_as_length_unit = 1; // g_GetPrivateProfileInt("simulation", "use_mile_or_km_as_length_unit", 1, g_DTASettingFileName);



	//		g_TollingMethodFlag = g_GetPrivateProfileInt("tolling", "method_flag", 0, g_DTASettingFileName);
	//		g_VMTTollingRate = g_GetPrivateProfileFloat("tolling", "VMTRate", 0, g_DTASettingFileName);

	g_MergeNodeModelFlag = 0;
	//	g_MergeNodeModelFlag = g_GetPrivateProfileInt("simulation", "merge_node_model", 0, g_DTASettingFileName);
	g_FIFOConditionAcrossDifferentMovementFlag = 0; // g_GetPrivateProfileInt("simulation", "first_in_first_out_condition_across_different_movements", 0, g_DTASettingFileName);
	g_MinimumInFlowRatio = 0; // g_GetPrivateProfileFloat("simulation", "minimum_link_in_flow_ratio", 0.00f, g_DTASettingFileName);
	g_RelaxInFlowConstraintAfterDemandLoadingTime = 60; // g_GetPrivateProfileFloat("simulation", "use_point_queue_model_x_min_after_demand_loading_period", 60.0f, g_DTASettingFileName);
	g_MaxDensityRatioForAgentLoading = 0.8; // g_GetPrivateProfileFloat("simulation", "max_density_ratio_for_loading_Agents", 0.8f, g_DTASettingFileName);
	g_DefaultSaturationFlowRate_in_vehphpl = g_GetPrivateProfileFloat("simulation", "default_saturation_flow_rate_in_vehphpl", 1800, g_DTASettingFileName);

	g_ConvergencyRelativeGapThreshold_in_perc = g_GetPrivateProfileFloat("assignment", "convergency_relative_gap_threshold_percentage", 5, g_DTASettingFileName);



	g_TravelTimeDifferenceForSwitching = g_GetPrivateProfileFloat("assignment", "travel_time_difference_for_switching_in_min", 1, g_DTASettingFileName);
	g_RelativeTravelTimePercentageDifferenceForSwitching = g_GetPrivateProfileFloat("assignment",
		"relative_travel_time_difference_in_percentage_for_switching_in_percentage", 15, g_DTASettingFileName);

	if (g_DemandLoadingEndTimeInMin >= 2000)
	{
		cout << "Error: g_DemandLoadingEndTimeInMin >=2000";
		g_ProgramStop();
	}

	g_NumberOfIterations = g_GetPrivateProfileInt("assignment", "number_of_iterations", 20, g_DTASettingFileName);
	int traffic_flow_model = g_GetPrivateProfileInt("simulation", "traffic_flow_model", 2, g_DTASettingFileName);  //2
	g_TrafficFlowModelFlag = (e_traffic_flow_model)traffic_flow_model;

	int UEAssignmentMethod = g_GetPrivateProfileInt("assignment", "assignment_method", 1, g_DTASettingFileName);

	g_RandomSeed = g_GetPrivateProfileInt("simulation", "random_seed", 1000, g_DTASettingFileName);




	srand(g_RandomSeed);
	g_LogFile << "Simulation Horizon (min) = " << g_PlanningHorizon << endl;
	g_LogFile << "Departure Time Interval (min) = " << g_AggregationTimetInterval << endl;
	g_LogFile << "Number of Iterations = " << g_NumberOfIterations << endl;

	}


void g_FreeMemory(bool exit_flag = true)
{
	cout << "Free memory... " << endl;
	return;
	// Free pointers
	// ask the operating system to free the memory  after the program complete
	exit(0);

	int LinkSizeForLinkCostArray = g_LinkVector.size() + g_NodeVector.size(); // double the size to account for artificial connectors

	if (g_LinkTDCostAry != NULL)
		DeallocateDynamicArray<DTALinkToll>(g_LinkTDCostAry, LinkSizeForLinkCostArray, g_NumberOfSPCalculationPeriods);


	try
	{
		cout << "Free link set... " << endl;
		for (unsigned li = 0; li < g_LinkVector.size(); li++)
		{
			DTALink* pLink = g_LinkVector[li];

			if (pLink != NULL && pLink->m_OutflowNumLanes >= 1)
			{
				delete pLink;
			}

		}

	}
	catch (const std::exception& e)
	{
		std::cout << "Got exception: " << e.what() << std::endl;

	}


	cout << "Free node set... " << endl;
	g_NodeVector.clear();

	g_LinkVector.clear();


	g_NodeID2NoMap.clear();
	g_DemandTypeVector.clear();
	g_FreeMemoryForAgentVector();
	g_NetworkMOEAry.clear();

	g_AssignmentMOEVector.clear();

	if (g_TDOAgentArray != NULL && g_ZoneVector.size() > 0);
	{
		DeallocateDynamicArray<AgentArrayForOriginDepartrureTimeInterval>(g_TDOAgentArray, g_ZoneVector.size(), g_NumberOfSPCalculationPeriods);  // +1 is because the zone numbers start from 1 not from 0
		g_TDOAgentArray = NULL;
		g_NumberOfSPCalculationPeriods = -1;
	}

	//free zone map after g_TDOAgentArray as g_ZoneVector is still used in DeallocateDynamicArray for g_TDOAgentArray
	g_ZoneVector.clear();

	int number_of_threads = omp_get_max_threads();

}

void g_CloseFiles()
{

	if (g_DebugLogFile);
	fclose(g_DebugLogFile);

	if (g_DebugLogFile);
	fclose(g_DebugLogFile);


	if (g_simulation_log_file != NULL)
		fclose(g_simulation_log_file);




	g_LogFile.close();
	g_WarningFile.close();


	cout << "Assignment-Simulation Completed. " << g_GetAppRunningTime() << endl;
	g_LogFile << "Assignment-Simulation Completed. " << g_GetAppRunningTime() << endl;

}

void g_TrafficAssignmentSimulation()
{

	g_ReadInputFiles();

	cout << "Start Traffic Assignment/Simulation... " << endl;

	// to simplfy the computational process, we only perform agent-based assignment
		cout << "Agent based dynamic traffic assignment... " << endl;
		g_AgentBasedDynamicTrafficAssignmentSimulation();  // agent-based assignment

}



void DTANetworkForSP::IdentifyBottlenecks(int StochasticCapacityFlag)
{

	g_LogFile << "The following freeway/highway bottlenecks are identified." << endl;

	// ! there is an freeway or highway downstream with less number of lanes
	for (unsigned li = 0; li < g_LinkVector.size(); li++)
	{
		if (g_LinkTypeMap[g_LinkVector[li]->m_link_type].IsFreeway() && m_OutboundSizeAry[g_LinkVector[li]->m_ToNodeNo] == 1)  // freeway or highway
		{
			int FromID = g_LinkVector[li]->m_FromNodeNo;
			int ToID = g_LinkVector[li]->m_ToNodeNo;

			for (int i = 0; i < m_OutboundSizeAry[ToID]; i++)
			{
				DTALink* pNextLink = g_LinkVector[m_OutboundLinkAry[ToID][i]];
				if (g_LinkTypeMap[pNextLink->m_link_type].IsFreeway() && pNextLink->m_OutflowNumLanes < g_LinkVector[li]->m_OutflowNumLanes && pNextLink->m_ToNodeNo != FromID)
				{
					//					g_LinkVector[li]->m_StochaticCapcityFlag = StochasticCapacityFlag;  //lane drop from current link to next link
					g_LogFile << "lane drop:" << g_NodeVector[g_LinkVector[li]->m_FromNodeNo].m_NodeID << " ->" << g_NodeVector[g_LinkVector[li]->m_ToNodeNo].m_NodeID << endl;
				}

			}

		}
	}
	   // merge: one outgoing link, two more incoming links with at least freeway link

	for (unsigned li = 0; li < g_LinkVector.size(); li++)
	{
		int incoming_link_freeway_and_ramp_count = 0;
		bool no_arterial_incoming_link = true;


		for (int incoming_link = 0; incoming_link < m_InboundSizeAry[g_LinkVector[li]->m_FromNodeNo]; incoming_link++) // one outgoing link without considering u-turn
		{
			int incoming_link_id = m_InboundLinkAry[g_LinkVector[li]->m_FromNodeNo][incoming_link];

			if ((g_LinkVector[incoming_link_id]->m_FromNodeNo != g_LinkVector[li]->m_ToNodeNo)) // non-uturn link
			{
				if (g_LinkTypeMap[g_LinkVector[incoming_link_id]->m_link_type].IsFreeway() //freeway link
					|| g_LinkTypeMap[g_LinkVector[incoming_link_id]->m_link_type].IsRamp())
				{
					incoming_link_freeway_and_ramp_count++;

				}
				else
				{
					no_arterial_incoming_link = false;

				}
			}

		}



		if (incoming_link_freeway_and_ramp_count >= 2 && no_arterial_incoming_link)
		{

			if (m_OutboundSizeAry[g_LinkVector[li]->m_FromNodeNo] == 1)
			{
				TRACE("\nMerge link: %d->%d", g_LinkVector[li]->m_FromNodeID, g_LinkVector[li]->m_ToNodeID);
				g_LinkVector[li]->m_bMergeFlag = 1;

			}

		}

	}

	// first count # of incoming freeway, highway or ramp links to each freeway/highway link
	for (unsigned li = 0; li < g_LinkVector.size(); li++)
	{
		int FromID = g_LinkVector[li]->m_FromNodeNo;
		if (g_LinkVector[li]->m_bMergeFlag == 1 && m_InboundSizeAry[FromID] == 2)  // is a merge bottlebeck link with two incoming links
		{
			int il;
			bool bRampExistFlag = false;
			bool bFreewayExistFlag = false;

			for (il = 0; il < m_InboundSizeAry[FromID]; il++)
			{
				if (g_LinkTypeMap[g_LinkVector[m_InboundLinkAry[FromID][il]]->m_link_type].IsRamp())  // on ramp as incoming link
				{
					bRampExistFlag = true;

					g_LinkVector[m_InboundLinkAry[FromID][il]]->m_bOnRampType = true;
					g_LinkVector[li]->m_MergeOnrampLinkID = m_InboundLinkAry[FromID][il];
				}
				if (g_LinkTypeMap[g_LinkVector[m_InboundLinkAry[FromID][il]]->m_link_type].IsFreeway() ||
					g_LinkTypeMap[g_LinkVector[m_InboundLinkAry[FromID][il]]->m_link_type].IsHighway())  // freeway or highway
				{
					bFreewayExistFlag = true;
					g_LinkVector[li]->m_MergeMainlineLinkID = m_InboundLinkAry[FromID][il];
				}
				if (bRampExistFlag && bFreewayExistFlag)
				{
					g_LinkVector[li]->m_bMergeFlag = 2; // merge with ramp and mainline street
					g_LogFile << "merge with ramp:" << g_NodeVector[g_LinkVector[li]->m_FromNodeNo].m_NodeID << " ->" << g_NodeVector[g_LinkVector[li]->m_ToNodeNo].m_NodeID;
					g_LogFile << " with onramp:" << g_NodeVector[g_LinkVector[g_LinkVector[li]->m_MergeOnrampLinkID]->m_FromNodeNo].m_NodeID << " ->" << g_NodeVector[g_LinkVector[g_LinkVector[li]->m_MergeOnrampLinkID]->m_ToNodeNo].m_NodeID;
					g_LogFile << " and freeway mainline:" << g_NodeVector[g_LinkVector[g_LinkVector[li]->m_MergeMainlineLinkID]->m_FromNodeNo].m_NodeID << " ->" << g_NodeVector[g_LinkVector[g_LinkVector[li]->m_MergeMainlineLinkID]->m_ToNodeNo].m_NodeID << endl;
					break;
				}

			}


		}

		if (g_LinkVector[li]->m_bMergeFlag == 1 || g_LinkVector[li]->m_bMergeFlag == 2)
		{
			// merge with several merging ramps
			int ij;
			int TotalNumberOfLanes = 0;
			for (ij = 0; ij < m_InboundSizeAry[FromID]; ij++)
			{
				TotalNumberOfLanes += g_LinkVector[m_InboundLinkAry[FromID][ij]]->m_OutflowNumLanes;
			}

			for (ij = 0; ij < m_InboundSizeAry[FromID]; ij++)
			{
				MergeIncomingLink mil;
				mil.m_LinkNo = m_InboundLinkAry[FromID][ij];
				mil.m_link_type = g_LinkVector[mil.m_LinkNo]->m_link_type;
				mil.m_OutflowNumLanes = g_LinkVector[mil.m_LinkNo]->m_OutflowNumLanes;
				mil.m_LinkInCapacityRatio = (float)(mil.m_OutflowNumLanes) / TotalNumberOfLanes;
				g_LinkVector[li]->MergeIncomingLinkVector.push_back(mil);
				g_LogFile << "merge into freeway with multiple freeway/ramps:" << "No." << ij << " " << g_NodeVector[g_LinkVector[mil.m_LinkNo]->m_FromNodeNo].m_NodeID << " -> " << g_NodeVector[g_LinkVector[mil.m_LinkNo]->m_ToNodeNo].m_NodeID << " with " << g_LinkVector[mil.m_LinkNo]->m_OutflowNumLanes << " lanes and in flow capacity split " << mil.m_LinkInCapacityRatio << endl;
			}

		}

	}

	// determine offramp
	for (unsigned li = 0; li < g_LinkVector.size(); li++)
	{
		if (g_LinkTypeMap[g_LinkVector[li]->m_link_type].IsRamp())
		{

			if (g_LinkVector[li]->m_bOnRampType == false)
			{

				g_LinkVector[li]->m_bOffRampType = true;

				g_LogFile << "Offramp:" << g_NodeVector[g_LinkVector[li]->m_FromNodeNo].m_NodeID << " -> " << g_NodeVector[g_LinkVector[li]->m_ToNodeNo].m_NodeID << endl;

				if (g_settings.use_point_queue_model_for_off_ramps)
				{
					g_LinkVector[li]->m_AgentSpaceCapacity = 99999; // change to point queue model for offramp
				}
			}
			else
			{
				if (g_settings.use_point_queue_model_for_on_ramps)
				{
					g_LinkVector[li]->m_AgentSpaceCapacity = 99999; // change to point queue model for offramp
				}

			}


		}

	}
}


void g_ProhibitMovement(int up_node_id, int node_id, int dest_node_id)
{
	string movement_id = GetMovementStringID(up_node_id, node_id, dest_node_id);

	if (g_NodeID2NoMap.find(node_id) == g_NodeID2NoMap.end())
		return;
	int node_no = g_NodeID2NoMap[node_id];
	g_NodeVector[node_no].m_MovementMap[movement_id].in_link_from_node_id = up_node_id;
	g_NodeVector[node_no].m_MovementMap[movement_id].in_link_to_node_id = node_id;
	g_NodeVector[node_no].m_MovementMap[movement_id].out_link_to_node_id = dest_node_id;

	g_NodeVector[node_no].m_MovementMap[movement_id].b_turning_prohibited = true;   // assign movement to individual node

	g_number_of_prohibited_movements++;
}

void g_ReadAMSMovementData()
{
	CCSVParser parser_movement;

	int count = 0;
	int zero_effective_green_time_error_count = 0;

	if (parser_movement.OpenCSVFile("input_movement.csv", false))  // not required
	{
		int up_node_id = 0;
		int dest_node_id = 0;


		while (parser_movement.ReadRecord())
		{
			int node_id = 0;
			parser_movement.GetValueByFieldName("node_id", node_id);
			if (g_NodeID2NoMap.find(node_id) == g_NodeID2NoMap.end())
				continue;  // skip this record


			std::string turn_type;

			parser_movement.GetValueByFieldName("turn_type", turn_type);

			std::string turn_direction;
			parser_movement.GetValueByFieldName("turn_direction", turn_direction);

			parser_movement.GetValueByFieldName("up_node_id", up_node_id);
			parser_movement.GetValueByFieldName("dest_node_id", dest_node_id);


			string strid = GetLinkStringID(up_node_id, node_id);
			if (g_LinkMap.find(strid) != g_LinkMap.end())
			{
				// map from turn dir_flag to link id
				g_NodeVector[g_NodeID2NoMap[node_id]].m_Movement2LinkIDStringMap[turn_direction] = strid;

			}

			int prohibited_flag = 0;

			parser_movement.GetValueByFieldName("prohibited_flag", prohibited_flag);


			if (prohibited_flag == 1)
			{
				g_ProhibitMovement(up_node_id, node_id, dest_node_id);


				continue; // do not need to check further
			}

		}

		parser_movement.CloseCSVFile();
	}


}




//------------------------------------------------------//

//------------------------------------------------------//

using namespace std;

float DTALink::GetTimeDependentCapacityAtSignalizedIntersectionFromTimingStatus(double CurrentTimeInMin)
{
	float simulation_time_interval_in_second = g_DTASimulationInterval_InMin * 60;

	int simulation_start_time = CurrentTimeInMin * 60;

	int green_time_count_in_seconds = 0;

	for (int t = simulation_start_time; t < simulation_start_time + simulation_time_interval_in_second; t++)
	{

		if (m_Timing_Status_Map.find(t) != m_Timing_Status_Map.end())  // has been defined as status of 1
		{
			green_time_count_in_seconds++;
		}
	}

	return green_time_count_in_seconds * 1.0 / simulation_time_interval_in_second * 1800;  // 1800 is the saturation flow rate per hour

}


void g_ReadSignalTimingStatusFile()
{
	//for (unsigned li = 0; li < g_LinkVector.size(); li++)
	//{

	//	g_LinkVector[li]->m_Timing_Status_Map.clear();

	//}

	//DTALink* pLink = 0;
	//CCSVParser parser_link;
	//int count = 0;
	//if (parser_link.OpenCSVFile("input_timing_status.csv"))
	//{
	//	cout << "reading signal data from input_timing_status.csv..." << endl;
	//	while (parser_link.ReadRecord())
	//	{
	//		int start_time_in_sec = 0;
	//		int end_time_in_sec = 0;
	//		int signal_status = -1;

	//		string turn_type;

	//		int from_node_id = 0;
	//		int to_node_id = 0;

	//		if (g_LinkMap.find(GetLinkStringID(from_node_id, to_node_id)) == g_LinkMap.end())
	//		{
	//			cout << "Link " << from_node_id << "-> " << to_node_id << " at line " << count + 1 << " of file input_timging_status.csv" << " has not been defined in road_link.csv. Please check.";
	//			g_ProgramStop();
	//		}

	//		parser_link.GetValueByFieldNameRequired("start_time_in_sec", start_time_in_sec);
	//		parser_link.GetValueByFieldNameRequired("end_time_in_sec", end_time_in_sec);
	//		parser_link.GetValueByFieldNameRequired("signal_status", signal_status);
	//		parser_link.GetValueByFieldNameRequired("turn_type", turn_type);


	//		DTALink* pLink = g_LinkMap[GetLinkStringID(from_node_id, to_node_id)];

	//		if (pLink != NULL && signal_status == 1)
	//		{
	//			pLink->m_bSignalizedArterialType = true;

	//		for (int t = start_time_in_sec; t < end_time_in_sec; t++)
	//		{
	//			pLink->m_Timing_Status_Map[t] = 1;    // assign the values to timing map, for the use in function GetTimeDependentCapacityAtSignalizedIntersectionFromTimingStatus
	//		}

	//	}

	//	g_LogFile << "timing records = " << count << endl;
	//	g_SummaryStatFile.WriteTextLabel("# of signal timing records=");
	//	g_SummaryStatFile.WriteNumber(count);

	//}
}


void g_DTALiteMultiScenarioMain()
{

	//if (g_InitializeLogFiles() == 0)
	//	return;


	g_SummaryStatFile.Open("output_summary.csv");
	g_SummaryStatFile.WriteTextLabel("DTALite:\nA Fast Open Source DTA Engine\n");
	g_SummaryStatFile.WriteTextLabel("Software Version =,2.0.0\nRelease Date=,");
	g_SummaryStatFile.WriteTextLabel(__DATE__);
	g_SummaryStatFile.WriteTextLabel("\n");



	cout << "DTALite: A Fast Open-Source DTA Simulation Engine" << endl;
	cout << "Version 2.0.0, Release Date " << __DATE__ << "." << endl;

	g_LogFile << "---DTALite: A Fast Open-Source DTA Simulation Engine---" << endl;
	g_LogFile << "Version 2.0.0, Release Date " << __DATE__ << "." << endl;


	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	CString time_str;
	//`January', `February', `March', `April', `May', `June', `July', `August', `September', `October', `November', `December'.
	time_str.Format("Simulation Date =,year: %d,month:%02d,day:%d,hour:%2d,min:%2d\n", now->tm_year + 1900, (now->tm_mon + 1), now->tm_mday, now->tm_hour, now->tm_min);
	g_SummaryStatFile.WriteTextLabel(time_str);

	int scenario_no;
	string scenario_name;

	int TestDemandLevel = 80;
	int TestFromNode = 0;
	int TestToNode = 0;
	float TestNumberOfLanes = 2;
	float LOVCost = 0;
	float HOVCost = 0;
	float TruckCost = 0;

	int NumberOfCriticalLinks = 3;
	int NumberOfCriticalODPairs = 3;


	int cl;


	int max_scenarios = 1;



	int traffic_flow_model = 1;

	int SignalRepresentationFlag = 0;

	g_CalculateUEGapForAllAgents = 1;

	g_LogFile << "Traffic Flow Model =  ";
	g_SummaryStatFile.WriteTextLabel("Traffic Flow Model =,");

	switch (g_TrafficFlowModelFlag)
	{
	case tfm_BPR: 		g_LogFile << "BPR Function" << endl;
		g_SummaryStatFile.WriteTextString("BPR Function");
		break;

	case tfm_point_queue: 		g_LogFile << "Point Queue Model" << endl;
		g_SummaryStatFile.WriteTextString("Point Queue Model");
		break;

	case tfm_newells_model: 		g_LogFile << "Newell's Cumulative Flow Count Model" << endl;
		g_SummaryStatFile.WriteTextString("Newell's Cumulative Flow Count Model");
		break;

		traffic_flow_model = tfm_newells_model;  // newell's model
		break;


	default: 		g_LogFile << "No Valid Model is Selected" << endl;
		g_SummaryStatFile.WriteTextString("Invalid Model");
		break;
	}






		g_SummaryStatFile.WriteParameterValue("Assignment method", "MSA");

	g_SummaryStatFile.WriteTextString(" ");




	g_ReadInputFiles();

	// if the global traffic flow model flag is not used define type, then apply the globally defined parameter in input_scenario_settings.csv to all links
	{
		for (unsigned li = 0; li < g_LinkVector.size(); li++)
		{
			g_LinkVector[li]->m_traffic_flow_model = g_TrafficFlowModelFlag;
		}

	}

	//			g_ReadScenarioInputFiles(scenario_no);

	cout << "Start Traffic Assignment/Simulation... " << endl;

	cout << "Agent based dynamic traffic assignment... " << endl;



		g_SummaryStatFile.WriteParameterValue("Routing method", "Individual agent-based routing");
		g_AgentBasedDynamicTrafficAssignmentSimulation();  // agent-based assignment

	g_SummaryStatFile.WriteTextLabel("\n");
	g_OutputSimulationStatistics(g_NumberOfIterations);

	///

	g_FreeMemory(0);  // free memory at the end of multiple scenarios
	exit(0);
}


//split the string by "_"
vector<string> split(const string &s, const string &seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}



vector<float> g_time_parser(vector<string>& inputstring)
{
	vector<float> output_global_minute;

	for (int k = 0; k < inputstring.size(); k++)
	{
		vector<string> sub_string = split(inputstring[k], "_");

		for (int i = 0; i < sub_string.size(); i++)
		{
			//HHMM
			//012345
			char hh1 = sub_string[i].at(0);
			char hh2 = sub_string[i].at(1);
			char mm1 = sub_string[i].at(2);
			char mm2 = sub_string[i].at(3);

			float hhf1 = ((float)hh1 - 48);
			float hhf2 = ((float)hh2 - 48);
			float mmf1 = ((float)mm1 - 48);
			float mmf2 = ((float)mm2 - 48);

			float hh = hhf1 * 10 * 60 + hhf2 * 60;
			float mm = mmf1 * 10 + mmf2;
			float global_mm_temp = hh + mm;
			output_global_minute.push_back(global_mm_temp);
		}
	}

	return output_global_minute;
} // transform hhmm to minutes




void g_AllocateDynamicArrayForAgents()
{
	if (g_TDOAgentArray == NULL)  // has not allocated memory yet
	{
		TRACE("Allocate memory for %d zones and %d SP calculation intervals.\n", g_ZoneVector.size(), g_NumberOfSPCalculationPeriods);

		g_TDOAgentArray = AllocateDynamicArray<AgentArrayForOriginDepartrureTimeInterval>(g_ZoneVector.size(), g_NumberOfSPCalculationPeriods);

		std::map<int, DTAAgent*>::iterator iterVM;
		for (iterVM = g_AgentMap.begin(); iterVM != g_AgentMap.end(); iterVM++)
		{

			DTAAgent* pAgent = iterVM->second;

			int AssignmentInterval = g_FindAssignmentIntervalIndexFromTime(pAgent->m_DepartureTime);
			g_TDOAgentArray[pAgent->m_o_ZoneNo][AssignmentInterval].AgentArray.push_back(pAgent->m_AgentID);
		}


	}
}

vector<int> ParseLineToIntegers(string line)
{
	vector<int> SeperatedIntegers;
	string subStr;
	istringstream ss(line);


	char Delimiter = ';';


	while (std::getline(ss, subStr, Delimiter))
	{
		int integer = atoi(subStr.c_str());
		SeperatedIntegers.push_back(integer);
	}
	return SeperatedIntegers;
}



bool AddPathToAgent(DTAAgent * pAgent, std::vector<int> path_node_sequence, CString FileName)
{

	if (pAgent->m_NodeSize >= 1 && pAgent->m_LinkAry != NULL)
	{
		delete pAgent->m_LinkAry;
	}

	pAgent->m_NodeSize = path_node_sequence.size();

	if (pAgent->m_NodeSize >= 1)  // in case reading error
	{
		pAgent->m_LinkAry = new SAgentLink[pAgent->m_NodeSize];
		pAgent->m_NodeIDSum = 0;

		pAgent->m_Distance = 0;  // reset distanace when there are new paths assigned.
		for (int i = 0; i < pAgent->m_NodeSize; i++)
		{

			int node_id;
			float event_time_stamp, travel_time;

			pAgent->m_NodeIDSum += path_node_sequence[i];

			if (i == 0)
				pAgent->m_o_NodeNo = g_NodeID2NoMap[path_node_sequence[0]];

			if (i == pAgent->m_NodeSize - 1)
				pAgent->m_d_NodeNo = g_NodeID2NoMap[path_node_sequence[pAgent->m_NodeSize - 1]];

			if (i >= 1)
			{
				DTALink* pLink = g_LinkMap[GetLinkStringID(path_node_sequence[i - 1], path_node_sequence[i])];
				if (pLink == NULL && FileName.GetLength() > 0)
				{
					CString msg;
					msg.Format("Error in reading link %d->%d for Agent id %d  in file %s.", path_node_sequence[i - 1], path_node_sequence[i], pAgent->m_AgentID, FileName);
					cout << msg << endl;

					return false;
				}

				pAgent->m_Distance += pLink->m_Length;

				pAgent->m_LinkAry[i - 1].LinkNo = pLink->m_LinkNo; // start from 0
			}


		}

	}
	return true;
}



bool g_ReadInputAgentCSVFile(string file_name, bool bOutputLogFlag)
{
	int	LineCount = 0;

	bool bOutputDebugLogFile = true;

	CCSVParser parser_agent;

	if (parser_agent.OpenCSVFile(file_name, false))
	{
		TRACE("**step 0: reading file %s\n", file_name.c_str());

		int line_no = 1;

		int i = 0;

		int count = 0;
		int count_for_sameOD = 0;
		int count_for_not_defined_zones = 0;
		while (parser_agent.ReadRecord())
		{
			//---initializing for checking status---//

			//--------------------------------------//

			if ((count + 1) % 1000 == 0)
			{
				cout << "reading " << count + 1 << " records..." << endl;

			}
			count++;

			int agent_id = 0;

			parser_agent.GetValueByFieldNameRequired("agent_id", agent_id);

			TRACE("--step 1: read agent id = %d \n", agent_id);

			DTAAgent sAgent;

			sAgent.m_AgentID = agent_id;

			bool bCreateNewAgent = false;

			if (g_AgentMap.find(agent_id) != g_AgentMap.end())
			{
				TRACE("--step 2: agent id =%d found in memory, will update data\n", agent_id);
				continue;
			}



			int o_zone_id = -1;
			int d_zone_id = -1;

			parser_agent.GetValueByFieldNameRequired("o_zone_id", o_zone_id);
			parser_agent.GetValueByFieldNameRequired("d_zone_id", d_zone_id);

			int o_node_id = -1;

			parser_agent.GetValueByFieldNameRequired("o_node_id", o_node_id);

			int dnode_id = -1;
			parser_agent.GetValueByFieldNameRequired("d_node_id", dnode_id);


			if (o_zone_id >= 1 && g_ZoneID2NoMap.find(o_zone_id) != g_ZoneID2NoMap.end())  // new Agent
			{
				sAgent.m_o_ZoneNo = g_ZoneID2NoMap[o_zone_id];
			}
			else
			{
				continue;
			}

			if (d_zone_id >= 1 && g_ZoneID2NoMap.find(d_zone_id) != g_ZoneID2NoMap.end())  // new Agent
			{
				sAgent.m_d_ZoneNo = g_ZoneID2NoMap[d_zone_id];
			}
			else
			{
				continue;
			}


			TRACE("--step 4: read o_zone_id = %d, d_zone_id=%d \n", o_zone_id, d_zone_id);


			if (g_NodeID2NoMap.find(o_node_id) != g_NodeID2NoMap.end())  // convert node number to internal node id
			{
				sAgent.m_o_NodeNo = g_NodeID2NoMap[o_node_id];
			}
			else
			{

				sAgent.m_o_NodeNo = g_ZoneVector[sAgent.m_o_ZoneNo].GetRandomOriginNodeIDInZone((sAgent.m_AgentID % 100) / 100.0f);  // use sAgent.m_AgentID/100.0f as random number between 0 and 1, so we can reproduce the results easily
			}

			if (g_NodeID2NoMap.find(dnode_id) != g_NodeID2NoMap.end()) // convert node number to internal node id
			{
				sAgent.m_d_NodeNo = g_NodeID2NoMap[dnode_id];
			}
			else
			{
				sAgent.m_d_NodeNo = g_ZoneVector[sAgent.m_d_ZoneNo].GetRandomDestinationIDInZone((sAgent.m_AgentID % 100) / 100.0f);

			}

			TRACE("--step 5: read o_node_id = %d, d_node_id=%d \n",
				o_node_id, dnode_id);


			string time_period;

			int agent_type = 0;
			float PCE = 1.0;

			std::vector<int> path_node_sequence;

			if (parser_agent.GetValueByFieldName("agent_type", agent_type) == true)
			{
				sAgent.m_DemandType = agent_type;

				TRACE("--step 7: read agent_type = %d\n",
					agent_type);

				parser_agent.GetValueByFieldName("PCE", sAgent.m_PCE );

				//====feasible path checking==========//
				string path_node_sequence_str;
				parser_agent.GetValueByFieldName("node_sequence", path_node_sequence_str);

				if (path_node_sequence_str.size() > 2)
				{
					path_node_sequence = ParseLineToIntegers(path_node_sequence_str);


				}
				//------------------------------------//

			}

			float departure_time_start = -1;
			float departure_time_end = -1;

			if (parser_agent.GetValueByFieldNameRequired("time_period", time_period) == true)
			{

				vector<float> global_minute_vector;

				vector<string> input_string;

				input_string.push_back(time_period);
				//input_string includes the start and end time of a time period with hhmm format
				global_minute_vector = g_time_parser(input_string); //global_minute_vector incldue the starting and ending time
				if (global_minute_vector.size() == 2)
				{

					departure_time_start = global_minute_vector[0];
					departure_time_end = global_minute_vector[1];

					if (departure_time_start < g_DemandLoadingStartTimeInMin)
					{
						g_DemandLoadingStartTimeInMin = departure_time_start;
					}
					if (departure_time_end > g_DemandLoadingEndTimeInMin)
					{

						g_DemandLoadingEndTimeInMin = departure_time_end;
					}

				}

				double volume = 1.0000;
				if (parser_agent.GetValueByFieldName("volume", volume) == true)
				{

					int number_of_agents = g_GetRandomInteger(volume);


					// additional error checking for updating agent data

					for (int i = 0; i < number_of_agents; i++)
					{
						DTAAgent* pAgent = new (std::nothrow) DTAAgent;
						if (pAgent == NULL)
						{
							cout << "Insufficient memory...";
							getchar();
							exit(0);

						}
						if(number_of_agents ==1)
							pAgent->m_AgentID = sAgent.m_AgentID;
						else
							pAgent->m_AgentID = sAgent.m_AgentID * 10000 + i;

						pAgent->m_o_ZoneNo = sAgent.m_o_ZoneNo;
						pAgent->m_d_ZoneNo = sAgent.m_d_ZoneNo;
						pAgent->m_o_NodeNo = sAgent.m_o_NodeNo;
						pAgent->m_d_NodeNo = sAgent.m_d_NodeNo;
						pAgent->m_DemandType = sAgent.m_DemandType;
						pAgent->m_PCE = sAgent.m_PCE;

						pAgent->m_RandomSeed = sAgent.m_AgentID;


						// generate the random departure time during the interval
						float RandomRatio = 0;
						if (number_of_agents < 10) // small demand volume
						{
							RandomRatio = g_GetRandomRatio(); // use random number twice to determine exact departure time of vehicles
						}
						else
						{
							RandomRatio = (i + 1)*1.0f / (number_of_agents + 1); // uniform distribution
						}

						pAgent->m_DepartureTime = departure_time_start + RandomRatio * (departure_time_end - departure_time_start);

						pAgent->m_ArrivalTime = 0;
						pAgent->m_bComplete = false;
						pAgent->m_bLoaded = false;
						pAgent->m_TollDollarCost = 0;
						pAgent->m_Distance = 0;
						pAgent->m_NodeSize = 0;
						pAgent->m_NodeIDSum = 0;


						TRACE("--step 11: create new path\n");


						if (path_node_sequence.size() >= 2)
						{
							if (AddPathToAgent(pAgent, path_node_sequence, file_name.c_str()) == FALSE)  // some links are not correctly defined.
								continue;

						}

						g_AgentMap[pAgent->m_AgentID] = pAgent;
						g_AgentVector.push_back(pAgent);

						int simulation_time_intervalNo = (int)(pAgent->m_DepartureTime * g_simulation_time_intervals_per_min);
						g_AgentTDListMap[simulation_time_intervalNo].m_AgentIDVector.push_back(pAgent->m_AgentID);

					}
				}
				else
				{
					cout << "Error: File input_agent.csv does not have a field of volume." << endl;
					g_ProgramStop();
				}


			}

			line_no++;

		}

			if (bOutputLogFlag)
			{

				cout << i << " agents have been read from file " << file_name << endl;

				if (count_for_sameOD >= 1)
					cout << "there are " << count_for_sameOD << " agents with the same o_zone_id and d_zone_id, which will not be simulated. " << endl;


				if (count_for_not_defined_zones >= 1)
					cout << "there are " << count_for_not_defined_zones << " agents with zones not being defined in input_zone.csv file, which will not be simulated. " << endl;

			}
			LineCount = count;

	}

	return true;

}


bool g_AgentBasedShortestPathGeneration()
{

	// find unique origin nodegf
	// find unique destination node

	int node_size = g_NodeVector.size();
	int link_size = g_LinkVector.size();

	int line = 0;

	bool bFirstGetChar = false;

	FILE* st_input = NULL;
	fopen_s(&st_input, "input_agent.csv", "r");
	if (st_input != NULL)
	{
		cout << "reading file input_od_pairs.csv " << endl;

		char str[100];

		fscanf(st_input, "%[^\n]", str);  // read a line

		int o_node_id, d_node_id, agent_id;

		while (!feof(st_input))
		{
			o_node_id = 0;
			d_node_id = 0;
			agent_id = 0;

			int number_of_values = fscanf(st_input, "%d,%d,%d\n", &agent_id, &o_node_id, &d_node_id);

			// the expected number of items = 3;
			if (number_of_values < 3)
				break;

			TRACE("%d;%d;%d\n", agent_id, &o_node_id, &d_node_id);

			if (g_NodeID2NoMap.find(o_node_id) == g_NodeID2NoMap.end())
			{
				cout << "o_node_id " << o_node_id << " in input_od_pairs.csv has not be defined in input_node.csv.  Please check line =" << line << endl;
				if (bFirstGetChar == false)
				{
					cout << "press any key to continue " << endl;
					getchar();
					bFirstGetChar = true;
				}
				//exit(0);
				continue;
			}

			if (g_NodeID2NoMap.find(d_node_id) == g_NodeID2NoMap.end())
			{
				cout << "d_node_id " << d_node_id << " in input_od_pairs.csv has not be defined in input_node.csv. Please check line =" << line << endl;
				if (bFirstGetChar == false)
				{
					cout << "press any key to continue " << endl;
					getchar();
					bFirstGetChar = true;
				}
				//exit(0);
				continue;
			}

			int node_no = g_NodeID2NoMap[o_node_id];
			int dest_node_index = g_NodeID2NoMap[d_node_id];

			DTADestination element;
			element.destination_number = d_node_id;
			element.agent_id = agent_id;
			element.d_node_no = dest_node_index;

			g_NodeVector[node_no].m_DestinationVector.push_back(element);
			g_NodeVector[node_no].m_bOriginFlag = true;

			if (line % 10000 == 0)
				cout << g_GetAppRunningTime() << " reading line " << line / 1000 << "k in input_od_pairs.csv." << endl;
			line++;
		}
		fclose(st_input);
	}
	else
	{
		return false;
	}


	unsigned int i;
	int UniqueOriginSize = 0;
	int UniqueDestinationSize = 0;
	int number_of_threads = omp_get_max_threads();


	for (i = 0; i < g_NodeVector.size(); i++)
	{
		if (g_NodeVector[i].m_bOriginFlag == true)
		{
			UniqueOriginSize += 1;
		}



		if (g_NodeVector[i].m_bDestinationFlag == true)
			UniqueDestinationSize += 1;

	}

	cout << "# of OD pairs = " << line << endl;
	cout << "# of unique origins = " << UniqueOriginSize << " with " << line / max(UniqueOriginSize, 1) << " nodes per origin" << endl;
	cout << "# of processors = " << number_of_threads << endl;

	g_LogFile << "# of OD pairs = " << line << endl;

	g_LogFile << "# of unique origins = " << UniqueOriginSize << " with " << line / max(UniqueOriginSize, 1) << " nodes per origin" << endl;
	g_LogFile << g_GetAppRunningTime() << "# of processors = " << number_of_threads << endl;



#pragma omp parallel for
	for (int ProcessID = 0; ProcessID < number_of_threads; ProcessID++)
	{
		// create network for shortest path calculation at this processor
		DTANetworkForSP g_TimeDependentNetwork_MP(node_size, link_size, 1, g_AdjLinkSize); //  network instance for single processor in multi-thread environment
		int	cpu_id = omp_get_thread_num();  // starting from 0
		g_TimeDependentNetwork_MP.BuildPhysicalNetwork(0, 0, g_TrafficFlowModelFlag);  // build network for this zone, because different zones have different connectors...

		for (int node_index = 0; node_index < node_size; node_index++)
		{
			if (node_index %number_of_threads == ProcessID && g_NodeVector[node_index].m_bOriginFlag)
			{
				if (node_index % 100 == cpu_id)
				{

					if (node_index % 100 == 0)  // only one CUP can output to log file
					{
						cout << g_GetAppRunningTime() << "processor " << cpu_id << " working on node  " << node_index << ", " << node_index * 1.0f / node_size * 100 << "%" << endl;
						g_LogFile << g_GetAppRunningTime() << "processor " << cpu_id << " working on node  " << node_index << ", " << node_index * 1.0f / node_size * 100 << "%" << endl;
					}
				}

				// this generate the one-to-all shortest path tree
				g_TimeDependentNetwork_MP.TDLabelCorrecting_DoubleQueue(node_index, 0, 0, 1, DEFAULT_VOT, true, false, false);  // g_NodeVector.size() is the node ID corresponding to CurZoneNo

				for (int dest_no = 0; dest_no < g_NodeVector[node_index].m_DestinationVector.size(); dest_no++)
				{
					int dest_node_index = g_NodeVector[node_index].m_DestinationVector[dest_no].d_node_no;
					g_NodeVector[node_index].m_DestinationVector[dest_no].destination_node_distance = g_TimeDependentNetwork_MP.LabelCostAry[dest_node_index];
					//					 TRACE("Label: %f: \n",g_NodeVector[node_index].m_DestinationVector[dest_no].destination_node_cost_label);

				}

			}
		}
	}


	FILE* st = NULL;
	fopen_s(&st, "output_accessibility.csv", "w");
	if (st != NULL)
	{
		fprintf(st, "agent_id,o_node_id,d_node_id,distance\n");
		for (int node_index = 0; node_index < node_size; node_index++)
		{
			if (node_index % 100000 == 0)
				cout << g_GetAppRunningTime() << " DTALite is outputing results for node no. " << node_index << endl;
			for (int dest_no = 0; dest_no < g_NodeVector[node_index].m_DestinationVector.size(); dest_no++)
			{
				int dest_node_index = g_NodeVector[node_index].m_DestinationVector[dest_no].d_node_no;
				float label = g_NodeVector[node_index].m_DestinationVector[dest_no].destination_node_distance;

				fprintf(st, "%d,%d,%d,%4.3f\n", g_NodeVector[node_index].m_DestinationVector[dest_no].agent_id,
					g_NodeVector[node_index].m_NodeID,
					g_NodeVector[node_index].m_DestinationVector[dest_no].destination_number, label);

			}

		}

		fclose(st);

		cout << "please check file output_accessibility.csv. " <<endl;
	}
	else
	{
		cout << "File output_shortest_path.csv cannot be opened. Please check!" << endl;
		g_ProgramStop();

	}

	cout << g_GetAppRunningTime() << " Program completes!" << endl;
	exit(1);
	return true;
}



using namespace std;

CString g_timestamp_HHMM_convertor(float time1)
{
	// in min

	CString str;

	int hour_1 = max(0, time1 / 60);
	int min_1 = (time1 - hour_1 * 60);
	int sec = (time1 * 60 - hour_1 * 60 * 60 - min_1 * 60);

	if (time1 < 9999)
		str.Format("%02d%02d:%02d", hour_1, min_1, sec);
	else
		str.Format("9999");

	return str;

}


CString g_time_period_HHMM_convertor(float time1, float time2)
{
	// in min

	CString str;

	int hour_1 = time1 / 60;
	int min_1 = (time1 - hour_1 * 60);
	int sec_1 = (time1 * 60 - hour_1 * 60 * 60 - min_1 * 60);

	int hour_2 = time2 / 60;
	int min_2 = (time2 - hour_2 * 60);
	int sec_2 = (time2 * 60 - hour_2 * 60 * 60 - min_2 * 60);


	str.Format("%02d%02d:%02d_%02d%02d:%02d", hour_1, min_1, sec_1, hour_2, min_2, sec_2);

	return str;

}


void OutputTimeDependentODMOEData(ofstream &output_ODMOE_file, int department_time_intreval, int end_of_period, int cut_off_volume)
{

	// step 1: collect statistics based on Agent file


	output_ODMOE_file << "o_zone_id,d_zone_id,agent_type,departure_time,number_of_agents,travel_time_in_min,distance" << endl;

	ODStatistics*** ODMOE3dArray = NULL;

	int StatisticsIntervalSize = (g_DemandLoadingEndTimeInMin - g_DemandLoadingStartTimeInMin) / department_time_intreval;

	cout << "allocating memory for time-dependent ODMOE data...for " << g_ZoneSize << " X " << g_ZoneSize << "zones for " << StatisticsIntervalSize << " 15-min time intervals" << endl;
	ODMOE3dArray = Allocate3DDynamicArray<ODStatistics>(g_ZoneSize + 1, g_ZoneSize + 1, StatisticsIntervalSize);
	cout << "end of allocating memory for time-dependent ODMOE data." << endl;

	for (int agent_type = 1; agent_type <= g_DemandTypeVector.size(); agent_type++)
	{


		for (int o = 0; o < g_ZoneSize; o++)
			for (int d = 0; d < g_ZoneSize; d++)
				for (int StatisticsInterval = 0; StatisticsInterval < StatisticsIntervalSize; StatisticsInterval++)
				{

					ODMOE3dArray[o][d][StatisticsInterval].TotalAgentSize = 0;
					ODMOE3dArray[o][d][StatisticsInterval].TotalTravelTime = 0;
					ODMOE3dArray[o][d][StatisticsInterval].TotalDistance = 0;
				}

		std::map<int, DTAAgent*>::iterator iterVM;
		for (iterVM = g_AgentMap.begin(); iterVM != g_AgentMap.end(); iterVM++)
		{

			DTAAgent* pAgent = iterVM->second;

			if (pAgent->m_NodeSize >= 2 && pAgent->m_bComplete)  // with physical path in the network
			{
				if (pAgent->m_DemandType != agent_type)
					continue;

				int StatisticsInterval = (pAgent->m_DepartureTime - g_DemandLoadingStartTimeInMin) / department_time_intreval;

				if (StatisticsInterval >= StatisticsIntervalSize)  // avoid out of bound errors
					StatisticsInterval = StatisticsIntervalSize - 1;

				int o_no = g_ZoneID2NoMap[pAgent->m_o_ZoneNo];
				int d_no = g_ZoneID2NoMap[pAgent->m_d_ZoneNo];

				ODMOE3dArray[o_no][d_no][StatisticsInterval].TotalAgentSize += 1;
				ODMOE3dArray[o_no][d_no][StatisticsInterval].TotalTravelTime += pAgent->m_TravelTime;
				ODMOE3dArray[o_no][d_no][StatisticsInterval].TotalDistance += pAgent->m_Distance;

			}
		}


		cout << "output od-based MOE statistics..." << endl;

		// step 2: output od-based MOE statistics
		for (int i = 0; i < g_ZoneSize; i++)
			for (int j = 0; j < g_ZoneSize; j++)
				for (int t = 0; t < StatisticsIntervalSize; t++)
				{
					int number_of_Agents = ODMOE3dArray[i][j][t].TotalAgentSize;

					if (ODMOE3dArray[i][j][t].TotalAgentSize >= 1)
					{
						float avg_travel_time = ODMOE3dArray[i][j][t].TotalTravelTime / max(1, number_of_Agents);
						float avg_travel_distance = ODMOE3dArray[i][j][t].TotalDistance / max(1, number_of_Agents);

						output_ODMOE_file << g_ZoneVector[i].m_ZoneID <<
							"," << g_ZoneVector[j].m_ZoneID << "," << agent_type << "," <<
							g_DemandLoadingStartTimeInMin + t * +department_time_intreval << "," <<
							number_of_Agents << ","
							<< avg_travel_time << ","
							<< avg_travel_distance << "," << endl;
					}
				}
	}

	if (ODMOE3dArray != NULL)
		Deallocate3DDynamicArray<ODStatistics>(ODMOE3dArray, g_ZoneSize + 1, g_ZoneSize + 1);


}



int g_OutputSimulationSummary(float& AvgTripTime, float& AvgDistance, float& AvgSpeed, float& AvgCost, int InformationClass = -1, int DemandType = -1, int AgentType = -1, int DepartureTimeInterval = -1)
{
	float TripTimeSum = 0;
	float TravelCostSum = 0;
	float DistanceSum = 0;
	int AgentCount = 0;

	for (vector<DTAAgent*>::iterator v = g_AgentVector.begin(); v != g_AgentVector.end();v++)
	{

		DTAAgent* pAgent = (*v);
		if (pAgent->m_bComplete)  // Agent completes the trips
		{
			if (
				(pAgent->m_DemandType == DemandType || DemandType == -1)
				&& (g_FindAssignmentIntervalIndexFromTime(pAgent->m_DepartureTime) == DepartureTimeInterval || DepartureTimeInterval == -1))
			{
				TripTimeSum += pAgent->m_TripTime;
				TravelCostSum += pAgent->m_TollDollarCost;

				DistanceSum += pAgent->m_Distance;

				AgentCount++;

			}
		}
	}

	AvgTripTime = TripTimeSum / max(1, AgentCount);  // unit: min
	AvgDistance = DistanceSum / max(1, AgentCount);  // unit: mile
	AvgSpeed = AvgDistance / max(0.0001, AvgTripTime) * 60;  // unit: mph
	AvgCost = TravelCostSum / max(1, AgentCount);  // unit: mph

	return AgentCount;
}




void g_OutputSimulationStatistics(int Iteration)
{

	std::vector<int> MOE_time_interval_vector;
	MOE_time_interval_vector.push_back(60);
	MOE_time_interval_vector.push_back(15);

	for (int MOE_interval_index = 0; MOE_interval_index < MOE_time_interval_vector.size(); MOE_interval_index++)
	{

		int time_interval = MOE_time_interval_vector[MOE_interval_index];

		CString title_str;
		title_str.Format("\n--Time Dependent Network MOE--,%d min interval", time_interval);
		g_SummaryStatFile.WriteTextString(title_str);
		g_SummaryStatFile.Reset();
		g_SummaryStatFile.SetFieldName("time_stamp_in_min");
		g_SummaryStatFile.SetFieldName("time_clock");
		g_SummaryStatFile.SetFieldName("cumulative_in_flow_count");
		g_SummaryStatFile.SetFieldName("cumulative_out_flow_count");
		g_SummaryStatFile.SetFieldName("number_agents_in_network");
		g_SummaryStatFile.SetFieldName("number_agents_entering_network");
		g_SummaryStatFile.SetFieldName("total_agent_distance_traveled");
		g_SummaryStatFile.SetFieldName("avg_trip_time (departure_time_based)");
		g_SummaryStatFile.SetFieldName("avg_trip_time_index = trip time/FFTT");
		g_SummaryStatFile.SetFieldName("avg_distance");
		g_SummaryStatFile.SetFieldName("avg_speed");

		g_SummaryStatFile.WriteHeader(false, false);

		int start_time = (int)(g_DemandLoadingStartTimeInMin / time_interval)*time_interval;

		for (int time = start_time; time < min(g_NetworkMOEAry.size(), g_PlanningHorizon);time++)
		{

			if (time >= 1)
			{
				// deal with interval without data
				if (g_NetworkMOEAry[time].CumulativeInFlow < g_NetworkMOEAry[time - 1].CumulativeInFlow)
					g_NetworkMOEAry[time].CumulativeInFlow = g_NetworkMOEAry[time - 1].CumulativeInFlow;

				if (g_NetworkMOEAry[time].CumulativeOutFlow < g_NetworkMOEAry[time - 1].CumulativeOutFlow)
					g_NetworkMOEAry[time].CumulativeOutFlow = g_NetworkMOEAry[time - 1].CumulativeOutFlow;
			}


			if (time%time_interval == 0)
			{
				g_SummaryStatFile.SetValueByFieldName("time_stamp_in_min", time);
				std::string time_str = GetTimeClockString(time);
				g_SummaryStatFile.SetValueByFieldName("time_clock", time_str);
				g_SummaryStatFile.SetValueByFieldName("cumulative_in_flow_count", g_NetworkMOEAry[time].CumulativeInFlow);
				g_SummaryStatFile.SetValueByFieldName("cumulative_out_flow_count", g_NetworkMOEAry[time].CumulativeOutFlow);

				int previous_time_interval = 0;
				if (time > start_time)
					previous_time_interval = time - time_interval;

				int count_previous_time_interval = g_NetworkMOEAry[previous_time_interval].CumulativeInFlow;

				int number_Agents_entering_network = g_NetworkMOEAry[time].CumulativeInFlow - count_previous_time_interval;

				g_SummaryStatFile.SetValueByFieldName("number_Agents_entering_network", number_Agents_entering_network);

				int number_Agents_in_network = g_NetworkMOEAry[time].CumulativeInFlow - g_NetworkMOEAry[time].CumulativeOutFlow;
				g_SummaryStatFile.SetValueByFieldName("number_Agents_in_network", number_Agents_in_network);

				int arrival_flow = 0;
				float total_trip_time = 0;
				float total_free_flow_trip_time = 0;
				float total_distance = 0;
				float total_waiting_time_at_origin = 0;

				for (int tt = time; tt < min(g_NetworkMOEAry.size(), time + time_interval); tt++)
				{
					arrival_flow += g_NetworkMOEAry[tt].Flow_in_a_min;
					total_trip_time += g_NetworkMOEAry[tt].AbsArrivalTimeOnDSN_in_a_min;
					total_free_flow_trip_time += g_NetworkMOEAry[tt].TotalFreeFlowTravelTime;
					total_distance += g_NetworkMOEAry[tt].TotalDistance;
					total_waiting_time_at_origin += g_NetworkMOEAry[tt].TotalBufferWaitingTime;

				}

				float avg_trip_time = total_trip_time / max(1, arrival_flow);
				float avg_trip_time_index = total_trip_time / max(0.001, total_free_flow_trip_time);

				float avg_distance = total_distance / max(1, arrival_flow);
				float avg_speed = total_distance / max(1, total_trip_time) * 60;

				g_SummaryStatFile.SetValueByFieldName("total_in_flow", arrival_flow);
				g_SummaryStatFile.SetValueByFieldName("avg_trip_time (departure_time_based)", avg_trip_time);
				g_SummaryStatFile.SetValueByFieldName("avg_trip_time_index = trip time/FFTT", avg_trip_time_index);

				g_SummaryStatFile.SetValueByFieldName("total_Agent_mile_traveled", total_distance);
				g_SummaryStatFile.SetValueByFieldName("avg_distance", avg_distance);
				g_SummaryStatFile.SetValueByFieldName("avg_speed", avg_speed);

				g_SummaryStatFile.WriteRecord();
			}


			if ((g_NetworkMOEAry[time].CumulativeInFlow - g_NetworkMOEAry[time].CumulativeOutFlow) == 0)
				break; // stop, as all Agents leave.
		}

	}
}



void OutputAgentTrajectoryData(std::string fname_agent)
{

	FILE* st_agent = NULL;

	fopen_s(&st_agent, fname_agent.c_str(), "w");



	if (st_agent != NULL)
	{
		std::map<int, DTAAgent*>::iterator iterVM;
		int AgentCount_withPhysicalPath = 0;
		// output statistics
		fprintf(st_agent, "agent_id,o_zone_id,d_zone_id,o_node_id,d_node_id,agent_type,PCE,time_period,distance,travel_time,node_sequence,time_sequence\n");


		for (iterVM = g_AgentMap.begin(); iterVM != g_AgentMap.end(); iterVM++)
		{

			DTAAgent* pAgent = iterVM->second;


			if (pAgent->m_NodeSize >= 2)  // with physical path in the network
			{


				int UpstreamNodeNo = 0;
				int DownstreamNodeNo = 0;

				int LinkID_0 = pAgent->m_LinkAry[0].LinkNo;
				UpstreamNodeNo = g_LinkVector[LinkID_0]->m_FromNodeNo;
				DownstreamNodeNo = g_LinkVector[LinkID_0]->m_ToNodeNo;

				float TripTime = 0;

				if (pAgent->m_bComplete)
					TripTime = max(0, pAgent->m_ArrivalTime - pAgent->m_DepartureTime);


				float m_gap = 0;
				fprintf(st_agent, "%d,%d,%d,%d,%d,%d,%6.4f,",
					pAgent->m_AgentID,
					g_ZoneVector[pAgent->m_o_ZoneNo].m_ZoneID,
					g_ZoneVector[pAgent->m_d_ZoneNo].m_ZoneID,
					g_NodeVector[pAgent->m_o_NodeNo].m_NodeID,
					g_NodeVector[pAgent->m_d_NodeNo].m_NodeID,
					pAgent->m_DemandType,
					pAgent->m_PCE);

				fprintf(st_agent, "%s,", g_time_period_HHMM_convertor(pAgent->m_DepartureTime, pAgent->m_DepartureTime));

				fprintf(st_agent, "%4.3f,%4.3f,", pAgent->m_Distance, TripTime);
				// path_node_sequence
				int j = 0;

				if (g_LinkVector[pAgent->m_LinkAry[0].LinkNo] == NULL)
				{

					cout << "Error: Agent" << pAgent->m_AgentID << "at LinkID" << pAgent->m_LinkAry[0].LinkNo << endl;
					cin.get();  // pause

				}

				int NodeNo = g_LinkVector[pAgent->m_LinkAry[0].LinkNo]->m_FromNodeNo;  // first node
				int NodeID = g_NodeVector[NodeNo].m_NodeID;

				fprintf(st_agent, "%d;", NodeID);

				for (j = 0; j < pAgent->m_NodeSize - 1; j++)  // for all nodes
				{
					int LinkID = pAgent->m_LinkAry[j].LinkNo;
					int NodeNo = g_LinkVector[LinkID]->m_ToNodeNo;
					int NodeID = g_NodeVector[NodeNo].m_NodeID;
					fprintf(st_agent, "%d;", NodeID);
				}


				fprintf(st_agent, ",");


				// path timestamp sequence

				if (pAgent->m_bLoaded == true)
				{

					fprintf(st_agent, "%s;", g_timestamp_HHMM_convertor(pAgent->m_LeavingTimeFromLoadingBuffer));
					for (int j = 0; j < pAgent->m_NodeSize - 1; j++)  // for all nodes
					{
						float time = pAgent->m_LinkAry[j].AbsArrivalTimeOnDSN;
						if (time >= 0 && time < 1440)
						{
							fprintf(st_agent, "%s;", g_timestamp_HHMM_convertor(time));
						}
					}
					fprintf(st_agent, "\n");
				}

			}
		} // for all paths

		fclose(st_agent);

	}
	else
	{
		cout << "Error: File agent.csv cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
		getchar();
	}

}


void g_OutputLinkPerformanceData()
{
	FILE* st = NULL;

	int link_performance_aggregation_time_interval_in_min = g_GetPrivateProfileInt("output", "link_performance_aggregation_time_interval_in_min", 1, g_DTASettingFileName);

	if (link_performance_aggregation_time_interval_in_min < 1)
		link_performance_aggregation_time_interval_in_min = 1;

	fopen_s(&st, "link_performance.csv", "w");

	if (st != NULL)
	{
		fprintf(st, "road_link_id,time_period,travel_time,delay,volume,density,speed,queue\n");

		for (unsigned li = 0; li < g_LinkVector.size(); li++)
		{
			DTALink* pLink = g_LinkVector[li];
			for (int time = g_DemandLoadingStartTimeInMin; time <= g_SimululationReadyToEnd; time += link_performance_aggregation_time_interval_in_min)
			{

				if (time < pLink->m_LinkMOEAry.size())
				{
					float LinkOutFlow = float(pLink->GetDepartureFlow(time));
					float travel_time = pLink->GetTravelTimeByMin(time, link_performance_aggregation_time_interval_in_min, pLink->m_traffic_flow_model);
					float density = (pLink->m_LinkMOEAry[time].CumulativeArrivalCount - pLink->m_LinkMOEAry[time].CumulativeDepartureCount)*1.0 / pLink->m_Length / max(1, pLink->m_InflowNumLanes);

					float queue_length_ratio = 0;
					queue_length_ratio = pLink->m_LinkMOEAry[time].ExitQueueLength / (pLink->m_KJam * pLink->m_Length * pLink->m_InflowNumLanes); /* in ratio*/
					int p = g_FindAssignmentIntervalIndexFromTime(time);  // based on departure time

					if (LinkOutFlow > 0 || (travel_time - pLink->m_FreeFlowTravelTime) > 0 || density > 0)
					{
						fprintf(st, "%s,%s,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,",
							pLink->m_road_link_id.c_str(),
							g_time_period_HHMM_convertor(time, time + link_performance_aggregation_time_interval_in_min),
							travel_time,
							travel_time - pLink->m_FreeFlowTravelTime,
							LinkOutFlow,
							pLink->GetSpeed(time),
							queue_length_ratio);
						fprintf(st, "\n");
					}

				}

			}

		}
		fclose(st);
	}
	else
	{
		cout << "Error: File link_performance.csv.csv cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
		cin.get();  // pause
	}
}


double network_simulation(int a, int b)
{

g_ReadDTALiteSettings();
g_DTALiteMultiScenarioMain();
return 1;
}
