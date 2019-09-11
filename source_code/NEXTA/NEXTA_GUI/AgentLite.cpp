// AgentLite.cpp : Defines the entry point for the console application.
/* Copyright (C) 2015 Xuesong Zhou - All Rights Reserved*/

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

#include "AgentLite.h"



// The one and only application object
using namespace std;

std::string g_AgentLiteProjectDirectory;


FILE* g_pFileDebugLog = NULL;
FILE* g_pFileOutputLog = NULL;

std::vector<float> g_link_volume_vector;
std::vector<float> g_link_speed_vector;

std::vector<CNode> g_node_vector;
std::vector<CLink> g_link_vector;
std::vector<COZone> g_zone_vector;
NetworkForSP* g_pNetworkForSP = NULL;

int g_AgentLite_number_of_max_zones = 0;
int g_AgentLite_number_of_k_path = 0;
float*** totaltype_destination_k_path_ODVolume_experienced_cost;
float*** totaltype_destination_k_path_ODVolume_tree_cost;
float*** totaltype_destination_k_path_ODVolume_tree_distance;
float*** totaltype_relative_gap_pi;
float*** totaltype_destination_k_path_pi_ODVolume_cost;
float*** total_od_cost;



float** totaltype_totaldestination_tree_cost;
float** totaltype_totaldestination_tree_distance;
float** totaltype_totaldestination_pi_cost;
float** totaltype_totaldestination_experienced_cost;
float** totaltype_totaldestination_relative_gap_pi;
float** totaltype_od_cost;
float** avg_od_cost_totaldestination;


float* total_tree_cost;
float* total_tree_distance;
float* total_pi_cost;
float* total_experienced_cost;
float* total_relative_gap_pi_cost_donot_consider_volume;
float* total_od_cost_totalorigin;
float* avg_od_cost_totalorigin;
float* _gap_;
float* _gap_relative_;


template <typename T>
T **Allocate2DDynamicArray_Al(int nRows, int nCols)
{
	T **dynamicArray;

	dynamicArray = new T*[nRows];

	for (int i = 0; i < nRows; i++)
	{
		dynamicArray[i] = new T[nCols];

		if (dynamicArray[i] == NULL)
		{
			cout << "Error: insufficent memory.";
			fprintf(g_pFileDebugLog, "Error: insufficent memory.");
			g_ProgramStop_agentlite();
		}

	}

	return dynamicArray;
}

template <typename T>
void Deallocate2DDynamicArray_Al(T** dArray, int nRows)
{
	for (int x = 0; x < nRows; x++)
	{
		delete[] dArray[x];
	}

	delete[] dArray;

}

template <typename T>
T ***Allocate3DDynamicArray_Al(int nX, int nY, int nZ)
{
	T ***dynamicArray;

	dynamicArray = new T**[nX];

	if (dynamicArray == NULL)
	{
		cout << "Error: insufficent memory.";
		fprintf(g_pFileDebugLog, "Error: insufficent memory.");
		g_ProgramStop_agentlite();
	}

	for (int x = 0; x < nX; x++)
	{
		dynamicArray[x] = new T*[nY];

		if (dynamicArray[x] == NULL)
		{
			cout << "Error: insufficent memory.";
			fprintf(g_pFileDebugLog, "Error: insufficent memory.");
			g_ProgramStop_agentlite();
		}

		for (int y = 0; y < nY; y++)
		{
			dynamicArray[x][y] = new T[nZ];
			if (dynamicArray[x][y] == NULL)
			{
				cout << "Error: insufficent memory.";
				fprintf(g_pFileDebugLog, "Error: insufficent memory.");
				g_ProgramStop_agentlite();
			}

		}
	}

	return dynamicArray;

}

template <typename T>
void Deallocate3DDynamicArray_Al(T*** dArray, int nX, int nY)
{
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
T ****Allocate4DDynamicArray_Al(int nM, int nX, int nY, int nZ)
{
	T ****dynamicArray;

	dynamicArray = new (std::nothrow) T***[nM];

	if (dynamicArray == NULL)
	{
		cout << "Error: insufficient memory.";
		fprintf(g_pFileDebugLog, "Error: insufficent memory.");
		g_ProgramStop_agentlite();
	}
	for (int m = 0; m < nM; m++)
	{
		dynamicArray[m] = new (std::nothrow) T**[nX];

		if (dynamicArray[m] == NULL)
		{
			cout << "Error: insufficient memory.";
			fprintf(g_pFileDebugLog, "Error: insufficent memory.");
			g_ProgramStop_agentlite();
		}

		for (int x = 0; x < nX; x++)
		{
			dynamicArray[m][x] = new (std::nothrow) T*[nY];

			if (dynamicArray[m][x] == NULL)
			{
				cout << "Error: insufficient memory.";
				fprintf(g_pFileDebugLog, "Error: insufficent memory.");
				g_ProgramStop_agentlite();
			}

			for (int y = 0; y < nY; y++)
			{
				dynamicArray[m][x][y] = new (std::nothrow) T[nZ];
				if (dynamicArray[m][x][y] == NULL)
				{
					cout << "Error: insufficient memory.";
					fprintf(g_pFileDebugLog, "Error: insufficent memory.");
					g_ProgramStop_agentlite();
				}
			}
		}
	}
	return dynamicArray;

}

template <typename T>
void Deallocate4DDynamicArray_Al(T**** dArray, int nM, int nX, int nY)
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


int g_number_of_CPU_threads()
{
	int number_of_threads = omp_get_max_threads();

	int max_number_of_threads = 4;

	if (number_of_threads > max_number_of_threads)
		number_of_threads = max_number_of_threads;

	return number_of_threads;

}


CTime g_AppStartTime;


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

bool g_read_a_line_agentlite(FILE* f)
/* read a line from the current line from the file */
{

	char ch;

	while (1) {
		ch = getc(f);
		if (ch != 13 && ch != 10 && ch != EOF)
		{
			// do nothing
		}
		else { /* terminate if it's end of line or end of file */
			{
				// do nothing
			}
			if (ch == EOF)
				return false;

			return true;
		}
	}
}


bool g_read_a_line_agentlite(FILE* f, char* aline, int & size)
/* read a line from the current line from the file */
{
	int max_size = size;
	char ch;
	size = 0;

	while (size < max_size) {
		ch = getc(f);
		if (ch != 13 && ch != 10 && ch != EOF)
			aline[size++] = ch;
		else { /* terminate if it's end of line or end of file */
			aline[size] = 0;
			if (ch == EOF)
				return false;

			return true;
		}
	}
	return false;
}



int g_read_number_of_numerical_values(char* line_string, int length)
//read a floating point number from the current pointer of the file,
//skip all spaces

{
	char ch, buf[32];

	int number_count = 0;
	int string_index = 0;

	/* returns -1 if end of file is reached */
	while (string_index<length)
	{

		int i = 0;
		int flag = 1;

		while (true)
		{
			if (string_index == length)
			{
				break;
			}


			ch = line_string[string_index++];
			if (ch == EOF) return number_count;
			if (isdigit(ch))
				break;

			if (ch == '-')
				flag = -1;
			else
				flag = 1;

		};
		if (ch == EOF) return number_count;
		while (isdigit(ch) || ch == '.') {
			buf[i++] = ch;
			ch = line_string[string_index++];

		}
		buf[i] = 0;

		double value = atof(buf);
		if (value>-0.0000001)  // positive values
		{

			number_count++;
		}
	}

	/* atof function converts a character string (char *) into a doubleing
	pointer equivalent, and if the string is not a floting point number,
	a zero will be return.
	*/

	return number_count;

}

int g_read_integer_agentlite(FILE* f, bool speicial_char_handling)
// read an integer from the current pointer of the file, skip all spaces
{
	char ch, buf[32];
	int i = 0;
	int flag = 1;
	/* returns -1 if end of file is reached */

	while (true)
	{
		ch = getc(f);
		//cout << "get from node successful: " << ch;
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
		//cout << "isdigit" << buf[i++] << endl;
		ch = fgetc(f);
		//cout << "new ch" << ch;
	}
	buf[i] = 0;


	return atoi(buf) * flag;

}


float g_read_float_agentlite(FILE *f)
//read a floating point number from the current pointer of the file,
//skip all spaces

{
	char ch, buf[32];
	int i = 0;
	int flag = 1;

	/* returns -1 if end of file is reached */

	while (true)
	{
		ch = getc(f);
		if (ch == EOF || ch == '*' || ch == '$') return -1;
		if (isdigit(ch))
			break;

		if (ch == '-')
			flag = -1;
		else
			flag = 1;

	};
	if (ch == EOF) return -1;
	while (isdigit(ch) || ch == '.') {
		buf[i++] = ch;
		ch = fgetc(f);

	}
	buf[i] = 0;

	/* atof function converts a character string (char *) into a doubleing
	pointer equivalent, and if the string is not a floting point number,
	a zero will be return.
	*/

	return (float)(atof(buf) * flag);

}


int g_read_numbers_from_a_line(FILE *f, std::vector<float> &ValueVector)
//read a floating point number from the current pointer of the file,
//skip all spaces
{
	char ch, buf[32];

	int string_index = 0;

	ValueVector.clear();

	/* returns -1 if end of file is reached */
	while (1)
	{

		int i = 0;
		int flag = 1;

		while (true)
		{
			ch = getc(f);
			if (ch == EOF)
				return -100;
			if (isdigit(ch))
				break;

			if (ch == '\n')
				return ValueVector.size();

			if (ch == '-')
				flag = -1;
			else
				flag = 1;

		};
		if (ch == EOF)
			return -100;

		while (isdigit(ch) || ch == '.') {
			buf[i++] = ch;
			ch = getc(f);

		}
		buf[i] = 0;

		double value = atof(buf);
		if (value>-0.0000001)  // positive values
		{
			ValueVector.push_back(value);

		}

		if (ch == '\n')
			return ValueVector.size();

	}

	/* atof function converts a character string (char *) into a doubleing
	pointer equivalent, and if the string is not a floting point number,
	a zero will be return.
	*/

	return ValueVector.size();

}


float g_read_float_from_a_line(FILE *f)
//read a floating point number from the current pointer of the file,
//skip all spaces

{
	char ch, buf[32];
	int i = 0;
	int flag = 1;

	/* returns -100 if end of line is reached */

	while (true)
	{
		ch = getc(f);
		if (ch == EOF || ch == '*' || ch == '$')
			return -1;

		if (ch == '\n')
			return -100;

		if (isdigit(ch))
			break;

		if (ch == '-')
			flag = -1;
		else
			flag = 1;

	};
	if (ch == EOF) return -1;
	while (isdigit(ch) || ch == '.') {
		buf[i++] = ch;
		ch = fgetc(f);

	}
	buf[i] = 0;

	/* atof function converts a character string (char *) into a doubleing
	pointer equivalent, and if the string is not a floting point number,
	a zero will be return.
	*/

	return (float)(atof(buf) * flag);

}



void g_ProgramStop_agentlite()
{

	cout << "AgentLite Program stops. Press any key to terminate. Thanks!" << endl;
	fprintf(g_pFileDebugLog, "AgentLite Program stops. Press any key to terminate. Thanks!");
	getchar();
	exit(0);
};


std::string CString2StdString(CString str)
{	 // Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(str);

	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);

	return strStd;
}

void ReadLinkTollScenarioFile(Assignment& g_assignment)
{

	for (unsigned li = 0; li < g_link_vector.size(); li++)
	{

		g_link_vector[li].TollMAP.erase(g_link_vector[li].TollMAP.begin(), g_link_vector[li].TollMAP.end()); // remove all previouly read records
	}

	// generate toll based on demand type code in input_link.csv file
	int demand_mode_type_count = 0;

	for (unsigned li = 0; li < g_link_vector.size(); li++)
	{
		if (g_link_vector[li].demand_type_code.size() >= 1)
		{  // with data string

			std::string demand_type_code = g_link_vector[li].demand_type_code;

			vector<float> TollRate;
			for (int pt = 1; pt <= g_assignment.g_DemandTypeVector.size(); pt++)
			{
				CString number;
				number.Format(_T("%d"), pt);

				std::string str_number = CString2StdString(number);
				if (demand_type_code.find(str_number) == std::string::npos)   // do not find this number
				{
					g_link_vector[li].TollMAP[pt] = 999;
					demand_mode_type_count++;
				}
				else
				{
					g_link_vector[li].TollMAP[pt] = 0;
				}

			}  //end of pt
		}
	}
}


void g_ReadDemandFileBasedOnDemandFileList(Assignment& g_assignment)
{

	float total_demand_in_demand_file = 0;
	float total_number_of_vehicles_to_be_generated = 0;

	int max_line_number_for_logging = 10;

	CCSVParser parser;
	cout << "Step 7.1: Reading file input_demand_file_list.csv..." << endl;
	fprintf(g_pFileDebugLog, "Step 7.1: Reading file input_demand_file_list.csv...");

	if (parser.OpenCSVFile(g_AgentLiteProjectDirectory+"input_demand_file_list.csv", true))
	{
		int i = 0;

		while (parser.ReadRecord())
		{

			int file_sequence_no = 1;
			string file_name;
			string format_type = "null";
			int number_of_lines_to_be_skipped = 0;
			int subtotal_in_last_column = 0;

			int number_of_demand_types = 0;
			float local_demand_loading_multiplier = 1;
			char demand_type_field_name[20];
			int demand_type_code[20] = { 0 };

			int demand_format_flag = 0;

			if (parser.GetValueByFieldName("file_sequence_no", file_sequence_no) == false)
				break;

			if (file_sequence_no <= -1)  // skip negative sequence no 
				continue;

			parser.GetValueByFieldName("file_name", file_name);

			file_name = g_AgentLiteProjectDirectory + file_name;
			if (file_name.length() == 0)  // no file name input
			{
				break;
			}

			parser.GetValueByFieldName("format_type", format_type);
			if (format_type.find("null") != string::npos)  // skip negative sequence no 
			{
				cout << "Please provide format_type in file input_demand_file_list.csv" << endl;
				fprintf(g_pFileDebugLog, "Step 7.1: Reading file input_demand_file_list.csv...");
				g_ProgramStop_agentlite();
			}

			{ // error checking

				if (file_name.find("AMS_OD_table.csv") != string::npos && format_type.find("column") == string::npos)
				{
					cout << "Please specify column format for demand file AMS_OD_table.csv, other than " << format_type << endl;
				}

				if (file_name.find("demand.dat") != string::npos && format_type.find("dynasmart") == string::npos)
				{
					cout << "Please specify dynasmart format for demand file demand.dat, other than " << format_type << endl;
				}

				if (file_name.find("demand_HOV.dat") != string::npos && format_type.find("dynasmart") == string::npos)
				{
					cout << "Please specify dynasmart format for demand file demand_HOV.dat, other than " << format_type << endl;
				}

				if (file_name.find("demand_truck.dat") != string::npos && format_type.find("dynasmart") == string::npos)
				{
					cout << "Please specify dynasmart format for demand_truck file demand.dat, other than " << format_type << endl;
				}
				if (file_name.compare("agent.bin") == 0 && format_type.find("agent_bin") == string::npos)
				{
					cout << "Please specify agent_bin format for agent binary file , other than " << format_type << endl;
				}

			}

			parser.GetValueByFieldName("number_of_lines_to_be_skipped", number_of_lines_to_be_skipped);
			parser.GetValueByFieldName("subtotal_in_last_column", subtotal_in_last_column);
			parser.GetValueByFieldName("loading_multiplier", local_demand_loading_multiplier);

			double total_ratio = 0;

			parser.GetValueByFieldName("number_of_demand_types", number_of_demand_types);

			for (int type = 1; type <= number_of_demand_types; type++)
			{
				sprintf(demand_type_field_name, "demand_type_%d", type);
				int demand_type = -1;
				parser.GetValueByFieldName(demand_type_field_name, demand_type);

				if (demand_type <= 0)
				{
					cout << "Missing input: no value has been specified for field " << demand_type_field_name << " in file " << file_name << " in demand meta file input_demand_file_list.csv. " << endl;
					fprintf(g_pFileDebugLog, "Missing input: no value has been specified for field %ld in file %s in demand meta file input_demand_file_list.csv.\n", demand_type_field_name, file_name);
					g_ProgramStop_agentlite();

				}

				demand_type_code[type] = demand_type;
			}



			if (format_type.find("column") != string::npos)  // or muliti-column
			{

				if (number_of_demand_types == 0)
				{
					cout << "number_of_demand_types = 0 in file input_demand_file_list.csv. Please check." << endl;
					fprintf(g_pFileDebugLog, "number_of_demand_types = 0 in file input_demand_file_list.csv. Please check.");
					g_ProgramStop_agentlite();
				}

				bool bFileReady = false;
				int i;

				FILE* st;

				//test # of numerical values per line
				fopen_s(&st, file_name.c_str(), "r");
				if (st != NULL)
				{

					char  str_line[_MAX_STRING_LINE]; // input string
					int str_line_size = _MAX_STRING_LINE;

					// skip lines
					for (int line_skip = 0; line_skip < number_of_lines_to_be_skipped; line_skip++)
					{
						str_line_size = _MAX_STRING_LINE;
						g_read_a_line_agentlite(st, str_line, str_line_size); //  skip the first line
						cout << str_line << endl;
					}

					str_line_size = _MAX_STRING_LINE;
					g_read_a_line_agentlite(st, str_line, str_line_size); //  skip the first line
					int number_of_values = g_read_number_of_numerical_values(str_line, str_line_size);

					fclose(st);
				}

				// read the file formaly after the test. 

				fopen_s(&st, file_name.c_str(), "r");
				if (st != NULL)
				{
					char  str_line[2000]; // input string
					int str_line_size = _MAX_STRING_LINE;

					// skip lines
					for (int line_skip = 0; line_skip < number_of_lines_to_be_skipped; line_skip++)
					{
						str_line_size = _MAX_STRING_LINE;
						g_read_a_line_agentlite(st, str_line, str_line_size); //  skip the first line
					}

					bFileReady = true;
					int line_no = number_of_lines_to_be_skipped;

					while (true)
					{
						int origin_zone = g_read_integer_agentlite(st, true);

						if (origin_zone <= 0)
						{

							if (line_no == 1 && !feof(st))  // read only one line, but has not reached the end of the line
							{
								cout << endl << "Error: Only one line has been read from file. Are there multiple columns of demand type in file " << file_name << " per line?" << endl;
								fprintf(g_pFileDebugLog, "Error: Only one line has been read from file. Are there multiple columns of demand type in file %s  per line?\n ", file_name);
								g_ProgramStop_agentlite();

							}
							break;
						}

						if (g_assignment.g_zoneid_to_zone_seq_no_mapping.find(origin_zone) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())
						{
							continue; // origin zone  has not been defined, skipped. 
						}

						int destination_zone = g_read_integer_agentlite(st, true);

						if (g_assignment.g_zoneid_to_zone_seq_no_mapping.find(destination_zone) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())
						{
							continue; // origin zone  has not been defined, skipped. 
						}

						int from_zone_seq_no = 0;
						int to_zone_seq_no = 0;
						from_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[origin_zone];
						to_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[destination_zone];

						float number_of_vehicles;

						for (int type = 1; type <= number_of_demand_types; type++)
						{

							float demand_value = g_read_float_from_a_line(st);

							if (demand_value < -99) // encounter return 
							{
								break;
							}

							number_of_vehicles = demand_value*g_assignment.g_DemandGlobalMultiplier*local_demand_loading_multiplier;

							if (demand_type_code[type] >= 1)  // load this demand type
							{

								g_assignment.total_demand[demand_type_code[type]] += number_of_vehicles;
								g_assignment.g_demand_array[from_zone_seq_no][to_zone_seq_no][demand_type_code[type]] += number_of_vehicles;
								//g_assignment.total_demand_volume += number_of_vehicles;

								// we generate vehicles here for each OD data line
								if (line_no <= 5)  // read only one line, but has not reached the end of the line
									cout << "origin:" << origin_zone << ", destination: " << destination_zone << ", value = " << number_of_vehicles << endl;

								if (line_no % 100000 == 0)
								{
									cout << "Reading file no." << file_sequence_no << ": " << file_name << " at " << line_no / 1000 << "K lines..." << endl;
								}


							}
							else if (type != 0)  // if demand type == 0 then we will skip this value. By doing so, we can reading only one demand type per record with demand-type specific departure time loading profile. e.g. truck vs. HOV
							{
								cout << "demand type " << type << " in file input_demand_file_list has not been defined. Please check." << endl;
								fprintf(g_pFileDebugLog, "demand type %d in file input_demand_file_list has not been defined. Please check.",type);
								g_ProgramStop_agentlite();

							}
						}  // for each demand type

						if (subtotal_in_last_column == 1)
							g_read_float_from_a_line(st);  // so skip this last number

						line_no++;
					}  // scan lines


					fclose(st);
					for (int i = 1;i <= number_of_demand_types; i++)
					{
						cout << "total_demand_of" << file_name << "'s demand_type_" << i << " is " << g_assignment.total_demand[i] << " " << endl;
					}
					//cout << "total_demand_volume is " << g_assignment.total_demand_volume << endl;
				}
				else  //open file
				{
					cout << "Error: File " << file_name << " cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
					fprintf(g_pFileDebugLog, "Error: File %s cannot be opened.\n It might be currently used and locked by EXCEL.",file_name);
					g_ProgramStop_agentlite();

				}
			}

			else if (format_type.compare("matrix") == 0)
			{

				vector<int> LineIntegerVector;

				CCSVParser parser;
				parser.IsFirstLineHeader = false;
				if (parser.OpenCSVFile(file_name, true))
				{
					int control_type_code;
					int i = 0;
					if (parser.ReadRecord())
					{
						parser.ConvertLineStringValueToIntegers();
						LineIntegerVector = parser.LineIntegerVector;
					}
				}

				int number_of_zones = LineIntegerVector.size();


				bool bFileReady = false;
				int i;

				FILE* st;
				fopen_s(&st, file_name.c_str(), "r");
				if (st != NULL)
				{
					// read the first line
					g_read_a_line_agentlite(st);

					cout << "number of zones to be read = " << number_of_zones << endl;

					//test if a zone has been defined. 
					for (int destination_zone_index = 0; destination_zone_index < number_of_zones; destination_zone_index++)
					{
						int zone = LineIntegerVector[destination_zone_index];

						if (g_assignment.g_zoneid_to_zone_seq_no_mapping.find(zone) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())
						{
							continue; // origin zone  has not been defined, skipped. 
						}
					}

					int line_no = 0;
					for (int origin_zone_index = 0; origin_zone_index < number_of_zones; origin_zone_index++)
					{
						int origin_zone = g_read_integer_agentlite(st, true); // read the origin zone number

						if (origin_zone <= 0)
							break;

						cout << "Reading file no." << file_sequence_no << " " << file_name << " at zone " << origin_zone << " ... " << endl;

						for (int destination_zone_index = 0; destination_zone_index < number_of_zones; destination_zone_index++)
						{
							int destination_zone = LineIntegerVector[destination_zone_index];
							int from_zone_seq_no = 0;
							int to_zone_seq_no = 0;
							from_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[origin_zone];
							to_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[destination_zone];

							float value = g_read_float_agentlite(st);

							float number_of_vehicles = value*g_assignment.g_DemandGlobalMultiplier*local_demand_loading_multiplier;  // read the value

							if (line_no <= 5)  // read only one line, but has not reached the end of the line
								cout << "origin:" << origin_zone << ", destination: " << destination_zone << ", value = " << number_of_vehicles << endl;

							line_no++;
							int type = 1;  // first demand type definition
							if (demand_type_code[type] >= 1)  // feasible demand type
							{
								g_assignment.total_demand[demand_type_code[type]] += number_of_vehicles;
								g_assignment.g_demand_array[from_zone_seq_no][to_zone_seq_no][demand_type_code[type]] += number_of_vehicles;
								//g_assignment.total_demand_volume += number_of_vehicles;
							}

						}
						//
						if (subtotal_in_last_column == 1)
							g_read_float_agentlite(st); //read sub total value

					}
					fclose(st);
					for (int i = 1;i <= number_of_demand_types; i++)
					{
						cout << "total_demand_of" << file_name << "'s demand_type_" << i << " is " << g_assignment.total_demand[i] << " " << endl;
					}
					//cout << "total_demand_volume is " << g_assignment.total_demand_volume << endl;
					fprintf(g_pFileDebugLog, "total_demand_volume is %.3f", g_assignment.total_demand_volume);
				}
				else  //open file
				{
					cout << "Error: File " << file_name << " cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
					fprintf(g_pFileDebugLog, "Error: File  %s cannot be opened.\n It might be currently used and locked by EXCEL.", file_name);
					g_ProgramStop_agentlite();

				}
			}
			else if (format_type.compare("full_matrix") == 0)
			{
				vector<int> LineIntegerVector;

				CCSVParser parser;
				parser.IsFirstLineHeader = false;
				if (parser.OpenCSVFile(file_name, false))
				{
					int control_type_code;
					int i = 0;
					if (parser.ReadRecord())
					{
						parser.ConvertLineStringValueToIntegers();
						LineIntegerVector = parser.LineIntegerVector;
					}

				}

				int number_of_zones = LineIntegerVector.size();


				bool bFileReady = false;
				int i;

				FILE* st;
				fopen_s(&st, file_name.c_str(), "r");
				if (st != NULL)
				{
					// read the first line
					g_read_a_line_agentlite(st);

					cout << "number of zones to be read = " << number_of_zones << endl;

					int line_no = 0;


					for (int origin_zone_index = 0; origin_zone_index < number_of_zones; origin_zone_index++)
					{
						int origin_zone = g_read_integer_agentlite(st, true); // read the origin zone number

						if (origin_zone == -1)
							break;

						if (g_assignment.g_zoneid_to_zone_seq_no_mapping.find(origin_zone) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())
						{
							continue; // origin zone  has not been defined, skipped. 
						}

						if (origin_zone % 100 == 0)
							cout << "Reading file no." << file_sequence_no << ": " << file_name << " at zone " << origin_zone << ",total demand = " << total_demand_in_demand_file << "..." << endl;

						for (int destination_zone_index = 0; destination_zone_index < number_of_zones; destination_zone_index++)
						{
							int destination_zone = LineIntegerVector[destination_zone_index];
							float value = g_read_float_agentlite(st);
							int from_zone_seq_no = 0;
							int to_zone_seq_no = 0;
							from_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[origin_zone];
							to_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[destination_zone];

							float number_of_vehicles = value*g_assignment.g_DemandGlobalMultiplier*local_demand_loading_multiplier;  // read the value

							if (line_no <= 5)  // read only one line, but has not reached the end of the line
								cout << "origin:" << origin_zone << ", destination: " << destination_zone << ", value = " << number_of_vehicles << endl;

							line_no++;
							int type = 1;  // first demand type definition
							if (demand_type_code[type] >= 1)  // feasible demand type
							{
								g_assignment.total_demand[demand_type_code[type]] += number_of_vehicles;
								g_assignment.g_demand_array[from_zone_seq_no][to_zone_seq_no][demand_type_code[type]] += number_of_vehicles;
								//g_assignment.total_demand_volume += number_of_vehicles;
							}

						}
						//
						if (subtotal_in_last_column == 1)
							g_read_float_agentlite(st); //read sub total value

					}

					fclose(st);
					for (int i = 1;i <= number_of_demand_types; i++)
					{
						cout << "total_demand_of" << file_name << "'s demand_type_" << i << " is " << g_assignment.total_demand[i] << " " << endl;
					}
					//cout << "total_demand_volume is " << g_assignment.total_demand_volume << endl;
					fprintf(g_pFileDebugLog, "total_demand_volume is %.3f", g_assignment.total_demand_volume);
				}
				else  //open file
				{
					cout << "Error: File " << file_name << " cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
					fprintf(g_pFileDebugLog, "Error: File  %s cannot be opened.\n It might be currently used and locked by EXCEL.", file_name);
					g_ProgramStop_agentlite();

				}


			}
			else
			{
				cout << "Error: format_type = " << format_type << " is not supported. Currently DTALite supports multi_column, matrix, full_matrix, dynasmart, agent_csv, agent_bin, trip_csv,transims_trip_file." << endl;
				fprintf(g_pFileDebugLog, "Error: format_type = %s is not supported. Currently DTALite supports multi_column, matrix, full_matrix, dynasmart, agent_csv, agent_bin, trip_csv,transims_trip_file.", format_type);
				g_ProgramStop_agentlite();

			}
		}
	}

}


void g_perform_tree_based_assignment(int iteration_number, int b_debug_detail_flag, Assignment& g_assignment)
{
	// given precalculated k SP tree
	// to find the link volume and link cost
	// to do: perfor multiple UE or likely iterations 

#pragma omp parallel for  
	// step 1: C++ open mp automatically create n threads., each thread has its own computing thread on a cpu core 
	//
	for (int OZone = 0; OZone < g_zone_vector.size(); OZone++)
	{
		g_zone_vector[OZone].calculate_link_flow_second(iteration_number, b_debug_detail_flag, g_assignment); //changed by zhuge

	}

#pragma omp parallel for  // step 2 collect all partial link volume to compute link volume across all zones
	for (int l = 0; l < g_link_vector.size(); l++)
	{
		g_link_vector[l].tally_flow_volume_across_all_processors_second(iteration_number);
		g_link_vector[l].CalculateBPRFunction();

	}

	if (iteration_number >= 10)   // perform ODME
	{
		for (int OZone = 0; OZone < g_zone_vector.size(); OZone++)
		{
			g_zone_vector[OZone].update_od_volume_with_measurements();
		}

	}
}

void COZone::calculate_link_flow_second(int iteration_number, int b_debug_detail_flag, Assignment& assignment)  // this is the main function
{
	//Notes: 
	// given input from the shortest path tree at iteration k, with the least cost pi
	// given previous sth shortest path tree, s =0,1,..., k-1
	// use the link cost g_link_vector[current_link_seq_no].cost + g_link_vector[current_link_seq_no].TollMAP[g_DemandTypeVector[i]];
	// backtrace sth shortest tree,
	// 1) to obtain the experienced cost for the sth tree =-> only used for gap calculation
	// 2) accumulate the OD volume from o to d * destination_k_path_prob (1/(k+1)) to the corresponding link volume along the path in the sth sp tree
	// this zone-specific link volume vector is served for all volume from this origin zone, across different  destination, and demand type, and s = 0, 1, 2, ...., k-1 trees
	// this zone-specific link volume vector will be aggregated further across all origin zones, to obtain the total link volume to be used in BRP function 
	// record the pie for relative gap in the final statistic output

	reset_link_volume(g_link_vector.size());
	m_TotalObsError = 0;
	for (int d = 0; d < g_zone_vector.size(); d++)
	{
		for (int tau = 0; tau < _MAX_TIMEPERIODS; tau++)
		{
			for (int i = 0; i < assignment.g_DemandTypeVector.size(); i++)
			{
				if (d == this->zone_seq_no) // the same zone
					continue;  //skip

				destination_k_path_prob[d] = float(1) / float(iteration_number + 1);//each tree takes the average volume
				int destination_node_no = g_zone_vector[d].node_seq_no;

				// at each zone, m_memory_index_vector is equal to the k of k shortest path tree

				//vector<int> temp_path_node_vector; //node seq vector for each ODK, zhuge
				//if (b_debug_detail_flag)
				//{
				//	//push back D,K,demand_type,S into the vector to record
				//	temp_path_node_vector.push_back(d);
				//	temp_path_node_vector.push_back(iteration_number);
				//	temp_path_node_vector.push_back(i);
				//	temp_path_node_vector.push_back(s);
				//}

				int memory_block_index = m_memory_index_vector[assignment.g_DemandTypeVector.size()*iteration_number*_MAX_TIMEPERIODS + i*_MAX_TIMEPERIODS + tau];
				// get the memory block index

				//destination_k_path_cost[d][i][s][tau] = 0.0;  //calculate path cost for ODMOE
				//destination_k_path_distance[d][i][s][tau] = 0.0;
				int current_node_seq_no = destination_node_no;
				int current_link_seq_no = -1;

				// backtrace the sp tree from the destination to the root (at origin) 
				while (current_node_seq_no >= 0 && current_node_seq_no < assignment.g_number_of_nodes)
				{
					if (current_node_seq_no >= 0 && current_node_seq_no < assignment.g_number_of_nodes)  // this is valid node 
					{
						//if (b_debug_detail_flag)
						//{
						//	//push back element into the path node seq vector
						//	temp_path_node_vector.push_back(g_node_vector[current_node_seq_no].node_id);
						//}

						current_link_seq_no = g_pNetworkForSP[memory_block_index].m_link_predecessor_pertype[tau][i][current_node_seq_no];
						m_TotalObsError += g_link_vector[current_link_seq_no].m_ObsVolumeError;

						// fetch m_link_predecessor to mark the link volume

						if (current_link_seq_no >= 0 && current_link_seq_no < assignment.g_number_of_links)
						{
							link_volume[current_link_seq_no] += (destination_k_path_prob[d] * destination_OD_volume[d][assignment.g_DemandTypeVector[i]][tau]);
							/*destination_k_path_cost[d][i][s][tau] += g_link_vector[current_link_seq_no].cost + g_link_vector[current_link_seq_no].TollMAP[assignment.g_DemandTypeVector[i]];
							destination_k_path_distance[d][i][s][tau] += g_link_vector[current_link_seq_no].length;*/

						}
					}
					current_node_seq_no = g_pNetworkForSP[memory_block_index].m_node_predecessor_pertype[tau][i][current_node_seq_no];  // update node seq no		
				}

				//if (b_debug_detail_flag)
				//{
				//	//push back the temp path node vector to global path node vector for each origin
				//	m_path_node_vector.push_back(temp_path_node_vector);
				//	temp_path_node_vector.clear();
				//}

				//calculating the experienced cost for trees of iteration_number 
				/*if (s != iteration_number)
				{
				destination_k_path_ODVolume_experienced_cost[d][iteration_number][i][tau] += (float(1) / float(iteration_number)) * destination_OD_volume[d][assignment.g_DemandTypeVector[i]][tau] * destination_k_path_cost[d][i][s][tau];
				destination_k_path_ODVolume_tree_distance[d][iteration_number][i][tau] += (float(1) / float(iteration_number)) * destination_OD_volume[d][assignment.g_DemandTypeVector[i]][tau] * destination_k_path_distance[d][i][s][tau];
				}*/

				//calculating the pi cost for trees of iteration_number 
				//destination_k_path_pi_ODVolume_cost[d][iteration_number][i][tau] = destination_k_path_cost[d][i][iteration_number][tau] * destination_OD_volume[d][assignment.g_DemandTypeVector[i]][tau];
				//destination_k_path_cost[d][iteration_number][i][iteration_number] is the pie 

				//calculating the total pi_cost for the relative_gap calculating
				/*if (destination_OD_volume[d][assignment.g_DemandTypeVector[i]] == 0)
				{
				pie_used_in_relative_gap_function[d][iteration_number][i][tau] = 0;
				}
				else
				{
				pie_used_in_relative_gap_function[d][iteration_number][i][tau] = destination_k_path_cost[d][i][iteration_number][tau];
				}*/

			}
		}

	}

}


void COZone::calculate_link_flow(int iteration_number, int b_debug_detail_flag, Assignment& g_assignment)  // this is the main function
{
	//Notes: 
	// given input from the shortest path tree at iteration k, with the least cost pi
	// given previous sth shortest path tree, s =0,1,..., k-1
	// use the link cost g_link_vector[current_link_seq_no].cost + g_link_vector[current_link_seq_no].TollMAP[g_DemandTypeVector[i]];
	// backtrace sth shortest tree,
	// 1) to obtain the experienced cost for the sth tree =-> only used for gap calculation
	// 2) accumulate the OD volume from o to d * destination_k_path_prob (1/(k+1)) to the corresponding link volume along the path in the sth sp tree
	// this zone-specific link volume vector is served for all volume from this origin zone, across different  destination, and demand type, and s = 0, 1, 2, ...., k-1 trees
	// this zone-specific link volume vector will be aggregated further across all origin zones, to obtain the total link volume to be used in BRP function 
	// record the pie for relative gap in the final statistic output

	reset_link_volume(g_link_vector.size());
	m_TotalObsError = 0;

	for (int d = 0; d < g_zone_vector.size(); d++)
	{
		for (int tau = 0; tau < _MAX_TIMEPERIODS; tau++)
		{
			for (int i = 0; i < g_assignment.g_DemandTypeVector.size(); i++)
			{
				if (d == this->zone_seq_no) // the same zone
					continue;  //skip

				destination_k_path_prob[d] = float(1) / float(iteration_number + 1);//each tree takes the average volume
				int destination_node_no = g_zone_vector[d].node_seq_no;

				// at each zone, m_memory_index_vector is equal to the k of k shortest path tree

				for (int s = 0; s <= iteration_number; s++)
				{

					vector<int> temp_path_node_vector; //node seq vector for each ODK, zhuge
					if (b_debug_detail_flag)
					{
						//push back D,K,demand_type,S into the vector to record
						temp_path_node_vector.push_back(d);
						temp_path_node_vector.push_back(iteration_number);
						temp_path_node_vector.push_back(i);
						temp_path_node_vector.push_back(s);
					}

					int memory_block_index = m_memory_index_vector[g_assignment.g_DemandTypeVector.size()*s*_MAX_TIMEPERIODS + i*_MAX_TIMEPERIODS + tau];
					// get the memory block index

					destination_k_path_cost[d][i][s][tau] = 0.0;  //calculate path cost for ODMOE
					destination_k_path_distance[d][i][s][tau] = 0.0;
					int current_node_seq_no = destination_node_no;
					int current_link_seq_no = -1;

					// backtrace the sp tree from the destination to the root (at origin) 
					while (current_node_seq_no >= 0 && current_node_seq_no < g_assignment.g_number_of_nodes)
					{
						if (current_node_seq_no >= 0 && current_node_seq_no < g_assignment.g_number_of_nodes)  // this is valid node 
						{
							if (b_debug_detail_flag)
							{
								//push back element into the path node seq vector
								temp_path_node_vector.push_back(g_node_vector[current_node_seq_no].node_id);
							}

							current_link_seq_no = g_pNetworkForSP[memory_block_index].m_link_predecessor_pertype[tau][i][current_node_seq_no];

							// fetch m_link_predecessor to mark the link volume

							if (current_link_seq_no >= 0 && current_link_seq_no < g_assignment.g_number_of_links)
							{
								link_volume[current_link_seq_no] += (destination_k_path_prob[d] * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau]);

								m_TotalObsError += g_link_vector[current_link_seq_no].m_ObsVolumeError;

								destination_k_path_cost[d][i][s][tau] += g_link_vector[current_link_seq_no].cost + g_link_vector[current_link_seq_no].TollMAP[g_assignment.g_DemandTypeVector[i]];
								destination_k_path_distance[d][i][s][tau] += g_link_vector[current_link_seq_no].length;

							}
						}
						current_node_seq_no = g_pNetworkForSP[memory_block_index].m_node_predecessor_pertype[tau][i][current_node_seq_no];  // update node seq no		
					}

					if (b_debug_detail_flag)
					{
						//push back the temp path node vector to global path node vector for each origin
						m_path_node_vector.push_back(temp_path_node_vector);
						temp_path_node_vector.clear();
					}

					//calculating the experienced cost for trees of iteration_number 
					if (s != iteration_number)
					{
						destination_k_path_ODVolume_experienced_cost[d][iteration_number][i][tau] += (float(1) / float(iteration_number)) * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau] * destination_k_path_cost[d][i][s][tau];
						destination_k_path_ODVolume_tree_distance[d][iteration_number][i][tau] += (float(1) / float(iteration_number)) * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau] * destination_k_path_distance[d][i][s][tau];
					}
				}

				//calculating the pi cost for trees of iteration_number 
				destination_k_path_pi_ODVolume_cost[d][iteration_number][i][tau] = destination_k_path_cost[d][i][iteration_number][tau] * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau];
				//destination_k_path_cost[d][iteration_number][i][iteration_number] is the pie 

				//calculating the total pi_cost for the relative_gap calculating
				if (destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]] == 0)
				{
					pie_used_in_relative_gap_function[d][iteration_number][i][tau] = 0;
				}
				else
				{
					pie_used_in_relative_gap_function[d][iteration_number][i][tau] = destination_k_path_cost[d][i][iteration_number][tau];
				}

			}
		}

	}

}

void CLink::tally_flow_volume_across_all_processors_second(int iteration_number)
{
	float new_volume = flow_volume * (float(iteration_number) / float(iteration_number + 1));
	float link_volume_private = 0.0;
	link_volume_private += new_volume;
	for (int OZone = 0; OZone < g_zone_vector.size(); OZone++)
	{
		link_volume_private += g_zone_vector[OZone].link_volume[this->link_seq_no];

	}
	flow_volume = link_volume_private;
	//cout << "ha" << endl;
}

void CLink::tally_flow_volume_across_all_processors()
{
	float link_volume_private = 0.0;
	for (int OZone = 0; OZone < g_zone_vector.size(); OZone++)
	{
		link_volume_private += g_zone_vector[OZone].link_volume[this->link_seq_no];

	}
	flow_volume = link_volume_private;

	if(m_ObsVolume>0)
	{ 
		m_ObsVolumeError = m_ObsVolume - flow_volume;
	}
}

void g_ReadInputData(Assignment& g_assignment)
{
	g_assignment.g_number_of_nodes = 0;
	g_assignment.g_number_of_links = 0;  // initialize  the counter to 0

	// step 1:read config file
	CCSVParser parser_config;

	if (parser_config.OpenCSVFile(g_AgentLiteProjectDirectory + "input_config.csv", true))
	{
		while (parser_config.ReadRecord())  // if this line contains [] mark, then we will also read field headers.
		{
			int from_node_id = 0;
			int to_node_id = 0;
			if (parser_config.GetValueByFieldName("number_of_threads", g_assignment.g_number_of_threads) == false)
				continue;
			if (parser_config.GetValueByFieldName("number_of_base_K_paths", g_assignment.g_number_of_K_paths) == false)
				continue;
			if (parser_config.GetValueByFieldName("number_of_reassignment_K_paths", g_assignment.g_reassignment_number_of_K_paths) == false)
				continue;
			if (parser_config.GetValueByFieldName("DemandGlobalMultiplier", g_assignment.g_DemandGlobalMultiplier) == false)
				continue;
			g_AgentLite_number_of_k_path = g_assignment.g_number_of_K_paths + g_assignment.g_reassignment_number_of_K_paths;
		}
	}
	// we now know the number of links
	cout << "number of threads = " << g_assignment.g_number_of_threads << endl;

	fprintf(g_pFileOutputLog, "number of threads =,%d\n", g_assignment.g_number_of_threads);
	fprintf(g_pFileOutputLog, "number of base_K_paths =,%d\n", g_assignment.g_number_of_K_paths);
	fprintf(g_pFileOutputLog, "number of reassignment_K_paths =,%d\n", g_assignment.g_reassignment_number_of_K_paths);
	fprintf(g_pFileOutputLog, "DemandGlobalMultiplier =,%d\n", g_assignment.g_DemandGlobalMultiplier);

	parser_config.CloseCSVFile();



	int internal_node_seq_no = 0;
	double x, y;
	// step 2: read node file 
	CCSVParser parser;

	g_AgentLite_number_of_max_zones = 0;
	if (parser.OpenCSVFile(g_AgentLiteProjectDirectory + "input_node.csv", true))
	{
		while (parser.ReadRecord())  // if this line contains [] mark, then we will also read field headers.
		{

			int node_id;
			int zone_id;

			if (parser.GetValueByFieldName("node_id", node_id) == false)
				continue;
			parser.GetValueByFieldName("zone_id", zone_id);

			
			if (zone_id >= 1 && g_assignment.g_zoneid_to_zone_seq_no_mapping.find(zone_id) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())
			{
				g_assignment.g_zoneid_to_zone_seq_no_mapping[zone_id] = g_AgentLite_number_of_max_zones;
				g_AgentLite_number_of_max_zones++;
			}
		}

	}

	g_assignment.g_zoneid_to_zone_seq_no_mapping.clear();
	parser.CloseCSVFile();

	// second time to open
	if (parser.OpenCSVFile(g_AgentLiteProjectDirectory+"input_node.csv", true))
	{

		while (parser.ReadRecord())  // if this line contains [] mark, then we will also read field headers.
		{

			int node_id;

			if (parser.GetValueByFieldName("node_id", node_id) == false)
				continue;

			if (g_assignment.g_internal_node_seq_no_map.find(node_id) != g_assignment.g_internal_node_seq_no_map.end())
			{
				continue; //has been defined
			}
			g_assignment.g_internal_node_seq_no_map[node_id] = internal_node_seq_no;

			parser.GetValueByFieldName("x", x, false);
			parser.GetValueByFieldName("y", y, false);


			CNode node;  // create a node object 

			node.node_id = node_id;
			node.node_seq_no = internal_node_seq_no;
			parser.GetValueByFieldName("zone_id", node.zone_id);

			if (node.zone_id >= 1 && g_assignment.g_zoneid_to_zone_seq_no_mapping.find(node.zone_id) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())  // create a new zone  // we assume each zone only has one node
			{
				COZone* p_ozone;
				p_ozone = new COZone();
				p_ozone->node_seq_no = node.node_seq_no;
				p_ozone->zone_id = node.zone_id;
				p_ozone->zone_seq_no = g_zone_vector.size();
				g_zone_vector.push_back((*p_ozone));  // add element into vector
				g_assignment.g_zoneid_to_zone_seq_no_mapping[p_ozone->zone_id] = p_ozone->zone_seq_no;  // create the zone id to zone seq no mapping
			}

			node.x = x;
			node.y = y;
			internal_node_seq_no++;

			g_node_vector.push_back(node);  // push it to the global node vector

			g_assignment.g_number_of_nodes++;
			if (g_assignment.g_number_of_nodes % 1000 == 0)
				cout << "reading " << g_assignment.g_number_of_nodes << " nodes.. " << endl;
		}

		cout << "number of nodes = " << g_assignment.g_number_of_nodes << endl;

		fprintf(g_pFileOutputLog, "number of nodes =,%d\n", g_assignment.g_number_of_nodes);
		parser.CloseCSVFile();
	}


	cout << "number of zones = " << g_zone_vector.size() << endl;
	fprintf(g_pFileOutputLog, "number of zones =,%d\n", g_zone_vector.size());

	// step 3: read link file 

	CCSVParser parser_link;

	if (parser_link.OpenCSVFile(g_AgentLiteProjectDirectory+"input_link.csv", true))
	{
		while (parser_link.ReadRecord())  // if this line contains [] mark, then we will also read field headers.
		{
			int from_node_id = 0;
			int to_node_id = 0;
			if (parser_link.GetValueByFieldName("from_node_id", from_node_id) == false)
				continue;
			if (parser_link.GetValueByFieldName("to_node_id", to_node_id) == false)
				continue;

			// add the to node id into the outbound (adjacent) node list

			int internal_from_node_seq_no = g_assignment.g_internal_node_seq_no_map[from_node_id];  // map external node number to internal node seq no. 
			int internal_to_node_seq_no = g_assignment.g_internal_node_seq_no_map[to_node_id];

			CLink link;  // create a link object 

			link.from_node_seq_no = internal_from_node_seq_no;
			link.to_node_seq_no = internal_to_node_seq_no;
			link.link_seq_no = g_assignment.g_number_of_links;
			link.to_node_seq_no = internal_to_node_seq_no;

			parser_link.GetValueByFieldName("link_type", link.type);

			string demand_type_code;
			if (!parser_link.GetValueByFieldName("demand_type_code", demand_type_code))
				demand_type_code = "";
			link.demand_type_code = demand_type_code;

			float length = 1.0; // km or mile
			float speed_limit = 1.0;
			parser_link.GetValueByFieldName("length", length);
			parser_link.GetValueByFieldName("speed_limit", speed_limit);

			parser_link.GetValueByFieldName("BPR_alpha_term", link.BRP_alpha);
			parser_link.GetValueByFieldName("BPR_beta_term", link.BRP_beta);
			int number_of_lanes = 1;
			float lane_cap = 1000;
			parser_link.GetValueByFieldName("number_of_lanes", number_of_lanes);
			parser_link.GetValueByFieldName("lane_cap", lane_cap);
			parser_link.GetValueByFieldName("obs_volume", link.m_ObsVolume);
			
			link.m_OutflowNumLanes = number_of_lanes;//visum lane_cap is actually link_cap

			link.link_capacity = lane_cap* number_of_lanes;
			//link.link_capacity = lane_cap;//only for visum

			link.free_flow_travel_time_in_min = length / speed_limit * 60.0;

			link.length = length;
			link.cost = length / speed_limit * 60;
			// min // calculate link cost based length and speed limit // later we should also read link_capacity, calculate delay 


			g_node_vector[internal_from_node_seq_no].m_outgoing_link_vector.push_back(link);  // add this link to the corresponding node as part of outgoing node/link
			g_node_vector[internal_to_node_seq_no].m_incoming_link_seq_no_vector.push_back(link.link_seq_no);  // add this link to the corresponding node as part of outgoing node/link


			g_link_vector.push_back(link);
			//g_pLink_map[link.link_seq_no] = &(g_link_vector[g_link_vector.size() - 1]);  // record the pointer to this link according to its link_seq_no

			g_assignment.g_number_of_links++;

			if (g_assignment.g_number_of_links % 10000 == 0)
				cout << "reading " << g_assignment.g_number_of_links << " links.. " << endl;
		}
	}
	// we now know the number of links
	cout << "number of links = " << g_assignment.g_number_of_links << endl;

	fprintf(g_pFileOutputLog, "number of links =,%d\n", g_assignment.g_number_of_links);

	parser_link.CloseCSVFile();


	//step 4:read demand type file
	CCSVParser parser_demand_type;
	cout << "Step 7.1: Reading file input_demand_type.csv..." << endl;
	//g_LogFile << "Step 7.1: Reading file input_demand_type.csv..." << g_GetUsedMemoryDataInMB() << endl;
	if (!parser_demand_type.OpenCSVFile(g_AgentLiteProjectDirectory+"input_demand_type.csv", true))
	{
		cout << "input_demand_type.csv cannot be opened. " << endl;
		fprintf(g_pFileDebugLog, "input_demand_type.csv cannot be opened. ");
		g_ProgramStop_agentlite();

	}

	if (parser_demand_type.inFile.is_open() || parser_demand_type.OpenCSVFile("input_demand_type.csv", true))
	{
		g_assignment.g_DemandTypeVector.clear();
		while (parser_demand_type.ReadRecord())
		{
			int demand_type = 1;
			float VOT = 10;

			if (parser_demand_type.GetValueByFieldName("demand_type", demand_type) == false)
				break;

			if (demand_type != g_assignment.g_DemandTypeVector.size() + 1)
			{
				cout << "Error: demand_type = " << demand_type << " in file input_demand_type.csv should be a sequential number: " << g_assignment.g_DemandTypeVector.size() + 1 << endl << "Please check!";

				fprintf(g_pFileDebugLog, "Error: demand_type =  %d  in file input_demand_type.csv should be a sequential number:%d Please check! ", demand_type, g_assignment.g_DemandTypeVector.size() + 1);
				g_ProgramStop_agentlite();
			}

			if (demand_type >= _MAX_DEMANDTYPES)
			{
				cout << "Error: demand_type = " << demand_type << " in file input_demand_type.csv is too large. " << "_MAX_DEMANDTYPES = " << _MAX_DEMANDTYPES << "Please contact program developers!";

				g_ProgramStop_agentlite();
			}
			g_assignment.g_DemandTypeVector.push_back(demand_type);
			if (parser_demand_type.GetValueByFieldName("VOT", VOT) == false)
				break;
			parser_link.GetValueByFieldName("VOT", VOT);
			g_assignment.g_VOT_PerDemandType_MAP[demand_type] = VOT;
		}

	}
	else
	{
		cout << "Error: File input_demand_type.csv cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
		fprintf(g_pFileDebugLog, "Error: File input_demand_type.csv cannot be opened.\n It might be currently used and locked by EXCEL.");
		g_ProgramStop_agentlite();
	}

}

void g_ClearOldDemandData(Assignment& g_assignment)
{
	//clear old demand 

	for (int i = 0;i < g_AgentLite_number_of_max_zones;i++)
	{
		for (int j = 0;j < g_AgentLite_number_of_max_zones;j++)
		{
			for (int k = 0;k < _MAX_DEMANDTYPES;k++)
			{
				g_assignment.g_demand_array[i][j][k] = 0.0;
			}
		}
	}
	//g_assignment.total_demand_volume = 0.0;
	for (int i = 0;i <_MAX_DEMANDTYPES;i++)
	{
		g_assignment.total_demand[i] = 0.0;
	}
}

//read new od demand to do reassignment 
void g_ReadNewDemandFile(Assignment& g_assignment)
{

	float total_demand_in_demand_file = 0;


	int max_line_number_for_logging = 10;

	CCSVParser parser;
	cout << "Step 7.1: Reading file input_new_demand_file_list.csv..." << endl;

	if (parser.OpenCSVFile(g_AgentLiteProjectDirectory+"input_new_demand_file_list.csv", true))
	{
		int i = 0;

		while (parser.ReadRecord())
		{

			int file_sequence_no = 1;
			string file_name;
			string format_type = "null";
			int number_of_lines_to_be_skipped = 0;
			int subtotal_in_last_column = 0;

			int number_of_demand_types = 0;
			float local_demand_loading_multiplier = 1;
			char demand_type_field_name[20];
			int demand_type_code[20] = { 0 };

			int demand_format_flag = 0;

			if (parser.GetValueByFieldName("file_sequence_no", file_sequence_no) == false)
				break;

			if (file_sequence_no <= -1)  // skip negative sequence no 
				continue;

			parser.GetValueByFieldName("file_name", file_name);

			if (file_name.length() == 0)  // no file name input
			{
				break;
			}

			parser.GetValueByFieldName("format_type", format_type);
			if (format_type.find("null") != string::npos)  // skip negative sequence no 
			{
				cout << "Please provide format_type in file input_demand_file_list.csv" << endl;
				fprintf(g_pFileDebugLog, "Please provide format_type in file input_demand_file_list.csv");
				g_ProgramStop_agentlite();
			}

			{ // error checking

				if (file_name.find("AMS_OD_table.csv") != string::npos && format_type.find("column") == string::npos)
				{
					cout << "Please specify column format for demand file AMS_OD_table.csv, other than " << format_type << endl;
				}

				if (file_name.find("demand.dat") != string::npos && format_type.find("dynasmart") == string::npos)
				{
					cout << "Please specify dynasmart format for demand file demand.dat, other than " << format_type << endl;
				}

				if (file_name.find("demand_HOV.dat") != string::npos && format_type.find("dynasmart") == string::npos)
				{
					cout << "Please specify dynasmart format for demand file demand_HOV.dat, other than " << format_type << endl;
				}

				if (file_name.find("demand_truck.dat") != string::npos && format_type.find("dynasmart") == string::npos)
				{
					cout << "Please specify dynasmart format for demand_truck file demand.dat, other than " << format_type << endl;
				}
				if (file_name.compare("agent.bin") == 0 && format_type.find("agent_bin") == string::npos)
				{
					cout << "Please specify agent_bin format for agent binary file , other than " << format_type << endl;
				}

			}

			parser.GetValueByFieldName("number_of_lines_to_be_skipped", number_of_lines_to_be_skipped);
			parser.GetValueByFieldName("subtotal_in_last_column", subtotal_in_last_column);
			parser.GetValueByFieldName("loading_multiplier", local_demand_loading_multiplier);

			double total_ratio = 0;

			parser.GetValueByFieldName("number_of_demand_types", number_of_demand_types);

			for (int type = 1; type <= number_of_demand_types; type++)
			{
				sprintf(demand_type_field_name, "demand_type_%d", type);
				int demand_type = -1;
				parser.GetValueByFieldName(demand_type_field_name, demand_type);

				if (demand_type <= 0)
				{
					cout << "Missing input: no value has been specified for field " << demand_type_field_name << " in file " << file_name << " in demand meta file input_demand_file_list.csv. " << endl;
					g_ProgramStop_agentlite();

				}

				demand_type_code[type] = demand_type;
			}



			if (format_type.find("column") != string::npos)  // or muliti-column
			{

				if (number_of_demand_types == 0)
				{
					cout << "number_of_demand_types = 0 in file input_demand_file_list.csv. Please check." << endl;
					fprintf(g_pFileDebugLog, "number_of_demand_types = 0 in file input_demand_file_list.csv. Please check.");
					g_ProgramStop_agentlite();
				}

				bool bFileReady = false;
				int i;

				FILE* st;

				//test # of numerical values per line
				fopen_s(&st, file_name.c_str(), "r");
				if (st != NULL)
				{

					char  str_line[_MAX_STRING_LINE]; // input string
					int str_line_size = _MAX_STRING_LINE;

					// skip lines
					for (int line_skip = 0; line_skip < number_of_lines_to_be_skipped; line_skip++)
					{
						str_line_size = _MAX_STRING_LINE;
						g_read_a_line_agentlite(st, str_line, str_line_size); //  skip the first line
						cout << str_line << endl;
					}

					str_line_size = _MAX_STRING_LINE;
					g_read_a_line_agentlite(st, str_line, str_line_size); //  skip the first line
					int number_of_values = g_read_number_of_numerical_values(str_line, str_line_size);

					fclose(st);
				}

				// read the file formaly after the test. 

				fopen_s(&st, file_name.c_str(), "r");
				if (st != NULL)
				{
					char  str_line[2000]; // input string
					int str_line_size = _MAX_STRING_LINE;

					// skip lines
					for (int line_skip = 0; line_skip < number_of_lines_to_be_skipped; line_skip++)
					{
						str_line_size = _MAX_STRING_LINE;
						g_read_a_line_agentlite(st, str_line, str_line_size); //  skip the first line
					}

					bFileReady = true;
					int line_no = number_of_lines_to_be_skipped;

					while (true)
					{
						int origin_zone = g_read_integer_agentlite(st, true);

						if (origin_zone <= 0)
						{

							if (line_no == 1 && !feof(st))  // read only one line, but has not reached the end of the line
							{
								cout << endl << "Error: Only one line has been read from file. Are there multiple columns of demand type in file " << file_name << " per line?" << endl;
								fprintf(g_pFileDebugLog, "Error: Only one line has been read from file. Are there multiple columns of demand type in file per line?");
								g_ProgramStop_agentlite();

							}
							break;
						}

						if (g_assignment.g_zoneid_to_zone_seq_no_mapping.find(origin_zone) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())
						{
							g_read_float_from_a_line(st);
							continue; // origin zone  has not been defined, skipped. 
						}

						int destination_zone = g_read_integer_agentlite(st, true);

						if (g_assignment.g_zoneid_to_zone_seq_no_mapping.find(destination_zone) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())
						{
							g_read_float_from_a_line(st);
							continue; // origin zone  has not been defined, skipped. 
						}

						int from_zone_seq_no = 0;
						int to_zone_seq_no = 0;
						from_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[origin_zone];
						to_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[destination_zone];

						float number_of_vehicles;

						for (int type = 1; type <= number_of_demand_types; type++)
						{

							float demand_value = g_read_float_from_a_line(st);

							if (demand_value < -99) // encounter return 
							{
								break;
							}

							number_of_vehicles = demand_value*g_assignment.g_DemandGlobalMultiplier*local_demand_loading_multiplier;

							if (demand_type_code[type] >= 1)  // load this demand type
							{

								g_assignment.total_demand[demand_type_code[type]] += number_of_vehicles;
								g_assignment.g_demand_array[from_zone_seq_no][to_zone_seq_no][demand_type_code[type]] += number_of_vehicles;
								//g_assignment.total_demand_volume += number_of_vehicles;

								// we generate vehicles here for each OD data line
								if (line_no <= 5)  // read only one line, but has not reached the end of the line
									cout << "origin:" << origin_zone << ", destination: " << destination_zone << ", value = " << number_of_vehicles << endl;

								if (line_no % 100000 == 0)
								{
									cout << "Reading file no." << file_sequence_no << ": " << file_name << " at " << line_no / 1000 << "K lines..." << endl;
								}


							}
							else if (type != 0)  // if demand type == 0 then we will skip this value. By doing so, we can reading only one demand type per record with demand-type specific departure time loading profile. e.g. truck vs. HOV
							{
								cout << "demand type " << type << " in file input_demand_file_list has not been defined. Please check." << endl;
								
								g_ProgramStop_agentlite();

							}
						}  // for each demand type

						if (subtotal_in_last_column == 1)
							g_read_float_from_a_line(st);  // so skip this last number

						line_no++;
					}  // scan lines


					fclose(st);
					for (int i = 1;i <= number_of_demand_types; i++)
					{
						cout << "total_demand_of" << file_name << "'s demand_type_" << i << " is " << g_assignment.total_demand[i] << " " << endl;
					}
					//cout << "total_demand_volume is " << g_assignment.total_demand_volume << endl;
				}
				else  //open file
				{
					cout << "Error: File " << file_name << " cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
					g_ProgramStop_agentlite();

				}
			}

			else if (format_type.compare("matrix") == 0)
			{

				vector<int> LineIntegerVector;

				CCSVParser parser;
				parser.IsFirstLineHeader = false;
				if (parser.OpenCSVFile(g_AgentLiteProjectDirectory+file_name, true))
				{
					int control_type_code;
					int i = 0;
					if (parser.ReadRecord())
					{
						parser.ConvertLineStringValueToIntegers();
						LineIntegerVector = parser.LineIntegerVector;
					}
				}

				int number_of_zones = LineIntegerVector.size();


				bool bFileReady = false;
				int i;

				FILE* st;
				fopen_s(&st, file_name.c_str(), "r");
				if (st != NULL)
				{
					// read the first line
					g_read_a_line_agentlite(st);

					cout << "number of zones to be read = " << number_of_zones << endl;

					//test if a zone has been defined. 
					for (int destination_zone_index = 0; destination_zone_index < number_of_zones; destination_zone_index++)
					{
						int zone = LineIntegerVector[destination_zone_index];

						if (g_assignment.g_zoneid_to_zone_seq_no_mapping.find(zone) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())
						{
							continue; // origin zone  has not been defined, skipped. 
						}
					}

					int line_no = 0;
					for (int origin_zone_index = 0; origin_zone_index < number_of_zones; origin_zone_index++)
					{
						int origin_zone = g_read_integer_agentlite(st, true); // read the origin zone number

						if (origin_zone <= 0)
							break;

						cout << "Reading file no." << file_sequence_no << " " << file_name << " at zone " << origin_zone << " ... " << endl;

						for (int destination_zone_index = 0; destination_zone_index < number_of_zones; destination_zone_index++)
						{
							int destination_zone = LineIntegerVector[destination_zone_index];
							int from_zone_seq_no = 0;
							int to_zone_seq_no = 0;
							from_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[origin_zone];
							to_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[destination_zone];

							float value = g_read_float_agentlite(st);

							float number_of_vehicles = value*g_assignment.g_DemandGlobalMultiplier*local_demand_loading_multiplier;  // read the value

							if (line_no <= 5)  // read only one line, but has not reached the end of the line
								cout << "origin:" << origin_zone << ", destination: " << destination_zone << ", value = " << number_of_vehicles << endl;

							line_no++;
							int type = 1;  // first demand type definition
							if (demand_type_code[type] >= 1)  // feasible demand type
							{
								g_assignment.total_demand[demand_type_code[type]] += number_of_vehicles;
								g_assignment.g_demand_array[from_zone_seq_no][to_zone_seq_no][demand_type_code[type]] += number_of_vehicles;
								//g_assignment.total_demand_volume += number_of_vehicles;
							}

						}
						//
						if (subtotal_in_last_column == 1)
							g_read_float_agentlite(st); //read sub total value

					}
					fclose(st);
					for (int i = 1;i <= number_of_demand_types; i++)
					{
						cout << "total_demand_of" << file_name << "'s demand_type_" << i << " is " << g_assignment.total_demand[i] << " " << endl;
					}
					//cout << "total_demand_volume is " << g_assignment.total_demand_volume << endl;
				}
				else  //open file
				{
					cout << "Error: File " << file_name << " cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
					g_ProgramStop_agentlite();

				}
			}
			else if (format_type.compare("full_matrix") == 0)
			{
				vector<int> LineIntegerVector;

				CCSVParser parser;
				parser.IsFirstLineHeader = false;
				if (parser.OpenCSVFile(g_AgentLiteProjectDirectory+file_name, true))
				{
					int control_type_code;
					int i = 0;
					if (parser.ReadRecord())
					{
						parser.ConvertLineStringValueToIntegers();
						LineIntegerVector = parser.LineIntegerVector;
					}

				}

				int number_of_zones = g_zone_vector.size();


				bool bFileReady = false;
				int i;

				FILE* st;
				fopen_s(&st, file_name.c_str(), "r");
				if (st != NULL)
				{
					// read the first line
					g_read_a_line_agentlite(st);

					cout << "number of zones to be read = " << number_of_zones << endl;

					int line_no = 0;


					for (int origin_zone_index = 0; origin_zone_index < number_of_zones; origin_zone_index++)
					{
						int origin_zone = g_read_integer_agentlite(st, true); // read the origin zone number

						if (origin_zone == -1)
							break;

						if (g_assignment.g_zoneid_to_zone_seq_no_mapping.find(origin_zone) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end())
						{
							bool a = (g_assignment.g_zoneid_to_zone_seq_no_mapping.find(origin_zone) == g_assignment.g_zoneid_to_zone_seq_no_mapping.end());
							continue; // origin zone  has not been defined, skipped. 
						}

						if (origin_zone % 100 == 0)
							cout << "Reading file no." << file_sequence_no << ": " << file_name << " at zone " << origin_zone << ",total demand = " << total_demand_in_demand_file << "..." << endl;

						for (int destination_zone_index = 0; destination_zone_index < number_of_zones; destination_zone_index++)
						{
							int destination_zone = LineIntegerVector[destination_zone_index];
							float value = g_read_float_agentlite(st);
							int from_zone_seq_no = 0;
							int to_zone_seq_no = 0;
							from_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[origin_zone];
							to_zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[destination_zone];

							float number_of_vehicles = value*g_assignment.g_DemandGlobalMultiplier*local_demand_loading_multiplier;  // read the value

							if (line_no <= 5)  // read only one line, but has not reached the end of the line
								cout << "origin:" << origin_zone << ", destination: " << destination_zone << ", value = " << number_of_vehicles << endl;

							line_no++;
							int type = 1;  // first demand type definition
							if (demand_type_code[type] >= 1)  // feasible demand type
							{
								g_assignment.total_demand[demand_type_code[type]] += number_of_vehicles;
								g_assignment.g_demand_array[from_zone_seq_no][to_zone_seq_no][demand_type_code[type]] += number_of_vehicles;
								//g_assignment.total_demand_volume += number_of_vehicles;
							}

						}
						//
						if (subtotal_in_last_column == 1)
							g_read_float_agentlite(st); //read sub total value

					}

					fclose(st);
					for (int i = 1;i <= number_of_demand_types; i++)
					{
						cout << "total_demand_of" << file_name << "'s demand_type_" << i << " is " << g_assignment.total_demand[i] << " " << endl;
					}
					//cout << "total_demand_volume is " << g_assignment.total_demand_volume << endl;

				}
				else  //open file
				{
					cout << "Error: File " << file_name << " cannot be opened.\n It might be currently used and locked by EXCEL." << endl;
					g_ProgramStop_agentlite();

				}


			}
			else
			{
				cout << "Error: format_type = " << format_type << " is not supported. Currently DTALite supports multi_column, matrix, full_matrix, dynasmart, agent_csv, agent_bin, trip_csv,transims_trip_file." << endl;
				g_ProgramStop_agentlite();

			}
		}
	}

}


//tree_based reassignment method
void g_perform_tree_based_reassignment(int reassignment_iteration_number, int b_debug_detail_flag, Assignment& g_assignment)
{
	// given precalculated k SP tree
	// to find the link volume and link cost
	// to do: perfor multiple UE or likely iterations 

#pragma omp parallel for  
	// step 1: C++ open mp automatically create n threads., each thread has its own computing thread on a cpu core 
	//
	for (int OZone = 0; OZone < g_zone_vector.size(); OZone++)
	{
		g_zone_vector[OZone].calculate_reassignment_link_flow(reassignment_iteration_number, b_debug_detail_flag, g_assignment); //changed by zhuge

	}

#pragma omp parallel for  // step 2 collect all partial link volume to compute link volume across all zones
	for (int l = 0; l < g_link_vector.size(); l++)
	{
		g_link_vector[l].tally_flow_volume_across_all_processors();
		g_link_vector[l].CalculateBPRFunction();

	}
}

void COZone::calculate_reassignment_link_flow(int reassignment_iteration_number, int b_debug_detail_flag, Assignment& g_assignment)
{
	reset_link_volume(g_link_vector.size());

	for (int d = 0; d < g_zone_vector.size(); d++)
	{
		for (int tau = 0; tau < _MAX_TIMEPERIODS; tau++)
		{
			for (int i = 0; i < g_assignment.g_DemandTypeVector.size(); i++)
			{
				if (d == this->zone_seq_no) // the same zone
					continue;  //skip

				int destination_node_no = g_zone_vector[d].node_seq_no;

				// at each zone, m_memory_index_vector is equal to the k of k shortest path tree

				for (int s = 0; s <= reassignment_iteration_number; s++)
				{
					if (s <= g_assignment.g_number_of_K_paths - 1)
					{
						destination_k_path_prob[d] = float(1 - float(0.05*(reassignment_iteration_number - g_assignment.g_number_of_K_paths + 1))) / float(g_assignment.g_number_of_K_paths);//each tree takes the average volume
					}
					else
					{
						destination_k_path_prob[d] = 0.05;
					}
					destination_k_path_prob_forreassignment_ODVolume_experienced_cost[d] = float(1 - float(0.05*(reassignment_iteration_number - g_assignment.g_number_of_K_paths))) / float(g_assignment.g_number_of_K_paths);
					vector<int> temp_path_node_vector; //node seq vector for each ODK
					if (b_debug_detail_flag)
					{
						//push back D,K,demand_type,S into the vector to record
						temp_path_node_vector.push_back(d);
						temp_path_node_vector.push_back(reassignment_iteration_number);
						temp_path_node_vector.push_back(i);
						temp_path_node_vector.push_back(s);

					}

					int memory_block_index = m_memory_index_vector[g_assignment.g_DemandTypeVector.size()*s*_MAX_TIMEPERIODS + i*_MAX_TIMEPERIODS + tau];

					destination_k_path_cost[d][i][s][tau] = 0.0;  //calculate path cost for ODMOE
					destination_k_path_distance[d][i][s][tau] = 0.0;
					int current_node_seq_no = destination_node_no;
					int current_link_seq_no = -1;

					// back trace the sp tree from the destination to the root (at origin) 
					while (current_node_seq_no >= 0 && current_node_seq_no < g_assignment.g_number_of_nodes)
					{
						if (current_node_seq_no >= 0 && current_node_seq_no < g_assignment.g_number_of_nodes)  // this is valid node 
						{
							if (b_debug_detail_flag)
							{
								//push back element into the path node seq vector
								temp_path_node_vector.push_back(g_node_vector[current_node_seq_no].node_id);
							}

							current_link_seq_no = g_pNetworkForSP[memory_block_index].m_link_predecessor_pertype[tau][i][current_node_seq_no];

							// fetch m_link_predecessor to mark the link volume

							if (current_link_seq_no >= 0 && current_link_seq_no < g_assignment.g_number_of_links)
							{
								link_volume[current_link_seq_no] += (destination_k_path_prob[d] * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau]);
								destination_k_path_cost[d][i][s][tau] += g_link_vector[current_link_seq_no].cost + g_link_vector[current_link_seq_no].TollMAP[g_assignment.g_DemandTypeVector[i]];
								destination_k_path_distance[d][i][s][tau] += g_link_vector[current_link_seq_no].length;
							}
						}
						current_node_seq_no = g_pNetworkForSP[memory_block_index].m_node_predecessor_pertype[tau][i][current_node_seq_no];  // update node seq no		
					}

					//calculating the experienced cost for trees of reassignment_iteration_number 
					if (s != reassignment_iteration_number)
					{
						if (s <= g_assignment.g_number_of_K_paths - 1)
						{
							destination_k_path_ODVolume_experienced_cost[d][reassignment_iteration_number][i][tau] += destination_k_path_prob_forreassignment_ODVolume_experienced_cost[d] * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau] * destination_k_path_cost[d][i][s][tau];
							destination_k_path_ODVolume_tree_distance[d][reassignment_iteration_number][i][tau] += destination_k_path_prob_forreassignment_ODVolume_experienced_cost[d] * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau] * destination_k_path_distance[d][i][s][tau];
						}
						else
						{
							destination_k_path_ODVolume_experienced_cost[d][reassignment_iteration_number][i][tau] += 0.05 * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau] * destination_k_path_cost[d][i][s][tau];
							destination_k_path_ODVolume_tree_distance[d][reassignment_iteration_number][i][tau] += 0.05 * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau] * destination_k_path_distance[d][i][s][tau];
						}

					}

					if (b_debug_detail_flag)
					{
						//push back the temp path node vector to global path node vector for each origin
						m_path_node_vector.push_back(temp_path_node_vector);
						temp_path_node_vector.clear();
					}
				}
				//calculating the pi cost for trees of reassignment_iteration_number 
				destination_k_path_pi_ODVolume_cost[d][reassignment_iteration_number][i][tau] = destination_k_path_cost[d][i][reassignment_iteration_number][tau] * destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]][tau];

				//calculating the total pi_cost for the relative_gap calculating
				if (destination_OD_volume[d][g_assignment.g_DemandTypeVector[i]] == 0)
				{
					pie_used_in_relative_gap_function[d][reassignment_iteration_number][i][tau] = 0.0;
				}
				else
				{
					pie_used_in_relative_gap_function[d][reassignment_iteration_number][i][tau] = destination_k_path_cost[d][i][reassignment_iteration_number][tau];
				}

			}
		}
	}
}


int get_memory_index_no(int zone_seq_no, int k, int demand_type_no, int time_period_no, Assignment& g_assignment)
{
	int memory_index_no = k * g_AgentLite_number_of_max_zones *_MAX_TIMEPERIODS * g_assignment.g_DemandTypeVector.size() + zone_seq_no *_MAX_TIMEPERIODS* g_assignment.g_DemandTypeVector.size() + demand_type_no * _MAX_TIMEPERIODS + time_period_no;  //devide the agent number by total number of threads 
	return memory_index_no;
}

void g_assign_computing_tasks_to_each_memory_blocks(int number_of_memory_blocks, Assignment& g_assignment)
{
	// step 1: beginning of allocate memory
	cout << "beginning of allocate memory for " << number_of_memory_blocks << " copies of NetworkForSP" << endl;

	g_pNetworkForSP = new NetworkForSP[number_of_memory_blocks]; // create n copies of network, each for a subset of agents to use 


	for (int i = 0; i < number_of_memory_blocks; i++)
	{
		cout << "allocate memory for " << i << "th NetworkForSP " << endl;
		g_pNetworkForSP[i].m_threadNo = i;   // each thread/network has its own thread number. // each network has its own label cost vector for shortest path calculation
		g_pNetworkForSP[i].AllocateMemory(g_assignment.g_DemandTypeVector.size(), g_assignment.g_number_of_nodes, g_assignment.g_number_of_links, _MAX_TIMEPERIODS);

	}
	cout << "end of allocate memory for NetworkForSP" << endl;

	// step 2: assign node to thread
	for (int k = 0; k < g_assignment.g_number_of_K_paths + g_assignment.g_reassignment_number_of_K_paths; k++)
	{
		for (int i = 0; i < g_zone_vector.size(); i++)  //assign all nodes to the corresponding thread
		{
			for (int j = 0; j < g_assignment.g_DemandTypeVector.size(); j++)
			{
				for (int t = 0; t < _MAX_TIMEPERIODS; t++)
				{
					int zone_seq_no = g_zone_vector[i].zone_seq_no;
					int memory_index = get_memory_index_no(zone_seq_no, k, j, t, g_assignment);  //devide the node number by total number of threads 
					g_pNetworkForSP[memory_index].m_node_demandtype_vector.push_back(i);												  // linking zone vector and K SP trees
					g_zone_vector[zone_seq_no].m_memory_index_vector.push_back(memory_index);
					g_pNetworkForSP[memory_index].m_node_demandtype_vector.push_back(j);
					g_pNetworkForSP[memory_index].m_node_demandtype_vector.push_back(t);
				}
			}
		}
	}
}



void g_setup_link_cost_in_each_memory_block(int iteration_number, Assignment& g_assignment)
{
	float cost = 0;
	// step 2: assign node to thread

	for (int i = 0; i < g_node_vector.size(); i++)  //assign all nodes to the corresponding thread
	{
		if (g_node_vector[i].zone_id >= 1)
		{
			for (int j = 0; j < g_assignment.g_DemandTypeVector.size(); j++)
			{
				for (int t = 0; t < _MAX_TIMEPERIODS; t++)
				{
					int zone_seq_no = g_assignment.g_zoneid_to_zone_seq_no_mapping[g_node_vector[i].zone_id];
					int memory_block_index = get_memory_index_no(zone_seq_no, iteration_number, j, t, g_assignment);  //devide the node number by total number of threads 

																													// linking zone vector and K SP trees
					for (int l = 0; l < g_assignment.g_number_of_links; l++)
					{

						if (iteration_number == 0)
						{
							cost = g_link_vector[l].TollMAP[g_assignment.g_DemandTypeVector[j]] + g_link_vector[l].free_flow_travel_time_in_min*g_assignment.g_VOT_PerDemandType_MAP[g_assignment.g_DemandTypeVector[j]];
						}
						else
						{
							cost = g_link_vector[l].cost*g_assignment.g_VOT_PerDemandType_MAP[g_assignment.g_DemandTypeVector[j]] + g_link_vector[l].TollMAP[g_assignment.g_DemandTypeVector[j]];
						}
						g_pNetworkForSP[memory_block_index].m_link_cost_array_pertype[t][j][l] = cost;

					}
				}
			}
		}
	}
}



void g_calculate_K_SPTree(int iteration_number, Assignment & g_assignment)
{
	// calculate accessibility 
#pragma omp parallel for  // step 3: C++ open mp automatically create n threads., each thread has its own computing thread on a cpu core 
	for (int ProcessID = iteration_number*g_AgentLite_number_of_max_zones*g_assignment.g_DemandTypeVector.size()*_MAX_TIMEPERIODS; ProcessID < (iteration_number + 1)*g_AgentLite_number_of_max_zones*g_assignment.g_DemandTypeVector.size()*_MAX_TIMEPERIODS; ProcessID++) //changed by zhuge
	{
		//NetworkForSP network;
		g_pNetworkForSP[ProcessID].optimal_label_correcting_for_all_nodes_assigned_to_processor(g_assignment);
	}
}


void g_output_simulation_result(int number_of_threads, int b_debug_detail_flag, Assignment& g_assignment)
{
	//output the GAP and relative gap value for each iteration
	FILE* g_pFileGAPMOE = NULL;
	g_pFileGAPMOE = fopen("output_GAPMOE.csv", "w");
	if (g_pFileGAPMOE == NULL)
	{
		cout << "File output_GAPMOE.csv cannot be opened." << endl;
		g_ProgramStop_agentlite();
	}
	else
	{
		fprintf(g_pFileGAPMOE, "k,od_gap,od_gap_relative,avg_cost,avg_distance\n");
		for (int l = 0;l < g_assignment.g_number_of_K_paths + g_assignment.g_reassignment_number_of_K_paths;l++)
		{
			for (int i = 0; i < g_zone_vector.size(); i++) //Initialization for all nodes
			{
				for (int j = 0; j < g_zone_vector.size(); j++)
				{
					for (int tau = 0; tau < _MAX_TIMEPERIODS; tau++)
					{
						for (int k = 0; k < g_assignment.g_DemandTypeVector.size(); k++)
						{
							COZone* pZone1 = &(g_zone_vector[i]);
							totaltype_destination_k_path_pi_ODVolume_cost[i][j][l] += pZone1->destination_k_path_pi_ODVolume_cost[j][l][k][tau];
							totaltype_destination_k_path_ODVolume_experienced_cost[i][j][l] += pZone1->destination_k_path_ODVolume_experienced_cost[j][l][k][tau];
							totaltype_destination_k_path_ODVolume_tree_cost[i][j][l] += pZone1->destination_k_path_ODVolume_experienced_cost[j][l][k][tau];
							totaltype_destination_k_path_ODVolume_tree_distance[i][j][l] += pZone1->destination_k_path_ODVolume_tree_distance[j][l][k][tau];
							if (i == j)
							{
								pZone1->pie_used_in_relative_gap_function[j][l][k][tau] = 0;
							}
							totaltype_relative_gap_pi[i][j][l] += pZone1->pie_used_in_relative_gap_function[j][l][k][tau];
						}
						totaltype_totaldestination_pi_cost[i][l] += totaltype_destination_k_path_pi_ODVolume_cost[i][j][l];
						totaltype_totaldestination_experienced_cost[i][l] += totaltype_destination_k_path_ODVolume_experienced_cost[i][j][l];
						totaltype_totaldestination_tree_cost[i][l] += totaltype_destination_k_path_ODVolume_tree_cost[i][j][l];
						totaltype_totaldestination_tree_distance[i][l] += totaltype_destination_k_path_ODVolume_tree_distance[i][j][l];
						totaltype_totaldestination_relative_gap_pi[i][l] += totaltype_relative_gap_pi[i][j][l];
					}
				}
				total_pi_cost[l] += totaltype_totaldestination_pi_cost[i][l];
				total_experienced_cost[l] += totaltype_totaldestination_experienced_cost[i][l];
				total_tree_cost[l] += totaltype_totaldestination_tree_cost[i][l];
				total_tree_distance[l] += totaltype_totaldestination_tree_distance[i][l];
				total_relative_gap_pi_cost_donot_consider_volume[l] += totaltype_totaldestination_relative_gap_pi[i][l];

			}
			if (l == 0) //when iteration_number=0,set all the gap to 0.
			{
				_gap_[l] = 0.0;
				_gap_relative_[l] = 0.0;
			}
			else
			{
				_gap_[l] = (total_experienced_cost[l] - total_pi_cost[l]) / g_assignment.total_demand_volume;
				_gap_relative_[l] = _gap_[l] / total_relative_gap_pi_cost_donot_consider_volume[l];
			}
			total_tree_cost[l] = total_tree_cost[l] / g_assignment.total_demand_volume;
			total_tree_distance[l] = total_tree_distance[l] / g_assignment.total_demand_volume;

			fprintf(g_pFileGAPMOE, "%d,%.3f,%.6f,%.3f,%.6f\n",
				l,
				_gap_[l],
				_gap_relative_[l],
				total_tree_cost[l],
				total_tree_distance[l]);
		}
		fclose(g_pFileGAPMOE);
	}



	FILE* g_pFileLinkMOE = NULL;
	g_pFileLinkMOE = fopen("output_LinkMOE.csv", "w");
	if (g_pFileLinkMOE == NULL)
	{
		cout << "File output_LinkMOE.csv cannot be opened." << endl;
		g_ProgramStop_agentlite();
	}
	else
	{
		fprintf(g_pFileLinkMOE, "from_node_id,to_node_id,total_link_volume,volume_over_capacity_ratio,speed,travel_time\n");

		for (int l = 0; l < g_link_vector.size(); l++) //Initialization for all nodes
		{
			CLink* pLink = &(g_link_vector[l]);

			fprintf(g_pFileLinkMOE, "%d,%d,%.3f,%.3f,%.3f,%.3f\n",
				g_node_vector[pLink->from_node_seq_no].node_id,
				g_node_vector[pLink->to_node_seq_no].node_id,
				pLink->flow_volume,
				pLink->get_VOC_ratio(),
				pLink->get_speed(),
				pLink->travel_time);

		}
		fclose(g_pFileLinkMOE);
	}
}

void g_set_demand_volume_withalphagamma(Assignment & g_assignment, float Alpha)
{
	Demand_Parameters alphagamma;

	g_assignment.total_demand_volume = 0.0;
	for (int o = 0; o < g_zone_vector.size(); o++)
	{
		g_zone_vector[o].setup_link_volume(g_link_vector.size());
		g_zone_vector[o].reset_od_volume_with_alphagamma(g_zone_vector[o].zone_seq_no, g_assignment, Alpha, alphagamma);  // to do, we use default values now, we should read from OD tables. 

	}
}
void g_get_link_volume(Assignment& g_assignment)
{
	g_link_volume_vector.clear();
	g_link_speed_vector.clear();

	for (int l = 0; l < g_link_vector.size(); l++) //Initialization for all nodes
	{
		float volume = g_link_vector[l].flow_volume;
		g_link_volume_vector.push_back(volume);

		g_link_vector[l].CalculateBPRFunction();
		float speed = g_link_vector[l].get_speed();
		g_link_speed_vector.push_back(speed);
	}

}

bool AgentLite_init_flag = false;

Assignment g_assignment;


void Assignment::Assignment_Init()
	{

		g_demand_array = Allocate3DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones, _MAX_DEMANDTYPES);

		for (int i = 0; i < g_AgentLite_number_of_max_zones; i++)
		{
			for (int j = 0; j < g_AgentLite_number_of_max_zones; j++)
			{
				for (int d = 0; d < _MAX_DEMANDTYPES; d++)
				{
					g_demand_array[i][j][d] = { 0.0 };
				}
			}
		}
		total_demand_volume = 0.0;
		for (int i = 0; i < _MAX_DEMANDTYPES; i++)
		{
			total_demand[i] = { 0.0 };
		}

	}

void COZone::COZone_Init()
{
	m_TotalObsError = 0;
	destination_k_path_prob_forreassignment_ODVolume_experienced_cost = new float[g_AgentLite_number_of_max_zones];
	destination_k_path_prob = new float[g_AgentLite_number_of_max_zones];

	for (int i = 0; i < g_AgentLite_number_of_max_zones; i++)
	{
		destination_k_path_prob_forreassignment_ODVolume_experienced_cost[i] = 0;
		destination_k_path_prob[i] = 0;
	}

	destination_OD_volume = Allocate3DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, _MAX_DEMANDTYPES, _MAX_TIMEPERIODS);

	for (int zi = 0; zi < g_AgentLite_number_of_max_zones; zi++)
		for (int di = 0; di < _MAX_DEMANDTYPES; di++)
				for (int t = 0; t < _MAX_TIMEPERIODS; t++)
				{
					destination_OD_volume[zi][di][t] = 0.0;
				}

	destination_k_path_cost = Allocate4DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, _MAX_DEMANDTYPES, g_AgentLite_number_of_k_path, _MAX_TIMEPERIODS);
	destination_k_path_distance = Allocate4DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, _MAX_DEMANDTYPES, g_AgentLite_number_of_k_path, _MAX_TIMEPERIODS);
	destination_k_path_ODVolume_experienced_cost = Allocate4DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path, _MAX_DEMANDTYPES, _MAX_TIMEPERIODS);
	destination_k_path_ODVolume_tree_distance = Allocate4DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path, _MAX_DEMANDTYPES, _MAX_TIMEPERIODS);
	destination_k_path_pi_ODVolume_cost = Allocate4DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path, _MAX_DEMANDTYPES, _MAX_TIMEPERIODS);
	pie_used_in_relative_gap_function = Allocate4DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path, _MAX_DEMANDTYPES, _MAX_TIMEPERIODS);


	for (int zi = 0; zi < g_AgentLite_number_of_max_zones; zi++)
		for (int di = 0; di < _MAX_DEMANDTYPES; di++)
			for (int k = 0; k < g_AgentLite_number_of_k_path; k++)
				for (int t = 0; t < _MAX_TIMEPERIODS; t++)
				{
				destination_k_path_cost[zi][di][k][t] = 0.0;
				destination_k_path_distance[zi][di][k][t] = 0.0;
				destination_k_path_ODVolume_experienced_cost[zi][k][di][t] = 0.0;
				destination_k_path_ODVolume_tree_distance[zi][k][di][t] = 0.0;
				destination_k_path_pi_ODVolume_cost[zi][k][di][t] = 0.0;
				pie_used_in_relative_gap_function[zi][k][di][t] = 0.0;


				}
}

COZone::~COZone()
{
	//delete functions
}
void AgentLite_Initialization()
{
	if (AgentLite_init_flag == false)
	{
		g_pFileDebugLog = fopen("Debug1zhuge.txt", "w");
		if (g_pFileDebugLog == NULL)
		{
			cout << "File Debug.txt cannot be opened." << endl;
			g_ProgramStop_agentlite();
		}
		g_pFileOutputLog = fopen("output_solution.csv", "w");
		if (g_pFileOutputLog == NULL)
		{
			cout << "File output_solution.csv cannot be opened." << endl;
			g_ProgramStop_agentlite();
		}


		g_ReadInputData(g_assignment);
		
		for (int OZone = 0; OZone < g_zone_vector.size(); OZone++)
		{
			g_zone_vector[OZone].COZone_Init();
		}


		g_assignment.Assignment_Init();

		totaltype_destination_k_path_ODVolume_experienced_cost = Allocate3DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		totaltype_destination_k_path_ODVolume_tree_cost = Allocate3DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		totaltype_destination_k_path_ODVolume_tree_distance = Allocate3DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		totaltype_relative_gap_pi = Allocate3DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		totaltype_destination_k_path_pi_ODVolume_cost = Allocate3DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		total_od_cost = Allocate3DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);

		for (int zi = 0; zi < g_AgentLite_number_of_max_zones; zi++)
			for (int zj = 0; zj < g_AgentLite_number_of_max_zones; zj++)
				for (int k = 0; k < g_AgentLite_number_of_k_path; k++)
				{
					totaltype_destination_k_path_ODVolume_experienced_cost[zi][zj][k] = 0.0;
					totaltype_destination_k_path_ODVolume_tree_cost[zi][zj][k] = 0.0;
					totaltype_destination_k_path_ODVolume_tree_distance[zi][zj][k] = 0.0;
					totaltype_relative_gap_pi[zi][zj][k] = 0.0;
					totaltype_destination_k_path_pi_ODVolume_cost[zi][zj][k] = 0.0;
					total_od_cost[zi][zj][k] = 0.0;

				}



		totaltype_totaldestination_tree_cost = Allocate2DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		totaltype_totaldestination_tree_distance = Allocate2DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		totaltype_totaldestination_pi_cost = Allocate2DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		totaltype_totaldestination_experienced_cost = Allocate2DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		totaltype_totaldestination_relative_gap_pi = Allocate2DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		totaltype_od_cost = Allocate2DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);
		avg_od_cost_totaldestination = Allocate2DDynamicArray_Al<float>(g_AgentLite_number_of_max_zones, g_AgentLite_number_of_k_path);

		for (int zi = 0; zi < g_AgentLite_number_of_max_zones; zi++)
				for (int k = 0; k < g_AgentLite_number_of_k_path; k++)
				{
					totaltype_totaldestination_tree_cost[zi][k] = 0.0;
					totaltype_totaldestination_tree_distance[zi][k] = 0.0;
					totaltype_totaldestination_pi_cost[zi][k] = 0.0;
					totaltype_totaldestination_experienced_cost[zi][k] = 0.0;
					totaltype_totaldestination_relative_gap_pi[zi][k] = 0.0;
					totaltype_od_cost[zi][k] = 0.0;
					avg_od_cost_totaldestination[zi][k] = 0.0;

				}

		total_tree_cost = new float[g_AgentLite_number_of_k_path];
		total_tree_distance = new float[g_AgentLite_number_of_k_path];
		total_pi_cost = new float[g_AgentLite_number_of_k_path];
		total_experienced_cost = new float[g_AgentLite_number_of_k_path];
		total_relative_gap_pi_cost_donot_consider_volume = new float[g_AgentLite_number_of_k_path];
		total_od_cost_totalorigin = new float[g_AgentLite_number_of_k_path];
		avg_od_cost_totalorigin = new float[g_AgentLite_number_of_k_path];
		_gap_ = new float[g_AgentLite_number_of_k_path];
		_gap_relative_ = new float[g_AgentLite_number_of_k_path];

		for (int zi = 0; zi < g_AgentLite_number_of_max_zones; zi++)
			{
			    total_tree_cost[zi] = 0.0;
				total_tree_distance[zi] = 0.0;
				total_pi_cost[zi] = 0.0;
				total_experienced_cost[zi] = 0.0;
				total_relative_gap_pi_cost_donot_consider_volume[zi] = 0.0;
				total_od_cost_totalorigin[zi] = 0.0;
				avg_od_cost_totalorigin[zi] = 0.0;
				_gap_[zi] = 0.0;
				_gap_relative_[zi] = 0.0;

			}

		g_ReadDemandFileBasedOnDemandFileList(g_assignment);
		ReadLinkTollScenarioFile(g_assignment);
		g_set_demand_volume_withalphagamma(g_assignment, 1.0);
		cout << "total_demand_volume is " << g_assignment.total_demand_volume << endl;
		fprintf(g_pFileDebugLog, "total_demand_volume is %.3f\n", g_assignment.total_demand_volume);
		int i;

		//CPU computation processor thread p scan through the memory blocks, # of CPU threads is 8 or determined by openMP.
		int number_of_threads = max(1, g_number_of_CPU_threads());
		number_of_threads = min(number_of_threads, g_assignment.g_number_of_threads);
		fprintf(g_pFileDebugLog, "input threads = %d\n", g_assignment.g_number_of_threads);
		fprintf(g_pFileDebugLog, "CPU threads = %d\n", number_of_threads);

		// the number of memory blocks is # of Ozones * of demand type * of iteration number 
		int number_of_memory_blocks = g_AgentLite_number_of_max_zones *_MAX_TIMEPERIODS*(g_assignment.g_number_of_K_paths + g_assignment.g_reassignment_number_of_K_paths)*g_assignment.g_DemandTypeVector.size();

		//step 2: allocate memory and assign computing tasks
		g_assign_computing_tasks_to_each_memory_blocks(number_of_memory_blocks, g_assignment);

		AgentLite_init_flag = true;
		g_AppStartTime = CTime::GetCurrentTime();

		//g_set_demand_volume_withalphagamma(g_assignment, Alpha);
		clock_t start_t, end_t, total_t;
		// step 1: read input data of network / demand tables / Toll

		for (int iteration_number = 0; iteration_number < g_assignment.g_number_of_K_paths; iteration_number++)
		{

			//step 3: compute K SP tree
			start_t = clock();
			g_setup_link_cost_in_each_memory_block(iteration_number, g_assignment);
			g_calculate_K_SPTree(iteration_number, g_assignment);
			end_t = clock();
			total_t = (end_t - start_t);
			cout << "CPU Running Time for SP and loading path = " << total_t << " milliseconds" << " " << "k=" << iteration_number << endl;
			fprintf(g_pFileDebugLog, "CPU Running Time for SP and loading path  = %ld milliseconds\n", total_t);

			//step 4: tree based g_assignment 
			start_t = clock();
			g_perform_tree_based_assignment(iteration_number, g_assignment.b_debug_detail_flag, g_assignment);
			end_t = clock();
			total_t = (end_t - start_t);
		}

		g_get_link_volume(g_assignment);
		cout << "CPU Running Time for base_assignment= " << total_t << " milliseconds" << endl;
		fprintf(g_pFileDebugLog, "CPU Running Time for base_assignment=  ,%ld, milliseconds\n", total_t);
	}

}

void AgentLite_End()
{
	//step 5: output simulation results
	//g_output_simulation_result(number_of_threads,g_assignment.b_debug_detail_flag, g_assignment);


	Deallocate3DDynamicArray_Al(totaltype_destination_k_path_ODVolume_experienced_cost, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones);
	Deallocate3DDynamicArray_Al(totaltype_destination_k_path_ODVolume_tree_cost, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones);
	Deallocate3DDynamicArray_Al(totaltype_destination_k_path_ODVolume_tree_distance, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones);
	Deallocate3DDynamicArray_Al(totaltype_relative_gap_pi, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones);
	Deallocate3DDynamicArray_Al(totaltype_destination_k_path_pi_ODVolume_cost, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones);
	Deallocate3DDynamicArray_Al(total_od_cost, g_AgentLite_number_of_max_zones, g_AgentLite_number_of_max_zones);

	Deallocate2DDynamicArray_Al(totaltype_totaldestination_tree_distance, g_AgentLite_number_of_max_zones);
	Deallocate2DDynamicArray_Al(totaltype_totaldestination_pi_cost, g_AgentLite_number_of_max_zones);
	Deallocate2DDynamicArray_Al(totaltype_totaldestination_experienced_cost, g_AgentLite_number_of_max_zones);
	Deallocate2DDynamicArray_Al(totaltype_totaldestination_relative_gap_pi, g_AgentLite_number_of_max_zones);
	Deallocate2DDynamicArray_Al(totaltype_od_cost, g_AgentLite_number_of_max_zones);
	Deallocate2DDynamicArray_Al(avg_od_cost_totaldestination, g_AgentLite_number_of_max_zones);

	delete[] total_tree_cost;
	delete[] total_tree_distance;
	delete[] total_pi_cost;
	delete[] total_experienced_cost;
	delete[] total_relative_gap_pi_cost_donot_consider_volume;
	delete[] total_od_cost_totalorigin;
	delete[] avg_od_cost_totalorigin;
	delete[] _gap_;
	delete[] _gap_relative_;

fclose(g_pFileOutputLog);
fclose(g_pFileDebugLog);

cout << "End of Optimization " << endl;
cout << "free memory.." << endl;
cout << "done." << endl;
delete[] g_pNetworkForSP;

g_node_vector.clear();

for (int l = 0; l < g_link_vector.size(); l++)
{
	g_link_vector[l].free_memory();
}
g_link_vector.clear();


}
int AgentLite_main(float Alpha = 1.0)
{
	// definte timestamps
	

	AgentLite_Initialization();
	
	g_set_demand_volume_withalphagamma(g_assignment, Alpha);
	//cout << "total_demand_volume is " << assignment.total_demand_volume << endl;
	fprintf(g_pFileDebugLog, "reassignment new total_demand_volume is %.3f\n", g_assignment.total_demand_volume);
	//step 6-2 reassgnment the demand on the trees found by the assignment step
	clock_t start_t, end_t, total_t;
	g_perform_tree_based_assignment(g_assignment.g_number_of_K_paths - 1, g_assignment.b_debug_detail_flag, g_assignment);
	for (int reassignment_iteration_number = g_assignment.g_number_of_K_paths; reassignment_iteration_number < g_assignment.g_number_of_K_paths + g_assignment.g_reassignment_number_of_K_paths; reassignment_iteration_number++)
	{

		//step 6-3: compute K SP tree for new demand and g_assignment_number_of_K_paths
		start_t = clock();

		g_setup_link_cost_in_each_memory_block(reassignment_iteration_number, g_assignment);
		g_calculate_K_SPTree(reassignment_iteration_number, g_assignment);

		end_t = clock();
		total_t = (end_t - start_t);
		cout << "CPU Running Time for SP and loading path = " << total_t << " milliseconds" << " " << "k=" << reassignment_iteration_number << endl;
		fprintf(g_pFileDebugLog, "CPU Running Time for SP and loading path  = %ld milliseconds\n", total_t);

		//step 6-4: tree based assignment of new demand
		start_t = clock();

		g_perform_tree_based_reassignment(reassignment_iteration_number, g_assignment.b_debug_detail_flag, g_assignment);

		end_t = clock();
		total_t = (end_t - start_t);
	}
	g_get_link_volume(g_assignment);
	//step 6-5: output simulation results of the new demand 
	//g_output_simulation_result(number_of_threads, assignment.b_debug_detail_flag, assignment);

	cout << "CPU Running Time for Reassignment= " << total_t << " milliseconds" << endl;
	fprintf(g_pFileDebugLog, "CPU Running Time for Reassignment=  ,%ld, milliseconds\n", total_t);

	return 1;
}
