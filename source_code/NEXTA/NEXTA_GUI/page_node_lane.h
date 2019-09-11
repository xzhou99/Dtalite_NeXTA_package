#pragma once


// CPage_Node_Lane dialog
#include "TLiteDoc.h"
#include "TLiteView.h"
// CPage_Node_Lane dialog
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
class LinkRecord
{
public:
	LinkRecord();
	LinkRecord(MLink* pLink,int nIO,int nAngle);
	~LinkRecord();
	int		nLinkID;
	int		nIO;     // 1: inLink; 0: outLink
	int     nLaneNum;
	int		nP2PAngle;
	double  dTheta;
	int		nPcktNum;
	float	fPcktLngth[5]; //max
	int		nChnlNum;
	float	fChnlLngth[5];
	int		nNormAngle;
	double	dNormTheta;

	float   fOA;
	MLink*  m_pLink;
public:
	int		GetTotalLaneNum();
};
class DrawPara
{
public:
	int nR1;
	int nR2;
	int nR3;
	int nR4;
	int nRL;
	int nR5;
	int nCentroidRadius;
	int nPinPointRadius;
	int nLaneWidth;
	int nLaneLength;

	int		nALength;
	float	fAWidth; // percentage of nLaneWidth
	int		nAL1;
	int		nAL2;
	int		nAL3;
	int		nAL4;
	float	fAL1;
	float	fAL2;
	float	fAL3;

	int  nR180;
};

class CPage_Node_Lane : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Node_Lane)

	int m_NumberOfPhases;
public:
	CPage_Node_Lane();
	virtual ~CPage_Node_Lane();

// Dialog Data
	enum { IDD = IDD_DIALOG_NODE_LANE };

	CGridListCtrlEx m_ListCtrl;
	
	int m_SelectedMovementIndex;
	int m_SelectedPhaseIndex;
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
	CTLiteView* m_pView;
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
	void DrawMovementsOld(CPaintDC* pDC,CRect PlotRect);
	void DrawPhaseMovements(CPaintDC* pDC,CRect PlotRect, int CurrentPhaseIndex);

	void DrawLink(CPaintDC* pDC,GDPoint pt1, GDPoint pt2, int NumberOfLanes,double theta, int lane_width);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	int m_CurrentNodeName;
	afx_msg void OnLvnItemchangedGridlistctrlex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedLaneP();
	afx_msg void OnBnClickedLaneC();
	afx_msg void OnBnClickedLaneD();

public:
	std::vector<LinkRecord*> m_LinkData;
	int  m_nSelectedRowIndex;
	int  m_nSelectedLinkID;
	DrawPara		 m_Para;
	bool			 m_bOriginAngle;

	void InitLinkData();   // create original link data
	void SerialLinkRecord(LinkRecord* pRecord);
	void NormalizeAngles();
	void FillLinkData();
	void UpdateLinkDataDelete();
	void UpdateLinkDataAdd(int nCP); // 1=c, 2=p

	void SetDrawPara();
	void DrawCentroidRadicalLines(CPaintDC* pDC,CRect PlotRect,bool bOriginAngle=true);
	void DrawRadicalLine(CPaintDC* pDC,LinkRecord* pRecord,bool bOriginAngle=true);
	void DrawLanes(CPaintDC* pDC,LinkRecord* pRecord,bool bOriginAngle=true);
	void DrawOneLane(CPaintDC* pDC,LinkRecord* pRecord,int nLaneNum,int nArrowType,bool bOriginAngle=true);
	void DrawLinkPinPoint(CPaintDC* pDC,LinkRecord* pRecord,bool bOriginAngle=true);

	int  FindClickedLink(CPoint pt,bool bOriginAngle=true);
	afx_msg void OnBnClickedCheckNorm();
	afx_msg void OnLvnEndlabeleditGridlistctrlex(NMHDR *pNMHDR, LRESULT *pResult);

	void UpdateLength(int nSelected,int nLinkID,int nLaneNo,float fLength);
	void FromLinkRecord2Node();
	LinkRecord* GetLinkRecord(int nLinkID);
};


