// Page_Node_Lane.cpp : implementation file
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
#include "Page_Node_Lane.h"

#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"

#include <string>
#include <sstream>
// CPage_Node_Lane dialog


IMPLEMENT_DYNAMIC(CPage_Node_Lane, CPropertyPage)

CPage_Node_Lane::CPage_Node_Lane()
	: CPropertyPage(CPage_Node_Lane::IDD)
	, m_CurrentNodeName(0)
{
	m_SelectedMovementIndex = -1;
	m_SelectedPhaseIndex = -1;
	m_NumberOfPhases = 0;

	SetDrawPara();
	m_nSelectedLinkID = -1;
	m_nSelectedRowIndex = -1;
	m_bOriginAngle = true;
}

CPage_Node_Lane::~CPage_Node_Lane()
{
	for(int i=0;i<m_LinkData.size();i++)
	{
		LinkRecord* pRecord = m_LinkData[i];
		delete pRecord;
	}
	m_LinkData.clear();
}

void CPage_Node_Lane::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GRIDLISTCTRLEX,m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_CURRENT_NODEID, m_CurrentNodeName);
}


BEGIN_MESSAGE_MAP(CPage_Node_Lane, CPropertyPage)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GRIDLISTCTRLEX, &CPage_Node_Lane::OnLvnItemchangedGridlistctrlex)
	ON_BN_CLICKED(IDC_LANE_P, &CPage_Node_Lane::OnBnClickedLaneP)
	ON_BN_CLICKED(IDC_LANE_C, &CPage_Node_Lane::OnBnClickedLaneC)
	ON_BN_CLICKED(IDC_LANE_D, &CPage_Node_Lane::OnBnClickedLaneD)
	ON_BN_CLICKED(IDC_CHECK_NORM, &CPage_Node_Lane::OnBnClickedCheckNorm)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_GRIDLISTCTRLEX, &CPage_Node_Lane::OnLvnEndlabeleditGridlistctrlex)
END_MESSAGE_MAP()



BOOL CPage_Node_Lane::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_CurrentNodeID =  m_pDoc->m_SelectedNodeID ;
	m_CurrentNodeName = m_pDoc->m_NodeIDtoNameMap [m_CurrentNodeID];

	// Give better margin to editors
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);

	std::vector<std::string> m_Column_names;
	int nWidth[7] = {60,60,70,90,60,80,90};

	m_Column_names.push_back ("Index");
	m_Column_names.push_back ("Link ID");
	m_Column_names.push_back ("In/Out Link");
	m_Column_names.push_back ("From/To Node");
	m_Column_names.push_back ("Lane No.");
	m_Column_names.push_back ("Lane Type");
	m_Column_names.push_back ("Pocket Length");

	//Add Columns and set headers
	for (size_t i=0;i<m_Column_names.size();i++)
	{
		CGridColumnTrait* pTrait = NULL;
		if(i==m_Column_names.size()-1) // pocket length
		{
			pTrait = new CGridColumnTraitEdit();
		}
		m_ListCtrl.InsertColumnTrait((int)i,m_Column_names.at(i).c_str(),LVCFMT_LEFT,nWidth[i],i,pTrait);
	}

	//Add Rows, all link indexed lanes
	InitLinkData();
	NormalizeAngles();
	FillLinkData();

	UpdateData(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPage_Node_Lane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect PlotRect;
	GetClientRect(PlotRect);
	m_PlotRect = PlotRect;

		m_PlotRect.top += 35;
		m_PlotRect.bottom -= 35;
		m_PlotRect.left += 550;
		m_PlotRect.right -= 50;

		DrawCentroidRadicalLines(&dc,m_PlotRect,m_bOriginAngle);
	//DrawMovements(&dc,m_PlotRect);

	//DTANode* pNode  = m_pDoc->m_NodeIDMap [m_CurrentNodeID];


	//int left = PlotRect.left + 100;
	//int right = PlotRect.right - 100;

	//int width = 100;
	//int height  = 100;
	//int bottom  = PlotRect.bottom - 20;
	//int top  = bottom - height;
	//int spacing = 20;

	//for (unsigned int i=0;i< pNode->m_PhaseVector .size();i++)
	//{
	//	CRect PlotPhaseRect;
	//	PlotPhaseRect.left = left + i* (width + spacing);
	//	PlotPhaseRect.right = PlotPhaseRect.left + width;

	//	PlotPhaseRect.top = top;
	//	PlotPhaseRect.bottom  = bottom;
	//	m_PlotRect= PlotPhaseRect;

	//	//DrawPhaseMovements(&dc,PlotPhaseRect, i);

	//}

}

void CPage_Node_Lane::DrawMovementsOld(CPaintDC* pDC,CRect PlotRect)
{
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

		DTANode* pNode  = m_pDoc->m_NodeIDMap [m_CurrentNodeID];

		int node_size = 10;
		int node_set_back = 50;

		int link_length = 150;
		int lane_width = 10;
		int text_length = link_length+ 20;

		CString str;
		str.Format("%d",m_CurrentNodeName);

		pDC->TextOutA( PlotRect.CenterPoint().x-5, PlotRect.CenterPoint().y-5,str);


	for (unsigned int i=0;i< pNode->m_MovementVector .size();i++)
	{
		DTANodeMovement movement = pNode->m_MovementVector[i];
		DTALink* pInLink  = m_pDoc->m_LinkNoMap [movement.IncomingLinkID];
		DTALink* pOutLink  = m_pDoc->m_LinkNoMap [movement.OutgoingLinkID ];

		GDPoint p1, p2, p3;
		// 1: fetch all data
		p1  = m_pDoc->m_NodeIDMap[movement.in_link_from_node_id ]->pt;
		p2  = m_pDoc->m_NodeIDMap[movement.in_link_to_node_id ]->pt;
		p3  = m_pDoc->m_NodeIDMap[movement.out_link_to_node_id]->pt;
		
		double DeltaX = p2.x - p1.x ;
		double DeltaY = p2.y - p1.y ;
		double theta = atan2(DeltaY, DeltaX);

		GDPoint p1_new, p2_new, p3_new;
		GDPoint p1_text, p3_text;
		GDPoint pt_movement[3];


		// 2. set new origin
		p2_new.x = (-1)*node_set_back*cos(theta);  
		p2_new.y = (-1)*node_set_back*sin(theta);


		int link_mid_offset  = (pInLink->m_NumLanes/2 +1)*lane_width;  // mid
		
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
		str.Format("%d",m_pDoc->m_NodeIDtoNameMap [movement.in_link_from_node_id]);
		if(p1_text.y < -50)
			p1_text.y +=10;

		CPoint pt_text = NPtoSP(p1_text);

		pDC->TextOutA(pt_text.x-10,pt_text.y,str);

		DrawLink(pDC,p1_new,p2_new,pInLink->m_NumLanes,theta,lane_width);

////////////////////////////////////////////
		//5: outgoing link
		DeltaX = p3.x - p2.x ;
		DeltaY = p3.y - p2.y ;
		theta = atan2(DeltaY, DeltaX);

		// set new origin
		p2_new.x = node_set_back*cos(theta);  
		p2_new.y = node_set_back*sin(theta);

		link_mid_offset  = (pOutLink->m_NumLanes/2+1)*lane_width;
		pt_movement[2].x = p2_new.x + link_mid_offset* cos(theta-PI/2.0f);
		pt_movement[2].y = p2_new.y + link_mid_offset* sin(theta-PI/2.0f);


		p3_new.x = link_length*cos(theta);
		p3_new.y = link_length*sin(theta);

		p3_text.x= text_length*cos(theta);
		p3_text.y= text_length*sin(theta);


		//draw to node name
		str.Format("%d",m_pDoc->m_NodeIDtoNameMap [movement.out_link_to_node_id]);

		if(p3_text.y < -100)
			p3_text.y +=10;

		pt_text = NPtoSP(p3_text);


		pDC->TextOutA(pt_text.x-10 ,pt_text.y,str);

		DrawLink(pDC,p2_new,p3_new,pOutLink->m_NumLanes,theta,lane_width);


		// draw movement 

		CPoint Point_Movement[4];

		int CurrentPhaseIndex =0;
		if(i == m_SelectedMovementIndex)
			pDC->SelectObject(&SelectedPen);
		else if(movement.phase_index  == CurrentPhaseIndex)
			pDC->SelectObject(&SelectedPhasePen);
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

void CPage_Node_Lane::DrawPhaseMovements(CPaintDC* pDC,CRect PlotRect,  int CurrentPhaseIndex = -1)
{

	m_MovementBezierVector.clear();

	CPen NormalPen(PS_SOLID,1,RGB(0,0,0));
	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_SOLID,0,RGB(0,0,0));
	CPen SelectedPen(PS_SOLID,1,RGB(255,0,0));
	CPen SelectedPhasePen(PS_SOLID,1,RGB(0,0,255));

	CBrush  WhiteBrush(RGB(255,255,255)); 

	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&DataPen);
	pDC->SelectObject(&WhiteBrush);

	pDC->Rectangle (PlotRect);

	CBrush  BrushLinkBand(RGB(152,245,255)); 
	pDC->SelectObject(&BrushLinkBand);

		DTANode* pNode  = m_pDoc->m_NodeIDMap [m_CurrentNodeID];

		int node_size = 20;
		int node_set_back = 25;

		int link_length = 10;
		int lane_width = 2;
		int text_length = link_length+ 2;

		CString str;
		str.Format("%d",m_CurrentNodeName);

//		pDC->TextOutA( PlotRect.CenterPoint().x-5, PlotRect.CenterPoint().y-5,str);


	for (unsigned int i=0;i< pNode->m_MovementVector .size();i++)
	{
		DTANodeMovement movement = pNode->m_MovementVector[i];

		if(CurrentPhaseIndex >=0 && movement.phase_index  == CurrentPhaseIndex)
				continue;  // draw movements in a phase only

		DTALink* pInLink  = m_pDoc->m_LinkNoMap [movement.IncomingLinkID];
		DTALink* pOutLink  = m_pDoc->m_LinkNoMap [movement.OutgoingLinkID ];

		GDPoint p1, p2, p3;
		// 1: fetch all data
		p1  = m_pDoc->m_NodeIDMap[movement.in_link_from_node_id ]->pt;
		p2  = m_pDoc->m_NodeIDMap[movement.in_link_to_node_id ]->pt;
		p3  = m_pDoc->m_NodeIDMap[movement.out_link_to_node_id]->pt;
		
		double DeltaX = p2.x - p1.x ;
		double DeltaY = p2.y - p1.y ;
		double theta = atan2(DeltaY, DeltaX);

		GDPoint p1_new, p2_new, p3_new;
		GDPoint p1_text, p3_text;
		GDPoint pt_movement[3];


		// 2. set new origin
		p2_new.x = (-1)*node_set_back*cos(theta);  
		p2_new.y = (-1)*node_set_back*sin(theta);


		int link_mid_offset  = (pInLink->m_NumLanes/2 +1)*lane_width;  // mid
		
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
		str.Format("%d",m_pDoc->m_NodeIDtoNameMap [movement.in_link_from_node_id]);
		if(p1_text.y < -50)
			p1_text.y +=10;

		CPoint pt_text = NPtoSP(p1_text);

//		pDC->TextOutA(pt_text.x-10,pt_text.y,str);

		DrawLink(pDC,p1_new,p2_new,pInLink->m_NumLanes,theta,lane_width);

////////////////////////////////////////////
		//5: outgoing link
		DeltaX = p3.x - p2.x ;
		DeltaY = p3.y - p2.y ;
		theta = atan2(DeltaY, DeltaX);

		// set new origin
		p2_new.x = node_set_back*cos(theta);  
		p2_new.y = node_set_back*sin(theta);

		link_mid_offset  = (pOutLink->m_NumLanes/2+1)*lane_width;
		pt_movement[2].x = p2_new.x + link_mid_offset* cos(theta-PI/2.0f);
		pt_movement[2].y = p2_new.y + link_mid_offset* sin(theta-PI/2.0f);


		p3_new.x = link_length*cos(theta);
		p3_new.y = link_length*sin(theta);

		p3_text.x= text_length*cos(theta);
		p3_text.y= text_length*sin(theta);


		//draw to node name
		str.Format("%d",m_pDoc->m_NodeIDtoNameMap [movement.out_link_to_node_id]);

		if(p3_text.y < -100)
			p3_text.y +=10;

		pt_text = NPtoSP(p3_text);


//		pDC->TextOutA(pt_text.x-10 ,pt_text.y,str);

		DrawLink(pDC,p2_new,p3_new,pOutLink->m_NumLanes,theta,lane_width);


		// draw movement 

		CPoint Point_Movement[4];


		if(i == m_SelectedMovementIndex)
			pDC->SelectObject(&SelectedPen);
		else if(movement.phase_index  == CurrentPhaseIndex)
			pDC->SelectObject(&SelectedPhasePen);
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
void CPage_Node_Lane::DrawLink(CPaintDC* pDC,GDPoint pt_from, GDPoint pt_to,int NumberOfLanes, double theta, int lane_width)
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
// CPage_Node_Lane message handlers

void CPage_Node_Lane::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_nSelectedLinkID = FindClickedLink(point,m_bOriginAngle);
	// TODO: Add your message handler code here and/or call default
	//	unsigned int i;

	//	DTANode* pNode  = m_pDoc->m_NodeIDMap [m_CurrentNodeID];
	//	for ( i=0;i< pNode->m_MovementVector.size();i++)
	//	{
	//	m_ListCtrl.SelectRow (i,false);
	//	}

	//m_SelectedMovementIndex =  FindClickedMovement(point);
	//
	//if(m_SelectedMovementIndex >=0)
	//{
	//	for ( i=0;i< pNode->m_MovementVector.size();i++)
	//	{
	//		char str[100];
	//		m_ListCtrl.GetItemText (i,0,str,20);
	//		int MovementIndex = atoi(str)-1; // the movement index has been sorted 

	//		if(i == m_SelectedMovementIndex)
	//		{
	//			m_ListCtrl.SelectRow (i,true);
	//		}
	//	}
	//}

	CPropertyPage::OnLButtonDown(nFlags, point);
	Invalidate();

}

void CPage_Node_Lane::OnLvnItemchangedGridlistctrlex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	//for(unsigned int i = 0; i < m_SelectedRowVector.size(); i++)
	//	m_SelectedRowVector[i] = false;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
		int nSelectedRow = m_ListCtrl.GetNextSelectedItem(pos);

		char str[100];
		m_ListCtrl.GetItemText (nSelectedRow,0,str,20);
		m_nSelectedRowIndex = atoi(str)-1;

		m_ListCtrl.GetItemText (nSelectedRow,1,str,20);
		m_nSelectedLinkID = atoi(str);

/*
	m_ListCtrl.GetItemText (nSelectedRow,1,str,20);
	m_SelectedPhaseIndex = atoi(str)-1;
*/
	//if(m_SelectedMovementIndex>=0 && m_SelectedMovementIndex < m_SelectedRowVector.size())
	//{
	//m_SelectedRowVector[m_SelectedMovementIndex] = true;
	//}


	}
	Invalidate();
}

void CPage_Node_Lane::OnBnClickedLaneP()
{
	UpdateLinkDataAdd(2);
	int nCount = m_ListCtrl.GetItemCount();  
	while (nCount--)  
	{  
		m_ListCtrl.DeleteItem(nCount);  
	}
	FillLinkData();
}

void CPage_Node_Lane::OnBnClickedLaneC()
{
	UpdateLinkDataAdd(1);
	int nCount = m_ListCtrl.GetItemCount();  
	while (nCount--)  
	{  
		m_ListCtrl.DeleteItem(nCount);  
	}
	FillLinkData();
}

void CPage_Node_Lane::OnBnClickedLaneD()
{

	UpdateLinkDataDelete();

	int nCount = m_ListCtrl.GetItemCount();  
	while (nCount--)  
	{  
		m_ListCtrl.DeleteItem(nCount);  
	}
	FillLinkData();
}
void CPage_Node_Lane::UpdateLinkDataDelete()
{
	// find out the link record and number of m_nSelectedRowIndex, so we know the type
	LinkRecord * pRecord = NULL;
	int nLaneNum = 0,nType = 0; //1 c, 2 b, 3 p
	int i,j,nIndex=0;
	bool bFind = false;

	for(i=0;i<m_LinkData.size();i++)
	{
		LinkRecord* p = m_LinkData[i];
		nLaneNum = 1;
		bFind = false;
		for(j=0;j<p->nChnlNum;j++)
		{

			if ( nIndex == m_nSelectedRowIndex)
			{
				bFind = true;
				nType = 1;
				break;
			}
			nLaneNum ++;
			nIndex ++;		
		}
		if (bFind)
		{
			pRecord = p;
			break;
		}
		for(j=0;j<p->nLaneNum;j++)
		{
			if ( nIndex == m_nSelectedRowIndex)
			{
				bFind = true;
				nType = 2;
				break;
			}
			nLaneNum ++;
			nIndex ++ ;
		}
		if (bFind)
		{
			pRecord = p;
			break;
		}
		for(j=0;j<p->nPcktNum;j++)
		{
			if ( nIndex == m_nSelectedRowIndex)
			{
				bFind = true;
				nType = 3;
				break;
			}
			nLaneNum ++;
			nIndex ++ ;
		}
		if (bFind)
		{
			pRecord = p;
			break;
		}
	}
	if (!pRecord) return;
	if ( 2 == nType )
	{
		AfxMessageBox("You can not delete a basic lane!",MB_OK);
		return;
	}
	if ( 1 == nType )
	{
		//AfxMessageBox("A channel lane is deleted!",MB_OK);
		pRecord->nChnlNum --;
	}
	if ( 3 == nType )
	{
		//AfxMessageBox("A pocket lane is deleted!",MB_OK);
		pRecord->nPcktNum --;
	}
	std::vector<MLane*>::iterator iLane;
	MLane* pLane;
	for(iLane = pRecord->m_pLink->inLanes.begin();iLane != pRecord->m_pLink->inLanes.end();iLane++)
	{
		if ( nLaneNum == (*iLane)->m_Index )
		{
			pLane = (*iLane);
			break;
		}
	}
	if (pLane)
	{
		delete pLane;
		pRecord->m_pLink->inLanes.erase(iLane);
	}
}
void CPage_Node_Lane::UpdateLinkDataAdd(int nCP)
{
	// m_nSelectedLinkID
	for(int i=0;i<m_LinkData.size();i++)
	{
		LinkRecord* pRecord = m_LinkData[i];
		MLink* pLink = pRecord->m_pLink;

		if ( pRecord->nLinkID == m_nSelectedLinkID )
		{
			if ( 1 == nCP )
			{
				// 需要把之前index都加1
				for(int j=0;j<pLink->inLanes.size();j++)
				{
					pLink->inLanes[j]->m_Index ++;
				}
				for(int j= pRecord->nChnlNum;j>0;j++)
				{
					if(j<=4)
					{
					pRecord->fChnlLngth[j]=pRecord->fChnlLngth[j-1];
					}
				}
				pRecord->fChnlLngth[0] = 40.0;
				pRecord->nChnlNum ++;

				MLane* pLane = new MLane();
				pLane->leftTurn = false;
				pLane->rightTurn = true;
				pLane->through = false;
				pLane->m_ChannelLength = 40.0;
				pLane->m_PocketLength = 0.0;
				pLane->m_Index = 1;
				pLane->m_LinkID = pLink->m_LinkID;
				pLane->m_NodeID = pLink->m_ToNodeID;
				pLink->inLanes.push_back(pLane);
			}
			else
			{
				pRecord->fPcktLngth[pRecord->nPcktNum] = 40.0;
				pRecord->nPcktNum ++;

				MLane* pLane = new MLane();
				pLane->leftTurn = true;
				pLane->rightTurn = false;
				pLane->through = false;
				pLane->m_ChannelLength = 0.0;
				pLane->m_PocketLength = 40.0;
				pLane->m_Index = pRecord->nChnlNum+pRecord->nLaneNum+pRecord->nPcktNum;
				pLane->m_LinkID = pLink->m_LinkID;
				pLane->m_NodeID = pLink->m_ToNodeID;
				pLink->inLanes.push_back(pLane);
			}
		}
	}

}
void CPage_Node_Lane::DrawMovements(CPaintDC *pDC,CRect PlotRect)
{
	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));
	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_SOLID,0,RGB(0,0,0));
	CPen SelectedPen(PS_SOLID,4,RGB(255,0,0));
	CPen SelectedPhasePen(PS_SOLID,4,RGB(0,0,255));
	CPen redPen(PS_SOLID,3,RGB(255,0,0));
	CPen bluePen(PS_SOLID,3,RGB(0,0,255));


	CBrush  WhiteBrush(RGB(255,255,255)); 

	pDC->SetBkMode(TRANSPARENT);
	CPen *pOldPen	  =	pDC->SelectObject(&DataPen);
	CBrush *pOldBrush = pDC->SelectObject(&WhiteBrush);

	pDC->Rectangle (PlotRect);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

}
void CPage_Node_Lane::InitLinkData()
{
	m_CurrentNodeID =  m_pDoc->m_SelectedNodeID ;
	m_CurrentNodeName = m_pDoc->m_NodeIDtoNameMap [m_CurrentNodeID];
	MNode* pNode = m_pView->m_ms.GetMNodebyID(m_CurrentNodeID);

	for(int i=0;i<pNode->inLinks.size();i++)
	{
		MLink* pLink = pNode->inLinks[i];
		LinkRecord *pRecord = new LinkRecord(pLink,1,pLink->nInAngle);
		SerialLinkRecord(pRecord);
	}
	for(int i=0;i<pNode->outLinks.size();i++)
	{
		MLink* pLink = pNode->outLinks[i];
		LinkRecord *pRecord = new LinkRecord(pLink,0,pLink->nOutAngle);
		SerialLinkRecord(pRecord);
	}

	//for(i=0;i<m_pDoc->m_Network.m_InboundSizeAry[m_CurrentNodeID];i++)
	//{
	//	nLinkID  = m_pDoc->m_Network.m_InboundLinkAry[m_CurrentNodeID][i];
	//	DTALink *pLink = m_pDoc->m_LinkNoMap[nLinkID];
	//	p1  = m_pDoc->m_NodeIDMap[pLink->m_ToNodeID]->pt;    // current node
	//	p2  = m_pDoc->m_NodeIDMap[pLink->m_FromNodeID]->pt;
	//	nAngle = m_pDoc->Find_P2P_Angle(p1,p2);
	//	LinkRecord *pRecord = new LinkRecord(pLink,1,nAngle);
	//	SerialLinkRecord(pRecord);
	//}
	//for(i=0;i<m_pDoc->m_Network.m_OutboundSizeAry[m_CurrentNodeID];i++)
	//{
	//	nLinkID  = m_pDoc->m_Network.m_OutboundLinkAry[m_CurrentNodeID][i];
	//	DTALink *pLink = m_pDoc->m_LinkNoMap[nLinkID];
	//	p1  = m_pDoc->m_NodeIDMap[pLink->m_FromNodeID]->pt;  // current node
	//	p2  = m_pDoc->m_NodeIDMap[pLink->m_ToNodeID]->pt;
	//	nAngle = m_pDoc->Find_P2P_Angle(p1,p2);
	//	LinkRecord *pRecord = new LinkRecord(pLink,0,nAngle);
	//	SerialLinkRecord(pRecord);
	//}
}
void CPage_Node_Lane::SerialLinkRecord(LinkRecord* pRecord)
{
	bool bInserted = false;
	int nNewAngle  = pRecord->nP2PAngle;

	std::vector<LinkRecord*>::iterator iRecord;
	for(iRecord = m_LinkData.begin();iRecord!=m_LinkData.end();iRecord++)
	{
		int nAngle = (*iRecord)->nP2PAngle;
		if (nNewAngle <= nAngle)
		{
			if ( (nNewAngle < nAngle) || (nNewAngle == nAngle &&  0 == pRecord->nIO))
			{ // insert before this pos
				m_LinkData.insert(iRecord,pRecord);
			}
			else // nNewAngle == nAngle && 1 == pRecord->nIO
			{// insert after this pose
				m_LinkData.insert(iRecord+1,pRecord);
			}
			bInserted = true;
			break;
		}
	}
	if (!bInserted)
	{
		m_LinkData.push_back(pRecord);
	}
}
void CPage_Node_Lane::FillLinkData()
{
	int nTotalIndex = 0;
	int i,j;
	CString str;

	for(i=0;i<m_LinkData.size();i++)
	{
		LinkRecord* pRecord = m_LinkData[i];
		MLink* pLink = m_pView->m_ms.GetMLinkbyID(pRecord->nLinkID);

		for(j=0;j<pRecord->nChnlNum;j++)
		{
			str.Format("%d",nTotalIndex+1);
			int Index = m_ListCtrl.InsertItem(LVIF_TEXT,nTotalIndex,str, 0, 0, 0, NULL);
			nTotalIndex ++ ;

			str.Format ("%d", pRecord->nLinkID);
			m_ListCtrl.SetItemText(Index, 1, str);

			m_ListCtrl.SetItemText(Index, 2, pRecord->nIO == 1 ? "In Link" : "Out Link");
			
			str.Format ("%d",pRecord->nIO == 1 ? pLink->m_FromNodeNumber : pLink->m_ToNodeNumber);
			m_ListCtrl.SetItemText(Index,3,str);

			str.Format ("%d",j+1);
			m_ListCtrl.SetItemText(Index,4,str);

			m_ListCtrl.SetItemText(Index,5,"Channel");

			str.Format("%.2f",pRecord->fChnlLngth[j]);
			m_ListCtrl.SetItemText(Index,6,str);
		}
		for(j=1;j<=pRecord->nLaneNum;j++)
		{
			str.Format("%d",nTotalIndex+1);
			int Index = m_ListCtrl.InsertItem(LVIF_TEXT,nTotalIndex,str, 0, 0, 0, NULL);
			nTotalIndex ++;

			str.Format ("%d", pRecord->nLinkID);
			m_ListCtrl.SetItemText(Index, 1,str);

			m_ListCtrl.SetItemText(Index, 2,pRecord->nIO == 1 ? "In Link" : "Out Link");
			
			str.Format ("%d",pRecord->nIO == 1 ? pLink->m_FromNodeNumber : pLink->m_ToNodeNumber);
			m_ListCtrl.SetItemText(Index,3,str);

			str.Format ("%d",j+pRecord->nChnlNum);
			m_ListCtrl.SetItemText(Index,4,str);

			m_ListCtrl.SetItemText(Index,5,"Basic");

			str.Format("%.2f",0.0);
			m_ListCtrl.SetItemText(Index,6,str);
		}
		for(j=0;j<pRecord->nPcktNum;j++)
		{
			str.Format("%d",nTotalIndex+1);
			int Index = m_ListCtrl.InsertItem(LVIF_TEXT,nTotalIndex,str, 0, 0, 0, NULL);
			nTotalIndex ++ ;

			str.Format ("%d", pRecord->nLinkID);
			m_ListCtrl.SetItemText(Index, 1, str);

			m_ListCtrl.SetItemText(Index, 2, pRecord->nIO == 1 ? "In Link" : "Out Link");
			
			str.Format ("%d",pRecord->nIO == 1 ? pLink->m_FromNodeNumber : pLink->m_ToNodeNumber);
			m_ListCtrl.SetItemText(Index,3,str);

			str.Format ("%d",j+1+pRecord->nChnlNum+pRecord->nLaneNum);
			m_ListCtrl.SetItemText(Index,4,str);

			m_ListCtrl.SetItemText(Index,5,"Pocket");

			str.Format("%.2f",pRecord->fPcktLngth[j]);
			m_ListCtrl.SetItemText(Index,6,str);
		}
	}
}

/////// LinkRecord //////////
LinkRecord::LinkRecord()
{
}
LinkRecord::LinkRecord(MLink* pLink,int nIO,int nAngle)
{
	m_pLink   = pLink;
	nLinkID   = pLink->m_LinkID;
	this->nIO = nIO;
	nP2PAngle = nAngle;
	dTheta    = nAngle * PI / 180;
	nLaneNum  = pLink->m_NumLanes;
	int nP=0,nC=0;
	for( int i=0;i<5;i++)
	{
		fPcktLngth[i] = 0.0;
		fChnlLngth[i] = 0.0;
	}
	if ( 1 == nIO)
	{
		for(int i=0;i<pLink->inLanes.size();i++)
		{
			MLane* pLane = pLink->inLanes[i];
			if ( pLane->m_ChannelLength > 0.0 ) 	nC ++;
			if ( pLane->m_PocketLength > 0.0 )		nP ++;
		}
		for(int i=0;i<pLink->inLanes.size();i++)
		{
			MLane* pLane = pLink->inLanes[i];
			if ( pLane->m_ChannelLength > 0.0 ) 	fChnlLngth[pLane->m_Index-1] = pLane->m_ChannelLength;
			if ( pLane->m_PocketLength > 0.0 )		fPcktLngth[pLane->m_Index-1-nLaneNum-nC] = pLane->m_PocketLength;
		}
	}
	nPcktNum = nP;
	nChnlNum = nC;
}
LinkRecord::~LinkRecord()
{
}
int LinkRecord::GetTotalLaneNum()
{
	return nChnlNum + nLaneNum + nPcktNum;
}
///////// DrawLane //////////////////
void CPage_Node_Lane::SetDrawPara()
{
	m_Para.nR1			= 30;
	m_Para.nR2			= 200;
	m_Para.nR3          = 110;
	m_Para.nR4          = 170;
	m_Para.nRL			= 180;
	m_Para.nR5			= 210;
	m_Para.nCentroidRadius = 6;
	m_Para.nPinPointRadius = 6;
	m_Para.nLaneWidth	= 12;
	m_Para.nLaneLength	= 90;
	m_Para.nALength		= 10;
	m_Para.fAWidth		= 0.2;
	
	m_Para.nAL1	= 0;
	m_Para.nAL2 = 10;
	m_Para.nAL3 = 10;
	m_Para.nAL4 = 30;
	m_Para.fAL1 = 0.2;
	m_Para.fAL2 = 0.4;
	m_Para.fAL3 = 0.5;

	m_Para.nR180 = 30;
	
}
void CPage_Node_Lane::DrawCentroidRadicalLines(CPaintDC *pDC, CRect PlotRect,bool bOriginAngle/*=true*/)
{
	CPen blackPen(PS_SOLID,0,RGB(0,0,0));
	CPen greyPen(PS_SOLID,2,RGB(128,128,128));
	CPen lanePen(PS_SOLID,1,RGB(0,0,0));
	CPen laneSelectedPen(PS_SOLID,2,RGB(255,0,0));

	CBrush  greyBrush(RGB(128,128,128)); 
	CBrush  whiteBrush(RGB(255,255,255)); 
	CBrush  redBrush(RGB(255,0,0)); 

	pDC->SetBkMode(TRANSPARENT);
	CPen *pOldPen = pDC->SelectObject(&blackPen);
	CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
	pDC->Rectangle(PlotRect);

	GDPoint cntPoint,lefttop,bottomright;
	CRect centerRect;

	cntPoint.x = 0.0;
	cntPoint.y = 0.0;
	int nRadius2 = m_Para.nCentroidRadius;
	lefttop.x = cntPoint.x - nRadius2;
	lefttop.y = cntPoint.y - nRadius2;
	bottomright.x = cntPoint.x + nRadius2;
	bottomright.y = cntPoint.y + nRadius2;
	
	pDC->SelectObject(&greyPen);
	pDC->SelectObject(&greyBrush);
	centerRect = CRect(NPtoSP(lefttop),NPtoSP(bottomright));
	pDC->Ellipse(centerRect);

	for(int i=0;i<m_LinkData.size();i++)
	{
		LinkRecord *pRecord = m_LinkData[i];
		pDC->SelectObject(&greyPen);
		DrawRadicalLine(pDC,pRecord,bOriginAngle);
		pDC->SelectObject(&lanePen);
		pDC->SelectObject(&greyBrush);
		DrawLanes(pDC,pRecord,bOriginAngle);
		pDC->SelectObject(&blackPen);
		if (pRecord->nLinkID == m_nSelectedLinkID)
			pDC->SelectObject(&redBrush);
		DrawLinkPinPoint(pDC,pRecord,bOriginAngle);
	}

	// draw selected link pin point

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	
}
void CPage_Node_Lane::DrawRadicalLine(CPaintDC *pDC, LinkRecord *pRecord,bool bOriginAngle /* = true*/)
{
	GDPoint p1,p2;
	int nR1 = m_Para.nR1;
	int nR2 = m_Para.nR2;
	double theta = bOriginAngle ? pRecord->dTheta : pRecord->dNormTheta;

	p1.x	= nR1 * cos(theta);
	p1.y	= nR1 * sin(theta);
	
	p2.x	= nR2 * cos(theta);
	p2.y	= nR2 * sin(theta);

	pDC->MoveTo(NPtoSP(p1));
	pDC->LineTo(NPtoSP(p2));
}
void CPage_Node_Lane::DrawLanes(CPaintDC *pDC, LinkRecord *pRecord,bool bOriginAngle /* = true*/)
{
	int nLaneNum = 0 ;
	int nType = 0;

	for(int i=0;i<pRecord->nChnlNum;i++)
	{
		nLaneNum ++;
		nType = (pRecord->nIO==1) ? 2 : 0;
		DrawOneLane(pDC,pRecord,nLaneNum,nType,bOriginAngle);
	}
	for(int i=0;i<pRecord->nLaneNum;i++)
	{
		nLaneNum ++;
		nType = (pRecord->nIO==1) ? 4 : 0;
		DrawOneLane(pDC,pRecord,nLaneNum,nType,bOriginAngle); // if is 5 then is test only
	}
	for(int i=0;i<pRecord->nPcktNum;i++)
	{
		nLaneNum ++;
		nType = (pRecord->nIO==1) ? 1 : 0;
		DrawOneLane(pDC,pRecord,nLaneNum,nType,bOriginAngle);
	}
}
void CPage_Node_Lane::DrawOneLane(CPaintDC *pDC, LinkRecord *pRecord, int nLaneNum,int nArrowType,bool bOriginAngle/*=true*/)
{
	GDPoint p1,p2,prl;
	int nR1 = m_Para.nR3;
	int nR2 = m_Para.nR4;
	int nDis= m_Para.nLaneWidth;
	int nRL	= m_Para.nRL;
	double theta = bOriginAngle ? pRecord->dTheta : pRecord->dNormTheta;
	int i = pRecord->nChnlNum + pRecord->nLaneNum + pRecord->nPcktNum +1 - nLaneNum;

	if ( pRecord->nIO == 1)
	{
		nDis = -1 * nDis;
	}
	p1.x	= (nR1+m_Para.nAL4) * cos(theta) + i * nDis * cos(theta-PI/2.0f);
	p1.y	= (nR1+m_Para.nAL4) * sin(theta) + i * nDis * sin(theta-PI/2.0f);
	
	p2.x	= nR2 * cos(theta) + i * nDis * cos(theta-PI/2.0f);
	p2.y	= nR2 * sin(theta) + i * nDis * sin(theta-PI/2.0f);

	prl.x	= nRL * cos(theta) + i * nDis * cos(theta-PI/2.0f);
	prl.y	= nRL * sin(theta) + i * nDis * sin(theta-PI/2.0f);

	pDC->MoveTo(NPtoSP(p2));
	pDC->LineTo(NPtoSP(p1));

	CPoint pLaneNum = NPtoSP(prl);
	
	CString str;
	str.Format("%d",nLaneNum);
	pDC->TextOutA(pLaneNum.x,pLaneNum.y,str);

	// draw arrow
	if (nArrowType >= 4)
	{// thru arrow
		GDPoint pTop,pLeft,pRight;
		CPoint arrow[3];
		int nALength = m_Para.nALength;
		float fAHalf = m_Para.fAWidth;
		pTop.x		= (nR1-nALength) * cos(theta) + i * nDis * cos(theta-PI/2.0f);
		pTop.y		= (nR1-nALength) * sin(theta) + i * nDis * sin(theta-PI/2.0f);
		pLeft.x		= nR1 * cos(theta) + (i-fAHalf) * nDis * cos(theta-PI/2.0f);
		pLeft.y		= nR1 * sin(theta) + (i-fAHalf) * nDis * sin(theta-PI/2.0f);
		pRight.x	= nR1 * cos(theta) + (i+fAHalf) * nDis * cos(theta-PI/2.0f);
		pRight.y	= nR1 * sin(theta) + (i+fAHalf) * nDis * sin(theta-PI/2.0f);

		pDC->MoveTo(NPtoSP(p1));
		pDC->LineTo(NPtoSP(pTop));

		arrow[0]	= NPtoSP(pTop);
		arrow[1]	= NPtoSP(pLeft);
		arrow[2]	= NPtoSP(pRight);
		pDC->Polygon(arrow,3);

		nArrowType -= 4;
	}
	if (nArrowType == 1)
	{// left turn arrow
		GDPoint pTop,pLeft,pRight,pBrunch;
		CPoint arrow[3];
		pTop.x		= (nR1+m_Para.nAL1) * cos(theta) + (i-m_Para.fAL2) * nDis * cos(theta-PI/2.0f);
		pTop.y		= (nR1+m_Para.nAL1) * sin(theta) + (i-m_Para.fAL2) * nDis * sin(theta-PI/2.0f);
		pRight.x	= (nR1+m_Para.nAL2) * cos(theta) + (i-m_Para.fAL1) * nDis * cos(theta-PI/2.0f);
		pRight.y	= (nR1+m_Para.nAL2) * sin(theta) + (i-m_Para.fAL1) * nDis * sin(theta-PI/2.0f);
		pLeft.x		= (nR1+m_Para.nAL3) * cos(theta) + (i-m_Para.fAL3) * nDis * cos(theta-PI/2.0f);
		pLeft.y		= (nR1+m_Para.nAL3) * sin(theta) + (i-m_Para.fAL3) * nDis * sin(theta-PI/2.0f);
		//pBrunch.x	= (nR1+m_Para.nAL4) * cos(theta) + i * nDis * cos(theta-PI/2.0f);
		//pBrunch.y	= (nR1+m_Para.nAL4) * sin(theta) + i * nDis * sin(theta-PI/2.0f);

		pDC->MoveTo(NPtoSP(p1));
		pDC->LineTo(NPtoSP(pTop));

		arrow[0]	= NPtoSP(pTop);
		arrow[1]	= NPtoSP(pLeft);
		arrow[2]	= NPtoSP(pRight);
		pDC->Polygon(arrow,3);
		
	}
	if (nArrowType == 2)
	{// right turn arrow
		GDPoint pTop,pLeft,pRight,pBrunch;
		CPoint arrow[3];
		pTop.x		= (nR1+m_Para.nAL1) * cos(theta) + (i+m_Para.fAL2) * nDis * cos(theta-PI/2.0f);
		pTop.y		= (nR1+m_Para.nAL1) * sin(theta) + (i+m_Para.fAL2) * nDis * sin(theta-PI/2.0f);
		pRight.x	= (nR1+m_Para.nAL2) * cos(theta) + (i+m_Para.fAL1) * nDis * cos(theta-PI/2.0f);
		pRight.y	= (nR1+m_Para.nAL2) * sin(theta) + (i+m_Para.fAL1) * nDis * sin(theta-PI/2.0f);
		pLeft.x		= (nR1+m_Para.nAL3) * cos(theta) + (i+m_Para.fAL3) * nDis * cos(theta-PI/2.0f);
		pLeft.y		= (nR1+m_Para.nAL3) * sin(theta) + (i+m_Para.fAL3) * nDis * sin(theta-PI/2.0f);
		//pBrunch.x	= (nR1+m_Para.nAL4) * cos(theta) + i * nDis * cos(theta-PI/2.0f);
		//pBrunch.y	= (nR1+m_Para.nAL4) * sin(theta) + i * nDis * sin(theta-PI/2.0f);

		pDC->MoveTo(NPtoSP(p1));
		pDC->LineTo(NPtoSP(pTop));

		arrow[0]	= NPtoSP(pTop);
		arrow[1]	= NPtoSP(pLeft);
		arrow[2]	= NPtoSP(pRight);
		pDC->Polygon(arrow,3);
	}
}
void CPage_Node_Lane::DrawLinkPinPoint(CPaintDC *pDC, LinkRecord *pRecord, bool bOriginAngle /* = true */)
{
	GDPoint cntPoint,lefttop,bottomright;
	CRect centerRect;
	int nR5 = m_Para.nR5;
	int nDis = m_Para.nLaneWidth;
	double theta = bOriginAngle ? pRecord->dTheta : pRecord->dNormTheta;
	int i = (pRecord->nChnlNum+pRecord->nLaneNum+pRecord->nPcktNum + 1) / 2 ;

	if ( pRecord->nIO == 1)
	{
		nDis = -1 * nDis;
	}
	cntPoint.x	= nR5 * cos(theta) + i * nDis * cos(theta-PI/2.0f);
	cntPoint.y	= nR5 * sin(theta) + i * nDis * sin(theta-PI/2.0f);
	
	int nRadius2 = m_Para.nPinPointRadius;
	lefttop.x = cntPoint.x - nRadius2;
	lefttop.y = cntPoint.y - nRadius2;
	bottomright.x = cntPoint.x + nRadius2;
	bottomright.y = cntPoint.y + nRadius2;
	
	centerRect = CRect(NPtoSP(lefttop),NPtoSP(bottomright));
	pDC->Ellipse(centerRect);
}
int CPage_Node_Lane::FindClickedLink(CPoint pt,bool bOriginAngle /* = true */)
{
	int nRtLinkID = -1;
	for(int j=0;j<m_LinkData.size();j++)
	{
		LinkRecord* pRecord = m_LinkData[j];
		GDPoint cntPoint,lefttop,bottomright;
		CRect centerRect;
		int nR5 = m_Para.nR5;
		int nDis = m_Para.nLaneWidth;
		double theta = bOriginAngle ? pRecord->dTheta : pRecord->dNormTheta;
		int i = (pRecord->nChnlNum+pRecord->nLaneNum+pRecord->nPcktNum + 1) / 2 ;

		if ( pRecord->nIO == 1)
		{
			nDis = -1 * nDis;
		}
		cntPoint.x	= nR5 * cos(theta) + i * nDis * cos(theta-PI/2.0f);
		cntPoint.y	= nR5 * sin(theta) + i * nDis * sin(theta-PI/2.0f);
		
		int nRadius2 = m_Para.nPinPointRadius;
		lefttop.x = cntPoint.x - nRadius2;
		lefttop.y = cntPoint.y - nRadius2;
		bottomright.x = cntPoint.x + nRadius2;
		bottomright.y = cntPoint.y + nRadius2;
		
		centerRect = CRect(NPtoSP(lefttop),NPtoSP(bottomright));
		centerRect.NormalizeRect();
		if ( centerRect.PtInRect(pt) )
		{
			nRtLinkID = pRecord->nLinkID;
			break;
		}
	}
	return nRtLinkID;
}
void CPage_Node_Lane::NormalizeAngles()
{
	bool bNormalized = false;
	if (m_LinkData.size() == 8 &&
		m_LinkData[0]->nP2PAngle == m_LinkData[1]->nP2PAngle &&
		m_LinkData[2]->nP2PAngle == m_LinkData[3]->nP2PAngle &&
		m_LinkData[4]->nP2PAngle == m_LinkData[5]->nP2PAngle &&
		m_LinkData[6]->nP2PAngle == m_LinkData[7]->nP2PAngle )
	{
		int nAngle[4];
		for(int i=0;i<4;i++)
		{
			nAngle[i] = m_LinkData[2*i]->nP2PAngle;
		}
		if ( nAngle[0] <= 45 ) // first as the horizontal
		{
			int nClockwiseOff = nAngle[0];
			nAngle[0] = 0;
			nAngle[1] -= nClockwiseOff;
			if (nAngle[1] < 45 ) nAngle[1] = 45;
			if (nAngle[1] > 135) nAngle[1] = 135;
			nAngle[2] = 180;
			nAngle[3] -= nClockwiseOff;
			if (nAngle[3] < 225) nAngle[3] = 225;
			if (nAngle[3] > 315) nAngle[3] = 315;
		}
		else // last as the horizontal
		{
			int nCounterOff = 360 - nAngle[3];
			nAngle[3] = 0;
			nAngle[0] += nCounterOff;
			if (nAngle[0] < 45 ) nAngle[0] = 45;
			if (nAngle[0] > 135) nAngle[0] = 135;
			nAngle[1] = 180;
			nAngle[2] += nCounterOff;
			if (nAngle[2] < 225) nAngle[2] = 225;
			if (nAngle[2] > 315) nAngle[2] = 315;
		}
		for(int i=0;i<4;i++)
		{
			m_LinkData[2*i]->nNormAngle = nAngle[i];
			m_LinkData[2*i]->dNormTheta = nAngle[i] * PI / 180;
			m_LinkData[2*i+1]->nNormAngle = nAngle[i];
			m_LinkData[2*i+1]->dNormTheta = nAngle[i] * PI / 180;
		}
		bNormalized = true;
	}
	else // other types, process pending
	{
	}

	if (!bNormalized)
	{
		for(int i=0;i<m_LinkData.size();i++)
		{
			m_LinkData[i]->nNormAngle = m_LinkData[i]->nP2PAngle;
			m_LinkData[i]->dNormTheta = m_LinkData[i]->dTheta;
		}
	}
	
	return;
}
void CPage_Node_Lane::OnBnClickedCheckNorm()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bOriginAngle = !m_bOriginAngle;
	Invalidate();
}


void CPage_Node_Lane::OnLvnEndlabeleditGridlistctrlex(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int nSelected = -1;
	int nLinkID,nLaneNo;
	CString strIndex,strLink,strLane,strLaneType,strChange;

	while(pos!=NULL)
	{
		int nSelectedRow = m_ListCtrl.GetNextSelectedItem(pos);
		strIndex = m_ListCtrl.GetItemText (nSelectedRow,0);
		nSelected = atoi(strIndex)-1;
		strLink = m_ListCtrl.GetItemText (nSelectedRow,1);
		strLane = m_ListCtrl.GetItemText (nSelectedRow,4);
		strLaneType = m_ListCtrl.GetItemText (nSelectedRow,5);
		strChange = m_ListCtrl.GetItemText (nSelectedRow,6);
	}
	if ( nSelected >= 0 )
	{
		float fLength;
		if ( _T("Basic") == strLaneType ) fLength = -1.0;
		else fLength = atof(strChange);
		UpdateLength(nSelected,atoi(strLink),atoi(strLane),fLength);
	}
	Invalidate();
}
void CPage_Node_Lane::UpdateLength(int nSelected, int nLinkID, int nLaneNo, float fLength)
{// 修改record里数据，以及相关联的link中数据
	if ( fLength < 0 ) return;

	for(int i=0;i<m_LinkData.size();i++)
	{
		LinkRecord* pRecord = m_LinkData[i];
		MLink*      pLink   = pRecord->m_pLink;

		if ( nLinkID == pRecord->nLinkID )
		{
			if ( nLaneNo <= pRecord->nChnlNum )
			{
				pRecord->fChnlLngth[nLaneNo-1] = fLength;
			}
			if ( nLaneNo >= pRecord->nChnlNum+pRecord->nLaneNum )
			{
				pRecord->fPcktLngth[nLaneNo-pRecord->nChnlNum-pRecord->nLaneNum-1] = fLength;
			}
			for(int j=0;j<pLink->inLanes.size();j++)
			{
				MLane* pLane = pLink->inLanes[j];
				if ( nLaneNo == pLane->m_Index )
				{
					if ( nLaneNo <= pRecord->nChnlNum )
						pLane->m_ChannelLength = fLength;
					else
						pLane->m_PocketLength  = fLength;
				}
			}
			break;
		}
	}
}
void CPage_Node_Lane::FromLinkRecord2Node()
{
	MNode* pNode = m_pView->m_ms.GetMNodebyID(m_CurrentNodeID);
	for(int i=0;i<pNode->inLinks.size();i++)
	{
		MLink* pLink = pNode->inLinks[i];
		for(int j=0;j<pLink->inLanes.size();j++)
		{
			MLane* pLane = pLink->inLanes[j];
			delete pLane;
		}
		pLink->inLanes.clear();

		LinkRecord* pRecord = GetLinkRecord(pLink->m_LinkID);
		for(int j=0;j<pRecord->nChnlNum;j++)
		{
			MLane* pLane = new MLane();
			pLink->inLanes.push_back(pLane);
			pLane->m_Index = j+1;
			pLane->m_LinkID = pLink->m_LinkID;
			pLane->m_ChannelLength = pRecord->fChnlLngth[j];
			pLane->m_PocketLength  = 0.0;
			pLane->m_NodeID = pNode->m_NodeID;
			pLane->rightTurn = 1;
			pLane->through   = 0;
			pLane->leftTurn  = 0;
		}
		for(int j=0;j<pRecord->nLaneNum;j++)
		{
			MLane* pLane = new MLane();
			pLink->inLanes.push_back(pLane);
			pLane->m_Index = j+pRecord->nChnlNum+1;
			pLane->m_LinkID = pLink->m_LinkID;
			pLane->m_ChannelLength = 0.0;
			pLane->m_PocketLength  = 0.0;
			pLane->m_NodeID = pNode->m_NodeID;
			pLane->rightTurn = pRecord->nChnlNum==0&&j==0 ? 1 : 0;
			pLane->through   = 1;
			pLane->leftTurn  = pRecord->nPcktNum==0&&j==pRecord->nLaneNum-1 ? 1 : 0;
		}
		for(int j=0;j<pRecord->nPcktNum;j++)
		{
			MLane* pLane = new MLane();
			pLink->inLanes.push_back(pLane);
			pLane->m_Index = j+pRecord->nChnlNum+pRecord->nLaneNum+1;
			pLane->m_LinkID = pLink->m_LinkID;
			pLane->m_ChannelLength = 0.0;
			pLane->m_PocketLength  = pRecord->fPcktLngth[j];
			pLane->m_NodeID = pNode->m_NodeID;
			pLane->rightTurn = 0;
			pLane->through   = 0;
			pLane->leftTurn  = 1;
		}
	}
	// outLink暂时不考虑改动！
}
LinkRecord* CPage_Node_Lane::GetLinkRecord(int nLinkID)
{
	LinkRecord* pRecord=NULL;
	for(int i=0;i<m_LinkData.size();i++)
	{
		if ( nLinkID == m_LinkData[i]->nLinkID )
		{
			pRecord = m_LinkData[i];
			break;
		}
	}
	return pRecord;
}