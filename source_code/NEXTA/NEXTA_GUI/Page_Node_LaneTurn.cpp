// Page_Node_LaneTurn.cpp : implementation file
//  Portions Copyright 2010 Xuesong Zhou (xzhou99@gmail.com); Peng Du ()

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
#include "Page_Node_LaneTurn.h"

#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"

#include <string>
#include <sstream>
// CPage_Node_LaneTurn dialog


IMPLEMENT_DYNAMIC(CPage_Node_LaneTurn, CPropertyPage)

CPage_Node_LaneTurn::CPage_Node_LaneTurn()
	: CPropertyPage(CPage_Node_LaneTurn::IDD)
	, m_CurrentNodeName(0)
{
	m_SelectedMovementIndex = -1;
	
}

CPage_Node_LaneTurn::~CPage_Node_LaneTurn()
{
}

void CPage_Node_LaneTurn::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GRIDLISTCTRLEX,m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_CURRENT_NODEID, m_CurrentNodeName);
}


BEGIN_MESSAGE_MAP(CPage_Node_LaneTurn, CPropertyPage)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GRIDLISTCTRLEX, &CPage_Node_LaneTurn::OnLvnItemchangedGridlistctrlex)
END_MESSAGE_MAP()



BOOL CPage_Node_LaneTurn::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_CurrentNodeID =  m_pDoc->m_SelectedNodeID ;

	m_CurrentNodeName = m_pDoc->m_NodeNotoNumberMap [m_CurrentNodeID];
	// Give better margin to editors
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);

	std::vector<std::string> m_Column_names;

	m_Column_names.push_back ("Movement Index");
	m_Column_names.push_back ("Incoming Node");
	m_Column_names.push_back ("Outgoing Node");
	m_Column_names.push_back ("Turn Type");
	m_Column_names.push_back ("Prohibition");
	m_Column_names.push_back ("Turnning %");
	m_Column_names.push_back ("Signal Group No");

	//Add Columns and set headers
	for (size_t i=0;i<m_Column_names.size();i++)
	{

		CGridColumnTrait* pTrait = NULL;
//		pTrait = new CGridColumnTraitEdit();
		m_ListCtrl.InsertColumnTrait((int)i,m_Column_names.at(i).c_str(),LVCFMT_LEFT,-1,-1, pTrait);
		m_ListCtrl.SetColumnWidth((int)i,LVSCW_AUTOSIZE_USEHEADER);

	}
	m_ListCtrl.SetColumnWidth(0, 80);

	//Add Rows

	DTANode* pNode  = m_pDoc->m_NodeNoMap [m_CurrentNodeID];

	for (unsigned int i=0;i< pNode->m_MovementDataMap["0"].m_MovementVector .size();i++)
	{
		CString str;
		str.Format("%d",i+1);
		int Index = m_ListCtrl.InsertItem(LVIF_TEXT,i,str , 0, 0, 0, NULL);

		DTANodeMovement movement = pNode->m_MovementDataMap["0"].m_MovementVector[i];

		str.Format ("%d", m_pDoc->m_NodeNotoNumberMap[movement.in_link_from_node_id] );
		m_ListCtrl.SetItemText(Index, 1,str);

		str.Format ("%d", m_pDoc->m_NodeNotoNumberMap[movement.out_link_to_node_id ] );
		m_ListCtrl.SetItemText(Index, 2,str);
		
		m_ListCtrl.SetItemText(Index, 3,m_pDoc->GetTurnString(movement.movement_turn));
		m_SelectedRowVector.push_back(false);
	}

	UpdateData(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPage_Node_LaneTurn::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect PlotRect;
	GetClientRect(PlotRect);
	m_PlotRect = PlotRect;

		m_PlotRect.top += 35;
		m_PlotRect.bottom -= 35;
		m_PlotRect.left += 450;
		m_PlotRect.right -= 50;

	DrawMovements(&dc,m_PlotRect);
}

void CPage_Node_LaneTurn::DrawMovements(CPaintDC* pDC,CRect PlotRect)
{

	m_MovementBezierVector.clear();

	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));
	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_SOLID,0,RGB(0,0,0));
	CPen SelectedPen(PS_SOLID,4,RGB(255,0,0));
	CPen SelectedPhasePen(PS_SOLID,4,RGB(0,0,255));

	CBrush  WhiteBrush(RGB(255,255,255)); 

	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&DataPen);
	pDC->SelectObject(&WhiteBrush);

	pDC->Rectangle (PlotRect);

	CBrush  BrushLinkBand(RGB(152,245,255)); 
	pDC->SelectObject(&BrushLinkBand);

		DTANode* pNode  = m_pDoc->m_NodeNoMap [m_CurrentNodeID];

		int node_size = 10;
		int node_set_back = 50;

		int link_length = 150;
		int lane_width = 10;
		int text_length = link_length+ 20;

		CString str;
		str.Format("%d",m_CurrentNodeName);

		pDC->TextOutA( PlotRect.CenterPoint().x-5, PlotRect.CenterPoint().y-5,str);


	for (unsigned int i=0;i< pNode->m_MovementDataMap["0"].m_MovementVector .size();i++)
	{
		DTANodeMovement movement = pNode->m_MovementDataMap["0"].m_MovementVector[i];
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
		p1_new.x = (-1)*link_length*cos(theta);
		p1_new.y = (-1)*link_length*sin(theta);


		p1_text.x= (-1)*(text_length)*cos(theta);
		p1_text.y= (-1)*(text_length)*sin(theta);

		// 4: draw from node name
		str.Format("%d",m_pDoc->m_NodeNotoNumberMap [movement.in_link_from_node_id]);
		if(p1_text.y < -50)
			p1_text.y +=10;

		CPoint pt_text = NPtoSP(p1_text);

		pDC->TextOutA(pt_text.x-10,pt_text.y,str);

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


		//draw to node name
		str.Format("%d",m_pDoc->m_NodeNotoNumberMap [movement.out_link_to_node_id]);

		if(p3_text.y < -100)
			p3_text.y +=10;

		pt_text = NPtoSP(p3_text);


		pDC->TextOutA(pt_text.x-10 ,pt_text.y,str);

		DrawLink(pDC,p2_new,p3_new,pOutLink->m_NumberOfLanes,theta,lane_width);


		// draw movement 

		CPoint Point_Movement[4];


		if(i == m_SelectedMovementIndex)
			pDC->SelectObject(&SelectedPen);
		else
			pDC->SelectObject(&NormalPen);



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


		pDC->PolyBezier(Point_Movement,4);

		//restore pen
		pDC->SelectObject(&DataPen);

	}
}

void CPage_Node_LaneTurn::DrawLink(CPaintDC* pDC,GDPoint pt_from, GDPoint pt_to,int NumberOfLanes, double theta, int lane_width)
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

		link_offset = NumberOfLanes*lane_width ;
		pt_from.x += link_offset* cos(theta-PI/2.0f);
		pt_to.x += link_offset* cos(theta-PI/2.0f);

		pt_from.y += link_offset* sin(theta-PI/2.0f);
		pt_to.y += link_offset* sin(theta-PI/2.0f);

		DrawPoint[2] = NPtoSP(pt_to);
		DrawPoint[3] = NPtoSP(pt_from);

		pDC->Polygon(DrawPoint, 4);

}
// CPage_Node_LaneTurn message handlers

void CPage_Node_LaneTurn::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
		unsigned int i;

		DTANode* pNode  = m_pDoc->m_NodeNoMap [m_CurrentNodeID];
		for ( i=0;i< pNode->m_MovementDataMap["0"].m_MovementVector.size();i++)
		{
		m_ListCtrl.SelectRow (i,false);
		}

	m_SelectedMovementIndex =  FindClickedMovement(point);
	
	if(m_SelectedMovementIndex >=0)
	{
		for ( i=0;i< pNode->m_MovementDataMap["0"].m_MovementVector.size();i++)
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

void CPage_Node_LaneTurn::OnLvnItemchangedGridlistctrlex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	for(unsigned int i = 0; i < m_SelectedRowVector.size(); i++)
		m_SelectedRowVector[i] = false;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
	int nSelectedRow = m_ListCtrl.GetNextSelectedItem(pos);

	char str[100];
	m_ListCtrl.GetItemText (nSelectedRow,0,str,20);
	m_SelectedMovementIndex = atoi(str)-1;


	m_SelectedRowVector[m_SelectedMovementIndex] = true;


	}
		Invalidate();
}
