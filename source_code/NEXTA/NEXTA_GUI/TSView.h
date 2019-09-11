// TSView.h : interface of the CTimeSpaceView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSVIEW_H__81CB4DEC_FFC3_480A_9B92_163C4C8226DA__INCLUDED_)
#define AFX_TSVIEW_H__81CB4DEC_FFC3_480A_9B92_163C4C8226DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _MAX_LANE_SIZE 7

enum eNGSIMDisplayMode {NGSIM_trajectory = 0,NGSIM_density_profile, NGSIM_simulated_trajectory, NGSIM_simulated_density_profile};



class PointSensorData
{
public: 
	float DetectorLocalY;
	int sampling_time_interval_in_sec;
//input
	std::vector<int> PassingTimeStampVector;  // has been sorted

// for reliability analysis
	std::vector<int> PassingHeadwayVector;  // unit: second

	std::vector<float> PassingTravelTimeVector;  // unit: second: from upstream to here (DetectorLocalY)

	std::vector<float> EstimatedPassingTravelTimeVector;  // unit: second: from upstream to here (DetectorLocalY)
	// based sampling ratio

	void ConstructEsimatedTravelTimeVector()
	{
	 // input: true PassingTravelTimeVector
	 // input: sampling_time_interval_in_sec
	
	 // output: EstimatedPassingTravelTimeVector
	}


//output
	std::vector<int> CumulativeFlowVector;

	float GetFlowCount(int beginning_time_interval)
	{
	
		int flow_count = 0;
		
		if(beginning_time_interval < CumulativeFlowVector.size()-1)
		{
		flow_count =  CumulativeFlowVector[beginning_time_interval+1] - CumulativeFlowVector[beginning_time_interval] ;
		}else
		{
		 flow_count = 0;
		}
		 
		return flow_count;

	}


	void ConstructCumulativeFlowVector(int data_collection_time_interval_in_sec, int number_of_time_intervals)
	{
		CumulativeFlowVector.clear();

		int last_timestamp = PassingTimeStampVector[PassingTimeStampVector.size()-1];

		for(unsigned int i = 1 ; i < PassingTimeStampVector.size(); i++)
		{
		    int headway = PassingTimeStampVector[i] - PassingTimeStampVector[i-1];
		
			PassingHeadwayVector.push_back(headway);

		}

		CumulativeFlowVector.reserve (number_of_time_intervals+1);

		int t; 
		for(t= 0; t <  number_of_time_intervals; t++)
		{
			CumulativeFlowVector.push_back(0);  // setup vector first
		}

		int data_collection_time_interval = 0;
		int time_interval_index = data_collection_time_interval_in_sec*10;  // unit: 0.1 seconds
		int  v = 0;

		// Xuesong: even there are two loops for cumulative flow count there, let us make the logic simple first without worring about the complexity of the algorithm
		for(t= 0; t <  number_of_time_intervals; t++)
		{
			for(v = 0; v< PassingTimeStampVector.size(); v++)
			{
				if(PassingTimeStampVector[v] > t * data_collection_time_interval_in_sec*10)  // greater than the end of data collection time interval
				{
				break;
				}

			}
				CumulativeFlowVector[t] = v;

		}

	}
	
};

class LinkSensorData 
{
public:
	LinkSensorData()
	{
	DataCollectionTimeInterval_in_sec = 30;
	}
	float DetectorLocalY_Upstream;
	float DetectorLocalY_Downstream;
	float DetectorSpacing_;

	std::vector<int> SpaceCountVector;  // space count between cumulative flow counts of two sensors
	std::vector<int> SpaceScanCountVector;  // space-based scan count using image scanning
	std::vector<float> DensityVector;  // space mean speed
	std::vector<float> SpeedVector;  // space mean density
	std::vector<float> FlowVector;  // average flow rates
	std::vector<int> UpstreamCumulativeFlowVector;  // average flow rates


	std::vector<int> EstimatedSpaceScanCountVector;  // space-based scan count using image scanning



	float GetDensity(int time_in_sec)
	{
		int time_interval = time_in_sec/DataCollectionTimeInterval_in_sec;
		if( time_interval < DensityVector.size())
			return DensityVector[time_interval];
		else
			return 0;
	}

	float GetSpaceCount(int time_in_sec)
	{
		int time_interval = time_in_sec/DataCollectionTimeInterval_in_sec;
		if( time_interval < SpaceCountVector.size())
			return SpaceCountVector[time_interval];
		else
			return 0;
	}

	float GetSpaceScanCount(int time_in_sec)
	{
		int time_interval = time_in_sec/DataCollectionTimeInterval_in_sec;
		if( time_interval < SpaceScanCountVector.size())
			return SpaceScanCountVector[time_interval];
		else
			return 0;
	}

	float GetEstimatedSpaceScanCount(int time_in_sec)
	{
		int time_interval = time_in_sec/DataCollectionTimeInterval_in_sec;
		if( time_interval < EstimatedSpaceScanCountVector.size())
			return EstimatedSpaceScanCountVector[time_interval];
		else
			return 0;
	}

	int GetCumulativeFlowCount(int time_in_sec)
	{
		int time_interval = time_in_sec/DataCollectionTimeInterval_in_sec;
		if( time_interval < UpstreamCumulativeFlowVector.size())
			return UpstreamCumulativeFlowVector[time_interval];
		else
			return 0;
	}

	int GetFlowCount(int time_in_sec)
	{
		return GetCumulativeFlowCount(time_in_sec) - GetCumulativeFlowCount(time_in_sec-DataCollectionTimeInterval_in_sec);
	}

	int DataCollectionTimeInterval_in_sec;
	int TimeHorizon_in_sec;

	void SetupLinkMOEVector(int TimeHorizon_in_sec)
	{
		DensityVector.clear();
		SpeedVector.clear();
		FlowVector.clear();
		SpaceCountVector.clear();
		SpaceScanCountVector.clear();

		TimeHorizon_in_sec = TimeHorizon_in_sec;
		for(int t = 0; t < TimeHorizon_in_sec/DataCollectionTimeInterval_in_sec; t++)
		{
		DensityVector.push_back (0);
		SpeedVector.push_back(0);
		FlowVector.push_back(0);
		SpaceCountVector.push_back(0);
		SpaceScanCountVector.push_back(0);
		}
	
	}

	void ConstructDensityFromTwoDetectors(PointSensorData detector1, PointSensorData detector2, int data_collection_interval_in_sec, float detector_spacing_in_feet)
	{
		UpstreamCumulativeFlowVector = detector1.CumulativeFlowVector;

		DataCollectionTimeInterval_in_sec = data_collection_interval_in_sec;
		DetectorSpacing_ = detector_spacing_in_feet/5280;
	
		int TimeInterval = min(detector1.CumulativeFlowVector.size(), detector2.CumulativeFlowVector.size());
		{
		SetupLinkMOEVector(TimeInterval*DataCollectionTimeInterval_in_sec);
		}

		for(int t = 0; t < DensityVector.size(); t++)
		{

		SpaceCountVector[t] = abs(detector1.CumulativeFlowVector [t] - detector2.CumulativeFlowVector [t]);
		DensityVector[t] = abs(detector1.CumulativeFlowVector [t] - detector2.CumulativeFlowVector [t])/DetectorSpacing_;
		// approximation using mean
		FlowVector[t] = (detector1.GetFlowCount(t) + detector2.GetFlowCount(t))/2 * (3600/DataCollectionTimeInterval_in_sec);  // per hour volume

		//q= kv --> v = q/k
		SpeedVector[t] = 	FlowVector[t] / max(1,DensityVector[t]);
		}

	
	}

};
class CorridorSensorData
{
public: 

	CorridorSensorData()
	{
	Jam_density_veh_per_mile_per_lane = 200;
	backward_wave_speed_in_mile_per_hour = 15;
	}

	std::vector <PointSensorData> PointSensorDataVector;
	std::vector <LinkSensorData> LinkSensorDataVector;

	std::vector<int> UpstreamCumulativeFlowVector;
	std::vector<int> DownstreamCumulativeFlowVector;

	float LocalY_Upstream;
	float LocalY_Downstream;

	int DataCollectionTimeInterval_in_sec;

	float Jam_density_veh_per_mile_per_lane;
	float backward_wave_speed_in_mile_per_hour;

	void EstimateSpaceScanCountVectorForAllLinks()
	{
		for(int link_index = 0; link_index < LinkSensorDataVector.size(); link_index++)
		{
		for(int t = 0; t < LinkSensorDataVector[link_index].DensityVector.size(); t+= 1)
		{
			// estimate cell based upstream count

			int link_based_cumulative_upstream_count = 0;
			int link_based_cumulative_downstream_count = 0;
			float distance_to_downstream_in_mile = (LocalY_Downstream - LinkSensorDataVector[link_index].DetectorLocalY_Upstream)/5280;  // convert from feet to mile

			int time_in_sec = t*DataCollectionTimeInterval_in_sec;

			int CellUpstreamCount, CellDownstreamCount;

			if(t == 27)
			{
			TRACE("nnn");
			}
			int time_stamp_backward = time_in_sec - distance_to_downstream_in_mile/backward_wave_speed_in_mile_per_hour*3600; // 3600 converts hour to seconds


			if(time_stamp_backward >= 0 && time_stamp_backward < DownstreamCumulativeFlowVector.size())
			{
			link_based_cumulative_upstream_count = DownstreamCumulativeFlowVector[time_stamp_backward] + distance_to_downstream_in_mile*Jam_density_veh_per_mile_per_lane;

			CellUpstreamCount = DownstreamCumulativeFlowVector[time_stamp_backward];

			}

			// estimate cell based downstream count

			distance_to_downstream_in_mile = (LocalY_Downstream - LinkSensorDataVector[link_index].DetectorLocalY_Downstream)/5280;
			time_stamp_backward = time_in_sec - distance_to_downstream_in_mile/backward_wave_speed_in_mile_per_hour*3600; // 3600 converts hour to seconds

			if(time_stamp_backward >= 0 && time_stamp_backward < DownstreamCumulativeFlowVector.size())
			{
			link_based_cumulative_downstream_count = DownstreamCumulativeFlowVector[time_stamp_backward] + distance_to_downstream_in_mile*Jam_density_veh_per_mile_per_lane;
			CellDownstreamCount = DownstreamCumulativeFlowVector[time_stamp_backward];
			}
			// take the difference
			int count  = max(0,link_based_cumulative_upstream_count - link_based_cumulative_downstream_count);
			LinkSensorDataVector[link_index].EstimatedSpaceScanCountVector.push_back(count);

		}
		}
	
	}

	void ConstructCumulativeFlowCount(std::vector<int> PassingTimeStampVector, int time_horizon_in_sec)
	{

		// Xuesong: even there are two loops for cumulative flow count there, let us make the logic simple first without worring about the complexity of the algorithm
		for(int t= 0; t < time_horizon_in_sec ; t++)
		{
			if(t >= 120)
				TRACE("");
			
			int v;
			for(v = 0; v< PassingTimeStampVector.size(); v++)
			{
				if(PassingTimeStampVector[v] > t * 10)  // greater than the end of data collection time interval
				{
				break;
				}

			}
				DownstreamCumulativeFlowVector.push_back(v);

		}
		TRACE("");
	}

	void CreateLinkSensorDataFromPointSensorVector()
	{
		if(PointSensorDataVector.size () <2)
			return;
	
		for(int d = 1; d < PointSensorDataVector.size(); d++)
		{
			LinkSensorData element;
			element.DetectorLocalY_Upstream = PointSensorDataVector[d-1].DetectorLocalY;
			element.DetectorLocalY_Downstream = PointSensorDataVector[d].DetectorLocalY;
			element.ConstructDensityFromTwoDetectors(PointSensorDataVector[d-1],
			PointSensorDataVector[d], 
			DataCollectionTimeInterval_in_sec,
			PointSensorDataVector[d].DetectorLocalY - PointSensorDataVector[d-1].DetectorLocalY);
		
			LinkSensorDataVector.push_back (element);
		}
	
	}

};


class VehicleSnapshotData
{
public:
	VehicleSnapshotData()
	{
		bDataAvailableFlag = false;
		CalculatedSpeed_mph = 0;
	}

	int Frame_ID;
	int VehicleID;

	double LocalX;
	double LocalY;
	float CalculatedSpeed_mph;
	float Speed_mph;
	int VehicleClass;
	float Acceleration;
	int LaneID;
	int PrecedingVehicleID;
	int FollowingVehicleID;
	bool bDataAvailableFlag;
};
class CTimeSpaceView : public CView
{
protected: // create from serialization only
	CTimeSpaceView();
	DECLARE_DYNCREATE(CTimeSpaceView)

// Attributes
public:

	int m_DataCollectionTimeInternval_in_sec;

	CorridorSensorData m_CorridorSensorData;
	CPoint m_TempLinkStartPoint, m_TempLinkEndPoint;
	int m_SelectedVehicleCount; 
	bool m_bMouseDownFlag;

	
	bool m_bLoadedDataFlag;
	bool m_bAutoLoadDataFlag;
	void LoadVehicleTrajectoryFile(CString file_name);

	int m_NumberOfVehicles_lane;

	int m_lane_size;
	// processing NGSim data
	bool m_bShowSimulationDataFlag;
	bool m_bShowVehicleIDFlag;
	bool m_bShowLeaderPositionFlag;

	bool m_bShowBothResultsFlag;

	bool m_bColorBySpeedFlag; 

	VehicleSnapshotData ** m_VehicleSnapshotAry;
	int m_NumberOfVehicles;
	int m_NumberOfTimeSteps;  // 0.1 second as resultion

	std::map<int, int> m_VehicleIDtoNOMap;
	std::vector<VehicleCFData> m_VehicleDataList;
	int m_SelectedVehicleID;

	std::vector<int> m_VehicleNoVector_Lane;
	int FindPreceedingVehicleNo(int LaneNo,float LocalY, int t);
	int FindFollowingVehicleNo(int LaneNo,float LocalY, int t);


	float *** m_VehicleSnapshotAry_Lane; // simulation

	int ** m_VehicleStartTimeAry_Lane; // simulation
	int ** m_VehicleEndTimeAry_Lane; // simulation

	int StartTimeLane[_MAX_LANE_SIZE];  // for the first vehicle
	int EndTimeLane[_MAX_LANE_SIZE];

	float StartLocalYLane[_MAX_LANE_SIZE];
	float EndLocalYLane[_MAX_LANE_SIZE];

	int VehicleSizeLane[_MAX_LANE_SIZE];

	int m_SelectLaneID;

	void UpdateVehicleStartANDEndTimes(std::vector<int> &StartPassingTimeStamp, std::vector<int> &EndPassingTimeStamp);

	float m_KJam; // in veh/mile
	float m_WaveSpeed; // in mph 
	float m_FreeflowSpeed; // in mph 


	// end of processing NGSim data

	bool bRangeInitialized;

	CTLiteDoc* GetTLDocument();

	bool m_bShowResourcePrice;
	bool m_bShowNodeCostLabel;

	bool m_bShowTimetable; 
	bool m_bShowSlackTime;

	bool m_bShowCumulativeFlowCount; 
	bool m_bShowDensityContour; 
	bool m_bShowPDFPlot;


	eNGSIMDisplayMode Cur_MOE_type1; // 1: upper window
	eNGSIMDisplayMode Cur_MOE_type2; // 2: lower window

	bool m_bMoveDisplay;
	CPoint m_last_cpoint;

   float m_UnitDistance, m_UnitTime;
   int m_Range;
   int m_TimeLeft, m_TimeRight;
   float m_LocalYUpperBound;
   float m_LocalYLowerBound;

   double m_TotalDistance;

   CRect m_PlotRectNGSIM;
   CRect m_PlotRectSimulated;

   int m_SelectedStartTime,m_SelectedEndTime;
   float m_SelectedStartLocalY,m_SelectedEndLocalY;

   // Operations
public:
	void InitializeTimeRange();
	void DrawTemporalLink(CPoint start_point, CPoint end_point);

	int GetTimestampFromPointX (int TimeXPosition, CRect PlotRect)
	{
		int timestamp = 0;
		timestamp = (TimeXPosition - PlotRect.left) /m_UnitTime + m_TimeLeft;
		
		if( timestamp < m_TimeLeft) timestamp = m_TimeLeft;
		if( timestamp > m_TimeRight) timestamp = m_TimeRight;

		return max(0,timestamp);
	}

	int GetLocalYFromPointY (int YPosition, CRect PlotRect)
	{
		float local_y = 0;

		local_y = (PlotRect.bottom - YPosition  )/m_UnitDistance + m_LocalYLowerBound;

		if(local_y < m_LocalYLowerBound) local_y = m_LocalYLowerBound;
		if(local_y > m_LocalYUpperBound) local_y = m_LocalYUpperBound;

		return max(0,local_y);;
	}

	int CountVehicles(int StartTime, int EndTime, float StartLocalY,float EndlocalY);

	void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}
	void GetColorFromPower(float power)
{
			float n= power*100;
			int R=(int)((255*n)/100);
			int G=(int)(255*(100-n)/100); 
			int B=0;


}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeSpaceView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool ExportDataToFile(char csv_file[_MAX_PATH], int DataFlag);
void RefreshWindow();
//	void DrawSchedule(int ScenarioNo);
//	void DrawTrain(int TrainNo);

	int m_MaxBinValue;  // 20 seconds
	int m_BinSize;
	int m_MaxFrequency;


	void SetupMaxFrequencyAndBinValue();
	void DrawPDFPlot(CDC* pDC,CRect PlotRect, std::vector<int> ValueVector1, std::vector<int> ValueVector2, int MaxBinValue, int BinSize,int MaxFrequency);

	void DrawSpaceTimeContour(CDC* pDC,eNGSIMDisplayMode MOEType,CRect PlotRect);
	void DrawPDFPlotsForAllLinks(CDC* pDC,eNGSIMDisplayMode MOEType,CRect PlotRect);
	void DrawNGSIMVehicleTrajectory(CDC* pDC,eNGSIMDisplayMode MOEType,CRect PlotRect);
	void DrawFramework(CDC* pDC,int MOEType,CRect PlotRect);

	int FindClosestTimeResolution(double Value)
{

		int ResolutionVector[6] = {100,300,600,1200,3000,15000};
		double min_distance  = 9999999;

		Value = max(1,Value/6);  //1/6 of time horizion as resolution

		int ClosestResolution=1;
		for(int i=0; i<6;i++)
		{
			if(	fabs(Value-ResolutionVector[i]) < min_distance)
			{
				min_distance = fabs(Value-ResolutionVector[i]);
				ClosestResolution = ResolutionVector[i];
			}
		}
		return ClosestResolution;
};

	
	virtual ~CTimeSpaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	void ConstructCellBasedDensityProfile();

// Generated message map functions
protected:
	//{{AFX_MSG(CTimeSpaceView)

	void OnToolsLoadvehicletrajactoryfile_backup();

	afx_msg void OnShowTimetable();
	afx_msg void OnUpdateShowTimetable(CCmdUI* pCmdUI);
	afx_msg void OnShowSlacktime();
	afx_msg void OnUpdateShowSlacktime(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimetableResourceprice();
	afx_msg void OnUpdateTimetableResourceprice(CCmdUI *pCmdUI);
	afx_msg void OnTimetableNodecostlabel();
	afx_msg void OnUpdateTimetableNodecostlabel(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnToolsLoadvehicletrajactoryfile();
	afx_msg void OnNgsimdataLane1();
	afx_msg void OnNgsimdataLane2();
	afx_msg void OnNgsimdataLane3();
	afx_msg void OnNgsimdataLane4();
	afx_msg void OnNgsimdataLane5();
	afx_msg void OnNgsimdataLane6();
	afx_msg void OnNgsimdataLane7();
	afx_msg void OnNgsimdataColorspeed();
	afx_msg void OnUpdateNgsimdataColorspeed(CCmdUI *pCmdUI);
	afx_msg void OnNgsimdataCarfollowingsimulation();
	afx_msg void OnNgsimdataShowsimulatedresults();
	afx_msg void OnUpdateNgsimdataShowsimulatedresults(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNgsimdataShowbothobservedandsimulationresults(CCmdUI *pCmdUI);
	afx_msg void OnNgsimdataShowbothobservedandsimulationresults();
	afx_msg void OnNgsimdataChangesettingsofcarfollowingmodel();
	afx_msg void OnNgsimdataCarfollowingdataextraction();
	afx_msg void OnNgsimdataShowvehicleid();
	afx_msg void OnUpdateNgsimdataShowvehicleid(CCmdUI *pCmdUI);
	afx_msg void OnNgsimdataShowpreceedingvehicleposition();
	afx_msg void OnUpdateNgsimdataShowpreceedingvehicleposition(CCmdUI *pCmdUI);
	afx_msg void OnNgsimdataSearchByVehicleId();
	afx_msg void OnNgsimmenuExportcumulativeflowcountdata();
	afx_msg void OnNgsimmenuShowcalculatedcumulativeflowcountanddensity();
	afx_msg void OnUpdateNgsimmenuShowcalculatedcumulativeflowcountanddensity(CCmdUI *pCmdUI);
	afx_msg void OnNgsimmenuShowspacetimeContour();
	afx_msg void OnUpdateNgsimmenuShowspacetimeContour(CCmdUI *pCmdUI);
	afx_msg void OnNgsimmenuShowcdfplot();
	afx_msg void OnUpdateNgsimmenuShowcdfplot(CCmdUI *pCmdUI);
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TSVIEW_H__81CB4DEC_FFC3_480A_9B92_163C4C8226DA__INCLUDED_)
