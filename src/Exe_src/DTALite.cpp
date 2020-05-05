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
#include "stdafx.h"
#include <stdlib.h>
#include <crtdbg.h>

#include "D:\SourceCode\DTALite2.0\DLL_src\DP\main_api.cpp"


double network_simulation(int a, int b);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{


		int nRetCode = 0;

		// initialize MFC and print and error on failure
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;

			return 0;
		}

		/**********************************************/
		//below is the main traffic assignment-simulation code



		// if  ./DTASettings.ini does not exist, then we should print out all the default settings for user to change
		//
	/*	g_AppStartTime = CTime::GetCurrentTime();
		g_AppLastIterationStartTime = g_AppStartTime;*/

		//Read DTALite Settings first
		network_simulation(0, 0);

		//	exit(0);   // rely on operating system to release all memory
		return nRetCode;
	}

