// DlgPathMOE.cpp : implementation file
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
#include "BaseDialog.h"
#include "Network.h"
#include "DlgPathMOE.h"
#include "TLiteDoc.h"

 

extern float g_Simulation_Time_Stamp;
extern CPen s_PenSimulationClock;

extern void g_SelectSuperThickPenColor(CDC* pDC, int ColorCount);
extern void g_SelectThickPenColor(CDC* pDC, int ColorCount);
extern void g_SelectBrushColor(CDC* pDC, int ColorCount);

static _TCHAR *_gColumnMOELabel[NUM_PATHMOES] =
{
	_T("Color"), _T("Distance"), _T("Avg Travel Time"), _T("Worst Cast TT"), 
	_T("Variability (STD)"), _T("Fuel (gallons)"), _T("CO2 (pounds)"), _T("Total Cost ($)")
};

static int _gColumnFormat[NUM_PATHMOES] =
{
	LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_CENTER
};

_TCHAR *_gPathItem[NUM_PATHS][NUM_PATHMOES] =
{
	_T("Red"),  _T("255"), _T("255"), _T("0"),   _T("40"),  _T("240"), _T("120"), _T("Neutral"),
	_T("Green"),     _T("255"), _T("0"),   _T("0"),   _T("0"),   _T("240"), _T("120"), _T("Warm"),
	_T("Magenta"),   _T("0"),   _T("255"), _T("0"),   _T("80"),  _T("240"), _T("120"), _T("Cool"),
	_T("Cyan"), _T("255"), _T("0"),   _T("255"), _T("200"), _T("240"), _T("120"), _T("Warm"),
	_T("Blue"),    _T("0"),   _T("255"), _T("255"), _T("120"), _T("240"), _T("120"), _T("Cool"),
	_T("Yellow"),    _T("0"),   _T("0"),   _T("255"), _T("160"), _T("240"), _T("120"), _T("Cool"),
};


// CDlgPathMOE dialog

IMPLEMENT_DYNAMIC(CDlgPathMOE, CDialog)

BEGIN_MESSAGE_MAP(CDlgPathMOE, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PATH_LIST_CONTROL, &CDlgPathMOE::OnLvnItemchangedPathListControl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_MOETYPE1_MEANTRAVELTIME, &CDlgPathMOE::OnMoetype1Meantraveltime)
	ON_COMMAND(ID_MOETYPE1_MAXTRAVELTIME, &CDlgPathMOE::OnMoetype1Maxtraveltime)
	ON_COMMAND(ID_MOETYPE1_UTILITY, &CDlgPathMOE::OnMoetype1Mean)
	ON_UPDATE_COMMAND_UI(ID_MOETYPE1_MEANTRAVELTIME, &CDlgPathMOE::OnUpdateMoetype1Meantraveltime)
	ON_UPDATE_COMMAND_UI(ID_MOETYPE1_MAXTRAVELTIME, &CDlgPathMOE::OnUpdateMoetype1Maxtraveltime)
	ON_UPDATE_COMMAND_UI(ID_MOETYPE1_UTILITY, &CDlgPathMOE::OnUpdateMoetype1Utility)
	ON_COMMAND(ID_MOETYPE2_MEANTRAVELTIME, &CDlgPathMOE::OnMoetype2Meantraveltime)
	ON_COMMAND(ID_MOETYPE2_MAXTRAVELTIME, &CDlgPathMOE::OnMoetype2Maxtraveltime)
	ON_COMMAND(ID_MOETYPE2_UTILITY, &CDlgPathMOE::OnMoetype2Utility)
	ON_UPDATE_COMMAND_UI(ID_MOETYPE2_MEANTRAVELTIME, &CDlgPathMOE::OnUpdateMoetype2Meantraveltime)
	ON_UPDATE_COMMAND_UI(ID_MOETYPE2_MAXTRAVELTIME, &CDlgPathMOE::OnUpdateMoetype2Maxtraveltime)
	ON_UPDATE_COMMAND_UI(ID_MOETYPE2_UTILITY, &CDlgPathMOE::OnUpdateMoetype2Utility)
	ON_COMMAND(ID_MOETYPE2_NONE, &CDlgPathMOE::OnMoetype2None)
	ON_COMMAND(ID_MOETYPE2_BAND, &CDlgPathMOE::OnMoetype2Band)
	ON_COMMAND(ID_MOETYPE1_BAND, &CDlgPathMOE::OnMoetype1Band)
	ON_COMMAND(ID_MOETYPE2_FUELCONSUMPTIONS, &CDlgPathMOE::OnMoetype2Fuelconsumptions)
	ON_COMMAND(ID_MOETYPE2_CO2EMISSIONS, &CDlgPathMOE::OnMoetype2Co2emissions)
	ON_COMMAND(ID_MOETYPE1_FUELCONSUMPTIONS, &CDlgPathMOE::OnMoetype1Fuelconsumptions)
	ON_COMMAND(ID_MOETYPE1_CO2EMISSIONS, &CDlgPathMOE::OnMoetype1Co2emissions)

	ON_COMMAND(ID_MOETYPE1_COST, &CDlgPathMOE::OnMoetype1Cost)
	ON_COMMAND(ID_MOETYPE2_COST, &CDlgPathMOE::OnMoetype2Cost)


END_MESSAGE_MAP()

CDlgPathMOE::CDlgPathMOE(CWnd* pParent /*=NULL*/)
: CBaseDialog(CDlgPathMOE::IDD, pParent)
{
	Cur_MOE_type1 = -1; 
	Cur_MOE_type2 = -1;
	m_TimeLeft = 0;
	m_TimeRight = 1440; 
	m_YLowerBound = 0;
	m_bShowMaxTravelTime = false;
}

CDlgPathMOE::~CDlgPathMOE()
{
}

void CDlgPathMOE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATH_LIST_CONTROL, m_PathListControl);
}


BOOL CDlgPathMOE::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SmallImageList.Create(IDB_PATH_ICON, 16, 1, RGB(255, 255, 255));
	m_SmallImageList.SetOverlayImage(NUM_PATHS, 1);

	m_PathListControl.SetImageList(&m_SmallImageList, LVSIL_SMALL);

	// insert columns

	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(i = 0; i<NUM_PATHMOES; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gColumnMOELabel[i];
		lvc.cx = 90;
		lvc.fmt = _gColumnFormat[i];
		m_PathListControl.InsertColumn(i,&lvc);
	}



	m_PathListControl.SetExtendedStyle(LVS_EX_AUTOSIZECOLUMNS | LVS_EX_FULLROWSELECT |LVS_EX_HEADERDRAGDROP);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgPathMOE::InsertPathMOEItem()
{
	m_PathListControl.DeleteAllItems ();
	// insert items

	LV_ITEM lvi;

	unsigned int i;
	for(i = 0; i < m_pDoc->m_PathDisplayList.size(); i++)
	{
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = _gPathItem[i][0];
		lvi.iImage = i;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(1);
		m_PathListControl.InsertItem(&lvi);
	}

	// set item text for additional columns

	int CurrentTime = g_Simulation_Time_Stamp/15*15;
	for(i = 0; i<m_pDoc->m_PathDisplayList.size(); i++)
	{

		char text[100];
		sprintf_s(text, "%5.1f",m_pDoc->m_PathDisplayList[i].m_Distance);
		m_PathListControl.SetItemText(i,1,text);

		sprintf_s(text, "%5.1f",m_pDoc->m_PathDisplayList[i].GetTimeDependentMOE(CurrentTime, 0));
		m_PathListControl.SetItemText(i,2,text);

		sprintf_s(text, "%5.1f",m_pDoc->m_PathDisplayList[i].GetTimeDependentMOE(CurrentTime, 1));
		m_PathListControl.SetItemText(i,3,text);

		sprintf_s(text, "%5.1f",m_pDoc->m_PathDisplayList[i].GetTimeDependentMOE(CurrentTime, 3));
		m_PathListControl.SetItemText(i,4,text);

		sprintf_s(text, "%5.2f",m_pDoc->m_PathDisplayList[i].m_WithinDayMeanTimeDependentFuelConsumption[CurrentTime]);
		m_PathListControl.SetItemText(i,5,text);

		sprintf_s(text, "%5.2f",m_pDoc->m_PathDisplayList[i].m_WithinDayMeanTimeDependentEmissions[CurrentTime]);
		m_PathListControl.SetItemText(i,6,text);

		sprintf_s(text, "%5.2f",m_pDoc->m_PathDisplayList[i].m_WithinDayMeanGeneralizedCost[CurrentTime]);
		m_PathListControl.SetItemText(i,7,text);

	}
}
void CDlgPathMOE::OnLvnItemchangedPathListControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION pos = m_PathListControl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		m_pDoc->m_SelectPathNo = m_PathListControl.GetNextSelectedItem(pos);
		Invalidate();

		m_pDoc->UpdateAllViews(0);
	}

}


// CDlgPathMOE message handlers
void CDlgPathMOE::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect PlotRect;
	GetClientRect(PlotRect);

	CRect PlotRectOrg = PlotRect;

	if(m_TimeLeft<0)
		m_TimeLeft = 0;

	if(m_TimeRight< m_TimeLeft+30)
		m_TimeRight= m_TimeLeft+30;


	if(Cur_MOE_type1>=0)
	{
	if(Cur_MOE_type2==-1 )
	{
		PlotRect.top += 200;
		PlotRect.bottom -= 35;
		PlotRect.left += 60;
		PlotRect.right -= 100;

		DrawPlot(&dc,Cur_MOE_type1, PlotRect);
	}else
	{
		PlotRect.top += 200;
		PlotRect.bottom = 200+ (PlotRect.bottom-200-35)/2;
		PlotRect.left += 60;
		PlotRect.right -= 100;

		DrawPlot(&dc, Cur_MOE_type1, PlotRect);

		PlotRect.top = PlotRect.bottom+45;
		PlotRect.bottom = PlotRectOrg.bottom -20;
		PlotRect.left = PlotRectOrg.left+60;
		PlotRect.right = PlotRectOrg.right-100;

		DrawPlot(&dc,Cur_MOE_type2, PlotRect);

	}
	}
}

void CDlgPathMOE::OnSize(UINT nType, int cx, int cy)
{
	RedrawWindow();

	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}



void CDlgPathMOE::DrawPlot(CPaintDC* pDC,int MOEType, CRect PlotRect)
{
	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));

	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_DOT,0,RGB(0,0,0));

	CString str_MOE;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&NormalPen);

// draw title
	switch (MOEType)
	{
	case 0: str_MOE.Format ("Mean Travel Time (min)"); break;
	case 1: str_MOE.Format ("Max Travel Time (min)"); break;
	case 2: str_MOE.Format ("Travel Time Utility: Mean + 1.67*STD (min)"); break;
	case 3: str_MOE.Format ("Travel Time Band [Mean, Max] (min)"); break;
	case 4: str_MOE.Format ("Fuel Consumptions (gallon)"); break;
	case 5: str_MOE.Format ("CO2 Emissions (pound)"); break;
	case 6: str_MOE.Format ("Generalized Cost ($)"); break;

	}
	pDC->TextOut(PlotRect.right/2,PlotRect.top-20,str_MOE);

	// step 1: calculate m_YUpperBound;
	m_YUpperBound = 0;
	int i;


	// step 2: calculate m_UnitDistance;
	// data unit
	m_UnitDistance = 1;
	if((m_YUpperBound - m_YLowerBound)>0)
		m_UnitDistance = (float)(PlotRect.bottom - PlotRect.top)/(m_YUpperBound - m_YLowerBound);


	// step 3: time interval
	int TimeXPosition;

	int TimeInterval = g_FindClosestTimeResolution(m_TimeRight - m_TimeLeft);

	// time unit
	m_UnitTime = 1;
	if((m_TimeRight - m_TimeLeft)>0)
		m_UnitTime = (float)(PlotRect.right - PlotRect.left)/(m_TimeRight - m_TimeLeft);


	// step 4: draw time axis

	pDC->SelectObject(&TimePen);



	char buff[20];
	for(i=m_TimeLeft;i<=m_TimeRight;i+=TimeInterval)
	{
		if(i == m_TimeLeft || i==m_TimeRight)
		{
			pDC->SelectObject(&NormalPen);

			//			i = int((m_TimeLeft/TimeInterval)+0.5)*TimeInterval; // reset time starting point
		}
		else
			pDC->SelectObject(&DataPen);

		TimeXPosition=(long)(PlotRect.left+(i-m_TimeLeft)*m_UnitTime);

		if(i>= m_TimeLeft)
		{
			pDC->MoveTo(TimeXPosition,PlotRect.bottom+2);
			pDC->LineTo(TimeXPosition,PlotRect.top);

			if(i/2 <10)
				TimeXPosition-=5;
			else
				TimeXPosition-=3;

			if(TimeInterval < 60)
			{
				int hour, min;
				hour = i/60;
				min =  i- hour*60;
				wsprintf(buff,"%2d:%02d",hour, min);
			}
			else
			{
				int min_in_a_day = i-int(i/1440*1440);

				wsprintf(buff,"%dh",min_in_a_day/60 );

			}
			pDC->TextOut(TimeXPosition,PlotRect.bottom+3,buff);
		}
	}

	pDC->SelectObject(&s_PenSimulationClock);
	if(g_Simulation_Time_Stamp >=m_TimeLeft && g_Simulation_Time_Stamp <= m_TimeRight )
	{
		TimeXPosition=(long)(PlotRect.left+(g_Simulation_Time_Stamp -m_TimeLeft)*m_UnitTime);
		pDC->MoveTo(TimeXPosition,PlotRect.bottom+2);
		pDC->LineTo(TimeXPosition,PlotRect.top);
	}


	m_YUpperBound = 0;
	m_YLowerBound = 0;
	TimeInterval = 15;

	int value_type = MOEType;
	if(MOEType==3)  // band: use max
		value_type = 1;

	for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++)
	{
		for(int t=m_TimeLeft;t<m_TimeRight;t+=TimeInterval)
		{
			if( m_YUpperBound < m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, value_type) )
				m_YUpperBound =  m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, value_type);

		}

	}

	if(m_YUpperBound>=10.0f)
		m_YUpperBound = (m_YUpperBound/10.0f+1.0f)*10.f;
	else
		m_YUpperBound = int(m_YUpperBound)+1.0f;

	float YInterval = 0.2f;

	if(m_YUpperBound>=2.0f)
		YInterval = 0.5f;

	if(m_YUpperBound>=5.0f)
		YInterval = 1.f;

	if(m_YUpperBound>=10.0f)
		YInterval = 2.0f;

	if(m_YUpperBound>=20.0f)
		YInterval = 5.0f;

	if(m_YUpperBound>=50.f)
		YInterval = 10.f;

	if(m_YUpperBound>=80.f)
		YInterval = 20.f;

	if(m_YUpperBound>=150.f)
		YInterval = 30.f;



	// data unit
	m_UnitData = 1.f;
	if((m_YUpperBound - m_YLowerBound)>0.0f)
		m_UnitData = (float)(PlotRect.bottom - PlotRect.top)/(m_YUpperBound - m_YLowerBound);

	// draw Y axis

	for(float ii=m_YLowerBound; ii <= m_YUpperBound; ii+= YInterval)
	{
		if( ii > m_YUpperBound)
			ii = m_YUpperBound;

		if(ii == m_YLowerBound)
			pDC->SelectObject(&NormalPen);
		else
			pDC->SelectObject(&DataPen);

		int TimeYPosition= PlotRect.bottom - (int)((ii*m_UnitData)+0.50f);

		pDC->MoveTo(PlotRect.left-2, TimeYPosition);
		pDC->LineTo(PlotRect.right,TimeYPosition);

		if(ii <= m_YUpperBound)
		{
			if(YInterval>=1)
			sprintf_s(buff,"%3.0f",ii);
			else
			sprintf_s(buff,"%3.1f",ii);

			pDC->TextOut(PlotRect.left-55,TimeYPosition-5,buff);
		}
	}

	if(MOEType!=3)
	{
	for(unsigned int p = 0; p < m_pDoc->m_PathDisplayList.size(); p++)
	{
		if(p== m_pDoc->m_SelectPathNo)
			g_SelectSuperThickPenColor(pDC,p);
		else
			g_SelectThickPenColor(pDC,p);

		for(int t=m_TimeLeft;t<m_TimeRight;t+=TimeInterval)
		{
			int TimeYPosition= PlotRect.bottom - (int)((m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, MOEType)*m_UnitData)+0.50);
			TimeXPosition=(long)(PlotRect.left+(t-m_TimeLeft)*m_UnitTime);


			if(t==m_TimeLeft)
				pDC->MoveTo(TimeXPosition,TimeYPosition);
			else
				pDC->LineTo(TimeXPosition,TimeYPosition);

		}

	}
	}

	// max travel time  // draw band (mean, max) for selected paths
	if(MOEType==3)  
	{
	POSITION pos = m_PathListControl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int p = m_PathListControl.GetNextSelectedItem(pos);

		TimeInterval = 15;

		if(p== m_pDoc->m_SelectPathNo)
			g_SelectSuperThickPenColor(pDC,p);


		CPoint pt[192];

		int pt_count = 0;
		int t;
		for(t=0;t<1440;t+=15)
			{
			int TimeYPosition= PlotRect.bottom - (int)((m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, 1)*m_UnitData)+0.50);
				TimeXPosition=(long)(PlotRect.left+(t-m_TimeLeft)*m_UnitTime);

			pt[pt_count].x =TimeXPosition;
			pt[pt_count].y =TimeYPosition;
			
			pt_count++;

			}

		for(t=1440-15;t>=0;t-=15)
			{
			int TimeYPosition= PlotRect.bottom - (int)((m_pDoc->m_PathDisplayList[p].GetTimeDependentMOE(t, 0)*m_UnitData)+0.50);
			TimeXPosition=(long)(PlotRect.left+(t-m_TimeLeft)*m_UnitTime);

			pt[pt_count].x =TimeXPosition;
			pt[pt_count].y =TimeYPosition;
			
			pt_count++;

			}

		g_SelectThickPenColor(pDC,p);
		g_SelectBrushColor(pDC, p);
		pDC->Polygon (pt,192);

	}
	}


/*
// step 5: draw segments along the select path

m_SegmentDistanceVector.resize(m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkSize);

for (i=0 ; i<m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkSize; i++)
{
DTALink* pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkVector[i]];
if(pLink!=NULL)
{
m_YUpperBound+=pLink->m_Length ;

m_SegmentDistanceVector[i] = m_YUpperBound;
}
}

int TimeYPosition;
int TimeYPositionPrev;

for (i=0 ; i<m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkSize; i++)
{
DTALink* pLink;

// extra bottom line
if(i == 0)
{

pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkVector[i]];

pDC->SelectObject(&NormalPen);
TimeYPosition= PlotRect.bottom;
TimeYPositionPrev = TimeYPosition;

if(pLink!=NULL)
{
wsprintf(buff,"A: %d",pLink->m_FromNodeNumber );
pDC->TextOut(PlotRect.left-50,TimeYPosition-5,buff);
}

pDC->MoveTo(PlotRect.left-2, TimeYPosition);
pDC->LineTo(PlotRect.right,TimeYPosition);
}


pDC->SelectObject(&DataPen);

TimeYPosition= PlotRect.bottom - (int)((m_SegmentDistanceVector[i]*m_UnitDistance)+0.50);

pDC->MoveTo(PlotRect.left-2, TimeYPosition);
pDC->LineTo(PlotRect.right,TimeYPosition);

pLink = m_pDoc->m_LinkNoMap[m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkVector[i]];
if(pLink!=NULL)
{
if(i== m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkSize-1)
{
wsprintf(buff,"B: %d",pLink->m_ToNodeNumber );
pDC->TextOut(PlotRect.left-50,TimeYPosition-5,buff);
}
else
{
if(TimeYPosition < TimeYPositionPrev-10 && TimeYPosition >= PlotRect.bottom-10)
{
wsprintf(buff,"%d",pLink->m_ToNodeNumber );
pDC->TextOut(PlotRect.left-40,TimeYPosition-5,buff);
}
}

TimeYPositionPrev = TimeYPosition;

}
}

// draw every day time series
// step 5: draw segments along the select path

pDC->SelectObject(&DataPen);

int DepartureTime = 0;

for(day = 0; day <Number_of_days; day++)
{
for (i=0 ; i<m_pDoc->m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkSize; i++)
{
DTALink* pLink;
int TimeYPosition;

pDC->MoveTo(PlotRect.left-2, TimeYPosition);
pDC->LineTo(PlotRect.right,TimeYPosition);

pLink = m_pDoc->m_LinkNoMap[m_PathDisplayList[m_pDoc->m_SelectPathNo]->m_LinkVector[i]];


}
}
*/

}


void CDlgPathMOE::OnMoetype1Meantraveltime()
{
	Cur_MOE_type1 = 0;
	Invalidate();
}

void CDlgPathMOE::OnMoetype1Maxtraveltime()
{
	Cur_MOE_type1 = 1;
	Invalidate();
}

void CDlgPathMOE::OnMoetype1Mean()
{
	Cur_MOE_type1 = 2;
	Invalidate();
}

void CDlgPathMOE::OnUpdateMoetype1Meantraveltime(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Cur_MOE_type1==0);
}

void CDlgPathMOE::OnUpdateMoetype1Maxtraveltime(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Cur_MOE_type1==1);
}

void CDlgPathMOE::OnUpdateMoetype1Utility(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Cur_MOE_type1==2);
}

void CDlgPathMOE::OnMoetype2Meantraveltime()
{
	Cur_MOE_type2 = 0;
	Invalidate();
}

void CDlgPathMOE::OnMoetype2Maxtraveltime()
{
	Cur_MOE_type2 = 1;
	Invalidate();
}

void CDlgPathMOE::OnMoetype2Utility()
{
	Cur_MOE_type2 = 2;
	Invalidate();
}

void CDlgPathMOE::OnUpdateMoetype2Meantraveltime(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Cur_MOE_type2==0);
}

void CDlgPathMOE::OnUpdateMoetype2Maxtraveltime(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Cur_MOE_type2==1);
}

void CDlgPathMOE::OnUpdateMoetype2Utility(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Cur_MOE_type2==2);
}

void CDlgPathMOE::OnMoetype2None()
{
	Cur_MOE_type2 = -1;
	Invalidate();
}

void CDlgPathMOE::OnMoetype2Band()
{
	Cur_MOE_type2 = 3;
	Invalidate();
}

void CDlgPathMOE::OnMoetype1Band()
{
	Cur_MOE_type1 = 3;
	Invalidate();
}

void CDlgPathMOE::OnMoetype2Fuelconsumptions()
{
	Cur_MOE_type2 = 4;
	Invalidate();
}

void CDlgPathMOE::OnMoetype2Co2emissions()
{
	Cur_MOE_type2 = 5;
	Invalidate();
}

void CDlgPathMOE::OnMoetype1Fuelconsumptions()
{
	Cur_MOE_type1 = 4;
	Invalidate();
}

void CDlgPathMOE::OnMoetype1Co2emissions()
{
	Cur_MOE_type1 = 5;
	Invalidate();
}
void CDlgPathMOE::OnMoetype1Cost()
{
	Cur_MOE_type1 = 6;
	Invalidate();
}
void CDlgPathMOE::OnMoetype2Cost()
{
	Cur_MOE_type2 = 6;
	Invalidate();
}


