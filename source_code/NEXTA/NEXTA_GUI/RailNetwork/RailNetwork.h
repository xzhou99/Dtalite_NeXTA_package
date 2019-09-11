#pragma once
#include <string>
class train_movement_arc
{
	
public:
	train_movement_arc()
	{
	direction = 1;
	b_train_label = false;
	}
	string train_id;
	bool b_train_label;
	int arc_sequence_no;
	int direction;
	int from_node_id;
	int to_node_id;
	float entry_time_in_sec;
	float exit_time_in_sec_with_train_length_time;
	float exit_time_in_sec;  // without train length time
};

class train_movements
{
public:
	string train_id;
	std::vector<train_movement_arc> movement_arc_vector;
	float destination_entry_time_in_sec;
};

class train_schedule
{
public:
	std::vector<train_movements> movement_vector; 
};



class train_info
{
public:
	string train_header;
	int entry_time;

	int origin_node_id;
	int destination_node_id;

	string direction;
	float speed_multiplier;
	float train_length;
	int tob;
	string hazmat;
	int sa_status_at_origin;
	int terminal_want_time;
	int terminal_arrival_time;
	int main_track_speed_difference_delay_in_second;
	
	train_info ()
	{
		main_track_speed_difference_delay_in_second = 0;
		train_length = 1.0f;
		speed_multiplier  = 1.0f;
		terminal_arrival_time = 0;
	
	}
};

class CLinkTimeTable
{
public: 
	GDPoint pt_from_start, pt_from_end, pt_to_start, pt_to_end;
	int start_time_in_min;
	int end_time_in_min;
	CLinkTimeTable()
	{};

	CLinkTimeTable(GDPoint from_start_pt, GDPoint from_end_pt, GDPoint to_start_pt, GDPoint to_end_pt, 
		int start_time, int end_time, std::vector<train_movement_arc> train_movement_vector)
	{
	pt_from_start = from_start_pt;
	pt_from_end = from_end_pt;
	pt_to_start = to_start_pt;
	pt_to_end = to_end_pt;
	start_time_in_min = start_time;
	end_time_in_min = end_time;
	m_train_movement_vector = train_movement_vector;
	};

	std::vector<train_movement_arc> m_train_movement_vector;  

};

class MaintenanceOfWay
{
public: 
	int from_node_id;
	int to_node_id;
	int start_time_in_min;
	int end_time_in_min;
};

