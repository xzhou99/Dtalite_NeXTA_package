// MainFrm.h : interface of the CMainFrame class
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
#include "PlayerSeekBar.h"
#include "TLiteDoc.h"

// Nathan Lewis
//http://www.codeproject.com/Articles/1118/Adding-Text-to-a-Docking-Toolbar


class CFeatureInfo
{
public:
	string Attribute;
	CString Data;
};

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:

	float m_simulation_time_step;
	CMainFrame();

// Attributes
public:
	
	std::vector <CFeatureInfo> m_FeatureInfoVector;

	void FillFeatureInfo();

	CDialogBar  m_GISLayerBar;
	bool m_bFeatureInfoInitialized;
	layer_mode m_iSelectedLayer;
	std::map<layer_mode, bool> m_bShowLayerMap;
	bool bLayerInitialized;

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	bool m_bSynchronizedDisplay;
	char m_CurrentDirectory[MAX_PATH+1];
	void UpdateAllViews();
	void UpdateLegendView();

public:
	CStatusBar  m_wndStatusBar;
    CReBar      m_wndReBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndSimulationToolBar;
    CPlayerSeekBar m_wndPlayerSeekBar;

   CToolBar m_MOEToolBar;
   CToolBar m_AMSToolBar;

   bool m_bShowGISLayerToolBar;

   bool m_bShowMOEToolBar;
   bool m_bShowSimulationoolBar;
   bool m_bShowDataToolBar;
   bool m_bShowAMSToolBar;

	int OnCreate_TrafficNetwork(LPCREATESTRUCT lpCreateStruct);
	int OnCreate_RailNetwork(LPCREATESTRUCT lpCreateStruct);
   
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowNew3dview();
	afx_msg void OnShowTimetable();
	afx_msg void OnUpdateWindowNew3dview(CCmdUI *pCmdUI);
	afx_msg void OnWindow2dview();
	afx_msg void OnWindowShow2dview();
	afx_msg void On3ddisplayAnimation();
	afx_msg void OnTimer(UINT_PTR  nIDEvent);

	afx_msg void OnAnimationRewind();
	afx_msg void OnAnimationPause();
	afx_msg void OnAnimationStop();
	afx_msg void OnViewMoetoolbar();
	afx_msg void OnUpdateViewMoetoolbar(CCmdUI *pCmdUI);
	afx_msg void OnWindowShowestimationview();
	afx_msg void OnViewDatatoolbar();
	afx_msg void OnUpdateViewDatatoolbar(CCmdUI *pCmdUI);
	
	afx_msg void OnAnimationPlay();
	afx_msg void OnAnimationForward();
	afx_msg void OnAnimationBackward();

	afx_msg void OnAnimationPlay_second();
	afx_msg void OnAnimationForward_second();
	afx_msg void OnAnimationBackward_second();

	afx_msg void OnAnimationSkipforward();
	afx_msg void OnAnimationSkipbackward();
	afx_msg void OnWindowSynchronizeddisplay();
	afx_msg void OnUpdateWindowSynchronizeddisplay(CCmdUI *pCmdUI);
	afx_msg void OnViewGisLayerPanel();
	afx_msg void OnUpdateViewGisLayerPanel(CCmdUI *pCmdUI);
	afx_msg void OnLvnItemchangedListGislayer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListGislayer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListGislayer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDatabase();
	afx_msg void OnBnClickedButtonConfiguration();
	afx_msg void OnNgsimmenuActivatespace();
	afx_msg void OnViewSimulationtoolbar();
	afx_msg void OnUpdateViewSimulationtoolbar(CCmdUI *pCmdUI);
};


