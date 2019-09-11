// Dlg_Legend.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_Legend.h"


// CDlg_Legend dialog

IMPLEMENT_DYNAMIC(CDlg_Legend, CBaseDialog)

CDlg_Legend::CDlg_Legend(CWnd* pParent /*=NULL*/)
: CBaseDialog(CDlg_Legend::IDD, pParent)
, m_bShowRadarChart(FALSE)
{
	m_bShowRadarChart = false;
}

CDlg_Legend::~CDlg_Legend()
{
}

void CDlg_Legend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EMISSIONTYPE, m_ComboBox_EmissionType);
	DDX_Control(pDX, IDC_COMBO_AGGREGATION, m_QueueCutOffComboBox);
	DDX_Control(pDX, IDC_STATIC_QUEUE_CUT_OFF, m_TextCutOff);
	DDX_Control(pDX, IDC_STATIC_QUEUE_STUDY_PERIOD, m_StudyPeriodText);
	DDX_Control(pDX, IDC_COMBO_STUDY_PERIOD, m_ComboxStudyPeriod);
	DDX_Check(pDX, IDC_CHECK_RADAR_CHART, m_bShowRadarChart);
	DDX_Control(pDX, IDC_CHECK_RADAR_CHART, m_RadarChartButton);
}


BEGIN_MESSAGE_MAP(CDlg_Legend, CBaseDialog)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_EMISSIONTYPE, &CDlg_Legend::OnCbnSelchangeComboEmissiontype)
	ON_CBN_SELCHANGE(IDC_COMBO_AGGREGATION, &CDlg_Legend::OnCbnSelchangeComboAggregation)
	ON_STN_CLICKED(IDC_STATIC_QUEUE_CUT_OFF, &CDlg_Legend::OnStnClickedStaticQueueCutOff)
	ON_CBN_SELCHANGE(IDC_COMBO_STUDY_PERIOD, &CDlg_Legend::OnCbnSelchangeComboStudyPeriod)
	ON_BN_CLICKED(IDC_CHECK_RADAR_CHART, &CDlg_Legend::OnBnClickedCheckRadarChart)
END_MESSAGE_MAP()


// CDlg_Legend message handlers
BOOL CDlg_Legend::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	m_ComboBox_EmissionType.AddString ("CO2");
	m_ComboBox_EmissionType.AddString ("NOx");
	m_ComboBox_EmissionType.AddString ("CO");
	m_ComboBox_EmissionType.AddString ("HC");

	m_ComboBox_EmissionType.SetCurSel (0);

	m_ComboBox_EmissionType.ShowWindow (0); // not showing


	for(int p = 10; p<=100; p+=10)
	{
		CString str;
		str.Format("%d%%",p);
		m_QueueCutOffComboBox.AddString(str);
	}


	for(int p = 1; p<=24; p++)  //24 hours
	{
		CString str;
		str.Format("%d",p);
		m_ComboxStudyPeriod.AddString(str);
	}

	m_QueueCutOffComboBox.SetCurSel(8-1); // 80%
	m_QueueCutOffComboBox.ShowWindow (0);
	m_TextCutOff.ShowWindow (0);


	m_ComboxStudyPeriod.SetCurSel(0);
	m_ComboxStudyPeriod.ShowWindow (0);
	m_StudyPeriodText.ShowWindow (0);




	// Calculate your x and y coordinates of the upper-left corner 
	// of the dialog (in screen coordinates) based on some logic

	SetWindowPos(NULL, 200, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	return TRUE;
}
void CDlg_Legend::OnPaint()
{

	CRect rectClient(0,0,0,0);
	GetClientRect(&rectClient);
	CPaintDC dc(this); // device context for painting

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rectClient.Width(),
		rectClient.Height());
	memDC.SelectObject(&bmp);


	CBrush brush;

	if (!brush.CreateSolidBrush(theApp.m_BackgroundColor))
		return;

	brush.UnrealizeObject();
	memDC.FillRect(rectClient, &brush);

	DrawObjects(&memDC);

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0,
		0, SRCCOPY);

	ReleaseDC(&dc);

}
void CDlg_Legend::DrawObjects(CDC* pDC)
{

	CPen penLOS[MAX_LOS_SIZE];
	CBrush brushLOS[MAX_LOS_SIZE];
	CBrush BlueBrush(RGB(0, 0, 255));

	int i;

	if(m_pDoc==NULL)
		return;
	for(i = 1; i<MAX_LOS_SIZE-1; i++)
	{
		penLOS[i].CreatePen(PS_SOLID, 1, m_pDoc->m_colorLOS[i]);
		brushLOS[i].CreateSolidBrush(m_pDoc->m_colorLOS[i]);
	}


	SetBackgroundColor(RGB(255, 255, 255));

	if(m_pDoc->m_LinkMOEMode == MOE_emissions)
		m_ComboBox_EmissionType.ShowWindow (1);
	else
		m_ComboBox_EmissionType.ShowWindow (0); // not showing

	if(m_pDoc->m_LinkMOEMode == MOE_impact || m_pDoc->m_LinkMOEMode == MOE_bottleneck  )
	{

		m_ComboxStudyPeriod.ShowWindow (1);
		m_StudyPeriodText.ShowWindow (1);

	if(m_pDoc->m_LinkMOEMode == MOE_impact  )
	{
		m_QueueCutOffComboBox.ShowWindow (1);
		m_TextCutOff.ShowWindow (1);
	}else
	{
		m_TextCutOff.ShowWindow (0);
		m_QueueCutOffComboBox.ShowWindow (0);

	
	}

	}else
	{
		m_QueueCutOffComboBox.ShowWindow (0); // not showing
		m_TextCutOff.ShowWindow (0);
		m_StudyPeriodText.ShowWindow (0);
		m_ComboxStudyPeriod.ShowWindow (0);
	}

	if(m_pDoc->m_LinkMOEMode == MOE_bottleneck)
	{
	
		m_RadarChartButton.ShowWindow(1);
	}else
	{
	m_RadarChartButton.ShowWindow(0);
	}


	switch(m_pDoc->m_LinkMOEMode )
	{
	case MOE_volume: SetWindowText("Link Volume"); break;
	case MOE_speed: SetWindowText("% of Speed Limit"); break;
	case MOE_density: SetWindowText("Density (vhc/distance/ln)"); break;
	case MOE_reliability: SetWindowText("Variability Ratio"); break;
	case MOE_impact: SetWindowText("Queue Duration and Congestion Age"); break;
	case MOE_bottleneck: SetWindowText("Bottleneck Charts and Congestion Age"); break;
	case MOE_emissions: 

		switch (m_pDoc->m_EmissionType )
		{
		case  DTA_CO2:  SetWindowText("Avg CO2 (kg) Per Vehicle Per Distance Unit"); break;	 
		case  DTA_NOX:  SetWindowText("Avg NOx (g) Per Vehicle Per Distance Unit"); break;	 
		case  DTA_CO:  SetWindowText("Avg CO (g) Per Vehicle Per Distance Unit"); break;	 
		case  DTA_HC:  SetWindowText("Avg HC (g) Per Vehicle Per Distance Unit"); break;	 
		default:  SetWindowText("Avg Energy (KJ) Per Vehicle Per Distance Unit"); 
		}


		break;
	case MOE_safety: SetWindowText("Annual Crash Frequency (per mile)"); break;
	default:  SetWindowText("Default Legend");

	}

	int TimeBound[10]={0};
	CString TimeStringVector[10];

	for(int los= 2; los<=7; los++)
	{
		TimeBound[los] = g_Simulation_Time_Stamp + (m_pDoc->m_LOSBound[MOE_impact][los])/100.0*g_ImpactStudyPeriodInMin;
	}

	TimeStringVector[1] = "New Congestion";

	int hour = g_ImpactStudyPeriodInMin/60;
	TimeStringVector[6].Format("%d-hour old", hour) ;


	pDC->SetBkMode(TRANSPARENT);


	int width =  40;
	int height = 20;

	CRect lr;

	if(m_pDoc->m_LinkMOEMode != MOE_impact && m_pDoc->m_LinkMOEMode != MOE_bottleneck)
		lr.top = 10;
	else
		lr.top = 40;


	lr.bottom = lr.top + height;
	lr.left = 10;
	lr.right = lr.left + width;

	if(m_pDoc->m_LinkMOEMode < 0)
		return;


	int MOE_checking_index = m_pDoc->m_LinkMOEMode ;

	if(m_pDoc->m_LinkMOEMode == MOE_emissions )
	{

		MOE_checking_index =  m_pDoc->m_LinkMOEMode + m_pDoc->m_EmissionType;
	}

	if(m_pDoc->m_LinkMOEMode != MOE_volume )
	{
	if(m_pDoc->m_LinkMOEMode != MOE_impact && m_pDoc->m_LinkMOEMode != MOE_bottleneck)
	{

		for(i = 1; i< MAX_LOS_SIZE-1; i++)
		{
			pDC->SelectObject(penLOS[i]);
			pDC->SelectObject(brushLOS[i]);
			CString lengend_interval_str;

			if(i != MAX_LOS_SIZE-2) 
				lengend_interval_str.Format(" %.2f - %.2f",m_pDoc->m_LOSBound[MOE_checking_index][i],m_pDoc->m_LOSBound[MOE_checking_index][i+1]);
			else
			{
				if(m_pDoc->m_LOSBound[MOE_checking_index][i] > m_pDoc->m_LOSBound[MOE_checking_index][i+1])
					lengend_interval_str.Format(" < %.2f ",m_pDoc->m_LOSBound[MOE_checking_index][i]);
				else
					lengend_interval_str.Format(" > %.2f ",m_pDoc->m_LOSBound[MOE_checking_index][i]);
			}

			pDC->TextOut(lr.right,lr.top,lengend_interval_str);
			pDC->Rectangle(lr);
			lr.top = lr.bottom;
			lr.bottom = lr.top + height;
		}
	}else
	{
		width =  30;

		for(i = MAX_LOS_SIZE-2; i>=1 ; i--)
		{
			pDC->SelectObject(penLOS[i]);
			pDC->SelectObject(brushLOS[i]);
			CString lengend_interval_str;


			if(TimeStringVector[i].GetLength () > 0)
			{
				lengend_interval_str.Format("%s",TimeStringVector[i]);
			}

			pDC->TextOut(lr.left,lr.bottom+10,lengend_interval_str);
			pDC->Rectangle(lr);
			lr.left = lr.right;
			lr.right =  lr.left + width;

		}

		lr.left = 10;
		lr.right = 10;
	}
	}
	
	CBrush WhiteBrush(RGB(255, 255, 255));

	if(m_pDoc->m_LinkMOEMode != MOE_volume )
	{
	pDC->SelectObject(WhiteBrush);	
	}
	else
	{
	pDC->SelectObject(BlueBrush);

	}

	//
	//	  width

	CPen BlackPen(PS_SOLID,1,RGB(0,0,0));

	pDC->SelectObject(BlackPen);


	if(m_pDoc->m_LinkMOEMode != MOE_bottleneck)
	{ // draw width legend

	width =  90;
	lr.left = 10;
	lr.right = lr.left + width;


	if(m_pDoc->m_LinkMOEMode != MOE_impact && m_pDoc->m_LinkMOEMode != MOE_bottleneck)
		lr.top = lr.bottom + 80;
	else
		lr.top = lr.bottom + 130;

	double lane_offset = m_pDoc->m_UnitFeet*m_pDoc->m_LaneWidthInFeet;  // 20 feet per lane

	double BandWidthValue = 0;
	CString band_width_str;

	CString first_str, second_str;

	if(m_pDoc->m_LinkBandWidthMode == LBW_number_of_lanes)
	{

		BandWidthValue =  m_pDoc->GetLinkBandWidth(4);
		band_width_str = "Band Width: Number of Lanes";
		first_str = "4 lanes";
		second_str = "2 lanes";


	}else if (m_pDoc->m_LinkBandWidthMode == LBW_link_volume)
	{

		double base_bandwidth_value = 3000;

		//first check
	
		BandWidthValue =  m_pDoc->GetLinkBandWidth(base_bandwidth_value);
		height = max(1,lane_offset * BandWidthValue *1 *m_pDoc->m_Doc_Resolution);

		while(height>50)
		{
		base_bandwidth_value = base_bandwidth_value/2;
		BandWidthValue =  m_pDoc->GetLinkBandWidth(base_bandwidth_value);
		height = max(1,lane_offset * BandWidthValue *1 *m_pDoc->m_Doc_Resolution);

		
		}
		//second check

		// final calculation
		band_width_str = "Band Width: Link Volume Per Hour";

		height = max(1,lane_offset * BandWidthValue *1 *m_pDoc->m_Doc_Resolution);

		first_str.Format("%.0f",base_bandwidth_value);
		second_str.Format("%.0f",base_bandwidth_value/2);
		//if(height>50)
		//{
		//	BandWidthValue = 50/m_pDoc->m_Doc_Resolution/lane_offset;
		//	first_str.Format ("%.0f",BandWidthValue);
		//	second_str.Format ("%.0f",BandWidthValue/2);
		//	

		//}


	}else if (m_pDoc->m_LinkBandWidthMode == LBW_congestion_duration)
	{
		band_width_str = "Band Width: Queue Duration";

		double base_bandwidth_value = g_ImpactStudyPeriodInMin;

		BandWidthValue =  m_pDoc->GetLinkBandWidth(base_bandwidth_value);
		height = max(1,lane_offset * BandWidthValue *1 *m_pDoc->m_Doc_Resolution);

		while(height>50)
		{
		base_bandwidth_value = base_bandwidth_value/2;
		BandWidthValue =  m_pDoc->GetLinkBandWidth(base_bandwidth_value);
		height = max(1,lane_offset * BandWidthValue *1 *m_pDoc->m_Doc_Resolution);
		}

		first_str.Format ("%.0f min",BandWidthValue);
		second_str.Format ("%.0f min",BandWidthValue/2);
		
	} 


	//6000 veh/h/link
	//60 min

	height = max(1,lane_offset * BandWidthValue * m_pDoc->m_Doc_Resolution);
	lr.bottom = lr.top + height;
	pDC->TextOut(lr.left,lr.top-20,band_width_str);
	pDC->TextOut(lr.left,lr.bottom+10,first_str);

	pDC->Rectangle(lr); // (1) 

	height = max(1,lane_offset * BandWidthValue *0.5 *m_pDoc->m_Doc_Resolution);

	lr.left = lr.right;
	lr.right = lr.left + width;
	lr.top =  lr.bottom - height;
	pDC->Rectangle(lr); // (1) 
	pDC->TextOut(lr.left,lr.bottom+10,second_str);

	}else
	{

	float avg_delay = 60;
	float size = avg_delay*m_pDoc->m_BottleneckDisplaySize;

	//back calculate delay if there is a size constraint
	if(size > 30)
	{
		size = 30;
		avg_delay = size/m_pDoc->m_BottleneckDisplaySize;
	}


	CString first_str;
	
	first_str.Format ("%.0f min",avg_delay);

	CPoint point;

	point.x	= lr.left + 80 + size;
	point.y	= lr.bottom + 130 + size;
	pDC->TextOut(point.x , lr.bottom+110,first_str);


	CRect ChartRect(point.x - size, point.y + size,
			point.x + size, point.y - size);

	CRect rect = ChartRect;
  
    CPen pen(PS_SOLID, 1, RGB(0,0,255));
    pDC->SelectObject(&pen);


    pDC->Ellipse(rect);
	
	}
}


void CDlg_Legend::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);
	RedrawWindow();
}

void CDlg_Legend::OnCbnSelchangeComboEmissiontype()
{
	if(m_pDoc->m_LinkMOEMode == MOE_emissions)
	{
		m_pDoc->m_EmissionType = (DTA_EMISSION_TYPE) m_ComboBox_EmissionType.GetCurSel ();

		Invalidate(1);
		m_pDoc->UpdateAllViews (0);
	}
}

void CDlg_Legend::OnCbnSelchangeComboAggregation()
{
	if(m_pDoc->m_LinkMOEMode == MOE_impact || m_pDoc->m_LinkMOEMode == MOE_bottleneck)
	{
		g_ImpactThreshold_QueueLengthPercentage= ( m_QueueCutOffComboBox.GetCurSel () +1)*10;

		Invalidate(1);
		m_pDoc->UpdateAllViews (0);
	}
}

void CDlg_Legend::OnStnClickedStaticQueueCutOff()
{
	// TODO: Add your control notification handler code here
}



void CDlg_Legend::OnCbnSelchangeComboStudyPeriod()
{
	if(m_pDoc->m_LinkMOEMode == MOE_impact ||  m_pDoc->m_LinkMOEMode == MOE_bottleneck)
	{
		g_ImpactStudyPeriodInMin= ( m_ComboxStudyPeriod.GetCurSel () +1)*60;

		Invalidate(1);
		m_pDoc->UpdateAllViews (0);
	}
}

void CDlg_Legend::OnBnClickedCheckRadarChart()
{
	UpdateData(1);

	g_bShowRadarChart = m_bShowRadarChart;
}
