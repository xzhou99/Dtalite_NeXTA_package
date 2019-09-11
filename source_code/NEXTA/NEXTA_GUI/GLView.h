//  Portions Copyright 1999 Wei Chen, cw7788@gmail.com
//  Portions Copyright 2010 Xuesong Zhou (xzhou99@gmail.com), xzhou99@gmail.com

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

#if !defined(AFX_GLVIEW_H__31C9B700_1B9B_11D6_A073_0050DA5DBC9D__INCLUDED_)
#define AFX_GLVIEW_H__31C9B700_1B9B_11D6_A073_0050DA5DBC9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGLView view
#include "GL/glaux.h"
#include <GL\gl.h>
#include <GL\glu.h> 

typedef struct VectorTag{
	double x;
	double y;
	double z;
}VectorStruct;
union Vector{
	VectorStruct v;
	double d[3];
};

enum glState{
	glNone,
	glMove,
	glRotate,
	glZoom,
};


struct doublePointStruct{
	double x;
	double y;
	double z;
	CPoint GetPoint(){
		return CPoint((int)x,(int)y);
	}
	doublePointStruct(double xx=-1.0,double yy=-1.0,double zz=-1.0){
		x=xx;
		y=yy;
		z=zz;
	}
	doublePointStruct operator =(doublePointStruct dd){
		x=dd.x;
		y=dd.y;
		z=dd.z;
		return *this;
	}
};

class CGLView : public CView
{
protected:
	CGLView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGLView)

// Attributes
public:

	int m_PredictionHorizon;
	CTLiteDoc* GetDocument() const;
	float m_XYResolution;
	float m_MinX;
	float m_MinY;
	float m_OrgX;
	float m_OrgY;

	float m_ZResolution;


	 float NXtoSX(float x) // convert network coordinate to screen coordinate
	{
		return (x-m_MinX)*m_XYResolution;
	}

	 float NYtoSY(float y) // convert network coordinate to screen coordinate
	{
		return (y-m_MinY)*m_XYResolution;
	}

	 float NXtoSX_org(float x) // convert network coordinate to screen coordinate
	{
		return (x-m_OrgX)*m_XYResolution;
	}

	 float NYtoSY_org(float y) // convert network coordinate to screen coordinate
	{
		return (y-m_OrgY)*m_XYResolution;
	}

	 glState m_State;
	CPoint m_StartPoint;
	AUX_RGBImageRec *m_pBackgroundImage;
	double m_ViewPortCX;
	double m_ViewPortCY;

	bool m_bShowBackgroundImage;
	bool m_bLoadBackgroundImage;

	bool m_bAnimation;

	double m_BackgroundMapHeight;
	double m_BackgroundMapHeight_UpperLayer;
	double m_BackgroundMapHeight_MedLayer;

	bool m_bShowDualLayer;

	bool m_bShowSecurityCube;


	AUX_RGBImageRec *m_pGrassImg;
	AUX_RGBImageRec *m_pRoadMap;
	CClientDC *m_pDC;
	BOOL m_bLockZ;
	BOOL m_bShowGrass;
	BOOL m_bFollowCar;
	doublePointStruct m_ObservePoint;
	doublePointStruct m_EyePoint, m_CarPosition;
	double m_EyeAng; // eye angle
	doublePointStruct m_Normal;
	double m_zAng;
	double m_xyAng;
	double m_EyeLength;
public:


	void SelectNumuericalColor(int Number)
	{
		switch(Number)
		{
		case 0: glColor3d(1,0,0); break;
		case 1: glColor3d(0,1,0); break;
		case 2: glColor3d(1,0,1); break;
		case 3: glColor3d(0,1,1); break;
		case 4: glColor3d(0,0,1); break;
		case 5: glColor3d(1,1,0); break; //yellow
		default: glColor3d(1,1,1);
		
		}
	}


	void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
/*	int i;
	float f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
*/

}
	void GetColorFromPower(float power)
{
    float H = power * 0.4f; // Hue (note 0.4 = Green
    float S = 0.9f; // Saturation
    float B = 0.9f; // Brightness

	float Red,Green,Blue;

	HSVtoRGB(&Red,&Green,&Blue, H,S,B);

    glColor3d(Red/255.0f,Green/255.0f,Blue/255.f);

}

	void GetRGBColorFromPower(float power)
	{
	float n= power*100;
	float R=(255*n)/100;
	float G=(255*(100-n))/100; 
	float B=0;
    glColor3d(R/255,G/255,B/255);
	}

	void ComputeEyePosition();

	bool m_ShowSpeedVariability;
	bool m_ShowAllPaths;
	bool m_ShowAllTrains;

	CString m_WindowText;


	void DrawAllObjects();
	void Render();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void On3DZoom();
	afx_msg void OnUpdate3DZoom(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void On3DPam();
	afx_msg void OnUpdate3DPam(CCmdUI* pCmdUI);
	afx_msg void On3DRotate();
	afx_msg void OnUpdate3DRotate(CCmdUI* pCmdUI);
	afx_msg void On3DLockZ();
	afx_msg void OnUpdate3DLockZ(CCmdUI* pCmdUI);
	afx_msg void On3DReset();
	afx_msg void OnUpdate3DReset(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void On3ddisplayTraveltimevariability();
	afx_msg void On3ddisplayTimeDependentPaths();
	afx_msg void On3ddisplayAnimation();
	afx_msg void OnUpdate3ddisplayTraveltimevariability(CCmdUI *pCmdUI);
	afx_msg void OnUpdate3ddisplayTimeDependentPaths(CCmdUI *pCmdUI);
	afx_msg void OnUpdate3ddisplayAnimation(CCmdUI *pCmdUI);
	afx_msg void On3ddisplayBasicview();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR  nIDEvent);
	afx_msg void On3dCarfol();
	afx_msg void OnUpdate3dCarfol(CCmdUI *pCmdUI);
};
inline CTLiteDoc* CGLView::GetDocument() const
   { return reinterpret_cast<CTLiteDoc*>(m_pDocument); }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLVIEW_H__31C9B700_1B9B_11D6_A073_0050DA5DBC9D__INCLUDED_)
