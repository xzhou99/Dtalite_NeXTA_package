//  Portions Copyright 2010-2020 Xuesong (Simon) Zhou

//   If you help write or modify the code, please also list your names here.
//   The reason of having Copyright info here is to ensure all the modified version, as a whole, under the GPL 
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

#pragma once

#pragma warning(disable:4244)  // stop warning: "conversion from 'int' to 'float', possible loss of data"
#pragma warning(disable:4996)  // stop warning: 'MBCS_Support_Deprecated_In_MFC': MBCS support in MFC is deprecated 


#include <math.h>
#include <deque>
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <list>

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

using std::string;
using std::ifstream;
using std::vector;
using std::map;
using std::istringstream;
using std::ostringstream;

#include <iostream>
#include <fstream>


using namespace std;
#define _MAX_NUMBER_OF_PROCESSORS  64
#define PI 3.1415626
#define _MAX_STRING_LINE 30000
#define _MAX_TRANSIT_GRID_SIZE 100


enum e_traffic_flow_model { tfm_BPR =0, tfm_point_queue, tfm_newells_model};
extern e_traffic_flow_model g_TrafficFlowModelFlag;


// extention for multi-day equilibirum
#define MAX_FIFO_QUEUESIZE 5000
#define MAX_PATH_LINK_SIZE 300
#define MAX_MEASUREMENT_INTERVAL 15 

#define MAX_INFO_CLASS_SIZE 7
#define MAX_agent_type_SIZE  40 // because C starts from 0 
#define MAX_TIME_INTERVAL_SIZE 300

#define MAX_SIZE_INFO_USERS 5 
#define DEFAULT_VOT 12
#define _MAX_ODT_PATH_SIZE_4_ODME 50
#define _MAX_PATH_NODE_SIZE_4_ODME 300

extern float g_simulation_time_interval_step;
extern int g_simulation_time_intervals_per_min;


extern int g_ZoneSize;
extern int g_number_of_prohibited_movements;

enum Traffic_State {FreeFlow,PartiallyCongested,FullyCongested};

extern double g_DTASimulationInterval_InMin;
extern int g_CalculateUEGapForAllAgents;

#define	MAX_SPLABEL 9999.0f  // this value cannot be further increase as an extremely large value could lead to overflow
#define MAX_TIME_INTERVAL_ADCURVE 300  // 300 simulation intervals of data are stored to keep tract Cumulative flow counts of each link
extern int g_AggregationTimetInterval;
extern int g_TDSPTimetIntervalSizeForMin;

extern float g_MinimumInFlowRatio;
extern float g_RelaxInFlowConstraintAfterDemandLoadingTime;
extern float g_MaxDensityRatioForAgentLoading;
extern float g_DefaultSaturationFlowRate_in_vehphpl;

#ifdef _WIN64
#define MAX_LINK_NO 99999999
#endif 

#ifndef _WIN64
#define MAX_LINK_NO 65530
#endif

#define MAX_NODE_SIZE_IN_A_PATH 3000
#define MAX_LINK_SIZE_IN_VMS 20

#define MAX_CPU_SIZE 20
// Linear congruential generator 
#define LCG_a 17364
#define LCG_c 0
#define LCG_M 65521  // it should be 2^32, but we use a small 16-bit number to save memory

void g_ProgramStop();
extern int g_ProgramStopFlag;
void g_ProgramTrace(CString str);
float g_RNNOF();
string GetLinkStringID(int FromNodeID, int ToNodeID);

extern int g_NumberOfSPCalculationPeriods;
extern int g_FindAssignmentIntervalIndexFromTime(float time_in_min);

template <typename T>
T **AllocateDynamicArray(int nRows, int nCols)
{
	T **dynamicArray;

	dynamicArray = new (std::nothrow) T*[nRows];

	if (dynamicArray == NULL)
	{
		cout << "Error: insufficient memory.";
		g_ProgramStop();

	}

	for (int i = 0; i < nRows; i++)
	{
		dynamicArray[i] = new (std::nothrow) T[nCols];

		if (dynamicArray[i] == NULL)
		{
			cout << "Error: insufficient memory.";
			g_ProgramStop();
		}

	}

	return dynamicArray;
}

template <typename T>
void DeallocateDynamicArray(T** dArray, int nRows, int nCols)
{
	if (!dArray)
		return;

	for (int x = 0; x < nRows; x++)
	{
		delete[] dArray[x];
	}

	delete[] dArray;

}


template <typename T>
T ***Allocate3DDynamicArray(int nX, int nY, int nZ)
{
	T ***dynamicArray;

	dynamicArray = new (std::nothrow) T**[nX];

	if (dynamicArray == NULL)
	{
		cout << "Error: insufficient memory.";
		g_ProgramStop();
	}

	for (int x = 0; x < nX; x++)
	{
		dynamicArray[x] = new (std::nothrow) T*[nY];

		if (dynamicArray[x] == NULL)
		{
			cout << "Error: insufficient memory.";
			g_ProgramStop();
		}

		for (int y = 0; y < nY; y++)
		{
			dynamicArray[x][y] = new (std::nothrow) T[nZ];
			if (dynamicArray[x][y] == NULL)
			{
				cout << "Error: insufficient memory.";
				g_ProgramStop();
			}
		}
	}

	return dynamicArray;

}

template <typename T>
void Deallocate3DDynamicArray(T*** dArray, int nX, int nY)
{
	if (!dArray)
		return;
	for (int x = 0; x < nX; x++)
	{
		for (int y = 0; y < nY; y++)
		{
			delete[] dArray[x][y];
		}

		delete[] dArray[x];
	}

	delete[] dArray;

}



template <typename T>
T ****Allocate4DDynamicArray(int nM, int nX, int nY, int nZ)
{
	T ****dynamicArray;

	dynamicArray = new (std::nothrow) T***[nM];

	if (dynamicArray == NULL)
	{
		cout << "Error: insufficient memory.";
		g_ProgramStop();
	}
	for (int m = 0; m < nM; m++)
	{
		dynamicArray[m] = new (std::nothrow) T**[nX];

		if (dynamicArray[m] == NULL)
		{
			cout << "Error: insufficient memory.";
			g_ProgramStop();
		}

		for (int x = 0; x < nX; x++)
		{
			dynamicArray[m][x] = new (std::nothrow) T*[nY];

			if (dynamicArray[m][x] == NULL)
			{
				cout << "Error: insufficient memory.";
				g_ProgramStop();
			}

			for (int y = 0; y < nY; y++)
			{
				dynamicArray[m][x][y] = new (std::nothrow) T[nZ];
				if (dynamicArray[m][x][y] == NULL)
				{
					cout << "Error: insufficient memory.";
					g_ProgramStop();
				}
			}
		}
	}
	return dynamicArray;

}

template <typename T>
void Deallocate4DDynamicArray(T**** dArray, int nM, int nX, int nY)
{
	if (!dArray)
		return;
	for (int m = 0; m < nM; m++)
	{
		for (int x = 0; x < nX; x++)
		{
			for (int y = 0; y < nY; y++)
			{
				delete[] dArray[m][x][y];
			}

			delete[] dArray[m][x];
		}
		delete[] dArray[m];
	}
	delete[] dArray;

}



class CCSVParser
{
public: ifstream inFile;

		string mFileName;
		vector<int> LineIntegerVector;
		bool IsFirstLineHeader;
		char Delimiter;

private:
	vector<string> LineFieldsValue;

	map<string, int> FieldsIndices;


public:

	void  ConvertLineStringValueToIntegers()
	{
		LineIntegerVector.clear();
		for (unsigned i = 0; i < LineFieldsValue.size(); i++)
		{
			std::string si = LineFieldsValue[i];
			int value = atoi(si.c_str());

			if (value >= 1)
				LineIntegerVector.push_back(value);

		}
	}

	CCSVParser(void)
	{
		Delimiter = ',';
		IsFirstLineHeader = true;
	}

	~CCSVParser(void)
	{
		if (inFile.is_open()) inFile.close();
	}


	bool OpenCSVFile(string fileName, bool b_required=true)

	{
		mFileName = fileName;
		inFile.open(fileName.c_str());

		if (inFile.is_open())
		{
			if (IsFirstLineHeader)
			{
				string s;
				std::getline(inFile, s);
				vector<string> FieldNames = ParseLine(s);

				for (size_t i = 0;i < FieldNames.size();i++)
				{
					string tmp_str = FieldNames.at(i);
					size_t start = tmp_str.find_first_not_of(" ");

					string name;
					if (start == string::npos)
					{
						name = "";
					}
					else
					{
						name = tmp_str.substr(start);
						//			TRACE("%s,", name.c_str());
					}


					FieldsIndices[name] = (int)i;
				}
			}

			return true;
		}
		else
		{
			if (b_required)
			{

				cout << "File " << fileName << " does not exist. Please check." << endl;
				//g_ProgramStop();
			}
			return false;
		}
	}

	void CloseCSVFile(void)
	{
		inFile.close();
	}



	bool ReadRecord()
	{
		LineFieldsValue.clear();

		if (inFile.is_open())
		{
			string s;
			std::getline(inFile, s);
			if (s.length() > 0)
			{

				LineFieldsValue = ParseLine(s);

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


	template <class T> bool GetValueByFieldNameRequired(string field_name, T& value)
	{
		bool required_field = true;
		bool print_out = false;
		if (FieldsIndices.find(field_name) == FieldsIndices.end())
		{
			if (required_field)
			{
				cout << "Field " << field_name << " in file " << mFileName << " does not exist. Please check the file." << endl;

				g_ProgramStop();
			}
			return false;
		}
		else
		{
			if (LineFieldsValue.size() == 0)
			{
				return false;
			}

			if (FieldsIndices[field_name] >= LineFieldsValue.size())  // no value is read for index FieldsIndices[field_name]
			{
				return false;
			}
			string str_value = LineFieldsValue[FieldsIndices[field_name]];

			if (str_value.length() <= 0)
			{
				return false;
			}

			istringstream ss(str_value);

			T converted_value;
			ss >> converted_value;

			if (print_out)
			{
				cout << "Field " << field_name << " = " << converted_value << endl;
			}

			if (/*!ss.eof() || */ ss.fail())
			{
				return false;
			}

			value = converted_value;
			return true;
		}
	}

	vector<string> ParseLine(string line)
	{
		vector<string> SeperatedStrings;
		string subStr;

		if (line.length() == 0)
			return SeperatedStrings;

		istringstream ss(line);


		if (line.find_first_of('"') == string::npos)
		{

			while (std::getline(ss, subStr, Delimiter))
			{
				SeperatedStrings.push_back(subStr);
			}

			if (line.at(line.length() - 1) == ',')
			{
				SeperatedStrings.push_back("");
			}
		}
		else
		{
			while (line.length() > 0)
			{
				size_t n1 = line.find_first_of(',');
				size_t n2 = line.find_first_of('"');

				if (n1 == string::npos && n2 == string::npos) //last field without double quotes
				{
					subStr = line;
					SeperatedStrings.push_back(subStr);
					break;
				}

				if (n1 == string::npos && n2 != string::npos) //last field with double quotes
				{
					size_t n3 = line.find_first_of('"', n2 + 1); // second double quote

					//extract content from double quotes
					subStr = line.substr(n2 + 1, n3 - n2 - 1);
					SeperatedStrings.push_back(subStr);

					break;
				}

				if (n1 != string::npos && (n1 < n2 || n2 == string::npos))
				{
					subStr = line.substr(0, n1);
					SeperatedStrings.push_back(subStr);
					if (n1 < line.length() - 1)
					{
						line = line.substr(n1 + 1);
					}
					else // comma is the last char in the line string, push an empty string to the back of vector
					{
						SeperatedStrings.push_back("");
						break;
					}
				}

				if (n1 != string::npos && n2 != string::npos && n2 < n1)
				{
					size_t n3 = line.find_first_of('"', n2 + 1); // second double quote
					subStr = line.substr(n2 + 1, n3 - n2 - 1);
					SeperatedStrings.push_back(subStr);
					size_t idx = line.find_first_of(',', n3 + 1);

					if (idx != string::npos)
					{
						line = line.substr(idx + 1);
					}
					else
					{
						break;
					}
				}
			}

		}

		return SeperatedStrings;
	}


	template <class T> bool GetValueByFieldNameWithPrintOut(string field_name, T& value)
	{
		bool required_field = true;
		bool print_out = true;
		if (FieldsIndices.find(field_name) == FieldsIndices.end())
		{
			if (required_field)
			{
				cout << "Field " << field_name << " in File " << mFileName << " does not exist." << endl;

				g_ProgramStop();
			}
			return false;
		}
		else
		{
			if (LineFieldsValue.size() == 0)
			{
				return false;
			}

			if (FieldsIndices[field_name] >= LineFieldsValue.size())  // no value is read for index FieldsIndices[field_name]
			{
				cout << "Missing value for " << field_name << " in File " << mFileName << endl;

				return false;
			}
			string str_value = LineFieldsValue[FieldsIndices[field_name]];

			if (str_value.length() <= 0)
			{
				return false;
			}

			istringstream ss(str_value);

			T converted_value;

			ss >> std::noskipws;
			ss >> converted_value;

			if (print_out)
			{
				cout << "Field " << field_name << " = " << converted_value << endl;
			}

			if (/*!ss.eof() || */ ss.fail())
			{
				return false;
			}

			value = converted_value;
			return true;
		}
	}

	template <class T> bool GetValueByFieldName(string field_name, T& value, bool with_whitespace = false)
	{
		if (FieldsIndices.find(field_name) == FieldsIndices.end())
		{
			return false;
		}
		else
		{
			if (LineFieldsValue.size() == 0)
			{
				return false;
			}

			if (FieldsIndices[field_name] >= LineFieldsValue.size())  // no value is read for index FieldsIndices[field_name]
			{
				return false;
			}
			string str_value = LineFieldsValue[FieldsIndices[field_name]];

			if (str_value.length() <= 0)
			{
				return false;
			}

			istringstream ss(str_value);

			if (with_whitespace)
			{
				ss >> std::noskipws;
			}
			T converted_value;
			ss >> converted_value;

			if (/*!ss.eof() || */ ss.fail())
			{
				return false;
			}

			value = converted_value;

			return true;
		}
	}

	bool GetValueByFieldNameWithPrintOut(string field_name, string& value)
	{
		if (FieldsIndices.find(field_name) == FieldsIndices.end())
		{
			return false;
		}
		else
		{
			if (LineFieldsValue.size() == 0)
			{
				return false;
			}

			int index = FieldsIndices[field_name];
			int size = LineFieldsValue.size();

			if (FieldsIndices[field_name] >= LineFieldsValue.size())
			{
				return false;
			}
			if (LineFieldsValue[FieldsIndices[field_name]].length() <= 0)
			{
				return false;
			}
			string str_value = LineFieldsValue[FieldsIndices[field_name]];



			value = str_value;
			return true;
		}
	}

	bool GetValueByFieldName(string field_name, string& value)
	{
		if (FieldsIndices.find(field_name) == FieldsIndices.end())
		{
			return false;
		}
		else
		{
			if (LineFieldsValue.size() == 0)
			{
				return false;
			}

			int index = FieldsIndices[field_name];
			int size = LineFieldsValue.size();

			if (FieldsIndices[field_name] >= LineFieldsValue.size())
			{
				return false;
			}
			if (LineFieldsValue[FieldsIndices[field_name]].length() <= 0)
			{
				return false;
			}
			string str_value = LineFieldsValue[FieldsIndices[field_name]];



			value = str_value;
			return true;
		}
	}

;
};

class CCSVWriter
{

	template <typename T>
	string NumberToString(T Number)
	{
		ostringstream ss;
		ss << Number;
		return ss.str();
	}


public:
	ofstream outFile;
	char Delimiter;
	int FieldIndex;
	bool IsFirstLineHeader;
	map<int, string> LineFieldsValue;
	vector<string> LineFieldsName;
	vector<string> LineFieldsCategoryName;
	map<string, int> FieldsIndices;

	bool row_title;

public:
	void SetRowTitle(bool flag)
	{
		row_title = flag;
	}

	bool OpenCSVFile(string fileName, bool b_required = true);
	void CloseCSVFile(void);
	template <class T> bool SetValueByFieldName(string field_name, T& value)  // by doing so, we do not need to exactly follow the sequence of field names
	{
		if (FieldsIndices.find(field_name) == FieldsIndices.end())
		{
			return false;
		}
		else
		{

			LineFieldsValue[FieldsIndices[field_name]] = NumberToString(value);

			return true;
		}
	}

	void Reset()
	{

		LineFieldsValue.clear();
		LineFieldsName.clear();
		LineFieldsCategoryName.clear();
		FieldsIndices.clear();

	}
	void SetFieldName(string field_name)
	{
		FieldsIndices[field_name] = LineFieldsName.size();
		LineFieldsName.push_back(field_name);
		LineFieldsCategoryName.push_back(" ");

	}

	void SetFieldNameWithCategoryName(string field_name, string category_name)
	{
		FieldsIndices[field_name] = LineFieldsName.size();
		LineFieldsName.push_back(field_name);
		LineFieldsCategoryName.push_back(category_name);

	}


	void WriteTextString(CString textString)
	{
		if (!outFile.is_open())
			return;
		outFile << textString << endl;

	}

	void WriteTextLabel(CString textString)
	{
		if (!outFile.is_open())
			return;
		outFile << textString;

	}

	template <class T>  void WriteNumber(T value)
	{
		if (!outFile.is_open())
			return;
		outFile << NumberToString(value) << endl;
	}

	template <class T>  void WriteParameterValue(CString textString, T value)
	{
		if (!outFile.is_open())
			return;

		outFile << textString << "=," << NumberToString(value) << endl;
	}

	void WriteNewEndofLine()
	{
		if (!outFile.is_open())
			return;
		outFile << endl;
	}


	void WriteHeader(bool bCategoryNameLine = true, bool bRowTitle = true)
	{
		if (!outFile.is_open())
			return;


		if (bCategoryNameLine == true)
		{
			for (unsigned int i = 0; i < FieldsIndices.size(); i++)
			{
				outFile << LineFieldsCategoryName[i] << ",";
			}
			outFile << endl;

		}

		if (bRowTitle == true)
			outFile << ",";

		for (unsigned int i = 0; i < FieldsIndices.size(); i++)
		{
			outFile << LineFieldsName[i] << ",";
		}

		outFile << endl;
	}
	void WriteRecord()
	{
		if (!outFile.is_open())
			return;

		for (unsigned int i = 0; i < FieldsIndices.size(); i++)
		{
			string str;
			if (LineFieldsValue.find(i) != LineFieldsValue.end()) // has been initialized
				outFile << LineFieldsValue[i].c_str() << ",";
			else
				outFile << ' ' << ",";
		}

		LineFieldsValue.clear();

		outFile << endl;
	}

	CCSVWriter()
	{
		row_title = false;
		FieldIndex = 0;
		Delimiter = ',';
		IsFirstLineHeader = true;
	}

	~CCSVWriter(void)
	{
		if (outFile.is_open()) outFile.close();
	}


	CCSVWriter(string fileName)
	{
		Open(fileName);

	};

	void Open(string fileName)
	{
		outFile.open(fileName.c_str());

		if (outFile.is_open() == false)
		{
			cout << "File " << fileName.c_str() << " cannot be opened." << endl;
			getchar();
			exit(0);
		}

	};

	void OpenAppend(string fileName)
	{
		outFile.open(fileName.c_str(), fstream::app);

		if (outFile.is_open() == false)
		{
			cout << "File " << fileName.c_str() << " cannot be opened." << endl;
			getchar();
			exit(0);
		}

	};
};





class DemandType
{
public:
	int agent_type;
	double agent_type_percentage;
	double cumulative_agent_type_percentage;

	double info_class_percentage[MAX_INFO_CLASS_SIZE];
	double cumulative_info_class_percentage[MAX_INFO_CLASS_SIZE];
	double Avg_VOT;
	string agent_type_name;

	DemandType()
	{
		agent_type = 1;
		Avg_VOT = 12;
		cumulative_agent_type_percentage = 0;
		agent_type_percentage = 0;
		for (int info_class = 0; info_class < MAX_INFO_CLASS_SIZE; info_class++)
		{
			info_class_percentage[info_class] = 0;
			cumulative_info_class_percentage[info_class] = 0;
		}

	}

};

extern std::vector <DemandType> g_DemandTypeVector;


//////////////////////
class DTAZone
{ 
public:
	int m_ZoneID;
	int m_ZoneNo;

	std::vector<int> m_OriginNodeNoVector;
	std::vector<int> m_DestinationNodeNoVector;


	int GetRandomOriginNodeIDInZone(float random_ratio)
	{
		int ArraySize  = m_OriginNodeNoVector.size();

		if(ArraySize ==0)
			return -1;
		if (ArraySize == 1)
			return  m_OriginNodeNoVector[0];

		int node_index = int(random_ratio*ArraySize);

		if (node_index <  0)
			node_index = 0;

		if(node_index >= ArraySize)
			node_index = ArraySize -1;

		return m_OriginNodeNoVector[node_index];
	}

	int GetRandomDestinationIDInZone(float random_ratio)
	{
		int ArraySize  = m_DestinationNodeNoVector.size();
		if(ArraySize ==0)
			return -1;
		if (ArraySize == 1)
			return m_DestinationNodeNoVector[0];

		int node_index = int(random_ratio*ArraySize);

		if (node_index <  0)
			node_index = 0;

		if(node_index >= ArraySize)
			node_index = ArraySize -1;

			return m_DestinationNodeNoVector[node_index];
	}

	DTAZone()
	{
		m_ZoneID = -1;

		m_ZoneNo = 0;
	}
	~DTAZone()
	{
		m_OriginNodeNoVector.clear();
		m_DestinationNodeNoVector.clear();
	}
};


class DTAMovement
{
public:
	DTAMovement()
	{

		starting_time_in_min = 0;
		ending_time_in_min = 1440;
		turnning_percentage = 0;
		b_turning_prohibited = false;
		signal_control_no = 0;
		signal_group_no = 0;
		phase_index = 0;
		turn_volume = 10;
		total_Agent_count = 0;
		total_Agent_delay = 0;
		movement_capacity_per_simulation_interval = 0;
		movement_Agent_counter = 0 ;
		movement_hourly_capacity = 100;
		movement_effective_green_time_in_min = 0;
	}

	float GetAvgDelay_In_Min()
	{
		if(b_turning_prohibited == true)  // movement is prohibited. 
			return 99;  // this is still lower than 99999 as maximal cost

		float avg_delay = total_Agent_delay/ max(1, total_Agent_count );

		if(movement_hourly_capacity<=0.1)
			avg_delay = 99;

		return avg_delay;
	}


		
	int in_link_from_node_id;
	int in_link_to_node_id;  // this equals to the current node number
	int out_link_to_node_id;

	string turning_direction;

	int total_Agent_count;
	float total_Agent_delay;

	float movement_effective_green_time_in_min;

	float movement_hourly_capacity;
	float movement_capacity_per_simulation_interval;
	int movement_Agent_counter;
	int starting_time_in_min;
	int ending_time_in_min;
	float turnning_percentage;
	bool b_turning_prohibited;
	int phase_index;
	int signal_control_no;  // for meso-scopic, link -based
	int signal_group_no;  // for meso-scopic, link -based
	int turn_volume;
};
// event structure in this "event-based" traffic simulation
typedef struct{
	int veh_id;
	int veh_next_node_number; // next link's downstream node number
	float event_time_stamp;

}struc_Agent_item;

// accessibility

class DTADestination
{
public:
	int from_TAZ;
	int to_TAZ;
	int agent_id;
	int destination_number;
	int d_node_no;
	float destination_node_distance;
	std::vector<float> destination_node_travel_time_vector;
	std::vector<float> destination_node_travel_distance_vector;

	DTADestination()
	{
		from_TAZ = 0;
		to_TAZ = 0;
		agent_id = 0;
		destination_number = 0;
		d_node_no = 0;
		destination_node_distance = 0;

	}
};


class DTANode
{
public:
	DTANode()
	{
		m_CycleLength_In_Second = 60;
		m_ControlType = 0;
		m_ZoneID = 0;
		m_ABMZoneID = 0; // for ABM output
		m_TotalCapacity = 0;

		m_bOriginFlag = false;
		m_bDestinationFlag = false;
		m_TotalDelay = 0;

	};
	~DTANode()
	{
	};


	int m_NodeNo;
	int m_NodeID;
	int m_ZoneID;  // If ZoneID > 0 --> centriod,  otherwise a physical node.
	int m_ABMZoneID;
	int m_ControlType; // Type:
	int m_CycleLength_In_Second;
	std::map<std::string, std::string> m_Movement2LinkIDStringMap;
	float m_TotalCapacity;

	std::vector<int> m_IncomingLinkVector;
	std::vector<int> m_OutgoingLinkVector;

	std::vector<double> m_IncomingLinkDelay;

	void AddIncomingLinkDelay(int LinkID, double Delay)
	{
		for(int i = 0; i< m_IncomingLinkVector.size(); i++)
		{
		if(m_IncomingLinkVector[i]== LinkID)
			m_IncomingLinkDelay[i]+= Delay;
		
		}
	
	}


	std::map<string, DTAMovement> m_MovementMap;


	bool m_bOriginFlag;
	bool m_bDestinationFlag;
	std::vector<DTADestination> m_DestinationVector;  // for accessility

	double m_TotalDelay;

};

class SLinkMOE  // time-dependent link MOE
{
public:


	float UserDefinedTravelTime_in_min;
	float TotalTravelTime;   // cumulative travel time for Agents departing at this time interval
	int TotalFlowCount;

	
	int CumulativeArrivalCount; 
	int CumulativeDepartureCount;
	int ExitQueueLength;


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
		CumulativeArrivalCount  = 0;
		CumulativeDepartureCount = 0;
		ExitQueueLength = 0;

		EndTimeOfPartialCongestion = 0;
		TrafficStateCode = 0;  // free-flow


	};


	void SetupMOE(float FreeflowTravelTime)
	{
		
		TotalTravelTime = 0;
		TotalFlowCount = 0;
		CumulativeArrivalCount  = 0;
		CumulativeDepartureCount = 0;
		ExitQueueLength = 0;
		UserDefinedTravelTime_in_min = FreeflowTravelTime;
		EndTimeOfPartialCongestion = 0;
		TrafficStateCode = 0;  // free-flow
	

	}

} ;

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


class CapacityReduction
{
public:
	int StartDayNo;
	int EndDayNo;
	float StartTime;  
	float EndTime;    
	float LaneClosureRatio;
	float SpeedLimit;
};




class Toll
{
public:
	int StartDayNo;
	int EndDayNo;
	float StartTime;
	float EndTime;
	float TollRate[MAX_agent_type_SIZE];  // 4 is 3_+1 , as pricing 

	Toll()
	{
		for(int vt = 1; vt<MAX_agent_type_SIZE; vt++)
			TollRate[vt]=0;
	}
};


class DTALink
{
public:
	DTALink(int TimeSize, double Length, double JamDensity)  // TimeSize's unit: per min
	{
		m_ProhibitedU_Turn = 0;


		m_CumulativeOutCapacityCount = 0.0f;
		m_CumulativeOutCapacityCountAtPreviousInterval =0;
		m_CumulativeInCapacityCountAtPreviousInterval =0 ;
		number_of_timing_plans = 0;

		m_CumulativeMergeOutCapacityCount = 0.0f;
		m_CumulativeInCapacityCount = 0.0f;
		m_Direction;

		m_bOnRampType  = false;
		m_bOffRampType = false;

		m_bFreewayType = false;
		m_bArterialType = false;
		m_bSignalizedArterialType = false;

		CurrentSequenceNoForVechileDistanceAry = 0;
		CycleSizeForVechileDistanceAry = 0;

		m_ObservedFlowVolume = 0;
		m_FlowMeasurementError = 0;
		m_AADT = 0;
		m_bSensorData = false;

		m_BPR_Distance =0;
		m_SimulationHorizon = TimeSize;


			for (int p = 0; p < 20; p++)
			{
				m_GreenEndTime_In_Second[p] = 0;
				m_GreenStartTime_In_Second[p] = 0;
				m_SaturationFlowRate_In_vhc_per_hour_per_lane[p] = 1800;
			}
			m_LinkMOEAry.resize(m_SimulationHorizon + 1);

			m_CumuArrivalFlow.resize(MAX_TIME_INTERVAL_ADCURVE + 1);         // for Cumulative flow counts: unit is per simulation time interval. e.g. 6 seconds 
			m_CumuDeparturelFlow.resize(MAX_TIME_INTERVAL_ADCURVE + 1);      // TimeSize  (unit: min), TimeSize*10 = 0.1 min: number of simulation time intervals



			m_BPRLinkVolume= 0;
			m_BPRLinkTravelTime = 0;
		
		m_bMergeFlag = 0;
		m_MergeOnrampLinkID = -1;
		m_MergeMainlineLinkID = -1;
		m_TollSize = 0;
		pTollVector = NULL;

		m_LoadingBufferWaitingTime = 0;

		m_BPR_Alpha = 0.15f;
		m_BPR_Beta = 4.0f;

		m_traffic_flow_model = tfm_point_queue;
	};
	int m_FromNodeID;
	int m_ToNodeID;
	int m_LinkNo;
	string m_road_link_id;
	std::map<int, int> m_Timing_Status_Map;

	float GetTimeDependentCapacityAtSignalizedIntersectionFromTimingStatus(double CurrentTimeInMin);

	

	void ResizeData(int TimeSize)  // TimeSize's unit: per min
	{
		m_SimulationHorizon	= TimeSize;
		m_LinkMOEAry.resize(m_SimulationHorizon+1);
		//		m_LinkMeasurementAry.clear();  do not remove measurement data for ODME 

	}

	bool 	GetImpactedFlag(int DayNo=0, int DepartureTime = -1)
	{
		for(unsigned int il = 0; il< CapacityReductionVector.size(); il++)
		{
			if((CapacityReductionVector[il].StartDayNo  <=DayNo && DayNo <= CapacityReductionVector[il].EndDayNo ) && (DepartureTime >= CapacityReductionVector[il].StartTime && DepartureTime<=CapacityReductionVector[il].EndTime + 60))  // 60 impacted after capacity reduction
			{
				return true;
			}
		}



		return false;
	}


	float GetHourlyPerLaneCapacity(int Time=-1)
	{
		return m_LaneCapacity;
	}

	float GetInflowNumberOfLanes(int DayNo = 0, int Time = 0, bool OutputFlowFlag = false)  // with lane closure
	{

		int NumLanes = m_InflowNumLanes;

		for (unsigned int il = 0; il< CapacityReductionVector.size(); il++)
		{
			if ((CapacityReductionVector[il].StartDayNo <= DayNo && DayNo <= CapacityReductionVector[il].EndDayNo) && (Time >= CapacityReductionVector[il].StartTime && Time <= CapacityReductionVector[il].EndTime))
			{
				return (1 - CapacityReductionVector[il].LaneClosureRatio)*NumLanes;
			}
		}


	return (float)NumLanes;

	}
	float GetNumberOfLanes(int DayNo=0, double Time=0, bool OutputFlowFlag = false)  // with lane closure
	{

		int NumLanes = m_OutflowNumLanes;

		for(unsigned int il = 0; il< CapacityReductionVector.size(); il++)
		{
			if( (CapacityReductionVector[il].StartDayNo  <=DayNo && DayNo <= CapacityReductionVector[il].EndDayNo ) && (Time>= CapacityReductionVector[il].StartTime && Time<=CapacityReductionVector[il].EndTime))
			{
				return (1-CapacityReductionVector[il].LaneClosureRatio)*NumLanes;
			}
		}


	
		return (float)NumLanes;

	}

	float GetNumberOfLanes_ImpactedByWorkZoneConditions(int DayNo=0, double Time=-1, bool OutputFlowFlag = false)  // with lane closure
	{

		int NumLanes = m_OutflowNumLanes;

		for(unsigned int il = 0; il< CapacityReductionVector.size(); il++)
		{
			if( (CapacityReductionVector[il].StartDayNo  <=DayNo && DayNo <= CapacityReductionVector[il].EndDayNo ) && (Time>= CapacityReductionVector[il].StartTime && Time<=CapacityReductionVector[il].EndTime))
			{
				return (1-CapacityReductionVector[il].LaneClosureRatio)*NumLanes;
			}
		}

		return (float)NumLanes;

	}

	float GetTollRateInDollar(int DayNo=0, float Time=0, int DemandType=1)  
	{
		for(int il = 0; il< m_TollSize; il++)
		{
			if(( pTollVector[il].StartDayNo  <= DayNo &&  DayNo <= pTollVector[il].EndDayNo) && (Time >= pTollVector[il].StartTime && Time<=pTollVector[il].EndTime))
			{
				return pTollVector[il].TollRate[DemandType];
			}
		}
		return 0;
	}

	unsigned int m_RandomSeed; 
	float GetRandomRatio()  // get link_specific random seed
	{
		m_RandomSeed = (LCG_a * m_RandomSeed + LCG_c) % LCG_M;  //m_RandomSeed is automatically updated.

		return float(m_RandomSeed)/LCG_M;
	}

	float m_CumulativeOutCapacityCount; 
	int m_CumulativeOutCapacityCountAtPreviousInterval; 
	int m_CumulativeInCapacityCountAtPreviousInterval; 

	float m_CumulativeLeftOutCapacityCount; 
	float m_CumulativeMergeOutCapacityCount; 

	int m_ProhibitedU_Turn;

	float m_CumulativeInCapacityCount;



	float	m_Length;  // in miles
	float   m_AgentSpaceCapacity; // in Agents
	int	m_OutflowNumLanes;
	float m_OutflowPriority;

	int m_Orginal_OutflowNumLanes;
	int m_Orginal_InflowNumLane;

	int m_InflowNumLanes;
	float	m_SpeedLimit;
	float	m_KCritical;

	float m_KJam;
	float m_BackwardWaveSpeed; // unit: mile/ hour
	float	m_LaneCapacity;  //Capacity used in BPR for each link, reduced due to link type and other factors.
	float m_LoadingBufferWaitingTime;

	char m_Direction;

	std::string m_geometry_string;
	bool m_bOnRampType;
	bool m_bOffRampType;
	string m_LinkTypeName;

	std::vector <SLinkMOE> m_LinkMOEAry;

	int GetSimulateAvgDensityPerLane(int start_timestamp, int end_timestamp)
	{

		if(start_timestamp < end_timestamp && start_timestamp >= 0 && start_timestamp < m_LinkMOEAry.size() &&  end_timestamp < m_LinkMOEAry.size()  )
			{
			int timestamp = (start_timestamp + end_timestamp) / 2;
			int TotalSimulatedNumberOfAgents = m_LinkMOEAry[timestamp].CumulativeArrivalCount - m_LinkMOEAry[timestamp].CumulativeDepartureCount;
			// we can do a more complicated summation here but we perfer to use a simpler calculation for a single time stamp
			return TotalSimulatedNumberOfAgents / max(1, this->m_OutflowNumLanes) / max(0.01, m_Length);

			}
		return 0;

	}

	std::vector <int> m_CumuArrivalFlow;
	std::vector <int> m_CumuDeparturelFlow;

	std::vector<CapacityReduction> CapacityReductionVector;
	std::vector<Toll> TollVector;

	int  m_DynamicTollType;  // 0: static toll as default: 1: dynamic toll: 


	int m_TollSize;
	Toll *pTollVector;  // not using SLT here to avoid issues with OpenMP


	int m_bMergeFlag;  // 1: freeway and freeway merge, 2: freeway and ramp merge
	std::vector<MergeIncomingLink> MergeIncomingLinkVector;
	int m_MergeOnrampLinkID;
	int m_MergeMainlineLinkID;

	std::list<struc_Agent_item> LoadingBuffer;  //loading buffer of each link, to prevent grid lock

	std::list<struc_Agent_item> EntranceQueue;  //link-in queue  of each link
	std::list<struc_Agent_item> ExitQueue;      // link-out queue of each link

//

	int CurrentSequenceNoForVechileDistanceAry;  // start from 0, 
	int CycleSizeForVechileDistanceAry; // cycle size


	string m_LinkID;
	int m_FromNodeNo;  // index starting from 0
	int m_ToNodeNo;    // index starting from 0

	int  m_StochaticCapcityFlag;  // 0: deterministic cacpty, 1: lane drop. 2: merge, 3: weaving
	// optional for display only
	int	m_link_type;

	string agent_type;

	bool m_bFreewayType;  //created to store the freeway type, used in simulation
	bool m_bArterialType;
	bool m_bSignalizedArterialType;
	
	e_traffic_flow_model m_traffic_flow_model;

	int m_to_node_offset_in_sec[20];

	int number_of_timing_plans;
	float m_SaturationFlowRate_In_vhc_per_hour_per_lane[20];
	int m_GreenStartTime_In_Second[20];
	int m_GreenEndTime_In_Second[20];

	// for MOE data array
	int m_SimulationHorizon;

	// traffic flow propagation
	float m_FreeFlowTravelTime; // min
	float m_BPR_Alpha;
	float m_BPR_Beta;
	float m_BPR_Distance;

	int m_BackwardWaveTimeInSimulationInterval; // simulation time interval

	float m_BPRLinkVolume;
	float m_BPRLinkTravelTime;



	~DTALink()
	{

		if(LoadingBufferVector!=NULL && LoadingBufferSize>=1)
			delete LoadingBufferVector;

		if(pTollVector)
			delete pTollVector;

		LoadingBuffer.clear();
		EntranceQueue.clear();
		ExitQueue.clear();
		MergeIncomingLinkVector.clear();

		m_LinkMOEAry.clear();

		m_CumuArrivalFlow.clear();
		m_CumuDeparturelFlow.clear();

		CapacityReductionVector.clear();
		TollVector.clear();


	};

	float GetFreeMovingTravelTime(int TrafficModelFlag = 2, int DayNo=0, float Time = -1, int analysis_period_no = 0)
	{
		 if(TrafficModelFlag == 0) // BPR model
			return m_BPRLinkTravelTime;  // we use time, not the cost here
		else 
		{
			for(unsigned int il = 0; il< CapacityReductionVector.size(); il++)
			{
				if((CapacityReductionVector[il].StartDayNo  <=DayNo && DayNo <= CapacityReductionVector[il].EndDayNo ) &&(Time>=CapacityReductionVector[il].StartTime && Time<=CapacityReductionVector[il].EndTime))
				{
					return m_Length/max(1,CapacityReductionVector[il].SpeedLimit)*60.0f;  // convert from hour to min;
				}
			}
	

				return m_FreeFlowTravelTime;

		}
	}



	void SetupMOE()
	{


		departure_count = 0;
		total_departure_based_travel_time = 0;
		m_CumulativeOutCapacityCount = 0;
		m_CumulativeOutCapacityCountAtPreviousInterval = 0;
		m_CumulativeInCapacityCountAtPreviousInterval =0 ;
		m_CumulativeMergeOutCapacityCount = 0;
		m_CumulativeInCapacityCount = 0;
		m_JamTimeStamp = (float) m_SimulationHorizon;
		m_FreeFlowTravelTime = m_Length/m_SpeedLimit*60.0f;  // convert from hour to min
		total_departure_based_travel_time = m_FreeFlowTravelTime;

			m_BPRLinkVolume= 0;
			m_BPRLinkTravelTime= m_FreeFlowTravelTime;

		m_FFTT_simulation_interval = int(m_FreeFlowTravelTime/g_DTASimulationInterval_InMin);
		LoadingBufferVector = NULL;


		LoadingBufferSize = 0;

		m_BackwardWaveTimeInSimulationInterval = int(m_Length/m_BackwardWaveSpeed*60/g_DTASimulationInterval_InMin); // assume backwave speed is 20 mph, 600 conversts hour to simulation intervals

		CFlowArrivalCount = 0;
		CFlowImpactedCount = 0;

	
		for(int t=0; t<MAX_TIME_INTERVAL_ADCURVE; t++)
		{
			A[t] = 0;
			D[t] = 0;
		}

		StartIndexOfLoadingBuffer = 0;
		LoadingBufferVector = NULL;

		CFlowDepartureCount = 0;

		LinkOutCapacity = 0;
		LinkInCapacity = 0;

		AgentCount = 0;

		int t;


		for(t=0; t< m_CumuArrivalFlow.size(); t++)
		{
			m_CumuArrivalFlow[t] = 0;
			m_CumuDeparturelFlow[t] = 0;

		}

		for(t=0; t<=m_SimulationHorizon; t++)
		{
			m_LinkMOEAry[t].SetupMOE(m_FreeFlowTravelTime);
		}

		LoadingBuffer.clear();
		EntranceQueue.clear();
		ExitQueue.clear();

	}

	void ResetUserDefinedTravelTime()
	{
			for(int t=0; t<m_LinkMOEAry.size(); t++)
		{
			m_LinkMOEAry[t] .UserDefinedTravelTime_in_min = -1;
		}
	}
	void UseUserDefinedAttribute(int start_time_in_min, int time_step_in_min, float travel_time = -1, float toll_value_in_dollar = -1, float lane_capacity = -1) 
	{
		for(int t = max(0,start_time_in_min); t<= min(start_time_in_min + time_step_in_min, m_SimulationHorizon); t++)
		{
			if(travel_time >=0.1)  // minimum simulation time  = 0.1 min
			{
			m_LinkMOEAry[t].UserDefinedTravelTime_in_min  = travel_time;
			}
		}
	
	}

	float m_JamTimeStamp;

	int CFlowArrivalCount;
	int CFlowDepartureCount;

	int CFlowImpactedCount;

	int  A[MAX_TIME_INTERVAL_ADCURVE];
	int  D[MAX_TIME_INTERVAL_ADCURVE];
	int  m_FFTT_simulation_interval;  // integral value of  FFTT, in terms of simulation  time interval

	int* LoadingBufferVector;
	int  LoadingBufferSize;


	int  StartIndexOfLoadingBuffer;

	std::string m_Name;

	float m_ObservedFlowVolume;
	float m_FlowMeasurementError ;
	float m_AADT;
	bool m_bSensorData;

	int LinkOutCapacity;  // unit: number of vehiles
	int LinkInCapacity;   // unit: number of vehiles
	int AgentCount;

	int departure_count;
	float total_departure_based_travel_time;

	float GetSpeed(int time, int aggregation_time_interval=1)
	{
		return m_Length / max(0.1, GetTravelTimeByMin(time, aggregation_time_interval, m_traffic_flow_model))*60.0f;  // 60.0f converts min to hour, unit of speed: mph
	}

	int GetArrivalFlow(int time, int aggregation_time_interval=1)
	{

		time = max(0, time);
		if (time + aggregation_time_interval< m_SimulationHorizon - 1)  // if time = m_SimulationHorizon-1, time+1 = m_SimulationHorizon  --> no data available
			return max(0,m_LinkMOEAry[time + aggregation_time_interval].CumulativeArrivalCount - m_LinkMOEAry[time].CumulativeArrivalCount);
		else
			return 0;

	};

	int GetDepartureFlow(int time, int aggregation_time_interval=1)
	{
		time = max(0, time);
		if (time + aggregation_time_interval < m_SimulationHorizon - 1)  // if time = m_SimulationHorizon-1, time+1 = m_SimulationHorizon  --> no data available
			return max(0,m_LinkMOEAry[time + aggregation_time_interval].CumulativeDepartureCount - m_LinkMOEAry[time].CumulativeDepartureCount);
		else
			return 0;

	};




	float GetTrafficVolumeByMin(int DayNo,int starting_time, int time_interval)  // DayNo =-1: unknown day
	{

		int t;
		int total_flow =0;
		int time_end = min(starting_time+time_interval, m_SimulationHorizon);
		for(t=starting_time; t< time_end; t++)
		{
			total_flow +=  m_LinkMOEAry[t].TotalFlowCount ;
		}

		return total_flow;

	};


	float GetTravelTimeByMin(int starting_time, int time_interval, e_traffic_flow_model TrafficModelFlag)  // DayNo =-1: unknown day
	{
		float travel_time  = 0.0f;
		starting_time = max(0, starting_time);

		//// condition 1:road blockage caused by work zones
		//if(GetNumberOfLanes_ImpactedByWorkZoneConditions(DayNo,starting_time)<=0.001)   // 
		//	return 9999; // unit min

		ASSERT(m_SimulationHorizon < m_LinkMOEAry.size());

		// condition 2:  BRF travel time

		if (TrafficModelFlag == tfm_BPR) // BPR model
		{
			return m_BPRLinkTravelTime;  // used in shortest path
		}



		// condition 3: traffic flow model to determine time-dependent travel time for point queue, spatial queue or Newell's model
		int t;
		float total_travel_time = 0;
		int total_flow =0;
		int time_end = min(starting_time+time_interval, m_SimulationHorizon);

		float time_dependent_free_flow_traveltime = GetFreeMovingTravelTime(2, 0, starting_time);
		for(t=starting_time; t< time_end; t++)
		{
			total_travel_time += m_LinkMOEAry[t].TotalTravelTime;
			total_flow +=  m_LinkMOEAry[t].TotalFlowCount ;
		}

		if(total_flow >= 1)
		{
			travel_time =  total_travel_time/total_flow;
			if(travel_time < time_dependent_free_flow_traveltime)
				travel_time = time_dependent_free_flow_traveltime; // minimum travel time constraint for shortest path calculation
		}
		else
			travel_time =  time_dependent_free_flow_traveltime;

		if(travel_time > 99999)
		{
			TRACE("Error, link %d", this ->m_LinkNo );
			for(t=starting_time; t< time_end; t++)
			{
				TRACE("t = %d,%f\n",t,m_LinkMOEAry[t].TotalTravelTime);
			}

		}

		return travel_time;

	};


};

// link element of a Agent path

class SAgentLink
{  public:

unsigned long  LinkNo;  // range: 4294967295

float AbsArrivalTimeOnDSN;     // absolute arrvial time at downstream node of a link: 0 for the departure time, including delay/stop time
//   float LinkWaitingTime;   // unit: 0.1 seconds
SAgentLink()
{
	LinkNo = MAX_LINK_NO;
	AbsArrivalTimeOnDSN = 99999;
	//		LinkWaitingTime = 0;
}

};




class DTALinkType
{
public:
	float default_lane_capacity;
	int link_type;
	string link_type_name;
	string type_code;

	float link_type_bias_factor;

	int number_of_links;

	float total_lane_capacity;
	float total_speed_limit;
	float total_number_of_lanes;
	float total_k_jam;
	float total_length;

	DTALinkType()
	{
		number_of_links = 0;
		link_type_bias_factor = 1.0;
		default_lane_capacity = 1000;

		total_speed_limit = 0;
		total_number_of_lanes =0 ;
		total_k_jam = 0;
		total_length = 0;

	}


	bool IsFreeway()
	{
		if(type_code.find('f')!= string::npos)
			return true;
		else
			return false;
	}

	bool IsHighway()
	{
		if(type_code.find('h')!= string::npos)
			return true;
		else
			return false;
	}

	bool IsArterial()
	{
		if(type_code.find('a')!= string::npos)
			return true;
		else
			return false;
	}

	bool IsRamp()
	{
		if(type_code.find('r')!= string::npos)
			return true;
		else
			return false;
	}

	bool IsConnector()
	{
		if(type_code.find('c')!= string::npos)
			return true;
		else
			return false;
	}

	/*bool IsTransit()
	{
		if(type_code.find('t')!= string::npos)
			return true;
		else
			return false;
	}
	bool IsWalking()
	{
		if(type_code.find('w')!= string::npos)
			return true;
		else
			return false;
	}*/
};

class DTAVehListPerTimeInterval
{
public: 
	std::vector<int> m_AgentIDVector;

};


class DTAAgent
{
public:
	int m_NodeSize;
	int m_NodeIDSum;  // used for comparing two paths
	SAgentLink *m_LinkAry; // link list arrary of a Agent path  // to do list, change this to a STL vector for better readability


	unsigned int m_RandomSeed;
	int m_AgentID;
	int m_o_ZoneNo;  
	int m_d_ZoneNo; 

	int m_o_NodeNo;
	int m_d_NodeNo;

	int m_DemandType;     // 1: passenger,  2, HOV, 3, truck, 3: bus
	
	float m_PCE;     


	int m_SimLinkSequenceNo; //  range 0, 65535

	bool  m_bImpacted;

	float m_DepartureTime;
	float m_LeavingTimeFromLoadingBuffer;

	float m_Distance;

	float m_ArrivalTime;
	float m_TripTime;
	float m_TripFFTT;
	float m_BufferWaitingTime;
	float m_TravelTime;
	float m_Delay;

	bool m_bSwitch;  // switch route in assignment
	float m_gap;
	bool m_gap_update;
	bool m_bConsiderToSwitch;  //consider to switch route in assignment

	// used for simulation
	bool m_bLoaded; // be loaded into the physical network or not
	bool m_bComplete;


	float m_VOT;        // range 0 to 255
	float m_TollDollarCost;

	string m_routeID;
	float GetLinkTravelTime(int link_sequence_no)
	{
		
		if (link_sequence_no >= this->m_NodeSize - 1)
			return 0;

		float TravelTime = 0;
		if (link_sequence_no >= 1)
		{
			TravelTime =m_LinkAry[link_sequence_no].AbsArrivalTimeOnDSN -m_LinkAry[link_sequence_no - 1].AbsArrivalTimeOnDSN;
		}
		else if (link_sequence_no == 0)
		{
			TravelTime = m_LinkAry[link_sequence_no].AbsArrivalTimeOnDSN - m_DepartureTime;
		
		}
	return TravelTime;
	}




	DTAAgent()
	{

		m_DepartureTime = -1;
		m_NodeIDSum = 0;
		m_o_NodeNo = -1;
		m_d_NodeNo = -1;

		m_LeavingTimeFromLoadingBuffer = -1;

		m_AgentID = 0;
		m_gap = 0;
		m_gap_update = false;

		m_SimLinkSequenceNo = 0;

		m_PCE = 1;

		m_VOT = DEFAULT_VOT;
		m_TollDollarCost = 0;

		m_LinkAry = NULL;
		m_NodeSize	= 0;
		m_bImpacted = false; 
		m_DemandType = 1;
		m_DemandType = 0;
		m_ArrivalTime = 0;
		//      m_FinalArrivalTime = 0;
		m_bLoaded = false;
		m_bSwitch = false;
		m_bConsiderToSwitch = false;
		m_bComplete = false;
		m_TripTime = 900;  // default: for incomplete Agents, they have an extremey long trip time
		m_TravelTime = 900;
		m_Distance =0;
		m_Delay = 0;

		m_o_ZoneNo = -1;
		m_d_ZoneNo = -1;

		m_o_NodeNo = -1;
		m_d_NodeNo = -1;

	};
	~DTAAgent()
	{
		if(m_LinkAry != NULL && m_NodeSize > 0)
			delete m_LinkAry;

	};

	float GetRandomRatio()
	{
		m_RandomSeed = (LCG_a * m_RandomSeed + LCG_c) % LCG_M;  //m_RandomSeed is automatically updated.

		return float(m_RandomSeed)/LCG_M;
	}

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


template <typename M> void FreeClearMap( M & amap ) 
{
	for ( typename M::iterator it = amap.begin(); it != amap.end(); ++it ) {
		delete it->second;
	}
	amap.clear();
}

class DTALinkToll
{
public:

	bool m_bMonetaryTollExist ;
	bool m_bTravelTimeTollExist; // unit: generalized travel time in min 

	float TollValue[MAX_agent_type_SIZE];

	DTALinkToll()
	{
		m_bMonetaryTollExist = false;
		m_bTravelTimeTollExist = false;
		for(int i=0; i< MAX_agent_type_SIZE; i++)
			TollValue[i] = 0;

	}
};

struct PathLinkStruct
{
	std::vector<int> LinkNoVector;

};


extern DTALinkToll** g_LinkTDCostAry;

class DTANetworkForSP  // mainly for shortest path calculation, not just physical network
	// for shortest path calculation between zone centroids, for origin zone, there are only outgoing connectors, for destination zone, only incoming connectors
	// different shortest path calculations have different network structures, depending on their origions/destinations
{
public:
	

	int m_NumberOfSPCalculationIntervals;
	int m_NumberOfTDSPCalculationIntervals;

	int m_PlanningHorizonInMin;
	int m_StartTimeInMin;

	int m_NodeSize;
	int m_PhysicalNodeSize;
	int m_LinkSize;

// the following is for node-based scan eligible list
	
// the following is for link-based scan eligible list
	int m_LinkBasedSEListFront;
	int m_LinkBasedSEListTail;
	int* m_LinkBasedSEList;  // dimension: number of links


	int** m_OutboundNodeAry; //Outbound node array
	int** m_OutboundLinkAry; //Outbound link array
	
	int** m_OutboundConnectorOriginZoneIDAry; //Outbound connector array
	int** m_OutboundConnectorDestinationZoneIDAry; //Outbound connector array (destination zone)
	
	int* m_LinkConnectorFlag; //Outbound connector array
	int* m_LinkConnectorOriginZoneIDAry; 
	int* m_LinkConnectorDestinationZoneIDAry; 

	int* m_OutboundSizeAry;  //Number of outbound links

	int** m_OutboundMovementAry; //Outbound link movement array: for each link
	int* m_OutboundMovementSizeAry;  //Number of outbound movement for each link
	float** m_OutboundMovementDelayAry; //Outbound link movement array: for each link

	int** m_InboundLinkAry; //inbound link array
	int* m_InboundSizeAry;  //Number of inbound links

	int* m_FromNoAry;
	int* m_ToNoAry;

	float** m_LinkTDTimeAry;

	float** m_LinkTDTransitTimeAry;

	float*  m_LinkTDDistanceAry;
	float*  m_LinkFFTTAry;


	int** NodePredVectorPerType;  
	int** LinkPredVectorPerType;
	float** LabelCostVectorPerType;

	int* NodeStatusAry;                // Node status array used in KSP;
	float* LabelTimeAry;               // label - time
	int* NodePredAry;

	float* LabelCostAry;
	float* LabelDollarCostAry;

	float* LabelDistanceAry;

	int* LinkNoAry;  //record link no according to NodePredAry

	// movement calculation
	int* LinkStatusAry;                // Node status array used in KSP;
	float* LinkLabelTimeAry;               // label - time
	int* LinkPredAry;  
	float* LinkLabelCostAry;

	   	int m_Number_of_CompletedAgents;
	int m_AdjLinkSize;


	int** TD_LinkTimeIntervalAry;
	float** TD_LinkVolumeAry;  // to keep track of link volume for marginal cost calculation

	float** TD_InflowLinkLabelCostAry;
	int** TD_InflowLinkTimePredAry;  // pointer to previous TIME INDEX from the current label at current node and time

	float** TD_InflowEntranceQueueLabelCostAry;
	int** TD_InflowEntranceQueueTimePredAry;  // pointer to previous TIME INDEX from the current label at current node and time

	float** TD_OutflowLinkLabelCostAry;
	int** TD_OutflowLinkTimePredAry;  // pointer to previous TIME INDEX from the current label at current node and time

	float** TD_OutflowExitQueueLabelCostAry;
	int** TD_OutlowExitQueueTimePredAry;  // pointer to previous TIME INDEX from the current label at current node and time


	//	std::list<int> m_ScanLinkList;  // used for movement-based scanning process, use a std implementation for simplicity

	int PathLinkList[MAX_NODE_SIZE_IN_A_PATH];
	int CurrentPathLinkList[MAX_NODE_SIZE_IN_A_PATH];
	int PathNodeList[MAX_NODE_SIZE_IN_A_PATH];

	int temp_reversed_PathLinkList[MAX_NODE_SIZE_IN_A_PATH];  // tempory reversed path node list

	DTANetworkForSP()
	{
		m_NodeSize = 0;
	};

	void Setup(int NodeSize, int LinkSize, int PlanningHorizonInMin,int AdjLinkSize, int StartTimeInMin=0, bool bODMEFlag=false)
	{
		m_NodeSize = NodeSize;
		m_LinkSize = LinkSize;

		m_PlanningHorizonInMin = PlanningHorizonInMin;
		m_StartTimeInMin = StartTimeInMin;

	//		cout <<"start to allocate network memory, size  = " << size << endl;
		m_NumberOfSPCalculationIntervals = g_NumberOfSPCalculationPeriods;  // make sure it is not zero

	//	m_NumberOfTDSPCalculationIntervals = int(m_PlanningHorizonInMin - m_StartTimeInMin) * g_TDSPTimetIntervalSizeForMin + 1;  // unit: 0.1 min
		m_NumberOfTDSPCalculationIntervals = 120 * g_TDSPTimetIntervalSizeForMin + 1;  // unit: 0.1 min

		m_AdjLinkSize = AdjLinkSize;

		m_OutboundSizeAry = new int[m_NodeSize];
		m_InboundSizeAry = new int[m_NodeSize];


		m_OutboundNodeAry = AllocateDynamicArray<int>(m_NodeSize,m_AdjLinkSize+1);
		m_OutboundLinkAry = AllocateDynamicArray<int>(m_NodeSize,m_AdjLinkSize+1);
		m_OutboundConnectorOriginZoneIDAry = AllocateDynamicArray<int>(m_NodeSize,m_AdjLinkSize+1);
		m_OutboundConnectorDestinationZoneIDAry = AllocateDynamicArray<int>(m_NodeSize,m_AdjLinkSize+1);
		m_LinkConnectorFlag = new int[m_LinkSize];
		m_LinkConnectorOriginZoneIDAry = new int[m_LinkSize];
		m_LinkConnectorDestinationZoneIDAry = new int[m_LinkSize];



		m_InboundLinkAry = AllocateDynamicArray<int>(m_NodeSize,m_AdjLinkSize+1);

		//movement-specific array
		m_OutboundMovementAry = AllocateDynamicArray<int>(m_LinkSize,m_AdjLinkSize+1);
		m_OutboundMovementDelayAry = AllocateDynamicArray<float>(m_LinkSize,m_AdjLinkSize+1);
		m_OutboundMovementSizeAry = new int[m_LinkSize];

		m_NodeBasedSEList = new int[m_NodeSize];
		m_LinkBasedSEList = new int[m_LinkSize];

		m_LinkTDDistanceAry = new float[m_LinkSize];
		m_LinkFFTTAry  = new float[m_LinkSize];


	//	cout <<"start to allocate time-dependent network memory " << endl;



		m_LinkTDTransitTimeAry = NULL;
		m_LinkTDTimeAry = NULL;

		m_LinkTDTimeAry = AllocateDynamicArray<float>(m_LinkSize, m_NumberOfSPCalculationIntervals);

		//m_LinkTDTransitTimeAry = AllocateDynamicArray<float>(m_LinkSize, m_NumberOfSPCalculationIntervals);

		m_FromNoAry = new int[m_LinkSize];

		m_ToNoAry = new int[m_LinkSize];

		NodeStatusAry = new int[m_NodeSize];                    // Node status array used in KSP;
		NodePredAry = new int[m_NodeSize];
		LabelTimeAry = new float[m_NodeSize];                     // label - time
		LabelCostAry = new float[m_NodeSize];                     // label - cost
		LabelDistanceAry = new float[m_NodeSize];                     // label - distance


		NodePredVectorPerType   =  AllocateDynamicArray<int>(g_DemandTypeVector.size()+1,m_NodeSize);
		LinkPredVectorPerType = AllocateDynamicArray<int>(g_DemandTypeVector.size() + 1, max(m_NodeSize, m_LinkSize));
		LabelCostVectorPerType = AllocateDynamicArray<float>(g_DemandTypeVector.size() + 1, max(m_NodeSize, m_LinkSize));
		LinkNoAry = new int[m_NodeSize];
		LabelCostAry = new float[m_NodeSize];                     // label - cost
		LabelDistanceAry = new float[m_NodeSize];                     // label - distance
		LabelDollarCostAry = new float[m_NodeSize];                  //lable dolloar cost

		LinkStatusAry = new int[m_LinkSize];                    // Node status array used in KSP;
		LinkPredAry = new int[m_LinkSize];
		LinkLabelTimeAry = new float[m_LinkSize];                     // label - time
		LinkLabelCostAry = new float[m_LinkSize];                     // label - cost

		cout <<"end of network memory allocation. " << endl;

	};
	DTANetworkForSP(int NodeSize, int LinkSize, int PlanningHorizonInMin,int AdjLinkSize, int StartTimeInMin=0)
	{

		Setup(NodeSize, LinkSize, PlanningHorizonInMin,AdjLinkSize,StartTimeInMin=0);
	};

	float GetTDTravelCost(int LinkNo, int time_interval_no)
	{
		return 0; 
	}


	void Clean()
	{
		if(m_OutboundSizeAry && m_NodeSize>=1)  delete m_OutboundSizeAry;
		if(m_InboundSizeAry && m_NodeSize>=1)  delete m_InboundSizeAry;

		DeallocateDynamicArray<int>(m_OutboundNodeAry,m_NodeSize, m_AdjLinkSize+1);
		DeallocateDynamicArray<int>(m_OutboundLinkAry,m_NodeSize, m_AdjLinkSize+1);
		DeallocateDynamicArray<int>(m_OutboundConnectorOriginZoneIDAry,m_NodeSize, m_AdjLinkSize+1);
		DeallocateDynamicArray<int>(m_OutboundConnectorDestinationZoneIDAry,m_NodeSize, m_AdjLinkSize+1);

		if(m_LinkSize>=1)
		{
			delete m_LinkConnectorFlag;
			delete m_LinkConnectorOriginZoneIDAry; 
			delete m_LinkConnectorDestinationZoneIDAry; 
		}
		DeallocateDynamicArray<int>(m_InboundLinkAry,m_NodeSize, m_AdjLinkSize+1);

		// delete movement array
		if(m_OutboundMovementSizeAry)  delete m_OutboundMovementSizeAry;
		DeallocateDynamicArray<int>(m_OutboundMovementAry,m_LinkSize, m_AdjLinkSize+1);
		DeallocateDynamicArray<float>(m_OutboundMovementDelayAry,m_LinkSize, m_AdjLinkSize+1);


		if(m_LinkBasedSEList) delete m_LinkBasedSEList;
		if (m_NodeBasedSEList) delete m_NodeBasedSEList;

		


		DeallocateDynamicArray<float>(m_LinkTDTimeAry, m_LinkSize, m_NumberOfSPCalculationIntervals);

		if (m_LinkTDTransitTimeAry!=NULL)
		DeallocateDynamicArray<float>(m_LinkTDTransitTimeAry, m_LinkSize, m_NumberOfSPCalculationIntervals);





		if(m_LinkTDDistanceAry) delete m_LinkTDDistanceAry;
		if(m_LinkFFTTAry) delete m_LinkFFTTAry;


		


		if(m_FromNoAry)		delete m_FromNoAry;
		if(m_ToNoAry)	delete m_ToNoAry;

		DeallocateDynamicArray<int>(NodePredVectorPerType,g_DemandTypeVector.size()+1,m_NodeSize);
		DeallocateDynamicArray<int>(LinkPredVectorPerType, g_DemandTypeVector.size() + 1, max(m_NodeSize, m_LinkSize));
		DeallocateDynamicArray<float>(LabelCostVectorPerType, g_DemandTypeVector.size() + 1, max(m_NodeSize, m_LinkSize));

		if(LinkNoAry) delete LinkNoAry;
		if(LabelCostAry) delete LabelCostAry;
		if(LabelDistanceAry) delete LabelDistanceAry;
		if (LabelDollarCostAry) delete LabelDollarCostAry;
		if (LabelTimeAry) delete LabelTimeAry;

		if(LinkStatusAry) delete LinkStatusAry;                 // Node status array used in KSP;
		if(LinkPredAry) delete LinkPredAry;
		if(LinkLabelTimeAry) delete LinkLabelTimeAry;
		if(LinkLabelCostAry) delete LinkLabelCostAry;


		if (NodeStatusAry) delete NodeStatusAry;
		if (NodePredAry) delete NodePredAry;


	}

	~DTANetworkForSP()
	{
		if(m_NodeSize==0)
			return;

		// do not free memory now
		// Clean();


	};


	int GetFeasibleTDSPTimeInterval(int time_interval)
	{
		if (time_interval < 0)
			time_interval = 0;
		if (time_interval >= m_NumberOfTDSPCalculationIntervals)
			time_interval = m_NumberOfTDSPCalculationIntervals - 1;

		return time_interval;

	}



void BuildPhysicalNetwork(int DayNo, int CurZoneID, e_traffic_flow_model TraffcModelFlag, bool bUseCurrentInformation = false, double CurrentTime = 0);

	void IdentifyBottlenecks(int StochasticCapacityFlag);

	bool TDLabelCorrecting_DoubleQueue(int origin, int origin_zone, int departure_time, int agent_type, float VOT, bool bDistanceCost, bool debug_flag, bool bDistanceCostOutput);   // Pointer to previous node (node)

	//movement based shortest path
	int FindBestPathWithVOTForSingleAgent_Movement(int origin_zone, int origin, int departure_time, int destination_zone, int destination, int agent_type, float VOT, int PathLinkList[MAX_NODE_SIZE_IN_A_PATH], float &TotalCost, bool bGeneralizedCostFlag, bool debug_flag, float PerceptionErrorRatio = 0);
	float AgentBasedPathFindingForEachZoneAndDepartureTimeInterval(int zone,int assignment_index, int iteration);

	// SEList: Scan List implementation: the reason for not using STL-like template is to avoid overhead associated pointer allocation/deallocation

	////////// link based SE List

		// SEList: Scan List implementation: the reason for not using STL-like template is to avoid overhead associated pointer allocation/deallocation
	void LinkBasedSEList_clear()
	{
		m_LinkBasedSEListFront= -1;
		m_LinkBasedSEListTail= -1;
	}

	void LinkBasedSEList_push_front(int link)
	{
		if(m_LinkBasedSEListFront == -1)  // start from empty
		{
			m_LinkBasedSEList[link] = -1;
			m_LinkBasedSEListFront  = link;
			m_LinkBasedSEListTail  = link;
		}
		else
		{
			m_LinkBasedSEList[link] = m_LinkBasedSEListFront;
			m_LinkBasedSEListFront  = link;
		}

	}
	void LinkBasedSEList_push_back(int link)
	{
		if(m_LinkBasedSEListFront == -1)  // start from empty
		{
			m_LinkBasedSEListFront = link;
			m_LinkBasedSEListTail  = link;
			m_LinkBasedSEList[link] = -1;
		}
		else
		{
			m_LinkBasedSEList[m_LinkBasedSEListTail] = link;
			m_LinkBasedSEList[link] = -1;
			m_LinkBasedSEListTail  = link;
		}
	}

	bool LinkBasedSEList_empty()
	{
		return(m_LinkBasedSEListFront== -1);
	}

	int LinkBasedSEList_front()
	{
		return m_LinkBasedSEListFront;
	}

	void LinkBasedSEList_pop_front()
	{
		int tempFront = m_LinkBasedSEListFront;
		m_LinkBasedSEListFront = m_LinkBasedSEList[m_LinkBasedSEListFront];
		m_LinkBasedSEList[tempFront] = -1;
	}

	/////////////// end of link-based SE list
	int  GetLinkNoByNodeIndex(int usn_index, int dsn_index);


	// the following is for node-based scan eligible list
	int m_NodeBasedSEListFront;
	int m_NodeBasedSEListTail;
	int* m_NodeBasedSEList;  // dimension: number of nodes



	void SEList_clear()
	{
		m_NodeBasedSEListFront = -1;
		m_NodeBasedSEListTail = -1;
	}

	void SEList_push_front(int node)
	{
		if (m_NodeBasedSEListFront == -1)  // start from empty
		{
			m_NodeBasedSEList[node] = -1;
			m_NodeBasedSEListFront = node;
			m_NodeBasedSEListTail = node;
		}
		else
		{
			m_NodeBasedSEList[node] = m_NodeBasedSEListFront;
			m_NodeBasedSEListFront = node;
		}

	}
	void SEList_push_back(int node)
	{
		if (m_NodeBasedSEListFront == -1)  // start from empty
		{
			m_NodeBasedSEListFront = node;
			m_NodeBasedSEListTail = node;
			m_NodeBasedSEList[node] = -1;
		}
		else
		{
			m_NodeBasedSEList[m_NodeBasedSEListTail] = node;
			m_NodeBasedSEList[node] = -1;
			m_NodeBasedSEListTail = node;
		}
	}

	bool SEList_empty()
	{
		return(m_NodeBasedSEListFront == -1);
	}

	int SEList_front()
	{
		return m_NodeBasedSEListFront;
	}

	void SEList_pop_front()
	{
		int tempFront = m_NodeBasedSEListFront;
		m_NodeBasedSEListFront = m_NodeBasedSEList[m_NodeBasedSEListFront];
		m_NodeBasedSEList[tempFront] = -1;
	}



};


struct NetworkLoadingTimeDepedentMOE
{
public: 

	NetworkLoadingTimeDepedentMOE()
	{
		 TotalTripTime = 0;
		 TotalTripFFTT = 0;
		 TotalTravelTime = 0;
		 TotalDelay = 0;
		 TotalDistance  = 0;
		 AgentSizeComplete = 0;
		 AvgTTI = 1;

		AvgTripTime = 0;
		AvgTravelTime = 0;
		AvgDelay = 0;
		AvgTripTimeIndex = 1;
		AvgDistance = 0;
		NumberofAgentsCompleteTrips = 0;
		NumberofAgentsGenerated = 0;
		SwitchPercentage = 0;
		ConsideringSwitchPercentage = 0;
		AvgUEGap = 0;
		AvgRelativeUEGap = 0;
	
	}

	double TotalTripTime; 
	double TotalTripFFTT;
	double TotalTravelTime; 
	double TotalDelay;
	double TotalDistance ;
	double AgentSizeComplete;

	double AvgTripTime;
	double AvgTravelTime;
	double AvgTripTimeIndex;
	double AvgDelay;
	double AvgTTI;
	double AvgDistance;
	int   NumberofAgentsCompleteTrips;
	int   NumberofAgentsGenerated;
	double SwitchPercentage;
	double ConsideringSwitchPercentage; 
	double AvgUEGap;
	double AvgRelativeUEGap;

};	

struct NetworkLoadingTimeDepedentGap
{
public:
	NetworkLoadingTimeDepedentGap()
	{
	
	total_gap = 0;
	total_relative_gap = 0;
	NumberofAgentsWithGapUpdate = 0;

	}
	double total_gap;
	double total_relative_gap;
	int NumberofAgentsWithGapUpdate;

};

struct NetworkLoadingOutput
{
public:


	NetworkLoadingOutput()
	{
		ResetStatistics();
	}

	void ResetStatistics ()
	{
		AvgUEGap = 0;
		AvgRelativeUEGap = 0;
		TotalDemandDeviation = 0;
		LinkVolumeAvgAbsError  =0 ;
		LinkVolumeAvgAbsPercentageError  =0 ;
		LinkVolumeRootMeanSquaredError = 0;
		CorrelationBetweenObservedAndSimulatedLinkVolume = 0;

		LinkSpeedAvgAbsError = 0;
		LinkSpeedAvgAbsPercentageError = 0;
		LinkSpeedRootMeanSquaredError = 0;
		CorrelationBetweenObservedAndSimulatedLinkSpeed = 0;

		AvgTripTime = 0;
		AvgTravelTime = 0;
		AvgDelay = 0;
		AvgTTI = 0;
		AvgDistance = 0;
		AvgCO = 0;
		NumberofAgentsCompleteTrips = 0;
		NumberofAgentsGenerated = 0;
		SwitchPercentage = 0;
		ConsideringSwitchPercentage = 0;
		NetworkClearanceTimeStamp_in_Min = 1440;
		NetworkClearanceTimePeriod_in_Min = 1440;

		TimeDedepentMOEMap.clear();
		TimeDedepentMOEMap.clear();
	}

	int   NetworkClearanceTimeStamp_in_Min;
	int  NetworkClearanceTimePeriod_in_Min;

	double AvgTripTime;
	double AvgTravelTime;
	double AvgDelay;
	double AvgTTI;
	double AvgDistance;

	double AvgCO;

	int   NumberofAgentsCompleteTrips;
	int   NumberofAgentsGenerated;
	double SwitchPercentage;
	double ConsideringSwitchPercentage; 
	double AvgUEGap;
	double AvgRelativeUEGap;

	std::map<int,NetworkLoadingTimeDepedentMOE> TimeDedepentMOEMap;
	std::map<int,NetworkLoadingTimeDepedentGap> TimeDedepentGapMap;


	double GetTimeDependentAvgTravelTime(int time)
	{
	int time_interval_no = time / 15;

		if(TimeDedepentMOEMap.find(time_interval_no)!= TimeDedepentMOEMap.end())
		{
			return TimeDedepentMOEMap[time_interval_no].AvgTravelTime;
			
		}else 
		{
			return 0;

		}
	}


	double GetTimeDependentAvgTravelDistance(int time)
	{
	int time_interval_no = time / 15;

		if(TimeDedepentMOEMap.find(time_interval_no)!= TimeDedepentMOEMap.end())
		{
			return TimeDedepentMOEMap[time_interval_no].AvgDistance ;
			
		}else 
		{
			return 0;

		}
	}

		double GetTimeDependentAvgGap(int time)
	{
	int time_interval_no = time / 15;

		if(TimeDedepentGapMap.find(time_interval_no)!= TimeDedepentGapMap.end())
		{
		 return TimeDedepentGapMap[time_interval_no].total_gap /max(1,TimeDedepentGapMap[time_interval_no].NumberofAgentsWithGapUpdate);
			
		}else 
		{
			return 0;

		}
	}


		double GetTimeDependentAvgRelativeGapInPercentage(int time)
	{
	int time_interval_no = time / 15;

		if(TimeDedepentGapMap.find(time_interval_no)!= TimeDedepentGapMap.end())
		{
		 return TimeDedepentGapMap[time_interval_no].total_relative_gap *100 /max(1,TimeDedepentGapMap[time_interval_no].NumberofAgentsWithGapUpdate);
			
		}else 
		{
			return 0;

		}
	}
	double TotalDemandDeviation;

	double LinkVolumeAvgAbsError;
	double LinkVolumeAvgAbsPercentageError;
	double LinkVolumeRootMeanSquaredError;
	double CorrelationBetweenObservedAndSimulatedLinkVolume;

	double LinkSpeedAvgAbsError;
	double LinkSpeedAvgAbsPercentageError;
	double LinkSpeedRootMeanSquaredError;
	double CorrelationBetweenObservedAndSimulatedLinkSpeed;

};

NetworkLoadingOutput g_NetworkLoading(e_traffic_flow_model TrafficFlowModelFlag, int SimulationMode, int Iteration);  // NetworkLoadingFlag = 0: static traffic assignment, 1: vertical queue, 2: spatial queue, 3: Newell's model, 


class NetworkSimulationResult
{
public: 

	int number_of_Agents;
	int number_of_Agents_DemandType[MAX_agent_type_SIZE];

	float avg_travel_time_in_min, avg_distance_s, avg_speed,avg_trip_time_in_min;

	NetworkSimulationResult()
	{

		number_of_Agents = 0;
		for(int p = 0; p < MAX_agent_type_SIZE; p++)
		{
			number_of_Agents_DemandType [p] = 0;
		}
		avg_trip_time_in_min = 0;
		avg_travel_time_in_min = 0;
		avg_distance_s = 0;

	}
};


class ODStatistics
{
public: 

	ODStatistics()
	{
		TotalAgentSize = 0;
		TotalCompleteAgentSize = 0;
		TotalTravelTime = 0;
		TotalDistance = 0;
		TotalCost = 0;

		Diverted_TotalAgentSize = 0;
		Diverted_TotalTravelTime = 0;
		Diverted_TotalDistance = 0;

		OriginZoneNumber = 0;
		DestinationZoneNumber = 0;

	}

	int OriginZoneNumber;
	int DestinationZoneNumber;

	int   TotalAgentSize;
	int   TotalCompleteAgentSize;
	float TotalTravelTime;
	float TotalDistance;
	float TotalCost;

	int   Diverted_TotalAgentSize;
	float Diverted_TotalTravelTime;
	float Diverted_TotalDistance;

};

	typedef  struct  
	{
		int NodeID;
		float AbsArrivalTimeOnDSN;
	} struct_Agent_Node;

class AgentArrayForOriginDepartrureTimeInterval
{
public:

	AgentArrayForOriginDepartrureTimeInterval()
	{
		m_DemandGenerationRatio = 1.0f;
	}

	float m_DemandGenerationRatio;
	std::vector<int> AgentArray;

	~AgentArrayForOriginDepartrureTimeInterval()
	{
		AgentArray.clear();
	}
};

class DTASettings
{
public:
	int use_mile_or_km_as_length_unit;
	int AdditionalYellowTimeForSignals;
	int IteraitonNoStartSignalOptimization;
	int IteraitonStepSizeSignalOptimization;
	int DefaultCycleTimeSignalOptimization;

	int pretimed_signal_control_type_code;
	int actuated_signal_control_type_code;

	int no_signal_control_type_code;

	int use_point_queue_model_for_on_ramps;  // no queue spillback
	int use_point_queue_model_for_off_ramps; //
	DTASettings()
	{

		use_mile_or_km_as_length_unit = 1;
		AdditionalYellowTimeForSignals = 0;
		IteraitonNoStartSignalOptimization = 10000;
		IteraitonStepSizeSignalOptimization = 5;
		DefaultCycleTimeSignalOptimization = 60;

		pretimed_signal_control_type_code = 5;
		actuated_signal_control_type_code = 6;
		no_signal_control_type_code = 1;
		use_point_queue_model_for_on_ramps = 1;
		use_point_queue_model_for_off_ramps = 1;

	}

};


extern DTASettings g_settings;

void OutputAgentTrajectoryData(std::string fname_agent);
void g_OutputLinkPerformanceData();

float g_GetPrivateProfileFloat( LPCTSTR section, LPCTSTR key, float def_value, LPCTSTR filename,bool print_out=false);
int g_WritePrivateProfileInt( LPCTSTR section, LPCTSTR key, int def_value, LPCTSTR filename) ;
int g_GetPrivateProfileInt( LPCTSTR section, LPCTSTR key, int def_value, LPCTSTR filename, bool print_out=false);


void InitWELLRNG512a (unsigned int *init);
double WELLRNG512a (void);

double g_GetRandomRatio();
int g_GetRandomInteger(double Value);

void g_ReadInputLinkTravelTime();
bool g_ReadInputAgentCSVFile(string file_name, bool bOutputLogFlag);

void g_AgentBasedDynamicTrafficAssignmentSimulation();

extern CString g_GetAppRunningTime(bool with_title = true);
extern CString g_GetAppRunningTimePerIteration(bool with_title = true);



extern CString g_timestamp_HHMM_convertor(float time1);


extern void g_FreeMemoryForAgentVector();


extern AgentArrayForOriginDepartrureTimeInterval** g_TDOAgentArray; // TDO for time-dependent origin;
extern std::vector<NetworkLoadingOutput>  g_AssignmentMOEVector;

// for fast data acessing
extern int g_LastLoadedAgentID; // scan Agents to be loaded in a simulation interval

extern FILE* g_ErrorFile;
extern FILE* g_DebugLogFile;
extern ofstream g_LogFile;
extern CCSVWriter g_SummaryStatFile;
extern ofstream g_LogFile;
void g_DTALiteMultiScenarioMain();

extern int g_InitializeLogFiles();
extern void g_ReadDTALiteSettings();
extern int g_ProhibitUTurnOnFeewayLinkFlag;

extern void g_TrafficAssignmentSimulation();
extern void g_OutputSimulationStatistics(int Iteration);
extern void g_FreeMemory(bool exit_flag);
extern void g_CloseFiles();

extern NetworkSimulationResult g_SimulationResult;
extern TCHAR g_DTASettingFileName[_MAX_PATH];
extern void g_ReadInputFiles();
extern void g_ReadScenarioInputFiles(int scenario_no);

extern void g_CreateLinkTollVector();
extern void g_ReadInputLinkAttribute_Parser();

extern std::string CString2StdString(CString str);


extern void g_AllocateDynamicArrayForAgents();
extern bool AddPathToAgent(DTAAgent * pAgent, std::vector<int> path_node_sequence, CString FileName);

extern void g_SetupTDTollValue(int DayNo);

//////////////////log file //////////////
extern FILE *g_simulation_log_file;
extern int g_simulation_log_level;

void _proxy_simulation_log(int level, int simulation_interval_no, double simulation_in_min, int debug_agent_id, const char *fmt, ...);

extern double g_simulation_time;
extern int g_GetFreeMemoryDataInMB();
extern CString g_GetUsedMemoryDataInMB();
extern int g_InitialFreeMemory;
extern vector<float> g_time_parser(vector<string>& inputstring);
extern bool g_AgentBasedShortestPathGeneration();
 // transform hhmm to minutes 

////////////////////////////////

void g_GenerateSimulationSummary(int iteration, bool NotConverged, int TotalNumOfAgentsGenerated, NetworkLoadingOutput* p_SimuOutput);
void g_ReadAMSMovementData();