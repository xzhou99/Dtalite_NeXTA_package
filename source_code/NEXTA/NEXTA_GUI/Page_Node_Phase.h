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
// CPage_Node_Phase dialog
#include "GridCtrl_src\\GridCtrl.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include "afxwin.h"

#define _max_phase_number 16
#define _max_timing_plans 20

class CPage_Node_Phase : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Node_Phase)

public:

	std::vector<int> m_ControlTypeVector;
	std::vector<int> m_TimePlanVector;


	void UpdatePhasingDataInGrid();
		   

	int m_SelectedPhaseNumber;
	int m_SelectedTimingPlanNo;

	CGridCtrl m_PhasingGrid;
	
	std::vector<CString> MovementVectorString;

	int m_SelectedTimingPlan;


	void DisplayPhasingGrid();

	std::vector<std::string> m_Column_names;

	void UpdatePhaseData();

	bool m_bAvailablePhaseVector[17]; 
	int m_EffectiveGreenTime[17];

	CPage_Node_Phase();
	virtual ~CPage_Node_Phase();

	bool m_bModifiedFlag;
// Dialog Data
	enum { IDD = IDD_DIALOG_NODE_PHASES };

	int m_SelectedMovementIndex;
	int m_CurrentNodeID;
	int m_CurrentNodeNumber;



	void SetItemTextInPhasingGrid(int i, int j, CString str, bool bReadOnly= false, COLORREF crBkClr=   RGB(255,255,255))
	{
		GV_ITEM item;
		item.mask = GVIF_TEXT;
		item.row = i;
		item.col = j;
		item.crBkClr = crBkClr;

		item.strText = str;

		if(bReadOnly)
		{
		item.nState  = GVIS_READONLY;
		}
		m_PhasingGrid.SetItem(&item);

		if(bReadOnly)
			m_PhasingGrid.SetItemState(i,j, m_PhasingGrid.GetItemState(i,j)| GVIS_READONLY);
		else
			m_PhasingGrid.SetItemState(i,j, m_PhasingGrid.GetItemState(i,j));
	}

	std::vector<MovementBezier> m_MovementBezierVector;

	int FindClickedMovement(CPoint pt)
	{
	
		float min_distance  = 50;
		int SelectedMovement = -1;

		DTANode* pNode  = m_pDoc->m_NodeNoMap [m_CurrentNodeID];
		for(unsigned int i = 0; i< m_MovementBezierVector.size(); i++)
		{
		
			if(m_bHideRightTurnMovement &&( pNode->m_MovementDataMap["0"].m_MovementVector[i].movement_turn == DTA_RightTurn ||  pNode->m_MovementDataMap["0"].m_MovementVector[i].movement_turn == DTA_RightTurn2))
			continue;

			float distance  = m_MovementBezierVector[i].GetMinDistance(pt) ;

			if(distance < min_distance)
			{
				min_distance = distance;
				SelectedMovement = i;
			}
		}

		return SelectedMovement;
	}
	
	bool m_bColumnWidthIncludeHeader;
	CTLiteDoc* m_pDoc;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CRect m_PlotRect;
	CPoint NPtoSP(GDPoint net_point)
	{
		CPoint pt;
		pt.x = m_PlotRect.CenterPoint().x + net_point.x;
		pt.y = m_PlotRect.CenterPoint().y - net_point.y;  // -1 here as the dialog screen is up side down
		return pt;
	}

	void SaveData();
	void DisplayList();
	void UpdateList();

	virtual void OnOK( );
	virtual void OnCancel( );
	void DrawMovements(CPaintDC* pDC,CRect PlotRect, bool bPhaseWindow);
	void DrawPhaseGreenTimeBand(CPaintDC* pDC,CRect PlotRect,int CycleLength, float PhaseStartTime,float PhaseEndTime, float Yellow, float Red );
	void DrawLink(CPaintDC* pDC,GDPoint pt1, GDPoint pt2, int NumberOfLanes,double theta, int lane_width);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonSave();
	float m_PeakHourFactor;
	CComboBox m_PhaseNumber;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnDblclkList1();

    afx_msg void OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);


	BOOL m_bPhasingDataEditMode;
	afx_msg void OnBnClickedEditMode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	afx_msg void OnCbnSelchangeComboTimingPlan();
	CString m_CurrentNode_Name;
	CString m_MovementMsg;
	CComboBox m_ControlTypeComboBox;
	CComboBox m_TimingPlanComboBox;

	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	int m_CycleLength;
	int m_Offset;
	BOOL m_bHideRightTurnMovement;
	afx_msg void OnBnClickedEditMode2();
	afx_msg void OnCbnSelchangeComboPhasemovementdiagram();
	afx_msg void OnBnClickedButtonQemView();
	afx_msg void OnBnClickedCheckMultiplePhaseDiagram();
	BOOL m_bMultiPhaseDisplay;
	BOOL m_bOptimizationMethod;
	afx_msg void OnBnClickedCheckOptimizationmethod();
	float m_VolumeAdjustmentFactor;
	afx_msg void OnBnClickedCheckOptimizationObs();
	BOOL m_bUsingObsHourlyCount;
	afx_msg void OnCbnEditchangeComboTimingPlan();
	afx_msg void OnBnClickedButtonsave();
};
