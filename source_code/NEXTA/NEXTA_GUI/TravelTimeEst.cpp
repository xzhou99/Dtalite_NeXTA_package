// Traffic Time Estimation, part of TLiteDoc.cpp : implementation of the CTLiteDoc class
//

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
#include "CSVParser.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "TLiteView.h"
#include "DlgMOE.h"
#include "DlgPathMOE.h"
#include "DlgTrainInfo.h"
#include "DlgFileLoading.h"
#include "MainFrm.h"
#include "Shellapi.h"
#include "DlgDefaultLinkProperties.h"
#include "DlgAssignmentSettings.h"
#include "DlgLinkList.h"
#include "DlgGridCtrl.h"
#include "Shellapi.h"
#include "DlgSensorDataLoading.h"
#include "Dlg_ImportODDemand.h"
#include "DlgNetworkAlignment.h"
#include "Dlg_VehEmissions.h"
#include "DlgPathList.h"
extern CDlgPathList* g_pPathListDlg;
extern CDlgPathMOE	*g_pPathMOEDlg;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void DTALink::ComputeHistoricalAvg(int number_of_weekdays)
{

	m_MinSpeed = 200;
	m_MaxSpeed = 0;

	m_HistLinkMOEAry.reserve(1440);
	int t;
	float VolumeSum = 0;
	float SpeedSum = 0;


	for( t=0; t< 1440; t++)
	{
		SLinkMOE element; 
		// reset
		m_HistLinkMOEAry.push_back(element);

		// start counting
		int count = 0;


		for(int day =0; day <number_of_weekdays; day ++)
		{
				m_HistLinkMOEAry[t].Speed +=m_LinkMOEAry[day*1440+t].Speed;
				m_HistLinkMOEAry[t].LinkFlow +=m_LinkMOEAry[day*1440+t].LinkFlow;
				m_HistLinkMOEAry[t].Density += m_LinkMOEAry[day*1440+t].Density;
				m_HistLinkMOEAry[t].TravelTime += m_LinkMOEAry[day*1440+t].TravelTime;

				count++;

				// update min and max speed

				if((t>=8*60 && t<9*60)) //8-9AM
				{
					// update link-specific min and max speed
					if(m_LinkMOEAry[day*1440+t].Speed < m_MinSpeed)
						m_MinSpeed = m_LinkMOEAry[day*1440+t].Speed;

					if(m_LinkMOEAry[day*1440+t].Speed > m_MaxSpeed)
						m_MaxSpeed = m_LinkMOEAry[day*1440+t].Speed;


				}

		}

		if(count>=1) 
		{
			// calculate final mean statistics
			m_HistLinkMOEAry[t].Speed /=count;
			m_HistLinkMOEAry[t].LinkFlow /=count;
			m_HistLinkMOEAry[t].ArrivalCumulativeFlow /=count;
			m_HistLinkMOEAry[t].Density /=count;
			m_HistLinkMOEAry[t].TravelTime /=count;
		}


	}

}



struc_traffic_state DTALink::GetPredictedState(int CurrentTime, int PredictionHorizon)  // return value is speed
{

	struc_traffic_state future_state;
	// step 1: calculate delta w
	float DeltaW =  m_LinkMOEAry[CurrentTime].TravelTime -  m_HistLinkMOEAry[CurrentTime%1440].TravelTime;

	// step 2: propogate delta w to Furture time
	//this is the most tricky part

	float FutureDeltaW = max(0,(1-PredictionHorizon)/45.0f)*DeltaW;   // after 45 min, FutureDeltaW becomes zero, completely come back to historical pattern
	// step 3: add future delta w to historical time at future time

	future_state.traveltime  = FutureDeltaW+ m_HistLinkMOEAry[(CurrentTime+PredictionHorizon)%1440].TravelTime;
	// step 4: produce speed

	future_state.speed = m_Length/max(m_FreeFlowTravelTime,future_state.traveltime);

	return future_state;
}

void DTAPath::UpdateWithinDayStatistics()
{
	int t;
	for(t=0; t<1440; t++)
	{
		m_WithinDayMeanTimeDependentTravelTime[t] = 0;
		m_WithinDayMaxTimeDependentTravelTime[t] = 0;
	}

	for(t=0; t<1440*m_number_of_days; t++)
	{
		m_WithinDayMeanTimeDependentTravelTime[t%1440] += m_TimeDependentTravelTime[t]/m_number_of_days;
		m_WithinDayMaxTimeDependentTravelTime[t%1440] = max(m_WithinDayMaxTimeDependentTravelTime[t%1440],m_TimeDependentTravelTime[t]);
	}
}

bool CTLiteDoc::ReadSensorCountData(LPCTSTR lpszFileName, bool bErrorMessage )
{

	CCSVParser parser;
	int error_count = 0;

	int data_count = 0;
	if (parser.OpenCSVFile(CString2StdString(lpszFileName)))
	{

			std::list<DTALink*>::iterator iLink;
			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
			(*iLink)->m_total_sensor_link_volume = 0;
			}

			int sensor_count = 0;
			CString error_message;

			CString prev_error_message;
		while(parser.ReadRecord())
		{
			DTA_sensor sensor;

			std::string count_sensor_id;
			std::string direction;
			int from_node_id = 0;
			int to_node_id = 0; 


			parser.GetValueByFieldName("count_sensor_id",count_sensor_id);

			if (count_sensor_id.size() == 0)
			{
				parser.GetValueByFieldName("sensor_id", count_sensor_id);
			}

			if (count_sensor_id.size() == 0)
			{
				continue;  //skip empty line
			}
			parser.GetValueByFieldName("direction", direction);
			

			int day_no = 0;

			parser.GetValueByFieldName("day_no",day_no ); 

			g_SensorDayDataMap[ day_no] = true;

			g_SensorLastDayNo= max(g_SensorLastDayNo,  day_no);
			g_SensorDayNo = g_SensorLastDayNo;
			sensor.SensorID = count_sensor_id;



			DTALink* pLink = NULL;

			if(count_sensor_id.size  () > 0 && m_CountSensorIDMap.find(count_sensor_id.c_str ())!=m_CountSensorIDMap.end())
			{
				pLink = m_CountSensorIDMap[count_sensor_id.c_str ()];
			}
			

				if(pLink!=NULL)
			{
				sensor.LinkID = pLink->m_LinkNo ;

				sensor.FromNodeNumber = pLink->m_FromNodeNumber ;
				sensor.ToNodeNumber = pLink->m_ToNodeNumber ;


				if(m_SensorMap.find(sensor.SensorID) == m_SensorMap.end())
				{
					m_SensorMap[sensor.SensorID] = sensor;
				}

				pLink->m_bSensorData  = true;
				pLink->m_bCountSensorData  = true;

				float start_time_in_min =0;
				float end_time_in_min = 0;

				parser.GetValueByFieldName("start_time_in_min",start_time_in_min );
				parser.GetValueByFieldName("end_time_in_min",end_time_in_min );


				// skip the data if the sensor data's time range is out of the calibration time window 
				if (end_time_in_min <= m_calibration_data_start_time_in_min || start_time_in_min >= m_calibration_data_end_time_in_min)
					continue;

				int count= -1;  // no data
				parser.GetValueByFieldName("link_count",count );

				if(count ==0)
					parser.GetValueByFieldName("count",count );

				if(count<=1)  //skip data
					continue;


				float travel_time_in_min = -1;
				parser.GetValueByFieldName("travel_time_in_min",travel_time_in_min );
				

				pLink->m_SensorTypeString = sensor.SensorType;

				data_count++;

				if(start_time_in_min > end_time_in_min && error_message.GetLength () < 1000)
				{
						CString msg;
						msg.Format ("Sensor %d-> %d has an error of start_time_in_min = %d > end_time_in_min = %d.\n",
							sensor.FromNodeNumber , sensor.ToNodeNumber,start_time_in_min > end_time_in_min);

					if(prev_error_message!=msg)
					{
						error_message+=msg;
						prev_error_message=  msg;
					}

				}

				if(count ==0 && error_message.GetLength () < 1000)
				{
						CString msg;
						msg.Format ("Sensor %d->%d has an error of link_count =0.\n",pLink->m_FromNodeNumber, pLink->m_ToNodeNumber);

					if(prev_error_message!=msg)
					{
						error_message+=msg;
						prev_error_message=  msg;
					}

				}

			std::string second_count_sensor_id;

			parser.GetValueByFieldName("second_count_sensor_id",second_count_sensor_id); 

			DTALink* pLink2 = NULL;

			if(second_count_sensor_id.size  () > 0 && m_CountSensorIDMap.find(second_count_sensor_id.c_str ())!=m_CountSensorIDMap.end())
			{
				pLink2 = m_CountSensorIDMap[second_count_sensor_id.c_str ()];
			}


				DTASensorData element;
				element.start_time_in_min = start_time_in_min;
				element.end_time_in_min = end_time_in_min;
				element.count = count;
				element.count_sensor_id = count_sensor_id;
				element.day_no = day_no;
				element.direction = direction;
				element.second_count_sensor_id = second_count_sensor_id;

				element.link_from_node_id = pLink->m_FromNodeNumber;
				element.link_to_node_id = pLink->m_ToNodeNumber;

				if (count_sensor_id == "41008_2" && day_no == 16 && start_time_in_min == 405)
				{
					TRACE("");
				}
				element.derived_lane_hourly_volume = count * 60 / max(1, end_time_in_min - start_time_in_min)/ pLink->m_NumberOfLanes ;


				float density = -1;
				parser.GetValueByFieldName("lane_density_per_mile",density );

				if(density >=1)
					element.density = density;
				

				float speed = -1;
				parser.GetValueByFieldName("speed",speed );


				if(speed >0.001)  // valid data
				{
				pLink->m_bSpeedSensorData  = true;
				}

				pLink->m_SensorDataVector.push_back(element);

				m_SensorCountVector.push_back(element);
				pLink->m_total_sensor_link_volume += count;

				pLink->m_sensor_hourly_lane_volume_min = min(pLink->m_sensor_hourly_lane_volume_min, element.derived_lane_hourly_volume);
				pLink->m_sensor_hourly_lane_volume_max = max(pLink->m_sensor_hourly_lane_volume_max, element.derived_lane_hourly_volume);


				for(int t = start_time_in_min; t< min (1440,end_time_in_min); t++)
				{
	
					int time = day_no*1440 + t;  // allow shift of start time

					if(second_count_sensor_id.size() == 0 ) // link count only
					{


					if(count>=1)
					{
					// day specific value	
					pLink->m_LinkSensorMOEMap[ time].LinkFlow = count/(max(1.0,end_time_in_min-start_time_in_min)); 
					// overall value 
					pLink->m_LinkSensorMOEMap[ t].LinkFlow = count/(max(1.0,end_time_in_min-start_time_in_min));  
					}

					if(density>0.001)
					{
					pLink->m_LinkSensorMOEMap[ t].Density = density ;  // convert to per hour link flow
					}

					if(speed>0.001)
					{
					pLink->m_LinkSensorMOEMap[ t].Speed  = speed ;  // convert to per hour link flow
					}

					} // handle movement data later
					else
					{
							
							DTANodeMovement* pMovement = FindMovement(pLink->m_FromNodeNumber , pLink->m_ToNodeNumber , pLink2->m_ToNodeNumber );
						
							
								if(pMovement !=NULL)
								{
									pMovement->obs_turn_hourly_count  =  count/(max(1.0,end_time_in_min-start_time_in_min))*60;

									// mark the related movement has obs_turn_hourly_count
								}						
							}					
					
					}
			}else
			{
				if(error_message.GetLength () < 1000)
				{

				CString msg;


				if(count_sensor_id.size() >0)
				{			
				msg.Format ("count_sensor_id %s in sensor_count.csv does not exist in input_link.csv.\n", count_sensor_id.c_str ());
				}else
				{
				msg.Format ("link %d->%d in sensor_count.csv does not exist in input_link.csv.\n", from_node_id,to_node_id );
				}
	
				if(prev_error_message!=msg)
				{
					error_message+=msg;
					prev_error_message=  msg;
				}

				error_count++;


				continue;
				}
			}

		}

		if (error_message.GetLength() >= 1 )
		{
			m_SensorCountDataErrorMessage = error_message;

			if (bErrorMessage)
			{
				AfxMessageBox(error_message);
			}
		}


		if(m_SensorMap.size()>0)
		{
			m_SensorCountDataLoadingStatus.Format("%d sensor records are loaded from file sensor_count.csv.", data_count);
			return true;
		}
		else
			return false; // no sensors have been specified
	}

	return false;
}



bool CTLiteDoc::ReadSensorSpeedData(LPCTSTR lpszFileName, bool ErrorMessage)
{

	CCSVParser parser;
	int error_count = 0;

	int data_count = 0;
	if (parser.OpenCSVFile(CString2StdString(lpszFileName)))
	{

		int sensor_count = 0;
		CString error_message;

		CString prev_error_message;
		while (parser.ReadRecord())
		{
			DTA_sensor sensor;

			std::string speed_sensor_id;

			parser.GetValueByFieldName("speed_sensor_id", speed_sensor_id);


			if (speed_sensor_id.size() == 0)
			{
				continue;  //skip empty line
			}


			int day_no = -1;

			parser.GetValueByFieldName("day_no", day_no);

			if (day_no == -1)
			{
			
			CString msg;
			msg.Format("Field day_no is missing in sensor_speed.csv.\n");
			AfxMessageBox(msg);
			return false; 

			}

			g_SensorDayDataMap[day_no] = true;

			g_SensorLastDayNo = max(g_SensorLastDayNo, day_no);
			g_SensorDayNo = g_SensorLastDayNo;
			sensor.SensorID = speed_sensor_id;



			DTALink* pLink = NULL;

			if (speed_sensor_id.size() > 0 && m_SpeedSensorIDMap.find(speed_sensor_id.c_str()) != m_SpeedSensorIDMap.end())
			{
				pLink = m_SpeedSensorIDMap[speed_sensor_id.c_str()];
			}


			if (pLink != NULL)
			{
				sensor.LinkID = pLink->m_LinkNo;

				sensor.FromNodeNumber = pLink->m_FromNodeNumber;
				sensor.ToNodeNumber = pLink->m_ToNodeNumber;



				pLink->m_bSensorData = true;
				pLink->m_bSpeedSensorData = true;

				float start_time_in_min = 0;
				float end_time_in_min = 0;

				parser.GetValueByFieldName("start_time_in_min", start_time_in_min);
				parser.GetValueByFieldName("end_time_in_min", end_time_in_min);


				// skip the data if the sensor data's time range is out of the calibration time window 
				if (end_time_in_min <= m_calibration_data_start_time_in_min || start_time_in_min >= m_calibration_data_end_time_in_min)
					continue;

				float speed = -1;  // no data
				parser.GetValueByFieldName("speed", speed);

				if (speed <= 1)  //skip data
					continue;

				int population_count = 0;
				parser.GetValueByFieldName("population_count", population_count);

				pLink->m_SensorTypeString = sensor.SensorType;


				if (start_time_in_min > end_time_in_min && error_message.GetLength() < 1000)
				{
					CString msg;
					msg.Format("Sensor %d-> %d has an error of start_time_in_min = %d > end_time_in_min = %d.\n",
						sensor.FromNodeNumber, sensor.ToNodeNumber, start_time_in_min > end_time_in_min);

					if (prev_error_message != msg)
					{
						error_message += msg;
						prev_error_message = msg;
					}

				}

				if (speed <=1  && error_message.GetLength() < 1000)
				{
					CString msg;
					msg.Format("Sensor %d->%d has an error of speed <=1.\n", pLink->m_FromNodeNumber, pLink->m_ToNodeNumber);

					if (prev_error_message != msg)
					{
						error_message += msg;
						prev_error_message = msg;
					}

				}

			
				DTASensorData element;
				element.start_time_in_min = start_time_in_min;
				element.end_time_in_min = end_time_in_min;
				element.speed = speed;
				element.speed_sensor_id = speed_sensor_id;
				element.day_no = day_no;
				element.link_from_node_id = pLink->m_FromNodeNumber;
				element.link_to_node_id = pLink->m_ToNodeNumber;

				DTASensorData sensor_data_element;
				sensor_data_element.start_time_in_min = start_time_in_min;
				sensor_data_element.end_time_in_min = end_time_in_min;
				sensor_data_element.speed = speed;

				pLink->m_SensorDataVector.push_back(sensor_data_element);

				for (int t = start_time_in_min; t< min(1440, end_time_in_min); t++)
				{

					int time = day_no * 1440 + t;  // allow shift of start time

						// day specific value	
						pLink->m_LinkSensorMOEMap[time].Speed = speed;
						pLink->m_LinkSensorMOEMap[time].PopulationFlow = population_count / max(1, end_time_in_min - start_time_in_min);
						// overall value 
						pLink->m_LinkSensorMOEMap[t].Speed = speed;
						pLink->m_LinkSensorMOEMap[t].PopulationFlow = pLink->m_LinkSensorMOEMap[time].PopulationFlow;

				}
			}
			else
			{
				if (error_message.GetLength() < 1000)
				{

					CString msg;


					if (speed_sensor_id.size() >0)
					{
						msg.Format("speed_sensor_id %s in sensor_speed.csv does not exist in input_link.csv.\n", speed_sensor_id.c_str());
					}
					
					if (prev_error_message != msg)
					{
						error_message += msg;
						prev_error_message = msg;
					}

					error_count++;


					continue;
				}
			}
			data_count++;
		}

		if (error_message.GetLength() >= 1 )
		{
			m_SensorSpeedDataErrorMessage = error_message;
			
			if (ErrorMessage)
			AfxMessageBox(error_message);

		}


			m_SensorSpeedDataLoadingStatus.Format("%d sensor records are loaded from file sensor_speed.csv.", data_count);
			return true;
	}

	return false;
}


bool CTLiteDoc::ReadSensorLocationData(LPCTSTR lpszFileName)
{

	CCSVParser parser;
	int error_count = 0;

	int data_count = 0;
	if (parser.OpenCSVFile(CString2StdString(lpszFileName)))
	{

		int sensor_count = 0;
		CString error_message;

		CString prev_error_message;
		while (parser.ReadRecord())
		{
			DTA_sensor sensor;

			std::string sensor_id;
			int from_node_id = 0;
			int to_node_id = 0;


			parser.GetValueByFieldName("sensor_id", sensor_id);

			sensor.SensorID = sensor_id;
			parser.GetValueByFieldName("name", sensor.description);

			parser.GetValueByFieldName("x", sensor.pt.x, false);
			parser.GetValueByFieldName("y", sensor.pt.y, false);
			parser.GetValueByFieldName("direction", sensor.direction);
			parser.GetValueByFieldName("orientation_code", sensor.orientation_code);
			parser.GetValueByFieldName("orientation2_code", sensor.orientation2_code);
			parser.GetValueByFieldName("type_code", sensor.type_code);
			parser.GetValueByFieldName("loop_code", sensor.loop_code);

			parser.GetValueByFieldName("matched_from_node_id", sensor.matched_to_node_id);
			parser.GetValueByFieldName("matched_to_node_id", sensor.matched_to_node_id);

			DTALink* pLink = NULL;

			if (sensor_id.size() > 0 && m_CountSensorIDMap.find(sensor_id.c_str()) != m_CountSensorIDMap.end())
			{
				pLink = m_CountSensorIDMap[sensor_id.c_str()];
			}

			if (pLink == NULL)
			{
				parser.GetValueByFieldName("from_node_id", from_node_id);
				parser.GetValueByFieldName("to_node_id", to_node_id);

				pLink = FindLinkWithNodeNumbers(from_node_id, to_node_id);
			}

			std::string sensor_key = sensor_id + "_" + sensor.direction;

			if (m_SensorMap.find(sensor_key) == m_SensorMap.end())  // if the sensor id + direction has not been defined 
			{
				m_SensorMap[sensor_key] = sensor;
			}


		}
	}

		if (m_SensorMap.size()>0)
		{
			m_SensorLocationLoadingStatus.Format("%d sensor location records are loaded from file input_sensor_location.csv.", m_SensorMap.size());
			return true;
		}
		else
			return false; // no sensors have been specified
	

	return false;
}


bool CTLiteDoc::ReadMultiDaySensorData(LPCTSTR lpszFileName)
{
/*
	CWaitCursor wc;
	FILE* st = NULL;
	std::list<DTALink*>::iterator iLink;


	int TimeHorizon = 1440; // 1440

	m_SamplingTimeInterval = 5;

	m_NumberOfDays = 30;

	float Occ_to_Density_Coef = 100.0f;


	CDlgSensorDataLoading dlg;

	if(m_SimulationLinkMOEDataLoadingStatus.GetLength ()>0)
		dlg.m_NumberOfDays  = 1;

	if(dlg.DoModal() ==IDOK)
	{
		m_SamplingTimeInterval = dlg.m_ObsTimeInterval;
		m_NumberOfDays = dlg.m_NumberOfDays;
		Occ_to_Density_Coef = dlg.m_Occ_to_Density_Coef;

		bool bResetMOEAryFlag = false;
			CString str_day;

			fopen_s(&st,lpszFileName,"r");

			if(st!=NULL)
			{
			for(int day =1; day <= m_NumberOfDays; day++)
					{
				if(!bResetMOEAryFlag && m_SimulationLinkMOEDataLoadingStatus.GetLength () == 0) // simulation data are not loaded. reset data array
				{

					g_Simulation_Time_Horizon = 1440*m_NumberOfDays;


					for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
					{
						if((*iLink)->m_bSensorData == true)  // identified in the sensor_location.csv
						{
							(*iLink)->ResetMOEAry(g_Simulation_Time_Horizon);
						}
					}

					bResetMOEAryFlag = true;
				}

			
			int number_of_samples = 0;
			while(!feof(st))
			{

				int Month = g_read_integer(st);
				if(Month == -1)  // reach end of file
					break;

				int DayOfMonth   = g_read_integer(st);
				int Year  = g_read_integer(st);
				int Hour   = g_read_integer(st);
				int Min   = g_read_integer(st);

				int SensorID  =  g_read_integer(st);
				float TotalFlow = g_read_float(st);
				float Occupancy = g_read_float(st);
				float AvgLinkSpeed = g_read_float(st);

				map<long, DTALink*>::iterator it;

				DTALink* pLink ;
				if ( (it = m_SensorIDtoLinkMap.find(SensorID)) != m_SensorIDtoLinkMap.end()) 
				{
					pLink = it->second ;

				}else
				{
					CString error_message;
					error_message.Format ("Reading error: Sensor ID %d has not been defined in file sensor_location.csv.");
					AfxMessageBox(error_message);
					fclose(st);
					return true;
				}

				if(pLink!=NULL && pLink->m_bSensorData)
				{
					int t  = ((day - 1)*1440+ Hour*60+Min) ;

					if(t<pLink->m_SimulationHorizon)
					{

						if(AvgLinkSpeed<=1)  // 0 or negative values means missing speed
							AvgLinkSpeed = pLink->m_SpeedLimit ;


						ASSERT(pLink->m_NumberOfLanes > 0);

						if(m_SimulationLinkMOEDataLoadingStatus.GetLength () == 0)  // simulation data not loaded
						{
 							pLink->m_LinkMOEAry[ t].LinkFlow = TotalFlow*60/m_SamplingTimeInterval/pLink->m_NumberOfLanes;  // convert to per hour link flow
							pLink->m_LinkMOEAry[ t].Speed = AvgLinkSpeed; 
							pLink->m_LinkMOEAry[ t].TravelTime = pLink->m_SpeedLimit /max(1,AvgLinkSpeed)*100;


							if(Occupancy <=0.001)
								pLink->m_LinkMOEAry[t].Density = pLink->m_LinkMOEAry[t].LinkFlow / max(1.0f,pLink->m_LinkMOEAry[t].Speed);
							else
								pLink->m_LinkMOEAry[t].Density = Occupancy * Occ_to_Density_Coef;

							// copy data to other intervals
							for(int tt = 1; tt<m_SamplingTimeInterval; tt++)
							{
								pLink->m_LinkMOEAry[ t+tt].LinkFlow = pLink->m_LinkMOEAry[t].LinkFlow ;
								pLink->m_LinkMOEAry[t+tt].Speed = pLink->m_LinkMOEAry[t].Speed;
								pLink->m_LinkMOEAry[t+tt].Density = pLink->m_LinkMOEAry[t].Density;
								pLink->m_LinkMOEAry[t+tt].TravelTime = pLink->m_LinkMOEAry[t].TravelTime;

							}
						}else // simulation data loaded
						{

							pLink->m_LinkMOEAry[ t].LinkFlow = TotalFlow*60/m_SamplingTimeInterval;  // convert to per hour link flow
							pLink->m_LinkMOEAry[ t].Speed = AvgLinkSpeed; 
							pLink->m_LinkMOEAry[ t].TravelTimeCopy = pLink->m_SpeedLimit /max(1,AvgLinkSpeed)*100;


							if(Occupancy <=0.001)
								pLink->m_LinkMOEAry[t].Density = pLink->m_LinkMOEAry[t].LinkFlow / max(1.0f,pLink->m_LinkMOEAry[t].Speed);
							else
								pLink->m_LinkMOEAry[t].Density = Occupancy * Occ_to_Density_Coef;

							// copy data to other intervals
							for(int tt = 1; tt<m_SamplingTimeInterval; tt++)
							{
								pLink->m_LinkMOEAry[ t+tt].LinkFlow = pLink->m_LinkMOEAry[t].LinkFlow ;
								pLink->m_LinkMOEAry[t+tt].Speed = pLink->m_LinkMOEAry[t].Speed;
								pLink->m_LinkMOEAry[t+tt].Density = pLink->m_LinkMOEAry[t].Density;
								pLink->m_LinkMOEAry[t+tt].TravelTimeCopy = pLink->m_LinkMOEAry[t].TravelTimeCopy;

							}
						}


					}

				}	
				number_of_samples++;
			}

			m_SensorCountDataLoadingStatus.Format("%d sensor data records are loaded from file SensorDataDay***.csv.",number_of_samples);


			fclose(st);
		}
	}

	}else
	{
	m_NumberOfDays = 0;
	}
*/
	return false;
}

void CTLiteDoc::ReadEventData(CString directory)
{
	//CWaitCursor wc;
	//FILE* st = NULL;

	//fopen_s(&st,directory+"event.csv","r");

	//if(st!=NULL)
	//{

	//	int number_of_samples = 0;
	//	int episode_no =1;
	//	while(!feof(st))
	//	{

	//		int DayNo =  g_read_integer(st);
	//		if(DayNo == -1)  // reach end of file
	//			break;

	//		int Month = g_read_integer(st);

	//		int DayOfMonth   = g_read_integer(st);
	//		int Year  = g_read_integer(st);
	//		int Hour   = g_read_integer(st);
	//		int Min   = g_read_integer(st);

	//		int start_t  = ((DayNo - 1)*1440+ Hour*60+Min);

	//		Month = g_read_integer(st);

	//		DayOfMonth   = g_read_integer(st);
	//		Year  = g_read_integer(st);
	//		Hour   = g_read_integer(st);
	//		Min   = g_read_integer(st);

	//		int end_t  = ((DayNo - 1)*1440+ Hour*60+Min);

	//		int EventCode   = g_read_integer(st);

	//		std::list<DTALink*>::iterator iLink;
	//		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	//		{

	//			int episode_duration  = end_t - start_t;
	//			for(int t = start_t; t<end_t; t+= 1)
	//			{

	//				if(t<(*iLink)->m_SimulationHorizon)
	//				{
	//					(*iLink)->m_LinkMOEAry[t].EventCode  = EventCode;
	//					(*iLink)->m_LinkMOEAry[t].EpisodeNo = episode_no;
	//					(*iLink)->m_LinkMOEAry[t].EpisoDuration = episode_duration;

	//					number_of_samples++;

	//				}

	//			}
	//		}


	//		episode_no++;
	//	}	

	//	m_EventDataLoadingStatus.Format("%d event data records are loaded from file event.csv.",number_of_samples);

	//	fclose(st);
	//}
}


void CTLiteDoc::BuildHistoricalDatabase()
{
	std::list<DTALink*>::iterator iLink;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		if((*iLink)->m_bSensorData  == true)
		{
			for(int t = 0; t<(*iLink)->m_SimulationHorizon; t+= m_SamplingTimeInterval)
			{
				if(t%1440 ==0)
				{  // reset at the begining of day
					(*iLink)->m_LinkMOEAry[t].ArrivalCumulativeFlow = (*iLink)->m_LinkMOEAry[t].LinkFlow;
				}else
				{
					(*iLink)->m_LinkMOEAry[t].ArrivalCumulativeFlow = (*iLink)->m_LinkMOEAry[t-m_SamplingTimeInterval].ArrivalCumulativeFlow  + (*iLink)->m_LinkMOEAry[t].LinkFlow ;

				}

				for(int tt= 1; tt<m_SamplingTimeInterval;tt++)
				{
					(*iLink)->m_LinkMOEAry[t+tt].ArrivalCumulativeFlow = 	(*iLink)->m_LinkMOEAry[t].ArrivalCumulativeFlow;
				}

			}


			(*iLink)->ComputeHistoricalAvg(m_NumberOfDays); 
		}
	}


}

void CTLiteDoc::OnToolsExporttoHistDatabase()
{
	CWaitCursor wc;
		bool bFormatFlag = true;
		FILE* st = NULL;

		CString file_name = this->m_ProjectDirectory + "gams_input.txt";
		fopen_s(&st, file_name, "w");
		if (st != NULL)
		{
			fprintf(st, "parameter arcs(i,j,t_e,t_l) link travel time /\n");
			std::list<DTALink*>::iterator iLink;

			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				int beg_time_index = m_DemandLoadingStartTimeInMin;
				int end_time_index = m_DemandLoadingStartTimeInMin + 120;

				for (int t = beg_time_index + 1; t < end_time_index; t++)
				{

					fprintf(st, "%d. %d. ", (*iLink)->m_FromNodeID+1, (*iLink)->m_ToNodeID+1);
					int travel_time = max(1, int((*iLink)->m_FreeFlowTravelTime*2+0.5));

					int time_index_from = t - beg_time_index;
					int time_index_to = t + travel_time - beg_time_index;
					fprintf(st, "%d. %d %d\n", time_index_from, time_index_to, travel_time);

				}


			}

			fprintf(st, "/;\n");

			fprintf(st, "--link list: from node no, to node no: from node number -> to node number--\n ");
			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{

				fprintf(st, "%d->%d: %d,%d\n", (*iLink)->m_FromNodeID + 1, (*iLink)->m_ToNodeID + 1, (*iLink) -> m_FromNodeNumber, (*iLink)->m_ToNodeNumber);


			}


			//	capacity

			//		fprintf(st,"parameter capacity(i,j,t_e) capacity /\n");

			//	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			//	{

			//		   
			//		for(int t= beg_time_index+1; t< end_time_index; t++)
			//			{
			//					fprintf(st,"%d. %d. %d  6\n", (*iLink)->m_FromNodeNumber, (*iLink)->m_ToNodeNumber,t );

			//			}

			//		
			//	}

			//fprintf(st,"/;\n");


			fclose(st);
			HINSTANCE result = ShellExecute(NULL, _T("open"), file_name, NULL, NULL, SW_SHOW);

		}

}


int CTLiteDoc::AlternativeRouting(int NumberOfRoutes = 2)
{

	return 0;
}


int CTLiteDoc::Routing(bool bCheckConnectivity, bool bRebuildNetwork)
{
	CWaitCursor cws;
	m_NodeSizeSP = 0;  // reset 

	std::list<DTALink*>::iterator iLink;

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_OverlappingCost  = 0;  // randomize link cost to avoid overlapping

			if(m_LinkTypeMap[(*iLink)->m_link_type].IsConnector ())
				(*iLink)->m_bConnector = true;

			if(m_LinkTypeMap[(*iLink)->m_link_type].IsTransit  ())
				(*iLink)->m_bTransit  = true;
	
			if(m_LinkTypeMap[(*iLink)->m_link_type].IsConnector ())
				(*iLink)->m_bWalking  = true;

		// consider intermediate destination here
	}


	
	if(bCheckConnectivity == false)
	{
		if(m_OriginNodeID < 0 || m_DestinationNodeID <0)
	{
		m_SelectPathNo = -1;
		return 0;
	}
	}



 // create network every time, because we might add nodes/links on the fly
	
	if(m_pNetwork ==NULL)  
		{
		m_pNetwork = new DTANetworkForSP(m_NodeSet.size(), m_LinkSet.size(), 1, 1, m_AdjLinkSize);  //  network instance for single processor in multi-thread environment
		m_pNetwork->BuildPhysicalNetwork(&m_NodeSet, &m_LinkSet, m_RandomRoutingCoefficient, false, m_OriginNodeID, m_DestinationNodeID);
		}

	if(bRebuildNetwork)  // link cost changed
	{
		m_pNetwork->BuildPhysicalNetwork(&m_NodeSet, &m_LinkSet, m_RandomRoutingCoefficient, false,m_OriginNodeID, m_DestinationNodeID);

	}
		int NodeNodeSum = 0;

		int PathLinkList[MAX_NODE_SIZE_IN_A_PATH];

			float TotalCost;
			bool distance_flag = true;

			
			int     NodeSize ;
			
			if(bCheckConnectivity==true)
			{
				m_pNetwork->SimplifiedTDLabelCorrecting_DoubleQueue(m_OriginNodeID, 0, m_DestinationNodeID, 1, 10.0f,PathLinkList,TotalCost, distance_flag, true, false,0);   // Pointer to previous node (node)

					for (std::list<DTANode*>::iterator  iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
					{
						(*iNode)->m_DistanceToRoot  = m_pNetwork->LabelCostAry[(*iNode)->m_NodeNo ];
					}
		
				return 0;
			}

			//the code below is not checking connectivity

			std::vector<int> ODNodeSequenceVector;

			ODNodeSequenceVector.push_back(m_OriginNodeID);

			// add intermediate destinations 
			for(unsigned int idest = 0; idest < m_IntermediateDestinationVector.size(); idest++)
			{
			ODNodeSequenceVector.push_back(m_IntermediateDestinationVector[idest]);
			}

			ODNodeSequenceVector.push_back(m_DestinationNodeID);


				DTAPath path_element;
				path_element.Init (0,g_Simulation_Time_Horizon);

			for(unsigned int ODSequenceNo = 0; ODSequenceNo < ODNodeSequenceVector.size()-1; ODSequenceNo++)
			{
			NodeSize= m_pNetwork->SimplifiedTDLabelCorrecting_DoubleQueue(ODNodeSequenceVector[ODSequenceNo], 0, ODNodeSequenceVector[ODSequenceNo+1], 1, 10.0f,PathLinkList,TotalCost, distance_flag, false, false,m_RandomRoutingCoefficient);   // Pointer to previous node (node)
			
			// update m_PathDisplayList
			if(NodeSize <= 1)
			{
				TRACE("error");
				return 0;
			}


				for (int i=0 ; i < NodeSize-1; i++)
				{

						path_element.m_LinkVector.push_back(PathLinkList[i]) ; //starting from m_NodeSizeSP-2, to 0

						CString label;
						label.Format ("%d", 1);
						path_element.m_PathLabelVector.push_back(label);

						DTALink* pLink = m_LinkNotoLinkMap[PathLinkList[i]];

					
						if(pLink!=NULL)
						{ 
							path_element.m_Distance += m_LinkNotoLinkMap[PathLinkList[i]]->m_Length ;
							path_element.m_NumberOfSensorsPassed += pLink->m_bSensorData;

						if(path_element.m_LinkVector.size()==1) // first link
						{
							path_element.m_TravelTime = pLink->GetDynamicTravelTime(g_Simulation_Time_Stamp,m_PrimaryDataSource );
						}else
						{
							path_element.m_TravelTime = path_element.m_TravelTime + pLink->GetDynamicTravelTime(path_element.m_TravelTime,m_PrimaryDataSource );

						}
						}
				}
			}  // for each origin sequence


			m_SelectPathNo = min( m_SelectPathNo, m_PathDisplayList.size()-1);
			if(m_PathDisplayList.size()> 0) 
			{
				string old_path_name  = m_PathDisplayList[m_SelectPathNo] .m_path_name ;
				m_PathDisplayList[m_SelectPathNo]  = path_element;
				m_PathDisplayList[m_SelectPathNo].m_path_name = old_path_name;
			
			}else{
					m_PathDisplayList.push_back (path_element);
					m_SelectPathNo = 0;
				}
			
				UpdateAllViews(0);

				if( g_pPathListDlg!= NULL && g_pPathListDlg->GetSafeHwnd())
					 g_pPathListDlg ->ReloadData();


	// calculate time-dependent travel time


		for(unsigned int p = 0; p < m_PathDisplayList.size(); p++) // for each path
		{
			DTAPath path_element = m_PathDisplayList[p];

			for(int t=0; t< g_Simulation_Time_Horizon; t+= TIME_DEPENDENT_TRAVLE_TIME_CALCULATION_INTERVAL)  // for each starting time
			{
				path_element.m_TimeDependentTravelTime[t] = t;  // t is the departure time

				for (int i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
				{
					DTALink* pLink = m_LinkNoMap[m_PathDisplayList[p].m_LinkVector[i]];
					if(pLink == NULL)
						break;

					path_element.m_TimeDependentTravelTime[t] += pLink->GetDynamicTravelTime(path_element.m_TimeDependentTravelTime[t], m_PrimaryDataSource );

				}

				path_element.m_TimeDependentTravelTime[t] -= t; // remove the starting time, so we have pure travel time;

				ASSERT(path_element.m_TimeDependentTravelTime[t]>=0);

				if( path_element.m_MaxTravelTime < path_element.m_TimeDependentTravelTime[t])
					path_element.m_MaxTravelTime = path_element.m_TimeDependentTravelTime[t];

				for(int tt=1; tt<TIME_DEPENDENT_TRAVLE_TIME_CALCULATION_INTERVAL; tt++)
				{
					path_element.m_TimeDependentTravelTime[t+tt] = path_element.m_TimeDependentTravelTime[t];
				}


				//                              TRACE("\n path %d, time at %d = %f",p, t,path_element.m_TimeDependentTravelTime[t]  );

			}

			path_element.UpdateWithinDayStatistics();

			/// calculate fuel consumptions
			for(unsigned int p = 0; p < m_PathDisplayList.size(); p++) // for each path
			{
				path_element = m_PathDisplayList[p];

				for(int t=0; t< 1440; t+= TIME_DEPENDENT_TRAVLE_TIME_CALCULATION_INTERVAL)  // for each starting time
				{
					float CurrentTime = t;
					float FuelSum = 0;
					float CO2EmissionsSum = 0;
					float CO2;


					for (int i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
					{
						DTALink* pLink = m_LinkNoMap[path_element.m_LinkVector[i]];

						ASSERT(pLink!=NULL);
						FuelSum += pLink->ObtainHistFuelConsumption(CurrentTime);
						CO2= pLink->ObtainHistCO2Emissions(CurrentTime);
						CO2EmissionsSum+=CO2;

						CurrentTime += pLink->ObtainHistTravelTime(CurrentTime);

						//                                      TRACE("\n path %d, time at %f, TT = %f, Fuel %f. FS %f",p, path_element.m_TimeDependentTravelTime[t], pLink->GetDynamicTravelTime(path_element.m_TimeDependentTravelTime[t] ,m_pDoc->m_PrimaryDataSource ),Fuel, FuelSum );

					}


					path_element.m_WithinDayMeanTimeDependentFuelConsumption[t] = FuelSum;
					path_element.m_WithinDayMeanTimeDependentEmissions[t]=CO2EmissionsSum;

					float value_of_time = 6.5f/60.0f;   // per min
					float value_of_fuel = 3.0f;  // per gallon
					float value_of_emissions = 0.24f;  // per pounds

					path_element.m_WithinDayMeanGeneralizedCost[t] = value_of_time* path_element.GetTimeDependentMOE(t,2)
						+ value_of_fuel* path_element.m_WithinDayMeanTimeDependentFuelConsumption[t]
					+ value_of_emissions*path_element.m_WithinDayMeanTimeDependentEmissions[t];

					for(int tt=1; tt<TIME_DEPENDENT_TRAVLE_TIME_CALCULATION_INTERVAL; tt++)
					{
						path_element.m_WithinDayMeanTimeDependentFuelConsumption[t+tt] = path_element.m_WithinDayMeanTimeDependentFuelConsumption[t];
						path_element.m_WithinDayMeanTimeDependentEmissions[t+tt] = path_element.m_WithinDayMeanTimeDependentEmissions[t];
						path_element.m_WithinDayMeanGeneralizedCost[t+tt] = path_element.m_WithinDayMeanGeneralizedCost[t];

					}
				}

			}


		}


/*
	if(g_pPathMOEDlg  && g_pPathMOEDlg ->GetSafeHwnd ())
	{
		m_PathMOEDlgShowFlag = true;
		if(m_PathDisplayList.size() > 0)
		{
			if(g_pPathMOEDlg==NULL)
			{
				g_pPathMOEDlg = new CDlgPathMOE();
				g_pPathMOEDlg->m_pDoc  = this;

				g_pPathMOEDlg->Create(IDD_DIALOG_PATHMOE);
			}
			g_pPathMOEDlg->InsertPathMOEItem();

			g_pPathMOEDlg->ShowWindow(SW_SHOW);
		}
	}
	*/
		return 1;
}


DTALink* CTLiteDoc::FindLinkFromSensorLocation(float x, float y, CString orientation)
{
	double Min_distance = m_NetworkRect.Width()/100;  // set the selection threshod

	std::list<DTALink*>::iterator iLink;

	int SelectedLinkID = -1;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		GDPoint p0, pfrom, pto;
		p0.x  = x; p0.y  = y;
		pfrom.x  = (*iLink)->m_FromPoint.x; pfrom.y  = (*iLink)->m_FromPoint.y;
		pto.x  = (*iLink)->m_ToPoint.x; pto.y  = (*iLink)->m_ToPoint.y;

		if(orientation.MakeUpper().Find('E')>0 && pfrom.x > pto.x)  // East, Xfrom should be < XTo
			continue;  //skip

		if(orientation.MakeUpper().Find('S')>0 && pfrom.y < pto.y)  // South, Yfrom should be > YTo
			continue;

		if(orientation.MakeUpper().Find('W')>0 && pfrom.x < pto.x)  // West, Xfrom should be > XTo
			continue;

		if(orientation.MakeUpper().Find('N')>0 && pfrom.y > pto.y)  // North, Yfrom should be < YTo
			continue;

		float distance = g_GetPoint2LineDistance(p0, pfrom, pto, m_UnitMile);

		if(distance >=0 && distance < Min_distance)
		{
			return (*iLink);

		}
	}

	return NULL;

}

void CapacityReduction::GenerateAdditionalDelayDistribution(float EntranceTime,int VehicleID)
{
// to do: 
	// 1. from mean, COV -> lognormal 
	// without incidents -> random capacity reduction due to irregularity 
	// with incidents, start with capacity reduction 
	// 2. additional delay

	int RandomSeed = VehicleID;

	for (int i=0;i<100;i++)
	{

	}

	// call g_GetRandomRatio() 100 times;
	// for incidents, first percentage

}