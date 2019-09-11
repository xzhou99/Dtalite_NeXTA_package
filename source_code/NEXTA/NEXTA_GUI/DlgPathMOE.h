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
#include "afxcmn.h"
#include "afxwin.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "BaseDialog.h"

 

// CDlgPathMOE dialog


class CDlgPathMOE : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgPathMOE)

public:

	CTLiteDoc* m_pDoc;

	int Cur_MOE_type1; // 0: mean travel time
	int Cur_MOE_type2; // 
	int Cur_MOE_type3; // 

	CDlgPathMOE(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPathMOE();

// Dialog Data
	enum { IDD = IDD_DIALOG_PATHMOE };
	void InsertPathMOEItem();
protected:
	CImageList m_SmallImageList;
	
	HICON m_hSmallIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void DrawPlot(CPaintDC* pDC,int MOEType, CRect PlotRect);

	DECLARE_MESSAGE_MAP()

   float m_UnitDistance, m_UnitTime, m_UnitData;
   int m_Range;
   int m_TimeLeft, m_TimeRight;
   float m_YUpperBound;
   float m_YLowerBound;
   std::vector<double> m_SegmentDistanceVector;

   bool m_bShowMaxTravelTime;


public:

	virtual BOOL OnInitDialog();
	CListCtrl m_PathListControl;
	afx_msg void OnLvnItemchangedPathListControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnMoetype1Meantraveltime();
	afx_msg void OnMoetype1Maxtraveltime();
	afx_msg void OnMoetype1Mean();
	afx_msg void OnUpdateMoetype1Meantraveltime(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoetype1Maxtraveltime(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoetype1Utility(CCmdUI *pCmdUI);
	afx_msg void OnMoetype2Meantraveltime();
	afx_msg void OnMoetype2Maxtraveltime();
	afx_msg void OnMoetype2Utility();
	afx_msg void OnUpdateMoetype2Meantraveltime(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoetype2Maxtraveltime(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMoetype2Utility(CCmdUI *pCmdUI);
	afx_msg void OnMoetype2None();
	afx_msg void OnMoetype2Band();
	afx_msg void OnMoetype1Band();
	afx_msg void OnMoetype2Fuelconsumptions();
	afx_msg void OnMoetype2Co2emissions();
	afx_msg void OnMoetype1Fuelconsumptions();
	afx_msg void OnMoetype1Co2emissions();
	afx_msg void OnMoetype2Cost();
	afx_msg void OnMoetype1Cost();
};
