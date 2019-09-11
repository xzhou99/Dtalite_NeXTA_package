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
// CPage_Node_Movement dialog
#include "GridCtrl_src\\GridCtrl.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include "afxwin.h"

#define _max_phase_number 16
class CPage_Node_Movement : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Node_Movement)

public:

	int m_SelectedTimingPlanNo;

	bool m_bSigalizedNode;


	int GetColumnIndex(std::string str)
	{
		for(int i=0; i< m_Column_names.size(); i++)
		{

			if(m_Column_names[i].compare (str) ==0)

				return i;
	
		}
		return 0;
	
	}



	int m_SelectedPhaseNumber;

	
	std::vector<CString> MovementVectorString;

	int m_SelectedTimingPlan;
	std::string m_TimingPlanName;



	std::vector<std::string> m_Column_names;

	bool m_bAvailablePhaseVector[17]; 
	int m_EffectiveGreenTime[17];

	CPage_Node_Movement();
	virtual ~CPage_Node_Movement();

	bool m_bModifiedFlag;
// Dialog Data
	enum { IDD = IDD_DIALOG_NODE_MOVEMENTS };

	CGridListCtrlEx m_ListCtrl;
	int m_SelectedMovementIndex;
	int m_CurrentNodeID;
	int m_CurrentNodeNumber;





	std::vector<MovementBezier> m_MovementBezierVector;

	
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
	void DrawLink(CPaintDC* pDC,GDPoint pt1, GDPoint pt2, int NumberOfLanes,double theta, int lane_width);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLvnItemchangedGridlistctrlex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSave();
	float m_PeakHourFactor;
	int m_CycleLengthInSec;
	afx_msg void OnBnClickedButtonExtendcolumewidth();
	CComboBox m_PhaseNumber;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnDblclkList1();

    afx_msg void OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);


	BOOL m_bPhasingDataEditMode;
	afx_msg void OnBnClickedEditMode();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	afx_msg void OnCbnSelchangeComboTimingPlan();
	CString m_CurrentNode_Name;
	CString m_MovementMsg;
	CComboBox m_ControlTypeComboBox;
	CComboBox m_RingTypeComboBox;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	int m_CycleLength;
	int m_Offset;
	BOOL m_bHideRightTurnMovement;
	afx_msg void OnBnClickedEditMode2();

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
	afx_msg void OnBnClickedHideRightTurn();
	afx_msg void OnCbnSelchangeComboTimingPlan2();
	afx_msg void OnEnChangeEditCurrentNodename();
};
