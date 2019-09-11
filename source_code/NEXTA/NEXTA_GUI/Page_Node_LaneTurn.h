#pragma once

#include <vector>
#include "TLiteDoc.h"
// CPage_Node_LaneTurn dialog
#include "CGridListCtrlEx\\CGridListCtrlEx.h"

class CPage_Node_LaneTurn : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Node_LaneTurn)

public:
	CPage_Node_LaneTurn();
	virtual ~CPage_Node_LaneTurn();

// Dialog Data
	enum { IDD = IDD_DIALOG_NODE_LANETURN };

	CGridListCtrlEx m_ListCtrl;
	int m_SelectedMovementIndex;
	int m_CurrentNodeID;
	std::vector<bool> m_SelectedRowVector;
	std::vector<MovementBezier> m_MovementBezierVector;

	int FindClickedMovement(CPoint pt)
	{
	
		float min_distance  = 50;
		int SelectedMovement = -1;

		for(unsigned int i = 0; i< m_MovementBezierVector.size(); i++)
		{
			float distance  = m_MovementBezierVector[i].GetMinDistance(pt) ;

			if(distance < min_distance)
			{
				min_distance = distance;
				SelectedMovement = i;
			}
		}

		return SelectedMovement;
	}
	
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

	void DrawMovements(CPaintDC* pDC,CRect PlotRect);
	void DrawLink(CPaintDC* pDC,GDPoint pt1, GDPoint pt2, int NumberOfLanes,double theta, int lane_width);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	int m_CurrentNodeName;
	afx_msg void OnLvnItemchangedGridlistctrlex(NMHDR *pNMHDR, LRESULT *pResult);
};
