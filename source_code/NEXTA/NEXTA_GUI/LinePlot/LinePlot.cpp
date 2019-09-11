// LinePlot.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//  CLinePlot, CPlotData - a simple 2D graph control
//
//  Author: Paul Grenz
//  Email:  pgrenz@irlabs.com
//
//  You may freely use or modify this code provided this
//  message is included in all derived versions.
//
//  History - 2004/10/28: Initial release to codeguru.com
//
//
//  This class implements a simple 2D graph control which supports
//  multiple plots, locking and unlocking of axes, printing, and 
//  zooming.
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\TLite.h"
#include "..\Network.h"
#include "..\TLiteDoc.h"
#include "LinePlotTest.h"
#include "LinePlot.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotData class
/////////////////////////////////////////////////////////////////////////////

CPlotData::CPlotData(CString szName, COLORREF crColor, int nStyle, std::vector<FLOATPOINT> *pvecData)
{
	m_szName = szName;
	m_nStyle = nStyle;
	m_oIsDirty = true;
	m_rcKeyArea.SetRectEmpty();
	m_crColor = crColor;
	m_eXDataMin = 0.0f;
	m_eXDataMax = 0.0f;
	m_eYDataMin = 0.0f;
	m_eYDataMax = 0.0f;
	m_eYDataMean = 0.0f;
	m_PointData = NULL;
	m_pptScreen = NULL;
	m_uiPointCount = 0;

	//  copy the point data over, calculating max & min.
	UINT uiPointCount = pvecData->size();
	if (uiPointCount>0)
	{
		m_PointData = new FLOATPOINT[uiPointCount];
		m_pptScreen = new POINT[uiPointCount];
		m_uiPointCount = uiPointCount;
		if (m_PointData!=NULL && m_pptScreen!=NULL)
		{
			UpdateData(pvecData);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////

CPlotData::CPlotData(CString szName, COLORREF crColor, int nStyle, FLOATPOINT *pptData, UINT uiPointCount)
{

	m_szName = szName;
	m_nStyle = nStyle;
	m_oIsDirty = true;
	m_rcKeyArea.SetRectEmpty();
	m_crColor = crColor;
	m_eXDataMin = 0.0f;
	m_eXDataMax = 0.0f;
	m_eYDataMin = 0.0f;
	m_eYDataMax = 0.0f;
	m_eYDataMean = 0.0f;
	m_PointData = NULL;
	m_pptScreen = NULL;
	m_uiPointCount = 0;

	//  copy the point data over, calculating max & min.
	if (uiPointCount>0)
	{
		m_PointData = new FLOATPOINT[uiPointCount];
		m_pptScreen = new POINT[uiPointCount];
		m_uiPointCount = uiPointCount;
		if (m_PointData!=NULL && m_pptScreen!=NULL)
		{
			UpdateData(pptData);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////

CPlotData::CPlotData(const CPlotData &pd)
{
	m_szName = pd.m_szName;
	m_nStyle = pd.m_nStyle;
	m_oIsDirty = pd.m_oIsDirty;
	m_rcKeyArea = pd.m_rcKeyArea;
	m_crColor = pd.m_crColor;
	m_eXDataMin = pd.m_eXDataMin;
	m_eXDataMax = pd.m_eXDataMax;
	m_eYDataMin = pd.m_eYDataMin;
	m_eYDataMax = pd.m_eYDataMax;
	m_eYDataMean = pd.m_eYDataMean;
	m_PointData = NULL;
	m_pptScreen = NULL;
	m_uiPointCount = 0;

	//  copy the point data over, calculating max & min.
	if (pd.m_uiPointCount>0)
	{
		m_PointData = new FLOATPOINT[pd.m_uiPointCount];
		m_pptScreen = new POINT[pd.m_uiPointCount];
		m_uiPointCount = pd.m_uiPointCount;
		if (m_PointData!=NULL && m_pptScreen!=NULL)
		{
			UpdateData(pd.m_PointData);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////

CPlotData& CPlotData::operator=(const CPlotData& pd)
{
	m_szName = pd.m_szName;
	m_nStyle = pd.m_nStyle;
	m_oIsDirty = pd.m_oIsDirty;
	m_rcKeyArea = pd.m_rcKeyArea;
	m_crColor = pd.m_crColor;
	m_eXDataMin = pd.m_eXDataMin;
	m_eXDataMax = pd.m_eXDataMax;
	m_eYDataMin = pd.m_eYDataMin;
	m_eYDataMax = pd.m_eYDataMax;
	m_eYDataMean = pd.m_eYDataMean;
	if (m_PointData!=NULL)
		delete[] m_PointData;
	if (m_pptScreen !=NULL)
		delete[] m_pptScreen;
	m_uiPointCount = 0;

	//  copy the point data over, calculating max & min.
	if (pd.m_uiPointCount>0)
	{
		m_PointData = new FLOATPOINT[pd.m_uiPointCount];
		m_pptScreen = new POINT[pd.m_uiPointCount];
		m_uiPointCount = pd.m_uiPointCount;
		if (m_PointData!=NULL && m_pptScreen!=NULL)
		{
			UpdateData(pd.m_PointData);
		}
	}
	return *this;
}
/////////////////////////////////////////////////////////////////////////////

bool CPlotData::UpdateData(FLOATPOINT *pptData)
{
	//  init the min and max.
	m_eXDataMin = pptData[0].x;
	m_eXDataMax = pptData[0].x;
	m_eYDataMin = pptData[0].y;
	m_eYDataMax = pptData[0].y;
	double xAccum = 0.0f;
	m_eYDataMean = 0;
	//  calc the min and max.
	for (UINT ii=0; ii<m_uiPointCount; ii++)
	{
		m_PointData[ii] = pptData[ii];
		xAccum += m_PointData[ii].y;
		m_eXDataMin = (pptData[ii].x<m_eXDataMin) ? (pptData[ii].x) : (m_eXDataMin);
		m_eXDataMax = (pptData[ii].x>m_eXDataMax) ? (pptData[ii].x) : (m_eXDataMax);
		m_eYDataMin = (pptData[ii].y<m_eYDataMin) ? (pptData[ii].y) : (m_eYDataMin);
		m_eYDataMax = (pptData[ii].y>m_eYDataMax) ? (pptData[ii].y) : (m_eYDataMax);
	}
	//  set the mean.
	m_eYDataMean = (float)( xAccum / (double)(m_uiPointCount) );
	//  force a recalc of the screen polyline.
	m_oIsDirty = true;
	return true;
}
/////////////////////////////////////////////////////////////////////////////

bool CPlotData::UpdateData(std::vector<FLOATPOINT> *pvecData)
{
	//  init the min and max.
	m_eXDataMin = pvecData->at(0).x;
	m_eXDataMax = pvecData->at(0).x;
	m_eYDataMin = pvecData->at(0).y;
	m_eYDataMax = pvecData->at(0).y;
	double xAccum = 0.0f;
	m_eYDataMean = 0;
	//  calc the min and max.
	for (UINT ii=0; ii<m_uiPointCount; ii++)
	{
		m_PointData[ii] = pvecData->at(ii);
		xAccum += m_PointData[ii].y;
		m_eXDataMin = (pvecData->at(ii).x<m_eXDataMin) ? (pvecData->at(ii).x) : (m_eXDataMin);
		m_eXDataMax = (pvecData->at(ii).x>m_eXDataMax) ? (pvecData->at(ii).x) : (m_eXDataMax);
		m_eYDataMin = (pvecData->at(ii).y<m_eYDataMin) ? (pvecData->at(ii).y) : (m_eYDataMin);
		m_eYDataMax = (pvecData->at(ii).y>m_eYDataMax) ? (pvecData->at(ii).y) : (m_eYDataMax);
	}
	//  set the mean.
	m_eYDataMean = (float)( xAccum / (double)(m_uiPointCount) );
	//  force a recalc of the screen polyline.
	m_oIsDirty = true;
	return true;
}
/////////////////////////////////////////////////////////////////////////////

CPlotData::~CPlotData()
{
	Clear();
}
/////////////////////////////////////////////////////////////////////////////

void CPlotData::Clear()
{
	//  clean up the memory.
	if (m_PointData!=NULL)
	{
		delete m_PointData;
	}
	if (m_pptScreen!=NULL)
	{
		delete m_pptScreen;
	}
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//  CLinePlot control
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CLinePlot, CWnd)
	//{{AFX_MSG_MAP(CLinePlot)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CLinePlot::CLinePlot()
{
	m_SelectedDataItemNo = -1;

	m_XScaleInterval = 1;

	m_szXCaption = "X Axis";
	m_szYCaption = "Y Axis";
	m_nXIntervals = 5;
	m_nYIntervals = 5;
	m_uiXPrecision = 1;
	m_uiYPrecision = 1;
	m_eXMin = 0.0f;
	m_eXMax = 2;
	m_eYMin = 0.0f;
	m_eYMax = 1.0f;
	memset(m_prcUndoLimits, 0, LINE_PLOT_UNDO_SIZE*sizeof(FLOATRECT));
	m_nUndoIndex = -1;
	m_eXRatio = 1.0f;
	m_eYRatio = 1.0f;
	m_rcXLock.SetRectEmpty();
	m_rcYLock.SetRectEmpty();
	m_rcKeyArea.SetRectEmpty();
	m_rcPlotArea.SetRectEmpty();
	m_rcXMin.SetRectEmpty();
	m_rcXMax.SetRectEmpty();
	m_rcYMin.SetRectEmpty();
	m_rcYMax.SetRectEmpty();
	m_nCursor = -1;
	m_nSelected = -1;
	m_nMouseOverKey = -1;
	m_uiMouseOver = PLOT_OVER_NOTHING;
	m_rcZoom.SetRectEmpty();
	m_oIsMouseDown = false;
	m_oHasPassedOverPlot = false;
	m_oIsXLocked = false;
	m_oIsYLocked = false;

	//  add one to the zoom undo to start.
	FLOATRECT rcLimits;
	rcLimits.left = m_eXMin;
	rcLimits.top = m_eYMax;
	rcLimits.right = m_eXMax;
	rcLimits.bottom = m_eYMin;
	AddToZoomUndo(rcLimits);
}
/////////////////////////////////////////////////////////////////////////////

CLinePlot::~CLinePlot()
{
	Clear();
}
/////////////////////////////////////////////////////////////////////////////

BOOL CLinePlot::Create(const RECT &rect, UINT uiFlags, CWnd *pParentWnd, UINT uiID)
{
	BOOL oOkay = CWnd::Create(NULL, _T("LinePlot"), uiFlags, rect, pParentWnd, uiID);

	if (oOkay==TRUE)
	{
		//  create an edit box.
		DWORD dwEditStyle = WS_BORDER | WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL; //|ES_MULTILINE
		m_edtDataEntry.Create(dwEditStyle, CRect(0, 0, 1, 1), this, ID_EDT_DATA_ENTRY);
		//  set the scale font as the font for the edit box.
		//m_edtDataEntry.SetFont(&m_fonScale);
	}

	return oOkay;
}
/////////////////////////////////////////////////////////////////////////////

BOOL CLinePlot::OnEraseBkgnd(CDC* pDC) 
{
	//  to eliminate flicker....
	return 1;
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::DrawPlotData(CDC* pDC, UINT uiIndex, CRect &rcBounds)
{
	//  define a clipping region so we don't
	//  draw the data outside of the graph area.
	pDC->IntersectClipRect(rcBounds);

	//  should we recalculate the screen coordinates?
	if (m_lstPlotData[uiIndex].m_oIsDirty==true)
	{
		//  create the screen data points.
		double xXScreenRatio = rcBounds.Width() / (m_eXMax - m_eXMin);
		double xYScreenRatio = rcBounds.Height() / (m_eYMax - m_eYMin);
		for (UINT jj=0; jj < m_lstPlotData[uiIndex].m_uiPointCount; jj++)
		{
			m_lstPlotData[uiIndex].m_pptScreen[jj].x = rcBounds.left +
				(int)( ( m_lstPlotData[uiIndex].m_PointData[jj].x - m_eXMin ) 
				* xXScreenRatio );
			m_lstPlotData[uiIndex].m_pptScreen[jj].y = rcBounds.bottom - 
				(int)( ( m_lstPlotData[uiIndex].m_PointData[jj].y  - m_eYMin ) 
				* xYScreenRatio );
		}
		//  the screen points are up-to-date.
		m_lstPlotData[uiIndex].m_oIsDirty = false;
	}

	UINT ii = 0;
	int nLastWidth = 1;

	switch (m_lstPlotData[uiIndex].m_nStyle)
	{
	case enum_LpLine:
		{
			//  create a pen to draw with.
			CPen penPlot(PS_SOLID, 1, m_lstPlotData[uiIndex].m_crColor);
			CPen *ppenOld = pDC->SelectObject(&penPlot);
			//  draw the polyline
			pDC->Polyline(m_lstPlotData[uiIndex].m_pptScreen, 
				m_lstPlotData[uiIndex].m_uiPointCount);
			pDC->SelectObject(ppenOld);
			break;
		}
	case enum_LpScatter:
		{
			//  draw the dots

			int p;
			for(p = 0; p< m_lstPlotData[uiIndex].m_uiPointCount; p++)
			{
			int x = m_lstPlotData[uiIndex].m_pptScreen[p].x;
			int y = m_lstPlotData[uiIndex].m_pptScreen[p].y;

			CPen penPlot(PS_SOLID, 1, m_lstPlotData[uiIndex].m_PointData[p].crColor);
			CPen *ppenOld = pDC->SelectObject(&penPlot);

			int size = 2;
			
			int hour  = m_lstPlotData[uiIndex].m_PointData[p].Hour;
			if( hour == -1 || hour >=  m_StartHour ||  hour <= m_StartHour  + m_AggregationWindow)
			{
			CPen penPlot(PS_SOLID, 1, m_lstPlotData[uiIndex].m_PointData[p].crColor);
			CPen *ppenOld = pDC->SelectObject(&penPlot);
			pDC->Rectangle(x-size,y-size,x+size,y+size);
			pDC->SelectObject(ppenOld);
			}
			//else
			//{
			//CPen penPlot(PS_SOLID, 2, m_lstPlotData[uiIndex].m_PointData[p].crColor);
			//CPen *ppenOld = pDC->SelectObject(&penPlot);
			//pDC->Rectangle(x-size,y-size,x+size,y+size);
			//pDC->SelectObject(ppenOld);
			//}


		   }

			if (m_SelectedDataItemNo >=0)
			{
			p = m_SelectedDataItemNo;
			int x = m_lstPlotData[uiIndex].m_pptScreen[p].x;
			int y = m_lstPlotData[uiIndex].m_pptScreen[p].y;

			CPen penSelectPlot(PS_SOLID, 2, RGB(255,0,0));
			CPen *ppenOld = pDC->SelectObject(&penSelectPlot);
		
			int size = 4;
			pDC->Rectangle(x-size,y-size,x+size,y+size);
			pDC->SelectObject(ppenOld);
			}

			// diag  line
			CPen penBlack(PS_SOLID, 1, RGB(0,0,0));
			pDC->SelectObject(&penBlack);

		double xXScreenRatio = rcBounds.Width() / (m_eXMax - m_eXMin);
		double xYScreenRatio = rcBounds.Height() / (m_eYMax - m_eYMin);

		float Max_value  = min(m_eXMax, m_eYMax);

			pDC->MoveTo(rcBounds.left,rcBounds.bottom);

			float Max_value_screen_x = rcBounds.left +
				(int)( Max_value - m_eXMin ) 
				* xXScreenRatio ;

			float Max_value_screen_y = rcBounds.bottom - 
				(int)( Max_value - m_eYMin ) 
				* xYScreenRatio ;

			pDC->LineTo(Max_value_screen_x, Max_value_screen_y);
		
			


			break;
		}
	case enum_LpBar:
		{
			//  draw the bars
			RECT rcBar;
			//  save the background color.
			COLORREF crBack = pDC->GetBkColor();
			for (ii=0; ii<m_lstPlotData[uiIndex].m_uiPointCount; ii++)
			{
				//  create a rect.
				rcBar.left = (m_lstPlotData[uiIndex].m_pptScreen[ii].x<=rcBounds.left) ?
					(rcBounds.left+1) : (m_lstPlotData[uiIndex].m_pptScreen[ii].x);
				rcBar.top = m_lstPlotData[uiIndex].m_pptScreen[ii].y;
				//  the last one is a special case.
				if (ii==m_lstPlotData[uiIndex].m_uiPointCount-1)
				{
					rcBar.right = m_lstPlotData[uiIndex].m_pptScreen[ii].x + nLastWidth;
				}
				else
				{
					rcBar.right = m_lstPlotData[uiIndex].m_pptScreen[ii+1].x;
					nLastWidth = m_lstPlotData[uiIndex].m_pptScreen[ii+1].x - 
						m_lstPlotData[uiIndex].m_pptScreen[ii].x;
				}
				rcBar.bottom = rcBounds.bottom;
				//  draw the rect.
				pDC->FillSolidRect(&rcBar, m_lstPlotData[uiIndex].m_crColor);
			}
			//  restore the background color.
			pDC->SetBkColor(crBack);
			break;
		}
	default:
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////

CRect CLinePlot::DrawFramework(CDC *pDC, CRect rcBounds, int nFontHeight, bool oIsPrinting)
{
	CPen *ppenOld;
	CFont *pfonOld;
	CFont fonScale;
	CFont fonHorz;
	CFont fonVert;

	//  create the fonts to use here.
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	//lf.lfWeight = FW_NORMAL;
	lf.lfHeight = nFontHeight;
	fonScale.CreateFontIndirect(&lf);
	lf.lfWeight = FW_BOLD;
	fonHorz.CreateFontIndirect(&lf);
	lf.lfEscapement = 900;
	fonVert.CreateFontIndirect(&lf);

	//  create a white brush for the background.
	CBrush bkBrush(RGB(255, 255, 255));
	//  create a gray pen for the ruler lines.
	CPen penGray(PS_SOLID, 1, RGB(220, 220, 220));
	//  create a black brush for the data rectangle.
	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));

	//  get the height of the caption for the font height.
	int nCaptionHeight = nFontHeight + 4;
	int nSmCaptionHeight = (int)(nCaptionHeight * 0.75f); //  GetSystemMetrics(SM_CYSMCAPTION);
	int nSpacer = nCaptionHeight/2;

	//  calculate the usable area to draw in.
	CRect rcPlotArea(rcBounds);
	rcPlotArea.DeflateRect(5, 5, 5, 5);

	//  calculate the rect for the Y caption.
	CRect rcYCaption(rcPlotArea);
	rcYCaption.right = rcYCaption.left + nCaptionHeight;

	//  calculate the rect for the X caption.
	CRect rcXCaption(rcPlotArea);
	rcXCaption.left = rcXCaption.left + nCaptionHeight;
	rcXCaption.top = rcXCaption.bottom - nCaptionHeight;

	//  adjust the client area.
	rcPlotArea.left += nCaptionHeight+20; // + nSpacer;
	rcPlotArea.bottom -= nCaptionHeight + nSpacer;

	//  calculate the widest rect for the y scale.
	CString szTestNumMin;
	szTestNumMin.Format("%0.f", m_eYMin);
	int nNumCharsMin = szTestNumMin.GetLength();
	CString szTestNumMax;
	szTestNumMax.Format("%0.f", m_eYMax);
	int nNumCharsMax = szTestNumMin.GetLength();
	int nNumChars = (nNumCharsMin<nNumCharsMax) ? (nNumCharsMax) : (nNumCharsMin);
	CString szScaleTest('9', nNumChars+3);
	//  use the font for the scale.
	pfonOld = pDC->SelectObject(&fonScale);
	//  calculate the nominal height and width of each scale entry.
	SIZE siScale = pDC->GetTextExtent(szScaleTest);

	//  create a starting rect for the scale.
	CRect rcStartScale(rcPlotArea);
	rcStartScale.right = rcStartScale.left + siScale.cx;
	rcStartScale.top = rcStartScale.bottom - siScale.cy;

	//  shrink the client rect by the height & width of the scale + a little more. 
	rcPlotArea.left += siScale.cx + nSpacer/2;
	rcPlotArea.bottom -= siScale.cy + nSpacer/2;
	rcPlotArea.right -= nCaptionHeight;
	rcPlotArea.top += nSpacer;

	//  create a rect to hold the stats at the top.
	CRect rcStats(rcPlotArea);
	rcStats.bottom = rcStats.top + siScale.cy;

	//  shrink the client rect away from the stats rect.
	rcPlotArea.top += siScale.cy;

	////  should we calculate a key rect?
	//if (m_lstPlotData.size()>=1)
	//{
	//	//  what is the longest key?
	//	int nMaxKey = 5;
	//	for (int ii=0; ii<m_lstPlotData.size(); ii++)
	//	{
	//		nMaxKey = (m_lstPlotData[ii].m_szName.GetLength() > nMaxKey) ?
	//			(m_lstPlotData[ii].m_szName.GetLength()) : (nMaxKey);
	//	}
	//	int nKeyWidth = (nFontHeight*nMaxKey)/2;
	//	//  create a rect to hold the key
	//	m_rcKeyArea = rcPlotArea;
	//	m_rcKeyArea.left = m_rcKeyArea.right-nKeyWidth;
	//	m_rcKeyArea.right = m_rcKeyArea.right+10;

	//	//  center & shrink the key rect to hold only the number of plots.
	//	int nYCenter = (m_rcKeyArea.top + m_rcKeyArea.bottom) / 2;
	//	m_rcKeyArea.top = nYCenter - ((siScale.cy+4) * m_lstPlotData.size()) / 2;
	//	m_rcKeyArea.bottom = nYCenter + ((siScale.cy+4) * m_lstPlotData.size()) / 2;

	//	//  shrink the client rect away from the key rect.
	//	rcPlotArea.right -= (nKeyWidth+10);
	//	//  shrink the stats rect away from the key rect.
	//	rcStats.right -= (nKeyWidth+10);
	//}

	//  adjust the space for the x caption so that it is centered.
	rcXCaption.left = rcPlotArea.left;
	rcXCaption.right = rcPlotArea.right;

	//  adjust the space for the y caption so that it is centered.
	rcYCaption.top = rcPlotArea.top;
	rcYCaption.bottom = rcPlotArea.bottom;

	//  calculate the rect for the min x.
	m_rcXMin = rcStartScale;
	m_rcXMin.OffsetRect(nSpacer/2, 0);
	m_rcXMin.InflateRect(2, 2, 2, 2);

	//  calculate the rect for the max x.
	m_rcXMax = rcStartScale;
	m_rcXMax.OffsetRect(rcPlotArea.right - m_rcXMax.right, 0);
	m_rcXMax.InflateRect(2, 2, 2, 2);

	//  calculate the rect for the x-locked icon in the corner.
	m_rcXLock.left = rcYCaption.left;
	m_rcXLock.top = m_rcXMin.top;
	m_rcXLock.right = rcYCaption.right;
	m_rcXLock.bottom = m_rcXMin.bottom;

	//  calculate the rect for the min y.
	m_rcYMin = rcStartScale;
	m_rcYMin.OffsetRect(0, rcPlotArea.bottom - m_rcYMin.bottom);
	m_rcYMin.InflateRect(2, 2, 2, 2);

	//  calculate the rect for the max y.
	m_rcYMax = rcStartScale;
	m_rcYMax.OffsetRect(0, rcPlotArea.top - m_rcYMax.bottom);
	m_rcYMax.InflateRect(2, 2, 2, 2);

	//  calculate the rect for the y-locked icon in the corner.
	m_rcYLock.left = m_rcYMin.right - m_rcXLock.Width();
	m_rcYLock.top = rcXCaption.top;
	m_rcYLock.right = m_rcYMin.right;
	m_rcYLock.bottom = rcXCaption.bottom;

	//  calculate how big each screen interval is.
	float eScreenXInterval = (float)(rcPlotArea.Width()) / (float)(m_nXIntervals);
	float eScreenYInterval = (float)(rcPlotArea.Height()) / (float)(m_nYIntervals);

	//  calculate how big each actual interval is.
	float eXInterval = (m_eXMax - m_eXMin) / (float)(m_nXIntervals);
	float eYInterval = (m_eYMax - m_eYMin) / (float)(m_nYIntervals);

	//  now we can calculate the ratio of actual data range 
	//  to screen area.
	m_eXRatio = (float)(rcPlotArea.Width()) / (m_eXMax - m_eXMin);
	m_eYRatio = (float)(rcPlotArea.Height()) / (m_eYMax - m_eYMin);

	//  calculate the format for the scale.
	CString szXFormat;
//	szXFormat.Format("%%0.%uf", m_uiXPrecision);
	szXFormat.Format("%%0.0f");
	CString szYFormat;
//	szYFormat.Format("%%0.%uf", m_uiYPrecision);
	szYFormat.Format("%%0.1f");

	/*
	...... now start drawing!
	*/

	//  fill the total background.
	pDC->FillSolidRect(&rcBounds, GetSysColor(COLOR_WINDOW));

	//  draw the sunken edge around the whole control.
	if (oIsPrinting==false)
		pDC->DrawEdge(rcBounds, EDGE_SUNKEN, BF_RECT);

	//  do we have enough room to draw?
	if (rcPlotArea.left < rcPlotArea.right)
	{

		//  draw the y caption
		pfonOld = pDC->SelectObject(&fonVert);
		pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
		pDC->TextOut(rcYCaption.right, (rcYCaption.top+rcYCaption.bottom)/2, m_szYCaption);

		//  draw the x caption
		pfonOld = pDC->SelectObject(&fonHorz);
		pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
		pDC->TextOut((rcXCaption.left+rcXCaption.right)/2, rcXCaption.bottom, m_szXCaption);

		//  setup to draw the scale.
		pDC->SetTextAlign(TA_RIGHT | TA_BOTTOM);
		POINT ptXScale;
		POINT ptYScale;
		CString szScale;
		pfonOld = pDC->SelectObject(&fonScale);
		ppenOld = pDC->SelectObject(&penGray);

		//  draw each x scale entry & line.
		for (int ii=0; ii<m_nXIntervals+1; ii+=m_XScaleInterval)
		{
			szScale.Format(szXFormat, m_eXMin+ii*eXInterval);
			ptXScale.x = (int)(rcStartScale.right+(float)(ii)*eScreenXInterval) + nSpacer/2;
			ptXScale.y = rcStartScale.bottom;
			pDC->TextOut(ptXScale.x, ptXScale.y, szScale);
			pDC->MoveTo(ptXScale.x, rcPlotArea.top);
			pDC->LineTo(ptXScale.x, rcPlotArea.bottom);
		}

		//  is the mouse over the x min?
		if (m_uiMouseOver&PLOT_OVER_X_MIN && oIsPrinting==false)
		{
			pDC->DrawEdge(m_rcXMin, BDR_RAISEDOUTER, BF_RECT);
		}

		//  is mouse over the x max?
		if (m_uiMouseOver&PLOT_OVER_X_MAX && oIsPrinting==false)
		{
			pDC->DrawEdge(m_rcXMax, BDR_RAISEDOUTER, BF_RECT);
		}

		//  draw each y scale entry & line.
		for (int jj=0; jj<m_nYIntervals+1; jj++)
		{
			szScale.Format(szYFormat, m_eYMin+jj*eYInterval);
			ptYScale.x = rcStartScale.right;
			ptYScale.y = (int)(rcStartScale.top-(float)(jj)*eScreenYInterval) - nSpacer/2;
			pDC->TextOut(ptYScale.x, ptYScale.y, szScale);
			pDC->MoveTo(rcPlotArea.left, ptYScale.y);
			pDC->LineTo(rcPlotArea.right, ptYScale.y);
		}

		//  is the mouse over the y min?
		if (m_uiMouseOver&PLOT_OVER_Y_MIN && oIsPrinting==false)
		{
			pDC->DrawEdge(m_rcYMin, BDR_RAISEDOUTER, BF_RECT);
		}

		//  is the mouse over the y max?
		if (m_uiMouseOver&PLOT_OVER_Y_MAX && oIsPrinting==false)
		{
			pDC->DrawEdge(m_rcYMax, BDR_RAISEDOUTER, BF_RECT);
		}

		pDC->SelectObject(&penBlack);

		//  draw black lines around the data.
		pDC->MoveTo(rcPlotArea.left, ptYScale.y);
		pDC->LineTo(rcPlotArea.right, ptYScale.y);
		pDC->LineTo(rcPlotArea.right, rcPlotArea.bottom);
		pDC->LineTo(rcPlotArea.left, rcPlotArea.bottom);
		pDC->LineTo(rcPlotArea.left, ptYScale.y);

		//  is the mouse over the x-lock area?
		if (m_uiMouseOver&PLOT_OVER_X_LOCK && oIsPrinting==false)
		{
			if (m_oIsMouseDown==true)
				pDC->DrawEdge(m_rcXLock, BDR_SUNKENINNER, BF_RECT);
			else
				pDC->DrawEdge(m_rcXLock, BDR_RAISEDOUTER, BF_RECT);
		}

		//  get the center of the x-lock area.
		//POINT ptXLock = m_rcXLock.CenterPoint();
		////  draw the bottom of the lock.
		//CRect rcXLockBottom;
		//rcXLockBottom.left = ptXLock.x - 5;
		//rcXLockBottom.right = ptXLock.x + 6;
		//rcXLockBottom.top = ptXLock.y - 2;
		//rcXLockBottom.bottom = ptXLock.y + 7;
		//pDC->FillSolidRect(rcXLockBottom, RGB(0, 0, 0));
		//pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		////  draw the lock top.
		//pDC->MoveTo(ptXLock.x - 4, ptXLock.y);
		//pDC->LineTo(ptXLock.x - 2, ptXLock.y - 7);
		//pDC->LineTo(ptXLock.x + 2, ptXLock.y - 7);
		//if (m_oIsXLocked==true)
		//	pDC->LineTo(ptXLock.x + 4, ptXLock.y);
		//else
		//	pDC->LineTo(ptXLock.x + 4, ptXLock.y-6);

		////  is the mouse over the y-lock area?
		//if (m_uiMouseOver&PLOT_OVER_Y_LOCK && oIsPrinting==false)
		//{
		//	if (m_oIsMouseDown==true)
		//		pDC->DrawEdge(m_rcYLock, BDR_SUNKENINNER, BF_RECT);
		//	else
		//		pDC->DrawEdge(m_rcYLock, BDR_RAISEDOUTER, BF_RECT);
		//}

		////  get the center of the y-lock area.
		//POINT ptYLock = m_rcYLock.CenterPoint();
		////  draw the bottom of the lock.
		//CRect rcYLockBottom;
		//rcYLockBottom.left = ptYLock.x - 5;
		//rcYLockBottom.right = ptYLock.x + 6;
		//rcYLockBottom.top = ptYLock.y - 2;
		//rcYLockBottom.bottom = ptYLock.y + 7;
		//pDC->FillSolidRect(rcYLockBottom, RGB(0, 0, 0));
		//pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		////  draw the lock top.
		//pDC->MoveTo(ptYLock.x - 4, ptYLock.y);
		//pDC->LineTo(ptYLock.x - 2, ptYLock.y - 7);
		//pDC->LineTo(ptYLock.x + 2, ptYLock.y - 7);
		//if (m_oIsYLocked==true)
		//	pDC->LineTo(ptYLock.x + 4, ptYLock.y);
		//else
		//	pDC->LineTo(ptYLock.x + 4, ptYLock.y-6);

		////  should we draw a key?
		//if (m_lstPlotData.size()>1)
		//{
		//	//  setup to draw the keys.
		//	POINT ptLeftBottom;
		//	CString szName;
		//	pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);
		//	CRect rcKey;
		//	CRect rcColor;
		//	//  draw all the keys
		//	for (int ii=0; ii<m_lstPlotData.size(); ii++)
		//	{
		//		//  get the color.
		//		COLORREF crKey = m_lstPlotData[ii].m_crColor;
		//		//  get the name.
		//		szName = m_lstPlotData[ii].m_szName;
		//		//  calculate the bottom corner of the text.
		//		ptLeftBottom.x = m_rcKeyArea.left + siScale.cy + 4;
		//		ptLeftBottom.y = m_rcKeyArea.top + (ii+1)*(siScale.cy+4) - 2;
		//		//  calculate a rect around the text.
		//		rcKey.left = m_rcKeyArea.left;
		//		rcKey.right = m_rcKeyArea.right;
		//		rcKey.top = m_rcKeyArea.top + (ii)*(siScale.cy+4);
		//		rcKey.bottom = m_rcKeyArea.top + (ii+1)*(siScale.cy+4) - 1;
		//		m_lstPlotData[ii].m_rcKeyArea = rcKey;
		//		//  get a rect for the color.
		//		rcColor = rcKey;
		//		rcColor.right = rcKey.left + siScale.cy+4;
		//		rcColor.DeflateRect(4, 4, 4, 4);
		//		//  draw the color;
		//		pDC->FillSolidRect(&rcColor, crKey);
		//		pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		//		//  draw the key text.
		//		pDC->TextOut(ptLeftBottom.x, ptLeftBottom.y, szName);
		//		//  is this one selected?
		//		if (ii==m_nSelected && oIsPrinting==false)
		//		{
		//			//  draw a rect around the key.
		//			pDC->DrawEdge(rcKey, BDR_SUNKENINNER, BF_RECT);
		//		}
		//		//  is the mouse over a key?
		//		else if (ii==m_nMouseOverKey && 
		//			(m_uiMouseOver&PLOT_OVER_KEY) && oIsPrinting==false)
		//		{
		//			//  draw a rect around the key.
		//			pDC->DrawEdge(rcKey, BDR_RAISEDOUTER, BF_RECT);
		//		}
		//	}
		//}

		//  draw the position & stats.
		/*
		CString szPosition;
		int nCount = m_lstPlotData.size();
		if (nCount>0 && m_nSelected>=0 && m_nSelected<nCount)
		{
		if (m_nCursor >= 0 && (UINT)(m_nCursor) < m_lstPlotData[m_nSelected].m_uiPointCount)
		{
		FLOATPOINT ptDisplay;
		ptDisplay.x = m_lstPlotData[m_nSelected].m_PointData[m_nCursor].x;
		ptDisplay.y = m_lstPlotData[m_nSelected].m_PointData[m_nCursor].y;
		szPosition.Format("Position: %0.3f    Value: %0.3f",
		ptDisplay.x, ptDisplay.y);
		pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);
		pDC->TextOut(rcStats.left+1, rcStats.bottom-1, szPosition);
		}

		float eYMin = m_lstPlotData[m_nSelected].m_eYDataMin;
		float eYMax = m_lstPlotData[m_nSelected].m_eYDataMax;
		float eYMean = m_lstPlotData[m_nSelected].m_eYDataMean;


		szPosition.Format("Mean: %0.3f    Min: %0.3f    Max: %0.3f",
		eYMean, eYMin, eYMax);
		pDC->SetTextAlign(TA_RIGHT | TA_BOTTOM);
		pDC->TextOut(rcStats.right-1, rcStats.bottom-1, szPosition);

		}
		*/
	}

	//  delete the fonts.
	DeleteObject(&fonScale);
	DeleteObject(&fonHorz);
	DeleteObject(&fonVert);


	//  return the area we have to draw the plot in.
	return rcPlotArea;
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::DrawData(CDC *pDC, CRect rcBounds, bool oIsPrinting) 
{
	//  draw all the plots in the correct order.
	for (int ii=0; ii<m_lstPlotData.size(); ii++)
	{
		//  if we are printing, force a recalc of the screen plot data.
		if (oIsPrinting==true)
			m_lstPlotData[ii].m_oIsDirty = true;

		if(ii==m_nSelected)
		{
			DrawPlotData(pDC, (UINT)(ii), rcBounds);
		}
		//  if we are printing, force a recalc of the screen plot data.
		if (oIsPrinting==true)
			m_lstPlotData[ii].m_oIsDirty = true;
	}

	int nCount = m_lstPlotData.size();
	CPen penDotBlack(PS_DOT, 1, RGB(0, 0, 0));
	CPen *ppenOld = pDC->SelectObject(&penDotBlack);

	//  should we draw a zoom rect?
	if (m_oIsMouseDown==true && oIsPrinting==false)
	{
		pDC->MoveTo(m_rcZoom.left, m_rcZoom.top);
		pDC->LineTo(m_rcZoom.right, m_rcZoom.top);
		pDC->LineTo(m_rcZoom.right, m_rcZoom.bottom);
		pDC->LineTo(m_rcZoom.left, m_rcZoom.bottom);
		pDC->LineTo(m_rcZoom.left, m_rcZoom.top);
	}

	////  draw the cursor position.
	//else if (nCount>0 && m_nSelected>=0 && 
	//	m_nSelected<nCount && oIsPrinting==false)
	//{
	//	if (m_nCursor>=0 && (UINT)(m_nCursor) < m_lstPlotData[m_nSelected].m_uiPointCount)
	//	{
	//		//  set the cursor position on the plot.
	//		POINT ptCursor;
	//		ptCursor.x = m_lstPlotData[m_nSelected].m_pptScreen[m_nCursor].x;
	//		ptCursor.y = m_lstPlotData[m_nSelected].m_pptScreen[m_nCursor].y;
	//		pDC->MoveTo(m_rcPlotArea.left, ptCursor.y);
	//		pDC->LineTo(m_rcPlotArea.right, ptCursor.y);
	//		pDC->MoveTo(ptCursor.x, rcBounds.top);
	//		pDC->LineTo(ptCursor.x, rcBounds.bottom);
	//	}
	//}
	pDC->SelectObject(ppenOld);
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//  get the area to draw in.
	CRect rcClient;
	GetClientRect(&rcClient);

	//  get the height of the font to use.
//	int nFontHeight = GetSystemMetrics(SM_CYCAPTION);

	int nFontHeight = 14;
	//  create a memory dc for double buffering.
	CMemDc dcMem(&dc, &rcClient);

	dcMem.SaveDC();

	//  draw all the support graphics.
	m_rcPlotArea = DrawFramework(&dcMem, rcClient, nFontHeight, false);
	//  draw the plots.
	if (m_rcPlotArea.left < m_rcPlotArea.right)
	{
		DrawData(&dcMem, m_rcPlotArea, false);
	}

	dcMem.RestoreDC(-1);
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::Print()
{
	// print dialog options
	CPrintDialog dlgPrint(FALSE,
		PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_NOSELECTION | PD_DISABLEPRINTTOFILE,
		this);

	//  show the print dialog.
	if(dlgPrint.DoModal() == IDOK)
	{
		//  get handle to printer object.
		HDC hdcPrinter = dlgPrint.GetPrinterDC();
		if (hdcPrinter!=NULL)
		{
			// get printer dc
			CDC dcPrinter;
			dcPrinter.Attach(hdcPrinter);

			//  fill the docinfo structure.
			DOCINFO diPrinter;
			::ZeroMemory(&diPrinter, sizeof(DOCINFO));
			diPrinter.cbSize = sizeof(DOCINFO);
			diPrinter.lpszOutput = NULL;
			diPrinter.lpszDocName = m_szName.GetBuffer(1);

			if (dcPrinter.StartDoc(&diPrinter) >= 0)
			{
				// start page
				VERIFY(dcPrinter.StartPage() >= 0);

				//  get the printer width.
				int nPrinterWidth = 0;
				nPrinterWidth = dcPrinter.GetDeviceCaps(HORZRES);

				//  get the printer height.
				int nPrinterHeight = 0;
				nPrinterHeight = dcPrinter.GetDeviceCaps(VERTRES);

				//  claculate a size for the text.
				int nFontHeight = (int)(nPrinterHeight / 120.0f);

				//  get the area to draw in.
				CRect rcClient(0, 0, nPrinterWidth, nPrinterHeight);

				dcPrinter.SaveDC();

				//  draw all the support graphics.
				CRect rcPlotArea = DrawFramework(&dcPrinter, rcClient, nFontHeight, true);
				//  draw the plots.
				DrawData(&dcPrinter, rcPlotArea, true);

				dcPrinter.RestoreDC(-1);

				VERIFY(dcPrinter.EndPage() >= 0);
				VERIFY(dcPrinter.EndDoc() >= 0);

			}
			else //  there was a printer error of some sort....
			{
				VERIFY(dcPrinter.AbortDoc( ) >= 0 );
				AfxMessageBox("Cannot Start Print Job.");
			}
			//  clean up
			dcPrinter.Detach();
			VERIFY( DeleteDC(hdcPrinter));
		}
	}
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	Refresh();
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::Refresh()
{
	//  force a recalculation of all the plots next repaint.
	for (int ii=0; ii<m_lstPlotData.size(); ii++)
	{
		m_lstPlotData[ii].m_oIsDirty = true;
	}
	if (m_hWnd!=NULL)
		Invalidate();
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::Clear()
{
	m_lstPlotData.clear();

	//  redraw if we can....
	if (m_hWnd!=NULL)
		Invalidate();

	//  nothing is selected.
	m_nSelected = -1;
}
/////////////////////////////////////////////////////////////////////////////

CPlotData &CLinePlot::operator[](UINT unIndex)
{
	return m_lstPlotData.at(unIndex);
};
/////////////////////////////////////////////////////////////////////////////

int CLinePlot::Add(CString szName, COLORREF crColor, enumPlotStyle nStyle, FLOATPOINT *pptData, UINT uiPointCount, int LinkNo)
{
	//  create a new plot
	CPlotData PlotData(szName, crColor, (int)(nStyle), pptData, uiPointCount);
	m_lstPlotData.push_back(PlotData);

	//  change the selected one to this one.
	m_nSelected = m_lstPlotData.size()-1;
	//  should the x-axis limits be adjusted?
	if (m_oIsXLocked == false)
	{
		if (PlotData.m_eXDataMin < m_eXMin)
			m_eXMin = PlotData.m_eXDataMin;
		if (PlotData.m_eXDataMax > m_eXMax)
			m_eXMax = PlotData.m_eXDataMax;
		for (UINT ii=0; ii < m_lstPlotData.size(); ii++)
			m_lstPlotData[ii].m_oIsDirty = true;
	}
	//  should the y-axis limits be adjusted?
	if (m_oIsYLocked == false)
	{
		if (PlotData.m_eYDataMin < m_eYMin)
			m_eYMin = PlotData.m_eYDataMin;
		if (PlotData.m_eYDataMax > m_eYMax)
			m_eYMax = PlotData.m_eYDataMax;
		for (UINT ii=0; ii < m_lstPlotData.size(); ii++)
			m_lstPlotData[ii].m_oIsDirty = true;
	}

	if(nStyle == enum_LpScatter)  // xy scatter plot
	{
		// use the same scale
	m_eXMax = max(m_eXMax, m_eYMax);
	m_eYMax = max(m_eXMax, m_eYMax);
	}

	if (m_hWnd!=NULL)
		Invalidate();
	EmitSelectionChanged();
	return m_nSelected;
}
/////////////////////////////////////////////////////////////////////////////

int CLinePlot::Add(CString szName, COLORREF crColor, enumPlotStyle nStyle, std::vector<FLOATPOINT> *pvecData)
{
	//  create a new plot
	CPlotData PlotData(szName, crColor, (int)(nStyle), pvecData);
	m_lstPlotData.push_back(PlotData);
	//  change the selected one to this one.
	m_nSelected = m_lstPlotData.size()-1;
	//  should the x-axis limits be adjusted?
	if (m_oIsXLocked == false)
	{
		if (PlotData.m_eXDataMin < m_eXMin)
			m_eXMin = PlotData.m_eXDataMin;
		if (PlotData.m_eXDataMax > m_eXMax)
			m_eXMax = PlotData.m_eXDataMax;
		for (UINT ii=0; ii < m_lstPlotData.size(); ii++)
			m_lstPlotData[ii].m_oIsDirty = true;
	}
	//  should the y-axis limits be adjusted?
	if (m_oIsYLocked == false)
	{
		if (PlotData.m_eYDataMin < m_eYMin)
			m_eYMin = PlotData.m_eYDataMin;
		if (int(PlotData.m_eYDataMax+1) > m_eYMax)
			m_eYMax = (int)(PlotData.m_eYDataMax+1);  // set to integer numbers
		for (UINT ii=0; ii < m_lstPlotData.size(); ii++)
			m_lstPlotData[ii].m_oIsDirty = true;
	}
	if (m_hWnd!=NULL)
		Invalidate();
	EmitSelectionChanged();
	return m_nSelected;
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::Remove(int nIndex)
{
	//  remove a plot.
	bool oRetVal = false;
	if (nIndex<0 || nIndex>=m_lstPlotData.size())
		return false;
	else
	{
		m_lstPlotData.erase(m_lstPlotData.begin()+nIndex);
		//  change the selected by one, if necessary.
		if (m_nSelected >= nIndex)
		{
			m_nSelected--;
			int nMaxIndex = m_lstPlotData.size() - 1;
			//  make sure the selection makes sense.
			if (m_nSelected > nMaxIndex)
				m_nSelected = nMaxIndex;
			else if (m_nSelected < 0)
				m_nSelected = 0;
			EmitSelectionChanged();
		}
		if (m_hWnd!=NULL)
			Invalidate();
		return true;
	}
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::OnMouseMove(UINT nFlags, CPoint point) 
{
	UINT uiMouseOver = PLOT_OVER_NOTHING;
	int nMouseOverKey = -1;

	//  are we drawing a zoom rect?
	if (m_oIsMouseDown==true)
	{
		if (m_rcPlotArea.PtInRect(point)==TRUE)
			m_oHasPassedOverPlot = true;
		m_rcZoom.right = point.x;
		m_rcZoom.bottom = point.y;
		if (m_hWnd!=NULL)
			Invalidate();
	}

	//  is the mouse over the x min button?
	else if (m_rcXMin.PtInRect(point)==TRUE)
	{
		uiMouseOver |= PLOT_OVER_X_MIN;
	}
	//  is the mouse over the x max button?
	if (m_rcXMax.PtInRect(point)==TRUE)
	{
		uiMouseOver |= PLOT_OVER_X_MAX;
	}
	//  is the mouse over the y min button?
	if (m_rcYMin.PtInRect(point)==TRUE)
	{
		uiMouseOver |= PLOT_OVER_Y_MIN;
	}
	//  is the mouse over the y max button?
	if (m_rcYMax.PtInRect(point)==TRUE)
	{
		uiMouseOver |= PLOT_OVER_Y_MAX;
	}
	//  is the mouse over the x-lock button?
	else if (m_rcXLock.PtInRect(point)==TRUE)
	{
		uiMouseOver |= PLOT_OVER_X_LOCK;
	}
	//  is the mouse over the y-lock button?
	else if (m_rcYLock.PtInRect(point)==TRUE)
	{
		uiMouseOver |= PLOT_OVER_Y_LOCK;
	}
	//  is the mouse over the key area?
	nMouseOverKey = -1;
	CRect rcKey;
	for (int ii=0; ii<m_lstPlotData.size(); ii++)
	{
		rcKey = m_lstPlotData[ii].m_rcKeyArea;
		if (rcKey.PtInRect(point)==TRUE)
		{
			uiMouseOver |= PLOT_OVER_KEY;
			nMouseOverKey = ii;
			break;
		}
	}

/* comment it out for debuggin in the future
	//  is the mouse over the plot area?
	if (m_rcPlotArea.PtInRect(point)==TRUE)
	{
		m_uiMouseOver |= PLOT_OVER_DATA;
		//  get the nearest data point
		int nCount = m_lstPlotData.size();
		if (nCount>0 && m_nSelected>=0 && m_nSelected<nCount)
		{
			int jj = 0;
			while (m_lstPlotData[m_nSelected].m_pptScreen[jj].x < point.x)
				jj++;
			//  set the current cursor data point.
			m_nCursor = jj;
			EmitMouseMove();      
		}
	}
*/
	if (uiMouseOver!=m_uiMouseOver || nMouseOverKey!=m_nMouseOverKey)
	{
		m_uiMouseOver = uiMouseOver;
		m_nMouseOverKey = nMouseOverKey;
		//  redraw the data.
		if (m_hWnd!=NULL)
			Invalidate();
	}
	CWnd::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////

//void CLinePlot::OnLButtonDown(UINT nFlags, CPoint point) 
//{
//	//  setup to draw the zoom rect.
//	m_rcZoom.left = point.x;
//	m_rcZoom.top = point.y;
//	m_oIsMouseDown = true;
//
//	//  is the mouse over a key?
//	if (m_nMouseOverKey>=0 && (m_uiMouseOver&PLOT_OVER_KEY))
//	{
//		if (m_nSelected!=m_nMouseOverKey)
//		{
//			m_nSelected = m_nMouseOverKey;
//			if (m_hWnd!=NULL)
//				Invalidate();
//			// let the parent know that the selection changed.
//			EmitSelectionChanged();
//		}
//	}
//
//	//  is the mouse over the x-lock button?
//	else if (m_uiMouseOver&PLOT_OVER_X_LOCK)
//	{
//		if (m_hWnd!=NULL)
//			Invalidate();
//	}
//	//  is the mouse over the y-lock button?
//	else if (m_uiMouseOver&PLOT_OVER_Y_LOCK)
//	{
//		if (m_hWnd!=NULL)
//			Invalidate();
//	}
//
//	CWnd::OnLButtonDown(nFlags, point);
//}
///////////////////////////////////////////////////////////////////////////////
//
//void CLinePlot::OnLButtonUp(UINT nFlags, CPoint point) 
//{
//	CString szFormat;
//	CString szData;
//
//	//  are we drawing a zoom rect?
//	if (m_oIsMouseDown==true && m_oHasPassedOverPlot==true)
//	{
//		//  we are no longer drawing a zoom rect.
//		m_rcZoom.right = point.x;
//		m_rcZoom.bottom = point.y;
//
//		//  was it drawn backwards or forwards?
//		if (m_rcZoom.bottom<m_rcZoom.top)
//		{
//			FLOATRECT rcLimits = GetLastZoomUndo();
//			m_eXMin = rcLimits.left;
//			m_eYMax = rcLimits.top;
//			m_eXMax = rcLimits.right;
//			m_eYMin = rcLimits.bottom;
//
//			//  force a recalculation of all the plots next repaint.
//			Refresh();
//			//  tell the parent that the limits are different.
//			EmitLimitsChanged();
//
//		}
//		else
//		{
//			//  save the current limits to the undo list.
//			FLOATRECT rcLimits;
//			rcLimits.left = m_eXMin;
//			rcLimits.top = m_eYMax;
//			rcLimits.right = m_eXMax;
//			rcLimits.bottom = m_eYMin;
//			AddToZoomUndo(rcLimits);
//
//			//  if the x-axis is locked, do nothing.
//			if (m_oIsXLocked==false)
//			{
//				//  determine the ratio of the ranges.
//				//  assume the inside dimensions are correct.
//				float eXDataRange = m_eXMax - m_eXMin + 1.0f;
//				float eXPixelRange = (float)(m_rcPlotArea.Width());
//				float eXRatio = eXDataRange / eXPixelRange;
//
//				//  get distance of left edge of rect from inside left edge.
//				float eXNewMin = m_eXMin + (m_rcZoom.left - m_rcPlotArea.left) * eXRatio;
//				//  make sure the new min is not less than the current min.
//				eXNewMin = (eXNewMin<m_eXMin) ? (m_eXMin) : (eXNewMin);
//
//				//  get distance of right edge of rect from inside left edge.
//				float eXNewMax = m_eXMin + (m_rcZoom.right - m_rcPlotArea.left) * eXRatio;
//				//  make sure the new max is not greater than the current min.
//				eXNewMax = (eXNewMax>m_eXMax) ? (m_eXMax) : (eXNewMax);
//
//				//  assign to variables
//				m_eXMin = eXNewMin;
//				m_eXMax = eXNewMax;
//			}
//
//			//  if the y-axis is locked, do nothing.
//			if (m_oIsYLocked==false)
//			{
//				//  determine the ratio of the ranges.
//				//  assume the inside dimensions are correct.
//				float eYDataRange = m_eYMax - m_eYMin + 1.0f;
//				float eYPixelRange = (float)(m_rcPlotArea.Height());
//				float eYRatio = eYDataRange / eYPixelRange;
//
//				//  get distance of bottom edge of rect from inside bottom edge.
//				float eYNewMin = m_eYMin + (m_rcPlotArea.bottom - m_rcZoom.bottom) * eYRatio;
//				//  make sure the new min is not less than the current min.
//				eYNewMin = (eYNewMin<m_eYMin) ? (m_eYMin) : (eYNewMin);
//
//				//  get distance of right edge of rect from inside left edge.
//				float eYNewMax = m_eYMin + (m_rcPlotArea.bottom - m_rcZoom.top) * eYRatio;
//				//  make sure the new max is not greater than the current min.
//				eYNewMax = (eYNewMax>m_eYMax) ? (m_eYMax) : (eYNewMax);
//
//				//  assign to variables
//				m_eYMin = eYNewMin;
//				m_eYMax = eYNewMax;
//			}
//
//			//  force a recalculation all the plots next time.
//			Refresh();
//			//  tell the parent that the limits are different.
//			EmitLimitsChanged();
//		}
//	}
//
//	//  not drawing a zoom rect now.
//	m_oIsMouseDown = false;
//	m_oHasPassedOverPlot = false;
//
//	//  is the mouse over the x min?
//	if ((m_uiMouseOver & PLOT_OVER_X_MIN) && m_oIsXLocked!=true)
//	{
//		szFormat.Format("%%0.%uf", m_uiXPrecision);
//		szData.Format(szFormat, m_eXMin);
//		m_edtDataEntry.SetWindowText(szData);
//		m_edtDataEntry.MoveWindow(m_rcXMin);
//		m_edtDataEntry.ShowWindow(TRUE);
//		m_edtDataEntry.SetFocus();
//	}
//
//	//  is the mouse over the x max?
//	else if ((m_uiMouseOver & PLOT_OVER_X_MAX) && m_oIsXLocked!=true)
//	{
//		szFormat.Format("%%0.%uf", m_uiXPrecision);
//		szData.Format(szFormat, m_eXMax);
//		m_edtDataEntry.SetWindowText(szData);
//		m_edtDataEntry.MoveWindow(m_rcXMax);
//		m_edtDataEntry.ShowWindow(TRUE);
//		m_edtDataEntry.SetFocus();
//	}
//
//	//  is the mouse over the y min?
//	else if ((m_uiMouseOver & PLOT_OVER_Y_MIN) && m_oIsYLocked!=true)
//	{
//		szFormat.Format("%%0.%uf", m_uiYPrecision);
//		szData.Format(szFormat, m_eYMin);
//		m_edtDataEntry.SetWindowText(szData);
//		m_edtDataEntry.MoveWindow(m_rcYMin);
//		m_edtDataEntry.ShowWindow(TRUE);
//		m_edtDataEntry.SetFocus();
//	}
//
//	//  is the mouse over the y max?
//	else if ((m_uiMouseOver & PLOT_OVER_Y_MAX) && m_oIsYLocked!=true)
//	{
//		szFormat.Format("%%0.%uf", m_uiYPrecision);
//		szData.Format(szFormat, m_eYMax);
//		m_edtDataEntry.SetWindowText(szData);
//		m_edtDataEntry.MoveWindow(m_rcYMax);
//		m_edtDataEntry.ShowWindow(TRUE);
//		m_edtDataEntry.SetFocus();
//	}
//
//	//  is the mouse over the x-lock button?
//	else if (m_uiMouseOver&PLOT_OVER_X_LOCK)
//	{
//		m_oIsXLocked = !m_oIsXLocked;
//		if (m_hWnd!=NULL)
//			Invalidate();
//	}
//	//  is the mouse over the y-lock button?
//	else if (m_uiMouseOver&PLOT_OVER_Y_LOCK)
//	{
//		m_oIsYLocked = !m_oIsYLocked;
//		if (m_hWnd!=NULL)
//			Invalidate();
//	}
//
//	else
//	{
//		//  if there is an edit window visible, hide it
//		HideEditWindow();
//		if (m_hWnd!=NULL)
//			Invalidate();
//	}
//
//	CWnd::OnLButtonUp(nFlags, point);
//}
///////////////////////////////////////////////////////////////////////////////

void CLinePlot::EmitSelectionChanged()
{
	//CWnd *pwndParent = GetParent();
	//NMHDR msgNotif;
	//msgNotif.code = NM_PLOT_SEL_CHANGE;
	//msgNotif.hwndFrom = this->GetSafeHwnd();
	//msgNotif.idFrom = this->GetDlgCtrlID();
	//pwndParent->SendMessage(WM_NOTIFY, 0, (UINT)&msgNotif);
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::EmitLimitsChanged()
{
	CWnd *pwndParent = GetParent();
	NMHDR msgNotif;
	msgNotif.code = NM_PLOT_LIMITS_CHANGE;
	msgNotif.hwndFrom = this->GetSafeHwnd();
	msgNotif.idFrom = this->GetDlgCtrlID();
	pwndParent->SendMessage(WM_NOTIFY, 0, (UINT)&msgNotif);
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::EmitMouseMove()
{
	CWnd *pwndParent = GetParent();
	NMHDR msgNotif;
	msgNotif.code = NM_PLOT_MOUSE_MOVE;
	msgNotif.hwndFrom = this->GetSafeHwnd();
	msgNotif.idFrom = this->GetDlgCtrlID();
	pwndParent->SendMessage(WM_NOTIFY, 0, (UINT)&msgNotif);
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::AddToZoomUndo(FLOATRECT &rcLimits)
{
	//  is the undo list full?
	if (m_nUndoIndex==LINE_PLOT_UNDO_SIZE-1)
	{
		//  move everything back one...
		for (UINT ii = 0; ii<LINE_PLOT_UNDO_SIZE-1; ii++)
		{
			m_prcUndoLimits[ii] = m_prcUndoLimits[ii+1];
		}
		//  subtract one from the index so it is now correct.
		m_nUndoIndex--;
	}
	//  increment the index to store the new undo.
	m_nUndoIndex++;
	m_prcUndoLimits[m_nUndoIndex] = rcLimits;
}
/////////////////////////////////////////////////////////////////////////////

FLOATRECT CLinePlot::GetLastZoomUndo()
{
	int nCurrIndex = m_nUndoIndex;

	//  are we on the first one?
	m_nUndoIndex = (m_nUndoIndex>0) ? 
		(m_nUndoIndex-1) : (m_nUndoIndex);

	return m_prcUndoLimits[nCurrIndex];
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::HideEditWindow()
{
	//  is the edit window visible?
	if (m_edtDataEntry.IsWindowVisible()==TRUE)
	{
		//  hide the edit window.
		m_edtDataEntry.ShowWindow(FALSE);

		//  get the value from the window.
		CString szText;
		m_edtDataEntry.GetWindowText(szText);
		float eValue  = (float)(atof(szText.GetBuffer(1)));

		//  where is its center?
		CRect rcWindow;
		m_edtDataEntry.GetWindowRect(&rcWindow);
		ScreenToClient(&rcWindow);
		CPoint ptCenter = rcWindow.CenterPoint();

		//  based on the center, update the correct min or max.
		if (m_rcXMin.PtInRect(ptCenter)==TRUE)
		{
			m_eXMin = eValue;
			EmitLimitsChanged();
		}
		else if (m_rcXMax.PtInRect(ptCenter)==TRUE)
		{
			m_eXMax = eValue;
			EmitLimitsChanged();
		}
		else if (m_rcYMin.PtInRect(ptCenter)==TRUE)
		{
			m_eYMin = eValue;
			EmitLimitsChanged();
		}
		else if (m_rcYMax.PtInRect(ptCenter)==TRUE)
		{
			m_eYMax = eValue;
			EmitLimitsChanged();
		}

		Refresh();
	}
}
/////////////////////////////////////////////////////////////////////////////

CString CLinePlot::GetName(int nIndex)
{
	return m_lstPlotData[nIndex].m_szName;
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetName(int nIndex, CString szName)
{
	m_lstPlotData[nIndex].m_szName = szName;
	if (m_hWnd!=NULL)
		Invalidate();
	return true;
}
/////////////////////////////////////////////////////////////////////////////

enumPlotStyle CLinePlot::GetStyle(int nIndex)
{
	return (enumPlotStyle)(m_lstPlotData[nIndex].m_nStyle);
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetStyle(int nIndex, enumPlotStyle nStyle)
{
	m_lstPlotData[nIndex].m_nStyle = nStyle;
	if (m_hWnd!=NULL)
		Invalidate();
	return true;
}
/////////////////////////////////////////////////////////////////////////////

COLORREF CLinePlot::GetColor(int nIndex)
{
	return m_lstPlotData[nIndex].m_crColor;
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetColor(int nIndex, COLORREF crColor)
{
	m_lstPlotData[nIndex].m_crColor = crColor;
	if (m_hWnd!=NULL)
		Invalidate();
	return true;
}
/////////////////////////////////////////////////////////////////////////////

FLOATPOINT *CLinePlot::GetData(int nIndex)
{
	return m_lstPlotData[nIndex].m_PointData;
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetData(int nIndex, FLOATPOINT *pptData)
{
	m_lstPlotData[nIndex].UpdateData(pptData);
	if (m_hWnd!=NULL)
		Invalidate();
	return true;
}
/////////////////////////////////////////////////////////////////////////////

UINT CLinePlot::GetPointCount(int nIndex)
{
	return m_lstPlotData[nIndex].m_uiPointCount;
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetXMin(float &eXMin) 
{ 
	if (m_oIsXLocked==true)
		return false;
	else
	{
		if (m_eXMin!=eXMin)
		{
			m_eXMin = eXMin;
			EmitLimitsChanged();
			if (m_hWnd!=NULL)
				Invalidate();
		}
		return true;
	}
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetXMax(float &eXMax) 
{ 
	if (m_oIsXLocked==true)
		return false;
	else
	{
		if (m_eXMax!=eXMax)
		{
			m_eXMax = eXMax;
			EmitLimitsChanged();
			if (m_hWnd!=NULL)
				Invalidate();
		}
		return true;
	}
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetYMin(float &eYMin) 
{ 
	if (m_oIsYLocked==true)
		return false;
	else
	{
		if (m_eYMin!=eYMin)
		{
			m_eYMin = eYMin;
			EmitLimitsChanged();
			if (m_hWnd!=NULL)
				Invalidate();
		}
		return true;
	}
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetYMax(float &eYMax) 
{ 
	if (m_oIsYLocked==true)
		return false;
	else
	{
		if (m_eYMax!=eYMax)
		{
			m_eYMax = eYMax;
			EmitLimitsChanged();
			if (m_hWnd!=NULL)
				Invalidate();
		}
		return true;
	}
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetIsXLocked(bool &oLock)
{ 
	if (m_oIsXLocked!=oLock)
	{
		m_oIsXLocked = oLock; 
		if (m_hWnd!=NULL)
			Invalidate();
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////

bool CLinePlot::SetIsYLocked(bool &oLock)
{
	if (m_oIsYLocked!=oLock)
	{
		m_oIsYLocked = oLock; 
		if (m_hWnd!=NULL)
			Invalidate();
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlot::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	m_SelectedDataItemNo = -1;
	double screen_distance = 99999;
	int ii_selected = -1;

	for (int ii=0; ii<m_lstPlotData.size(); ii++)
	{
	if (m_lstPlotData[ii].m_nStyle == enum_LpScatter && ii == m_nSelected)
		{
			for(int p = 0; p< m_lstPlotData[ii].m_uiPointCount; p++)
			{
			int hour  = m_lstPlotData[ii].m_PointData[p].Hour;
				if( hour == -1 || hour >=  m_StartHour ||  hour <= m_StartHour  + m_AggregationWindow)
				{

				int x = m_lstPlotData[ii].m_pptScreen[p].x;
				int y = m_lstPlotData[ii].m_pptScreen[p].y;
				
				double dis = (x - point.x)*(x - point.x) + (y - point.y)*(y - point.y);
				double distance  = sqrt(dis);

				if( distance < screen_distance && distance < 50)
				{
					screen_distance = distance;
					m_SelectedDataItemNo = p;
					ii_selected = ii;
				}
				}
			}

	  }
	}
	if (screen_distance < 50)
	{

		int SelDataItemNo = m_SelectedDataItemNo;

		UpdateData(1);

		if (SelDataItemNo >= 0)
		{
			int LinkNo = m_lstPlotData[ii_selected].m_PointData[m_SelectedDataItemNo].LinkNo;
			if (LinkNo >= 0)
			{

				std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin();
				while (iDoc != g_DocumentList.end())
				{
					(*iDoc)->m_SelectedLinkNo = LinkNo;
					(*iDoc)->ZoomToSelectedLink((*iDoc)->m_SelectedLinkNo);


					iDoc++;
				}





			}

			Invalidate();
		}
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}
