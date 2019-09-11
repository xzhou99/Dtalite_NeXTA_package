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
#include "Network.h"
#include "TLiteDoc.h"

// CDlgCarFollowing dialog

struct VehicleCarFollowingData
{
	float Distance;  // m
	float Acceration;// m/s^2
};

struct VehicleData
{
	float StartTime; // in time interval
	float EndTime; // in time interval
	float WaveSpeed;
	float FreeflowSpeed;   // m/s
};


class CDlgCarFollowing : public CDialog
{
	DECLARE_DYNAMIC(CDlgCarFollowing)

public:
	int m_SelectedLinkNo;
	CTLiteDoc* pDoc;
	CDlgCarFollowing(CWnd* pParent = NULL);   // standard constructor
	~CDlgCarFollowing()
	{
	if(m_VechileCFDataAry!=NULL)
		delete m_VechileCFDataAry;
	};

// Dialog Data
	enum { IDD = IDD_DIALOG_CARFOLLOWING };


	int m_MOEType ;

	int m_NumberOfVehicles;
	int m_NumberOfTimeSteps;  // 0.1 second as resultion

	VehicleCarFollowingData** m_VechileCFDataAry;
	std::vector<VehicleData> m_VehicleDataList;
	
	CPoint m_last_cpoint;
	bool m_bMoveDisplay;

   float m_UnitDistance, m_UnitTime;
   int m_Range;
   int m_TimeLeft, m_TimeRight;
   float m_YUpperBound;
   float m_YLowerBound;
   int m_NumberOfLanes;

   float m_FreeflowSpeed; // m/s
   float m_SimulationTimeInterval;
   float m_KJam;
   float m_MeanWaveSpeed;

   bool m_bDataAvailable;
   void CarFollowingSimulation();
   void DrawTimeSpaceGraph(CPaintDC* pDC,int MOEType, CRect PlotRect, bool LinkTextFlag);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSimulationRun();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

