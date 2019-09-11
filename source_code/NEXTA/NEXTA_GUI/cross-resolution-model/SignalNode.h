
#pragma once

#include <math.h>
#include <deque>
#include <map>
#include <set>
#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;


class CDataElement
{
public:
	int colume_index, row_index;
	float m_text;
	string m_string;

	CDataElement()
	{
	m_text = -1;
	colume_index = 0;
	row_index = 0;

	}

};





class DTA_Timing
{
public:
	int Node_id;
	int DataAry[8];
	int Cycle, OFF, LD, CLR;
	string REF;

	DTA_Timing()
	{
	}

	void initial(int cl)
	{
		Cycle = cl;

		DataAry[0] = 0;
		DataAry[1] = 30;
		DataAry[2] = 0;
		DataAry[3] = 30;
		DataAry[4] = 0;
		DataAry[5] = 30;
		DataAry[6] = 0;
		DataAry[7] = 30;


		LD = 1357;
		OFF = 0;
		REF = "26+";
	}

};




//class DTA_NodeMovementSet
//{
//public:
//	int CurrentNodeID; 
//	CDataElement DataMatrix[50][50]; // generic data matrix (to store number and string)
//	DTA_Movement MovementMatrix[50];   //32 possible movements
////	DTA_NodeMovementSet &operator=( DTA_NodeMovementSet & );
//
//	DTA_NodeMovementSet()
//	{
//		//Default values here
//	}
//
//	void copy_from_MovementData_toTextMatrix(DTA_Movement movement, DTA_SIG_MOVEMENT c)
//	{
//
//		int i = 0;  // i is row index 
//		DataMatrix[i][c].m_text = movement.UpNodeID; i++; 
//		DataMatrix[i][c].m_text = movement.DestNodeID; i++; 
//		DataMatrix[i][c].m_text = movement.TurnVolume; i++; 
//		
//		DataMatrix[i][c].m_text = movement.Lanes; i++; 
//		DataMatrix[i][c].m_text = movement.Shared; i++; 
//		DataMatrix[i][c].m_text = movement.Width; i++; 
//		DataMatrix[i][c].m_text = movement.Storage; i++; 
//		DataMatrix[i][c].m_text = movement.StLanes; i++; 
//		DataMatrix[i][c].m_text = movement.Grade; i++; 
//		DataMatrix[i][c].m_text = movement.Speed; i++; 
//		DataMatrix[i][c].m_text = movement.FirstDetect; i++; 
//		DataMatrix[i][c].m_text = movement.LastDetect; i++; 
//		DataMatrix[i][c].m_text = movement.Phase1; i++; 
//		DataMatrix[i][c].m_text = movement.PermPhase1; i++; 
//		DataMatrix[i][c].m_text = movement.DetectPhase1; i++; 
//
//		DataMatrix[i][c].m_text = movement.EffectiveGreen; i++; 
//		DataMatrix[i][c].m_text = movement.Capacity; i++; 
//		DataMatrix[i][c].m_text = movement.VOC; i++; 
//		DataMatrix[i][c].m_text = movement.Delay; i++; 
//		DataMatrix[i][c].m_string  = movement.LOS; i++; 
//
//		DataMatrix[i][c].m_text = movement.IdealFlow; i++; 
//		DataMatrix[i][c].m_text = movement.LostTime; i++; 
//		DataMatrix[i][c].m_text = movement.SatFlow; i++; 
//		DataMatrix[i][c].m_text = movement.SatFlowPerm; i++; 
//		DataMatrix[i][c].m_text = movement.SatFlowRTOR; i++; 
//		DataMatrix[i][c].m_text = movement.HeadwayFact; i++; 
//		DataMatrix[i][c].m_text = movement.Volume; i++; 
//		DataMatrix[i][c].m_text = movement.Peds; i++; 
//		DataMatrix[i][c].m_text = movement.Bicycles; i++; 
//		DataMatrix[i][c].m_text = movement.PHF; i++; 
//		DataMatrix[i][c].m_text = movement.Growth; i++; 
//		DataMatrix[i][c].m_text = movement.HeavyVehicles; i++; 
//		DataMatrix[i][c].m_text = movement.BusStops; i++; 
//		DataMatrix[i][c].m_text = movement.Midblock; i++; 
//		DataMatrix[i][c].m_text = movement.Distance; i++; 
//		DataMatrix[i][c].m_text = movement.TravelTime; i++; 
//		DataMatrix[i][c].m_string  = movement.StreetName; i++; 
//
//
//
//	}
//
//	void copy_from_TextMatrix_to_Movement(DTA_Movement &movement, DTA_SIG_MOVEMENT c)
//	{
//		int i = 2;
//		movement.TurnVolume = DataMatrix[i][c].m_text; i++; 
//		movement.Lanes = DataMatrix[i][c].m_text; i++; 
//		movement.Shared = DataMatrix[i][c].m_text; i++; 
//		movement.Width = DataMatrix[i][c].m_text; i++; 
//		movement.Storage = DataMatrix[i][c].m_text; i++; 
//		movement.StLanes = DataMatrix[i][c].m_text; i++; 
//		movement.Grade = DataMatrix[i][c].m_text; i++; 
//		movement.Speed = DataMatrix[i][c].m_text; i++; 
//		movement.FirstDetect = DataMatrix[i][c].m_text; i++; 
//		movement.LastDetect = DataMatrix[i][c].m_text; i++; 
//		movement.Phase1 = DataMatrix[i][c].m_text; i++; 
//		movement.PermPhase1 = DataMatrix[i][c].m_text; i++; 
//		movement.DetectPhase1 = DataMatrix[i][c].m_text; i++;
//
//		movement.EffectiveGreen = DataMatrix[i][c].m_text; i++;
//		movement.Capacity = DataMatrix[i][c].m_text; i++;
//		movement.VOC = DataMatrix[i][c].m_text; i++;
//		movement.Delay = DataMatrix[i][c].m_text; i++;
//		movement.LOS = DataMatrix[i][c].m_string; i++;
//
//
//		movement.IdealFlow = DataMatrix[i][c].m_text; i++; 
//		movement.LostTime = DataMatrix[i][c].m_text; i++; 
//		movement.SatFlow = DataMatrix[i][c].m_text; i++; 
//		movement.SatFlowPerm = DataMatrix[i][c].m_text; i++; 
//		movement.SatFlowRTOR = DataMatrix[i][c].m_text; i++; 
//		movement.HeadwayFact = DataMatrix[i][c].m_text; i++; 
//		movement.Volume = DataMatrix[i][c].m_text; i++; 
//		movement.Peds = DataMatrix[i][c].m_text; i++; 
//		movement.Bicycles = DataMatrix[i][c].m_text; i++; 
//		movement.PHF = DataMatrix[i][c].m_text; i++; 
//		movement.Growth = DataMatrix[i][c].m_text; i++; 
//		movement.HeavyVehicles = DataMatrix[i][c].m_text; i++; 
//		movement.BusStops = DataMatrix[i][c].m_text; i++; 
//		movement.Midblock = DataMatrix[i][c].m_text; i++; 
//		movement.Distance = DataMatrix[i][c].m_text; i++; 
//		movement.TravelTime = DataMatrix[i][c].m_text; i++; 
//
//		movement.StreetName = DataMatrix[i][c].m_string; i++; 
//	}
//	void copy_parameters(DTA_NodeMovementSet M)
//	{
//		int i,j;
//		CurrentNodeID = M.CurrentNodeID;
//		for(j=0; j<12;j++)
//		{
//			for(i=0; i<30; i++)
//			{
//				DataMatrix[i][j] = M.DataMatrix[i][j];
//			}
//		}
//	}
//};




