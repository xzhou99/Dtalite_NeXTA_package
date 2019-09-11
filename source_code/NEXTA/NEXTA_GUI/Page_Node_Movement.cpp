// Page_Node_Movement.cpp : implementation file
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

#include "stdafx.h"
#include "TLite.h"
#include "Page_Node_Movement.h"

#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"
#include "MainFrm.h"
#include <string>
#include <sstream>
// CPage_Node_Movement dialog

extern CPen g_PenProhibitedMovement;

IMPLEMENT_DYNAMIC(CPage_Node_Movement, CPropertyPage)

CPage_Node_Movement::CPage_Node_Movement()
: CPropertyPage(CPage_Node_Movement::IDD)
, m_CycleLengthInSec(0)
, m_bPhasingDataEditMode(FALSE)
, m_CurrentNode_Name(_T(""))
, m_MovementMsg(_T(""))
, m_CycleLength(0)
, m_Offset(0)
, m_bHideRightTurnMovement(FALSE)
{
	m_SelectedTimingPlanNo = 0;
	m_TimingPlanName = "0";
	m_bColumnWidthIncludeHeader = true;
	m_SelectedMovementIndex = -1;
	m_bModifiedFlag = false;
	m_PeakHourFactor = 1.0;

	m_SelectedTimingPlanNo = 0;

	m_SelectedPhaseNumber = 0;

	m_bSigalizedNode = false;

}

CPage_Node_Movement::~CPage_Node_Movement()
{
}

void CPage_Node_Movement::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GRIDLISTCTRLEX,m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_CURRENT_NODEID, m_CurrentNodeNumber);
	DDX_Text(pDX, IDC_EDIT_CURRENT_NODENAME, m_CurrentNode_Name);
	DDX_Check(pDX, IDC_HIDE_RIGHT_TURN, m_bHideRightTurnMovement);
}


BEGIN_MESSAGE_MAP(CPage_Node_Movement, CPropertyPage)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GRIDLISTCTRLEX, &CPage_Node_Movement::OnLvnItemchangedGridlistctrlex)
	ON_BN_CLICKED(IDC_BUTTON_ExtendColumeWidth, &CPage_Node_Movement::OnBnClickedButtonExtendcolumewidth)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPage_Node_Movement::OnLbnSelchangeList1)
	ON_LBN_DBLCLK(IDC_LIST1, &CPage_Node_Movement::OnLbnDblclkList1)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_HIDE_RIGHT_TURN, &CPage_Node_Movement::OnBnClickedHideRightTurn)
	ON_CBN_SELCHANGE(IDC_COMBO_Timing_Plan_2, &CPage_Node_Movement::OnCbnSelchangeComboTimingPlan2)
	ON_EN_CHANGE(IDC_EDIT_CURRENT_NODENAME, &CPage_Node_Movement::OnEnChangeEditCurrentNodename)
END_MESSAGE_MAP()



BOOL CPage_Node_Movement::OnInitDialog()
{

	CPropertyPage::OnInitDialog();
	int PlanNo = m_pDoc->GetTimingPlanNo("0"); 

	for(int p = 1; p <= 	_max_phase_number; p++)
	{
		m_bAvailablePhaseVector [p] = false; 
		m_EffectiveGreenTime [p]= 0;
	}

	m_CurrentNodeID =  m_pDoc->m_SelectedNodeID ;
	m_CurrentNodeNumber = m_pDoc->m_NodeNoMap [m_CurrentNodeID]->m_NodeNumber ;
	

	m_CycleLengthInSec = m_pDoc->m_NodeNoMap [m_CurrentNodeID]->m_CycleLengthInSecond ;
	// Give better margin to editors
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);



	m_Column_names.push_back ("Incoming Node");  //0
	m_Column_names.push_back ("Outgoing Node");   //1
	m_Column_names.push_back ("Turn Type"); //2
	m_Column_names.push_back ("Direction"); //3

	m_Column_names.push_back ("Prohibition"); //4

	m_Column_names.push_back ("Simu Total Volume"); //5
	m_Column_names.push_back ("Simu Hourly Volume"); //6
	m_Column_names.push_back ("Simu Delay (sec)"); //7

	//if(m_bSigalizedNode == true)
	//{
	//m_Column_names.push_back ("Green Start Time"); //9
	//m_Column_names.push_back ("Green End Time"); //10

	//}

	CHeaderCtrl* pHeader = m_ListCtrl.GetHeaderCtrl();
	if( pHeader!=NULL)
	{
		pHeader->ModifyStyle(HDS_BUTTONS, 0);    // disable the sorting.
	}

	//Add Columns and set headers
	for (size_t i=0;i<m_Column_names.size();i++)
	{

		CGridColumnTrait* pTrait = NULL;
		//              pTrait = new CGridColumnTraitEdit();

		if(m_Column_names[i].find ("Turn Type")!=  string::npos)
		{
			pTrait = new CGridColumnTraitEdit();
		}

		if(m_Column_names[i].find ("Direction")!=  string::npos)
		{
			pTrait = new CGridColumnTraitEdit();
		}

		if(m_Column_names[i].find ("Prohibition")!=  string::npos)
		{
			CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;

			pComboTrait->SetStyle (CBS_DROPDOWNLIST);
			pComboTrait->AddItem(0, _T(""));
			pComboTrait->AddItem(0, _T("Prohibited"));

			pTrait = pComboTrait;
		}

		m_ListCtrl.InsertColumnTrait((int)i,m_Column_names.at(i).c_str(),LVCFMT_LEFT,-1,-1, pTrait);

		if(i<=10)
			m_ListCtrl.SetColumnWidth((int)i,100);
		else
			m_ListCtrl.SetColumnWidth((int)i,100);
	}


	//Add Rows

	DTANode* pNode  = m_pDoc->m_NodeNoMap [m_CurrentNodeID];

	for (unsigned int i=0;i< pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector .size();i++)
	{

		DTANodeMovement movement = pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i];


		CString str;
		str.Format ("%d", m_pDoc->m_NodeNoMap[movement.in_link_from_node_id]->m_NodeNumber  );  // 0: from node
		int Index = m_ListCtrl.InsertItem(LVIF_TEXT,i,str , 0, 0, 0, NULL);

		int column_index = 1;

		str.Format ("%d", m_pDoc->m_NodeNoMap[movement.out_link_to_node_id ]->m_NodeNumber ); // 1: to node
		m_ListCtrl.SetItemText(Index, column_index++,str);

		m_ListCtrl.SetItemText(Index, column_index++,m_pDoc->GetTurnString(movement.movement_turn)); //2: turn type
		m_ListCtrl.SetItemText(Index, column_index++,m_pDoc->GetTurnDirectionString( movement.movement_approach_turn)); //3: turn type

		if(movement.turning_prohibition_flag  == 1)
			str.Format ("Prohibited"); 
		else
			str.Format (""); 


		m_ListCtrl.SetItemText(Index, column_index++,str );  //4


		str.Format ("%.0f",movement.sim_turn_count   ); // 5: simulated volume
		m_ListCtrl.SetItemText(Index, column_index++,str );

		float number_of_hours = max(0.01,(m_pDoc->m_DemandLoadingEndTimeInMin -  m_pDoc->m_DemandLoadingStartTimeInMin)/60.0);
		float sim_turn_hourly_count = movement.sim_turn_count / number_of_hours;
		str.Format ("%.0f",sim_turn_hourly_count); // 6: simulated hourly volume
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%.1f",movement.sim_turn_delay*60   ); // 7: simulated turn delay
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%.1f",movement.QEM_StartTime   ); // green start time 
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%.1f",movement.QEM_EndTime  ); // green end green time 
		m_ListCtrl.SetItemText(Index, column_index++,str );

	}

	UpdateData(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CPage_Node_Movement::UpdateList()
{

	//Add Rows

	DTANode* pNode  = m_pDoc->m_NodeNoMap [m_CurrentNodeID];


	for (unsigned int i=0;i< pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector .size();i++)
	{

		DTANodeMovement movement = pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i];

		int column_index = 1;


		CString str; 
		int Index = i;
		str.Format ("%d", m_pDoc->m_NodeNoMap[movement.out_link_to_node_id ]->m_NodeNumber ); // 1: to node
		m_ListCtrl.SetItemText(Index, column_index++,str);

		m_ListCtrl.SetItemText(Index, column_index++,m_pDoc->GetTurnString(movement.movement_turn)); //2: turn type
		m_ListCtrl.SetItemText(Index, column_index++,m_pDoc->GetTurnDirectionString( movement.movement_approach_turn)); //2: turn type

		if(movement.turning_prohibition_flag  == 1)
			str.Format ("Prohibited"); 
		else
			str.Format (""); 


		m_ListCtrl.SetItemText(Index, column_index++,str );


		str.Format ("%d",movement.QEM_Lanes ); // 4: number of lanes 
		m_ListCtrl.SetItemText(Index, column_index++,str );


		str.Format ("%.0f",movement.sim_turn_count   ); // 7: simulated volume
		m_ListCtrl.SetItemText(Index, column_index++,str );

		float number_of_hours = max(0.01,(m_pDoc->m_TimingPlanVector[m_SelectedTimingPlanNo].end_time_in_min -  m_pDoc->m_TimingPlanVector[m_SelectedTimingPlanNo].start_time_in_min )/60.0);
		float sim_turn_hourly_count = movement.sim_turn_count / number_of_hours;

		str.Format ("%.0f",sim_turn_hourly_count   ); // simulated hourly volume
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%.1f",movement.sim_turn_delay*60   ); // simulated turn delay
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%.1f",movement.QEM_StartTime   ); // green start time 
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%.1f",movement.QEM_EndTime  ); // green end green time 
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%.0f",movement.QEM_SatFlow    ); 
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%.0f",movement.QEM_VOC    ); 
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%.2f",movement.QEM_Delay    ); 
		m_ListCtrl.SetItemText(Index, column_index++,str );

		str.Format ("%s",movement.QEM_LOS    ); 
		m_ListCtrl.SetItemText(Index, column_index++,str );



	}


	UpdateData(0);
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPage_Node_Movement::DisplayList()
{


}
void CPage_Node_Movement::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect PlotRect;
	GetClientRect(PlotRect);
	m_PlotRect = PlotRect;

	m_PlotRect.top += 55;
	m_PlotRect.bottom -= 290;
	m_PlotRect.left += 50;
	m_PlotRect.right -= 50;

	int GreenTimeDiagramHeight = 25;

	DrawMovements(&dc,m_PlotRect,false);


}

void CPage_Node_Movement::DrawMovements(CPaintDC* pDC,CRect PlotRect, bool bPhaseWindow =false)
{

	m_MovementBezierVector.clear();

	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));
	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	
	CPen DASHPen(PS_SOLID,1,RGB(255,178,102));

	CPen DataPen(PS_SOLID,0,RGB(0,0,0));
	CPen SelectedPen(PS_SOLID,3,RGB(255,0,0));
	CPen SelectedPhasePen(PS_SOLID,4,RGB(0,0,255));

	CBrush  WhiteBrush(RGB(255,255,255)); 

	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&DataPen);
	pDC->SelectObject(&WhiteBrush);

	pDC->Rectangle (PlotRect);

	CString str;
		
	if(bPhaseWindow == true )
		{
			str.Format("Phase %d",m_SelectedPhaseNumber);
			pDC->TextOutA(PlotRect.left+10,PlotRect.top +5,str);
		
		
		}


	CBrush  BrushLinkBand(RGB(152,245,255)); 
	pDC->SelectObject(&BrushLinkBand);

	DTANode* pNode  = m_pDoc->m_NodeNoMap [m_CurrentNodeID];

	int node_size = 10;
	int node_set_back = 50;

	int link_length = 100;
	int lane_width = 10;
	int text_length = link_length+ 20;

	str.Format("%d",m_CurrentNodeNumber);

  if(bPhaseWindow == false || m_SelectedPhaseNumber == 1)
   pDC->TextOutA( PlotRect.CenterPoint().x-5, PlotRect.CenterPoint().y-5,str);

	for (unsigned int i=0;i< pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector .size();i++)
	{
		DTANodeMovement movement = pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i];

		if( m_pDoc->m_hide_non_specified_movement_on_freeway_and_ramp && movement.bNonspecifiedTurnDirectionOnFreewayAndRamps && i != m_SelectedMovementIndex)
			continue;



		DTALink* pInLink  = m_pDoc->m_LinkNoMap [movement.IncomingLinkNo];
		DTALink* pOutLink  = m_pDoc->m_LinkNoMap [movement.OutgoingLinkNo ];

		GDPoint p1, p2, p3;
		// 1: fetch all data
		p1  = m_pDoc->m_NodeNoMap[movement.in_link_from_node_id ]->pt;
		p2  = m_pDoc->m_NodeNoMap[movement.in_link_to_node_id ]->pt;
		p3  = m_pDoc->m_NodeNoMap[movement.out_link_to_node_id]->pt;

		double DeltaX = p2.x - p1.x ;
		double DeltaY = p2.y - p1.y ;
		double theta = atan2(DeltaY, DeltaX);

		GDPoint p1_new, p2_new, p3_new;
		GDPoint p1_text, p3_text;
		GDPoint pt_movement[3];


		// 2. set new origin
		p2_new.x = (-1)*node_set_back*cos(theta);  
		p2_new.y = (-1)*node_set_back*sin(theta);


		int link_mid_offset  = (pInLink->m_NumberOfLanes/2 +1)*lane_width;  // mid

		pt_movement[0].x = p2_new.x + link_mid_offset* cos(theta-PI/2.0f);
		pt_movement[0].y = p2_new.y + link_mid_offset* sin(theta-PI/2.0f);

		// 3 determine the control point for  PolyBezier
		float control_point_ratio = 0;
		if(movement.movement_turn == DTA_Through ) 
		{
			control_point_ratio = 0;
		}else if(movement.movement_turn == DTA_LeftTurn ) 
		{
			control_point_ratio = 1.2;
		}else
		{
			control_point_ratio = 0.5;
		}
		
		pt_movement[1].x = pt_movement[0].x + node_set_back*control_point_ratio*cos(theta);
		pt_movement[1].y = pt_movement[0].y + node_set_back*control_point_ratio*sin(theta);


		if(bPhaseWindow == true)
		{
		link_length = 1;
		text_length = node_set_back+10;
		}
		
		p1_new.x = (-1)*link_length*cos(theta);
		p1_new.y = (-1)*link_length*sin(theta);


		p1_text.x= (-1)*(text_length)*cos(theta);
		p1_text.y= (-1)*(text_length)*sin(theta);

		// 4: draw from node name
		
		str.Format("%d",m_pDoc->m_NodeNoMap [movement.in_link_from_node_id]->m_NodeNumber );

	
		if(p1_text.y < -50)
			p1_text.y +=10;

		CPoint pt_text = NPtoSP(p1_text);

		if(bPhaseWindow == false  || m_SelectedPhaseNumber == 1)
		{
		pDC->SetTextColor(RGB(0,0,255));
		pDC->TextOutA(pt_text.x-10,pt_text.y,str);
		pDC->SetTextColor(RGB(0,0,0));
		}

		if(bPhaseWindow == false)
		DrawLink(pDC,p1_new,p2_new,pInLink->m_NumberOfLanes,theta,lane_width);

		////////////////////////////////////////////
		//5: outgoing link
		DeltaX = p3.x - p2.x ;
		DeltaY = p3.y - p2.y ;
		theta = atan2(DeltaY, DeltaX);

		// set new origin
		p2_new.x = node_set_back*cos(theta);  
		p2_new.y = node_set_back*sin(theta);

		link_mid_offset  = (pOutLink->m_NumberOfLanes/2+1)*lane_width;
		pt_movement[2].x = p2_new.x + link_mid_offset* cos(theta-PI/2.0f);
		pt_movement[2].y = p2_new.y + link_mid_offset* sin(theta-PI/2.0f);


		p3_new.x = link_length*cos(theta);
		p3_new.y = link_length*sin(theta);

		p3_text.x= text_length*cos(theta);
		p3_text.y= text_length*sin(theta);

		if(bPhaseWindow == false)
		DrawLink(pDC,p2_new,p3_new,pOutLink->m_NumberOfLanes,theta,lane_width);

		DTALink * pRevLink = NULL; //reversed link
		unsigned long ReversedLinkKey = m_pDoc->GetLinkKey(pOutLink->m_ToNodeID, pOutLink->m_FromNodeID);

		int reversed_link_id = 0;
		if ( m_pDoc->m_NodeNotoLinkMap.find ( ReversedLinkKey) == m_pDoc->m_NodeNotoLinkMap.end())
		{
		str.Format("%d",m_pDoc->m_NodeNoMap [movement.out_link_to_node_id ]->m_NodeNumber );
	
		if(p3_text.y < -50)
			p3_text.y +=10;

		pt_text = NPtoSP(p3_text);

			if(bPhaseWindow == false  || m_SelectedPhaseNumber == 1)
			pDC->TextOutA(pt_text.x-10,pt_text.y,str);
	
		}


		//draw to node label

		//if(SelectedPhaseNumber>=1 && movement.QEM_Phase1 == SelectedPhaseNumber)  // seleted phase

		//{
		//CString Label = m_ListCtrl.GetItemText(i, m_SelectedColumnIndex);

		//str.Format("%s",Label );

		//if(p3_text.y < -100)
		//	p3_text.y +=10;

		//pt_text = NPtoSP(p2_new);

		//pDC->TextOutA(pt_text.x-10 ,pt_text.y,str);

		//}


		// draw movement 

		CPoint Point_Movement[4];



		if(i == m_SelectedMovementIndex)
		{
			pDC->SelectObject(&SelectedPen);
		}
		else
		{
			if(m_ListCtrl.GetItemText (i,4).Find("Prohibited") != -1) 			// select prohibited movement pen: 
				pDC->SelectObject(&g_PenProhibitedMovement);
			else
				pDC->SelectObject(&NormalPen);
		}

		if(movement.movement_turn == DTA_LeftTurn )
		{
			float weight = 0.9;
		pt_movement[1].x = (1-weight)*(pt_movement[0].x + pt_movement[2].x)/2 +0*weight;
		pt_movement[1].y = (1-weight)*(pt_movement[0].y + pt_movement[2].y)/2 +0*weight;
		}

		Point_Movement[0]= NPtoSP(pt_movement[0]);
		Point_Movement[1]= NPtoSP(pt_movement[1]);
		Point_Movement[2]= NPtoSP(pt_movement[1]);
		Point_Movement[3]= NPtoSP(pt_movement[2]);

		Point_Movement[0]= NPtoSP(pt_movement[0]);
		Point_Movement[1]= NPtoSP(pt_movement[1]);
		Point_Movement[2]= NPtoSP(pt_movement[1]);
		Point_Movement[3]= NPtoSP(pt_movement[2]);

		MovementBezier element(Point_Movement[0], Point_Movement[1],Point_Movement[3]);

		m_MovementBezierVector.push_back (element);

		if(m_bHideRightTurnMovement &&( pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].movement_turn == DTA_RightTurn ||  pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].movement_turn == DTA_RightTurn2))
			continue;

		bool bMovementIncluded = false;
	
		if(m_SelectedPhaseNumber >=1)
		{
		bMovementIncluded = m_pDoc->IfMovementIncludedInPhase(m_CurrentNodeNumber , m_pDoc->m_TimingPlanVector[m_SelectedTimingPlanNo].timing_plan_name,m_SelectedPhaseNumber, movement.in_link_from_node_id,movement.out_link_to_node_id );
		}

		if(m_SelectedPhaseNumber <=0 ||  /* all phases*/
		  m_SelectedPhaseNumber>=1 && ( bMovementIncluded == true || m_bPhasingDataEditMode) )  // seleted phase
		{
				
			
			pDC->SelectObject(&NormalPen);

			 if(m_SelectedPhaseNumber>=1 && m_bPhasingDataEditMode)
			 {
				if(  bMovementIncluded == true)
					pDC->SelectObject(&NormalPen);
				else
					pDC->SelectObject(&DASHPen);

			 }
	
			if(i == m_SelectedMovementIndex && m_bPhasingDataEditMode)
			{
				pDC->SelectObject(&SelectedPen);
			}

			//overwrite
		if(i == m_SelectedMovementIndex)
		{
			pDC->SelectObject(&SelectedPen);
		}

			pDC->PolyBezier(Point_Movement,4);

			bool bShowArrow = false;

			if(m_SelectedPhaseNumber>=1 &&  bMovementIncluded == true)
			{
				bShowArrow = true;

			}
			if(bShowArrow)
			{
			CPoint FromPoint = Point_Movement[2] ; 
			CPoint ToPoint = Point_Movement[3];


			CPoint arrow_pts[3];
			double slopy = atan2((double)(FromPoint.y - ToPoint.y), (double)(FromPoint.x - ToPoint.x));
			double cosy = cos(slopy);
			double siny = sin(slopy);   
			double display_length  = sqrt((double)(FromPoint.y - ToPoint.y)*(FromPoint.y - ToPoint.y)+(double)(FromPoint.x - ToPoint.x)*(FromPoint.x - ToPoint.x));
			double arrow_size = min(10,display_length/3.0);

			if(arrow_size>0.2)
			{

				arrow_pts[0] = ToPoint;
				arrow_pts[1].x = ToPoint.x + (int)(arrow_size * cosy - (arrow_size / 2.0 * siny) + 0.5);
				arrow_pts[1].y = ToPoint.y + (int)(arrow_size * siny + (arrow_size / 2.0 * cosy) + 0.5);
				arrow_pts[2].x = ToPoint.x + (int)(arrow_size * cosy + arrow_size / 2.0 * siny + 0.5);
				arrow_pts[2].y = ToPoint.y - (int)(arrow_size / 2.0 * cosy - arrow_size * siny + 0.5);

				pDC->Polygon(arrow_pts, 3);

			}
			}



		}
		//restore pen
		pDC->SelectObject(&DataPen);

	}
}

void CPage_Node_Movement::DrawLink(CPaintDC* pDC,GDPoint pt_from, GDPoint pt_to,int NumberOfLanes, double theta, int lane_width)
{
	CPoint DrawPoint[4];

	//then offset
	int link_offset = lane_width;

	pt_from.x += link_offset* cos(theta-PI/2.0f);
	pt_to.x += link_offset* cos(theta-PI/2.0f);

	pt_from.y += link_offset* sin(theta-PI/2.0f);
	pt_to.y += link_offset* sin(theta-PI/2.0f);

	DrawPoint[0] = NPtoSP(pt_from);
	DrawPoint[1] = NPtoSP(pt_to);

	link_offset = min(5,NumberOfLanes)*lane_width ;
	pt_from.x += link_offset* cos(theta-PI/2.0f);
	pt_to.x += link_offset* cos(theta-PI/2.0f);

	pt_from.y += link_offset* sin(theta-PI/2.0f);
	pt_to.y += link_offset* sin(theta-PI/2.0f);

	DrawPoint[2] = NPtoSP(pt_to);
	DrawPoint[3] = NPtoSP(pt_from);

	pDC->Polygon(DrawPoint, 4);

}
// CPage_Node_Movement message handlers

void CPage_Node_Movement::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	unsigned int i;

	DTANode* pNode  = m_pDoc->m_NodeNoMap [m_CurrentNodeID];
	for ( i=0;i< pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector.size();i++)
	{
		m_ListCtrl.SelectRow (i,false);
	}

	m_SelectedMovementIndex =  FindClickedMovement(point);

	if(m_SelectedMovementIndex >=0)
	{
		for ( i=0;i< pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector.size();i++)
		{
			char str[100];
			m_ListCtrl.GetItemText (i,0,str,20);
			int MovementIndex = atoi(str)-1; // the movement index has been sorted 

			if(i == m_SelectedMovementIndex)
			{
				m_ListCtrl.SelectRow (i,true);
			}
		}
	}

	CPropertyPage::OnLButtonDown(nFlags, point);
	Invalidate();

}

void CPage_Node_Movement::OnLvnItemchangedGridlistctrlex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{

		int nSelectedRow = m_ListCtrl.GetNextSelectedItem(pos);
		
		char str[100];
		m_ListCtrl.GetItemText (nSelectedRow,0,str,20);
		m_SelectedMovementIndex = nSelectedRow;


	}


	Invalidate();
}

void CPage_Node_Movement::SaveData()
{

	bool bTurnVolumeModified = false;
	UpdateData(1);

	std::map<int, int> IncomingLinkMap;

	DTANode* pNode  = m_pDoc->m_NodeNoMap [m_CurrentNodeID];

	if(	pNode->m_CycleLengthInSecond  != m_CycleLengthInSec || pNode->m_SignalOffsetInSecond   != m_Offset)
	{
		m_pDoc->Modify (true);

		pNode->m_SignalOffsetInSecond   = m_Offset;
	}


	for (unsigned int i=0; i < pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector.size(); i++)
	{
		int turning_prohibition_flag=  pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].turning_prohibition_flag;
		
		int PrevQEM_lanes = pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].QEM_Lanes;
		int QEM_Phase1 =  pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].QEM_Phase1;

	
		int obs_turn_hourly_count = pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].obs_turn_hourly_count  ;
		int obs_turn_delay = pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].obs_turn_delay  ;

		DTA_SIG_MOVEMENT movement_approach_turn = pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].movement_approach_turn  ;
		int effective_green=  (int)(pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].QEM_EffectiveGreen);
		int saturation_flow_rate=  (int)(pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].QEM_SatFlow);

		DTANodeMovement movement = pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i];

		int colume_index = 3;

		CString str;

		str = m_ListCtrl.GetItemText (i,GetColumnIndex("Direction"));

		pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].movement_approach_turn = m_pDoc->GetTurnDirectionFromString(str);

		str = m_ListCtrl.GetItemText (i,GetColumnIndex("Prohibition"));

		if(str.Find("Prohibited") == -1)  // not found 
			pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].turning_prohibition_flag  = 0;
		else
			pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].turning_prohibition_flag  = 1;


		str = m_ListCtrl.GetItemText (i,GetColumnIndex("# of Lanes"));
		pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].QEM_Lanes = atoi(str);


		if(m_bSigalizedNode == true)
		{
		str = m_ListCtrl.GetItemText (i,GetColumnIndex("Green Start Time"));
		pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].QEM_StartTime  = atoi(str);

		str = m_ListCtrl.GetItemText (i,GetColumnIndex("Green End Time"));
		pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].QEM_EndTime  = atoi(str);

		str = m_ListCtrl.GetItemText (i,GetColumnIndex("Sat Flow Rate Per Lane Group"));
		pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].QEM_SatFlow   = atoi(str);
		}

		if(movement_approach_turn != pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].movement_approach_turn ||
			obs_turn_hourly_count != pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].obs_turn_hourly_count  ||
			obs_turn_delay != pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].obs_turn_delay  || 
			turning_prohibition_flag != pNode->m_MovementDataMap[m_TimingPlanName].m_MovementVector[i].turning_prohibition_flag
	)

		{
			m_bModifiedFlag = true;
			m_pDoc->Modify (true);
		}

	}

}

void CPage_Node_Movement::OnOK( )
{
	SaveData();
	CPropertyPage::OnOK();
}

void CPage_Node_Movement::OnCancel( )
{
	
	CPropertyPage::OnCancel();
}

void CPage_Node_Movement::OnBnClickedButtonSave()
{


}

void CPage_Node_Movement::OnBnClickedButtonExtendcolumewidth()
{
	m_ListCtrl.SetColumnWidthAuto(-1,m_bColumnWidthIncludeHeader);
	m_bColumnWidthIncludeHeader = !m_bColumnWidthIncludeHeader;

	UpdateList();
}



void CPage_Node_Movement::OnLbnSelchangeList1()
{
	Invalidate();
}
void CPage_Node_Movement::OnLbnDblclkList1()
{
	Invalidate();
}



void CPage_Node_Movement::OnBnClickedHideRightTurn()
{
	UpdateData(1);
	Invalidate();
}

void CPage_Node_Movement::OnCbnSelchangeComboTimingPlan2()
{

	UpdateList();

}


void CPage_Node_Movement::OnEnChangeEditCurrentNodename()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
