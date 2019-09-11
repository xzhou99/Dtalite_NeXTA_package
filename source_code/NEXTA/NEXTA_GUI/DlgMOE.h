//  Portions Copyright 2010 Xuesong Zhou (xzhou99@gmail.com)

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

#pragma once
#include <vector>
#include "TLiteDoc.h"
#include "BaseDialog.h"
#include "afxwin.h"

// CDlgMOE dialog

class CTimeSeriesLine 
{
	public:
		CTimeSeriesLine()
		{
		bReferenceMode = 0;
		}
		std::map<int, float> value_map;
		CString time_series_label;
		bool bReferenceMode;
};

class CDlgMOE : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgMOE)
	
public:
	std::vector<int> m_AggregationValueVector;
	float m_HourlyBackgroundFlow;
	int m_CarFollowingTimeResolutionPerMin;  // unit: x intervals per min
	CTLiteDoc* m_pDoc;  // main document
	CTLiteDoc* m_pDoc2; // alternative document
	bool m_bDoc2Ready;

	float ** m_VehicleDistanceAry;
	int m_LinkNoWithCFData;

	std::vector<CTimeSeriesLine> m_DataVector;

	std::vector<VehicleCFData> m_VehicleDataList;

	int m_NumberOfVehicles;
	void CarFollowingSimulation();
	int m_DisplayVehicleResolution;

	int m_MinDisplayInterval;

	CDlgMOE::CDlgMOE(CWnd* pParent =NULL)
	: CBaseDialog(CDlgMOE::IDD, pParent)
{

	m_bFixYRange = false;
	m_NumberOfVehicles = 0;
	m_pDoc = NULL;
	m_pDoc2 = NULL; // alternative document
	m_bDoc2Ready = false;

	m_HourlyBackgroundFlow = 1000;
	m_MinDisplayInterval = 10;
	m_DisplayVehicleResolution = 1;

	m_LinkNoWithCFData = -1;
	m_CarFollowingTimeResolutionPerMin = 100;  // 10 intervals per min, 6 seconds

	m_YUpperBound = 100;
	m_YLowerBound = 0;
	m_TimeLeft = 0;
	m_Range = g_Simulation_Time_Horizon;
	m_TimeRight = m_Range;  // use 5 days as starting show
	Cur_MOE_type1 = lane_volume;
	Cur_MOE_type2 = no_display;

	m_bMoveDisplay = false;

	m_bShowHistPattern = false;
	m_bShowVariability = false;
	m_bShowEventLabel = true;
	m_bShowWeatherLabel = false;
	m_bShowPrediction = false;
	m_bShowSensorData  = true;
	m_bShowSimulationData = true;

	m_ViewMode = 0;

	m_K_Jam = 250;
	m_Max_Flow =2500;

	}
	virtual ~CDlgMOE();

	double m_K_Jam;
	double m_Max_Flow;

	CPoint m_last_cpoint;
	bool m_bMoveDisplay;

   int m_ViewMode;  // time series, QK curve

   void DrawSingleQKPlot(CPaintDC* pDC, CRect PlotRect);
   void DrawSingleVKPlot(CPaintDC* pDC, CRect PlotRect);
   void DrawSingleVQPlot(CPaintDC* pDC, CRect PlotRect);


   void DrawTimeSeriesPlot();
   void DrawQKCurve();
   void ExportData(int EventType);

   eLinkMOEMode Cur_MOE_type1;
   eLinkMOEMode Cur_MOE_type2;

   bool m_bShowSensorData;
   bool m_bShowSimulationData;
   bool m_bShowGrid;
  bool m_bShowHistPattern;
   bool m_bShowPrediction;
   bool m_bShowVariability;
   bool m_bShowEventLabel;
   bool m_bShowWeatherLabel;
   float m_UnitData, m_UnitTime;

  int m_max_project_string_length;
  void GetMaxLabelLength();

   int m_Range;
   bool m_bFirstLoadingFlag;
   int m_TimeLeft, m_TimeRight;
   float m_YUpperBound;
   float m_YLowerBound;

   COLORREF itsBackgroundColor;

	void DrawPlot(CPaintDC* pDC,eLinkMOEMode MOEType, CRect PlotRect, bool LinkTextFlag);
	int m_LinkDisplayCount;
	void DrawTimeSeries(eLinkMOEMode MOEType , CPaintDC* pDC, CRect PlotRect, bool LinkTextFlag);
	void DrawVehicleTrajectory(eLinkMOEMode MOEType , CPaintDC* pDC, CRect PlotRect, bool LinkTextFlag);

	void DrawEventCode(eLinkMOEMode  MOEType , CPaintDC* pDC, CRect PlotRect,bool TextFlag);
	bool ExportDataToCSVFile(CString csv_file);
    float GetMaxYValue(eLinkMOEMode MOEType);

	bool m_bFixYRange;
// Dialog Data
	enum { IDD = IDD_DIALOG_MOE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnMoetypeLinkvolume();
	afx_msg void OnMoetypeSpeed();
	afx_msg void OnMoetypeCumulativevolume();
	afx_msg void OnUpdateMoetypeLinkvolume(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoetypeSpeed(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoetypeCumulativevolume(CCmdUI *pCmdUI);
	afx_msg void OnMoetype2Linkvolume();
	afx_msg void OnMoetype2Speed();
	afx_msg void OnMoetype2Cumulativevolume();
	afx_msg void OnMoetype2None();
	afx_msg void OnDataExport();
	afx_msg void OnMoetype1Linkvolume();
	afx_msg void OnMoetype1Speed();
	afx_msg void OnMoetype2LinkvolumeVph();
	afx_msg void OnMoetype2SpeedMph();
	afx_msg void OnViewMoetimeseries();
	afx_msg void OnViewQ();
	afx_msg void OnMoetype3Q();
	afx_msg void OnMoetype2Density();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnViewResettimerange();
	afx_msg void OnEstimationHistoricalavgpattern();
	afx_msg void OnUpdateEstimationHistoricalavgpattern(CCmdUI *pCmdUI);
	afx_msg void OnViewMoevariabilityplot();
	afx_msg void OnUpdateViewMoevariabilityplot(CCmdUI *pCmdUI);
	afx_msg void OnMoetype1Traveltime();
	afx_msg void OnEstimationPrediction();
	afx_msg void OnMoetype3None();
	afx_msg void OnMoetype1Density();
	afx_msg void OnUpdateMoetype1Density(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);
	afx_msg void OnEstimationShoweventlabel();
	afx_msg void OnUpdateEstimationShoweventlabel(CCmdUI *pCmdUI);
	afx_msg void OnDataExportWeatherData();
	afx_msg void OnDataExportIncidentData();
	afx_msg void OnDataExportHighDemandData();
	afx_msg void OnDataExportSpecicalEventData();
	afx_msg void OnEstimationShowweatherdata();
	afx_msg void OnUpdateEstimationShowweatherdata(CCmdUI *pCmdUI);
	afx_msg void OnMoetype1Vehicletrajectory();
	afx_msg void OnMoetype1Vehicletrajectory10();
	afx_msg void OnMoetype1Vehicletrajectory30();
	afx_msg void OnMoetypeCumulativevolumeOblique();
	afx_msg BOOL OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDataExportvehicletrajectory();
	afx_msg void OnUpdateEstimationPrediction(CCmdUI *pCmdUI);
	afx_msg void OnViewSimulationvs();
	afx_msg void OnUpdateViewSimulationvs(CCmdUI *pCmdUI);
	afx_msg void OnMoetype1Queuelength();
	afx_msg void OnMoetype2Queuelength();
	afx_msg void OnMoetype1Linkoutflowvolume();
	afx_msg void OnDataFixyrange();
	afx_msg void OnUpdateDataFixyrange(CCmdUI *pCmdUI);
	CComboBox m_StartHour;
	CComboBox m_EndHour;
	afx_msg void OnCbnSelchangeComboStarttime();
	afx_msg void OnCbnSelchangeComboEndtime();
	CComboBox m_AggregationIntervalList;
	afx_msg void OnCbnSelchangeComboAgginterval();
	afx_msg void OnMoetype2Linkoutflowvolume();
	afx_msg void OnMoetype1Linkinandoutflow();
	afx_msg void OnMoetype2Linkinandoutflow();
	afx_msg void OnMoetype1Linktraveltime();
	afx_msg void OnMoetype2Linktraveltime();
	CComboBox m_SimuDayNo;
	CComboBox m_SensorDayNo;
	afx_msg void OnCbnSelchangeComboSimudayno();
	afx_msg void OnCbnSelchangeComboSensordayno();
	afx_msg void OnMoe2Energy();
	afx_msg void OnMoeEnergy();
	afx_msg void OnMoeCo2();
	afx_msg void OnMoeCo();
	afx_msg void OnMoeHc();
	afx_msg void OnMoeNox();
	afx_msg void OnMoe2Co2();
	afx_msg void OnMoe2Co();
	afx_msg void OnMoe2Hc();
	afx_msg void OnMoe2Nox();
	afx_msg void OnDataShowsimulationtimeseries();
	afx_msg void OnUpdateDataShowsimulationtimeseries(CCmdUI *pCmdUI);
	afx_msg void OnDataShowgrid();
	afx_msg void OnUpdateDataShowgrid(CCmdUI *pCmdUI);
	afx_msg void OnMoeNumberofqueuedvehicles();
	afx_msg void OnMoeNumberofqueuedvehicles33788();
};
