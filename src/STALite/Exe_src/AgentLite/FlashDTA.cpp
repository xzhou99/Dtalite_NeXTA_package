//  Portions Copyright 2019
// Xuesong Zhou
//   If you help write or modify the code, please also list your names here.
//   The reason of having Copyright info here is to ensure all the modified version, as a whole, under the GPL
//   and further prevent a violation of the GPL.

// More about "How to use GNU licenses for your own software"
// http://www.gnu.org/licenses/gpl-howto.html

#include "stdafx.h"

extern double SignalAPI(int iteration_number, int MainSigModual_mode, int signal_updating_output);
#include "..\AgentLite\main_api.cpp";
#include "..\AgentLite\signal_api.cpp";


int main(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int iteration_number = 20;
	int column_updating_iterations = 40;
	int signal_updating_iterations = -1;
	int signal_updating_output = 0;
	int assignment_mode = 1;  // generate link performance and agent file

	CCSVParser parser_settings;
	parser_settings.IsFirstLineHeader = false;

	if (parser_settings.OpenCSVFile("settings.csv", false))
	{
			while (parser_settings.ReadRecord_Section())
		{
			if (parser_settings.SectionName == "[assignment]")
			{
			
			int value_int;
			string assignment_mode_str;
			parser_settings.GetValueByFieldName("number_of_iterations", iteration_number,true,true);
			parser_settings.GetValueByFieldName("assignment_mode", assignment_mode_str);
			
			if (assignment_mode_str == "lue")
				assignment_mode = 0;
			else if (assignment_mode_str == "ue")
				assignment_mode = 1;
			else if (assignment_mode_str == "dta")
				assignment_mode = 2;
			else if (assignment_mode_str == "odme")
				assignment_mode = 3;
			else if (assignment_mode_str == "sig")
				assignment_mode = 4;
			else
			{
				cout << "assignment_mode " << assignment_mode_str.c_str() << " in settings.csv is invalid." << endl;
					g_ProgramStop();
			}

			parser_settings.GetValueByFieldName("column_updating_iterations", column_updating_iterations, true, true);

			parser_settings.GetValueByFieldName("signal_updating_iterations", signal_updating_iterations, false, false);
			parser_settings.GetValueByFieldName("signal_updating_output", signal_updating_output, false, true);


			break;  // just one record
			}
		}
	}

	if (assignment_mode == 4)
	{
		SignalAPI(0, assignment_mode, 1);
	
		return 0;
	}


	network_assignment(iteration_number, assignment_mode, column_updating_iterations, signal_updating_iterations);  // obtain initial flow values
}
