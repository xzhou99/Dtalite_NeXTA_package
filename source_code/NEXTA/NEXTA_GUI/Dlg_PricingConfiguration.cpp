// Dlg_TDDemandProfile.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_PricingConfiguration.h"


// CDlg_PricingConfiguration dialog

CDlg_PricingConfiguration::CDlg_PricingConfiguration(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PricingConfiguration::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_PricingConfiguration)
	m_dValue = 0.0;
	m_sLabel = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_iItem = -1;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlg_PricingConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_PricingConfiguration)
	DDX_Text(pDX, IDC_VALUE, m_dValue);
	DDX_Text(pDX, IDC_LABEL, m_sLabel);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DEMAND_TYPE_LIST, m_DemandTypeList);
}

BEGIN_MESSAGE_MAP(CDlg_PricingConfiguration, CDialog)
	//{{AFX_MSG_MAP(CDlg_PricingConfiguration)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, OnRadioButtons)
	ON_BN_CLICKED(IDC_RADIO3, OnRadioColors)
	ON_BN_CLICKED(IDC_CUSTOM1, OnAdd)
	ON_NOTIFY(NSCS_SELECTEDITEM, IDC_CUSTOM1, OnChartSelectedItem)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioButtons)
	ON_BN_CLICKED(IDC_RADIO4, OnRadioColors)
	ON_BN_CLICKED(IDC_RADIO5, OnRadioColors)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_MODIFY,OnUpdateModify)
	ON_UPDATE_COMMAND_UI(IDC_DEL,OnUpdateModify)
	ON_UPDATE_COMMAND_UI(IDC_ADD,OnUpdateAdd)
	ON_LBN_SELCHANGE(IDC_DEMAND_TYPE_LIST, &CDlg_PricingConfiguration::OnLbnSelchangeDemandProfileList)
	ON_BN_CLICKED(IDOK, &CDlg_PricingConfiguration::OnBnClickedOk)
	ON_BN_CLICKED(ID_VOT_Distribution, &CDlg_PricingConfiguration::OnBnClickedVotDistribution)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_PricingConfiguration message handlers

BOOL CDlg_PricingConfiguration::OnInitDialog()
{

	CDialog::OnInitDialog();

	//Subclassing

	m_chart.SubclassDlgItem(IDC_CUSTOM1,this);

	// Add "About..." menu item to system menu.

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_chart.PrepareColors(SimpleColors);

	// processing demand data 

//		fprintf(st,"from_zone_id,to_zone_id,demand_type,time_series_name,");

		int CurDemandType = 0;
		m_StartTimeInterval = 6;  // 6AM
		m_EndTimeInterval = 20; // 10PM

		m_DemandTypeList.AddString("SOV");
		m_DemandTypeList.AddString("HOV");
		m_DemandTypeList.AddString("Truck");


			for(int t = m_StartTimeInterval; t< m_EndTimeInterval; t++)
			{
				CString time_stamp_str = m_pDoc->GetTimeStampStrFromIntervalNo (t*4,false);
				double toll_value = 1;

				if(t >= 8 && t <= 10) // between 8AM and 10AM
					toll_value = 2;

				m_chart.AddValue(toll_value,time_stamp_str);
			}

	
	m_chart.SetChartStyle(NSCS_BAR);

	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
	CheckRadioButton(IDC_RADIO3,IDC_RADIO5,IDC_RADIO4);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlg_PricingConfiguration::OnSysCommand(UINT nID, LPARAM lParam)
{
		CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlg_PricingConfiguration::OnPaint() 
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
HCURSOR CDlg_PricingConfiguration::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDlg_PricingConfiguration::OnRadioButtons() 
{
	int state = GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2);
	
	if(state == IDC_RADIO1)
	{
		m_chart.SetChartStyle(NSCS_PIE);
	}
	else
	{
		m_chart.SetChartStyle(NSCS_BAR);
	}		
}

void CDlg_PricingConfiguration::OnRadioColors() 
{
	int state = GetCheckedRadioButton(IDC_RADIO3,IDC_RADIO5);
	
	switch(state){
		case IDC_RADIO3:
			m_chart.PrepareColors(GrayScale);
			break;
		case IDC_RADIO4:
			m_chart.PrepareColors(SimpleColors);
			break;
		case IDC_RADIO5:
			m_chart.ResetColors();
			m_chart.AddBrush(new CBrush(HS_BDIAGONAL,0x0));
			m_chart.AddBrush(new CBrush(HS_HORIZONTAL,0x0));
			m_chart.AddBrush(new CBrush(HS_VERTICAL,0x0));
			m_chart.AddBrush(new CBrush(HS_CROSS,0x0));
			m_chart.AddBrush(new CBrush(HS_DIAGCROSS,0x0));
			m_chart.AddBrush(new CBrush(HS_FDIAGONAL,0x0));
			break;
	}
	m_chart.Invalidate(FALSE);
}



void CDlg_PricingConfiguration::OnAdd() 
{
	if(m_iItem < 0)
	{
		UpdateData();
		if(m_dValue > 0.0 && !m_sLabel.IsEmpty())
		{
			m_chart.AddValue(m_dValue,m_sLabel,TRUE);
			m_iItem = -1;
			m_dValue = 0.0;
			m_sLabel = "";
			UpdateData(FALSE);
		}
	}
	UpdateDialogControls(this,FALSE);
}

void CDlg_PricingConfiguration::OnChartSelectedItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCHARTCTRL nmchart = (LPNMCHARTCTRL)pNMHDR;
	
	m_iItem = nmchart->iItem;

	if(m_iItem >= 0)
	{
		m_dValue = (float)(nmchart->dValue);  // convert to float to avoid many decimals
		m_sLabel = nmchart->sLabel;
	}else
	{
		m_dValue = 0;
		m_sLabel = "";
	}
	UpdateData(FALSE);
	UpdateDialogControls(this,FALSE);
	*pResult = FALSE;
}


void CDlg_PricingConfiguration::OnDel() 
{
	if(m_chart.DeleteItem(m_iItem))
	{
		m_dValue = 0;
		m_sLabel = "";
		m_iItem  = -1;
		UpdateData(FALSE);
		UpdateDialogControls(this,FALSE);
	}
}

void CDlg_PricingConfiguration::OnModify() 
{
	UpdateData();
	if(m_dValue > 0.0 && !m_sLabel.IsEmpty())
	{
		m_chart.ModifyItem(m_iItem,m_dValue,m_sLabel);
	}
}
void CDlg_PricingConfiguration::OnUpdateModify(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_iItem>=0);
}


void CDlg_PricingConfiguration::OnUpdateAdd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_iItem < 0);
}


// CDlg_PricingConfiguration message handlers

void CDlg_PricingConfiguration::OnLbnSelchangeDemandProfileList()
{
		int CurDemandType = m_DemandTypeList.GetCurSel ();

		m_chart.ResetChart();

			for(int t = m_StartTimeInterval; t< m_EndTimeInterval; t++)
			{
				CString time_stamp_str = m_pDoc->GetTimeStampStrFromIntervalNo (t*4,false);
				double toll_value = 1;

				if(t >= 8 && t <= 10) // between 8AM and 10AM
					toll_value = 2;

				m_chart.AddValue(toll_value,time_stamp_str);
			}

		Invalidate();	


}

void CDlg_PricingConfiguration::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlg_PricingConfiguration::OnBnClickedVotDistribution()
{
	// TODO: Add your control notification handler code here
}
