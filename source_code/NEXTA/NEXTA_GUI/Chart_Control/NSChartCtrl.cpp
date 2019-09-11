// NSChartCtrl.cpp : implementation file
//
// NSChartCtrl is a MFC chart control. 
//
// Written by Ivan Cachicatari (ivancp@viabcp.com)
// Copyright (c) 2004.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
// Updated 02 Aug 2004
//         
// Version History
// ----------------
//	1.0.1	25 Jun 2004
//			- Initial release.
//
//	1.1.0	02 Aug 2004
//			- Fix some painting problems.
//			- Added HitTest().
//			- Added ModifyItem().
//			- Added DeleteItem().
//			- Added the double buffering paint
//			- Added Notify functions and _NMCHARTCTRL structure



#include "stdafx.h"
#include "NSChartCtrl.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNSChartCtrl

CNSChartCtrl::CNSChartCtrl()
{
	m_bShowPercentage = false;
	m_bIntegerValue = false;
	RegisterWndClass(AfxGetInstanceHandle());
	m_txtFont.CreateStockObject(DEFAULT_GUI_FONT);
	m_boldFont.CreateStockObject(DEFAULT_GUI_FONT);
	
	LOGFONT lf;
	if(m_txtFont.GetLogFont(&lf)){
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 15;
		strcpy(lf.lfFaceName,"Arial");
		m_titleFont.DeleteObject();
		m_titleFont.CreateFontIndirect(&lf);
	}

	if(m_boldFont.GetLogFont(&lf)){
		lf.lfWeight = FW_BOLD;
		m_boldFont.DeleteObject();
		m_boldFont.CreateFontIndirect(&lf);
	}

	m_dTotal = 0.0L;
	m_dMax = 0.0l;
	m_dwStyle = NSCS_PIE;

	m_iCurSel = -1;
}

CNSChartCtrl::~CNSChartCtrl()
{
	m_txtFont.DeleteObject();
	m_boldFont.DeleteObject();

	ResetChart();
	ResetColors();	

	if(m_bmpScreen.m_hObject != NULL) {
		m_bmpScreen.DeleteObject();
		m_bmpScreen.m_hObject = NULL;
	}
}


BEGIN_MESSAGE_MAP(CNSChartCtrl, CWnd)
	//{{AFX_MSG_MAP(CNSChartCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNSChartCtrl message handlers

BOOL CNSChartCtrl::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.lpszClassName = "MFC_NSChartCtrl"; 
	wc.hInstance = hInstance;
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = 0;
	wc.lpszMenuName = NULL;
	wc.hbrBackground = (HBRUSH) ::GetStockObject(WHITE_BRUSH);
	wc.style = CS_GLOBALCLASS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	return (::RegisterClass(&wc) != 0);
}

void CNSChartCtrl::OnPaint() 
{
	CPaintDC dc(this); 

	int iValues	= m_ChartValues.GetSize();
	int iColors		= m_BrushArray.GetSize();

	CString str;
	CRect rect;
	GetClientRect(&rect);

	// Check values count
	if(iValues <= 0 )
	{
		CFont* oldFont = dc.SelectObject(&m_txtFont);

		str = "No data are available...";
		dc.FillSolidRect(rect,RGB(255,255,255));
		rect.top += 50;
		dc.DrawText(str,&rect,DT_CENTER|DT_VCENTER);
		dc.SelectObject(oldFont);
		return;
	}
	// Check color count
	if(iColors <= 1)
	{
		CFont* oldFont = dc.SelectObject(&m_txtFont);
		str = "Color count mus be > 1";
		dc.FillSolidRect(rect,RGB(255,255,255));
		rect.top += 50;
		dc.DrawText(str,&rect,DT_CENTER|DT_VCENTER);
		dc.SelectObject(oldFont);
		return;
	}

	//Creating double buffer painting

	CDC imageDC;
	imageDC.CreateCompatibleDC(&dc);

	CBitmap* pOldMemDCBitmap = NULL;

	if(m_bmpScreen.m_hObject == NULL) 
		m_bmpScreen.CreateCompatibleBitmap( &dc, rect.Width(), rect.Height() );
	pOldMemDCBitmap = (CBitmap*)imageDC.SelectObject(&m_bmpScreen);
	
	imageDC.FillSolidRect(rect,RGB(255,255,255));

//	rect.DeflateRect(0,5);

	// Drawing the chart
	if(m_dwStyle == NSCS_BAR ){
		DrawBarChart(&imageDC);
	}
	if(m_dwStyle == NSCS_PIE ){
		DrawPieChart(&imageDC);
	}
	if(m_dwStyle == NSCS_LINE ){
		DrawLineChart(&imageDC);
	}
	// Drawing the Title

	if(m_Caption.GetLength() == 0 )
		GetWindowText(str);
	else
	{
		str = m_Caption;
	}



	CFont* oldFont = imageDC.SelectObject(&m_titleFont);
	imageDC.DrawText(str,rect,DT_CENTER);
	imageDC.SelectObject(oldFont);

	// Drawing the new bitmap

	dc.BitBlt( rect.left , rect.top , rect.Width(), rect.Height(), 
					&imageDC, 0, 0, SRCCOPY );

	imageDC.SelectObject(pOldMemDCBitmap);

}

void CNSChartCtrl::DrawPieChart(CDC *pDC)
{
	ASSERT(m_dTotal > 0.0L);

	double pi	= 3.1415926535*2;
	int iValues	= m_ChartValues.GetSize();
	int iColors	= m_BrushArray.GetSize();

	CNSChartValue* pTmp;
	CString str;
	CRect  rect;
	GetClientRect(&rect);

	double dAngle = 0;
	double dMidAngle = 0;
	double r = min((rect.Width()/2)-50,(rect.Height()/2)-40);
	CPoint p1 = rect.CenterPoint();
	p1.y += 15;
	CPoint p2(p1.x + (int)r ,p1.y);
	CPoint p3;
	CPoint p4;

	pDC->Ellipse(p1.x-(int)r,p1.y-(int)r,p1.x+(int)r,p2.y+(int)r);
	pDC->MoveTo(p1);
	pDC->LineTo(p2);
	CFont *oldFont = pDC->SelectObject(&m_txtFont);

	for (int i = 0; i < iValues; i++ )
	{
		pTmp	= m_ChartValues.GetAt(i);
		dMidAngle = pTmp->m_dValue * pi / m_dTotal;
		
		dAngle += dMidAngle;
		p2.x = (int)(cos(dAngle)*r + p1.x);
		p2.y = (int)(p1.y - sin(dAngle)*r );

		pDC->MoveTo(p1);
		pDC->LineTo(p2);

		dMidAngle = dAngle - dMidAngle/2;
		p3.x = (int)(cos(dMidAngle)*r + p1.x);
		p3.y = (int)(p1.y - sin(dMidAngle)*r );

		p4.x = (int)(cos(dMidAngle)*(r*1.2) + p1.x);
		p4.y = (int)(p1.y - sin(dMidAngle)*(r*1.2) );

		pDC->MoveTo(p3);
		pDC->LineTo(p4);
		pDC->MoveTo(p4);
		p4.x += 40*((p4.x > p1.x)?(1):(-1));
		pDC->LineTo(p4);

		rect.left = p4.x - 60;
		rect.right = p4.x + 60;
		rect.top = p4.y - 15;
		rect.bottom = p4.y + 15;

		str.Format(_T("%s\n%.1f%%"),pTmp->m_sLabel,(pTmp->m_dValue*100.0)/m_dTotal);
		if(m_iCurSel == i)
		{
			CFont *tmpFont = pDC->SelectObject(&m_boldFont);
			rect.InflateRect(2,0);
			pDC->DrawText(str,&rect,DT_CENTER|DT_VCENTER);
			pDC->SelectObject(tmpFont);
		}else{
			pDC->DrawText(str,&rect,DT_CENTER|DT_VCENTER);
		} 

		p4.x = (int)(cos(dMidAngle)*(r*0.8) + p1.x);
		p4.y = (int)(p1.y - sin(dMidAngle)*(r*0.8) );

		if(pDC->GetPixel(p4) > 0x0)
		{
			CBrush* tmpBr = pDC->SelectObject((CBrush*)m_BrushArray.GetAt(i%iColors));
			pDC->FloodFill(p4.x,p4.y,0x0);
			pDC->SelectObject(tmpBr);
		}
	}

	pDC->SelectObject(oldFont);
}
void CNSChartCtrl::DrawBarChart(CDC *pDC)
{
	if(m_dTotal < 0.000001)
	{
	AfxMessageBox("error in drawing bar chart.");
	return;
	}

	int iValues	= m_ChartValues.GetSize();
	int iColors	= m_BrushArray.GetSize();
	CString str;

	CRect rect,rcBar;
	GetClientRect(&rect);

	CNSChartValue* pTmp;

    double t = 1.0L;
	if(m_dMax >= 1.0L)
	{
		str.Format("%.0f",m_dMax);
		int d = 1;
		for(int i = 0;i<str.GetLength()-1;i++) d*=10;
		t = max((((double)str.GetAt(0)-48)+1)*d,m_dMax);
	}else
	{
	t = m_dMax;
	}
	
	rect.DeflateRect(40,30,20,20);
	
	rcBar.left   =  5; 
	rcBar.right  = rcBar.left + 30; 
	rcBar.top    = 25;
	rcBar.bottom = rcBar.top + 15;
	int i = rect.Height();

	CFont *oldFont = pDC->SelectObject(&m_txtFont);

	pDC->MoveTo(rect.left - 7,rect.top);
	pDC->LineTo(rect.left + 7,rect.top);
	
	str.Format((t<1.0)?("%.1f"):("%.0f"),t);
	pDC->DrawText(str,rcBar,DT_RIGHT);
	
	pDC->MoveTo(rect.left - 5,rect.top + i/4);
	pDC->LineTo(rect.left + 5,rect.top + i/4);

	pDC->MoveTo(rect.left - 7,rect.top + i/2);
	pDC->LineTo(rect.left + 7,rect.top + i/2);
	
	str.Format((t<1.0)?("%.1f"):("%.0f"),t/2);
	rcBar.top    = rect.top + (int)(i/2);
	rcBar.bottom = rcBar.top + 15;
	pDC->DrawText(str,rcBar,DT_RIGHT);

	pDC->MoveTo(rect.left - 5,rect.top + i*3/4);
	pDC->LineTo(rect.left + 5,rect.top + i*3/4);

	rcBar = rect;
	rcBar.DeflateRect(0,10,0,0);

	pDC->MoveTo(rect.left  -15, rect.bottom);
	pDC->LineTo(rect.right +15, rect.bottom);

	pDC->MoveTo(rect.left , rect.top    -15);
	pDC->LineTo(rect.left , rect.bottom +15);

	int scale = rcBar.Width()/iValues;
	int height = rcBar.Height();
	CRect tmpRect;
	
	for ( i = 0; i < iValues; i++ )
	{
		pTmp = m_ChartValues.GetAt(i);
		rcBar.left  = rect.left + scale*i + 2;
		rcBar.right = rcBar.left + max(1,scale - 4);
		rcBar.top = (rcBar.bottom - (long)(pTmp->m_dValue*height/t));
		rcBar.bottom = rect.bottom+1;

		//Drawing bar's
		CBrush* tmpBr = pDC->SelectObject((CBrush*)m_BrushArray.GetAt(i%iColors));
		pDC->Rectangle(rcBar);
		pDC->SelectObject(tmpBr);

		//Drawing top text percents
		tmpRect = rcBar;
		tmpRect.bottom = tmpRect.top - 1;
		tmpRect.top = tmpRect.bottom - 15;

		if(m_bShowPercentage)
			str.Format("%.1f%%",100*pTmp->m_dValue/m_dTotal);
		else
		{
			if(m_bIntegerValue)
				str.Format("%5d",(int)(pTmp->m_dValue));
			else
				str.Format("%5.2f",pTmp->m_dValue);
		}

		
		if(m_iCurSel == i)
		{
			CFont *tmpFont = pDC->SelectObject(&m_boldFont);			
			pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER);
			pDC->SelectObject(tmpFont);
		}else{
			pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER);
		} 

/*
		//Drawing bar text values
		tmpRect.top    = rcBar.top + 2;
		tmpRect.bottom = tmpRect.top + 15;
		str.Format((t<1.0)?("%.1f"):("%.0f"),pTmp->m_dValue);
		pDC->Rectangle(tmpRect);
		tmpRect.top += 1;
		pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER);
*/

		//Drawing bottom text labels
		tmpRect.top		= rcBar.bottom + 2;
		tmpRect.bottom  = tmpRect.top + 15;
		pDC->DrawText(pTmp->m_sLabel,tmpRect,DT_CENTER);
	}

	pDC->SelectObject(oldFont);
}

void CNSChartCtrl::DrawLineChart(CDC *pDC)
{
	ASSERT(m_dTotal > 0.0L);

	int iValues	= m_ChartValues.GetSize();
	int iColors	= m_BrushArray.GetSize();
	CString str;

	CRect rect,rcBar;
	GetClientRect(&rect);

	CNSChartValue* pTmp;

    double t = 1.0L;
	if(m_dMax >= 1.0L){
		str.Format("%.0f",m_dMax);
		int d = 1;
		for(int i = 0;i<str.GetLength()-1;i++) d*=10;
		t = max((((double)str.GetAt(0)-48)+1)*d,m_dMax);
	}
	
	rect.DeflateRect(40,30,20,20);
	
	rcBar.left   =  5; 
	rcBar.right  = rcBar.left + 30; 
	rcBar.top    = 25;
	rcBar.bottom = rcBar.top + 15;
	int i = rect.Height();

	CFont *oldFont = pDC->SelectObject(&m_txtFont);

	pDC->MoveTo(rect.left - 7,rect.top);
	pDC->LineTo(rect.left + 7,rect.top);
	
	str.Format((t<1.0)?("%.1f"):("%.0f"),t);
	pDC->DrawText(str,rcBar,DT_RIGHT);
	
	pDC->MoveTo(rect.left - 5,rect.top + i/4);
	pDC->LineTo(rect.left + 5,rect.top + i/4);

	pDC->MoveTo(rect.left - 7,rect.top + i/2);
	pDC->LineTo(rect.left + 7,rect.top + i/2);
	
	str.Format((t<1.0)?("%.1f"):("%.0f"),t/2);
	rcBar.top    = rect.top + (int)(i/2);
	rcBar.bottom = rcBar.top + 15;
	pDC->DrawText(str,rcBar,DT_RIGHT);

	pDC->MoveTo(rect.left - 5,rect.top + i*3/4);
	pDC->LineTo(rect.left + 5,rect.top + i*3/4);

	rcBar = rect;
	rcBar.DeflateRect(0,10,0,0);

	pDC->MoveTo(rect.left  -15, rect.bottom);
	pDC->LineTo(rect.right +15, rect.bottom);

	pDC->MoveTo(rect.left , rect.top    -15);
	pDC->LineTo(rect.left , rect.bottom +15);

	int scale = rcBar.Width()/iValues;
	int height = rcBar.Height();
	CRect tmpRect;

	pDC->MoveTo(rect.left, rect.bottom);  // start from origin of chart

	int ValueStep = max(1,iValues/4);
	for ( i = 0; i < iValues; i++ )
	{
		pTmp = m_ChartValues.GetAt(i);
		rcBar.left  = rect.left + scale*i + 2;
		rcBar.right = rcBar.left + max(1,scale - 4);
		rcBar.top = (rcBar.bottom - (long)(pTmp->m_dValue*height/t));
		rcBar.bottom = rect.bottom+1;

		pDC->LineTo(rcBar.right, rcBar.top);

		if(i%ValueStep ==0)    ////Drawing top text
		{
			//Drawing top text percents
			tmpRect = rcBar;
			tmpRect.bottom = tmpRect.top - 1;
			tmpRect.top = tmpRect.bottom - 15;

			if(m_bShowPercentage)
				str.Format("%.1f%%",100*pTmp->m_dValue/m_dTotal);
			else
			{
				if(m_bIntegerValue)
					str.Format("%5d",(int)(pTmp->m_dValue));
				else
					str.Format("%.1f",pTmp->m_dValue);
			}
			if(m_iCurSel == i)
			{
				CFont *tmpFont = pDC->SelectObject(&m_boldFont);			
				pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER|DT_NOCLIP);
				pDC->SelectObject(tmpFont);
			}else{
				pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER|DT_NOCLIP);
			} 

			//Drawing bottom text labels
			tmpRect.top		= rcBar.bottom + 2;
			tmpRect.bottom  = tmpRect.top + 15;
			pDC->DrawText(pTmp->m_sLabel,tmpRect,DT_CENTER|DT_NOCLIP);

		}
/*
		//Drawing bar text values
		tmpRect.top    = rcBar.top + 2;
		tmpRect.bottom = tmpRect.top + 15;
		str.Format((t<1.0)?("%.1f"):("%.0f"),pTmp->m_dValue);
		pDC->Rectangle(tmpRect);
		tmpRect.top += 1;
		pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER);
*/

	}

	pDC->SelectObject(oldFont);
}


void CNSChartCtrl::PrepareColors(DefaultColors defaultColor)
{
//			AddSolidBrush(0x0000CC00);
			AddSolidBrush(0x00FF0000);
			AddSolidBrush(0x00CCCC00);
			AddSolidBrush(0x000000FF);
			AddSolidBrush(0x000000CC);
			AddSolidBrush(0x0000FF00);
			AddSolidBrush(0x00FF00FF);
}

BOOL CNSChartCtrl::AddSolidBrush(COLORREF cr)
{
	CBrush *br = NULL;
	try 
	{
		br = new CBrush(cr);
		m_BrushArray.Add(br);		
		return TRUE;
	}
	catch (CMemoryException* e)
	{
		if (br != NULL) 
		{
			delete br;
		}
		e->Delete();
		return FALSE;
	}
}


void CNSChartCtrl::SetChartStyle(NSChart_STYLES dStyle)
{
	m_dwStyle = dStyle;	
	Invalidate(TRUE);
}

void CNSChartCtrl::ResetColors()
{
	int iSize = m_BrushArray.GetSize();
	for (int i = 0; i < iSize; i++ )
	{
		delete m_BrushArray.GetAt(i);
	}
	m_BrushArray.RemoveAll();	
}

BOOL CNSChartCtrl::AddBrush(CBrush *br)
{
	ASSERT(br->m_hObject);

	try 
	{
		m_BrushArray.Add(br);
		return TRUE;
	}
	catch (CMemoryException* e)
	{
		if (br != NULL) 
		{
			delete br;
		}
		e->Delete();
		return FALSE;
	}
}

BOOL CNSChartCtrl::AddValue(DOUBLE dValue, LPCTSTR strLabel,BOOL bRepaint /*= FALSE*/)
{
	if(dValue <= 0)
	{
		TRACE("The double value in function AddValue must be > 0 (dValue = %lf)",dValue);
		return FALSE;
	}

	CNSChartValue* newValue = new CNSChartValue;
	newValue->m_dValue  = dValue;
	newValue->m_sLabel  = strLabel;

	try 
	{
		m_ChartValues.Add(newValue);
		m_dTotal += dValue;
		if(m_dMax < dValue)
		{
			m_dMax = dValue;
		}
		
		if(bRepaint)
		{
			Invalidate();
		}

		m_iCurSel = -1;
		return TRUE;
	}
	catch (CMemoryException* e)
	{
		if (newValue !=NULL) 
		{
			delete newValue;
		}
		e->Delete();
		return FALSE;
	}
}

void CNSChartCtrl::ResetChart()
{
	int iSize = m_ChartValues.GetSize();
	
	for (int i = 0; i < iSize; i++ )
	{
		delete m_ChartValues.GetAt(i);
	}
	m_ChartValues.RemoveAll();

		m_dTotal = 0;
		m_dMax = 0;
}

void CNSChartCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	static int ilastSel = -2;
	
	m_iCurSel = HitTest(point);

	Invalidate(FALSE);

	Notify(NSCS_SELECTEDITEM,m_iCurSel);
}

int CNSChartCtrl::HitTest(CPoint &pt)
{
	if(m_dTotal <= 0.0)
	{
		return -1;
	}


	if(m_dwStyle == NSCS_BAR )
	{

		int iValues	= m_ChartValues.GetSize();

		CRect rect,rcBar;
		CString str;
		GetClientRect(&rect);

		double t = 1.0L;
		if(m_dMax >= 1.0L){
			str.Format("%.0f",m_dMax);
			int d = 1;
			for(int i = 0;i<str.GetLength()-1;i++) d*=10;
			t = max((((double)str.GetAt(0)-48)+1)*d,m_dMax);
		}
		
		rect.DeflateRect(40,30,20,20);
		rcBar = rect;
		rcBar.bottom = rect.bottom+1;

		int scale = rcBar.Width()/iValues;
		int height = rcBar.Height();
	
		CNSChartValue* pTmp;

		for (int i = 0; i < iValues; i++ )
		{
			pTmp = m_ChartValues.GetAt(i);
			rcBar.left  = rect.left + scale*i + 2;
			rcBar.right = rcBar.left + scale - 4;
			rcBar.top = (rcBar.bottom - (long)(pTmp->m_dValue*height/t));
			if(rcBar.PtInRect(pt))
			{
				return i;
			}
		}

	}
	if(m_dwStyle == NSCS_PIE )
	{
		CRect  rect;
		GetClientRect(&rect);
		double pi	= 3.1415926535;
		double r = min((rect.Width()/2)-50,(rect.Height()/2)-40);
		CPoint p1 = rect.CenterPoint();
		p1.y += 15;

		double x = (pt.x - p1.x);
		double y = (pt.y - p1.y);
		double h = sqrt(x*x + y*y);

		if(h > 0.0 && h < r)
		{
			double dClickAngle = -asin(y/h);
			
			if(p1.x > pt.x) dClickAngle = pi - dClickAngle;
			if(dClickAngle < 0.0) dClickAngle = pi*2 + dClickAngle;
			
			int iValues	= m_ChartValues.GetSize();
			int iColors	= m_BrushArray.GetSize();

			CNSChartValue* pTmp;
			double dCurrAngle = 0;
			double dLastAngle = 0;

			for (int i = 0; i < iValues; i++ )
			{
				pTmp	= m_ChartValues.GetAt(i);
				dCurrAngle = dLastAngle + pTmp->m_dValue * 2*pi / m_dTotal;
				
				if(dClickAngle >= dLastAngle &&  dClickAngle <= dCurrAngle )
				{
					return i;
				}
				dLastAngle = dCurrAngle;
			}
		}
	}
	return -1;
}


BOOL CNSChartCtrl::Notify(UINT nCode, INT iItem)
{
	NMCHARTCTRL	nm;
	nm.hdr.hwndFrom = GetSafeHwnd();
	nm.hdr.idFrom = GetDlgCtrlID();
	nm.hdr.code = nCode;

	nm.iItem = iItem;
	
	if(iItem >= 0 && iItem < m_ChartValues.GetSize())
	{
		CNSChartValue* tmp = m_ChartValues.GetAt(iItem);
		nm.dValue = tmp->m_dValue;
		_tcsncpy(nm.sLabel,tmp->m_sLabel,MAXCHARLABEL+1);
	}else
	{
		_tcsncpy(nm.sLabel,"",MAXCHARLABEL+1);
	}
	
	return Notify(&nm);
}

BOOL CNSChartCtrl::Notify(LPNMCHARTCTRL lpnm)
{
	CWnd* pWnd = GetParent();
	if(pWnd)
	{
		return pWnd->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)lpnm);
	}
	return FALSE;
}

bool CNSChartCtrl::ModifyItem(INT iItem, DOUBLE dValue,CString &str)
{
	if(iItem >= 0 && iItem < m_ChartValues.GetSize())
	{
		CNSChartValue *tmp = m_ChartValues.GetAt(iItem);

		m_dTotal += dValue - tmp->m_dValue;
		tmp->m_dValue = dValue;
		tmp->m_sLabel = str;
		if(m_dMax < dValue)
		{
			m_dMax = dValue;
		}
		Invalidate();
		return true;
	}
	return false;
}

BOOL CNSChartCtrl::OnEraseBkgnd(CDC* pDC) 
{
	
	return TRUE;
}

bool CNSChartCtrl::DeleteItem(INT iItem)
{
	if(iItem >= 0 && iItem < m_ChartValues.GetSize())
	{
		CNSChartValue *rip = m_ChartValues.GetAt(iItem);
		
		m_dTotal = 0;
		m_dMax = 0;
		delete rip;
		m_ChartValues.RemoveAt(iItem);

		CNSChartValue *tmp; 
		int iSize = m_ChartValues.GetSize();
			
		for (int i = 0; i < iSize; i++ )
		{
			tmp = m_ChartValues.GetAt(i);
			if(m_dMax < tmp->m_dValue )
			{
				m_dMax = tmp->m_dValue;
			}
			m_dTotal += tmp->m_dValue;
		}
		m_iCurSel = -1;
		Invalidate();

		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////

CNodeChart::CNodeChart()
{
	m_bShowPercentage = false;
	m_bIntegerValue = false;
	m_txtFont.CreateStockObject(DEFAULT_GUI_FONT);
	m_boldFont.CreateStockObject(DEFAULT_GUI_FONT);
	
	LOGFONT lf;
	if(m_txtFont.GetLogFont(&lf)){
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 15;
		strcpy(lf.lfFaceName,"Arial");
		m_titleFont.DeleteObject();
		m_titleFont.CreateFontIndirect(&lf);
	}

	if(m_boldFont.GetLogFont(&lf)){
		lf.lfWeight = FW_BOLD;
		m_boldFont.DeleteObject();
		m_boldFont.CreateFontIndirect(&lf);
	}

	m_dTotal = 0.0L;
	m_dMax = 0.0l;
	m_dwStyle = NSCS_PIE;

	m_iCurSel = -1;



	m_ColorArray[0] = RGB(190,190,190);
	m_ColorArray[1] = RGB(0,255,0);
	m_ColorArray[2] = RGB(255,250,117);
	m_ColorArray[3] = RGB(255,250,0);
	m_ColorArray[4] = RGB(255,216,0);
	m_ColorArray[5] = RGB(255,153,0);
	m_ColorArray[6] = RGB(255,0,0);
	for(int i = 0; i<MAX_PIE_SIZE-1; i++)
	{
		m_BrushArray[i].CreateSolidBrush(m_ColorArray[i]);
	}
}

CNodeChart::~CNodeChart()
{
	m_txtFont.DeleteObject();
	m_boldFont.DeleteObject();

	ResetChart();

	if(m_bmpScreen.m_hObject != NULL) {
		m_bmpScreen.DeleteObject();
		m_bmpScreen.m_hObject = NULL;
	}
}



void CNodeChart::DrawObject(CDC* pDC) 
{

	int iValues	= m_ChartValues.GetSize();
	int iColors		= MAX_PIE_SIZE;

	CString str;
	CRect rect = m_DrawRect;

	// Check values count
	if(iValues <= 0 )
	{
		CFont* oldFont = pDC->SelectObject(&m_txtFont);

		str = "No data are available...";
		pDC->FillSolidRect(rect,RGB(255,255,255));
		rect.top += 50;
		pDC->DrawText(str,&rect,DT_CENTER|DT_VCENTER);
		pDC->SelectObject(oldFont);
		return;
	}
	// Check color count
	if(iColors <= 1)
	{
		CFont* oldFont = pDC->SelectObject(&m_txtFont);
		str = "Color count mus be > 1";
		pDC->FillSolidRect(rect,RGB(255,255,255));
		rect.top += 50;
		pDC->DrawText(str,&rect,DT_CENTER|DT_VCENTER);
		pDC->SelectObject(oldFont);
		return;
	}

	// Drawing the chart
	if(m_dwStyle == NSCS_BAR ){
		DrawBarChart(pDC);
	}
	if(m_dwStyle == NSCS_PIE ){
		DrawPieChart(pDC);
	}
	if(m_dwStyle == NSCS_LINE ){
		DrawLineChart(pDC);
	}
	// Drawing the Title

	str = m_Caption;



	CFont* oldFont = pDC->SelectObject(&m_titleFont);
	pDC->DrawText(str,rect,DT_CENTER);
	pDC->SelectObject(oldFont);

}



void CNodeChart::DrawPieChart(CDC *pDC)
{
 
    //double xpos = rect.right + margin;
    //double ypos = rect.top + margin;
    //for(int i = 0; i < theSliceCount; i++)
    //{
    //    CPen p(PS_SOLID, 1, RGB(0,0,0));
    //    pDC->SelectObject(&p);
    //    CBrush br;
    //    br.CreateSolidBrush(thePieInfo[i].color);
    //    CRect r(xpos, ypos, xpos + 30, ypos + 25);
    //    pDC->Rectangle(r);
    //    r.DeflateRect(1, 1);
    //    pDC->FillRect(r, &br);
    //    pDC->TextOut(xpos + 35, ypos + 5, CString(thePieInfo[i].desc));
    //    ypos += 35;
    //}

//	ASSERT(m_dTotal > 0.0L);
//
//	double pi	= 3.1415926535*2;
//	int iValues	= m_ChartValues.GetSize();
//	int iColors	= MAX_PIE_SIZE;
//
//	CNSChartValue* pTmp;
//	CString str;
//	CRect rect = m_DrawRect;
//
//	double dAngle = 0;
//	double dMidAngle = 0;
//	double r = 10;
//	CPoint p1 = rect.CenterPoint();
////	p1.y += 15;
//	CPoint p2(p1.x + (int)r ,p1.y);
//	CPoint p3;
//	CPoint p4;
//
//	CBrush BlueBrush(RGB(0, 0, 255));
//	pDC->SelectObject(BlueBrush);
//	pDC->Ellipse(p1.x-(int)r,p1.y-(int)r,p1.x+(int)r,p2.y+(int)r);
//
//	pDC->MoveTo(p1);
//	pDC->LineTo(p2);
//	CFont *oldFont = pDC->SelectObject(&m_txtFont);
//
//	for (int i = 0; i < iValues; i++ )
//	{
//		pTmp	= m_ChartValues.GetAt(i);
//		dMidAngle = pTmp->m_dValue * pi / m_dTotal;
//		
//		dAngle += dMidAngle;
//		p2.x = (int)(cos(dAngle)*r + p1.x);
//		p2.y = (int)(p1.y - sin(dAngle)*r );
//
//		pDC->MoveTo(p1);
//		pDC->LineTo(p2);
//
//		dMidAngle = dAngle - dMidAngle/2;
//		p3.x = (int)(cos(dMidAngle)*r + p1.x);
//		p3.y = (int)(p1.y - sin(dMidAngle)*r );
//
//		p4.x = (int)(cos(dMidAngle)*(r*1.2) + p1.x);
//		p4.y = (int)(p1.y - sin(dMidAngle)*(r*1.2) );
//
//		pDC->MoveTo(p3);
//		pDC->LineTo(p4);
//		pDC->MoveTo(p4);
//		p4.x += 40*((p4.x > p1.x)?(1):(-1));
//		pDC->LineTo(p4);
//
//		rect.left = p4.x - 60;
//		rect.right = p4.x + 60;
//		rect.top = p4.y - 15;
//		rect.bottom = p4.y + 15;
//
//		//str.Format(_T("%s\n%.1f%%"),pTmp->m_sLabel,(pTmp->m_dValue*100.0)/m_dTotal);
//		//if(m_iCurSel == i)
//		//{
//		//	CFont *tmpFont = pDC->SelectObject(&m_boldFont);
//		//	rect.InflateRect(2,0);
//		//	pDC->DrawText(str,&rect,DT_CENTER|DT_VCENTER);
//		//	pDC->SelectObject(tmpFont);
//		//}else{
//		//	pDC->DrawText(str,&rect,DT_CENTER|DT_VCENTER);
//		//} 
//
//		CPoint center = rect.CenterPoint();
//		p4.x = (int)(cos(dMidAngle)*(r*0.8) + center.x);
//		p4.y = (int)(center.y - sin(dMidAngle)*(r*0.8) );
//
//	//	if(pDC->GetPixel(p4) > 0x0)
//		{
//		pDC->FloodFill(p4.x,p4.y,m_ColorArray[i]);
//
//		}
//	}
//
//	pDC->SelectObject(oldFont);
}
void CNodeChart::DrawBarChart(CDC *pDC)
{
	if(m_dTotal < 0.000001)
	{
	AfxMessageBox("error in drawing bar chart.");
	return;
	}

	int iValues	= m_ChartValues.GetSize();
	int iColors	= MAX_PIE_SIZE;
	CString str;

	CRect rect = m_DrawRect;
	CRect reBar;

	CNSChartValue* pTmp;

    double t = 1.0L;
	if(m_dMax >= 1.0L)
	{
		str.Format("%.0f",m_dMax);
		int d = 1;
		for(int i = 0;i<str.GetLength()-1;i++) d*=10;
		t = max((((double)str.GetAt(0)-48)+1)*d,m_dMax);
	}else
	{
	t = m_dMax;
	}
	
	rect.DeflateRect(40,30,20,20);
	
	CRect rcBar;
	rcBar.left   =  5; 
	rcBar.right  = rcBar.left + 30; 
	rcBar.top    = 25;
	rcBar.bottom = rcBar.top + 15;
	int i = rect.Height();

	CFont *oldFont = pDC->SelectObject(&m_txtFont);

	pDC->MoveTo(rect.left - 7,rect.top);
	pDC->LineTo(rect.left + 7,rect.top);
	
	str.Format((t<1.0)?("%.1f"):("%.0f"),t);
	pDC->DrawText(str,rcBar,DT_RIGHT);
	
	pDC->MoveTo(rect.left - 5,rect.top + i/4);
	pDC->LineTo(rect.left + 5,rect.top + i/4);

	pDC->MoveTo(rect.left - 7,rect.top + i/2);
	pDC->LineTo(rect.left + 7,rect.top + i/2);
	
	str.Format((t<1.0)?("%.1f"):("%.0f"),t/2);
	rcBar.top    = rect.top + (int)(i/2);
	rcBar.bottom = rcBar.top + 15;
	pDC->DrawText(str,rcBar,DT_RIGHT);

	pDC->MoveTo(rect.left - 5,rect.top + i*3/4);
	pDC->LineTo(rect.left + 5,rect.top + i*3/4);

	rcBar = rect;
	rcBar.DeflateRect(0,10,0,0);

	pDC->MoveTo(rect.left  -15, rect.bottom);
	pDC->LineTo(rect.right +15, rect.bottom);

	pDC->MoveTo(rect.left , rect.top    -15);
	pDC->LineTo(rect.left , rect.bottom +15);

	int scale = rcBar.Width()/iValues;
	int height = rcBar.Height();
	CRect tmpRect;
	
	for ( i = 0; i < iValues; i++ )
	{
		pTmp = m_ChartValues.GetAt(i);
		rcBar.left  = rect.left + scale*i + 2;
		rcBar.right = rcBar.left + max(1,scale - 4);
		rcBar.top = (rcBar.bottom - (long)(pTmp->m_dValue*height/t));
		rcBar.bottom = rect.bottom+1;

		//Drawing bar's

		CBrush BlueBrush(RGB(0, 0, 255));
		pDC->SelectObject(BlueBrush);
		pDC->Rectangle(rcBar);

		//Drawing top text percents
		tmpRect = rcBar;
		tmpRect.bottom = tmpRect.top - 1;
		tmpRect.top = tmpRect.bottom - 15;

		if(m_bShowPercentage)
			str.Format("%.1f%%",100*pTmp->m_dValue/m_dTotal);
		else
		{
			if(m_bIntegerValue)
				str.Format("%5d",(int)(pTmp->m_dValue));
			else
				str.Format("%5.2f",pTmp->m_dValue);
		}

		
		if(m_iCurSel == i)
		{
			CFont *tmpFont = pDC->SelectObject(&m_boldFont);			
			pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER);
			pDC->SelectObject(tmpFont);
		}else{
			pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER);
		} 

/*
		//Drawing bar text values
		tmpRect.top    = rcBar.top + 2;
		tmpRect.bottom = tmpRect.top + 15;
		str.Format((t<1.0)?("%.1f"):("%.0f"),pTmp->m_dValue);
		pDC->Rectangle(tmpRect);
		tmpRect.top += 1;
		pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER);
*/

		//Drawing bottom text labels
		tmpRect.top		= rcBar.bottom + 2;
		tmpRect.bottom  = tmpRect.top + 15;
		pDC->DrawText(pTmp->m_sLabel,tmpRect,DT_CENTER);
	}

	pDC->SelectObject(oldFont);
}

void CNodeChart::DrawLineChart(CDC *pDC)
{
	ASSERT(m_dTotal > 0.0L);

	int iValues	= m_ChartValues.GetSize();
	int iColors	= MAX_PIE_SIZE;
	CString str;

	CRect rcBar;
	CRect rect = m_DrawRect;


	CNSChartValue* pTmp;

    double t = 1.0L;
	if(m_dMax >= 1.0L){
		str.Format("%.0f",m_dMax);
		int d = 1;
		for(int i = 0;i<str.GetLength()-1;i++) d*=10;
		t = max((((double)str.GetAt(0)-48)+1)*d,m_dMax);
	}
	
	rect.DeflateRect(40,30,20,20);
	
	rcBar.left   =  5; 
	rcBar.right  = rcBar.left + 30; 
	rcBar.top    = 25;
	rcBar.bottom = rcBar.top + 15;
	int i = rect.Height();

	CFont *oldFont = pDC->SelectObject(&m_txtFont);

	pDC->MoveTo(rect.left - 7,rect.top);
	pDC->LineTo(rect.left + 7,rect.top);
	
	str.Format((t<1.0)?("%.1f"):("%.0f"),t);
	pDC->DrawText(str,rcBar,DT_RIGHT);
	
	pDC->MoveTo(rect.left - 5,rect.top + i/4);
	pDC->LineTo(rect.left + 5,rect.top + i/4);

	pDC->MoveTo(rect.left - 7,rect.top + i/2);
	pDC->LineTo(rect.left + 7,rect.top + i/2);
	
	str.Format((t<1.0)?("%.1f"):("%.0f"),t/2);
	rcBar.top    = rect.top + (int)(i/2);
	rcBar.bottom = rcBar.top + 15;
	pDC->DrawText(str,rcBar,DT_RIGHT);

	pDC->MoveTo(rect.left - 5,rect.top + i*3/4);
	pDC->LineTo(rect.left + 5,rect.top + i*3/4);

	rcBar = rect;
	rcBar.DeflateRect(0,10,0,0);

	pDC->MoveTo(rect.left  -15, rect.bottom);
	pDC->LineTo(rect.right +15, rect.bottom);

	pDC->MoveTo(rect.left , rect.top    -15);
	pDC->LineTo(rect.left , rect.bottom +15);

	int scale = rcBar.Width()/iValues;
	int height = rcBar.Height();
	CRect tmpRect;

	pDC->MoveTo(rect.left, rect.bottom);  // start from origin of chart

	int ValueStep = max(1,iValues/4);
	for ( i = 0; i < iValues; i++ )
	{
		pTmp = m_ChartValues.GetAt(i);
		rcBar.left  = rect.left + scale*i + 2;
		rcBar.right = rcBar.left + max(1,scale - 4);
		rcBar.top = (rcBar.bottom - (long)(pTmp->m_dValue*height/t));
		rcBar.bottom = rect.bottom+1;

		pDC->LineTo(rcBar.right, rcBar.top);

		if(i%ValueStep ==0)    ////Drawing top text
		{
			//Drawing top text percents
			tmpRect = rcBar;
			tmpRect.bottom = tmpRect.top - 1;
			tmpRect.top = tmpRect.bottom - 15;

			if(m_bShowPercentage)
				str.Format("%.1f%%",100*pTmp->m_dValue/m_dTotal);
			else
			{
				if(m_bIntegerValue)
					str.Format("%5d",(int)(pTmp->m_dValue));
				else
					str.Format("%.1f",pTmp->m_dValue);
			}
			if(m_iCurSel == i)
			{
				CFont *tmpFont = pDC->SelectObject(&m_boldFont);			
				pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER|DT_NOCLIP);
				pDC->SelectObject(tmpFont);
			}else{
				pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER|DT_NOCLIP);
			} 

			//Drawing bottom text labels
			tmpRect.top		= rcBar.bottom + 2;
			tmpRect.bottom  = tmpRect.top + 15;
			pDC->DrawText(pTmp->m_sLabel,tmpRect,DT_CENTER|DT_NOCLIP);

		}
/*
		//Drawing bar text values
		tmpRect.top    = rcBar.top + 2;
		tmpRect.bottom = tmpRect.top + 15;
		str.Format((t<1.0)?("%.1f"):("%.0f"),pTmp->m_dValue);
		pDC->Rectangle(tmpRect);
		tmpRect.top += 1;
		pDC->DrawText(str,tmpRect,DT_BOTTOM|DT_CENTER);
*/

	}

	pDC->SelectObject(oldFont);
}




void CNodeChart::SetChartStyle(NSChart_STYLES dStyle)
{
	m_dwStyle = dStyle;	
}




BOOL CNodeChart::AddValue(DOUBLE dValue, LPCTSTR strLabel,BOOL bRepaint /*= FALSE*/)
{
	if(dValue <= 0)
	{
		TRACE("The double value in function AddValue must be > 0 (dValue = %lf)",dValue);
		return FALSE;
	}

	CNSChartValue* newValue = new CNSChartValue;
	newValue->m_dValue  = dValue;
	newValue->m_sLabel  = strLabel;

	try 
	{
		m_ChartValues.Add(newValue);
		m_dTotal += dValue;
		if(m_dMax < dValue)
		{
			m_dMax = dValue;
		}
		
		m_iCurSel = -1;
		return TRUE;
	}
	catch (CMemoryException* e)
	{
		if (newValue !=NULL) 
		{
			delete newValue;
		}
		e->Delete();
		return FALSE;
	}
}

void CNodeChart::ResetChart()
{
	int iSize = m_ChartValues.GetSize();
	
	for (int i = 0; i < iSize; i++ )
	{
		delete m_ChartValues.GetAt(i);
	}
	m_ChartValues.RemoveAll();

		m_dTotal = 0;
		m_dMax = 0;
}





