//Dlg_KML_Configuration.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "TLiteDoc.h"
#include "TLiteView.h"

#include "Dlg_KML_Configuration_Zone.h"


// CDlg_KML_Configuration_Zone dialog


IMPLEMENT_DYNAMIC(CDlg_KML_Configuration_Zone, CDialog)

CDlg_KML_Configuration_Zone::CDlg_KML_Configuration_Zone(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_KML_Configuration_Zone::IDD, pParent)

	, m_Transparency(0)
	, m_MaxValue(0)
{
	m_Zone_Height_Ratio = 1;
	m_Transparency = 50;

	m_Zone_Transparency = 50;

	for(int i = 0; i <=7; i++)
	{
	m_ColorCategoryValue[i] = i*1000;
	}
}

CDlg_KML_Configuration_Zone::~CDlg_KML_Configuration_Zone()
{
}

void CDlg_KML_Configuration_Zone::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Zone_Height, m_Zone_Height_Combo);
	DDX_Text(pDX, IDC_EDIT_L1, m_ColorCategoryValue[1]);
	DDX_Text(pDX, IDC_EDIT_L2, m_ColorCategoryValue[2]);
	DDX_Text(pDX, IDC_EDIT_L3, m_ColorCategoryValue[3]);
	DDX_Text(pDX, IDC_EDIT_L4, m_ColorCategoryValue[4]);
	DDX_Text(pDX, IDC_EDIT_L5, m_ColorCategoryValue[5]);
	DDX_Text(pDX, IDC_EDIT_L6, m_ColorCategoryValue[6]);
	DDX_Text(pDX, IDC_EDIT_L7, m_ColorCategoryValue[7]);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_BITMAP, m_ColorRampStatic);


	DDX_Text(pDX, IDC_EDIT_HEIGHT_ZONE, m_Zone_Height_Ratio);

	DDV_MinMaxFloat(pDX, m_Zone_Height_Ratio, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_Transparency_Zone, m_Zone_Transparency);
	DDV_MinMaxInt(pDX, m_Zone_Transparency, 0, 100);
	DDX_Text(pDX, IDC_EDIT1, m_MaxValue);
}


BEGIN_MESSAGE_MAP(CDlg_KML_Configuration_Zone, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_KML_Configuration_Zone::OnBnClickedOk)
	ON_BN_CLICKED(IDOK3, &CDlg_KML_Configuration_Zone::OnBnClickedOk3)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlg_KML_Configuration_Zone::OnLbnSelchangeList1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &CDlg_KML_Configuration_Zone::OnBnClickedButtonReverse)
	ON_CBN_SELCHANGE(IDC_COMBO_Zone_Height, &CDlg_KML_Configuration_Zone::OnCbnSelchangeComboZoneHeight)
END_MESSAGE_MAP()


// CDlg_KML_Configuration_Zone message handlers

void CDlg_KML_Configuration_Zone::OnBnClickedButtonSpaceevenly()
{
	// TODO: Add your control notification handler code here
}

void CDlg_KML_Configuration_Zone::OnBnClickedOk()
{
	UpdateData(1);
	SelchangeComboHeight();
	m_Zone_Height_Mode  = m_Zone_Height_Combo.GetCurSel();
	m_Zone_Color_Mode = m_List.GetCurSel();
	OnOK();
}
void CDlg_KML_Configuration_Zone::UpdateCategoryValues()
{
	m_Zone_Color_Mode  = (eKML_ZoneColorMode) m_List.GetCurSel ();

	std::list<DTALink*>::iterator iLink;
	float max_link_volume = 0;

	float max_value = 0;
	float min_value = 999999;


		CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_L1);
		CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_L2);
		CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT_L3);
		CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT_L4);
		CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_L5);
		CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT_L6);
		CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT_L7);

	if(m_Zone_Color_Mode == e_KML_zone_green ||
		m_Zone_Color_Mode == e_KML_zone_yellow ||
		m_Zone_Color_Mode == e_KML_zone_red)
	{

		for(int i = 1; i<=7; i++)
		{
			m_ColorCategoryValue[i] = 0;
		}


		pEdit1->EnableWindow (0);
		pEdit2->EnableWindow (0);
		pEdit3->EnableWindow (0);
		pEdit4->EnableWindow (0);
		pEdit5->EnableWindow (0);
		pEdit6->EnableWindow (0);
		pEdit7->EnableWindow (0);



	}else
	{
		pEdit1->EnableWindow (1);
		pEdit2->EnableWindow (1);
		pEdit3->EnableWindow (1);
		pEdit4->EnableWindow (1);
		pEdit5->EnableWindow (1);
		pEdit6->EnableWindow (1);
		pEdit7->EnableWindow (1);

		std::map<int, DTAZone>	:: iterator itr_o;
		for(itr_o = m_pDoc->m_ZoneMap.begin(); itr_o != m_pDoc->m_ZoneMap.end(); itr_o++)
		{
		
			DTAZone* pZone = &((*itr_o).second);
		

		pZone->KML_color_value  = 0;
		switch (m_Zone_Color_Mode)
		{

		case e_KML_zone_total_vehicles_origin: pZone->KML_color_value = pZone->m_OriginTotalNumberOfVehicles ; break;
		case e_KML_zone_total_vehicles_destination: pZone->KML_color_value = pZone->m_DestinationTotalNumberOfVehicles ; break;

		case e_KML_zone_total_distance_origin: pZone->KML_color_value = pZone->m_OriginTotalTravelDistance ; break;
		case e_KML_zone_total_distance_time_destination: pZone->KML_color_value = pZone->m_DestinationTotalTravelDistance ; break;

		case e_KML_zone_total_travel_time_origin: pZone->KML_color_value = pZone->m_OriginTotalTravelTime ; break;
		case e_KML_zone_total_travel_time_destination: pZone->KML_color_value = pZone->m_DestinationTotalTravelTime ; break;

		case e_KML_zone_avg_distance_origin: pZone->KML_color_value = pZone->GetOriginAvgTravelDistance() ; break;
		case e_KML_zone_avg_distance_destination: pZone->KML_color_value = pZone->GetDestinationAvgTravelDistance() ; break;

		case e_KML_zone_avg_travel_time_origin: pZone->KML_color_value = pZone->GetOriginAvgTravelDistance() ; break;
		case e_KML_zone_avg_travel_time_destination: pZone->KML_color_value = pZone->GetDestinationAvgTravelDistance() ; break;


		case e_KML_zone_avg_speed_origin: pZone->KML_color_value = pZone->GetOriginAvgSpeed() ; break;
		case e_KML_zone_avg_speed_destination: pZone->KML_color_value = pZone->GetDestinationAvgSpeed() ; break;


		}

		max_value = max(max_value, pZone->KML_color_value );
		min_value = min(min_value, pZone->KML_color_value );

	}
	min_value = 1;

	m_ColorCategoryValue[1] = min_value;
	m_ColorCategoryValue[7] = max_value;

	float incremental = (max_value - min_value)/ 6;

	for(int i = 2; i<=6; i++)
	{
		m_ColorCategoryValue[i] = m_ColorCategoryValue[i-1] + incremental;
	}

	}
	UpdateData(0);
}

BOOL CDlg_KML_Configuration_Zone::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Zone_Height_Combo.AddString("Default Height of 1");
	m_Zone_Height_Combo.AddString("Total Production at Origin");
	m_Zone_Height_Combo.AddString("Total Attraction at Destination");
	m_Zone_Height_Combo.SetCurSel(1);

	m_List.AddString ("Red");
	m_List.AddString ("Yellow");
	m_List.AddString ("Green");

	m_List.AddString ("Total number of vehicles (origin)");
	m_List.AddString ("Total number of vehicles (destination)");

	m_List.AddString ("Total traveled distance (origin)");
	m_List.AddString ("Total traveled distance   (destination)");

	m_List.AddString ("Total travel time (min) (origin)");
	m_List.AddString ("Total travel time (min) (destination)");

	m_List.AddString ("Avg distance  (origin)");
	m_List.AddString ("Avg distance  (destination)");

	m_List.AddString ("Avg travel time (min) (origin)");
	m_List.AddString ("Avg travel time (min)(destination)");

	m_List.AddString ("Avg speed (origin)");
	m_List.AddString ("Avg speed (destination)");


	m_List.SetCurSel (0);

	UpdateCategoryValues();

	OnCbnSelchangeComboZoneHeight();
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_KML_Configuration_Zone::OnPaint()
{

   CPaintDC dc(this); 
   CPen penLOS[MAX_LOS_SIZE];
   CBrush brushLOS[MAX_LOS_SIZE];

   int i;

   if(m_pDoc==NULL)
	   return;
   for(i = 1; i<MAX_LOS_SIZE-1; i++)
   {
      penLOS[i].CreatePen(PS_SOLID, 1, m_pDoc->m_colorLOS[i]);
      brushLOS[i].CreateSolidBrush(m_pDoc->m_colorLOS[i]);
   }

   dc.SetBkMode(TRANSPARENT);

    CRect entire_rect;

	m_ColorRampStatic.GetWindowRect(&entire_rect);
	ScreenToClient(&entire_rect); 

   
   CRect lr;

   float height = entire_rect.Height () / 6.0;

	  for(i = 1; i< MAX_LOS_SIZE-1; i++)
   {
      lr.top = entire_rect.top +height*(i-1) ;
      lr.bottom = lr.top + height;

      lr.left = entire_rect.left;
      lr.right = entire_rect.right; 

	  if(m_pDoc->m_ColorDirection==-1)  // reverse direction 
	  {
	  dc.SelectObject(penLOS[MAX_LOS_SIZE-1-i]);
      dc.SelectObject(brushLOS[MAX_LOS_SIZE-1-i]);
	  }else
	  {
	  dc.SelectObject(penLOS[i]);
      dc.SelectObject(brushLOS[i]);
  	  }

	  CString lengend_interval_str;
      dc.Rectangle(lr);
	}

}

void CDlg_KML_Configuration_Zone::OnBnClickedButtonReverse()
{
	if(m_pDoc->m_ColorDirection == 1)
		m_pDoc->m_ColorDirection = -1;
	else
		m_pDoc->m_ColorDirection = 1;

	Invalidate();
}

void CDlg_KML_Configuration_Zone::SelchangeComboHeight()
{
}

void CDlg_KML_Configuration_Zone::OnLbnSelchangeList1()
{
	UpdateCategoryValues();
}

void CDlg_KML_Configuration_Zone::OnBnClickedOk3()
{
	UpdateData(1);
	m_Zone_Height_Mode  = m_Zone_Height_Combo.GetCurSel();
	m_Zone_Color_Mode = m_List.GetCurSel();

	CString Zone3DFile = m_pDoc->m_ProjectDirectory+"AMS_zone.kml";
	DeleteFile(Zone3DFile);
	m_pDoc->ExportZoneLayerToKMLFiles(Zone3DFile,"LIBKML", m_Zone_Height_Mode, m_Zone_Height_Ratio, m_Zone_Color_Mode, m_Zone_Transparency,m_ColorCategoryValue );
	HINSTANCE result = ShellExecute(NULL, _T("open"), Zone3DFile, NULL,NULL, SW_SHOW);


}
void CDlg_KML_Configuration_Zone::OnCbnSelchangeComboZoneHeight()
{
	float max_value  =0 ;
		m_Zone_Height_Mode  = m_Zone_Height_Combo.GetCurSel();
		
		std::map<int, DTAZone>	:: iterator itr_o;
		for(itr_o = m_pDoc->m_ZoneMap.begin(); itr_o != m_pDoc->m_ZoneMap.end(); itr_o++)
		{
			float height = 0;
			switch(m_Zone_Height_Mode)
			{
			case 0: 
				height = 1;
			break;

			case 1:
				height = itr_o->second.m_OriginTotalNumberOfVehicles;
			break;

			case 2:
				height = itr_o->second.m_DestinationTotalNumberOfVehicles;
			break;
			
			}

			if(max_value < height)
			max_value = height;
		}

		m_MaxValue = max_value;

		m_Zone_Height_Ratio = 100 / max(1, m_MaxValue);
		UpdateData(0);


}
