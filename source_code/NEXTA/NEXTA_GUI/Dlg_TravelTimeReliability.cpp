// Dlg_TravelTimeReliability.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_TravelTimeReliability.h"
#include "math.h"
#include <vector>
#include "Utility.h"

using std::vector;


// CDlg_TravelTimeReliability dialog

float proportion[4] = {0.6f,0.18f,0.12f,0.1f};
int IntProportion[4];

enum RELIABILITY_Y_CLASSIFICATION {
	REL_travel_time_90_percentile = 0,
	REL_travel_time_80_percentile,
	//REL_travel_time_Planning_Time_Index,
	//REL_travel_time_Buffer_Index,
	//REL_travel_time_Skew_Index,
	REL_capacity_impact,
	REL_occurance_probability
};
void g_RandomCapacity(float* ptr, int num, float mean, float COV,int seed)
{
	float mu, sigma;
	float variance = pow(mean*COV,2);
	mu = log(mean) - 1/2*log(1+variance/pow(mean,2));
	sigma = sqrt(log(1+variance/pow(mean,2)));

	srand(seed);

	for (int i=0;i<num;i++,ptr++)
	{
		*ptr = exp(mu+sigma*g_RNNOF());
	}
}

IMPLEMENT_DYNAMIC(CDlg_TravelTimeReliability, CDialog)

CDlg_TravelTimeReliability::CDlg_TravelTimeReliability(CWnd* pParent /*=NULL*/)
: CDialog(CDlg_TravelTimeReliability::IDD, pParent)
{
	m_dValue = 0;
	m_PathFreeFlowTravelTime = 15;

	for (int i=0;i<MAX_SAMPLE_SIZE;i++)
	{
		TravelTime[i] = Capacity[i] = AdditionalDelay[i] = 0.0f;
	}

	this->m_bImpacted = false;
	this->m_ImpactedLinkIdx = -1;
	this->m_ImpactDuration = 0;

	CalculationMethod = 0;

}

void CDlg_TravelTimeReliability::UpdateCapacityAndDelay()
{

	for (int i=0;i<MAX_SAMPLE_SIZE;i++)
	{
		TravelTime[i] = Capacity[i] = AdditionalDelay[i] = 0.0f;
	}

	int sum=0;

	for (int i=1;i<4;i++)
	{
		IntProportion[i] = (int)(proportion[i]*MAX_SAMPLE_SIZE);
		sum += IntProportion[i];
	}

	//Normal
	IntProportion[0] = MAX_SAMPLE_SIZE - sum;

	float capacity_lower_bound  = 2000;  // to aoid extrem large travel time


	if (CalculationMethod == 1)
	{
		for (unsigned int i=0;i<LinkCapacity.size();i++)
		{
			if (i != m_BottleneckIdx && i != m_ImpactedLinkIdx) //Non-bottleneck Link 
			{
				for (int j=0;j<MAX_SAMPLE_SIZE;j++)
				{
					TravelTime[j] += LinkTravelTime[i];
				}
			}
			else
			{
				if (m_bImpacted && i == m_ImpactedLinkIdx)
				{
					g_RandomCapacity(&Capacity[0],IntProportion[0],LinkCapacity[i],0.1f,100);
					g_RandomCapacity(&Capacity[IntProportion[0]],IntProportion[1],LinkCapacity[i]*m_LaneClosurePercentage,0.3f,100);
					g_RandomCapacity(&Capacity[IntProportion[0]+IntProportion[1]],IntProportion[2],LinkCapacity[i]*0.5f,0.2f,100);
					g_RandomCapacity(&Capacity[MAX_SAMPLE_SIZE-IntProportion[3]],IntProportion[3],LinkCapacity[i]*0.7f,0.2f,100);

					for (int j=0;j<MAX_SAMPLE_SIZE;j++)
					{
						AdditionalDelay[j] = LinkTravelTime[i]*(1-Capacity[j]/LinkCapacity[i]);
						TravelTime[j] += LinkTravelTime[i] + AdditionalDelay[j];
					}
				}
				else
				{
					if (m_bImpacted == false && i == m_BottleneckIdx)
					{
						g_RandomCapacity(&Capacity[0],IntProportion[0],LinkCapacity[i],0.3f,100);
						g_RandomCapacity(&Capacity[IntProportion[0]],IntProportion[1],LinkCapacity[i]*0.4,0.4f,100);
						g_RandomCapacity(&Capacity[IntProportion[0]+IntProportion[1]],IntProportion[2],LinkCapacity[i],0.25f,100);
						g_RandomCapacity(&Capacity[MAX_SAMPLE_SIZE-IntProportion[3]],IntProportion[3],LinkCapacity[i],0.25f,100);

						for (int j=0;j<MAX_SAMPLE_SIZE;j++)
						{
							AdditionalDelay[j] = LinkTravelTime[i]*(1-Capacity[j]/LinkCapacity[i]);
							TravelTime[j] += LinkTravelTime[i] + AdditionalDelay[j];
						}
					}
					else
					{
						for (int j=0;j<MAX_SAMPLE_SIZE;j++)
						{
							TravelTime[j] += LinkTravelTime[i];
						}
					}
				}
			}
		}
	}
	else
	{
		float baseCapacity = 1800.0f;

		if (m_bImpacted)
		{
			baseCapacity = LinkCapacity[m_ImpactedLinkIdx];
		}
		else
		{
			baseCapacity = LinkCapacity[m_BottleneckIdx];
		}

		g_RandomCapacity(&Capacity[0],IntProportion[0],baseCapacity,0.2f,100);
		g_RandomCapacity(&Capacity[IntProportion[0]],IntProportion[1],baseCapacity*0.5f,0.2f,100);
		g_RandomCapacity(&Capacity[IntProportion[0]+IntProportion[1]],IntProportion[2],baseCapacity*0.6f,0.2f,100);
		g_RandomCapacity(&Capacity[MAX_SAMPLE_SIZE-IntProportion[3]],IntProportion[3],baseCapacity*0.7f,0.2f,100);

		for (int j=0;j<MAX_SAMPLE_SIZE;j++)
		{
			AdditionalDelay[j] = m_PathFreeFlowTravelTime *(1-Capacity[j]/baseCapacity);
			TravelTime[j] += m_PathFreeFlowTravelTime + AdditionalDelay[j];
		}
	}
}

CDlg_TravelTimeReliability::~CDlg_TravelTimeReliability()
{
}

void CDlg_TravelTimeReliability::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_1, m_ReliabilityMOEList);
	DDX_Control(pDX, IDC_LIST2, m_7FactorMOEList);
	DDX_Text(pDX, IDC_VALUE, m_dValue);
	DDX_Text(pDX, IDC_LABEL, m_sLabel);

}


BEGIN_MESSAGE_MAP(CDlg_TravelTimeReliability, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, OnRadioButtons)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioButtons)
	ON_BN_CLICKED(IDC_RADIO3, OnRadioColors)
	ON_BN_CLICKED(IDC_RADIO4, OnRadioColors)
	ON_BN_CLICKED(IDC_RADIO5, OnRadioColors)
	ON_NOTIFY(NSCS_SELECTEDITEM, IDC_Factor_CHART, OnChartSelectedItem)
	ON_LBN_SELCHANGE(IDC_LIST_1, &CDlg_TravelTimeReliability::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CDlg_TravelTimeReliability::OnLbnSelchangeList2)
	ON_BN_CLICKED(ID_EXPORT_DATA, &CDlg_TravelTimeReliability::OnBnClickedExportData)
	ON_BN_CLICKED(IDC_MODIFY, &CDlg_TravelTimeReliability::OnBnClickedModify)
	ON_BN_CLICKED(ID_EDIT_SCENARIO, &CDlg_TravelTimeReliability::OnBnClickedEditScenario)
END_MESSAGE_MAP()


// CDlg_TravelTimeReliability message handlers

BOOL CDlg_TravelTimeReliability::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString DTASettingsPath = m_pDoc->m_ProjectDirectory+"DTASettings.txt";

	proportion[0] = g_GetPrivateProfileDouble("TravelTimeReliabilityDemo", "Normal", 0.6f, DTASettingsPath);
	proportion[1] = g_GetPrivateProfileDouble("TravelTimeReliabilityDemo", "Incident", 0.15f, DTASettingsPath);
	proportion[2] = g_GetPrivateProfileDouble("TravelTimeReliabilityDemo", "Workzone", 0.15f, DTASettingsPath);
	proportion[3] = g_GetPrivateProfileDouble("TravelTimeReliabilityDemo", "Weather", 0.1f, DTASettingsPath);

	CalculationMethod = g_GetPrivateProfileInt("TravelTimeReliabilityDemo", "CalculationMethod", 0, DTASettingsPath);

	UpdateCapacityAndDelay();

	m_FactorLabel[0]= "Inadequate Base Cap";
	m_FactorLabel[1]= "Traffic Incidents";
	m_FactorLabel[2]= "Work Zones";
	m_FactorLabel[3]= "Severe Weather";


	m_FactorSize= 4;

//	m_7FactorMOEList.AddString ("Reliability Impact: 1.65*STD of delay");

	m_7FactorMOEList.AddString ("90th Percentile Travel Time (min)");
	m_7FactorMOEList.AddString ("80th Percentile Travel Time (min)");
	//m_7FactorMOEList.AddString ("Planning Time Index: 95th percentile/FFTT"); 
	//m_7FactorMOEList.AddString ("Buffer Index: (95th percentile - mean)/mean"); //(95th percentile travel time – mean travel time) / mean travel time
	//m_7FactorMOEList.AddString ("Skew Index: (90th %ile-median)/(median-10th %ile)"); // (90th percentile travel time – median travel time) / (median travel time – 10th percentile travel time)

	m_7FactorMOEList.AddString ("Capacity Impact" );
	m_7FactorMOEList.AddString ("Occurrence Probability" );
	m_7FactorMOEList.SetCurSel(0);

	m_chart_7factors.SubclassDlgItem(IDC_Factor_CHART,this);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_chart_7factors.PrepareColors(SimpleColors);

	Display7FactorChart();


	m_chart_7factors.SetChartStyle(NSCS_BAR);

	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
	CheckRadioButton(IDC_RADIO3,IDC_RADIO5,IDC_RADIO4);

	//*************************************************************//
	// setup travel time chart
	m_chart_traveltime.SubclassDlgItem(IDC_TravelTime_CHART,this);
	m_chart_traveltime.PrepareColors(GrayScale);


	m_ReliabilityMOEList.AddString ("Travel Time Histogram");
	m_ReliabilityMOEList.AddString ("Cumulative Distribution Function");
	m_ReliabilityMOEList.SetCurSel (0);

	DisplayTravelTimeChart();

	m_chart_traveltime.SetChartStyle(NSCS_BAR);
//	m_chart_traveltime.SetChartStyle(NSCS_LINE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlg_TravelTimeReliability::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlg_TravelTimeReliability::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDlg_TravelTimeReliability::OnRadioButtons() 
{
	int state = GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2);

	if(state == IDC_RADIO1)
	{
		m_chart_7factors.SetChartStyle(NSCS_PIE);
	}
	else
	{
		m_chart_7factors.SetChartStyle(NSCS_BAR);
	}		
}

void CDlg_TravelTimeReliability::OnRadioColors() 
{
	int state = GetCheckedRadioButton(IDC_RADIO3,IDC_RADIO5);

	switch(state){
		case IDC_RADIO3:
			m_chart_7factors.PrepareColors(GrayScale);
			break;
		case IDC_RADIO4:
			m_chart_7factors.PrepareColors(SimpleColors);
			break;
		case IDC_RADIO5:
			m_chart_7factors.ResetColors();
			m_chart_7factors.AddBrush(new CBrush(HS_BDIAGONAL,0x0));
			m_chart_7factors.AddBrush(new CBrush(HS_HORIZONTAL,0x0));
			m_chart_7factors.AddBrush(new CBrush(HS_VERTICAL,0x0));
			m_chart_7factors.AddBrush(new CBrush(HS_CROSS,0x0));
			m_chart_7factors.AddBrush(new CBrush(HS_DIAGCROSS,0x0));
			m_chart_7factors.AddBrush(new CBrush(HS_FDIAGONAL,0x0));
			break;
	}
	m_chart_7factors.Invalidate(FALSE);
}


void CDlg_TravelTimeReliability::OnLbnSelchangeList1()
{
	DisplayTravelTimeChart();


}

void CDlg_TravelTimeReliability::OnLbnSelchangeList2()
{
	Display7FactorChart();
}

void CDlg_TravelTimeReliability::OnBnClickedExportData()
{
	CFileDialog dlg (FALSE, "*.csv", "*.csv",OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_LONGNAMES,
		"Data File (*.csv)|*.csv||", NULL);
	if(dlg.DoModal() == IDOK)
	{
		ExportData(dlg.GetPathName ());
	}
	m_pDoc->OpenCSVFileInExcel(dlg.GetPathName ());
}

void CDlg_TravelTimeReliability::ExportData(CString fname)
{
	FILE* st;
	fopen_s(&st,fname,"w");

	if(st!=NULL)
	{
		CWaitCursor wc;
		fprintf(st,"Category,count\n");
		int count=0;
		CString travel_time_str;


		for(int t=10; t<30; t+=5)
		{
			travel_time_str.Format("%d",t);
			count += 30*g_GetRandomRatio();

			fprintf(st,"%s,%d",travel_time_str,count);

		}


		// to do, 7 factors 

		fclose(st);
	}else
	{
		AfxMessageBox("The selected data file cannot be opened.");
	}
}

void CDlg_TravelTimeReliability::OnBnClickedModify()
{
	UpdateData();

	if (m_dValue >= 1.0f)
	{
		MessageBox("Value cannot be greater than 1.0!","Error",MB_OK | MB_ICONSTOP);
		return;
	}
	int idx=0;
	for (int i=0;i<4;i++)
	{
		if (m_FactorLabel[i].Compare(this->m_sLabel)==0) 
		{
			idx = i;
			break;
		}
	}

	proportion[idx] = m_dValue;

	float sum = 0.0f;
	for (int i=0;i<4;i++)
	{
		if (idx == i) continue;
		sum += proportion[i];
	}

	float rest_proportion = 1.0f - proportion[idx];

	for (int i=0;i<4;i++)
	{
		if (idx == i) continue;
		proportion[i] = rest_proportion * (proportion[i] / sum);
	}

	UpdateCapacityAndDelay();

	Invalidate();
	Display7FactorChart();
	DisplayTravelTimeChart();


}
void CDlg_TravelTimeReliability::OnChartSelectedItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCHARTCTRL nmchart = (LPNMCHARTCTRL)pNMHDR;

	m_iItem = nmchart->iItem;

	if(m_iItem >= 0)
	{
		m_dValue = proportion[m_iItem];
		m_sLabel = nmchart->sLabel;

	}
	UpdateData(FALSE);
	UpdateDialogControls(this,FALSE);
	*pResult = FALSE;
}

void CDlg_TravelTimeReliability::OnBnClickedEditScenario()
{
	m_pDoc->OnScenarioConfiguration();
}

void GenerateXAndY(float* x_ptr, int num, vector<CString>& X, vector<float>& Y, int& step_size)
{
	float max=-1.0f;
	float min= 999999.0f;
	float* ptr = x_ptr;
	for (int i=0;i<num;i++)
	{
		if (*ptr > max) max = *ptr;
		if (*ptr < min) min = *ptr;
		ptr++;
	}

	int lowerbound = (min < (int)(min))?(int)min - 1:(int)min;
	int upperbound = (max > (int)(max))?(int)max + 1:(int)max;

	int diff = upperbound - lowerbound;
	if (diff < 10)
	{
		step_size = 1;
	}
	else
	{
		if (diff >= 10 && diff < 50)
		{
			step_size = 5;
		}
		else
		{
			step_size = 10;
		}
	}

	//if (upperbound < max) upperbound += step_size;

	for (int i=lowerbound;i<upperbound;i+=step_size)
	{
		CString str;
		str.Format("%d",i);
		X.push_back(str);
		Y.push_back(0.0f);
	}

	ptr = x_ptr;
	for (int i=0;i<num;i++)
	{
		float tmp = *(ptr+i);
		int idx = (int)(tmp-lowerbound)/step_size;
		if ((unsigned int)idx < Y.size())
		{
			Y[idx]++;
		}
		else
		{
			Y[Y.size()-1]++;
		}
	}


}

void CDlg_TravelTimeReliability::DisplayTravelTimeChart()
{
	int CurSelectionNo = m_ReliabilityMOEList.GetCurSel ();

	if(CurSelectionNo == 0)  //PDF
	{
		m_chart_traveltime.m_Caption  = "Travel Time Histogram";
		m_chart_traveltime.m_bShowPercentage = false;
		m_chart_traveltime.m_bIntegerValue = true;
		m_chart_traveltime.SetChartStyle(NSCS_BAR);
	}
	else  // CDF
	{
		m_chart_traveltime.m_Caption  = "Cumulative Distribution Function";
		m_chart_traveltime.m_bShowPercentage = false;
		m_chart_traveltime.m_bIntegerValue = false;
		m_chart_traveltime.SetChartStyle(NSCS_LINE);
	}

	CString travel_time_str;


	vector<CString> XLabel;
	vector<float> YLabel;
	int step_size = 5;
	GenerateXAndY(TravelTime,MAX_SAMPLE_SIZE,XLabel,YLabel,step_size);

	m_chart_traveltime.ResetChart();		


	if(CurSelectionNo == 0)
	{
		for (size_t i=0;i<XLabel.size();i++)
		{
			m_chart_traveltime.AddValue(YLabel[i],XLabel[i]);
		}

	}
	else
	{
		float total_count = 0.0f;
		for(size_t i=0;i<YLabel.size();i++)
		{
			total_count+=YLabel[i];
		}

		float count = 0.0f;
		for (size_t i=0;i<XLabel.size();i++)
		{
			count += YLabel[i];
			m_chart_traveltime.AddValue(count/total_count*100.0f,XLabel[i]);
		}
	}
	Invalidate();	

}

float GetMean(float* p,int num)
{
	float sum = 0.0f;
	float* ptr = p;

	for (int i=0;i<num;i++)
	{
		sum += *ptr++;
	}

	return sum/num;
}
float GetSTD(float* p, int num,float mean)
{
	float sum=0.0f;
	float* ptr = p;

	for (int i=0;i<num;i++)
	{
		sum += pow((mean-*ptr),2);
	}

	return sqrt(sum/(num-1));
}
void CDlg_TravelTimeReliability::Display7FactorChart()
{
	RELIABILITY_Y_CLASSIFICATION CurSelectionNo = (RELIABILITY_Y_CLASSIFICATION)m_7FactorMOEList.GetCurSel ();

	float value =0;

	m_chart_7factors.ResetChart();		

	if(CurSelectionNo == 0)
	{
	m_chart_7factors.m_Caption = "Sources of Unreliable Travel Times";
	}

	if(CurSelectionNo == 1)
	{
	m_chart_7factors.m_Caption = "Sources of Unreliable Travel Times";
	}

	if(CurSelectionNo == 2)
	{
	m_chart_7factors.m_Caption = "Capacity Distribution";
	}

	if(CurSelectionNo == 3)
	{
	m_chart_7factors.m_Caption = "Occurrence Probability of Impact Factors";
	}

	switch(CurSelectionNo)
	{
	case 0:
		for(int i =0; i< m_FactorSize; i++)
		{
			int pos = 0;
			for (int j=0;j<i;j++)
			{
				pos += IntProportion[j];
			}

			value = 1.65*GetSTD(TravelTime+pos,IntProportion[i],this->m_PathFreeFlowTravelTime);
			m_chart_7factors.AddValue(value,m_FactorLabel[i]);
		}
		break;
	case 1:
		for(int i =0; i< m_FactorSize; i++)
		{
			int pos = 0;
			for (int j=0;j<i;j++)
			{
				pos += IntProportion[j];
			}

			value = 1.65*GetSTD(AdditionalDelay+pos,IntProportion[i],GetMean(AdditionalDelay,MAX_SAMPLE_SIZE));
			m_chart_7factors.AddValue(value,m_FactorLabel[i]);
		}
		break;
	case 2:
		for(int i =0; i< m_FactorSize; i++)
		{
			int pos = 0;
			for (int j=0;j<i;j++)
			{
				pos += IntProportion[j];
			}

			value = GetMean(Capacity+pos,IntProportion[i]);
			m_chart_7factors.AddValue(value,m_FactorLabel[i]);
		}
		break;
	case 3:
		for(int i =0; i< m_FactorSize; i++)
		{
			m_chart_7factors.AddValue(proportion[i],m_FactorLabel[i]);
		}
		break;
	}

	Invalidate();	
}
