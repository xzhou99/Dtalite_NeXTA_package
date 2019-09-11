// TLiteView.h : interface of the CTLiteView class
//
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
#include <afxstr.h>
#include <atlimage.h>
#include <comdef.h>

#include "VISSIM_EXPORT\\VISSIM_Export.h"

enum tool
   { move_tool, 
   select_link_tool, 
   select_feature_tool, 
   backgroundimage_tool,
   network_coordinate_tool, 
   create_1waylink_tool,
   create_2waylinks_tool, 
   create_node_tool, 
   subarea_tool,
   move_node_tool,
   add_zone_tool,
   sensor_location_move_tool,
   network_location_move_tool

   };

enum link_text_display_mode
   {
   link_display_none = 0, 
   link_display_street_name, 
   link_display_speed_limit_, 
   link_display_length_, 
   link_display_number_of_lanes, 
   link_display_link_capacity_per_hour,
   link_display_lane_capacity_per_hour,
   link_display_total_link_volume,
   link_display_LevelOfService,
	link_display_avg_simulated_speed_mph,
	link_display_avg_travel_time,
	link_display_avg_delay,
   link_display_link_id, 
   link_display_speed_sensor_id, 
   link_display_count_sensor_id, 
   link_display_demand_type_code,

   link_display_from_id_to_id, 


   link_display_free_flow_travel_time_in_min, 
   link_display_free_flow_travel_time_in_hour, 
   link_display_orientation_code,
   link_display_loop_code,

   link_display_number_of_right_turn_lanes, 

   link_display_length_of_left_turn_lanes, 
   link_display_length_of_right_turn_lanes, 

   link_display_separator_1,
   link_display_saturation_flow_rate,
   link_display_link_grade,
   link_display_jam_density_in_vhc_pmpl,
   link_display_wave_speed_in_mph,

   link_display_BPR_alpha_term,
   link_display_BPR_beta_term,

   link_display_link_type_in_text,
   link_display_link_type_in_number,
    link_display_internal_link_id,
	link_display_link_id_speed_sensorid,

	link_display_separator_3,

	link_display_total_delay,
	link_display_volume_over_capacity_ratio,

	link_display_avg_simulated_speed_kmph,

	link_display_avg_waiting_time_on_loading_buffer,

	link_display_time_dependent_link_volume,
	link_display_time_dependent_lane_volume,
	link_display_time_dependent_speed_mph,
	link_display_time_dependent_speed_kmph,
	link_display_time_dependent_density,
	link_display_time_dependent_queue_length,

	link_display_time_dependent_congestion_duration,
	link_display_time_dependent_congestion_start_time,

	link_display_separator_4,
	link_display_total_sensor_link_volume,
	link_display_total_sensor_vs_simulated_link_volume,
	link_display_total_link_count_error,
	link_display_simulated_AADT,
	link_display_observed_AADT,
	link_display_observed_peak_hourly_volume,


	link_display_separator_5,
	link_display_Num_Driveways_Per_Mile,
	link_display_volume_proportion_on_minor_leg,
	link_display_Num_3SG_Intersections, 
	link_display_Num_3ST_Intersections, 
	link_display_Num_4SG_Intersections,
	link_display_Num_4ST_Intersections,

	link_display_crash_prediction_group_1_code,
	link_display_crash_prediction_group_2_code,
	link_display_crash_prediction_group_3_code,
	link_display_number_of_crashes,
	link_display_num_of_fatal_and_injury_crashes_per_year,
	link_display_num_of_PDO_crashes_per_year,

	link_display_number_of_intersection_crashes,
	link_display_num_of_intersection_fatal_and_injury_crashes_per_year,
	link_display_num_of_intersection_PDO_crashes_per_year,


	// reserve
	link_display_total_assigned_link_volume,
	link_display_total_incomplete_link_volume,
	link_display_number_of_left_turn_lanes

};
enum movement_text_display_mode
   { 
   movement_display_none = 0, 
   movement_display_turn_type, 
   movement_display_turn_direction,

   movement_display_QEM_Lanes,
   movement_display_sim_turn_hourly_count,
   movement_display_sim_turn_percentage, 
   movement_display_sim_turn_delay, 
   movement_display_QEM_Phase1,
   movement_display_QEM_EffectiveGreen,

   movement_display_turn_up_node_number, 
   movement_display_turn_dest_node_number, 
   movement_display_turn_three_node_numbers, 
   movement_display_turn_protected_permited_prohibited,
   movement_display_sim_turn_count, 



   movement_display_sim_turn_delay_in_min, 
   movement_display_obs_turn_hourly_count,
   movement_display_obs_turn_percentage, 
   movement_display_obs_turn_delay,

   movement_display_QEM_Shared,
   movement_display_QEM_Width,
   movement_display_QEM_Storage,
   movement_display_QEM_StLanes,
   movement_display_QEM_Grade,
   movement_display_QEM_Speed,
   movement_display_QEM_IdealFlow,
   movement_display_QEM_LostTime,
   movement_display_QEM_PermPhase1,
   movement_display_QEM_DetectPhase1,

   movement_display_QEM_TurnVolume,
   movement_display_QEM_TurnPercentage,

   movement_display_QEM_Capacity,
   movement_display_QEM_VOC,
   movement_display_QEM_SatFlow,
   movement_display_QEM_Delay,
   movement_display_QEM_LOS,
};

enum node_display_mode
   { node_display_none = 0, 
   node_display_node_number, 
   node_display_sequential_node_number, 
   node_display_zone_number, 
   node_display_intersection_name,
   node_display_control_type,
   node_display_cycle_length_in_second,
   node_display_cycle_length_in_second_for_signal_only, 
   node_display_offset_in_second_for_signal_only, 
   node_display_travel_time_from_origin

};

enum GPS_display_mode
   { GPS_display_none = 0, 
     GPS_display_vehicle_id, 
     GPS_display_timestamp_in_min, 
     GPS_display_timegap_in_min, 
     GPS_display_speed,
     GPS_display_all 
};

enum link_display_mode
   { link_display_mode_line, link_display_mode_band };

// ARROWSTRUCT
//
// Defines the attributes of an arrow.
// source: http://www.codeproject.com/Articles/3274/Drawing-Arrows
typedef struct tARROWSTRUCT 
{
	int nWidth;		// width (in pixels) of the full base of the arrowhead
	float fTheta;	// angle (in radians) at the arrow tip between the two
					//  sides of the arrowhead
	bool bFill;		// flag indicating whether or not the arrowhead should be
					//  filled
} ARROWSTRUCT;

/////////////////
/// reference: http://www.codeproject.com/Articles/25237/Bezier-Curves-Made-Simple
///
/// 

   class BezierCurve
    {
		private :
		double FactorialLookup[33];

	public:	
        BezierCurve()
        {
            FactorialLookup[0] = 1.0;
            FactorialLookup[1] = 1.0;
            FactorialLookup[2] = 2.0;
            FactorialLookup[3] = 6.0;
            FactorialLookup[4] = 24.0;
            FactorialLookup[5] = 120.0;
            FactorialLookup[6] = 720.0;
            FactorialLookup[7] = 5040.0;
            FactorialLookup[8] = 40320.0;
            FactorialLookup[9] = 362880.0;
            FactorialLookup[10] = 3628800.0;
            FactorialLookup[11] = 39916800.0;
            FactorialLookup[12] = 479001600.0;
            FactorialLookup[13] = 6227020800.0;
            FactorialLookup[14] = 87178291200.0;
            FactorialLookup[15] = 1307674368000.0;
            FactorialLookup[16] = 20922789888000.0;
            FactorialLookup[17] = 355687428096000.0;
            FactorialLookup[18] = 6402373705728000.0;
            FactorialLookup[19] = 121645100408832000.0;
            FactorialLookup[20] = 2432902008176640000.0;
            FactorialLookup[21] = 51090942171709440000.0;
            FactorialLookup[22] = 1124000727777607680000.0;
            FactorialLookup[23] = 25852016738884976640000.0;
            FactorialLookup[24] = 620448401733239439360000.0;
            FactorialLookup[25] = 15511210043330985984000000.0;
            FactorialLookup[26] = 403291461126605635584000000.0;
            FactorialLookup[27] = 10888869450418352160768000000.0;
            FactorialLookup[28] = 304888344611713860501504000000.0;
            FactorialLookup[29] = 8841761993739701954543616000000.0;
            FactorialLookup[30] = 265252859812191058636308480000000.0;
            FactorialLookup[31] = 8222838654177922817725562880000000.0;
            FactorialLookup[32] = 263130836933693530167218012160000000.0;
        }

        // just check if n is appropriate, then return the result
        double factorial(int n)
        {
            if ( n >=0 && n < 32)
	            return FactorialLookup[n]; /* returns the value n! as a SUMORealing point number */
			else return 1;
        }


        double Ni(int n, int i)
        {
            double ni;
            double a1 = factorial(n);
            double a2 = factorial(i);
            double a3 = factorial(n - i);
            ni =  a1/ (a2 * a3);
            return ni;
        }

        // Calculate Bernstein basis
        double Bernstein(int n, int i, double t)
        {
            double basis;
            double ti; /* t^i */
            double tni; /* (1 - t)^i */

            /* Prevent problems with pow */

            if (t == 0.0 && i == 0) 
                ti = 1.0; 
            else 
                ti = pow(t, i);

            if (n == i && t == 1.0) 
                tni = 1.0; 
            else 
                tni = pow((1 - t), (n - i));

            //Bernstein basis
            basis = Ni(n, i) * ti * tni; 
            return basis;
        }

		void Bezier2D(std::vector<GDPoint> ShapePoints, std::vector<GDPoint> &NewShapePoints)
        {
			std::vector<double> b;
			int cpts = 100;
			double p[200]; // 400 is approximating points

			int i;
			for(i =0; i< ShapePoints.size(); i++)
			{
				b.push_back(ShapePoints[i].x);
				b.push_back(ShapePoints[i].y);
			}
            int npts = b.size() / 2;
            int icount, jcount;
            double step, t;

            // Calculate points on curve

            icount = 0;
            t = 0;
            step = (double)1.0 / (cpts - 1);

            for (int i1 = 0; i1 != cpts; i1++)
            { 
                if ((1.0 - t) < 5e-6) 
                    t = 1.0;

                jcount = 0;
                p[icount] = 0.0;
                p[icount + 1] = 0.0;
                for (int i = 0; i != npts; i++)
                {
                    double basis = Bernstein(npts - 1, i, t);
                    p[icount] += basis * b[jcount];
                    p[icount + 1] += basis * b[jcount + 1];
                    jcount = jcount +2;
                }

                icount += 2;
                t += step;
            }

			// send approimating point back
			for(i= 0 ; i< cpts; i++)
			{
				GDPoint pt;
				pt.x = p[2*i];
				pt.y = p[2*i+1];
				NewShapePoints.push_back(pt);
			}
			

		}

    };


struct IncomingLinkAngle
{ public:
	DTALink* pLink; 
	int Angle;
};



class CTLiteView : public CView
{
protected: // create from serialization only
	CTLiteView();
	DECLARE_DYNCREATE(CTLiteView)

// Attributes
public:

	CString m_PathNodeSequence;
	bool m_bShowTop10ODOnly;
	bool m_bShowTop20ODOnly;
	bool m_bShowTop50ODOnly;
	bool m_bShowTop100ODOnly;


	bool m_bUpdateLinkAttributeBasedOnType;
	bool bShowVehiclesWithIncompleteTrips;
		MicroSimulatorInterface m_ms;
		int m_msStatus;

		void CreateDefaultJunction();

	BezierCurve m_BezierCurve;

	bool m_bLineDisplayConditionalMode;
	link_display_mode m_link_display_mode;

void SetStatusText(CString text) const
{
   GetParentFrame()->SetMessageText(text);
}

bool RectIsInsideScreen(CRect rect, CRect screen_bounds)
{
      if( screen_bounds.top <= rect.top 
		  && screen_bounds.bottom >= rect.bottom 
		  && screen_bounds.left <= rect.left 
		  && screen_bounds.right >= rect.right)
		  return true;  // rect is entirely contained in screen_bounds

		CRect intersect_rect;
	  intersect_rect.IntersectRect (screen_bounds,rect);

	 return !intersect_rect.IsRectEmpty();
}
	bool isCreatingSubarea, isFinishSubarea;
	
	int m_SelectFromNodeNumber; 
	int m_SelectToNodeNumber; 

	bool m_bHighlightSubareaLinks;
	bool m_bHighlightSubareaBoundaryLinks;

	bool m_bShowODDemandVolume;
	bool m_bShowConnector;
	bool m_bHighlightActivityLocation;

	bool m_bShowTransit;
	bool m_bShowAVISensor;
	
	bool m_bShowReferenceNetwork;
	bool m_bShowSensorDecription;
	bool m_bShowSensorMatchedLink;
	LPPOINT m_subarea_points;
	// Create a polygonal region
    HRGN m_polygonal_region;

	CTLiteDoc* GetDocument() const;
	tool m_ToolMode; 
	bool m_bNetworkCooridinateHints;
	double m_GridResolution;
	CPoint m_last_cpoint;
	CPoint m_last_left_down_point;
	bool m_bMoveDisplay;

	bool m_bMoveImage;
	bool m_bMoveSensorMap;
	bool m_bMoveNetwork;
	bool m_bShowGrid;
	bool m_bShowLinkArrow;
	bool m_bShowNode;
	

	node_display_mode m_ShowNodeTextMode;
	GPS_display_mode m_ShowGPSTextMode;

	link_text_display_mode m_ShowLinkTextMode;
	movement_text_display_mode m_ShowMovementTextMode;

	double 	m_MovmentTextSize;
	double 	m_LinkTextSize;
	int m_NodeDisplayBoundarySize; 

	bool m_bShowVehicleNumber;
	bool m_bShowSelectedVehicleOnly;
	bool m_bShowImage;
	bool m_bShowLinkType;

	int m_VehicleSize;

   virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
   virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);


	void FitNetworkToScreen();
	void DrawObjects(CDC* pDC);
	void DrawBitmap(CDC *pDC, CPoint point,UINT nIDResource );
	void DrawPublicTransitLayer(CDC *pDC);
	void DrawPublicTransitAccessibilityLayer(CDC *pDC);
	void DrawTemporalLink(CPoint start_point, CPoint end_point);


	CPoint m_TempLinkStartPoint, m_TempLinkEndPoint;
	CPoint m_TempZoneStartPoint, m_TempZoneEndPoint;

	CPoint m_FirstSubareaPoints;

	bool bFindCloseSubareaPoint(CPoint pt)
	{
		if(GetDocument()->m_SubareaShapePoints.size()>0)
		{
			double value = (pt.x - m_FirstSubareaPoints.x)*(pt.x - m_FirstSubareaPoints.x) + (pt.y - m_FirstSubareaPoints.y)*(pt.y -m_FirstSubareaPoints.y);
		double distance = sqrt(value);
			if(distance < 10) 
				return true;

		}

		return false;
	}

	bool m_bMouseDownFlag;

	CPoint m_ScreenOrigin;
	GDPoint m_Origin;
	double m_Resolution;

	int m_OriginOnBottomFlag;

	int m_ViewID;

	void FollowGlobalViewParameters(int CurrentViewID)
	{
/*		if(GetDocument()->m_bSetView && m_ViewID!=CurrentViewID)
		{
		 GetDocument()->m_Doc_Origin = m_Origin;
		 GetDocument()->m_Doc_Resolution = m_Resolution;
		}
		 
*/
	}
	void SetGlobalViewParameters();


	void MoveNode(int NodeID, CPoint point)
	{

		CTLiteDoc* pDoc = GetDocument();

		DTANode * pNode = pDoc->m_NodeNoMap[NodeID];
		

		// update node's position

		pNode->pt = SPtoNP(point);

		// update all connected links' distance 

		for (std::list<DTALink*>::iterator iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
		{

			bool bConnected = false;
			if((*iLink)->m_FromNodeID == pNode->m_NodeNo )
			{
				(*iLink)->m_FromPoint = pNode->pt;
				bConnected = true;
			}

			if((*iLink)->m_ToNodeID == pNode->m_NodeNo )
			{
				(*iLink)->m_ToPoint = pNode->pt;
				bConnected = true;

			}

			if(bConnected)
			{
				if(pDoc->m_LongLatFlag)  // bLongLatFlag is user input,  m_LongLatFlag is the system input from the project file 
					(*iLink)->m_Length   =  g_CalculateP2PDistanceInMileFromLatitudeLongitude((*iLink)->m_FromPoint , (*iLink)->m_ToPoint);
				else 
					(*iLink)->m_Length   = (*iLink)->DefaultDistance()/max(0.0000001,pDoc->m_UnitMile);

				(*iLink)->UpdateShapePointsBasedOnEndPoints(pDoc->m_UnitFeet*pDoc->m_LaneWidthInFeet);  // 20 feet per lane)
			}
		}
	}

	char* m_NodeTypeFaceName;

    CPoint NPtoSP(GDPoint net_point) // convert network coordinate to screen coordinate
	{
		CPoint pt;
		pt.x = int((net_point.x-m_Origin.x)*m_Resolution+ m_ScreenOrigin.x+0.5);
		pt.y = int((net_point.y-m_Origin.y)*m_OriginOnBottomFlag*m_Resolution + m_ScreenOrigin.y+0.5);
		return pt;
	}

    GDPoint SPtoNP(CPoint screen_point)
	{
		GDPoint pt;
		pt.x = (screen_point.x-m_ScreenOrigin.x)/m_Resolution+m_Origin.x;
		pt.y = (screen_point.y-m_ScreenOrigin.y)/(m_OriginOnBottomFlag*m_Resolution)+m_Origin.y;
	
		return pt;
	}

	int FindClosestNode(CPoint point, float Min_distance);
	int FindClosestZone(CPoint point, float Min_distance);

	CPoint m_CurrentMousePoint;
	

// Operations
public:

	void FindAccessibleTripIDWithCurrentMousePoint();
	bool m_ShowAllPaths;

	void CopyLinkSetInSubarea();
		
	
	CPoint FromPoint, ToPoint; // avoid creating variables multiple times in drawing links
	GDPoint fromp, top;
	CPoint m_arrow_pts[3];
	CPoint m_BandPoint[2000];  // maximum 1000 feature points
	int m_LinkTextFontSize;
	int m_NodeTextFontSize;

	void DrawNode(CDC *pDC, DTANode* pNode, CPoint point, int node_size,TEXTMETRIC tm);
	void DrawNodePieChart(CDC *pDC, DTANode* pNode, CPoint point, int chart_size, int LOS);
	void DrawNodeRadarChart(CDC *pDC, DTANode* pNode, CPoint point, int chart_size, float delay_ratio, int LOS);



	

	void DrawLinkAsLine(DTALink* pLink, CDC* pDC);
	void DrawLinkAsStraightLine(DTALink* pLink, CDC* pDC);

	bool DrawLinkAsBand(DTALink* pLink, CDC* pDC, bool bObservationFlag);
	bool DrawLinkAsStraightBand(DTALink* pLink, CDC* pDC, bool bObservationFlag);
	bool DrawLinkTimeTable(DTALink* pLink, CDC* pDC, int DisplayMode);
	bool DrawLinkAsLaneGroup(DTALink* pLink, CDC* pDC);
	
void ArrowTo(HDC hDC, int x, int y, ARROWSTRUCT *pA) 
{

	POINT ptTo = {x, y};

	ArrowTo(hDC, &ptTo, pA);
}

void DrawMovementLink(CDC* pDC,GDPoint pt_from, GDPoint pt_to,int NumberOfLanes, double theta, int lane_width);
void DrawNodeMovements(CDC* pDC, DTANode* pNode, CRect PlotRect);


// ArrowTo()
//
void ArrowTo(HDC hDC, const POINT *lpTo, ARROWSTRUCT *pA) 
{

	POINT pFrom;
	POINT pBase;
	POINT aptPoly[3];
	float vecLine[2];
	float vecLeft[2];
	float fLength;
	float th;
	float ta;

	// get from point
	MoveToEx(hDC, 0, 0, &pFrom);

	// set to point
	aptPoly[0].x = lpTo->x;
	aptPoly[0].y = lpTo->y;

	// build the line vector
	vecLine[0] = (float) aptPoly[0].x - pFrom.x;
	vecLine[1] = (float) aptPoly[0].y - pFrom.y;

	// build the arrow base vector - normal to the line
	vecLeft[0] = -vecLine[1];
	vecLeft[1] = vecLine[0];

	// setup length parameters
	fLength = (float) sqrt(vecLine[0] * vecLine[0] + vecLine[1] * vecLine[1]);
	th = pA->nWidth / (2.0f * fLength);
	ta = pA->nWidth / (2.0f * (tanf(pA->fTheta) / 2.0f) * fLength);

	// find the base of the arrow
	pBase.x = (int) (aptPoly[0].x + -ta * vecLine[0]);
	pBase.y = (int) (aptPoly[0].y + -ta * vecLine[1]);

	// build the points on the sides of the arrow
	aptPoly[1].x = (int) (pBase.x + th * vecLeft[0]);
	aptPoly[1].y = (int) (pBase.y + th * vecLeft[1]);
	aptPoly[2].x = (int) (pBase.x + -th * vecLeft[0]);
	aptPoly[2].y = (int) (pBase.y + -th * vecLeft[1]);

	MoveToEx(hDC, pFrom.x, pFrom.y, NULL);

	// draw we're fillin'...
	if(pA->bFill) {
		LineTo(hDC, aptPoly[0].x, aptPoly[0].y);
		Polygon(hDC, aptPoly, 3);
	}

	// ... or even jes chillin'...
	else {
		LineTo(hDC, pBase.x, pBase.y);
		LineTo(hDC, aptPoly[1].x, aptPoly[1].y);
		LineTo(hDC, aptPoly[0].x, aptPoly[0].y);
		LineTo(hDC, aptPoly[2].x, aptPoly[2].y);
		LineTo(hDC, pBase.x, pBase.y);
		MoveToEx(hDC, aptPoly[0].x, aptPoly[0].y, NULL);
	}
}

bool m_bShowTransitAccessibility;
bool m_bShowProhibitedMovements;

bool m_bShowTransitLinksOnly;
bool m_bShowWalkLinksOnly;

bool m_bShowFixedDetectorLocation;
bool m_bShowSpeedSensorLocation;

int m_DislayedDemandType;
bool m_DislayReferenceLineNo;
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	int m_MoveLayerNo;
	bool m_bShowCompleteTrajectory;
	bool m_bShowAllCompleteTrajectory;
protected:

// Implementation
public:
	virtual ~CTLiteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void OnClickLink(UINT nFlags, CPoint point);
protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnViewShownetwork();
	afx_msg void OnViewMove();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateViewMove(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewSelect();
	afx_msg void OnUpdateViewSelect(CCmdUI *pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNodeOrigin();
	afx_msg void OnNodeDestination();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSearchFindlink();
	afx_msg void OnEditChangebackgroupimagethroughmouse();
	afx_msg void OnUpdateEditChangebackgroupimagethroughmouse(CCmdUI *pCmdUI);
	afx_msg void OnViewBackgroundimage();
	afx_msg void OnUpdateViewBackgroundimage(CCmdUI *pCmdUI);
	afx_msg void OnViewShowlinktype();
	afx_msg void OnUpdateViewShowlinktype(CCmdUI *pCmdUI);
	afx_msg void OnShowShownode();
	afx_msg void OnUpdateShowShownode(CCmdUI *pCmdUI);
	afx_msg void OnShowShowallpaths();
	afx_msg void OnUpdateShowShowallpaths(CCmdUI *pCmdUI);
	afx_msg void OnShowShownodenumber();
	afx_msg void OnUpdateShowShownodenumber(CCmdUI *pCmdUI);
	afx_msg void OnEditCreate1waylink();
	afx_msg void OnEditCreate2waylinks();
	afx_msg void OnUpdateEditCreate1waylink(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCreate2waylinks(CCmdUI *pCmdUI);
	afx_msg void OnShowGrid();
	afx_msg void OnUpdateShowGrid(CCmdUI *pCmdUI);
	afx_msg void OnShowLinkarrow();
	afx_msg void OnUpdateShowLinkarrow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoeVehicle(CCmdUI *pCmdUI);
	afx_msg void OnImageLockbackgroundimageposition();
	afx_msg void OnViewTextlabel();
	afx_msg void OnUpdateViewTextlabel(CCmdUI *pCmdUI);
	afx_msg void OnLinkDelete();
	afx_msg void OnUpdateEditDeleteselectedlink(CCmdUI *pCmdUI);
	afx_msg void OnLinkEditlink();
	afx_msg void OnEditCreatenode();
	afx_msg void OnUpdateEditCreatenode(CCmdUI *pCmdUI);
	afx_msg void OnEditDeleteselectednode();
	afx_msg void OnEditSelectnode();
	afx_msg void OnViewSelectNode();
	afx_msg void OnUpdateViewSelectNode(CCmdUI *pCmdUI);
	afx_msg void OnEditCreatesubarea();
	afx_msg void OnUpdateEditCreatesubarea(CCmdUI *pCmdUI);
	afx_msg void OnToolsRemovenodesandlinksoutsidesubarea();
	afx_msg void OnViewShowAVISensor();
	afx_msg void OnUpdateViewShowAVISensor(CCmdUI *pCmdUI);
	afx_msg void OnFileDataexchangewithgooglefusiontables();
	afx_msg void OnEditDeletelinksoutsidesubarea();
	afx_msg void OnEditMovenetworkcoordinates();
	afx_msg void OnUpdateEditMovenetworkcoordinates(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedFindNode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewIncreasenodesize();
	afx_msg void OnViewDecreatenodesize();
	afx_msg void OnViewDisplaylanewidth();
	afx_msg void OnNodeCheckconnectivityfromhere();
	afx_msg void OnNodeDirectiontohereandreliabilityanalysis();
	afx_msg void OnLinkIncreasebandwidth();
	afx_msg void OnLinkDecreasebandwidth();
	afx_msg void OnLinkSwichtolineBandwidthMode();
	afx_msg void OnViewTransitlayer();
	afx_msg void OnUpdateViewTransitlayer(CCmdUI *pCmdUI);
	afx_msg void OnNodeMovementproperties();
	afx_msg void OnLinkLinedisplaymode();
	afx_msg void OnUpdateLinkLinedisplaymode(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewZoneboundary(CCmdUI *pCmdUI);
	afx_msg void OnViewShowConnector();
	afx_msg void OnUpdateViewShowConnector(CCmdUI *pCmdUI);
	afx_msg void OnViewHighlightcentroidsandactivitylocations();
	afx_msg void OnUpdateViewHighlightcentroidsandactivitylocations(CCmdUI *pCmdUI);
	afx_msg void OnViewBackgroundcolor();
	afx_msg void OnActivitylocationmodeNolanduseactivity();
	afx_msg void OnUpdateActivitylocationmodeNolanduseactivity(CCmdUI *pCmdUI);
	afx_msg void OnActivitylocationmodeLanduseactivity();
	afx_msg void OnUpdateActivitylocationmodeLanduseactivity(CCmdUI *pCmdUI);
	afx_msg void OnActivitylocationmodeExternalorigin();
	afx_msg void OnUpdateActivitylocationmodeExternalorigin(CCmdUI *pCmdUI);
	afx_msg void OnActivitylocationmodeExternaldestination();
	afx_msg void OnUpdateActivitylocationmodeExternaldestination(CCmdUI *pCmdUI);
	afx_msg void OnMoeOddemand();
	afx_msg void OnUpdateMoeOddemand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLinkIncreasebandwidth(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLinkDecreasebandwidth(CCmdUI *pCmdUI);
	afx_msg void OnExportCreatevissimfiles();
	afx_msg void OnDebugShowvehicleswithincompletetripsonly();
	afx_msg void OnUpdateDebugShowvehicleswithincompletetripsonly(CCmdUI *pCmdUI);
	afx_msg void OnVehicleVehiclenumber();
	afx_msg void OnUpdateVehicleVehiclenumber(CCmdUI *pCmdUI);
	afx_msg void OnVehicleShowselectedvehicleonly();
	afx_msg void OnUpdateVehicleShowselectedvehicleonly(CCmdUI *pCmdUI);
	afx_msg void OnNodeAddintermediatedestinationhere();
	afx_msg void OnNodeRemoveallintermediatedestination();
	afx_msg void OnLinkAvoidusingthislinkinrouting();
	afx_msg void OnBnClickedButtonConfiguration();
	afx_msg void OnNodeNodeproperties();
	afx_msg void OnNodeAvoidthisnode();
	afx_msg void OnNodeRemovenodeavoidanceconstraint();
	afx_msg void OnEditMovenode();
	afx_msg void OnUpdateEditMovenode(CCmdUI *pCmdUI);

	afx_msg void OnBackgroundimageMarklongA();
	afx_msg void OnBackgroundimageMarklongB();
	afx_msg void OnBackgroundimageAddlat();
	afx_msg void OnZoneHighlightassociatedacititylocations();
	afx_msg void OnUpdateZoneHighlightassociatedacititylocations(CCmdUI *pCmdUI);
	afx_msg void OnZoneCreatezone();
	afx_msg void OnEditCreatezone();
	afx_msg void OnUpdateEditCreatezone(CCmdUI *pCmdUI);
	afx_msg void OnSubareaHighlightlinksinsidesubarea();
	afx_msg void OnUpdateSubareaHighlightlinksinsidesubarea(CCmdUI *pCmdUI);
	afx_msg void OnSubareaHighlightlinksacosssubarea();
	afx_msg void OnUpdateSubareaHighlightlinksacosssubarea(CCmdUI *pCmdUI);
	afx_msg void OnOdmatrixViewtop50odpairsonly();
	afx_msg void OnUpdateOdmatrixViewtop50odpairsonly(CCmdUI *pCmdUI);
	afx_msg void OnTransitShowtransitaccessibility();
	afx_msg void OnUpdateTransitShowtransitaccessibility(CCmdUI *pCmdUI);
	afx_msg void OnTransitCalculatetransitaccesssibilityfromhere();
	afx_msg void OnTransitOutputtransitaccesssibilityfromhere();
	afx_msg void OnMovementHighlightprohibitedmovements();
	afx_msg void OnUpdateMovementHighlightprohibitedmovements(CCmdUI *pCmdUI);
	afx_msg void OnTransitShowtransitlinksonly();
	afx_msg void OnTransitShowwalklinksonly();
	afx_msg void OnUpdateTransitShowtransitlinksonly(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransitShowwalklinksonly(CCmdUI *pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnVehicledataShowcompletetrajectory();
	afx_msg void OnUpdateVehicledataShowcompletetrajectory(CCmdUI *pCmdUI);
	afx_msg void OnVehicledataShow();
	afx_msg void OnUpdateVehicledataShow(CCmdUI *pCmdUI);
	afx_msg void OnDetectorUnlock();
	afx_msg void OnUpdateDetectorUnlock(CCmdUI *pCmdUI);
	afx_msg void OnDetectorShowsensordescription33769();
	afx_msg void OnUpdateDetectorShowsensordescription33769(CCmdUI *pCmdUI);
	afx_msg void OnDetectorShowmatchedlinkforsensor();
	afx_msg void OnUpdateDetectorShowmatchedlinkforsensor(CCmdUI *pCmdUI);
	afx_msg void OnGridUnlock();
	afx_msg void OnUpdateGridUnlock(CCmdUI *pCmdUI);
	afx_msg void OnLinkShowhidereferencenetwork();
	afx_msg void OnUpdateLinkShowhidereferencenetwork(CCmdUI *pCmdUI);
	afx_msg void OnEditMovereferencenetworkcoordinates();
	afx_msg void OnUpdateEditMovereferencenetworkcoordinates(CCmdUI *pCmdUI);
	afx_msg void OnReferenceShowreferencelinkidandbaselinespeedsensorid();
	afx_msg void OnUpdateReferenceShowreferencelinkidandbaselinespeedsensorid(CCmdUI *pCmdUI);
	afx_msg void OnDetectorShowspeedsensorlocation();
	afx_msg void OnUpdateDetectorShowfixedcountdetectorlocation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDetectorShowspeedsensorlocation(CCmdUI *pCmdUI);
	afx_msg void OnDetectorShowfixedcountdetectorlocation();
	afx_msg void OnShowallowedlinksfordemandtypeDemandtype1();
	afx_msg void OnShowallowedlinksfordemandtypeDemandtype2();
	afx_msg void OnShowallowedlinksfordemandtypeDemandtype3();
	afx_msg void OnShowallowedlinksfordemandtypeDemandtype4();
	afx_msg void OnUpdateShowallowedlinksfordemandtypeDemandtype1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowallowedlinksfordemandtypeDemandtype2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowallowedlinksfordemandtypeDemandtype3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowallowedlinksfordemandtypeDemandtype4(CCmdUI *pCmdUI);
	afx_msg void OnShowallowedlinksfordemandtypeShowall();
	afx_msg void OnUpdateShowallowedlinksfordemandtypeShowall(CCmdUI *pCmdUI);
	afx_msg void OnReferencelineShowreferencelineno();
	afx_msg void OnBnClickedButtonSearch();
};
struct PieInfo
{
	public:

    CString desc;
    double percentage;
    COLORREF color;
    PieInfo() { }
    PieInfo(CString d, double p, COLORREF c)
    {
        desc =d;
        percentage = p;
        color = c;
    }
};

extern std::list<CTLiteView*>	g_ViewList;


#ifndef _DEBUG  // debug version in TLiteView.cpp
inline CTLiteDoc* CTLiteView::GetDocument() const
   { return reinterpret_cast<CTLiteDoc*>(m_pDocument); }
#endif

